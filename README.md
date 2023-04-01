# Example Python `pybind11`/OpenCV C/C++ Extension

## Introduction
OpenCV uses it's own idiosyncratic system for building the Python `cv2` version of OpenCV as a Python C/C++ extension.  Although it would make sense to use the same system for building Python C/C++ extensions that use OpenCV, it's hard to find good documentation how to do this.  `pybind11` has emerged as a popular tool for building Python C/C++ extensions there also doesn't seem to be a lot of documentation or examples for doing this that clearly deals with the key OpenCV and Python data structures in OpenCV C++ and Python `cv2`. This repo provides a simple Python C/C++ extension using OpenCV++ and `pybind11` built with `setuptools` and its version of Python's `setup.py` functionality.

## C++/OpenCV and Python/Numpy Data Structures
OpenCV is a library for working with digital images in C++ embodied as modestly large (compared to big ML models anyway) 3-D `cv::Mat` data objects. Numpy is a Python package for working with modestly large `np.array` multidimensonal numeric matrices.  `cv:Mat`  and `np.array` objects both store data in a non-Pythonic, flexible C/C++ fashion and can be quite large.  Bindings such as `pybind11` and good programming practice strive to avoid copying this data in transfers between language domains.  Instead, transfers hand-off control of the memory and data it contains, including de-allocation when the data is no longer valid according to the rules of the language domain that has control of it when it becomes invalid.

Applications using OpenCV also commonly use small nested native `std:vector<T>` objects in C++ and native *List* `[]` objects in Python. Python `list` and C++ `std::vector<>` objects are Python `[]` are native complex data types with their own memory organization in the respective languages.  For that reason, the binding for one language to the other can only efficiently transfer a read-only accessor in the destination language to the data in the source language. Otherwise, when read/write access is required, the binding must copy the data from the source language item into a destination object with the destination language format.  

The `cv::Mat` objects used for images in OpenCV are 3-D matrices.  This usefully constrains the other three data objects and the code in this example.  Although `np.array` objects can have more than 3 dimensions, only 3-D `np.array` objects are needed here.  Similarly, nested `std::vector<T>` objects are only nested to depth `3` and the deepest `std::vector<T>` instances in a nested instance have the same dimension. As `cv::Mat` and `np.array` objects are contrained to have numeric elements, the C++ `std::vector<T>` and Python `[]` objects are limited to numeric elements in their innermost level. 

These limitation greatly simplify the example transfer operations between C++ and Python.  C++ is statically-typed and Python is dynamically-typed.  This constrains bindings between native complex data types in different languages.   Best performance, with the added benefit of simpler binding code, results when there are individual bindings for each C++ native complex data type used in an application.  Greatest flexibility results at the cost of more code complexity when the C++ code for a binding does dynamic type checking of Python objects and generates a C++ object matched to the particular Python object. These limitation also simplifies the example transfer operations between dynamically type Python and strictly static typed C++.

## Functional Description
This example implements four approaches to matrix copying.  Each version begins with a Python `np.array` to C++/OpenCV `cv::Mat` transfer, is followed by a `cv::Mat` to `cv::Mat` copy, and concludes with a C++/OpenCV `cv::Mat` to a Python `np.array` transfer. The four versions demonstrate four different approaches for implementing the C++ copy operation.  One pair demonstrates C++ style copy operations and the other pair demonstrates a Pythonic-style copy operations.  One version in each pair uses `cv::Mat` source and destination objects.  The other version in each pair uses OpenCV `InputArray` and `OutputArray` proxy operations.  In Python:
```
    <dst> = px.testcopyCPP(<src>)   - C++ style copy with cv::Mat objects
    <dst> = px.testcopyCPP2(<src>)  - C++ style copy with InputArray, OutputArray objects
    <dst> = px.testcopyP(<src>)     - Python style copy with cv::Mat objects
    <dst> = px.testcopyP2(<src>)    - Python style copy with InputArray and cv::Mat object
```

This example also includes two versions of nested vector copying. Similar to the matrix operations, these copy operations begin by with a nested Python `[]` to C++ `std::vector<T>` *copy*, is followed by a `std::vector<T>` to `std::vector<T>` copy, and concludes with a C++ `std::vector<T>` to Python `[]` *copy*.  Because the vectors are native constructs in their respective languages with limited copy operations in C++, the somewhat challenging task here is using C++ templates to copy between variable depth Python `[]` and variable depth C++ `std::vector<T>` objects and, to a lesser degree,  copying between variable depth C++ `std::vector<T>` objects.
```
    <dst> = px.testcopylvCPP(<src>)   - C++ style copy
    <dst> = px.testcopylvP(<src>)     - Python style copy
```

## Code Overview
The example includes the following files:
```
* bin
      rebuild           - script to rebuild and install extension with "./bin/rebuild"
      
* docs
    pybind11.pdf        - current version of "pybind11 Documentation" by Wenzel Jakob
    
* py11ocvex
    init.py             - copy of what could be "__init__.py" file for Python module
    demo1.py            - simple demo of this extension
    
* src
    pybindings.cpp      - the pybind11 binding specification
    testcopy.hpp        - example C++ application to copy/clone cv::Mat instances
    testcopylv.hpp      - example C++ application to copy nested std:vector<T> instances
    utils.hpp           - pure C++ utilities
    utilsbindings.hpp   - OpenCV/pybind11 utilities to copy/clone cv::Mat instances
    
  README.md             - This file
  pyproject.toml        - minimal project specification file
  setup.py              - "setuptools" version of this file for building the extension
```
The `setup.py` routine requires pointers to an OpenCV instatllation in two environment variables:
* `OPENCV_INCLUDE_PATH` - OpenCV headers
* `OPENCV_LIBRARY_PATH` - OpenCV libraries

## Notes:


## References:
* Python data types - [https://docs.python.org/3/library/struct.html](https://docs.python.org/3/library/struct.html)
* `pybind11` github site - [https://github.com/pybind/pybind11](https://github.com/pybind/pybind11)
* `setuptools` - [https://setuptools.pypa.io/en/latest/userguide/index.html](https://setuptools.pypa.io/en/latest/userguide/index.html)
* NumPy buffer protocol -  [https://pybind11.readthedocs.io/en/stable/advanced/pycpp/numpy.html](https://pybind11.readthedocs.io/en/stable/advanced/pycpp/numpy.html)
