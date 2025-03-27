#ifndef BBSIM_H
#define BBSIM_H

#include "mesh-network.h"
#include "buffalo-byte.h"

double RunSimulation(
	int seconds,        // Simulation runtime in seconds
	int numNodes,       // Number of child nodes
	uint16_t ttl,       // Time to leave for all sent packets
	int blacklistLen,   // Length of the forwarding blacklist for all nodes
	int packetInterval, // Interval in seconds at which packets are sent
	bool animate        // Plot results with NetAnim if true
);

#endif
