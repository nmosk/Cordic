#include<math.h>
#include "dft.h"
#include"coefficients256.h"

#define N 256 // Size of dft

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE])
{
	//Write your code here

#pragma HLS INTERFACE mode=s_axilite port=real_sample
#pragma HLS INTERFACE mode=s_axilite port=imag_sample
#pragma HLS INTERFACE mode=s_axilite port=return

	int i, j;
		DTYPE w;
		DTYPE c, s;

		DTYPE real_temp[N];
		DTYPE imag_temp[N];

		for(i=0; i < N; i += 1) {
			real_temp[i] = 0;
			imag_temp[i] = 0;

			w = (2.0 * 3.141592653589 / N) * (DTYPE)i;

			for (j = 0; j < N; j += 1) {

				c = cos(j*w);
				s = -sin(j*w);

				real_temp[i] += (real_sample[j]*c - imag_sample[j]*s);
				imag_temp[i] += (real_sample[j]*s + imag_sample[j]*c);

			}

		}

		for(i = 0; i < N; i += 1){
			real_sample[i] = real_temp[i];
			imag_sample[i] = imag_temp[i];
		}

		return;
}
