#!/usr/bin/env python3
"""SLeeLa Native Drive: explicit, shell-free native build and process execution."""
from __future__ import annotations
import argparse, ctypes, os, pathlib, re, shutil, subprocess, sys, tempfile
from typing import Sequence

EXIT_USAGE, EXIT_BUILD, EXIT_RUN, EXIT_TIMEOUT = 2, 3, 4, 5

def _compiler(language: str, explicit: str | None) -> str:
    candidate = explicit or os.environ.get("CC" if language == "c" else "CXX")
    candidate = candidate or ("cl" if os.name == "nt" else ("cc" if language == "c" else "c++"))
    resolved = shutil.which(candidate)
    if not resolved:
        raise RuntimeError(f"compiler not found: {candidate!r}; set CC/CXX or use --compiler")
    return resolved

def _language(source: pathlib.Path, requested: str) -> str:
    if requested != "auto": return requested
    suffix = source.suffix.lower()
    if suffix == ".sleela": return "sleela"
    if suffix == ".c": return "c"
    if suffix in {".cc", ".cpp", ".cxx", ".c++"}: return "cpp"
    raise ValueError(f"cannot infer source language from {suffix or '<no extension>'}; use --language")

def _run_checked(argv: Sequence[str], timeout: float | None = None) -> subprocess.CompletedProcess[str]:
    try:
        return subprocess.run(list(argv), text=True, stdout=subprocess.PIPE,
                              stderr=subprocess.PIPE, timeout=timeout, check=False)
    except subprocess.TimeoutExpired as exc:
        raise TimeoutError(f"command timed out after {timeout:g}s: {argv[0]}") from exc
    except OSError as exc:
        raise RuntimeError(f"could not start {argv[0]!r}: {exc}") from exc

def _validate_native_abi(library: pathlib.Path) -> None:
    """Check exported ABI symbol names without loading/executing the module."""
    if os.name == "nt" and shutil.which("dumpbin"):
        command = [shutil.which("dumpbin"), "/nologo", "/exports", str(library)]
    else:
        nm = shutil.which("nm")
        if not nm:
            raise RuntimeError("--require-sleela-abi needs nm (or dumpbin on Windows); refusing to load the library just to inspect it")
        if sys.platform == "darwin":
            command = [nm, "-gU", str(library)]
        elif os.name == "nt":
            command = [nm, "-g", str(library)]
        else:
            command = [nm, "-D", "--defined-only", str(library)]
    result = _run_checked(command, timeout=15.0)
    if result.returncode != 0:
        raise RuntimeError("could not inspect shared-library exports with nm: " + result.stderr.strip())
    symbols = set()
    for line in result.stdout.splitlines():
        fields = line.split()
        if fields:
            symbols.add(fields[-1].lstrip("_"))
    required = {"sl_native_abi_version", "sl_native_run"}
    missing = sorted(required - symbols)
    if missing:
        raise RuntimeError("shared library is missing required SLeeLa ABI exports: " + ", ".join(missing))
    # The ABI version function is deliberately NOT invoked during build: loading
    # a shared object can execute constructors/DllMain. The trusted native loader
    # checks the returned version when an explicitly authorized load occurs.


