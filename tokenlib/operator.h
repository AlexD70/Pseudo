//this contains the Operator class derived from Token
#include "token.h"
#include "../dtypelib/none.h"

#ifndef PSE_OPERATOR
#define PSE_OPERATOR

class Operator : virtual public Token<None>{
    protected:
        int type;
        enum operations {
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
            ATTR = 100
        };

    public:
        Operator(int _type, std::string _repr, int _line) : Token(_repr, _line){
            type = _type;
        }
        Operator(int _type, int _line) : Token() {
            line = _line;
            type = _type;
            switch(_type){
                case ADD:
                    repr = "+";
                    break;
                case MLT:
                    repr = "*";
                    break;
                case SUB:
                    repr = "-";
                    break;
                case DIV:
                    repr = "/";
                    break;
                case MOD:
                    repr = "%";
                    break;
                case POW:
                    repr = "^";
                    break;
                case EQ:
                    repr = "=";
                    break;
                case NOTEQ:
                    repr = "NU=";
                    break;
                case GT:
                    repr = ">";
                    break;
                case GTEQ:
                    repr = ">=";
                    break;
                case LT:
                    repr = "<";
                    break;
                case LTEQ:
                    repr = "<=";
                    break;
                case NOT:
                    repr = "NU";
                    break;
                case AND:
                    repr = "SI";
                    break;
                case OR:
                    repr = "SAU";
                    break;
                case ATTR:
                    repr = "<-";
                    break;
                default:
                    repr = "";
                    break;
            }
        }

        int __type__() {
            return type;
        }
};

#endif