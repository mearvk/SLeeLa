# PA-RISC CPU

SLeeLa models Hewlett-Packard Precision Architecture as PA-RISC 1.0, 1.1, and 2.0 profiles rather than one generic microarchitecture.

PA-RISC 1.x is 32-bit; PA-RISC 2.0 extends the architecture to 64 bits while retaining compatibility with 32-bit PA-RISC 1.1 software. citeturn0search1turn0search12

## Composition

SLPARISCCPU composes instruction fetch/decode, 32 general registers, control/status state, integer ALU, branch/control, load/store, optional FPU/MAX units, MMU/TLB, cache hierarchy, pipeline/issue logic, trap/interrupt control, and system interconnect.

## Implementation boundary

PA-7000 through PA-7300LC and PA-8000 through PA-8900 have materially different pipelines, superscalar widths, caches, TLBs, and execution resources. These remain concrete profiles rather than being falsely normalized.
