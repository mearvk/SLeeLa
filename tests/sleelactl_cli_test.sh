#!/usr/bin/env sh
# Functional regression tests for sleelactl's honesty and safety contract:
# registry verbs, exit codes, diagnostics, help topics, --dry-run/--yes,
# input validation, validate-target preference, and tool binary resolution.
# Runs sleelactl from a throwaway tree with fake sleela/systemctl/tools, so it
# never touches the host's services or needs a built runtime. POSIX sh.
set -eu
REPO=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT HUP INT TERM

T="$TMP/root"
mkdir -p "$T/bin/sleelactl.d" "$T/lib/codecs" "$T/proto" "$T/decompiler/build" "$TMP/fake"
cp "$REPO/bin/sleelactl" "$T/bin/sleelactl"
chmod +x "$T/bin/sleelactl"
CTL="$T/bin/sleelactl"

cat >"$T/bin/sleelactl.d/registry.conf" <<'EOF'
# test registry
example     daemon      example.service   Test daemon
codecs      registry    codecs            Test codec registry (3 handlers; load/unload/probe)
db          datasource  db                Test data source
proto       protocol    proto             Test protocol
decompiler  tool        decompiler        Test decompiler
math        subject     demo.sleela       Test subject
EOF
: >"$T/lib/codecs/codec-pipeline.sleela"
: >"$T/demo.sleela"
mkdir -p "$T/lib/db"

# Protocol Makefile lists test/syntax BEFORE validate: validate must still win.
printf 'test:\n\t@echo ran-test\nsyntax:\n\t@echo ran-syntax\nvalidate:\n\t@echo ran-validate\n' >"$T/proto/Makefile"

# Decompiler build dir: an example binary sorts before the real CLI.
printf '#!/bin/sh\necho WRONG-api-inspect\n' >"$T/decompiler/build/slecompiler-api-inspect"
printf '#!/bin/sh\necho RIGHT-decompiler "$@"\n' >"$T/decompiler/build/sleela-decompiler"
chmod +x "$T/decompiler/build/"*

# Fake sleela: logs each invocation; FAKE_SLEELA_FAIL=1 makes it fail loudly.
cat >"$TMP/fake/sleela" <<'EOF'
#!/bin/sh
printf '%s\n' "$*" >>"$SLEELA_LOG"
echo "verification: PASS (1 item(s))" >&2
if [ "${FAKE_SLEELA_FAIL:-0}" = 1 ]; then echo "boom: real pipeline error" >&2; exit 1; fi
echo "pipeline-ran"
EOF
cat >"$TMP/fake/systemctl" <<'EOF'
#!/bin/sh
printf '%s\n' "$*" >>"$SCTL_LOG"
EOF
cat >"$TMP/fake/id" <<'EOF'
#!/bin/sh
echo 0
EOF
chmod +x "$TMP/fake/"*

export PATH="$TMP/fake:$PATH"
export SLEELA_BIN="$TMP/fake/sleela"
export SLEELA_LOG="$TMP/sleela.log"
export SCTL_LOG="$TMP/systemctl.log"
export NO_COLOR=1
unset SLEELACTL_REGISTRY 2>/dev/null || true
: >"$SLEELA_LOG"; : >"$SCTL_LOG"

fails=0
check() { # description expected-rc command...
  desc=$1; want=$2; shift 2
  set +e; "$@" >"$TMP/out" 2>"$TMP/err"; got=$?; set -e
  if [ "$got" -ne "$want" ]; then
    echo "FAIL: $desc (rc=$got, want $want)"; sed 's/^/    | /' "$TMP/out" "$TMP/err"; fails=$((fails + 1))
  fi
}
has()    { grep -F -- "$2" "$TMP/$1" >/dev/null || { echo "FAIL: expected '$2' in $1"; fails=$((fails + 1)); }; }
hasnt()  { if grep -F -- "$2" "$TMP/$1" >/dev/null; then echo "FAIL: unexpected '$2' in $1"; fails=$((fails + 1)); fi; }
lines()  { wc -l <"$1" | tr -d ' '; }

