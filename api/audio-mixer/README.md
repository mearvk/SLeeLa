# SLeeLa Synchronized Audio Mixer Configuration

Three equivalent configuration representations are provided:
- JSON: audio-mix.json
- BODI XML: audio-mix.xml
- Standard text: audio-mix.conf

## Input model
Each input has role (master, second, or input), type (file or live), a path or device, load_at_ns, start_at_ns, quality, and gain_db.

## Relative quality
quality=1.0 is the reference quality. Values below 1 reduce that track's contribution. The normalization policy is declared separately so it can evolve without changing the native mixer ABI.

## Synchronization
All sources share one processing timeline. Track offsets are preserved. The output sink may consume each completed synchronized block as it is produced, so processing and saving proceed together rather than requiring all files to be loaded first.

## Sources
The media provider boundary decodes supported audio files into native PCM frames. A live capture provider supplies the same frame form. The mixer therefore treats file and live input uniformly after acquisition.

## Controls
Bass, mid, treble, master gain, pan, and independent channel gains are exposed. The design follows established audio graph concepts for gain, frequency filters, and channel routing.