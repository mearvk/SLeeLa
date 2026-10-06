# SLeeLa VM Opcode Map

Max Rupplin - MEARVK LLC - 2026

This is the human-readable support map for the canonical source-defined SLeeLa ISA in `lib/vm/InstructionSet.sleela`.

**Verified master-tree state:** 124 source ISA entries, 124 native `SLOp` entries, and 124 native dispatch cases. There are no missing native dispatch cases. The tail groups are the array ops (98–102) and the operating-system system calls (103–123, `OP_OS_*`), both appended to the end of the enum so earlier opcode numbers are unchanged.

| Code | Opcode | Runtime status | Native dispatch |
|---:|---|---|---|
00 | `OP_NOP` | Implemented | `impl/core/sleela_core.c`
01 | `OP_CONST` | Implemented | `impl/core/sleela_core.c`
02 | `OP_POP` | Implemented | `impl/core/sleela_core.c`
03 | `OP_DUP` | Implemented | `impl/core/sleela_core.c`
04 | `OP_LOADG` | Implemented | `impl/core/sleela_core.c`
05 | `OP_STOREG` | Implemented | `impl/core/sleela_core.c`
06 | `OP_LOADL` | Implemented | `impl/core/sleela_core.c`
07 | `OP_STOREL` | Implemented | `impl/core/sleela_core.c`
08 | `OP_ADD` | Implemented | `impl/core/sleela_core.c`
09 | `OP_SUB` | Implemented | `impl/core/sleela_core.c`
10 | `OP_MUL` | Implemented | `impl/core/sleela_core.c`
11 | `OP_DIV` | Implemented | `impl/core/sleela_core.c`
12 | `OP_MOD` | Implemented | `impl/core/sleela_core.c`
13 | `OP_NEG` | Implemented | `impl/core/sleela_core.c`
14 | `OP_EQ` | Implemented | `impl/core/sleela_core.c`
15 | `OP_NE` | Implemented | `impl/core/sleela_core.c`
16 | `OP_LT` | Implemented | `impl/core/sleela_core.c`
17 | `OP_LE` | Implemented | `impl/core/sleela_core.c`
18 | `OP_GT` | Implemented | `impl/core/sleela_core.c`
19 | `OP_GE` | Implemented | `impl/core/sleela_core.c`
20 | `OP_AND` | Implemented | `impl/core/sleela_core.c`
21 | `OP_OR` | Implemented | `impl/core/sleela_core.c`
22 | `OP_NOT` | Implemented | `impl/core/sleela_core.c`
23 | `OP_JMP` | Implemented | `impl/core/sleela_core.c`
24 | `OP_JMPF` | Implemented | `impl/core/sleela_core.c`
25 | `OP_CALL` | Implemented | `impl/core/sleela_core.c`
26 | `OP_RET` | Implemented | `impl/core/sleela_core.c`
27 | `OP_PRINT` | Implemented | `impl/core/sleela_core.c`
28 | `OP_SPAWN` | Implemented | `impl/core/sleela_core.c`
29 | `OP_JOINALL` | Implemented | `impl/core/sleela_core.c`
30 | `OP_LOCK` | Implemented | `impl/core/sleela_core.c`
31 | `OP_UNLOCK` | Implemented | `impl/core/sleela_core.c`
32 | `OP_SEND` | Implemented | `impl/core/sleela_core.c`
33 | `OP_RECV` | Implemented | `impl/core/sleela_core.c`
34 | `OP_LISTEN` | Implemented | `impl/core/sleela_core.c`
35 | `OP_ACCEPT` | Implemented | `impl/core/sleela_core.c`
36 | `OP_CONNECT` | Implemented | `impl/core/sleela_core.c`
37 | `OP_SOCKREAD` | Implemented | `impl/core/sleela_core.c`
38 | `OP_SOCKWRITE` | Implemented | `impl/core/sleela_core.c`
39 | `OP_SOCKCLOSE` | Implemented | `impl/core/sleela_core.c`
40 | `OP_PIPE` | Implemented | `impl/core/sleela_core.c`
41 | `OP_PIPEPEER` | Implemented | `impl/core/sleela_core.c`
42 | `OP_FIFO_MK` | Implemented | `impl/core/sleela_core.c`
43 | `OP_FILEOPEN` | Implemented | `impl/core/sleela_core.c`
44 | `OP_FILEREAD` | Implemented | `impl/core/sleela_core.c`
45 | `OP_FILEWRITE` | Implemented | `impl/core/sleela_core.c`
46 | `OP_FILECLOSE` | Implemented | `impl/core/sleela_core.c`
47 | `OP_FILEUNLINK` | Implemented | `impl/core/sleela_core.c`
48 | `OP_HALT` | Implemented | `impl/core/sleela_core.c`
49 | `OP_TIME_UTC_MS` | Implemented | `impl/core/sleela_core.c`
50 | `OP_TIME_UTC_NS` | Implemented | `impl/core/sleela_core.c`
51 | `OP_TIME_MONO_NS` | Implemented | `impl/core/sleela_core.c`
52 | `OP_TIME_PRECISION_MS` | Implemented | `impl/core/sleela_core.c`
53 | `OP_TIME_LOCATION` | Implemented | `impl/core/sleela_core.c`
54 | `OP_TIME_HTTP_DATE` | Implemented | `impl/core/sleela_core.c`
55 | `OP_TIME_JSON` | Implemented | `impl/core/sleela_core.c`
56 | `OP_TIME_NTP` | Implemented | `impl/core/sleela_core.c`
57 | `OP_TIME_SET_LOCATION` | Implemented | `impl/core/sleela_core.c`
58 | `OP_NEWSTRUCT` | Implemented | `impl/core/sleela_core.c`
59 | `OP_GETFIELD` | Implemented | `impl/core/sleela_core.c`
60 | `OP_SETFIELD` | Implemented | `impl/core/sleela_core.c`
61 | `OP_STRUCTPACK` | Implemented | `impl/core/sleela_core.c`
62 | `OP_STRUCTUNPACK` | Implemented | `impl/core/sleela_core.c`
63 | `OP_SYN_OPEN` | Implemented | `impl/core/sleela_core.c`
64 | `OP_SYN_DISPATCH` | Implemented | `impl/core/sleela_core.c`
65 | `OP_SYN_STAT` | Implemented | `impl/core/sleela_core.c`
66 | `OP_SYN_REPORT` | Implemented | `impl/core/sleela_core.c`
67 | `OP_SYN_CLOSE` | Implemented | `impl/core/sleela_core.c`
68 | `OP_MUN_START` | Implemented | `impl/core/sleela_core.c`
69 | `OP_MUN_CONNECT` | Implemented | `impl/core/sleela_core.c`
70 | `OP_MUN_ENABLE` | Implemented | `impl/core/sleela_core.c`
71 | `OP_MUN_SEND` | Implemented | `impl/core/sleela_core.c`
72 | `OP_MUN_THATCH` | Implemented | `impl/core/sleela_core.c`
73 | `OP_MUN_CONSUME` | Implemented | `impl/core/sleela_core.c`
74 | `OP_MUN_LATCH` | Implemented | `impl/core/sleela_core.c`
75 | `OP_MUN_RECEPTION` | Implemented | `impl/core/sleela_core.c`
76 | `OP_MUN_CLOSE` | Implemented | `impl/core/sleela_core.c`
77 | `OP_BEST_NEW` | Implemented | `impl/core/sleela_core.c`
78 | `OP_BEST_WEIGHT` | Implemented | `impl/core/sleela_core.c`
79 | `OP_BEST_MINVER` | Implemented | `impl/core/sleela_core.c`
80 | `OP_BEST_BUDGET` | Implemented | `impl/core/sleela_core.c`
81 | `OP_BEST_CAND` | Implemented | `impl/core/sleela_core.c`
82 | `OP_BEST_RECORD` | Implemented | `impl/core/sleela_core.c`
83 | `OP_BEST_SCORE` | Implemented | `impl/core/sleela_core.c`
84 | `OP_BEST_BEST` | Implemented | `impl/core/sleela_core.c`
85 | `OP_BEST_STAT` | Implemented | `impl/core/sleela_core.c`
86 | `OP_BEST_CHOICE` | Implemented | `impl/core/sleela_core.c`
87 | `OP_BEST_REPORT` | Implemented | `impl/core/sleela_core.c`
88 | `OP_BEST_ARCH` | Implemented | `impl/core/sleela_core.c`
89 | `OP_BEST_ARCH_STATE` | Implemented | `impl/core/sleela_core.c`
90 | `OP_BEST_CLOSE` | Implemented | `impl/core/sleela_core.c`
91 | `OP_AUDIO_NEW` | Implemented | `impl/core/sleela_core.c`
92 | `OP_AUDIO_ADD` | Implemented | `impl/core/sleela_core.c`
93 | `OP_AUDIO_CONTROLS` | Implemented | `impl/core/sleela_core.c`
94 | `OP_AUDIO_VALIDATE` | Implemented | `impl/core/sleela_core.c`
95 | `OP_AUDIO_RENDER` | Implemented | `impl/core/sleela_core.c`
96 | `OP_AUDIO_CLOSE` | Implemented | `impl/core/sleela_core.c`
97 | `OP_AUDIO_PLATFORM` | Implemented | `impl/core/sleela_core.c`
98 | `OP_NEWARRAY` | Implemented | `impl/core/sleela_core.c`
99 | `OP_ARRGET` | Implemented | `impl/core/sleela_core.c`
100 | `OP_ARRSET` | Implemented | `impl/core/sleela_core.c`
101 | `OP_ARRLEN` | Implemented | `impl/core/sleela_core.c`
102 | `OP_ARRPUSH` | Implemented | `impl/core/sleela_core.c`
103 | `OP_OS_PLATFORM` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)
104 | `OP_OS_CAPABILITY` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)
105 | `OP_OS_GETENV` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)
106 | `OP_OS_SETENV` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)
107 | `OP_OS_CWD` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)
108 | `OP_OS_CHDIR` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)
109 | `OP_OS_HOSTNAME` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)
110 | `OP_OS_USERNAME` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)
111 | `OP_OS_TEMPDIR` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)
112 | `OP_OS_PID` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)
113 | `OP_OS_EXISTS` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)
114 | `OP_OS_ISDIR` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)
115 | `OP_OS_FILESIZE` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)
116 | `OP_OS_MKDIR` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)
117 | `OP_OS_REMOVE` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)
118 | `OP_OS_RENAME` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)
119 | `OP_OS_RUN` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)
120 | `OP_OS_SPAWN` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)
121 | `OP_OS_WAIT` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)
122 | `OP_OS_KILL` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)
123 | `OP_OS_PCLOSE` | Implemented | `impl/core/sleela_core.c` (`sleela_os.c`)

