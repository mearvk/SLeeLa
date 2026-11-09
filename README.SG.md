# SLeeLa 3.0 — Singapore (SG)

> **Singapore multilingual edition.** This page provides introductory and practical project guidance in English, Simplified Chinese, Bahasa Melayu, and Tamil. Technical identifiers, source code, commands, filenames, and API names remain unchanged.

## English

SLeeLa is a native systems programming language, compiler, runtime, and virtual-machine ecosystem. It brings together the language, compiler, libraries, developer tools, and platform contracts needed to build and run software.

## 简体中文

SLeeLa 是一套原生系统编程语言、编译器、运行时和虚拟机生态系统。它将语言、编译器、函式库、开发工具及平台契约整合在一起，用于构建和运行软件。

## Bahasa Melayu

SLeeLa ialah bahasa pengaturcaraan sistem natif yang merangkumi pengkompil, persekitaran masa jalan dan ekosistem mesin maya. Bahasa, pengkompil, pustaka, alat pembangunan dan kontrak platformnya bekerjasama untuk membina serta menjalankan perisian.

## தமிழ்

SLeeLa என்பது ஒரு இயல்பான கணினி அமைப்பு நிரலாக்க மொழி; இதில் தொகுப்பி, இயக்கநேரச் சூழல் மற்றும் மெய்நிகர் இயந்திரச் சூழல் அடங்கும். மொழி, தொகுப்பி, நூலகங்கள், உருவாக்குநர் கருவிகள் மற்றும் தள ஒப்பந்தங்கள் இணைந்து மென்பொருளை உருவாக்கவும் இயக்கவும் உதவுகின்றன.

## I. Start here / 开始使用 / Bermula / தொடங்குதல்

- [Build the native implementation](impl/README.md)
- [Virtual machines and Modium user guide](virtual-machine/USER-GUIDE.md) — `sleela vm`, `sleela modium`
- [CPU architecture in SLeeLa source](lib/cpu/CPU.md) — more than 130 CPU profiles; see [timing controls](lib/cpu/TIMING.md).
- [Documentation index](docs/README.md)
- [Generated API documentation](docs/api/README.md) and [API catalogue](api/README.md)
- [Platform support and validation](docs/PLATFORM-SUPPORT.md)
- [Testing policy](markdown/TESTING.md)

## II. Build and test

Requirements include a C/C++ toolchain (GCC or Clang), GNU Make, and Python 3. Linux graphical components may also require X11/Xrandr development packages.

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

A successful build of one component does not establish that every library or platform is release-ready. Consult the build and platform documentation for target-specific requirements.

## III. Install

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

The conventional system prefix is generally `/usr/local`. Setting `PREFIX="$HOME/.local"` provides a user-local installation and normally avoids administrator privileges. For the guided installation flow and package details, see the [installation guide](install/README.md).

## IV. Language and runtime

The normative grammar is [`SLEELA.syntax`](SLEELA.syntax). This repository documents syntax **3.0**. Compiler version and source-language syntax version are distinct. Technical references cover `@mix`, extended integer families, Preonem document markers, SleelaDoc, API generation, and Nordshrift. Identifiers and code examples are retained verbatim.

## V. Engineering and validation

SLeeLa emphasizes evidence, explicit component contracts, portability, safe failure behavior, and reproducible releases. Validation must be assessed by module and platform. Skipped checks must not be reported as passing, and a single successful component build is not full release validation.

## VI. Editors and contributions

SLeeLa provides **Vim** and **Emacs** themes and syntax integration, plus a native C/C++ palette utility. See the [editor integration guide](linux-editors/README.md).

Contributions should be focused, include positive and negative tests, and state the operating system and toolchain used. Do not describe skipped checks as successful.


## X. API contracts, testing and maintenance / API 契约、测试与维护 / Kontrak API, ujian dan penyelenggaraan / API ஒப்பந்தங்கள், சோதனை மற்றும் பராமரிப்பு

### English

