CXX ?= c++
CPPFLAGS ?=
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -pthread
LDFLAGS ?=
PYTHON ?= python3
SERVER_TEST_BINARY = ./$(TARGET)
ifeq ($(OS),Windows_NT)
SERVER_TEST_BINARY = ./$(TARGET).exe
endif
LDLIBS ?= -lssl -lcrypto
TARGET=sleelas
SOURCES=sleela_server.cpp server.cpp packet_logger_bridge.cpp ../logger/sleela_logger.cpp
all: $(TARGET)

TEST_TARGET=server_protocol_test
TEST_SOURCES=sleela_server.cpp packet_logger_bridge.cpp ../logger/sleela_logger.cpp tests/server_protocol_test.cpp
$(TARGET): $(SOURCES) sleela_server.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ $(SOURCES) $(LDLIBS)
$(TEST_TARGET): $(TEST_SOURCES) sleela_server.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ $(TEST_SOURCES) $(LDLIBS)
test: $(TEST_TARGET)
	./$(TEST_TARGET)
clean:
	rm -f $(TARGET) $(TARGET).exe $(TEST_TARGET) *.o
.PHONY: all test clean
