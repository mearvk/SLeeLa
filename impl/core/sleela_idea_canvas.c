/*
 * impl/core/sleela_idea_canvas.c
 * SLeeLa VM <-> Idea System(TM) realtime canvas bridge.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * Default build (no SLEELA_WITH_IDEA_CANVAS): a dependency-free stub, so the
 * ideaCanvas* opcodes exist and fail cleanly without pulling OpenMP/GLFW into
 * the standard VM build.
 *
 * With SLEELA_WITH_IDEA_CANVAS: forwards to the module's C ABI. Build the VM
 * with that define and link idea/realtime (libideacanvas.a) + OpenMP, e.g.:
 *     make -C idea/realtime           # build libideacanvas.a
 *     CXXFLAGS += -DSLEELA_WITH_IDEA_CANVAS -I../idea/realtime
 *     LDFLAGS  += idea/realtime/build/libideacanvas.a -fopenmp
 */
#include "sleela_idea_canvas.h"

#ifdef SLEELA_WITH_IDEA_CANVAS
#include "idea_canvas.h"   /* idea/realtime/idea_canvas.h on the include path */

int   sl_idea_canvas_available(void) { return 1; }

void *sl_idea_canvas_create(int cols, int rows) {
    return (void*)idea_canvas_create(cols, rows);
}
void  sl_idea_canvas_destroy(void *c) { idea_canvas_destroy((idea_canvas*)c); }

int sl_idea_canvas_cols(void *c)  { return idea_canvas_cols((idea_canvas*)c); }
int sl_idea_canvas_rows(void *c)  { return idea_canvas_rows((idea_canvas*)c); }
int sl_idea_canvas_count(void *c) { return (int)idea_canvas_count((idea_canvas*)c); }

int sl_idea_canvas_set_params(void *c, double decay, double flow_gain,
                              double hue_base, double coupling) {
    return idea_canvas_set_params((idea_canvas*)c, (float)decay, (float)flow_gain,
                                  (float)hue_base, (float)coupling);
}
int sl_idea_canvas_excite(void *c, int col, int row, double amount) {
    return idea_canvas_excite((idea_canvas*)c, col, row, (float)amount);
}
int sl_idea_canvas_step(void *c, double dt) {
    return idea_canvas_step((idea_canvas*)c, NULL, (float)dt);
}
int sl_idea_canvas_advance(void *c, double frame_dt_seconds) {
    static double acc = 0.0;  /* a single-canvas convenience accumulator */
    return idea_canvas_advance((idea_canvas*)c, NULL, frame_dt_seconds, &acc);
}
double sl_idea_canvas_timestep(void) { return idea_canvas_timestep(); }
double sl_idea_canvas_intensity_at(void *c, int col, int row) {
    return idea_canvas_intensity_at((idea_canvas*)c, col, row);
}

int sl_idea_canvas_has_renderer(void) { return idea_canvas_has_renderer(); }
int sl_idea_canvas_open_window(void *c, int w, int h, const char *title) {
    return idea_canvas_open_window((idea_canvas*)c, w, h, title);
}
int sl_idea_canvas_should_close(void *c) {
    return idea_canvas_should_close((idea_canvas*)c);
}
int sl_idea_canvas_present(void *c) { return idea_canvas_present((idea_canvas*)c); }
int sl_idea_canvas_run(void *c, int w, int h, const char *title) {
    return idea_canvas_run((idea_canvas*)c, NULL, w, h, title);
}

#else /* ---- dependency-free stub (default build) ---- */

int   sl_idea_canvas_available(void)              { return 0; }
void *sl_idea_canvas_create(int cols, int rows)   { (void)cols;(void)rows; return 0; }
void  sl_idea_canvas_destroy(void *c)             { (void)c; }
int sl_idea_canvas_cols(void *c)                  { (void)c; return 0; }
int sl_idea_canvas_rows(void *c)                  { (void)c; return 0; }
int sl_idea_canvas_count(void *c)                 { (void)c; return 0; }
int sl_idea_canvas_set_params(void *c, double d, double f, double h, double k) {
    (void)c;(void)d;(void)f;(void)h;(void)k; return -3; /* NO_RENDERER-ish */
}
int sl_idea_canvas_excite(void *c, int col, int row, double a) {
    (void)c;(void)col;(void)row;(void)a; return -3;
}
int sl_idea_canvas_step(void *c, double dt)       { (void)c;(void)dt; return -3; }
int sl_idea_canvas_advance(void *c, double dt)    { (void)c;(void)dt; return 0; }
double sl_idea_canvas_timestep(void)              { return 1.0 / 120.0; }
double sl_idea_canvas_intensity_at(void *c, int col, int row) {
    (void)c;(void)col;(void)row; return 0.0;
}
int sl_idea_canvas_has_renderer(void)             { return 0; }
int sl_idea_canvas_open_window(void *c, int w, int h, const char *t) {
    (void)c;(void)w;(void)h;(void)t; return -3;
}
int sl_idea_canvas_should_close(void *c)          { (void)c; return 1; }
int sl_idea_canvas_present(void *c)               { (void)c; return -3; }
int sl_idea_canvas_run(void *c, int w, int h, const char *t) {
    (void)c;(void)w;(void)h;(void)t; return -3;
}

#endif
