#include "WebServer.h"

/**
 * @brief Initializes the server as idle (timeLeft = 0) and sets its ID.
 */
WebServer::WebServer(int id) {
	serverID = id;
	timeLeft = 0; // 0 means the server is currently free
}

/**
 * @brief Takes a request and sets the processing countdown to the request's time.
 */
void WebServer::addRequest(Request r) {
	currentRequest = r;
	timeLeft = r.time;
}

/**
 * @brief Returns true if there is still time left on the current request.
 */
bool WebServer::isBusy() const {
	return timeLeft > 0;
}

/**
 * @brief Processes the request by reducing the remaining time by 1 clock cycle.
 */
void WebServer::iterate() {
	if (timeLeft > 0) {
		timeLeft--;
	}
}

/**
 * @brief Gets the server ID.
 */
int WebServer::getServerID() const {
	return serverID;
}

/**
 * @brief Gets the current request.
 */
Request WebServer::getCurrentRequest() const {
	return currentRequest;
}