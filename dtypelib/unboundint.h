//this contains the UnboundInt class derived from 
//be careful while using cuz its memory hungry

#ifndef PSE_INTERNAL_VECTOR
#define PSE_INTERNAL_VECTOR
#include <vector>
#endif

#include "number.h"

#ifndef PSE_UNBOUND_INT
#define PSE_UNBOUND_INT

class UnboundInt : virtual public Number<std::vector<uint_fast8_t> , UnboundInt>{
    protected:
        enum OPERATIONS {
            ADDITION = 0,
            SUBTRACTION = 1,
            MULTIPLICATION = 2,
            POWER = 3
        };

        std::string buildRepr() noexcept {
            std::string result = "";

            for(uint_fast8_t e : val){
                result += std::to_string(e);
            }

            repr = result;
            return result;
        }

        void pushToFirstPos(uint_fast8_t _val) noexcept {
            val.reserve(val.size() + 1);
            val.insert(val.begin(), _val);
        }

        void appendToVal(uint_fast8_t _val) noexcept {
            val.reserve(val.size() + 1);
            val.push_back(_val);
        }

        template <typename... Args>
        void appendToVal(uint_fast8_t _val, Args... _args) noexcept {
            appendToVal(_val);
            appendToVal(_args);
        }

        void caseAddition(uint32_t a, uint32_t b) noexcept {
            uint_fast8_t _a, _b, sum;
            size_t i;
            bool addToNext = false;

            while( true ) {
                if ((a == 0) || (b == 0) && (!(addToNext))){
                    break;
                }

                _a = a % 10;
                _b = b % 10;
                a = a / 10;
                b = b / 10;

                sum = _a + _b + ((uint_fast8_t) addToNext);
                if (sum >= 10){
                    sum = sum % 10;
                    addToNext = true;
                } else {
                    addToNext = false;
                }

                pushToFirstPos(sum);
            }
        }
        void caseSubtraction(){}
    public:
        UnboundInt(uint_fast8_t _val) noexcept : Number(){
            appendToVal(_val);
        }

        template <typename... Args>
        UnboundInt(uint_fast8_t _val, Args... _args) noexcept : Number(){
            appendToVal(_val, _args);
        }

        UnboundInt(uint32_t a, uint32_t b, unsigned char operation = 0) noexcept : Number(){
            switch(operation) {
                case ADDITION:
                    caseAddition(a, b);
                    break;
                case SUBTRACTION:
                    break;
                case MULTIPLICATION:
                    break;
                case POWER:
                    break;
                default:
                    break;
            }
        }

        UnboundInt* add(UnboundInt &other) override {}
        UnboundInt* sub(UnboundInt &other) override {}
        UnboundInt* mlt(UnboundInt &other) override {}
        UnboundInt* div(UnboundInt &other) override {}
        UnboundInt* pow(UnboundInt &other) override {}
};

#endif