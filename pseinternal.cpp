#include <string>
#include <vector>

#ifndef PSE_TOKENS_BASIC
#define PSE_TOKENS_BASIC

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

        void setReprOnce(std::string str) {
            if (__repr__ != ""){
                return;
            } else {
                __repr__ = str;
            }
        }    

        void setReprOnce(std::vector<char> str){
            if (__repr__ != ""){
                return;
            } else {
                for (char e : str){
                    __repr__ += e;
                }
            }   
        }
};

#endif

#ifndef PSE_TOKENS
#define PSE_TOKENS

class DType : public Token {

};

#endif