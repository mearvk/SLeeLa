# Skya JavaFX GUI

The Skya JavaFX user Client and administrative Client Monitor share a common visual identity and configuration model.

## Logo and Header

Both JavaFX applications use:

`images/skya-logo-blue.jpeg`

The Maven build copies the repository logo into the JavaFX application resources, so the GUI loads the repository asset locally rather than requiring a network request during normal execution.

The header uses a proportional, non-distorted logo scale of approximately 180px maximum width and 72px maximum height. The logo is intentionally **left-oriented** beside the application title. This provides a stable visual anchor while preserving the center and right portions of the header for future connection state, identity, or operational controls.

## Normal Client Menu Bar

The normal `SkyaClientApp` provides:

- **File**
  - Load Config
  - Save Config
  - Save Config As
  - Delete Old Config
  - Exit
- **Settings**
  - Chat Settings
  - Audio Settings
  - Video Settings
  - File Transfer Settings
- **Config**
  - Edit Current Config
  - Open Config Folder
- **Help**
  - About Skya

## Administrative Menu Bar

The administrative `SkyaApp` provides:

- **Settings**
  - Chat Settings
  - Audio Settings
  - Video Settings
  - File Transfer Settings
- **Config**
  - Load Config
  - Save Config
  - Save Config As
  - Delete Old Config
- **Service**
  - Restart Monitor
- **Help**
  - About Skya Admin

## Configuration Storage

Skya configuration files are stored under:

`~/.sleela/skya/`

Configuration names are restricted to safe filename characters and are stored as Java properties files.

The configuration layer currently provides the foundation for persistent host, port, room, and feature settings. Media transport, codecs, file-transfer engines, and native service controls remain separate implementation layers.

## Look and Feel

The GUI intentionally keeps the logo and application identity prominent without allowing the branding to consume the working area. Tabs remain dedicated to Chat, Video, Audio, and File Transfer in the normal Client, while administrative lifecycle controls remain in the separate Monitor.

Guia™ remains the common GUI-to-SLeeLa client/listener protocol boundary; JavaFX is the presentation adapter.
