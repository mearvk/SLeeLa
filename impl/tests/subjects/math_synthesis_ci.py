#!/usr/bin/env python3
"""Run the native Math subject through sleelactl and record accuracy + speed."""
from __future__ import annotations
import os, re, statistics, subprocess, sys, time
from datetime import datetime, timezone
from pathlib import Path
ROOT = Path(__file__).resolve().parents[3]
SLEELA = ROOT / "impl/build/sleela"
SLEELACTL = ROOT / "bin/sleelactl"
MATH_TEST = ROOT / "impl/tests/subjects/math_values.sleela"
REPORT = ROOT / "impl/tests/subjects/MATH_SYNTHESIS_RESULTS.md"
REPEATS = int(os.environ.get("MATH_BENCH_REPEATS", "7"))
TICK = chr(96)
def run(command):
    return subprocess.run(command, cwd=ROOT, text=True, capture_output=True, check=False)
def main():
    if not SLEELA.is_file() or not os.access(SLEELA, os.X_OK):
        raise SystemExit(f"Missing executable SLeeLa runtime: {SLEELA}")
    subject = run([str(SLEELACTL), "run", "math"])
    if subject.returncode:
        print(subject.stdout, end=""); print(subject.stderr, file=sys.stderr)
        raise SystemExit("sleelactl run math failed")
    source = MATH_TEST.read_text(encoding="utf-8")
    marker = re.search(r"EXPECTED_PASS=(\d+)", source)
    if not marker:
        raise SystemExit("math_values.sleela has no EXPECTED_PASS marker")
    expected = int(marker.group(1))
    durations, pass_counts = [], []
    for index in range(REPEATS):
        started = time.perf_counter()
        result = run([str(SLEELA), "run", str(MATH_TEST)])
        elapsed = time.perf_counter() - started
        if result.returncode:
            print(result.stdout, end=""); print(result.stderr, file=sys.stderr)
            raise SystemExit(f"math accuracy execution {index + 1} failed")
        failures = [x for x in result.stdout.splitlines() if x.startswith("FAIL ")]
        passes = [x for x in result.stdout.splitlines() if x.startswith("PASS ")]
        if failures:
            raise SystemExit("Math accuracy failures:\n" + "\n".join(failures))
        if len(passes) != expected:
            raise SystemExit(f"Expected {expected} assertions, got {len(passes)} on run {index + 1}")
        durations.append(elapsed); pass_counts.append(len(passes))
    mean, median = statistics.mean(durations), statistics.median(durations)
    total = expected * REPEATS
    rate = total / sum(durations) if sum(durations) else 0.0
    commit = os.environ.get("GITHUB_SHA", "local/unknown")
    workflow = os.environ.get("GITHUB_RUN_ID", "local")
    timestamp = datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M:%S UTC")
    q = lambda value: TICK + value + TICK
    fence = TICK * 3
    lines = [
        "# SLeeLa Native Math Synthesis CI Results", "",
        f"- **Recorded:** {timestamp}", f"- **Commit:** {q(commit)}",
        f"- **Workflow run:** {q(workflow)}", "- **Platform:** Ubuntu latest (GitHub-hosted runner)",
        f"- **Build:** {q('make -C impl all')}", f"- **Subject activation:** {q('bin/sleelactl run math')}",
        f"- **Math program:** {q('impl/tests/subjects/math_values.sleela')} ({q('import math;')})", "",
        "## Accuracy", "", f"- Assertions per execution: **{expected}**",
        f"- Executions: **{REPEATS}**", f"- Total successful assertions: **{sum(pass_counts)} / {total}**",
        "- Failed assertions: **0**", "- Result: **PASS**", "",
        "Accuracy compares native math results against reference values using the tolerances in math_values.sleela.",
        "", "## Speed", "", "| Repetition | End-to-end runtime (ms) | Assertions |",
        "|---:|---:|---:|"
    ]
    for i, (duration, count) in enumerate(zip(durations, pass_counts), 1):
        lines.append(f"| {i} | {duration * 1000:.3f} | {count} |")
    lines += [
        "", f"- Mean end-to-end runtime: **{mean * 1000:.3f} ms**",
        f"- Median end-to-end runtime: **{median * 1000:.3f} ms**",
        f"- Minimum / maximum: **{min(durations) * 1000:.3f} / {max(durations) * 1000:.3f} ms**",
        f"- Aggregate assertion throughput: **{rate:.2f} assertions/s**", "",
        "Timing includes SLeeLa process startup, source loading, execution, and output capture. "
        "This is an end-to-end smoke benchmark, not an isolated native-function microbenchmark; "
        "compare runs on the same runner class for meaningful trends.", "",
        "## Subject activation output", "", fence + "text", subject.stdout.strip(), fence, "",
        "## Interpretation", "",
        "This run verifies that the source builds, sleelactl dispatches the native math subject, "
        "a SLeeLa program imports math, and numerical assertions pass. Timing is retained per repetition.",
        ""
    ]
    REPORT.write_text("\n".join(lines), encoding="utf-8")
    print(f"Math synthesis CI: PASS ({total}/{total} assertions)")
    print(f"Mean {mean * 1000:.3f} ms; median {median * 1000:.3f} ms")
    return 0
if __name__ == "__main__":
    raise SystemExit(main())
