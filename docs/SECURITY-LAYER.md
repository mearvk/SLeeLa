# SLVM Security Layer

The security layer monitors behavior at the VM boundary without assuming that unusual behavior is malicious.

It observes instruction load, resource-request frequency, object-request volume, managed-memory pressure, file activity, network activity, initial file loading, dynamic instantiation, and resource-control operations.

States are NORMAL, ELEVATED, RESTRICTED, and DENIED. The state is policy telemetry: it does not grant OS privileges. Capability checks remain authoritative.

The model uses execution windows rather than requiring a platform-specific clock. High request or object rates increase an anomaly score; resetting a window provides controlled score decay. Thresholds are configurable.
