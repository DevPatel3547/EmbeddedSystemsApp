# Makefile for building the system_monitor daemon

CXX = g++
CXXFLAGS = -std=c++11 -pthread -Wall
TARGET = system_monitor

all: $(TARGET)

$(TARGET): system_monitor.cpp
	$(CXX) $(CXXFLAGS) system_monitor.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)
