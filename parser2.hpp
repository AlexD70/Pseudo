#ifndef PSE_INTERNAL_FILEIO
#define PSE_INTERNAL_FILEIO
#include <fstream>
#endif

#ifndef PSE_INTERNAL_VECTOR
#define PSE_INTERNAL_VECTOR
#include <vector>
#endif

#include "token.hpp"

#ifndef PSE_PARSER
#define PSE_PARSER

namespace pflags {
    struct stringFlags
    {
        bool inString = false;
        char stringType;
    };

    stringFlags stringFlagsImpl;

    //used to track whether 4 spaces change scope or not in the actual context
    bool scopeChangeContext = true;
    int paranthesesDepth = 0;
    int scopeDepth = 0;
    int sucessiveSpaces = 0;
    std::size_t line = 1;
    std::size_t col = 0;
    //bool inSubscript = false;
    bool isTokenStringEmpty = true;

    void reset() {
        stringFlagsImpl.inString = false;
        scopeChangeContext = true;
        paranthesesDepth = 0;
        scopeDepth = 0;
        sucessiveSpaces = 0;
        line = 1;
        isTokenStringEmpty = true;
        return;
    }
}

void appendToVector(std::vector<char> & vector, char what){
    vector.reserve(vector.size() + 1);
    vector.push_back(what);
    return;
}

void appendToVector(std::vector<Token> & vector, Token what){
    vector.reserve(vector.size() + 1);
    vector.push_back(Token(what));
}

