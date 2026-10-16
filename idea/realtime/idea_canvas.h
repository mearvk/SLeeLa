/*
 * idea/realtime/idea_canvas.h -- Idea System(TM) realtime canvas: public C ABI.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * A native rendering + physics module the Sleela runtime can reference and
 * load for the cases where a VM does not have realtime canvas support natively.
 * It is deliberately split into two layers:
 *
 *   1. The PHYSICS / compute core (idea_canvas_sim.*): a large contiguous
 *      vertex block advanced by an OpenMP + SIMD decay/physics step.  This
 *      layer has NO graphics dependency -- it is pure C/C++ and is built and
 *      verified anywhere a C++20 + OpenMP compiler exists.
 *
 *   2. The RENDER layer (idea_canvas_gl.*): a GLFW window with a persistent-
 *      mapped, triple-buffered OpenGL vertex buffer, so the exact memory block
 *      the physics core writes is drawn by the GPU with no re-upload.  This
 *      layer requires GLFW + OpenGL at build time and is OPTIONAL.
 *
 * The whole module is loadable through this one C ABI (extern "C"), so a VM
 * can dlopen/LoadLibrary it and bind the entry points as native built-ins.
 * Build with IDEA_CANVAS_WITH_GL to include layer 2; without it, only the
 * verified physics core is compiled and the render entry points return
 * IDEA_CANVAS_NO_RENDERER.
 *
 * Hz contract (mirrors idea/idea_system.h):
 *   present at 60 Hz; advance the live simulation at a deterministic 120 Hz
 *   fixed timestep.
 */
#ifndef IDEA_CANVAS_H
#define IDEA_CANVAS_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IDEA_CANVAS_VERSION      "1.0.0"
#define IDEA_CANVAS_PRESENT_HZ   60.0
#define IDEA_CANVAS_SIM_HZ       120.0

/* Status codes returned across the ABI. */
typedef enum idea_canvas_status {
    IDEA_CANVAS_OK            =  0,
    IDEA_CANVAS_INVALID       = -1,  /* bad argument / null handle           */
    IDEA_CANVAS_OOM           = -2,
    IDEA_CANVAS_NO_RENDERER   = -3,  /* built without GL, or no display      */
    IDEA_CANVAS_GL_ERROR      = -4   /* GLFW / OpenGL init or runtime failure */
} idea_canvas_status;

/*
 * One canvas vertex.  Laid out as a flat POD so the whole array is a single
 * contiguous block that maps 1:1 onto an interleaved OpenGL vertex buffer
 * (position, RGBA, and the scalar fields the physics step evolves).
 *
 * Keep this struct tightly packed and stable: the GL vertex-attribute layout
 * and the VM's binding both depend on the exact field order and offsets.
 */
typedef struct idea_vertex {
    float x, y;          /* position in canvas space [0,1]x[0,1]             */
    float r, g, b, a;    /* straight-alpha colour, 0..1                      */
    float intensity;     /* current energy; decays toward 0 each step        */
    float velocity;      /* scalar flow speed (drives colour-predictive hue) */
} idea_vertex;

/*
 * Compute backends the simulation step can run on.
 *   CPU: the OpenMP + SIMD physics core (always available, verified).
 *   GPU: an offscreen GL compute path keeping data in device/native memory
 *        (EGL-surfaceless on a real GPU; OSMesa software fallback). Available
 *        only when the module was built with IDEA_CANVAS_WITH_GL and a context
 *        could be created at runtime.
 */
typedef enum idea_backend {
    IDEA_BACKEND_CPU = 0,
    IDEA_BACKEND_GPU = 1
} idea_backend;

/*
 * Backend-selection policy.  AUTO lets the VM decide: it runs on the CPU and,
 * if sustained throughput falls below the 120 Hz budget AND a GPU backend is
 * available, it offloads to the GPU.  FORCE_* pin a backend.
 */
typedef enum idea_backend_policy {
    IDEA_POLICY_AUTO      = 0,  /* measure; offload CPU->GPU under budget   */
    IDEA_POLICY_FORCE_CPU = 1,
    IDEA_POLICY_FORCE_GPU = 2
} idea_backend_policy;

/* Opaque handle to a realtime canvas (owns the vertex block + optional GL). */
typedef struct idea_canvas idea_canvas;

/* ---- lifecycle -------------------------------------------------------- */

/*
 * Create a canvas with a `cols` x `rows` grid of vertices.  Allocates the
 * contiguous vertex block (cols*rows elements).  Does NOT open a window.
 * Returns NULL on allocation failure.
 */
idea_canvas *idea_canvas_create(int cols, int rows);

/* Destroy a canvas, release the vertex block and any GL resources. */
void idea_canvas_destroy(idea_canvas *c);

/* Grid dimensions and total vertex count. */
int    idea_canvas_cols(const idea_canvas *c);
int    idea_canvas_rows(const idea_canvas *c);
size_t idea_canvas_count(const idea_canvas *c);

/*
 * Direct access to the contiguous vertex block (count = idea_canvas_count).
 * The VM may read/write this directly between steps; it is the same memory the
 * GL layer maps.  Returns NULL for a null handle.
 */
idea_vertex       *idea_canvas_vertices(idea_canvas *c);
const idea_vertex *idea_canvas_vertices_const(const idea_canvas *c);

/* ---- physics / compute core (OpenMP + SIMD; no graphics dep) ---------- */

/* Decay/flow parameters for the simulation step. */
typedef struct idea_sim_params {
    float decay;        /* per-second multiplicative decay of intensity      */
    float flow_gain;    /* how strongly velocity feeds the colour-predictive  */
    float hue_base;     /* base hue in degrees (205 ~ water)                 */
    float coupling;     /* neighbour coupling (diffusion of intensity)       */
} idea_sim_params;

