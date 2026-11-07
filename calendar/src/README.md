# calendar/src — SLeeLa sources

The SLeeLa implementation of the Calendar system. Each class maps to a concept
in the calendar documents (`../00`…`../99`) and carries the required
**Metadata Class metapattern** (see `.kiro/steering/metadata-class-standard.md`).

## Classes

| Class | Concept | Document |
|-------|---------|----------|
| `SLCalendarJulian` | Julian baseline, `y % 4` leap rule, serial-day arithmetic | `../01-julian-baseline.md` |
| `SLCalendarGregorian` | Gregorian 4/100/400 leap rule, day-of-week | `../02-gregorian-reform.md` |
| `SLCalendarConverter` | Old Style / New Style offset (10/11/13 days) | `../02`, `../04`, `../06` |
| `SLCalendarIntegrationUS` | The U.S. System of Integration (4 layers) | `../06-system-of-integration-united-states.md` |
| `SLUnifiedCalendar` | The Unified Calendar + overlay principle | `../99-unified-calendar.md` |

## Two forms of the source

- **`CalendarSystem.sleela`** — the **runnable** unit. It contains all five
  classes plus a `CalendarSystemDemo` with `main()`. This is the file to
  `check` and `run`, because the toolchain resolves inter-class references
  **within a single compilation unit** (it does not auto-link sibling `.sleela`
  files — mirrors the house pattern in `examples/extends/`).

- **`SLCalendar*.sleela` + `CalendarDemo.sleela`** — the same classes kept as
  **one-class-per-file** for readability and as the canonical, documented source
  of each class. Checked in isolation, the *dependent* classes report unresolved
  cross-class return types (sibling linking is not performed by the prebuilt
  toolchain); they are validated through `CalendarSystem.sleela`.

## Validate

```sh
# from the repo root, using the committed prebuilt toolchain:
./working-binaries/sleela check calendar/src/CalendarSystem.sleela
./working-binaries/sleela run   calendar/src/CalendarSystem.sleela
```

Expected highlights from `run` (all verified):

```
Julian 1900 leap? true          Gregorian 1900 leap? false   Gregorian 2000 leap? true
Offset 1582-10-15 (expect 10): 10
Offset 1752-09-14 (expect 11): 11
Offset 1918-02-14 (expect 13): 13
Weekday 2000-01-01 (expect 6=Sat): 6
Canonical date 2026-11-7: 2026-11-07
```

## Notes

- `calendar/` is a top-level folder, **not** under `lib/**`, so it is not covered
  by `security/sha256-manifest.json` (the verified-build gate) nor the
  library-inventory test. No manifest or inventory regeneration is required for
  edits here.
- Comments use the go-forward angle-bracket style (`<` / `<<` … `>>`).
