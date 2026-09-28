# Debugger Command Language

Version: 0.8.0

Commands map to the existing ActionController; the command layer must never become a second execution engine.

Examples:
- `break main`
- `break file.cpp:120`
- `watch variable`
- `thread 4`
- `stack`
- `registers`
- `memory 0x1000`
- `continue`
- `step`
- `next`
- `inspect object`
- `record start`
- `record checkpoint`
- `replay`
- `evidence export`

Inputs are bounded, parsed without shell interpretation, and converted to ActionRequest objects.