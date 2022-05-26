#include <fstream>
#include <iostream>
#include <filesystem>
#include "classes.cpp"
#ifndef _GLIBCXX_STRING
#include <string>
#endif

int main() {
    std::string filename;

    std::cin >> filename;
    int k = std::filesystem::file_size(filename);

    std::ifstream f(filename);
    char* filebuffer = new char[k];
    
    for (int i = 0; i < k; i++){
        filebuffer[i] = f.get();
        std::cout << filebuffer[i];
    }

    std::cout << "\n\n";

    char* ptr = nullptr;
    int size = 0;
    Token* tokens = new Token[k];
    int j = 0;

    for (int i = 0; i < k; i++){
        if (filebuffer[i] == '+'){
            tokens[j].ptr = filebuffer + i;
            j += 1;
            continue;

        } else if (filebuffer[i] == '-'){
            tokens[j].ptr = filebuffer + i;
            j += 1;
            continue;

        } else if (filebuffer[i] == '*'){
            tokens[j].ptr = filebuffer + i;
            j += 1;
            continue;

        } else if (filebuffer[i] == '%'){
            tokens[j].ptr = filebuffer + i;
            j += 1;
            continue;

        } else if (filebuffer[i] == '='){
            tokens[j].ptr = filebuffer + i;
            j += 1;
            continue;

        } else if (filebuffer[i] == ','){
            tokens[j].ptr = filebuffer + i;
            j += 1;
            continue;

        } else if (filebuffer[i] == '>'){
            tokens[j].ptr = filebuffer + i;
            if (filebuffer[j + 1] == '='){
                tokens[j].size += 1;
                i += 1;
            }
            j += 1;
            continue;

        } else if (filebuffer[i] == '<'){
            tokens[j].ptr = filebuffer + i;
            if (filebuffer[j + 1] == '='){
                tokens[j].size += 1;
                i += 1;
            } else if (filebuffer[j + 1] == '-'){
                tokens[j].size += 1;
                i += 1;
            }
            j += 1;
            continue;

        } else if (filebuffer[i] == '\''){
            tokens[j].ptr = filebuffer + i;
            j += 1;
            continue;

        } else if (filebuffer[i] == '\"'){
            tokens[j].ptr = filebuffer + i;
            j += 1;
            continue;

        } else if (filebuffer[i] == '('){
            tokens[j].ptr = filebuffer + i;
            j += 1;
            continue;

        } else if (filebuffer[i] == ')'){
            tokens[j].ptr = filebuffer + i;
            j += 1;
            continue;

        } else if ((filebuffer[i] == '/') && (filebuffer[i + 1] == '/')){
            do{
                i += 1;
            } while (filebuffer[i] != '\n');
            continue;

        } else if ((filebuffer[i] == '/') && (filebuffer[i + 1] == '*')){
            do{
                i += 1;
            } while ((filebuffer[i] != '/')&&(filebuffer[i + 1] != '*'));
            i += 1;
            continue;

        } else if (filebuffer[i] == '/'){
            tokens[j].ptr = filebuffer + i;
            j += 1;
            continue;

        }
    }


    for (int i = 0; i < j; i++){
        std::cout << *(tokens[i].ptr) << '\n';
    }

    std::cout << j;
}