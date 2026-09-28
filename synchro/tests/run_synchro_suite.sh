#!/usr/bin/env bash
set -euo pipefail
ROOT="${BASH_SOURCE[0]}"
ROOT="$(cd "$(dirname "${ROOT}")/.." && pwd)"
WORK="${ROOT}/tests/.build"
CC="${CC:-cc}"; CXX="${CXX:-c++}"; JAVAC="${JAVAC:-javac}"; JAVA="${JAVA:-java}"
rm -rf "${WORK}"; mkdir -p "${WORK}"
echo "=== Synchro C integration smoke ==="
"${CC}" -std=c11 -Wall -Wextra -Werror -I"${ROOT}/c" "${ROOT}/c/synchro.c" "${ROOT}/c/synchro_integration.c" "${ROOT}/tests/integration_smoke.c" -lm -o "${WORK}/c_smoke"
"${WORK}/c_smoke"
echo "=== Synchro C protocol/negative suite ==="
"${CC}" -std=c11 -Wall -Wextra -Werror -I"${ROOT}/c" "${ROOT}/c/synchro.c" "${ROOT}/c/synchro_integration.c" "${ROOT}/tests/protocol_negative.c" -lm -o "${WORK}/c_protocol"
"${WORK}/c_protocol"
echo "=== Synchro C++ runtime suite ==="
"${CXX}" -std=c++17 -Wall -Wextra -Werror -I"${ROOT}/c" -I"${ROOT}/cpp" "${ROOT}/c/synchro.c" "${ROOT}/c/synchro_integration.c" "${ROOT}/cpp/synchro_integration.cpp" "${ROOT}/cpp/synchro_integration_test.cpp" -lm -o "${WORK}/cpp_integration"
"${WORK}/cpp_integration"
echo "=== Synchro Java integration suite ==="
rm -rf "${WORK}/java"; mkdir -p "${WORK}/java"
"${JAVAC}" -d "${WORK}/java" "${ROOT}"/java/src/com/mearvk/sleela/synchro/*.java
"${JAVA}" -cp "${WORK}/java" com.mearvk.sleela.synchro.SynchroIntegrationTest
echo "=== Synchro suite: PASS ==="
