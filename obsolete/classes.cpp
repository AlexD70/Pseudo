#include <string>

enum TokenType{
    WHITESPACE,             //0
    IDENTIFIER,             //1
    INTEGER,                //2 
    STRING,                 //3   
    OPERATOR,               //4 
    SEPARATOR,              //5
    PARANTHESES,            //6 
    INDENTATION,            //7  
    VALUE_ASSIGNEMENT,      //8
    KEYWORD,                //9
    OPERATOR__,             //10; special wrapper for -
    SUBSCRIPT,              //11
    DEFAULT                 //12
};

class Token{
    public:
        Token(){}

        char* firstChr;
        int size = 0; //size = real_size - 1
        enum TokenType tokenType;
        std::string self = "";

        void buildString(){
            for (int i = 0; i <= size; i++){
			    self.push_back(*(firstChr + i));
			}
        }

		int __repr__(){
			return tokenType;
		}
};

/*
tab: 4spaces \n4space
string
nr
+ - / % *
< > = NOT NOT= SI SAU <= >=
, () "" ''
<-
//
/* */


//reminder noi n am fc nimic pt numerele negative, dar deocamdata pastram doar nr naturale 