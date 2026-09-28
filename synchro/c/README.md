# C Synchro

Portable C11 packet, latency-statistics, and language-integration core for SLeeLa Synchro.

## Standalone build

    cc -std=c11 -Wall -Wextra -pedantic -c synchro.c
    cc -std=c11 -Wall -Wextra -pedantic -I. -c synchro_integration.c

The integration layer exposes prepare/ack/timeout operations around the stable
16-byte Synchro packet contract. It is built by impl/Makefile as part of the
repository-native integration tests.

The wire format is compatible with the Python and Java implementations.
Synchro statistics are observations; they are not delivery guarantees.
