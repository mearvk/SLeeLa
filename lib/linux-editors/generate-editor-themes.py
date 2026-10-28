#!/usr/bin/env python3
"""Generate SLeeLa color schemes / syntax highlighting for Neovim, Helix, nano,
and VS Code from one palette table, mirroring the existing vim/emacs themes.

The palette hex values are kept identical to lib/linux-editors/vim/colors/*.vim
(the source of truth). Run from the repo root or anywhere:

    python3 lib/linux-editors/generate-editor-themes.py

It writes, under lib/linux-editors/:
  neovim/  lua/sleela/<theme>.lua colorschemes + ftdetect + syntax shim
  helix/   languages.toml + themes/sleela-<theme>.toml
  nano/    sleela.nanorc (syntax) + sleela-<theme>.nanorc (color overrides)
  vscode/  package.json + syntaxes/sleela.tmLanguage.json + themes/*.json

Ed / Sed are line editors with no color-scheme concept; see nano/README note
and the top-level editor README for the scripted-editing guidance instead.
"""
import json
import os

HERE = os.path.dirname(os.path.abspath(__file__))

# ---------------------------------------------------------------------------
# Palette table. Each entry mirrors the matching vim/colors/sleela-<id>.vim and
# emacs/sleela-<id>-theme.el exactly. Keys:
#   name        human-readable scheme name
#   bg/fg       editor background / default foreground
#   cursorline  subtle current-line background
#   linenr      gutter line-number foreground
#   visual      selection background
#   searchbg    incremental-search background (fg is bg of the theme)
#   statusbg    status line background
#   keyword type string number constant function comment operator
#   preproc     document markers / pragmas / annotations face (PreProc)
#   special     brander / guide face (Special)
#   error       diagnostic foreground
# ---------------------------------------------------------------------------
THEMES = {
    "american": dict(
        name="SLeeLa American Professional",
        bg="#101820", fg="#F4F7FA", cursorline="#172632", linenr="#687887",
        visual="#233D55", searchbg="#F2C078", statusbg="#233D55",
        keyword="#7FB3FF", type="#73DACA", string="#F2C078", number="#C4A7E7",
        constant="#FF8F8F", function="#82C91E", comment="#8A96A3",
        operator="#D8DEE9", preproc="#F0A6CA", special="#73DACA", error="#FF6B6B",
    ),
    "eurasia": dict(
        name="SLeeLa Pan-Eurasian",
        bg="#111827", fg="#F0F2F5", cursorline="#1B293A", linenr="#7F8B9B",
        visual="#30435A", searchbg="#D7B56D", statusbg="#30435A",
        keyword="#7AA7E8", type="#69C9B3", string="#D7B56D", number="#C5A6E8",
        constant="#D98A9A", function="#8FCB8A", comment="#8995A5",
        operator="#D0D8E2", preproc="#D98A9A", special="#69C9B3", error="#FF7777",
    ),
    "russian-slavic": dict(
        name="SLeeLa Russian and Slavic (Medium)",
        bg="#202733", fg="#E8EAF0", cursorline="#293341", linenr="#929BA8",
        visual="#3B4655", searchbg="#C9AD76", statusbg="#3B4655",
        keyword="#A9C0DD", type="#8FC4B2", string="#D6BD8B", number="#C2B0D8",
        constant="#D99198", function="#A7C69C", comment="#9AA3AF",
        operator="#D1D6DE", preproc="#D5A6B4", special="#8FC4B2", error="#E58B91",
    ),
    "silk-road": dict(
        name="SLeeLa Central Asian / Silk Road",
        bg="#1C2529", fg="#F5F0E5", cursorline="#26363A", linenr="#87999A",
        visual="#315052", searchbg="#D9B56D", statusbg="#315052",
        keyword="#7FADE0", type="#45C6C2", string="#D9B56D", number="#C4A4DE",
        constant="#E58B79", function="#A7C982", comment="#92A3A0",
        operator="#D5DEDA", preproc="#E5A9A0", special="#45C6C2", error="#FF7777",
    ),
    "east-asian": dict(
        name="SLeeLa East Asian",
        bg="#171A1C", fg="#F2EFE7", cursorline="#242A2B", linenr="#8C9693",
        visual="#3A4541", searchbg="#D8B76A", statusbg="#343D39",
        keyword="#D97878", type="#72C7A5", string="#D8B76A", number="#BDA6DE",
        constant="#E59B83", function="#9FCB9A", comment="#969F9B",
        operator="#D6DBD7", preproc="#D97878", special="#72C7A5", error="#FF7777",
    ),
    "greek": dict(
        name="SLeeLa Greek",
        bg="#0A1A30", fg="#FFFFFF", cursorline="#102744", linenr="#6E86A6",
        visual="#0D5EAF", searchbg="#FFFFFF", statusbg="#0D5EAF",
        keyword="#2E7BD6", type="#5E9FE0", string="#9FC3EC", number="#C7DBF3",
        constant="#4C8FDE", function="#FFFFFF", comment="#6E86A6",
        operator="#C7D4E4", preproc="#5E9FE0", special="#9FC3EC", error="#FF6B6B",
    ),
    "turkish": dict(
        name="SLeeLa Turkish",
        bg="#2A0609", fg="#FFFFFF", cursorline="#3E0F13", linenr="#A8767A",
        visual="#E30A17", searchbg="#FFFFFF", statusbg="#E30A17",
        keyword="#F23D48", type="#F0A9AE", string="#F0C9A9", number="#F3D5C7",
        constant="#E8505B", function="#FFFFFF", comment="#A8767A",
        operator="#E4CCCE", preproc="#F0A9AE", special="#F0C9A9", error="#FF8A3D",
    ),
}

