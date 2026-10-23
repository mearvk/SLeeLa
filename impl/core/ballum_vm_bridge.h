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
#ifdef __cplusplus
}
#endif
#endif
