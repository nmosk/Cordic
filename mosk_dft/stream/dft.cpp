#include<math.h>
#include "dft.h"
#include"coefficients1024.h"

#define N 1024 // Size of dft

// Read Data from Global Memory and write into Stream inStream
static void read_input(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE], hls::stream<unsigned int>& inStreamR, hls::stream<unsigned int>& inStreamI, int mysize) {
// Auto-pipeline is going to apply pipeline to this loop
mem_rd:
    for (int i = 0; i < mysize; i++) {
//#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
        // Blocking write command to inStream
        inStreamI << imag_sample[i];
        inStreamR << real_sample[i];
    }
}

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE], hls::stream<int> &outStreamR, hls::stream<int> &outStreamI)
{
	//Write your code here




	int i, j,spot;

	DTYPE real_temp[N];
	DTYPE imag_temp[N];


#pragma HLS ARRAY_PARTITION variable=real_sample type=block factor=4
#pragma HLS ARRAY_PARTITION variable=imag_sample type=block factor=4


		for(i=0; i < N; i += 1) {
			real_temp[i] = 0;
			imag_temp[i] = 0;


			for (j = 0; j < N; j += 1) {


#pragma HLS PIPELINE II=4


				spot = i * j % SIZE;

				real_temp[i] += (inStreamR.read()*cos_coefficients_table[spot] - inStreamI.read()*sin_coefficients_table[spot]);
				imag_temp[i] += (inStreamR.read()*sin_coefficients_table[spot] + inStreamI.read()*cos_coefficients_table[spot]);

			}

						outStreamR << real_temp[i];
						outStreamI << imag_temp[i];
		}

		return;
}

static void write_result(DTYPE real_out[SIZE], DTYPE imag_out[SIZE], hls::stream<int> &outStreamR, hls::stream<int> &outStreamI, int mysize) {
mem_wr:
    for (int i = 0; i < mysize; i++) {
       #pragma HLS PIPELINE II=1
       #pragma HLS LOOP_TRIPCOUNT min=c_size max=c_size
        //Blocking read command from OutStream
        real_out[i] = outStreamR.read();
        imag_out[i] = outStreamI.read();
    }
}

void top_dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE], DTYPE real_out[SIZE], DTYPE imag_out[SIZE], int mysize) {

#pragma HLS INTERFACE mode=s_axilite port=real_sample
#pragma HLS INTERFACE mode=s_axilite port=imag_sample
#pragma HLS INTERFACE mode=s_axilite port=return


	// took from the example
    static hls::stream<unsigned int> inStreamR("input_stream_R");
    static hls::stream<unsigned int> inStreamI("input_stream_I");
    static hls::stream<unsigned int> outStreamR("output_stream_R");
    static hls::stream<unsigned int> outStreamI("output_stream_I");
#pragma HLS STREAM variable = inStreamI depth = 64
#pragma HLS STREAM variable = inStreamR depth = 64
#pragma HLS STREAM variable = outStreamR depth = 64
#pragma HLS STREAM variable = outStreamI depth = 64

#pragma HLS dataflow
    // dataflow pragma instruct compiler to run following three APIs in parallel
    read_input(real_sample, imag_sample, inStreamR, inStreamI, mysize);
    dft(inStreamR, inStreamI, outStreamR,outStreamI, mysize);
    write_result(imag_out, real_out, outStreamR, outStreamI, mysize);



}