DEFAULT = "american"

# Shared language facts (kept in step with vim/syntax/sleela.vim).
KEYWORDS = ("let var const if else while for in return break continue class "
            "interface extends implements new this super import package public "
            "private protected static final true false null nil and or not "
            "try catch throw synchronized finally switch case default do assert "
            "yield instanceof").split()
TYPES = ("String SLString Integer Int Long Double Float Boolean Character Byte "
         "Short Void Object List Map Set Array int64 int128 long double boolean").split()
MARKERS = ("author", "authorial", "material", "memory", "determine")


def w(relpath, text):
    path = os.path.join(HERE, relpath)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w") as f:
        f.write(text if text.endswith("\n") else text + "\n")
    print("wrote", os.path.relpath(path, HERE))


# ===========================================================================
# Neovim — native Lua colorschemes. Neovim reads the existing vim/syntax file,
# so these supply the colours (highlight groups) in idiomatic Lua, plus a tiny
# filetype shim.
# ===========================================================================
def gen_neovim():
    for tid, p in THEMES.items():
        lines = [
            f'-- {p["name"]} colorscheme for Neovim (Lua).',
            '-- Mirrors lib/linux-editors/vim/colors/sleela-%s.vim.' % tid,
            'vim.cmd("highlight clear")',
            'if vim.fn.exists("syntax_on") == 1 then vim.cmd("syntax reset") end',
            'vim.o.background = "dark"',
            'vim.g.colors_name = "sleela-%s"' % tid,
            'local s = vim.api.nvim_set_hl',
            'local function hi(group, opts) s(0, group, opts) end',
        ]
        g = lambda grp, **kw: lines.append(
            "hi(%r, %s)" % (grp, "{ " + ", ".join(
                (f'{k} = {v!r}' if isinstance(v, str) else f'{k} = {str(v).lower()}')
                for k, v in kw.items()) + " }"))
        g("Normal", fg=p["fg"], bg=p["bg"])
        g("Cursor", fg=p["bg"], bg=p["keyword"])
        g("CursorLine", bg=p["cursorline"])
        g("LineNr", fg=p["linenr"], bg=p["bg"])
        g("CursorLineNr", fg=p["keyword"], bold=True)
        g("Visual", bg=p["visual"])
        g("Search", fg=p["bg"], bg=p["searchbg"])
        g("StatusLine", fg=p["fg"], bg=p["statusbg"], bold=True)
        g("Pmenu", fg=p["fg"], bg=p["statusbg"])
        g("MatchParen", fg=p["fg"], bg=p["visual"], bold=True)
        g("Keyword", fg=p["keyword"], bold=True)
        g("Statement", fg=p["keyword"], bold=True)
        g("Type", fg=p["type"], bold=True)
        g("String", fg=p["string"])
        g("Number", fg=p["number"])
        g("Constant", fg=p["constant"])
        g("Function", fg=p["function"], bold=True)
        g("Identifier", fg=p["fg"])
        g("Operator", fg=p["operator"])
        g("Comment", fg=p["comment"], italic=True)
        g("PreProc", fg=p["preproc"])
        g("Special", fg=p["special"])
        g("Error", fg=p["error"], undercurl=True)
        g("Todo", fg=p["searchbg"], bg=p["statusbg"], bold=True)
        # Tree-sitter capture links (Neovim's modern highlighter).
        for cap, grp in (("@keyword", "Keyword"), ("@type", "Type"),
                         ("@string", "String"), ("@number", "Number"),
                         ("@comment", "Comment"), ("@function", "Function"),
                         ("@constant", "Constant"), ("@operator", "Operator"),
                         ("@preproc", "PreProc")):
            lines.append("hi(%r, { link = %r })" % (cap, grp))
        w("neovim/lua/sleela/%s.lua" % tid, "\n".join(lines))

    # colors/ shim so :colorscheme sleela-<id> works the vim way too.
    for tid in THEMES:
        w("neovim/colors/sleela-%s.lua" % tid,
          'require("sleela.%s")\n' % tid)

    # filetype detection + load the shared vim syntax file.
    w("neovim/ftdetect/sleela.lua",
      'vim.filetype.add({ extension = { sleela = "sleela" } })\n')
    w("neovim/README.md", NEOVIM_README)


