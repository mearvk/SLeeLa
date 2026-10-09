# LINC-8 Architecture

The LINC-8 execution model contains two explicit instruction environments.

1. Fetch the next 12-bit word using the active machine state.
2. Dispatch to the LINC decoder or PDP-8 decoder according to the active mode.
3. Execute only instructions valid for that mode.
4. Apply mode transitions only through the defined instruction or machine-control path.
5. Route I/O through the device map associated with the active machine profile.
6. Preserve mode and pending I/O state across save/restore.

Do not guess mode-switch opcodes or address translation. The profile must supply these details. Unsupported instructions should produce a structured diagnostic without being reinterpreted under the other decoder.