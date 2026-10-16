# Sleela-Native — Provenance & License

*Idea System(TM) / Idea(TM). From Author; distributive.*

## License

**SPDX-License-Identifier: MIT**

Copyright (c) 2026 MEARVK LLC / the Author.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the inclusion of the above copyright notice and this permission
notice in all copies or substantial portions of the Software. THE SOFTWARE IS
PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.

## Originality statement

Every source file in `idea/sleela-native/` is **clean-room original work**
written for this project. No third-party source code was copied, adapted, or
vendored into this directory. Specifically:

- **Math** (`sln_math.h`) — our own vector/matrix/camera math.
- **GL function loader** (`sln_gl_loader.*`) — our own; it **replaces** GLEW and
  GLAD. We depend on neither.
- **Windowing / context** — uses the platform standards directly (WGL/CGL/EGL)
  via `idea/realtime/idea_canvas_ctx*`; we **do not** use GLFW, SDL, or similar.
- **Renderer + GLSL** (`sln_render_gl.*`) — our own shaders and draw logic.
- **Scene model** (`sln_scene.h`) — our own camera, moving light, procedural
  model, vignette, and sound-cue model. No imported assets.
- **Audio** (`sln_audio.*`) — our own PCM mixer and WAV writer; per-OS device
  output is our own thin backend (WASAPI/CoreAudio/ALSA), no third-party audio
  library.

## What we reference (standards and OS APIs — not copied code)

The code *calls* open standards and operating-system APIs. These are
specifications / system interfaces, not third-party source we redistribute:

| Referenced | Nature | Why it is safe |
|---|---|---|
| **OpenGL** (3.3 core / ES3), **GLSL** | Khronos open standard | An API/spec; calling it is not copying it. Shaders here are our own. |
| **WGL / CGL / EGL** | Standard GL context bindings per OS | OS/standard interfaces; our glue is original. |
| **Win32, Cocoa/AppKit** | OS windowing APIs | System APIs provided by the OS. |
| **WASAPI / CoreAudio / ALSA** | OS audio APIs | System APIs; our mixer is original. |

No GPL/LGPL code is included; nothing here imposes a copyleft obligation. The
only runtime dependencies are the operating system's own graphics/audio drivers
and the C++ standard library.

## Ethical / safety notes

- The renderer draws a procedural geometric model and a configurable light and
  vignette — no scraped, trademarked, or third-party assets.
- The audio engine synthesises simple tones; it ships no sampled/licensed audio.
- Device I/O (window, GPU, speaker) uses only the user's own system via its
  public APIs; nothing is accessed covertly.

If a reviewer finds any passage that appears derived from a specific external
source, treat it as an oversight to be rewritten — the intent and the practice
here is 100% original authorship.