# ===========================================================================
# Helix — languages.toml (language def) + per-theme TOML files.
# ===========================================================================
def gen_helix():
    langs = (
        "# SLeeLa language definition for Helix. Place in ~/.config/helix/ or\n"
        "# merge into your existing languages.toml.\n"
        '[[language]]\n'
        'name = "sleela"\n'
        'scope = "source.sleela"\n'
        'file-types = ["sleela"]\n'
        'comment-tokens = ["//", "#"]\n'
        'block-comment-tokens = [{ start = "/*", end = "*/" }, { start = "<<", end = ">>" }]\n'
        'indent = { tab-width = 2, unit = "  " }\n'
    )
    w("helix/languages.toml", langs)

    for tid, p in THEMES.items():
        t = [
            '# SLeeLa %s theme for Helix.' % p["name"],
            '# Mirrors lib/linux-editors/vim/colors/sleela-%s.vim.' % tid,
            '"ui.background" = { bg = "bg" }',
            '"ui.text" = "fg"',
            '"ui.cursor" = { fg = "bg", bg = "keyword" }',
            '"ui.cursorline.primary" = { bg = "cursorline" }',
            '"ui.linenr" = "linenr"',
            '"ui.linenr.selected" = { fg = "keyword", modifiers = ["bold"] }',
            '"ui.selection" = { bg = "visual" }',
            '"ui.statusline" = { fg = "fg", bg = "statusbg", modifiers = ["bold"] }',
            '"ui.menu" = { fg = "fg", bg = "statusbg" }',
            '"ui.menu.selected" = { fg = "bg", bg = "keyword" }',
            '"ui.help" = { fg = "fg", bg = "statusbg" }',
            '"ui.popup" = { bg = "statusbg" }',
            '"ui.virtual.ruler" = { bg = "cursorline" }',
            '"ui.virtual.whitespace" = "linenr"',
            '"keyword" = { fg = "keyword", modifiers = ["bold"] }',
            '"type" = { fg = "type", modifiers = ["bold"] }',
            '"type.builtin" = { fg = "type", modifiers = ["bold"] }',
            '"string" = "string"',
            '"constant.numeric" = "number"',
            '"constant" = "constant"',
            '"constant.builtin" = "constant"',
            '"function" = { fg = "function", modifiers = ["bold"] }',
            '"function.builtin" = { fg = "function", modifiers = ["bold"] }',
            '"variable" = "fg"',
            '"operator" = "operator"',
            '"comment" = { fg = "comment", modifiers = ["italic"] }',
            '"keyword.directive" = "preproc"',
            '"special" = "special"',
            '"label" = "preproc"',
            '"diagnostic" = { underline = { color = "error", style = "curl" } }',
            '"error" = "error"',
            '"warning" = "string"',
            '"info" = "keyword"',
            '"markup.heading" = { fg = "keyword", modifiers = ["bold"] }',
            '',
            '[palette]',
            'bg = "%s"' % p["bg"],
            'fg = "%s"' % p["fg"],
            'cursorline = "%s"' % p["cursorline"],
            'linenr = "%s"' % p["linenr"],
            'visual = "%s"' % p["visual"],
            'statusbg = "%s"' % p["statusbg"],
            'keyword = "%s"' % p["keyword"],
            'type = "%s"' % p["type"],
            'string = "%s"' % p["string"],
            'number = "%s"' % p["number"],
            'constant = "%s"' % p["constant"],
            'function = "%s"' % p["function"],
            'comment = "%s"' % p["comment"],
            'operator = "%s"' % p["operator"],
            'preproc = "%s"' % p["preproc"],
            'special = "%s"' % p["special"],
            'error = "%s"' % p["error"],
        ]
        w("helix/themes/sleela-%s.toml" % tid, "\n".join(t))
    w("helix/README.md", HELIX_README)


