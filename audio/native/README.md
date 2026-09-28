# SLeeLa Native Audio

Native command-line adapter for the SLeeLa Audio API.

## Contract

`sleela-audio-native --output OUTPUT --sample-rate RATE --input PATH START_SECONDS GAIN_DB ... [controls]`

The adapter accepts PCM16 mono/stereo WAV inputs and produces stereo PCM16 WAV output. EQ fields are carried by the contract and currently remain reserved; bass/mid/treble are not silently claimed as implemented.
