# Sleela VoIP Class API

## Sleela-Complete

Use **Sleela-Complete Application** when the application itself is authored entirely in Sleela source. **Sleela-Backed** means Sleela source is implemented by native C/C++ and platform layers underneath. **Native** refers to those implementation layers. This terminology lets us say precisely that an application is entirely written in Sleela while still using native backends.

## Class surface

Application: VoIPApplication, VoIPAccount, VoIPSession.

SIP: SipEndpoint, SipMessage, SipCall.

Session description: SdpSession, SdpMedia.

Media: RtpPacket, RtpSession, RtcpSession.

Codecs: Codec, CodecSet.

Audio: AudioFormat, AudioBuffer, AudioStream, CaptureStream, PlaybackStream, AudioInput, AudioOutput.

Devices: AudioDevice, AudioDeviceManager, Microphone, Speaker, Headset.

## Architecture

Sleela-Complete application -> VoIPApplication -> SIP/SDP -> RTP/RTCP -> Codec -> Capture/Playback -> Audio devices -> Linux/Windows/macOS backend -> hardware.

A developer should be able to construct the application in the Sleela editor without directly authoring the C/C++ driver implementation. Native and driver layers implement the public Sleela contracts.

## Security

Production implementations must support secure signaling and secure media as first-class capabilities, including TLS where appropriate, SRTP/SRTCP, authentication, certificate validation, replay protection and secure credential handling.

## Status

These classes define the application-facing source vocabulary. State-only method bodies are API contracts, not proof of production interoperability. Native implementation still needs packet I/O, timing, jitter buffering, codecs, device enumeration, platform audio, NAT traversal, security and interoperability testing.
