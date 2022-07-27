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
        virtual ~Throwable() = 0; 

        const char* what(){
            return stackTrace.c_str();
        }

        bool terminate(){
            return reqTermination;
        }

    protected:
        std::string stackTrace;
        bool reqTermination = false;
};

#endif

