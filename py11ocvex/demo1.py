#!/usr/bin/env python

import os
import sys


import numpy as np
import py11ocvex as px

def copy_and_compare_matrix(f, m):
    mc = f(m)
    return np.array_equal(m, mc)

def valid_copy_matrix(f, m):
    mch = copy_and_compare_matrix(f, m)
    msg = "success" if mch else "fail"  
    return(msg)

def test_copy_matrix(m, mnm):
    msg = valid_copy_matrix(px.testcopyCPP, m)
    print(mnm + ", px.testcopyCPP, " + msg)
    msg = valid_copy_matrix(px.testcopyCPP2, m)
    print(mnm + ", px.testcopyCPP2, " + msg)
    msg = valid_copy_matrix(px.testcopyP, m)
    print(mnm + ", px.testcopyP, " + msg)
    msg = valid_copy_matrix(px.testcopyP2, m)
    print(mnm + ", px.testcopyP2, " + msg)    


def copy_and_compare_vector(f, v):
    vc = f(v)
    return v == vc

def valid_copy_vector(f, v):
    mch = copy_and_compare_vector(f, v)
    msg = "success" if mch else "fail"  
    return(msg)

def test_copy_vector(v, vnm):
    msg = valid_copy_vector(px.testcopylvCPP, v)
    print(vnm + ", px.testcopylvCPP, " + msg)
    msg = valid_copy_vector(px.testcopylvP, v)
    print(vnm + ", px.testcopylvP, " + msg)  
  

def main():

    # non-exhaustive np.array <-> cv::Mat tests
    # cv::Mats limited to 2-D and 3_D numeric matrices
    # Behavior of copy operation is unpredictable for other np.array types

    a2_8s = np.array([[1,-2,3],[-4,5,-6]], dtype = np.int8)
    test_copy_matrix(a2_8s, "a2_8s")
    
    a3_8s = np.array([[[-1,2,3],[4,5,-6],[7,-8,9]],
                     [[11,12,-13],[14,-15,16],[-17,18,19]]],dtype=np.int8)
    test_copy_matrix(a3_8s, "a3_8s")


    a2_8u = np.array([[1,2,3],[4,5,6]], dtype = np.int8)
    test_copy_matrix(a2_8u, "a2_8u")

    a3_8u = np.array([[[1,2,3],[4,5,6],[7,8,9]],
                     [[11,12,13],[14,15,16],[17,18,19]]],dtype=np.int8)
    test_copy_matrix(a3_8u, "a3_8u")


    a2_16s = np.array([[1,-2,3],[-4,5,-6]], dtype = np.int16)
    test_copy_matrix(a2_16s, "a2_16s")

    a3_16s = np.array([[[-1,2,3],[4,5,-6],[7,-8,9]],
                       [[11,12,-13],[14,-15,16],[-17,18,19]]],dtype=np.int16)
    test_copy_matrix(a3_16s, "a3_16s")


    a2_16u = np.array([[1,-2,3],[-4,5,-6]], dtype = np.uint16)
    test_copy_matrix(a2_16u, "a2_16u")

    a3_16u = np.array([[[-1,2,3],[4,5,-6],[7,-8,9]],
                       [[11,12,-13],[14,-15,16],[-17,18,19]]],dtype=np.uint16)
    test_copy_matrix(a3_16u, "a3_16u")


    a2_32s = np.array([[1,-2,3],[-4,5,-6]], dtype = np.int32)
    test_copy_matrix(a2_32s, "a2_32s")

    a3_32s = np.array([[[-1,2,3],[4,5,-6],[7,-8,9]],
                       [[11,12,-13],[14,-15,16],[-17,18,19]]],dtype=np.int32)
    test_copy_matrix(a3_32s, "a3_32s")

 
    a2_32f = np.array([[1.6,-2.5,3.4],[-4.3,5.2,-6.1]], dtype = np.float32)
    test_copy_matrix(a2_32f, "a2_32f")

    a3_32f = np.array([[[-1.2,2.3,3.4],[4.5,5.6,-6.7],[7.8,-8.9,9.8]],
                       [[11.7,12.6,-13.5],[14.4,-15.3,16.2],[-17.1,18.2,19.3]]],dtype=np.float32)
    test_copy_matrix(a3_32f, "a3_32f")

 
    a2_64f = np.array([[1.6,-2.5,3.4],[-4.3,5.2,-6.1]], dtype = np.float64)
    test_copy_matrix(a2_64f, "a2_64f")

    a3_64f = np.array([[[-1.2,2.3,3.4],[4.5,5.6,-6.7],[7.8,-8.9,9.8]],
                       [[11.7,12.6,-13.5],[14.4,-15.3,16.2],[-17.1,18.2,19.3]]],dtype=np.float64)
    test_copy_matrix(a3_64f, "a3_64f")


    # non-exhaustive [] <-> std::vector<T> tests
    # [], std::vector<T> limited to 1-D, 2-D and 3_D boolean and numeric vectors
    # Behavior of copy operation is unpredictable for other vector types

    b1_b = [True,False,True]
    test_copy_vector(b1_b, "b1_b")

    b1_l = [1,2,3]
    test_copy_vector(b1_l, "b1_l")

    b1_f = [1.2,2.3,3.4]
    test_copy_vector(b1_f, "b1_f")

    b2_b = [[True,False,True],[False,False,True]]
    test_copy_vector(b2_b, "b2_b")

    b2_l = [[1,2,3],[4,5,6]]
    test_copy_vector(b2_l, "b2_l")

    b2_f = [[1.2,2.3,3.4],[4.5,5.6,6.7]]
    test_copy_vector(b2_f, "b2_f")

    b3_b = [[[True,False,True],[False,False,True]],[[True,False,True],[False,False,True]]]
    test_copy_vector(b3_b, "b3_b")

    b3_l = [[[1,2,3],[4,5,6]],[[1,2,3],[4,5,6]]]
    test_copy_vector(b3_l, "b3_l")

    b3_f = [[[1.2,2.3,3.4],[4.5,5.6,6.7]],[[1.2,2.3,3.4],[4.5,5.6,6.7]]]
    test_copy_vector(b3_f, "b3_f")


    return;

if __name__ == "__main__":
    main()
