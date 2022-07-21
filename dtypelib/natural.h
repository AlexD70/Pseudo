//this contains Natural dtype derived from Number

#include "number.h"
#include "unboundint.h"
#include "../castlib/widen.h"

#ifndef PSE_NATURAL
#define PSE_NATURAL

class Natural : virtual public Number<uint32_t, Natural>{
    public:
        Natural(uint32_t _val) : Number(){
            val = _val;
            repr = std::to_string(_val);
            setFlags();
        }
        Natural(std::string _val) : Number(){
            if (_val == "Inf"){
                setFlags(false, true);
                repr = "Inf";
            } else {
                setFlags(true);
                repr = "NaN";
            }
            val = 1;
        }
        Natural(bool nan = false, bool inf = false) : Number(){
            if (nan) {
                inf = false;
            }

            setFlags(nan, inf);
            if (nan){
                repr = "NaN";
            } else if (inf){
                repr = "Inf";
            } else {
                repr = "1";
            }
            val = 1;
        }
        ~Natural(){}

        int sign() noexcept override{
            if (isNaN){
                return 2;
            }
            return ((isZero())?(0):(1));
        }

        bool __minusinf__() const noexcept override{
            return false;
        }

        Natural* add(Natural &other) override {
            if ((other.__nan__()) || (isNaN)){
                return &Natural(true);

            } else if ((other.__inf__()) || (isInf)){
                return &Natural(false, true);

            } else if ((other.__val__() + val) < 
                       ( (other.__val__() < val)?(val):(other.__val__()) )){
                UnboundInt wider = UnboundInt(val, other.__val__());
                Widen<Natural, UnboundInt> cast = Widen<Natural, UnboundInt>(wider);
                return &cast;

            } else {
                return &Natural((uint32_t) (other.__val__() + val));
            }
        }
        Natural* sub(Natural &other) override {
            if ((other.__nan__()) || (isNaN)){
                return &Natural(true);
            } else if ((isInf) && (other.__inf__())) {
                return &Natural(true);
            } else if (isInf) {
                return &Natural(false, true);
            } else if (other.__inf__()){
                //TODO wrap to Integer dtype
            } else if ((val < other.__val__()) &&
                       ((val - other.__val__()) < other.__val__())) {
                //TODO wrap to Integer
            } else if ((val < other.__val__()) &&
                       ((val - other.__val__()) > other.__val__())) {
                //TODO wrap to UnboundInt
            } else {
                return &Natural(val - other.__val__());
            }
        }       
        Natural* mlt(Natural &other) override {
            bool zeroFound = false;

            if ((other.__nan__()) || (isNaN)){
                return &Natural(true);
            } else if (isZero() || other.isZero()) {
                zeroFound = true;
            } else if ((isInf) || (other.__inf__())) {
                if (zeroFound) {
                    return &Natural(true);
                }
                return &Natural(false, true);
            } else {
                bool outOfBounds = false;
                uint32_t res = 0;

                if (zeroFound){
                    return &Natural((uint32_t)0);
                }
                for (uint32_t i = 1; i <= val; i++){
                    res += other.__val__();
                    if (res < other.__val__()){
                        outOfBounds = true;
                        break;
                    }

                    if (outOfBounds){
                        //TODO wrap to UnboundInt
                    } else {
                        return &Natural(res);
                    }
                }
            }
        }
        Natural* div(Natural &other) override {
            if ((isNaN) || (other.__nan__())){
                return &Natural(true);
            } else if (other.isZero()){
                return &Natural(true);
            } else if ((isInf) && (other.__inf__())) {
                return &Natural(true);
            } else if (isInf) {
                return &Natural(false, true);
            } else {
                return &Natural(val / other.__val__());
            }
        }
        Natural* pow(Natural &other) override {
            if ((other.__nan__()) || (isNaN)){
                return &Natural(true);
            } else if (other.isZero()) {
                return &Natural((uint32_t) 1);
            } else if (isZero()) {
                return &Natural((uint32_t) 0);
            } else if (isInf) {
                return &Natural(false, true);
            } else if ((other.__inf__()) && (val != 1)) {
                return &Natural(false, true);
            } else {
                bool outOfBounds = false;
                uint32_t res = 0;

                for (uint32_t i = 0; i < other.__val__(); i ++){
                    for (uint32_t j = 0; j < val; j ++){
                        res += val;

                        if (res < val){
                            outOfBounds = true;
                            break;
                        }

                        if (outOfBounds){
                            break;
                        }
                    }
                }

                if (outOfBounds){
                    //TODO cast to UnboundInt
                } else {
                    return &Natural(res);
                }
            }
        }

        Natural* mod(Natural &other) override {
            if ((isNaN) || (other.__nan__()) || (isInf) || 
                (other.__inf__()) || (other.isZero())) {
                    return &Natural(true);
            } else {
                return &Natural(val % other.__val__());
            }
        }
        Natural* sqrt() override {
            //TODO cast to Float/Double/Real
        }
};

#endif