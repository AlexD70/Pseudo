//this contains Natural dtype derived from Number

#include "number.h"

#ifndef PSE_NATURAL
#define PSE_NATURAL

class Natural : public Number<unsigned int, Natural>{
    public:
        Natural(unsigned int _val) : Number(){
            val = _val;
            repr = std::to_string(_val);
            setFlags();
        }
        Natural(std::string _val) : Number(){
            if (_val == "Inf"){
                setFlags(false, true);
            } else {
                setFlags(true);
            }
            val = 1;
        }
        Natural(bool nan = false, bool inf = false) : Number(){
            setFlags(nan, inf);
            val = 1;
        }

        int sign() override{
            if (isNaN){
                return 2;
            }
            return ((isZero())?(0):(1));
        }

        bool __minusinf__() override {
            return false;
        }

        Natural* add(Natural &other) override {
            if ((other.__nan__()) || (isNaN)){
                return &Natural(true);

            } else ((other.__inf__()) || (isInf)){
                return &Natural(false, true);

            } else if ((other.__val__() + val) < 
                       ( (other.__val__() < val)?(val):(other.__val__()) )){
                //TODO something to wrap into a large number
                return &Natural(0);

            } else {
                return &Natural(other.__val__() + val);
            }
        }

        Natural* sub(Natural &other) override {}
        Natural* mlt(Natural &other) override {}
        Natural* pow(Natural &other) override {}
};

#endif