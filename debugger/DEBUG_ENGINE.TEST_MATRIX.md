# SLeeLa Debug Engine Test Matrix

| Capability | Engine model | Linux native | macOS native | Windows native |
|---|---|---|---|---|
| Breakpoints | required | ptrace binding | LLDB binding | Debug API binding |
| Watchpoints | required | hardware/OS | LLDB | debug registers/API |
| Threads | required | ptrace/wait | LLDB | Debug API |
| Stack | required | DWARF/unwind | DWARF/LLDB | PDB/unwind |
| Symbols/source | required | ELF/DWARF | Mach-O/DWARF | PE/PDB |
| Memory | required | ptrace | LLDB | Debug API |
| Registers | required | ptrace | LLDB | Debug API |
| Exceptions | required | signals | exceptions/signals | exceptions |
| Expressions | bounded unit tests | symbol integration | LLDB integration | PDB integration |
| Evidence | required | native capture | native capture | native capture |
| Capability truth | required | must match implementation | must match implementation | must match implementation |
| Reverse debugging | roadmap | native recording required | native recording required | native recording required |

A platform cell is an integration target, not a claim of current native support.