# ===========================================================================
# nano — nanorc. nano maps colours to 8 named colours or (newer nano) to a
# "color #RRGGBB" 24-bit form. We emit a base syntax file using 24-bit hex
# (nano >= 5.0) plus one per-theme override file that repaints the same groups
# in that palette's hex.
# ===========================================================================
def _nano_syntax(p):
    kw = "|".join(KEYWORDS)
    ty = "|".join(TYPES)
    mk = "|".join(MARKERS)
    L = [
        '## SLeeLa syntax highlighting for nano (%s palette).' % p["name"],
        '## Requires nano >= 5.0 for 24-bit "color #RRGGBB" values.',
        'syntax sleela "\\.sleela$"',
        'comment "//"',
        '',
        '## keywords, types, literals',
        'color %s "\\<(%s)\\>"' % (fg(p["keyword"]), kw),
        'color %s "\\<(%s)\\>"' % (fg(p["type"]), ty),
        '## function-call heads',
        'color %s "[A-Za-z_][A-Za-z0-9_]*[[:space:]]*\\("' % fg(p["function"]),
        '## numbers',
        'color %s "\\<[0-9]+\\>"' % fg(p["number"]),
        '## strings (double- and single-quoted; inner double-quote escaped as \\")',
        'color %s "\\"(\\\\.|[^\\"\\\\])*\\""' % fg(p["string"]),
        "color %s \"'(\\\\.|[^'\\\\])*'\"" % fg(p["string"]),
        '## annotations and the document pragmas',
        'color %s "@[A-Za-z_][A-Za-z0-9_]*"' % fg(p["preproc"]),
        'color %s "^[[:space:]]*#(sleela|schema)\\>.*$"' % fg(p["preproc"]),
        '## document markers (only when alone on a line, optional ;)',
        'color %s "^[[:space:]]*(%s)[[:space:]]*;?[[:space:]]*$"' % (fg(p["preproc"]), mk),
        '## comments: // line and /* */ block, plus << >> and < > angle forms',
        'color %s "//.*$"' % fg(p["comment"]),
        'color %s start="/\\*" end="\\*/"' % fg(p["comment"]),
        'color %s start="^[[:space:]]*<<" end=">>"' % fg(p["comment"]),
        'color %s "^[[:space:]]*<[^<].*>?$"' % fg(p["comment"]),
        '## TODO/FIXME/NOTE/XXX inside comments',
        'color %s "\\<(TODO|FIXME|NOTE|XXX)\\>"' % fg(p["searchbg"]),
    ]
    return "\n".join(L)


