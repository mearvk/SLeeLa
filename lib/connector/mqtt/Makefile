CC ?= cc
CXX ?= c++
AR ?= ar
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -O2 -Wall -Wextra -Wpedantic -std=c++17
MQTT_CFLAGS ?= $(shell pkg-config --cflags libmosquitto 2>/dev/null) $(shell pkg-config --cflags libcjson 2>/dev/null)
MQTT_LIBS ?= $(shell pkg-config --libs libmosquitto 2>/dev/null || echo -lmosquitto)
.PHONY: all build test clean
all: libsleela_mqtt.a mqtt_connector_cpp.o
mqtt_connector.o: mqtt_connector.c mqtt_connector.h
	$(CC) $(CFLAGS) $(MQTT_CFLAGS) -c $< -o $@
mqtt_connector_cpp.o: mqtt_connector.cpp mqtt_connector.hpp mqtt_connector.h
	$(CXX) $(CXXFLAGS) $(MQTT_CFLAGS) -c $< -o $@
libsleela_mqtt.a: mqtt_connector.o
	$(AR) rcs $@ $^
test_mqtt_connector: test_mqtt_connector.c mqtt_connector.c mqtt_connector.h
	$(CC) $(CFLAGS) $(MQTT_CFLAGS) test_mqtt_connector.c mqtt_connector.c $(MQTT_LIBS) -o $@
test: test_mqtt_connector
	./test_mqtt_connector
clean:
	rm -f *.o *.a test_mqtt_connector
build: all
