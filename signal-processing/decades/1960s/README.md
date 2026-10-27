<!-- Author: Max Rupplin — MEARVK LLC — 2026 -->

# 1960s — Digital signal processing takes shape

Research groups explored how programmable computers could carry out analysis
previously performed with analog or specialized techniques. The Cooley–Tukey
fast Fourier transform paper appeared in 1965, making Fourier analysis more
practical for large sampled sequences. Research also included geophysics,
radar/sonar, speech and spectrum analysis.

**Library connection:** slsp_fft() implements an in-place radix-2 transform;
slsp_dft() is a slower direct reference for small sizes and tests. Both use a
documented forward/inverse sign convention; the inverse is normalized by N.

Further reading: [IBM Research: Cooley–Tukey paper](https://research.ibm.com/publications/an-algorithm-for-the-machine-calculation-of-complex-fourier-series) and [MIT DSP Group history](https://dsp-group.mit.edu/history/).