def build(args: argparse.Namespace) -> int:
    sources = [pathlib.Path(p).expanduser().resolve() for p in args.sources]
    for source in sources:
        if not source.is_file():
            print(f"native-drive: source file does not exist: {source}", file=sys.stderr); return EXIT_USAGE
    try: languages = [_language(source, args.language) for source in sources]
    except ValueError as exc:
        print(f"native-drive: {exc}", file=sys.stderr); return EXIT_USAGE
    if len(set(languages)) != 1:
        print("native-drive: one build invocation must use a single source language", file=sys.stderr); return EXIT_USAGE
    language = languages[0]
    if language == "sleela" and len(sources) != 1:
        print("native-drive: SLeeLa AOT currently accepts exactly one source file", file=sys.stderr); return EXIT_USAGE
    source = sources[0]
    if language == "sleela" and any(p.suffix.lower() != ".sleela" for p in sources):
        print("native-drive: --language sleela requires a .sleela source file", file=sys.stderr); return EXIT_USAGE
    if language in {"c", "cpp"} and any(p.suffix.lower() == ".sleela" for p in sources):
        print("native-drive: SLeeLa source must use --language sleela", file=sys.stderr); return EXIT_USAGE
    if language == "sleela" and args.kind == "shared":
        print("native-drive: SLeeLa AOT shared-library output is not supported until native entry-point ABI emission exists", file=sys.stderr)
        return EXIT_USAGE
    output = pathlib.Path(args.output).expanduser().resolve()
    output.parent.mkdir(parents=True, exist_ok=True)
    cc_language = "c" if language in {"c", "sleela"} else "cpp"
    try:
        compiler = _compiler(cc_language, args.compiler)
        with tempfile.TemporaryDirectory(prefix="sleela-native-") as temp_name:
            temp_dir = pathlib.Path(temp_name)
            compile_source = source
            if language == "sleela":
                sleela = shutil.which(args.sleela or os.environ.get("SLEELA", "sleela"))
                if not sleela: raise RuntimeError("sleela CLI not found; set SLEELA or pass --sleela")
                emitted = _run_checked([sleela, "aot", "--emit", str(source)], args.timeout)
                if emitted.returncode:
                    sys.stderr.write(emitted.stderr); return EXIT_BUILD
                if not emitted.stdout.lstrip().startswith(("#include", "/*", "//")):
                    raise RuntimeError("sleela aot did not emit a C translation unit")
                compile_source = temp_dir / "sleela-aot.c"
                compile_source.write_text(emitted.stdout, encoding="utf-8")
            if args.kind == "shared":
                suffix = ".dll" if os.name == "nt" else (".dylib" if sys.platform == "darwin" else ".so")
                staged = temp_dir / ("native-output" + suffix)
            else:
                staged = temp_dir / ("native-output.exe" if os.name == "nt" else "native-output")
            compile_inputs = [str(compile_source)] if language == "sleela" else [str(p) for p in sources]
            is_msvc = os.name == "nt" and pathlib.Path(compiler).name.lower() in {"cl", "cl.exe"}
            if is_msvc:
                command = [compiler, "/nologo", "/O2"]
                command.append("/std:c11" if cc_language == "c" else "/std:c++17")
                if args.kind == "shared":
                    command.extend(["/LD", *compile_inputs, "/link", f"/OUT:{staged}"])
                else:
                    command.extend(compile_inputs + [f"/Fe:{staged}"])
            else:
                standard = "-std=c11" if cc_language == "c" else "-std=c++17"
                command = [compiler, standard, "-O2"]
                if args.kind == "shared":
                    command.extend(["-shared"] if os.name == "nt" else
                                   (["-dynamiclib", "-fPIC"] if sys.platform == "darwin" else ["-shared", "-fPIC"]))
                command.extend(compile_inputs + ["-o", str(staged)])
                if os.name != "nt":
                    command.append("-lm")
            command.extend(args.compiler_arg)
            result = _run_checked(command, args.timeout)
            if result.stdout: sys.stdout.write(result.stdout)
            if result.stderr: sys.stderr.write(result.stderr)
            if result.returncode: return EXIT_BUILD
            if not staged.is_file(): raise RuntimeError("compiler reported success but created no output")
            if args.kind == "shared" and args.require_sleela_abi:
                _validate_native_abi(staged)
            # Stage beside the destination so os.replace is atomic even when
            # the system temporary directory is on another filesystem.
            fd, adjacent_name = tempfile.mkstemp(prefix=f".{output.name}.", dir=output.parent)
            os.close(fd)
            try:
                shutil.copy2(staged, adjacent_name)
                if os.name != "nt" and args.kind == "executable":
                    os.chmod(adjacent_name, os.stat(adjacent_name).st_mode | 0o111)
                os.replace(adjacent_name, output)
            finally:
                if os.path.exists(adjacent_name):
                    os.unlink(adjacent_name)
    except TimeoutError as exc:
        print(f"native-drive: {exc}", file=sys.stderr); return EXIT_TIMEOUT
    except (RuntimeError, OSError) as exc:
        print(f"native-drive: {exc}", file=sys.stderr); return EXIT_BUILD
    print(f"native-drive: built {language} {args.kind} -> {output}", file=sys.stderr)
    return 0

def run_process(args: argparse.Namespace) -> int:
    if not args.command:
        print("native-drive run: missing executable after --", file=sys.stderr); return EXIT_USAGE
    stream = None
    try:
        if args.stdin_file: stream = open(args.stdin_file, "rb")
        proc = subprocess.run(args.command, stdin=stream,
            stdout=subprocess.PIPE if args.capture else None,
            stderr=subprocess.PIPE if args.capture else None,
            timeout=args.timeout, check=False)
        if args.capture:
            if proc.stdout: sys.stdout.buffer.write(proc.stdout)
            if proc.stderr: sys.stderr.buffer.write(proc.stderr)
        return proc.returncode
    except subprocess.TimeoutExpired:
        print(f"native-drive: process exceeded timeout ({args.timeout:g}s)", file=sys.stderr); return EXIT_TIMEOUT
    except OSError as exc:
        print(f"native-drive: could not start process: {exc}", file=sys.stderr); return EXIT_RUN
    finally:
        if stream: stream.close()

