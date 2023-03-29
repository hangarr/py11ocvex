# Building antineutron 1.1 (AN 1.1) C++

## Introduction
antineutron 1.0 (AN 1.0) uses `pybind11` to build the Python bindings for the C++ core. While this approached allows one to leave the C++ code alone, one does have to manually code a `pybindings.cpp` module that provides some idiosyncratic `pybind11` declarations and all translations between Python/C++ complex data types.  Modifying the C++ code often requires modifying the `pybindings.cpp`.

AN 1.1 uses a slightly modified version of the OpenCV tools for automatically building the Python bindings.  These tools only require that one annotate the core C++ `.hpp` files with just a few macros, all supporting code and information is in other external files in the repo. The macros should be transparent to other C++ build chains. The general approach and the macros are explained in the OpenCV doc [How OpenCV-Python Bindings Works? ](https://docs.opencv.org/4.5.5/da/d49/tutorial_py_bindings_basics.html), although not very clearly.

## OpenCV-Python Bindings

Ultimately C++ extensions to Python must be packaged in a `mypackage.fullname.so` shared object library on *nix systems.  If this `mypackage.fullname.so` file is packaged in either a Python egg (old-style packaging) or Python wheel (new-style packaging), the bindings can be made available on a Python installation by:
```
pip install .
```
This executes the `setup.py` script in a suitably structured repo to build an install the extension which includes the `mypackage.fullname.so`.

The module is invoked in a Python script as
```
import mypackage
```

These days [`setuptools`](https://setuptools.pypa.io/en/latest/userguide/index.html) is the preferred library to support Python packaging including C/C++ extensions.  `setuptools` is configured by a `pyproject.toml` file in a package.  But `setuptools` also can use a `setup.py` (or `setup.cfg`) file to configure modulle packaging.

## Building A C++ Extension

### Install *robot-build* to automatically build Python wrappers for *pybind11* wrappers for the C++

* [Documentation](https://robotpy-build.readthedocs.io/en/latest/)
* [Installation](https://robotpy-build.readthedocs.io/en/latest/install.html):
```
$ pip3 install robotpy-build
```

```
struct buffer_info {
    void *ptr;
    py::ssize_t itemsize;   -- in bytes
    std::string format;
    py::ssize_t ndim;
    std::vector<py::ssize_t> shape;
    std::vector<py::ssize_t> strides;
};

int dtype = CV_MAKETYPE(CV_8U, info.shape[2]);
```
[NumPy Buffer Protocol](https://pybind11.readthedocs.io/en/stable/advanced/pycpp/numpy.html)


## Notes:

1. `np.array`s have unlimited dimensions. But `cv:Mat`s have only 3 dimensions where the third dimension is referred to as *channels*.  **WARNING**: This demo does not handle `np.array`s with more than 3 dimensions.
2. `np.array`s and `cv:Mat`s use memory in a non-Pythonic, flexible C/C++ fashion and can be quite large.  Bindings such as `pybind11` and good programming practice strive to avoid copying the data in transfers between language domains.  Instead, transfers hand-off control of the memory and data it contains, including de-allocation when the data is no longer valid according to the rules of the language domain that has control of it when it becomes invalid.
3.  Python `list` and C++ `std::vector<>` objects are native complex data types with their own memory organization in the respective languages.  For that reason, the binding for one language to the other can only efficiently transfer a read-only accessor in the destination language to the data in the source language. Otherwise, when read/write access is required, the binding must copy the data from the source language item into a destination object with the destination language format.
4. C++ is statically-typed and Python is dynamically-typed.  This constrains bindings between native complex data types in different languages.   Best performance, with the added benefit of simpler binding code, results when there are individual bindings for each C++ native complex data type used in an application.  Greatest flexibility results at the cost of more code complexity when the C++ code for a binding does dynamic type checking of Python objects and generates a C++ object matched to the particular Python object.