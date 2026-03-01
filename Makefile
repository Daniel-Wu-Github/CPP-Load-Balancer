# Makefile for CPP Load Balancer

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g

SRCS = main.cpp Request.cpp WebServer.cpp LoadBalancer.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = loadbalancer

.PHONY: all clean run

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	-rm -f $(OBJS) $(EXEC) log.txt

run: $(EXEC)
	./$(EXEC)

switch: switch.o Request.o WebServer.o LoadBalancer.o
	$(CXX) $(CXXFLAGS) -o loadbalancer_switch switch.o Request.o WebServer.o LoadBalancer.o

run-switch: switch
	./loadbalancer_switch