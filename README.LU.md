<img align="right" src="images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — Lëtzebuerg (LU)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Eng nativ Systemsprooch, e Compiler, e Runtime an en Ökosystem vu virtuelle Maschinnen.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Internationalen Landeditioun.** Dës Editioun ass op Lëtzebuergesch geschriwwen. Technesch Identifikateuren, Kommandoen, Codebeispiller, Dateinimm, Weeër a Linken sinn am Verglach mat der kanonescher englescher [README.md](README.md) onverännert. Franséisch an Däitsch si vun dëser Editioun net ofgedeckt; fir dës Sproochen kuckt [README.FR](README.FR.md) a [README.DE](README.DE.md).

**The Now. The Ingredient.** Eng Sprooch ass méi wéi Syntax: Si ass de Compiler, de Runtime, d'Bibliothéiken, d'Instrumenter an d'Plattformkontrakter, déi Software notzbar maachen.

**Wat SLeeLa beschäftegt.** SLeeLa beschäftegt sech mat **Math**, **Object**,
**Science** a **Reach** — dat sinn déi véier Gesiichter vu senger Aarbecht: dat
Berechenbart (Math), dat festgeluechte Ding, dat am Katalog vun deem steet, wat ass
(Object), dat disziplinéiert Studium vum Wouren (Science) an d'Ausstreckung vun der
Hand vun engem System zu anere Systemer a Mënschen (Reach). **Nationalen Interesse
bleiwen** — si ginn duerch keng vun de véier op d'Säit gesat; wou en Thema en
Interessi vun enger Natioun beréiert, gëtt dee Status gehal (trei festgehalen,
z. B. am Metadata `metaOrigin` / `metaNationalSchedule` vun all Klass) a
ni opgeléist.

## I. Ufank
- [Déi nativ Implementatioun bauen](impl/README.md)
- [Virtuell Maschinnen & Modium — praktescht Benotzerhandbuch](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [CPU-Stack am SLeeLa-Quellcode](lib/cpu/CPU.md) — eng komplett Maschinn vun de Gatteren → ALU → Kären → Betribssystem → Programmer, e Katalog vu 130+ CPU-Profiler an astellbar [Spannungen, variabel Taktfrequenzen an Expertentimings](lib/cpu/TIMING.md), déi Dir [no Spezifikatioun lafe loosse kënnt an iwwertakten](lib/cpu/components/README.md)
- [Dokumentatiounsindex](docs/README.md)
- [CSPML-Datenquell a Fuerschungsreferenz Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — extern Hierkonft vun Daten an Integratiounshinweiser zu amerikanesche Plazen, Sozioökonomie, Aarbechtskräften, Bildung an ëffentleche Schoulen.
- [Generéiert API-Dokumentatioun](docs/api/README.md) — aktuell HTML-Referenz, versionéiert API-Editioun an Instruktioune fir d'Nei-Generéieren
- [API-Katalog](api/README.md)
- [Standarde fir computational Vergläicher](comparison/README.md) — Typ-, Erënnerungs-, ABI- a Konformitéitsstandarde fir SLeeLa VM, Java/JVM an natives C/C++.
- [Recursion stanchion — C- a C++-Prototypen](recursion/stanchion/README.md) — op Primitiver spezialiséiert Beispiller fir Boolean, Byte, Character, Integer, Long, Float an Double, niewent generesche, ugewandte, wëssenschaftleche a Léier-Rekursiounsmuster.
- [Plattformënnerstëtzung a Validéierungsmatrix](docs/PLATFORM-SUPPORT.md)
- [Markeguide](docs/BRAND-GUIDE.md)
- [Merchandise-Konzepter an -Assets](merchandise/README.md)
- [NitroWebExpress-Logo](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Testpolitik a Release-Krittären](markdown/TESTING.md)

## II. Neiegkeeten

Rezent Ergänzunge fir d'Sprooch, d'Treiber an d'Standardbibliothéik:

- **D'`advantage`-Standardbibliothéik-Pak — e Dividend „advantagéieren“, mat
  Mathematik, Physik, Integratioun, Notegebung a routbarem Output.**
  [`lib/advantage/`](lib/advantage/README.md) kapselt *d'Advantagéiere vun engem
  eenzegaartegen Dividend am Bezuch op e Modell* als geschichtenen **Sequitur-Zwiwwel**
  (Dividend → Iwwerpréiwung → Bedenken → Mathematik → Ergänzungen → Subtraktioun
  → Ëmgruppéierung → Iteratioun), no bannen/no baussen navigéierbar mat voller
  dynamescher Aktualiséierung a Restitutioun, zougänglech iwwer e
  einfachen **Benotzermodell** an en **Entwécklermodell** mat voller Kontroll. Hie
  enthält fënnef Fachberäicher: **`math/`** bilateral Handelswirtschaft (gemeinsam
  Profitmaximéierung, Nash-Verhandlung, dat bilateralt Newsvendor-Critical-Ratio),
  **`physics/`** eng strukturell Wirtschaftsphysik vun de „Vertrauensfelder“
  (Vertrauen als metreschen Tensor, déi transaktional Geodät, eng
  Ukënnegungs-Kontaktstruktur, eng CIR+Jump-Vertrauenszerfalls-SDE — eng
  Modellanalogie, als sou gekennzeechent), **`integration/`** e rekursiv
  zesummesetzbart Modell vun der vertikaler Integratioun fir Kapitalismus/Einzelhandel
  (Kompartimenter, Ingenieursbuchten, Probekompartimenter, Newton-Median-Schaalen
  an e Systemraffinesse-Score), **`grading/`** e Raffinesse-Circuitgrader, deen
  all Modul an all Set vu Gewënner-/Verléierer-/Grader-/Absolutmoduler vun A–D
  bewäert, an **`output/`** eng routbar Emit-Nahtstell, déi de Moduloutput un
  d'Terminal, un eng **CommonRails**-formatéiert Uewerfläch oder un e
  Socket/Connector schéckt (gerahmt fir en nativen `slnet_write`). All Klass
  dréit de Metadata-Class-Rekord; fënnef lafbar Demoe validéieren d'Fächer, an déi
  begleedend
  [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md),
  [`MATH.md`](lib/advantage/MATH.md) an
  [`PHYSICS.md`](lib/advantage/PHYSICS.md) dokumentéieren se. E nativen C/C++-
  Konsument ënner [`advantage/`](advantage/README.md) (de C-Kär
  `advantage_value`/`advantage_mean`, en XML-Wëssensspäicher, eng sécher
  HTTPS/SFTP/SSH-Acquisitioun + Keystore + DH/ECDH-Transportbibliothéik) verlénkt
  d'Modell mat ausgeschafften
  [Handelsbeispiller](advantage/examples/README.md) — positiv Handele vu
  virausgesate Internetquellen, eng Publikumsrelevanz-Sequenz (Traders,
  Consumers, Helpers, Healers) an aacht Modeller, gerangéiert vun A–D (D = am
  meeschte Cement, A = am meeschte Brand).
- **Loop-Timing-Annotatiounen — `@loopDelay` / `@loopBudget` (+ en `sleep(ms)`-
  Built-in).** Déi éischt **statementniveau-, verhalensbezunn** Annotatiounen an
  SLeeLa: op enger eegener Zeil virun enger `for`-/`while`-/`do`-Schleif geschriwwen,
  setzen si eng richteg Paus an d'Ausféierung vun der Schleif an. `@loopDelay(ms = 250)` /
  `@loopDelay(seconds = 2)` schléift pro Duerchlaf eng fix Zäit (en nacktent
  `@loopDelay(250)` hëlt Millisekonne als Standard); `@loopBudget(seconds = 10,
  iterations = 1000)` verdeelt eng bekannten Unzuel vun Duerchleef gläichméisseg
  iwwer e gesamt Wall-Clock-Budget. Béid ginn op den neien append-only Opcode
  **`OP_SLEEP`** (ISA 157) erofgesat, deen och e allgemengt **`sleep(<ms>)`**-Built-in
  ënnerstëtzt, dat iwwerall opruffbar ass; d'Paus ass portabel (`nanosleep` / `Sleep`)
  implementéiert. Spezifizéiert an
  [`SLEELA.syntax` §32](SLEELA.syntax); API-Referenz an
  [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md);
  lafbar
  [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela).
  E Sessiouns-Ergänzungsindex (dorënner dësen, de nativen `OP_BIGMUL`-
  Big-Integer-Multiplikator an déi eegestänneg Tools **[local-generator](local-generator/README.md)**
  a **[pi-generator](pi-generator/README.md)**) fënnt Dir ënner
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md).
  Déi dräi generéiert API-Uewerflächen
  ([`sleeladoc-api/`](sleeladoc-api/README.md),
  [`docs/api/`](docs/api/README.md) an den onverännerleche
  [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html)-
  Snapshot) goufen mat `make api-docs` nei gebaut an droen elo déi nei
  `SLISASleep`- / `SLISABigMul`-Instruktiounsentréeën op hirer `vm`-Paketsäit.
- **SleelaDoc vun der Kommandozeil — `sleela docs`.** SleelaDoc ass elo direkt
  vum Terminal aus erreechbar. `sleela docs` lëscht all dokumentéierbart Thema;
  `sleela docs crypto` weist e Paket; `sleela docs stringbuilder` léist eng
  Klass op (ouni Rücksicht op Grouss-/Klengschreiwung an iwwer de
  `SL`-Standardbibliothéik-Präfix → `SLStringBuilder`) an dréckt eng räich Säit
  aus — **Definitioun, Designzweck, Bezéiungen, Struktur, Benotzer-Clays an eng
  Meeschterleeder mat fënnef Sprossen** — mat gerangéierten „Hutt Dir gemengt“-
  Resultater bei engem knappe Feelschlag. Auteure beräicheren all Säit einfach
  duerch SleelaDoc-`@`-Tags (`@intent`, `@thesis`, `@axiom`, `@param`,
  `@see`, `@mastery`). `sleela docs --generate [dir]` schreift e Klartext-Korpus
  (eng Säit pro Klass + en `TOPICS.index`), deen d'CLI dann offline liest;
  **`make sleeladoc`** bündelt dëse Korpus **an déi passend `.sleela`-Quellen**
  an `sleeladoc/build/sleeladoc.jar`. Fir Facade-eleng-Fächer (z. B.
  `psychiatry`, `politico`) duerchsicht `sleela docs --facade <topic>` zur
  Laufzäit d'Dateisystem a lëscht déi richteg **Dossieren (dann Dateien, als natierlech
  Beweegung)**, begrenzt duerch vum Benotzer iwwerschreiwbar Limiten vu **6 MB / 6000 Dateien / 6
  Minutten**. D'`lib/sleeladoc`-Eenheete ginn op der aktueller Editioun,
  **`#sleela 3.0`**, ënnerhalen. Kuckt [`man/man1/sleela.1`](man/man1/sleela.1) an
  [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md).
