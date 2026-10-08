> Revision 0.43 adds the `amazon` workforce and organization package: 12 new Master Classes plus one `SLPackage` facade, with native C/C++ originals under `/amazon`. Employment actions remain auditable decision support with human review.

# SLeeLa /lib Library Index

**Revision:** 0.43  
**Packages:** 89  
**SLeeLa source units:** 10417  
**Module-facade symbols:** 61  
**Total symbol records:** 10478  
**Symbol manifest:** `LIBRARY.SYMBOLS.md`

> Revision 0.42 adds a **flow/layout manager** to the `user-interface` package.
> Two new one-class-per-file Master Classes: `SLMeasure` (a standardized unit of
> measurement across US customary — in/ft/pt/pica — Eurasian metric — mm/cm/m —
> device px, and relative %/fr/em, resolving to pixels at a DPI) and `SLLayout`
> (the manager: a UI as an orthogonal conflation of unit parts and
> organizations, with named **groups** of named **items** adjusted singly, by
> group, or **n-ary**; several flow solutions — stack, wrap, grid, dock, and
> central weight/mass; Named placement where **MEDIUM** implies center and
> **CENTRAL** implies weight/mass; and named ergonomic presets for the
> ergonomics of publics). Each bottoms out in `measure*`/`layout*` SLVM built-ins
> calling the genuine C ABI (`sleela_ui_layout.h`); see
> `user-interface/LAYOUT.md`. Two new source units: 10,463 -> **10,465** total
> records (10,403 -> **10,405** source classes); facade and package counts
> unchanged at 60 and 88. `LIBRARY.SYMBOLS.md` was regenerated from the live tree
> and the counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.41 adds **moods, washes, a Calculus-8, and millimetre light
> placement** to the `user-interface` package. Four new one-class-per-file
> Master Classes: `SLMm` (place a light/dark/shadow emitter in millimetres
> relative to the text's font-paint location — a HEIGHT it shines down from plus
> left/right/up/down, resolved to pixels at a DPI), `SLCalculus8` (a careful
> 8-stage calculus over **100 differentiable** inputs that maps a light's control
> points into a mood scalar, with an analytic d(mood)/d(input) slope),
> `SLWash` (an Excellent Wash — a soft multi-stop graded colour field), and
> `SLMood` (a named mood = wash + a natural tanor glow for lightless bulbs, with
> a refresh and an "a little left" bias, + a Calculus-8; colours and places
> lights in one call). Each bottoms out in `mm*`/`calc8*`/`wash*`/`mood*` SLVM
> built-ins calling the genuine C ABI (`sleela_ui_mood.h`); see
> `user-interface/MOODS.md`. Four new source units: 10,459 -> **10,463** total
> records (10,399 -> **10,403** source classes); facade and package counts
> unchanged at 60 and 88. `LIBRARY.SYMBOLS.md` was regenerated from the live tree
> and the counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.40 adds the new `gameplay` package — a turn/move system where a
> player makes one of three moves per turn, each resolved by deterministic,
> seeded rolls (so a replayed game, luck included, repeats). Thirteen
> one-class-per-file Master Classes. Core: `DiceRoll` (a seeded MINSTD roller
> giving the 80/20 and 50/50 rolls), `Player` (experience, Person/Citizen
> quality that sets information price, cash, boss understanding),
> `ManagementCourtroom` (banked file/tact/rolls and standing), `MoveOutcome`
> (the result record a move returns and applies to the player), and `GameTurn`
> (the orchestrator). Move (b)'s brain: `InferenceEngine` with `ExpectedFuture`
> (the on-schedule projection on a winning 50/50) and `DetailedReport` (the
> boss's pre-decided Game-Engineering moves on a losing 50/50, priced by player
> quality). Move (c)'s substance: `PortfolioDecision` (a medium bet on a
> realistic industry; a found BIG bet always wins). The three moves subclass a
> `GameMove` base: `HelpIndexMove` (a — help index/friend, 80% then 20% with
> live subevents), `AskForHelpMove` (b — ask for help/new routes, 50/50), and
> `NewDesireMove` (c — create a new desire, review portfolios for profits and
> futures). The base/subclass + override pattern mirrors lib/compiler's
> `SLLanguageCompiler` front ends. A matching `SLPackage` facade names the
> package, and a runnable orchestration example lives outside `/lib` at
> `gameplay/sleela/GameplayDemo.sleela` (so it is not double-counted). One new
> package and fourteen new `.sleela` units: 10,445 -> **10,459** total records
> (10,386 -> **10,399** source classes), 59 -> **60** facades, 87 -> **88**
> packages. `LIBRARY.SYMBOLS.md` was regenerated from the live tree and the
> counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.39 adds **fonts and font effects** to the `user-interface` package.
> Two new one-class-per-file Master Classes: `SLFont` (a face — family, size,
> weight, slant, spacing, line height — plus an ordered effect stack and a
> quality level draft..ultra; draws into an `SLDrawContext` and attaches to a
> text widget via `SLWidget.setFont`) and `SLFontEffect` (one effect: drop/inner
> shadow, glow, directional light sheen, an EMITTER that radiates the text into
> a light scene — a pure emitter reserving nothing, or a source binding an
> anchor — outline, relief emboss/engrave, or gradient fill). Each bottoms out in
> `font*`/`ui*` SLVM built-ins calling the genuine SleelaUI Font C ABI
> (`sleela_ui_font.h`); see `user-interface/FONTS.md`. Two new source units:
> 10,443 -> **10,445** total records (10,384 -> **10,386** source classes);
> facade and package counts unchanged at 59 and 87. `LIBRARY.SYMBOLS.md` was
> regenerated from the live tree and the counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.38 adds the new `alphabet` package — colors, themes, and alphabets,
> thematically important to all users of Sleela. It introduces eleven
> one-class-per-file Master Classes. The color/theme family: `Color` (one RGB
> color with hex, blending, and luminance), `Palette` (an ordered, labeled set
> of Colors), `Theme` (the base that binds a Palette + role colors
> background/foreground/accent + an Alphabet into one user-facing look), and
> three concrete themes `LightTheme`, `DarkTheme`, and `SolarizedTheme`. The
> writing-system family: `Letter` (one glyph with order and vowel flag),
> `Alphabet` (the base writing system a developer extends by overriding
> `compose()`), and three concrete scripts `LatinAlphabet` (26), `GreekAlphabet`
> (24), and `CyrillicAlphabet` (33). The base/subclass + override pattern mirrors
> lib/compiler's `SLLanguageCompiler` front ends. A matching `SLPackage` facade
> names the package, and a runnable orchestration example lives outside `/lib`
> at `alphabet/sleela/AlphabetDemo.sleela` (so it is not double-counted). One new
> package and twelve new `.sleela` units: 10,431 -> **10,443** total records
> (10,373 -> **10,384** source classes), 58 -> **59** facades, 86 -> **87**
> packages. `LIBRARY.SYMBOLS.md` was regenerated from the live tree and the
> counts are verified in lockstep by `test-suites/test-library-inventory.sh`.

