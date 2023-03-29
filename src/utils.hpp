#ifndef UTILS_PYCPP_HPP
#define UTILS_PYCPP_HPP

#include <any>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

//
// Collection of non-pybind11 C++ utility routines for building Python bindings.
//
// Includes many routines for managing complex data structures.
//
// https://stackoverflow.com/questions/71687088/c-template-recursively-print-a-vector-of-vector-using-template
//

//
// Vector operations
//

template<class T>
struct is_vector : std::false_type {};

template<class T, class Alloc>
struct is_vector<std::vector<T, Alloc>> : std::true_type {};

template<class T>
inline constexpr bool is_vector_v = is_vector<T>::value;

template <typename T>
std::vector<T> copyVector(const std::vector<T> &v)
{
    std::vector<T> vo;
    for (T ve : v) {
        if constexpr(is_vector_v<T>)
            vo.push_back(copyVector(ve));
        else 
            vo.push_back(ve);
    }

    return vo;
}


template <typename T>
std::string toStringVector(const std::vector<T> &v)
{
    std::string res = "[";
    int n = v.size();
    for (size_t i=0; i<n; i++) {
        if constexpr(is_vector_v<T>)
            res += toStringVector(v[i]);
        else 
            res += std::to_string(v[i]);
        if (i < n-1) res += ", ";
    }
    res += "]";

    return res;
}


#endif
