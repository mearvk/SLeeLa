# SLeeLa Java Interop Protocol (SLJIP) v1

Status: first transport contract; not yet wired to SLeeLa expression lowering.

## Frame

All integers are unsigned big-endian unless noted. A frame is exactly:

| Offset | Width | Field |
|---:|---:|---|
| 0 | 4 | ASCII magic SLJV |
| 4 | 2 | Protocol version (1) |
| 6 | 4 | Request ID |
| 10 | 1 | Opcode |
| 11 | 4 | Payload byte length |
| 15 | N | Payload |

Maximum payload: 1 MiB. The stream is binary; diagnostics belong on stderr.
A response uses the same request ID and opcode ORed with 0x80.

## Payload primitives

- u8, u16, u32, u64: fixed-width big-endian.
- string: u32 byte_length followed by strict UTF-8 bytes; maximum 256 KiB.
- value: one tag followed by its payload:
  - 0: null
  - 1: false
  - 2: true
  - 3: signed 32-bit integer (two's-complement bits in u32)
  - 4: signed 64-bit integer (two's-complement bits in u64)
  - 5: IEEE-754 binary64
  - 6: UTF-8 string
  - 7: object handle (u64, nonzero)
- Parameter type lists are u16 count, then count strings containing Java
  binary names or primitive names (boolean, byte, char, short, int, long,
  float, double).
- Arguments are u16 count, then that many value records.

## Opcodes and payloads

| Opcode | Operation | Payload |
|---:|---|---|
| 1 | PING | empty |
| 2 | RESOLVE | class name string |
| 3 | CONSTRUCT | class name, parameter type list, argument list |
| 4 | INVOKE_INSTANCE | handle u64, member name, parameter type list, argument list |
| 5 | INVOKE_STATIC | class name, member name, parameter type list, argument list |
| 6 | RELEASE | handle u64 |
| 7 | CLOSE | empty |
| 8 | GET_FIELD | handle u64, member name |
| 9 | GET_STATIC_FIELD | class name, member name |
| 10 | SET_FIELD | handle u64, member name, value |
| 11 | SET_STATIC_FIELD | class name, member name, value |

Class resolution is non-initializing. Constructors and method invocation are
explicit execution requests. Only public constructors, methods, and fields are exposed. Field reads/writes must match static versus instance ownership.
The endpoint's class/package allow-list is configured out-of-band through
SLEELA_JAVA_ALLOW_CLASSES and SLEELA_JAVA_ALLOW_PACKAGES, as comma-separated
binary names/prefixes. Optional class-directory/JAR roots are supplied through
SLEELA_JAVA_CLASS_PATHS using the platform path separator. A request cannot grant
itself permission.

## Response payload

First byte is status: 0 success, 1 failure. On success, the value format
above is used (PING/CLOSE return tag 0). On failure, two strings follow:
exception class name and safe message. Java stack traces are not sent over the
protocol. The server continues after a request-level error; invalid frame
headers, unsupported versions, oversized frames, or malformed payloads close
the session.

## Lifetime and security

Handles are monotonically allocated per process session and invalidated on
RELEASE or CLOSE. A handle is never a pointer and is never valid in another
session. No request executes a shell command. No class initialization occurs
during RESOLVE. Static initialization may occur only as a consequence of an
explicit CONSTRUCT, INVOKE_STATIC, GET_STATIC_FIELD, or SET_STATIC_FIELD request. Arguments are type-checked by
reflection. I32 values may be range-checked and widened to byte, short, char,
int, or long; F64 may be range-checked and narrowed to float or retained as
double. Incompatible conversions fail with a structured error.

## Current integration boundary

This v1 transport is an independently executable Java helper and a native
frame codec. The SLeeLa compiler/VM does not yet lower source expressions to
these opcodes. Do not label ordinary SLeeLa java.invoke: strings as executable
interoperability until an end-to-end compiler-to-host test exists.
