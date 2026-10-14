/*
 * impl/core/sleela_idea_canvas.h
 * SLeeLa VM <-> Idea System(TM) realtime canvas bridge (idea/realtime).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * A thin C bridge the VM core (sleela_core.c) calls to drive the native-CPU
 * rendering + physics canvas.  It forwards to the module's public C ABI
 * (idea/realtime/idea_canvas.h) when the VM is built with
 * SLEELA_WITH_IDEA_CANVAS, and otherwise provides a no-op stub so the opcodes
 * still exist and report "no native canvas" (handle -1 / NO_RENDERER).
 *
 * All functions are safe to call unconditionally from the C core.
 */
#ifndef SLEELA_IDEA_CANVAS_H
#define SLEELA_IDEA_CANVAS_H

#ifdef __cplusplus
extern "C" {
#endif

/* 1 if this VM was built with the native canvas linked in, else 0. */
int sl_idea_canvas_available(void);

/* Lifecycle. create returns an opaque module handle as a pointer-sized int in
 * the host (we expose it to the VM as an int slot index via the VM table, so
 * these take/return the module's own idea_canvas* behind a void*). */
void *sl_idea_canvas_create(int cols, int rows);
void  sl_idea_canvas_destroy(void *c);

int sl_idea_canvas_cols(void *c);
int sl_idea_canvas_rows(void *c);
int sl_idea_canvas_count(void *c);

/* Physics (native CPU). */
int    sl_idea_canvas_set_params(void *c, double decay, double flow_gain,
                                 double hue_base, double coupling);
int    sl_idea_canvas_excite(void *c, int col, int row, double amount);
int    sl_idea_canvas_step(void *c, double dt);
int    sl_idea_canvas_advance(void *c, double frame_dt_seconds);
double sl_idea_canvas_timestep(void);
double sl_idea_canvas_intensity_at(void *c, int col, int row);

/* Backend selection + auto GPU offload. */
int    sl_idea_canvas_set_policy(void *c, int policy);     /* 0 auto,1 cpu,2 gpu */
int    sl_idea_canvas_policy(void *c);
int    sl_idea_canvas_active_backend(void *c);             /* 0 cpu, 1 gpu       */
int    sl_idea_canvas_gpu_available(void);
double sl_idea_canvas_measured_hz(void *c);
int    sl_idea_canvas_migrate(void *c, int target);        /* 0 cpu, 1 gpu       */

/* Render (optional GL). */
int sl_idea_canvas_has_renderer(void);
int sl_idea_canvas_open_window(void *c, int w, int h, const char *title);
int sl_idea_canvas_should_close(void *c);
int sl_idea_canvas_present(void *c);
int sl_idea_canvas_run(void *c, int w, int h, const char *title);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_IDEA_CANVAS_H */
