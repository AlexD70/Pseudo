#include <stdio.h>
#include <iostream>
#include <vector>

void prtCharVector (std::vector<char> V){
    for (char e : V){
        std::cout << e << ' ';
        printf("%d \n", e);
    }
}