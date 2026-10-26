# GUI.WORK.md — SLeeLa Terminal GUI Work Log

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

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

### GUI component labels and visual inventory

Use the following names consistently in issues, code reviews, screenshots, and future GUI work. These labels describe the visible part and its role; they are documentation labels, not a request to add permanent text labels to every widget.

| Label | Location / appearance | Function and behavior |
|---|---|---|
| **Application title bar** | Top edge; dark-purple/blue gradient | Shared window chrome for the SleelaTerminal application. |
| **SLeeLa title-bar logo** | Far left of the title bar | Branding image; decorative, with a tooltip describing the application. |
| **New Tab (+) button** | Title bar, beside the logo | Opens a new independent terminal session. Shortcut: Ctrl+Shift+T. |
| **Window controls** | Right side of the title bar | Native minimize, maximize/restore, and close controls. |
| **Settings button (hamburger)** | Right side of the title bar, before the window controls | Opens the settings interface. |
| **Throbber / activity rail** | Thin, bright strip directly below the title bar | Animated light-blue cells indicate ambient activity; this is decorative and not a command-progress indicator. |
| **Terminal tab strip** | Above the terminal content | Switches between independent shell sessions. Tabs can be reordered. |
| **Terminal tab title** | Inside each tab, e.g. “Terminal 1” | Identifies the session. Titles are currently sequential labels, not live shell-directory names. |
| **Close Tab (×) button** | Inside each tab | Closes only that tab's shell session. Shortcut: Ctrl+Shift+W. Closing the final tab closes the application. |
| **Terminal workspace** | Main near-white outer work area, `#FCFCFC` | GTK workspace surrounding the active terminal view. |
| **Interactive terminal surface** | Main charcoal area, `#4B4B4B` | VTE terminal receiving keyboard input and displaying the PTY stream; text defaults to `#F2F2F2`. |
| **Terminal glow overlay** | Subtle light effect over the terminal surface | Decorative reflection from the activity rail. It must not intercept mouse or keyboard input. |
| **Command summary rectangle / warm-brown command strip** | Brown-toned row printed immediately after an interactive command is submitted | Summarizes the exact command text and its local date/time. This is the command-append summary row; the command's actual output begins on the following line. The brown appearance is ANSI background applied to VTE text cells, **not** a standalone GTK rectangle or a command-output panel. |
| **Command output** | Terminal lines after the command summary row | Actual stdout/stderr and interactive-program output. It remains in the charcoal terminal surface and must retain shell/PTY semantics. |
| **Footer brand** | Bottom-left, “SLeeLa” | Static product branding. |
| **Footer separator** | Vertical divider beside the footer brand | Separates branding from the footer message. |
| **Footer status/ticker text** | Middle of the footer | Displays the configured status/feed text within the available width. |
| **CMD / software launcher control** | Image control toward the footer's right side | Opens the software/install menu; it is a launcher, not a shell tab. |
| **Version label** | Bottom-right of the footer | Displays the GUI version string. |
| **Terminal context menu** | Right-click inside a terminal | Context actions for terminal interaction, such as copy/paste/select-all where available. |
| **Settings interface** | Opened from the title-bar Settings button | Holds presentation and data-orientation preferences; settings are shared by tabs. |

#### Command summary strip: exact meaning

The warm-brown row is specifically the **command summary rectangle** (also called the **command summary strip** or **command-append row**). It records what interactive command was submitted and when it was submitted, using the local timestamp. It is distinct from both the shell prompt and the output produced by the command:

1. **Prompt/input** — the user enters a command.
2. **Command summary strip** — the submitted command and local date/time are shown with the muted warm-brown ANSI cell background.
3. **Command output** — the command's actual result starts on the next line.

The strip is a presentation row for interactive shell use. It must not inject styling into scripts, pipelines, redirected file contents, or the actual command's argv. Do not describe it as a separate rounded GTK widget unless a future implementation actually creates one.

### Important implementation distinction

The current terminal uses GTK 4 with VTE. ANSI background sequences can color terminal cells, but they do **not** create a true independent GTK rounded rectangle around a block of output.

- The near-white workspace and charcoal interactive terminal remain unchanged.
- After each submitted interactive command, the shell prints a muted warm-brown summary strip with the command and local date/time; actual command output begins on the following line. This is ANSI styling on text cells, not an independent GTK panel.
- A separate rounded response panel that appears only when output exists is **not implemented** and remains a distinct architecture task.

Any new output presentation must preserve shell behavior:
- stdout and stderr should remain understandable and not be silently lost;
- command execution, exit statuses, interactive programs, and scrolling must continue to work;
- shell redirections and pipelines must remain machine-readable and must not receive GUI-only styling escape sequences;
- output should remain selectable/copyable where practical.

