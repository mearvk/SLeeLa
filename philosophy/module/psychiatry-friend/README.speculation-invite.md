# Speculation Invite — adult-psychiatry commentary on programming reach

A small C + C++ file set that **invites careful, attributed speculation from
adult-psychiatry professionals about what this software's programming reaches**
— its capabilities and, especially, its limits — and renders the collected
notes as a careful scientific report.

This is commentary **about the software**, collected as hypotheses. It is not a
clinical assessment of any person and sits inside the boundaries set by this
folder's [`README.md`](README.md).

## What it does

- Shows a standing **invitation** that asks professionals to frame each note as
  a hypothesis, attribute it to themselves, state its scope, and say how it
  could be shown wrong.
- Collects **speculations**, each with: author, self-reported role, scope, an
  aspect (capability / limitation / safety / ethics / research direction), a
  bounded **confidence** band (tentative / plausible / well-reasoned), the
  submitter's own text, and a **required falsifiability note**.
- Emits a **report** — the invitation, every speculation with its fields and
  attribution, and a standing **disclaimer**.

## Careful by construction

- A speculation with **no way to be shown wrong is rejected** — falsifiability
  is required, so the collected set reads as testable hypotheses, not findings.
- Confidence is a bounded band; there is no "certain" level.
- `about_software_not_person` is forced true: the API cannot record a
  speculation framed as being about a person.

## What it will not do (same boundaries as the folder README)

- No diagnosis of any condition, and none accepted as input.
- No treatment, medication, or hormone advice.
- **No credential verification** — roles and authorship are **self-reported**
  and labelled "self-attested, unverified." It cannot confirm a licence or
  clinical correctness.
- It never presents generated text as a professional's opinion; it stores the
  submitter's own words with attribution.
- No network I/O, no background listening. Not an emergency service.

## Files

| File | Role |
|---|---|
| `speculation_invite.h` / `.c` | C API and core (registry, validation, report) |
| `speculation_invite.hpp` / `.cpp` | inheritable C++17 wrapper (`accept()` policy hook; `BoundaryFocusedInvite` keeps only limitation/safety notes) |
| `speculation_example.cpp` | interactive demo (`--interactive`) + seeded careful report |
| `test_speculation_invite.c` | tests (validation, submit/dup/capacity, report) |

## Build, test, run

    make
    make test
    ./build/speculation-example              # seeded report
    ./build/speculation-example --interactive  # type your own speculations
