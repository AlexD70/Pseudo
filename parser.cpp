//#pragma once //for some reason this gives a warning
#include <fstream>
#include <iostream>
#include <vector>
#include "util.h"
#include "pseinternal.h"
#include "parsingflags.h"
#include "pseerrorslib.h"

/*
 PseudocodeParser class
*/
class PseudocodeParser {
    public:
        PseudocodeParser(){}
        std::vector<char> charBuffer = std::vector<char>();
        std::vector<Token> tokenVector = std::vector<Token>();
        struct paranthesesStackNode {
            Parantheses *self = nullptr;
            paranthesesStackNode *next = nullptr;
            paranthesesStackNode *back = nullptr;
        };
        paranthesesStackNode node1 = paranthesesStackNode();
        paranthesesStackNode *actualNode = &node1;
        Subscript *subscriptPtr = nullptr;

        //saves file contents into charBuffer
        void readFile(std::string filename) {
            char ch;
            std::ifstream f(filename);

            for (int i = 0;; i++) {

                ch = f.get();

                if (f.eof()) {
                    break;
                } else {
                    appendToVector(charBuffer, ch);
                }
            }

            f.close();
            return;
        }

        //calls parseFromBuffer(charBuffer)
        void parseFromBuffer() {
            parseFromBuffer(charBuffer);
        }

