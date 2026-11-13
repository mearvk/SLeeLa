# SLeeLa 3.0 — Pilipinas (PH)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Isang katutubong wika sa pagprograma ng sistema, compiler, runtime, at ekosistema ng mga virtual machine.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Pandaigdigang edisyon para sa bansa.** Ang edisyong ito ay nasa wikang Filipino (Filipino · English ang pamagat ng naunang gabay). Hindi binabago ang mga technical identifier, command, code example, filename, path, at link kumpara sa canonical na English [README.md](README.md).

**The Now. The Ingredient.** Hindi lamang syntax ang isang wika: ito ang compiler, runtime, mga library, mga tool, at mga kasunduan ng platform na nagpapagamit sa software.

**Ano ang kinasasangkutan ng SLeeLa.** Kasangkot ang SLeeLa sa **Math**, **Object**,
**Science**, at **Reach** — ang apat na mukha ng gawain nito: ang nakukwenta
(Math), ang naitatag na bagay na nakatayo sa katalogo ng kung ano ang umiiral (Object), ang
disiplinadong pag-aaral ng totoo (Science), at ang pag-abot ng kamay ng isang sistema
sa ibang mga sistema at tao (Reach). **Nananatili ang mga National Interest** — hindi
sila isinasantabi ng alinman sa apat; kapag may kinalaman ang isang paksa sa interes ng isang bansa,
iniingatan ang katayuang iyon (itinatala nang tapat, hal. sa `metaOrigin` / `metaNationalSchedule`
ng Metadata ng bawat class), at hindi kailanman binubuwag.

