# SleelaTerminal™ UI Principles

A short, enforceable set of rules for the look and feel of the SleelaTerminal™
graphical terminal (`SleelaTerminal`, GTK 4 + VTE). The goal is a **clean,
calm, legible** terminal where the shell content is the hero and the chrome is a
quiet, consistent frame.

These principles describe *presentation intent*. They do not change shell
behaviour, add network/data behaviour, or alter the configuration schema.

## 1. The terminal is the hero

The VTE terminal is the primary, centered workspace. Chrome (title bar, footer,
popovers) is a quiet frame around it. Chrome must never compete with terminal
content for attention: no blinking, flashing, or attention-grabbing motion.
Controls appear on demand (popovers / right-click), not as permanent panels.

The one sanctioned exception is the **title-bar throbber**: a 2px living
light-blue seam along the bottom of the title bar that replaces the old static
border. It is a field of many small excitations in relative shades of one base
light-blue, flowing mainly left→right at ~20 Hz with third-order (jerk-bounded)
speed changes, so it drifts and breathes rather than scrolling mechanically. It
is allowed because it is *ambient* — low-contrast, slow, edge-only, and carrying
no information the user must track — so it signals "alive" without pulling the
eye from the terminal. Any motion in the chrome must clear that same bar.

## 2. One palette, one source of truth

All colours come from a single named palette defined once at the top of the CSS
(as CSS custom properties / `@define-color`). No raw hex values are scattered
through individual selectors. Changing the theme means editing one block.

The window chrome retains the project's dark rich-purple family, while the
terminal text pane and command-result surfaces use a separate neutral, high-contrast
reading palette:

| Token | Value | Use |
|---|---|---|
| `--sl-bg` | `#15101c` | window chrome background |
| `--sl-surface` | `#21112f` | popovers, dialogs, raised surfaces |
| `--sl-surface-hi` | `#321a4d` | gradient top / hover surface |
| `--sl-chrome` | `#24103f` | title bar / footer base |
| `--sl-border` | `#7f56aa` | 1px separators and surface borders |
| `--sl-fg` | `#ffffff` | chrome text / controls |
| `--sl-fg-dim` | `rgba(255,255,255,.70)` | secondary / note text |
| `--sl-accent` | `#9d6cff` | focus ring, suggested action |

The GTK workspace uses `#fcfcfc` (RGB 252, 252, 252). The interactive VTE
terminal surface uses charcoal `#4b4b4b` with near-white `#f2f2f2` text. Keep
this three-colour hierarchy consistent: near-white workspace, dark terminal
surface, light readable terminal text. The purple/black palette belongs to the
surrounding window chrome only.

**Command summary row:** after an interactive command is submitted, `slsh`
prints a muted warm-brown summary strip containing the entered command and
local date/time. The command's output begins on the following line. This is
ANSI background applied to the summary text cells, not a separate GTK panel.
The terminal/workspace colours remain unchanged. Non-interactive scripts do
not receive these presentation rows.

**Implementation boundary:** VTE still presents an ongoing interactive PTY
stream, not discrete command-result widgets. The summary strip does not yet
create a separate rounded output panel that appears only when output exists.
Do not describe that larger output-view architecture as implemented.

## 3. Restraint over effects

- At most a **1px** border for separation; prefer a hairline border or a single
  subtle gradient over stacked `box-shadow` + `inset` + glow.
- No `-gtk-icon-shadow` glows on controls. Legibility comes from contrast, not
  halos.
- Hover/active states are a small background-opacity step (e.g. `.08 → .16 →
  .24`), nothing more.
- Transitions, if any, are a single short `120ms` ease on background/opacity.

## 4. Consistent spacing

Use a 4px spacing scale: **4, 8, 12, 16**. Container padding is `12`. Related
controls are spaced `6`. Section gaps are `12`. Do not invent one-off margins
(no `14`, `22`, `5`). Popovers and dialogs share the same inner padding (`12`).

## 5. Type hierarchy, minimal weights

- Terminal text: the user-configured monospace font.
- Chrome text: the system UI font, two weights only — `600` for labels, `800`
  for the brand/section headings. Avoid a third weight.
- Secondary/explanatory text uses `--sl-fg-dim` at `.9em`, never a new colour.
- Avoid letter-spacing tricks except the single brand label.

## 6. Calm status, no motion

The footer presents **static** status: brand, a separator, one status string,
the version. It does not rotate or scroll text in the resting state. (A ticker,
if ever desired, is opt-in via configuration — off by default — because moving
text competes with terminal output; see SETTINGS.md.)

## 7. Clear, honest affordances

- Clickable things look clickable (a surface tint + hover step); non-interactive
  labels do not.
- The one `suggested-action` button per dialog uses `--sl-accent`; everything
  else is a neutral surface button. Never two competing primary buttons.
- Focus is always visible: a `--sl-accent` focus ring on keyboard focus.

## 8. Accessibility baseline

- Body/control text on its background meets at least WCAG AA contrast (≈4.5:1).
  White (`#ffffff`) on the purple chrome and near-black terminal clears this; dim
  text is reserved for non-essential notes only.
- Nothing is conveyed by colour alone; icons carry tooltips and text labels.
- Hit targets are at least `32px` tall.

## 9. Correctness of the stylesheet

- The GTK CSS lives in a C++ raw string literal `R"CSS( … )CSS"`. Inside it,
  real newlines are literal — never write the two characters `\n`, which GTK's
  CSS parser rejects and which silently drops the rest of a rule.
- Keep selectors flat and class-scoped (`popover.sleela-settings …`) so styles
  don't leak into unrelated widgets.

## 10. Maintenance rule

When the GUI's look changes, update the palette block (not individual
selectors), keep this document in sync, and rebuild `make gui` to confirm the
stylesheet still parses. If a change adds motion, a new colour, or a new
spacing value, it must either fit these principles or update them deliberately.

— SleelaTerminal™ · MEARVK LLC

## 11. File browsing and `ls` output

The main workspace remains near-white (`#FCFCFC`); the interactive terminal
uses charcoal (`#4B4B4B`) with light text (`#F2F2F2`). The custom `ls`
builtin and per-file tiles have been removed. The shell resolves `ls` through
the normal external-command PATH lookup, so the platform's standard `ls`
provides its native output format and full flag set (including `-l` and
`-la`). Command flags are preserved as argv entries, and redirected output
and pipelines remain free of GUI styling sequences.

The command summary strip is separate from command output: it shows the command
text and local timestamp, then the result follows on the next line. It does
not alter command arguments, stdout bytes, or redirection contents.


## 12. Independent terminal tabs

The main terminal workspace supports multiple tabs in one window. Each tab owns
its own VTE widget, PTY, shell process, scrollback, and terminal state; commands
and working-directory changes in one tab must not affect another. The first
and subsequent tabs share the application frame, palette, settings, and footer.
A plus control creates a tab; each tab has its own close control. Keyboard
shortcuts: Ctrl+Shift+T creates a tab, Ctrl+Shift+W closes the current tab,
Ctrl+Tab / Ctrl+Page Down advances, and Ctrl+Shift+Tab / Ctrl+Page Up goes back.
Closing a shell tab must not terminate other tabs; closing the final tab closes
the application. The existing near-white workspace and charcoal terminal
surface are unchanged.
