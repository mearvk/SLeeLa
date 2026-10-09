# PDP-12 Memory and I/O

The PDP-12 profile separates shared memory access from instruction-environment-specific I/O semantics.

- Memory words use the configured 12-bit model and validated address range.
- I/O requests are decoded according to the active instruction environment.
- Devices are pluggable and identified by the selected machine profile.
- Unknown device codes or unsupported functions return structured errors and diagnostics.
- Device extensions must document their effects on memory, processor state, and instruction sequencing.

Do not assume every PDP-8 peripheral or LINC interface is installed on every configuration.