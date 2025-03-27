#include "buffalo-byte.h"

#include <cmath>
#include <numbers>

BuffaloByte::BuffaloByte(){
	m_x = m_y = m_angle = 0;
	m_movementState = RIGHT;
}

void
BuffaloByte::Update(){
	// Update the angle based on movement state
	double anglePrev = m_angle;
	if(m_movementState == LEFT){
		m_angle -= ANGULAR_VELOCITY * TIMESTEP;
		m_angle = m_angle <= -360 ? m_angle + 360 : m_angle;
	}
	else if(m_movementState == RIGHT){
		m_angle += ANGULAR_VELOCITY * TIMESTEP;
		m_angle = m_angle >= 360 ? m_angle - 360 : m_angle;
	}

	// Calculate the current and previous velocities
	double velocityX = SPEED * std::cos(m_angle * (std::numbers::pi / 180));
	double velocityY = SPEED * std::sin(m_angle * (std::numbers::pi / 180));
	double velocityXPrev = SPEED * std::cos(anglePrev * (std::numbers::pi / 180));
	double velocityYPrev = SPEED * std::sin(anglePrev * (std::numbers::pi / 180));

	// Update position based on current velocity
	m_x += velocityX * TIMESTEP;
	m_y += velocityY * TIMESTEP;

	// Update IMU acceleration data based on current and previous velocities
	m_accelX = (velocityX - velocityXPrev) / TIMESTEP;
	m_accelY = (velocityY - velocityYPrev) / TIMESTEP;
}

void
BuffaloByte::SetMovementState(enum BuffaloByte::MovementState movementState){
	m_movementState = movementState;
}

void
BuffaloByte::SetInitialPosition(std::tuple<double, double> pos){
	m_x = std::get<0>(pos);
	m_y = std::get<1>(pos);
}

double
BuffaloByte::GetAccelX(){
	return m_accelX;
}

double
BuffaloByte::GetAccelY(){
	return m_accelY;
}

double
BuffaloByte::GetPosX(){
	return m_x;
}

double
BuffaloByte::GetPosY(){
	return m_y;
}

double
BuffaloByte::GetAngle(){
	return m_angle;
}
