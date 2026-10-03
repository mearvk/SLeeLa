# Tutorial 01 — Artifact to SLeeLa IR

Treat every executable, library, object, or byte-stream input as untrusted data. Acquire and identify it without executing it; collect format and architecture evidence; decode conservatively; build control flow; lift to SLIR; perform semantic analysis; reconstruct SLeeLa or another requested target; then apply compiler/VM validation when a VM artifact is desired.

Do not claim exact original source when evidence does not support that conclusion.
