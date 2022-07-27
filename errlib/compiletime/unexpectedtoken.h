#include "../throwable.h"

#ifndef PSE_UNEXPECTED_TOKEN
#define PSE_UNEXPECTED_TOKEN

class UnexpectedToken : virtual public Throwable {
    public:
        UnexpectedToken(std::string _token, int _line) : 
        Throwable("Unexpected token " + _token + " at line: " + std::to_string(_line) + ".\n", true) {}

        ~UnexpectedToken() override = default;
};

#endif