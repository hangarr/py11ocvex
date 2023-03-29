#ifndef UTILSBINDINGS_PYCPP_HPP
#define UTILSBINDINGS_PYCPP_HPP

#include <any>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <opencv2/core.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include "utils.hpp"

namespace py = pybind11;

//
// Collection of pybind11 C++ routines for building Python bindings.
//
// Includes many routines for translating between complex Python and C++ data structures
// that arise from OpenCV.
//

//
// Elemental data types
//

std::string ptypes = "bBhHilfdspPxc?ILqQnNe";
int cvtypes[] = {CV_8S, CV_8U, CV_16S, CV_16U,
                 CV_32S, CV_32S, CV_32F, CV_64F,
                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//
// converters Python lists <-> C++ vectors
// NOTE: These only handle the subset of Python lists that are equivalent to rows in
// OpenCV cv:Mat objects.
//
// std::vector<T>
// std::vector<std::vector<T>>
// std::vector<std::vector<std::vector<T>>>
//

// https://pybind11.readthedocs.io/en/stable/advanced/pycpp/object.html
// https://docs.python.org/3/library/struct.html

template<class T>
struct is_vector_lv : std::false_type {};

template<class T, class Alloc>
struct is_vector_lv<std::vector<T, Alloc>> : std::true_type {};

template<class T>
inline constexpr bool is_vector_v_lv = is_vector_lv<T>::value;

template <typename T>
py::list vectorToList(const std::vector<T> &v) {
    py::list lo;
    for (T ve : v) {
        if constexpr(is_vector_v_lv<T>)
            lo.append(vectorToList(ve));
        else 
            lo.append(py::cast(ve));
    }

    return lo;
}


template <typename T>
void listToVector(const py::list l, T & vo) {}

template <typename T>
void listToVector(const py::list l, std::vector<T> & vo) {
    for (py::handle le : l) {
        if (py::isinstance<py::list>(le)) {
            T vi;
            listToVector(le.template cast<py::list>(), vi);
            vo.push_back(vi);
        }
        else 
            vo.push_back(le.template cast<T>());
    }
}


// get the depth and elemental data type of a Python list
// if isinstance(o, str):
// if type(o) is str:
// https://github.com/pybind/pybind11/issues/1619
// https://github.com/pybind/pybind11/blob/master/include/pybind11/pytypes.h

void listAttributes(const py::list l, int & ldepth, int & ltype, int depth = 1) {
    for (py::handle le : l) {
        if(py::isinstance<py::list>(le))
            listAttributes(le.template cast<py::list>(), ldepth, ltype, depth + 1);
        else {
            ldepth = std::max(ldepth, depth);
            int t = PyBool_Check(le.ptr()) ? 1 :
                    (PyLong_Check(le.ptr()) ? 2 : 
                    (PyFloat_Check(le.ptr()) ? 3 : 0));
            ltype = std::max(ltype, t);
        }
    }
}



//
// converters np.array <-> cv::Mat
//
// These only handle 1-D/2-D/3-D arrays
//

cv::Mat numpyToMatrix(py::buffer abuf) {
    py::buffer_info info = abuf.request();

    int depth = (info.shape.size() == 3) ? info.shape[2] : 1;

    char c = info.format.at(0);
    int ic = ptypes.find(c);
    int ctype = (ic != std::string::npos) ? cvtypes[ic] : CV_8U;
    int dtype = CV_MAKETYPE(ctype, depth);

    std::vector<int> sizeVec;

    // check out whether info.shape[2] is needed here
    for (int i = 0; i < 2; i++)
        sizeVec.push_back((int)info.shape[i]);

    return cv::Mat(sizeVec, dtype, info.ptr);
}

py::dtype npType(int depth) {
    switch (depth) {
    case CV_8U: return py::dtype::of<uint8_t>();
    case CV_16U: return py::dtype::of<uint16_t>();
    case CV_8S: return py::dtype::of<int8_t>();
    case CV_16S: return py::dtype::of<int16_t>();
    case CV_32S: return py::dtype::of<int32_t>();
    case CV_32F: return py::dtype::of<float>();
    case CV_64F: return py::dtype::of<double>();
    default: throw std::invalid_argument("Unsupported data type.");
    }
}


std::vector<std::size_t> npShape(cv::Mat& m) {

    std::vector<std::size_t> retv3 = {static_cast<size_t>(m.rows),
                                      static_cast<size_t>(m.cols),
                                      static_cast<size_t>(m.channels())};

    std::vector<std::size_t> retv = {static_cast<size_t>(m.rows),
                                     static_cast<size_t>(m.cols)};

    return (m.channels() > 1) ? retv3 : retv;
}

py::capsule makeCapsule(cv::Mat& m) {
    return py::capsule(new cv::Mat(m),
                       [](void *v) {
                           delete reinterpret_cast<cv::Mat*>(v);
                       });
}

// It's not clear that this is still the right way to convert a cv::Mat to a
// py::array, and in particular if the makeCapsule() is still needed.  But it
// still works so use it for now
py::array matrixToNumpy(cv::Mat& m) {
    if (!m.isContinuous())
        throw std::invalid_argument("Only continuous Mats supported.");

    return py::array(npType(m.depth()),
                     npShape(m),
                     m.data,
                     makeCapsule(m));
}


#endif