> Revision 0.37 sets Sleela's UI **base colour to #2B1608** (a deep warm umber;
> the default `SLUI_THEME_SLEELA_BASE`, configurable at code or config time, with
> the whole warm palette derived from the one base) and adds a **lighting /
> shadow / relief** system for Canvas-aware objects. Three new one-class-per-file
> Master Classes in `user-interface`: `SLLight` (a light descriptor with a KIND,
> a ROLE — SOURCE reserves its anchor object, EMITTER reserves nothing — and a
> POLARITY so both light and shadow are first-class emissions), `SLLightScene`
> (a scene owning the source-reservation table and ambient fill), and
> `SLMaterial` (a relief profile — flat/rounded/bevel/engraved/embossed — plus
> depth, gloss, and occlusion). `SLTheme` gains base-colour hooks (setBaseColor /
> loadConfig). Each bottoms out in `ui*`/`light*` SLVM built-ins calling the
> genuine C ABI (`sleela_ui_light.h`); see `user-interface/LIGHTING.md` and
> `user-interface/THEMING.md`. Three new source units: 10,428 -> **10,431** total
> records (10,370 -> **10,373** source classes); facade and package counts
> unchanged at 58 and 86. `LIBRARY.SYMBOLS.md` was regenerated from the live tree
> and the counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.36 adds motion and a comprehensive drawing surface to the
> **`user-interface`** package: four new one-class-per-file Master Classes.
> `SLThrobber` is a width-adjustable, full-motion, colour-predictive, water-like
> flowing activity field (a fluid shallow-water crest field whose hue leads the
> flow; see `user-interface/THROBBER.md`). `SLCanvasView` is a general animated
> drawing surface whose per-frame draw routine paints with the Draw API.
> `SLDrawContext` binds the comprehensive SleelaUI Draw API
> (`user-interface/include/sleela_ui_draw.h`): owned RGBA surfaces, single/double
> buffering, pixel-level get/set/blend, blend modes, a clip stack, and the full
> primitive set (rects, rounded rects, lines, circles, ellipses, arcs,
> triangles, gradients, text, blits). `SLFrameClock` binds the refresh-rate
> controller (target FPS, delta time, measured rate, sleep hint, fixed-step).
> Each bottoms out in `ui*`/`draw*` SLVM built-ins calling the genuine C ABI;
> see `user-interface/DRAW-API.md`. Four new source units: 10,424 -> **10,428**
> total records (10,366 -> **10,370** source classes); facade and package counts
> unchanged at 58 and 86. `LIBRARY.SYMBOLS.md` was regenerated from the live tree
> and the counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.35 expands the **`user-interface`** package with the full SleelaUI™
> widget collection: twenty-two new one-class-per-file Master Classes binding
> the toolkit's expanded native widget set. Selection/boolean: `SLCheckBox`,
> `SLRadioButton`, `SLComboBox`, `SLSpinButton`. Indicators: `SLProgressBar`,
> `SLLevelBar`, `SLSpinner`, `SLScrollBar`. Containers: `SLFrame`, `SLCard`,
> `SLGrid`, `SLStatusBar`. Display/feedback: `SLImage`, `SLAvatar`, `SLBadge`,
> `SLChip`, `SLHeading`, `SLTooltip`, `SLInfoBar`. Text variants: `SLLinkButton`,
> `SLSearchEntry`, `SLPasswordEntry`. Each bottoms out in a `ui*` SLVM built-in
> calling the SleelaUI C ABI (`user-interface/include/sleela_ui.h`); the full
> catalogue is tabulated in `user-interface/WIDGETS.DESCRIPTION.md` and the class
> roster + `ui*` surface in `lib/user-interface/USER-INTERFACE.md`. Twenty-two
> new source units: 10,402 -> **10,424** total records (10,344 -> **10,366**
> source classes); facade and package counts unchanged at 58 and 86.
> `LIBRARY.SYMBOLS.md` was regenerated from the live tree and the counts are
> verified in lockstep by `test-suites/test-library-inventory.sh`.

