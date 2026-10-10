<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# `extends` examples (syntax 1.10)

Runnable programs for the two meanings of `extends`. See
[`../../markdown/EXTENDS.md`](../../markdown/EXTENDS.md) for the normative spec.

| File | Shows |
|---|---|
| [`inheritance.sleela`](inheritance.sleela) | the OOD meaning — a class extends a base class; inherited fields and methods, overrides, and a multi-level chain (`Dog` → `Pet` → `Animal`) |
| [`document_grouping.sleela`](document_grouping.sleela) | the document meaning — `extends to <targets> <grouper>;` with all three groupers: `linear` (linear reals), `group`, and `services` (a group behind a Server of Services) |

## Run

```sh
make -C impl                              # build the compiler once
SLEELA_BIN=impl/build/sleela bin/SLeeLa run examples/extends/inheritance.sleela
SLEELA_BIN=impl/build/sleela bin/SLeeLa run examples/extends/document_grouping.sleela
```

Expected output:

```
# inheritance.sleela
pet Rex of Sam
Rex fetches for Sam
animal Generic

# document_grouping.sleela
document extends to: 1 linear chain, 1 group, 1 services group
```

Both require `#sleela 1.10`. Under `#sleela 1.9` or lower, class `extends` is
retained metadata only (historical behavior) and `extends to ...` is rejected.