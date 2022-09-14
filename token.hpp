#ifndef PSE_INTERNAL_STRING
#define PSE_INTERNAL_STRING
#include <string>
#include <cstring>
#endif

#ifndef PSE_TOKEN
#define PSE_TOKEN

enum DTYPE {
    INT = 0,
    STRING = 1,
    DOUBLE = 2, 
    VOID_PTR = 3
};
enum TYPE {
    INDENTATION = 0,
    IDENTIFIER = 1,
    KEYWORD = 2,
    LITERAL = 3,
    OPERATOR = 4,
    PARANTHESES_START = 5,
    PARANTHESES_END = 6,
    /* SUBSCRIPT_START = 7,
    SUBSCRIPT_END = 8, */
    LINE_END = 9,
    COMMA = 10,
    DEFAULT = -1
};
enum OPERATION {
    ADD = 1,
    SUB = 2,
    MLT = 3,
    DIV = 4,
    MOD = 5,
    POW = 6,
    EQ = 7,
    NOTEQ = 8,
    GT = 9,
    GTEQ = 10,
    LT = 11,
    LTEQ = 12,
    NOT = 13,
    AND = 14,
    OR = 15,
    ATTR = 16,
    NONE = -1        
};

class Token {
    protected:
        const int UNSET = -100;
    
    public:    
        std::string repr;
        std::size_t line, col = UNSET, len;
        long long int intval = 0;
        double dblval = 0;
        std::string strval = "";
        bool ignore = false;
        
        TYPE type = TYPE::DEFAULT;
        OPERATION op = OPERATION::NONE;
        DTYPE dtype = DTYPE::VOID_PTR;

    
        Token(){}
        Token(const Token & _token) noexcept { // regular copy constructor
            repr = _token.repr;
            line = _token.line;
            col = _token.col;
            len = _token.len;
            type = _token.type;
            op = _token.op;
            intval = _token.intval;
            strval = _token.strval;
            dblval = _token.dblval;
            dtype = _token.dtype;
        }
        Token(
            const char * _repr, std::size_t _line, std::size_t _col, TYPE _type = TYPE::DEFAULT,
            OPERATION _op = OPERATION::NONE, DTYPE _dtype = DTYPE::VOID_PTR, long long int _val = 0, int _len = -100
        ) noexcept {
            line = _line;
            col = _col;
            type = _type;
            op = _op;
            dtype = _dtype;
            intval = _val;
            repr = _repr;
            len = strlen(_repr);
            if (_len >= 0){
                len = _len;
            }
        }
        Token(
            const char * _repr, std::size_t _line, std::size_t _col, TYPE _type = TYPE::DEFAULT,
            OPERATION _op = OPERATION::NONE, DTYPE _dtype = DTYPE::VOID_PTR, double _val = 0, int _len = -100
        ) noexcept {
            line = _line;
            col = _col;
            type = _type;
            op = _op;
            dtype = _dtype;
            dblval = _val;
            repr = _repr;
            len = strlen(_repr);
            if (_len >= 0){
                len = _len;
            }
        }
        Token(
            const char * _repr, std::size_t _line, std::size_t _col, TYPE _type = TYPE::DEFAULT,
            OPERATION _op = OPERATION::NONE, DTYPE _dtype = DTYPE::VOID_PTR, const char * _val = "", int _len = -100
        ) noexcept {
            line = _line;
            col = _col;
            type = _type;
            op = _op;
            dtype = _dtype;
            strval = _val;
            repr = _repr;
            len = strlen(_repr);
            if (_len >= 0){
                len = _len;
            }
        }
        Token(
            const char * _repr, std::size_t _line, std::size_t _col, TYPE _type = TYPE::DEFAULT, bool noConfusion = false
        ) noexcept {
            line = _line;
            col = _col;
            type = _type;
            repr = _repr;
            len = strlen(_repr);
        }
         Token(
            const char * _repr, std::size_t _line, std::size_t _col, TYPE _type = TYPE::DEFAULT, OPERATION _op = OPERATION::NONE, bool noConfusion = false
        ) noexcept {
            line = _line;
            col = _col;
            type = _type;
            repr = _repr;
            len = strlen(_repr);
            op = _op;
        }   

        void ignoreThis(){
            ignore = true;
        }

        bool operator==(Token & _other) noexcept {
            return ((type == _other.type) && (op == _other.op) && (dtype == _other.dtype));
        }

        bool perfectMatch(Token * _other) noexcept {
            return ((line == (* _other).line) && (col == (* _other).col) && (repr == (* _other).repr));
        }
};

#endif