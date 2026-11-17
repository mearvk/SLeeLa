"""Shared plumbing for the SLeeLa regression set (Python layer).

* locates the repository, the toolchain and the SLeeLa compiler binary;
* builds the C and C++ regression binaries (strict warnings, -Werror, with the
  instrumented allocator linked in) and runs them;
* provides helpers to run `sleela` hermetically (own scratch directory, fast
  integrity manifest) and to filter its banner noise;
* records KNOWN ISSUES: contracts the code is expected to honour but does not.
  They are reported, never silently dropped, and never fail the run.

Standard library only.
"""
from __future__ import annotations

import hashlib
import json
import os
import re
import shutil
import subprocess
import tempfile
import threading
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, Iterable, List, Optional, Sequence

HERE = Path(__file__).resolve().parent
REG = HERE.parent                      # test-suites/regression
ROOT = REG.parents[1]                  # repository root
BUILD = REG / "build"                  # ignored by the repository's **/build/ rule
LOGS = BUILD / "logs"
# per-process scratch: two concurrent runs must not delete each other's working files
SCRATCH = BUILD / "scratch" / f"run-{os.getpid()}"

CORE = ROOT / "impl" / "core"
FRONTEND = ROOT / "impl" / "frontend"

CC = os.environ.get("CC", "cc")
CXX = os.environ.get("CXX", "c++")

# The project's own audit flags (test-suites/run-all.sh) for the test TUs.
STRICT_C = ["-std=c11", "-O1", "-g", "-Wall", "-Wextra", "-Wpedantic", "-Wconversion",
            "-Wshadow", "-Wformat=2", "-Werror"]
STRICT_CXX = ["-std=c++17", "-O1", "-g", "-Wall", "-Wextra", "-Wpedantic", "-Wconversion",
              "-Wshadow", "-Wformat=2", "-Werror"]
# Code under test is compiled as the project builds it: warnings on, not fatal.
LIB_C = ["-std=gnu11", "-O1", "-g", "-Wall"]
LIB_CXX = ["-std=c++17", "-O1", "-g", "-Wall"]
WRAP = ["-Wl,--wrap=malloc", "-Wl,--wrap=calloc", "-Wl,--wrap=realloc", "-Wl,--wrap=free"]

# logical name -> source file of the code under test
SOURCES: Dict[str, Path] = {
    "utf16": CORE / "sleela_utf16.c",
    "java_string": CORE / "sleela_java_string.c",
    "java_string_codec": CORE / "sleela_java_string_codec.c",
    "regex": CORE / "sleela_regex.c",
    "path": CORE / "sleela_path.c",
    "time": CORE / "sleela_time.c",
    "memory": CORE / "sleela_memory.c",
    "events": CORE / "sleela_events.c",
    "lexer": FRONTEND / "lexer.cpp",
}

# suite -> (language, [code-under-test objects it links])
SUITES: Dict[str, tuple] = {
    "reg_utf16": ("c", ["utf16"]),
    "reg_java_string": ("c", ["utf16", "java_string"]),
    "reg_java_string_codec": ("c", ["java_string", "java_string_codec"]),
    "reg_regex": ("c", ["regex"]),
    "reg_path": ("c", ["path"]),
    "reg_time_memory": ("c", ["time", "memory"]),
    "reg_events": ("c", ["events"]),
    "reg_lexer": ("cpp", ["lexer"]),
    "reg_model": ("cpp", ["utf16", "java_string", "java_string_codec", "regex", "path"]),
}


class BuildError(RuntimeError):
    pass


def have_toolchain() -> Optional[str]:
    """Return None when a C and C++ compiler are usable, else a reason."""
    for tool in (CC, CXX):
        if shutil.which(tool) is None:
            return f"compiler not found: {tool}"
    return None


def _run(cmd: Sequence[str], cwd: Optional[Path] = None, timeout: int = 600) -> subprocess.CompletedProcess:
    return subprocess.run([str(c) for c in cmd], cwd=str(cwd) if cwd else None, capture_output=True,
                          text=True, timeout=timeout)


_build_lock = threading.Lock()
_built: Dict[str, Path] = {}


def _extra_flags() -> List[str]:
    return ["-fsanitize=address,undefined", "-fno-omit-frame-pointer"] if os.environ.get("REG_SANITIZE") == "1" else []


def compile_object(src: Path, obj: Path, cxx: bool = False, flags: Optional[Iterable[str]] = None) -> Path:
    obj.parent.mkdir(parents=True, exist_ok=True)
    base = LIB_CXX if cxx else LIB_C
    cmd = [CXX if cxx else CC, *(flags if flags is not None else base), *_extra_flags(), "-c", src, "-o", obj]
    r = _run(cmd)
    if r.returncode != 0:
        raise BuildError(f"compiling {src} failed:\n{r.stderr}")
    return obj


def support_object() -> Path:
    key = "support"
    with _build_lock:
        if key not in _built:
            _built[key] = compile_object(REG / "c" / "reg_support.c", BUILD / "obj" / "reg_support.o",
                                         flags=["-std=gnu11", "-O1", "-g", "-Wall", "-Wextra"])
    return _built[key]


def library_object(name: str) -> Path:
    with _build_lock:
        if name not in _built:
            src = SOURCES[name]
            _built[name] = compile_object(src, BUILD / "obj" / f"{name}.o", cxx=src.suffix == ".cpp")
    return _built[name]