> Revision 0.34 adds the **`user-interface`** package: the SLeeLa-source binding
> for **SleelaUI™**, SLeeLa's own original cross-platform UI toolkit (native
> toolkit under `/user-interface`; not GTK/Qt). It gains fifteen one-class-per-
> file Master Classes — the capstone `SLUserInterface`, `SLWindow`, the widget
> base `SLWidget`, containers `SLBox`/`SLHeaderBar`, controls `SLLabel`/
> `SLButton`/`SLToggle`/`SLEntry`/`SLSlider`/`SLSeparator`/`SLSpacer`, the
> palette pair `SLTheme`/`SLColor` (default **Slick Black**), and the runnable
> `SLGalleryDemo` — plus the `SLPackage` facade. Every method bottoms out in a
> `ui*` SLVM built-in that calls the genuine SleelaUI C ABI
> (`user-interface/include/sleela_ui.h`), which drives X11 / Cocoa / Win32; the
> `ui*` surface and the class roster are documented in
> `lib/user-interface/USER-INTERFACE.md`. Sixteen new source units (1 facade +
> 15 sources): 10,386 -> **10,402** total records (10,329 -> **10,344** source
> classes), 57 -> **58** facades, 85 -> **86** packages. `LIBRARY.SYMBOLS.md`
> was regenerated from the live tree and the counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.33 copies the fourteen `character` business-model Master Classes
> into a new nested sub-area, `lib/citizen/character/`, so the `citizen` package
> carries its own in-place copy of `Character`, `BusinessModel`, and the twelve
> concrete models (`SubscriptionModel`, `FreemiumModel`, `MarketplaceModel`,
> `AdvertisingModel`, `RetailModel`, `WholesaleModel`, `FranchiseModel`,
> `LicensingModel`, `SaaSModel`, `ConsultingModel`, `ManufacturingModel`,
> `BrokerageModel`). The sources are unchanged apart from their header path
> comments. Following the one-facade-per-package convention (see
> `lib/compiler/frontends/*`, which hold source classes but no nested
> `SLPackage`), the copied `SLPackage` facade was NOT duplicated into the
> sub-area — the `citizen` package keeps its single root facade. Fourteen new
> source units, all attributed to the `citizen` package: 10,372 -> **10,386**
> total records (10,315 -> **10,329** source classes); facade and package counts
> unchanged at 57 and 85. `LIBRARY.SYMBOLS.md` was regenerated from the live
> tree and the counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.32 adds two new packages — `citizen` and `character` — together.
>
> The `citizen` package is the civic layer of the Republic of Sleela. It
> introduces four one-class-per-file Master Classes: `Citizen` (one honest,
> taxpaying, coffee-drinking person who is, famously, able), `Industry` (their
> place across the whole economy, sector by sector), `BankAccount` (the banking
> relationship, with a 9-digit ABA routing number and account number), and
> `FederalReserveID` (the central-bank clearing identity that maps money to one
> of the twelve Federal Reserve Districts). A runnable example lives outside
> `/lib` at `citizen/sleela/CitizenDemo.sleela`.
>
> The `character` package is a classful study in business models. It introduces
> fourteen one-class-per-file Master Classes: `Character` (a named actor who
> adopts exactly one model and lives or dies by its monthly profit),
> `BusinessModel` (the language-neutral base contract a developer extends,
> following the same base/subclass pattern as lib/compiler's
> `SLLanguageCompiler` front ends), and twelve concrete models that each override
> the revenue/cost math — `SubscriptionModel`, `FreemiumModel`,
> `MarketplaceModel`, `AdvertisingModel`, `RetailModel`, `WholesaleModel`,
> `FranchiseModel`, `LicensingModel`, `SaaSModel`, `ConsultingModel`,
> `ManufacturingModel`, and `BrokerageModel`. A `Character` holds its model
> through a single base-typed reference, so any one of the twelve plugs in
> interchangeably and answers the same question — did we make money? — with
> entirely different arithmetic. A runnable example lives outside `/lib` at
> `character/sleela/CharacterDemo.sleela`.
>
> Each package ships a matching `SLPackage` facade, and the runnable examples
> live outside `/lib` so they are not double-counted in the inventory. Two new
> packages and twenty new `.sleela` units: 10,352 -> **10,372** total records
> (10,297 -> **10,315** source classes), 55 -> **57** facades, 83 -> **85**
> packages. `LIBRARY.SYMBOLS.md` was regenerated from the live tree and the
> counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.31 promotes the emblematic Skya telephony modules into first-class
> `/lib` Master Classes. The `telephony-skya` package gains six one-class-per-
> file source units — `Socio` (social fabric), `Network` (transport
> reachability), `Servers` (server-side presence), `Communication` (message
> exchange), `RealAcquaintances` (confirmed trust roster), and the `SkyaModules`
> loader — so they are counted and discoverable as Master Classes rather than
> living only as facade stubs or top-level runnables. The matching facade stubs
> were removed from `telephony-skya/SLPackage.sleela` to avoid duplicate class
> declarations. Six new source units: 10,346 -> **10,352** total records (10,291
> -> **10,297** source classes); package count unchanged at 83.
> `LIBRARY.SYMBOLS.md` was regenerated from the live tree and the counts are
> verified in lockstep by `test-suites/test-library-inventory.sh`.

