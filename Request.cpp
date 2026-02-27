#include "Request.h"
#include <cstdlib>

/**
 * @brief Generates a random IPv4 address string.
 */
std::string generateRandomIP() {
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
    
	time = (rand() % 496) + 5; 
    
	jobType = (rand() % 2 == 0) ? 'P' : 'S';
}