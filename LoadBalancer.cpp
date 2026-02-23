#include "LoadBalancer.h"

/**
 * @brief Initializes the load balancer, clock, and starting server pool.
 */
LoadBalancer::LoadBalancer(int initialServers) {
	systemTime = 0;
	nextServerID = 1;
	blockedIPRange = ""; // Firewall disabled by default

	// Spin up the initial pool of servers
	for (int i = 0; i < initialServers; ++i) {
		servers.push_back(WebServer(nextServerID++));
	}
}

/**
 * @brief Configures the firewall IP blocker.
 */
void LoadBalancer::setBlockedIPRange(std::string ipPrefix) {
	blockedIPRange = ipPrefix;
}

/**
 * @brief Adds a request if it passes the firewall check.
 */
void LoadBalancer::addRequest(Request r) {
	// Firewall check: If the blocked prefix is set AND the incoming IP starts with it, drop it.
	if (!blockedIPRange.empty() && r.ip_in.find(blockedIPRange) == 0) {
		// Simulating a dropped DOS attack packet. We simply return without queuing.
		return; 
	}
	requests.push(r);
}

/**
 * @brief Core loop logic matching the flowchart: distributes jobs and ticks the clock.
 */
void LoadBalancer::distributeRequest() {
	for (size_t i = 0; i < servers.size(); ++i) {
		// If the server is free and we have waiting requests, assign it
		if (!servers[i].isBusy() && !requests.empty()) {
			servers[i].addRequest(requests.front());
			requests.pop();
		}
		// Tell the server to process 1 clock cycle
		servers[i].iterate(); 
	}
	systemTime++; // Advance the main simulation clock
}

/**
 * @brief Handles dynamic allocation and deallocation (scaling).
 */
void LoadBalancer::balanceServers() {
	int qSize = requests.size();
	int sSize = servers.size();

	// Prevent divide-by-zero or scaling when we have no servers
	if (sSize == 0) return; 

	// Scale UP: If queue is over 80x the server count
	if (qSize > (sSize * 80)) {
		servers.push_back(WebServer(nextServerID++));
	} 
	// Scale DOWN: If queue drops below 50x the server count (keeping at least 1 server alive)
	else if (qSize < (sSize * 50) && sSize > 1) {
		// For simulation purposes, we remove the last server in the vector
		servers.pop_back();
	}
}

// --- Getters for Logging ---
int LoadBalancer::getSystemTime() const { return systemTime; }
int LoadBalancer::getQueueSize() const { return requests.size(); }
int LoadBalancer::getServerCount() const { return servers.size(); }