        //the actual parsing function
        void parseFromBuffer(std::vector<char> buffer) {
            resetFlags();
            std::string tokenString = "";

            // used for stange purposes
            // reuse it whenever you like
            int otherData = 0;

            for (int i = 0; i < buffer.size(); i ++){
                char c = buffer[i];

                //inString path
                if (pflags::stringFlagsImpl.inString) {
                    if (c == '\\'){ //validate escape characters
                        peek(i, buffer);
                        i += 1;

                        if (peekData == '\\') /*backslash escape*/ { 
                            tokenString = tokenString + '\\';
                        
                        } else if (peekData == 'n') /*newline escape*/ {
                            tokenString = tokenString + '\n';
                              
                        } else if (peekData == 't') /*tab escape*/ {
                            tokenString + '\t';

                        } else if (peekData == 'b') /*backspace escape*/ {
                            tokenString = tokenString + '\b';

                        } else if (peekData == '\"') /*double quote escape*/ {
                            tokenString = tokenString + '\"';

                        } else if (peekData == '\'') /*single quote escape*/ {
                            tokenString = tokenString + '\'';

                        } else if (peekData == '\n') /*string break (backslash + newline)*/ {
                            pflags::line += 1;
                            otherData += 1; // remember the line changed so when the token is created
                            // we subtract otherData from line to get the line the quotes were opened

                        } else /*invalid escape*/ {
                            i -= 1;
                            InvalidEscape err = InvalidEscape(pflags::line, std::string() + c + peekData);
                            pseutils::raise(err);
                        }

                    } else if ((c == '\n') || (iscntrl(c)) || !(isascii(c))){ // validate character
                        //unescaped newlines, ctrl and non-ascii characters will raise an error
                        InvalidString err = InvalidString(pflags::line);
                        pseutils::raise(err);

                    } else if (c == pflags::stringFlagsImpl.stringType) { //check for string end
                        pflags::stringFlagsImpl.inString = false;
                        pflags::scopeChangeContext = false;
                        appendToParanthesesStack(Literal<dtypes::String, std::string>(dtypes::String(tokenString), pflags::line - otherData));
                        otherData = 0; //reset this for later use
                        pflags::scopeDepth = 0;

                    } else { //default
                        tokenString = tokenString + c;
                    }
                }

                if ((c == '\t') || (c == ' ') || (c == '\n')){ 
                    //seek until buffer[i] is not space, newline or tab
                    //this also takes care of flags
                    seek(&i, buffer);
                    continue;
                }
                
                if (pflags::scopeChangeContext){ //performs this at the beginning of every line that has at least one token
                    //or at least this is the intended behavior
                    appendToParanthesesStack(Indentation(pflags::line, pflags::scopeDepth));
                    pflags::scopeChangeContext = false;
                    pflags::scopeDepth = 0;
                }

                //TODO IMPORTANT: problem w scopeChangeContext flag for multi-line comments
                if (c == '/'){ // division and comments
                    if (buffer[i + 1] == '/'){ // single-line comment
                        i += 1;
                        do{
                            i += 1;
                        } while ((i < buffer.size()) && (buffer[i] != '\n'));
                        pflags::line += 1;
                        pflags::scopeChangeContext = true;
                    } else if (buffer[i + 1] == '*'){ // multi-line comment
                        i += 1;
                        do{
                            i += 1;
                            if (buffer[i] == '\n'){
                                pflags::line += 1;
                            }
                        } while((i < buffer.size()) && (buffer[i] != '*') && (buffer[i + 1] != '/'));
                        if (i >= buffer.size()){ 
                            // multi-line comment doesnt end, but it reaches EOF so it raises an error
                            UnexpectedEOF err = UnexpectedEOF();
                            pseutils::raise(err);
                        } else {
                            pflags::scopeChangeContext = true;
                        }
                    } else { // division
                        char arr[2] = {c, '\0'}; // builds a c-style null-terminated string
                        appendToParanthesesStack(Token(pflags::line, arr, 1));
                        continue;
                    }
                }

                if ((c == '+') || (c == '*') || (c == '%') || (c == '-')){ //addition, multiplication, modulus, subtraction
                    char arr[2] = {c, '\0'}; // builds a c-style null-terminated string
                    appendToParanthesesStack(Token(pflags::line, arr, 1));
                    continue;
                }

                if (c  == '<'){ // LT, LTEQ, attribution
                    if (buffer[i + 1] == '-'){ // atttribution
                        i += 1;
                        char arr[3] = {'<', '-', '\0'};
                        appendToParanthesesStack(Token(pflags::line, arr, 3));

                    } else if (buffer[i + 1] == '=') {  // LTEQ
                        i += 1;
                        char arr[3] = {'<', '=', '\0'};
                        appendToParanthesesStack(Token(pflags::line, arr, 4));

                    } else { // LT
                        char arr[2] = {'<', '\0'};
                        appendToParanthesesStack(Token(pflags::line, arr, 4));
                    }
                    continue;
                }
            
                if (c == '>'){ // GT, GTEQ
                    if (buffer[i + 1] == '='){  // GTEQ
                        char arr[3] = {'>', '=', '\0'};
                        appendToParanthesesStack(Token(pflags::line, arr, 4));

                    } else {  // GT
                        char arr[2] = {'>', '\0'};
                        appendToParanthesesStack(Token(pflags::line, arr, 4));
                    }
                    continue;
                }

                if ((c == 'N') && (buffer[i + 1] == 'U')){ // NOT (NU), NOTEQ (NU=)
                    i += 2;
                    if (buffer[i + 1] == '='){ // NOTEQ (NU=)
                        char arr[4] = {'N', 'U', '=', '\0'};
                        appendToParanthesesStack(Token(pflags::line, arr, 4));
                        continue;
                    } else if (buffer[i + 1] == ' '){ // NOT
                        //checks for ' ' cuz if smth is appended to NU directly it should be an indentifier
                        //rather than a logical operator
                        //this is subject to change tho
                        char arr[3] = {'N', 'U'};
                        appendToParanthesesStack(Token(pflags::line, arr, 2));
                        continue;
                    } else {
                        i -= 2;
                    }
                }
            
                if ((c == 'S') && (buffer[i + 1] == 'I') && (buffer[i + 2] == ' ')){ // AND (SI)
                    // performs the blank space check too (see NOT)
                    i += 2;
                    char arr[3] = {'S', 'I', '\0'};
                    appendToParanthesesStack(Token(pflags::line, arr, 2));
                    continue;
                }
            
                if ((c == 'S') && (buffer[i + 1] == 'A') && 
                    (buffer[i + 2] == 'U') && (buffer[i + 3] == ' ')){ // OR (SAU)
                    // same as AND and NOT
                    i += 3;
                    char arr[4] = {'S', 'A', 'U', '\0'};
                    appendToParanthesesStack(Token(pflags::line, arr, 2));
                    continue;
                }
            
                if (c == '='){ // EQ
                    char arr[2] = {'=', '\0'};
                    appendToParanthesesStack(Token(pflags::line, arr, 2));
                    continue;
                }
            
                if ((c == '\"') || (c == '\'')){ // " or ' -> string start
                    pflags::stringFlagsImpl.inString = true;
                    pflags::stringFlagsImpl.stringType = c;
                    continue;
                }
            
                if (c == '('){
                    //expect this to give SIGSEGV often
                    /*
                    i can almost picture it:
                    Segmentation failed. Core dumped.
                    */
                    pflags::paranthesesDepth += 1;
                    Parantheses newParantheses = Parantheses(pflags::line, pflags::paranthesesDepth);
                    if (pflags::paranthesesDepth == 1){
                        node1.self = &newParantheses;
                        appendToVector<Token>(tokenVector, newParantheses);
                    }

                    //ahem a lot of pointers, im actually using a double-linked list
                    paranthesesStackNode node;
                    paranthesesStackNode *lastNode;
                    node.self = &newParantheses;
                    (*actualNode).next = &node;
                    lastNode = actualNode;
                    actualNode = &node;
                    (*actualNode).back = lastNode;
                }

                if (c == ')'){
                    pflags::paranthesesDepth -= 1;
                    if (pflags::paranthesesDepth == 0){
                        continue;
                    } else if (pflags::paranthesesDepth < 0){
                        BadParanthesesNesting err = BadParanthesesNesting(pflags::line);
                        pseutils::raise(err);
                    }

                    actualNode = (*actualNode).back;
                }

                
                //TODO debug this shit
                //CHARACTERS LEFT: [], f (f-strings, if we decide to implement them + changes in string path)
                //                 default (we shouldnt check for numbers at this level and pass that to the token wrapper)

                //TODO this should be even more modularized, so that we use as little code here as possible
                // id rather have 27822782 small functions rather than a single, but very long, function
            }

            if (pflags::paranthesesDepth > 0){
                UnexpectedEOF err = UnexpectedEOF("UnexpextedEOF: raised while parsing due to opened but not closed parantheses.");
                pseutils::raise(err);
            }
        }

