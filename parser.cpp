#include <fstream>
#include <iostream>
#include "classes.cpp"
#include <vector>

int main() {
    /*
    ** READ FROM filename INTO filebuffer
    */

    std::vector<char> filebuffer;
    std::string filename;
    

    std::cin >> filename;
    std::ifstream f(filename);
    int k = 0;
    
    for (int i = 0; ; i++){
        char ch;
        filebuffer.reserve(i + 2);

        ch = f.get();
        if (f.eof()){
            break;
        }

        filebuffer.push_back(ch);
        //std::cout << filebuffer[i];
        k = i;
    }

    //std::cout << "\n\n";


    /*
    ** CREATE TOKENS
    */

    char* ptr = nullptr;
    int size = 0;
    bool inString = false;
    char stringType;
    bool inParantheses = false;
    int paranthesesDepth = 0;
    
    std::vector<Token> tokens;
    tokens.reserve(k);

    int j = 0;

    for (int i = 0; i < k; i++){

        /*
        ** OPERATORS
        */

       char ref = filebuffer[i];

       while (inString){
           if ((filebuffer[i] == stringType) && (filebuffer[i - 1] != '\\')){
                inString = false;
           }
           if (i == k - 1){
               std::cout << "SyntaxError: string literal reaches EOF.\n";
               exit(-1);
           }
           if (filebuffer[i] == '\n'){
               std::cout << "SyntaxError: multi-line string literals not allowed.\n";
               exit(-1);
           }
           tokens[j - 1].size += 1;
           i += 1;
       }

        if (ref == '+'){  //ADDITION
            tokens[j].ptr = &filebuffer[i];
            tokens[j].tokenType = OPERATOR; 
            j += 1;
            continue;

        } else if (ref == '-'){  //SPECIAL WRAPPER FOR MINUS SIGN
            tokens[j].ptr = &filebuffer[i];
            tokens[j].tokenType = OPERATOR__; 
            j += 1;
            continue;

        } else if (ref == '*'){  //MULTIPLY
            tokens[j].ptr = &filebuffer[i];
            tokens[j].tokenType = OPERATOR;
            j += 1;
            continue;

        } else if (ref == '%'){  //MODULUS
            tokens[j].ptr = &filebuffer[i];
            tokens[j].tokenType = OPERATOR;
            j += 1;
            continue;

        } else if (ref == '='){  //EQUALITY (NON STRICT)
            tokens[j].ptr = &filebuffer[i];
            tokens[j].tokenType = OPERATOR;
            j += 1;
            continue;

        } else if (ref == '>'){  //GT and GTorEQ
            tokens[j].ptr = &filebuffer[i];
            if (filebuffer[i + 1] == '='){
                tokens[j].size += 1;
                tokens[j].tokenType = OPERATOR;
                i += 1;
            }
            j += 1;
            continue;

        } else if (ref == '<'){  //LT and LTorEQ
            tokens[j].ptr = &filebuffer[i];
            if (filebuffer[i + 1] == '='){
                tokens[j].size += 1;
                tokens[j].tokenType = OPERATOR;
                i += 1;

                /*
                ** ASSIGNMENT
                */

            } else if (filebuffer[i + 1] == '-'){
                tokens[j].size += 1;
                tokens[j].tokenType = VALUE_ASSIGNEMENT;
                i += 1;
            }
            j += 1;
            continue;

        /*
        ** STRINGS
        */

        } else if (ref == '\''){
            tokens[j].ptr = &filebuffer[i];
            tokens[j].tokenType = STRING;
            j += 1;
            inString = true;
            stringType = '\'';
            continue;

        } else if (ref == '\"'){
            tokens[j].ptr = &filebuffer[i];
            tokens[j].tokenType = STRING;
            j += 1;
            inString = true;
            stringType = '\"';
            continue;

        /*
        ** SEPARATORS
        */

        } else if (ref == ','){
            tokens[j].ptr = &filebuffer[i];
            tokens[j].tokenType = SEPARATOR;
            j += 1;
            continue;

        /*
        ** PARANTHESES
        */

        } else if (ref == '('){
            tokens[j].ptr = &filebuffer[i];
            tokens[j].tokenType = PARANTHESES;
            inParantheses = true;
            paranthesesDepth += 1;
            j += 1;
            continue;

        } else if (ref == ')'){
            tokens[j].ptr = &filebuffer[i];
            tokens[j].tokenType = PARANTHESES;
            paranthesesDepth -= 1;
            if (paranthesesDepth == 0){
                inParantheses = false;
            } else if (paranthesesDepth < 0){
                std::cout << "SyntaxError: bad parantheses nesting.\n";
                exit(-1);
            }
            j += 1;
            continue;

        /*
        ** COMMENTS - literally skips them
        */

        } else if ((ref == '/') && (filebuffer[i + 1] == '/')){
            do{
                i += 1;
            } while (filebuffer[i] != '\n');
            continue;

        } else if ((ref == '/') && (filebuffer[i + 1] == '*')){
            do{
                i += 1;
                if (i == k - 2){
                    std::cout << "SyntaxError: multi-line comment reches EOF.\n";
                    exit(-1);
                } 

            } while ((filebuffer[i] != '/')&&(filebuffer[i + 1] != '*'));
            i += 1;
            continue;

        /*
        ** DEFAULT - maps later to IDENTIFIER or KEYWORD
        */

        } else if (isalpha(ref) || ref == '_'){
            tokens[j].ptr = &filebuffer[i];
            tokens[j].tokenType = DEFAULT;

            while(true){
                i += 1;
                if (!((isalnum(filebuffer[i])) || (filebuffer[i] == '_'))){
                    break;
                }
                tokens[j].size += 1;
            } 

            j += 1;
            continue;

        /*
        ** INTEGER LITERALS
        */

        } else if (isdigit(ref)){
            tokens[j].ptr = &filebuffer[i];
            tokens[j].tokenType = INTEGER;

            while((filebuffer[i+1] > 64 && filebuffer[i+1] < 91) ||
                (filebuffer[i+1] > 96 && filebuffer[i+1] < 123)) {
                    tokens[j].size += 1;
                    i += 1;
            }
            continue;
        }
    }

    if (inParantheses){
        std::cout << "SyntaxError: bad parantheses nesting.\n";
        exit(-1);
    }


    for (int i = 0; i < j; i++){
        std::cout << *(tokens[i].ptr) << '\n';
    }

    std::cout << j << '\n';
}