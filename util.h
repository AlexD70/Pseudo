#ifndef PSE_INTERNAL_VECTOR
#define PSE_INTERNAL_VECTOR
#include <vector>
#endif

//#include <typeinfo>

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

//the Union class can be useful in other projects too
// template <typename T1, typename T2> //unused cuz i found a better solution
// class Union{
//     protected:
//         union {
//             T1 _t1;
//             T2 _t2;
//         };
//         bool which;
//         const int CHECK_VALUE = 100;

//     public:
//         Union(T1 val, bool k = false){
//             //k is used to prevent ambiguous function call
//             _t1 = val;
//             which = true;
//         }
//         Union(T2 val, int k = 0){
//             //k is used to prevent ambiguous function call
//             _t2 = val;
//             which = false;
//         }

//         template <typename T>
//         T __repr__(){
//             T tInstance;
//             T1 t1Instance;
//             if (typeid(tInstance).name() == typeid(t1Instance).name()){
//                 if (!which){
//                     throw std::exception();
//                 }
//                 return _t1;
//             } else {
//                 if (which){
//                     throw std::exception();
//                 }
//                 return _t2;
//             }
//         }
// };

//maybe adding a header guard somewhere around here would be a good idea