#ifndef IMU_DATA_H
#define IMU_DATA_H

#include "mesh-network.h"

class BuffaloByte{
public:
	void Update();
	double GetAccelX();
	double GetAccelY();
private:
	const double TIMESTEP = 1.0 / 100;    // Run simulation at 100Hz
	const double ACCEL = 0.8;             // 0.8 meters per s^2
	const double MAX_SPEED = 0.2;         // 0.2 meters per s
	const double ANGULAR_VELOCITY = 100;  // 10 degrees per s

	// Real position in meters
	double m_x;
	double m_y;

	// State for each motor, either forward or off
	bool m_motorL;
	bool m_motorR;

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
