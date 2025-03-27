#include "bbsim.h"

#include <cstdlib>
#include <ctime>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>

static void
PrintUsage(){
	std::cout << "Usage: bbsim -t -n -ttl -bl -p [-anim] [-csv]" << std::endl; 
}

int
main(int argc, char **argv){
	// Define all command line parameters
	enum Params{RUNTIME, NUM_NODES, TTL, BLACKLIST_LENGTH, PACKET_INTERVAL};

	// Map each command line parameters to its associated flag
	std::vector<std::string> paramFlags(5);
	paramFlags[RUNTIME] = "-t";
	paramFlags[NUM_NODES] = "-n";
	paramFlags[TTL] = "-ttl";
	paramFlags[BLACKLIST_LENGTH] = "-bl";
	paramFlags[PACKET_INTERVAL] = "-p";

	// Decode command line parameters
	int params[5] = {-1, -1, -1, -1, -1};
	bool animate = false, genCSV = false;
	for(int i = 1; i < argc; i++){
		if(std::strcmp(argv[i], "-anim") == 0) animate = true;
		else if(std::strcmp(argv[i], "-csv") == 0) genCSV = true;
		else{
			// Find parameter index
			int paramIndex = 0;
			for(; paramIndex < 5; paramIndex++)
				if(argv[i] == paramFlags[paramIndex]) break;

			// Give an error if an invalid parameter is provided
			if(paramIndex == 5){
				std::cerr << argv[i] << " is not a valid parameter" << std::endl;
				PrintUsage();
				return 1;
			}
			// Check if a value is supplied, exiting with an error if it is not
			if(i + 1 >= argc){
				std::cerr << "No value supplied for parameter " << argv[i] << std::endl;
				PrintUsage();
				return 1;
			}
			// Set the specified parameter
			params[paramIndex] = std::stoi(argv[++i]);
		}
	}

	// Ensure supplied parameters are valid
	if(params[RUNTIME] < 1){
		std::cerr << "Supplied runtime is not valid, set it with `-t'" << std::endl;
		return 1;
	}
	if(!genCSV && params[NUM_NODES] < 1){
		std::cerr << "Supplied node count is not valid, set it with `-n'" << std::endl;
		return 1;
	}
	if(!genCSV && params[TTL] < 0){
		std::cerr << "Supplied TTL is not valid, set it with `-ttl'" << std::endl;
		return 1;
	}
	if(!genCSV && params[BLACKLIST_LENGTH] < 0){
		std::cerr << "Supplied blacklist length is not valid, set it with `-bl'" << std::endl;
		return 1;
	}
	if(!genCSV && params[PACKET_INTERVAL] < 1){
		std::cerr << "Supplied packet send interval is not valid, set it with `-p'" << std::endl;
		return 1;
	}

	// Seed the random number generator
	std::srand(time(nullptr));

	// Run the appropriate simulation
	if(genCSV){
		MovementCSV(params[RUNTIME]);
		std::cout << "Simulation finished, saved to data.csv" << std::endl;
	}
	else{
		double packetLoss = RunSimulation(
			params[RUNTIME],
			params[NUM_NODES],
			params[TTL],
			params[BLACKLIST_LENGTH],
			params[PACKET_INTERVAL],
			animate
		);
		std::cout << packetLoss << std::endl;
	}
	return 0;
}
