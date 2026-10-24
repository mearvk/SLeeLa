# Lesson 5 — Mouse and Touch Input

Ballum normalizes pointer input into raw events and, in the higher-level listener, gestures such as click and drag. The documented gesture codes are:

| Code | Gesture |
|---:|---|
| 0 | NONE |
| 1 | DOWN |
| 2 | MOVE |
| 3 | CLICK |
| 4 | DRAG_BEGIN |
| 5 | DRAG_MOVE |
| 6 | DRAG_END |

Input is polled rather than callback-driven. A frame can drain available events until the poll returns no event, but a tutorial or diagnostic should use a bounded loop so it cannot hang indefinitely.

Platform support differs: the package README currently documents live X11 pointer input on Linux and unavailable global input capture on macOS and Windows pending their respective permission-aware hooks. See [04-input-polling.sleela](../examples/04-input-polling.sleela).
