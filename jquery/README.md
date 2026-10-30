# jQuery

This folder contains the official **jQuery 4.0.0** distribution (the latest release, shipped January 2026).

## Source

Files were taken verbatim from jQuery's official distribution repository at the release tag:

- Repo: [`jquery/jquery-dist`](https://github.com/jquery/jquery-dist)
- Tag: `4.0.0`
- Path: `dist/`

These are byte-for-byte identical to the files served by the jQuery CDN (`code.jquery.com`), cdnjs, and jsDelivr.

## Contents

| File | Description |
|------|-------------|
| `jquery.js` | Full uncompressed build |
| `jquery.min.js` | Full minified build |
| `jquery.min.map` | Source map for `jquery.min.js` |
| `jquery.slim.js` | Slim uncompressed build (excludes `ajax` and `effects` modules) |
| `jquery.slim.min.js` | Slim minified build |
| `jquery.slim.min.map` | Source map for `jquery.slim.min.js` |
| `jquery.factory.js` | Factory (module) build |
| `jquery.factory.slim.js` | Slim factory build |
| `package.json` | Declares `{"type":"commonjs"}` |
| `LICENSE.txt` | jQuery's MIT license (vendored alongside the code) |
| `SHA256SUMS` | SHA-256 checksums of the distribution files (integrity) |

### Source maps

Only the two **minified** builds ship a source map: `jquery.min.js` →
`jquery.min.map`, and `jquery.slim.min.js` → `jquery.slim.min.map`. The
uncompressed builds (`jquery.js`, `jquery.slim.js`) and the factory builds
(`jquery.factory.js`, `jquery.factory.slim.js`) have no maps — this matches
upstream. Note the minified files do not carry a trailing
`//# sourceMappingURL=` comment (upstream strips it), so load a map manually if
you need it.

## Usage

Browser (full build):

```html
<script src="jquery/jquery.min.js"></script>
```

Browser (slim build — smaller, no ajax/effects):

```html
<script src="jquery/jquery.slim.min.js"></script>
```

## Integrity

This folder is a **vendored third-party library**; its files are not modified.
`SHA256SUMS` records the SHA-256 of each distribution artifact so any local
drift or tampering is detectable. Verify from this directory:

```sh
sha256sum -c SHA256SUMS
```

Every line should report `OK`. The manifest covers only the distribution files
(`jquery*.js`, `*.map`, `package.json`); it intentionally excludes `README.md`,
`LICENSE.txt`, and `SHA256SUMS` itself. If you ever re-vendor a new jQuery
release, regenerate it with
`sha256sum jquery*.js jquery*.map package.json > SHA256SUMS`.

## License

jQuery is released under the **MIT license** — Copyright OpenJS Foundation and
other contributors. The full text is vendored here as
[`LICENSE.txt`](LICENSE.txt), and the MIT banner is also embedded at the top of
`jquery.js`, `jquery.min.js`, and `jquery.slim.js`. See
<https://jquery.com/license/>.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>
