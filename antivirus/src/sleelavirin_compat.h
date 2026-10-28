/*
 * Sleelavirin(TM) portability shim.
 * Max Rupplin - MEARVK LLC - 2026.
 *
 * Smooths over the few places where the host C library differs across the
 * supported targets so the rest of the engine can stay plain ISO C11 / C++17:
 *
 *   - Windows XP+ / 7+ / 10+ built with MSVC (incl. very old toolchains),
 *   - Windows built with MinGW-w64 (GCC/Clang),
 *   - macOS (Clang / Apple toolchain),
 *   - Linux and other POSIX (GCC / Clang).
 *
 * Nothing here is OS-specific antivirus logic; it is only library glue. Include
 * this FIRST in .c/.cpp translation units that do formatted I/O or need file
 * sizes, before the standard headers it wraps.
 */
#ifndef SLEELAVIRIN_COMPAT_H
#define SLEELAVIRIN_COMPAT_H

/* -------------------------------------------------------------------------
 * POSIX large-file API opt-in.
 *
 * fseeko/ftello and off_t are POSIX, not ISO C, so a strict -std=c11 build does
 * not expose them unless we request a POSIX profile and 64-bit file offsets
 * BEFORE including <stdio.h>. These macros are harmless on Windows/MSVC (ignored)
 * and on toolchains that already define them (guarded).
 * ------------------------------------------------------------------------- */
#if !defined(_WIN32)
#  ifndef _FILE_OFFSET_BITS
#    define _FILE_OFFSET_BITS 64
#  endif
#  ifndef _LARGEFILE_SOURCE
#    define _LARGEFILE_SOURCE 1
#  endif
#  if !defined(_POSIX_C_SOURCE) && !defined(_DARWIN_C_SOURCE) && !defined(__APPLE__)
#    define _POSIX_C_SOURCE 200809L
#  endif
#endif

/* -------------------------------------------------------------------------
 * Windows version floor.
 *
 * We do not raise the floor if the build already set one (e.g. a toolchain
 * targeting Windows XP sets _WIN32_WINNT=0x0501). When nothing is set we pick a
 * conservative Windows 7 (0x0601) baseline, which still runs on 7/8/10/11. An
 * XP build simply defines _WIN32_WINNT=0x0501 on the command line (see
 * PLATFORMS.md); this engine uses only CRT calls available since XP, so it
 * compiles against either floor.
 * ------------------------------------------------------------------------- */
#if defined(_WIN32)
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN 1
#  endif
#  ifndef _WIN32_WINNT
#    define _WIN32_WINNT 0x0601 /* Windows 7+ default; override to 0x0501 for XP */
#  endif
#  ifndef WINVER
#    define WINVER _WIN32_WINNT
#  endif
#  ifndef _CRT_SECURE_NO_WARNINGS
#    define _CRT_SECURE_NO_WARNINGS 1 /* we use the standard CRT deliberately */
#  endif
#endif

#include <stdio.h>
#include <stdint.h>
#if !defined(_WIN32)
#  include <sys/types.h> /* off_t */
#endif

/* Portable "this static helper may be unused in some TUs" marker. */
#if defined(__GNUC__) || defined(__clang__)
#  define SLV_MAYBE_UNUSED __attribute__((unused))
#else
#  define SLV_MAYBE_UNUSED
#endif

/* -------------------------------------------------------------------------
 * snprintf.
 *
 * MSVC provides a conformant snprintf only from Visual Studio 2015 (_MSC_VER
 * >= 1900). Older MSVC (the kind used for XP-era builds) has _snprintf, which
 * does NOT always NUL-terminate. We provide a correct, always-terminating
 * slv_snprintf for those compilers and map snprintf onto it.
 *
 * Everywhere else (C99/C11, MinGW, Clang, GCC, MSVC 2015+) snprintf is used
 * as-is. The C++ wrapper uses std::snprintf, which is fine on every supported
 * C++17 toolchain, so this block is C-only.
 * ------------------------------------------------------------------------- */
#if defined(_MSC_VER) && (_MSC_VER < 1900) && !defined(__cplusplus)
#  include <stdarg.h>
static int slv_snprintf(char *buf, size_t size, const char *fmt, ...)
{
    int n;
    va_list ap;
    if (buf == 0 || size == 0) return -1;
    va_start(ap, fmt);
    /* _vsnprintf_s truncates and always NUL-terminates. */
    n = _vsnprintf_s(buf, size, _TRUNCATE, fmt, ap);
    va_end(ap);
    if (n < 0) {
        buf[size - 1] = '\0'; /* guarantee termination on truncation */
        return (int)(size - 1);
    }
    return n;
}
#  ifdef snprintf
#    undef snprintf
#  endif
#  define snprintf slv_snprintf
#endif

/* -------------------------------------------------------------------------
 * 64-bit file size.
 *
 * ftell/fseek use 'long', which is 32-bit on Windows (and on 32-bit XP builds)
 * and so cannot describe files larger than 2 GiB. slv_file_size() uses the
 * widest seek/tell the platform offers and returns the size in a 64-bit value.
 * Returns 0 on success (size in *out), -1 on error. The stream is left at its
 * previous position is not guaranteed; callers rewind if they continue reading.
 * ------------------------------------------------------------------------- */
static SLV_MAYBE_UNUSED int slv_file_size(FILE *f, uint64_t *out)
{
    if (f == 0 || out == 0) return -1;
#if defined(_MSC_VER)
    if (_fseeki64(f, 0, SEEK_END) != 0) return -1;
    {
        __int64 end = _ftelli64(f);
        if (end < 0) return -1;
        *out = (uint64_t)end;
    }
    (void)_fseeki64(f, 0, SEEK_SET);
    return 0;
#elif defined(__MINGW32__) || defined(__MINGW64__)
    if (fseeko64(f, 0, SEEK_END) != 0) return -1;
    {
        off64_t end = ftello64(f);
        if (end < 0) return -1;
        *out = (uint64_t)end;
    }
    (void)fseeko64(f, 0, SEEK_SET);
    return 0;
#else
    /* POSIX (Linux, macOS, other Unix): _FILE_OFFSET_BITS=64 above makes off_t
     * 64-bit where supported. */
    if (fseeko(f, 0, SEEK_END) != 0) return -1;
    {
        off_t end = ftello(f);
        if (end < 0) return -1;
        *out = (uint64_t)end;
    }
    (void)fseeko(f, 0, SEEK_SET);
    return 0;
#endif
}

#endif /* SLEELAVIRIN_COMPAT_H */
