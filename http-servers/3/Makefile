CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
TARGET=http-server-3

all: $(TARGET)

$(TARGET): main.cpp http3/http3_server.cpp http3/http3_server.h
	$(CXX) $(CXXFLAGS) main.cpp http3/http3_server.cpp -o $@

clean:
	rm -f $(TARGET)
