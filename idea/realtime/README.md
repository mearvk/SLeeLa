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
