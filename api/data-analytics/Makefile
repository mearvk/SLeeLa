CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -pedantic
all: libsleela-data-analytics.a
sleela_data_analytics.o: sleela_data_analytics.c sleela_data_analytics.h
	$(CC) $(CFLAGS) -c sleela_data_analytics.c -o $@
libsleela-data-analytics.a: sleela_data_analytics.o
	ar rcs $@ $^
clean:
	rm -f sleela_data_analytics.o libsleela-data-analytics.a
