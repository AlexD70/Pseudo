//this contains None data type

#ifndef PSE_INTERNAL_STR
#define PSE_INTERNAL_STR
#include <string>
#endif

#include "dtype.h"

#ifndef PSE_NONE
#define PSE_NONE

class None : virtual public Dtype<unsigned char>{
    public:
        None() : Dtype(){
            repr = "None";
            val = 0;
        }
        ~None() override = default;

        bool isNone() override{
            return true;
        }
};

#endif