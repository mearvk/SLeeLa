# Skya Client GUI and Monitoring Circuit

The Skya Client now has a JavaFX launch path. The GUI loads a declarative BODI monitoring description and starts the SLeeLa client runnable as its monitoring circuit.

Flow:
Skya client launcher -> native Skya client build -> JavaFX SkyaApp -> BODI monitoring XML -> SkyaClient.sleela -> SLeeLa runner -> monitoring surface

Linux: ./telephony-skya/build/linux/client.sh

macOS: ./telephony-skya/build/macos/client.sh

Windows: powershell -ExecutionPolicy Bypass -File .\telephony-skya\build\windows\client.ps1

SKYA_SLEEELA_CIRCUIT overrides the circuit path. SLEELA_COMMAND overrides the SLeeLa executable name/path.

The JavaFX layer is a monitoring/control surface; native Skya remains authoritative for networking and session state.


## Guia™ GUI Protocol

Document that the JavaFX GUI uses the repository's Guia™ 1.0 protocol for all GUI-to-SLeeLa client/listener interaction. BODI describes the UI; Guia™ supplies the runtime contract; the JavaFX adapter implements that contract.

See `docs/SLEELA_GUI_PROTOCOL.md`, `docs/GUIA_PROTOCOL_REFERENCE.md`, `docs/GUIA_OBJECTS.md`, and `docs/GUIA_TRANSITIONS.md` for the normative Guia™ references.
