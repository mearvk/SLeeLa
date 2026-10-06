# SLeeLa OS-Aware Networking

SLeeLa's TCP networking is being separated from the VM's operating-system assumptions.

## Native backends

`impl/core/sleela_net.h` defines the common networking contract and `impl/core/sleela_net.c` provides native implementations for:

- Linux/POSIX sockets.
- Windows Winsock2.

The network API uses `SLNetHandle`, an `intptr_t`-sized value. This is important on Windows because a native `SOCKET` must not be narrowed to a POSIX `int`.

## Platform selection

Use:

```text
SLEELA_NET_PLATFORM=auto
SLEELA_NET_PLATFORM=linux
SLEELA_NET_PLATFORM=windows
```

`auto` selects the native host. An explicit non-native request fails during networking startup.

## Common operations

The backend exposes:

```text
slnet_startup
slnet_shutdown
slnet_listen
slnet_accept
slnet_connect
slnet_read
slnet_write
slnet_close
slnet_valid
slnet_last_error
```

Linux uses the normal POSIX/BSD socket implementation. Windows initializes Winsock with `WSAStartup()` and releases it with `WSACleanup()`.

## Build integration

The verified `impl/Makefile` now builds `sleela_net.o` and includes a `test-network-platform` smoke target alongside the existing core TCP test and OS-aware file-I/O test.

The new backend is intentionally independent of the existing VM until the VM socket table and network opcodes are migrated from raw POSIX descriptors to `SLNetHandle`. This keeps the portability boundary explicit and avoids claiming complete Windows runtime support prematurely.

## Next networking migration

The remaining VM migration is:

```text
OP_LISTEN
OP_ACCEPT
OP_CONNECT
OP_SOCKREAD
OP_SOCKWRITE
OP_SOCKCLOSE
```

Those operations currently live in `sleela_core.c` and still use the older POSIX socket implementation. They should be routed through `sleela_net.*` in the next networking integration step.
