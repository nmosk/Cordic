#include "cordiccart2pol.h"
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <ap_int.h>

data_t Kvalues[20] = {1,	0.500000000000000,	0.250000000000000,	0.125000000000000,	0.0625000000000000,	0.0312500000000000,	0.0156250000000000,	0.00781250000000000,	0.00390625000000000,	0.00195312500000000,	0.000976562500000000,	0.000488281250000000,	0.000244140625000000,	0.000122070312500000,	6.10351562500000e-05,	3.05175781250000e-05,	1.52587890625e-05,	7.6293945312e-06,	3.8146972656e-06,	1.9073486328e-06};

data_t angles[20] = {0.785398163397448,	0.463647609000806,	0.244978663126864,	0.124354994546761,	0.0624188099959574,	0.0312398334302683,	0.0156237286204768,	0.00781234106010111,	0.00390623013196697,	0.00195312251647882,	0.000976562189559320,	0.000488281211194898,	0.000244140620149362,	0.000122070311893670,	6.10351561742088e-05,	3.05175781155261e-05,	1.52587890625e-05,	7.6293945312e-06,	3.8146972656e-06,	1.9073486328e-06};


void cordiccart2pol(data_t x, data_t y, data_t * r,  data_t * theta)
{
	// initialize my starting x and y and create tracker theta
	// we will iterate x and y to zero position and integrate the theta steps
	data_t current_x = x;
	data_t current_y = y;
	data_t current_theta = 0;
	data_t r0 = sqrt(x*x + y*y);
	bool QUAD23;

	// assume x y is in or near unit cirlce (less than 2,2)
	short cur_int_x = (short)(current_x * 10000);
	short cur_int_y = (short)(current_y * 10000);

	/*
	debugging
	printf("cur_int_x: %d\n",cur_int_x);
	printf("cur_int_y: %d\n",cur_int_y);
	*/


	if (x < 0) {
		QUAD23 = 1;
		cur_int_x = -1* cur_int_x;
	}
	else
		QUAD23 = 0;

	short cos_shift;
	short sin_shift;
	data_t factor = 1.0;
	for (int j = 0 ; j < NO_ITER ; j++ ){

		// determine direction of rotation
		coef_t sigma = ( cur_int_y < 0 ) ? 1:-1;  // if y is not negative, then we will rotate clockwise towards y = 0

		if (cur_int_y < 0) {
			// determine magnitude of xy vector change
			cos_shift = cur_int_x >> j;
			sin_shift = cur_int_y >> j;
			printf("cos_shift: %d\n",cos_shift);
			printf("sin_shift: %d\n",sin_shift);
		}
		else
		{
			// determine magnitude of xy vector change
			cos_shift = ~(cur_int_x-1) >> j;
			sin_shift = ~(cur_int_y-1) >> j;
			printf("cos_shift: %d\n",cos_shift);
			printf("sin_shift: %d\n",sin_shift);
			}

		// rotate values and add to angle tracker
		cur_int_x = cur_int_x - sin_shift;
		cur_int_y = cur_int_y + cos_shift;
		current_theta = current_theta - sigma*angles[j];


	}

	if (QUAD23) {
		if (y > 0) current_theta = 4*angles[0] - current_theta;
		else current_theta = -4*angles[0] - current_theta;
	}

	*theta = current_theta;
	*r = r0;

}
