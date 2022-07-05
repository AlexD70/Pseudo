#include <fstream>
#include <iostream>
//#include "classes.cpp"
//#include "debuglib.cpp"
#include "pseerrorslib.cpp"
#include <vector>
#include "pseinternal.cpp"

/**
 * @b pflags namespace
 * @brief flags used for parsing
 * @details @li @c stringFlagsImpl.stringType is either '\'' or '\"'
 *          @li @c trailingZeros and @c startingZeros are intended to be used to raise warnings
 *          @li @c scopeDepth keeps account of current scope by counting indentation
 *          @li @c line is used for stackTrace building purposes @see @c Throwable.buildStackTrace()
 */
namespace pflags {
    struct stringFlags
    {
        bool inString = false;
        char stringType;
    };

    stringFlags stringFlagsImpl;
    bool scopeChangeContext = false;
    int paranthesesDepth = 0;
    int scopeDepth = 0;
    int sucessiveSpaces = 0;
    int line = 1;
}

/**
 * @b PseudocodeParser class
 * @c constructors
 *     @li @c PseudocodeParser(){}
 * 
 * @c public fields
 *     @li @c charBuffer std::vector<char>
 * 
 * @c public methods
 *     @li @c readFile(std::string) void
 *     @li @c parseFromBuffer(std::vector<char>s) void
 * 
 * @c private fields
 *     @li @c peekData char
 * 
 * @c private methods
 *     @li @c seek(int*) void
 *     @li @c peek(int) void
 */
class PseudocodeParser {
    public:
        PseudocodeParser(){}
        std::vector<char> charBuffer = std::vector<char>();
        std::vector<Token> tokenVector = std::vector<Token>();

        /**
         * @b readFile public method
         * @param filename
         * @returns void
         */
        void readFile(std::string filename) {
            char ch;
            std::ifstream f(filename);

            for (int i = 0;; i++) {

                ch = f.get();

                if (f.eof()) {
                    break;
                } else {
                    charBuffer.reserve(i + 1);
                    charBuffer.push_back(ch);
                }
            }

            f.close();
            return;
        }

        void parseFromBuffer() {
            parseFromBuffer(charBuffer);
        }

