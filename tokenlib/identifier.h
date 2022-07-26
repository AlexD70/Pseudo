//this contains the Identifier class derived from Token

#include "token.h"
#include "../dtypelib/natural.h"
#include "../dtypelib/integer.h"
#include "../dtypelib/none.h"

#ifndef PSE_INTERNAL_TYPEINFO
#define PSE_INTERNAL_TYPEINFO
#include <typeinfo>
#endif

#ifndef PSE_IDENTIFIER
#define PSE_IDENTIFIER

template <class D, class P>
class Identifier : virtual public Token<D> {
    public:
        Identifier(std::string _repr, D _self, int _line) : Token(_repr, _line) {
            linkedDtype = _self;
        }
        ~Identifier() override = default;

        D __val__() {
            return linkedDtype.__val__();
        }

        std::string __type__() {
            return typeid(linkedDtype).name();
        }

        void __set__(D _self){
            linkedDtype = _self;
        }

        void __set__(P _val){
            linkedDtype = D((P) _val);
        }
};

#endif