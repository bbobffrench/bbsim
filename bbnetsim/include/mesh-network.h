#ifndef MESH_NETWORK_H
#define MESH_NETWORK_H

#include <cstdint>
#include <tuple>
#include <list>

#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/wifi-module.h>

class MeshNode{
public:
	static void InitNetworkParams(int ttl, int packetSize);

	MeshNode();
	void SetVelocity(double velocityX, double velocityY);
	void SendPacket(uint16_t destIndex);
private:
	// Default TTL and packet size values for all packets in the network
	static uint8_t s_ttl;
	static int s_packetSize;

	// Used for assigning a unique index to each node
	static uint16_t s_curIndex;

	// Helpers to be installed all nodes in the network
	static ns3::WifiHelper s_wifi;
	static ns3::YansWifiPhyHelper s_phy;
	static ns3::YansWifiChannelHelper s_channel;
	static ns3::WifiMacHelper s_mac;

	// Unique node index, used for identifying packet destination
	uint16_t m_index;

	// The current packet index, incremented after every packet sent
	uint16_t m_packetIndex; 

	// ns-3 node, device, and socket
	ns3::NodeContainer m_node;
	ns3::NetDeviceContainer m_dev;
	ns3::Ptr<ns3::Socket> m_socket;

	// Store a list of received packet identifiers (source node and packet index)
	std::list<std::tuple<uint16_t, uint16_t>> m_receivedPackets;

	void SendPacket(uint16_t destIndex, uint8_t ttl, uint16_t sourceIndex, uint16_t packetIndex);
	void ReceivePacket(ns3::Ptr<ns3::Socket> socket);
};

#endif
