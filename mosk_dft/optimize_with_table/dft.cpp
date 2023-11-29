#include<math.h>
#include "dft.h"
#include"coefficients256.h"

#define N 256 // Size of dft

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE], DTYPE real_out[SIZE], DTYPE imag_out[SIZE])
{
	//Write your code here

#pragma HLS INTERFACE mode=s_axilite port=real_sample
#pragma HLS INTERFACE mode=s_axilite port=imag_sample
#pragma HLS INTERFACE mode=s_axilite port=return


	int i, j;

	DTYPE real_temp[N];
	DTYPE imag_temp[N];


		for(i=0; i < N; i += 1) {
			real_temp[i] = 0;
			imag_temp[i] = 0;

			for (j = 0; j < N; j += 1) {

				int spot = i * j % 256;

				real_temp[i] += (real_sample[j]*cos_coefficients_table[spot] - imag_sample[j]*sin_coefficients_table[spot]);
				imag_temp[i] += (real_sample[j]*sin_coefficients_table[spot] + imag_sample[j]*cos_coefficients_table[spot]);

			}

						real_out[i] = real_temp[i];
						imag_out[i] = imag_temp[i];
		}

		return;
}
