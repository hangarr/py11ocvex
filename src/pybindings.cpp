/*
 * The code here enables communication between C++ and python.  To
 * expose functions to python, use the code inside the macro
 * PYBIND11_MODULE below.
 */

#include <iostream>
#include <cmath>

#include <opencv2/core.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include "utilsbindings.hpp"

#include "testcopy.hpp"
#include "testcopylv.hpp"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;


//
// pybind11 wrappers for example C++ application that copies Python np.arrays
//
py::array testcopyCPPwrapper(py::buffer src_b) {
  cv::Mat src = numpyToMatrix(src_b);
  cv::Mat dst;

  testcopyCPP(src, dst);

  return matrixToNumpy(dst);
}

py::array testcopyCPP2wrapper(py::buffer src_b) {
  cv::Mat src = numpyToMatrix(src_b);
  cv::Mat dst;

  testcopyCPP2(src, dst);

  return matrixToNumpy(dst);
}

py::array testcopyPwrapper(py::buffer src_b) {
  cv::Mat src = numpyToMatrix(src_b);

  cv::Mat dst = testcopyP(src);

  return matrixToNumpy(dst);
}

py::array testcopyP2wrapper(py::buffer src_b) {
  cv::Mat src = numpyToMatrix(src_b);

  cv::Mat dst = testcopyP2(src);

  return matrixToNumpy(dst);
}


//
// pybind11 wrappers for example C++ application that copies Python lists
//

template <typename T>
py::list testcopylvCPPcopy(py::list src, std::vector<T> & typer) {
    std::vector<T> src_v;
    listToVector(src, src_v);

    std::vector<T> dst;
    testcopyCPP(src_v, dst);

    return vectorToList(dst);
}


py::list testcopylvCPPwrapper(py::list src) {

    int ldepth = 0;
    int ltype = 0;

    listAttributes(src, ldepth, ltype);

    py::list dst;

    switch(ldepth) {
        case 1: {
            switch(ltype) {
                case 1: {
                    std::vector<bool> typer1_1 = {false};
                    dst = testcopylvCPPcopy(src, typer1_1);
                    break;
                }
                case 2: {
                    std::vector<long> typer1_2 = {0};
                    dst = testcopylvCPPcopy(src, typer1_2);
                    break;
                }
                case 3: {
                    std::vector<double> typer1_3 = {0};
                    dst = testcopylvCPPcopy(src, typer1_3);
                    break;
                }
                default: {
                    dst = py::cast<py::none>(Py_None);
                }
            }
            break;
        }  
        case 2: {
            switch(ltype) {
                case 1: {
                    std::vector<std::vector<bool>> typer2_1 = {{false}};
                    dst = testcopylvCPPcopy(src, typer2_1);
                    break;
                }
                case 2: {
                    std::vector<std::vector<long>> typer2_2 = {{0}};
                    dst = testcopylvCPPcopy(src, typer2_2);
                    break;
                }
                case 3: {
                    std::vector<std::vector<double>> typer2_3 = {{0}};
                    dst = testcopylvCPPcopy(src, typer2_3);
                    break;
                }
                default: {
                    dst = py::cast<py::none>(Py_None);
                }
            }
            break;
        }
        case 3: {
            switch(ltype) {
                case 1: {
                    std::vector<std::vector<std::vector<bool>>> typer3_1 = {{{false}}};
                    dst = testcopylvCPPcopy(src, typer3_1);
                    break;
                }
                case 2: {
                    std::vector<std::vector<std::vector<long>>> typer3_2 = {{{0}}};
                    dst = testcopylvCPPcopy(src, typer3_2);
                    break;
                }
                case 3: {
                    std::vector<std::vector<std::vector<double>>> typer3_3 = {{{0}}};
                    dst = testcopylvCPPcopy(src, typer3_3);
                    break;
                }
                default: {
                    dst = py::cast<py::none>(Py_None);
                }
            }
            break;
        }
        default: {
            dst = py::cast<py::none>(Py_None);
        }
    }

    return dst;  
}

