# SLeeLa 3.0 — Pilipinas (PH)

## Filipino · English

Ang SLeeLa ay isang katutubong wika sa pagprograma ng sistema na may compiler, runtime, at ekosistema ng mga virtual machine. Pinagsasama nito ang wika, compiler, mga library, tool para sa developer, at mga kasunduan ng platform upang makabuo at magpatakbo ng software.

**The Now. The Ingredient.** Hindi lamang syntax ang isang programming language. Bahagi rin ng isang magagamit na software system ang mga toolchain at runtime nito.

## I. Magsimula rito

- [Buuin ang native implementation](impl/README.md)
- [Gabay sa virtual machine at Modium](virtual-machine/USER-GUIDE.md) — `sleela vm`, `sleela modium`
- [Arkitektura ng CPU sa SLeeLa source](lib/cpu/CPU.md) — mahigit 130 CPU profile; tingnan ang [mga timing control](lib/cpu/TIMING.md).
- [Indeks ng dokumentasyon](docs/README.md)
- [Nabuong dokumentasyon ng API](docs/api/README.md) at [katalogo ng API](api/README.md)
- [Suporta sa platform at validation](docs/PLATFORM-SUPPORT.md)
- [Patakaran sa testing](markdown/TESTING.md)

## II. Pagbuo at pagsubok

Kabilang sa mga kailangan ang C/C++ toolchain (GCC o Clang), GNU Make, at Python 3. Maaaring mangailangan din ang mga graphical component sa Linux ng X11/Xrandr development package.

Patakbuhin mula sa root directory ng repository:

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

Hindi nangangahulugan ang matagumpay na build ng isang component na handa na para sa release ang lahat ng library o platform. Tingnan ang dokumentasyon ng build at platform para sa mga requirement ng bawat target.

## III. Pag-install

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

Karaniwang `/usr/local` ang system installation prefix. Sa paggamit ng `PREFIX="$HOME/.local"`, makakapag-install ang user sa sariling directory at karaniwang hindi kailangan ang administrator privileges. Para sa gabay sa installation at detalye ng package, tingnan ang [installation guide](install/README.md).

## IV. Wika at runtime

Nasa [`SLEELA.syntax`](SLEELA.syntax) ang normative grammar. Syntax version **3.0** ang ginagamit sa dokumentasyon ng repository. Magkaiba ang compiler version at source-language syntax version.

Kabilang sa mga teknikal na paksa ang mga contiguous field na `@mix`, pinalawak na pamilya ng integer, Preonem document marker, SleelaDoc, API documentation generation, at mga tool ng Nordshrift. Hindi binabago ang mga technical identifier, command, filename, API name, at code example upang magamit nang direkta.

## V. Mga prinsipyo sa engineering at validation

Binibigyang-diin ng SLeeLa ang ebidensiya, malinaw na kontrata sa pagitan ng mga component, portability, ligtas na pag-uugali kapag may failure, at reproducible release. Dapat suriin ang validation ayon sa module at platform. Hindi dapat iulat na pumasa ang mga test na nilaktawan; hindi rin katumbas ng buong release validation ang matagumpay na build ng iisang component.

Kabilang sa Sleelavirin protection tooling ang lokal na SHA-256 verifier, opsyonal na ClamAV adapter, at integrity manifest. Maituturing lamang na validated ang isang release kapag kumpleto na ang CI checks para sa release commit at nasuri na ang mga artifact.

## VI. Mga editor at kontribusyon

May mga theme at syntax integration ang SLeeLa para sa **Vim** at **Emacs**, pati native C/C++ palette utility. Tingnan ang [gabay sa editor integration](linux-editors/README.md).

Mga mungkahi para sa kontribusyon:

1. Basahin ang documentation index at testing policy.
2. Gumawa ng malinaw ang saklaw na pagbabago at magsama ng positive at negative test.
3. Patakbuhin ang mga kaugnay na test, pagkatapos ay ang mas malawak na test suite.
4. Ilahad ang operating system at toolchain na ginamit. Huwag sabihing pumasa ang mga check na nilaktawan.

Binuo ang SLeeLa ng **MEARVK LLC**.

## VII. Iba pang teknikal na paksa

Tinatalakay rin ng pangunahing README ang mga library ng Advantage, SleelaDoc, at XML DOM; mga pangunahing integer type; character runtime; AutoCAD Creator™; eksperimental na UTF-4088 catalogue; mga tool na Pi, Modium, Registry, at `sleelactl`; at ang unang yugto ng native C/C++ compilation path na pinapagana ng `@native` marker.

## Kumpletong teknikal na sanggunian

Nasa [canonical README](README.md) ang buong feature catalogue, sanggunian sa wika at runtime, mga tagubilin sa build at installation, mga link sa API, halimbawa, at release notes. Pinalawak ng edisyong Filipino ang praktikal na gabay, ngunit hindi pa ito kumpletong salin, bawat linya, ng lahat ng teknikal na seksyon. Mananatiling normative technical source ang English README hanggang masuri ang buong salin.

