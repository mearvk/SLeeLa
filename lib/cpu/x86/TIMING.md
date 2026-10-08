# x86 / x86-64 Timing

## Architectural versus microarchitectural timing

x86 instruction latency and throughput vary significantly between generations and implementations. Intel publishes separate optimization and microarchitecture material rather than one universal timing table. citeturn0search7

SLeeLa timing records therefore include:

- instruction bytes;
- decode length;
- decoded operation count;
- execution domain;
- issue/dispatch;
- dependencies;
- execution latency;
- throughput;
- load/store latency;
- cache hit/miss;
- TLB hit/miss;
- branch prediction/result;
- retirement/completion;
- exception/interrupt;
- serialization.

## Legacy timing

Older x86 implementations can use a more direct bus/cycle model.

## Modern timing

Modern implementations may include decode queues, micro-op caches, out-of-order execution, speculative execution, multiple execution ports, and retirement. These are concrete implementation properties, not universal ISA requirements.