- **SleelaDoc — eng Javadoc-artig Dokumentatiounslouso, mat generéierter HTML-Säit.**
  **SleelaDoc** ass SLeeLas Doc-Kommentar-Konventioun am Quelltext
  (Wénkelklammer-Kommentaren, mat `@doc` / `@page` markéiert; kuckt
  [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)), an
  [`lib/sleeladoc`](lib/sleeladoc/README.md) ass den **Extraktor**, deen eng
  `.sleela`-Eenheet als Markdown- oder HTML-API-Säit duerstellt — esou wéi `javadoc`
  Java duerstellt, an dokumentéiert d'*Absicht*. Eng komplett, duerchsicht
  baar **HTML-Dokumentatiounssäit fir de ganze `/lib`** gëtt vum
  `tools/generate-sleeladoc-api.py` an [`/sleeladoc-api`](sleeladoc-api/README.md)
  generéiert (`sleeladoc-api/index.html` opmaachen; e kloere
  Stil a Blo / Hellblo / Hellgro / Wäiss mat enger Hierarchie Paket → Klass → Member
  an engem Glossar, geschriwwen fir de *Student-2*-Lieser). De Compiler
  gëtt och vum Auteur markéiert Dokuausgabe als Nebenprodukt vun der Kompilatioun aus (`--wiki`
  MediaWiki-Säit + en originéierten Quell-Zwilling), dokumentéiert an
  [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md). Lafbar:
  [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela).
- **XML Document Object Model (`lib/xml`).** E SLeeLa-säitegen XML-DOM — en Dokument,
  dat e Bam aus Element-/Text-/Kommentar-/CDATA-/PI-Noden mat geuerdente
  Kanner a benannten Attributer besëtzt, e Serialiséierer (kompakt/schéin, Escaping no XML 1.0 §2.4),
  e Char-Cell-Parser an de `SLXmlAspect`-Qualitéitsregister. Node-Gläichheet ass eng
  Dräi-Sprossen-Kette vum Vertrauen (Identitéit → selwecht Sujet → Struktur), reflexiv,
  symmetresch an deterministesch. Kuckt [`lib/xml/README.md`](lib/xml/README.md) an
  dat lafbart [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela).
- **Breet signéiert Ganzzuelprimitiven (`long`, `int64`, `int128`).** `int`, `long`
  an `int64` si ganz 64-Bit-signéiert Ganzzuelen, déi identesch typkontrolléiert
  ginn a lafen; `int128` gëtt als Ganzzueltyp akzeptéiert (fir de Moment op 64 Bit
  baséiert, richtegt 128-Bit ass en dokumentéierte Follow-up). Déi Basis-2-Serie vu signéierten Zuelen
  `int32`..`int16777216` (Syntax 2.7) an déi passend Serie ouni Virzeechen
  `uint32`..`uint16777216` (Syntax 2.8) erweideren dat. Kuckt
  [`SLEELA.syntax`](SLEELA.syntax) §21, §24, §25.
- **`@mix`-Konsequenzfelder (Syntax 2.9).** Eng Klass däerf méi Instanzfelder mam
  **selwechten Numm** deklaréieren, wann all Widderhuelung `@mix` dréit; d'Sprooch versteet
  d'Grupp als konsequent (en Array / eng Lëscht mat Index), standardméisseg vun uewen no
  ënnen `1,2,3,…` oder manuell iwwer `@mix(1,2,3,...)`, wou d'Unzuel vun den Indizes mat der
  Unzuel vun de Felder iwwereneestëmme muss. Kuckt [`SLEELA.syntax`](SLEELA.syntax) §28.
