CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -pthread
TARGET = logger-smoke
SOURCES = sleela_logger.cpp logger_smoke.cpp
all: $(TARGET)
$(TARGET): $(SOURCES) sleela_logger.hpp
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES)
clean:
	rm -f $(TARGET) *.o
.PHONY: all clean
