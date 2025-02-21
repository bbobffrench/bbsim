#ifndef BUFFALO_BYTE_H
#define BUFFALO_BYTE_H

#define BB_MAX_SPEED 0.25
#define BB_ACCEL 0.1
#define BB_DECCEL 0.1

typedef struct buffalo_byte{
	char motor_l, motor_r;
	double x, y;
	double speed, speed_prev;
	double theta, theta_prev; /* With respect to y-axis */
} buffalo_byte_t;

void bb_init(buffalo_byte_t *bb);
void bb_update(buffalo_byte_t *bb, double);

#endif
