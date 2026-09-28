CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
API = ../../api/http-multiplexing/sleela_http_api.c
API_HEADER = ../../api/http-multiplexing/sleela_http_api.h

all: httpctl

httpctl: httpctl.c $(API) $(API_HEADER)
	$(CC) $(CFLAGS) httpctl.c $(API) -o $@

clean:
	rm -f httpctl
