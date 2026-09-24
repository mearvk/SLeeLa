# Skya Telephony Driver Version Catalog

Here, **version** means a device model, series, or hardware family. Firmware/software releases are tracked separately when tested driver behavior depends on them.

## Yealink
MP45 USB Phone; MP50 USB Phone; UH42; UH44; WH64; WH68

## Poly
Blackwire 3320; Blackwire 5220; Savi 8200 Series; Savi 8400 Series; Voyager 4320; Voyager 5200 UC

## Jabra
Biz 1500 USB; Evolve2 40; Evolve3 65; Evolve3 75; SPEAK2 55; SPEAK2 75

## Grandstream
GUV3000; GUV3005; GXP21xx; GRP26xx

## EPOS / Sennheiser
IMPACT SC 200; IMPACT SC 600; IMPACT 1000; IMPACT 500; SDW 5000; ADAPT

## Logitech
Zone Wired 2; Zone Wireless 2; Zone Vibe; H570e; Zone 305; SPEAK

## Fanvil
X4U-V2; X5U-V2; X6U-V2; V63; V64; V65; V66; V66 Pro; X210i-V2

## Snom
A330D; A330M; Snom USB Headset Family

## Cisco
320 Series headsets (321/322)

## Discovery rule

The catalog is a target matrix, not a blanket compatibility assertion. Skya should discover the physical device, identify vendor/product information, determine the OS transport, probe capabilities, match the model/family, then select the narrowest compatible driver.

Unsupported features must be reported explicitly.
