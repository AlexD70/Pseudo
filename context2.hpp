#ifndef PSE_INTERNAL_VECTOR
#define PSE_INTERNAL_VECTOR
#include <cstddef>
#include <vector>
#endif

#include "token.hpp"

#ifndef PSE_CTX
#define PSE_CTX

enum KEYWORDS {
    _INT = 0,
    _DOUBLE = 1,
    _STRING = 2,
    _BOOL = 3,
    _IF = 4,
    _ELSE = 5,
    _FOR = 6,
    _WHILE = 7,
    _DO = 8,
    __WHILE = 9,
    _KEYWORD_NOT_FOUND = -1
};

enum FNS {
    _PRINT = 0,
    _INPUT = 1,
    _CAST_INT = 2,
    _CAST_DOUBLE = 3,
    _CAST_BOOL = 4,
    _FN_NOT_FOUND = -1
};

namespace ctxflags {
    bool indentedBlockExpected = false;
    Token* lastIndentation;
};

struct Expect {
    int what = 0;
    /* 
     -1 means expression
     0 means keyword (default)
     1 means token type
    */
    DTYPE retval = DTYPE::VOID_PTR; 
    KEYWORDS keyword = KEYWORDS::_KEYWORD_NOT_FOUND;
    TYPE type = TYPE::DEFAULT;
};
struct Symbol {
    std::string name;
    DTYPE dtype = VOID_PTR;
    int isInputDependent = 0;
    int intval = 0;
    bool boolval = false;
    double dblval = 0;
    std::string strval = "";
    std::string expr = "";
};
class SymbolValidator {
    public:
	std::vector<Symbol> allSymbolsVector = std::vector<Symbol>();
        SymbolValidator(){} 
        void newSymbol(const char * name) {
            if (getSymbolByName(name)){
	        return; //TODO raise symbol has already been declared once
	    }
	    Symbol sym;
	    sym.name = name;
	    allSymbolsVector.reserve(allSymbolsVector.size() + 1);
	    allSymbolsVector.push_back(sym);
	}

        Symbol * getSymbolByName(const char * name) noexcept {
	    for (std::size_t i = 0; i < allSymbolsVector.size(); i ++){
	        if (allSymbolsVector[i].name.c_str() == name){
		    return & allSymbolsVector[i];
		}
	    }
	    return nullptr; 
	}

        void assignValue(Symbol * symptr, const char * val){
	    if (symptr->dtype != DTYPE::STRING){
	        return; //TODO raise type error
	    }
	    symptr->strval = val;	
	}
        void assignValue(Symbol * symptr, int val){
	    if (symptr->dtype != DTYPE::INT){
	        return; //TODO raise type error
	    }
	    symptr->intval = val;
	}
        void assignValue(Symbol * symptr, double val){
	    if (symptr->dtype != DTYPE::DOUBLE){
	        return; //TODO raise type error
	    }
	    symptr->dblval = val;
	}
        /*void assignValue(Symbol * symptr, bool val){
	    if (symptr->dtype != DTYPE::BOOL){ //TODO bool doesnt even exist in DTYPE
	        return; //TODO raise type error
	    }
	    symptr->boolval = val;
	}*/
        void markInputDependent(Symbol * symptr, bool isInput = false) noexcept {
	    symptr->isInputDependent = (isInput)?(2):(1);
	}
	void assignExpr(Symbol * symptr, const char * _expr) {
	    markInputDependent(symptr);
	    symptr->expr = _expr;
	}
};
struct ExpressionTerm {
    int stuckInHighOrderOperation = 0; //order of operation; 0 is false; 4+ is parantheses
    Token * literalptr;
    Symbol * symptr; 
    OPERATION op;
    int casttype;
    /*
     0 - int
     1 - double
    */
    int termtype = 0;
    /*
     0 - literal
     1 - symbol
     2 - operation
     3 - parantheses start
     4 - parantheses end
     5 - cast
    */
};
struct Retval_selfEval {
    int errcode = 0;
    std::string expression = "";
    Token result;
    std::vector<ExpressionTerm>::iterator iter;
    DTYPE dtype = DTYPE::VOID_PTR;
};
class Expression{
    private:
	Retval_selfEval calcFirstOrderOperations(std::vector<ExpressionTerm>::iterator begin) {
	    std::vector<ExpressionTerm> terms = std::vector<ExpressionTerm>();
	    DTYPE rettype = DTYPE::VOID_PTR;
	    double dblval = 0;
	    std::string strval = "";
	    bool boolval = false;
	    bool sign = false; // false is +

	    ExpressionTerm t = * begin;
	    while (t.termtype < 2 || t.op == OPERATION::ADD || t.op == OPERATION::SUB) {
	        if (t.termtype == 0){
		    switch (t.literalptr->dtype){
		        case DTYPE::INT:
			    if (rettype == DTYPE::STRING) {
			        Retval_selfEval errEval;
				errEval.errcode = -1; //TODO type error
				errEval.iter = std::vector<ExpressionTerm>::iterator(begin);
				return errEval;
			    }
			    dblval += (sign)?(-1):(1) * t.literalptr->intval;
			    break;
			case DTYPE::DOUBLE:
			    if (rettype == DTYPE::STRING) {
			        Retval_selfEval errEval;
				errEval.errcode = -1; //TODO type error
				errEval.iter = std::vector<ExpressionTerm>::iterator(begin);
				return errEval;
			    }
			    rettype = DTYPE::DOUBLE;
			    dblval += (sign)?(-1):(1) * t.literalptr->dblval;
			    break;
			case DTYPE::STRING:
			    if (rettype != DTYPE::VOID_PTR && rettype != DTYPE::STRING){
			        Retval_selfEval errEval;
				errEval.errcode = -1; //TODO type error
				errEval.iter = std::vector<ExpressionTerm>::iterator(begin);
				return errEval;
			    }
			    if (sign) {/*TODO raise some error*/}
			    rettype = DTYPE::STRING;
			    strval += t.literalptr->strval;
			    break;
		        default:
			    break;
		    }
		}
	    }     
	}
    public:
        bool isConstExpr = false;
	std::vector<ExpressionTerm> termVector = std::vector<ExpressionTerm>();