# 1. Registry mutations: validated, then refused honestly; never run the pipeline.
check "load valid id refused (process-local)" 3 "$CTL" load codecs 2
has err "not offered from the command line"
check "load out-of-range id" 2 "$CTL" load codecs 3
check "load non-integer id" 2 "$CTL" load codecs x
check "load missing id" 2 "$CTL" load codecs
check "can valid id refused" 3 "$CTL" can codecs 0
[ "$(lines "$SLEELA_LOG")" -eq 0 ] || { echo "FAIL: a registry mutation ran the pipeline"; fails=$((fails + 1)); }

# 2/3. probe runs the reference pipeline; failures propagate with diagnostics.
check "probe runs reference pipeline" 0 "$CTL" probe codecs
has out "pipeline-ran"
hasnt err "verification: PASS"
check "probe rejects an id" 2 "$CTL" probe codecs 1
check "probe failure is non-zero" 1 env FAKE_SLEELA_FAIL=1 "$CTL" probe codecs
has err "boom: real pipeline error"
hasnt err "verification: PASS"
check "subject run failure is non-zero" 1 env FAKE_SLEELA_FAIL=1 "$CTL" run math
has err "boom: real pipeline error"
check "status all non-zero when a module fails" 1 env FAKE_SLEELA_FAIL=1 "$CTL" status all

# 4. help topics and exit codes.
check "help <verb>" 0 "$CTL" help start
has out "a verb accepted by: daemon"
check "help <nature>" 0 "$CTL" help registry
has out "codecs"
check "help <module>" 0 "$CTL" help codecs
check "help unknown topic" 2 "$CTL" help nosuchthing
check "help documents exit codes" 0 "$CTL" help
has out "Exit codes"
check "unknown module" 2 "$CTL" start nope
check "verb without module" 2 "$CTL" start
check "illegal verb for nature" 64 "$CTL" start math
check "unknown global option" 2 "$CTL" --bogus list

# 5. Fan-out safety: mutating daemon verbs need --yes; --dry-run changes nothing.
: >"$SCTL_LOG"
check "stop all without --yes refused" 2 "$CTL" stop all
check "dry-run stop all" 0 "$CTL" --dry-run stop all
has out "[dry-run] would run: systemctl --system stop example.service"
check "dry-run after all" 0 "$CTL" stop all --dry-run
[ "$(lines "$SCTL_LOG")" -eq 0 ] || { echo "FAIL: systemctl called without --yes / in dry-run"; fails=$((fails + 1)); }
check "stop all --yes" 0 "$CTL" stop all --yes
grep -Fx -- '--system stop example.service' "$SCTL_LOG" >/dev/null || { echo "FAIL: --yes did not stop the daemon"; fails=$((fails + 1)); }
: >"$SLEELA_LOG"
check "enable all --yes skips process-local registries" 0 "$CTL" enable all --yes
has out "skipped: 'enable' on a process-local registry"
[ "$(lines "$SLEELA_LOG")" -eq 0 ] || { echo "FAIL: enable all ran a registry pipeline"; fails=$((fails + 1)); }
check "single-module dry-run" 0 "$CTL" --dry-run restart example

# 6. Input validation.
check "list unknown nature" 2 "$CTL" list nosuchnature
check "list known nature" 0 "$CTL" list registry
check "missing registry override fails" 2 env SLEELACTL_REGISTRY="$TMP/missing.conf" "$CTL" list
has err "module registry not found"

# 7. validate prefers the validate target; tools resolve their real CLI by name.
check "validate prefers validate target" 0 "$CTL" validate proto
has out "ran-validate"
hasnt out "ran-test"
check "run decompiler resolves real CLI" 0 "$CTL" run decompiler --version
has out "RIGHT-decompiler --version"
hasnt out "WRONG"
rm "$T/decompiler/build/sleela-decompiler"
check "run decompiler with no real CLI" 3 "$CTL" run decompiler
hasnt out "WRONG"

if [ "$fails" -ne 0 ]; then echo "sleelactl CLI tests: $fails FAILURE(S)"; exit 1; fi
echo "sleelactl CLI tests: PASS"
