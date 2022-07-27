//this contains the Parantheses class derived from Token

#include "token.h"
#include "../dtypelib/none.h"

#ifndef PSE_PARANTHESES
#define PSE_PARANTHESES

class Parantheses : virtual public Token<None> {
    protected:
        int depth;
        bool type; //0 - START, 1 - END

    public:
        Parantheses(int _depth, bool _type, int _line) : Token() {
            line = _line;
            depth = _depth;
            type = _type;

            if (_type){
                repr = ")";
            } else {
                repr = "(";
            }
        }
        ~Parantheses() override = default;

        int __depth__(){
            return depth;
        }
};

#endif