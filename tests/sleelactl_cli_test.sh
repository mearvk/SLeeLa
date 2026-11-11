#!/usr/bin/env sh
# Functional regression tests for sleelactl 2.0: registry-driven dispatch (no
# module knowledge in the script), registry validation and use-time safety,
# honest registry verbs, exit codes, diagnostics, help topics, --dry-run/--yes,
# dependency-ordered fan-out, service-manager adapters (systemd/launchd/
# windows), --json output, script-driven tools, completion, and the docs table
# derived from the single nature table. Runs sleelactl from a throwaway tree
# with fake sleela/systemctl/launchctl/sc.exe, so it never touches the host.
# POSIX sh; needs python3 only to validate JSON and run fake tool drivers.
set -eu
REPO=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT HUP INT TERM

T="$TMP/root"
mkdir -p "$T/bin/sleelactl.d" "$T/lib/codecs" "$T/lib/db" "$T/proto/tools" "$T/decompiler/build" "$T/tools" "$TMP/fake"
cp "$REPO/bin/sleelactl" "$T/bin/sleelactl"; chmod +x "$T/bin/sleelactl"
CTL="$T/bin/sleelactl"

# `dep` is listed BEFORE `example` but depends on it: ordering must come from after=.
cat >"$T/bin/sleelactl.d/registry.conf" <<'EOF'
format=2
dep         nature=daemon      unit=dep.service  after=example  summary="Depends on example"
example     nature=daemon      unit=example.service  launchd=com.example.svc  winsvc=ExampleSvc  summary="Test daemon"
codecs      nature=registry    dir=lib/codecs  demo=lib/codecs/codec-pipeline.sleela  handlers=3  summary="Test codec registry"
db          nature=datasource  dir=lib/db  summary="Test data source"
proto       nature=protocol    dir=proto  sim=proto/tools/proto-sim  summary="Test protocol"
decompiler  nature=tool        dir=decompiler  bin=sleela-decompiler  summary="Test decompiler"
drv         nature=tool        script=tools/drv.py  runsub=pipe  sub.service=tools/svc.py  summary="Script tool"
math        nature=subject     demo=demo.sleela  summary="Test subject"
EOF
: >"$T/lib/codecs/codec-pipeline.sleela"; : >"$T/lib/codecs/SLA.sleela"; : >"$T/demo.sleela"
printf 'test:\n\t@echo ran-test\nsyntax:\n\t@echo ran-syntax\nvalidate:\n\t@echo ran-validate\n' >"$T/proto/Makefile"
printf '#!/bin/sh\necho sim-ran "$@"\n' >"$T/proto/tools/proto-sim"
printf '#!/bin/sh\necho WRONG-api-inspect\n' >"$T/decompiler/build/slecompiler-api-inspect"
printf '#!/bin/sh\necho RIGHT-decompiler "$@"\n' >"$T/decompiler/build/sleela-decompiler"
printf 'import sys\nprint("drv " + " ".join(sys.argv[1:]))\n' >"$T/tools/drv.py"
printf 'import sys\nprint("svc " + " ".join(sys.argv[1:]))\n' >"$T/tools/svc.py"
chmod +x "$T/proto/tools/proto-sim" "$T/decompiler/build/"*

cat >"$TMP/fake/sleela" <<'EOF'
#!/bin/sh
printf '%s\n' "$*" >>"$SLEELA_LOG"
echo "verification: PASS (1 item(s))" >&2
if [ "${FAKE_SLEELA_FAIL:-0}" = 1 ]; then echo "boom: real pipeline error" >&2; exit 1; fi
echo "pipeline-ran"
EOF
for tool in systemctl launchctl sc.exe; do
  printf '#!/bin/sh\nprintf "%%s\\n" "%s $*" >>"$SCTL_LOG"\n' "$tool" >"$TMP/fake/$tool"
done
printf '#!/bin/sh\necho 0\n' >"$TMP/fake/id"
chmod +x "$TMP/fake/"*

export PATH="$TMP/fake:$PATH" SLEELA_BIN="$TMP/fake/sleela" SLEELA_LOG="$TMP/sleela.log" SCTL_LOG="$TMP/svc.log" NO_COLOR=1
export SLEELACTL_SERVICE_MANAGER=systemd
unset SLEELACTL_REGISTRY SLEELACTL_SYSTEMCTL_SCOPE 2>/dev/null || true
: >"$SLEELA_LOG"; : >"$SCTL_LOG"

fails=0
fail()  { echo "FAIL: $*"; fails=$((fails + 1)); }
check() { # description expected-rc command...
  desc=$1; want=$2; shift 2
  set +e; "$@" >"$TMP/out" 2>"$TMP/err"; got=$?; set -e
  [ "$got" -eq "$want" ] || { fail "$desc (rc=$got, want $want)"; sed 's/^/    | /' "$TMP/out" "$TMP/err"; }
}
has()   { grep -F -- "$2" "$TMP/$1" >/dev/null || fail "expected '$2' in $1"; }
hasnt() { if grep -F -- "$2" "$TMP/$1" >/dev/null; then fail "unexpected '$2' in $1"; fi; }
nlines() { wc -l <"$1" | tr -d ' '; }
json_ok() { python3 -c 'import json,sys; json.load(open(sys.argv[1]))' "$TMP/out" || fail "invalid JSON from: $1"; }