def fg(hexv):
    # nano color spec: "FG" or "FG,BG"; a bare 24-bit value needs no quotes.
    return hexv.lower()


def gen_nano():
    for tid, p in THEMES.items():
        w("nano/sleela-%s.nanorc" % tid, _nano_syntax(p))
    # convenience: the default palette as plain sleela.nanorc
    w("nano/sleela.nanorc", _nano_syntax(THEMES[DEFAULT]))
    w("nano/README.md", NANO_README)


# ===========================================================================
# VS Code — a TextMate grammar + one color theme per palette + manifest.
# ===========================================================================
def gen_vscode():
    grammar = {
        "$schema": "https://raw.githubusercontent.com/martinring/tmlanguage/master/tmlanguage.json",
        "name": "SLeeLa",
        "scopeName": "source.sleela",
        "patterns": [
            {"include": "#comments"},
            {"include": "#pragmas"},
            {"include": "#markers"},
            {"include": "#strings"},
            {"include": "#numbers"},
            {"include": "#keywords"},
            {"include": "#types"},
            {"include": "#annotations"},
            {"include": "#functions"},
        ],
        "repository": {
            "comments": {"patterns": [
                {"name": "comment.line.double-slash.sleela", "match": "//.*$"},
                {"name": "comment.block.sleela", "begin": "/\\*", "end": "\\*/"},
                {"name": "comment.block.angle.sleela", "begin": "^\\s*<<", "end": ">>"},
                {"name": "comment.line.angle.sleela", "match": "^\\s*<[^<].*>?$"},
            ]},
            "pragmas": {"name": "keyword.control.directive.sleela",
                        "match": "^\\s*#(sleela|schema)\\b.*$"},
            "markers": {"name": "keyword.other.marker.sleela",
                        "match": "^\\s*(author|authorial|material|memory|determine)\\s*;?\\s*$"},
            "strings": {"patterns": [
                {"name": "string.quoted.double.sleela", "begin": "\"", "end": "\"",
                 "patterns": [{"name": "constant.character.escape.sleela", "match": "\\\\."}]},
                {"name": "string.quoted.single.sleela", "begin": "'", "end": "'",
                 "patterns": [{"name": "constant.character.escape.sleela", "match": "\\\\."}]},
            ]},
            "numbers": {"name": "constant.numeric.sleela", "match": "\\b[0-9]+\\b"},
            "keywords": {"name": "keyword.control.sleela",
                         "match": "\\b(" + "|".join(KEYWORDS) + ")\\b"},
            "types": {"name": "storage.type.sleela",
                      "match": "\\b(" + "|".join(TYPES) + ")\\b"},
            "annotations": {"name": "entity.other.attribute-name.sleela",
                            "match": "@[A-Za-z_][A-Za-z0-9_]*"},
            "functions": {"name": "entity.name.function.sleela",
                          "match": "\\b[A-Za-z_][A-Za-z0-9_]*(?=\\s*\\()"},
        },
    }
    w("vscode/syntaxes/sleela.tmLanguage.json",
      json.dumps(grammar, indent=2))

    themes_manifest = []
    for tid, p in THEMES.items():
        theme = {
            "name": p["name"],
            "type": "dark",
            "colors": {
                "editor.background": p["bg"],
                "editor.foreground": p["fg"],
                "editorCursor.foreground": p["keyword"],
                "editor.lineHighlightBackground": p["cursorline"],
                "editorLineNumber.foreground": p["linenr"],
                "editorLineNumber.activeForeground": p["keyword"],
                "editor.selectionBackground": p["visual"],
                "editor.findMatchBackground": p["searchbg"],
                "statusBar.background": p["statusbg"],
                "statusBar.foreground": p["fg"],
                "editorError.foreground": p["error"],
            },
            "tokenColors": [
                {"scope": ["keyword", "keyword.control.sleela", "storage.modifier"],
                 "settings": {"foreground": p["keyword"], "fontStyle": "bold"}},
                {"scope": ["storage.type.sleela", "support.type"],
                 "settings": {"foreground": p["type"], "fontStyle": "bold"}},
                {"scope": ["string", "string.quoted"],
                 "settings": {"foreground": p["string"]}},
                {"scope": ["constant.numeric.sleela"],
                 "settings": {"foreground": p["number"]}},
                {"scope": ["constant", "constant.language"],
                 "settings": {"foreground": p["constant"]}},
                {"scope": ["entity.name.function.sleela"],
                 "settings": {"foreground": p["function"], "fontStyle": "bold"}},
                {"scope": ["comment"],
                 "settings": {"foreground": p["comment"], "fontStyle": "italic"}},
                {"scope": ["keyword.control.directive.sleela",
                           "keyword.other.marker.sleela",
                           "entity.other.attribute-name.sleela"],
                 "settings": {"foreground": p["preproc"]}},
                {"scope": ["keyword.operator"],
                 "settings": {"foreground": p["operator"]}},
            ],
        }
        w("vscode/themes/sleela-%s-color-theme.json" % tid,
          json.dumps(theme, indent=2))
        themes_manifest.append({
            "label": p["name"],
            "uiTheme": "vs-dark",
            "path": "./themes/sleela-%s-color-theme.json" % tid,
        })

    pkg = {
        "name": "sleela",
        "displayName": "SLeeLa",
        "description": "SLeeLa language grammar and color themes (mirrors the vim/emacs palettes).",
        "version": "1.0.0",
        "engines": {"vscode": "^1.60.0"},
        "categories": ["Programming Languages", "Themes"],
        "contributes": {
            "languages": [{
                "id": "sleela",
                "aliases": ["SLeeLa", "sleela"],
                "extensions": [".sleela"],
                "configuration": "./language-configuration.json",
            }],
            "grammars": [{
                "language": "sleela",
                "scopeName": "source.sleela",
                "path": "./syntaxes/sleela.tmLanguage.json",
            }],
            "themes": themes_manifest,
        },
    }
    w("vscode/package.json", json.dumps(pkg, indent=2))
    langcfg = {
        "comments": {"lineComment": "//", "blockComment": ["/*", "*/"]},
        "brackets": [["{", "}"], ["[", "]"], ["(", ")"]],
        "autoClosingPairs": [["{", "}"], ["[", "]"], ["(", ")"],
                             ["\"", "\""], ["'", "'"]],
        "surroundingPairs": [["{", "}"], ["[", "]"], ["(", ")"],
                             ["\"", "\""], ["'", "'"]],
    }
    w("vscode/language-configuration.json", json.dumps(langcfg, indent=2))
    w("vscode/README.md", VSCODE_README)


