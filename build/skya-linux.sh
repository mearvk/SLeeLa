#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname "$0")/.." && pwd)
SRC="$ROOT/telephony-skya/native"
OUT="$ROOT/build/skya/linux"
CXX="${CXX:-g++}"
CXXFLAGS="${CXXFLAGS:--std=c++17 -O2 -Wall -Wextra -Wpedantic -pthread}"

command -v "$CXX" >/dev/null 2>&1 || {
    echo "ERROR: C++ compiler not found." >&2
    exit 1
}

mkdir -p "$OUT"
"$CXX" $CXXFLAGS "$SRC/main.cpp" "$SRC/skya_engine.cpp" -o "$OUT/skya"
chmod +x "$OUT/skya"

echo "Skya Linux binary: $OUT/skya"
echo ""

# Never block a build or CI job waiting for the server. Only offer the
# interactive launch choice when stdin is a terminal; non-interactive builds
# leave the server stopped.
if [ -t 0 ]; then
    printf "Start the Skya server now in the background? [y/N] "
    if IFS= read -r answer; then
        case "$answer" in
            y|Y|yes|YES|Yes)
                LOG_FILE="$OUT/skya.log"
                PID_FILE="$OUT/skya.pid"

                # Avoid launching a second copy when the recorded process
                # is still alive.
                if [ -f "$PID_FILE" ]; then
                    old_pid=$(cat "$PID_FILE")
                    case "$old_pid" in
                        ''|*[!0-9]*) old_pid= ;;
                    esac
                    if [ -n "$old_pid" ] && kill -0 "$old_pid" 2>/dev/null; then
                        echo "Skya is already running (PID $old_pid)."
                        exit 0
                    fi
                fi

                nohup "$OUT/skya" --both --room "${SKYA_ROOM:-lobby}" \
                    >"$LOG_FILE" 2>&1 </dev/null &
                skya_pid=$!
                printf '%s\n' "$skya_pid" >"$PID_FILE"
                echo "Skya started in the background (PID $skya_pid)."
                echo "Log: $LOG_FILE"
                echo "PID file: $PID_FILE"
                ;;
            *)
                echo "Skya was not started."
                ;;
        esac
    else
        echo "No response received; Skya was not started."
    fi
else
    echo "Non-interactive build: Skya was built but not started."
fi

echo "Skya build script finished."
exit 0
