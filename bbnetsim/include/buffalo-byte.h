#ifndef BUFFALO_BYTE_H
#define BUFFALO_BYTE_H

#include "mesh-network.h"

class BuffaloByte{
public:
	const double TIMESTEP = 0.001; // Update in 1ms increments

	enum MovementState{LEFT, RIGHT};

	BuffaloByte();
	void Update();
	void SetMovementState(enum MovementState);
	double GetAccelX();
	double GetAccelY();
	double GetPosX();
	double GetPosY();
	double GetAngle();
private:
	const double SPEED = 0.2;
	const double ANGULAR_VELOCITY = 90;

	enum MovementState m_movementState;

	// Real position in meters
	double m_x;
	double m_y;

	// Direction of the velocity vector
	double m_angle;

	// X and Y components of the acceleration vector
	double m_accelX;
	double m_accelY;
};

#endif
