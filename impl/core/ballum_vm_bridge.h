#ifndef SLEELA_BALLUM_VM_BRIDGE_H
#define SLEELA_BALLUM_VM_BRIDGE_H
#ifdef __cplusplus
extern "C" {
#endif
/*
 * VM-safe facade over the native Ballum display-control core (ballum/).
 *
 * The uiBallum* SLVM built-ins lower to OP_UI_CALL and route through
 * sleela_ui_vm.c into these flat entry points, which drive the single process
 * ballum_manager_t over the compiled-in per-OS backend. Returned strings are
 * borrowed until the next call to the same accessor. Every geometry/control
 * request is validated by the native manager (bounds, capability, range) before
 * it touches the device; a backend that cannot perform an operation yields a
 * clean status code, never a false success.
 *
 * All ids are the backend-stable display ids reported by enumerate.
 */
int         sleela_ballum_vm_supported(void);                 /* 1 real backend, 0 fallback          */
const char *sleela_ballum_vm_backend_name(void);              /* compiled-in backend name            */
int         sleela_ballum_vm_enumerate(void);                 /* refresh display list -> status      */
int         sleela_ballum_vm_display_count(void);             /* cached display count                */
int         sleela_ballum_vm_display_id(int index);           /* id of cached display #index, or -1  */
const char *sleela_ballum_vm_display_name(int index);         /* name of cached display #index       */
int         sleela_ballum_vm_display_width(int index);        /* width px, or 0                      */
int         sleela_ballum_vm_display_height(int index);       /* height px, or 0                     */
int         sleela_ballum_vm_display_is_primary(int index);   /* 1 primary, 0 not, -1 bad index      */
int         sleela_ballum_vm_display_caps(int index);         /* capability bitmask, or 0            */
int         sleela_ballum_vm_set_pixel(int display_id, int x, int y,
                                       int r, int g, int b, int a); /* -> status                     */
int         sleela_ballum_vm_get_pixel(int display_id, int x, int y); /* packed 0xAARRGGBB, or -1    */
int         sleela_ballum_vm_set_brightness(int display_id, int percent);  /* -> status              */
int         sleela_ballum_vm_set_warmth(int display_id, int kelvin);       /* -> status              */
int         sleela_ballum_vm_set_gamma(int display_id, int gamma_scaled);  /* -> status              */
const char *sleela_ballum_vm_status_name(int status);         /* human-readable status text          */
/* Fullscreen surface: total, owned, fast per-pixel control. claim_surface()
 * creates an override-redirect fullscreen framebuffer the program owns; while
 * claimed, set_pixel/get_pixel/logo_blend write to the framebuffer (no X
 * round-trips) and present() pushes the whole frame in one XPutImage (MIT-SHM
 * when available). A backend without the capability yields a clean status and
 * has_surface() reports 0 -- fail closed, never a false success. */
int         sleela_ballum_vm_has_surface(int display_id);     /* 1 claimable, 0 not                  */
int         sleela_ballum_vm_claim_surface(int display_id);   /* -> status                           */
int         sleela_ballum_vm_present(int display_id);         /* -> status (push framebuffer)        */
int         sleela_ballum_vm_release_surface(int display_id); /* -> status                           */
/* Pointer input (mouse/touch). poll() dequeues one event into a VM-local latch
 * and returns its type (0 none, 1 down, 2 move, 3 up); the accessors read the
 * latched event. has_input() reports whether the backend supplies any input. */
int         sleela_ballum_vm_has_input(void);                 /* 1 input available, 0 not            */
int         sleela_ballum_vm_poll(void);                      /* latch one event -> type (0..3)      */
int         sleela_ballum_vm_event_x(void);                   /* latched event pixel x               */
int         sleela_ballum_vm_event_y(void);                   /* latched event pixel y               */
int         sleela_ballum_vm_event_button(void);             /* 0 none,1 left,2 middle,3 right       */
int         sleela_ballum_vm_event_source(void);             /* 0 mouse, 1 touch                     */
int         sleela_ballum_vm_event_contact(void);            /* touch contact id (0 for mouse)       */
int         sleela_ballum_vm_event_modifiers(void);          /* BALLUM_MOD_* bitmask                 */
const char *sleela_ballum_vm_event_type_name(int type);      /* "none"/"down"/"move"/"up"            */
/* Keyboard input. poll_key() dequeues one key and returns its ballum_key_t code
 * (0 none; 27 ESC; 13 ENTER; 32 SPACE; printable ASCII as itself). has_keyboard()
 * reports whether the backend supplies keys; a backend without a keyboard queue
 * returns 0 keys -- fail closed, never a false key. */
int         sleela_ballum_vm_has_keyboard(void);             /* 1 keyboard available, 0 not          */
int         sleela_ballum_vm_poll_key(void);                 /* dequeue one key -> code (0 none)     */
/* Logo splash compositing hook, used by a full-screen SLBallumSession. The host
 * (where an image decoder is available) decodes the session's logo and blends
 * it over the background at a given opacity; without a decoder these are honest
 * no-ops -- logo_ready() reports 0 and logo_blend() does nothing and returns a
 * clean status, so the session's white fill + fade timeline + ESC still run and
 * never report a false success. */
int         sleela_ballum_vm_logo_ready(void);               /* 1 logo decoded + loaded, 0 not */
int         sleela_ballum_vm_logo_load(const char *path);    /* decode a JPEG logo from a file -> status */
int         sleela_ballum_vm_logo_set_effect(int fx);        /* choose a transition effect (ballum_fx_t) -> status */
int         sleela_ballum_vm_logo_blend(int display_id, int opacity_percent,
                                        int screen_w, int screen_h,
                                        int bg_r, int bg_g, int bg_b); /* composite one frame -> status */
int         sleela_ballum_vm_fx_count(void);                 /* number of transition effects */
const char *sleela_ballum_vm_fx_name(int fx);                /* effect name, e.g. "iris-in" */
int         sleela_ballum_vm_fx_by_name(const char *name);   /* effect id for a name (fade if unknown) */
#ifdef __cplusplus
}
#endif
#endif