> Revision 0.30 reconciles the `/lib` inventory to the live tree after the
> **modular multi-language compiler framework** and the operating-system
> system-call work. The `compiler` package gains the language-neutral framework
> (`SLLanguageCompiler`, `SLCompileRequest`, `SLCompilePlan`,
> `SLCompilerRegistry`) and seven modular front ends under `frontends/<lang>/`
> (C, C++, Java, Python, JavaScript, Rust, Go), each an independent front end
> that lowers its own language toward the common SLeeLa IR; see
> `lib/compiler/MULTI-LANGUAGE.FRAMEWORK.md`. This revision also folds in four
> previously-uncounted units (`os/SLLinuxOS`, `os/SLMacOS`, `os/SLWindowsOS`,
> `vm/SleelaVMSystemCallBridge`). Sixteen `.sleela` units in total: 10,330 ->
> **10,346** total records (10,275 -> **10,291** source classes); package count
> unchanged at 83. `LIBRARY.SYMBOLS.md` was regenerated from the live tree
> (`collection-revision: 2.1`) and the counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.29 adds `SLGuestExecutable`: a simple on-disk guest executable
> format (a miniature ELF - magic header + entry + opcode section). A compiled
> program serializes to it, is written to the guest filesystem, and is loaded
> back FROM DISK into the substrate (header validated) - so guest programs come
> from storage, not only inline compilation. SLGuestInit gains execFromImage()/
> installAndExec() (compile -> install -> load-from-disk -> run). 10,274 ->
> **10,275** source classes (**10,330** total). See `cpu/EXECUTION.md`.

