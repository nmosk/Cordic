#include "cordiccart2pol.h"
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

data_t Kvalues[NO_ITER] = {1,	0.500000000000000,	0.250000000000000,	0.125000000000000,	0.0625000000000000,	0.0312500000000000,	0.0156250000000000,	0.00781250000000000,	0.00390625000000000,	0.00195312500000000,	0.000976562500000000,	0.000488281250000000,	0.000244140625000000,	0.000122070312500000,	6.10351562500000e-05,	3.05175781250000e-05};

data_t angles[NO_ITER] = {0.785398163397448,	0.463647609000806,	0.244978663126864,	0.124354994546761,	0.0624188099959574,	0.0312398334302683,	0.0156237286204768,	0.00781234106010111,	0.00390623013196697,	0.00195312251647882,	0.000976562189559320,	0.000488281211194898,	0.000244140620149362,	0.000122070311893670,	6.10351561742088e-05,	3.05175781155261e-05};


void cordiccart2pol(data_t x, data_t y, data_t * r,  data_t * theta)
{

	// initialize my starting x and y and create tracker theta
	// we will iterate x and y to zero position and integrate the theta steps
	data_t current_x = x;
	data_t current_y = y;
	data_t current_theta = 0;
	data_t r0 = sqrt(x*x + y*y);
	bool QUAD23;

	if (x < 0) {
		QUAD23 = 1;
		current_x = -1* current_x;
	}
	else
		QUAD23 = 0;

	data_t factor = 1.0;
	for (int j = 0 ; j < NO_ITER ; j++ ){

		// determine direction of rotation
		coef_t sigma = ( current_y < 0 ) ? 1:-1;  // if y is not negative, then we will rotate clockwise towards y = 0

		// determine magnitude of xy vector change
		data_t cos_shift = sigma * factor * current_x;
		data_t sin_shift = sigma * factor * current_y;

		// rotate values and add to angle tracker
		current_x = current_x - sin_shift;
		current_y = current_y + cos_shift;
		current_theta = current_theta - sigma*angles[j];
		// resize factor for next iteration 2^-j
		factor = factor/2;
	}

	if (QUAD23) {
		if (y > 0) current_theta = 4*angles[0] - current_theta;
		else current_theta = -4*angles[0] - current_theta;
	}

	*theta = current_theta;
	*r = r0;

}