## I. Magsimula Rito
- [Buuin ang native implementation](impl/README.md)
- [Mga virtual machine at Modium — praktikal na gabay ng user](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [CPU stack sa SLeeLa source](lib/cpu/CPU.md) — isang buong makinang gates→ALU→cores→OS→programs, isang listahan ng mahigit 130 CPU profile, at maiaakma na [mga voltage, variable na clock, at expert na timing](lib/cpu/TIMING.md) na maaari mong [patakbuhin ayon sa spec at i-overclock](lib/cpu/components/README.md)
- [Indeks ng dokumentasyon](docs/README.md)
- [CSPML data source at sanggunian sa pananaliksik na Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — pinagmulan ng datos at gabay sa integrasyon para sa panlabas na datos ng U.S. tungkol sa lugar, sosyo-ekonomiya, lakas-paggawa, edukasyon, at pampublikong paaralan.
- [Nabuong dokumentasyon ng API](docs/api/README.md) — kasalukuyang HTML reference, may bersiyong edisyon ng API, at mga tagubilin sa muling pagbuo
- [Katalogo ng API](api/README.md)
- [Mga pamantayan sa paghahambing ng computation](comparison/README.md) — mga pamantayan ng SLeeLa VM, Java/JVM, at native C/C++ sa type, memory, ABI, at conformance.
- [Recursion stanchion — mga prototype sa C at C++](recursion/stanchion/README.md) — mga halimbawang espesyalisado sa primitive para sa Boolean, Byte, Character, Integer, Long, Float, at Double, kasama ang generic, applied, scientific, at teaching na mga pattern ng recursion.
- [Suporta sa platform at validation matrix](docs/PLATFORM-SUPPORT.md)
- [Brand guide](docs/BRAND-GUIDE.md)
- [Mga konsepto at asset ng merchandise](merchandise/README.md)
- [NitroWebExpress logo](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Patakaran sa testing at mga release gate](markdown/TESTING.md)

## II. Balita

Mga kamakailang dagdag sa wika, mga driver, at standard library:

- **Ang `advantage` standard-library package — pag-advantage ng isang dividend, na may
  math, physics, integration, grading, at routable na output.**
  Isinasakapsula ng [`lib/advantage/`](lib/advantage/README.md) ang *pag-advantage ng
  natatanging dividend kaugnay ng isang modelo* bilang patong-patong na **Sequitur onion**
  (dividend → review → concerns → maths → additions → subtraction → regrouping
  → iteration), na maaaring tahakin papasok/palabas na may ganap na dynamic update
  at restitute, at inilalantad sa pamamagitan ng simpleng **user's model** at ng
  may ganap na kontrol na **developer's model**. May limang sakop na paksa ito: ang **`math/`**
  na bilateral trade economics (joint profit maximization, Nash bargaining, ang
  bilateral newsvendor critical ratio), ang **`physics/`** na structural-economics na
  "physics of trust fields" (trust bilang metric tensor, ang transactional geodesic, isang
  announcement contact structure, isang CIR+jump trust-decay SDE — isang modeling
  analogy, na minarkahang ganoon), ang **`integration/`** na recursively-composable
  na vertical-integration model para sa capitalism/retail (compartments, engineering
  bays, trial compartments, newton median trays, at system-sophistication
  score), ang **`grading/`** na sophistication circuit grader na nagmamarka sa bawat module
  at sa anumang set ng winning/losing/grader/absolute modules ng A–D, at ang **`output/`**
  na routable na emit seam na nagpapadala ng output ng module sa terminal, sa isang
  surface na naka-format sa **CommonRails**, o sa isang socket/connector (naka-frame para sa native
  na `slnet_write`). Nagdadala ng Metadata-Class record ang bawat class; limang tumatakbong
  demo ang nagva-validate sa mga paksa, at dinodokumento ang mga ito ng kasamang
  [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md),
  [`MATH.md`](lib/advantage/MATH.md), at
  [`PHYSICS.md`](lib/advantage/PHYSICS.md). Isang native na C/C++
  consumer sa ilalim ng [`advantage/`](advantage/README.md) (ang C core
  na `advantage_value`/`advantage_mean`, isang XML knowledge store, isang ligtas na
  HTTPS/SFTP/SSH acquisition + keystore + DH/ECDH transport library) ang nag-uugnay sa
  modelo sa mga gawang
  [halimbawa ng kalakalan](advantage/examples/README.md) — mga positibong kalakalan mula sa
  ipinapalagay na pinagmulang internet, isang audience-relevance sequence (Traders,
  Consumers, Helpers, Healers), at walong modelong niraranggo ng A–D (D = pinakamaraming Cement,
  A = pinakamaraming Brand).
- **Mga loop-timing annotation — `@loopDelay` / `@loopBudget` (+ isang built-in na `sleep(ms)`).**
  Ang mga unang **statement-level, behavioral** na annotation sa SLeeLa:
  isinusulat sa sarili nilang linya bago ang `for` / `while` / `do` loop, naglalagay sila ng
  totoong pause sa pagpapatakbo ng loop. Ang `@loopDelay(ms = 250)` /
  `@loopDelay(seconds = 2)` ay natutulog nang takdang tagal sa bawat iteration (ang payak na
  `@loopDelay(250)` ay default sa milliseconds); ang `@loopBudget(seconds = 10,
  iterations = 1000)` ay pantay na ipinapamahagi ang kilalang bilang ng iteration sa kabuuang
  wall-clock budget. Ibinababa ang dalawa sa bagong append-only na opcode na **`OP_SLEEP`**
  (ISA 157), na sumusuporta rin sa pangkalahatang built-in na **`sleep(<ms>)`** na maaaring tawagin
  kahit saan; portable ang pagpapatupad ng pause (`nanosleep` / `Sleep`). Tinukoy
  sa [`SLEELA.syntax` §32](SLEELA.syntax); ang API reference ay nasa
  [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md);
  tumatakbong
  [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela).
  Ang indeks ng mga dagdag ng session (kabilang ito, ang native na `OP_BIGMUL`
  na big-integer multiply, at ang mga standalone na tool na **[local-generator](local-generator/README.md)**
  at **[pi-generator](pi-generator/README.md)**) ay nasa
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md).
  Ang tatlong nabuong API surface
  ([`sleeladoc-api/`](sleeladoc-api/README.md),
  [`docs/api/`](docs/api/README.md), at ang hindi nababagong
  [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html)
  snapshot) ay muling binuo gamit ang `make api-docs` at taglay na ngayon ang bagong entry ng instruction na `SLISASleep`
  / `SLISABigMul` sa kanilang `vm` package page.
- **SleelaDoc mula sa command line — `sleela docs`.** Maaabot na ngayon ang SleelaDoc
  direkta mula sa terminal. Inililista ng `sleela docs` ang bawat dokumentableng paksa;
  ipinapakita ng `sleela docs crypto` ang isang package; nire-resolve ng `sleela docs stringbuilder`
  ang isang class (hindi sensitibo sa laki ng titik, at sa pamamagitan ng prefix ng `SL` standard-library →
  `SLStringBuilder`) at nagpi-print ng mayamang pahina — **kahulugan, layunin ng disenyo,
  ugnayan, estruktura, user clays, at limang-baitang na mastery ladder** — na may niraranggong
  "did you mean" na resulta kapag malapit na mali ang nai-type. Mapayayaman ng mga may-akda ang anumang pahina sa simpleng
  pagdaragdag ng mga SleelaDoc `@`-tag (`@intent`, `@thesis`, `@axiom`, `@param`,
  `@see`, `@mastery`). Ang `sleela docs --generate [dir]` ay sumusulat ng plain-text na corpus
  (isang pahina bawat class + isang `TOPICS.index`) na binabasa ng CLI offline;
  ang **`make sleeladoc`** ay nagbubundle ng corpus na iyon **at ng katugmang `.sleela` source**
  sa `sleeladoc/build/sleeladoc.jar`. Para sa mga paksang facade-only (hal.
  `psychiatry`, `politico`), hinahanap ng `sleela docs --facade <topic>` ang
  filesystem sa runtime at inililista ang tunay na **mga folder (pagkatapos ay mga file, bilang natural
  na galaw)**, na may hangganan ng mga cap na maaaring i-override ng user na **6 MB / 6000 file / 6
  minuto**. Pinananatili ang mga unit ng `lib/sleeladoc` sa kasalukuyang edisyon,
  **`#sleela 3.0`**. Tingnan ang [`man/man1/sleela.1`](man/man1/sleela.1) at
  [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md).
- **SleelaDoc — solusyon sa dokumentasyon na istilong Javadoc, na may nabuong HTML
  site.** Ang **SleelaDoc** ay ang in-source na kombensiyon ng SLeeLa para sa doc-comment
  (mga angle-bracket comment na may sentinel na `@doc` / `@page`; tingnan ang
  [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)), at ang
  [`lib/sleeladoc`](lib/sleeladoc/README.md) ang **extractor** na nagre-render ng
  `.sleela` unit bilang Markdown o HTML na API page — kung paanong nagre-render ng Java ang `javadoc`,
  na nagdodokumento ng *intent*. Isang kumpleto at mabubrowse na **HTML documentation site para sa
  buong `/lib`** ang nabubuo sa [`/sleeladoc-api`](sleeladoc-api/README.md)
  sa pamamagitan ng `tools/generate-sleeladoc-api.py` (buksan ang `sleeladoc-api/index.html`; malinis na
  istilong blue / light-blue / light-grey / white na may hierarchy na package → class → member
  at glossary na isinulat para sa mambabasang *Student 2*). Naglalabas din ang compiler
  ng mga doc output na minarkahan ng may-akda bilang by-product ng compilation (`--wiki`
  na MediaWiki page + isang originated na source twin), na dinodokumento sa
  [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md). Tumatakbo:
  [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela).
- **XML Document Object Model (`lib/xml`).** Isang XML DOM sa panig ng SLeeLa — isang dokumentong
  nagmamay-ari ng puno ng element/text/comment/CDATA/PI node na may nakaayos na children at
  may pangalang attribute, isang serializer (compact/pretty, XML 1.0 §2.4 escaping), isang
  char-cell parser, at ang `SLXmlAspect` quality register. Ang pagkakapantay ng node ay
  tatlong-baitang na tali ng tiwala (identity → same subject → structure), reflexive,
  symmetric, at deterministic. Tingnan ang [`lib/xml/README.md`](lib/xml/README.md) at
  ang tumatakbong [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela).
- **Mga malapad na signed integer primitive (`long`, `int64`, `int128`).** Ang `int`, `long`,
  at `int64` ay pawang 64-bit signed integer na magkapareho ang type-check at takbo;
  tinatanggap ang `int128` bilang integer type (64-bit ang batayan sa ngayon, at ang tunay na
  128-bit ay dokumentadong susunod na gawain). Pinalalawak ito ng base-2 signed na serye na `int32`..`int16777216`
  (syntax 2.7) at ng katugmang unsigned na serye na `uint32`..`uint16777216`
  (syntax 2.8). Tingnan ang [`SLEELA.syntax`](SLEELA.syntax) §21, §24, §25.
- **Mga `@mix` consequent field (syntax 2.9).** Maaaring magdeklara ang isang class ng maraming
  instance field na may **parehong pangalan** kung bawat ulit ay may `@mix`; nauunawaan ng
  wika ang grupo bilang consequent (isang array / list na may susing index),
  mula itaas pababa na `1,2,3,…` bilang default o manwal sa pamamagitan ng `@mix(1,2,3,...)` na ang bilang ng
  index ay dapat tumugma sa bilang ng mga field. Tingnan ang [`SLEELA.syntax`](SLEELA.syntax) §28.
- **Mga document marker — ang Natural Process, na pinamagatang Preonems
  (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).**
  Sama-samang pinamagatang **Preonems** ang mga hubad na salitang document-marker na ito
  (ang isang marker ay isang *Preonem*; ang set, ang *Preonems* — tingnan ang
  [`SLEELA.syntax`](SLEELA.syntax) §26.1). Mga first-class, declarative-only
  na document marker na nagpapangalan kung ano ang *isang dokumento* ayon sa pagkakasunod ng paggawa:
  `author`/`authorial` (syntax 2.4; hinahawakan ng propesyonal na konseho — ang tanging
  miyembrong may behavioral na epekto), `material;` (naitatag na Source) at `memory;`
  (syntax 2.5), `determine;` (syntax 2.6; isang dokumentong bukas upang prosesuhin pa
  ng doktor, physician, o ghost tungo sa pagkumpleto sa legal na kamay),
  at ang dalawang pinakabago, **`object;`** (isang naitatag na bagay na nakatayo sa katalogo
  ng kung ano ang umiiral — **reified, binigyan ng kongkretong katayuan, sa syntax 2.8**) at
  **`field;`** (ang batayan ng ugnayan kung saan nakatayo at kumikilos ang mga object) —
  parehong may gate mula syntax 2.6. Bawat isa ay hubad, contextual, at version-gated ng
  front end na walang epekto sa parsing, typing, o codegen; sa labas ng posisyon ng marker
  nananatiling ordinaryong identifier ang bawat isa (kaya hindi ginagambala ng `field` ang
  member ng class o ang `.field` access). Tingnan ang [`SLEELA.syntax`](SLEELA.syntax)
  §18–§20, §23, at §26.
- **Nordshrift sa syntax 3.0 na may buong abot sa library.** Nagre-resolve ang Nordshrift
  sa pamamagitan ng parehong recursive na `/lib` index gaya ng compiler, na umaabot sa bawat
  package at `.sleela` unit; iniuulat ng `nordshrift check` ang mga bilang sa buong puno.
- **Recursion stanchion.** Kasama na ngayon sa direktoryong [`recursion/stanchion/`](recursion/stanchion/README.md) ang mga nakalaang C at C++ reference prototype para sa pitong dokumentadong primitive contract, pati generic, applied, scientific, at teaching na halimbawa. Mga reference source ang mga ito; i-compile at subukan bago ituring na verified.

- **Mga angle-bracket comment (`<` / `<<`).** Isang bagong go-forward na istilo ng comment —
  `< line >` at `<< block >>` — na nilelex ng SLeeLa frontend at dokumentado sa
  [`SLEELA.syntax`](SLEELA.syntax) §1.1.
- **Mga radix integer literal.** Tumatanggap na ngayon ang mga `int` literal ng hexadecimal (`0x`),
  binary (`0b`), at octal (`0o`) na anyo na may `_` na digit separator, sa SLeeLa
  source language ([`SLEELA.syntax`](SLEELA.syntax) §1.4) at sa
  [`.xlt` CPU-model format](lib/xlt/README.md).
- **Ang Metadata Class standard sa buong library.** Ang descriptive-metadata
  metapattern (provenance, legalidad, saklaw ng hurisdiksiyon) ay dala na ngayon ng
  bawat `.sleela` file na may class sa ilalim ng `/lib`; tingnan ang
  [`SLEELA.syntax`](SLEELA.syntax) §15 at
  [ang aralin sa tutorial](tutorial/13-metadata-class.md).
- **Mas mayamang text / String family.** Nagkakaroon ang `lib/text` ng totoong in-language na helper,
  isang char-cell view na nagbibigay ng tunay na `charAt`/`substring`/`indexOf`/`reverse`/
  column-alignment, at totoong positional templater — tingnan ang
  [tutorial sa text at String](tutorial/14-text-and-strings.md) at ang tumatakbong
  [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela).
- **First-class na `String` sa mga Nordshrift sheet.** Maaaring magdeklara ang isang `.sst` sheet ng
  typed na `strings:` block na nag-aambag sa component manifest ng build;
  tingnan ang [Nordshrift Sheet Reference](wiki/Nordshrift-Sheet-Reference.md).
- **Tumatakbo ang mga CPU model sa pamamagitan ng parehong opcode at function-call equivalence.** Minamana ng bawat
  `SL<ARCH>CPU` ang batayan ng pagpapatakbo ng native instruction stream
  sa fetch-decode-execute loop nito *at* ng pagpapatakbo ng code ng ipinapalagay na
  ibang proseso sa pamamagitan ng katumbas na mga tawag sa shared VM substrate — tingnan ang
  [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5.

## III. Build at Test

**Mga kinakailangan:** isang C/C++ toolchain (GCC o Clang) at GNU Make; Python 3 para sa
manifest/flow tooling; sa Linux, ang X11/Xrandr development package para sa mga
display product (iniuulat/ini-install ng `make deps` ng isang module ang kailangan nito). Ang
`impl/` build ay fail-closed sa isang pinagkakatiwalaang SHA-256 manifest, na awtomatikong tinutukoy ng root
`Makefile`.

Mula sa malinis na checkout, ang pinakamabilis na simula:

```sh
make                                  # build everything from the repo root
```

O buuin at subukan lamang ang native core:

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

Binubuo ng `make` sa repo-root ang bawat product (core, VM, codecs, churn,
antivirus, …) at nagtatapos sa pagbuo ng **registry** at pag-materialize ng
baseline table nito — kaya ang nabuong puno ay may dalang mare-review na registry agad.
Ang mga mahahalagang target:

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

Iba pang kapaki-pakinabang na target:

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

Pinapatakbo ng `registry` target ang [`build-registry.sh`](build-registry.sh): binubuo nito
ang registry core, inilalagay ang baseline table sa portable text store
na `registry/build/sleela.sleelareg`, at ine-export ang MySQL table script
na `registry/build/registry.sql` (sa pamamagitan ng `sleela registry export`). I-review ito gamit ang
`sleela registry` o `registry/build/sleela-registry review`. Tingnan ang
[gabay sa registry](registry/README.md) at [arkitektura ng Registry 3.0](registry/REGISTRY-3.0-ARCHITECTURE.md).

Kumonsulta sa mga gabay sa build at testing para sa mga kinakailangan at hakbang na partikular sa platform. Hindi nangangahulugang handa na sa release ang bawat library o platform dahil lamang matagumpay ang build ng isang component.

## IV. Pag-install

Matapos buuin ang mga native product, i-install ang mga command at runtime support file:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

Ginagamit ng kumbensiyonal na installer bilang default ang `/usr/local/bin`, `/usr/local/lib/sleela`,
at `/usr/local/share/sleela`. Pinapanatili nito ang umiiral na startup
configuration at tumatangging mag-overwrite ng mga umiiral na command na hindi nito sinusubaybayan. Ang pag-uninstall
ay nag-aalis lamang ng mga file na itinala ng installer, at pinapanatili ang configuration at
mga runtime data na hindi nito sinusubaybayan.

Para sa nakatuon at gabay na deployment ng pangunahing package lamang — ang mga binary na `sleela`,
`sleelvac`, at `nordshrift`, ang standard library, mga protocol, at
mga support module — gamitin ang clean installer:

```sh
make clean-install       # guided install of the main package only
```

Gumagawa ito ng isang pangunahing desisyon (i-install ang pangunahing package), pagkatapos ay pagpili ng saklaw,
at kapag nagtagumpay, isang opsyonal na picker para sa mga piling enhancement module. Para sa
interactive na workflow ng deployment at lahat ng opsyon sa pag-install, tingnan ang
[Mabilis at Ligtas na Pag-install](install/README.md).

## V. Wika at Runtime
Ang awtoritatibong grammar ay ang [`SLEELA.syntax`](SLEELA.syntax). Dinodokumento ng kasalukuyang repository ang syntax **3.0**; magkaiba ang bersiyon ng compiler/tool at ang bersiyon ng source syntax. Idinaragdag ng 3.0 surface ang **mga document marker na `pure` at `gasoline`** (§30) — isang pares ng hubad, contextual, declarative-only na Preonems na halos magkapareho ang ibig sabihin, na ang bawat isa ay nagmamarka sa katayuan ng dokumento ayon sa sarili nitong katapatan. Pinagsasama-sama ng syntax 3.0 ang 1.0–2.9 na language surface sa kasalukuyang normative na edisyon, habang pinapanatili ang lahat ng minimum gate na partikular sa feature. Idinaragdag ng naunang 2.9 surface ang **`@mix` consequent field** (§28) — maaaring magdeklara ang isang class ng maraming instance field na magkapareho ang pangalan kapag bawat isa ay may `@mix`, na nauunawaan bilang isang indexed na grupo (mula itaas pababa na `1,2,3,…` o manwal na `@mix(1,2,3,...)` na ang bilang ng index ay dapat tumugma sa grupo). Dala nito ang base-2 signed/unsigned na wide-integer family na `int32`..`int16777216` (§24) at `uint32`..`uint16777216` (§25) sa ibabaw ng naunang `long`/`int64`/`int128` (§21); ang **keyword supporter document** (§22); at ang **Preonems** — ang mga declarative-only na document marker na `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30), at `artifact` (§35, katayuang pang-lehislatura na iginagradwa ayon sa oras at espasyo), na sama-samang pinamagatang Preonems (§26.1). Tahasang nasa labas ng na-verify na kontrata ng syntax 1.8 ang constructor overload resolution at ang `this(...)`/base-constructor delegation. Kumonsulta sa grammar at sa mga implementation note bago umasa sa isang feature. Idineklara na ngayon ng siyam na source unit ng SleelaDoc ang `#sleela 3.0`; tumatanggap ang nakabahaging compiler gate ng source syntax 1.0–3.0. Ang bagong API build ay nabubuo ng `make api-docs`.

May native na C/C++ implementation ang repository, mga SLVM generation reference, mga module ng standard library, test, halimbawa, at integrasyon sa platform. Nag-iiba ang kalagayan ng implementation at validation ayon sa module; kumonsulta sa specification nito at sa kasalukuyang resulta ng CI sa halip na ipalagay na pare-pareho ang pagkakumpleto.

## VI. Mga Prinsipyo sa Engineering
- **Ebidensiya kaysa pahayag:** ibukod ang ipinatupad, nasubukan, eksperimental, at pinaplanong behavior.
- **Isang kontrata sa bawat hangganan:** idokumento ang syntax, mga API, mga format ng file, mga palagay sa platform, at behavior kapag may failure.
- **Portable sa disenyo:** ihiwalay ang mga backend na partikular sa platform at subukan sa mga native runner.
- **Ligtas na pagkabigo:** subukan ang malformed na input, mga limitasyon ng resource, pagkabigo ng proseso, at mga landas ng cleanup.
- **Reproducible na release:** muling buuin nang malinis at ilathala ang mga artifact hash at release note.

## VII. Antivirus Build at Integridad

Kasama sa Sleelavirin antivirus package ang isang native na SHA-256 scanner, isang opsyonal
na ClamAV adapter, mga integrity manifest ng source/index, at mga checksum ng platform package.
Buuin at subukan ang package mula sa root ng repository:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Nabubuo ang integrity manifest sa ilalim ng `antivirus/build/`; saklaw nito ang mga
tracked na source at documentation input pati ang mga protektadong signature-index file, habang
hindi isinasama ang nabuong output at ang nababagong ClamAV cache. May sarili nilang manifest ang mga snapshot ng
database ng ClamAV. Kasama sa mga release ZIP ang `INTEGRITY-MANIFEST.json`,
`SHA256SUMS`, at isang `.sha256` sidecar. Beripikahin ang mga hash ng na-download na archive laban sa
pinagkakatiwalaang halagang nakuha nang hiwalay: ang checksum na nakatabi sa isang archive ay
hindi lagda ng publisher. Tingnan ang [gabay sa Sleelavirin](antivirus/README.md),
[security checklist](antivirus/SECURITY.md), at
[antivirus build workflow](.github/workflows/antivirus-binaries.yml).

Hindi ituturing na verified ang isang release hangga't hindi pumapasa ang mga kaugnay na CI job sa
eksaktong release commit at hindi nasusuri ang mga nagresultang artifact.


## VIII. Mga Theme ng SLeeLa Editor — Vim at Emacs

Kasama sa SLeeLa ang koordinadong hanay ng mga propesyonal na dark color scheme para sa **Vim** at **Emacs**, pati isang native na C/C++ palette utility. Nananatiling default ang orihinal na scheme na **American Professional**. May sampung kasamang palette na hango sa iba't ibang kultural at institusyonal na inspirasyon sa disenyo; mga interpretibong design palette ang mga ito at hindi opisyal na representasyon ng alinmang bansa, kultura, hukuman, o institusyon.

### Ang Labing-isang Scheme at ang Kanilang mga Kulay

| Scheme | Background | Foreground | Keywords | Types | Strings | Comments |
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

#### Mga kulay ng pambansang watawat (source palette)

Hango ang mga scheme na Greek at Turkish sa **opisyal na kulay ng pambansang watawat** ng dalawang bansa, na ipinapakita rito kasama ang eksaktong inilathalang hex value ng mga ito. Mga dinilimang interpretasyon ang mga background/accent ng scheme sa itaas na angkop sa editor, batay sa mga source colour na ito — ang talahanayan sa ibaba ang sanggunian ng "mga kulay ayon sa pambansang pagkakalantad".

| Bansa | Kulay | Hex | Batayan at kahulugan |
|---|---|---|---|
| **Greece** | Asul | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | Asul ng pambansang watawat (Batas 851/1978). Ang langit at mga dagat; tinutukoy sa heraldry bilang *azure*. |
| **Greece** | Puti | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Puti ng pambansang watawat. Kadalisayan ng pakikibaka para sa kalayaan; ang krus ay sumasagisag sa Eastern Orthodox na Kristiyanismo. |
| **Türkiye** | Pula (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | Pula ng pambansang watawat (Batas sa Watawat ng Turkey). Ang larangan; ayon sa tradisyon, ang dugo ng mga martir at ang soberanya ng Ottoman. |
| **Türkiye** | Puti | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Puti ng pambansang watawat. Ang gasuklay at bituin — kapayapaan, kadalisayan, at simbolo ng pamanang Turkic/Islamic. |

Inaangkla ng mga scheme ang foreground ng bawat editor sa pambansang **puti**, at ang mga keyword, cursor, selection, at status line nito sa eksaktong pambansang **asul** (`#0D5EAF`) o **pula** (`#E30A17`), kaya literal na pambansang kulay ang mga kulay na mahalaga. Mga interpretibong design palette ang mga ito; inilalaan ang sanggunian sa kulay ng watawat para sa provenance at hindi ito opisyal na representasyon ng alinmang estado. Sumusunod ang kahulugan ng mga kulay sa paglalarawan ng state symbols ng MFA ng Hellenic Republic at sa tradisyon ng Batas sa Watawat ng Turkey.


### Talahanayan ng Color Swatch

Gumagamit ang mga palette cell sa ibaba ng **SVG swatch**, kaya ipinapakita ng GitHub ang aktuwal na mga kulay sa halip na mga walang lamang HTML color cell. Ipinapakita ng bawat strip ang anim na kulay sa pagkakasunod na ito: background, foreground, keywords, types, strings, at comments. Nakalista sa tabi ng bawat swatch ang eksaktong hex code, sa parehong pagkakasunod ng scheme gaya ng talahanayan ng sanggunian sa itaas.

| Scheme | Mga kulay ng palette na na-render | Mga hex code (background · foreground · keywords · types · strings · comments) |
|---|---|---|
| **American Professional** | ![American Professional palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Pan-Eurasian palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russian and Slavic — Medium** | ![Russian and Slavic Medium palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Central Asian / Silk Road** | ![Central Asian Silk Road palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **East Asian** | ![East Asian palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC — International Criminal Court** | ![ICC palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS — U.S. Supreme Court** | ![SCOTUS palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Greek (Hellenic)** | ![Greek Hellenic palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turkish (Al Bayrak)** | ![Turkish Al Bayrak palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Kiro palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![SLeeLa palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

May mga karagdagang accent color para sa mga function, constant, selection, diagnostic, at preprocessor directive. Sadyang gumagamit ang palette na Russian and Slavic ng katamtamang tindi ng slate, asul, sage, ruby, at antique gold para sa mas kalmadong propesyonal na hitsura.

### I-install ang mga Vim Scheme

Mula sa root ng repository:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Idagdag ang sumusunod sa `~/.vimrc` upang paganahin ang SLeeLa syntax highlighting at pumili ng scheme:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Pumili ng eksaktong isa sa mga pangalang ito ng colorscheme:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Halimbawa, gamitin ang `colorscheme sleela-russian-slavic` para sa katamtamang palette na Russian and Slavic, o ang `colorscheme sleela-greek` / `colorscheme sleela-turkish` para sa mga palette na Hellenic at Al Bayrak. I-restart ang Vim o i-source ang iyong configuration gamit ang `:source $MYVIMRC`.

### I-install ang mga Emacs Scheme

Gumawa ng lokal na theme directory at kopyahin ang mga mode at theme file mula sa root ng repository:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Idagdag ito sa `~/.emacs` o `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Palitan ang `sleela-american` ng `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro`, o `sleela-sleela` upang pumili ng ibang scheme. Kung itanong ng Emacs kung pagkakatiwalaan ang theme, suriin muna ang lokal na theme file bago tanggapin.

### I-preview ang mga Native na C/C++ Palette

Kayang ilista ng maliit na native palette utility ang mga scheme, i-print ang mga terminal color ng mga ito, at maghanap ng isang indibidwal na kulay:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Para sa backward compatibility, pinipili ng `--palette` at `--hex COLOR` na walang `--theme` ang American Professional. Dapat suriin ang resulta ng build at test sa target na sistema at sa CI; hindi sa sarili nitong ebidensiya ng pagpasa ang dokumentasyon ng isang command.

Tingnan ang [gabay sa editor integration](linux-editors/README.md) para sa kumpletong paglalarawan ng palette at mga detalye ng utility.


## IX. Kontribusyon
1. Basahin ang indeks ng dokumentasyon at ang patakaran sa testing.
2. Gumawa ng nakatuong pagbabago na may positibo at negatibong test.
3. Patakbuhin muna ang mga targeted na test, pagkatapos ay ang malawak na suite.
4. Iulat ang detalye ng OS/toolchain; huwag ilarawan bilang pumasa ang mga check na nilaktawan.

Binuo ang SLeeLa ng **MEARVK LLC**. Pinapanatili ang historikal na materyal ng landing-page sa [`docs/README-LEGACY.md`](docs/README-LEGACY.md).

## X. AE6E66 — Inaasahang Levy sa Round Public Email

Para sa inisyatibang AE6E66, ang ipinahayag na inaasahan ng proyekto ay **$300,000,000.00 USD bawat Round Public email** bilang iminumungkahing halaga ng tax levy na kaugnay ng bawat kwalipikadong Round Public email.

Itinatala lamang ng pahayag na ito ang inaasahan ng proyekto. Ito ay **hindi** legal na tax assessment, utos ng pamahalaan, hatol ng hukuman, o independiyenteng naitatag na pananagutan sa buwis. Mangangailangan ang anumang aktuwal na levy ng naaangkop na legal na awtoridad, hurisdiksiyon, mga depinisyon, pamamaraan, at kakayahang pagpapasya ng pamahalaan.


## XI. Alphabet at Character-set Runtime

Pinananatili ang dokumentasyon ng alphabet ng SLeeLa at ang native character runtime nito sa alphabet/sleela/ at lib/alphabet/native/. Sinasaklaw ng mga native na C++17 test ang mahigpit na UTF-8 scalar decoding/encoding, mga signature ng kapasidad na C<n>, at bounded na character-set membership:

    make -C lib/alphabet/native test

Dokumentado sa utf-4088/SLeeLa-ALPHABET-INTEGRATION.md ang eksperimental na UTF-4088 catalogue at ang SLeeLa lookup adapter nito. Magkahiwalay na namespace ang mga UTF-4088 ID at hindi mga Unicode scalar value. Hiwalay na sinusubaybayan nang tahasan ang compiler at SLVM binding work mula sa mga native na pundasyong ito.

## XII. AutoCAD Creator™

Ang [AutoCAD Creator™](autocad/creator/README.md) ay isang interactive na 2D CAD
application na buong nakasulat sa SLeeLa. Kung ang kapatid nitong AutoCAD Renderer ay ginagawang
drawing ang isang plano nang minsanan, hinahayaan ka ng Creator na **lumikha at magbago** ng 2D
AutoCAD document nang live — magdagdag ng mga pader, silid, dimensyon, at tala, pagkatapos ay ilipat,
palitan ang label, ilipat ng layer, burahin, at i-undo ang inilagay mo — at **i-save** ang resulta bilang
totoong AutoCAD DXF drawing (R12 text, nabubuksan sa AutoCAD / LibreCAD).

Ang graphical interface nito ay binuo sa **SleelaUI™**, ang sariling cross-platform UI
toolkit ng SLeeLa (sa ilalim ng [`lib/user-interface`](lib/user-interface)) — hindi JavaFX — na nagtataas
ng totoong native window sa X11 / Cocoa / Win32. Sinusunod ng disenyo ang isang tuntunin,
**"SleelaUI presents; SLeeLa decides"**: hindi kailanman kinukwenta ng GUI ang geometry o isinusulat ang
DXF. Nag-iisyu ang bawat tool ng create / alter / delete / undo / save na intent sa awtoritatibong
[`SLCadCreator`](lib/autocad/SLCadCreator.sleela), na nagmamay-ari ng nae-edit na document at ng
nababaligtad na command history, habang ipinapakita lamang ng canvas ang preview ng
geometry na hawak ng Creator.

Ang reusable na editing engine ay nasa [`lib/autocad`](lib/autocad) —
[`SLCadEntity`](lib/autocad/SLCadEntity.sleela),
[`SLCadDocument`](lib/autocad/SLCadDocument.sleela),
[`SLCadCommand`](lib/autocad/SLCadCommand.sleela), at
[`SLCadCreator`](lib/autocad/SLCadCreator.sleela) — at tumatakbo mula simula hanggang wakas nang headless.
Ang GUI shell at demo ay nasa ilalim ng [`autocad/creator`](autocad/creator):

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

Sa isang display, binubuksan nito ang live na editing window; sa headless na host, binubuo nito ang
drawing sa memory, nagpi-print ng ulat at ng DXF, at maayos na lumalabas.


## XIII. Pagkakasunod ng Input-map ng UTF-4088

Tinutukoy ng espesyalisadong UTF-4088 catalogue ang spatial input-map traversal at abstract pressure iteration nito sa [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). Nasa [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md) ang patakaran ng National Lien subject-index. Magkakahiwalay na field ang mga map coordinate, pressure level (1–50), permanenteng catalogue ID, at alphabetic rank; wala sa mga ito ang dapat palihim na ipalit sa iba. Ang pressure ay antas ng algorithmic na iteration, hindi pisikal na dami.


## XIV. Kamakailan at Pinalawak na Indeks ng Package

Itinatampok ng idinagdag na indeks na ito ang mga kamakailang pinalawak na package, domain library, at
koleksiyon ng source. Pinagpapangkat ang mga entry ayon sa alpabeto ng pangalan ng package o paksa.
Tinutukoy ng mga paglalarawan ang lokasyon sa repository; dapat suriin sa sariling
dokumentasyon at test ng bawat package ang kalagayan ng implementation at validation.

### A–C

- **Advantage** — `lib/advantage/` at `advantage/`: pag-advantage ng natatanging
  dividend kaugnay ng isang modelo bilang patong-patong na Sequitur onion, na may limang
  subfolder ng paksa — `math/` (joint profit maximization, Nash bargaining,
  bilateral newsvendor), `physics/` (trust-field metric/geodesic/contact/SDE,
  isang minarkahang modeling analogy), `integration/` (mga compartment ng
  vertical-integration at isang sophistication score), `grading/` (isang A–D na sophistication
  circuit grader para sa mga module at set), at `output/` (terminal / CommonRails /
  socket emit). Isang native na C/C++ consumer (`advantage/`) ang nag-uugnay sa modelo sa
  mga gawang halimbawa ng kalakalan. Tingnan ang [`lib/advantage/README.md`](lib/advantage/README.md)
  at [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alphabet at UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/`, at
  `utf-4088/`: mga catalogue ng alphabet, native na suporta sa character-set, input-map
  ordering, at dokumentasyon ng integrasyon ng UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` at `lib/autocad/`: mga interactive
  na workflow sa 2D drawing at reusable na component ng CAD document/editing.
- **Ballum** — kontrol sa display sa antas ng pixel: per-pixel set/get, region blit,
  brightness/warmth/gamma ng buong display, pointer/keyboard input, at isang
  **fullscreen framebuffer surface** para sa ganap, pag-aari, at mabilis na per-pixel na kontrol
  (override-redirect window + iisang `XPutImage`/MIT-SHM present sa Linux/X11).
  Native core sa ilalim ng `ballum/` na may SLeeLa facade sa `lib/ballum/` at VM
  bridge sa ilalim ng `impl/`; tingnan ang [`ballum/README.md`](ballum/README.md) at ang
  tumatakbong [`ballum/example/`](ballum/example/).
- **CDMA** — `cdma/` at `lib/cdma/`: teknikal na materyal na may kaugnayan sa CDMA at
  gawaing SLeeLa source-library.
- **Character** — `lib/character/`: mga profile ng character at reusable na estadistika
  ng character, kabilang ang mga halimbawa ng biographical profile.
- **Coorenagraph** — `lib/coorenagraph/`: etikal na pangangatwiran, bokabularyong moral,
  deliberasyon, at mga kaugnay na konsepto; tingnan ang README ng package at mga source file nito.

### D–G

- **Mga governance source model** — Tinutukoy ng [lib/governance](lib/governance/README.md) ang mga declarative na hugis ng SLeeLa source para sa mga panukala, anonymous na manwal na balota, at pinagsama-samang tally. Dokumentado sa [registry/DEMOCRATIC-VOTING-SYNTAX.md](registry/DEMOCRATIC-VOTING-SYNTAX.md) ang iminumungkahing @vote annotation at manwal na proseso; itinuturing ito ng kasalukuyang mga compiler bilang inert na comment metadata.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` hanggang sa mga available na may-bersiyong
  HTTP package directory: mga espesipikasyon ng protocol at implementation na
  partikular sa bersiyon. Kumonsulta sa dokumentasyon ng bawat bersiyon para sa eksaktong saklaw
  at kalagayan ng verification nito.
- **International Criminal Court** — `lib/international-criminal-court/` at
  `INTERNATIONAL.CRIMINAL.COURTS.md`: kaugnay na source at dokumentasyong
  partikular sa domain.

### O–S

- **OFDMA** — `ofdma/`: teknikal na materyal tungkol sa orthogonal frequency-division
  multiple-access at mga resource sa C/C++ development.
- **Philosophy at ang Conversation Mask module** — Taglay ng [`philosophy/`](philosophy/README.md)
  ang mga native na C/C++ na pundasyon at inoorganisa ng [`philosophy/module/`](philosophy/module/README.md)
  ang susunod nitong layer ng module sa tatlong tahasang bahagi:
  [`data/`](philosophy/module/data/README.md) para sa typed at may-petsang obserbasyon,
  pagtatapos ng event, setting ng half-life, provenance, at pahintulot; [`helper/`](philosophy/module/helper/README.md)
  para sa deterministic na validation, relevance decay, magkatugmang paghahambing, at
  presentasyong sinalang ayon sa pahintulot; at [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md)
  para sa suportado at di-klinikal na conversation interface. Ang mga handle na nakaharap sa SLeeLa
  at ang binuong Tree ay nasa [`lib/philosophy/`](lib/philosophy/README.md),
  kasama ng mas malawak na philosophy package. Explicit-input at
  consent-first ang module: hindi ito pasibong nagmamanman ng tao o media, nagdidiagnose, nagpapalagay ng
  pribadong intensiyon, nag-iiskor ng halaga ng tao, o nagrerekomenda ng pagbabago sa gamot. Pinapanatili ng
  mga event link nito ang provenance at nabigasyon, hindi ang independiyenteng pagpapatunay.
- **Synchro** — native na runtime/compiler integration at packet-dispatch at
  mga functionality na may kaugnayan sa pagsukat; tingnan ang kaugnay na `impl/` source at
  implementation ng compiler.
- **SleelaUI / terminal UI** — `lib/user-interface/` at ang
  proyektong `sleela-terminal`: integrasyon ng SLeeLa UI at mga component ng
  interaksiyong nakatuon sa terminal.

### U–W

- **United Nations** — `united-nations/`: mga materyal ng repository na partikular sa domain.
- **Wi-Fi 4, Wi-Fi 5, at Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`,
  `wi-fi-6/`, at kaugnay na mga source directory ng `lib/wifi/`: materyal sa wireless
  networking at mga resource ng configuration na partikular sa henerasyon.

### Pagpapanatili ng Indeks ng Package

Kapag may idinagdag o malaking pinalawak na package, idagdag ang entry nito rito sa
angkop na grupong alpabetiko, i-link sa canonical na directory at README nito,
at ilarawan lamang ang mga kakayahang suportado ng mga naka-check-in na source. Panatilihing additive
ang indeks na ito: huwag tanggalin o isulat muli ang umiiral na mga seksyon ng README bilang
bahagi ng mga update sa indeks ng package.


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

Demokratiko ng US at Patakaran ng US.

---

## XVI. Imaheng Pang-brand ng National Science

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. Eksperimental na Catalogue ng UTF-4088 na May Isang Milyong Entry

Deterministikong binubuo ng [UTF-4088 catalogue generator](utf-4088/tools/generate_catalogue.py) ang isang eksperimental na symbol catalogue na may 1,048,576 entry. Inilalarawan ang dokumentadong compact format sa [utf-4088/FORMAT.md](utf-4088/FORMAT.md); nasa [utf-4088/GENERATION-RESULTS.md](utf-4088/GENERATION-RESULTS.md) ang kontrata sa generation at validation.

Patakbuhin ang generator gamit ang Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Ang nabuong binary ay 16,777,244 byte at dinisenyong manatili sa ibaba ng 20,000,000 byte. Beripikado ng mga unit test at ng isang GitHub Actions workflow ang bilang ng entry, pagiging natatangi ng glyph, laki, at SHA-256; ang matagumpay na takbo ay naglalathala ng binary at summary bilang `utf4088-million-catalogue` na workflow artifact. Sadyang ibinubukod ang nabuong binary sa kasaysayan ng Git. Nananatiling eksperimental ang UTF-4088 at hindi ito tugma sa Unicode.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Mga Kamakailang Dagdag — Pi Constant, Startup ng Modium/Registry, Math ng sleelactl, at `@native`

Itinatala ng seksiyong ito ang mga feature na idinagdag noong gawaing 2026-11, ang pinakabagong grupo muna. Awtoritatibo ang mga naka-link na source file at ang kalagayan ng kanilang validation; hindi inilalarawang naka-merge ang mga pagbabagong nasa ilalim ng review hangga't hindi pa nakakarating sa `master`.

### Ang Pi catalog constant na may 100,000,000 digit

Nagpapadala ang repository ng na-verify na halagang **isandaang milyong digit** ng π sa [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (isang maayos na decimal, `3.14159…`), kasama ang halagang may 57,344 digit na [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Parehong independiyenteng na-verify bilang totoong π — tingnan ang [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md), na dinodokumento ang mga check (prefix sa textbook, ang Feynman point sa fractional na posisyon 762, ang tanyag na ika-isang-milyong digit, pagtutugma sa `lib/math/SLPiReference.sleela`, at pagsusuri ng dalas ng digit sa buong file) at isang one-shot na reproduction script.

### Nilo-load ng Modium™ / Registry™ ang Pi constant sa startup ng subsystem

Sa mga nakalaang entry point ng subsystem — `sleela registry` at `sleela modium` (at ang unang takbo ng installer), **hindi** sa bawat `sleela run` — nilo-load ng mga subsystem na Modium at Registry ang malaking π catalog constant. Sa interactive na terminal, nagpi-print sila ng pansamantalang pulang linyang `Loading Modium™` / `Loading Registry™` na binubura sa mismong lugar bago magsimula ang normal na output; mahigpit na TTY-gated ang epektong ito, kaya nananatiling byte-for-byte na hindi nagbabago ang piped at test output. Ang locator (hindi ang mga byte) ay inilalagay bilang mga default ng `Math` key ng registry na `piReferenceLargePath` at `piReferenceLargeDigits`, sa parehong SLeeLa model ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) at sa native seeder ([`registry/sleela_registry.c`](registry/sleela_registry.c)); inilalantad ng `lib/math/SLPiReference.sleela` ang `largePath()` / `largeDigits()`. Ini-install ang π constant sa lokasyon ng SLeeLa ng OS (`<data>/pi/`) ng **parehong** karaniwang `make` installer at ng gabay na "beautiful" installer. Mga detalye: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — ang beautiful installer at isang native-backed na paksang `math`

- Ini-install na ngayon ng gabay ("beautiful") na installer ang `sleelactl` (ang control plane ng SLeeLa) at ang module registry nito, na tumutugma sa `make` installer. Tingnan ang [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- Nagkaroon ang `sleelactl` ng bagong **`subject`** na nature para sa mga native na C/C++ subject library na kinompile sa runtime, at nagrerehistro ng una: ang **`math`**. Sinusubukan ng `sleelactl run math` ang native na backend na `__NativeMath` (`impl/subjects/math/math_native.cpp`) mula simula hanggang wakas sa pamamagitan ng reference demo ng subject; ipinapaliwanag ng `sleelactl info math` kung paano ito naaabot (`import math;`). Nag-aalok lamang ang subject nature ng `run info` — sadyang walang `check` na verb, dahil hindi nilo-load ng `sleela check` ang mga native na subject na nakarehistro sa SHEET.

### `@native`, Native Drive, at multi-language na pagpapatakbo

Humihiling ang bagong method annotation na **`@native`** ng ahead-of-time na native compilation ng isang hot method. Hinahanap ng bagong command na `sleela aot` ang mga `@native` method, bine-verify na nasa loob ng sinusuportahang *offloadable subset* ang bawat isa, at naglalabas ng C/C++ — **fail-closed** (tumatangging magpatuloy, na may tiyak na dahilan) sa anumang construct na hindi pa nito kayang isalin, kaya hindi kailanman nalilikha ang kalahating naisaling method.

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

Nananatiling kahilingan para sa AOT na pagtrato ang `@native`, at tinatanggihan bago ang emission ang mga hindi suportadong construct. Sinusuportahan ng [Native Drive](impl/native-drive/README.md) ang multi-unit na C/C++ executable at shared-library build sa pamamagitan ng mga host compiler (kabilang ang MSVC), suportadong output ng SLeeLa AOT, inspeksiyon ng symbol laban sa ABI ng native-loader, shell-free na pagpapatakbo ng proseso, paghawak ng stdin/stdout/stderr, multi-stage na piping, at XClass delegation sa production loader. Inilalantad ng `sleelactl` ang mga build/run command nito at nagruruta ng XClass inspection at tahasang rendering/installation ng systemd unit sa pamamagitan ng `inspect native-drive`. Ang VM ay nagmamay-ari ng bounded na native-module handle table na may sapilitang host authorization at mga handle na tinitiyak ang generation. Idinaragdag na ngayon ng Syntax 3.0 ang `nativeOpen`, `nativeCall`, at `nativeClose`: ang append-only na bytecode dispatch ay tumatawid sa hiwalay na may-bersiyong null/bool/int64/double/UTF-8 ABI, habang nananatili sa loob ng interpreter ang mga array, struct, at iba pang composite value na pag-aari ng VM at maaaring gumamit ng bytecode fallback na may parehong pangalan. Maaaring opsyonal na tumanggap ang mga plugin ng may-bersiyong host callback table upang tumawag ng eksaktong mga SLeeLa function sa pamamagitan ng nested na VM execution, na may GC rooting ng suspended stack, tahasang allowlist ng pangalan ng host callback, at tinatanggihang recursive na native API call. Nananatiling pinagkakatiwalaang in-process code ang native code, hindi sandbox. May may-bersiyong schema ang mga source process-policy annotation ngunit hindi pa pina-parse/ipinapatupad. Mga detalye, hangganan ng seguridad, at ang ledger ng pitong workstream: [Native Drive](impl/native-drive/README.md) at [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).


## XIX. Mga Pandaigdigang Edisyon ng README

Nagbibigay ang SLeeLa ng mga entry point ng README na partikular sa bansa para sa 50 pamilihan sa pandaigdigang edisyon nito. May iisang file na pinangalanang `README.<CC>.md` ang bawat bansa; kung may maraming wikang kaugnay, pinagpapangkat ang mga ito sa loob ng file ng bansang iyon.

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

**Patakaran sa salin:** Mga paunang lokalisadong entry point ng bansa ang mga file na ito. Lokalisado ang mga panimula at heading ng wika ng mga ito, habang nananatili sa canonical na English [README.md](README.md) ang kumpletong teknikal na sanggunian. Hindi dapat ilarawan na kumpletong salin ang file ng isang bansa hangga't hindi naisasalin ang bawat seksiyon, nasusuri ang mga technical identifier at link, at nagkaroon ng language review ang edisyon. Kinatawan lamang ang saklaw ng wika sa unang yugtong ito at hindi ito pahayag na lubos nang naisalin ang bawat wikang kinikilala ng konstitusyon, panrehiyon, katutubo, o sign language.

## Appendix — Napanatiling Panrehiyong Gabay

Pinapanatili ng appendix na ito ang praktikal na gabay mula sa naunang maikling gabay sa Filipino na wala bilang sariling seksiyon sa canonical README. Mananaig ang canonical na English [README.md](README.md) sa anumang hindi pagkakatugma.

### Mga target, profile, at hangganan ng pagpapatupad

Ibinubukod ng SLeeLa ang source code sa mga target ng build at execution. Kabilang sa dokumentadong target family ang `SLVM`, `SLJVM`, at `SLIR`; ang `NATIVE` ay limitado at may mga kundisyon, kaya huwag ipagpalagay na available ito sa lahat ng platform. Magkaiba ang saklaw ng `BASIC_COMPLETE` at `ADVANCED_TOTAL`. Tiyakin ang aktuwal na target, profile, at platform support sa bawat build.

Tinutukoy ng reference ang file, module, o resource; hindi ito kusang nagbibigay ng pahintulot na patakbuhin iyon. Kapag gumagamit ng files, external libraries, native bindings, o system resources ang programa, dapat malinaw ang authorization boundary at responsibilidad sa execution. Hindi sapat ang pahayag sa dokumentasyon lamang upang patunayan ang runtime security guarantee; suriin ang implementation at kaugnay na tests.

### Resource lifecycle at debugging

Idokumento ang ownership, lifetime, at synchronization responsibilities ng resources na pinaghahatian ng functions o threads. Ihiwalay ang local variables, shared object state, VM handles, at serializable data. Hindi matatag na file format o network message ang VM-local handle. Gamitin ang serialization interface ng module at subukan ang invalid, missing, at boundary values.

Sa debugging, itala ang commit, OS, architecture, at toolchain. Unahin ang pinakamaliit na kaugnay na target, panatilihin ang buong error output, ihiwalay ang compiler errors sa runtime errors, at gumawa ng minimal reproducible example. Hindi pinatutunayan ng matagumpay na build lamang na tama ang runtime behavior.

Dapat ilista ng validation report ang exact commit, OS/architecture, compiler versions, kumpletong build/test commands, bilang ng pass/fail/skip, at mga kilalang limitasyon. Hindi dapat bilangin na passed ang tests na nilaktawan o hindi pinatakbo.

**Katayuan ng salin:** Naisalin sa Filipino ang mga seksiyon I–XIX. Pinananatili ang mga technical identifier, command, code sample, filename, link, at address. Inirerekomenda ang huling language review ng mga katutubong tagapagsalita.
