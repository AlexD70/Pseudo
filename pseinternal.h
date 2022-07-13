#ifndef PSE_INTERNAL_STR
#define PSE_INTERNAL_STR
#include <string>
#endif

#ifndef PSE_INTERNAL_VECTOR
#define PSE_INTERNAL_VECTOR
#include <vector>
#endif

#ifndef PSE_INTERNAL_TYPES
#define PSE_INTERNAL_TYPES
#include <typeinfo>
#endif

//error libs
#include "internalerrlib.h"

#ifndef PSE_TOKENS_BASIC
#define PSE_TOKENS_BASIC

/*
 Token base class
 all token types are derived from this
*/
class Token {
    protected:
        std::string repr = "";
        int line;
        int wrapperFlag = 0;
        /* wrapperFlag helps the token wrapper find the right type of token
        to wrap this into and is set at parsing time

        1 - arithmetic operator
        2 - logical operator
        3 - attribution operator
        4 - relational operator
        5 - string literal
        6 - int literal
        7 - float literal
        8 - bool literal (boolalpha)
        */

    public:
        Token(){} //internal use only
        Token(int line){ //internal use only
            line = line;
        }
        Token(int line, std::string repr){ //might delete later
            line = line;
            repr = repr;
        }
        Token(int line, const char* _repr){ //declaration w c-style strings, might delete this one
            line = line;
            repr = std::string(_repr);
        }
        Token(int line, const char* _repr, int flag){ //declaration w c-style strings
            line = line;
            repr = std::string(_repr);
            wrapperFlag = flag;
        }
        Token(int line, std::string repr, int flag){
            line = line;
            repr = repr;
            wrapperFlag = flag;
        }
        
        std::string __repr__(){ //get string representation
            return repr;
        }
};

#endif

#ifndef PSE_DTYPES
#define PSE_DTYPES

namespace dtypes{ // data types
    //TODO add booleans

    /*
    Number class, base for all numeric data types
    internal use only! do not declare Number objects!!!

    note: this has to implement type casts rather than what its doing rn
    note 2: this has to be worked on a bit after we write Natural
    note 3: maybe remove the operator overloads from Integer and Float????
    */
    class Number{
        protected:
            //flags for all numeric dtypes
            bool NaN = false;
            bool Inf = false;
            bool MinusInf = false;

            //flag setter function
            void setFlags(bool inf = false, bool minusinf = false, bool nan = false){
                Inf = inf;
                MinusInf = minusinf;
                NaN = nan;
            }

            //checks if a numeric dtype instance is zero
            template <class T>
            bool isZero(T number){
                return !(number.isNaN() || number.isInf() || number.isMinusInf() || number.getVal());
            }

