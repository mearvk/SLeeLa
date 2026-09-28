# Skya Driver Runtime — Buffering, Message Passing, Locks, and Hardware Boundary

Model adapters now sit on a concrete runtime rather than being only identification records.

`skya_driver_runtime` implements a bounded FIFO message queue with mutex protection, condition-variable wakeups, ordered sequence numbers, timestamps, blocking/timed receive, cancellation, explicit start/stop state, and full-queue rejection. C++ uses the same C ABI through `DriverRuntime`.

`skya_hardware_io` defines the device boundary for audio capture/playback, HID input/output, and control transfers. Platform-specific implementations supply callbacks. The common layer does not invent VID/PID values, endpoint addresses, HID report layouts, codec commands, or firmware protocols.

A USB endpoint is hardware and the host pipe is its software representation. citeturn0search8 Windows HID already maintains an input-report ring buffer and documents configurable buffer counts; Skya's queue is an additional adapter/application queue. citeturn0search0turn0search2 Windows audio and ALSA impose buffer, synchronization, and atomicity rules that the platform layer must honor. citeturn0search3turn0search4turn0search6

Lock conditions: queue mutation occurs under the runtime mutex; condition waits release/reacquire it atomically; producers signal after enqueue; stop/cancel broadcasts; kernel IRQ-safe locking remains the responsibility of the OS driver layer; hardware callbacks must not re-enter through an incompatible external lock.

Layering: `Model driver → Skya runtime → hardware I/O callbacks → OS audio/HID/USB stack → physical hardware`.
