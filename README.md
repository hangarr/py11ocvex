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
    rebuild             - script to rebuild and install extension with "./bin/rebuild"
      
* docs
    pybind11.pdf        - current version of "pybind11 Documentation" by Wenzel Jakob
    
* py11ocvex             - Simple python package within this example
    init__.py           - what could be "__init__.py" file for a Python module
    demo1.py            - simple demo of this extension
    
* cpp
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

## Notes

### Python packaging details
The folder naming and layout struture of this example may not conform to best Python practices. It is inherited from another source project, so it seems best to just note some key details.

First note that this example actually has a dual-nature:  It's a standard C/C++ extension, but it's also a so-called "flat layout" Python package.  The `py11ocvex` folder with Python files in this repo, including the `__init__.py` is what makes this a Python package and its location is what makes it a "flat-layout" style package.

This dual nature along with the `pybind11` C/C++ binding approach has several implications for naming components of the package:

#### 1. Extension Naming and Importing
The extension is packaged as shared-object (`py11ocvex_cpp.<platform-specifier>.so`) library of position-independent code.  The extension name is specified by the
**setup.py**
```
ext_modules = [
    Pybind11Extension(
        "py11ocvex_cpp",
    ...
]
```
The shared-object library name determines how the C/C++ extension is loaded, e.g:
```
import py11ocvex_cpp as px
```
#### 2. Extension initialization
The `import` statement causes loader to find and read the `*.so` file.  After a C/C++ extension file is loaded, the system searches for the module export function, `PyInit_py11ocvex_cpp` in this case, to initialize the module. The name of this file is specified in the `cpp/pybindings.cpp` file:
```
PYBIND11_MODULE(py11ocvex_cpp, m) {
...
}
```

#### 3. Python subpackage
The "flat-layout" style of this Python package means the package loader treats any folder in the project that contains a `__init__.py` file as a subpackage.  The subpackage name is determined by the folder name, in this case `py11ocvex`.


#### 4. Python `egg-info`
Python egg files have largely been replaced by Python wheels for packaging and distributing Python packages.  But `egg-info` files have been retained, in this case `py11ocvex-0.0.1.dist-info`.  The name for this file is derived from the `name` argument passed to `setup()` in the `setup.py` file:
```
setup(
    name="py11ocvex",
    ...
)
```

### Packaging issue
The multiple locations for specifying the names of the several components of this example as currently structured are not an issue in themselves.  But it appears one must keep in mind a detail of how the Python CLI works when choosing names.

Specifically, it seems that if one:
1. Names the C/C++ extension module export function `PyInit_py11ocvex`, rather than `PyInit_py11ocvex_cpp`, and
2. Names the Python subpackage folder `py11ocvex`, and
3. Starts the Python CLI from inside the outer `py11ocvex`, at the same level is the folder hierarchy as the inner `py11ocvex` file, and finally
4. The `py11ocvex` subfolder contains a `__init__.py` file;
add attempts to load the C/C++ extension with the statement:
```
import py11ocvex as px
```
then:
1. Python first attempts to load the `py11ocvex` Python subpackage, and
2. Fails to load and properly initialize the C/C++ extension.

The naming approach in this "flat-layout" example circumvents this problem.  Alternatively, and different file and folder layout may also avoid the problem and may be preferable for new projects.


## References:
* Python data types - [https://docs.python.org/3/library/struct.html](https://docs.python.org/3/library/struct.html)
* `pybind11` github site - [https://github.com/pybind/pybind11](https://github.com/pybind/pybind11)
* `setuptools` - [https://setuptools.pypa.io/en/latest/userguide/index.html](https://setuptools.pypa.io/en/latest/userguide/index.html)
* NumPy buffer protocol -  [https://pybind11.readthedocs.io/en/stable/advanced/pycpp/numpy.html](https://pybind11.readthedocs.io/en/stable/advanced/pycpp/numpy.html)
* Module Import Precedence in Python [https://trstringer.com/python-module-import-precedence/](https://trstringer.com/python-module-import-precedence/)
