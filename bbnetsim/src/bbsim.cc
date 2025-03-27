#include <ns3/netanim-module.h>

#include "bbsim.h"

#include <cstdlib>
#include <vector>
#include <iostream>

using namespace ns3;

static void
PopulatePositions(MeshNetwork &mesh, std::vector<BuffaloByte> &bb){
	// Retrieve positions allocated during mesh network initialization
	std::vector<std::tuple<double, double>> positions = mesh.GetNodePositions();

	// Store initial position of every node in its corresponding BB
	for(int i = 0; i < bb.size(); i++) bb[i].SetInitialPosition(positions[i]);
}

static void
StepMovement(MeshNetwork *mesh, std::vector<BuffaloByte> *bb){
	for(int i = 0; i < (*bb).size(); i++){
		(*bb)[i].Update(); // Update movement simulation by one timestep

		// Update node positions in the mesh network
		mesh->SetPosition(i, (*bb)[i].GetPosX(), (*bb)[i].GetPosY());
	}

	// Schedule the next step
	Simulator::Schedule(Seconds(BuffaloByte::TIMESTEP), StepMovement, mesh, bb);
}

static void
UpdateMovementStates(std::vector<BuffaloByte> *bb){
	// Randomly change the movement state for each node
	for(int i = 0; i < (*bb).size(); i++)
		(*bb)[i].SetMovementState((BuffaloByte::MovementState)(std::rand() % 2));
	
	// Schedule the next movement state update
	Simulator::Schedule(Seconds(1), UpdateMovementStates, bb);
}

static void
SendPacket(MeshNetwork *mesh, int index, int interval){
	// Send a packet on the specified node
	mesh->SendPacket(index);

	// Schedule the next packet send
	Simulator::Schedule(Seconds(interval), SendPacket, mesh, index, interval);
}

double
RunSimulation(int seconds, int numNodes, uint16_t ttl, int blacklistLen, int packetInterval){
	// Create buffalo bytes
	std::vector<BuffaloByte> bb(numNodes);

	// Create mesh network
	MeshNetwork mesh(numNodes, ttl, blacklistLen);

	// Copy initial mesh node positions to their corresponding BB
	PopulatePositions(mesh, bb);

	// Start movement simulation
	Simulator::Schedule(Seconds(0), StepMovement, &mesh, &bb);
	Simulator::Schedule(Seconds(0), UpdateMovementStates, &bb);

	// Start sending packets
	for(int i = 0; i < numNodes; i++)
		Simulator::Schedule(
			Seconds(std::rand() % packetInterval),
			SendPacket, &mesh, i, packetInterval
		);

	// Tell the simulation when to end
	Simulator::Stop(Seconds(seconds));

	// Start the simulation
	Simulator::Run();

	int sent = mesh.SentPackets();
	int received = mesh.ReceivedPackets();
	return (double)(sent - received) / sent * 100;
}
