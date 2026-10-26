CXX ?= c++
CPPFLAGS ?=
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -pthread
LDFLAGS ?=
PYTHON ?= python3
TARGET = sleelas
ifneq (,$(findstring Windows_NT,$(OS)))
TARGET = sleelas.exe
else ifneq (,$(findstring MINGW,$(shell uname -s)))
TARGET = sleelas.exe
endif
SERVER_TEST_BINARY = ./$(TARGET)
LDLIBS ?= -lssl -lcrypto
SOURCES=sleela_server.cpp server.cpp packet_logger_bridge.cpp ../logger/sleela_logger.cpp
all: $(TARGET)

TEST_TARGET = server_protocol_test
ifneq (,$(findstring Windows_NT,$(OS)))
TEST_TARGET = server_protocol_test.exe
else ifneq (,$(findstring MINGW,$(shell uname -s)))
TEST_TARGET = server_protocol_test.exe
endif
TEST_SOURCES=sleela_server.cpp packet_logger_bridge.cpp ../logger/sleela_logger.cpp tests/server_protocol_test.cpp
$(TARGET): $(SOURCES) sleela_server.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ $(SOURCES) $(LDLIBS)
$(TEST_TARGET): $(TEST_SOURCES) sleela_server.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ $(TEST_SOURCES) $(LDLIBS)
test: $(TEST_TARGET) $(TARGET)
	./$(TEST_TARGET)
	$(PYTHON) -m unittest discover -s tests -p 'test_nat_awareness.py' -v
	$(PYTHON) tests/test_tls_integration.py $(SERVER_TEST_BINARY)
clean:
	rm -f $(TARGET) $(TARGET).exe $(TEST_TARGET) *.o
.PHONY: all test clean
