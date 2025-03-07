#include "mesh-network.h"

int
main(int argc, char **argv){
	MeshNode::InitNetworkParams(10, 8);
	MeshNode node;
	return 0;
}
