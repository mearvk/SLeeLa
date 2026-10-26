# Envelopment — Side 2 of the web server interface

The web server supports two sides of one interface.

## Side 1 — a program goes to a domain and then a meme (always on)

A client hits a domain and the domain resolves to a single coherent response
unit: one program's output, one surface. This is the base server's behaviour
and needs no configuration — static files plus the optional `/db` SQL surface.
Side 1 is what runs whenever envelopment is not active, and it is never changed
by the envelopment machinery.

## Side 2 — a domain envelops many programs (opt-in, Scientist-gated)

A single domain may carry many **subjects/warrants for causes**, so the website
becomes an **envelopment** of many disparate programs behind one domain. A
**warrant** binds a *cause* (the reason-to-serve) to a *subject* (a host, with
an optional path gate) and a *program* (what runs). Each program has a backing
kind:

| kind | backing |
|---|---|
| `meme` | a single fixed response (the Side-1 "meme", now one of many) |
| `static` | a document-root subtree |
| `sql` | a `/db`-style SQL surface over its own CSV table directory |

Envelopment is **available for Scientists**: it only activates when the
**Scientist capability** is granted, so ordinary Side-1 operation can never
accidentally fan out into many programs.

### Activation rule

Envelopment is active only when **all** of these hold:

1. `envelopment = on` (or `--envelopment`), and
2. `scientist = true` (or `--scientist`) — the capability gate, and
3. at least one warrant is declared.

If `envelopment` is on but the Scientist capability is absent, the server
**refuses to start** rather than silently running Side 1. When envelopment is
inactive for any other reason, the server runs Side 1 unchanged.

### Control surface

The design calls for this to live in **startup settings or configuration file
settings**. Both are supported and share one code path:

```sh
# Startup settings (CLI):
sleela-web-server --port 8090 --config site.conf --envelopment --scientist

# Or entirely from the configuration file (CLI flags optional):
sleela-web-server --port 8090 --config site.conf
```

The CLI flags can only *raise* the gates; they are still subject to the same
activation rule, so `--envelopment` without a Scientist is refused.

## Configuration grammar

Line-oriented. `#` starts a comment; blank lines are ignored. Unknown keys are
ignored so one file can hold both base-server and envelopment settings. A
`program` or `warrant` line opens a block; subsequent block keys apply to the
most recently opened block.

```text
envelopment = on | off
scientist   = true | false
domain      = <host>

program     = <name>
kind        = static | sql | meme
mount       = /prefix
root        = /path                 # static
db          = /path                 # sql
db-writable = true | false          # sql  (default false)
meme-type   = <content-type>        # meme (default text/plain; charset=utf-8)
meme-body   = <single-line body>    # meme

warrant     = <cause>
subject     = <host> | *
path        = /prefix               # optional path gate
use         = <program-name>
enabled     = true | false          # default true
```

## Resolution

For each request the resolver takes the `Host` header (port stripped) and the
request path and selects a program:

1. Consider enabled warrants whose `subject` is `*` or equals the host, and
   whose `path` prefix (if any) is a prefix of the request path.
2. Among the candidates, the one with the **longest matching path prefix**
   wins; ties break by declaration order. This lets a broad warrant (`path /`)
   coexist with a specific one (`path /papers`) without shadowing it.
3. The selected program is run in its own **mount-relative** path space: a SQL
   program at `mount /lab` answers `/lab/db/query` as if it were `/db/query`.
4. If no warrant matches, the request **falls through to Side 1**.

A warrant that names a missing program is skipped (never a crash).

## Example

See [`../examples/envelopment.conf`](../examples/envelopment.conf): one domain
`science.example` enveloping a `meme` landing page, a `static` `/papers`
subtree, and a read-only `sql` `/lab` dataset. The end-to-end test
`../native/envelopment-test.sh` exercises the Scientist gate, Side-1
fall-through, and warrant routing across all three program kinds.

## SLeeLa model

`lib/web-server-technology/` carries the SLeeLa-side facade:

| SLeeLa class | Models |
|---|---|
| `SLWebProgram` | a mounted program (kind static/sql/meme) |
| `SLWarrant` | a cause served for a subject by a program |
| `SLEnvelopment` | the domain envelopment and its Scientist-gated `active()` rule |

The runnable `demo-web-server-technology.sleela` asserts that the facade's
`active()` rule matches the native `Envelopment::active()` exactly.
