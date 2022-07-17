//this contains the Indentation token derived from Token

#include "token.h"
#include "none.h"

#ifndef PSE_INDENTATION
#define PSE_INDENTATION

class Indentation : public Token<None>{
    public:
        int depth;

        Indentation(int _line, int _depth = 0) : 
        Token("Indentation: depth " + std::to_string(_depth), _line){
            depth = _depth;
            linkedDtype = None();
        }

        std::string __repr__() override{
            return repr;
        }
        int __line__() override{
            return line;
        }
        None __self__() override{
            return linkedDtype;
        }
};

#endif