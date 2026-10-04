<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Language Packs

Localize SLeeLa's outputs and prompts into a language other than English.

A **language pack** is a small, self-contained, dependency-free text file that
maps a stable **message key** to its translation in one language. SLeeLa
programs look up keys through the `lib/languages` library
(`SLLanguage` / `SLLanguageCatalog`); tooling and front ends can read the same
files directly. English (`en`) is the reference pack — every other pack
translates the same key set.

## Why keys, not sentences

Programs reference a stable key (e.g. `prompt.continue`) and the active pack
supplies the display text. Changing language is then a one-line choice
(`SLLanguageCatalog.use("fr")`) with no change to program logic — the SLeeLa
rule *decide in code, present in the user's language*.

## Pack file format

- UTF-8 text, one entry per line: `key = value`.
- Blank lines and lines beginning with `#` are comments.
- The leading `#!locale` / `#!name` / `#!direction` header lines carry metadata
  (BCP-47 code, endonym, and `ltr`/`rtl`). `direction=rtl` marks
  right-to-left scripts (Arabic, Hebrew).
- A value may contain `{0}`, `{1}`, … positional placeholders that the caller
  fills at runtime (e.g. a count or a file name).
- Keys absent from a pack fall back to the English (`en`) value.

```
#!locale = fr
#!name = Français
#!direction = ltr
greeting.hello = Bonjour
prompt.continue = Continuer ?
```

## Message key namespaces

| Namespace    | Purpose                                        |
|--------------|------------------------------------------------|
| `greeting.*` | Common greetings / salutations.                |
| `prompt.*`   | Interactive prompts (yes/no, continue, retry). |
| `word.*`     | Single-word UI labels (yes, no, ok, cancel).   |
| `status.*`   | Runtime status lines (ready, done, failed).    |
| `error.*`    | Error message leads (not found, denied).       |
| `unit.*`     | Measurement/あtime words (seconds, bytes).      |

The canonical key set and its English reference are in
[`en.lang`](en.lang). The complete key list is in [`KEYS.md`](KEYS.md).

## Included packs

Each pack below is an original, concise translation of the reference key set,
authored for this repository and released under the repository's terms — no
third-party pack content is bundled, so there are no external licenses to track.

| Code | Language    | File        | Script direction |
|------|-------------|-------------|------------------|
| `en` | English     | `en.lang`   | ltr (reference)  |
| `es` | Español     | `es.lang`   | ltr              |
| `fr` | Français    | `fr.lang`   | ltr              |
| `de` | Deutsch     | `de.lang`   | ltr              |
| `pt` | Português   | `pt.lang`   | ltr              |
| `it` | Italiano    | `it.lang`   | ltr              |
| `ja` | 日本語       | `ja.lang`   | ltr              |
| `zh` | 中文 (简体)  | `zh.lang`   | ltr              |
| `hi` | हिन्दी        | `hi.lang`   | ltr              |
| `ar` | العربية      | `ar.lang`   | rtl              |

## Using a pack from SLeeLa

```
import languages;

class Hello {
    void main() {
        SLLanguageCatalog cat = new SLLanguageCatalog();
        cat.use("fr");
        print(cat.text("greeting.hello"));   // Bonjour
        print(cat.text("prompt.continue"));  // Continuer ?
    }
}
```

A runnable demonstration is `lib/languages/languages.sleela`.

**Max Rupplin — MEARVK LLC — 2026**