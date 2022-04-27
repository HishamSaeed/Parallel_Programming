# SIMD Tutorial

This tutorial is about using SIMD intrinsics, the tutorial consists of matrix matrix multiplication. It requires to implement the multiplication using SIMD
intrinsics to achieve speed up

# Running the example

There exist a dirrectory called student, inside it the developed code, from the main directory, run make all to build example, and run make unit_test to bulid unit tests, all the binaries are built inside the student directory. 

Inside the student directory
run ./dgemm_par <array_size>
    ./unit_test <array_size>