#ifndef BUFFALO_BYTE_H
#define BUFFALO_BYTE_H

#include "mesh-network.h"

class BuffaloByte{
public:
	enum MovementState{STOPPED, LEFT_TURN, RIGHT_TURN, BB_LEFT, BB_RIGHT};

	void Update();
	double GetAccelX();
	double GetAccelY();
	void SetMovementState(enum MovementState);
private:
	const double TIMESTEP = 1.0 / 100;    // Run simulation at 100Hz
	const double ACCEL = 0.8;             // 0.8 meters per s^2
	const double MAX_SPEED = 0.2;         // 0.2 meters per s
	const double ANGULAR_VELOCITY = 100;  // 10 degrees per s

	// Real position in meters
	double m_x;
	double m_y;

	// Movement state and reference angle (for bang-bang correction)
	enum MovementState m_movementState;
	double m_refAngle;

	// Current speed, and angle
	double m_speed;
	double m_angle;

	// Simulated IMU acceleration data
	double m_accelX;
	double m_accelY;

	// Local node in the mesh network
	MeshNode m_node;
};

#endif
