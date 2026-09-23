CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -pthread
LIBS ?= -lnghttp2
TARGET=http-server-2-h2
all: $(TARGET)
$(TARGET): main.cpp http2_server.cpp http2_server.h
	$(CXX) $(CXXFLAGS) main.cpp http2_server.cpp -o $@ $(LIBS)
clean:
	rm -f $(TARGET)
