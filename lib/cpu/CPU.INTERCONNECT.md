# CPU Interconnect and I/O Bus Specification

The interconnect layer represents CPU-to-memory, CPU-to-device and device-to-device transactions.

A bus transaction records:

- requester/master
- target/slave
- address
- width
- read/write direction
- byte enables
- burst/beat information
- priority
- clock domain
- request, accept, transfer and completion timing
- ordering/barrier requirements
- cacheability and coherency attributes

ARM AMBA AXI explicitly defines master/slave transaction roles and recognizes DMA as a component that can both be programmed as a slave and initiate memory transactions as a master. citeturn0search36

The SLeeLa abstraction is deliberately protocol-neutral so Z80, 68000, SH-2, x86-64, ARM and custom console SoCs can use the same coupler interfaces.