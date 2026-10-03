<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa Video

**Version:** 0.1.0-dev  
**Author:** Max Rupplin - MEARVK LLC - 2026

The SLeeLa Video subsystem provides portable C and C++ contracts for video frames, timing, pixel formats, codec identification, capability reporting, and safe codec dispatch. Codec metadata is separated from third-party encoder/decoder implementations.

Pipeline: `container/input → codec → validated VideoFrame → renderer/mixer → output`

The initial registry covers RAW, H.264/AVC, H.265/HEVC, AV1, VP8, VP9, MPEG-2 Video, Theora, Motion JPEG, FFV1, ProRes, and DNxHD/DNxHR.

```sh
make -C video test
```

## Corrections

Fixed so `make` builds and tests clean: the missing `src/sleela_video.cpp` thin
C++ wrapper was added, and the C++ `Frame` constructor now populates the
underlying `raw.{width,height,format}` fields (previously left zero, so
`Frame::valid(...)` wrongly failed). See the 2026-10-03 entry in
[`../REVISIONS.md`](../REVISIONS.md).