#!/usr/bin/env sh
# =============================================================================
# install-modules.sh -- iteratively install (or build) the SLeeLa modules,
# paced over a modest window (default ~10-20 minutes).
#
# Lives in bin/. Driven by bin/Makefile.modules and the bin/install-modules
# launcher, but runnable directly. It derives the repo root from its own
# location (bin/..) and cd's there, so every `make -C <module>` and the build/
# state files resolve correctly regardless of the caller's cwd. For each module
# in order it runs the module's own
# `install` target when it has one, else `all` (build). It is resilient: a
# module that fails or lacks its OS dependencies is logged and SKIPPED, never
# fatal -- the walk continues. Progress, a resumable state file, and a final
# summary are printed.
#
# Pacing: the total wall-clock is spread across TARGET_SECONDS by giving each
# module a time budget; if a module's build finishes early, the remainder of
# its budget is slept, so the run lands in the intended window instead of
# hammering the machine. Set PACE=off to disable the sleeps entirely.
#
# Environment (all optional):
#   TARGET_SECONDS   total target wall-clock (default 900 = 15 min; clamped 600..1200)
#   PACE             on|off   (default on) -- whether to sleep to hit the window
#   RESUME           on|off   (default on) -- skip modules already marked done
#   PREFIX, DESTDIR  passed through to each module's install/all
#   MODULES          override the module list (space-separated dir names)
#   STATE_FILE       progress file (default build/module-install.state)
#   LOG_FILE         log file     (default build/module-install.log)
# =============================================================================
set -u

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
cd "$ROOT" || exit 1

TARGET_SECONDS=${TARGET_SECONDS:-900}
PACE=${PACE:-on}
RESUME=${RESUME:-on}
PREFIX=${PREFIX:-/usr/local}
DESTDIR=${DESTDIR:-}
STATE_FILE=${STATE_FILE:-build/module-install.state}
LOG_FILE=${LOG_FILE:-build/module-install.log}

# Clamp the target into the modest 10..20 minute window.
[ "$TARGET_SECONDS" -lt 600 ] 2>/dev/null && TARGET_SECONDS=600
[ "$TARGET_SECONDS" -gt 1200 ] 2>/dev/null && TARGET_SECONDS=1200

# Default module order: foundational / lightweight first, heavier native and
# OS-linking modules later. Each name is a top-level dir with a Makefile.
DEFAULT_MODULES="\
config common-rails resolver preferred-routers logger ledger registry \
regex crypto compression codecs \
compiler decompiler debugger \
idea os drivers native \
wifi-common ofdma \
http-1.0 http-2.0 http-3.0 http-4.0 http-5.0 http-6.0 http-7.0 http-8.0 http-9.0 \
autocad churn website-generator linux-editors \
user-interface ballum video \
sleela-scripting sleela-terminal server-edition \
antivirus connector"

MODULES=${MODULES:-$DEFAULT_MODULES}

mkdir -p build
: > "$LOG_FILE"
[ "$RESUME" = on ] || : > "$STATE_FILE"
[ -f "$STATE_FILE" ] || : > "$STATE_FILE"

log() { printf '%s\n' "$*" | tee -a "$LOG_FILE"; }

already_done() { grep -qx "done $1" "$STATE_FILE" 2>/dev/null; }
mark()         { printf '%s %s\n' "$1" "$2" >> "$STATE_FILE"; }

now() { date +%s 2>/dev/null || echo 0; }

# Count the modules we will actually attempt (exist + not already done).
total=0; pending=0
for m in $MODULES; do
    [ -f "$m/Makefile" ] || continue
    total=$((total+1))
    if [ "$RESUME" = on ] && already_done "$m"; then continue; fi
    pending=$((pending+1))
done
[ "$pending" -gt 0 ] || pending=1

# Per-module time budget so the whole pending set spans TARGET_SECONDS.
budget=$((TARGET_SECONDS / pending))
[ "$budget" -lt 1 ] && budget=1

log "=== SLeeLa iterative module install ==="
log "modules: $total total, $pending to do this run"
log "pace: $PACE   target window: ${TARGET_SECONDS}s (~$((TARGET_SECONDS/60)) min)   budget/module: ${budget}s"
log "prefix: $PREFIX   destdir: ${DESTDIR:-<none>}   resume: $RESUME"
log "state: $STATE_FILE   log: $LOG_FILE"
log ""

installed=0; built=0; skipped=0; missing=0; n=0
run_start=$(now)

for m in $MODULES; do
    if [ ! -f "$m/Makefile" ]; then
        log "  --  $m: no Makefile, skipping"
        missing=$((missing+1)); continue
    fi
    if [ "$RESUME" = on ] && already_done "$m"; then
        log "  ==  $m: already done (resume), skipping"
        continue
    fi

    n=$((n+1))
    step_start=$(now)
    # Choose the best target: install if the module defines one, else all.
    if grep -qE '^install:' "$m/Makefile"; then target=install; else target=all; fi
    log "[$n/$pending] $m: make $target ..."

    if make -C "$m" "$target" PREFIX="$PREFIX" DESTDIR="$DESTDIR" >>"$LOG_FILE" 2>&1; then
        if [ "$target" = install ]; then
            log "        OK (installed)"; installed=$((installed+1))
        else
            log "        OK (built)"; built=$((built+1))
        fi
        mark done "$m"
    else
        log "        SKIP ($m: '$target' failed -- see $LOG_FILE; likely missing OS deps)"
        skipped=$((skipped+1))
        mark skipped "$m"
    fi

    # Pace: sleep the remainder of this module's time budget.
    if [ "$PACE" = on ]; then
        step_end=$(now)
        elapsed=$((step_end - step_start))
        remain=$((budget - elapsed))
        if [ "$remain" -gt 0 ] && [ "$n" -lt "$pending" ]; then
            log "        (pacing ${remain}s)"
            sleep "$remain"
        fi
    fi
done

run_end=$(now)
total_elapsed=$((run_end - run_start))

log ""
log "=== summary ==="
log "  installed: $installed   built: $built   skipped: $skipped   missing: $missing"
log "  wall-clock: ${total_elapsed}s (~$((total_elapsed/60)) min)"
log "  state file: $STATE_FILE (delete it, or RESUME=off, to start over)"
[ "$skipped" -eq 0 ] && log "  all attempted modules succeeded." || log "  some modules were skipped; re-run to retry just those (RESUME=on)."
exit 0
