# SLeeLa Language Pack — Message Keys

Every pack in this folder translates the same key set. English (`en.lang`) is
the reference; a key missing from a pack falls back to the English value.

| Key | English reference | Placeholders |
|-----|-------------------|--------------|
| `greeting.hello` | Hello | — |
| `greeting.welcome` | Welcome | — |
| `greeting.goodbye` | Goodbye | — |
| `prompt.continue` | Continue? | — |
| `prompt.yes_no` | Yes or no? | — |
| `prompt.retry` | Try again? | — |
| `prompt.enter_name` | Enter your name: | — |
| `prompt.confirm` | Are you sure? | — |
| `prompt.press_enter` | Press Enter to continue. | — |
| `word.yes` | Yes | — |
| `word.no` | No | — |
| `word.ok` | OK | — |
| `word.cancel` | Cancel | — |
| `word.open` | Open | — |
| `word.close` | Close | — |
| `word.save` | Save | — |
| `word.help` | Help | — |
| `status.ready` | Ready | — |
| `status.running` | Running | — |
| `status.done` | Done | — |
| `status.failed` | Failed | — |
| `status.loading` | Loading | — |
| `status.saved` | Saved | — |
| `error.not_found` | Not found: {0} | `{0}` = subject |
| `error.denied` | Permission denied | — |
| `error.invalid` | Invalid input | — |
| `error.timeout` | Operation timed out | — |
| `error.unknown` | Unknown error | — |
| `unit.seconds` | seconds | — |
| `unit.minutes` | minutes | — |
| `unit.bytes` | bytes | — |
| `unit.items` | items | — |

## Adding a language

1. Copy `en.lang` to `<code>.lang` (BCP-47 code, e.g. `ko.lang`).
2. Set the `#!locale`, `#!name`, and `#!direction` header lines.
3. Translate each value, leaving `{0}`-style placeholders in place.
4. Add a row to the table in `README.md`.

**Max Rupplin — MEARVK LLC — 2026**
