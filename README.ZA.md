# SLeeLa 3.0 — South Africa (ZA)

## Language editions

This country edition groups English, isiZulu, isiXhosa, Afrikaans, Sepedi, Setswana, Sesotho, siSwati, Tshivenda, Xitsonga, isiNdebele, and South African Sign Language. The introductory descriptions below are retained; the practical technical guide is provided in English pending reviewed translations in the other languages.

SLeeLa is a native systems programming language, compiler, runtime, and virtual-machine ecosystem. It brings together the language, compiler, libraries, developer tools, and platform contracts needed to build and run software.

- isiZulu: I-SLeeLa iwulimi lokuhlela amasistimu.
- isiXhosa: I-SLeeLa lulwimi lwenkqubo yeenkqubo.
- Afrikaans: SLeeLa is ’n inheemse stelselprogrammeertaal.
- Sepedi: SLeeLa ke leleme la tlhago la go rulaganya ditshepedišo.
- Setswana: SLeeLa ke puo ya tlhago ya go kwala mananeo a ditsamaiso.
- Sesotho: SLeeLa ke puo ea tlhaho ea mananeo a litsamaiso.
- siSwati: I-SLeeLa lulwimi lwemvelo lwekuhlela tinhlelo.
- Tshivenda: SLeeLa ndi luambo lwa u dzudzanya sisiteme.
- Xitsonga: SLeeLa i ririmi ra minongonoko ya tisisiteme.
- isiNdebele: I-SLeeLa lilimi lemvelo lokuhlela amasistimu.

## I. Start here

- [Build the native implementation](impl/README.md)
- [Virtual-machine and Modium user guide](virtual-machine/USER-GUIDE.md) — `sleela vm`, `sleela modium`
- [CPU architecture in SLeeLa source](lib/cpu/CPU.md) — more than 130 CPU profiles; see [timing controls](lib/cpu/TIMING.md).
- [Documentation index](docs/README.md)
- [Generated API documentation](docs/api/README.md) and [API catalogue](api/README.md)
- [Platform support and validation](docs/PLATFORM-SUPPORT.md)
- [Testing policy](markdown/TESTING.md)

## II. Build and test

Requirements include a C/C++ toolchain (GCC or Clang), GNU Make, and Python 3. Linux graphical components may also require X11/Xrandr development packages.

Run from the repository root:

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

The conventional system prefix is generally `/usr/local`. Setting `PREFIX="$HOME/.local"` provides a user-local installation and normally avoids administrator privileges. See the [installation guide](install/README.md) for details.

## IV. Language and runtime

The normative grammar is [`SLEELA.syntax`](SLEELA.syntax). This repository documents syntax **3.0**. Compiler version and source-language syntax version are distinct.

Technical references cover `@mix` contiguous fields, extended integer families, Preonem document markers, SleelaDoc, API documentation generation, and Nordshrift. Technical identifiers, commands, filenames, API names, and code examples remain unchanged.

## V. Engineering and validation

SLeeLa emphasizes evidence, explicit component contracts, portability, safe failure behavior, and reproducible releases. Validation must be assessed by module and platform. Skipped checks must not be reported as passing; one successful component build is not full release validation.

Sleelavirin protection tooling includes a local SHA-256 verifier, an optional ClamAV adapter, and an integrity manifest. A release should be considered validated only after its commit's CI checks finish and the resulting artifacts are reviewed.

## VI. Editors and contributions

SLeeLa provides **Vim** and **Emacs** themes and syntax integration, plus a native C/C++ palette utility. See the [editor integration guide](linux-editors/README.md).

Contributions should be focused, include positive and negative tests, and state the operating system and toolchain used. Do not describe skipped checks as successful.

SLeeLa is developed by **MEARVK LLC**.

## VII. Other technical areas

The main README also covers the Advantage, SleelaDoc, and XML DOM libraries; primitive integer types; the character runtime; AutoCAD Creator™; the experimental UTF-4088 catalogue; Pi, Modium, Registry, and `sleelactl` tools; and the initial native C/C++ compilation path enabled by the `@native` marker.

## Complete technical reference

The [canonical README](README.md) contains the complete feature catalogue, language and runtime reference, build and installation instructions, API links, examples, and release notes. This South African edition expands the practical English guide and retains the multilingual introductions, but it is not a complete line-by-line translation of the canonical technical README. English remains the normative technical source until reviewed translations are available.

