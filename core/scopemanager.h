//this is the ScopeManager class
//use it to take care of parantheses, indentation and subscript
//not that it doesnt store the tokens, only some data about them

#include <cstddef>
//fixed size integers
#include <cstdint>
#include <vector>
#include "../tokenlib/indentation.h"
#include "../tokenlib/parantheses.h"
#include "../errlib/compiletime/unexpectedtoken.h"

#ifndef PSE_CORE_SCOPEMANAGER
#define PSE_CORE_SCOPEMANAGER

class ScopeManager {
    protected:
        int groupDepth = 0, lastId = 999;
        enum types {
            INDENTATION = 0,
            PARANTHESES_START = -1,
            PARANTHESES_END = 1,
            SUBSCRIPT_START = -2,
            SUBSCRIPT_END = 2,
            PAIRED_PARANTHESES_START = -3,
            PAIRED_PARANTHESES_END = 3,
            PAIRED_SUBSCRIPT_START = -4,
            PAIRED_SUBSCRIPT_END = 4
        };

        enum token_type_counterparts {
            TK_PARANTHESES_START = 0,
            TK_PARANTHESES_END = 1,
            TK_SUBSCRIPT_START = 0,
            TK_SUBSCRIPT_END = 1
        };

        bool UNPAIRED_PARANTHESES_END = false;
        bool UNPAIRED_SUBSCRIPT_END = false;

        struct Item {
            size_t pos;
            int id;
            types type;
        };
        std::vector<Item> itemVector = std::vector<Item>();

        void appendToVector(Item &_item){
            itemVector.reserve(itemVector.size() + 1);
            itemVector.push_back(_item);
        }

        int newId() {
            lastId += 1;
            return lastId;
        }

        int matchId(types _type) {
            //some crazy reverse_iterator logic
            //@see https://m.cplusplus.com/reference/iterator/reverse_iterator/
            //! do not use crend(), crbegin() or std::vector<Item>::const_iterator
            //! it will produce an unmodifiable lvalue, causing an error at the line marked below
            
            int idToMatch = -1;
            std::reverse_iterator<std::vector<Item>::iterator> begin = itemVector.rbegin();
            std::reverse_iterator<std::vector<Item>::iterator> end = itemVector.rend();

            do {
                if ((*begin).type == _type){
                    idToMatch = (*begin).id;
                    (*begin).type = PAIRED_PARANTHESES_START; //!!! HERE
                    break;
                } else {
                    begin += 1;
                }
            } while (begin != end);

            if (idToMatch == -1){
                return -1;
            } else {
                return idToMatch;
            }
        }

    public:
        ScopeManager(){}
        ~ScopeManager() = default;

        Indentation generateIndentation(int _pos, int _line, int _depth) {
            Item newItem;
            newItem.id = _depth;
            newItem.type = INDENTATION;
            newItem.pos = _pos;

            appendToVector(newItem);

            return Indentation(_line, _depth);
        }

        Parantheses openParantheses(int _pos, int _line){
            groupDepth += 1;
            Item newItem;
            newItem.id = newId();
            newItem.type = PARANTHESES_START;
            newItem.pos = _pos;

            appendToVector(newItem);

            return Parantheses(groupDepth, TK_PARANTHESES_START, _line);
        }

        Parantheses closeParantheses(int _pos, int _line){
            groupDepth -= 1;
            Item newItem;
            newItem.id = matchId(PARANTHESES_START);
            newItem.type = (newItem.id == -1)?(PARANTHESES_END):(PAIRED_PARANTHESES_END);

            if (newItem.type == PARANTHESES_END){
                UNPAIRED_PARANTHESES_END = true;
            }

            newItem.pos = _pos;
            
            appendToVector(newItem);
            return Parantheses(groupDepth, TK_PARANTHESES_END, _line);
        }


};

#endif