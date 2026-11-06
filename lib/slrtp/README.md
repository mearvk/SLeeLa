# SLRTP — Sleela Retrouval Tactical Processor

**Aspect-Oriented Programming for SLeeLa.** SLRTP brings the vocabulary and
mechanics of Java/AspectJ-style AOP — **aspects, pointcuts, join points, and
before / after-returning / after-throwing / around advice** — to SLeeLa as a
pure `#sleela 3.0` standard-library package.

## How it differs from Java AOP

Java weaves advice into bytecode (AspectJ) or behind a dynamic proxy (Spring
AOP). SLeeLa's VM has no bytecode weaver and its semantic checker is nominal
(no upcasting), so SLRTP uses **explicit, data-driven interception**:

- A target method **weaves the processor around its own body** — it calls
  `runBefore` / `runAround` / `runAfterReturning` / `runAfterThrowing` on an
  `SLRTPProcessor`, passing a described join point. This is exactly the seam a
  Spring proxy inserts, made explicit.
- Advice is **declarative**: an `SLRTPAdvice` carries a pointcut plus a built-in
  action code (log, count, around-skip, around-substitute) instead of a
  subclassed method body, because the checker will not dispatch advice through a
  base type. The built-in actions cover the common cross-cutting concerns.

Everything is **string-first** (join points render their type, method, args, and
result as strings) so interception needs no reflection the VM does not provide,
and a trace is printable and testable without stdout.

## Vocabulary

| SLRTP type | Role | Java analogue |
|---|---|---|
| `SLRTPJoinPoint` | one described execution point (type, method, phase, args, result) | join point / `ProceedingJoinPoint` |
| `SLRTPPointcut` | selects join points by type / method / phase (exact or `*` wildcard) | pointcut expression |
| `SLRTPAdvice` | a pointcut + an action (log / count / skip / substitute) | `@Before` / `@AfterReturning` / `@AfterThrowing` / `@Around` |
| `SLRTPAspect` | a named bundle of advice installed as a unit | `@Aspect` |
| `SLRTPProcessor` | registers advice and fires matching advice per join point | the AOP proxy / weaver |

## Phases

A join point is visited once per phase around the target call:

- **before** — ahead of the call.
- **around** — wraps the call; an advice may decline to proceed (`ACTION_SKIP`)
  or install a fixed result (`ACTION_SUBSTITUTE`). The join point's
  `didProceed()` tells the caller whether to run the real body.
- **afterReturning** — after a successful call, carrying the result.
- **afterThrowing** — after the call raised, carrying an error marker.

## Pointcut matching

A pointcut matches a join point on three independent fields — target type,
method name, and phase — each either an exact string or the universal wildcard
`"*"`. Matching uses only string equality (the primitive `String` type here has
no substring/regex), which keeps selection exact and portable. The default
pointcut (all `"*"`) is a catch-all.

## Minimal use

```sleela
SLRTPProcessor p = new SLRTPProcessor();

SLRTPPointcut pc = new SLRTPPointcut();
pc.select("pay.Service", "charge", "around");   // type, method, phase

SLRTPAdvice guard = new SLRTPAdvice();
guard.define("cache-guard", pc, guard.ACTION_SUBSTITUTE, "CACHED-RECEIPT-42");
p.register(guard);

// In pay.Service.charge(...), weave the processor around the body:
p.runBefore("pay.Service", "charge", args, 2);
SLRTPJoinPoint jp = p.runAround("pay.Service", "charge", args, 2, proposedResult);
String result = jp.didProceed() ? proposedResult : jp.getReturnValue();
p.runAfterReturning("pay.Service", "charge", args, 2, result);
```

## Files

```text
lib/slrtp/
  SLPackage.sleela        Package facade (compiler / loader / Nordshrift).
  SLRTPJoinPoint.sleela   The described execution point + mutable result state.
  SLRTPPointcut.sleela    Type / method / phase matcher (exact or "*").
  SLRTPAdvice.sleela      Pointcut + built-in action (log / count / skip / substitute).
  SLRTPAspect.sleela      A named bundle of advice installed as a unit.
  SLRTPProcessor.sleela   Registry + per-join-point dispatch + bounded trace.
  slrtp-demo.sleela       Runnable end-to-end demonstration.
  README.md               This document.
```

## Run the demo

```sh
sleela run lib/slrtp/slrtp-demo.sleela
```

It installs a tracing aspect and a caching guard aspect, then shows a target
service weaving the processor around `charge(...)`: the before advice traces,
the around guard substitutes a cached receipt (short-circuiting the real
charge), and the after-returning advice traces the result — then a second call
with no guard proceeds normally.

**Max Rupplin — MEARVK LLC — 2026**
