# `guidoc` — the document GUI, in white

A document editor with **menus, tabs, an editor, and load/save**, built on a
**GUI document listener**: each tab holds a standing document with its own
dynamic observer, and a white GUI listener admitted to that observer keeps the
screen in step with the document, telling edits made through the GUI apart from
edits made by someone else.

```
sleela run lib/guidoc/guidoc-demo.sleela        # 99 self-checks, headless
python3 tools/validate_gui_document.py          # XML schema + rules + drift
```

## What it looks like (white)

```
 SLeeLa Document Editor - /home/me/notes.txt*                      bg #F7F8FA  ink #16202C
┌──────────────────────────────────────────────────────────────────────────┐
│ File  Edit  View  Help                                    chrome #FFFFFF │
├──────────────────────────────────────────────────────────────────────────┤
│ [ notes.txt* ] [ second.txt ] [ + ]                      active tab: accent #1F6FEB │
├────┬─────────────────────────────────────────────────────────────────────┤
│  1 │ alpha                                              surface #FFFFFF  │
│  2 │ gamma            ◀ selected row: accent #1F6FEB, white text          │
│  3 │ tail                                                                │
├────┴─────────────────────────────────────────────────────────────────────┤
│ [ text for the next edit                                               ] │
├──────────────────────────────────────────────────────────────────────────┤
│ inserted line 3        Ln 3 of 3  rev 6        White | in step at rev 6  │
└──────────────────────────────────────────────────────────────────────────┘
```

Every colour comes from the **White** flavor of the standard UI themes
(`lib/ui` `SLThemeFlavor.white()`) through `SLGuiDocWhite`; unsaved changes show
in the warning ink, external changes in the status line, errors in the danger ink.

## The XML document

[`gui-document.xml`](gui-document.xml) describes the GUI and
[`gui-document.xsd`](gui-document.xsd) is its schema:

| Element | Describes |
|---|---|
| `<window title theme>` | title and theme flavor (`white`, `blue`, `gray`, `black`; this document is white) |
| `<commands><command id handler confirmsDiscard>` | every command and the `SLGuiDocApp` method that runs it; commands that can discard work set `confirmsDiscard` |
| `<menus><menu><item id label command accelerator enabledWhen>` | the menu bar. `command` must name a declared command (a keyref); `enabledWhen` is one of `always hasTab dirty editable selectionEditable multipleTabs` |
| `<tabs maxTabs dirtyMarker newTabTitle>` | the tab strip (at most 14, echoing the 1..14 watch bound of `lib/gui`) |
| `<editor rows lineNumbers readOnlyWhenUnsplit>` | the editor |
| `<fileDialog sidecarSuffix maxLines maxBytes>` | load and save limits |
| `<listener port repaintOn flagExternal>` | the GUI document listener |

`SLGuiDocSpec.standard()` is the executable copy; the GUI is **built from it**.
SLeeLa cannot parse XML at run time (no string access), so
`tools/validate_gui_document.py` keeps the two together: it validates the XML
against the schema (`xmllint`), applies the rules the schema cannot express
(declared commands, unique accelerators, no orphan commands, discarding commands
must confirm, every handler exists in `SLGuiDocApp`, theme is white) and fails if
the XML and `standard()` differ.

### Commands

| Menu | Item | Accelerator | Enabled when | Does |
|---|---|---|---|---|
| File | New | Ctrl+N | always | a new untitled tab |
| File | Open... | Ctrl+O | always | open dialog; opens in a new tab, or switches to the tab that has it |
| File | Save | Ctrl+S | dirty | saves to the tab's path (Save As if it has none) |
| File | Save As... | Ctrl+Shift+S | hasTab | save dialog |
| File | Close Tab | Ctrl+W | hasTab | asks first if there are unsaved changes |
| File | Quit | Ctrl+Q | always | asks first if any tab has unsaved changes |
| Edit | Insert Line | Ctrl+I | editable | inserts the entry text after the selected line (or at the end) |
| Edit | Replace Line | Ctrl+R | selectionEditable | replaces the selected line |
| Edit | Delete Line | Ctrl+D | selectionEditable | deletes the selected line |
| View | Next / Previous Tab | Ctrl+Tab / Ctrl+Shift+Tab | multipleTabs | cycles tabs |
| Help | About | F1 | always | status line |