# --- registry verbs: list/probe/status only; mutations are in-program API -------
check "load is not a CLI verb" 64 "$CTL" load codecs 2
has err "in-program Controller API"
check "list <registry> shows declared handlers" 0 "$CTL" list codecs
has out "handlers: 3 (ids 0..2)"
[ "$(nlines "$SLEELA_LOG")" -eq 0 ] || fail "list/load ran the pipeline"
check "probe runs reference pipeline" 0 "$CTL" probe codecs
has out "pipeline-ran"; hasnt err "verification: PASS"
check "probe rejects arguments" 2 "$CTL" probe codecs 1
check "probe failure is non-zero with diagnostics" 1 env FAKE_SLEELA_FAIL=1 "$CTL" probe codecs
has err "boom: real pipeline error"; hasnt err "verification: PASS"
check "subject failure is non-zero" 1 env FAKE_SLEELA_FAIL=1 "$CTL" run math
check "status all non-zero when a module fails" 1 env FAKE_SLEELA_FAIL=1 "$CTL" status all

# --- --json -------------------------------------------------------------------
check "json list" 0 "$CTL" --json list; json_ok "list"; has out '"handlers":"3"'
check "json status all" 1 env FAKE_SLEELA_FAIL=1 "$CTL" --json status all; json_ok "status"; has out '"state":"failed"'
check "json natures" 0 "$CTL" --json natures; json_ok "natures"
check "json where" 0 "$CTL" --json where; json_ok "where"; has out '"registry_source":"beside"'
check "json rejected for other verbs" 2 "$CTL" --json probe codecs

# --- help topics, exit codes, option errors ------------------------------------
check "help <verb>" 0 "$CTL" help start; has out "a verb accepted by: daemon"
check "help <nature>" 0 "$CTL" help registry; has out "codecs"
check "help <module>" 0 "$CTL" help codecs
check "help in-program call" 0 "$CTL" help unload; has out "not a CLI verb"
check "help unknown topic" 2 "$CTL" help nosuchthing
check "help documents exit codes" 0 "$CTL" help; has out "Exit codes"
check "unknown module" 2 "$CTL" start nope
check "verb without module" 2 "$CTL" start
check "illegal verb for nature" 64 "$CTL" start math
check "unknown global option" 2 "$CTL" --bogus list

# --- fan-out safety and dependency order ----------------------------------------
: >"$SCTL_LOG"
check "stop all without --yes refused" 2 "$CTL" stop all
check "dry-run stop all" 0 "$CTL" --dry-run stop all
[ "$(nlines "$SCTL_LOG")" -eq 0 ] || fail "service manager called in dry-run / without --yes"
stop_order=$(grep -n "would run" "$TMP/out" | sed 's/.* stop //' | tr '\n' ' ')
[ "$stop_order" = "dep.service example.service " ] || fail "stop order should be dependents first, got: $stop_order"
check "start all --yes" 0 "$CTL" start all --yes
start_order=$(sed 's/.* start //' "$SCTL_LOG" | tr '\n' ' ')
[ "$start_order" = "example.service dep.service " ] || fail "start order should honour after=, got: $start_order"
check "single-module dry-run" 0 "$CTL" --dry-run restart example

# --- service-manager adapters -----------------------------------------------------
: >"$SCTL_LOG"
check "launchd start (user domain)" 0 env SLEELACTL_SERVICE_MANAGER=launchd SLEELACTL_SYSTEMCTL_SCOPE=user "$CTL" start example
grep -Fx "launchctl kickstart gui/0/com.example.svc" "$SCTL_LOG" >/dev/null || fail "launchd start argv"
check "launchd module without a label" 3 env SLEELACTL_SERVICE_MANAGER=launchd "$CTL" start dep
check "launchd reload unsupported" 3 env SLEELACTL_SERVICE_MANAGER=launchd "$CTL" reload example
check "windows enable" 0 env SLEELACTL_SERVICE_MANAGER=windows "$CTL" enable example
grep -Fx "sc.exe config ExampleSvc start= auto" "$SCTL_LOG" >/dev/null || fail "windows enable argv"
: >"$SCTL_LOG"
check "windows restart = stop + start" 0 env SLEELACTL_SERVICE_MANAGER=windows "$CTL" restart example
[ "$(sed 's/sc.exe //' "$SCTL_LOG" | tr '\n' ' ')" = "stop ExampleSvc start ExampleSvc " ] || fail "windows restart sequence"
check "no service manager" 3 env SLEELACTL_SERVICE_MANAGER=none "$CTL" start example
check "invalid service manager" 2 env SLEELACTL_SERVICE_MANAGER=bogus "$CTL" start example