> Revision 0.28 adds `SLGuestShell`: a minimal interactive shell spawned by
> init as its first child (builtins help/echo/ls/cat/ps/exit), issuing every
> command through the guest syscall ABI (console write, file read, getpid,
> exit). It is the guest's first interactive user program and exercises the full
> user->kernel boundary. 10,273 -> **10,274** source classes (**10,329** total).
> See `cpu/EXECUTION.md`.

> Revision 0.27 adds guest IPC (2 classes): `SLGuestPipe` (a unidirectional
> byte FIFO between tasks with would-block/EOF semantics) and `SLGuestSignal`
> (asynchronous signal delivery with per-task pending sets; default actions for
> SIGKILL/SIGTERM terminate the target). SLGuestSyscall gains pipe()/kill() plus
> pipe-fd read/write routing; SLGuestVM delivers pending signals at scheduling
> points. 10,271 -> **10,273** source classes (**10,328** total). See
> `cpu/EXECUTION.md`.

> Revision 0.26 adds `SLGuestSyscall`: the guest kernel's syscall ABI +
> dispatcher (write/read/open/close/brk/yield/getpid/exit). Guest service-opcode
> VM-exits are now routed through it by SLGuestVM.handleExit() and emulated
> against the guest console, filesystem, and scheduler, with the result pushed
> back for the guest to resume (SYS_EXIT finishes the running task). This
> completes the user->kernel boundary. 10,270 -> **10,271** source classes
> (**10,326** total). See `cpu/EXECUTION.md`.

