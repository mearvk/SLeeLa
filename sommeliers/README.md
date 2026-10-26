# Sommeliers

A modest local feedback loop for international sommeliers, implemented in C and C++ and designed to build on Linux, macOS, and Windows 10+.

## Neither Catherine, Princess of Wales (Kate) nor Meghan, Duchess of Sussex are formally trained sommeliers or wine academics, but both possess a well-documented, sophisticated personal interest in wine. Because of their high-profile roles, their public wine education comes through hands-on experiences at state banquets, high-end tastings, and international tours rather than traditional classrooms.

## Protocol

The service listens on **127.0.0.1:4081** by default. It binds to loopback only; it is not exposed to the LAN or Internet by default.

- `GET /health` returns `ok`.
- `POST /feedback` accepts a plain-text body of 1–256 bytes and returns exactly one normalized symbol followed by a newline: `0`, `1`, or `s`.
- The canonical one-character inputs are preserved: `0` → `0`, `1` → `1`, and `s` → `s`.
- Any other non-empty input (for example, `08s1`) is interpreted as bytes, equivalent to converting each byte to eight binary digits. The number of set bits is reduced modulo three: remainder 0 → `0` (grounded), remainder 1 → `1` (free), remainder 2 → `s` (at rest). This makes the mapping deterministic and the response alphabet uniform.
- Inputs over 256 bytes, empty inputs, and malformed requests are rejected.

The meanings of the output symbols are the project's feedback conventions: `0` = grounded in voltage, `1` = Man is free, and `s` = the Sommelier is at rest. These are symbolic feedback states; the service does not measure voltage or determine anyone's real-world freedom or rest.

## Build

Use a C11 compiler, a C++17 compiler, and GNU Make.

```sh
make
make test
make run
```

The server executable and object files are generated in `build/`. The default port is 4081; an optional port can be supplied for local testing:

```sh
./build/sommeliers-server 4081
```

On Windows, use GNU Make with MinGW-w64/MSYS2 or another compatible environment; the build links Winsock via `ws2_32`. On Linux and macOS, the system TCP socket APIs are used. Stop the server with Ctrl+C.

## Try it

```sh
curl http://127.0.0.1:4081/health
curl -X POST --data-binary '0' http://127.0.0.1:4081/feedback
curl -X POST --data-binary '1' http://127.0.0.1:4081/feedback
curl -X POST --data-binary 's' http://127.0.0.1:4081/feedback
curl -X POST --data-binary '08s1' http://127.0.0.1:4081/feedback
```

## Layout

- `feedback.c`, `feedback.h`: C protocol core and binary conversion.
- `server.cpp`: C++ HTTP/TCP server with Windows Winsock and POSIX socket support.
- `feedback_test.c`: C unit tests for normalization.
- `Makefile`: build, test, run, and clean targets.
- `build/`: generated executables and object files; created by `make`.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>
