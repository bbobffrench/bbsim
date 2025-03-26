#include "mesh-network.h"

#include <ns3/mobility-module.h>

#include <algorithm>

using namespace ns3;

MeshNetwork::MeshNetwork(int numNodes, int ttl, int blacklistLen){
	m_numNodes = numNodes;
	m_ttl = ttl;
	m_blacklistLen = blacklistLen;
	m_sentPackets = 0;
	m_sentSeqs.resize(numNodes);
	m_blacklists.resize(numNodes);

	m_mainNode.Create(1);
	m_childNodes.Create(numNodes);

	// Position the main node at the origin
	MobilityHelper constMobility;
	constMobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	auto constPosAlloc = Create<ListPositionAllocator>();
	constPosAlloc->Add(Vector(0, 0, 0));
	constMobility.SetPositionAllocator(constPosAlloc);
	constMobility.Install(m_mainNode);

	// Use the constant velocity model for child nodes
	MobilityHelper mobility;
	mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel");

	// Choose range of distances
	auto distanceRange = CreateObject<UniformRandomVariable>();
	distanceRange->SetAttribute("Min", DoubleValue(0));
	distanceRange->SetAttribute("Max", DoubleValue(5 * numNodes));

	// Choose range of angles
	auto angleRange = CreateObject<UniformRandomVariable>();
	angleRange->SetAttribute("Min", DoubleValue(0));
	angleRange->SetAttribute("Max", DoubleValue(360));

	// Randomly place child nodes across assigned range
	auto randomPosAlloc = Create<RandomDiscPositionAllocator>();
	randomPosAlloc->SetRho(distanceRange);
	randomPosAlloc->SetTheta(angleRange);
	randomPosAlloc->SetX(0);
	randomPosAlloc->SetY(0);
	mobility.SetPositionAllocator(randomPosAlloc);
	mobility.Install(m_childNodes);

	// Set the Wi-Fi standard to 802.11b
	WifiHelper wifi;
	wifi.SetStandard(WIFI_STANDARD_80211b);

	// Create a 2.4Ghz Wi-Fi channel
	YansWifiChannelHelper channel;
	channel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
	channel.AddPropagationLoss(
		"ns3::LogDistancePropagationLossModel",
		"Exponent", DoubleValue(2), // Assume ideal conditions with minimal obstructions
		"ReferenceLoss", DoubleValue(40) // Set reference loss for 2.4Ghz at 1m distance
	);

	// Configure the Wi-Fi PHY layer with values from ESP8266 and AN9520 datasheets
	YansWifiPhyHelper phy;
	phy.Set("TxPowerStart", DoubleValue(20));
	phy.Set("TxPowerEnd", DoubleValue(20));
	phy.Set("TxGain", DoubleValue(1.5));
	phy.Set("RxGain", DoubleValue(1.5));
	phy.Set("EnergyDetectionThreshold", DoubleValue(-91));
	phy.SetChannel(channel.Create());

	// Configure the Wi-Fi MAC layer in ad-hoc mode
	WifiMacHelper mac;
	mac.SetType("ns3::AdhocWifiMac");

	// Install Wi-Fi network devices with the configured PHY and MAC layers, on all nodes
	m_mainNodeDevice = wifi.Install(phy, mac, m_mainNode);
	m_childDevices = wifi.Install(phy, mac, m_childNodes);

	// Set the receive callback for all nodes
	std::function callback = [this](
		Ptr<NetDevice> device,
		Ptr<const Packet> packet,
		uint16_t protocol,
		const Address &address) -> bool
	{
		ReceivePacket(device, packet);
		return true;
	};

	m_mainNodeDevice.Get(0)->SetReceiveCallback(callback);
	for(int i = 0; i < numNodes; i++)
		m_childDevices.Get(i)->SetReceiveCallback(callback);
}

void
MeshNetwork::SetVelocity(uint16_t index, double xSpeed, double ySpeed){
	auto mobilityModel = m_childNodes.Get(index)->GetObject<ConstantVelocityMobilityModel>();
	mobilityModel->SetVelocity(Vector(xSpeed, ySpeed, 0));
}

void 
MeshNetwork::ForwardPacket(uint16_t index, uint16_t sourceIndex, uint16_t seq, uint16_t ttl){
	// Create buffer large enough to store header and payload
	uint8_t data[PAYLOAD_SIZE + 3 * sizeof(uint16_t)];

	// Add header information
	data[0] = sourceIndex >> 8;
	data[1] = sourceIndex;
	data[2] = seq >> 8;
	data[3] = seq;
	data[4] = ttl >> 8;
	data[5] = ttl;

	// Create and send the packet
	Ptr<Packet> packet = Create<Packet>((uint8_t *)data, sizeof(data));
	Ptr<NetDevice> device = m_childDevices.Get(index);
	device->Send(packet, device->GetBroadcast(), 0);
}

void
MeshNetwork::SendPacket(uint16_t sourceIndex){
	// Send a packet with the current sequence number of the source index, and the initial TTL
	ForwardPacket(sourceIndex, sourceIndex, m_sentSeqs[sourceIndex]++, m_ttl);
	m_sentPackets++;

	// Schedule the next packet to be sent
	Simulator::Schedule(
		Seconds(PACKET_INTERVAL),
		[this, sourceIndex](){ SendPacket(sourceIndex); }
	);
}

void
MeshNetwork::ReceivePacket(Ptr<NetDevice> device, Ptr<const Packet> packet){
	// Copy packet data to a data buffer
	uint8_t data[PAYLOAD_SIZE + 4 * sizeof(uint16_t)];
	packet->CopyData(data, sizeof(data));

	// Extract header information
	uint16_t sourceIndex = (data[0] << 8) + data[1];
	uint16_t seq = (data[2] << 8) + data[3];
	uint16_t ttl = (data[4] << 8) + data[5];

	auto pair = std::make_tuple(sourceIndex, seq);

	if(device != m_mainNodeDevice.Get(0)){
		// Get the index of the node on which the packet was received
		int index = 0;
		for(; index < m_numNodes; index++)
			if(m_childDevices.Get(index) == device) break;

		auto &blacklist = m_blacklists[index];

		// Don't forward the packet if it is blacklisted
		if(std::find(blacklist.begin(), blacklist.end(), pair) != blacklist.end())
			return;

		// Don't forward the packet if the forwarding index is the original sender
		if(sourceIndex == index)
			return;

		// Forward the packet with a decremented TTL if TTL > 0
		if(ttl > 0) ForwardPacket(index, sourceIndex, seq, --ttl);

		// Add packet to the blacklist after sending
		blacklist.insert(blacklist.begin(), pair);
		if (blacklist.size() > m_blacklistLen) blacklist.pop_back();
	}
	else m_receivedPackets.insert(m_receivedPackets.begin(), pair);
}

std::vector<std::tuple<double, double>>
MeshNetwork::GetNodePositions(){
	std::vector<std::tuple<double, double>> positions(m_numNodes);
	for(int i = 0; i < m_numNodes; i++){
		Vector pos = m_childNodes.Get(i)->GetObject<MobilityModel>()->GetPosition();
		positions[i] = std::make_tuple(pos.x, pos.y);
	}
	return positions;
}

int
MeshNetwork::GetSentPackets(){
	return m_sentPackets;
}

int
MeshNetwork::GetReceivedPackets(){
	// Remove duplicate packets
	m_receivedPackets.sort();
	m_receivedPackets.unique();
	return m_receivedPackets.size();
}

int
main(int argc, char **argv){
	return 0;
}
