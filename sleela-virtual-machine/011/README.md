<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Virtual Machine 11

SLVM/11 establishes a filesystem-module host above the verified storage contracts of SLVM/9 and SLVM/10. It provides a stable slot for custom filesystem definitions and structures while keeping filesystem semantics versioned and capability-driven.

`file-system/modules/<module-id>/` is the standard extension point. TAC3 is the first formulaically standardized module and is byte-identical with SLVM/9 and SLVM/10.

Custom modules do not receive authority merely by being present; they must be discovered, version-checked, identity-verified, and capability-qualified.

## Runnable VM

SLVM/11 is a **real, runnable virtual machine**, not only a specification. It
links the shared native Core engine (`impl/build/libslcore.a`) and runs compiled
`.sleela` artifacts; its generation contribution — the filesystem-module host —
is applied as a pre-boot admission step (the baseline TAC3 module must validate
before a program boots).

```sh
make -C ../../impl libslcore        # once: build the shared Core engine archive
make                                # build vm-build/slvm-11
./vm-build/slvm-11 --selfcheck      # verify the Core link + TAC3 admit/reject check
./vm-build/slvm-11 program.art      # run a compiled .sleela artifact on SLVM/11
```

From the repository CLI: `sleela vm run premium 11 program.sleela` builds this
VM on demand, compiles the program, and runs it here. `src/slvm11_vm.c` is the
VM entry; `src/slvm11_filesystem_module.c` is the generation feature.

## intXXX / uintXXX number series support

As the highest premium generation, SLVM/11 also admits SLeeLa's fixed-width
integer number series:

- **Signed** `int32 … int16777216` (syntax 2.7)
- **Unsigned** `uint32 … uint16777216` (syntax 2.8)

A member is the prefix (`int`/`uint`) followed by a bit-width that must be a
**power of two in `[2^5, 2^24]`** — twenty widths per family. `src/slvm11_intxxx_module.c`
is the number-series feature: it recognizes the series, enforces the width rule
and the twenty-width count, and classifies every width to its runtime slot using
the exact split the compiler uses:

| Width | Runtime slot | Backing |
|-------|--------------|---------|
| `≤ 64` (`int32/int64`, `uint32/uint64`) | `SL_INT` | `int64_t` |
| `≥ 128` (`int128…`, `uint128…`) | `SL_INT128` | `__int128` (true 128-bit) |

Unsigned-ness is a compile-time discipline (positive-only, width-strict), so at
runtime a `uint<width>` shares the `SL_INT` / `SL_INT128` tag with the signed
series. The VM's admission **proves** the Core value model carries both slots
end to end: a 64-bit value survives `slval_int` (`SL_INT`), and a value wider
than 64 bits survives `slval_int128` (`SL_INT128`) with its high bits intact. A
malformed series descriptor (wrong ceiling / bogus width count) is rejected, so
the support is demonstrable rather than asserted. `--selfcheck` exercises all of
this; `tests/slvm11_intxxx_module_selftest.c` covers the recognizer standalone.

> Canonical spec: `SLEELA.syntax` §24 (signed) and §25 (unsigned); recognizer of
> record: `impl/frontend/wide_integer.h`.

Copyright (c) Max Rupplin - MEARVK LLC - 2026