            //add two numbers of the same type and return a member of the same class
            template <class T>
            T add(T j, T i){
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

            //get sign of a numeric dtype instance
            //returns 2 for NaN, 1 for +, -1 for - and 0 for 0
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

            //change sign of a numeric dtype instance
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

            //multiply two objects of the same numeric dtype and return another instance of the same class
            template <class T>
            T mlt(T j, T i){
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

            //*** template <class T2>  //unused but keep it for now
            // T2 revertFraction(T2 number){
            //     if(isZero(number)){
            //         return T2("NaN");
            //     } else {
            //         std::cout << "reverted: " << 1/number.getVal() << "\nresult: ";
            //         return T2(1.0 / (float) number.getVal());
            //     }
            // }

            //divide two objects of the same numeric dtype and return another instance of the same class
            template <class T2>
            T2 div(T2 j, T2 i){
                int sgn = sign(j) * sign(i);

                if ((sgn > 1) || (sgn < -1)){
                    return T2("NaN");
                } else if ((j.isInf()) || (j.isMinusInf()) || (i.isInf()) || (i.isMinusInf())){
                    if (sgn == -1) {
                        return T2("-Inf");
                    } else if (sgn == 1) {
                        return T2("Inf");
                    }
                } else {
                    return T2(j.getVal() / i.getVal());
                }

                return T2(0);
            }

            //divide two objects of the same numeric dtype and return an instance of another class
            //intended usage example:
            //div<Float>(Integer(2), Integer(9));
            template <class T1, class T2>
            T2 div(T1 j, T1 i){
                T2 a = T2(j.__repr__()), b = T2(i.__repr__());
                return div<T2>(a, b);
            } 

            //divide two different class dtype instances and return an instnace of the second objects class
            //i didnt actually test this one
            //but its intended usage is (this example should return a dtypes::Float):
            //div(Integer(8), Float(1.2));
            //it might work like this too, but as i said i didnt test:
            //div(Float(7.9), Integer(3));  //returns dtypes::Integer
            template <class T1, class T2>
            T2 div(T1 j, T2 i){
                // no i am not ok
                return div<T2>(T2(j.__repr__()), i);
            }

            
        public:
            //get functions

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

    //this is a TODO class
    class Natural{};

    /*
    Float class
    float numbers, maybe implement some very big number wrapper
    */
    class Float : public Number{
        private:
            float val = 0;

            bool strToFloat(std::string str){
                bool isFloat = true;
                int i = 0, __i = 0;
                bool periodFound = false;
                bool fractionLineFound = false;

                for (char e : str){
                    if (isdigit(e)){
                        i++;
                        continue;
                    } else if (e == '.' && !periodFound && !fractionLineFound){
                        i++;
                        periodFound = true;
                        continue;
                    } else if (e == '/' && !fractionLineFound && !periodFound){
                        if (i == 0){
                            isFloat = false;
                            break;
                        }
                        i++;
                        __i = i;
                        fractionLineFound = true;
                        continue;
                    } else {
                        isFloat = false;
                        break;
                    }
                }

                if (isFloat) {
                    if (!fractionLineFound){
                        val = std::stof(str);
                    } else {
                        float temp = std::stof(str.substr(__i));
                        if (temp == 0){
                            return false;
                        }
                        val = std::stof(str.substr(0, __i - 1)) / temp;
                    }
                    return true;
                } else {
                    return false;
                }
            }

        public:
            //get function
            float getVal() {
                return val;
            }

            Float(float _val){
                val = _val;
            }

            // Float(int val){  // i dont remember why i did this but lets leave it as-is for now
            //     val = (float) val;
            // }

            Float(std::string str){
                if (str == "NaN"){
                    NaN = true;
                } else if (str == "Inf"){
                    Inf = true;
                } else if (str == "-Inf"){
                    MinusInf = true;
                } else if(!strToFloat(str)){
                    NaN = true;
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
    };

    /*
    Integer class; id like a big nr wrapper here too
    */
    class Integer : public Number{
        private:
            int val = 0;

            //this returns whether a string is an integer or not
            bool strToInt(std::string str){
                bool isInt = true;
                bool i = true;

                for (char e : str){
                    if (i){
                        if (!( ( e == '-' ) || ( isdigit(e) ) )){
                            isInt = false;
                            i = false;
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
                } else if(!strToInt(str)){
                        NaN = true;
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

            //operator overloads -- im not sure how much we will actually use them

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
                return Integer((int) div<Integer, Float>(*this, i).getVal() );
            }

            Integer operator% (Integer& i){
                int val = (*this).getVal(), div = i.getVal();
                bool isNegative = false;

                if (val < 0){
                    isNegative = true;
                    val = -val;
                }
                while (val >= div){
                    val -= div;
                }
                if (isNegative){
                    val = -val;
                }

                return Integer(val);
            }
    };

    /*
    String class
    TODO we hv to add more functions here as we will need them
    */
    class String {
        protected:
            std::string repr = "";

        public:
            String(){}
            String(std::string str){
                repr = str;
            }
            String(char const* str){ //c-style string constructor
                repr = std::string(str);
            }
            //maybe add a copy object constructor here and on the classes before

            std::string __repr__(){
                return repr;
            }

            //returns a c-style string from repr
            const char* toCStr(){
                return repr.c_str();
            }

            String toUpper(){
                std::string __new_repr__ = "";
                for (char c : repr){
                    __new_repr__ = __new_repr__ + (char) std::toupper(c);
                }

                repr = __new_repr__;
                return *this;
            }
            
            //static overload of toUpper()
            //note that i mean static as in java-static
            //cpp static is smth else, dont use it!
            String toUpper(std::string str){
                std::string __new_str__ = "";
                for (char c : str){
                    __new_str__ = __new_str__ + (char) std::toupper(c);
                }

                return String(__new_str__);
            }
    
            String toLower(){
                std::string __new_repr__ = "";
                for (char c : repr){
                    __new_repr__ = __new_repr__ + (char) std::tolower(c);
                }

                repr = __new_repr__;
                return *this;
            }

            //TODO: some refactorings have to be done
            std::string getVal(){
                return __repr__();
            }

            //static overload of toLower()
            String toLower(std::string str){
                std::string __new_str__ = "";
                for (char c : str){
                    __new_str__ = __new_str__ + (char) std::tolower(c);
                }

                return String(__new_str__);
            }
    
            Integer length(){
                return Integer(repr.size());
            }
    
            //returns repr's length as an int, unlike length() which returns a dtypes::Integer
            int __len__(){
                return repr.size();
            }
    };
};

#endif

#ifndef PSE_TOKENS
#define PSE_TOKENS

/*
 Indentation class
 this has a lot of constructors of which some may be useless
 i also feel like it misses smth
*/
class Indentation : public Token {
    public:
    int depth = 0;
    
    Indentation(int line, int depth) : Token(line){
        depth = depth;
    }
    Indentation(int line, int depth, std::string repr) : Token(line, repr){
        depth = depth;
    }
    Indentation(int line, int depth, const char* repr) : Token(line, repr){
        depth = depth;
    }
    Indentation(int line) : Token(line){}
    Indentation(int line, std::string repr) : Token(line, repr){}
    Indentation(int line, const char* repr) : Token(line, repr){}
};

//TODO
class Identifier : public Token {};

//TODO
template <class DT1, class DT2>
class Operator : public Token {};

//check for improvements
//maybe remove encapsulation
class Parantheses : public Token {
    private:
        std::vector<Token> tokenVector = std::vector<Token>();
        int stackDepth;

    public:
        Parantheses(int line, int stackDepth) : Token(line){
            stackDepth = stackDepth;
        }

        std::vector<Token> dumpAll() {
            return std::vector<Token>(tokenVector);
        }

        void ressignTokenVector(std::vector<Token> &newVector){
            tokenVector = newVector;
            return;
        }

        void appendToVector(Token &what){
            tokenVector.reserve(tokenVector.size() + 1);
            tokenVector.push_back(what);
            return;
        }

        int getStackDepth(){
            return stackDepth;
        }
};

//TODO
class Keyword : public Token {};

/*
Literal class
used for String, Integer, Float and Bool literals
we need to work on this one a little more
*/
template <class T, typename P> // T is supposed to be from dtypes namespace
class Literal : public Token {
    protected:
        P repr;
        T self;

    public:
        Literal(T self, int line) : Token(line){
            self = self;
            repr = self.getVal();
        }

        P __repr__(){
            return repr;
        }

        T __self__(){
            return self;
        }
};

//actually i think we dont need this overload at all
// template <class T, class P> //overload to allow P to be std::string
// class Literal : public Token {
//     protected:
//         P repr;

//     public:
//         Literal(T self, int line) : Token(line){
//             self = self;
//             repr = self.getVal();
//         }

//         P __repr__(){
//             return repr;
//         }

//         T __self__(){
//             return self;
//         }
// };

//maybe improve the logic of how the parser handles subscripts and parantheses idk
//cuz if we keep this logic theres a chance we will have a hard time at context validating and debugging
class Subscript : public Token {
    protected:
        //this has an internal logic similar to the parser itself
        //TODO maybe add an expression validator built in for this, or maybe use an external one
        std::vector<Token> tokenVector = std::vector<Token>();
        struct paranthesesStackNode {
            Parantheses *self = nullptr;
            paranthesesStackNode *next = nullptr;
            paranthesesStackNode *back = nullptr;
        };

        paranthesesStackNode node1 = paranthesesStackNode();
        paranthesesStackNode *actualNode = &node1;

        Subscript *subscriptPtr = nullptr;
        bool inSubscript = false;
        bool inParantheses = false;
        bool subscriptInParantheses = false;

    public:
        Subscript(int line) : Token(line){}

        void appendToken(Token what){
            if (inSubscript){
                if (subscriptPtr){
                    (*subscriptPtr).appendToken(what);
                } else { 
                    InternalError err = InternalError("pseinternal.h, in class Subscript, in function appendToken");
                    pseutils::raise(err);
                }
            } else if (inParantheses){
                if (actualNode){
                    (*(*actualNode).self).appendToVector(what);
                } else {
                    InternalError err = InternalError("pseinternal.h, in class Subscript, in function appendToken");
                    pseutils::raise(err);
                }
            } else {
                tokenVector.reserve(tokenVector.size() + 1);
                tokenVector.push_back(what);
            }
            
            return;
        }


        void appendParantheses(Parantheses what){
            //this trusts the caller function took care of all relevant flags

            if (!inParantheses){
                node1.self = &what;
                appendToken(what);
                inParantheses = true;
            }  else {
                (*(*actualNode).self).appendToVector(what);
                paranthesesStackNode node;
                paranthesesStackNode *lastNode;
                node.self = &what;
                (*actualNode).next = &node;
                lastNode = actualNode;
                actualNode = &node;
                (*actualNode).back = lastNode;
            }

            if (inSubscript){
                (*subscriptPtr).appendParantheses(what);
            }
        }

        void appendSubscript(Subscript what){
            if (inParantheses){
                inParantheses = false;
                subscriptInParantheses = true;
                inSubscript = true;

                (*(*actualNode).self).appendToVector(what);
            } else if (inSubscript){
                (*subscriptPtr).appendSubscript(what);
            } else {
                appendToken(what);
            }
        }

        void exitParantheses(){
            if (!inParantheses){
                BadParanthesesNesting err = BadParanthesesNesting(pflags::line);
                pseutils::raise(err);
            }
            if ((*actualNode).back){
                actualNode = (*actualNode).back;
            } else {
                inParantheses = false;
            }
        }

        bool exitSubscript(){
            if (inSubscript){
                inSubscript = !(*subscriptPtr).exitSubscript();
                if ((!inSubscript) && (subscriptInParantheses)){
                    inParantheses = true;
                }
                return false;
            } else {
                exitThis();
                return true;
            }
            
        }

        void exitThis(){
            if (inParantheses){
                BadParanthesesNesting err = BadParanthesesNesting(pflags::line);
                pseutils::raise(err);

            } else if (inSubscript){
                BadSubscriptUsage err = BadSubscriptUsage(pflags::line);
                pseutils::raise(err);
            } else {
                return; //dont call ~Subscript() yet
            }
        }

        std::vector<Token> dumpAll(){
            return std::vector<Token>(tokenVector);
        }
};

namespace pseutils{
    //TODO
    //main purpose is to wrap a Token into one of its subclasses and call validator functions
    //for some of them (example: Keywords)
    Token wrapToken(Token T){
        return Token(1);
    }
};

#endif