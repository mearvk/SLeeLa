CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -pthread
TARGET=http-server-1
all: $(TARGET)
$(TARGET): main.cpp ../common/http_server.cpp ../common/http_server.h
	$(CXX) $(CXXFLAGS) main.cpp ../common/http_server.cpp -o $@
clean:
	rm -f $(TARGET)
