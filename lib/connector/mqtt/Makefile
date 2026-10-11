CC ?= cc
CXX ?= c++
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -O2 -Wall -Wextra -Wpedantic -std=c++17
MQTT_CFLAGS ?= $(shell pkg-config --cflags libmosquitto 2>/dev/null)
MQTT_LIBS ?= $(shell pkg-config --libs libmosquitto 2>/dev/null || echo -lmosquitto)
all: libsleela_mqtt.a mqtt_connector_cpp.o
mqtt_connector.o: mqtt_connector.c mqtt_connector.h
	$(CC) $(CFLAGS) $(MQTT_CFLAGS) -c $< -o $@
mqtt_connector_cpp.o: mqtt_connector.cpp mqtt_connector.hpp mqtt_connector.h
	$(CXX) $(CXXFLAGS) $(MQTT_CFLAGS) -c $< -o $@
libsleela_mqtt.a: mqtt_connector.o
	ar rcs $@ $^
clean:
	rm -f *.o *.a
.PHONY: all clean

# Explicit alias for scripts and IDE build actions.
build: all
.PHONY: build
