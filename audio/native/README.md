<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">







# SLeeLa Native Audio

Native command-line adapter for the SLeeLa Audio API.

## Contract

`sleela-audio-native --output OUTPUT --sample-rate RATE --input PATH START_SECONDS GAIN_DB ... [controls]`

The adapter accepts PCM16 mono/stereo WAV inputs and produces stereo PCM16 WAV output. EQ fields are carried by the contract and currently remain reserved; bass/mid/treble are not silently claimed as implemented.