Every public API should document accepted inputs, outputs, error conditions, memory ownership and side effects. Module documentation should state preconditions, postconditions and platform limitations. A symbol name or module reference does not itself authorize execution; authorization must be checked at the appropriate execution boundary.

Test the compiler pipeline in layers: lexing, parsing, semantic analysis, IR lowering, code generation, VM execution and native integration. Regression tests should preserve a minimal reproducer, expected result and actual result. Include negative cases for malformed syntax, invalid types, missing resources and ABI mismatches. Never report skipped or unrun tests as passing.

For reproducibility, record the Git commit, operating system, architecture, compiler version, toolchain and exact commands. A result on one platform does not establish support on another. Consult the [platform matrix](docs/PLATFORM-SUPPORT.md) and [testing policy](markdown/TESTING.md).

### 简体中文

每个公开 API 都应记录可接受的输入、输出、错误条件、内存所有权和副作用。模块文档应说明前置条件、后置条件及平台限制。符号名称或模块引用本身并不授予代码执行权限；必须在适当的执行边界检查授权。

测试应分层覆盖 compiler pipeline：词法分析、语法分析、语义分析、IR lowering、代码生成、VM execution 和 native integration。回归测试应保留最小复现输入、预期结果和实际结果，并覆盖语法错误、类型错误、资源缺失和 ABI 不匹配等负面场景。跳过或未运行的测试不得报告为通过。

为保证结果可复现，应记录 Git commit、操作系统、体系结构、compiler 版本、toolchain 和完整命令。一个平台上的结果不能证明其他平台也受支持。请参阅[平台支持矩阵](docs/PLATFORM-SUPPORT.md)和[测试政策](markdown/TESTING.md)。

### Bahasa Melayu

Setiap API awam perlu mendokumenkan input yang diterima, output, keadaan ralat, pemilikan memori dan kesan sampingan. Dokumentasi modul hendaklah menyatakan prasyarat, pascasyarat dan batasan platform. Nama simbol atau rujukan modul tidak memberikan kebenaran untuk melaksanakan kod; kebenaran mesti disahkan pada sempadan pelaksanaan yang sesuai.

Uji compiler pipeline secara berperingkat: analisis leksikal, parsing, analisis semantik, IR lowering, penjanaan kod, pelaksanaan VM dan integrasi native. Ujian regresi perlu menyimpan input minimum yang menghasilkan semula masalah, hasil dijangka dan hasil sebenar. Sertakan kes negatif untuk sintaks rosak, jenis tidak sah, sumber tiada dan ketidakpadanan ABI. Ujian yang dilangkau atau tidak dijalankan tidak boleh dilaporkan sebagai lulus.

Untuk kebolehulangan, rekodkan Git commit, sistem operasi, seni bina, versi compiler, toolchain dan arahan tepat. Keputusan pada satu platform tidak membuktikan sokongan pada platform lain. Rujuk [matriks sokongan platform](docs/PLATFORM-SUPPORT.md) dan [dasar ujian](markdown/TESTING.md).

### தமிழ்

ஒவ்வொரு public API-யும் ஏற்கப்படும் input, output, error conditions, memory ownership மற்றும் side effects ஆகியவற்றை ஆவணப்படுத்த வேண்டும். Module ஆவணம் preconditions, postconditions மற்றும் platform limitations-ஐத் தெளிவாகக் குறிப்பிட வேண்டும். Symbol பெயர் அல்லது module reference மட்டும் code execution-க்கு அனுமதி அளிக்காது; உரிய execution boundary-இல் authorization சரிபார்க்கப்பட வேண்டும்.

Compiler pipeline-ஐ அடுக்குகளாகச் சோதிக்கவும்: lexical analysis, parsing, semantic analysis, IR lowering, code generation, VM execution மற்றும் native integration. Regression test-களில் குறைந்தபட்ச மறுஉருவாக்க input, எதிர்பார்க்கப்படும் முடிவு மற்றும் உண்மையான முடிவு இருக்க வேண்டும். தவறான syntax, invalid types, இல்லாத resources மற்றும் ABI mismatch ஆகிய negative cases-ஐச் சேர்க்கவும். தவிர்க்கப்பட்ட அல்லது இயக்கப்படாத tests வெற்றி எனக் குறிப்பிடப்படக்கூடாது.

