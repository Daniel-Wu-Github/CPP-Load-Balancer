# Placeholder MakefileCXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

SRCS = main.cpp Request.cpp WebServer.cpp LoadBalancer.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = loadbalancer

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC) log.txt