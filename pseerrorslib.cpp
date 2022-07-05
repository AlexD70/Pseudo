#include <exception>
#include <string>
#include <iostream>

#ifndef PSEERR_BASIC
#define PSERR_BASIC
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

#endif

#ifndef PSE_PARSE_ERR
#define PSE_PARSE_ERR

class UnexpectedEOF : public Throwable{
    public:
        UnexpectedEOF(UnexpectedEOF &err) : Throwable(err){}
        UnexpectedEOF() : Throwable("UnexpectedEOF: raised while parsing.", true){}
        UnexpectedEOF(std::string message) : Throwable(message, true){}
};

class InvalidEscape : public Throwable{
    public:
        std::string buildStackTrace(int line, std::string escape){
            return ("InvalidEscape: " + escape + " at line " + std::to_string(line) + ".");
        }

        InvalidEscape(InvalidEscape &err) : Throwable(err){}
        InvalidEscape(std::string message) : Throwable(message, true){}
        InvalidEscape(int line, std::string escape) : Throwable(buildStackTrace(line, escape), true){}
};

class InvalidString : public Throwable{
    public:
        InvalidString(int line) : Throwable("InvalidString: at line " + std::to_string(line), true){}
};

#endif

#ifndef PSE_UTILS
#define PSE_UTILS

namespace pseutils{
    void raise(Throwable &T){
        std::cout << T.what() << '\n';

        if (T.terminate()){
            //idk what to do here probably a goto or setting a bool flag to true/false
        }

        return;
    }
};

#endif
