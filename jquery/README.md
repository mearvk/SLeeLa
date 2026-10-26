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

## Usage

Browser (full build):

```html
<script src="jquery/jquery.min.js"></script>
```

Browser (slim build — smaller, no ajax/effects):

```html
<script src="jquery/jquery.slim.min.js"></script>
```

## License

jQuery is released under the MIT license. (c) OpenJS Foundation and other contributors — see <https://jquery.com/license/>.
