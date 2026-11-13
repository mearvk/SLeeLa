<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# decompiler: Tutorial

| | |
|---|---|
| Nature | tool (`bin/sleelactl help decompiler`) |
| Registry summary | Slecompiler native-binary analysis/decompilation CLI |
| Lives in | `decompiler/` (binary `sleela-decompiler`, built to `decompiler/build/sleela-decompiler`) |
| Writeup type | Tutorial, because the CLI is operated by a person and every step below ran successfully; the output has real defects, listed in "Limits". |
| Verified | 2026-11-13 against `master` e048707fd: `sleelactl build decompiler`, then `inspect`, `disassemble`, `functions`, `cfg` and `decompile --output sleela|c|java` on a 3-function x86-64 ELF built for this page; `sleela-decompiler-test` and `ctest` ran with exit 0. |

## What it is

Slecompiler is the SLeeLa native-binary analysis subsystem (`decompiler/README.md`). The CLI
`sleela-decompiler` reads an ELF, PE or Mach-O file without executing it, reports its structure,
disassembles code, recovers basic blocks and function candidates, and writes an evidence-annotated
"reconstruction" in SLeeLa, C, C++ or Java syntax. The README states that the reconstruction is not a
guarantee of the original source.

## Before you start

```sh
cd SLeeLa            # the repository root
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
mkdir -p /tmp/decomp-demo
```

You need CMake, a C++20 compiler and `gcc`. The sample binary is made in Step 2 from source you write.
The output shown was captured in another scratch directory; only the `name=` path differs.

## Step 1 - Build

```sh
bin/sleelactl build decompiler
```

```
...
[ 96%] Linking CXX executable sleela-decompiler
[100%] Linking CXX executable sleela-decompiler-test
  ✓ decompiler built
```

Products are in `decompiler/build/` (gitignored): `sleela-decompiler`, `sleela-decompiler-test`,
`libsleela_decompiler.a` and five `slecompiler-api-*` example programs.

## Step 2 - Build a small native binary to analyze

```sh
cat > /tmp/decomp-demo/t.c <<'EOF'
int add(int a,int b){return a+b;}
int fact(int n){int r=1;for(int i=2;i<=n;i++)r*=i;return r;}
int main(void){return add(2,3)+fact(4)>0?0:1;}
EOF
cd /tmp/decomp-demo
gcc -O0 -fno-pie -no-pie -nostdlib -ffreestanding -fno-stack-protector -o t t.c -e main
gcc -O0 -c t.c -o t.o
gcc -O0 -o tfull t.c
cd SLeeLa            # the repository root
```

`t` is a static, freestanding ELF with `add` at 0x401000 and `fact` at 0x401014 (`nm t`). `t.o` is a
relocatable object, `tfull` a normal dynamic executable.

## Step 3 - Inspect

```sh
S=decompiler/build/sleela-decompiler
$S inspect /tmp/decomp-demo/t
bin/sleelactl run decompiler inspect /tmp/decomp-demo/t.o
```

```
name=/tmp/decomp-demo/t
size=9448
format=3
sha256=573b790b...e89f9
architecture=2
artifact_class=1
sections=10 symbols=0 imports=0 exports=0 relocations=0 archive_members=0
build_id=4bb687d5a0530b2702e893f78730ad4b61f5d80d
```

For `t.o` the report shows `artifact_class=4 sections=13 ... relocations=5`. For `tfull` it shows
`sections=30 symbols=4 imports=4 ... needs=libc.so.6`. Note `symbols=0` for `t` although it has a
`.symtab` (see "Limits").

## Step 4 - Disassemble

```sh
$S disassemble /tmp/decomp-demo/t | head -24
```

```
1000: push rbp
1001: mov rbp, rsp
1004: mov [rbp-0x4], edi
1007: mov [rbp-0x8], esi
100a: mov edx, [rbp-0x4]
100d: mov eax, [rbp-0x8]
1010: add eax, edx
1012: pop rbp
1013: ret
1014: push rbp
...
102e: db 0xf
102f: db 0xaf
1030: db 0xf8
```

The first nine lines are `add`. Addresses are file offsets (0x1000), not the virtual address 0x401000.
The bytes `0f af f8` at 0x102e are `imul`; the decoder does not know it and prints `db`, as the README
says unknown encodings degrade to `db`.

## Step 5 - Functions and control flow

```sh
$S functions /tmp/decomp-demo/t | tail -1
$S cfg /tmp/decomp-demo/t | tail -2
```

```
sub_1000 @ 1000
1 -> 3
2 -> 3
```

## Step 6 - Decompile

```sh
$S decompile /tmp/decomp-demo/t --output sleela
```

```
module decompiled {
  << recovered native function sub_1000 @ 0x1000 (decoded) >>
  function sub_1000() {
    < push(rbp);
    < rbp = rsp;
    ...
    < eax += edx;
    < pop(rbp);
    < return;

    < control flow: 9 insn, 0 branch, 0 call, 1 return
    < evidence (address | bytes | instruction | basis):
    <   00001000  55                       push rbp                      decoded
    ...
  << recovered native function sub_1014 @ 0x1014 (decoded) >>
```

`--output c` and `--output java` give the same statements in C and Java wrappers. Each statement
carries its address, bytes and basis, so a reader can check every claim against the binary.

## Step 7 - Run the tests

```sh
decompiler/build/sleela-decompiler-test; echo $?
ctest --test-dir decompiler/build
```

Both exit 0; ctest reports `100% tests passed, 0 tests failed out of 1`.

## Limits seen while doing this (not fixed)

- `symbols=0` for `t` and `t.o` although `readelf -S t` shows `.symtab` and `nm t` lists `add` and `fact`. `decompiler/src/elf.cpp:99` reads `.symtab` only to find ARM mapping symbols; `decompiler/README.md` lists "symbols" as fully parsed for ELF.
- `functions` printed one candidate (`sub_1000`); `decompile` printed two (`sub_1000`, `sub_1014`). `main` was not recovered, and no function is named.
- The `--output c` text is not valid C: the header banner starts `/* ---- */` and the next lines begin ` * ...`, and statements such as `[rbp-0x4] = edi;` are not C. `gcc -fsyntax-only` on it fails at line 2. The same applies to Java. Treat the output as annotated pseudo-code.
- `cfg` printed only edges (`1 -> 3`), without block addresses.

## What you did

You built the tool, made a binary, and ran each analysis stage on it, from identity to a
provenance-annotated reconstruction, without executing the binary.

## Next steps

- [`decompiler/README.md`](../../../decompiler/README.md), [`decompiler/TUTORIAL.md`](../../../decompiler/TUTORIAL.md) (API exemplars; its Step 1 build scripts `build/slecompiler-linux.sh` exist but were not run here)
- [`decompiler/docs/FORMATS.md`](../../../decompiler/docs/FORMATS.md), [`decompiler/API.md`](../../../decompiler/API.md)

## Sources

Read: `bin/sleelactl.d/registry.conf` (line 70), `decompiler/README.md`, `decompiler/TUTORIAL.md`,
`decompiler/src/elf.cpp` (line 99). Ran: `bin/sleelactl help|info|inspect|build|run decompiler`,
`sleela-decompiler --help|inspect|disassemble|functions|cfg|decompile`, `slecompiler-api-inspect` on `tfull`,
`nm`, `readelf -S`, `gcc -fsyntax-only` on the C output, `sleela-decompiler-test`, `ctest`.
The `decompile --to`, PE, Mach-O, ARM and archive paths were not run.
