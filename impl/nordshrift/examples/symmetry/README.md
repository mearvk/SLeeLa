# system-symmetry — Sleela sources driven by a `.sst` style sheet

This module demonstrates **server/client system symmetry** across two protocols
(**HTTP** and **FTP**) over one protocol-neutral core, written in Sleela and
**driven entirely by a Nordshrift `.sst` style sheet** ([`build.sst`](build.sst)).

The `.sst` sheet is the *control surface*: it names the sources (`source:` glob
`**/*.sleela`), selects the target (`target-language sleela` — runs on the C
core), sequences the passes (`pipeline:`), and states the analysis posture
(`rules:`/`effects:`/`derive:`/`guards:`/`interop:`). The `.sleela` files are the
*program*. Flip `target-language` to `java` or `c` to retarget the same sources.

## Run it

```sh
cd impl && make
export SLEELA_SHEET=$PWD/../SHEET.sheet          # binds the SHEET.sheet catalog

nordshrift check nordshrift/examples/symmetry/build.sst   # validate the sheet
nordshrift build nordshrift/examples/symmetry/build.sst   # transpile + run all sources
```

`build` transpiles every source the sheet names and, because the target is
`sleela`, runs each on the Sleela core.

## The sources (10 files)

| File | Symmetry it demonstrates | Catalogued objects (conducted methods) |
|------|--------------------------|-----------------------------------------|
| `System.sleela`        | Roll-up: HTTP + FTP server rule == client mirror (`8/8` agree) | `System`, `Service`, `Protocol` |
| `core/Symmetry.sleela` | Mirror + round-trip laws (`mirror(mirror(x))==x`) | `Service`, `Channel` |
| `core/Channel.sleela`  | The shared medium: a spawned writer's frames are read back in order (mailbox `send`/`recv`) | `Channel`, `Pipe`, `Stream` |
| `core/Message.sleela`  | Request/Response framing: sender frame size == receiver frame size | `Message`, `Request`, `Response` |
| `http/HttpServer.sleela` | Server maps `(method, path)` → status | `Service`, `Router`, `Session` |
| `http/HttpClient.sleela` | Client mirrors the server's status table; 2xx success class | `Gateway`, `Retry` |
| `http/Router.sleela`   | Bijective dispatch: `toPath(toHandler(p)) == p` (`3/3`) | `Router`, `Middleware`, `Pipeline` |
| `ftp/FtpServer.sleela` | Server maps command → reply code | `Service`, `Connector` |
| `ftp/FtpClient.sleela` | Client mirrors reply codes; proceed vs abort | `Gateway`, `Session` |
| `ftp/Session.sleela`   | Balanced lifecycle: opens and closes must net to depth 0 | `Session`, `StateMachine`, `Watchdog` |

## How the style sheet drives the build

- **`source:`** — one glob (`**/*.sleela`) enumerates all ten files under
  `src/`; the sheet's `exclude` patterns keep stubs/legacy out.
- **`target:`** — `target-language sleela` sends the transpiled output to the C
  core for execution; change it to `java`/`c` to retarget with no source edits.
- **`pipeline:` / `rules:` / `effects:`** — declare the pass order and the
  symmetric contract posture (e.g. the request/response cycle as an explicit
  effect surface: `RequestEmit`/`ResponseEmit`, `SocketRead`/`SocketWrite`).

## Two views of the conducted methods

Each source also names **catalogued `SHEET.sheet` objects** through Sleela's
conducted methods (`role`, `route`, `congruent`, `sysdepth`, `degreemax`):

- Run through **`nordshrift build`** (no catalog bound) the conducted-method
  lines resolve against an empty catalog (`role(...)` → `""`, `sysdepth()` → `0`)
  — the *computed* symmetry output is unaffected and fully deterministic.
- Run a single file through **`sleela run`** with `SLEELA_SHEET` set, the catalog
  resolves for real:

  ```sh
  ./build/sleela run nordshrift/examples/symmetry/src/core/Symmetry.sleela
  # system depth  = 3024
  # degree max    = 4
  # Service role  = root
  # Channel role  = conversation
  # Request/Response congruent = true
  ```

So the sources are demonstrably driven by both the `.sst` style sheet (which
selects and runs them) and the `SHEET.sheet` catalog (which backs their
conducted methods).
