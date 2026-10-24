# Ballum Examples

These examples demonstrate the SLeeLa-facing Ballum API and its native bridge boundary. They complement the aggregate demos in the parent directory; they do not replace testing on a real display.

## Start Here

| Example | Focus | Notes |
|---|---|---|
| [01-screen-inspection.sleela](01-screen-inspection.sleela) | Enumerate displays and inspect geometry/capabilities | Requires the host's Ballum VM bridge |
| [02-safe-pixel.sleela](02-safe-pixel.sleela) | Validate coordinates and issue a single pixel write | Deliberately includes an out-of-bounds case |
| [03-display-controls.sleela](03-display-controls.sleela) | Brightness, warmth, and gamma | Each control is capability-gated |
| [04-input-polling.sleela](04-input-polling.sleela) | Bounded pointer-event polling | Does not wait indefinitely |
| [05-headless-safe.sleela](05-headless-safe.sleela) | Exercise honest failure paths | Suitable for headless smoke tests when bridge symbols are available |

## Running

From the repository root, use the compiler/runtime commands documented by the SLeeLa build instructions. For example, where `sleelvac` is installed:

```sh
sleelvac lib/ballum/examples/01-screen-inspection.sleela
```

If your local toolchain separates compilation and execution, follow its documented invocation. These examples target SLeeLa 2.1 syntax, matching the current Ballum demo files.

## Safety

- Run display-writing examples only on a machine where you have permission to change display settings.
- Start with `05-headless-safe.sleela` or the existing `screen-demo.sleela` when testing an unfamiliar host.
- A successful validation is not proof of a successful device operation. Always inspect the returned Ballum status.
- Never assume every display supports per-pixel writes, brightness, warmth, gamma, or global pointer capture.
