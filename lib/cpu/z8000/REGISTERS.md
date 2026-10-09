# Zilog Z8000 Registers

SLeeLa models the sixteen 16-bit general registers R0-R15, including byte access and register-pair operations where supported by the instruction/profile.

Additional state includes:

- program counter;
- status/condition-code state;
- stack and system-control state;
- interrupt and privilege state;
- Z8001 segment/address state where applicable.

Profile differences are explicit; the Z80's AF/BC/DE/HL register-bank model is not reused.
