#ifndef BBSIM_H
#define BBSIM_H

#include "mesh-network.h"
#include "buffalo-byte.h"

double RunSimulation(int seconds, int numNodes, uint16_t ttl, int blacklistLen, int packetInterval);

#endif
