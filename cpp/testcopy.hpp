#ifndef TESTCOPY_PYCPP_HPP
#define TESTCOPY_PYCPP_HPP

#include <opencv2/core.hpp>

//
// C++ application to be accessed from Python.
//

// This example copies a cv::Mat.
// Could use cv::InputArray and cv::OutputArray proxy containers that
// admit references as the dst, but those objects don't have a clone
// function.

// CPP style

void testcopyCPP(const cv::Mat src, cv::Mat & dst) {

    dst = src.clone();
}


void testcopyCPP2(const cv::InputArray src, cv::OutputArray dst) {

    src.copyTo(dst);
}

// Python style

cv::Mat testcopyP(const cv::Mat src) {
    cv::Mat dst;
    
    dst = src.clone();

    return dst;
}

cv::Mat testcopyP2(const cv::InputArray src) {
    cv::Mat dst;
    
    src.copyTo(dst);

    return dst;
}

#endif
