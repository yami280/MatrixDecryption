MatrixDecryption
================

1. Use SSE Instructions (see lab 7)

load C[j*n to j*n+n] in a register on the outermost loop (j).
-store C[j*n to j*n+n] back into memory (sse)
load A[k*n to k*n+m] in a register on the 2nd loop (k).
-store A[k*n to k*n+m] back into memory (sse)
leave innermost loop(i) as is


2. Optimize loop ordering (see lab 5)
-j
-k
-i

3. Implement Register Blocking (load data into a register once and then use it several times)
store into register instead of going to cache every time
use intel insts and store info as vectors

load C[j*n to j*n+n] in a register on the outermost loop (j).
-store C[j*n to j*n+n] back into memory (sse)
load A[k*n to k*n+m] in a register on the 2nd loop (k).
-store A[k*n to k*n+m] back into memory (sse)
leave innermost loop(i) as is

4. Implement Loop Unrolling (see lab 7)
increment every loop by 4*(num of unrolled iterations)
unroll iterations of i (innermost loop)

fringe case: use same method as lab07 (sum.c), add extra check so that variable le less than height/width??

5. Compiler Tricks (minor modifications to your source code can cause the compiler to produce a faster program)
