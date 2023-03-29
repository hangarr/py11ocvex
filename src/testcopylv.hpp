#ifndef TESTCOPYLV_PYCPP_HPP
#define TESTCOPYLV_PYCPP_HPP

#include "utils.hpp"

//
// C++ application to be accessed from Python.
//

// This example copies a nested std::vector.
// The copy approach only works for situations where the total depth of the
// std::vector nesting can be inferred by the compiler from the source.

// CPP style

template <typename T>
void testcopyCPP(const std::vector<T> src, std::vector<T> & dst) {

    dst = copyVector(src);
}


// Python style

template <typename T>
std::vector<T> testcopyP(const std::vector<T> & src) {
    
    std::vector<T> dst = copyVector(src);
    
    return dst;
}

#endif
