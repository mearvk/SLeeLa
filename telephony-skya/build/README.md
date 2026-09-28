# Skya Platform Builds

The `telephony-skya/build` tree is the platform-specific build surface for Skya. Each operating system keeps its compiler/build entry point, native outputs, GUI launchers, and runtime branding together.

## Platform layout

```text
build/
├── linux/    Linux build + launch surface
├── windows/  Windows 10+ build + launch surface
└── macos/    macOS build + launch surface
```

Each platform directory is an independent build entry point. The generated native executables remain inside that platform directory.

## GUI separation

- **Skya Client** → `SkyaClientApp`: Chat, Audio, Video, File Transfer.
- **Skya Admin** → `SkyaApp`: local SLeeLa circuit/process monitoring and administrative lifecycle controls.
- **Remote Client** → `SkyaConnectApp`: remote connection surface.

Guia™ remains the GUI-to-client/listener contract; JavaFX is the presentation adapter.

## Branding and design philosophy

The Skya logo is the single repository-owned asset at `images/skya-logo-blue.jpeg`. Platform builds do not recreate or substitute the logo. Each build stages the exact repository asset at:

```text
build/<os>/assets/skya-logo-blue.jpeg
```

The GUI uses a restrained left-aligned header: logo first, then application title and role. The intent is strong identity without consuming the primary workspace. Scaling preserves the source aspect ratio and avoids distortion.

The same logo and visual hierarchy are used by both the normal Skya Client and Skya Admin so the applications read as one product family while remaining operationally distinct.

## Important scope

The platform build folders provide real native compilation and JavaFX launch paths. The current Audio/Video/File controls remain a GUI/runtime boundary; production media codecs, capture/playback, NAT traversal, authentication, and transport implementation are separate native work.