> Revision 0.25 adds `SLGuestFileSystem`: a minimal in-guest filesystem over
> the guest block device - a flat directory of files and an open-file-descriptor
> table with sequential read/write, mounted by SLGuestLinux on the block device.
> It backs the guest open/read/write/close syscalls. 10,269 -> **10,270** source
> classes (**10,325** total). See `cpu/EXECUTION.md`.

> Revision 0.24 adds guest user-space bring-up (2 classes): `SLGuestProgram`
> (a loadable guest *user* program compiled from source via the shared toolchain)
> and `SLGuestInit` (the guest init process, PID 1, which loads programs into the
> guest substrate and admits them as scheduler tasks). The guest now goes from
> "kernel running" to "running user processes" that the timer-driven scheduler
> multitasks. 10,267 -> **10,269** source classes (**10,324** total). See
> `cpu/EXECUTION.md`.

> Revision 0.23 completes the guest OS-VM run loop and multitasking (OS-VM
> roadmap steps 5-6). Step 5 (code changes only) wires VM-exit routing: hosted
> SLSleelaVM parks service opcodes as VM-exits that SLGuestVM routes to the
> device model and resumes. Step 6 adds 2 classes - `SLGuestTask` and
> `SLGuestScheduler` - giving the guest kernel timer-driven round-robin
> preemptive multitasking (the SLGuestTimer IRQ is the scheduler tick; task
> context = saved substrate IP). 10,265 -> **10,267** source classes (**10,322**
> total). See `cpu/EXECUTION.md`.

> Revision 0.22 lets the guest OS-VM boot a GENUINELY COMPILED kernel (2
> classes): `SLGuestKernelImage` (a bootable opcode-stream image + entry) and
> `SLGuestKernelBuilder` (compiles kernel source in any supported language
> through SLCompilerDriver -> register ISA -> SLOpcodeMap -> opcode image). The
> guest substrate loads and executes the compiled image instead of a hand-
> written stub (OS-VM roadmap step 4; a custom kernel, not upstream Linux).
> 10,263 -> **10,265** source classes (**10,320** total). See `cpu/EXECUTION.md`.

> Revision 0.21 adds a guest device model to the `cpu` package (5 classes):
> `SLGuestDevice` (MMIO base), `SLGuestTimer` (programmable interval timer +
> IRQ), `SLGuestConsole` (serial/console output), `SLGuestBlockDevice`
> (virtio-blk-style storage over a host drive), and `SLGuestDeviceBus` (MMIO
> routing + IRQ aggregation into the guest interrupt controller). The guest
> OS-VM now has the timer/console/block surface a kernel needs (OS-VM roadmap
> step 3). 10,258 -> **10,263** source classes (**10,318** total). See
> `cpu/EXECUTION.md`.

> Revision 0.20 adds `SLMMU` to the `cpu` package: a Memory Management Unit with
> single-level page tables, per-page present/write/exec permissions, and page
> faults (unmapped + protection), giving the guest OS-VM real second-level
> address translation (OS-VM roadmap step 2). 10,257 -> **10,258** source
> classes (**10,313** total). See `cpu/EXECUTION.md`.

> Revision 0.19 maps the `cpu` model onto the canonical SLeeLa opcode substrate
> (4 new classes): `SLSleelaOpcode` (the base-98 Turing-complete opcode registry
> with exact native codes), `SLSleelaVM` (a stack machine executing those
> opcodes directly, mirroring the native dispatch loop), `SLOpcodeMap` (maps the
> register ISA and OS system calls onto Sleela-opcode sequences), and
> `SLTuringBridge` (reduces any-language programs to the substrate and proves
> C/C++/Java/Sleela are 1:1 in Turing effect). This takes the collection from
> 10,253 to **10,257** source classes (**10,312** total records); package count
> unchanged at 83. See `cpu/EXECUTION.md` §4. Counts verified by
> `test-suites/test-library-inventory.sh`.

