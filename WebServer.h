#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

/**
 * @class WebServer
 * @brief Represents an individual processing unit in the load balancer system.
 * * The WebServer takes a single Request from the LoadBalancer, processes it over 
 * a set number of clock cycles (time), and alerts the system when it is available 
 * for a new job.
 */
class WebServer {
private:
	Request currentRequest; ///< The current request being processed.
	int timeLeft;           ///< The remaining clock cycles needed to finish the request.
	int serverID;           ///< A unique identifier for logging purposes.

public:
	/**
	 * @brief Constructor for the WebServer.
	 * @param id The unique identifier for this server instance (defaults to 0).
	 */
	WebServer(int id = 0);

	/**
	 * @brief Assigns a new request to the server to process.
	 * @param r The Request object to be processed.
	 */
	void addRequest(Request r);

	/**
	 * @brief Checks if the server is currently processing a request.
	 * @return true if the server is busy (timeLeft > 0), false otherwise.
	 */
	bool isBusy() const;

	/**
	 * @brief Simulates one clock cycle of processing.
	 * Decrements the timeLeft of the current request by 1 if the server is busy.
	 */
	void iterate();

	/**
	 * @brief Retrieves the server's unique ID.
	 * @return The integer ID of the server.
	 */
	int getServerID() const;

	/**
	 * @brief Retrieves the current request being processed.
	 * @return The Request object currently held by the server.
	 */
	Request getCurrentRequest() const;
};

#endif // WEBSERVER_H