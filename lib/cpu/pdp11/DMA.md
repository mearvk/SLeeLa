# DEC PDP-11 DMA

PDP-11 systems commonly use bus-master peripheral controllers for DMA.

SLeeLa models DMA through:

SLDMAController
-> SLBusArbiter
-> PDP-11 system bus
-> memory/device

DMA is not assumed to be an internal CPU execution unit.
