//this contains None data type

#ifndef PSE_INTERNAL_STR
#define PSE_INTERNAL_STR
#include <string>
#endif

#include "dtype.h"

#ifndef PSE_NONE
#define PSE_NONE

class None : public Dtype<unsigned char>{
    public:
        None() : Dtype(){
            repr = "None";
        }

        std::string __repr__() override{
            return repr;
        }
        unsigned char __val__() override{
            return 0;
        }
        bool isNone() override{
            return true;
        }
};

#endif