# GUI.WORK.md — SLeeLa Terminal GUI Work Log

> Working handoff for the next development session. Keep this file current as decisions are implemented, tested, or revised.

## 1. Purpose

This document records the SLeeLa Terminal GUI decisions, implementation context, visual direction, open questions, and next actions so work can resume without reconstructing prior conversations.

Repository: [mearvk/SLeeLa](https://github.com/mearvk/SLeeLa)  
Primary GUI source: `sleela-terminal/sleelaterminal-gui.cpp`  
Shell executor: `sleela-terminal/executor.cpp`  
GUI principles: `sleela-terminal/UI-PRINCIPLES.md`

Work should be committed to both `main` and `master`, keeping the branches aligned unless a task explicitly calls for a different workflow.

## 2. Current visual direction

### Main surfaces and command output

The current target palette is:

| UI element | Target color | RGB | Notes |
|---|---|---|---|
| Main terminal workspace | `#FCFCFC` | `(252, 252, 252)` | Very light neutral background |
| Command output response surface | `#4B4B4B` | `(75, 75, 75)` | Shared background for output text from every command |
| Text inside the response surface | `#F2F2F2` | `(242, 242, 242)` | Lighter than the response surface for legibility |

**Core rule:** whenever a command produces output text, present that output using the same dark response surface and light foreground text. This is not an `ls`-only treatment. It applies consistently to ordinary commands, compiler/build output, status messages, and error output. Preserve meaningful output and error distinctions through text, icons, or restrained accents without breaking the shared surface treatment.

The response surface should only appear when output exists. Empty commands should not create an empty response panel.

The intended design is a visually unified, rounded response rectangle/panel. Do not interpret this as requiring a separate background rectangle for every line or filename. The earlier per-file tile colors were an experiment for `ls`; the broader output-panel rule supersedes that treatment as the overall direction.

### Important implementation distinction

The current terminal uses GTK 4 with VTE. ANSI background sequences can color terminal cells, but they do **not** create a true independent GTK rounded rectangle around a block of output. If the visual requirement is a genuine rounded response panel, investigate an appropriate GTK layout/output component or a carefully designed output view. Do not claim that a real rounded panel exists if the implementation only colors VTE cells.

Any new output presentation must preserve shell behavior:
- stdout and stderr should remain understandable and not be silently lost;
- command execution, exit statuses, interactive programs, and scrolling must continue to work;
- shell redirections and pipelines must remain machine-readable and must not receive GUI-only styling escape sequences;
- output should remain selectable/copyable where practical.

## 3. `ls` output and shell behavior

A custom `ls` shell builtin has been added in `sleela-terminal/executor.cpp`.

Current intended/implemented behavior from the previous work:
- Enumerates directory entries using `std::filesystem`.
- Sorts directories before files.
- Supports `ls`, `ls -a`, `ls -A`, and path arguments.
- On a TTY, renders entries with Unicode rounded-corner tiles and ANSI background colors.
- On redirected/non-TTY output, emits plain newline-separated names without styling escape codes.
- A separate GUI “Files” button was removed; file listing belongs to the command output itself, not a separate Files option.

The current per-entry rendering uses a charcoal file color (`#626262`) and a muted purple directory color (`#6A5B7D`). Treat these as **legacy/current implementation details, not the new global visual rule**. Revisit the builtin so its output aligns with the unified command-response design while preserving normal `ls` semantics and plain output for pipes/redirection.

Known limitation to check: the current tile sizing uses byte length and truncation, which may misalign or truncate Unicode-wide filenames. Add tests for long names, Unicode names, hidden files, paths with spaces, empty directories, and redirected output.

## 4. Window chrome and motion

The broader GUI direction includes:

- A dark, rich-purple title bar.
- A light-blue animated throbber/progress strip.
- A subtle blue glow/reflection into the terminal pane.
- Motion that remains restrained and does not reduce readability or responsiveness.

Prior motion preferences:
- Large bars, over 50 px: approximately 1/1.7 speed (slower).
- Medium bars: approximately 1.12×.
- Small bars: approximately 1.32×.

Confirm the exact interpretation of these speed ratios against the animation code before changing timing. Motion should be smooth, bounded, and preferably respect reduced-motion/accessibility settings if supported.

Window title-bar icon palette previously discussed:
- Normal: `#D9D9D9`
- Hover: `#EEEEEE`
- Active: `#F5F5F5`

## 5. Prompt styling

The shell prompt should distinguish its components without making the prompt noisy:
- Username: bold green.
- Computer/host name: bold blue.
- `@` and `:`: normal foreground color.
- Other prompt punctuation and path styling should remain deliberate and consistent with the terminal palette.

A previous rendering issue was noted: when the prompt fades or repaints, reset the cursor to prompt column 1 before repainting. Verify that animations or redraws do not duplicate prompt text, leave fragments behind, or move the input cursor unexpectedly.

## 6. Technology and build context

- GUI: GTK 4 + VTE (GTK 4 integration).
- GUI source: `sleela-terminal/sleelaterminal-gui.cpp`.
- Shell implementation: `sleela-terminal/executor.cpp`.
- The GUI launches the existing `slsh` shell rather than replacing it with a separate command engine.
- The default Make build was previously restored to build `build/slsh`, `build/smoke`, and `build/SleelaTerminal`.
- Linux development packages noted previously: `libgtk-4-dev` and `libvte-2.91-gtk4-dev`.
- Keep cross-platform goals in view, particularly Windows 10+ and macOS, but do not assume the current GTK/VTE implementation is already portable to those platforms without verification.

## 7. Recent change references

The following commits were reported as pushed previously; this is historical context, not proof that the current branch heads or CI are still at these commits.

- `master` shell `ls` builtin: `b39a21bc57a802013efaba2131f8121ec9325b7f`
- `main` shell `ls` builtin: `5b939fe8af0562bfabbf58f517ae7e0fcabfaa5b`
- `master` GUI-principles documentation: `3d42c1e1ed9a9bdef2c37339772431f7152c9d84`
- `main` GUI-principles documentation: `44281b03b41ece31901cac167cc7ab5b477addd9`
- `master` removal of the separate Files button: `7babc310f8ffe8f6cea0ade056057c8e02a2fdb0`
- `main` removal of the separate Files button: `607f3a2caab739e018cb3ecf9fe69dfbf250a2a4`

Relevant source links:
- [executor.cpp — master](https://github.com/mearvk/SLeeLa/blob/master/sleela-terminal/executor.cpp)
- [executor.cpp — main](https://github.com/mearvk/SLeeLa/blob/main/sleela-terminal/executor.cpp)
- [GUI source — master](https://github.com/mearvk/SLeeLa/blob/master/sleela-terminal/sleelaterminal-gui.cpp)
- [GUI source — main](https://github.com/mearvk/SLeeLa/blob/main/sleela-terminal/sleelaterminal-gui.cpp)
- [UI-PRINCIPLES.md — master](https://github.com/mearvk/SLeeLa/blob/master/sleela-terminal/UI-PRINCIPLES.md)
- [UI-PRINCIPLES.md — main](https://github.com/mearvk/SLeeLa/blob/main/sleela-terminal/UI-PRINCIPLES.md)

## 8. Next-session checklist

### Priority 1 — Inspect before editing
- [ ] Fetch the latest `main` and `master` versions of the GUI, executor, Makefile, tests, and `UI-PRINCIPLES.md`.
- [ ] Compare branch heads and reconcile any divergence before making edits.
- [ ] Inspect how command stdout/stderr currently reaches the VTE widget and whether output can be grouped without disrupting interactive terminal behavior.

### Priority 2 — Implement the unified response appearance
- [ ] Decide whether the response surface can be achieved inside VTE with acceptable compromises or requires a dedicated GTK output view/panel.
- [ ] Apply the shared `#4B4B4B` output surface and `#F2F2F2` text to all command output, not only `ls`.
- [ ] Keep the workspace at `#FCFCFC`.
- [ ] Ensure no output means no response panel.
- [ ] Retire or adapt the `ls`-specific per-file colors so the behavior matches the unified design.
- [ ] Preserve selectable text, scrolling, stdout/stderr visibility, and correct redirection/pipeline behavior.

### Priority 3 — Build and verify
- [ ] Run the repository's documented build from `sleela-terminal` (previously `make -C sleela-terminal`).
- [ ] Run the relevant smoke/unit tests (previously `make test`, if still defined).
- [ ] Test `ls`, `echo`, a successful command, a failing command, compiler output, a command with no output, a pipeline, and redirected output.
- [ ] Check ANSI colors, Unicode, long filenames, terminal resizing, scrolling, copy/paste, and prompt redraw.
- [ ] Record actual build/test outcomes. Do not label checks as passing unless they were run and passed.

### Priority 4 — Document and synchronize
- [ ] Update `sleela-terminal/UI-PRINCIPLES.md` if implementation or visual decisions change.
- [ ] Keep this work log current with what changed, decisions made, tests run, and unresolved issues.
- [ ] Commit and push the verified changes to both `main` and `master`.
- [ ] Verify the file and commit links on both branches after pushing.

## 9. Definition of done

The next milestone is complete when all command output consistently uses the approved shared response styling, the visual treatment is honest about VTE/GTK capabilities, shell semantics remain intact, the GUI builds and relevant tests pass, and the resulting code/docs are present on both branches.

## 10. Session notes

Append dated notes here after each work session.

### 2026-10-12
- Consolidated the current terminal GUI direction and previous implementation context.
- Final visual target clarified: `#FCFCFC` workspace; `#4B4B4B` shared command-output surface; `#F2F2F2` lighter output text.
- The shared response surface applies to every command that emits output, not just `ls`.
- The separate Files button is not part of the intended design.
- Build/test status for the recent GUI/command-output changes remains unverified and must be checked during the next session.
