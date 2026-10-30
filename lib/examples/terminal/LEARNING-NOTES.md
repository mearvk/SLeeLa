# Terminal examples — what to learn here

These examples teach **console output and text layout** in SLeeLa: printing
menus, aligning columns, and rendering a progress bar — all by constructing
strings yourself. They are output-only (no interactive input), so each run
prints the same thing.

Read them in order.

## The three cases

| Level | File | The one new idea |
|---|---|---|
| Basic | [`BASIC-menu.sleela`](BASIC-menu.sleela) | Plain multi-line output: a framed menu. |
| Moderate | [`MODERATE-aligned-table.sleela`](MODERATE-aligned-table.sleela) | Fixed-width columns by right-padding strings with spaces. |
| Advanced | [`ADVANCED-progress-bar.sleela`](ADVANCED-progress-bar.sleela) | Build a `[#####-----] 62%` bar from characters, one cell at a time. |

The original [`CLASSIC-terminal-menu.sleela`](CLASSIC-terminal-menu.sleela) is
the shortest starter.

## What we want you to take away

1. **`print` writes one line.** Each call appends a newline, so a UI is just an
   ordered series of `print` calls. There is no cursor control in these examples
   — you compose each full line and print it.
2. **You build layout by building strings.** SLeeLa 1.7 has no padding or
   repeat function, so the moderate and advanced examples construct padding and
   bars by appending characters in a loop. This is the fundamental skill behind
   every text table and progress indicator.
3. **Progress is a ratio turned into characters.** The advanced bar maps a step
   count to a filled-cell count (`step * WIDTH / total`) and a percentage
   (`step * 100 / total`). Integer math keeps the output clean and exact.

## Practical notes for SLeeLa 1.7

- **No `length()` intrinsic.** String has only `+`, `==`, and `!=` at this level.
  The table example therefore takes each field's length as an explicit argument
  (`padTo("hello", 5, col)`). In production you would use the text library's
  string class instead of tracking lengths by hand.
- **Integer division truncates in these ratios**, which is exactly what you want
  for a progress bar — the bar fills in whole cells.
- **Output only.** Reading keystrokes is a separate concern; these files focus on
  rendering so they stay deterministic.

## Run them

```sh
sleela run lib/examples/terminal/MODERATE-aligned-table.sleela
sleela run lib/examples/terminal/ADVANCED-progress-bar.sleela
```

Output is deterministic: the table aligns three columns at width 10, and the
progress bar steps from 0% to 100% in nine lines.
