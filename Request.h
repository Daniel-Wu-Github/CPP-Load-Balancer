#ifndef REQUEST_H
#define REQUEST_H

#include <string>

/**
 * @struct Request
 * @brief A struct representing a single network request.
 * * This lower-level data structure encapsulates all the necessary information 
 * for a simulated network job, including source and destination IP addresses, 
 * the processing time required, and the type of job (Processing or Streaming).
 */
struct Request {
	std::string ip_in;  ///< The source IP address of the requester.
	std::string ip_out; ///< The destination IP address for the results.
	int time;           ///< The processing time required (in clock cycles).
	char jobType;       ///< The type of job: 'P' for Processing, 'S' for Streaming.

	/**
	 * @brief Default constructor for Request.
	 * * Automatically generates random IP addresses, a random required processing time, 
	 * and a random job type upon instantiation.
	 */
	Request();
};

/**
 * @brief Helper function to generate a random IPv4 address.
 * @return A std::string formatted as "X.X.X.X" where X is 0-255.
 */
std::string generateRandomIP();

#endif // REQUEST_H