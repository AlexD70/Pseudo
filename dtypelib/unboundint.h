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
        bool isNeg = false;

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
        
        void initFromAdd(uint32_t a, uint32_t b) noexcept {
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
        void initFromSub(uint32_t a, uint32_t b) noexcept {
            isNeg = (a < b);
            uint64_t k = ((isNeg)?(-1):(1)) * ((uint64_t) a - (uint64_t) b);
            uint_fast8_t kdigit = 0;

            while(k > 0){
                kdigit = k % 10;
                k = k / 10;
                pushToFirstPos(k);
            }
        }
        void initFromMlt(uint32_t a, uint32_t b) noexcept {
            uint_fast16_t k;
            uint32_t _a = a, _b = b;
            short dignr_a = 0, dignr_b = 0, over10;
            size_t i = 0;

            while(_a > 0){
                _a = _a / 10;
                dignr_a += 1;
            }

            uint_fast8_t* adigits = new uint_fast8_t[dignr_a + 1]{ 0 };

            while(_b > 0){
                _b = _b / 10;
                dignr_a += 1;
                dignr_b += 1;
            }

            uint_fast8_t* proddigits = new uint_fast8_t[dignr_a + 1]{ 0 };

            while(a > 0){
                adigits[i] = a % 10;
                a = a / 10;
                i += 1;
            }

            for (i = 0; i < dignr_a; i ++){
                uint16_t pow10 = 1;
                for (int j = 0; j < dignr_b; j ++){
                    k = adigits[i] * (b / (pow10) % 10);
                    pow10 = pow10 * 10;
                }
                proddigits[i] += k % 10;
                proddigits[i + 1] += k / 10;

                size_t j = i;
                while(proddigits[j] > 9){
                    over10 = proddigits[j] / 10;
                    proddigits[j] = proddigits[j] % 10;

                    proddigits[j + 1] += over10;
                    j += 1;
                }
            }

            for (i = 0; i < dignr_a; i ++){
                appendToVal(proddigits[i]);
            }
        }
        void initFromPow(uint32_t a, uint32_t b ) noexcept {}
        
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
                    initFromAdd(a, b);
                    break;
                case SUBTRACTION:
                    initFromSub(a, b);
                    break;
                case MULTIPLICATION:
                    initFromMlt(a, b);
                    break;
                case POWER:
                    initFromPow(a, b);
                    break;
                default:
                    break;
            }
        }

        void wrap(int a,  uint32_t b, uint32_t c) override {
            switch(a) {
                case ADDITION:
                    initFromAdd(b, c);
                    break;
                case SUBTRACTION:
                    initFromSub(b, c);
                    break;
                case MULTIPLICATION:
                    initFromMlt(b, c);
                    break;
                case POWER:
                    initFromPow(b, c);
                    break;
                default:
                    break;
            }
        }

        bool isZero() const noexcept override {
            return !((bool) val.size());
        }

        int sign() noexcept override {
            if (isNaN) {
                return 2;
            }
            return ((isZero())?(0):((isNeg)?(-1):(1)));
        }

        UnboundInt* add(UnboundInt &other) override {

        }
        UnboundInt* sub(UnboundInt &other) override {}
        UnboundInt* mlt(UnboundInt &other) override {}
        UnboundInt* div(UnboundInt &other) override {}
        UnboundInt* pow(UnboundInt &other) override {}
};

#endif