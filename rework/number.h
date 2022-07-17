//this contains the Number abstract class
//do not create instances of this class

#include "dtype.h"

#ifndef PSE_NUMBER
#define PSE_NUMBER

template <class T, class S>
class Number : public Dtype<T>{
    protected:
        bool isNaN, isInf, isMinusInf;

        void setFlags(bool nan = false, bool inf = false, bool minusinf = false){
            isNaN = nan;
            isInf = inf;
            isMinusInf = minusinf;
        }

    public:
        Number() : Dtype(){}

        bool isNone() override{
            return false;
        }
        bool isZero(){
            return (val == 0);
        }

        bool __nan__(){
            return isNaN;
        }
        bool __inf__(){
            return isInf;
        }

        virtual bool __minusinf__(){
            return isMinusInf;
        }
        virtual int sign(){
            if (isNaN){
                return 2;
            }
            return ((isZero())?(0):((val < 0)?(-1):(1)));
        }

        virtual S* add(S &other) = 0;
        virtual S* sub(S &other) = 0;
        virtual S* mlt(S &other) = 0;
        virtual S* pow(S &other) = 0;
};

#endif