//this contains the Indentation class derived from Token

#include "token.h"
#include "../dtypelib/none.h"

#ifndef PSE_INDENTATION
#define PSE_INDENTATION

class Indentation : virtual public Token<None>{
    public:
        int depth;

        Indentation(int _line, int _depth = 0) : 
        Token("Indentation: depth " + std::to_string(_depth), _line){
            depth = _depth;
            linkedDtype = None();
        }
        ~Indentation() override = default;
};

#endif