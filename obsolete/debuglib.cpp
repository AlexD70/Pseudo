#include <stdio.h>
#include <iostream>
#include <vector>

//kind of useless
//this file should probably be deleted

#ifndef PSE_DEBUG_LIB
#define PSE_DEBUG_LIB

void prtCharVector (std::vector<char> V){
    for (char e : V){
        std::cout << e << ' ';
        printf("%d \n", e);
    }
}

#endif