        void parseFromBuffer(std::vector<char> buffer) {
            resetFlags();
            std::string tokenString = "";
            int otherData = 0;

            for (int i = 0; i < buffer.size(); i ++){
                char c = buffer[i];

                if (pflags::stringFlagsImpl.inString) {
                    if (c == '\\'){
                        peek(i, buffer);
                        if (peekData == '\\'){
                            i += 1;
                            tokenString = tokenString + '\\';
                        } else if (peekData == 'n'){
                            i += 1;
                            tokenString = tokenString + '\n';
                        } else if (peekData == 't'){
                            i += 1;
                            tokenString + '\t';
                        } else if (peekData == 'b'){
                            i += 1;
                            tokenString = tokenString + '\b';
                        } else if (peekData == '\"'){
                            i += 1;
                            tokenString = tokenString + '\"';
                        } else if (peekData == '\''){
                            i += 1;
                            tokenString = tokenString + '\'';
                        } else if (peekData == '\n'){
                            pflags::line += 1;
                            i += 1;
                            otherData += 1;
                        } else {
                            InvalidEscape err = InvalidEscape(pflags::line, std::string() + c + peekData);
                            pseutils::raise(err);
                        }
                    } else if ((c == '\n') || (iscntrl(c)) || !(isascii(c))){
                        InvalidString err = InvalidString(pflags::line);
                        pseutils::raise(err);
                    } else if (c == pflags::stringFlagsImpl.stringType){
                        pflags::stringFlagsImpl.inString = false;
                        pflags::scopeChangeContext = false;
                        tokenVector.reserve(tokenVector.size() + 1);
                        tokenVector.push_back(Token(pflags::line - otherData, tokenString, 5));
                        otherData = 0;
                        pflags::scopeDepth = 0;
                    } else {
                        tokenString = tokenString + c;
                    }
                }

                if ((c == '\t') || (c == ' ') || (c == '\n')){
                    seek(&i, buffer);
                    continue;
                }
                
                if (pflags::scopeChangeContext){
                    tokenVector.reserve(tokenVector.size() + 1);
                    tokenVector.push_back(Indentation(pflags::line, pflags::scopeDepth));
                }

                pflags::scopeChangeContext = false;
                pflags::scopeDepth = 0;

                if (c == '/'){
                    if (buffer[i + 1] == '/'){
                        i += 1;
                        do{
                            i += 1;
                        } while ((i < buffer.size()) && (buffer[i] != '\n'));
                        pflags::line += 1;
                    } else if (buffer[i + 1] == '*'){
                        i += 1;
                        do{
                            i += 1;
                            if (buffer[i] == '\n'){
                                pflags::line += 1;
                            }
                        } while((i < buffer.size()) && (buffer[i] != '*') && (buffer[i + 1] != '/'));
                        if (i >= buffer.size()){
                            UnexpectedEOF err = UnexpectedEOF();
                            pseutils::raise(err);
                        }
                    } else {
                        tokenVector.reserve(tokenVector.size() + 1);
                        char arr[2] = {c, '\0'};
                        tokenVector.push_back(Token(pflags::line, arr, 1));
                        continue;
                    }
                }

                if ((c == '+') || (c == '*') || (c == '%') || (c == '-')){
                    tokenVector.reserve(tokenVector.size() + 1);
                    char arr[2] = {c, '\0'};
                    tokenVector.push_back(Token(pflags::line, arr, 1));
                    continue;
                }

                if (c  == '<'){
                    tokenVector.reserve(tokenVector.size() + 1);
                    if (buffer[i + 1] == '-'){
                        i += 1;
                        char arr[3] = {'<', '-', '\0'};
                        tokenVector.push_back(Token(pflags::line, arr, 3));
                    } else if (buffer[i + 1] == '=') {
                        i += 1;
                        char arr[3] = {'<', '=', '\0'};
                        tokenVector.push_back(Token(pflags::line, arr, 4));
                    } else {
                        char arr[2] = {'<', '\0'};
                        tokenVector.push_back(Token(pflags::line, arr, 4));
                    }
                    continue;
                }
            
                if (c == '>'){
                    tokenVector.reserve(tokenVector.size() + 1);
                    if (buffer[i + 1] == '='){
                        char arr[3] = {'>', '=', '\0'};
                        tokenVector.push_back(Token(pflags::line, arr, 4));
                    } else {
                        char arr[2] = {'>', '\0'};
                        tokenVector.push_back(Token(pflags::line, arr, 4));
                    }
                    continue;
                }

                if ((c == 'N') && (buffer[i + 1] == 'U')){
                    i += 1;
                    tokenVector.reserve(tokenVector.size() + 1);
                    if (buffer[i + 1] == '='){
                        i += 1;
                        char arr[4] = {'N', 'U', '=', '\0'};
                        tokenVector.push_back(Token(pflags::line, arr, 4));
                        continue;
                    } else if (buffer[i + 1] == ' '){
                        i += 1;
                        char arr[3] = {'N', 'U'};
                        tokenVector.push_back(Token(pflags::line, arr, 2));
                        continue;
                    } else {
                        i -= 1;
                    }
                }
            
                if ((c == 'S') && (buffer[i + 1] == 'I') && (buffer[i + 2] == ' ')){
                    i += 2;
                    char arr[3] = {'S', 'I', '\0'};
                    tokenVector.reserve(tokenVector.size() + 1);
                    tokenVector.push_back(Token(pflags::line, arr, 2));
                    continue;
                }
            
                if ((c == 'S') && (buffer[i + 1] == 'A') && 
                    (buffer[i + 2] == 'U') && (buffer[i + 3] == ' ')){
                        
                    i += 3;
                    char arr[4] = {'S', 'A', 'U', '\0'};
                    tokenVector.reserve(tokenVector.size() + 1);
                    tokenVector.push_back(Token(pflags::line, arr, 2));
                }
            
                if (c == '='){
                    tokenVector.reserve(tokenVector.size() + 1);
                    char arr[2] = {'=', '\0'};
                    tokenVector.push_back(Token(pflags::line, arr, 2));
                    continue;
                }
            
                if (c == '\"'){
                    pflags::stringFlagsImpl.inString = true;
                    pflags::stringFlagsImpl.stringType = '\"';
                }
            }
        }

    private:
        char peekData;

        void resetFlags(){
            pflags::line = 1;
            pflags::paranthesesDepth = 0;
            pflags::scopeChangeContext = false;
            pflags::scopeDepth = 0;
            pflags::stringFlagsImpl.inString = false;
            pflags::sucessiveSpaces = 0;

            return;
        }

        std::string buildToken(int *i, std::vector<char> buffer){
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

        /**
         * @b seek private method
         * @brief increments i until @code buffer[i] @endcode is alnum or
         *        a special character (is not space, tab or newline)
         * @details when @code charBuffer[i] @endcode is '\t' @link @c pflags::scopeDepth @endlink is changed 
         *          @if the context is right @see @c pflags::scopeChangeContext @endif
         * @details when @code pflags::sucessiveSpaces @endcode reaces 4 and the context is right,
         *          changes @link @c pflags::scopeDepth @endlink
         * @param i 
         * @returns void
         */
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

        /**
         * @b peek private method
         * @brief stores @code charBuffer[pos + 1] @endcode in peekData for easy retrieval
         * @param pos 
         * @returns void
         */
        void peek(int pos, std::vector<char> buffer) {
            peekData = buffer[pos + 1];
        }
};

int main()
{
    dtypes::Integer a = dtypes::Integer(5), b = dtypes::Integer(2);

    std::cout << ((-9) % 2) << '\n';
}
