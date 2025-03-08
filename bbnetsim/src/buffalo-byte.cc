#include "buffalo-byte.h"

#include <cmath>
#include <numbers>

void
BuffaloByte::Update(){
	// Update the speed based on motor state
	double speedPrev = m_speed;
	if(m_motorL || m_motorR){
		m_speed += ACCEL * TIMESTEP;
		m_speed = m_speed > MAX_SPEED ? MAX_SPEED : m_speed;
	}
	else{
		m_speed -= ACCEL * TIMESTEP;
		m_speed = m_speed < 0 ? 0 : m_speed;
	}

	// Update the angle based on motor state
	double anglePrev = m_angle;
	if(m_motorL && !m_motorR){
		m_angle += ANGULAR_VELOCITY * TIMESTEP;
		m_angle = m_angle >= 360 ? m_angle - 360 : m_angle;
	}
	if(!m_motorL && m_motorR){
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