	void appendSymbol(Symbol * _symptr) noexcept {
	    ExpressionTerm symTerm;
	    symTerm.symptr = _symptr;
	    symTerm.termtype = 1;

	    termVector.reserve(termVector.size() + 1);
	    termVector.push_back(symTerm);
	}

	void appendLiteral(Token * _literalptr) noexcept {
	    ExpressionTerm literalTerm;
	    literalTerm.literalptr = _literalptr;
	    literalTerm.termtype = 0;

	    termVector.reserve(termVector.size() + 1);
	    termVector.push_back(literalTerm);
	}

	void appendOperation(OPERATION _op) noexcept {
	    ExpressionTerm opTerm;
	    opTerm.op = _op;
	    opTerm.termtype = 2;

	    termVector.reserve(termVector.size() + 1);
	    termVector.push_back(opTerm);
	}
	void appendParantheses(bool isEnd = false) noexcept {
	    ExpressionTerm paranthesesTerm;
	    paranthesesTerm.termtype = 3 + (int) isEnd;

	    termVector.reserve(termVector.size() + 1);
	    termVector.push_back(paranthesesTerm);
	}
	void appendCast(Token * _castptr) noexcept {
	    ExpressionTerm castTerm;
	    castTerm
	}

	std::string selfEval() {
            	
	}
};
struct Retval_matchExpression {
    Expression expr;
    int tokensInExpr = 0;
    DTYPE retval = DTYPE::VOID_PTR;
};

class CompositeExpect {
    public:
        std::vector<Expect> expectVector = std::vector<Expect>();
        int index = 0;
};

class ContextValidator {
    private:
	bool validateIndentation(Token* _token, Token* _last, bool isBlockExpected) const noexcept{
    	    if (isBlockExpected) {
	        if (_last->intval == _token->intval + 1) {
	            return true;
	        } else {
	            return false;
	        }
	    } else if ((_last->intval > _token->intval) || (_token->intval == 0)){
	        return true;
	    } else {
	        return false;
            }
        }
    
	KEYWORDS findKeyword(Token* _token) const noexcept {
	    if (_token->type == TYPE::DEFAULT){
		std::string str = std::string(_token->repr);
		if (str == "intreg"){
	            return KEYWORDS::_INT;
		} else if (str == "real"){
		    return KEYWORDS::_DOUBLE;
		} else if (str == "cattimp"){
		    return KEYWORDS::_WHILE;
		} else if (str == "executa"){
		    return KEYWORDS::_DO;
		} else if (str == "panacand"){
		    return KEYWORDS::__WHILE;
		} else if (str == "bool"){
		    return KEYWORDS::_BOOL;
		} else if (str == "string"){
		    return KEYWORDS::_STRING;
		} else if (str == "daca"){
		    return KEYWORDS::_IF;
		} else if (str == "altfel"){
		    return KEYWORDS::_ELSE;
		} else if (str == "pentru"){
		    return KEYWORDS::_FOR;
		} else {
		    return KEYWORDS::_KEYWORD_NOT_FOUND;
		}
	    } else {
	        return KEYWORDS::_KEYWORD_NOT_FOUND;
	    }
	}

	FNS findBuiltInFunction(Token* _token) const noexcept{
            if (_token->type == TYPE::DEFAULT) {
		std::string str = std::string(_token->repr);
	        if (str == "scrie"){
		    return FNS::_PRINT;
		} else if (str == "citeste"){
		    return FNS::_INPUT;
		} else if (str == "intreg"){
		    return FNS::_CAST_INT;
		} else if (str == "real"){
		    return FNS::_CAST_DOUBLE;
		} else if (str == "bool"){
		    return FNS::_CAST_BOOL;
		} else {
		    return FNS::_FN_NOT_FOUND;	
		} 
	    } else {
	        return FNS::_FN_NOT_FOUND; 
	    }	
	}
};

#endif
