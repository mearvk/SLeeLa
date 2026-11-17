/*
 * regression.h -- minimal, dependency-free harness for the SLeeLa regression
 * set (C and C++ suites).
 *
 * What it adds on top of plain asserts:
 *   - non-fatal checks, so one run reports every broken contract;
 *   - an instrumented allocator (reg_support.c, linked with
 *     -Wl,--wrap=malloc,calloc,realloc,free) that
 *       * counts live allocations      -> every case is leak-checked,
 *       * guards both ends of a block  -> heap over/underflow is detected,
 *       * can fail the Nth allocation  -> out-of-memory paths are executed;
 *   - a deterministic PRNG, so "random" cases replay identically.
 *
 * Single-threaded by design.
 */
#ifndef SLEELA_REGRESSION_H
#define SLEELA_REGRESSION_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---- instrumented allocator ------------------------------------------- */
size_t reg_alloc_live(void);               /* live tracked blocks            */
unsigned long reg_alloc_calls(void);       /* allocation attempts so far     */
unsigned long reg_alloc_corruptions(void); /* guard-byte violations detected */
/* The n-th allocation attempt from now (1 = the very next one) fails with
 * NULL/ENOMEM, once. n == 0 disarms. Returns nothing; query injected(). */
void reg_alloc_fail_nth(unsigned long n);
int reg_alloc_injected(void);              /* 1 if the armed failure fired   */

/* ---- harness ------------------------------------------------------------ */
void reg_note_check(void);
void reg_fail(const char *file, int line, const char *fmt, ...)
#if defined(__GNUC__)
    __attribute__((format(printf, 3, 4)))
#endif
    ;
/* A KNOWN ISSUE is a contract the code is documented/expected to honour but
 * currently does not. It is reported (and counted) without failing the suite,
 * so the defect stays visible. When the code is fixed the line says so and the
 * assertion should be promoted to REG_CHECK. */
void reg_known(int contract_holds, const char *id, const char *description);
void reg_run_case(const char *name, void (*fn)(void));
int reg_finish(const char *suite);

/* ---- deterministic PRNG (xorshift64*) ----------------------------------- */
void reg_seed(uint64_t seed);
uint64_t reg_next(void);
uint32_t reg_below(uint32_t bound_exclusive); /* bound > 0 */

#ifdef __cplusplus
}
#endif

#define REG_CHECK(cond)                                                       \
    do {                                                                      \
        reg_note_check();                                                     \
        if (!(cond)) reg_fail(__FILE__, __LINE__, "check failed: %s", #cond); \
    } while (0)

/* Like REG_CHECK but returns from the (void) case function on failure. */
#define REG_REQUIRE(cond)                                                     \
    do {                                                                      \
        reg_note_check();                                                     \
        if (!(cond)) {                                                        \
            reg_fail(__FILE__, __LINE__, "required: %s", #cond);              \
            return;                                                           \
        }                                                                     \
    } while (0)

#define REG_EQ_I(expected, actual)                                            \
    do {                                                                      \
        long long reg_e_ = (long long)(expected), reg_a_ = (long long)(actual); \
        reg_note_check();                                                     \
        if (reg_e_ != reg_a_)                                                 \
            reg_fail(__FILE__, __LINE__, "%s: expected %lld, got %lld",       \
                     #actual, reg_e_, reg_a_);                                \
    } while (0)

#define REG_EQ_U(expected, actual)                                            \
    do {                                                                      \
        unsigned long long reg_e_ = (unsigned long long)(expected),           \
                           reg_a_ = (unsigned long long)(actual);             \
        reg_note_check();                                                     \
        if (reg_e_ != reg_a_)                                                 \
            reg_fail(__FILE__, __LINE__, "%s: expected %llu, got %llu",       \
                     #actual, reg_e_, reg_a_);                                \
    } while (0)

#define REG_EQ_S(expected, actual)                                            \
    do {                                                                      \
        const char *reg_e_ = (expected), *reg_a_ = (actual);                  \
        reg_note_check();                                                     \
        if (!reg_e_ || !reg_a_ || strcmp(reg_e_, reg_a_) != 0)                \
            reg_fail(__FILE__, __LINE__, "%s: expected \"%s\", got \"%s\"",   \
                     #actual, reg_e_ ? reg_e_ : "(null)",                     \
                     reg_a_ ? reg_a_ : "(null)");                             \
    } while (0)

#define REG_EQ_MEM(expected, actual, n)                                       \
    do {                                                                      \
        reg_note_check();                                                     \
        if (memcmp((expected), (actual), (n)) != 0)                           \
            reg_fail(__FILE__, __LINE__, "%s: %zu-byte buffers differ",       \
                     #actual, (size_t)(n));                                   \
    } while (0)

#define REG_KNOWN(cond, id, description) reg_known((cond) != 0, (id), (description))

#define REG_RUN(fn) reg_run_case(#fn, fn)

#endif /* SLEELA_REGRESSION_H */
