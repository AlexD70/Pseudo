//this contains the Number abstract class
//do not create instances of this class

#include "dtype.h"

#ifndef PSE_NUMBER
#define PSE_NUMBER

template <class T, class S>
class Number : virtual public Dtype<T>{
    protected:
        bool isNaN, isInf, isMinusInf;

        void setFlags(bool nan = false, bool inf = false, bool minusinf = false) noexcept{
            isNaN = nan;
            isInf = inf;
            isMinusInf = minusinf;
        }

    public:
        Number(){};
        virtual ~Number() override = default;

        virtual void wrap(int a, uint32_t b, uint32_t c) {}

        bool isNone() override {
            return false;
        }
        virtual bool isZero() const noexcept {
            return (val == 0);
        }

        bool __nan__() const noexcept{
            return isNaN;
        }
        bool __inf__() const noexcept{
            return isInf;
        }

        virtual bool __minusinf__() const noexcept{
            return isMinusInf;
        }
        virtual int sign() noexcept{
            if (isNaN){
                return 2;
            }
            return ((isZero())?(0):((val < 0)?(-1):(1)));
        }

        virtual S* add(S &other) = 0;
        virtual S* sub(S &other) = 0;
        virtual S* mlt(S &other) = 0;
        virtual S* div(S &other) = 0;
        virtual S* pow(S &other) = 0;

        virtual S* mod(S &other){
            return nullptr;
        }
        virtual void changeSign(){}
        virtual void abs(){}
        virtual S* sqrt(){
            return nullptr;
        }
};

#endif