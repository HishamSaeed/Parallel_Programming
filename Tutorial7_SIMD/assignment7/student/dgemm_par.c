#include "dgemm.h"
#include <immintrin.h>
#include <inttypes.h>

void dgemm(float *a, float *b, float *c, int n) {
    // TODO: insert your solution here
    __m128 add;
    int ub = n - (n%8);

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
          float* c_ptr = (float*)(c + ((i * n) + j));
          // __mmask4 mask;
          add = _mm_loadu_ps (c_ptr);
            //add = *c_ptr;
            for(int k = 0; k < ub; k += 8){
              // lpointing to the required rows and columns for multiplication from 
              // original matrix
              float* a_ptr = (float*)(a + ((i * n )+ k));
              float* b_ptr = (float*)(b + ((j * n) + k));
              // loading required rows and columns into SIMD registers
              __m256 a_i =  _mm256_loadu_ps (a_ptr);
              __m256 b_j =  _mm256_loadu_ps (b_ptr);

              // multiplying row and column
              __m256 mul =  _mm256_mul_ps (a_i, b_j);
              
              // adding the result of the multiplication
              __m128 v0h = _mm256_extractf128_ps (mul, 0);
              __m128 v0l = _mm256_extractf128_ps (mul, 1);
              // using horizontal add for efficiency
              v0h = _mm_add_ps (v0h, v0l);
              v0h = _mm_hadd_ps(v0h,v0h);
              v0h = _mm_hadd_ps(v0h,v0h);
              
              add = _mm_add_ss (add, v0h);
              
            }
            
            // storing the final result into the c pointer
            _mm_store_ss (c_ptr, add);
            for(int l = ub; l<n;l++){
              *c_ptr += a[i * n  + l] * b[j * n  + l];
            }

            
        }
    }
}

