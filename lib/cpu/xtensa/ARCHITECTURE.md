# Xtensa Architecture

Functional path: fetch and configuration-aware decode -> register/window mapping -> integer or optional extension execution -> memory access -> architectural writeback.

Xtensa is configurable. The profile defines instruction formats, register windows (if configured), exception/interrupt behavior, privilege modes, and optional units. Custom opcodes must be registered with explicit encoding, operand, privilege, and execution semantics; unknown encodings trap or report unsupported rather than being guessed. Do not treat Xtensa LX, LX7, and other configurations as identical.
