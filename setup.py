import glob
import os

from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import find_packages, setup

here = os.path.abspath(os.path.dirname(__file__))

# Get the long description from the relevant file
with open(os.path.join(here, "README.md"), "r") as fin:
    long_description = fin.read()

from py11ocvex import __version__

#
# reads OpenCV path info from OS environment variables
#
try:
    opencv_include = os.environ["OPENCV_INCLUDE_PATH"]
    opencv_lib = os.environ["OPENCV_LIBRARY_PATH"]
except KeyError:
    envs = sorted(os.environ.keys())
    sys.stderr.write(
        """
    ***************************************************
    Environment variables `OPENCV_INCLUDE_PATH` and/or
    `OPENCV_LIBRARY_PATH` are not defined!

    Vars found: %s
    ***************************************************

    """
        % envs
    )
    raise
"""
ext_modules = [
    Pybind11Extension(
        "py11ocvex_cpp",
        sorted(glob.glob("cpp/*.cpp")),  # Sort source files for reproducibility
        include_dirs=[opencv_include],
        library_dirs=[opencv_lib],
        libraries=[
            "opencv_core",
            "opencv_highgui",
            "opencv_imgcodecs",
            "opencv_imgproc",
            "opencv_videoio",
        ],
    ),
]
"""
ext_modules = [
    Pybind11Extension(
        "py11ocvex_cpp",
        sorted(glob.glob("cpp/*.cpp")),  # Sort source files for reproducibility
        include_dirs=[opencv_include],
        library_dirs=[opencv_lib],
        libraries=[
            "opencv_core",
        ],
    ),
]


setup(
    name="py11ocvex",
    version=__version__,
    description="Example pybind11 bindings for building opencv based apps",
    cmdclass={"build_ext": build_ext},
    ext_modules=ext_modules,
    long_description=long_description,
    long_description_content_type="text/markdown",
    install_requires=[
        "numpy>=1.21,<1.22",
    ],
    entry_points={},
)

