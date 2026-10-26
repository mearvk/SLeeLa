CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -pthread
LDLIBS ?= -lssl -lcrypto
TARGET=sleelas
SOURCES=sleela_server.cpp server.cpp packet_logger_bridge.cpp ../logger/sleela_logger.cpp
all: $(TARGET)

TEST_TARGET=server_protocol_test
TEST_SOURCES=sleela_server.cpp packet_logger_bridge.cpp ../logger/sleela_logger.cpp tests/server_protocol_test.cpp
$(TARGET): $(SOURCES) sleela_server.hpp
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES) $(LDLIBS)
$(TEST_TARGET): $(TEST_SOURCES) sleela_server.hpp
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_SOURCES) $(LDLIBS)
test: $(TEST_TARGET)
	./$(TEST_TARGET)
clean:
	rm -f $(TARGET) $(TEST_TARGET) *.o
.PHONY: all test clean
