/* ===========================================================================
 * sleela_ui_vm.h -- VM-facing interface of the ui* -> SleelaUI bridge.
 *
 * The VM core (sleela_core.c) pops the arguments for an OP_UI_CALL into a small
 * SLUIArg[] vector, calls sleela_ui_dispatch(), and PUSHes the returned SLUIArg.
 * sleela_ui_arity() tells the core how many values to pop for a given function
 * id. Kept toolkit-type-free (no slui_* types here) so the core includes only
 * this small header, not the whole SleelaUI ABI.
 * =========================================================================== */
#ifndef SLEELA_UI_VM_H
#define SLEELA_UI_VM_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { SLUI_ARG_INT = 0, SLUI_ARG_DOUBLE = 1, SLUI_ARG_STR = 2 } SLUIArgKind;

typedef struct {
    SLUIArgKind kind;
    long        i;   /* when kind == SLUI_ARG_INT (also carries handles)      */
    double      d;   /* when kind == SLUI_ARG_DOUBLE                          */
    const char* s;   /* when kind == SLUI_ARG_STR (borrowed; copied if kept)  */
} SLUIArg;

/* Number of SLeeLa arguments the given ui* function id consumes. */
int sleela_ui_arity(int id);

/* Call the ui* function `id` with the `n` popped arguments in `a` (left-to-
 * right); returns the result the core will PUSH. */
SLUIArg sleela_ui_dispatch(int id, const SLUIArg* a, int n);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_UI_VM_H */
