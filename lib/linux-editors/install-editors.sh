#!/bin/sh
# SLeeLa editor integration installer.
# User-local only: never invokes sudo and never edits an editor's main config.
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
HOME_DIR=${HOME:?HOME must be set}
XDG_CONFIG_HOME=${XDG_CONFIG_HOME:-"$HOME_DIR/.config"}
XDG_DATA_HOME=${XDG_DATA_HOME:-"$HOME_DIR/.local/share"}
STATE_DIR="$XDG_DATA_HOME/sleela/editor-installer"
BACKUP_DIR="$STATE_DIR/backups"
RECORD="$STATE_DIR/installed-files"
DRY_RUN=0
ACTION=install
EDITORS=${EDITORS:-}

usage() {
    cat <<'USAGE'
Usage: install-editors.sh [--dry-run] [--uninstall] [--editor LIST]
  --dry-run       Show actions without changing files.
  --uninstall     Restore pre-existing files and remove installed files.
  --editor LIST   Comma-separated: vim,neovim,emacs,vscode,helix,nano
Environment:
  EDITORS         Same comma-separated editor selection.
  HOME            User home directory (required).
  XDG_CONFIG_HOME / XDG_DATA_HOME  Standard XDG overrides.
USAGE
}
while [ "$#" -gt 0 ]; do
    case "$1" in
        --dry-run) DRY_RUN=1 ;;
        --uninstall) ACTION=uninstall ;;
        --editor)
            [ "$#" -ge 2 ] || { echo "--editor requires a list" >&2; exit 2; }
            EDITORS=$2; shift ;;
        --help|-h) usage; exit 0 ;;
        *) echo "Unknown option: $1" >&2; usage >&2; exit 2 ;;
    esac
    shift
done

say() { printf '%s\n' "$*"; }
run() {
    if [ "$DRY_RUN" -eq 1 ]; then
        say "DRY-RUN: $*"
    else
        "$@"
    fi
}
selected() {
    name=$1
    [ -z "$EDITORS" ] && return 0
    old_ifs=$IFS; IFS=,
    set -- $EDITORS
    IFS=$old_ifs
    for item do [ "$item" = "$name" ] && return 0; done
    return 1
}
available() {
    case "$1" in
        vim) command -v vim >/dev/null 2>&1 ;;
        neovim) command -v nvim >/dev/null 2>&1 ;;
        emacs) command -v emacs >/dev/null 2>&1 ;;
        vscode) command -v code >/dev/null 2>&1 ;;
        helix) command -v hx >/dev/null 2>&1 ;;
        nano) command -v nano >/dev/null 2>&1 ;;
    esac
}
record_file() {
    dest=$1
    backup=$2
    if [ "$DRY_RUN" -eq 1 ]; then return; fi
    mkdir -p "$STATE_DIR"
    # One record per destination; preserve the first pre-install backup.
    if ! grep -F -x "$dest" "$RECORD" >/dev/null 2>&1; then
        printf '%s\n' "$dest" >> "$RECORD"
    fi
    if [ -n "$backup" ] && [ ! -f "$BACKUP_DIR$dest" ]; then
        mkdir -p "$(dirname "$BACKUP_DIR$dest")"
        cp -p "$backup" "$BACKUP_DIR$dest"
    fi
}
install_file() {
    src=$1; dest=$2
    [ -f "$src" ] || { say "Skipping missing source: $src"; return; }
    if [ -f "$dest" ] && cmp -s "$src" "$dest"; then
        say "Current: $dest"; return
    fi
    backup=
    if [ -e "$dest" ]; then backup=$dest; fi
    say "Install: $src -> $dest"
    if [ "$DRY_RUN" -eq 0 ]; then
        record_file "$dest" "$backup"
        mkdir -p "$(dirname "$dest")"
        cp -p "$src" "$dest"
        mkdir -p "$(dirname "$STATE_DIR/installed$dest")"
        cp -p "$src" "$STATE_DIR/installed$dest"
    fi
}
install_tree() {
    srcroot=$1; destroot=$2
    [ -d "$srcroot" ] || return
    find "$srcroot" -type f ! -name README.md -print |
    while IFS= read -r src; do
        rel=${src#"$srcroot"/}
        install_file "$src" "$destroot/$rel"
    done
}
uninstall_all() {
    if [ ! -f "$RECORD" ]; then
        say "No SLeeLa editor files are recorded for removal."
        return
    fi
    remaining="$RECORD.tmp"
    if [ "$DRY_RUN" -eq 0 ]; then : > "$remaining"; fi
    while IFS= read -r dest; do
        [ -n "$dest" ] || continue
        backup="$BACKUP_DIR$dest"
        installed_copy="$STATE_DIR/installed$dest"
        if [ -f "$dest" ] && [ -f "$installed_copy" ] && ! cmp -s "$dest" "$installed_copy"; then
            say "Preserve locally modified file: $dest"
            printf '%s\\n' "$dest" >> "$remaining"
            continue
        fi
        if [ -f "$backup" ]; then
            say "Restore: $dest"
            if [ "$DRY_RUN" -eq 0 ]; then
                mkdir -p "$(dirname "$dest")"
                cp -p "$backup" "$dest"
                rm -f "$backup"
            fi
        else
            say "Remove: $dest"
            if [ "$DRY_RUN" -eq 0 ]; then rm -f "$dest"; fi
        fi
        if [ "$DRY_RUN" -eq 0 ]; then rm -f "$installed_copy"; fi
    done < "$RECORD"
    if [ "$DRY_RUN" -eq 0 ]; then
        if [ -s "$remaining" ]; then mv "$remaining" "$RECORD"; else rm -f "$remaining" "$RECORD"; fi
        # Remove only now-empty directories inside the dedicated state tree.
        if [ -d "$BACKUP_DIR" ]; then
            find "$BACKUP_DIR" -depth -type d -exec rmdir {} \\; 2>/dev/null || :
        fi
        rmdir "$STATE_DIR" 2>/dev/null || :
    fi
}
if [ "$ACTION" = uninstall ]; then
    uninstall_all
    exit 0
fi

found=0
for editor in vim neovim emacs vscode helix nano; do
    selected "$editor" || continue
    if [ -z "$EDITORS" ] && ! available "$editor"; then continue; fi
    found=1
    case "$editor" in
        vim) install_tree "$SCRIPT_DIR/vim" "$HOME_DIR/.vim" ;;
        neovim) install_tree "$SCRIPT_DIR/neovim" "$XDG_CONFIG_HOME/nvim" ;;
        emacs) install_tree "$SCRIPT_DIR/emacs" "$HOME_DIR/.emacs.d/sleela" ;;
        vscode) install_tree "$SCRIPT_DIR/vscode" "$HOME_DIR/.vscode/extensions/mearvk.sleela-1.0.0" ;;
        helix) install_tree "$SCRIPT_DIR/helix" "$XDG_CONFIG_HOME/helix" ;;
        nano) install_tree "$SCRIPT_DIR/nano" "$XDG_CONFIG_HOME/nano/sleela" ;;
    esac
done
if [ "$found" -eq 0 ]; then
    say "No selected editor detected. Set EDITORS=vim,neovim,emacs,vscode,helix,nano to install explicitly."
fi
say "SLeeLa editor integration install complete."
