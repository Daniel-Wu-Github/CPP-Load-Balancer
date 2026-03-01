/**
 * @file switch.cpp
 * @brief Switch-mode simulation entry point: routes requests to two LoadBalancers.
 */

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
const std::string MAGENTA = "\033[35m";

int main() {
    srand(static_cast<unsigned int>(time(0)));

    int numServers, runTime;
    std::cout << CYAN << "=== Multi-Load Balancer Simulation (Switch Mode) ===" << RESET << std::endl;
    std::cout << "Enter the total initial number of servers (e.g., 10): ";
    std::cin >> numServers;
    std::cout << "Enter the time to run in clock cycles (e.g., 10000): ";
    std::cin >> runTime;

    int pServers = numServers / 2;
    int sServers = numServers - pServers;

    LoadBalancer processLB(pServers);
    LoadBalancer streamLB(sServers);

    processLB.setBlockedIPRange("192.168.");
    streamLB.setBlockedIPRange("192.168.");

    std::ofstream logFile("log.txt");
    if (!logFile.is_open()) {
        std::cerr << RED << "Error: Could not open log.txt for writing." << RESET << std::endl;
        return 1;
    }

    std::string initMsg = "Starting Switch Simulation with " + std::to_string(numServers) + " total servers for " + std::to_string(runTime) + " cycles.\n" +
                          "-> Processing Balancer allocated " + std::to_string(pServers) + " servers.\n" +
                          "-> Streaming Balancer allocated " + std::to_string(sServers) + " servers.\n";
    std::cout << YELLOW << initMsg << RESET;
    logFile << "=== SIMULATION START ===\n" << initMsg;

    int initialQueueSize = numServers * 100;
    for (int i = 0; i < initialQueueSize; ++i) {
        Request r;
        if (r.jobType == 'P') processLB.addRequest(r);
        else streamLB.addRequest(r);
    }

    int prevPServers = pServers;
    int prevSServers = sServers;

    for (int cycle = 0; cycle < runTime; ++cycle) {
        int totalActiveServers = processLB.getServerCount() + streamLB.getServerCount();
        int newRequests = rand() % (totalActiveServers * 2 + 1);

        for (int i = 0; i < newRequests; ++i) {
            Request r;
            if (r.jobType == 'P') processLB.addRequest(r);
            else streamLB.addRequest(r);
        }

        processLB.distributeRequest();
        streamLB.distributeRequest();

        processLB.balanceServers();
        streamLB.balanceServers();

        int currPServers = processLB.getServerCount();
        if (currPServers > prevPServers) {
            std::string msg = "Cycle " + std::to_string(cycle) + ": [Process LB] Scaled UP to " + std::to_string(currPServers) + " servers.\n";
            std::cout << GREEN << msg << RESET;
            logFile << msg;
        } else if (currPServers < prevPServers) {
            std::string msg = "Cycle " + std::to_string(cycle) + ": [Process LB] Scaled DOWN to " + std::to_string(currPServers) + " servers.\n";
            std::cout << RED << msg << RESET;
            logFile << msg;
        }
        prevPServers = currPServers;

        int currSServers = streamLB.getServerCount();
        if (currSServers > prevSServers) {
            std::string msg = "Cycle " + std::to_string(cycle) + ": [Stream LB] Scaled UP to " + std::to_string(currSServers) + " servers.\n";
            std::cout << GREEN << msg << RESET;
            logFile << msg;
        } else if (currSServers < prevSServers) {
            std::string msg = "Cycle " + std::to_string(cycle) + ": [Stream LB] Scaled DOWN to " + std::to_string(currSServers) + " servers.\n";
            std::cout << RED << msg << RESET;
            logFile << msg;
        }
        prevSServers = currSServers;
    }

    int totalBlocked = processLB.getBlockedCount() + streamLB.getBlockedCount();

    std::string finalMsg = "\n=== SIMULATION COMPLETE ===\n"
                           "Final Time: " + std::to_string(processLB.getSystemTime()) + " cycles.\n"
                           "Task Time Range Configured: 5 to 500 clock cycles.\n"
                           "Total Requests Blocked by Firewall: " + std::to_string(totalBlocked) + " requests.\n\n"
                           "--- PROCESSING LOAD BALANCER STATS ---\n"
                           "Final Queue Size: " + std::to_string(processLB.getQueueSize()) + " requests.\n"
                           "Final Active Servers: " + std::to_string(processLB.getServerCount()) + " servers.\n\n"
                           "--- STREAMING LOAD BALANCER STATS ---\n"
                           "Final Queue Size: " + std::to_string(streamLB.getQueueSize()) + " requests.\n"
                           "Final Active Servers: " + std::to_string(streamLB.getServerCount()) + " servers.\n";

    std::cout << CYAN << finalMsg << RESET;
    logFile << finalMsg;

    logFile.close();
    return 0;
}