template <typename T>
py::list testcopylvPcopy(py::list src, std::vector<T> & typer) {
    std::vector<T> src_v;
    listToVector(src, src_v);

    std::vector<T> dst = testcopyP(src_v);

    return vectorToList(dst);
}

py::list testcopylvPwrapper(py::list src) {

    int ldepth = 0;
    int ltype = 0;

    listAttributes(src, ldepth, ltype);

    py::list dst;

    switch(ldepth) {
        case 1: {
            switch(ltype) {
                case 1: {
                    std::vector<bool> typer1_1 = {false};
                    dst = testcopylvPcopy(src, typer1_1);
                    break;
                }
                case 2: {
                    std::vector<long> typer1_2 = {0};
                    dst = testcopylvPcopy(src, typer1_2);
                    break;
                }
                case 3: {
                    std::vector<double> typer1_3 = {0};
                    dst = testcopylvPcopy(src, typer1_3);
                    break;
                }
                default: {
                    dst = py::cast<py::none>(Py_None);
                }
            }
            break;
        }  
        case 2: {
            switch(ltype) {
                case 1: {
                    std::vector<std::vector<bool>> typer2_1 = {{false}};
                    dst = testcopylvPcopy(src, typer2_1);
                    break;
                }
                case 2: {
                    std::vector<std::vector<long>> typer2_2 = {{0}};
                    dst = testcopylvPcopy(src, typer2_2);
                    break;
                }
                case 3: {
                    std::vector<std::vector<double>> typer2_3 = {{0}};
                    dst = testcopylvPcopy(src, typer2_3);
                    break;
                }
                default: {
                    dst = py::cast<py::none>(Py_None);
                }
            }
            break;
        }
        case 3: {
            switch(ltype) {
                case 1: {
                    std::vector<std::vector<std::vector<bool>>> typer3_1 = {{{false}}};
                    dst = testcopylvPcopy(src, typer3_1);
                    break;
                }
                case 2: {
                    std::vector<std::vector<std::vector<long>>> typer3_2 = {{{0}}};
                    dst = testcopylvPcopy(src, typer3_2);
                    break;
                }
                case 3: {
                    std::vector<std::vector<std::vector<double>>> typer3_3 = {{{0}}};
                    dst = testcopylvPcopy(src, typer3_3);
                    break;
                }
                default: {
                    dst = py::cast<py::none>(Py_None);
                }
            }
            break;
        }
        default: {
            dst = py::cast<py::none>(Py_None);
        }
    }

    return dst;  
}


PYBIND11_MODULE(antineutron_cpp1_1, m) {
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------
        .. currentmodule:: py11ocvex
        .. autosummary::
           :toctree: _generate
    )pbdoc";

    // It's unclear right now if these different approaches
    // are all memory-safe and equally efficient.
    m.def("testcopyCPP", &testcopyCPPwrapper,  R"pbdoc(
        "CPP style copy with cv:Mat source and destination"
    )pbdoc");

    m.def("testcopyCPP2", &testcopyCPP2wrapper,  R"pbdoc(
        "CPP style copy with InputArray source and OutputArray destinatione"
    )pbdoc");

    m.def("testcopyP", &testcopyPwrapper,  R"pbdoc(
        "Python style copy with cv:Mat source"
    )pbdoc");

    m.def("testcopyP2", &testcopyP2wrapper,  R"pbdoc(
        "Python style copy with InputArray source"
    )pbdoc");


    m.def("testcopylvCPP", &testcopylvCPPwrapper,  R"pbdoc(
        "CPP style copy with C++ nested std:vector source"
    )pbdoc");

    m.def("testcopylvP", &testcopylvPwrapper,  R"pbdoc(
        "Python style copy with C++ nested std:vector source"
    )pbdoc");
    
#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
