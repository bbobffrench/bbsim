#include "bb.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define BB_ACCEL 50
#define BB_MAX_SPEED 0.2
#define BB_OMEGA 100

void
bb_init(buffalo_byte_t *bb){
	bb->motor_l = bb->motor_r = 0;
	bb->speed = bb->speed_prev = 0;
	bb->angle = bb->angle_prev = 0;
	return;
}

void
bb_update(buffalo_byte_t *bb, double delta_t){
	double delta_speed = BB_ACCEL * pow(delta_t, 2);

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
	if(bb->motor_l && bb->motor_r) bb->angle += BB_OMEGA * delta_t;
	else if(!bb->motor_l && bb->motor_r) bb->angle -= BB_OMEGA * delta_t;
	else if(bb->motor_l && !bb->motor_r) bb->angle += BB_OMEGA * delta_t;

	bb->x += bb->speed * delta_t * sin(bb->angle * M_PI / 180);
	bb->y += bb->speed * delta_t * cos(bb->angle * M_PI / 180);
	return;
}