> Revision 0.18 expands the `cpu` package with a real multi-language toolchain
> and a virtualization layer (13 new classes): a hardware stack (`SLStack`) and
> an expanded `SLInstructionSet`/`SLControlUnit` with a full CALL/RET/PUSH/POP
> protocol and MUL/DIV/MOD; a common IR (`SLIRInstruction`, `SLIR`), a shared
> backend (`SLLowering`), language frontends (`SLFrontend`, `SLFrontendSleela`,
> `SLFrontendC`, `SLFrontendCpp`, `SLFrontendJava`), and a `SLCompilerDriver` so
> the ISA accepts C/C++/Java/Sleela; plus a hosted hypervisor (`SLHypervisor`,
> `SLGuestVM`, `SLGuestLinux`) modeling a Linux-style guest on the Sleela CPU.
> This takes the collection from 10,240 to **10,253** source classes (**10,308**
> total records); package count is unchanged at 83. See `cpu/EXECUTION.md`.
> Counts are verified by `test-suites/test-library-inventory.sh`.

> Revision 0.17 adds the new `cpu` package family — a complete CPU, operating
> system, and program stack in SLeeLa source (45 classes): logic gates, adders,
> an ALU, latches/flip-flops/registers/register file, RAM/cache/hard drive,
> clock/bus/program counter, an instruction set + decoder + control unit,
> interrupt controller, multi-core `SLCPU`, I/O ports/bus/DMA and the device
> driver hierarchy, and the OS layer (bootloader, memory manager, scheduler,
> processes, syscalls, filesystem, kernel) up to `SLProgram`/`SLAssembler`/
> `SLProgramLoader` and the capstone `SLMachine`. This takes the collection from
> 82 to **83** package families and from 10,195 to **10,240** source classes
> (**10,295** total records). See `cpu/CPU.md`. `LIBRARY.SYMBOLS.md` was
> regenerated and the counts are verified by `test-suites/test-library-inventory.sh`.

> Revision 0.16 adds the national-grade cryptography façades to the `crypto`
> package family — `SLNationalSuite`, `SLSha256`, `SLSha512`, `SLSha3`, `SLAes`,
> `SLAesGcm`, `SLHkdf`, `SLMlKem`, and `SLMlDsa` — the SLeeLa-layer front ends
> for the native suite implemented under `/crypto` (NSA CNSA / NIST FIPS: AES,
> SHA-2/SHA-3, AES-GCM, HMAC, HKDF, and the ML-KEM/ML-DSA post-quantum
> interfaces). Nine new source units take the collection from 10,186 to **10,195**
> source classes (**10,250** total records). `LIBRARY.SYMBOLS.md` was
> regenerated from the live tree and the counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.15 fully regenerates `LIBRARY.SYMBOLS.md` from the live `/lib`
> tree via `tools/generate-library-symbols.py` (one row per `.sleela` unit)
> and reconciles every count to the filesystem: **82** package families,
> **10,186** source classes + **55** `SLPackage.sleela` module facades =
> **10,241** total symbol records. This picks up the package families that had
> been added since the manifest was last written — `autocad`, `java`,
> `languages`, `opcodes`, `sldocument`, and `website` — and corrects the earlier
> internally inconsistent header (which claimed 10,054 sources / 90 facades over
> a 982-row body). The manifest body and header are now verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.14 reconciled the headline source-unit figure to the verified
> filesystem count of 10,241 `.sleela` units under `/lib` (matching
> `CLASS.INVENTORY.md` Revision 2.1). Earlier revision stamps lagged the live
> count; the figure here is the authoritative total.

