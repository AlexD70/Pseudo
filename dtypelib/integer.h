
#include "number.h"

#ifndef PSE_INTEGER
#define PSE_INTEGER

class Integer : virtual public Number<int, Integer>{
    public:
        Integer(int _val) : Number(){
            val = _val;
            repr = std::to_string(_val);
            setFlags();
        }
        
        Integer(std::string _val){
            if (_val == "Inf"){
                setFlags(false, true);
                repr = "Inf";
            } else if (_val == "-Inf"){
                setFlags(false, false, true);
                repr = "-Inf";
                val = 1;
                return;
            } else {
                setFlags(true);
                repr = "NaN";
            }

            val = 1;
        }
        Integer(bool nan = false, bool inf = false, bool minusinf = false) : Number(){
            if ((inf) && (minusinf)){
                inf = false;
                minusinf = false;
                nan = true;
            } else if (nan) {
                inf = false;
                minusinf = false;
            }

            setFlags(nan, inf, minusinf);

            if (nan) {
                repr = "NaN";
            } else if (inf) {
                repr = "Inf";
            } else if (minusinf){
                repr = "-Inf";
                val = -1;
                return;
            } else {
                repr = "1";
            }
            val = 1;
        }
        ~Integer(){}

        Integer* add(Integer &other) override{}
        Integer* sub(Integer &other) override{}
        Integer* mlt(Integer &other) override{}
        Integer* div(Integer &other) override{}
        Integer* pow(Integer &other) override{}
};

#endif