- **Dokumentmarkéierer — de Natierleche Prozess, Preonems genannt
  (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).**
  Dës nackte Dokumentmarkéierer-Wierder si kollektiv d'**Preonems** genannt
  (ee Markéierer ass e *Preonem*; d'Set, d'*Preonems* — kuckt
  [`SLEELA.syntax`](SLEELA.syntax) §26.1). Éischtklassesch, nëmmen deklarativ
  Dokumentmarkéierer, déi nennen, wat en Dokument an der Reihenfolg vum Maache *ass*:
  `author`/`authorial` (Syntax 2.4; vun engem professionelle Conseil behandelt — dat eent
  Member mat Verhalenseffekt), `material;` (festgeluechte Quell) a `memory;`
  (Syntax 2.5), `determine;` (Syntax 2.6; en Dokument, dat fir e Dokter, Dokter oder Geescht
  op weider Veraarbechtung bis zur Vervollstännegung duerch legal Hand ophält), an
  déi zwee neisten, **`object;`** (e festgeluechte Ding am Katalog vun deem, wat ass —
  **reifizéiert, konkret Stellung kritt, bei Syntax 2.8**) a
  **`field;`** (de Buedem vun der Relatioun, an deem Objeten stinn a sech beweegen) —
  béid ab Syntax 2.6 gesperrt. All ass nackt, kontextuell a vum Frontend
  versiounsofhängeg gesperrt, ouni Effekt op Parsing, Typéierung oder Codegeneratioun;
  ausserhalb vun der Markéierer-Positioun bleift all een en normalen Identifier (sou
  stéiert `field` ni e Klassemember oder e `.field`-Zougrëff). Kuckt [`SLEELA.syntax`](SLEELA.syntax)
  §18–§20, §23 a §26.
- **Nordshrift bei Syntax 3.0 mat voller Bibliothéiksreechwäit.** Nordshrift léist
  iwwer deeselwechten rekursive `/lib`-Index wéi de Compiler op, erreecht all
  Paket an all `.sleela`-Eenheet; `nordshrift check` mellt d'Zuelen iwwer de ganze Bam.
- **Recursion stanchion.** De Verzeechnes [`recursion/stanchion/`](recursion/stanchion/README.md) enthält elo speziell C- a C++-Referenzprototypen fir déi siwe dokumentéiert Primitivkontrakter, plus generesch, ugewandte, wëssenschaftlech a Léierbeispiller. Dat sinn Referenzquellen; kompiléiert a testet se, éier Dir se als verifizéiert behandelt.

- **Wénkelklammer-Kommentaren (`<` / `<<`).** E neie Go-forward-Kommentarstil —
  `< line >` an `<< block >>` — vum SLeeLa-Frontend gelexed an dokumentéiert an
  [`SLEELA.syntax`](SLEELA.syntax) §1.1.
- **Radix-Ganzzuelliteraler.** `int`-Literaler akzeptéieren elo hexadezimal (`0x`),
  binär (`0b`) an oktal (`0o`) Forme mat `_` als Zifferentrenner, souwuel an der
  SLeeLa-Quellsprooch ([`SLEELA.syntax`](SLEELA.syntax) §1.4) wéi och am
  [`.xlt`-CPU-Modellformat](lib/xlt/README.md).
