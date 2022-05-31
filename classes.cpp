#include <string>

enum TokenType{
    WHITESPACE,             //lasa-l ca poate ne ajuta mai trz
    IDENTIFIER,             //asta va fi string initial
    INTEGER,                //0 1 2 3 4 5 6 7 8 9 
    STRING,                 //kdjgfkd   
    OPERATOR,               //+ - / % * = < > <= >= 
    SEPARATOR,              //,
    PARANTHESES,            // ()
    INDENTATION,            // \n tab (\n 4 spaces) 
    VALUE_ASSIGNEMENT,      //<-
    KEYWORD,
    OPERATOR__,             //special wrapping for -
    SUBSCRIPT,              // []
    DEFAULT
};

class Token{
    public:
        char* ptr;
        int size = 0; //size = real_size - 1
        enum TokenType tokenType;
        std::string self = "";

        void buildString(){
            for (int i = 0; i <= size; i++){
			    self.push_back(*(ptr + i));
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