#include "LoadBalancer.h"

/**
 * @brief Initializes the load balancer, clock, and starting server pool.
 */
LoadBalancer::LoadBalancer(int initialServers) {
	systemTime = 0;
	nextServerID = 1;
	 blockedIPRange = "";
	blockedCount = 0;

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
	if (!blockedIPRange.empty() && r.ip_in.find(blockedIPRange) == 0) {
		blockedCount++;
		return; 
	}
	requests.push(r);
}

/**
 * @brief Core loop logic matching the flowchart: distributes jobs and ticks the clock.
 */
void LoadBalancer::distributeRequest() {
	for (size_t i = 0; i < servers.size(); ++i) {
		if (!servers[i].isBusy() && !requests.empty()) {
			servers[i].addRequest(requests.front());
			requests.pop();
		}
		servers[i].iterate(); 
	}
	systemTime++;
}

/**
 * @brief Handles dynamic allocation and deallocation (scaling).
 */
void LoadBalancer::balanceServers() {
	int qSize = requests.size();
	int sSize = servers.size();

	if (sSize == 0) return; 

	if (qSize > (sSize * 80)) {
		servers.push_back(WebServer(nextServerID++));
	} 
	else if (qSize < (sSize * 50) && sSize > 1) {
		servers.pop_back();
	}
}

int LoadBalancer::getSystemTime() const { return systemTime; }
int LoadBalancer::getQueueSize() const { return requests.size(); }
int LoadBalancer::getServerCount() const { return servers.size(); }
int LoadBalancer::getBlockedCount() const { return blockedCount; }