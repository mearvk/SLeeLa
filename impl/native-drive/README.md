# SLeeLa Native Drive

The Native Drive is the explicit host-toolchain boundary for native source and
process execution. It does not claim that the teaching C/C++ frontend classes
are complete production parsers.

## Implemented CLI

tools/sleela-native-drive.py provides:

- Direct C and C++ source compilation through the host toolchain.
- SLeeLa .sleela input through the existing sleela aot --emit contract,
  followed by a host C compiler.
- Argument-vector process execution without an implicit shell.
- Optional stdin-file input and captured stdout/stderr.
- A two-process pipeline connecting producer stdout to consumer stdin.
- Timeouts and nonzero exit propagation.

Build output is staged in a temporary directory and copied into place only
after the compiler succeeds. Compiler selection uses CC/CXX or --compiler.
Extra compiler arguments are passed as individual argv elements.

## Usage

    python3 tools/sleela-native-drive.py build source.c -o build/source
    python3 tools/sleela-native-drive.py build source.cpp -o build/source
    python3 tools/sleela-native-drive.py build hot.sleela --language sleela -o build/hot

    python3 tools/sleela-native-drive.py run --capture --timeout 10 -- build/source arg1
    python3 tools/sleela-native-drive.py pipe --timeout 10 -- producer arg :: consumer arg

The SLeeLa route is constrained by sleela aot --emit. Methods outside its
supported subset are rejected. The output must be a C translation unit.
This tool does not add full C/C++ frontend parsing, VM in-process native
dispatch, shared-library creation, or automatic systemd unit installation.

## Annotation and process contract

@native remains the SLeeLa source-level request for AOT treatment. Process
redirection is an execution policy, not an annotation that grants authority.
Future annotations such as @stdin("pipe"), @stdout("pipe"),
@stderr("pipe"), @timeout(30000), and @capability("process.spawn") are
design examples, not implemented language features. They must be parsed into
validated metadata and enforced by the process bridge before being advertised
as supported.

Native code runs with the host process privileges. Do not compile or load
untrusted source in a privileged service. A future shared-library path must
validate the native-loader ABI before exposing symbols; process isolation is
preferred for untrusted workloads.

## Platform notes

The utility uses portable Python process APIs and does not require systemctl.
C/C++ compilation still depends on a compatible host compiler and its flags.
systemctl remains a Linux/systemd-specific service-management integration.

## Tests

    python3 -m unittest discover -s tools/tests -p 'test_native_drive.py' -v
