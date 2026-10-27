<!-- Author: Max Rupplin — MEARVK LLC — 2026 -->

# 1990s — Multiresolution, filter banks and digital media

Wavelet analysis, multirate filter banks, subband coding, and transform-based
media systems became important research and engineering tools. Different
representations serve different signals: a single global Fourier spectrum is
not always enough to describe transient or nonstationary behavior.

**Library connection:** the current FFT, Hann window and convolution functions
can act as testable components for short-time analysis. This release does not
claim to provide wavelet transforms, perceptual codecs, or a complete
multirate filter-bank implementation.

Suggested exercise: add short-time framing with explicit hop size and window
normalization; test how it handles impulses and transients.
