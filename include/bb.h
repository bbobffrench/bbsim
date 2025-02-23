#ifndef BUFFALO_BYTE_H
#define BUFFALO_BYTE_H

typedef struct buffalo_byte{
	char motor_l, motor_r;
	double x, y;
	double speed, speed_prev;
	double angle, angle_prev;
	double timestep;
} buffalo_byte_t;

void bb_init(buffalo_byte_t *, double);
void bb_update(buffalo_byte_t *);
double imu_accel_x(const buffalo_byte_t *);
double imu_accel_y(const buffalo_byte_t *);

#endif
