# PDP-14 I/O and Signal Interface

Model the PDP-14 as a control-oriented interface to configured discrete signals.

- Inputs are sampled through an explicit input provider.
- Outputs are staged in an output image and committed according to profile policy.
- Every point has a configured identity, direction, and optional safety constraint.
- Unmapped points and invalid directions produce diagnostics.
- Host I/O must not be invoked implicitly by an instruction decoder; use the configured device/provider interface.

Do not impose modern industrial network protocols unless a selected hardware extension explicitly provides them.