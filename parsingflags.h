
#ifndef PSE_PARSING_FLAGS
#define PSE_PARSING_FLAGS

//parsing flags
namespace pflags {
    struct stringFlags
    {
        bool inString = false;
        char stringType;
    };

    stringFlags stringFlagsImpl;

    //used to track whether 4 spaces change scope or not in teh actual context
    bool scopeChangeContext = false;
    int paranthesesDepth = 0;
    int scopeDepth = 0;
    int sucessiveSpaces = 0;
    int line = 1;
    bool inSubscript = false;
}

#endif