The operating-system opcodes (`OP_OS_*`) are the host System Call API: the `os*`
built-ins (`osRun`/`osSpawn`/`osGetEnv`/`osExists`/...) lower to these opcodes,
which the VM services through `impl/core/sleela_os.c` on Windows (Win32), Linux,
and macOS (POSIX). A spawned process is a VM-local bounded handle, the same
discipline as sockets and files. Source-side they are mirrored by the
`SLISAOs*` classes in `lib/vm/InstructionSet.sleela` and surfaced to a VM via
`lib/vm/SleelaVMSystemCallBridge.sleela`. Because every VM generation (SLVM/1
through SLVM/11) shares this one ISA, the system-call surface is available to
all of them, not just the base VM.

## Runtime rule

The authoritative runtime is `/impl/core`. An opcode not present in the canonical map is not silently ignored or treated as a no-op.

When an unknown/unsupported opcode reaches the VM dispatch loop:

1. the VM writes a diagnostic to standard error; and
2. if `SLEELA_OPCODE_LOG` is set to a writable path, the diagnostic is appended to that log instead; and
3. execution is rejected with an `unsupported opcode` VM error.

Example:

```sh
SLEELA_OPCODE_LOG=/var/log/sleela/opcodes.log ./impl/build/sleela ...
```

The environment variable is optional. Standard error remains the fallback when the configured log cannot be opened.

## Maintenance rule

Any new source-defined opcode must be added in this order:

1. `lib/vm/InstructionSet.sleela`
2. native `SLOp` enum in `impl/core/sleela_core.h`
3. native dispatch in `impl/core/sleela_core.c`
4. this support map
5. the source/ISA coverage verifier
6. SLVM/1 through SLVM/11 source-to-VM contracts.

A source opcode is not considered supported merely because it appears in the source registry. Native dispatch is required.

> The `OP_OS_*` system-call group followed this rule end to end: added to
> `InstructionSet.sleela` (step 1), the native `SLOp` enum (step 2) and dispatch
> (step 3), this map (step 4), and reflected in the SLVM/1–11 runtime-services
> contract (step 6) via `RUNTIME-SERVICES.md`.