NEOVIM_README = """# SLeeLa color schemes for Neovim

Native Lua colorschemes mirroring the vim/emacs palettes. Neovim reads the
shared Vim syntax file (`../vim/syntax/sleela.vim`); these supply the colours.

## Install

```sh
mkdir -p ~/.config/nvim/{lua/sleela,colors,ftdetect,syntax}
cp -r lib/linux-editors/neovim/lua/sleela/* ~/.config/nvim/lua/sleela/
cp lib/linux-editors/neovim/colors/*.lua     ~/.config/nvim/colors/
cp lib/linux-editors/neovim/ftdetect/*.lua   ~/.config/nvim/ftdetect/
cp lib/linux-editors/vim/syntax/sleela.vim   ~/.config/nvim/syntax/
```

Then in `init.lua`:

```lua
vim.cmd.colorscheme("sleela-american")
```

Themes: `sleela-american`, `sleela-eurasia`, `sleela-russian-slavic`,
`sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`.

Each theme also links the common Tree-sitter capture groups (`@keyword`,
`@type`, `@string`, …) so it works with `nvim-treesitter` highlighting as well
as the classic regex syntax.
"""

HELIX_README = """# SLeeLa color schemes for Helix

Helix is modal with built-in Tree-sitter/LSP; these files give it the SLeeLa
language definition and the SLeeLa palettes.

## Install

```sh
mkdir -p ~/.config/helix/themes
# merge this language block into your languages.toml (or copy if you have none)
cat lib/linux-editors/helix/languages.toml >> ~/.config/helix/languages.toml
cp lib/linux-editors/helix/themes/*.toml ~/.config/helix/themes/
```

Then in `~/.config/helix/config.toml`:

```toml
theme = "sleela-american"
```

Themes: `sleela-american`, `sleela-eurasia`, `sleela-russian-slavic`,
`sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`.

> Full token highlighting uses a `sleela` Tree-sitter grammar; the theme colours
> map standard Helix scopes (`keyword`, `type`, `string`, `comment`, `function`,
> `keyword.directive`, …) so they apply to any grammar Helix resolves for the
> language, and the palette is identical to the vim/emacs schemes.
"""