## 3. `ls` output and shell behavior

The custom `ls` builtin has been removed. `slsh` now resolves `ls` through the normal external-command PATH lookup, so the operating system's standard `ls` handles `-l`, `-la`, and its full native option set. Command flags remain separate argv entries and are not stripped or reinterpreted by the shell. Standard output, pipelines, and redirection remain unstyled.

External commands continue to receive option flags in argv; the leading hyphen is not stripped by the shell. Regression checks exercise `ls -l`, `ls -la`, `ls -lh`, `ls -F`, `ls -d`, `ls -1 -- PATH`, and an external command receiving `-la` as an argument.

Continue testing long and Unicode filenames, paths with spaces, empty directories, and output redirection.

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
- [x] Apply the shared `#4B4B4B` terminal surface and `#F2F2F2` text to all VTE output, not only `ls`.
- [ ] Implement a distinct per-command rounded response panel that appears only when output exists; this requires an architecture change beyond VTE cell styling.
- [ ] Keep the workspace at `#FCFCFC`.
- [ ] Ensure no output means no response panel.
- [x] Retire the `ls`-specific per-file tiles and return to conventional plain-text output.
- [ ] Preserve selectable text, scrolling, stdout/stderr visibility, and correct redirection/pipeline behavior.

### Priority 3 — Build and verify
- [ ] Run the repository's documented build from `sleela-terminal` (previously `make -C sleela-terminal`).
- [ ] Run the relevant smoke/unit tests (previously `make test`, if still defined).
- [ ] Test `ls`, `echo`, a successful command, a failing command, compiler output, a command with no output, a pipeline, and redirected output.
- [ ] Verify rounded clipping and the foreground/background colors in a running GTK/VTE window.
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

### 2026-10-12 — terminal surface correction
- Corrected the VTE runtime background from near-white to charcoal and forced the default terminal foreground to near-white, matching the CSS palette. This resolves the CSS/runtime mismatch that caused the terminal to appear white inside the dark frame.
- Kept the workspace near-white and added GTK overflow clipping to the rounded terminal overlay.
- Scope note: this delivers a consistent dark interactive-terminal surface for all command output while preserving VTE/PTTY behavior. It does not implement output-only panels per command; that needs a larger output architecture change. Do not mark it complete prematurely.
- Build/runtime verification must be performed in an environment with GTK 4 and VTE GTK 4 development packages. No local GUI build was available during this edit.


### 2026-10-12 — standard command results and summary rows
- Removed the custom per-file `ls` tiles and then removed the custom `ls` builtin itself. The shell now resolves the system `ls` through PATH, restoring native output formatting and native handling of `-l`, `-la`, and other options.
- The observed `ls -l` issue came from the custom builtin's option handling; normal external commands already receive option flags as argv entries. Regression checks cover common `ls` invocations and flag preservation for an external command.
- Added an interactive summary row immediately after command submission: command text plus local date/time in a muted warm-brown ANSI text-cell background, followed by the command's actual output on the next line. The workspace/terminal palette remains unchanged.
- The summary treatment is not a standalone GTK rounded output panel; it colors the summary text cells in VTE. Shell scripts/non-interactive execution remain free of these presentation rows.
- Validation status: source and documentation updates pushed; GUI compilation/runtime still requires GTK 4 and VTE GTK 4 development packages. Run the shell smoke suite and explicit `ls -l`, `ls -la`, external flag, pipeline, and redirection checks before treating runtime validation as complete.


### 2026-10-12 — independent terminal tabs
- Added a GTK Notebook tab strip and a title-bar `+` button. Each tab has an independent VTE terminal, PTY, `slsh` process, scrollback, and shell working directory while sharing the main window chrome and palette.
- Added per-tab close buttons; closing one tab signals only its shell, and closing the final tab exits the application. Tabs are reorderable.
- Keyboard shortcuts: Ctrl+Shift+T new tab, Ctrl+Shift+W close current tab, Ctrl+Tab / Ctrl+Page Down next tab, Ctrl+Shift+Tab / Ctrl+Page Up previous tab.
- Kept the existing workspace (`#FCFCFC`), terminal surface (`#4B4B4B`), light text (`#F2F2F2`), and command-summary row treatment.
- Source committed to both `main` and `master`; the GUI has not been compiled or runtime-tested in this environment because GTK 4 / VTE GTK 4 development packages are unavailable. Verify tab creation, switching, independent commands/current directories, close-one-while-others-run, final-tab close, keyboard shortcuts, and resize/scrollback before marking runtime validation complete.
