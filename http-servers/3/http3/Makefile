CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
TARGET=http-server-3-http3

all: $(TARGET)

$(TARGET): main.cpp http3_server.cpp http3_server.h
	$(CXX) $(CXXFLAGS) main.cpp http3_server.cpp -o $@

clean:
	rm -f $(TARGET)
