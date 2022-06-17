#include <fstream>
#include "classes.cpp"
//#include "debuglib.cpp"
#include "pseerrorslib.cpp"
#include <vector>

namespace pflags {
    struct stringFlags
    {
        bool inString = false;
        char stringType;
    };

    stringFlags stringFlagsImpl;
    int trailingZeros = 0;
    int startingZeros = 0;
    int paranthesesDepth = 0;
    int scopeDepth = 0;
    int line = 1;
}

class PseudocodeParser {
    public:
        PseudocodeParser() {}
        std::vector<char> charBuffer;

        /**
         * @b readFile
         * @param filename
         * @return void
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

        void parseFromBuffer(){}

    private:
        char peekData;

        void seek(int *i) {
            while (true) {
                peek(*i);

                if ((peekData == ' ')) {
                    *i += 1;
                    continue;

                } else if ((peekData == '\t')) {
                    *i += 1;
                    pflags::scopeDepth += 1;
                    continue;

                } else if ((peekData == '\n')) {
                    *i += 1;
                    pflags::scopeDepth = 0;
                    pflags::line += 1;
                    continue;

                }
            }
        }

        void peek(int pos) {
            peekData = charBuffer[pos + 1];
        }
};

int main()
{
    PseudocodeParser P;
    P.readFile("pseudo.txt");

    Throwable T("Bye, bitch!");
    pseutils::_throw(T);

    /*
    int I[3] = {1, 2, 3};
    std::string S[3] = {"I <3 U", "JIAFEI", "I SAVE DICK BY GIVING IT CPR"};
    T.buildStackTrace("ArithmeticException", I, S, 3);
    */

    pseutils::_throw(T);

    // prtCharVector(P.charBuffer);
}