> Revision 0.9 adds the new `opcodes` package family: one SLeeLa class per
> canonical VM opcode (103 classes, codes 0–102; the base 98 are OP_NOP..
> OP_AUDIO_PLATFORM) plus `SLOpcodeBase` and `SLOpcodeStream` — 105 `.sleela`
> source units. Each class carries a single opcode and honours the fetch-then-
> execute-one contract against the VM. See `opcodes/OPCODES.md`.
>
> Revision 0.10 adds the `opcodes/governance` series — 8 classes giving SLeeLa
> procedural discretion over opcode execution: a Registrar considers a program
> A→B BEFORE it runs, a Listener confirms live fit DURING, and an Event Observer
> judges the whole as a musical, ordered process AFTER (with graded verdicts,
> base-concept checks, and known-symbol-map patching). See
> `opcodes/governance/GOVERNANCE.md`.
>
> Revision 0.11 adds the `opcodes/running` sub-family — 6 classes for richer
> opcode execution: grouping (`SLOpcodeGroup`, `SLOpcodeGroupSet`), a
> conditional-reactive layer (`SLOpcodeCondition`, `SLOpcodeConditionalReactive`,
> `SLOpcodeReactorBank`) that warms/gates/runs/skips groups on program state, and
> warming (`SLOpcodeWarmer`). See `opcodes/running/RUNNING.md`.
>
> Revision 0.12 adds the new `sldocument` package family and the `.sldocument`
> format — 6 classes for an ordered, top-down document that compiles against and
> with standard SLeeLa source (`SLDocument`, `SLDocumentStep`,
> `SLDocumentAnnotation`, `SLVeritable`, `SLDocumentResult`,
> `SLDocumentCompiler`). Each step is an annotated method that runs in order and
> usually returns a single binary veritable-and-kind value. See
> `sldocument/SLDOCUMENT.md`.
>
> Revision 0.13 makes `.sldocument` a selectable compile choice and adds naming
> conventions for comparing/converting the forms: `lib/compiler/SLSourceForm` and
> `lib/compiler/SLCompileChoice` let the compiler be told to compile a `.sleela`
> or a `.sldocument`; and `sldocument/SLDocumentNaming`,
> `sldocument/SLSourceNameComparison`, and `sldocument/SLDocumentConverter`
> synthesize method names for anonymous document steps so an engineer can convert
> a `.sldocument` to a named `.sleela` for safekeeping. 5 new `.sleela` classes.

The `/lib` tree is the canonical language-facing source collection. The compiler and Nordshrift use the same recursive library discovery implementation, so a package becomes importable when its directory contains SLeeLa source.

| Coverage | Count |
|---|---:|
| Repository module families represented under /lib | 83 |
| SLeeLa source units | 10,297 |
| Module-facade symbols | 55 |
| Total symbol records | 10,352 |

Every repository-level module family that is a language/runtime/package concern now has at least one SLeeLa source unit under `/lib`. Documentation, images, generated build output, tests, and CI-only directories remain non-library artifacts and are intentionally not presented as language packages.

## Compiler and Loader Resolution

Resolution is shared by the SLeeLa compiler and Nordshrift:

1. `$SLEELA_LIB`
2. `lib`
3. `../lib`
4. `../../lib`

Discovery is recursive. Package presence is therefore derived from the actual `/lib` tree rather than a hand-maintained package allow-list. `validateImports()` rejects an imported package that is not present.

The VM-facing `SLVMModuleLoader` source mirrors this contract: package names are discovered from the canonical library root, registered, and checked before use. Native loader facilities remain below the explicit C/C++ OS bridge.

## Inventory

The complete path-level and facade-level symbol collection is maintained in `LIBRARY.SYMBOLS.md`.

**Max Rupplin — MEARVK LLC — 2026**


## Compiler / Nordshrift / Loader Contract

The shared `sleela::library::Index` recursively discovers the 83 package families and all 10,352 `.sleela` source units. It now exposes package counts, per-package symbol counts, symbol lookup, and source-path resolution. Compiler and Nordshrift use this index; `lib/vm/SLVMModuleLoader.sleela` represents the same discovered package/symbol state at the SLeeLa layer.

## SST / Nordshrift Symbol Contract

SST declarations are typed symbols with package-qualified identity. Nordshrift resolves them through the same `sleela::library::Index` used by the compiler; `/lib` remains the sole authoritative symbol collection. See `impl/nordshrift/SST.SYMBOLS.md` and `impl/nordshrift/NORDSHRIFT.SYMBOLS.md`.
