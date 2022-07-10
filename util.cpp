#pragma once
#include <vector>

//these snippets can be used even outside this project to make appending to std::vector simpler

// template <typename T>
// void appendToVector(std::vector<T> vector, T what){
//     vector.reserve(vector.size() + 1);
//     vector.push_back(what);

//     return;
// }

template <class T> //commented the one above to prevent redefinition error
void appendToVector(std::vector<T> vector, T what){
    vector.reserve(vector.size() + 1);
    vector.push_back(what);

    return;
}