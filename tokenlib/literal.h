//this contains the Literal class derived from Token

#include "token.h"
#include "../dtypelib/natural.h"
#include "../dtypelib/integer.h"
#include "../dtypelib/none.h"

#ifndef PSE_LITERAL
#define PSE_LITERAL

template <class D, class P>
class Literal : virtual public Token<D> {
    public:
        Literal(D _self, int _line) : Token(_self.__repr__(), _line){
            linkedDtype = _self;
        }
        ~Literal() override = default;

        P __val__() {
            return linkedDtype.__val__();
        }
};

#endif