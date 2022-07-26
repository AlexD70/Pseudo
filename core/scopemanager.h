//this is the ScopeManager class

#include <cstddef>
#include <cstdint>
#include <vector>
#include "../tokenlib/indentation.h"
#include "../tokenlib/parantheses.h"


#ifndef PSE_CORE_SCOPEMANAGER
#define PSE_CORE_SCOPEMANAGER

class ScopeManager {
    protected:
        enum types {
            INDENTATION = 0,
            PARANTHESES_START = -1,
            PARANTHESES_END = 1,
            SUBSCRIPT_START = -2,
            SUBSCRIPT_END = 2
        };

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

    public:
        ScopeManager(){}
        ~ScopeManager() = default;

        Indentation generateIndentation(int _pos, int _line, int _depth) {
            Item newItem;
            _pos += 1;
            newItem.id = _depth;
            newItem.type = INDENTATION;
            newItem.pos = _pos;

            appendToVector(newItem);

            return Indentation(_line, _depth);
        }
};

#endif