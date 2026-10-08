# CPU Cache Specification

Cache descriptions must identify:

- level (L1/L2/L3)
- instruction/data/unified
- capacity
- line size
- associativity
- replacement policy when documented
- read/write policy when documented
- latency when documented
- coherence domain
- inclusion/exclusion when documented
- prefetch behavior when documented

A cache field marked `unknown` is intentional. Do not infer L2/L3 hardware for processors that do not have those levels.

Modern architectures can have substantially different cache hierarchies. AMD states that Zen 2 desktop designs increased L3 capacity to as much as 32 MB and expanded floating-point and load/store capabilities. citeturn0search0