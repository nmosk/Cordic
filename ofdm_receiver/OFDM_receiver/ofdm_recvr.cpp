#include "fft.h"
#include <stdio.h>

static unsigned short count;
static DTYPE xr[ SIZE ];
static DTYPE xi[ SIZE ];
static DTYPE xr_out[ SIZE ];
static DTYPE xi_out[ SIZE ];
static int   dout[ SIZE ];

void ofdm_receiver( volatile DTYPE *inptr, volatile uint32_t *outptr )
{
//#pragma AP interface ap_fifo port=inptr
//#pragma AP interface ap_fifo port=outptr
//#pragma AP interface ap_ctrl_none port=return
#pragma HLS INTERFACE s_axilite port=return bundle=my_ofdm
#pragma HLS INTERFACE mode=m_axi depth=35050 port=outptr offset=slave bundle=output
#pragma HLS INTERFACE mode=m_axi depth=35050 port=inptr offset=slave bundle=input
#pragma HLS INTERFACE s_axilite port=outptr bundle=my_ofdm
#pragma HLS INTERFACE s_axilite port=inptr bundle=my_ofdm

	*outptr++ = dout[ count ];

	xr[ count ] = *inptr++;
	xi[ count ] = *inptr++;
	count++;
	if( count == 1024 ){
		count = 0;
		demod( xr, xi, dout, xr_out, xi_out );
	}
}


/*
 * 	printf("Printing FFT results\n");
	for (int i = 0; i < 35000; i++) {
		printf("#%d FFT(R)(I): %f %f %d\n", i, xr_out[i], xi_out[i], dout[i]);
	}
 */
