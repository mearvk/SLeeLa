# SLVM/7 Memory Manager

The Memory Manager treats memory as both a resource and part of recoverable VM state.

It tracks committed, reserved, checkpoint, and high-watermark memory against policy. Pressure is detected before allocation crosses the configured limit.

The preferred pressure sequence is:

1. record the pressure event;
2. create a checkpoint when safe;
3. reclaim eligible memory;
4. throttle non-critical work;
5. recover or quarantine if integrity is compromised.

Sensitive released memory should be zeroized when policy requires it. Memory accounting remains available to health, resource, checkpoint, attestation, and recovery managers without granting those managers execution authority.