# --- input validation -----------------------------------------------------------
check "list unknown nature" 2 "$CTL" list nosuchnature
check "list known nature" 0 "$CTL" list registry
check "missing registry override fails" 2 env SLEELACTL_REGISTRY="$TMP/missing.conf" "$CTL" list
has err "module registry not found"

# --- protocol / tool resolution ---------------------------------------------------
check "validate prefers validate target" 0 "$CTL" validate proto; has out "ran-validate"; hasnt out "ran-test"
check "simulate uses registered sim" 0 "$CTL" simulate proto --x; has out "sim-ran --x"
check "run decompiler resolves bin= by name" 0 "$CTL" run decompiler --version
has out "RIGHT-decompiler --version"; hasnt out "WRONG"
check "script tool build forwards" 0 "$CTL" build drv a.c -o a; has out "drv build a.c -o a"
check "script tool runsub" 0 "$CTL" run drv pipe x; has out "drv pipe x"
check "script tool run" 0 "$CTL" run drv -- ./a; has out "drv run -- ./a"
check "script tool inspect sub.<word>" 0 "$CTL" inspect drv service --status; has out "svc --status"
check "script tool inspect other" 0 "$CTL" inspect drv xclass f; has out "drv xclass f"
rm "$T/decompiler/build/sleela-decompiler"
check "run decompiler without its CLI" 3 "$CTL" run decompiler; hasnt out "WRONG"

# --- check-registry and use-time safety -------------------------------------------
check "check-registry strict on a good registry" 0 "$CTL" check-registry --strict
cat >"$TMP/bad.conf" <<'EOF'
format=2
a      nature=daemon   unit=a.service  after=b
b      nature=daemon   unit=b.service  after=a
a      nature=daemon   unit=dup.service
weird  nature=bogus
esc    nature=protocol dir=../../etc
inj    nature=daemon   unit=foo;bar
lost   nature=daemon   unit=l.service  after=ghost
keyed  nature=subject  demo=demo.sleela  color=blue
nosvc  nature=daemon   summary="no service mapping"
badbin nature=tool     dir=x  bin="a b"
badlbl nature=daemon   launchd=bad/label
quote  nature=tool     dir=x  summary="unterminated
EOF
check "check-registry finds errors" 1 env SLEELACTL_REGISTRY="$TMP/bad.conf" "$CTL" check-registry
for msg in 'duplicate module "a"' 'unknown nature "bogus"' 'unsafe dir path "../../etc"' 'invalid systemd unit "foo;bar"' \
           'after= names unknown module "ghost"' 'dependency cycle' 'key "color" is not valid' 'unterminated quote' \
           'nosvc: needs one of unit=, launchd=, winsvc=' 'badbin: bin must be a comma list' 'invalid launchd name'; do
  has err "$msg"
done
: >"$SCTL_LOG"
check "unsafe path refused at use time" 2 env SLEELACTL_REGISTRY="$TMP/bad.conf" "$CTL" info esc
check "invalid unit refused at use time" 2 env SLEELACTL_REGISTRY="$TMP/bad.conf" "$CTL" --yes start inj
[ "$(nlines "$SCTL_LOG")" -eq 0 ] || fail "invalid unit reached the service manager"

# --- completion, docs table, where ------------------------------------------------
check "bash completion" 0 "$CTL" completion bash; has out "complete -F _sleelactl sleelactl"; has out "codecs"
if command -v bash >/dev/null 2>&1; then bash -n "$TMP/out" || fail "bash completion does not parse"; fi
check "zsh completion" 0 "$CTL" completion zsh; has out "bashcompinit"
check "completion needs a shell" 2 "$CTL" completion fish
"$REPO/bin/sleelactl" natures --markdown >"$TMP/gen.md"
awk '/<!-- natures:begin -->/{f=1;next} /<!-- natures:end -->/{f=0} f' "$REPO/bin/SLEELACTL.md" >"$TMP/doc.md"
cmp -s "$TMP/gen.md" "$TMP/doc.md" || { fail "bin/SLEELACTL.md natures table is out of date (regenerate: sleelactl natures --markdown)"; diff "$TMP/doc.md" "$TMP/gen.md" || true; }
"$REPO/bin/sleelactl" keys --markdown >"$TMP/gen.md"
awk '/<!-- keys:begin -->/{f=1;next} /<!-- keys:end -->/{f=0} f' "$REPO/bin/SLEELACTL.md" >"$TMP/doc.md"
cmp -s "$TMP/gen.md" "$TMP/doc.md" || { fail "bin/SLEELACTL.md keys table is out of date (regenerate: sleelactl keys --markdown)"; diff "$TMP/doc.md" "$TMP/gen.md" || true; }
check "json keys" 0 "$CTL" --json keys; json_ok "keys"
check "where" 0 "$CTL" where; has out "service manager:"

if [ "$fails" -ne 0 ]; then echo "sleelactl CLI tests: $fails FAILURE(S)"; exit 1; fi
echo "sleelactl CLI tests: PASS"
