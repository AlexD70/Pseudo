#ifndef PSE_INTERNAL_STR
#define PSE_INTERNAL_STR
#include <string>
#endif

#ifndef PSE_ERR_BASIC
#define PSE_ERR_BASIC
#include <exception>

class Throwable : public std::exception{
    public:
        Throwable(std::string message, bool reqTermination) {
            stackTrace = message;
            reqTermination = reqTermination;
        }

        Throwable(std::string message) {
            stackTrace = message;
        }

        Throwable(Throwable& T){
            stackTrace = T.what();
            reqTermination = T.terminate();
        }

        const char* what(){
            return stackTrace.c_str();
        }

        std::string buildStackTrace(std::string exceptionType, int* lines, std::string* lineContents, int scopeDepth){
            std::string newStackTrace;
            //std::string __tabs = "";
            newStackTrace = "\n" + exceptionType;

            for (int i = 0; i < scopeDepth; i++){
                //__tabs += "\t";
                newStackTrace +=  " at line " + std::to_string(lines[i]) + ":\n\t" /*__tabs*/ + lineContents[i] + "\n";
            }

            stackTrace = newStackTrace;
            return newStackTrace;
        }

        bool terminate(){
            return reqTermination;
        }

    protected:
        std::string stackTrace;
        bool reqTermination = false;
};

namespace pseutils{
    void raise(Throwable &T){
        std::cout << T.what() << '\n';

        if (T.terminate()){
            throw T;
        }

        return;
    }
};

#endif

