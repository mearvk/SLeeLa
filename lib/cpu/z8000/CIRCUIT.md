# Zilog Z8000 Circuit Model

Functional model, not transistor-level reconstruction. Blocks: instruction fetch/register, variable-length decoder, R0-R15 register file, effective-address generator, integer/logic unit, condition-code/status logic, optional Z8001 segment unit, memory/I/O interface, interrupt/control logic, clock/sequencing. SLCoupler models fetch-to-decode, decode-to-registers, registers-to-execution, address-to-memory, execution-to-status, interrupt-to-control, and DMA-to-bus-arbitration paths.