- [Canonical README](README.md)
- [SLeeLa syntax specification](SLEELA.syntax)
- [Documentation index](docs/README.md)
- [Platform support](docs/PLATFORM-SUPPORT.md)
- [GitHub repository](https://github.com/mearvk/SLeeLa)

**Translation status:** Practical technical guidance is expanded in English. Full technical translations into the listed South African languages, including South African Sign Language resources, remain outstanding and should be reviewed by fluent technical contributors.


## VIII. Afrikaanse tegniese gids

### Samestelling en uitvoeringsmodel
SLeeLa skei die brontaal, die samesteller se frontend, die intermediêre voorstelling, kodegenerering en runtime-uitvoering. Die samestellerweergawe, sintaksuitgawe en VM/ISA-weergawe is verskillende weergawe-asse. 'n Funksie wat in die spesifikasie genoem word, is nie noodwendig op elke teiken geïmplementeer en bekragtig nie.

Die pyplyn begin met UTF-8-`.sleela`-bronlêers, voer leksikale en sintaktiese ontleding uit, kontroleer simbole en tipes, verlaag 'n gestruktureerde intermediêre voorstelling, genereer uitset vir ondersteunde VM- of intermediêre teikens en voer die resultaat in die runtime uit. Die normatiewe grammatika is [SLEELA.syntax](SLEELA.syntax); sien ook [source documentation](markdown/SOURCE.md) en [compiler documentation](markdown/COMPILER.md).

### Tipes en objekmodel
Die gedokumenteerde brontipes sluit `int`, `double`, `boolean`, `String`, `void`, verklaarde `struct`-tipes en ondersteunde reference-type signatures in. `int`, `long` en `int64` is getekende 64-bis heelgetalle. `int128` word as 'n heelgetaltipe aanvaar, maar die huidige gedokumenteerde implementering is steeds 64-bis-gegrond; moenie volle 128-bis-rekenkunde aanvaar nie.

'n `struct` is 'n benoemde aggregate. VM-bestuurde struct-handvatsels is nie rou wysers nie; toekenning en parameteroordrag deel dieselfde instansie, en gelykheid berus op identiteit. Gebruik `structPack(instance)` en `structUnpack(TypeName, json)` vir die gedokumenteerde draagbare serialisering. 'n VM-plaaslike handvatsel is nie self 'n oordraagbare netwerkwaarde nie. Klasse bevat velde en metodes; huidige klasvelde is gedeelde toestand, terwyl metode-plaaslike veranderlikes thread-plaaslik is. Oorerwing, geneste klasse en generics is aan sintaksweergawepoorte onderhewig.

### Lusse, tydsberekening en biblioteke
Die stellingsvlak-aantekeninge `@loopDelay` en `@loopBudget` bepaal 'n lusvertraging of totale tydbegroting. Hulle word na VM-instruksie `OP_SLEEP` (ISA 157) verlaag; die ingeboude `sleep(ms)` gebruik dieselfde instruksie. Sien [SLEELA.syntax §32](SLEELA.syntax) en [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md).

Belangrike gedokumenteerde biblioteke sluit in `lib/advantage` (modelgebaseerde evaluering, wiskunde, integrasie, gradering en roeteerbare uitset), `lib/xml` (XML DOM, ontleding en serialisering), `lib/text` (teks- en stringbewerkings), `lib/sleeladoc` (brondokumentasie en CLI-verwysings), `lib/cpu` en `lib/xlt` (CPU-profiele en model-/tydsberekeningverwysings), en `recursion/stanchion` (C/C++-rekursieprototipes). 'n Module se teenwoordigheid bewys nie produksiegereedheid nie; hersien sy toetse en platformbeperkings.

### C/C++, bouwerk en installering
Die repository bevat C/C++-komponente en 'n beperkte native-kodegenereringspad. `@native` en `sleela aot` is ontwikkelingsmeganismes, nie algemene toestemming om arbitrêre native kode uit te voer nie. C/C++-grense moet ABI, oproepkonvensie, geheue-eienaarskap, datauitleg, foutafhandeling en platformafhanklikhede uitdruklik definieer. 'n Reference identifiseer 'n hulpbron, maar magtig nie uitvoering nie.

'n Tipiese bou- en toetsreeks:

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

Stelselinstallering gebruik gewoonlik `sudo make install`; gebruikersinstallering kan `make install PREFIX="$HOME/.local"` gebruik. Bouwerk vereis gewoonlik GCC of Clang en GNU Make; sommige gereedskap gebruik Python 3. Raadpleeg [installation guide](install/README.md) en [platform matrix](docs/PLATFORM-SUPPORT.md).

### Integriteit, veiligheid en bydraes
Hash-manifeste en verifikasiegereedskap vergelyk lêers met verwagte inhoud. 'n PASS-resultaat geld slegs vir die verklaarde omvang en tydstip. Toetsverslae moet pass, fail en skip onderskei; toetse wat oorgeslaan of nie uitgevoer is nie, mag nie as geslaag gerapporteer word nie.

Lees [testing policy](markdown/TESTING.md), [documentation index](docs/README.md), [generated API reference](docs/api/README.md) en die relevante module-README voordat jy bydra. Voeg positiewe en negatiewe toetse by, teken OS/toolchain/opdragte/werklike resultate aan en werk dokumentasie by wanneer openbare gedrag verander.

## IX. Ander Suid-Afrikaanse tale en toeganklikheid
Hierdie Afrikaanse tegniese afdeling vul die bestaande kort inleidings in isiZulu, isiXhosa, Sepedi, Setswana, Sesotho, siSwati, Tshivenda, Xitsonga en isiNdebele aan. Dit is nie 'n volledige tegniese vertaling in al elf amptelike tale nie. Suid-Afrikaanse Gebaretaal vereis toeganklike video- of gebaretaalmateriaal en kan nie deur 'n geskrewe taalopsomming vervang word nie. Verdere vertalings behoort deur moedertaalsprekers met tegniese kennis nagegaan te word.

**Translation status:** The South African edition now includes a substantial Afrikaans technical guide alongside the English practical guide and multilingual introductions. Full technical translations into all South African languages and dedicated South African Sign Language resources remain outstanding.
