<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# native-drive: Tutorial

| | |
|---|---|
| Nature | tool (`bin/sleelactl help native-drive`) |
| Registry summary | Shell-free C/C++ and supported SLeeLa AOT builds; process and XClass integration |
| Lives in | `tools/sleela-native-drive.py` (driver), `tools/sleela-native-service.py` (systemd unit helper), `impl/native-drive/` (docs, ABI headers, IPC client) |
| Writeup type | Tutorial, because the driver is a command a person runs and the build, run, pipe, `@native` and XClass paths all worked here; the systemd part could not be operated and is only rendered. |
| Verified | 2026-11-13 against `master` e048707fd: multi-file C build, shared-library build with ABI check, `.sleela` `@native` plugin build, `run`, `pipe`, timeout, XClass `info`, unit rendering, and `tools/tests/test_native_drive.py` (20 tests OK). |

## What it is

`native-drive` builds native code and starts processes without going through a shell
(`impl/native-drive/README.md`). It compiles C or C++ with the host compiler, can check that a shared
library exports the SLeeLa loader ABI, turns `@native` SLeeLa methods into a typed-ABI C plugin via
`sleela aot --emit`, runs one program or a pipeline with bounded output and a timeout, and delegates
`.xclass` files to the production loader. No SLeeLa program runs in the C/C++ paths: the system compiler
is the frontend.

## Before you start

```sh
cd SLeeLa            # the repository root
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
D=/tmp/nd-demo; mkdir -p $D/src $D/out
```

Needs Python 3, `cc`, and (for step 5) `impl/build/sleela`. There is nothing to build for the driver.
`sleelactl build native-drive ...` passes its arguments to the driver's `build` subcommand. Output below was
captured with a different `$D`; only paths differ.

## Step 1 - Build a two-file C program

```sh
cat > $D/src/main.c <<'EOF'
#include <stdio.h>
int mul(int,int);
int main(int argc,char**argv){printf("hello %d args, 6*7=%d\n",argc-1,mul(6,7));return argc>2?3:0;}
EOF
echo 'int mul(int a,int b){return a*b;}' > $D/src/math.c
bin/sleelactl build native-drive $D/src/main.c $D/src/math.c -o $D/out/app
```

```
native-drive: built c executable -> .../out/app
```

## Step 2 - Run it with captured output

```sh
bin/sleelactl run native-drive --capture -- $D/out/app
python3 tools/sleela-native-drive.py run --capture -- $D/out/app a b; echo "exit=$?"
```

```
hello 0 args, 6*7=42
hello 2 args, 6*7=42
exit=3
```

The child's exit status is propagated. `sleelactl run native-drive` supplies the subcommand `run` itself
(the registry's `runsub=pipe` means only the word `pipe` is passed through); writing
`sleelactl run native-drive run ...` fails with `could not start process: [Errno 2] No such file or directory: 'run'` (exit 4).

## Step 3 - Timeouts

```sh
python3 tools/sleela-native-drive.py run --timeout 5 -- /bin/sleep 20; echo "exit=$?"
```

```
native-drive: process exceeded timeout (5s)
exit=5
```

## Step 4 - Pipe two programs

```sh
bin/sleelactl run native-drive pipe -- $D/out/app :: /usr/bin/wc -c
```

```
21
```

`hello 0 args, 6*7=42\n` is 21 bytes. Without the word `pipe`, the `::` is passed to `app` as arguments.

## Step 5 - Shared libraries and `@native` SLeeLa

```sh
cat > $D/src/plug.c <<'EOF'
#include <stdint.h>
uint32_t sl_native_abi_version(void){return 1;}
int sl_native_run(int argc,const char*const argv[]){(void)argv;return argc;}
EOF
bin/sleelactl build native-drive $D/src/plug.c --kind shared --require-sleela-abi -o $D/out/plug.so

cat > $D/src/hot.sleela <<'EOF'
#sleela 3.1
class Hot {
    @native
    public static int add(int a, int b) { return a + b; }
}
EOF
bin/sleelactl build native-drive $D/src/hot.sleela --require-sleela-abi -o $D/out/hot.so
nm -D $D/out/hot.so | grep ' T '
```

```
native-drive: built c shared -> .../out/plug.so
native-drive: built sleela shared -> .../out/hot.so
... T sl_native_abi_version
... T sl_native_call
... T sl_native_run
... T sl_native_typed_abi_version
```

`--require-sleela-abi` only checks that the symbol names exist (README: it is not a trust check). Loading
`hot.so` from a running SLeeLa program needs a host authorization callback and was not tried here;
`impl/native-drive/AT-NATIVE-DISPATCH.md` says nothing is bound automatically by `sleela run`.

## Step 6 - XClass info

```sh
bin/sleelactl inspect native-drive xclass --mode info impl/xclass/samples/Greeter.xclass
```

```
Greeter (methods=1 fields=0 trust=3 classload=5 interface abstract signed)
  source    : tools/xmc/sample.rs
  signature : sha256:82a90cb0... (signed)
```

## Step 7 - Render a service unit (nothing installed)

```sh
python3 tools/sleela-native-service.py --name sleela-worker --executable $D/out/app --working-directory $D
```

This prints a hardened `[Unit]/[Service]/[Install]` text (`NoNewPrivileges=yes`, `ProtectSystem=strict`,
`PrivateTmp=yes`, ...) and installs nothing. The `--install`, `--enable`, `--start` and `inspect native-drive service --status`
paths need a systemd user manager and were **not operable here**: `sleelactl inspect native-drive service --status --name sleela-worker` printed
`Failed to connect to bus: No medium found`. That one read-only status call is the only time `systemctl` was invoked.

## What you did

You compiled and ran C code without a shell, built plugins that follow the SLeeLa native ABI, piped
processes, saw the timeout and exit-code behavior, queried an XClass, and rendered a service unit.

## Next steps

- [`impl/native-drive/README.md`](../../../impl/native-drive/README.md), [`impl/native-drive/AT-NATIVE-DISPATCH.md`](../../../impl/native-drive/AT-NATIVE-DISPATCH.md), [`impl/native-drive/IPC-PROTOCOL.md`](../../../impl/native-drive/IPC-PROTOCOL.md)
- [`bin/SLEELACTL.md`](../../../bin/SLEELACTL.md) (native-drive section)

## Sources

Read: `bin/sleelactl.d/registry.conf` (line 72), `bin/sleelactl` (line 539, `runsub` handling), `impl/native-drive/README.md`.
Ran: `bin/sleelactl help|info|inspect|build|run native-drive`, `python3 tools/sleela-native-drive.py build --help|run|...`,
`python3 tools/sleela-native-service.py` (render only), `python3 -m unittest tools/tests/test_native_drive.py` (20 tests, OK), `nm -D`.
Not run: `--language cpp`, `xclass --mode emit|run`, service install/enable/start/restart/stop/rollback, `run --max-output-bytes` limit (exit 6), `impl/native-drive/native_ipc_client.c`.
