#include "parser2.hpp"
//! THIS WHOLE SHIT WILL SEGFAULT SO BADLY CUZ IT BECAME POINTER HELL


enum CTYPE {
    _int = 0,
    _bool = 1,
    _str = 2,
    _dbl = 3
};

class Expect {
    public:
        bool expr;
        TYPE type;

        bool compareWith(Token * _token){
            return false;
        }

        int compareExprWith(std::vector<Token>::iterator _iter){
            return 0;
        }
};

class Expression {
    public:
        CTYPE evalTo;
        std::vector<Token>::iterator iter;
        int i;
        size_t max_i = 0;

        Expression(std::vector<Token>::iterator _iter, size_t _max_i){
            iter = _iter;
            max_i = _max_i;
        }

        void deductEvalAndLength(){

        }
};

namespace ctxflags {
    std::size_t line = 1, col = 0;
    int scopeDepth = 0, paranthesesDepth = 0;
    bool indentedBlockExpected() {
        return (expectedTokenType == TYPE::INDENTATION);
    }
    TYPE expectedTokenType = TYPE::INDENTATION;
    Token * lastIndentation = nullptr;
    void reset () {
        line = 1;
        col = 0;
        scopeDepth = 0;
        paranthesesDepth = 0;
        expectedTokenType = TYPE::INDENTATION;
        lastIndentation = nullptr;
    }
};

class Symbol {
    public:
        Symbol(){}
        bool operator==(Symbol & _other){
            return true;
        }
        bool operator==(Token & _other){
            return true;
        }
};

class SymbolManager {
    public:
        // you can also just access this directly
        std::vector<Symbol> symbolVector;

        SymbolManager(){}
        Symbol * findMatch(Symbol & _symbol) noexcept {
            for (int i = 0; i < symbolVector.size(); i ++){
                if (symbolVector[i] == _symbol){
                    return (& symbolVector[i]);
                }
            }
            return nullptr;
        }
        Symbol * findMatch(Token * _token) noexcept {
            for (int i = 0; i < symbolVector.size(); i ++){
                if (symbolVector[i] == _token){
                    return (& symbolVector[i]);
                }
            }
            return nullptr;
        }

        void appendSymbol(Symbol & what) {
            symbolVector.reserve(symbolVector.size() + 1);
            symbolVector.push_back(what);
        } 

        Symbol * accessSymbolAt(std::size_t i) noexcept {
            if (i == -1){
                return (& symbolVector[symbolVector.size() - 1]);
            } else {
                if (i < symbolVector.size()){
                    return (& symbolVector[i]);
                } else {
                    return nullptr;
                }
            }
        }
};


class Symbol {
    public:
        Token * token;
        bool hasValue = false;
        bool isConst = true;
        bool isInput = false;
        DTYPE dtype;

        bool operator==(Token * _other) noexcept {
            return ((* token).perfectMatch(_other));
        }

        bool operator==(Symbol & _other) noexcept {
            return ((* token).perfectMatch(_other.token));
        }

        Symbol(Token * _token, DTYPE _dtype) noexcept {
            token = _token;
            dtype = _dtype;
        }
        Symbol(Token * _token, long long int intinitval) noexcept {
            token = _token;
            (* token).intval = intinitval;
            dtype = DTYPE::INT;
            hasValue = true;
        }
        Symbol(Token * _token, double dblinitval) noexcept {
            token = _token;
            (* token).dblval = dblinitval;
            dtype = DTYPE::DOUBLE;
            hasValue = true;
        }
        Symbol(Token * _token, std::string strinitval) noexcept {
            token = _token;
            (* token).strval = strinitval.c_str();
            dtype = DTYPE::STRING;
            hasValue = true;
        }
        Symbol(Token * _token, DTYPE _dtype, bool _isInput) noexcept {
            token = _token;
            dtype = _dtype;
            isInput = _isInput;
        }
        Symbol autodeductSymbolInitval(Token * _token, DTYPE _dtype, SymbolManager * _symmanager, Token * rvalue){
            token = _token;
            dtype = _dtype;
            if ((* rvalue).type == TYPE::LITERAL){
                if ((* rvalue).dtype == _dtype){
                    //! WARNING: DO NOT READ THE NEXT STATEMENT -- ITS A CHAINED TERNARY OPERATOR
                    return (
                        (_dtype == DTYPE::INT)?(Symbol(_token, (*rvalue).intval)):((_dtype == DTYPE::DOUBLE)?(Symbol(_token, (*rvalue).dblval)):(Symbol(_token, (*rvalue).strval)))
                    );
                } else {
                    //TODO raise type error
                }
            }
            Symbol * _symbol = (* _symmanager).findMatch(rvalue);

            if (_symbol){

            } else {
                //TODO raise symbol or expression expected
            }
        }
};

class ContextValidator {
    protected:
        std::vector<Token> tokenVector;
        
        bool validateIndentation(Token * lastIndentation, Token * thisIndentation, Token * nextToken) {
            if ((* nextToken).type == TYPE::LINE_END){
                (* thisIndentation).ignoreThis();
                return true;
            } else if (!lastIndentation) {
                if ((* thisIndentation).intval != 0) {
                    return false;
                } else {
                    return true;
                }
            } else {
                long long int a = (* lastIndentation).intval, b = (* thisIndentation).intval;
                return (ctxflags::indentedBlockExpected())?(a + 1 == b):(b <= a);
            }
        }

        // DOES NOT VALIDATE STRINGS SINCE THOSE WERE CAUGHT AT RUNTIME
        bool validateLiteral(Token & _token){
            try {
                int _intval = std::stoi(_token.repr);
                _token.intval = _intval;
                _token.type = TYPE::LITERAL;
                _token.dtype = DTYPE::INT;
                return true;
            } catch (std::exception e) {
                try {
                    double _dblval = std::stod(_token.repr);
                    _token.dblval = _dblval;
                    _token.type = TYPE::LITERAL;
                    _token.dtype = DTYPE::DOUBLE;
                } catch (std::exception _e) {
                    return false;
                }
            }
        }

    public:
        void copyTokenVector(PseudocodeParser & _parser){
            tokenVector = _parser.getTokenVectorCopy();
        }

        void validateContext() {
            ctxflags::reset();

            //loop
            for (std::size_t i = 0; i < tokenVector.size(); i ++){
                
                

            } // end of loop
        }
};