#include <string>
#include <vector>

#ifndef PSE_TOKENS_BASIC
#define PSE_TOKENS_BASIC

class __Token{
    
};

template <class T>
class TokenHolder{
    public:
        TokenHolder(){}

        TokenHolder(TokenHolder& TokenH) {

        }

        std::vector<T> internalArray;

        T getTokenAt(int pos){
            if (pos >= counter) {
                return internalArray[pos - 1];
            }
            return internalArray[pos];
        }

        std::string getTypeOfTokenAt(int pos){
            return "";
        }
    
        int pushBack(T& Tk) {
            internalArray.reserve(counter + 1);
            internalArray.push_back(Tk);
            counter += 1;
            return (counter - 1);
        }

    private:
        int counter = 0;
};

class Token {
    protected:
        std::string __repr__ = "";
        int line;
        int tokenHolderPos;
        //int firstCharPos;
    public:
        static TokenHolder<Token> globalTokenHolder;

        Token(){
            //tokenHolderPos = globalTokenHolder.pushBack(*this);
        }
        Token(int line){
            line = line;
            //tokenHolderPos = globalTokenHolder.pushBack(*this);
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

TokenHolder<Token> Token::globalTokenHolder = TokenHolder<Token>();

#endif

#ifndef PSE_TOKENS
#define PSE_TOKENS

class DType : public Token {

};

#endif