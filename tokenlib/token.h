//this will contain the base class for all tokens
//do not create instances of this

#ifndef PSE_INTERNAL_STR
#define PSE_INTERNAL_STR
#include <string>
#endif

#include "dtype.h"

#ifndef PSE_TOKENS_BASIC
#define PSE_TOKENS_BASIC

template <class T>
class Token {
    protected:
        std::string repr;
        int line;
        int wrapperFlag;
        T linkedDtype;
        virtual ~Token() = 0;

    public:
        Token(){}
        Token(std::string _repr, int _line){
            repr = _repr;
            line = _line;
        }
        std::string __repr__(){
            return repr;
        };
        int __line__(){
            return line;
        };
        T __self__(){
            return linkedDtype;
        };
};

#endif