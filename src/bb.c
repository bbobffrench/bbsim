#include "bb.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PI 3.14159

void
bb_init(buffalo_byte_t *bb){
	bb->motor_l = bb->motor_r = 0;
	bb->speed = 0;
	bb->theta = 0;
	return;
}

void
bb_update(buffalo_byte_t *bb, double delta_t){
	double delta_theta, delta_speed;

	bb->speed_prev = bb->speed;
	if(!bb->motor_l && !bb->motor_r){
		delta_speed = BB_DECCEL * pow(delta_t, 2);
		if(bb->speed - delta_speed < 0) bb->speed = 0;
		else bb->speed -= delta_speed;
	}
	else{
		delta_speed = BB_ACCEL * pow(delta_t, 2);
		if(bb->speed + delta_speed > BB_MAX_SPEED) bb->speed = BB_MAX_SPEED;
		else bb->speed += delta_speed;
	}

	if(bb->motor_l && bb->motor_r){
		delta_theta = (PI + PI * (double)rand() / RAND_MAX) * delta_t;
		if(bb->theta < bb->theta_prev){
			bb->theta_prev = bb->theta;
			bb->theta += (rand() % 4 ? delta_theta : -delta_theta);
		}
		else{
			bb->theta_prev = bb->theta;
			bb->theta += (rand() % 4 ? -delta_theta : delta_theta);
		}
	}
	else if(!bb->motor_l && bb->motor_r){
		bb->theta_prev = bb->theta;
		bb->theta -= PI * delta_t;
	}
	else if(bb->motor_l && !bb->motor_r){
		bb->theta_prev = bb->theta;
		bb->theta += PI * delta_t;
	}
	else bb->theta_prev = bb->theta;

	bb->x += bb->speed * delta_t * sin(bb->theta);
	bb->y += bb->speed * delta_t * cos(bb->theta);
	return;
}
