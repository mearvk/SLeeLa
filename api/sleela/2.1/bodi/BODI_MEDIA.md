# BODI Media Extension

BODI reserves kind="media" for declarative audio/video/image processing projects.

Supported vocabulary includes:
- audio codec declarations for PCM, AAC, ALAC, AMR, FLAC, G.711, G.722, MP3, Opus and Vorbis
- video codec declarations for H.263, H.264/AVC, H.265/HEVC, AV1, MPEG-1/2/4, Theora, VP8, VP9 and FFV1
- common media containers including MP4, MKV, WebM, Ogg, MPEG-TS, MOV, AVI, 3GP, WAV, AIFF and ADTS
- image declarations for BMP, JPEG, PNG, GIF/APNG, WebP, TIFF, JPEG 2000, JPEG-LS, AVIF, HEIF/HEIC, EXR, HDR, PAM/PNM and DPX
- audio/video analysis and visualization
- controlled XML drop-in listeners
- future AI providers at the native media-frame boundary

BODI describes the requested media family; an installed provider must still advertise the actual encode/decode capability. This avoids making a particular external codec library mandatory.

A media XML document does not grant arbitrary native, shell, or unrestricted device execution. Drop-in XML is validated before admission to the processing queue.