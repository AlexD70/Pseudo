#include <string>
#include <vector>

#ifndef PSE_TOKENS_BASIC
#define PSE_TOKENS_BASIC

/*
so um this document is very crazy
i wonder how tf i wrote it but it works so im not questioning it
so i advise u not to question it as well
unless i did some major bad practice
also we wont talk abt how i wanted to write: #define elif else if
*/

class Token {
    protected:
        std::string __repr__ = "";
        int line;

    public:
        Token(){}
        Token(int line){
            line = line;
        }
        
        std::string strRepr(){
            return __repr__;
        }

        void setReprOnce(std::string str) { //std::strings
            if (__repr__ != ""){
                return;
            } else {
                __repr__ = str;
            }
        }    

        void setReprOnce(std::vector<char> str){ //char std::vectors
            if (__repr__ != ""){
                return;
            } else {
                for (char e : str){
                    __repr__ += e;
                }
            }   
        }

        void setReprOnce(const char* str, int len){ //c-style strings
            if (__repr__ != ""){
                return;
            } else {
                for (int i = 0; i < len; i++){
                    __repr__ += str[i];
                }
            }
        }
};


#endif

#ifndef PSE_DTYPES
#define PSE_DTYPES

namespace dtypes{

    class Number{
        protected:
            bool NaN = false;
            bool Inf = false;
            bool MinusInf = false;

            template <class T>
            bool isZero(T number){
                //dont question it just trust my methods
                return !(number.isNaN() || number.isInf() || number.isMinusInf() || number.getVal());
            }

            template <class T>
            T add(T& j, T& i){
                T result = T(0);

                if (j.isNaN() || i.isNaN()){
                    result = T("NaN");

                } else if ((( !i.isMinusInf() )&&( j.isInf() )) ||
                           (( i.isInf() ) && ( !j.isMinusInf() ))){
                    result = T("Inf");

                } else if ((( j.isMinusInf() ) && ( !i.isInf() )) ||
                           (( i.isMinusInf() ) && ( !j.isInf() ))) {
                    result = T("-Inf");

                } else if (( i.isInf() ) && ( j.isInf() )){
                    result = T("Inf");

                } else if (( i.isMinusInf() ) && ( j.isMinusInf() )){
                    result = T("-Inf");
                } else {
                    result = T(j.getVal() + i.getVal());
                }

                return result;
            };

            template <class T>
            int sign(T number){
                if (number.isNaN()){
                    return 2;
                } else if (number.isInf()){
                    return 1;
                } else if (number.isMinusInf()){
                    return -1;
                } else {
                    //note: trying to catch an arithmetic exception (division by 0)
                    //      with a try-catch block isnt allowed cuz its not a std::runtime_exception,
                    //      rather an undefined behaviour situation (is not derived from std::exception)
                    if (isZero(number)){
                        return 0;
                    } else {
                        return (number.getVal()) / abs(number.getVal());
                    }
                }
            }

            template <class T>
            T changeSign(T& number){
                if ( number.isNaN() ){
                    return T("NaN");
                } else if ( number.isInf() ){
                    return T("-Inf");
                } else if ( number.isMinusInf() ){
                    return T("Inf");
                } else  {
                    return T(-number.getVal());
                }
            }

            template <class T>
            T mlt(T& j, T& i){
                // this function is extremely unorthodox
                // it kind of reminds me of jiafei products for some reason
                int sgn = sign(j) * sign(i);

                if ((sgn > 1) || (sgn < -1)){
                    return T("NaN");
                } else if ((j.isInf()) || (j.isMinusInf()) || (i.isInf()) || (i.isMinusInf())){
                    if (sgn == -1) {
                        return T("-Inf");
                    } else if (sgn == 1) {
                        return T("Inf");
                    }
                } else {
                    return T(j.getVal() * i.getVal());
                }

                return T(0);
            }

            //for your sanity just dont read the division
            // NO LITERALLY ITS EXTREMELY CRAZY
            template <class T1, class T2>
            T2 div(T1& j, T1& i){
                //this is getting a bit out of hand
                return div<T2>(T2(j.__repr__()), T2(i.__repr__()));
            } 

            template <class T1, class T2>
            T2 div(T1& j, T2& i){
                // no i am not ok
                return div<T2>(T2(j.__repr__()), i);
            }

            template <class T2>
            T2 div(T2& j, T2& i){

            }

        public:
            bool isNaN() {
                return NaN;
            }

            bool isInf() {
                return Inf;
            }

            bool isMinusInf() {
                return MinusInf;
            }
    };

    class Integer : public Number{
        private:
            int val = 0;

            bool strToInt(std::string str){
                bool isInt = true;
                int i = 0;

                for (char e : str){
                    if (i == 0){
                        if (!( ( e == '-' ) || ( isdigit(e) ) )){
                            isInt = false;
                            break;
                        }
                    } else if (!isdigit(e)){
                        isInt = false;
                        break;
                    }
                }

                if (isInt) {
                    val = std::stoi(str);
                    return true;

                } else {
                    return false;
                }
            }

            void setFlags(bool inf = false, bool minusinf = false, bool nan = false){
                Inf = inf;
                MinusInf = minusinf;
                NaN = nan;
            }

        public:
            int getVal() {
                return val;
            }

            void setVal(int val){
                val = val;
                setFlags();
            }
            
            void setVal(std::string str) {
                if (str == "NaN"){
                    setFlags(false, false, true);
                    val = 0;
                } else if (str == "Inf"){
                    setFlags(true, false, false);
                    val = 0;
                } else if (str == "-Inf"){
                    setFlags(false, true, false);
                    val = 0;
                } else {
                    if(strToInt(str)){
                        setFlags();
                    } else {
                        val = 0;
                        setFlags(false, false, true);
                    }
                }
            }

            Integer(int i){
                val = i;
            }

            Integer(std::string str){
                if (str == "NaN"){
                    NaN = true;
                } else if (str == "Inf"){
                    Inf = true;
                } else if (str == "-Inf"){
                    MinusInf = true;
                } else {
                    if(!strToInt(str)){
                        NaN = true;
                    }
                }

            }

            std::string __repr__(){
                if (NaN) {
                    return "NaN";
                } else if (Inf) {
                    return "Inf";
                } else if (MinusInf) {
                    return "-Inf";
                } else {
                    return std::to_string(val);
                }
            }

            // bool isNum(){
            //     return !NaN;
            // }

            Integer operator+ (Integer& i){
                return add(*this, i);
            }

            Integer operator- (Integer& i){
                Integer new_i = Integer(0);
                if ( i.isNaN() || NaN ){
                    return Integer("NaN");
                } else {
                    new_i = changeSign(i);
                }

                return add(*this, new_i);
            }

            Integer operator* (Integer& i){
                return mlt(*this, i);
            }

            Integer operator/ (Integer& i){
                return Integer(0);
            }
    };
    class Natural{};
    class Float{};
    class String{};
};

#endif

#ifndef PSE_TOKENS
#define PSE_TOKENS

template <class DT>
class Literal : public Token {
    
};

class Separator : public Token {

};

class Identifier : public Token {

};

template <class DT1, class DT2>
class Operator : public Token {
    
};

class Indentation : public Token {

};

class Parantheses : public Token {
    class ParanthesesStart{};
    class ParanthesesEnd{};
};

class Keyword : public Token {

};

class Subscript : public Token {

};

namespace pseutils{
    Token wrapToken(Token T){
        return Token();
    }
};

#endif