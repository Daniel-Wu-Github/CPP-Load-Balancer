#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <queue>
#include <vector>
#include <string>
#include "WebServer.h"
#include "Request.h"

/**
 * @class LoadBalancer
 * @brief Manages the request queue and routes jobs to an active pool of web servers.
 * * The LoadBalancer handles dynamic scaling by provisioning and de-provisioning 
 * WebServers based on the current queue size. It also features a firewall mechanism 
 * to drop requests from blocked IP ranges.
 */
class LoadBalancer {
private:
	std::queue<Request> requests;   ///< Queue holding all pending requests.
	std::vector<WebServer> servers; ///< Dynamic pool of active web servers.
	int systemTime;                 ///< Current clock cycle of the simulation.
	int nextServerID;               ///< Tracker to assign unique IDs to new servers.
	std::string blockedIPRange;     ///< IP prefix to drop (Firewall).

public:
	/**
	 * @brief Constructs a LoadBalancer with a starting number of servers.
	 * @param initialServers The number of WebServers to start with.
	 */
	LoadBalancer(int initialServers);

	/**
	 * @brief Sets an IP prefix to block (e.g., "192.168.").
	 * @param ipPrefix The starting substring of IPs to reject.
	 */
	void setBlockedIPRange(std::string ipPrefix);

	/**
	 * @brief Adds a new request to the queue, subject to firewall rules.
	 * @param r The request to add.
	 */
	void addRequest(Request r);

	/**
	 * @brief Iterates through servers, assigning queued requests to idle ones, 
	 * and advances the simulation clock by 1.
	 */
	void distributeRequest();

	/**
	 * @brief Dynamically scales the server pool based on queue size.
	 * Adds a server if queue > servers * 80.
	 * Removes a server if queue < servers * 50.
	 */
	void balanceServers();

	/**
	 * @brief Retrieves the current system time (clock cycle).
	 * @return Integer representing system time.
	 */
	int getSystemTime() const;

	/**
	 * @brief Retrieves the current number of pending requests.
	 * @return Integer representing queue size.
	 */
	int getQueueSize() const;

	/**
	 * @brief Retrieves the current number of active servers.
	 * @return Integer representing the active server pool size.
	 */
	int getServerCount() const;
};

#endif // LOADBALANCER_H