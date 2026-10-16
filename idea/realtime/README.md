# Idea System(TM) — realtime canvas (C/C++ rendering + physics)

*From Author; distributive.*

A native rendering + physics module the **Sleela runtime can reference and load**
for the cases where a VM does not have realtime canvas support natively. It is
split into two layers behind a single C ABI (`idea_canvas.h`):

| Layer | File(s) | Dependency | Built & verified here? |
|---|---|---|---|
| **Physics / compute core** | `idea_canvas_sim.cpp` | C++20 + OpenMP | ✅ Yes |
| **Render (OpenGL)** | `idea_canvas_gl.cpp` | GLFW + OpenGL 4.4 + a loader | ❌ No — see below |
| No-GL stub | `idea_canvas_gl_stub.cpp` | none | ✅ Yes |
| Benchmark / demo | `idea_canvas_bench.cpp` | C++20 + OpenMP | ✅ Yes |

## The stack

As recommended: **C++20, OpenMP, GLFW, modern OpenGL**.

- A large **contiguous `std::vector<idea_vertex>`** is the canvas vertex block.
- **OpenMP** parallelises the decay/flow step across cores, with `#pragma omp
  simd` vectorising each row.
- The GL layer maps **that exact memory block** into a **persistent-mapped,
  coherent, triple-buffered** OpenGL buffer (`glBufferStorage` +
  `GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT`), with a `glFenceSync` per region
  so the CPU never writes a region the GPU is still reading. No per-frame
  `glBufferData` re-upload.
- **Hz contract:** present at **60 Hz** (vsync), advance the live simulation at
  a deterministic **120 Hz** fixed timestep (`idea_canvas_advance`).

## Build

### Physics core + benchmark (anywhere; no GPU/display)

```sh
cd idea/realtime
make run        # builds libideacanvas.a + idea-canvas-bench, runs the benchmark
```

### With the OpenGL render layer (needs GLFW + a GL loader + a display)

```sh
cd idea/realtime
cmake -S . -B build -DIDEA_CANVAS_WITH_GL=ON
cmake --build build
```

Provide a GL loader (glad or GLEW) as noted in `CMakeLists.txt` and
`idea_canvas_gl.cpp` (include it **before** `<GLFW/glfw3.h>`).

## Verified performance (this environment)

Physics core, 8 OpenMP threads, release build:

```
grid 128x128, 200 steps -> ~9,000 steps/s  (≈149 M vertex-updates/s)
headroom vs 120 Hz target: ~75x
```

Three excited points produce three spreading, slowly-decaying ripples — the
colour-predictive "water" flow field the renderer draws.

## CPU / GPU backend selection (auto-offload)

The VM chooses where the simulation runs and **falls back to the GPU when the
CPU cannot sustain 120 Hz**:

- `idea_canvas_set_policy(c, p)` — `AUTO` (default), `FORCE_CPU`, `FORCE_GPU`.
- Under **AUTO**, `idea_canvas_advance` measures sustained steps/sec
  (`idea_canvas_measured_hz`). If that drops below the 120 Hz budget for a short
  streak **and** a GPU backend is available, the VM migrates the canvas to the
  GPU (`idea_canvas_active_backend` becomes `GPU`), carrying its field state.
- With no GPU available, it stays on the CPU and reports honestly (no silent
  failure); `FORCE_GPU` returns `NO_RENDERER`.

The GPU backend (`idea_canvas_gpu.cpp`) is an **offscreen, windowless** GL
compute path: **EGL-surfaceless** on a real GPU, or **OSMesa** (software GL in
CPU/native memory) as a fallback — so it runs on a server with no display. The
step is a ping-pong FBO fragment shader; `glReadPixels` brings the field back
into the native vertex block on readback.

## Cross-platform rendering (Windows 10+, macOS, Linux)

On-screen and GPU rendering use **open standards only** — OpenGL plus each OS's
standard context binding — behind one interface (`idea_canvas_ctx.h`), selected
at build time by OS:

