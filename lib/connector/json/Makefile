CC ?= cc
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic
all: json_connector.o
json_connector.o: json_connector.c json_connector.h
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o
.PHONY: all clean
