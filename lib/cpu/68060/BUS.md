# Motorola 68060 Bus

The 68060 uses a 32-bit address/data system interface with burst-oriented memory transactions.

Transactions expose:

- address;
- function code;
- operation;
- transfer size;
- data;
- byte enables;
- burst state;
- acknowledge;
- bus error;
- arbitration state.

DMA and other bus masters remain outside the CPU execution core.
