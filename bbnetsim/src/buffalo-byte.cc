#include "buffalo-byte.h"

#include <cmath>
#include <numbers>

void
BuffaloByte::Update(){
	// Update the speed based on movement state
	double speedPrev = m_speed;
	if(m_movementState != STOPPED){
		m_speed += ACCEL * TIMESTEP;
		m_speed = m_speed > MAX_SPEED ? MAX_SPEED : m_speed;
	}
	else{
		m_speed -= ACCEL * TIMESTEP;
		m_speed = m_speed < 0 ? 0 : m_speed;
	}

	// Perform bang-bang movement correction
	if(m_movementState == BB_LEFT)
		m_movementState = m_angle - m_refAngle < 10 ? BB_RIGHT : BB_LEFT;
	else if(m_movementState == BB_RIGHT)
		m_movementState = m_angle - m_refAngle > 10 ? BB_LEFT : BB_RIGHT;

	// Update the angle based on movement state
	double anglePrev = m_angle;
	if(m_movementState == LEFT_TURN || m_movementState == BB_LEFT){
		m_angle += ANGULAR_VELOCITY * TIMESTEP;
		m_angle = m_angle >= 360 ? m_angle - 360 : m_angle;
	}
	if(m_movementState == LEFT_TURN || m_movementState == BB_LEFT){
		m_angle -= ANGULAR_VELOCITY * TIMESTEP;
		m_angle = m_angle <= -360 ? m_angle + 360 : m_angle;
	}

	// Calculate the current and previous velocities
	double velocityX = m_speed * std::cos(m_angle * (std::numbers::pi / 180));
	double velocityY = m_speed * std::sin(m_angle * (std::numbers::pi / 180));
	double velocityXPrev = speedPrev * std::cos(anglePrev * (std::numbers::pi / 180));
	double velocityYPrev = speedPrev * std::sin(anglePrev * (std::numbers::pi / 180));

	// Update ns-3 node velocity
	m_node.SetVelocity(velocityX, velocityY);

	// Update position based on current velocity
	m_x += velocityX * TIMESTEP;
	m_y += velocityY * TIMESTEP;

	// Update IMU acceleration data based on current and previous velocities
	m_accelX = (velocityX - velocityXPrev) / TIMESTEP;
	m_accelY = (velocityY - velocityYPrev) / TIMESTEP;
}

double
BuffaloByte::GetAccelX(){
	return m_accelX;
}

double
BuffaloByte::GetAccelY(){
	return m_accelY;
}

void
BuffaloByte::SetMovementState(enum BuffaloByte::MovementState movementState){
	if(movementState == BB_LEFT || movementState == BB_RIGHT)
		m_refAngle = m_angle;
	m_movementState = movementState;
}