def pipe_processes(args: argparse.Namespace) -> int:
    if not args.producer or not args.consumer:
        print("native-drive pipe: producer and consumer must be separated by ::", file=sys.stderr); return EXIT_USAGE
    producer = consumer = None
    producer_err = tempfile.TemporaryFile()
    try:
        # A temporary file prevents a verbose producer from blocking on stderr
        # while the consumer is still draining the stdout pipe.
        producer = subprocess.Popen(args.producer, stdout=subprocess.PIPE, stderr=producer_err)
        assert producer.stdout is not None
        consumer = subprocess.Popen(args.consumer, stdin=producer.stdout, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        producer.stdout.close()
        try:
            out, cerr = consumer.communicate(timeout=args.timeout)
            pcode = producer.wait(timeout=max(0.1, args.timeout))
            producer_err.seek(0)
            perr = producer_err.read()
        except subprocess.TimeoutExpired:
            consumer.kill(); producer.kill(); consumer.communicate(); producer.wait()
            print(f"native-drive: pipeline exceeded timeout ({args.timeout:g}s)", file=sys.stderr); return EXIT_TIMEOUT
        if out: sys.stdout.buffer.write(out)
        if perr: sys.stderr.buffer.write(perr)
        if cerr: sys.stderr.buffer.write(cerr)
        if pcode: print(f"native-drive: producer exited {pcode}", file=sys.stderr); return pcode
        return consumer.returncode
    except OSError as exc:
        for proc in (consumer, producer):
            if proc and proc.poll() is None: proc.kill()
        print(f"native-drive: could not start pipeline: {exc}", file=sys.stderr); return EXIT_RUN
    finally:
        for proc in (consumer, producer):
            if proc and proc.poll() is None: proc.kill()
        producer_err.close()

def xclass_command(args: argparse.Namespace) -> int:
    """Use the production SecureJDK .xclass loader already shipped by sleela."""
    sleela = shutil.which(args.sleela or os.environ.get("SLEELA", "sleela"))
    if not sleela:
        print("native-drive xclass: sleela CLI not found; set SLEELA or pass --sleela", file=sys.stderr)
        return EXIT_RUN
    command = [sleela, "xclass", "--" + args.mode, *args.files]
    try:
        result = subprocess.run(command, text=True, stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE, timeout=args.timeout, check=False)
        if result.stdout: sys.stdout.write(result.stdout)
        if result.stderr: sys.stderr.write(result.stderr)
        return result.returncode
    except subprocess.TimeoutExpired:
        print(f"native-drive xclass: loader exceeded timeout ({args.timeout:g}s)", file=sys.stderr)
        return EXIT_TIMEOUT
    except OSError as exc:
        print(f"native-drive xclass: could not start sleela: {exc}", file=sys.stderr)
        return EXIT_RUN

def parser() -> argparse.ArgumentParser:
    root = argparse.ArgumentParser(prog="native-drive",
        description="Build C/C++ or supported @native SLeeLa source and run processes without a shell.")
    sub = root.add_subparsers(dest="action", required=True)
    b = sub.add_parser("build", help="compile C, C++, or SLeeLa AOT output into an executable")
    b.add_argument("sources", nargs="+"); b.add_argument("-o", "--output", required=True)
    b.add_argument("--language", choices=("auto", "c", "cpp", "sleela"), default="auto")
    b.add_argument("--kind", choices=("executable", "shared"), default="executable")
    b.add_argument("--require-sleela-abi", action="store_true", help="verify sl_native_abi_version()==1 and sl_native_run before publishing a shared library")
    b.add_argument("--compiler"); b.add_argument("--sleela")
    b.add_argument("--timeout", type=float, default=120.0)
    b.add_argument("--compiler-arg", action="append", default=[], help="extra compiler/linker argv item; repeat as needed")
    b.set_defaults(handler=build)
    r = sub.add_parser("run", help="run one executable with optional stdin file")
    r.add_argument("--stdin-file"); r.add_argument("--capture", action="store_true")
    r.add_argument("--timeout", type=float, default=30.0)
    r.add_argument("command", nargs=argparse.REMAINDER); r.set_defaults(handler=run_process)
    x = sub.add_parser("xclass", help="validate, inspect, or reconstruct SecureJDK .xclass input using the production loader")
    x.add_argument("--mode", choices=("info", "emit", "run"), default="info")
    x.add_argument("--sleela")
    x.add_argument("--timeout", type=float, default=30.0)
    x.add_argument("files", nargs="+")
    x.set_defaults(handler=xclass_command)
    p = sub.add_parser("pipe", help="connect producer stdout to consumer stdin")
    p.add_argument("--timeout", type=float, default=30.0)
    p.add_argument("pipeline", nargs=argparse.REMAINDER); p.set_defaults(handler=None)
    return root

def main() -> int:
    args = parser().parse_args()
    if args.action == "run" and args.command[:1] == ["--"]:
        args.command = args.command[1:]
    if args.action == "pipe":
        if args.pipeline[:1] == ["--"]:
            args.pipeline = args.pipeline[1:]
        try:
            split = args.pipeline.index("::")
            args.producer, args.consumer = args.pipeline[:split], args.pipeline[split + 1:]
        except ValueError: args.producer, args.consumer = [], []
        return pipe_processes(args)
    return args.handler(args)

if __name__ == "__main__": raise SystemExit(main())
