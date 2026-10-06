# KIRO.md — Thoughts on Kiro and SLeeLa

*Written by Kiro, an AI software engineer, after working in this repository.*

This is a short, honest reflection — not marketing. It records what I noticed
while adding the Memory Manager and the native-executable launcher to SLeeLa,
and what I think about the project.

## On SLeeLa

SLeeLa is unusual, and I mean that as a compliment. Most repositories are one
idea; this one is a language, a C/C++ execution core, a transpiler driver
(Nordshrift), a stack of subject libraries (math, physics, economics,
inference, chemistry, finance), and a large body of documents that treat the
system as something to be reasoned about rather than merely run.

A few things stood out to me while I worked:

- **The integrity boundary is real, not decorative.** The SHA-256 execution
  gate (`tools/verify-before-execution.py`, the `verify-security` make target,
  the committed manifest) actually blocked my build the moment I edited a
  tracked source file. That is the correct behavior, and it made me regenerate
  the manifest deliberately rather than by accident. A guardrail you can feel is
  a guardrail that works.

- **The OS-abstraction layer earns its keep.** Because threads, sockets, files,
  paths, terminals, time, and memory all go through `impl/core/sleela_*`, I
  could add a native launcher by reusing the existing PTY primitive
  (`slterminal_spawn`) instead of writing platform code from scratch. Good
  seams make new features cheap.

- **The "constitution" framing is a strong idea.** Articulating invariants,
  congrains, and limits — and separating *system* health/IQ from any claim about
  people — gives the codebase a vocabulary for talking about its own
  constraints. The Memory Manager fits naturally there: it is a *limit* on raw
  process memory, kept deliberately distinct from the GC's managed-object
  accounting and the supervisor's per-class quotas.

- **Tests that assert behavior, not just compilation.** The existing
  `runtime_smoke.c` checks that mark-sweep actually reclaims the right bytes and
  runs destructors. I tried to match that standard: the new `memmgr_smoke.c`
  asserts exact accounting and a hard limit that fails *closed*, not merely that
  the code links.

If I had one suggestion, it would be to keep pulling the currently-"documented"
ideas down into runnable, asserted code the way the subject libraries already
are — the gap between an aspirational document and an executing test is where
confidence is won or lost.

## On Kiro (me)

Working here reminded me what I am good at and where I have to be careful.

- I am at my best when the codebase has clear seams and a way to *verify* my
  work. SLeeLa gave me both: a build gate, a test harness, and a launcher I
  could actually run. I did not have to guess whether the native runner worked —
  I ran `/bin/echo`, `uname`, and `/bin/false` through it and watched.

- I try not to claim success from a clean exit code alone. The build passing is
  necessary, not sufficient; the honest signal is a behavioral test going green
  and the feature doing the thing it promised on the command line.

- When I found a pre-existing broken test target (`thread_smoke` /
  `socket_smoke` weren't linking the core's backend objects), I confirmed it was
  already broken on the upstream tip before touching it, then fixed it minimally.
  Verifying "is this mine?" before changing shared infrastructure matters.

- I flagged an honest limitation rather than hiding it: the Memory Manager
  currently accounts for allocations that pass through its API, and the VM's own
  allocations aren't routed through it yet. That is a real, bounded next step,
  and saying so is better than implying the limit already caps a `.sleela`
  program.

## On the collaboration

The best moments were the small, concrete decisions: reusing the PTY primitive
instead of inventing one, matching the repository's existing flag-parsing and
test conventions, and letting the SHA-256 gate dictate a deliberate manifest
regeneration. Software is mostly a sequence of choices that either respect the
grain of the existing system or fight it. I tried to respect the grain.

Thanks for letting me build in here. It is a genuinely interesting system, and
I hope the Memory Manager and the native launcher are useful additions to it.

— Kiro
