# Station Signaling — Data Flow (Spec → Code)

Maps the Station design (`STATION-SIGNALING-SPEC.md`, J-STA-0001) to the source,
and traces one negotiation from a TCP-flag signal to a witnessed outcome.
Companion: [`STATUS.md`](STATUS.md).

## The data, and where it lives

| Spec concept | Section | Type / symbol | File |
|---|---|---|---|
| TCP flag bits | §2 | `SL_TCP_*` | `station_abi.h` |
| 8-URG magnitude | §2.1 | `urg_units`, `SL_STATION_URG_MAX` | `station_abi.h` |
| HOP verbs (DIST±N) | §3 | `sl_hop_verb_t`, `sl_classify_hop` | `station_abi.h` / `station_classifier.c` |
| Conditions | §5 | `sl_condition_t`, `sl_classify_condition` | `station_abi.h` / `station_classifier.c` |
| Premise triple | §4 | `sl_premise_t`, `sl_classify_signal` | `station_abi.h` / `station_classifier.c` |
| Signal (raw evidence) | §2 | `sl_signal_t` | `station_abi.h` |
| Witnessed record | §8 | `sl_record_t` | `station_abi.h` |
| Session + state machine | §6/§7 | `sl_session_t`, `sl_session_signal` | `station_session.{h,c}` |
| Witnessed view | §7 | `sl_session_view_t`, `sl_session_view` | `station_abi.h` / `station_session.c` |
| Known API (ioctl) | §9 | `SL_IOC_OPEN/SIGNAL/QUERY/CLOSE` | `station_abi.h` |
| Kernel driver + hook | §9 | `sl_dev_ioctl`, `sl_nf_hook` | `kernel/sleela_station.c` |
| Userland transports | §9 | `sl_station_*` (sim + device) | `station_backend.{h,c}` |

## End-to-end trace (one signal → witnessed state)

```text
          PARTY A / PARTY B  (two clients)
                │  emit a TCP segment on the signaling port
                │  flags = URG|ACK|... , urg_ptr = urg_units, hop field
                ▼
   ┌─────────────────────────────────────────────┐
   │  Router = STATION                            │
   │                                              │
   │  (kernel)  Netfilter PRE_ROUTING hook        │  kernel/sleela_station.c
   │    parse TCP flags/urg/hop -> sl_signal_t     │
   │      OR                                       │
   │  (userland) SL_IOC_SIGNAL / simulation        │  station_backend.c
   │    submit sl_signal_t directly                │
   │                │                              │
   │                ▼  §4/§5 classify (SHARED)     │  station_classifier.c
   │   sl_classify_signal(sig, current_open)       │
   │     -> condition, premise, hop_verb           │
   │                │                              │
   │                ▼  §6/§7/§8 fold + witness      │  station_session.c
   │   sl_session_signal(session, sig, &record)    │
   │     - append immutable record (seq++)         │
   │     - update this party's open_premise/ack    │
   │     - recompute joint state                   │
   │                │                              │
   │                ▼                              │
   │   state: OPEN -> CLEAR | UNCLEAR | HUNG-UP |  │
   │                          DISASTROUS           │
   └─────────────────────────────────────────────┘
                │  SL_IOC_QUERY / sl_station_query
                ▼
        sl_session_view_t  (witnessed outcome + record count)
```

## Premise triple (the "8 URG with HOP = DIST±N")

```text
A: urg=3 hop=DIST+1 (ADVANCE)  -> premise START          (opens)
B: urg=3 hop=DIST+1 (ADVANCE)  -> premise START
A: urg=5 hop=DIST+1 (ADVANCE)  -> premise INTERMEDIARY    (mediated) + ACK
B: urg=5 hop=DIST+1 (ADVANCE)  -> premise INTERMEDIARY               + ACK
   => both at INTERMEDIARY, both ACKed  => STATE = CLEAR

   any party: hop=DIST-1 (CANCEL) -> premise CANCELLING   => STATE = UNCLEAR
   any party: FIN                 -> HUNG-UP
   any party: RST                 -> DISASTROUS
```

`HOP = DIST+0` (HOLD) re-asserts the current premise (witnessed, not advanced).

## Condition classification (§5)

| Flags + magnitude | Condition |
|---|---|
| `URG`, `urg_units≥1` | INTENTIONAL |
| `URG+ACK`, `urg_units==8` | LEGAL |
| `URG+PSH` | WARNING |
| `URG+PSH+ACK`, `urg_units==8` | EMERGENCY |
| no `URG` / `urg_units==0` | NONE (bare ACK/keepalive) |

## Kernel / userland split (§9)

```text
station_abi.h        <-- single ABI, included by BOTH sides
station_classifier.c <-- SHARED judgment (kernel #includes it; userland links it)
station_session.c    <-- SHARED state machine (same)

kernel/sleela_station.c   -> /dev/sleela_station (ioctl) + Netfilter hook
station/station_backend.c -> device backend (ioctl)  OR  simulation backend
```

The device and simulation backends expose the identical userland API, and both
resolve to the same classifier/session code, so a negotiation witnessed in the
kernel and one witnessed in userland reach the same verdict by construction.

## Run it

```sh
cd http-3.0
make station        # build the userland Station + demo
make station-test   # run the manualog demo + Python parity tests
make test           # everything: pipeline + station + flow tests
make kernel-check   # confirms the kernel build scaffold (needs kernel headers to build the .ko)

# On a host with kernel headers:
cd kernel && make && sudo make load   # builds + inserts sleela_station.ko
```

## Boundary note

The Station witnesses and records; it is an intermediary, not a party. A LEGAL
condition records a legal *framing*, not an adjudication — consistent with the
HTTP 3.0 principle that context is never authority. The stored terminal state is
evidence of what the Station witnessed, distinct from any claim about what the
parties intended.
