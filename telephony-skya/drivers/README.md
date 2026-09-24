# Skya Telephony Drivers

The `drivers/` tree is the hardware-driver boundary for Skya telephony.

It covers SIP/VoIP desk phones, USB-connected phones, USB/Bluetooth headsets, speakerphones, and call-control peripherals.

## Architecture

```
Skya GUI / SLeeLa
      |
      v
Skya Telephony Driver API
      |
      +-- discovery
      +-- capabilities
      +-- audio
      +-- call-control HID
      +-- mute / hook / volume
      +-- firmware/version reporting
      |
      +-- vendor adapters
      |
      v
OS audio / USB / HID / network stack
```

The first driver layer is capability-oriented. A catalog entry does not claim that every function is implemented. The runtime must detect the actual device, transport and firmware before enabling optional controls.

See `BRANDS.md` and `brands/versions/README.md` for the initial hardware matrix.
