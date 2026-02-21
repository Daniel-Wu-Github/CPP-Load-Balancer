#include "Request.h"
#include <cstdlib> // For rand()

/**
 * @brief Generates a random IPv4 address string.
 */
std::string generateRandomIP() {
	// Generate 4 random octets between 0 and 255
	std::string ip = std::to_string(rand() % 256) + "." +
					 std::to_string(rand() % 256) + "." +
					 std::to_string(rand() % 256) + "." +
					 std::to_string(rand() % 256);
	return ip;
}

/**
 * @brief Constructs a new Request object with randomized data.
 */
Request::Request() {
	ip_in = generateRandomIP();
	ip_out = generateRandomIP();
    
	// Generate a random processing time between 5 and 500 clock cycles
	time = (rand() % 496) + 5; 
    
	// Randomly assign 'P' (Processing) or 'S' (Streaming)
	jobType = (rand() % 2 == 0) ? 'P' : 'S';
}