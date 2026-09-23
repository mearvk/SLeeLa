# SLeeLa Audio and Video API

Native-frame media foundation with broad format and codec discovery.

## Images

Recognized image families:
- BMP
- JPEG/JPG
- PNG
- GIF and APNG
- WebP
- TIFF
- JPEG 2000 and JPEG-LS
- AVIF
- HEIF/HEIC
- OpenEXR
- Radiance HDR
- PAM/PNM
- DPX

## Video

Recognized video codecs:
- H.263
- H.264/AVC
- H.265/HEVC
- AV1
- MPEG-1 Video
- MPEG-2 Video
- MPEG-4 Part 2
- Theora
- VP8
- VP9
- FFV1

Recognized containers:
- MP4
- MKV/Matroska
- WebM
- Ogg
- MPEG-TS
- MOV/QuickTime
- AVI
- 3GP

## Audio

Recognized audio codecs:
- PCM
- AAC
- ALAC
- AMR
- FLAC
- G.711
- G.722
- MP3
- Opus
- Vorbis

Recognized containers include WAV, AIFF, FLAC, MP3, ADTS, MP4, Ogg, WebM and MKV.

## Provider boundary

The registry identifies formats and codecs; it does not claim that every encoder/decoder is built into the core. SLMediaProviderInfo provides a stable place for native, FFmpeg, hardware, or platform media providers to advertise decode/encode capabilities.

Decoded media enters the same timestamped SLMediaFrame path used by analysis, visualization, transforms, listeners and future AI hooks.

This follows the practical distinction between containers and codecs: a container can carry multiple codec families, and codec availability is platform/provider dependent.