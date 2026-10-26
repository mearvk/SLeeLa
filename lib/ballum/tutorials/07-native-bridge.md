# Lesson 7 — The Native Bridge and Platform Limits

The SLeeLa facade is not itself an operating-system display driver. The native core lives under the repository's `ballum/` tree, while `impl/core/ballum_vm_bridge.*` provides the VM-facing bridge. A source-level method can only perform real device I/O if the running VM has the bridge implementation linked and the selected native backend can perform the requested operation.

## Trace one request end to end

1. A SLeeLa program enumerates displays and checks the target's capabilities.
2. The program calls the appropriate `uiBallum*` function.
3. The VM resolves that function to the Ballum bridge implementation.
4. The bridge validates and converts the VM arguments into the native API's expected types.
5. The native manager checks initialization, display identity, ranges, and operation support.
6. The selected platform backend attempts the operating-system operation.
7. The status travels back through the bridge to the SLeeLa caller.

This path is useful when debugging. If a SLeeLa method compiles but the runtime reports that a bridge is required, the issue may be the runtime build or registration path rather than the native display driver. If the bridge is reached but the backend is unavailable, investigate backend selection and platform support. If a backend call returns permission denied, follow the operating system's documented permission process.

## Platform support must be verified in source

The native specification currently describes Linux `x11-xrandr`, macOS CoreGraphics, and Windows 10+ GDI display backends. These names indicate intended implementation paths, not identical feature parity. The package README also documents current limitations for pointer capture. Always verify the actual backend files, build configuration, runtime selection, and tests for the commit being evaluated.

A backend may support display enumeration but not a particular control. It may compile on a platform but not be selected in the current runtime. It may be selected but fail at runtime because the host is headless, the display is inaccessible, or permission is denied. Report these cases separately.

## Integration checklist

### Build and registration

- Confirm `impl/core/ballum_vm_bridge.c` and its header are included in the intended VM build.
- Confirm every SLeeLa-visible `uiBallum*` operation has a matching bridge implementation and consistent argument/result handling.
- Check that native symbols are linked and that initialization/cleanup order is correct.
- Verify that a model/demo path cannot accidentally masquerade as a successful hardware path.

### Native boundary

- Validate sizes, IDs, ranges, and conversions at the boundary even if the SLeeLa facade already checks them.
- Avoid unchecked pointer dereferences, integer truncation, and buffer-size assumptions.
- Preserve native error codes and translate them only according to a documented mapping.
- Ensure resources and event buffers are released on both success and failure paths.
- Keep operations bounded and avoid holding locks across slow platform calls unless required by the design.

### Platform behavior

- Verify the backend chosen at runtime for each supported OS.
- Test a headless or unavailable-backend path.
- Check capabilities per display and per operation.
- Document required session type and permission.
- Treat unsupported features as unsupported; do not return success with simulated output.

### Evidence

- Run native unit tests and record the actual result.
- Compile each SLeeLa example with the intended compiler/runtime.
- Test bridge registration and a live operation on a supported host.
- Record the operating system, build configuration, test command, and observed status.
- Keep unexecuted platform tests explicitly marked as pending.

## Model demonstrations versus real I/O

The aggregate `lib/ballum/ballum.sleela` contains model/demo behavior, including a simulated handle path. That can be useful for teaching class shape or state transitions, but it must not be used as proof that a physical display changed. Similarly, a sample that prints a success-like message without checking the bridge's returned status is not a valid integration test.

A strong test makes the result falsifiable: it checks the real status, confirms the selected display and capability, and reports failure when the native operation fails. For display controls, observe the visible change where safe and restore the original setting. For pixel operations, use a minimal, bounded test on a known display. For input, confirm the host supports capture and avoid collecting more data than needed.

## Practice: follow the status

Choose one example and trace its operation from the SLeeLa call to the bridge function and native backend. Write down:

- the SLeeLa function name;
- the bridge entry point;
- the native manager function it calls;
- the platform backend selected;
- the possible status values;
- the test that demonstrates the behavior.

If any link in that chain cannot be found or tested, document it as an integration gap rather than assuming it works.

**Final checkpoint:** distinguish the presence of API classes, the presence of bridge symbols, the availability of a platform backend, and demonstrated success on real hardware.