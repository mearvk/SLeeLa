# The SLeeLa Service Standard

A single, professional common design that managed subsystems — **codecs,
compression, video, crypto, HTTP**, and future families — refer to, so the API
has **one working design** instead of a separate shape per subsystem.

The standard is three SLeeLa source contracts under `lib/core`, layered:

```
SLService            — the universal service lifecycle + identity + result text
  └── SLPluginService — a plugin-registry service (handlers by id, load/dispatch)
SLServiceResult      — the shared result-code vocabulary (used by both)
```

## 1. `SLService` — the universal lifecycle

Every managed subsystem is a *service*: it has a create → use → destroy
lifecycle. `SLService` fixes that contract so cross-cutting tooling (a GUI,
loader, or catalog) can drive any service uniformly:

| Member | Meaning |
|---|---|
| `create()` / `isReady()` / `destroy()` | managed lifecycle |
| `reset()` / `activate()` / `active()` / `status()` | the original low-level state (preserved) |
| `serviceName()` | a short identity for status surfaces |
| `resultText(code)` | render an outcome via the shared `SLServiceResult` vocabulary |

A concrete service overrides `create()` (to allocate its own state), `serviceName()`,
and — if it has extra codes — `resultText()`.

## 2. `SLPluginService` — the plugin-registry harness

The families that manage **a closed set of handlers addressed by 0-based id**
(codecs, compression, video) all converged, independently, on the *same*
Controller design. `SLPluginService` extracts it once so they stop carrying
three copies. It `extends SLService` and provides:

- **lifecycle tables** — `load` / `unload` / `unloadAll` / `isLoaded` / `loadedCount`
- **registry inspection** — `count` / `validId` / `isNative`
- **backend registration** — `registerBackend` / `clearBackend` / `hasBackend`
- **dispatch gate** — `canProcess(id)` and `dispatchCode(id)` (the shared core of
  every family's domain verb)

### Field-configured, not hook-configured

SLeeLa resolves method calls **statically** — a base method does *not* dispatch
to a subclass override. So the harness is configured by **fields**, not by
calling an overridden hook: a subclass calls `configure(count, nativeMask)` from
its own `create()`, and every inherited method reads those fields. `nativeMask`
is a bit set of the ids that are native (process in-package, no backend needed).

A family Controller then shrinks to just what is specific to it:

```sleela
class SLCodecController extends SLPluginService {
  SLCodecVerifier verifier;
  void create() { configure(20, nativeMaskForCodecs()); verifier = new SLCodecVerifier(); }
  int decode(int id) { return dispatchCode(id); }   < thin domain verb >
  int encode(int id) { return dispatchCode(id); }
  String serviceName() { return "SLCodecController"; }
}
```

`codecs` (20 handlers; native 0,1,15,16), `compression` (15; native 0–3), and
`video` (12; native 0) now all extend this harness.

## 3. `SLServiceResult` — the shared result vocabulary

One authoritative result-code table, instead of a copy per Verifier:

| Code | Name | Meaning |
|---|---|---|
| `0` | `OK` | success |
| `-1` | `UNSUPPORTED` | capability/backend not present |
| `-2` | `INVALID` | invalid arguments (e.g. bad id) |
| `-3` | `FORMAT` | input is not this service's format |
| `-4` | `IO` | I/O or allocation failure |
| `-5` | `STATE` | wrong state (e.g. not loaded) |

`text(code)` renders each honestly; `isOk` / `isFailure` classify. Mirrors the
native `sleela_codec_result` / `sleela_compression_result` conventions exactly.

## Honest scope — what does *not* adopt the harness

The standard is applied where it fits and **withheld where it would mislead**:

- **crypto** (`SLCipherContext`) conforms to the lifecycle *concept* and exposes
  `serviceName()`, but is **not** an `SLPluginService` (it is a cipher context,
  not a handler registry) and keeps its **own** result codes
  (`OK/ERR_PARAM/ERR_AUTH/ERR_UNIMPL`) — forcing the shared vocabulary would
  mislabel an authentication failure.
- **HTTP** (`SLHttpClient`, `SLHttpServer`) conforms to the lifecycle *concept*
  (`configure → open/active → close`) and exposes `serviceName()`, but is a
  request/response object graph, not a plugin registry.

A worse abstraction forced onto a subsystem is worse than a little duplication;
the standard unifies the three families that genuinely share a shape, and gives
the others a light, honest point of conformance (identity + lifecycle concept).

## Files

- [`SLService.sleela`](SLService.sleela) — the universal lifecycle base.
- [`SLPluginService.sleela`](SLPluginService.sleela) — the plugin-registry harness.
- [`SLServiceResult.sleela`](SLServiceResult.sleela) — the shared result vocabulary.
- Adopters: `lib/codecs/SLCodecController.sleela`,
  `lib/compression/SLCompressionController.sleela`,
  `lib/video/VideoCodecController.sleela` (extend the harness);
  `lib/crypto/SLCipherContext.sleela`, `lib/http/SLHttpClient.sleela`,
  `lib/http/SLHttpServer.sleela` (conform to the lifecycle concept).
