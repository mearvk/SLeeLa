# SLeeLa Video Codec Registry

**Version:** 0.1.0-dev

The registry is a capability and dispatch contract. It does not bundle proprietary SDKs or assert that every codec has a native encoder/decoder.

| Codec | Standard family | Role | Status |
|---|---|---|---|
| RAW | Uncompressed | Interchange | Native frame boundary |
| H.264/AVC | ISO/IEC 14496-10 | General delivery | Backend |
| H.265/HEVC | ISO/IEC 23008-2 | UHD delivery | Backend |
| AV1 | AOMedia AV1 | Modern web/UHD | Backend |
| VP8 | Web video | Legacy web/RTC | Backend |
| VP9 | Web video | Web/UHD | Backend |
| MPEG-2 Video | ISO/IEC 13818-2 | Broadcast/DVD | Backend |
| Theora | Ogg video | Open legacy web | Backend |
| Motion JPEG | JPEG sequence | Camera/editing | Backend |
| FFV1 | Lossless intra-frame | Preservation | Backend |
| ProRes | Production family | Editing/mastering | Qualified backend |
| DNxHD/DNxHR | Production family | Editing/mastering | Qualified backend |

Registry presence is not an implementation or license grant. Each decoder must bound dimensions, frame sizes, reference frames, timestamps, and allocations; detect overflow; reject truncation; and report unsupported profiles.
