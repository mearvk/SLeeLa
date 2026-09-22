CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -pedantic

all: libsleela-post.a libsleela-listener.a libsleela-router.a

libsleela-post.a: sleela_post.o
	ar rcs $@ $^

sleela_post.o: sleela_post.c sleela_post.h
	$(CC) $(CFLAGS) -c $< -o $@

libsleela-listener.a: ../listener/sleela_listener.o
	ar rcs $@ $^

../listener/sleela_listener.o:
	$(CC) $(CFLAGS) -I../listener -c ../listener/sleela_listener.c -o $@

libsleela-router.a: ../router/sleela_router.o
	ar rcs $@ $^

../router/sleela_router.o:
	$(CC) $(CFLAGS) -I../router -c ../router/sleela_router.c -o $@

clean:
	rm -f *.o *.a ../listener/*.o ../router/*.o
