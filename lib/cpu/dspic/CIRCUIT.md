# dsPIC Functional Circuit Model

Functional block model, not a transistor-level netlist. Blocks may include instruction fetch/decode, working register file, MCU ALU, DSP multiplier/MAC and accumulators, address-generation unit, loop controller, program flash interface, data RAM interface, interrupt controller, timers, serial peripherals, ADC, DMA, and clock. Instantiate only blocks supported by the selected device profile. SLCoupler models operand delivery, MAC writeback, memory/peripheral transactions, interrupt requests, and DMA arbitration.