    private:
        char peekData;

        //reset everything in pflags namespace
        void resetFlags(){
            pflags::line = 1;
            pflags::paranthesesDepth = 0;
            pflags::scopeChangeContext = false;
            pflags::scopeDepth = 0;
            pflags::stringFlagsImpl.inString = false;
            pflags::sucessiveSpaces = 0;

            return;
        }

        std::string buildToken(int *i, std::vector<char> buffer){ // this will probably be unused, delete later
            std::string result = "";
            char c;
            do{
                c = buffer[*i];
                result.push_back(c);
                if (c == pflags::stringFlagsImpl.stringType){
                    pflags::stringFlagsImpl.inString = false;
                }
                peek(*i, buffer);
                *i += 1;
            } while ((pflags::stringFlagsImpl.inString) ||
                     ((peekData != ' ') && (peekData != '\t') && (peekData != '\n')));

            return result;
        }

        //appending to parantheses stack if that is the case
        void appendToParanthesesStack(Token what){
            if (pflags::paranthesesDepth > 0){
                (*(*actualNode).self).appendToVector(what);
            } else {
                appendToVector<Token>(tokenVector, what);
            }
        }

        // increments i until buffer[i] is not space, tab or newline
        // also changes flags accordingly (at least thats what i expect)
        void seek(int *i, std::vector<char> buffer) {
            while (true) {
                if (*i >= buffer.size()){
                    return;
                }

                peek(*i, buffer);

                if (peekData == ' ') {
                    *i += 1;
                    pflags::sucessiveSpaces += 1;
                    continue;

                } else if ((peekData == '\t')) {
                    *i += 1;
                    if (pflags::scopeChangeContext){
                        pflags::scopeDepth += 1;
                    }
                    continue;

                } else if ((peekData == '\n')) {
                    *i += 1;
                    pflags::scopeDepth = 0;
                    pflags::line += 1;
                    pflags::sucessiveSpaces = 0;
                    pflags::scopeChangeContext = true;
                    continue;

                } else {
                    pflags::sucessiveSpaces = 0;
                    return;
                }

                if ((pflags::sucessiveSpaces == 4) && (pflags::scopeChangeContext)) {
                    pflags::scopeDepth += 1;
                    pflags::sucessiveSpaces = 0;
                }
            }
        }

        //saves buffer[pos + 1] in field peekData so it can be retrieved many times w/o accessing the
        //vector multiple times
        void peek(int pos, std::vector<char> buffer) {
            peekData = buffer[pos + 1];
        }
};

//use main for testing purposes
//uncomment those params to use cmdline arguments
//note 1: argc is the nr of params
//note 2: you dont hv to pass the nr of params yourself, only the params
//note 3: similar to $0 in bash, argv[0] is the name of the program that was invoked
int main(/*int argc, char *argv[]*/)
{
    dtypes::Integer a = dtypes::Integer(5), b = dtypes::Integer(2);

    std::cout << ((-9) % 2) << '\n';
}
