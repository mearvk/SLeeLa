# Native Runtime Foundation

Portable C++17 runtime primitives for Sleela-Complete applications.

Provides EventLoop, WorkQueue, CancellationToken and FutureResult. Lifecycle is explicit: construct, start, submit/post, stop, join, destroy. Worker/event callbacks are contained by a native exception boundary.

This is a foundation, not a claim that OS event sources, timers, GUI loops, signals, or platform adapters are complete. Those remain adapter work.
