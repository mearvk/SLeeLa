<img align="right" src="../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — Danmark (DK)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Et native systemsprog, en compiler, en runtime og et økosystem af virtuelle maskiner.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="../../docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **International landeudgave.** Denne udgave er på dansk. Tekniske identifikatorer, kommandoer, kodeeksempler, filnavne, stier og links er uændrede i forhold til den kanoniske engelske [README.md](../../README.md).

**The Now. The Ingredient.** Et sprog er mere end syntaks: Det er compileren, runtime, bibliotekerne, værktøjerne og platformaftalerne, der gør software anvendelig.

**Hvad SLeeLa beskæftiger sig med.** SLeeLa beskæftiger sig med **Math**,
**Object**, **Science** og **Reach** — de fire ansigter af dets arbejde: det
beregnelige (Math), den afklarede ting, der står i katalogtet over det, der er
(Object), det disciplinerede studium af det sande (Science) og udstrækningen af
et systems hånd til andre systemer og mennesker (Reach). **Nationale interesser
består** — de sættes ikke til side af nogen af de fire; hvor et emne berører en
nations interesse, bevares den stilling (registreret trofast, f.eks. i hver
klasses Metadata `metaOrigin` / `metaNationalSchedule`) og opløses aldrig.

## I. Kom godt i gang
- [Byg den native implementering](../../impl/README.md)
- [Virtuelle maskiner og Modium — praktisk brugervejledning](../../virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [CPU-stak i SLeeLa-kildekode](../../lib/cpu/CPU.md) — en komplet maskine fra logiske porte → ALU → kerner → operativsystem → programmer, en oversigt over 130+ CPU-profiler samt justerbare [spændinger, variable clocks og ekspert-timings](../../lib/cpu/TIMING.md), som du kan [køre efter specifikation og overclocke](../../lib/cpu/components/README.md)
- [Dokumentationsindeks](../../docs/README.md)
- [CSPML-datakilde og Salt & Salary-forskningsreference](../reference/CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — oprindelse og integrationsvejledning for eksterne amerikanske data om steder, socioøkonomi, arbejdsstyrke, uddannelse og offentlige skoler.
- [Genereret API-dokumentation](../../docs/api/README.md) — aktuel HTML-reference, versioneret API-udgave og anvisninger til regenerering
- [API-katalog](../../api/README.md)
- [Standarder for beregningsmæssig sammenligning](../../comparison/README.md) — standarder for typer, hukommelse, ABI og overensstemmelse for SLeeLa VM, Java/JVM og native C/C++.
- [Recursion stanchion — C- og C++-prototyper](../../recursion/stanchion/README.md) — primitivspecialiserede eksempler for Boolean, Byte, Character, Integer, Long, Float og Double ved siden af generiske, anvendte, videnskabelige og undervisningsorienterede rekursionsmønstre.
- [Platformunderstøttelse og valideringsmatrix](../../docs/PLATFORM-SUPPORT.md)
- [Brandguide](../../docs/BRAND-GUIDE.md)
- [Merchandise-koncepter og -materialer](../../merchandise/README.md)
- [NitroWebExpress-logo](../../images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Testpolitik og releasekrav](../../markdown/TESTING.md)

## II. Nyheder

Nyere tilføjelser til sproget, driverne og standardbiblioteket:

- **Standardbibliotekspakken `advantage` — at advantage et udbytte, med
  matematik, fysik, integration, bedømmelse og routbart output.**
  [`lib/advantage/`](../../lib/advantage/README.md) indkapsler *at advantage et
  entydigt udbytte i forhold til en model* som en lagdelt **Sequitur-løg**
  (udbytte → review → concerns → maths → additions → subtraction → regrouping
  → iteration), der kan navigeres indad/udad med fuld dynamisk opdatering og
  restitute, og som udstilles gennem en enkel **brugermodel** og en
  **udviklermodel** med fuld kontrol. Den indeholder fem emneområder: **`math/`**
  bilateral handelsøkonomi (fælles profitmaksimering, Nash-forhandling, den
  bilaterale newsvendor-kritiske ratio), **`physics/`** en strukturel økonomisk
  "fysik for tillidsfelter" (tillid som metrisk tensor, den transaktionelle
  geodæt, en announcement-kontaktstruktur, en CIR+jump-SDE for tillidsforfald
  — en modelanalogi, markeret som sådan), **`integration/`** en rekursivt
  sammensættelig vertikal integrationsmodel for kapitalisme/detailhandel
  (kompartmenter, engineering bays, trial-kompartmenter, newton median trays og
  en systemsofistikationsscore), **`grading/`** en sofistikationskreds-bedømmer,
  der vurderer hvert modul og ethvert sæt af winning/losing/grader/absolute-moduler
  A–D, og **`output/`** en routbar emit-søm, som sender modulets output til
  terminalen, en **CommonRails**-formateret overflade eller en socket/connector
  (indrammet til en native `slnet_write`). Hver klasse bærer Metadata-Class-posten;
  fem kørbare demoer validerer emnerne, og de tilhørende filer
  [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md),
  [`MATH.md`](../../lib/advantage/MATH.md) og
  [`PHYSICS.md`](../../lib/advantage/PHYSICS.md) dokumenterer dem. En native C/C++-
  forbruger under [`advantage/`](../../advantage/README.md) (C-kernen
  `advantage_value`/`advantage_mean`, et XML-videnslager samt et sikkert
  bibliotek til HTTPS/SFTP/SSH-erhvervelse + keystore + DH/ECDH-transport)
  kobler modellen til udarbejdede
  [handelseksempler](../../advantage/examples/README.md) — positive handler fra
  formodede internetkilder, en publikumsrelevant sekvens (Traders, Consumers,
  Helpers, Healers) og otte modeller rangeret A–D (D = mest Cement, A = mest
  Brand).
- **Annotationer for løkketiming — `@loopDelay` / `@loopBudget` (+ den
  indbyggede `sleep(ms)`).** De første **udsagnsniveau-, adfærdsmæssige**
  annotationer i SLeeLa: skrevet på egen linje før en `for`-/`while`-/`do`-løkke
  indsætter de en reel pause i løkkens udførelse. `@loopDelay(ms = 250)` /
  `@loopDelay(seconds = 2)` sover et fast tidsrum pr. iteration (et blot
  `@loopDelay(250)` bruger som standard millisekunder); `@loopBudget(seconds = 10,
  iterations = 1000)` fordeler et kendt antal iterationer jævnt over et samlet
  vægurs-budget. Begge oversættes til den nye append-only-opkode **`OP_SLEEP`**
  (ISA 157), som også ligger bag den generelle indbyggede funktion
  **`sleep(<ms>)`**, der kan kaldes overalt; pausen implementeres portabelt
  (`nanosleep` / `Sleep`). Specificeret i
  [`SLEELA.syntax` §32](../../SLEELA.syntax); API-reference i
  [`api/sleela/3.0/LOOP_TIMING_API.md`](../../api/sleela/3.0/LOOP_TIMING_API.md);
  kørbar
  [`examples/loop-timing-annotation-demo.sleela`](../../examples/loop-timing-annotation-demo.sleela).
  Et indeks over sessionstilføjelser (herunder denne, den native
  store-heltals-multiplikation `OP_BIGMUL` og de selvstændige værktøjer
  **[local-generator](../../local-generator/README.md)** og
  **[pi-generator](../../pi-generator/README.md)**) findes i
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](../../api/sleela/3.0/SESSION-ADDITIONS-API.md).
  De tre genererede API-overflader
  ([`sleeladoc-api/`](../../sleeladoc-api/README.md),
  [`docs/api/`](../../docs/api/README.md) og det uforanderlige
  [`api/sleela/3.0/version-014/`](../../api/sleela/3.0/version-014/index.html)-
  snapshot) blev genopbygget med `make api-docs` og indeholder nu de nye
  instruktionsposter `SLISASleep` / `SLISABigMul` på deres `vm`-pakkeside.
- **SleelaDoc fra kommandolinjen — `sleela docs`.** SleelaDoc kan nu nås direkte
  fra terminalen. `sleela docs` viser alle dokumenterbare emner;
  `sleela docs crypto` viser en pakke; `sleela docs stringbuilder` finder en
  klasse (uden skelnen mellem store og små bogstaver og via
  standardbibliotekets præfiks `SL` → `SLStringBuilder`) og udskriver en rig
  side — **definition, designformål, relationer, struktur, user clays og en
  mestringsstige med fem trin** — med rangerede "mente du"-forslag ved en
  nær-fejl. Forfattere beriger en side blot ved at tilføje SleelaDoc-`@`-mærker
  (`@intent`, `@thesis`, `@axiom`, `@param`, `@see`, `@mastery`).
  `sleela docs --generate [dir]` skriver et tekstkorpus (én side pr. klasse +
  en `TOPICS.index`), som CLI'en derefter læser offline; **`make sleeladoc`**
  samler det korpus **og de tilhørende `.sleela`-kilder** i
  `sleeladoc/build/sleeladoc.jar`. For emner, der kun har en facade (f.eks.
  `psychiatry`, `politico`), søger `sleela docs --facade <topic>` i filsystemet
  under kørsel og viser de reelle **mapper (derefter filer, som naturlig
  bevægelse)**, begrænset af kvoter, som brugeren kan tilsidesætte:
  **6 MB / 6000 filer / 6 minutter**. Enhederne i `lib/sleeladoc` vedligeholdes
  i den aktuelle udgave, **`#sleela 3.0`**. Se [`man/man1/sleela.1`](../../man/man1/sleela.1)
  og [`lib/sleeladoc/README.md`](../../lib/sleeladoc/README.md).
- **SleelaDoc — en dokumentationsløsning i Javadoc-stil med et genereret
  HTML-websted.** **SleelaDoc** er SLeeLas dokumentationskommentarkonvention i
  kildekoden (kommentarer i vinkelparenteser med sentinellerne `@doc` / `@page`;
  se [`manifesto/SLEELADOC.md`](../../manifesto/SLEELADOC.md)), og
  [`lib/sleeladoc`](../../lib/sleeladoc/README.md) er den **ekstraktor**, der
  gengiver en `.sleela`-enhed som en Markdown- eller HTML-API-side — på samme
  måde som `javadoc` gengiver Java — og dokumenterer *hensigten*. Et komplet,
  gennemsøgeligt **HTML-dokumentationssted for hele `/lib`** genereres i
  [`/sleeladoc-api`](../../sleeladoc-api/README.md) med
  `tools/generate-sleeladoc-api.py` (åbn `sleeladoc-api/index.html`; en ren
  stil i blå / lyseblå / lysegrå / hvid med hierarkiet pakke → klasse → medlem
  og en ordliste skrevet til læseren *Student 2*). Compileren udsender også
  forfattermarkerede dokumentationsoutput som biprodukt af kompileringen
  (`--wiki` MediaWiki-side + en oprindelig kildetvilling), dokumenteret i
  [`manifesto/SLEELADOC.WIKI.md`](../../manifesto/SLEELADOC.WIKI.md). Kørbar:
  [`lib/sleeladoc/sleeladoc-demo.sleela`](../../lib/sleeladoc/sleeladoc-demo.sleela).
- **XML Document Object Model (`lib/xml`).** Et XML-DOM på SLeeLa-siden — et
  dokument, der ejer et træ af element-/tekst-/kommentar-/CDATA-/PI-noder med
  ordnede børn og navngivne attributter, en serializer (kompakt/pæn, XML 1.0
  §2.4-escaping), en parser på tegncelle-niveau og kvalitetsregisteret
  `SLXmlAspect`. Nodelighed er et tretrins tillidsbånd (identitet → samme emne →
  struktur), refleksivt, symmetrisk og deterministisk. Se
  [`lib/xml/README.md`](../../lib/xml/README.md) og det kørbare
  [`lib/xml/xml-dom.sleela`](../../lib/xml/xml-dom.sleela).
- **Brede signerede heltals-primitiver (`long`, `int64`, `int128`).** `int`,
  `long` og `int64` er alle 64-bit signerede heltal, der typetjekkes og kører
  identisk; `int128` accepteres som heltalstype (indtil videre bakket af 64 bit,
  med ægte 128 bit som en dokumenteret opfølgning). Den signerede 2-talsserie
  `int32`..`int16777216` (syntaks 2.7) og den tilsvarende usignerede serie
  `uint32`..`uint16777216` (syntaks 2.8) udvider dette. Se
  [`SLEELA.syntax`](../../SLEELA.syntax) §21, §24, §25.
- **`@mix`-konsekvensfelter (syntaks 2.9).** En klasse kan deklarere flere
  instansfelter med **samme navn**, når hver gentagelse bærer `@mix`; sproget
  forstår gruppen som konsekvent (et array / en liste indekseret efter
  position), top-down `1,2,3,…` som standard eller manuelt via `@mix(1,2,3,...)`,
  hvis indeksantal skal svare til antallet af felter. Se
  [`SLEELA.syntax`](../../SLEELA.syntax) §28.
- **Dokumentmarkører — den Naturlige Proces, betitlet Preonems
  (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).**
  Disse bare dokumentmarkør-ord betegnes samlet **Preonems** (én markør er en
  *Preonem*; mængden, *Preonems* — se
  [`SLEELA.syntax`](../../SLEELA.syntax) §26.1). Førsteklasses, kun deklarative
  dokumentmarkører, der angiver, hvad et dokument *er* i rækkefølgen af tilblivelse:
  `author`/`authorial` (syntaks 2.4; håndteres af professionelt råd — det eneste
  medlem med en adfærdsmæssig virkning), `material;` (afklaret kilde) og
  `memory;` (syntaks 2.5), `determine;` (syntaks 2.6; et dokument, der er åbent
  for videre behandling af en læge, en fysiker eller et spøgelse hen imod
  færdiggørelse ved juridisk hånd), og de to nyeste, **`object;`** (en afklaret
  ting, der står i katalogtet over det, der er — **reificeret, givet konkret
  stilling, ved syntaks 2.8**) og **`field;`** (det relationsgrundlag, hvori
  objekter står og bevæger sig) — begge afgrænset fra syntaks 2.6. Hver er bar,
  kontekstuel og versionsafgrænset af frontenden uden virkning på parsing,
  typning eller kodegenerering; uden for markørpositionen forbliver hver af dem
  en almindelig identifikator (så `field` forstyrrer aldrig et klassemedlem
  eller en `.field`-adgang). Se [`SLEELA.syntax`](../../SLEELA.syntax)
  §18–§20, §23 og §26.
- **Nordshrift ved syntaks 3.0 med fuld biblioteksrækkevidde.** Nordshrift
  opløses gennem det samme rekursive `/lib`-indeks som compileren og når hver
  pakke og `.sleela`-enhed; `nordshrift check` rapporterer tallene for hele
  træet.
- **Recursion stanchion.** Mappen [`recursion/stanchion/`](../../recursion/stanchion/README.md) indeholder nu dedikerede C- og C++-referenceprototyper for de syv dokumenterede primitivkontrakter samt generiske, anvendte, videnskabelige og undervisningsorienterede eksempler. Det er referencekilder; kompilér og test dem, før de behandles som verificerede.

- **Vinkelparentes-kommentarer (`<` / `<<`).** En ny kommentarstil fremover —
  `< linje >` og `<< blok >>` — lekseret af SLeeLa-frontenden og dokumenteret i
  [`SLEELA.syntax`](../../SLEELA.syntax) §1.1.
- **Radix-heltalsliteraler.** `int`-literaler accepterer nu hexadecimal (`0x`),
  binær (`0b`) og oktal (`0o`) form med `_` som cifferseparator, både i
  SLeeLa-kildesproget ([`SLEELA.syntax`](../../SLEELA.syntax) §1.4) og i
  [`.xlt` CPU-modelformatet](../../lib/xlt/README.md).
- **Metadata Class-standarden i hele biblioteket.** Det beskrivende
  metadata-metamønster (proveniens, lovlighed, jurisdiktionelt omfang) bæres nu
  af hver klasse-bærende `.sleela`-fil under `/lib`; se
  [`SLEELA.syntax`](../../SLEELA.syntax) §15 og
  [tutoriallektionen](../../tutorial/13-metadata-class.md).
- **En rigere tekst-/String-familie.** `lib/text` får reelle hjælpefunktioner i
  selve sproget, en tegncellevisning, der giver ægte `charAt`/`substring`/`indexOf`/
  `reverse`/kolonnejustering, og en reel positionel skabelonmotor — se
  [tutorialen om tekst og String](../../tutorial/14-text-and-strings.md) og den kørbare
  [`lib/text/text-string-demo.sleela`](../../lib/text/text-string-demo.sleela).
- **Førsteklasses `String` i Nordshrift-ark.** Et `.sst`-ark kan deklarere en
  typet `strings:`-blok, der bidrager til buildets komponentmanifest;
  se [Nordshrift Sheet Reference](../../wiki/Nordshrift-Sheet-Reference.md).
- **CPU-modeller kører både via opkoder og via ækvivalens med funktionskald.**
  Hver `SL<ARCH>CPU` arver grundlaget for at køre en native instruktionsstrøm
  gennem sin fetch-decode-execute-løkke *og* køre koden for en antaget anden
  proces ved ækvivalente kald på det delte VM-substrat — se
  [`lib/cpu/EXECUTION.md`](../../lib/cpu/EXECUTION.md) §5.

## III. Byg og test

**Forudsætninger:** en C/C++-værktøjskæde (GCC eller Clang) og GNU Make;
Python 3 til manifest- og flowværktøjerne; på Linux X11/Xrandr-udviklingspakkerne
til displayprodukterne (et moduls `make deps` rapporterer/installerer det, det
har brug for). Buildet af `impl/` er fail-closed på et betroet SHA-256-manifest,
som rodens `Makefile` automatisk peger på.

Den hurtigste start fra et rent checkout:

```sh
make                                  # build everything from the repo root
```

Eller byg og test kun den native kerne:

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

`make` i repositoryets rod bygger alle produkter (kerne, VM, codecs, churn,
antivirus, …) og afslutter med at bygge **registry** og materialisere dets
baseline-tabel — så et bygget træ leveres med et gennemgåeligt registry fra
start. De væsentlige mål:

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

Andre nyttige mål:

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

Målet `registry` kører [`build-registry.sh`](../../build-registry.sh): det bygger
registry-kernen, fylder baseline-tabellen i det portable tekstlager
`registry/build/sleela.sleelareg` og eksporterer MySQL-tabelscriptet
`registry/build/registry.sql` (via `sleela registry export`). Gennemgå det med
`sleela registry` eller `registry/build/sleela-registry review`. Se
[registry-guiden](../../registry/README.md) og [Registry 3.0-arkitekturen](../../registry/REGISTRY-3.0-ARCHITECTURE.md).

Se bygge- og testguiderne for forudsætninger og platformspecifikke trin. At én komponent bygges med succes, beviser ikke, at hvert bibliotek eller hver platform er klar til release.

## IV. Installation

Når de native produkter er bygget, installeres kommandoerne og runtime-supportfilerne:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

Det konventionelle installationsprogram bruger som standard `/usr/local/bin`,
`/usr/local/lib/sleela` og `/usr/local/share/sleela`. Det bevarer en eksisterende
opstartskonfiguration og nægter at overskrive ikke-sporede eksisterende
kommandoer. Afinstallation fjerner kun filer, som installationsprogrammet har
registreret, og bevarer konfiguration og ikke-sporede runtime-data.

Til en fokuseret, guidet udrulning af kun hovedpakken — binærfilerne `sleela`,
`sleelvac` og `nordshrift`, standardbiblioteket, protokollerne og
supportmodulerne — bruges det rene installationsprogram:

```sh
make clean-install       # guided install of the main package only
```

Det træffer én primær beslutning (installér hovedpakken), derefter et valg af
omfang og ved succes en valgfri vælger til udvalgte forbedringsmoduler. Se
[Quick and Safe Install](../../install/README.md) for det interaktive udrulningsforløb
og alle installationsindstillinger.

## V. Sprog og runtime
Den autoritative grammatik er [`SLEELA.syntax`](../../SLEELA.syntax). Repositoryet dokumenterer i øjeblikket syntaks **3.0**; compiler-/værktøjsversion og kildesyntaksversion er to forskellige ting. Overfladen i 3.0 tilføjer **dokumentmarkørerne `pure` og `gasoline`** (§30) — et par bare, kontekstuelle, kun deklarative Preonems, der betyder næsten det samme og hver især markerer dokumentets stilling ved sin egen ærlighed. Syntaks 3.0 konsoliderer sprogoverfladen 1.0–2.9 i den aktuelle normative udgave og bevarer alle funktionsspecifikke mindstekrav til version. Den forudgående 2.9-overflade tilføjer **`@mix`-konsekvensfelter** (§28) — en klasse kan deklarere flere instansfelter med samme navn, når hvert bærer `@mix`, forstået som én indekseret gruppe (top-down `1,2,3,…` eller manuel `@mix(1,2,3,...)`, hvis indeksantal skal svare til gruppen). Den fører de base-2 signerede/usignerede brede heltalsfamilier `int32`..`int16777216` (§24) og `uint32`..`uint16777216` (§25) videre over de tidligere `long`/`int64`/`int128` (§21); **dokumenter med nøgleordsunderstøttelse** (§22); og **Preonems** — de kun deklarative dokumentmarkører `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) og `artifact` (§35, stilling på lovgivningsniveau graderet efter tid og rum), samlet betitlet Preonems (§26.1). Overload-opløsning for konstruktører og delegering med `this(...)`/basiskonstruktør ligger udtrykkeligt uden for den verificerede syntaks 1.8-kontrakt. Konsultér grammatikken og implementeringsnoterne, før du stoler på en funktion. SleelaDocs ni kildeenheder deklarerer nu `#sleela 3.0`; den fælles compilergate accepterer kildesyntaks 1.0–3.0. Den nye API-build fremstilles med `make api-docs`.

Repositoryet indeholder en native C/C++-implementering, SLVM-genereringsreferencer, standardbiblioteksmoduler, tests, eksempler og platformsintegrationer. Implementerings- og valideringsstatus varierer fra modul til modul; konsultér modulets specifikation og de aktuelle CI-resultater frem for at antage ensartet fuldstændighed.

## VI. Ingeniørprincipper
- **Beviser frem for påstande:** skeln mellem implementeret, testet, eksperimentel og planlagt adfærd.
- **Én kontrakt pr. grænseflade:** dokumentér syntaks, API'er, filformater, platformsforudsætninger og fejladfærd.
- **Portabel fra designet:** isolér platformsspecifikke backends og test på native runnere.
- **Sikker fejl:** test misdannet input, ressourcegrænser, procesfejl og oprydningsstier.
- **Reproducerbare releases:** byg rent igen og udgiv artefakt-hashes og udgivelsesnoter.

## VII. Antivirus-build og integritet

Antivirus-pakken Sleelavirin omfatter en native SHA-256-scanner, en valgfri
ClamAV-adapter, integritetsmanifester for kilder/indeks og checksummer for
platformspakker. Byg og test pakken fra repositoryets rod:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Integritetsmanifestet genereres under `antivirus/build/`; det dækker sporede
kilde- og dokumentationsinput samt beskyttede signaturindeksfiler, men udelader
genereret output og den foranderlige ClamAV-cache. Snapshots af ClamAV-databaser
har deres egne manifester. Release-ZIP-filer indeholder `INTEGRITY-MANIFEST.json`,
`SHA256SUMS` og en `.sha256`-sidecar. Kontrollér hashes for downloadede arkiver
mod en betroet værdi, der er indhentet uafhængigt: en checksum gemt ved siden af
et arkiv er ikke en udgiversignatur. Se [Sleelavirin-guiden](../../antivirus/README.md),
[sikkerhedstjeklisten](../../antivirus/SECURITY.md) og
[antivirus-build-workflowet](../../.github/workflows/antivirus-binaries.yml).

En release anses ikke for verificeret, før de relevante CI-jobs er bestået på
den præcise release-commit, og de resulterende artefakter er kontrolleret.


## VIII. SLeeLa-editortemaer — Vim og Emacs

SLeeLa indeholder et sammenhængende sæt professionelle mørke farveskemaer til **Vim** og **Emacs** samt et native C/C++-værktøj til paletter. Det oprindelige skema **American Professional** er fortsat standarden. Ti ledsagende paletter findes med forskellige kulturelle og institutionelle designinspirationer; det er fortolkende designpaletter og ikke officielle repræsentationer af nogen nation, kultur, domstol eller institution.

### De elleve skemaer og deres farver

| Skema | Baggrund | Forgrund | Nøgleord | Typer | Strenge | Kommentarer |
|---|---|---|---|---|---|---|
| **American Professional** | <span style="display:inline-block;width:1em;height:1em;background:#101820;border:1px solid #888"></span> `#101820` | <span style="display:inline-block;width:1em;height:1em;background:#F4F7FA;border:1px solid #888"></span> `#F4F7FA` | <span style="display:inline-block;width:1em;height:1em;background:#7FB3FF;border:1px solid #888"></span> `#7FB3FF` | <span style="display:inline-block;width:1em;height:1em;background:#73DACA;border:1px solid #888"></span> `#73DACA` | <span style="display:inline-block;width:1em;height:1em;background:#F2C078;border:1px solid #888"></span> `#F2C078` | <span style="display:inline-block;width:1em;height:1em;background:#8A96A3;border:1px solid #888"></span> `#8A96A3` |
| **Pan-Eurasian** | <span style="display:inline-block;width:1em;height:1em;background:#111827;border:1px solid #888"></span> `#111827` | <span style="display:inline-block;width:1em;height:1em;background:#F0F2F5;border:1px solid #888"></span> `#F0F2F5` | <span style="display:inline-block;width:1em;height:1em;background:#7AA7E8;border:1px solid #888"></span> `#7AA7E8` | <span style="display:inline-block;width:1em;height:1em;background:#69C9B3;border:1px solid #888"></span> `#69C9B3` | <span style="display:inline-block;width:1em;height:1em;background:#D7B56D;border:1px solid #888"></span> `#D7B56D` | <span style="display:inline-block;width:1em;height:1em;background:#8995A5;border:1px solid #888"></span> `#8995A5` |
| **Russian and Slavic — Medium** | <span style="display:inline-block;width:1em;height:1em;background:#202733;border:1px solid #888"></span> `#202733` | <span style="display:inline-block;width:1em;height:1em;background:#E8EAF0;border:1px solid #888"></span> `#E8EAF0` | <span style="display:inline-block;width:1em;height:1em;background:#A9C0DD;border:1px solid #888"></span> `#A9C0DD` | <span style="display:inline-block;width:1em;height:1em;background:#8FC4B2;border:1px solid #888"></span> `#8FC4B2` | <span style="display:inline-block;width:1em;height:1em;background:#D6BD8B;border:1px solid #888"></span> `#D6BD8B` | <span style="display:inline-block;width:1em;height:1em;background:#9AA3AF;border:1px solid #888"></span> `#9AA3AF` |
| **Central Asian / Silk Road** | <span style="display:inline-block;width:1em;height:1em;background:#1C2529;border:1px solid #888"></span> `#1C2529` | <span style="display:inline-block;width:1em;height:1em;background:#F5F0E5;border:1px solid #888"></span> `#F5F0E5` | <span style="display:inline-block;width:1em;height:1em;background:#7FADE0;border:1px solid #888"></span> `#7FADE0` | <span style="display:inline-block;width:1em;height:1em;background:#45C6C2;border:1px solid #888"></span> `#45C6C2` | <span style="display:inline-block;width:1em;height:1em;background:#D9B56D;border:1px solid #888"></span> `#D9B56D` | <span style="display:inline-block;width:1em;height:1em;background:#92A3A0;border:1px solid #888"></span> `#92A3A0` |
| **East Asian** | <span style="display:inline-block;width:1em;height:1em;background:#171A1C;border:1px solid #888"></span> `#171A1C` | <span style="display:inline-block;width:1em;height:1em;background:#F2EFE7;border:1px solid #888"></span> `#F2EFE7` | <span style="display:inline-block;width:1em;height:1em;background:#D97878;border:1px solid #888"></span> `#D97878` | <span style="display:inline-block;width:1em;height:1em;background:#72C7A5;border:1px solid #888"></span> `#72C7A5` | <span style="display:inline-block;width:1em;height:1em;background:#D8B76A;border:1px solid #888"></span> `#D8B76A` | <span style="display:inline-block;width:1em;height:1em;background:#969F9B;border:1px solid #888"></span> `#969F9B` |
| **ICC — International Criminal Court** | <span style="display:inline-block;width:1em;height:1em;background:#0E1A2B;border:1px solid #888"></span> `#0E1A2B` | <span style="display:inline-block;width:1em;height:1em;background:#EAF0F7;border:1px solid #888"></span> `#EAF0F7` | <span style="display:inline-block;width:1em;height:1em;background:#5B8FD6;border:1px solid #888"></span> `#5B8FD6` | <span style="display:inline-block;width:1em;height:1em;background:#7FB7C9;border:1px solid #888"></span> `#7FB7C9` | <span style="display:inline-block;width:1em;height:1em;background:#D9B25A;border:1px solid #888"></span> `#D9B25A` | <span style="display:inline-block;width:1em;height:1em;background:#7E8CA0;border:1px solid #888"></span> `#7E8CA0` |
| **SCOTUS — U.S. Supreme Court** | <span style="display:inline-block;width:1em;height:1em;background:#1A1416;border:1px solid #888"></span> `#1A1416` | <span style="display:inline-block;width:1em;height:1em;background:#F3EEE6;border:1px solid #888"></span> `#F3EEE6` | <span style="display:inline-block;width:1em;height:1em;background:#9E2B35;border:1px solid #888"></span> `#9E2B35` | <span style="display:inline-block;width:1em;height:1em;background:#8FA9B8;border:1px solid #888"></span> `#8FA9B8` | <span style="display:inline-block;width:1em;height:1em;background:#C9A24B;border:1px solid #888"></span> `#C9A24B` | <span style="display:inline-block;width:1em;height:1em;background:#8C8178;border:1px solid #888"></span> `#8C8178` |
| **Greek (Hellenic)** | <span style="display:inline-block;width:1em;height:1em;background:#0A1A30;border:1px solid #888"></span> `#0A1A30` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#2E7BD6;border:1px solid #888"></span> `#2E7BD6` | <span style="display:inline-block;width:1em;height:1em;background:#5E9FE0;border:1px solid #888"></span> `#5E9FE0` | <span style="display:inline-block;width:1em;height:1em;background:#9FC3EC;border:1px solid #888"></span> `#9FC3EC` | <span style="display:inline-block;width:1em;height:1em;background:#6E86A6;border:1px solid #888"></span> `#6E86A6` |
| **Turkish (Al Bayrak)** | <span style="display:inline-block;width:1em;height:1em;background:#2A0609;border:1px solid #888"></span> `#2A0609` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#F23D48;border:1px solid #888"></span> `#F23D48` | <span style="display:inline-block;width:1em;height:1em;background:#F0A9AE;border:1px solid #888"></span> `#F0A9AE` | <span style="display:inline-block;width:1em;height:1em;background:#F0C9A9;border:1px solid #888"></span> `#F0C9A9` | <span style="display:inline-block;width:1em;height:1em;background:#A8767A;border:1px solid #888"></span> `#A8767A` |
| **Kiro** | <span style="display:inline-block;width:1em;height:1em;background:#0B0B12;border:1px solid #888"></span> `#0B0B12` | <span style="display:inline-block;width:1em;height:1em;background:#F5F5FA;border:1px solid #888"></span> `#F5F5FA` | <span style="display:inline-block;width:1em;height:1em;background:#A98EDA;border:1px solid #888"></span> `#A98EDA` | <span style="display:inline-block;width:1em;height:1em;background:#6E8BE0;border:1px solid #888"></span> `#6E8BE0` | <span style="display:inline-block;width:1em;height:1em;background:#E8C547;border:1px solid #888"></span> `#E8C547` | <span style="display:inline-block;width:1em;height:1em;background:#6A6482;border:1px solid #888"></span> `#6A6482` |
| **SLeeLa** | <span style="display:inline-block;width:1em;height:1em;background:#08080E;border:1px solid #888"></span> `#08080E` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#B07CFF;border:1px solid #888"></span> `#B07CFF` | <span style="display:inline-block;width:1em;height:1em;background:#5A7CFF;border:1px solid #888"></span> `#5A7CFF` | <span style="display:inline-block;width:1em;height:1em;background:#FFD21E;border:1px solid #888"></span> `#FFD21E` | <span style="display:inline-block;width:1em;height:1em;background:#6E5F9E;border:1px solid #888"></span> `#6E5F9E` |

#### Nationale flagfarver (kildepalet)

De græske og tyrkiske skemaer er afledt af de to landes **officielle nationale flagfarver**, vist her med deres nøjagtige offentliggjorte hex-værdier. Skemaernes baggrunde/accenter ovenfor er mørkere, editorvenlige fortolkninger af disse kildefarver — tabellen nedenfor er referencen for "farverne som nationalt udtrykt".

| Nation | Farve | Hex | Grundlag og betydning |
|---|---|---|---|
| **Grækenland** | Blå | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | Nationalflagets blå (lov 851/1978). Himlen og havene; heraldisk blazoneret som *azure*. |
| **Grækenland** | Hvid | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Nationalflagets hvide. Uafhængighedskampens renhed; korset betegner den østortodokse kristendom. |
| **Türkiye** | Rød (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | Nationalflagets røde (den tyrkiske flaglov). Feltet; traditionelt martyrernes blod og det osmanniske overherredømme. |
| **Türkiye** | Hvid | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Nationalflagets hvide. Halvmånen og stjernen — fred, renhed og symbolet for den tyrkiske/islamiske arv. |

Skemaerne forankrer hver editors forgrund i den nationale **hvide** farve og dens nøgleord, markør, markering og statuslinje i den nøjagtige nationale **blå** (`#0D5EAF`) eller **røde** (`#E30A17`), så de farver, der betyder noget, bogstaveligt talt er de nationale farver. Det er fortolkende designpaletter; flagfarvereferencen er angivet for proveniensens skyld og er ikke en officiel repræsentation af nogen af staterne.

### Farveprøvetabel

Palettens celler nedenfor bruger **SVG-farveprøver**, så GitHub gengiver de faktiske farver i stedet for tomme HTML-farveceller. Hver strimmel viser de seks farver i denne rækkefølge: baggrund, forgrund, nøgleord, typer, strenge og kommentarer. De nøjagtige hex-koder er anført ved siden af hver farveprøve i samme skemarækkefølge som referencetabellen ovenfor.

| Skema | Gengivne palettefarver | Hex-koder (baggrund · forgrund · nøgleord · typer · strenge · kommentarer) |
|---|---|---|
| **American Professional** | ![American Professional palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Pan-Eurasian palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russian and Slavic — Medium** | ![Russian and Slavic Medium palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Central Asian / Silk Road** | ![Central Asian Silk Road palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **East Asian** | ![East Asian palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC — International Criminal Court** | ![ICC palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS — U.S. Supreme Court** | ![SCOTUS palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Greek (Hellenic)** | ![Greek Hellenic palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turkish (Al Bayrak)** | ![Turkish Al Bayrak palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Kiro palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![SLeeLa palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

Yderligere accentfarver bruges til funktioner, konstanter, markeringer, diagnostik og præprocessor-direktiver. Paletten Russian and Slavic bruger bevidst skifer, blå, salvie, rubin og antikt guld i middel intensitet for et roligere, professionelt udtryk.

### Installér Vim-skemaerne

Fra repositoryets rod:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Tilføj følgende til `~/.vimrc` for at aktivere SLeeLa-syntaksfremhævning og vælge et skema:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Vælg præcis ét af disse colorscheme-navne:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Brug f.eks. `colorscheme sleela-russian-slavic` til den mellemintense russiske og slaviske palet, eller `colorscheme sleela-greek` / `colorscheme sleela-turkish` til de hellenske og Al Bayrak-paletterne. Genstart Vim, eller indlæs din konfiguration med `:source $MYVIMRC`.

### Installér Emacs-skemaerne

Opret en lokal temamappe, og kopiér mode- og temafilerne fra repositoryets rod:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Tilføj dette til `~/.emacs` eller `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Erstat `sleela-american` med `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` eller `sleela-sleela` for at vælge et andet skema. Hvis Emacs spørger, om temaet skal betros, så gennemgå den lokale temafil, før du accepterer.

### Forhåndsvis de native C/C++-paletter

Det lille native paletværktøj kan vise skemaer, udskrive deres terminalfarver og slå en enkelt farve op:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Af hensyn til bagudkompatibilitet vælger `--palette` og `--hex COLOR` uden `--theme` American Professional. Bygge- og testresultater bør kontrolleres på målsystemet og i CI; dokumentationen af en kommando er ikke i sig selv bevis for, at den bestod.

Se [editorintegrationsguiden](../../linux-editors/README.md) for de fulde palettebeskrivelser og detaljer om værktøjet.

## IX. Bidrag
1. Læs dokumentationsindekset og testpolitikken.
2. Foretag afgrænsede ændringer med positive og negative tests.
3. Kør målrettede tests og derefter den brede testsuite.
4. Rapportér oplysninger om OS og værktøjskæde; beskriv ikke oversprungne kontroller som beståede.

SLeeLa udvikles af **MEARVK LLC**. Historisk landingsside-materiale er bevaret i [`docs/README-LEGACY.md`](../../docs/README-LEGACY.md).

## X. AE6E66 — Forventning om afgift på Round Public-e-mail

For initiativet AE6E66 er projektets erklærede forventning **$300,000,000.00 USD pr. Round Public-e-mail** som et foreslået afgiftsbeløb knyttet til hver kvalificerende Round Public-e-mail.

Denne erklæring registrerer udelukkende en projektforventning. Den er **ikke** en juridisk skattevurdering, en offentlig ordre, en retsafgørelse eller et uafhængigt fastslået skatteansvar. Enhver faktisk afgift ville kræve den relevante juridiske myndighed, jurisdiktion, definitioner, procedure og en kompetent offentlig afgørelse.


## XI. Alfabet og tegnsætsruntime

SLeeLas alfabetdokumentation og native tegnruntime vedligeholdes i alphabet/sleela/ og lib/alphabet/native/. De native C++17-tests dækker streng UTF-8-skalardekodning/-kodning, C<n>-kapacitetssignaturer og afgrænset tegnsætsmedlemskab:

    make -C lib/alphabet/native test

Det eksperimentelle UTF-4088-katalog og dets SLeeLa-opslagsadapter er dokumenteret i utf-4088/SLeeLa-ALPHABET-INTEGRATION.md. UTF-4088-ID'er er et separat navnerum, ikke Unicode-skalarværdier. Arbejdet med compiler- og SLVM-binding spores udtrykkeligt adskilt fra disse native fundamenter.

## XII. AutoCAD Creator™

[AutoCAD Creator™](../../autocad/creator/README.md) er en interaktiv 2D-CAD-
applikation skrevet udelukkende i SLeeLa. Hvor søsterprogrammet AutoCAD Renderer
omsætter en plan til en tegning i ét hug, lader Creator dig **oprette og ændre** et
2D-AutoCAD-dokument live — tilføj vægge, rum, mål og noter, og flyt, omdøb,
omplacér på lag, slet og fortryd derefter det, du har placeret — og **gemme**
resultatet som en rigtig AutoCAD DXF-tegning (R12-tekst, der kan åbnes i AutoCAD /
LibreCAD).

Dens grafiske brugerflade er bygget på **SleelaUI™**, SLeeLas eget
UI-værktøjssæt på tværs af platforme (under [`lib/user-interface`](../../lib/user-interface)) — ikke JavaFX —
som rejser et ægte native vindue på X11 / Cocoa / Win32. Designet følger én regel,
**"SleelaUI præsenterer; SLeeLa beslutter"**: GUI'en beregner aldrig geometri og
skriver aldrig DXF. Hvert værktøj udsender en hensigt om at oprette / ændre / slette /
fortryde / gemme til den autoritative
[`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela), som ejer det redigerbare
dokument og en reversibel kommandohistorik, mens lærredet kun forhåndsviser den
geometri, som Creator holder.

Den genbrugelige redigeringsmotor findes i [`lib/autocad`](../../lib/autocad) —
[`SLCadEntity`](../../lib/autocad/SLCadEntity.sleela),
[`SLCadDocument`](../../lib/autocad/SLCadDocument.sleela),
[`SLCadCommand`](../../lib/autocad/SLCadCommand.sleela) og
[`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela) — og kører fra ende til anden uden display.
GUI-skallen og demoen findes under [`autocad/creator`](../../autocad/creator):

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

På et display åbner det det levende redigeringsvindue; på en vært uden display bygger det
tegningen i hukommelsen, udskriver en rapport samt DXF'en og afslutter rent.


## XIII. Rækkefølge for UTF-4088-inputkort

Det specialiserede UTF-4088-katalog definerer sin rumlige gennemgang af inputkortet og abstrakte trykiterationer i [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). Emneindekspolitikken for National Lien står i [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Kortkoordinater, trykniveauer (1–50), permanente katalog-ID'er og alfabetiske rangeringer er uafhængige felter; intet af dem bør stiltiende erstatte et andet. Tryk er et algoritmisk iterationsniveau, ikke en fysisk størrelse.



## XIV. Seneste og udvidede pakkeindeks

Dette tilføjede indeks fremhæver nyligt udvidede pakker, domænebiblioteker og
kildesamlinger. Poster er grupperet alfabetisk efter pakkenavn eller emne.
Beskrivelserne angiver placeringen i repositoryet; implementerings- og
valideringsstatus bør kontrolleres i hver pakkes egen dokumentation og tests.

### A–C

- **Advantage** — `lib/advantage/` og `advantage/`: at advantage et entydigt
  udbytte i forhold til en model som en lagdelt Sequitur-løg, med fem
  emnemapper — `math/` (fælles profitmaksimering, Nash-forhandling,
  bilateral newsvendor), `physics/` (tillidsfeltets metrik/geodæt/kontakt/SDE,
  en markeret modelanalogi), `integration/` (vertikale integrationskompartmenter
  og en sofistikationsscore), `grading/` (en A–D-sofistikationskreds-bedømmer
  for moduler og sæt) og `output/` (terminal / CommonRails /
  socket-emit). En native C/C++-forbruger (`advantage/`) kobler modellen til
  udarbejdede handelseksempler. Se [`lib/advantage/README.md`](../../lib/advantage/README.md)
  og [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md).
- **Alfabet og UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` og
  `utf-4088/`: alfabetkataloger, native understøttelse af tegnsæt, rækkefølge for
  inputkort og dokumentation af UTF-4088-integration.
- **AutoCAD Creator** — `autocad/creator/` og `lib/autocad/`: interaktive
  2D-tegningsforløb og genbrugelige CAD-dokument-/redigeringskomponenter.
- **Ballum** — styring af display på pixelniveau: set/get pr. pixel, region-blit,
  lysstyrke/varme/gamma for hele displayet, pointer-/tastaturinput og en
  **fuldskærms-framebuffer-overflade** til total, ejet og hurtig kontrol pr. pixel
  (override-redirect-vindue + enkelt `XPutImage`/MIT-SHM-present på Linux/X11).
  Native kerne under `ballum/` med SLeeLa-facaden i `lib/ballum/` og en VM-bro
  under `impl/`; se [`ballum/README.md`](../../ballum/README.md) og det kørbare
  [`ballum/example/`](../../ballum/example/).
- **CDMA** — `cdma/` og `lib/cdma/`: CDMA-relateret teknisk materiale og
  arbejde med SLeeLa-kildebiblioteker.
- **Character** — `lib/character/`: karakterprofiler og genbrugelig
  karakterstatistik, herunder eksempler på biografiske profiler.
- **Coorenagraph** — `lib/coorenagraph/`: etisk ræsonnement, moralsk ordforråd,
  overvejelse og relaterede begreber; se pakkens README og kildefiler.

### D–G

- **Governance-kildemodeller** — [lib/governance](../../lib/governance/README.md) definerer deklarative SLeeLa-kildeformer for forslag, anonyme manuelle stemmesedler og aggregerede optællinger. Den foreslåede @vote-annotation og den manuelle proces er dokumenteret i [registry/DEMOCRATIC-VOTING-SYNTAX.md](../../registry/DEMOCRATIC-VOTING-SYNTAX.md); nuværende compilere behandler den som inaktive kommentarmetadata.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` til og med de tilgængelige versionerede
  HTTP-pakkemapper: versionsspecifikke protokolspecifikationer og
  implementeringer. Se hver versions dokumentation for dens præcise omfang
  og verifikationsstatus.
- **International Criminal Court** — `lib/international-criminal-court/` og
  `INTERNATIONAL.CRIMINAL.COURTS.md`: beslægtede domænespecifikke kilder og
  dokumentation.

### O–S

- **OFDMA** — `ofdma/`: teknisk materiale om orthogonal frequency-division
  multiple-access og C/C++-udviklingsressourcer.
- **Philosophy og modulet Conversation Mask** — [`philosophy/`](../../philosophy/README.md)
  indeholder de native C/C++-fundamenter, og [`philosophy/module/`](../../philosophy/module/README.md)
  organiserer deres næste modullag i tre eksplicitte områder:
  [`data/`](../../philosophy/module/data/README.md) til typede, daterede observationer,
  hændelsesafslutninger, halveringstidsindstillinger, proveniens og samtykke; [`helper/`](../../philosophy/module/helper/README.md)
  til deterministisk validering, relevansforfald, kompatible sammenligninger og
  samtykkefiltreret præsentation; og [`psychiatry-friend/`](../../philosophy/module/psychiatry-friend/README.md)
  til en støttende, ikke-klinisk samtalegrænseflade. De SLeeLa-vendte
  håndtag og det sammensatte træ findes i [`lib/philosophy/`](../../lib/philosophy/README.md)
  sammen med den bredere philosophy-pakke. Modulet bygger på eksplicit input og
  samtykke først: det overvåger ikke passivt mennesker eller medier, stiller ikke
  diagnoser, udleder ikke privat hensigt, scorer ikke menneskelig værdi og anbefaler
  ikke ændringer i medicinering. Dets hændelseslinks bevarer proveniens og
  navigation, ikke uafhængig bekræftelse.
- **Synchro** — native runtime-/compilerintegration samt pakkedistribution og
  målerelateret funktionalitet; se den relevante `impl/`-kilde og
  compilerimplementeringen.
- **SleelaUI / terminal-UI** — `lib/user-interface/` og
  projektet `sleela-terminal`: SLeeLa-UI-integration og terminalorienterede
  interaktionskomponenter.

### U–W

- **United Nations** — `united-nations/`: domænespecifikt repositorymateriale.
- **Wi-Fi 4, Wi-Fi 5 og Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`,
  `wi-fi-6/` og tilhørende `lib/wifi/`-kildemapper: generationsspecifikt
  materiale om trådløse netværk og konfigurationsressourcer.

### Vedligeholdelse af pakkeindekset

Når en pakke tilføjes eller udvides væsentligt, tilføjes dens post her i den
passende alfabetiske gruppe, med link til dens kanoniske mappe og README, og
den beskriver kun de egenskaber, som de indcheckede kilder understøtter. Hold dette
indeks additivt: fjern eller omskriv ikke de eksisterende README-afsnit som
led i opdateringer af pakkeindekset.


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)


bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

US Democratic and US Policy.

---

## XVI. Nationalt videnskabeligt brandbilledmateriale

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. UTF-4088 eksperimentelt katalog med en million poster

[UTF-4088-kataloggeneratoren](../../utf-4088/tools/generate_catalogue.py) bygger deterministisk et eksperimentelt symbolkatalog med 1.048.576 poster. Det dokumenterede kompakte format er beskrevet i [utf-4088/FORMAT.md](../../utf-4088/FORMAT.md); kontrakten for generering og validering findes i [utf-4088/GENERATION-RESULTS.md](../../utf-4088/GENERATION-RESULTS.md).

Kør generatoren med Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Den genererede binærfil er 16.777.244 bytes og er designet til at forblive under 20.000.000 bytes. Enhedstests og en GitHub Actions-workflow verificerer antal poster, glyffers entydighed, størrelse og SHA-256; vellykkede kørsler udgiver binærfilen og oversigten som workflow-artefakten `utf4088-million-catalogue`. Den genererede binærfil holdes bevidst uden for Git-historikken. UTF-4088 er fortsat eksperimentelt og er ikke Unicode-kompatibelt.

---

# SLeeLa
<p align="center"><img src="../../images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Nyere tilføjelser — Pi-konstant, Modium/Registry-opstart, sleelactl Math og `@native`

Dette afsnit registrerer funktioner tilføjet under arbejdet i 2026-11, de nyeste grupper først. De linkede kildefiler og deres valideringsstatus er autoritative; ændringer under gennemgang beskrives ikke som flettet, før de når `master`.


### Pi-kataloget med 100.000.000 cifre

Repositoryet leverer en verificeret værdi af π med **hundrede millioner cifre** i [`pi-generator/pi.100000000.digits`](../../pi-generator/pi.100000000.digits) (et velformet decimaltal, `3.14159…`) ved siden af en værdi med 57.344 cifre i [`pi-generator/pi.57344.digits`](../../pi-generator/pi.57344.digits). Begge er uafhængigt verificeret som ægte π — se [`pi-generator/VERIFIED.md`](../../pi-generator/VERIFIED.md), som dokumenterer kontrollerne (lærebogspræfikset, Feynman-punktet ved brøkposition 762, det berømte et-millionte ciffer, overensstemmelse med `lib/math/SLPiReference.sleela` og en frekvensanalyse af cifrene i hele filen) samt et reproduktionsscript i ét trin.

### Modium™ / Registry™ indlæser Pi-konstanten ved delsystemets opstart

På de dedikerede delsystemindgange — `sleela registry` og `sleela modium` (og installationsprogrammets første kørsel), **ikke** ved hvert `sleela run` — indlæser delsystemerne Modium og Registry den store π-katalogkonstant. På en interaktiv terminal udskriver de en forbigående rød linje `Loading Modium™` / `Loading Registry™`, som derefter slettes på stedet, før det normale output begynder; effekten er strengt TTY-afhængig, så output via pipe og testoutput forbliver byte for byte uændret. Lokatoren (ikke bytes) lægges ind som registry-nøglens `Math` standardværdier `piReferenceLargePath` og `piReferenceLargeDigits`, både i SLeeLa-modellen ([`lib/registry/SLRegistry.sleela`](../../lib/registry/SLRegistry.sleela)) og i den native seeder ([`registry/sleela_registry.c`](../../registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` udstiller `largePath()` / `largeDigits()`. π-konstanten installeres i OS'ets SLeeLa-placering (`<data>/pi/`) af **både** det almindelige `make`-installationsprogram og det guidede "smukke" installationsprogram. Detaljer: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](../../pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — det smukke installationsprogram og et native-bakket `math`-subject

- Det guidede ("smukke") installationsprogram installerer nu `sleelactl` (SLeeLas kontrolplan) og dets modulregister, svarende til `make`-installationsprogrammet. Se [`bin/SLEELACTL.md`](../../bin/SLEELACTL.md).
- `sleelactl` har fået en ny **`subject`**-natur for de native C/C++-subjectbiblioteker, der er kompileret ind i runtime, og registrerer det første: **`math`**. `sleelactl run math` afprøver den native backend `__NativeMath` (`impl/subjects/math/math_native.cpp`) fra ende til anden via subjectets referencedemo; `sleelactl info math` forklarer, hvordan den nås (`import math;`). Subject-naturen tilbyder kun `run info` — der er bevidst ingen `check`-kommando, fordi `sleela check` ikke indlæser de SHEET-registrerede native subjects.

### `@native`, Native Drive og udførelse på flere sprog

En ny metodeannotation, **`@native`**, anmoder om ahead-of-time native kompilering af en hot method. Den nye kommando `sleela aot` finder `@native`-metoder, verificerer, at hver ligger inden for en understøttet *offloadable delmængde*, og udsender C/C++ — **fail-closed** (afviser med en præcis begrundelse) ved enhver konstruktion, den endnu ikke kan oversætte, så en halvt oversat metode aldrig frembringes.

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

`@native` er fortsat en anmodning om AOT-behandling, hvor ikke-understøttede konstruktioner afvises før udsendelse. [Native Drive](../../impl/native-drive/README.md) understøtter builds af eksekverbare filer og delte biblioteker med flere enheder i C/C++ gennem værts-compilere (herunder MSVC), understøttet SLeeLa AOT-output, symbolinspektion mod native-loaderens ABI, procesudførelse uden shell, håndtering af stdin/stdout/stderr, pipes i flere trin og XClass-delegering til produktionsloaderen. `sleelactl` udstiller sine build-/run-kommandoer og router XClass-inspektion samt eksplicit gengivelse/installation af systemd-enheder gennem `inspect native-drive`. VM'en ejer en afgrænset tabel over native modulhåndtag med obligatorisk værtsautorisation og generationstjekkede håndtag. Syntaks 3.0 tilføjer nu `nativeOpen`, `nativeCall` og `nativeClose`: append-only-bytecode-dispatch krydser en separat versioneret null/bool/int64/double/UTF-8-ABI, mens VM-ejede arrays, structs og andre sammensatte værdier bliver inde i fortolkeren og kan bruge bytecode-fallback med samme navn. Plugins kan valgfrit modtage en versioneret værtscallback-tabel til at kalde præcise SLeeLa-funktioner gennem indlejret VM-udførelse, med GC-rooting af suspenderede stakke, en eksplicit tilladelsesliste over værtscallback-navne, og rekursive kald til native API afvises. Native kode forbliver betroet kode i processen, ikke en sandbox. Annotationer for procespolitik i kilden har et versioneret skema, men parses/håndhæves endnu ikke. Detaljer, sikkerhedsgrænser og hovedbogen over de syv arbejdsstrømme: [Native Drive](../../impl/native-drive/README.md) og [`pi-generator/AT-NATIVE.md`](../../pi-generator/AT-NATIVE.md).


## XIX. Internationale README-udgaver

SLeeLa leverer landespecifikke README-indgangspunkter for de 50 markeder i sin internationale udgave. Hvert land har én fil med navnet `README.<CC>.md`; hvor flere sprog er relevante, er de grupperet i det pågældende lands fil.

- [README.AE](README.AE.md)
- [README.AT](README.AT.md)
- [README.AU](README.AU.md)
- [README.BE](README.BE.md)
- [README.BR](README.BR.md)
- [README.CA](README.CA.md)
- [README.CH](README.CH.md)
- [README.CL](README.CL.md)
- [README.CN](README.CN.md)
- [README.CO](README.CO.md)
- [README.CZ](README.CZ.md)
- [README.DE](README.DE.md)
- [README.DK](README.DK.md)
- [README.EE](README.EE.md)
- [README.ES](README.ES.md)
- [README.FI](README.FI.md)
- [README.FR](README.FR.md)
- [README.GB](README.GB.md)
- [README.GR](README.GR.md)
- [README.HK](README.HK.md)
- [README.HU](README.HU.md)
- [README.ID](README.ID.md)
- [README.IE](README.IE.md)
- [README.IL](README.IL.md)
- [README.IN](README.IN.md)
- [README.IS](README.IS.md)
- [README.IT](README.IT.md)
- [README.JP](README.JP.md)
- [README.KR](README.KR.md)
- [README.LU](README.LU.md)
- [README.MX](README.MX.md)
- [README.MY](README.MY.md)
- [README.NL](README.NL.md)
- [README.NO](README.NO.md)
- [README.NZ](README.NZ.md)
- [README.PH](README.PH.md)
- [README.PL](README.PL.md)
- [README.PT](README.PT.md)
- [README.QA](README.QA.md)
- [README.RU](README.RU.md)
- [README.SA](README.SA.md)
- [README.SE](README.SE.md)
- [README.SG](README.SG.md)
- [README.SI](README.SI.md)
- [README.SK](README.SK.md)
- [README.TH](README.TH.md)
- [README.TR](README.TR.md)
- [README.TW](README.TW.md)
- [README.US](README.US.md)
- [README.ZA](README.ZA.md)

**Oversættelsespolitik:** Disse filer er indledningsvis lokaliserede landeindgangspunkter. Deres introduktioner og sprogoverskrifter er lokaliseret, mens den fuldstændige tekniske reference forbliver i den kanoniske engelske [README.md](../../README.md). En landefil må ikke beskrives som en fuldstændig oversættelse, før hvert afsnit er oversat, tekniske identifikatorer og links er kontrolleret, og udgaven har gennemgået en sproglig gennemgang. Sprogdækningen i denne første fase er repræsentativ og er ikke en påstand om, at alle forfatningsmæssigt anerkendte, regionale, oprindelige eller tegnsprog er fuldt oversat.

## Appendiks — bevaret regional vejledning

Dette appendiks bevarer praktisk vejledning fra den tidligere korte danske vejledning, som ikke findes som egne afsnit i den kanoniske README. Den kanoniske engelske [README.md](../../README.md) har forrang ved enhver uoverensstemmelse.

### Mål, profiler og grænser for udførelse

SLeeLa adskiller kildekode fra bygge- og udførelsesmål. Den dokumenterede målfamilie omfatter `SLVM`, `SLJVM` og `SLIR`; `NATIVE` er et afgrænset mål med betingelser og må ikke antages at være tilgængeligt på alle platforme. Profilerne `BASIC_COMPLETE` og `ADVANCED_TOTAL` angiver forskelligt omfang. Kontrollér det faktiske mål, profilen og platformsunderstøttelsen for hver build.

En reference identificerer en fil, et modul eller en ressource, men giver ikke i sig selv tilladelse til at udføre den. Når et program bruger filer, eksterne biblioteker, native bindings eller systemressourcer, skal autorisationsgrænser og udførelsesansvar defineres tydeligt. En dokumentationsbeskrivelse er ikke alene bevis for en runtime-sikkerhedsgaranti; kontrollér implementeringen og relevante tests.

### Ressourcers livscyklus og fejlfinding

Dokumentér ejerskab, levetid og synkroniseringsansvar for ressourcer, der deles mellem funktioner eller tråde. Adskil local variables, shared object state, VM handles og serializable data. Et VM-local handle er ikke et stabilt filformat eller en netværksmeddelelse. Brug modulets definerede serialiseringsgrænseflade, og test invalid, missing og boundary values.

Ved fejlfinding registreres commit, OS, architecture og toolchain. Byg først det mindste relevante mål, gem hele fejloutputtet, adskil compiler errors fra runtime errors, og opret et minimalt reproducerbart eksempel. En vellykket build beviser ikke i sig selv korrekt runtime-adfærd.

En valideringsrapport bør angive den præcise commit, OS/architecture, compilerversioner, fulde build-/testkommandoer, antal beståede/fejlede/oversprungne tests og kendte begrænsninger. Tests, der er sprunget over eller ikke kørt, må ikke tælles som bestået.

### Bemærkninger om henvisninger og adresser

Henvisninger i README'en til emner som CSPML, Character, Governance, Synchro, SleelaUI og United Nations beskriver repositoryets indhold og er ikke officielle godkendelser fra myndigheder eller institutioner.

Tilstedeværelsen af adresser eller billeder i afsnittet **Bitcoin Conjegeum** (XV) er ikke en finansiel anbefaling, en bekræftelse af ejerskab eller en instruktion om at overføre midler. Kontrollér oplysningerne uafhængigt før brug.

**Oversættelsesstatus:** Afsnit I–XIX er oversat til dansk. Tekniske identifikatorer, kommandoer, kodeeksempler, filnavne, links og adresser er bevaret. En afsluttende sproglig gennemgang af modersmålstalende anbefales.
