#!/usr/bin/env python
# coding: utf-8

# In[2]:


import numpy as np
import matplotlib.pyplot as plt

# Time step
dt = 0.1  

# State vector: [x, y, theta, vx, vy]
x = np.zeros((5, 1))

# State transition matrix (F)
F = np.array([
    [1, 0, 0, dt, 0], 
    [0, 1, 0, 0, dt], 
    [0, 0, 1, 0, 0], 
    [0, 0, 0, 1, 0], 
    [0, 0, 0, 0, 1]
])

# Control input matrix (B)
B = np.array([
    [0.5 * dt**2, 0], 
    [0, 0.5 * dt**2], 
    [dt, 0], 
    [dt, 0], 
    [0, dt]
])

# Process noise covariance (Q)
Q = np.eye(5) * 0.01  

# Measurement matrix (H) - Assume we measure (x, y, theta)
H = np.array([
    [1, 0, 0, 0, 0], 
    [0, 1, 0, 0, 0], 
    [0, 0, 1, 0, 0]
])

# Measurement noise covariance (R)
R = np.eye(3) * 0.1  

# Initial state covariance matrix (P)
P = np.eye(5) * 1  

# Number of simulation steps
num_steps = 50  

# Generate random IMU measurements (acceleration & angular velocity)
ax_noise = np.random.normal(0, 0.2, num_steps)
ay_noise = np.random.normal(0, 0.2, num_steps)
omega_noise = np.random.normal(0, 0.1, num_steps)

# Storage for plotting
positions = []
measurements = []

# Initialize real-time plotting
plt.figure(figsize=(8, 6))
plt.ion()  # Enable interactive mode

# EKF loop
for i in range(num_steps):
    # Simulated IMU readings (control inputs)
    ax, ay, omega = ax_noise[i], ay_noise[i], omega_noise[i]
    u = np.array([[ax], [ay]])

    # Prediction step
    x = np.dot(F, x) + np.dot(B, u)
    P = np.dot(F, np.dot(P, F.T)) + Q

    # Measurement update (simulate noisy observations)
    z = np.dot(H, x) + np.random.multivariate_normal([0, 0, 0], R).reshape(-1, 1)

    # Compute Kalman gain
    S = np.dot(H, np.dot(P, H.T)) + R
    K = np.dot(P, np.dot(H.T, np.linalg.inv(S)))

    # Update state
    y = z - np.dot(H, x)  
    x = x + np.dot(K, y)
    P = np.dot(np.eye(5) - np.dot(K, H), P)

    # Store positions for plotting
    positions.append((x[0, 0], x[1, 0]))
    measurements.append((z[0, 0], z[1, 0]))

    # Print debug information
    print(f"Step {i + 1}:")
    print(f"  IMU Readings -> ax: {ax:.3f}, ay: {ay:.3f}, omega: {omega:.3f}")
    print(f"  Measured Position -> x: {z[0, 0]:.3f}, y: {z[1, 0]:.3f}")
    print(f"  Estimated Position -> x: {x[0, 0]:.3f}, y: {x[1, 0]:.3f}\n")

    # Real-time plotting
    plt.clf()
    plt.plot(*zip(*positions), marker="o", linestyle="-", label="Estimated Position (EKF)", color="blue")
    plt.plot(*zip(*measurements), marker="x", linestyle="--", label="Measured Position (Noisy)", color="red")
    plt.xlabel("X Position (m)")
    plt.ylabel("Y Position (m)")
    plt.title("IMU-Based EKF Position Estimation")
    plt.legend()
    plt.grid()
    plt.pause(0.1)

plt.ioff()  # Disable interactive mode
plt.show()





