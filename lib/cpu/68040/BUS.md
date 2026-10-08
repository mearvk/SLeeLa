# Motorola 68040 Bus

The 68040 uses a 32-bit address/data system interface and supports burst-oriented memory transfers.

SLeeLa transactions expose:

- address;
- function code;
- read/write;
- transfer size;
- data;
- byte enables;
- burst state;
- acknowledge;
- bus error;
- arbitration state.

DMA and other bus masters remain system-level resources.
