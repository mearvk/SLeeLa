# Sleela-Native — original real-time renderer, physics model & audio

*Idea System(TM) / Idea(TM). From Author; distributive.*
**MIT licensed, clean-room original — see [PROVENANCE.md](PROVENANCE.md).**

A modest-but-complete, **self-contained** real-time renderer for a physical
model, written from scratch with **no third-party libraries** (no GLFW, no
GLEW/GLAD, no engine or math libs). It talks directly to open standards
(OpenGL + the OS's standard WGL/CGL/EGL context) and provides everything else
ourselves.

## What it renders

- A **physical model** — a procedural, flat-shaded solid (octahedron) on a
  ground plane, spinning in real time.
- A **camera** — an orbit camera (yaw/pitch/distance) with a perspective
  projection and a gentle auto-tour.
- A **moving light source** — a point light that orbits and bobs, with colour,
  intensity, and distance attenuation; diffuse + specular shading.
- A **vignette** post-process — a fullscreen pass darkening the frame toward the
  edges (radius / softness / strength).
- **Sound** — a PCM mixer that renders synthesised cues (e.g. as the light
  sweeps) to the speaker via a per-OS sink, or to a `.wav`.

## Files

| File | Role | Built & verified here? |
|---|---|---|
| `sln_math.h` | vectors, 4×4 matrices, lookAt/perspective | ✅ |
| `sln_scene.h` | camera, moving light, model, vignette, cues | ✅ |
| `sln_audio.{h,cpp}` | PCM mixer + WAV writer + sink interface | ✅ |
| `sln_demo.cpp` | headless demo: advances the scene + writes a WAV | ✅ |
| `sln_gl_loader.{h,cpp}` | our own GL function loader (replaces GLEW/GLAD) | compiles ✅ (not run) |
| `sln_render_gl.{h,cpp}` | OpenGL renderer + our own GLSL (lit model + vignette) | compiles ✅ (not run) |

## Build / run

### Model + audio (anywhere, no GPU)

```sh
cd idea/sleela-native
make run        # advances the scene, writes build/sleela-native-demo.wav
```

Verified output: 30-vertex model, orbiting camera + light, and a 3 s stereo
48 kHz WAV of the triggered sound cues.

### With the OpenGL renderer (GPU + display)

The renderer needs a current GL context — create one with the platform context
layer in [`../realtime/idea_canvas_ctx.h`](../realtime/idea_canvas_ctx.h)
(WGL on Windows 10+, CGL on macOS, EGL on Linux), then:

```cpp
sln::Scene scene; scene.build_default();
sln::GlRenderer r; r.init(scene, width, height);
while (running) { scene.advance(dt); r.render(scene); ctx_present(); }
```

Link against the OS GL + windowing libraries (e.g. `-lopengl32 -lgdi32` on
Windows; `-framework OpenGL -framework Cocoa` on macOS; `-lGL -lEGL` on Linux).

## ⚠️ Honest verification status

Built **and run** in the headless sandbox: the math, the scene model (camera,
moving light, vignette params), the audio mixer, and the WAV output.

**Compiled but not run here:** the OpenGL renderer, the GL loader, and the GLSL
— this environment has no GPU, GL driver, or display. They compile cleanly
against our own loader; run them on a GPU/display machine to see pixels.
