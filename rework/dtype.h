//this contains the base class for all data types
//do not create instances of this

#ifndef PSE_INTERNAL_STR
#define PSE_INTERNAL_STR
#include <string>
#endif

#ifndef PSE_DTYPES_BASIC
#define PSE_DTYPES_BASIC

template <class T>
class Dtype {
    protected:
        T val;
        std::string repr;

    public:
        Dtype(){}
        ~Dtype(){}
        std::string __repr__(){
            return repr;
        }
        
        T __val__(){
            return val;
        }
        virtual bool isNone() = 0;
};

#endif