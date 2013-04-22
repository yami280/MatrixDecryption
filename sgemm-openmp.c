#include <stdlib.h>
#include <stdio.h>
#include <nmmintrin.h>
#include <string.h>
#include <omp.h>

#define VERTICAL_ROLL 16
#define HORIZONTAL_ROLL 4
#define MIN(X, Y) (X<Y ? X : Y)

void sgemm( int m, int n, int d, float *A, float *C )
{
	#pragma omp parallel for
	for (int j = 0; j < n; j++) {
	    int jn1 = j*(n+1), jn = j*n; float *Cjn = C+jn;
		for (int i = 0; i < n/VERTICAL_ROLL*VERTICAL_ROLL; i+=VERTICAL_ROLL) {
			int i1 = i + 4;
			int i2 = i + 8;
			int i3 = i + 12;
			int i4 = i + 16;
			int i5 = i + 20;
			int i6 = i + 24;
			int i7 = i + 28;

			__m128 Cij = _mm_loadu_ps(Cjn+i);
			__m128 Cij1 = _mm_loadu_ps(Cjn+i1);
			__m128 Cij2 = _mm_loadu_ps(Cjn+i2);
			__m128 Cij3 = _mm_loadu_ps(Cjn+i3);

			for (int k = 0; k < m; k++) {
			    int k1 = k + 1; float *Akn = A+k*n;
				 __m128 Ajk = _mm_load1_ps(Akn+jn1);

				 __m128 Aik = _mm_loadu_ps(Akn+i);
				 __m128 Ai1k = _mm_loadu_ps(Akn+i1);
				 __m128 Ai2k = _mm_loadu_ps(Akn+i2);
				 __m128 Ai3k = _mm_loadu_ps(Akn+i3);

				 Cij = _mm_add_ps(Cij, _mm_mul_ps(Ajk, Aik));
				 Cij1 = _mm_add_ps(Cij1, _mm_mul_ps(Ajk, Ai1k));
				 Cij2 = _mm_add_ps(Cij2, _mm_mul_ps(Ajk, Ai2k));
				 Cij3 = _mm_add_ps(Cij3, _mm_mul_ps(Ajk, Ai3k));
			}
			_mm_store_ps(Cjn+i, Cij);
			_mm_store_ps(Cjn+i1, Cij1);
			_mm_store_ps(Cjn+i2, Cij2);
			_mm_store_ps(Cjn+i3, Cij3);
		}
	}

	if (n/VERTICAL_ROLL*VERTICAL_ROLL != n) {
		#pragma omp parallel for
		for (int j = 0; j < n; j++) {
		    	    for (int i = n/VERTICAL_ROLL*VERTICAL_ROLL; i < n; i++) {
				float *addrCij = C+i+j*n;
				__m128 Cij = _mm_loadu_ps(addrCij);
				for (int k = 0; k < m; k++) {
				    int kn = k*n;
				    __m128 Ajk = _mm_loadu_ps(A+j*(n+1)+kn);
					__m128 Aik = _mm_loadu_ps(A+i+kn);
					Cij = _mm_add_ps(Cij, _mm_mul_ps(Ajk, Aik));
				}
				_mm_store_ss(addrCij, Cij);
			}
		}
	}
}

