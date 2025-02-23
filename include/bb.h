#ifndef BUFFALO_BYTE_H
#define BUFFALO_BYTE_H

typedef struct buffalo_byte{
	char motor_l, motor_r;
	double x, y;
	double speed, speed_prev;
	double angle, angle_prev;
} buffalo_byte_t;

void bb_init(buffalo_byte_t *bb);
void bb_update(buffalo_byte_t *bb, double);

#endif
