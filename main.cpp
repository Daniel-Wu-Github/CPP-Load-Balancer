#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "LoadBalancer.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "LoadBalancer.h"

const std::string RESET = "\033[0m";
const std::string GREEN = "\033[32m";
const std::string RED = "\033[31m";
const std::string CYAN = "\033[36m";
const std::string YELLOW = "\033[33m";

int main() {
    srand(static_cast<unsigned int>(time(0)));
    
    int numServers, runTime;
    std::cout << CYAN << "=== Load Balancer Simulation ===" << RESET << std::endl;
    std::cout << "Enter the initial number of servers (e.g., 10): ";
    std::cin >> numServers;
    std::cout << "Enter the time to run the load balancer in clock cycles (e.g., 10000): ";
    std::cin >> runTime;

    LoadBalancer lb(numServers);
    lb.setBlockedIPRange("192.168."); // Simulating a blocked subnet

    std::ofstream logFile("log.txt");
    if (!logFile.is_open()) {
        std::cerr << RED << "Error: Could not open log.txt for writing." << RESET << std::endl;
        return 1;
    }

    std::string initMsg = "Starting simulation with " + std::to_string(numServers) + " servers for " + std::to_string(runTime) + " cycles.\n";
    std::cout << YELLOW << initMsg << RESET;
    logFile << "=== SIMULATION START ===\n" << initMsg;

    int initialQueueSize = numServers * 100;
    for (int i = 0; i < initialQueueSize; ++i) {
        lb.addRequest(Request());
    }
    
    std::cout << "Initial queue populated with " << initialQueueSize << " requests.\n";
    logFile << "Initial queue populated with " << initialQueueSize << " requests.\n";

    int previousServerCount = numServers;

    for (int cycle = 0; cycle < runTime; ++cycle) {
        
        int newRequests = rand() % (lb.getServerCount() * 2); 
        for(int i = 0; i < newRequests; ++i) {
            lb.addRequest(Request());
        }

        lb.distributeRequest();

        lb.balanceServers();

        int currentServerCount = lb.getServerCount();
        if (currentServerCount > previousServerCount) {
            std::string msg = "Cycle " + std::to_string(cycle) + ": " + 
                              "Queue large (" + std::to_string(lb.getQueueSize()) + "). " +
                              "Added a server. Total: " + std::to_string(currentServerCount) + "\n";
            std::cout << GREEN << msg << RESET;
            logFile << msg;
        } else if (currentServerCount < previousServerCount) {
            std::string msg = "Cycle " + std::to_string(cycle) + ": " + 
                              "Queue low (" + std::to_string(lb.getQueueSize()) + "). " +
                              "Removed a server. Total: " + std::to_string(currentServerCount) + "\n";
            std::cout << RED << msg << RESET;
            logFile << msg;
        }
        previousServerCount = currentServerCount;

        if (cycle % 1000 == 0 && cycle > 0) {
            logFile << "--- STATUS UPDATE (Cycle " << cycle << ") ---\n"
                    << "Active Servers: " << currentServerCount << "\n"
                    << "Pending Requests: " << lb.getQueueSize() << "\n--------------------------\n";
        }
    }

    std::string finalMsg = "\n=== SIMULATION COMPLETE ===\n"
                           "Final Time: " + std::to_string(lb.getSystemTime()) + " cycles.\n" +
                           "Final Queue Size: " + std::to_string(lb.getQueueSize()) + "\n" +
                           "Final Server Count: " + std::to_string(lb.getServerCount()) + "\n";
                           
    std::cout << CYAN << finalMsg << RESET;
    logFile << finalMsg;

    logFile.close();
    return 0;
}