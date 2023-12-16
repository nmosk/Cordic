/*
This is traditional 2-radix DIT FFT algorithm implementation.
It is based on conventional 3-loop structure. 
INPUT:
	In_R, In_I[]: Real and Imag parts of Complex signal

OUTPUT:
	In_R, In_I[]: Real and Imag parts of Complex signal
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<string.h>
#include "fft.h"
//#include "hls_stream.h"
//#include "ap_axi_sdata.h"


void fft_stage(INTTYPE stage, DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OR[SIZE], DTYPE OI[SIZE]);
void bit_reverse(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE temp_R[SIZE], DTYPE temp_I[SIZE]);
unsigned int reverse_bits(unsigned int input);

void demod(DTYPE X_R[SIZE], DTYPE X_I[SIZE], int D[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE])
{

	fft(X_R, X_I, OUT_R, OUT_I);
	bit_reverse(OUT_R, OUT_I,OUT_R, OUT_I);
	qpsk_decode(OUT_R, OUT_I, D);

}

void fft(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE])
{



//#pragma HLS array_partition variable=W_real complete
//#pragma HLS array_partition variable=W_imag complete

#pragma HLS DATAFLOW
	INTTYPE stage;
	DTYPE Stage_R[M][SIZE], Stage_I[M][SIZE];
	DTYPE temp_R2[SIZE];		/*temporary storage complex variable*/
	DTYPE temp_I2[SIZE];		/*temporary storage complex variable*/

#pragma HLS ARRAY_PARTITION variable=Stage_R dim=1 type=complete
#pragma HLS ARRAY_PARTITION variable=Stage_I dim=1 type=complete



	/*=====================BEGIN BIT REBERSAL===========================*/
	// write your code here

	bit_reverse(X_R,X_I,Stage_R[0],Stage_I[0]);

/*
	for(int i=0; i<SIZE; i++)
	{
		Stage_R[0][i] = temp_R2[i];
		Stage_I[0][i] = temp_I2[i];
		//printf("xr: %f, xi: %f\n", Stage_R[0][i],Stage_I[0][i]);
	}
*/

	/*++++++++++++++++++++++END OF BIT REVERSAL++++++++++++++++++++++++++*/

	/*=======================BEGIN: FFT=========================*/
	// Do M stages of butterflies


	stages:for(stage=1; stage< M; stage++)
	{
#pragma HLS UNROLL
		fft_stage(stage, Stage_R[stage-1], Stage_I[stage-1], Stage_R[stage], Stage_I[stage]);
	}
	fft_stage(M, Stage_R[M-1], Stage_I[M-1],OUT_R,OUT_I);

/*
	printf ("Nate Test \n");
	for(int i=0; i<SIZE; i++)
	{
		printf("xr: %f, xi: %f\n", Stage_R[0][i],Stage_I[0][i]);
	}
	*/
}
/*=======================END: FFT=========================*/


unsigned int reverse_bits(unsigned int input){
	int i, rev = 0;
	for(i = 0; i < M; i++){
		rev = (rev << 1) | (input & 1);
		input = input >> 1;
	}
	return rev;
}

void bit_reverse(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE temp_R[SIZE], DTYPE temp_I[SIZE]){
	unsigned int reversed;
	unsigned int i;
	DTYPE temp;

	for(i = 0; i < SIZE; i++){
		reversed = reverse_bits(i);
		if (i <= reversed){
			temp = X_R[i];
			temp_R[i] = X_R[reversed];
			temp_R[reversed] = temp;

			temp = X_I[i];
			temp_I[i] = X_I[reversed];
			temp_I[reversed] = temp;
			//OUT_R[i] = X_R[i];
			//OUT_I[i] = X_I[i];
		}
	}


}

void fft_stage(INTTYPE stage, DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OR[SIZE], DTYPE OI[SIZE]){



			DTYPE temp_R;		/*temporary storage complex variable*/
			DTYPE temp_I;		/*temporary storage complex variable*/

			int i,j,k;			/* loop indexes */
			int i_lower;		/* Index of lower point in butterfly */
			int step;

			for(i = 0;i<SIZE; i++){
						OR[i] = X_R[i];
						OI[i] = X_I[i];
			}

			//int stage;
			int DFTpts;
			int numBF;			/*Butterfly Width*/

			int N2 = SIZE2;	/* N2=N>>1 */

			step=N2;
			DTYPE c, s;
			int count = 0;

			DFTpts = 1 << stage;		// DFT = 2^stage = points in sub DFT
			numBF = DFTpts/2; 			// Butterfly WIDTHS in sub-DFT
			k=0;


			// Perform butterflies for j-th stage
			butterfly:for(j=0; j<numBF; j++)
			{

				//c = cos(a);
				//s = sin(a);
				c = W_real[count];
				s = W_imag[count];
				//a = a + e;
				//printf("test#:%d; c: %f\t j:%d\n",count,c,j);

				// Compute butterflies that use same W**k
				DFTpts:for(i=j; i<SIZE; i += DFTpts)
				{

					i_lower = i + numBF;			//index of lower point in butterfly
					temp_R = OR[i_lower]*c- OI[i_lower]*s;
					temp_I = OI[i_lower]*c+ OR[i_lower]*s;

					OR[i_lower] = OR[i] - temp_R;
					OI[i_lower] = OI[i] - temp_I;
					OR[i] = OR[i] + temp_R;
					OI[i] = OI[i] + temp_I;
					//OR[i] = X_R[i];
					//OI[i] = X_I[i];
				}
				k+=step;
				count+=512/numBF;

			}
			step=step/2;

}


/*
void export_fft(float *inR, float *inI, float *outR, float *outI,int len)
{
#pragma HLS INTERFACE s_axilite port=return bundle=my_fft
#pragma HLS INTERFACE s_axilite port=len bundle=my_fft
#pragma HLS INTERFACE mode=m_axi depth=1050 port=outR offset=slave bundle=outputr
#pragma HLS INTERFACE mode=m_axi depth=1050 port=inR offset=slave bundle=inputr
#pragma HLS INTERFACE mode=m_axi depth=1050 port=outI offset=slave bundle=outputi
#pragma HLS INTERFACE mode=m_axi depth=1050 port=inI offset=slave bundle=inputi
#pragma HLS INTERFACE s_axilite port=outR bundle=my_fft
#pragma HLS INTERFACE s_axilite port=inR bundle=my_fft
#pragma HLS INTERFACE s_axilite port=outI bundle=my_fft
#pragma HLS INTERFACE s_axilite port=inI bundle=my_fft

	float buffR[SIZE], buffI[SIZE];
	float outBuffR[SIZE], outBuffI[SIZE];
	memcpy(buffR,(const float*)inR,len*sizeof(DTYPE));
	memcpy(buffI,(const float*)inI,len*sizeof(DTYPE));
	fft(buffR, buffI, outBuffR, outBuffI);
	memcpy(outR,(const float*)outBuffR,len*sizeof(float));
	memcpy(outI,(const float*)outBuffI,len*sizeof(float));
}
*/