- [Canonical README](README.md)
- [SLeeLa syntax specification](SLEELA.syntax)
- [Documentation index](docs/README.md)
- [Platform support](docs/PLATFORM-SUPPORT.md)
- [GitHub repository](https://github.com/mearvk/SLeeLa)

**Katayuan ng salin:** Isinalin ang panimula, pagsisimula, build, installation, pangkalahatang paliwanag sa wika at runtime, mga prinsipyo sa engineering, validation, at kontribusyon. Kailangan pang isalin at repasuhin ng mga teknikal na tagasuri ang lahat ng natitirang seksyon.


## V. Compiler, representasyon, at virtual machine

Hinahati ng SLeeLa ang compilation sa source language, frontend analysis, intermediate representation, code generation, at runtime execution. Magkakaibang konsepto ang compiler version, syntax version, at VM/ISA version; suriin ang naaangkop na bersiyon sa source at specification bago ipalagay na available ang isang feature.

Karaniwang bahagi ng toolchain ang:

1. **Source code:** mga `.sleela` file na may language version at declarations.
2. **Lexer at parser:** ginagawang tokens at syntax structure ang source text.
3. **Semantic analysis:** sinusuri ang symbols, types, at mga tuntunin ng wika.
4. **Intermediate representation:** inihahanda ang program para sa mga susunod na lowering step.
5. **Code generation:** lumilikha ng output para sa mga VM o intermediate target na suportado.
6. **Runtime:** nagpapatakbo ng compiled content gamit ang mga mekanismo ng memory at control flow.

Maaaring magbago ang eksaktong kakayahan ayon sa kasalukuyang implementation. Hindi awtomatikong nangangahulugan na suportado sa lahat ng target ang isang feature dahil lamang nakalista ito sa specification.

## VI. Mga feature ng wika at library

Kabilang sa mga dokumentadong bahagi ng SLeeLa ang:

- **Advantage** (`lib/advantage/`) — layered model para sa pagsusuri ng resulta kaugnay ng isang modelo; saklaw nito ang bilateral trade economics, vertical integration, module grading, at routable output. Ang physics analogy sa dokumentasyon ay dapat unawain bilang modeling analogy, hindi bilang pahayag ng pisikal na batas.
- **XML DOM** (`lib/xml/`) — XML document model na may element, text, comment, CDATA, processing-instruction nodes, attributes, parsing, at serialization.
- **Text/String** (`lib/text/`) — mga character-cell text operation gaya ng `charAt`, `substring`, `indexOf`, `reverse`, at positional formatting.
- **SleelaDoc** (`lib/sleeladoc/`) — pagkuha ng documentation at reference pages. Maaaring gamitin ang `sleela docs` para mag-browse ng topics, packages, at classes; lumilikha naman ang `sleela docs --generate [dir]` ng lokal na text corpus.
- **CPU at XLT** (`lib/cpu/`, `lib/xlt/`) — architecture documentation, CPU profiles, components, models, at timing.
- **Recursion stanchion** (`recursion/stanchion/`) — C/C++ reference prototypes at mga halimbawa ng recursion patterns.

Basahin ang README ng bawat module para malaman ang saklaw, limitasyon, at aktuwal na halimbawa. Hindi garantiya ng production readiness ang pagkakaroon ng entry sa catalog.

## VII. Mga annotation, type, at syntax

Ilan sa mga feature na inilalarawan sa dokumentasyon:

- Itinatakda ng `@loopDelay` at `@loopBudget` ang pause at time budget ng loops. Ibinababa ang mga ito sa VM instruction na `OP_SLEEP` (ISA 157), na ginagamit din ng built-in na `sleep(ms)`. Tingnan ang [SLEELA.syntax §32](SLEELA.syntax) at [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md).
- Pinapangkat ng `@mix` ang mga instance field na may parehong pangalan bilang indexed sequence kapag nilagyan ng annotation ang bawat field.
- Ang Preonems — `author`, `authorial`, `material;`, `memory;`, `determine;`, `object;`, at `field;` — ay declarative marker para sa status at pinagmulan ng dokumento. Hindi sila pahintulot na magpatakbo ng code.
- Gumagamit ang angle-bracket comments ng mga anyong `< line >` at `<< block >>`.
- Sinusuportahan ng integer literals ang hexadecimal (`0x`), binary (`0b`), at octal (`0o`) notation, kasama ang digit separator na `_`.
- Ang `int`, `long`, at `int64` ay 64-bit signed integer. Tinatanggap ang `int128` bilang integer type ngunit 64-bit pa rin ang kasalukuyang batayan nito; huwag itong ituring na ganap na 128-bit arithmetic.

Para sa normative rules at version gates, gamitin ang [SLEELA.syntax](SLEELA.syntax).

## VIII. C, C++, at native integration

May mga C at C++ component ang repository at may limitadong native path. Kaugnay ng pagbuo ng native code ang `@native` marker at `sleela aot` command; hindi nangangahulugan ang mga ito ng pangkalahatang pahintulot na magpatakbo ng arbitrary native code.

Dapat malinaw ang interface boundary kapag ikinokonekta ang SLeeLa sa C o C++. Kailangang idokumento at subukan ang ABI contracts, memory ownership, data types, error handling, at platform dependencies. Ituring na hiwalay na trust boundary ang native code. Ang reference ay pagkakakilanlan lamang ng resource, hindi pahintulot na patakbuhin ito.

Tingnan ang [native implementation](impl/README.md), [platform matrix](docs/PLATFORM-SUPPORT.md), at [comparison standards](comparison/README.md).

## IX. Integridad, seguridad, at validation

Binibigyang-diin ng SLeeLa ang traceability sa pagitan ng source code, documentation, build process, at test results. Maaaring gamitin ang hash manifest at verification tools upang suriin kung tumutugma ang mga file sa inaasahang content. Ang matagumpay na check ay para lamang sa saklaw at oras kung kailan ito isinagawa.

Mahahalagang prinsipyo:

- Tinutukoy ng references ang resources; hindi sila awtomatikong nagbibigay ng pahintulot sa execution.
- Dapat tahasan at limitado ang native interfaces.
- Dapat magkahiwalay na iulat ang pass, fail, at skip.
- Dapat suportado ng aktuwal na build at tests ang mga claim tungkol sa platform support.
- Dapat matunton ang generated documentation sa source at generator.
- Dapat fail-closed ang security checks kapag wala ang kinakailangang integrity information.

Tingnan ang [testing policy](markdown/TESTING.md), [platform support](docs/PLATFORM-SUPPORT.md), at [installation guide](install/README.md).

## X. Mga editor at kontribusyon

Tingnan ang [Linux editor integrations](linux-editors/README.md) para sa mga resource ng Vim at Emacs. Tiyaking tugma ang mga tagubilin sa operating system at bersiyon ng editor bago mag-install.

Inirerekomendang proseso ng kontribusyon:

1. Basahin ang documentation index at README ng module na babaguhin.
2. Limitahan ang saklaw ng pagbabago at ipaliwanag ang dahilan.
3. Magdagdag ng positive at negative tests.
4. Patakbuhin ang kaugnay na tests at, kung kailangan, ang buong test suite.
5. Itala ang OS, toolchain, commands, at aktuwal na resulta.
6. I-update ang README at API documentation kapag nagbago ang public behavior.
7. Huwag markahang passed ang tests na nilaktawan o hindi pinatakbo.

## XI. Iba pang bahagi ng proyekto

Kasama rin sa SLeeLa repository ang CPU at hardware models, graphical interfaces, networking components, text processing, documentation, internationalization, at experimental catalogs. Maaaring magkakaiba ang maturity ng mga bahaging ito. Huwag ilarawan ang experimental catalog o halimbawa bilang stable production feature kung walang dokumentadong validation.

May magkakahiwalay na entry point at technical requirement ang UTF-4088 catalog, π tools, Modium, Registry, at `sleelactl`. Tingnan ang canonical README at documentation ng bawat module para sa kasalukuyang status.

## XII. AutoCAD Creator™

Nakalista ang AutoCAD Creator™ sa technical catalog ng proyekto. Basahin ang documentation ng kaugnay na module upang maihiwalay ang mga inilalarawang capability, halimbawa, at verified result. Huwag ipagpalagay ang compatibility sa isang partikular na bersiyon ng AutoCAD kung walang tahasang compatibility matrix at test result.

## XIII. UTF-4088 experimental catalog

Ang UTF-4088 materials ay isang experimental na bahagi ng character at catalog work ng proyekto. Dapat unawain ang laki, dami ng entries, memory requirement, at format details kasabay ng dokumentasyon at kasalukuyang implementation. Hindi patunay ng full Unicode compatibility o production readiness ang malaking bilang ng entries lamang.

## XIV. π, Modium, Registry, at sleelactl

May magkakahiwalay na tools at documentation area ang proyekto para sa π generation, Modium, Registry, at `sleelactl`. Magkakaiba ang layunin ng mga ito; sundin ang kani-kanilang build at validation instructions. Huwag ipagkamali ang pagkakaroon ng dokumentasyon bilang patunay ng production status.

## XV. Mga pangunahing sanggunian

- [Canonical README sa English](README.md)
- [SLeeLa syntax specification](SLEELA.syntax)
- [Documentation index](docs/README.md)
- [API documentation](docs/api/README.md)
- [Platform support](docs/PLATFORM-SUPPORT.md)
- [Native implementation](impl/README.md)
- [Installation guide](install/README.md)
- [Testing policy](markdown/TESTING.md)
- [SLeeLa GitHub repository](https://github.com/mearvk/SLeeLa)

**Katayuan ng salin:** Pinalawak ang Filipino edition bilang teknikal na gabay na sumasaklaw sa pagpapakilala, build, installation, wika at runtime, mga feature, library, native integration, seguridad, development workflow, at mga pangunahing bahagi ng proyekto. Hindi pa ito kumpletong salin, bawat linya at seksiyon, ng canonical English README. Inirerekomenda pa rin ang teknikal na pagsusuri at pagrebisa ng wikang Filipino ng isang reviewer na may kaalamang teknikal.
