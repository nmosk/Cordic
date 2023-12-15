#include "fft.h"
#include <stdio.h>

void qpsk_decode(DTYPE R[SIZE], DTYPE I[SIZE], int D[SIZE]) {

	//Write your code here
	int i;
	for(i = 0; i < SIZE; i++){
		if (R[i] > 0){
			if(I[i] > 0)
				D[i] = 0;
			else
				D[i] = 2;
		}
		if (R[i] < 0){
			if (I[i] > 0)
				D[i] = 1;
			else
				D[i] = 3;
		}
		//printf("#%d FFT(R)(I): %f %f %d\n", i, R[i], I[i], D[i]);
	}

}

