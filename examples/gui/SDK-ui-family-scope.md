# The separate `lib/ui` SDK family

The repository has two similarly named but materially different source
families:

- `lib/user-interface/`: the native SleelaUI binding. Classes such as
  `SLUserInterface`, `SLWindow`, `SLWidget`, `SLButton`, and `SLTheme`
  call the VM's `ui*` built-ins, which cross the C ABI into the real native
  X11, Cocoa, or Win32 backend. The runnable window examples in this directory
  use this family.
- `lib/ui/`: the unified SDK front-end family. Its current class sources expose
  simple object/model state (for example `configure`, `show`, `hide`,
  `add`, and `visible`) and metadata accessors. They do not currently
  demonstrate the native window/widget bridge used by `lib/user-interface/`.

Do not treat an instance of `lib/ui/SLWindow` as proof that a native OS window
was opened. Use `lib/user-interface/SLWindow` and
`lib/user-interface/SLUserInterface` for actual SleelaUI windows.

## Current SDK model vocabulary

The inspected `lib/ui` sources include `SLApplication`, `SLWindow`,
`SLWidget`, `SLTheme`, `SLButton`, `SLLabel`, `SLView`, and
`SLLayout`. Their shared state pattern is intentionally modest:

- `configure(title)` stores the title.
- `show()` sets visible state.
- `hide()` clears visible state.
- `add()` increments the child count.
- `visible()` reads the state.

See [SDK-ui-object-model.sleela](SDK-ui-object-model.sleela) for a runnable
object-model exercise. It is a model/API example, not a native GUI demo.

When the SDK family gains real native bridge methods, add separate examples
with explicit bridge prerequisites rather than silently changing the meaning
of these model demonstrations.
