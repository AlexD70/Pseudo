#ifndef PSE_INTERNAL_STR
#define PSE_INTERNAL_STR
#include <string>
#endif

//most of the following are self explanatory
//note: smth has to be done w pseutils::raise()
//might hv solved pseutils::raise() a double check wouldnt hurt tho

#ifndef PSE_PARSE_ERR
#define PSE_PARSE_ERR
#include <iostream>

#include "liberrbasic.h"

//TODO we should refine these a little
//note that parsing errors should not use stackTraces
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

class BadParanthesesNesting : public Throwable{
    public:
        BadParanthesesNesting(int line) : Throwable("BadParanthesesNesting: at line " + std::to_string(line), true){}
};

class BadSubscriptUsage : public Throwable{
    public:
        BadSubscriptUsage(int line) : Throwable("BadSubscriptUsage: at line " + std::to_string(line), true){}
};

#endif
