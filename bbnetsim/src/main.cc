#include "mesh-network.h"

int
main(int argc, char **argv){
	MeshNode::InitNetworkParams(10, 8); // TTL = 10 hops, packet size = 8 bytes
	MeshNode centralNode;


	return 0;
}
