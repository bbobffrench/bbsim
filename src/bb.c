#include "bb.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define BB_ACCEL 0.8
#define BB_MAX_SPEED 0.2
#define BB_OMEGA 100

void
bb_init(buffalo_byte_t *bb, double timestep){
	bb->motor_l = bb->motor_r = 0;
	bb->speed = bb->speed_prev = 0;
	bb->angle = bb->angle_prev = 0;
	bb->timestep = timestep;
	return;
}

void
bb_update(buffalo_byte_t *bb){
	double delta_speed = BB_ACCEL * bb->timestep;

	bb->speed_prev = bb->speed;
	if(!bb->motor_l && !bb->motor_r){
		if(bb->speed - delta_speed < 0) bb->speed = 0;
		else bb->speed -= delta_speed;
	}
	else{
		if(bb->speed + delta_speed > BB_MAX_SPEED) bb->speed = BB_MAX_SPEED;
		else bb->speed += delta_speed;
	}

	bb->angle_prev = bb->angle;
	if(bb->motor_l && bb->motor_r) bb->angle += BB_OMEGA * bb->timestep;
	else if(!bb->motor_l && bb->motor_r){
		bb->angle -= BB_OMEGA * bb->timestep;
		if(bb->angle < -360) bb->angle += 360;
	}
	else if(bb->motor_l && !bb->motor_r){
		bb->angle += BB_OMEGA * bb->timestep;
		if(bb->angle > 360) bb->angle -= 360;
	}

	bb->x += bb->speed * bb->timestep * sin(bb->angle * M_PI / 180);
	bb->y += bb->speed * bb->timestep * cos(bb->angle * M_PI / 180);
	return;
}

double
imu_accel_x(const buffalo_byte_t *bb){
	double velocity_x = bb->speed * sin(bb->angle * M_PI / 180);
	double velocity_x_prev = bb->speed_prev * sin(bb->angle_prev * M_PI / 180);

	return (velocity_x - velocity_x_prev) / bb->timestep;
}

double
imu_accel_y(const buffalo_byte_t *bb){
	double velocity_y = bb->speed * cos(bb->angle * M_PI / 180);
	double velocity_y_prev = bb->speed_prev * cos(bb->angle_prev * M_PI / 180);

	return (velocity_y - velocity_y_prev) / bb->timestep;
}
