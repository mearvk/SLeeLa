# Lesson 5 — Mouse and Touch Input

Ballum normalizes pointer input into raw events and, in the higher-level listener, gestures such as click and drag. The API is polled rather than relying on an application callback for every event. That makes polling straightforward to integrate into a loop, but it also means the program must manage work limits, idle behavior, and event processing deliberately.

## Documented gesture codes

| Code | Gesture |
|---:|---|
| 0 | NONE |
| 1 | DOWN |
| 2 | MOVE |
| 3 | CLICK |
| 4 | DRAG_BEGIN |
| 5 | DRAG_MOVE |
| 6 | DRAG_END |

Use the constants or mapping supplied by the current package where available; avoid scattering unexplained numeric values throughout an application. A raw event and a derived gesture are different abstraction levels. Do not assume every backend emits identical event sequences or that every physical input device produces every gesture type.

## Poll without hanging

A typical event-processing pass should have a clear upper bound. A frame can drain available events until the poll reports that no event is ready, but a teaching example or diagnostic should cap the number of polls/events it processes in one pass. That keeps an event burst from monopolizing the program and prevents a broken polling contract from creating an infinite loop.

Conceptually:

```text
processed = 0
while processed < maximum_events:
    result = poll next event
    if result means no event is available:
        stop this pass
    if result is an error:
        report it and stop or recover
    validate and handle the event
    processed = processed + 1
```

This is pseudocode; use the real SLeeLa syntax and Ballum return conventions. Distinguish “no event available” from an actual error if the API exposes those states separately.

## Handle event data carefully

- Validate that the poll succeeded before reading event fields.
- Use the event's documented coordinate space; do not mix display-local coordinates with global desktop coordinates.
- Treat event coordinates as data to validate, not as guaranteed valid pixel-write coordinates.
- Keep button/source fields within the documented enum or value range.
- Avoid doing expensive work for every pointer-move event; coalesce or throttle work if the application design allows it.
- Ensure the loop has an exit condition and does not poll aggressively while idle without a scheduling strategy.
- Avoid logging every movement in production. Event streams can be noisy and may reveal user interaction patterns.

## Platform and permission limits

The package README currently documents live X11 pointer input on Linux and unavailable global input capture on macOS and Windows pending their respective permission-aware hooks. This is a statement about the current project implementation, not a general statement that those operating systems cannot support input APIs. Platform support must be checked against the source and actual build in use.

Global input capture can have privacy and security implications. Respect operating-system permission prompts and user consent; do not attempt to bypass accessibility, input-monitoring, or desktop-session security boundaries.

## Example

See [04-input-polling.sleela](../examples/04-input-polling.sleela). It is intended to show bounded polling and basic event reporting. Before running it, confirm that input capture is implemented and permitted on the target host. On a host without support, the correct result is a clear unavailable/unsupported status, not fabricated events.

## Troubleshooting

- **No events:** verify the backend, session type, input capability, and permissions before assuming the loop is broken.
- **Repeated empty polls:** ensure the program backs off or returns to its main loop instead of spinning indefinitely.
- **Unexpected coordinates:** confirm whether the API reports display-local or desktop-global coordinates.
- **Unexpected gesture order:** inspect raw events and avoid assuming a universal sequence.
- **High CPU use:** bound polling, sleep or yield using facilities supported by the runtime, and avoid unbounded busy loops.

**Checkpoint:** explain why “no event” is not necessarily an error and why event polling needs both a bound and an idle strategy.