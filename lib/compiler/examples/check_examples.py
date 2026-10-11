#!/usr/bin/env python3
"""Structural CI checks for the SLeeLa compiler-construction examples.

These checks validate repository wiring and fail-closed teaching scaffolds. They
are deliberately not presented as a substitute for compiling the .sleela files
with the authoritative frontend.
"""
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parent
REPO = ROOT.parents[3]
failures = []


def require(condition: bool, message: str) -> None:
    if not condition:
        failures.append(message)


def read(relative: str) -> str:
    path = ROOT / relative
    require(path.is_file(), f"missing required example file: {relative}")
    return path.read_text(encoding="utf-8") if path.is_file() else ""


index = read("README.md")
levels = {
    "Novice": ("Novice/README.md", "Novice/HelloCompiler.sleela"),
    "Mid": ("Mid/README.md", "Mid/ExpressionCompiler.sleela"),
    "Senior": ("Senior/README.md", "Senior/CompilerPipeline.sleela"),
    "Very-Senior": ("Very-Senior/README.md", "Very-Senior/BootstrapCompiler.sleela"),
}

for level, files in levels.items():
    for relative in files:
        read(relative)
    require(level in index, f"top-level examples index does not mention {level}")

novice = read("Novice/HelloCompiler.sleela")
require('#sleela 1.3' in novice, "Novice scaffold must declare its language version")
require('keyword != "say"' in novice, "Novice scaffold must reject an unknown keyword")
require("!closedQuote" in novice, "Novice scaffold must reject an unterminated string")
require("if (!valid)" in novice, "Novice scaffold must block emission before validation")

mid = read("Mid/ExpressionCompiler.sleela")
require("if (!lexed)" in mid, "Mid scaffold must block parsing before lexing succeeds")
require("if (!parsed)" in mid, "Mid scaffold must block semantics before parsing succeeds")
require("validated" in mid, "Mid scaffold must track semantic validation")

senior = read("Senior/CompilerPipeline.sleela")
for token, label in (
    ("if (!sourceLoaded)", "source-load gate"),
    ("if (!frontendPassed)", "frontend gate"),
    ("if (!semanticPassed)", "semantic gate"),
    ("if (!mayEmitArtifact())", "artifact-emission gate"),
):
    require(token in senior, f"Senior scaffold missing {label}")
require("irValidated" in senior, "Senior scaffold must track IR validation")

bootstrap = read("Very-Senior/BootstrapCompiler.sleela")
for token, label in (
    ("if (!stage0Passed)", "Stage 0 ordering gate"),
    ("if (!stage0Passed || !stage1Passed)", "two-stage comparison gate"),
    ("stage0Hash", "Stage 0 artifact hash"),
    ("stage1Hash", "Stage 1 artifact hash"),
    ("reproducible = equal", "reproducibility comparison"),
):
    require(token in bootstrap, f"Very Senior scaffold missing {label}")

# Verify relative Markdown links in the examples index and each level guide.
for guide in [ROOT / "README.md"] + [ROOT / item[0] for item in levels.values()]:
    text = guide.read_text(encoding="utf-8")
    for target in re.findall(r"\[[^\]]+\]\(([^)]+)\)", text):
        if target.startswith(("https://", "http://", "#", "mailto:")):
            continue
        resolved = (guide.parent / target.split("#", 1)[0]).resolve()
        require(resolved.is_file(), f"broken Markdown link in {guide.relative_to(REPO)}: {target}")

if failures:
    print("compiler examples CI: FAIL")
    for failure in failures:
        print(f" - {failure}")
    sys.exit(1)

print("compiler examples CI: PASS (4 levels, source scaffolds, fail-closed gates, Markdown links)")
