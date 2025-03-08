#include "mesh-network.h"

#include <iostream>

#include <ns3/propagation-module.h>
#include <ns3/internet-module.h>
#include <ns3/mobility-module.h>

using namespace ns3;

WifiHelper MeshNode::s_wifi;
YansWifiPhyHelper MeshNode::s_phy;
YansWifiChannelHelper MeshNode::s_channel;
WifiMacHelper MeshNode::s_mac;

uint8_t MeshNode::s_ttl;
int MeshNode::s_packetSize;
uint16_t MeshNode::s_curIndex = 0;

void
MeshNode::InitNetworkParams(int ttl, int packetSize){
	// Set TTL and packet size for packets sent over the network
	s_ttl = ttl;
	s_packetSize = packetSize;

	// Use the 802.11b Wi-Fi standard (used by ESP8266)
	s_wifi.SetStandard(WIFI_STANDARD_80211b);

	// Use the log-distance path loss model (may change)
	YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
	channel.AddPropagationLoss("ns3::LogDistancePropagationLossModel");

	// Set up the physical layer to replicate the ESP8266 as closely as possible
	s_phy.SetChannel(channel.Create());
	s_phy.Set("TxPowerStart", DoubleValue(20)); // 20 dBm transmitter power

	// Set up the MAC layer, use ad hoc for the mesh network
	s_mac.SetType("ns3::AdhocWifiMac");
}

MeshNode::MeshNode(){
	// Assign a unique node index
	m_index = s_curIndex++;

	// Create the node and install the Wi-Fi device on it
	m_node.Create(1);
	m_dev = s_wifi.Install(s_phy, s_mac, m_node);

	// Use the constant velocity mobility model
	MobilityHelper mobility;
	mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel");
	mobility.Install(m_node.Get(0));

	// Configure MAC broadcast address
	PacketSocketAddress broadcastAddr;
	broadcastAddr.SetSingleDevice(m_dev.Get(0)->GetIfIndex());
	broadcastAddr.SetPhysicalAddress(Mac48Address("ff:ff:ff:ff:ff:ff"));
	broadcastAddr.SetProtocol(1); // Required to set but has no effect

	// Create a socket to send/receive raw sockets on the MAC broadcast address
	PacketSocketHelper packetSocket;
	packetSocket.Install(m_node.Get(0));
	m_socket = Socket::CreateSocket(
		m_node.Get(0),
		SocketFactory::GetTypeId()
	);
	m_socket->Bind(broadcastAddr);
	m_socket->SetRecvCallback(MakeCallback(&MeshNode::ReceivePacket, this));
}

void
MeshNode::SetVelocity(double velocityX, double velocityY){
	Ptr<ConstantVelocityMobilityModel> mobilityModel =
		m_node.Get(0)->GetObject<ConstantVelocityMobilityModel>();
	mobilityModel->SetVelocity(Vector(velocityX, velocityY, 0));
}

void
MeshNode::SendPacket(uint16_t destIndex, uint8_t ttl, uint16_t sourceIndex, uint16_t packetIndex){
	// Create data buffer storing source and destination index, TTL, and packet index
	uint8_t data[s_packetSize];
	data[0] = destIndex >> 8;
	data[1] = destIndex - data[0] << 8;
	data[2] = ttl;
	data[3] = sourceIndex >> 8;
	data[4] = sourceIndex - data[3] << 8;
	data[5] = packetIndex >> 8;
	data[6] = packetIndex - data[5] << 8;
	
	// Create and send a socket holding the data buffer
	Ptr<Packet> packet = Create<Packet>(
		reinterpret_cast<const uint8_t*>(data),
		s_packetSize
	);
	m_socket->Send(packet);
}

void
MeshNode::SendPacket(uint16_t destIndex){
	SendPacket(destIndex, s_ttl, m_index, m_packetIndex++);
}

void
MeshNode::ReceivePacket(Ptr<Socket> socket){
	// Get the packet buffer and extract information
	Ptr<Packet> packet = socket->Recv();
	uint8_t data[s_packetSize];
	packet->CopyData(data, s_packetSize);
	uint16_t destIndex = (data[0] << 8) + data[1];
	uint8_t ttl = data[2];
	uint16_t sourceIndex = data[3] << 8 + data[4];
	uint16_t packetIndex = data[5] << 8 + data[6];

	// Store identifying packet information if this is the destination node
	if(destIndex == m_index){
		std::cout << "Packet received on node " << m_index;
		std::cout << " from node " << sourceIndex << std::endl;
		m_receivedPackets.push_back(std::make_tuple(sourceIndex, packetIndex));
		return;
	}

	// Forward the packet until TTL goes down to 0
	if(ttl > 0) SendPacket(destIndex, ttl - 1, sourceIndex, packetIndex);
}
