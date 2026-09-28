#!/usr/bin/env python3
import pathlib
import re
ROOT = pathlib.Path(__file__).resolve().parents[1]
inventory = ROOT / "test-suites" / "FUNCTION.COVERAGE.md"
out = ROOT / "test-suites" / "generated"
out.mkdir(exist_ok=True)
if not inventory.exists():
    raise SystemExit("Run generate-function-coverage.py first.")
count = 0
for line in inventory.read_text(errors="ignore").splitlines():
    parts = [p.strip() for p in line.split("|")]
    if len(parts) < 5 or parts[0] or parts[-1]:
        continue
    status = parts[4].replace("*", "")
    fn = parts[3].replace("()", "").replace(chr(96), "")
    if status not in ("compile-only", "untested") or not fn:
        continue
    safe = re.sub(r"[^A-Za-z0-9_]+", "_", fn).strip("_").lower()
    target = out / (safe + ".test.cpp")
    if target.exists():
        continue
    target.write_text("// Generated behavioral-test review skeleton for " + fn + "\\n"
        "#include <cassert>\\n\\n"
        "int main() {\\n"
        "    // TODO normal behavior.\\n"
        "    // TODO boundary/empty behavior.\\n"
        "    // TODO invalid-input behavior.\\n"
        "    // TODO resource/error behavior.\\n"
        "    // TODO concurrency behavior.\\n"
        "    // TODO integration behavior.\\n"
        "    return 0;\\n"
        "}\\n")
    count += 1
print("Generated " + str(count) + " behavioral skeletons in " + str(out))
