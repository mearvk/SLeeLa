#!/usr/bin/env sh
set -eu

ACTION=${1:-}
EDITION=${2:-}
PORT=${3:-}
PROTO=${4:-tcp}

case "$ACTION" in open|close|status) ;; *) echo "usage: portctl.sh {open|close|status} EDITION PORT [tcp|udp]" >&2; exit 2;; esac
case "$EDITION" in Discord-1|Discord-2|Discord-3|basic-probe|basic-scan|participation) ;; *) echo "invalid Server Edition" >&2; exit 2;; esac
case "$PORT" in ''|*[!0-9]*) echo "invalid port" >&2; exit 2;; esac
[ "$PORT" -ge 1 ] && [ "$PORT" -le 65535 ] || { echo "port must be 1..65535" >&2; exit 2; }
case "$PROTO" in tcp|udp) ;; *) echo "protocol must be tcp or udp" >&2; exit 2;; esac

RULE="SLeeLa-$EDITION-$PROTO-$PORT"
OS="unknown"
VERSION="unknown"
FIREWALL="none"

if [ "$(uname -s 2>/dev/null || echo unknown)" = "Linux" ]; then
  OS="Linux"
  if [ -r /etc/os-release ]; then
    . /etc/os-release
    OS="${ID:-Linux}"
    VERSION="${VERSION_ID:-unknown}"
  fi
  if command -v ufw >/dev/null 2>&1; then FIREWALL="ufw"
  elif command -v firewall-cmd >/dev/null 2>&1; then FIREWALL="firewalld"
  fi
elif [ "$(uname -s 2>/dev/null || echo unknown)" = "Darwin" ]; then
  OS="macOS"
  VERSION=$(sw_vers -productVersion 2>/dev/null || echo unknown)
  command -v pfctl >/dev/null 2>&1 && FIREWALL="pf"
fi

as_root() {
  if [ "$(id -u)" -eq 0 ]; then "$@"; else sudo "$@"; fi
}

linux_remove() {
  if [ "$FIREWALL" = ufw ]; then
    as_root ufw delete allow in proto "$PROTO" from any to any port "$PORT" comment "$RULE" >/dev/null 2>&1 || true
  elif [ "$FIREWALL" = firewalld ]; then
    as_root firewall-cmd --remove-rich-rule="rule family=\"ipv4\" priority=1000 port port=\"$PORT\" protocol=\"$PROTO\" accept" --permanent >/dev/null 2>&1 || true
    as_root firewall-cmd --remove-rich-rule="rule family=\"ipv6\" priority=1000 port port=\"$PORT\" protocol=\"$PROTO\" accept" --permanent >/dev/null 2>&1 || true
    as_root firewall-cmd --reload >/dev/null 2>&1 || true
  fi
}

linux_open() {
  if [ "$FIREWALL" = ufw ]; then
    as_root ufw allow in proto "$PROTO" from any to any port "$PORT" comment "$RULE" >/dev/null
  elif [ "$FIREWALL" = firewalld ]; then
    as_root firewall-cmd --add-rich-rule="rule family=\"ipv4\" priority=1000 port port=\"$PORT\" protocol=\"$PROTO\" accept" --permanent >/dev/null
    as_root firewall-cmd --add-rich-rule="rule family=\"ipv6\" priority=1000 port port=\"$PORT\" protocol=\"$PROTO\" accept" --permanent >/dev/null
    as_root firewall-cmd --reload >/dev/null
  else
    echo "No supported Linux firewall controller (UFW or firewalld) found" >&2
    return 1
  fi
}

mac_pf_file="/tmp/sleela-${EDITION}-${PORT}-${PROTO}.pf"
mac_pf_open() {
  printf 'pass in proto %s to any port %s\n' "$PROTO" "$PORT" > "$mac_pf_file"
  as_root pfctl -a "com.mearvk.sleela/${EDITION}" -f "$mac_pf_file" >/dev/null
}
mac_pf_close() {
  as_root pfctl -a "com.mearvk.sleela/${EDITION}" -F rules >/dev/null 2>&1 || true
  rm -f "$mac_pf_file"
}

case "$ACTION:$OS:$FIREWALL" in
  open:Linux:ufw|open:Linux:firewalld) linux_remove; linux_open ;;
  close:Linux:ufw|close:Linux:firewalld) linux_remove ;;
  status:Linux:ufw) ufw status | grep -E "^\s*${PORT}/${PROTO}" || true ;;
  status:Linux:firewalld) firewall-cmd --list-ports 2>/dev/null | tr ' ' '\n' | grep -Fx "${PORT}/${PROTO}" || true ;;
  open:macOS:pf) mac_pf_close; mac_pf_open ;;
  close:macOS:pf) mac_pf_close ;;
  status:macOS:pf) pfctl -a "com.mearvk.sleela/${EDITION}" -sr 2>/dev/null || true ;;
  *) echo "Unsupported firewall platform: OS=$OS VERSION=$VERSION FIREWALL=$FIREWALL" >&2; exit 1 ;;
esac

printf 'SLeeLa portctl: action=%s edition=%s port=%s/%s os=%s version=%s firewall=%s rule=%s\n' "$ACTION" "$EDITION" "$PORT" "$PROTO" "$OS" "$VERSION" "$FIREWALL" "$RULE"
