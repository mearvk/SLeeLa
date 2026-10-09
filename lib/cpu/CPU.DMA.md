# CPU DMA Specification

DMA is modeled independently from the CPU execution core.

A DMA channel may contain:

- source address
- destination address
- transfer length
- width
- increment/fixed addressing
- burst size
- descriptor/list pointer
- completion state
- interrupt routing
- priority/arbitration
- cache coherency requirements

Intel's current published DMA register documentation demonstrates source/destination address registers, linked-list pointers and control/status state as concrete DMA programming concepts. citeturn0search7turn0search13

## Direct DMA option (implemented)

`SLDMAController` is a runnable direct memory-access engine, exposed as an
opt-in **option** across the stack (off by default). When enabled, a program
hands a bulk move to the controller instead of copying each word in the CPU's
fetch-decode-execute loop.

- Multi-channel (8), with per-channel busy/complete status.
- `transfer(channel, src, dst, length, mode)` — overlap-safe memory→memory block
  copy over the CPU's memory; also `memcpy(src, dst, length)` and
  `fill(channel, dst, length, value)`.
- Modes: `MODE_MEM_TO_MEM`, `MODE_MEM_TO_DEV`, `MODE_DEV_TO_MEM`.
- Accounting: transfers completed and total words moved.

Enable it at any layer:

| Layer | Enable | Access |
| --- | --- | --- |
| CPU (`SLCPURuntime`) | `enableDMA()` | `dmaEngine()`, `dmaCopy(src, dst, len)` |
| Secondary VM (`SLVM`) | `enableDMA()` before `hostCpu()` | hosted CPU inherits it |
| VM Creator (`SleelaVMCreator`) | `requestDMA()` | applied to the built VM/CPU |
| Build (`SLExecutorBuild`) | `enableDMA()` | on the build's CPU |

Corresponds to the `DMA` capability bit in `lib/vm/SleelaVMFeatureBits.sleela`.
A program that never needs DMA pays nothing; one that needs it enables the
option and calls the engine. See `lib/cpu/examples/dma-and-gpu-options.sleela`.
