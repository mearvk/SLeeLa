#!/usr/bin/env sh
# SLeeLa bounded known-connection verifier.
# Active probing is opt-in and requires an explicit allowlist. This avoids turning
# an international relay into an unrestricted Internet port scanner.
set -eu
STATE=${SLEELA_SERVER_STATE:-server-edition/state}
OUT_DIR=${SLEELA_KNOWN_CONNECTIONS_DIR:-$STATE/known-connections}
ALLOWLIST=${SLEELA_PROBE_ALLOWLIST:-}
PORTS=${SLEELA_KNOWN_PORTS:-2222,22220,19866,20000}
ACTIVE=${SLEELA_ACTIVE_PROBING:-false}
TIMEOUT=${SLEELA_PROBE_TIMEOUT:-1}
STAMP=$(date -u +%Y-%m-%dT%H-%M-%SZ 2>/dev/null || echo now)
DAY=$(date -u +%Y-%m-%d 2>/dev/null || echo unknown-date)
TIME=$(date -u +%H-%M-%S 2>/dev/null || echo unknown-time)
TARGET="$OUT_DIR/$DAY/$TIME/known.connections"
mkdir -p "$(dirname "$TARGET")"
{
  printf 'schema=sleela.known.connections.v1 timestamp=%s\n' "$STAMP"
  printf 'privacy=ip-redacted national-data-redacted contact-data-redacted\n'
  printf 'active_probing=%s\n' "$ACTIVE"
  if [ "$ACTIVE" != true ]; then
    printf 'status=observation-only; active probing requires SLEELA_ACTIVE_PROBING=true and SLEELA_PROBE_ALLOWLIST\n'
    exit 0
  fi
  [ -n "$ALLOWLIST" ] || { printf 'status=disabled reason=empty-allowlist\n'; exit 0; }
  for host in $(printf '%s' "$ALLOWLIST" | tr ',' ' '); do
    case "$host" in
      *[!A-Za-z0-9:._-]*) printf 'host=[REDACTED] status=invalid\n'; continue ;;
    esac
    for port in $(printf '%s' "$PORTS" | tr ',' ' '); do
      case "$port" in ''|*[!0-9]*) continue;; esac
      if command -v nc >/dev/null 2>&1 && nc -z -w "$TIMEOUT" "$host" "$port" >/dev/null 2>&1; then
        status=open
      else
        status=closed-or-unreachable
      fi
      # Never write the probed IP/hostname into the retained record.
      printf 'endpoint=[REDACTED] port=%s status=%s\n' "$port" "$status"
    done
  done
} > "$TARGET"
