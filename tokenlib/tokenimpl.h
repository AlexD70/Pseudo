//this contains the default implementation of Token

#include "token.h"
#include "none.h"

#ifndef PSE_TOKEN_IMPL
#define PSE_TOKEN_IMPL

class TokenImpl : virtual public Token<None> {
    public:
        TokenImpl(std::string _repr, int _line) : Token(_repr, _line){
            linkedDtype = None();
        }
        TokenImpl(const char* _repr, int _line) : Token(_repr, _line){
            linkedDtype = None();
        }
        ~TokenImpl() override = default;
};

#endif