def build_suite(name: str, replace: Optional[Dict[str, Path]] = None, tag: str = "") -> Path:
    """Compile and link one regression binary. `replace` swaps in a different
    object file for a logical library (used by the mutation tests)."""
    lang, libs = SUITES[name]
    cxx = lang == "cpp"
    src = REG / lang / f"{name}.{'cpp' if cxx else 'c'}"
    out = BUILD / "bin" / (name + (f".{tag}" if tag else ""))
    out.parent.mkdir(parents=True, exist_ok=True)
    objs = [(replace or {}).get(lib) or library_object(lib) for lib in libs]
    cmd = [CXX if cxx else CC, *(STRICT_CXX if cxx else STRICT_C), *_extra_flags(), src,
           support_object(), *objs, *WRAP, "-pthread", "-o", out]
    if not cxx:
        cmd += ["-lm"]
    r = _run(cmd)
    if r.returncode != 0:
        raise BuildError(f"building {name} failed:\n{r.stderr}")
    return out


@dataclass
class SuiteResult:
    returncode: int
    stdout: str
    stderr: str
    summary: Dict[str, int] = field(default_factory=dict)

    @property
    def failures(self) -> List[str]:
        return [ln for ln in self.stderr.splitlines() if "FAIL" in ln]

    @property
    def known_issues(self) -> List[str]:
        return [ln.strip() for ln in self.stdout.splitlines() if "KNOWN-ISSUE" in ln]


_SUMMARY = re.compile(r"^SUITE (\S+): (.*)$", re.M)


def run_binary(exe: Path, timeout: int = 900) -> SuiteResult:
    tmp = SCRATCH / "c-tmp"
    tmp.mkdir(parents=True, exist_ok=True)
    env = os.environ.copy()
    env["REG_TMPDIR"] = str(tmp)
    r = subprocess.run([str(exe)], capture_output=True, text=True, timeout=timeout, env=env)
    summary: Dict[str, int] = {}
    m = _SUMMARY.search(r.stdout)
    if m:
        for kv in m.group(2).split():
            k, _, v = kv.partition("=")
            if v.isdigit():
                summary[k] = int(v)
    LOGS.mkdir(parents=True, exist_ok=True)
    (LOGS / f"{exe.name}.log").write_text(r.stdout + "\n--- stderr ---\n" + r.stderr)
    return SuiteResult(r.returncode, r.stdout, r.stderr, summary)


# ---------------------------------------------------------------- known issues

_known: List[dict] = []


def record_known_issue(issue_id: str, description: str, still_open: bool, source: str) -> None:
    _known.append({"id": issue_id, "description": description, "open": still_open, "source": source})
    KNOWN_FILE.parent.mkdir(parents=True, exist_ok=True)
    existing = json.loads(KNOWN_FILE.read_text()) if KNOWN_FILE.exists() else []
    existing = [e for e in existing if not (e["id"] == issue_id and e["source"] == source)]
    existing.append(_known[-1])
    KNOWN_FILE.write_text(json.dumps(existing, indent=1, sort_keys=True))


KNOWN_FILE = BUILD / "known-issues.json"


# ------------------------------------------------------------------- sleela CLI

NOISE = ("[memory-manager]", "[security]", "[defender]", "verification:")


def sleela_binary() -> Optional[Path]:
    """Compiler under test: $SLEELA_BIN, else the source build, else the
    committed prebuilt binary."""
    for cand in (os.environ.get("SLEELA_BIN"), ROOT / "impl" / "build" / "sleela", ROOT / "working-binaries" / "sleela"):
        if cand and Path(cand).is_file() and os.access(cand, os.X_OK):
            return Path(cand)
    return None


def sha256_file(path: Path) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def fast_manifest() -> Path:
    """A tiny integrity manifest (SHEET.sheet + the verifier) so each compiler
    invocation does not re-hash the 18k-file repository. The real manifest and
    the fail-closed behaviour are covered by test_cli_gate / test_integrity."""
    out = BUILD / "fast-manifest.json"
    files = ["SHEET.sheet", "tools/verify-before-execution.py"]
    doc = {"algorithm": "sha256", "files": [{"path": f, "sha256": sha256_file(ROOT / f)} for f in files]}
    BUILD.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(doc, indent=1))
    return out


def cli_env(manifest: Optional[Path] = None, **extra: str) -> Dict[str, str]:
    env = os.environ.copy()
    env.update({
        "SLEELA_HOME": str(ROOT),
        "SLEELA_LIB": str(ROOT / "lib"),
        "SLEELA_SHEET": str(ROOT / "SHEET.sheet"),
        "SLEELA_SHA256_MANIFEST": str(manifest or fast_manifest()),
    })
    env.update(extra)
    return env


def run_sleela(args: Sequence[object], cwd: Path, timeout: int = 120, env: Optional[Dict[str, str]] = None,
               binary: Optional[Path] = None) -> subprocess.CompletedProcess:
    exe = binary or sleela_binary()
    if exe is None:
        raise RuntimeError("no sleela binary available")
    return subprocess.run([str(exe), *[str(a) for a in args]], cwd=str(cwd), capture_output=True, text=True,
                          encoding="utf-8", errors="replace", timeout=timeout, env=env or cli_env())


def program_output(cp: subprocess.CompletedProcess) -> List[str]:
    """Program stdout with the toolchain banner lines removed."""
    return [ln for ln in cp.stdout.splitlines() if not ln.startswith(NOISE)]


def diagnostics(cp: subprocess.CompletedProcess) -> str:
    """Compiler/runtime diagnostics (stderr) with banner lines removed."""
    return "\n".join(ln for ln in cp.stderr.splitlines() if not ln.startswith(NOISE))


def fresh_dir(label: str) -> Path:
    SCRATCH.mkdir(parents=True, exist_ok=True)
    return Path(tempfile.mkdtemp(prefix=label + "-", dir=str(SCRATCH)))


def write_program(directory: Path, name: str, source: str) -> Path:
    p = directory / name
    p.write_text(source, encoding="utf-8")
    return p


def cleanup_scratch() -> None:
    shutil.rmtree(SCRATCH, ignore_errors=True)