While a dialog is open, every other command is refused and the menus are disabled.

## The classes

| Class | Role |
|---|---|
| `SLGuiDocSpec` | the spec (validated as added), mirror of the XML |
| `SLGuiDocWhite` | the white look: a style for every role |
| `SLGuiDocFile` | load and save |
| `SLGuiDocListener` | the GUI document listener; nested `Responder` is an observer module |
| `SLGuiDocTab` | one open document: its observer, its listener, path, dirty state, selection, scroll |
| `SLGuiDocView` | the widget tree (76 nodes): tag, text, style, visible, enabled, command; press latches |
| `SLGuiDocApp` | the controller: builds the view from the spec, runs commands, dialogs and tabs, repaints |

## The GUI document listener

`SLGuiDocListener.Responder` is a drop-in observer module admitted to each tab's
observer, so it has the full listener lifecycle (pause, resume, alter, replace,
reinstate). Each change latches a repaint. A change whose source is not `gui` is
an **external change**: it is counted, named in the status line
(`changed by alice (1 external change(s))`) in the warning ink, and stays flagged
until the user acknowledges it by selecting a line or editing. A host loop calls
`app.refresh()` after anything outside the GUI may have changed a document.

## Load and save

* **Save** writes plain text (lines joined with `\n`, ending in one) to a
  temporary file and **renames** it into place, then writes a sidecar directory
  `<file>.lines/` with one file per line (`l0000`, `l0001`, ...). A shorter save
  removes the stale sidecar lines.
* **Load never trusts a split.** SLeeLa cannot cut a `String` into lines, so the
  loader reads the whole file and accepts the sidecar lines **only if joining
  them reproduces the file exactly**. If the file was changed by anything else,
  or has no sidecar, it opens **unsplit** (one block) and the tab is
  **read-only**: it can be viewed but not edited or saved, so nothing is ever
  loaded or written that the file does not literally contain.
* Limits: 64 lines, 65536 bytes. Over the limit is refused, not truncated.

| Save code | Meaning | Load code | Meaning |
|---|---|---|---|
| 0 | ok | 0 | exact lines |
| 1 | bad path | 10 | opened unsplit (read-only) |
| 2 | cannot write | 1 | bad path |
| 3 | cannot replace | 4 | cannot open / not found |
| 4 | cannot write sidecar | 5 | too many lines |
| 6 | too many lines | 7 | too large |
| 9 | read-only tab | 8 | target not empty |

## Honest limits

* **Not mounted on a real window here.** `SLGuiApp`/`SLGuiDom` do not link under
  the committed binary (see `lib/observer/OBSERVER.md`), so the view is a
  self-contained retained widget tree, like `SLObserverView`. A host walks
  `nodeCount()` / `idAt()` / `tagAt()` / `textAt()` / `styleAt()` / `visibleAt()` /
  `enabledAt()` to mount it, and calls `app.click(id)` / `app.type(id, text)` /
  `app.accelerate(key)`. The demo verifies the screen model, not pixels.
* **Plain-text files from elsewhere open read-only.** Splitting a foreign file
  into lines needs string access the committed binary lacks. The source tree has
  `javaString*` built-ins; a splitter built on them can feed `SLGuiDocFile` safely
  because the exact-rejoin check validates any split.
* **A line is a single line.** The entry is one line; lines cannot contain
  newlines. No undo. No multi-line selection. No syntax highlighting.
* **Paths pass through as given.** The GUI cannot inspect a path string, so the
  host decides which paths a user may reach.
* The display name of a tab opened from a file is its full path (no basename
  without string access).
