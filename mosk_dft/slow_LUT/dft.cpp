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

	DTYPE s_LUT[N][N];
	DTYPE c_LUT[N][N];

#pragma HLS ARRAY_PARTITION variable=s_LUT complete
#pragma HLS ARRAY_PARTITION variable=c_LUT complete

		for(i = 0; i < N; i +=1){

			w = (2.0 * 3.141592653589 / N) * (DTYPE)i;

			for (j = 0; j < N; j += 1) {

				c_LUT[i][j] = cos(j*w);
				s_LUT[i][j] = -sin(j*w);

			}

		}

		for(i=0; i < N; i += 1) {
			real_temp[i] = 0;
			imag_temp[i] = 0;

			for (j = 0; j < N; j += 1) {

				real_temp[i] += (real_sample[j]*c_LUT[i][j] - imag_sample[j]*s_LUT[i][j]);
				imag_temp[i] += (real_sample[j]*s_LUT[i][j] + imag_sample[j]*c_LUT[i][j]);

			}

		}

		for(i = 0; i < N; i += 1){
			real_sample[i] = real_temp[i];
			imag_sample[i] = imag_temp[i];
		}

		return;
}