- **De Metadata-Class-Standard duerch d'Bibliothéik.** Dat beschreiwend Metadate-
  Metamuster (Hierkonft, Legalitéit, jurisdiktionelle Geltungsberäich) gëtt elo vun
  all klassentragender `.sleela`-Datei ënner `/lib` gedroen; kuckt
  [`SLEELA.syntax`](SLEELA.syntax) §15 an
  [d'Tutorial-Lektioun](tutorial/13-metadata-class.md).
- **Eng méi räich Text-/String-Famill.** `lib/text` kritt richteg Hëllefsfunktiounen an
  der Sprooch, eng Char-Cell-Vue mat echtem `charAt`/`substring`/`indexOf`/`reverse`/
  Spaltenausriichtung, an e richtegen positionellen Templater — kuckt dat
  [Text-&-String-Tutorial](tutorial/14-text-and-strings.md) an dat lafbart
  [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela).
- **`String` als éischtklasseg Typ a Nordshrift-Sheeten.** E `.sst`-Sheet kann e
  typéierte `strings:`-Block deklaréieren, deen zum Komponentemanifest vum Build bäidréit;
  kuckt d'[Nordshrift-Sheet-Referenz](wiki/Nordshrift-Sheet-Reference.md).
- **CPU-Modeller lafen iwwer Opcodes an iwwer Funktiounsopruff-Äquivalenz.** All
  `SL<ARCH>CPU` ierft d'Basis, e nativen Instruktiounsstroum duerch seng
  Fetch-Decode-Execute-Schleif lafen ze loossen *an* de Code vun engem ugeholmenen
  anere Prozess duerch äquivalent Opruffer um gemeinsame VM-Substrat auszeféieren — kuckt
  [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5.

## III. Build an Test

**Viraussetzungen:** eng C/C++-Toolchain (GCC oder Clang) a GNU Make; Python 3 fir
d'Manifest-/Flow-Tooling; ënner Linux d'X11/Xrandr-Entwécklungspakete fir d'Displayprodukter
(`make deps` vun engem Modul mellt/installéiert, wat et brauch). De
`impl/`-Build ass fail-closed op engem vertrauenswierdege SHA-256-Manifest, op dee de Root-
`Makefile` automatesch weist.

Vun engem sauberen Checkout aus de schnellsten Ufank:

```sh
make                                  # build everything from the repo root
```

Oder nëmmen de nativen Kär bauen an testen:

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

De `make` am Repo-Root baut all Produkt (Kär, VM, Codecs, Churn,
Antivirus, …) a schléisst domat of, d'**Registry** ze bauen an hir
Basistabell ze materialiséieren — sou liwwert e gebauten Bam eng nokontrolléierbar Registry direkt mat.
Déi wichtegst Ziler:

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

Weider nëtzlech Ziler:

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

D'`registry`-Zil féiert [`build-registry.sh`](build-registry.sh) aus: et baut
de Registry-Kär, säit d'Basistabell an de portablen Textspäicher
`registry/build/sleela.sleelareg` an exportéiert de MySQL-Tabellescript
`registry/build/registry.sql` (iwwer `sleela registry export`). Kuckt se Iech un mat
`sleela registry` oder `registry/build/sleela-registry review`. Kuckt de
[Registry-Guide](registry/README.md) an d'[Registry-3.0-Architektur](registry/REGISTRY-3.0-ARCHITECTURE.md).

Liest d'Build- an Testguiden fir Viraussetzungen a plattformspezifesch Schrëtt. E gelongene Build vun enger Komponent beweist net, datt all Bibliothéik oder Plattform releasefäerdeg ass.

## IV. Installatioun

Nom Bauen vun den nativen Produkter installéiert d'Kommandoen an d'Runtime-Ënnerstëtzungsdateien:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

Den herkömmlechen Installer benotzt standardméisseg `/usr/local/bin`, `/usr/local/lib/sleela`
an `/usr/local/share/sleela`. Hie behält eng bestoend Startkonfiguratioun
an weigert sech, bestoend, onverfollegt Kommandoen ze iwwerschreiwen. D'Deinstallatioun
läscht nëmmen d'Dateien, déi den Installer opgeholl huet, a looss d'Konfiguratioun an
onverfollegt Runtime-Daten onberéiert.

Fir eng fokusséiert, gefouert Deployment vum Haaptpaket eleng — d'Binäre `sleela`,
`sleelvac` an `nordshrift`, d'Standardbibliothéik, d'Protokoller an d'
Ënnerstëtzungsmoduler — benotzt de proppen Installer:

```sh
make clean-install       # guided install of the main package only
```

Hien freet no enger Haaptentscheedung (d'Haaptpaket installéieren), dann no engem Geltungsberäich,
an bei Erfolleg no engem optionale Choix vu kuratéierten Erweiderungsmoduler. Fir den
interaktiven Deployment-Workflow an all Installatiounsoptiounen kuckt
[Séier a sécher Installatioun](install/README.md).

## V. Sprooch a Runtime
Déi autoritativ Grammatik ass [`SLEELA.syntax`](SLEELA.syntax). De aktuelle Repository dokumentéiert d'Syntax **3.0**; Compiler-/Tool-Versioun a Quellsyntax-Versioun sinn ënnerscheet. D'3.0-Uewerfläch füügt déi **Dokumentmarkéierer `pure` an `gasoline`** (§30) derbäi — e Paar nackt, kontextuell, nëmmen deklarativ Preonems, déi bal dat selwecht bedeiten, jidd markéiert de Status vum Dokument duerch seng eege Éierlechkeet. D'Syntax 3.0 konsolidéiert d'Sproochuewerfläch 1.0–2.9 an déi aktuell normativ Editioun a bewahrt all featurespezifesch Mindestgrenzen. Déi viregt 2.9-Uewerfläch füügt **`@mix`-Konsequenzfelder** (§28) derbäi — eng Klass däerf méi gläichnamig Instanzfelder deklaréieren, wann all dat `@mix` dréit, verstan als eng indexéiert Grupp (vun uewen no ënnen `1,2,3,…` oder manuell `@mix(1,2,3,...)`, wou d'Unzuel vun den Indizes mat der Grupp iwwereneestëmme muss). Si féiert d'Basis-2-Famille vu breede signéierten/net-signéierten Ganzzuelen `int32`..`int16777216` (§24) an `uint32`..`uint16777216` (§25) iwwer déi fréier `long`/`int64`/`int128` (§21) weider; **Schlësselwuert-Ënnerstëtzungsdokumenter** (§22); an d'**Preonems** — déi nëmmen deklarativ Dokumentmarkéierer `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) an `artifact` (§35, Stellung vu Gesetzgebungsgrad, no Zäit a Raum gestuft), kollektiv d'Preonems genannt (§26.1). D'Opléisung vun Konstruktor-Iwwerladungen an d'`this(...)`-/Basiskonstruktor-Delegatioun sinn ausdrécklech ausserhalb vum verifizéierte Syntax-1.8-Kontrakt. Kuckt d'Grammatik an d'Implementatiounsnotizen, ier Dir Iech op eng Funktioun verlooss. SleelaDocs néng Quelleenheeten deklaréieren elo `#sleela 3.0`; d'gemeinsamt Compiler-Gate akzeptéiert d'Quellsyntax 1.0–3.0. Dee neie API-Build gëtt mat `make api-docs` produzéiert.

De Repository enthält eng nativ C/C++-Implementatioun, SLVM-Generatiounsreferenzen, Standardbibliothéik-Moduler, Tester, Beispiller a Plattformintegratiounen. Implementatiouns- a Validéierungsstatus ännert sech vu Modul zu Modul; kuckt seng Spezifikatioun an déi aktuell CI-Resultater, amplaz eng eenheetlech Vollstännegkeet unzehuelen.

## VI. Ingenieursprinzipien
- **Beweiser virun Behaaptungen:** ënnerscheet implementéiert, getest, experimentell a geplangt Verhalen.
- **Ee Kontrakt pro Grenz:** dokumentéiert Syntax, APIen, Dateiformater, Plattformannahmen a Feelerverhalen.
- **Portabel vum Design:** isoléiert plattformspezifesch Backends a testet op nativen Runners.
- **Sécher Feeler:** testet fehlerhaften Input, Ressourcelimiten, Prozessfeeler a Botzweeër.
- **Reproduzéierbar Releases:** baut sauber nei a publizéiert Artefakt-Hashen a Release-Notizen.

## VII. Antivirus-Build an Integritéit

D'Antivirus-Paket Sleelavirin enthält e nativen SHA-256-Scanner, e
optionalen ClamAV-Adapter, Integritéitsmanifester fir Quell/Index a
Kontrollsummen fir Plattformpakéiten.
Baut an testet d'Paket vum Repository-Root aus:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Den Integritéitsmanifest gëtt ënner `antivirus/build/` generéiert; hie deckt verfollegt
Quell- an Dokumentatiounsinputen plus geschützt Signaturindex-Dateien of,
an exkludéiert generéiert Output an de verännerleche ClamAV-Cache. ClamAV-Datebank-
Snapshots hunn hir eege Manifester. Release-ZIPen enthalen `INTEGRITY-MANIFEST.json`,
`SHA256SUMS` an e `.sha256`-Sidecar. Kontrolléiert erofgelueden Archiv-Hashen géint
e vertrauenswierdege Wäert, deen onofhängeg kritt gouf: eng Kontrollsomm, déi niewent
engem Archiv gespäichert ass, ass keng Ënnerschrëft vum Publisher. Kuckt de [Sleelavirin-Guide](antivirus/README.md),
d'[Sécherheetscheckliste](antivirus/SECURITY.md) an de
[Antivirus-Build-Workflow](.github/workflows/antivirus-binaries.yml).

E Release gëllt net als verifizéiert, bis déi relevant CI-Jobs um
exakte Release-Commit bestane sinn an déi entstanen Artefakter kontrolléiert goufen.

## VIII. SLeeLa-Editorthemen — Vim an Emacs

SLeeLa enthält e koordinéiert Set vu professionelle donkele Faarfschemae fir **Vim** an **Emacs**, plus en nativt C/C++-Palettenutility. Dat ursprénglecht Schema **American Professional** bleift de Standard. Zéng Begleetpaletten si fir ënnerschiddlech kulturell an institutionell Design-Inspiratioune verfügbar; et si interpretativ Designpaletten, keng offiziell Duerstellunge vun enger Natioun, Kultur, engem Geriicht oder enger Institutioun.

### Déi eelef Schemae a hir Faarwen

| Schema | Hannergrond | Virdergrond | Schlësselwierder | Typen | Zeechenketten | Kommentaren |
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

#### Nationale Fändelsfaarwen (Quellpalett)

D'griichesch an d'tierkescht Schema sinn aus den **offiziellen nationale Fändelsfaarwe** vun deenen zwee Länner ofgeleet, hei mat hiren exakte publizéierte Hex-Wäerter gewisen. D'Hannergrënn/Akzenter vun de Schemae uewen sinn verdunkelt, editorfrëndlech Interpretatioune vun dëse Quellfaarwen — d'Tabell hei drënner ass d'Referenz „Faarwen, wéi se national ausgesat ginn“.

| Natioun | Faarf | Hex | Basis a Bedeitung |
|---|---|---|---|
| **Griicheland** | Blo | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | Blo vum Nationalfändel (Gesetz 851/1978). Den Himmel an d'Mierer; heraldesch als *azur* blasonéiert. |
| **Griicheland** | Wäiss | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Wäiss vum Nationalfändel. Rengheet vum Onofhängegkeetskampf; d'Kräiz steet fir dat ostkierchlecht orthodoxt Chrëschtentum. |
| **Türkei** | Rout (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | Rout vum Nationalfändel (tierkescht Fändelsgesetz). D'Feld; der Traditioun no d'Blutt vun de Märtyrer an d'osmanesch Souveränitéit. |
| **Türkei** | Wäiss | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Wäiss vum Nationalfändel. De Hallefmound a säi Stär — Fridden, Rengheet an d'Symbol vum tierkesch-islamesche Patrimoine. |

D'Schemae verankeren de Virdergrond vun all Editor um nationale **Wäiss** a seng Schlësselwierder, de Cursor, d'Selektioun an d'Statuszeil um exakte nationale **Blo** (`#0D5EAF`) oder **Rout** (`#E30A17`), sou datt déi Faarwen, déi zielen, buchstäblech d'Nationalfaarwen sinn. Dat sinn interpretativ Designpaletten; d'Referenz fir d'Fändelsfaarwen gëtt fir d'Provenienz bäigefüügt a ass keng offiziell Duerstellung vun engem vun deenen zwee Staaten. D'Faarfbedeitunge follegen der Beschreiwung vun de Staatssymboler vum Ministère des Affaires étrangères vun der Hellenescher Republik an der Traditioun vum tierkesche Fändelsgesetz.

### Faarfmusterstabell

D'Palettenzellen hei drënner benotzen **SVG-Swatchen**, sou datt GitHub déi tatsächlech Faarwen duerstellt amplaz vu leeren HTML-Faarfzellen. All Streif weist déi sechs Faarwen an dëser Reihenfolg: Hannergrond, Virdergrond, Schlësselwierder, Typen, Zeechenketten a Kommentaren. Déi exakt Hex-Coden stinn niewent all Swatch, an der selwechter Schema-Reihenfolg wéi an der Referenztabell uewen.

| Schema | Duergestallt Palettefaarwen | Hex-Coden (Hannergrond · Virdergrond · Schlësselwierder · Typen · Zeechenketten · Kommentaren) |
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

Zousätzlech Akzentfaarwe ginn fir Funktiounen, Konstanten, Selektiounen, Diagnostik a Preprozessor-Direktive benotzt. D'russesch a slawesch Palette benotzt bewosst Schiefer, Blo, Salbei, Rubin an Antikgold mat mëttlerer Intensitéit fir en rouegt, professionellt Ausgesinn.

### D'Vim-Schemae installéieren

Vum Repository-Root aus:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Füügt Folgendes an `~/.vimrc` derbäi, fir SLeeLa-Syntaxhervirhiewung z'aktivéieren an e Schema ze wielen:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Wielt genee ee vun dëse Colorscheme-Nimm:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Benotzt zum Beispill `colorscheme sleela-russian-slavic` fir d'mëttel russesch a slawesch Palette, oder `colorscheme sleela-greek` / `colorscheme sleela-turkish` fir d'hellenesch an d'Al-Bayrak-Palette. Start Vim nei oder lued Är Konfiguratioun mat `:source $MYVIMRC`.

### D'Emacs-Schemae installéieren

Leet e lokalen Theme-Verzeechnes un a kopéiert d'Mode- an Theme-Dateien vum Repository-Root aus:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Füügt dëst an `~/.emacs` oder `~/.emacs.d/init.el` derbäi:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Ersetzt `sleela-american` duerch `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` oder `sleela-sleela`, fir en anert Schema ze wielen. Wann Emacs freet, ob dem Theme vertraut soll ginn, préift déi lokal Theme-Datei, ier Dir akzeptéiert.

### D'nativ C/C++-Palette virukucken

Dat kleng native Palettenutility kann Schemae lëschten, hir Terminalfaarwen ausdrécken an eng eenzel Faarf nokucken:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Fir d'Réckwärtskompatibilitéit wielen `--palette` an `--hex COLOR` ouni `--theme` American Professional. Build- an Testresultater sollen um Zilsystem an am CI kontrolléiert ginn; d'Dokumentatioun vun engem Kommando ass u sech kee Beweis, datt e bestanen huet.

Kuckt [den Editor-Integratiounsguide](linux-editors/README.md) fir déi vollstänneg Palettebeschreiwungen an Utility-Detailer.

## IX. Matmaachen
1. Liest den Dokumentatiounsindex an d'Testpolitik.
2. Maacht fokusséiert Ännerungen mat positiven an negativen Tester.
3. Féiert gezielt Tester aus, dann déi breet Suite.
4. Mellt OS-/Toolchain-Detailer; beschreift iwwersprongene Kontrollen net als bestanen.

SLeeLa gëtt vun **MEARVK LLC** entwéckelt. Historescht Landing-Page-Material ass an [`docs/README-LEGACY.md`](docs/README-LEGACY.md) opgehalen.

## X. AE6E66 — Erwaardung vun enger Ofgab op Round-Public-E-Mailen

Fir d'Initiativ AE6E66 ass déi erkläert Erwaardung vum Projet **$300,000,000.00 USD pro Round-Public-E-Mail** als virgeschloe Steierbetrag, dee mat all qualifizéierender Round-Public-E-Mail verbonne ass.

Dës Erklärung hält nëmmen eng Projeterwaardung fest. Si ass **keng** juristesch Steiererhiewung, keng Regierungsuerdnung, kee Geriichtsuerteel an keng onofhängeg festgestallt Steierschold. All tatsächlech Ofgab géif déi zoustänneg juristesch Autoritéit, Juridictioun, Definitiounen, e Verfahren an eng kompetent staatlech Feststellung viraussetzen.


## XI. Alphabet- a Zeechesaz-Runtime

SLeeLas Alphabetdokumentatioun an nativ Zeechenruntime ginn an alphabet/sleela/ an lib/alphabet/native/ gepflegt. Déi native C++17-Tester decken strikt UTF-8-Skalar-Dekodéierung/-Kodéierung, C<n>-Kapazitéitssignaturen a begrenzt Zeechesaz-Memberschaft of:

    make -C lib/alphabet/native test

Dee experimentellen UTF-4088-Katalog a säin SLeeLa-Lookup-Adapter sinn an utf-4088/SLeeLa-ALPHABET-INTEGRATION.md dokumentéiert. UTF-4088-IDen sinn en eegene Nummraum, keng Unicode-Skalarwäerter. D'Compiler- an SLVM-Bindungsaarbecht gëtt ausdrécklech getrennt vun dësen nativen Fundamenter verfollegt.

## XII. AutoCAD Creator™

Den [AutoCAD Creator™](autocad/creator/README.md) ass eng interaktiv 2D-CAD-
Applikatioun, déi komplett an SLeeLa geschriwwen ass. Wou de Schwësterprogramm AutoCAD Renderer
e Plang mat engem Schlag an eng Zeechnung ëmwandelt, léisst Iech de Creator en 2D-
AutoCAD-Dokument live **uleeën a veränneren** — Maueren, Zëmmeren, Moossen an Notize
derbäisetzen, dann dat, wat Dir gesat hutt, verschiben, ëmbenennen, nei op eng Schicht leeën, läschen
an réckgängeg maachen — an d'Resultat als richtegt AutoCAD-DXF-Zeechnung **späicheren** (R12-
Text, oppent sech an AutoCAD / LibreCAD).

Seng grafesch Interface baséiert op **SleelaUI™**, dem eegene plattformiwwergräifenden UI-
Toolkit vu SLeeLa (ënner [`lib/user-interface`](lib/user-interface)) — net JavaFX —, dat
e richtegt nativt Fënster ënner X11 / Cocoa / Win32 opmécht. Den Design follegt enger Regel,
**„SleelaUI presents; SLeeLa decides“**: d'GUI berechent ni Geometrie a schreift ni
DXF. All Tool schéckt eng Create-/Alter-/Delete-/Undo-/Save-Intentioun un de
autoritativen [`SLCadCreator`](lib/autocad/SLCadCreator.sleela), deen dat veränerbart
Dokument an eng reversibel Kommandohistorik besëtzt, während d'Canvas nëmmen déi
Geometrie virundeet, déi de Creator hält.

De wiederverwendbare Editéiermotor läit an [`lib/autocad`](lib/autocad) —
[`SLCadEntity`](lib/autocad/SLCadEntity.sleela),
[`SLCadDocument`](lib/autocad/SLCadDocument.sleela),
[`SLCadCommand`](lib/autocad/SLCadCommand.sleela) an
[`SLCadCreator`](lib/autocad/SLCadCreator.sleela) — a leeft vun Ufank bis Enn headless.
D'GUI-Schuel an d'Demo sinn ënner [`autocad/creator`](autocad/creator):

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

Op engem Display mécht dat dat live Editéierfënster op; op engem Headless-Host baut et
d'Zeechnung am Aarbechtsspäicher, dréckt e Rapport plus den DXF aus a beendegt sech propper.


## XIII. Reihenfolg vun der UTF-4088-Input-Map

De spezialiséierten UTF-4088-Katalog definéiert seng räimlech Input-Map-Traversatioun an abstrakt Drock-Iteratiounen an [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). D'Politik fir den Sujetindex National Lien steet an [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Map-Koordinaten, Drockniveauen (1–50), permanent Katalog-IDen an alphabetesch Rängen sinn onofhängeg Felder; keent dovun däerf roueg duerch en anert ersat ginn. Drock ass en algorithmescht Iteratiounsniveau, keng physikalesch Gréisst.


## XIV. Rezenten an erweiderten Paketindex

Dësen ugehaangenen Index hieft rezent erweidert Paketer, Domainebibliothéiken a
Quellsammlungen ervir. D'Entréeë sinn alphabetesch no Paketnumm oder Thema gruppéiert.
D'Beschreiwunge bezeechnen de Repository-Standuert; Implementatiouns- a Validéierungsstatus
sollen an der eegener Dokumentatioun an an den Tester vun all Paket nogekuckt ginn.

### A–C

- **Advantage** — `lib/advantage/` an `advantage/`: e eenzegaartegen
  Dividend am Bezuch op e Modell als geschichtene Sequitur-Zwiwwel advantagéieren, mat fënnef
  Fachënnerverzeechnesser — `math/` (gemeinsam Profitmaximéierung, Nash-Verhandlung,
  bilateralen Newsvendor), `physics/` (Vertrauensfeld-Metrik/Geodät/Kontakt/SDE,
  eng gekennzeechent Modellanalogie), `integration/` (Kompartimenter vun der vertikaler Integratioun
  an e Raffinesse-Score), `grading/` (en A–D-Raffinesse-
  Circuitgrader fir Moduler a Sets) an `output/` (Terminal / CommonRails /
  Socket-Emit). E nativen C/C++-Konsument (`advantage/`) verlénkt d'Modell mat
  ausgeschaffte Handelsbeispiller. Kuckt [`lib/advantage/README.md`](lib/advantage/README.md)
  an [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alphabet an UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` an
  `utf-4088/`: Alphabetkataloger, nativ Zeechesaz-Ënnerstëtzung, Input-Map-
  Reihenfolg an UTF-4088-Integratiounsdokumentatioun.
- **AutoCAD Creator** — `autocad/creator/` an `lib/autocad/`: interaktiv
  2D-Zeechen-Workflows a wiederverwendbar CAD-Dokument-/Editéierkomponenten.
- **Ballum** — Pixelniveau-Display-Kontroll: Set/Get pro Pixel, Regioun-Blit,
  Helligkeet/Wäermt/Gamma fir de ganzen Display, Pointer-/Tastatureingab an eng
  **Fullscreen-Framebuffer-Uewerfläch** fir totaler, besëssener, séier Pixel-Kontroll
  (Override-Redirect-Fënster + eenzelt `XPutImage`/MIT-SHM-Present ënner Linux/X11).
  Nativen Kär ënner `ballum/` mat der SLeeLa-Facade an `lib/ballum/` an enger VM-
  Bréck ënner `impl/`; kuckt [`ballum/README.md`](ballum/README.md) an dat
  lafbart [`ballum/example/`](ballum/example/).
- **CDMA** — `cdma/` an `lib/cdma/`: CDMA-bezunnent technescht Material a
  SLeeLa-Quellbibliothéiksaarbecht.
- **Character** — `lib/character/`: Charakterprofiler a wiederverwendbar Charakter-
  Statistiken, dorënner biographesch Profilbeispiller.
- **Coorenagraph** — `lib/coorenagraph/`: ethescht Begrënnen, moralescht Vocabulaire,
  Iwwerleeung a verwandte Konzepter; kuckt säi Paket-README a seng Quelldateien.

### D–G

- **Governance-Quellmodeller** — [lib/governance](lib/governance/README.md) definéiert deklarativ SLeeLa-Quellformen fir Virschléi, anonym manuell Wahlzetelen an aggregéiert Auszielungen. Déi virgeschloen @vote-Annotatioun an de manuelle Prozess sinn an [registry/DEMOCRATIC-VOTING-SYNTAX.md](registry/DEMOCRATIC-VOTING-SYNTAX.md) dokumentéiert; aktuell Compiler behandelen se als inerten Kommentar-Metadaten.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` bis zu de verfügbare versionéierte HTTP-
  Paketverzeechnesser: versiounsspezifesch Protokollspezifikatiounen an
  Implementatiounen. Kuckt d'Dokumentatioun vun all Versioun fir hiren exakte Geltungsberäich
  a Verifikatiounsstatus.
- **International Criminal Court** — `lib/international-criminal-court/` an
  `INTERNATIONAL.CRIMINAL.COURTS.md`: verbonnen domainespezifesch Quell an
  Dokumentatioun.

### O–S

- **OFDMA** — `ofdma/`: technescht Material zum orthogonalen Frequenzmultiplex-
  Mehrfachzougrëff a C/C++-Entwécklungsressourcen.
- **Philosophie an de Conversation-Mask-Modul** — [`philosophy/`](philosophy/README.md)
  enthält déi nativ C/C++-Fundamenter, an [`philosophy/module/`](philosophy/module/README.md)
  organiséiert hir nächst Modulschicht an dräi explizit Beräicher:
  [`data/`](philosophy/module/data/README.md) fir typéiert, datéiert Observatiounen,
  Eventenden, Halbwäertszäit-Astellungen, Provenienz a Zoustëmmung; [`helper/`](philosophy/module/helper/README.md)
  fir deterministesch Validatioun, Relevanzzerfall, kompatibel Vergläicher a
  zoustëmmungsgefiltert Presentatioun; an [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md)
  fir eng ënnerstëtzend, net-klinesch Gespréichsinterface. D'SLeeLa-seiteg
  Handles an de komponéierte Bam sinn an [`lib/philosophy/`](lib/philosophy/README.md),
  niewent dem méi breede Philosophie-Paket. De Modul baséiert op explizitem Input a stellt
  d'Zoustëmmung un d'éischt Stell: hie iwwerwaacht Leit oder Medien net passiv, stellt keng Diagnose, schléisst
  net op private Absichten of, bewäert net de mënschleche Wäert a recommandéiert keng Ännerunge vu Medikamenter. Seng
  Event-Linke bewahre Provenienz a Navigatioun, keng onofhängeg Bestätegung.
- **Synchro** — nativ Runtime-/Compiler-Integratioun a Packet-Dispatch- a
  Miessfunktionalitéit; kuckt déi relevant `impl/`-Quell an d'
  Compiler-Implementatioun.
- **SleelaUI / Terminal-UI** — `lib/user-interface/` an dat
  `sleela-terminal`-Projet: SLeeLa-UI-Integratioun a terminalorientéiert
  Interaktiounskomponenten.

### U–W

- **United Nations** — `united-nations/`: domainespezifesch Repository-Materialien.
- **Wi-Fi 4, Wi-Fi 5 a Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`,
  `wi-fi-6/` a verbonne `lib/wifi/`-Quellverzeechnesser: generatiounsspezifescht
  Material a Konfiguratiounsressourcen fir Drahtlosnetzwierker.

### Ënnerhalt vum Paketindex

Wann e Paket derbäikënnt oder wesentlech erweidert gëtt, setzt säin Entrée hei an déi
passend alphabetesch Grupp, verlinkt säi kanonesche Verzeechnes a säi README
a beschreift nëmmen Fäegkeeten, déi vun den ageluechte Quelle gedeckt sinn. Haalt dësen
Index additiv: läscht oder iwwerschreift d'bestoend README-Sektiounen net am
Kader vun Aktualiséierunge vum Paketindex.


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

US Democratic and US Policy.

---

## XVI. Nationalt Wëssenschafts-Markebildmaterial


<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. Experimentellen UTF-4088-Katalog mat enger Millioun Entréeën

De [UTF-4088-Katalog-Generator](utf-4088/tools/generate_catalogue.py) baut deterministesch e experimentelle Symbolkatalog mat 1.048.576 Entréeën. D'dokumentéiert kompakt Format ass an [utf-4088/FORMAT.md](utf-4088/FORMAT.md) beschriwwen; de Generéierungs- a Validéierungskontrakt steet an [utf-4088/GENERATION-RESULTS.md](utf-4088/GENERATION-RESULTS.md).

Féiert de Generator mat Python 3 aus:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

De generéierte Binärfichier ass 16.777.244 Bytes grouss an ass dofir geduecht, ënner 20.000.000 Bytes ze bleiwen. Eenheetstester an e GitHub-Actions-Workflow kontrolléieren d'Unzuel vun den Entréeën, d'Eenzegaartegkeet vun de Glyphen, d'Gréisst an de SHA-256; erfollegräich Läf publizéieren de Binärfichier an d'Zesummefaassung als Workflow-Artefakt `utf4088-million-catalogue`. De generéierte Binärfichier gëtt bewosst aus der Git-Geschicht erausgehalen. UTF-4088 bleift experimentell a si net mat Unicode kompatibel.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Rezent Ergänzungen — Pi-Konstant, Modium-/Registry-Startup, sleelactl-Math a `@native`

Dës Sektioun hält Funktiounen fest, déi während der Aarbecht vum 2026-11 derbäikomm sinn, déi neisten Gruppen als éischt. D'verlinkt Quelldateien an hire Validéierungsstatus si maassgeblech; Ännerungen, déi nach an der Iwwerpréiwung sinn, ginn net als gemergt beschriwwen, bis se `master` erreechen.

### D'Pi-Katalogkonstant mat 100.000.000 Zifferen

De Repository liwwert e verifizéierte Wäert vun **honnert Millioune Zifferen** vu π ënner [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (eng wuelgeformt Dezimalzuel, `3.14159…`), niewent engem Wäert mat 57.344 Zifferen ënner [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Béid sinn onofhängeg als echt π verifizéiert — kuckt [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md), dat d'Kontrolle dokumentéiert (Léierbuch-Präfix, de Feynman-Punkt bei der Bruchstell 762, déi berühmt millionst Zifer, Iwwereneestëmmung mat `lib/math/SLPiReference.sleela` an eng Ziffer-Frequenzanalyse vun der ganzer Datei) an e One-Shot-Reproduktiounsscript.

### Modium™ / Registry™ lued d'Pi-Konstant beim Startup vum Subsystem

Op den dedizéierten Subsystem-Entréespunkten — `sleela registry` an `sleela modium` (an beim éischte Lancement vum Installer), **net** bei all `sleela run` — lueden d'Subsystemer Modium a Registry déi grouss π-Katalogkonstant. Op engem interaktiven Terminal drécke si eng flüchteg rout `Loading Modium™` / `Loading Registry™`-Zeil aus, déi dann op der Plaz geläscht gëtt, éier déi normal Ausgab ufänkt; de Effekt ass strikt un d'TTY gebonnen, sou datt Pipe- an Testausgab Byte fir Byte onverännert bleiwen. De Locator (net d'Bytes) gëtt als Registry-`Math`-Schlëssel-Standardwäerter `piReferenceLargePath` an `piReferenceLargeDigits` gesaat, souwuel am SLeeLa-Modell ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) wéi och am nativen Seeder ([`registry/sleela_registry.c`](registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` stellt `largePath()` / `largeDigits()` zur Verfügung. D'π-Konstant gëtt vu **béiden**, dem Standard-`make`-Installer an dem gefouerten „beautiful“-Installer, an de SLeeLa-Standuert vum OS (`<data>/pi/`) installéiert. Detailer: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — de schéinen Installer an en nativ ënnerstëtzte `math`-Subject

- De gefouerten („beautiful“) Installer installéiert elo `sleelactl` (d'SLeeLa-Kontrollebene) a seng Modulregistry, entspriechend dem `make`-Installer. Kuckt [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- `sleelactl` huet eng nei **`subject`**-Natur fir déi nativ C/C++-Subject-Bibliothéiken kritt, déi an d'Runtime kompiléiert sinn, a registréiert déi éischt: **`math`**. `sleelactl run math` féiert de nativen `__NativeMath`-Backend (`impl/subjects/math/math_native.cpp`) vun Ufank bis Enn iwwer d'Referenzdemo vum Subject aus; `sleelactl info math` erkläert, wéi een dohinner kënnt (`import math;`). D'Subject-Natur bitt nëmmen `run info` — et gëtt bewosst kee `check`-Verb, well `sleela check` déi SHEET-registréiert native Subjecten net lued.

### `@native`, Native Drive a méisproocheg Ausféierung

Eng nei **`@native`**-Methodeannotatioun freet d'Ahead-of-Time-native Kompilatioun vun enger heisser Method un. De neie Kommando `sleela aot` fënnt `@native`-Methoden, verifizéiert, datt all eenzel an engem ënnerstëtzten *auslagerbaren Ënnerset* läit, a gëtt C/C++ aus — **fail-closed** (hie weigert sech mat engem präzise Grond) bei all Konstrukt, dat hien nach net iwwersetze kann, sou datt ni eng halleft iwwersate Method entsteet.

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

`@native` bleift eng Ufro fir AOT-Behandlung, wou net ënnerstëtzt Konstrukter virun der Ausgab zeréckgewise ginn. De [Native Drive](impl/native-drive/README.md) ënnerstëtzt Méi-Eenheeten-C/C++-Executables- a Shared-Library-Builds iwwer Host-Compiler (dorënner MSVC), ënnerstëtzt SLeeLa-AOT-Ausgab, Symbolinspektioun géint d'ABI vum native Loader, shell-fräi Prozessausféierung, stdin-/stdout-/stderr-Behandlung, méistufeg Piping an XClass-Delegatioun un de Produktiv-Loader. `sleelactl` stellt seng Build-/Run-Kommandoen zur Verfügung a routéiert d'XClass-Inspektioun an explizit systemd-Unit-Rendering/-Installatioun iwwer `inspect native-drive`. De VM besëtzt eng begrenzt Handle-Tabell fir native Moduler mat obligatoresch Host-Autorisatioun a generatiounsgeprüfte Handles. D'Syntax 3.0 füügt elo `nativeOpen`, `nativeCall` an `nativeClose` derbäi: e append-only Bytecode-Dispatch iwwerschreit eng separat versionéiert null/bool/int64/double/UTF-8-ABI, während VM-eege Arrays, Structs an aner zesummegesate Wäerter am Interpreter bleiwen a gläichnamege Bytecode-Fallback benotze kënnen. Plugins kënnen optional eng versionéiert Host-Callback-Tabell kréien, fir exakt SLeeLa-Funktiounen duerch verschachtelt VM-Ausféierung opzeruffen, mat GC-Rooting vum suspendéierte Stack, enger expliziter Allowlist vun Host-Callback-Nimm a ofgeleent rekursiv native API-Opruffer. Native Code bleift vertrauenswierdegen In-Process-Code, keng Sandbox. Quell-Prozesspolitik-Annotatioune hunn en versionéierte Schema, ginn awer nach net geparst/duerchgesat. Detailer, Sécherheetsgrenzen an d'Ledger mat de siwe Workstreams: [Native Drive](impl/native-drive/README.md) an [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).


## XIX. Internationale README-Editiounen

SLeeLa bitt landspezifesch README-Entréeën fir déi 50 Mäert vu senger internationaler Editioun un. All Land huet eng Datei mam Numm `README.<CC>.md`; wou méi Sproochen relevant sinn, sinn se an der Datei vun deem Land gruppéiert.

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

**Iwwersetzungspolitik:** Dës Dateie sinn ufanks lokaliséiert Landentréeën. Hir Aféierungen a Sproochuewerschrëfte si lokaliséiert, während déi komplett technesch Referenz an der kanonescher englescher [README.md](README.md) bleift. Eng Landdatei däerf net als komplett Iwwersetzung beschriwwe ginn, bis all Sektioun iwwersat ass, technesch Identifikateuren a Linken nogekuckt goufen an d'Editioun eng Sproochiwwerpréiwung kritt huet. D'Sproochofdeckung an dësem éischte Duerchgang ass representativ an ass keng Behaaptung, datt all verfassungsrechtlech unerkannt, regional, indigen Sprooch oder Zeechesprooch komplett iwwersat ass.

## Appendix — erhale bliwwen regional Leedung

### 1. API-Kontrakter a Moduler

All ëffentlech API soll dokumentéieren, wéi eng Inputen akzeptéiert ginn, wéi eng Outputen zeréckkommen, wéi eng Feeler méiglech sinn, wien d'Erënnerung besëtzt a wéi eng Niewewierkunge virkommen. D'Dokumentatioun vun engem Modul soll Viraussetzungen, Nobedingungen a Plattformlimiten däitlech beschreiwen.

E Symbolnumm oder eng Modulreferenz ass keng Erlaabnis, fir Code auszeféieren. D'Autorisatioun muss un der richteger Ausféierungsgrenz kontrolléiert ginn. Bei ongültegen Inputen, feelende Ressourcen oder net ënnerstëtzte Funktiounen soll d'Operatioun sécher ofbriechen an eng diagnostizéierbar Feelermeldung liwweren.

### 2. Compiler-Pipeline an Teststrategie

D'Tester sollen sech op d'Etappe vun der Compiler-Pipeline verdeelen: lexikalesch Analyse, Parsing, semantesch Analyse, IR-Lowering, Codegeneratioun, VM-Ausféierung an nativ Integratioun. Testet all Etapp getrennt an och d'Iwwergäng tëscht den Etappen.

E Regressiounstest soll de klengste reproduzéierbaren Input, dat erwaart Resultat an dat tatsächlecht Resultat enthalen. Negativ Fäll sollen Syntax- a Typfeeler, feelend Inputen, Ressourcefeeler an ABI-Inkompatibilitéit ofdecken. Tester, déi iwwersprong oder net ausgefouert goufen, däerfen net als erfollegräich gemellt ginn.

### 3. Kompatibilitéit a Migratioun

Wann eng ëffentlech API, e Dateiformat oder d'Verhale vum Compiler geännert gëtt, dokumentéiert d'Auswierkungen op d'Réckwärtskompatibilitéit. Gitt Migratiounsschrëtt a Beispiller virun an no der Ännerung un. Erkläert, ob d'Ännerung Quellcode, kompiléiert Artefakter oder Runtime-Konfiguratiounen betrëfft.

Fir e Build- oder Testresultat reproduzéierbar ze maachen, notéiert de Git-Commit, de Betribssystem, d'Architektur, d'Compiler-Versioun, d'Toolchain an déi exakt Kommandoen. E Resultat op enger Plattform ass kee Beweis fir eng aner Plattform. Kuckt d'[Plattformmatrix](docs/PLATFORM-SUPPORT.md) an d'[Testpolitik](markdown/TESTING.md).

### 4. Lokaliséierung an Ënnerhalt

Dateinimm, Identifikateuren, Kommandoen, Weeër, URLen, Syntax a Codebeispiller mussen onverännert bleiwen. Iwwersat gëtt den Erklärungstext, net d'Tokens, déi de Compiler oder d'Shell erkenne muss. Benotzt konsequent Begrëffer fir Compiler, Runtime, virtuell Maschinn, Modul an API; wann e lëtzebuergesche Fachbegrëff net eendeiteg ass, kann den englesche Begrëff bei der éischter Ernimmung derbäistoen.

Wann déi kanonesch englesch README aktualiséiert gëtt, vergläicht déi entspriechend Kapitelen a passt dës Editioun un. Kontrolléiert Markdown-Linken, Iwwerschrëften a Code-Fences.

**Iwwersetzungsstatus:** D'Sektiounen I–XIX sinn op Lëtzebuergesch iwwersat. Technesch Identifikateuren, Kommandoen, Codebeispiller, Dateinimm, Linken an Adressen sinn erhale bliwwen. Eng ofschléissend sproochlech Iwwerpréiwung duerch Mammesproochler gëtt recommandéiert.
