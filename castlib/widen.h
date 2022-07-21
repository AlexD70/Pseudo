//this contains the Widen cast class
//never give Wide the narrower dtype

#ifndef PSE_WIDEN
#define PSE_WIDEN

template<class Narrow, class Wide>
class Widen : public Narrow{
    protected:
        Wide* wider;

    public:
        Widen(Wide &_wider){
            wider = _wider;
        }
        Widen(){}
        ~Widen(){}

        virtual Wide* cast() {
            return wider;
        }

        enum TYPE {
            TO_INTEGER = 0,
            TO_FLOAT = 1,
            TO_DOUBLE = 2,
            TO_RATIONAL = 3,
            TO_REAL = 4,
            TO_UNBOUND_INT = 5
        };
};

#endif