| Platform | Standard window + GL context | File |
|---|---|---|
| **Windows 10+** | Win32 + **WGL** (`wglCreateContextAttribsARB`, core profile) | `idea_canvas_ctx_wgl.cpp` |
| **macOS** | Cocoa + **CGL / NSOpenGLContext** (4.1 core) | `idea_canvas_ctx_cgl.mm` |
| **Linux/Unix** | **EGL** (surfaceless) / GLX, **OSMesa** software fallback | `idea_canvas_ctx_egl.cpp` |

No proprietary APIs (no D3D/Metal). The only hand-written glue is the minimal
per-OS window/context shim, for which no cross-platform standard exists. The
Windows and macOS builds compile their file via the native toolchain
(`build-windows.ps1` delegates to the same `impl/Makefile`; macOS uses the
Darwin branch); the `.mm`/WGL files are guarded so they are inert on other OSes.
`idea_canvas_gpu_available()` reflects whether the platform context layer can
create a context.

> Verified here: Linux build selects and compiles the EGL file (as the no-GL
> stub) and the VM runs. The Windows (WGL) and macOS (CGL) context code is
> written against the standard APIs but is **not compiled or run in this Linux
> sandbox** — build and run it on Windows 10+ / macOS.

Sleela built-ins: `ideaCanvasSetBackendPolicy`, `ideaCanvasPolicy`,
`ideaCanvasActiveBackend`, `ideaCanvasGpuAvailable`, `ideaCanvasMeasuredHz`,
`ideaCanvasMigrate`.

## ⚠️ What is NOT verified here

`idea_canvas_gl.cpp` (the GLFW/OpenGL layer) is **written but not compiled or
run** in the Idea System sandbox: there are no GL/GLFW headers, no GPU, and no
display. Treat it as reviewed-but-unverified; build and run it on a machine with
a GPU + display. The physics core, the no-GL stub, the benchmark, and the ABI
are all built and verified here.

When built without `IDEA_CANVAS_WITH_GL`, the render entry points return
`IDEA_CANVAS_NO_RENDERER` and the physics core runs fully headless.

## How the Sleela runtime loads it

The module exposes one flat `extern "C"` ABI (`idea_canvas.h`). A VM that lacks
native realtime-canvas support can `dlopen`/`LoadLibrary` the built library and
bind these entry points as native built-ins:

- `idea_canvas_create` / `idea_canvas_destroy`
- `idea_canvas_vertices` (direct access to the shared block)
- `idea_canvas_excite`, `idea_canvas_step`, `idea_canvas_advance` (physics)
- `idea_canvas_open_window`, `idea_canvas_present`, `idea_canvas_run` (render)
- `idea_canvas_has_renderer` to detect whether the GL layer is present

Because the physics and render layers share one ABI, a VM can use just the
compute core headless, or the full windowed renderer, with the same binding.

## Sleela binding: `lib/idea/SLIdeaCanvas.sleela`

A SLeeLa standard-library class binds this module so a runnable can drive the
canvas **on the native CPU** directly:

```
SLIdeaCanvas cv = new SLIdeaCanvas();
cv.create(256, 256);                       // allocate the native vertex block
cv.setParams(0.25, 0.9, 205.0, 0.06);      // water-like flow
cv.excite(128, 128, 1.0);                  // an importance lands
if (cv.hasRenderer()) {
    cv.run(720, 480, "Idea System(TM)");   // native window: 120 Hz sim / 60 Hz present
} else {
    cv.step(cv.timestep());                // headless: advance on the native CPU
}
cv.destroy();
```

Each method bottoms out in an `ideaCanvas*` native-bridge built-in. The physics
step runs on the **native CPU** (OpenMP across every core, SIMD per row); the
optional OpenGL layer presents the same block. Where a VM links this module,
the built-ins bind to the entry points below; where it does not, the render
calls return `NO_RENDERER` and the compute core still runs.

The binding references these ABI entry points (the first two added for it):
`idea_canvas_set_params`, `idea_canvas_intensity_at`, plus `create`, `destroy`,
`cols`/`rows`/`count`, `excite`, `step`, `advance`, `timestep`, `open_window`,
`should_close`, `present`, `run`, and `has_renderer`.
