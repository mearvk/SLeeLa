<!-- Author: Max Rupplin — MEARVK LLC — 2026 -->

# 1970s — Digital audio and practical filter algorithms

The 1970s saw important work on digital audio, restoration, filter design,
convolution, and prediction methods. Tom Stockham's work on digital recording
and remastering is a frequently noted milestone (1975). The adoption timeline
differed across laboratories, markets, and applications.

**Library connection:** slsp_fir_convolve() returns complete linear convolution;
slsp_hann_window() builds a standard analysis window. They are small building
blocks for offline filtering and block/spectrum pipelines. They do not, alone,
implement a designed low-pass or anti-alias filter.

Suggested exercise: apply a small low-pass kernel, compare edge effects, and
test its impulse response.

Further reading: [MIT DSP Group history](https://dsp-group.mit.edu/history/) and [IEEE FFT history](https://research.ibm.com/publications/historical-notes-on-the-fast-fourier-transform--1).
