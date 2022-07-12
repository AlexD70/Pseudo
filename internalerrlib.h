#ifndef PSE_INTERNAL_ERR
#define PSE_INTERNAL_ERR
// this error shouldnt occur on client side
// if they do it is probably because of an internal logic error in our code

#include "liberrbasic.h"

class InternalError : public Throwable {
    public:
        InternalError(std::string file) : Throwable("An internal error occured in file" + file + ". Please submit this as a bug report on" +
        "our github page along with the file you tried to parse. We will look into it soon.", true){}
};

#endif