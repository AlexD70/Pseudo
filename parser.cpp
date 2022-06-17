#include <fstream>
#include <iostream>
//#include "classes.cpp"
//#include "debuglib.cpp"
//#include "pseerrorslib.cpp"
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
    int trailingZeros = 0;
    int startingZeros = 0;
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
        PseudocodeParser() {}
        std::vector<char> charBuffer;

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

            return;
        }

        void parseFromBuffer(std::vector<char> buffer) {

        }

    private:
        char peekData;

        /**
         * @b seek private method
         * @brief increments i until @code charBuffer[i] @endcode is alnum or
         *        a special character (is not space, tab or newline)
         * @details when @code charBuffer[i] @endcode is '\t' @link @c pflags::scopeDepth @endlink is changed 
         *          @if the context is right @see @c pflags::scopeChangeContext @endif
         * @details when @code pflags::sucessiveSpaces @endcode reaces 4 and the context is right,
         *          changes @link @c pflags::scopeDepth @endlink
         * @param i 
         * @returns void
         */
        void seek(int *i) {
            while (true) {
                peek(*i);

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
                    pflags::scopeChangeContext = false;
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

        void peek(int pos) {
            peekData = charBuffer[pos + 1];
        }
};

int main()
{

    Token T = Token();
    Token M = Token();
    T.setReprOnce("Hello World");
    M.setReprOnce("Taylor Swift");

    TokenHolder<Token> TkH = Token::globalTokenHolder;
    TkH.pushBack(T);
    TkH.pushBack(M);

    std::cout << TkH.internalArray[0].strRepr();

    /*
    int I[3] = {1, 2, 3};
    std::string S[3] = {"I <3 U", "CUPCAKKE", "I SAVE DICK BY GIVING IT CPR"};
    T.buildStackTrace("ArithmeticException", I, S, 3);
    */

    //pseutils::_throw(T);

    // prtCharVector(P.charBuffer);
}
