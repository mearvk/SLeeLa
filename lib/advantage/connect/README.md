artifact;
pure;
gasoline;

# lib/advantage/connect — the connectivity medium

The **in-between component structure** that connects Advantage™ SLeeLa programs
to the outside world: databases, national clocks, networks, and money sources.
It is a *medium* — a thin, uniform layer that sits between a pure Advantage model
and the messy, stateful systems it must talk to — so the model code stays clean
and every external call is validated, status-coded, and fault-recorded the same
way.

> `artifact; pure; gasoline;` — an Advantage™ subject. These classes model
> lifecycle and the native boundary; they do **not** dial a live database, clock,
> socket, or bank from inside the SLeeLa VM, and this is not financial advice.

## The medium at a glance

| Class | Connects to | Native entry points (documented, host-adapter supplied) |
|-------|-------------|----------------------------------------------------------|
| `SLConnectStatus` | — (shared) | the universal integer status vocabulary (OK … NATIVE_ADAPTER_REQUIRED … INTERNAL) |
| `SLConnectFault` | — (shared) | the structured fault record every connector raises |
| `SLSqlConnector` | **MySQL** / databases | `slsql_open / ping / execute / query / close` (mirrors `lib/connector/mysql`) |
| `SLClockConnector` | **national clocks / NTP** | `slntp_open / sync / now / close` |
| `SLNetConnector` | **network connectivity** | `slnet_connect / probe / write / close` |
| `SLMoneyConnector` | **money sources** (banks, cards, wallets) | `slbank_open / authorize / capture / refund / void / close` |

A runnable `connect-demo.sleela` drives all four through their lifecycle, their
native-adapter boundary, and a fault path, and ends in `ALL CHECKS PASSED`.

## Exception handling for all classes of perform

SLeeLa the language has `try/catch/throw`, but the standard library signals
failure with **integer status codes plus a structured fault record** — a single,
uniform convention across every connector. That is the "exception handling for
all these classes of perform":

- **`SLConnectStatus`** — one vocabulary of outcome codes shared by all four
  connectors: `OK(0)`, `INVALID_ARGUMENT(1)`, `INVALID_STATE(2)`,
  `NOT_CONFIGURED(3)`, `UNAUTHORIZED(4)`, `TIMEOUT(5)`, `UNREACHABLE(6)`,
  `PROTOCOL(7)`, `DECLINED(8)`, `NATIVE_ADAPTER_REQUIRED(9)`, `INTERNAL(10)`.
  It also classifies a code as transient (safe to retry) or permanent.
- **`SLConnectFault`** — the exception stand-in: `code`, `subsystem`,
  `operation`, `message`, `retryable`, and `attempts`. Each connector exposes
  `lastFault()`; a caller inspects it instead of catching. Domain rules override
  retryability — e.g. a money `DECLINED` is marked **non-retryable** so no caller
  loops on a refusal.

Every method returns a status code; a non-OK code leaves a fully populated fault
behind. No connector throws.

## The native boundary (where the real connection plugs in)

Each connector models its lifecycle and accounting **in the VM** (so a program
can be written, type-checked, and validated offline) and defers the operation
that actually touches the outside world to a **host adapter**, returning
`NATIVE_ADAPTER_REQUIRED(9)`. The native entry points are named in each class's
header doc comment (and in the table above); a host runtime registers adapters
that implement them. This mirrors the existing precedent in
`lib/advantage/output/SLAdvantageSink.sleela` (`slnet_*`) and the native ABI in
`lib/connector/mysql`.

Host-adapter **callbacks** let a real result flow back in without breaking the
status model: `SLClockConnector.acceptSync(offset, stratum)`,
`SLNetConnector.acceptProbe(rtt)`, and `SLMoneyConnector.acceptAuthorization` /
`declineAuthorization`.

## Safety notes baked into the classes

- **No secrets in SLeeLa.** The SQL connector keeps only a redacted coordinate
  (`mysql://host:port/db`) and a *flag* that a credential is pinned; the money
  connector keeps only a **keystore alias** — never a PAN, CVV, or password. The
  real secret is resolved by the native adapter from the keystore
  (see `lib/advantage/transport`).
- **Money is integer cents** to avoid floating-point error; capture cannot
  exceed the authorization, and refund cannot exceed the captured net.
- **Clocks declare their schedule.** `SLClockConnector` cites NTPv4 (RFC 5905)
  and treats UTC (BIPM / national metrology institutes) as the reference scale;
  it records which authority was configured without endorsing any.

## Run it

```sh
./working-binaries/sleela run lib/advantage/connect/connect-demo.sleela
```