NANO_README = """# SLeeLa color schemes for nano

nano highlights via `nanorc` files. Each `sleela-<theme>.nanorc` carries the
full SLeeLa syntax rules painted in that palette's 24-bit colours (nano >= 5.0).

## Install

```sh
mkdir -p ~/.nano
cp lib/linux-editors/nano/sleela-american.nanorc ~/.nano/sleela.nanorc
echo 'include "~/.nano/sleela.nanorc"' >> ~/.nanorc
```

Swap `sleela-american.nanorc` for any of `sleela-eurasia`,
`sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`,
`sleela-greek`, or `sleela-turkish`.

> nano colours the *buffer* (syntax), not the whole UI chrome; the palette files
> use each scheme's keyword/type/string/comment/number hues so the code reads in
> the chosen scheme. On nano < 5.0 (no 24-bit colour), replace the `#RRGGBB`
> values with nano's named colours (e.g. `brightblue`, `cyan`, `yellow`).

## Ed / Sed

`ed` and `sed` are non-interactive line editors with **no** colour-scheme or
syntax-highlighting concept, so there is nothing to theme for them. Use them for
scripted `.sleela` edits; see the top-level `linux-editors/README.md`.
"""

VSCODE_README = """# SLeeLa grammar and color themes for VS Code

A minimal extension: a TextMate grammar for `.sleela` plus one color theme per
SLeeLa palette (mirroring the vim/emacs schemes).

## Install (from source, for local use)

```sh
mkdir -p ~/.vscode/extensions/sleela
cp -r lib/linux-editors/vscode/* ~/.vscode/extensions/sleela/
```

Restart VS Code, then pick a theme via **Preferences: Color Theme** — e.g.
*SLeeLa American Professional*, *SLeeLa Pan-Eurasian*, *SLeeLa Russian and
Slavic (Medium)*, *SLeeLa Central Asian / Silk Road*, *SLeeLa East Asian*,
*SLeeLa Greek*, or *SLeeLa Turkish*.

The grammar highlights keywords, types, strings, numbers, `@`-annotations, the
`#sleela`/`#schema` pragmas, the document markers (`author`/`authorial`/
`material`/`memory`/`determine`, matched only when alone on a line), and the
`//`, `/* */`, `<< >>`, and `< >` comment forms.

> For Vim-style modal editing in VS Code, pair this with a Vim-emulation
> extension; the grammar and themes are independent of the keymap.
"""


def main():
    gen_neovim()
    gen_helix()
    gen_nano()
    gen_vscode()
    print("done.")


if __name__ == "__main__":
    main()
