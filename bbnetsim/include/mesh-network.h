#ifndef MESH_NETWORK_H
#define MESH_NETWORK_H

#include <ns3/core-module.h>
#include <ns3/wifi-module.h>

#include <vector>
#include <list>
#include <tuple>

class MeshNetwork{
public:
	MeshNetwork(int numNodes, int ttl, int blacklistLen);
	void SendPacket(uint16_t sourceIndex);
	void SetPosition(uint16_t index, double x, double y);
	std::vector<std::tuple<double, double>> GetNodePositions();
	int GetSentPackets();
	int GetReceivedPackets();
private:
 	void ReceivePacket(ns3::Ptr<NetDevice>, ns3::Ptr<const ns3::Packet>);
	void ForwardPacket(uint16_t index, uint16_t sourceIndex, uint16_t seq, uint16_t ttl);

	const int PAYLOAD_SIZE = 5;
	const int PACKET_INTERVAL = 5;

	int m_numNodes;
	int m_ttl;

	ns3::NodeContainer m_childNodes;
	ns3::NodeContainer m_mainNode;
	ns3::NetDeviceContainer m_mainNodeDevice;
	ns3::NetDeviceContainer m_childDevices;

	// For tracking/updating sequence numbers for each node
	std::vector<uint8_t> m_sentSeqs;

	// Total number of packets sent by child nodes to the main node
	int m_sentPackets;

	// A list of (sequence number, source index) pairs for each node, that should not be forwarded
	int m_blacklistLen;
	std::vector<std::list<std::tuple<uint16_t, uint16_t>>> m_blacklists;

	// A list of all (sequence number, source index) pairs that have been received by the main node
	std::list<std::tuple<uint16_t, uint16_t>> m_receivedPackets; 
};

#endif
