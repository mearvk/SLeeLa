CC ?= cc
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic
MQTT_CFLAGS ?= $(shell pkg-config --cflags libmosquitto 2>/dev/null)
MQTT_LIBS ?= $(shell pkg-config --libs libmosquitto 2>/dev/null || echo -lmosquitto)
all: libsleela_mqtt.a
mqtt_connector.o: mqtt_connector.c mqtt_connector.h
	$(CC) $(CFLAGS) $(MQTT_CFLAGS) -c $< -o $@
libsleela_mqtt.a: mqtt_connector.o
	ar rcs $@ $^
clean:
	rm -f *.o *.a
.PHONY: all clean
