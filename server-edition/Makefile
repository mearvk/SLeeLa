CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -pthread
TARGET=sleelas
SOURCES=sleela_server.cpp server.cpp packet_logger_bridge.cpp ../logger/sleela_logger.cpp
all: $(TARGET)
$(TARGET): $(SOURCES) sleela_server.hpp
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES)
clean:
	rm -f $(TARGET) *.o
.PHONY: all clean
