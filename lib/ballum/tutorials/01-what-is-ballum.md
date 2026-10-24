# Lesson 1 — What Is Ballum?

Ballum is the SLeeLa interface for discovering displays and, where the host supports it, performing display operations and polling pointer input. Its intended surface includes display enumeration, display geometry and capability queries, pixel-level writes, brightness/color controls, and mouse or touch-style event handling.

Ballum is not a promise that every operation works on every machine. Display systems differ by operating system, driver, hardware, permissions, session type, and whether the program is running in a graphical or headless environment. Good Ballum code checks what the host actually offers and treats unavailable features as normal conditions.

## The three main layers

1. **SLeeLa facade — `lib/ballum/`**  
   This is the developer-facing layer. It organizes the API into SLeeLa classes, provides a consistent shape for calls, and should check arguments and capabilities before dispatching work.
2. **VM bridge — `impl/core/ballum_vm_bridge.*`**  
   This is the boundary between SLeeLa code and native code. The SLeeLa-facing `uiBallum*` operations need a corresponding bridge implementation in the runtime that is actually being executed.
3. **Native core and platform backend — `ballum/`**  
   This layer owns platform-facing operations and reports whether they succeeded. The selected backend may be unavailable or may implement only a subset of the API.

Think of the path as a chain of responsibility: the SLeeLa program requests an operation, the VM bridge forwards it, the native manager validates and dispatches it, and a platform backend attempts the operating-system operation. A failure at any stage must remain visible to the caller.

## Four different kinds of success

These cases are not interchangeable:

- **The source is valid:** the compiler accepts the program.
- **The API call is valid:** the arguments and ranges pass validation.
- **The feature is available:** the runtime and selected backend support the requested operation.
- **The operation succeeded:** the native host reports that the operation completed.

For example, a display-control method can exist in a class even when the current host cannot change the physical display's brightness. Likewise, a demo that updates local model state or uses a fabricated handle demonstrates an API shape, not a real monitor operation.

## First run: inspect before changing

Start with [05-headless-safe.sleela](../examples/05-headless-safe.sleela). It is intended to demonstrate graceful handling when no usable display backend is present. Then review [01-screen-inspection.sleela](../examples/01-screen-inspection.sleela) to learn what information to collect from each discovered display.

Before executing an example, check the source version declaration, confirm that your compiler supports it, and follow the repository's current build/run instructions. These examples have not been independently certified for every SLeeLa compiler/runtime combination; treat compiler feedback as actionable and correct source/runtime mismatches rather than assuming the file is executable just because it is in the repository.

## A good diagnostic log

For a useful report, capture:

- the operating system and whether the session is graphical or headless;
- whether the VM bridge is included in the running build;
- the returned status code and readable status name;
- the selected display ID and its reported capabilities;
- the operation and arguments that failed.

Avoid logging sensitive input data or collecting pointer events beyond what the application needs.

## Practice and review

1. Explain the role of each of the three layers without conflating them.
2. Explain why a successful compile does not prove that a monitor setting changed.
3. Run or inspect the headless-safe example and identify the path it takes when no backend is available.
4. Read the [native bridge lesson](07-native-bridge.md) before investigating a host integration issue.

**Learning objective:** distinguish source validity, API validation, backend availability, and a successful operating-system operation.