மீண்டும் உருவாக்கக்கூடிய முடிவுகளுக்காக Git commit, operating system, architecture, compiler version, toolchain மற்றும் துல்லியமான commands-ஐப் பதிவு செய்யவும். ஒரு platform-இல் கிடைத்த முடிவு மற்ற platform-களுக்கான ஆதாரமல்ல. [Platform support matrix](docs/PLATFORM-SUPPORT.md) மற்றும் [testing policy](markdown/TESTING.md)-ஐப் பார்க்கவும்.


---

## Complete technical reference

The [canonical README](README.md) contains the full feature catalogue, language and runtime reference, build and installation instructions, API links, examples, and release notes. This Singapore edition expands the country-language introductions and essential practical guidance; it is not yet a complete line-by-line translation of every technical section. The English README remains the normative source until full translations receive technical review.

- [Canonical README](README.md)
- [SLeeLa syntax specification](SLEELA.syntax)
- [Documentation index](docs/README.md)
- [Platform support](docs/PLATFORM-SUPPORT.md)
- [GitHub repository](https://github.com/mearvk/SLeeLa)

**Translation status:** The four language introductions are provided above. Practical guidance is available in English, while full technical translations into Simplified Chinese, Malay, and Tamil remain outstanding.


## VII. Technical guide in four languages / 四语技术指南 / Panduan teknikal empat bahasa / நான்கு மொழிகளில் தொழில்நுட்ப வழிகாட்டி

### English — compiler, types and runtime
SLeeLa separates source parsing, semantic analysis, intermediate representation, code generation and runtime execution. Compiler release, syntax edition and VM/ISA version are distinct. The normative grammar is [SLEELA.syntax](SLEELA.syntax); the implementation overview is [LANGUAGE.INTERNATIONAL.3.0.md](LANGUAGE.INTERNATIONAL.3.0.md). The pipeline reads UTF-8 `.sleela` files, checks tokens, symbols and types, lowers a structured representation, generates output for supported VM/intermediate targets, and executes it in the runtime.

The documented types include `int`, `double`, `boolean`, `String`, `void`, declared `struct` types and supported reference signatures. `int`, `long` and `int64` are signed 64-bit integers; `int128` remains 64-bit-backed in the documented implementation. A VM-managed struct handle is not a raw pointer or portable network value. Use `structPack(instance)` and `structUnpack(TypeName, json)` for documented serialization. Classes contain fields and methods; inheritance, nested classes and generics are syntax-version gated.

### 简体中文 — 编译器、类型与运行时
SLeeLa 将来源解析、语义分析、中间表示、代码生成与 runtime 执行分成不同阶段。Compiler release、syntax edition 与 VM/ISA version 是不同概念。规范语法见 [SLEELA.syntax](SLEELA.syntax)，实现概览见 [International Language Reference](LANGUAGE.INTERNATIONAL.3.0.md)。编译流程读取 UTF-8 `.sleela` 文件，检查 token、symbol 与 type，将结构化表示 lowering，再为受支持的 VM/中间目标生成代码，最后由 runtime 执行。

已记录的类型包括 `int`、`double`、`boolean`、`String`、`void`、声明的 `struct` 与受支持的 reference signatures。`int`、`long`、`int64` 是 signed 64-bit integers；当前文档中的 `int128` 仍以 64-bit 为基础。VM 管理的 struct handle 不是 raw pointer，也不是可移植网络值。序列化请使用 `structPack(instance)` 与 `structUnpack(TypeName, json)`。Class 包含 fields 与 methods；继承、嵌套类和 generics 受 syntax version 限制。

### Bahasa Melayu — pengkompil, jenis dan runtime
SLeeLa memisahkan parsing sumber, analisis semantik, representasi perantaraan, penjanaan kod dan pelaksanaan runtime. Compiler release, syntax edition dan VM/ISA version ialah konsep berasingan. Tatabahasa normatif terdapat dalam [SLEELA.syntax](SLEELA.syntax); gambaran pelaksanaan terdapat dalam [International Language Reference](LANGUAGE.INTERNATIONAL.3.0.md). Proses kompilasi membaca fail UTF-8 `.sleela`, menyemak token, simbol dan jenis, melakukan lowering representasi berstruktur, menjana kod untuk sasaran VM/perantaraan yang disokong, kemudian melaksanakannya dalam runtime.

Jenis yang didokumenkan termasuk `int`, `double`, `boolean`, `String`, `void`, jenis `struct` dan reference signatures yang disokong. `int`, `long` dan `int64` ialah integer bertanda 64-bit; `int128` masih berasaskan 64-bit dalam pelaksanaan yang didokumenkan. Struct handle yang diurus VM bukan raw pointer atau nilai rangkaian mudah alih. Gunakan `structPack(instance)` dan `structUnpack(TypeName, json)` untuk serialization. Class mempunyai fields dan methods; inheritance, nested classes dan generics tertakluk pada syntax version.

### தமிழ் — தொகுப்பி, வகைகள் மற்றும் runtime
SLeeLa source parsing, semantic analysis, intermediate representation, code generation மற்றும் runtime execution ஆகியவற்றைத் தனித்தனி கட்டங்களாகப் பிரிக்கிறது. Compiler release, syntax edition, VM/ISA version ஆகியவை வேறுபட்டவை. நியமமான இலக்கணம் [SLEELA.syntax](SLEELA.syntax)-இல் உள்ளது; implementation overview [International Language Reference](LANGUAGE.INTERNATIONAL.3.0.md)-இல் உள்ளது. UTF-8 `.sleela` கோப்புகள் வாசிக்கப்பட்டு token, symbol, type சரிபார்ப்புகள் செய்யப்படுகின்றன; பின்னர் intermediate representation lowering, ஆதரிக்கப்படும் VM/இடைநிலை இலக்குகளுக்கான code generation, runtime execution நடைபெறும்.

ஆவணப்படுத்தப்பட்ட வகைகளில் `int`, `double`, `boolean`, `String`, `void`, அறிவிக்கப்பட்ட `struct` மற்றும் ஆதரிக்கப்படும் reference signatures அடங்கும். `int`, `long`, `int64` signed 64-bit integers; ஆவணப்படுத்தப்பட்ட implementation-இல் `int128` இன்னும் 64-bit அடிப்படையிலானது. VM நிர்வகிக்கும் struct handle raw pointer அல்ல; portable network value-யும் அல்ல. Serialization-க்கு `structPack(instance)`, `structUnpack(TypeName, json)` பயன்படுத்தவும். Class-களில் fields, methods உள்ளன; inheritance, nested classes, generics syntax version-ஐப் பொறுத்தவை.

## VIII. Timing, libraries and native boundaries / 計时与库 / Pemasaan dan pustaka / நேரமிடலும் நூலகங்களும்

### English
The annotations `@loopDelay` and `@loopBudget` set loop pauses or a total time budget. They lower to VM instruction `OP_SLEEP` (ISA 157); built-in `sleep(ms)` uses the same instruction. See [SLEELA.syntax §32](SLEELA.syntax) and [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md).

Key modules include `lib/advantage` (model-based evaluation, mathematics, integration, grading and routed output), `lib/xml` (XML DOM, parser and serializer), `lib/text` (character-cell text/string operations), `lib/sleeladoc` (source documentation and CLI reference), `lib/cpu` and `lib/xlt` (CPU profiles and timing/model references), and `recursion/stanchion` (C/C++ recursion prototypes). A module's presence does not prove production readiness; inspect its tests and platform limitations.

The C/C++ integration boundary must specify ABI, calling convention, memory ownership, data layout, error handling and platform dependencies. `@native` and `sleela aot` do not grant general permission to execute arbitrary native code. A reference identifies a resource; it does not authorize execution.

### 简体中文
`@loopDelay` 与 `@loopBudget` 用于设置循环暂停或总时间预算，并会转换为 VM 指令 `OP_SLEEP`（ISA 157）；内建函数 `sleep(ms)` 使用相同指令。参阅 [SLEELA.syntax §32](SLEELA.syntax) 与 [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md)。

主要模块包括 `lib/advantage`（模型评估、数学、整合、分级及路由输出）、`lib/xml`（XML DOM、解析与序列化）、`lib/text`（字符单元文字/字串操作）、`lib/sleeladoc`（来源文件与 CLI 参考）、`lib/cpu`、`lib/xlt`（CPU profiles 与 timing/model references），以及 `recursion/stanchion`（C/C++ 递归原型）。模块存在不等于已可用于生产；请检查测试与平台限制。

C/C++ 边界必须明确 ABI、calling convention、memory ownership、data layout、error handling 与平台相依性。`@native` 和 `sleela aot` 不构成任意执行 native code 的一般授权。Reference 只识别资源，不授予执行权限。

### Bahasa Melayu
Anotasi `@loopDelay` dan `@loopBudget` menetapkan jeda gelung atau belanjawan masa keseluruhan. Kedua-duanya diturunkan kepada arahan VM `OP_SLEEP` (ISA 157); `sleep(ms)` menggunakan arahan yang sama. Lihat [SLEELA.syntax §32](SLEELA.syntax) dan [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md).

Modul utama termasuk `lib/advantage` (penilaian model, matematik, integrasi, penggredan dan output berhala), `lib/xml` (XML DOM, parser dan serializer), `lib/text` (operasi teks/rentetan), `lib/sleeladoc` (dokumentasi sumber dan rujukan CLI), `lib/cpu`, `lib/xlt` (profil CPU dan rujukan model/pemasaan), serta `recursion/stanchion` (prototaip rekursi C/C++). Kehadiran modul tidak membuktikan kesediaan produksi; semak ujian dan had platform.

Sempadan integrasi C/C++ mesti menyatakan ABI, calling convention, memory ownership, data layout, error handling dan kebergantungan platform. `@native` dan `sleela aot` bukan kebenaran umum untuk melaksanakan native code sewenang-wenangnya. Reference mengenal pasti sumber tetapi tidak memberi kebenaran pelaksanaan.

### தமிழ்
`@loopDelay`, `@loopBudget` ஆகியவை loop pause அல்லது மொத்த நேர வரம்பை அமைக்கின்றன. அவை VM instruction `OP_SLEEP` (ISA 157) ஆக மாற்றப்படுகின்றன; built-in `sleep(ms)` அதையே பயன்படுத்துகிறது. [SLEELA.syntax §32](SLEELA.syntax), [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md) பார்க்கவும்.

முக்கிய modules: `lib/advantage` (model evaluation, mathematics, integration, grading, routed output), `lib/xml` (XML DOM, parser, serializer), `lib/text` (character-cell text/string operations), `lib/sleeladoc` (source documentation, CLI reference), `lib/cpu`, `lib/xlt` (CPU profiles, timing/model references), `recursion/stanchion` (C/C++ recursion prototypes). Module இருப்பது மட்டும் production readiness அல்ல; tests மற்றும் platform limitations சரிபார்க்கப்பட வேண்டும்.

C/C++ integration boundary-இல் ABI, calling convention, memory ownership, data layout, error handling, platform dependencies குறிப்பிடப்பட வேண்டும். `@native`, `sleela aot` ஆகியவை arbitrary native code execution-க்கான பொது அனுமதி அல்ல. Reference resource-ஐ அடையாளம் காட்டும்; execution authorization தராது.

## IX. Build, installation and verification / 建置与验证 / Bina dan pengesahan / கட்டமைப்பும் சரிபார்ப்பும்

### English
A native build commonly requires GCC or Clang and GNU Make; some tools use Python 3. Run from the repository root:

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

System installation commonly uses `sudo make install`; user-local installation can use `make install PREFIX="$HOME/.local"`. Read [installation guide](install/README.md) and [platform matrix](docs/PLATFORM-SUPPORT.md). Linux, macOS and Windows do not necessarily share identical dependencies or install procedures.

Hash manifests and verification tools check files against expected content. A PASS applies only to the stated scope and time. Test reports must distinguish pass, fail and skip. Read [testing policy](markdown/TESTING.md), [documentation index](docs/README.md) and [API reference](docs/api/README.md) before contributing. Contributions should include positive and negative tests and record OS, toolchain, commands and actual results.

### 简体中文
Native build 通常需要 GCC 或 Clang 与 GNU Make；部分工具使用 Python 3。在 repository 根目录运行：

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

系统安装通常使用 `sudo make install`；用户本地安装可使用 `make install PREFIX="$HOME/.local"`。请阅读 [installation guide](install/README.md) 与 [platform matrix](docs/PLATFORM-SUPPORT.md)。Linux、macOS、Windows 的依赖与安装流程不一定相同。

Hash manifests 与验证工具可检查文件是否符合预期内容。PASS 仅适用于声明的范围与时间。测试报告必须区分 pass、fail、skip。贡献前请阅读 [testing policy](markdown/TESTING.md)、[documentation index](docs/README.md) 与 [API reference](docs/api/README.md)。贡献应包含正向和负向测试，并记录 OS、toolchain、命令及实际结果。

### Bahasa Melayu
Native build lazimnya memerlukan GCC atau Clang dan GNU Make; sesetengah alat menggunakan Python 3. Jalankan dari root repository:

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

Pemasangan sistem biasanya menggunakan `sudo make install`; pemasangan setempat pengguna boleh menggunakan `make install PREFIX="$HOME/.local"`. Baca [installation guide](install/README.md) dan [platform matrix](docs/PLATFORM-SUPPORT.md). Linux, macOS dan Windows tidak semestinya mempunyai kebergantungan atau proses pemasangan yang sama.

Hash manifests dan alat pengesahan menyemak fail berbanding kandungan yang dijangka. PASS hanya terpakai pada skop dan masa yang dinyatakan. Laporan ujian mesti membezakan pass, fail dan skip. Sebelum menyumbang, baca [testing policy](markdown/TESTING.md), [documentation index](docs/README.md) dan [API reference](docs/api/README.md). Sumbangan perlu menyertakan ujian positif dan negatif serta rekod OS, toolchain, arahan dan hasil sebenar.

### தமிழ்
Native build-க்கு பொதுவாக GCC அல்லது Clang மற்றும் GNU Make தேவை; சில tools Python 3 பயன்படுத்துகின்றன. Repository root-இல்:

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

System install பொதுவாக `sudo make install`; user-local install `make install PREFIX="$HOME/.local"` பயன்படுத்தலாம். [installation guide](install/README.md), [platform matrix](docs/PLATFORM-SUPPORT.md) பார்க்கவும். Linux, macOS, Windows ஆகியவற்றில் dependencies மற்றும் install நடைமுறைகள் ஒன்றாக இருக்க வேண்டியதில்லை.

Hash manifests மற்றும் verification tools கோப்புகளை எதிர்பார்க்கப்படும் உள்ளடக்கத்துடன் ஒப்பிடுகின்றன. PASS என்பது குறிப்பிடப்பட்ட scope மற்றும் நேரத்திற்கே பொருந்தும். Test report pass, fail, skip ஆகியவற்றைத் தனித்தனியாகக் காட்ட வேண்டும். Contribution செய்வதற்கு முன் [testing policy](markdown/TESTING.md), [documentation index](docs/README.md), [API reference](docs/api/README.md) படிக்கவும். Positive/negative tests சேர்த்து OS, toolchain, commands, actual results பதிவு செய்யவும்.

**Translation status:** Singapore now includes practical technical material in English, Simplified Chinese, Bahasa Melayu and Tamil. These sections cover compiler flow, types, objects, timing, libraries, native boundaries, build, installation and validation. They are substantial introductions, not yet line-by-line translations of every canonical README section; technical and language review remains recommended.