void readFile(std::string filename, std::vector<char> & charBuffer) {
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

// after getting all useful data from the object, destroy it to save memory
class PseudocodeParser {
    private:
        std::vector<char> charBuffer = std::vector<char>();
        std::vector<Token> tokenVector = std::vector<Token>();
        std::string tokenString = "";

        // i recommend not touching any of these functions cuz it may break parser basic functionality

        bool doExitString(std::vector<char>::const_iterator c) {
            return ((pflags::stringFlagsImpl.inString) && ((* c) == pflags::stringFlagsImpl.stringType) && ((* (c - 1)) != '\\'));
        }
        bool isCharValid(char c) noexcept {
            return (isascii(c) && c != '\n' && c != '\b');
        }
        bool isEscapeValid(char c) noexcept {
            switch (c) {
                case 'n':
                    break;
                case 't':
                    break;
                case 'b':
                    break;
                case '\"':
                    break;
                case '\'':
                    break;
                default:
                    return false;
            }
            return true;
        }
        
        bool isBufferAtEnd(std::size_t index, std::size_t bufferSize) noexcept {
            return (index >= bufferSize);
        }
        
        void newlineFlags() noexcept {
            pflags::col = 0;
            pflags::line += 1;
            pflags::scopeChangeContext = true;
            pflags::scopeDepth = 0;
            pflags::sucessiveSpaces = 0;
        }
        void blankspaceFlags() noexcept {
            pflags::sucessiveSpaces += 1;
            
            if ((pflags::scopeChangeContext) && (pflags::sucessiveSpaces == 4)){
                pflags::scopeDepth += 1;
                pflags::sucessiveSpaces = 0;
            }
        }
        void tabFlags() noexcept {
            pflags::col += 3;
            pflags::scopeDepth += 1;
            pflags::sucessiveSpaces = 0;
        }
        void defaultFlags() noexcept {
            pflags::scopeChangeContext = false;
            pflags::isTokenStringEmpty = false;
            pflags::sucessiveSpaces = 0;
        }

        bool isCharBlank(char c) noexcept {
            return ((c == ' ') || (c == '\n') || (c == '\t'));
        }

        // DONT TOUCH THIS ONE. ITS IMPORTANT EVEN TOUGH IM NOT SURE HOW TO EXPLAIN WHY
        void dubiousLogic_dontTouch() { // not noexcept
            if (!pflags::isTokenStringEmpty) {
                Token defaultToken = Token(tokenString.c_str(), pflags::line, pflags::col - tokenString.size(), TYPE::DEFAULT, true);
                appendToVector(tokenVector, defaultToken); // this can raise exceptions, so this fun is not noexcept
                pflags::isTokenStringEmpty = true;
                tokenString = "";
            }
        }

    public:
        PseudocodeParser(){} // not noexcept cuz vector construction can raise exceptions

        void parseFromFile(std::string filename){
            readFile(filename, charBuffer);

            pflags::reset();
            
            // loop
            for (std::size_t i = 0; i < charBuffer.size(); i ++){
                char c = charBuffer[i];
                pflags::col += 1;

                if (pflags::stringFlagsImpl.inString){
                    if (doExitString(charBuffer.cbegin() + i)) {
                        pflags::stringFlagsImpl.inString = false;
                        std::string rvalue = pflags::stringFlagsImpl.stringType + tokenString + pflags::stringFlagsImpl.stringType;

                        Token stringLiteral = Token(
                            rvalue.c_str(), pflags::line, pflags::col - rvalue.size(), TYPE::LITERAL, OPERATION::NONE,
                            DTYPE::STRING, tokenString.c_str()
                        );
                        appendToVector(tokenVector, stringLiteral);
                        tokenString = "";
                        continue;
                    }

                    if (isBufferAtEnd(i, charBuffer.size())){
                        pflags::stringFlagsImpl.inString = false;

                        //TODO raise unexpectedEOF
                    }

                    if (isCharValid(c)) {
                        tokenString += c;
                    } else {
                        //TODO raise invalidCharacter or tokenExpected
                    }

                    if (c == '\\') {
                        if (isEscapeValid(charBuffer[i + 1])){
                            i += 1;
                            tokenString += (c + charBuffer[i + 1]);
                        } else {
                            //TODO raise invalidEscape
                        }
                    }

                    continue;
                }

                if (c == '\n') {
                    dubiousLogic_dontTouch();
                    std::string rvalue = "\n";
                    Token lineEnd = Token(rvalue.c_str(), pflags::line, pflags::col - rvalue.size(), TYPE::LINE_END, true);
                    appendToVector(tokenVector, lineEnd);
                    newlineFlags();
                    continue;
                } else if (c == ' '){
                    dubiousLogic_dontTouch();
                    blankspaceFlags();
                    continue;
                } else if (c == '\t'){
                    dubiousLogic_dontTouch();
                    tabFlags();
                    continue;
                }
            
                if (pflags::scopeChangeContext) {
                    unsigned int j = 0;
                    std::string indentationRepr = "";
                    while (j --> pflags::scopeDepth + 1) {
                        indentationRepr += '\t';
                    } //! j has a garbage value after this
                    Token indentationToken = Token(
                        indentationRepr.c_str(), pflags::line, pflags::col - 1, TYPE::INDENTATION,
                        OPERATION::NONE, DTYPE::VOID_PTR, (long long int) pflags::scopeDepth
                    );
                    appendToVector(tokenVector, indentationToken);
                    pflags::scopeChangeContext = false;
                    pflags::sucessiveSpaces = 0;
                }

                if (c == '/') { // division and comments
                    dubiousLogic_dontTouch();
                    if (charBuffer[i + 1] == '/') { // single-line comment
                        i += 1;
                        pflags::col += 1;
                        do {
                            i += 1;
                            pflags::col += 1;
                        } while ((!isBufferAtEnd(i, charBuffer.size())) && (charBuffer[i + 1] != '\n'));
                        newlineFlags();
                    } else if (charBuffer[i + 1] == '*') { // multi-line comment
                        i += 1;
                        pflags::col += 1;
                        do { // loop through the comment
                            i += 1;
                            pflags::col += 1;
                            if (charBuffer[i] == '\n') {
                                newlineFlags();
                            }
                        } while ((!isBufferAtEnd(i, charBuffer.size())) && (charBuffer[i] != '*') && (charBuffer[i + 1] != '/'));
                        i += 1;
                        pflags::col += 1;
                        if (!isBufferAtEnd(i, charBuffer.size())) {
                            // multi-line comment doesnt end, but it reaches EOF so it raises an error
                            //TODO raise the error
                        }
                    } else { // division
                        std::string rvalue = "/";
                        Token operationToken = Token(rvalue.c_str(), pflags::line, pflags::col - rvalue.size(), TYPE::OPERATOR, OPERATION::DIV, true);
                        appendToVector(tokenVector, operationToken);
                    }
                    
                    continue;
                }
            
                if ((c == '+') || (c == '*') || (c == '%') || (c == '-') || (c == '^')) {
                    dubiousLogic_dontTouch();
                    std::string rvalue;
                    rvalue.reserve(1);
                    rvalue.push_back(c);
                    OPERATION _op;
                    switch (c){
                        case '+':
                            _op = OPERATION::ADD;
                            break;
                        case '-':
                            _op = OPERATION::SUB;
                            break;
                        case '*':
                            _op = OPERATION::MLT;
                            break;
                        case '%':
                            _op = OPERATION::MOD;
                            break;
                        case '^':
                            _op = OPERATION::POW;
                            break;
                        default:
                            break;
                    }
                    Token operationToken = Token(rvalue.c_str(), pflags::line, pflags::col - 1, TYPE::OPERATOR, _op, true);
                    appendToVector(tokenVector, operationToken);
                    continue;
                }
            
                if (c == '<') { // LT, LTEQ, ATTR
                    dubiousLogic_dontTouch();
                    if (charBuffer[i + 1] == '-') { // ATTR
                        i += 1;
                        pflags::col += 1;
                        std::string rvalue = "<-";
                        Token operationToken = Token(rvalue.c_str(), pflags::line, pflags::col - 2, TYPE::OPERATOR, OPERATION::ATTR, true);
                        appendToVector(tokenVector, operationToken);
                    } else if (charBuffer[i + 1] == '=') { // LTEQ
                        i += 1;
                        pflags::col += 1;
                        std::string rvalue = "<=";
                        Token operationToken = Token(rvalue.c_str(), pflags::line, pflags::col - 2, TYPE::OPERATOR, OPERATION::LTEQ, true);
                        appendToVector(tokenVector, operationToken);
                    } else { // LT
                        std::string rvalue = "<";
                        Token operationToken = Token(rvalue.c_str(), pflags::line, pflags::col - 1, TYPE::OPERATOR, OPERATION::LT, true);
                        appendToVector(tokenVector, operationToken);
                    }
                    continue;
                }

                if (c == '>') { // GT, GTEQ
                    dubiousLogic_dontTouch();
                    if (charBuffer[i + 1] == '=') { // GTEQ
                        i += 1;
                        pflags::col += 1;
                        std::string rvalue = ">=";
                        Token operationToken = Token(rvalue.c_str(), pflags::line, pflags::col - 2, TYPE::OPERATOR, OPERATION::GTEQ, true);
                        appendToVector(tokenVector, operationToken);
                    } else { // GT
                        std::string rvalue = ">";
                        Token operationToken = Token(rvalue.c_str(), pflags::line, pflags::col - 1, TYPE::OPERATOR, OPERATION::GT, true);
                        appendToVector(tokenVector, operationToken);
                    }
                    continue;
                }

                if ((c == 'N') && (charBuffer[i + 1] == 'U')) { // NOT (NU), NOTEQ (NU=)
                    dubiousLogic_dontTouch();
                    i += 1;
                    pflags::col += 1;
                    if (charBuffer[i + 1] == '=') { // NOTEQ (NU=)
                        i += 1;
                        pflags::col += 1;
                        std::string rvalue = "NU=";
                        Token operationToken = Token(rvalue.c_str(), pflags::line, pflags::col - 3, TYPE::OPERATOR, OPERATION::NOTEQ, true);
                        appendToVector(tokenVector, operationToken);
                        continue;
                    } else if (isCharBlank(charBuffer[i + 1])) { // NOT
                        // checks for blank cuz if smth is appended to NU directly it should be an indentifier
                        // rather than a logical operator
                        // this is subject to change tho
                        std::string rvalue = "NU";
                        Token operationToken = Token(rvalue.c_str(), pflags::line, pflags::col - 2, TYPE::OPERATOR, OPERATION::NOT, true);
                        appendToVector(tokenVector, operationToken);
                        continue;
                    } else {
                        i -= 1;
                        pflags::col -= 1;
                    }
                }
            
                if ((c == 'S') && (charBuffer[i + 1] == 'I') && (isCharBlank(charBuffer[i + 2]))) { // AND (SI)
                    dubiousLogic_dontTouch();
                    // performs the blank space check too (see NOT)
                    i += 1;
                    pflags::col += 1;
                    std::string rvalue = "SI";
                    Token operationToken = Token(rvalue.c_str(), pflags::line, pflags::col - 2, TYPE::OPERATOR, OPERATION::AND, true);
                    appendToVector(tokenVector, operationToken);
                    continue;
                }

                if ((c == 'S') && (charBuffer[i + 1] == 'A') && (
                    charBuffer[i + 2] == 'U'
                ) && (isCharBlank(charBuffer[i + 3]))) { // OR (SAU)
                    dubiousLogic_dontTouch();
                    // same as AND and NOT
                    i += 2;
                    pflags::col += 2;
                    std::string rvalue = "SAU";
                    Token operationToken = Token(rvalue.c_str(), pflags::line, pflags::col - 3, TYPE::OPERATOR, OPERATION::OR, true);
                    appendToVector(tokenVector, operationToken);
                    continue;
                }

                if (c == '=') { // EQ
                    dubiousLogic_dontTouch();
                    std::string rvalue = "=";
                    Token operationToken = Token(rvalue.c_str(), pflags::line, pflags::col - 1, TYPE::OPERATOR, OPERATION::EQ, true);
                    appendToVector(tokenVector, operationToken);
                    continue;
                }

                if ((c == '\"') || (c == '\'')) { // " or ' => string start
                    dubiousLogic_dontTouch();
                    pflags::stringFlagsImpl.inString = true;
                    pflags::stringFlagsImpl.stringType = c;
                    continue;
                }

                if (c == '(') { // parantheses start
                    dubiousLogic_dontTouch();
                    pflags::paranthesesDepth += 1;
                    std::string rvalue = "(";
                    Token paranthesesStart = Token(rvalue.c_str(), pflags::line, pflags::col - 1, TYPE::PARANTHESES_START, true);
                    appendToVector(tokenVector, paranthesesStart);
                    continue;
                }

                if (c == ')') { // parantheses end
                    dubiousLogic_dontTouch();
                    pflags::paranthesesDepth -= 1;
                    if (pflags::paranthesesDepth < 0){
                        // TODO raise error
                        continue;
                    }
                    std::string rvalue = ")";
                    Token paranthesesStart = Token(rvalue.c_str(), pflags::line, pflags::col - 1, TYPE::PARANTHESES_END, true);
                    appendToVector(tokenVector, paranthesesStart);
                    continue;
                }

                if (c == ',') { // separators (commas)
                    dubiousLogic_dontTouch();
                    std::string rvalue = ",";
                    Token separator = Token(rvalue.c_str(), pflags::line, pflags::col - 1, TYPE::COMMA, true);
                    appendToVector(tokenVector, separator);
                    continue;
                }

                defaultFlags();
                tokenString += c;

            } // end loop
        }        

        std::vector<Token> getTokenVectorCopy() {
            return std::vector<Token>(tokenVector);
        }

        ~PseudocodeParser() = default;
};

#endif