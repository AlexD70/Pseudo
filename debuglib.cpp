#include <stdio.h>
#include <iostream>
#include <vector>

//kind of useless

#ifndef PSE_DEBUG_LIB
#define PSE_DEBUG_LIB

void prtCharVector (std::vector<char> V){
    for (char e : V){
        std::cout << e << ' ';
        printf("%d \n", e);
    }
}

#endif