/* Sensible defaults (water-like flow field). */
idea_sim_params idea_sim_defaults(void);

/*
 * Inject energy at a grid cell (e.g. where a signal "importance" lands), so a
 * ripple of intensity propagates on subsequent steps.
 */
idea_canvas_status idea_canvas_excite(idea_canvas *c, int col, int row,
                                      float amount);

/* Store the simulation parameters used by subsequent idea_canvas_step /
 * idea_canvas_advance calls (so a VM can configure the flow without passing a
 * struct across the bridge each step).  NULL fields are ignored. */
idea_canvas_status idea_canvas_set_params(idea_canvas *c, float decay,
                                          float flow_gain, float hue_base,
                                          float coupling);

/* Read one vertex's current intensity (0..1), or 0 for an out-of-range cell. */
float idea_canvas_intensity_at(const idea_canvas *c, int col, int row);

/*
 * Advance the simulation by ONE fixed timestep (dt seconds, normally
 * 1/IDEA_CANVAS_SIM_HZ).  Parallelised with OpenMP and vectorised per row;
 * updates intensity decay, neighbour coupling, velocity, and the
 * colour-predictive RGBA of every vertex.  Safe to call with any dt > 0.
 */
idea_canvas_status idea_canvas_step(idea_canvas *c, const idea_sim_params *p,
                                    float dt);

/*
 * Drain a fixed-step accumulator: feed the real per-frame delta and run as
 * many 120 Hz steps as are due (clamping a long stall).  Returns the number
 * of steps run.  `accumulator` is owned by the caller.
 */
int idea_canvas_advance(idea_canvas *c, const idea_sim_params *p,
                        double frame_dt_seconds, double *accumulator);

/* Fixed simulation timestep in seconds (1 / IDEA_CANVAS_SIM_HZ). */
double idea_canvas_timestep(void);

/* ---- backend selection + auto GPU offload ----------------------------- */

/*
 * Set the backend policy (default IDEA_POLICY_AUTO).  Under AUTO the canvas
 * starts on the CPU; idea_canvas_advance measures sustained throughput and, if
 * it falls below the 120 Hz budget and a GPU backend is available, migrates
 * the canvas to the GPU (keeping its vertex state).  FORCE_GPU with no GPU
 * available stays on CPU and reports GPU as unavailable.
 */
idea_canvas_status idea_canvas_set_policy(idea_canvas *c, idea_backend_policy p);
idea_backend_policy idea_canvas_policy(const idea_canvas *c);

/* The backend the canvas is currently simulating on. */
idea_backend idea_canvas_active_backend(const idea_canvas *c);

/* Is a GPU backend available in this build + at runtime? (1/0) */
int idea_canvas_gpu_available(void);

/*
 * The measured sustained simulation rate (steps/second), a rolling average
 * updated by idea_canvas_advance.  0 before enough samples.  This is the
 * signal the AUTO policy compares against IDEA_CANVAS_SIM_HZ.
 */
double idea_canvas_measured_hz(const idea_canvas *c);

/*
 * Force a one-shot backend migration now (CPU<->GPU), copying vertex state to
 * the target backend's memory.  Returns IDEA_CANVAS_NO_RENDERER if the GPU is
 * requested but unavailable.  Normally you set a policy and let advance decide;
 * this is for tests and explicit control.
 */
idea_canvas_status idea_canvas_migrate(idea_canvas *c, idea_backend target);

/* ---- render layer (GLFW + OpenGL; optional) --------------------------- */

/*
 * Open a window and initialise the GL renderer: compiles the shaders and
 * allocates a persistent-mapped, triple-buffered vertex buffer sized to the
 * vertex block.  Returns IDEA_CANVAS_NO_RENDERER if the module was built
 * without IDEA_CANVAS_WITH_GL (then run headless via idea_canvas_step only).
 */
idea_canvas_status idea_canvas_open_window(idea_canvas *c, int width,
                                           int height, const char *title);

/* True if the window has been asked to close. */
int idea_canvas_should_close(const idea_canvas *c);

/*
 * Present one frame: copy the current vertex block into the next persistent-
 * mapped buffer region (guarded by a fence), issue the draw call, swap, and
 * poll events.  Call at the present rate (60 Hz).  No-op/NO_RENDERER when the
 * GL layer is absent.
 */
idea_canvas_status idea_canvas_present(idea_canvas *c);

/*
 * The full driving loop: open a window, then repeatedly advance the physics at
 * 120 Hz fixed step and present at ~60 Hz until the window closes.  A VM that
 * just wants "run the canvas" calls this.  NO_RENDERER if GL is absent.
 */
idea_canvas_status idea_canvas_run(idea_canvas *c, const idea_sim_params *p,
                                   int width, int height, const char *title);

/* Was the module built with the GL render layer? (1/0) */
int idea_canvas_has_renderer(void);

/* ---- GPU compute backend hooks (implemented in idea_canvas_gpu.cpp) ----
 * These are called by the core when a canvas is on IDEA_BACKEND_GPU.  Without
 * IDEA_CANVAS_WITH_GL they are stubs that report unavailable.  Not normally
 * called directly by a VM -- use the policy/migrate API above. */
int    idea_canvas_gpu_init(idea_canvas *c);   /* create ctx + upload; 0 ok, <0 err */
int    idea_canvas_gpu_step(idea_canvas *c, const idea_sim_params *p, float dt);
int    idea_canvas_gpu_readback(idea_canvas *c);  /* GPU -> native vertex block */
void   idea_canvas_gpu_shutdown(idea_canvas *c);

/* Human-readable status name (never NULL). */
const char *idea_canvas_status_name(idea_canvas_status s);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* IDEA_CANVAS_H */
