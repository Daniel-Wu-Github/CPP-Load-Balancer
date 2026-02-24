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