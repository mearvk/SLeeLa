<img align="right" src="../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — Norge (NO)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Et native systemspråk, en kompilator, et kjøretidsmiljø og et økosystem av virtuelle maskiner.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="../../docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Internasjonal landsutgave.** Denne utgaven er på norsk bokmål. Tekniske identifikatorer, kommandoer, kodeeksempler, filnavn, stier og lenker er uendret i forhold til den kanoniske engelske [README.md](../../README.md).

**The Now. The Ingredient.** Et språk er mer enn syntaks: Det er kompilatoren, kjøretidsmiljøet, bibliotekene, verktøyene og plattformavtalene som gjør programvare anvendelig.

**Hva SLeeLa er involvert i.** SLeeLa er involvert i **Math**, **Object**,
**Science** og **Reach** — de fire ansiktene til arbeidet: det beregnelige
(Math), den avklarte tingen som står i katalogen over det som er (Object), det
disiplinerte studiet av det sanne (Science) og utvidelsen av et systems hånd
til andre systemer og mennesker (Reach). **Nasjonale interesser består** — de
settes ikke til side av noen av de fire; der et emne berører en nasjons
interesse, beholdes den stillingen (registrert troskapsfullt, f.eks. i hver
klasses Metadata `metaOrigin` / `metaNationalSchedule`) og oppløses aldri.

## I. Kom i gang
- [Bygg den native implementasjonen](../../impl/README.md)
- [Virtuelle maskiner og Modium — praktisk brukerveiledning](../../virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [CPU-stakk i SLeeLa-kildekode](../../lib/cpu/CPU.md) — en komplett maskin fra logiske porter → ALU → kjerner → operativsystem → programmer, en oversikt over 130+ CPU-profiler samt justerbare [spenninger, variable klokkefrekvenser og ekspertinnstilte tidtakinger](../../lib/cpu/TIMING.md) som du kan [kjøre etter spesifikasjon og overklokke](../../lib/cpu/components/README.md)
- [Dokumentasjonsindeks](../../docs/README.md)
- [CSPML-datakilde og forskningsreferanse for Salt & Salary](../../CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — opphav og integrasjonsveiledning for eksterne amerikanske data om steder, sosioøkonomi, arbeidsstyrke, utdanning og offentlige skoler.
- [Generert API-dokumentasjon](../../docs/api/README.md) — gjeldende HTML-referanse, versjonert API-utgave og instruksjoner for regenerering
- [API-katalog](../../api/README.md)
- [Standarder for beregningsmessig sammenligning](../../comparison/README.md) — standarder for typer, minne, ABI og samsvar for SLeeLa VM, Java/JVM og native C/C++.
- [Recursion stanchion — C- og C++-prototyper](../../recursion/stanchion/README.md) — primitivspesialiserte eksempler for Boolean, Byte, Character, Integer, Long, Float og Double, ved siden av generiske, anvendte, vitenskapelige og pedagogiske rekursjonsmønstre.
- [Plattformstøtte og valideringsmatrise](../../docs/PLATFORM-SUPPORT.md)
- [Merkevareveiledning](../../docs/BRAND-GUIDE.md)
- [Merchandise-konsepter og -materiell](../../merchandise/README.md)
- [NitroWebExpress-logo](../../images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Testpolicy og utgivelseskrav](../../markdown/TESTING.md)

## II. Nyheter

Nyere tillegg til språket, driverne og standardbiblioteket:

- **Standardbibliotekpakken `advantage` — å advantage et utbytte, med
  matematikk, fysikk, integrasjon, gradering og rutbar utdata.**
  [`lib/advantage/`](../../lib/advantage/README.md) innkapsler *å advantage et
  entydig utbytte i forhold til en modell* som en lagdelt **Sequitur-løk**
  (utbytte → review → concerns → maths → additions → subtraction → regrouping
  → iteration), som kan navigeres innover/utover med full dynamisk oppdatering
  og restitute, og som eksponeres gjennom en enkel **brukermodell** og en
  **utviklermodell** med full kontroll. Den har fem fagområder: **`math/`**
  bilateral handelsøkonomi (felles profittmaksimering, Nash-forhandling, den
  bilaterale newsvendor-kritiske ratioen), **`physics/`** en strukturell
  økonomisk «fysikk for tillitsfelt» (tillit som metrisk tensor, den
  transaksjonelle geodeten, en announcement-kontaktstruktur, en CIR+jump-SDE for
  tillitsforfall — en modellanalogi, merket som slik), **`integration/`** en
  rekursivt sammensettbar vertikal integrasjonsmodell for kapitalisme/detaljhandel
  (kompartmenter, engineering bays, trial-kompartmenter, newton median trays og
  en systemsofistikasjonsscore), **`grading/`** en sofistikasjonskrets-gradering
  som vurderer hver modul og ethvert sett av winning/losing/grader/absolute-moduler
  A–D, og **`output/`** en rutbar emit-søm som sender modulens utdata til
  terminalen, en **CommonRails**-formatert flate eller en socket/connector
  (innrammet for en native `slnet_write`). Hver klasse bærer Metadata-Class-posten;
  fem kjørbare demoer validerer fagområdene, og de tilhørende
  [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md),
  [`MATH.md`](../../lib/advantage/MATH.md) og
  [`PHYSICS.md`](../../lib/advantage/PHYSICS.md) dokumenterer dem. En native C/C++-forbruker
  under [`advantage/`](../../advantage/README.md) (C-kjernen
  `advantage_value`/`advantage_mean`, et XML-kunnskapslager, et sikkert
  HTTPS/SFTP/SSH-innhentings- + nøkkellager- + DH/ECDH-transportbibliotek) kobler
  modellen til utarbeidede
  [handelseksempler](../../advantage/examples/README.md) — positive handler fra
  antatte internettkilder, en publikumsrelevanssekvens (Traders,
  Consumers, Helpers, Healers) og åtte modeller rangert A–D (D = mest Cement,
  A = mest Brand).
- **Annotasjoner for løkketiming — `@loopDelay` / `@loopBudget` (+ den innebygde
  funksjonen `sleep(ms)`).** De første **setningsnivå-, atferdsmessige**
  annotasjonene i SLeeLa: skrevet på egen linje foran en `for`-/`while`-/`do`-løkke
  setter de inn en reell pause i løkkens kjøring. `@loopDelay(ms = 250)` /
  `@loopDelay(seconds = 2)` sover en fast tid per iterasjon (en bar
  `@loopDelay(250)` bruker millisekunder som standard); `@loopBudget(seconds = 10,
  iterations = 1000)` fordeler et kjent antall iterasjoner jevnt over et totalt
  veggklokkebudsjett. Begge senkes til den nye append-only-opkoden **`OP_SLEEP`**
  (ISA 157), som også ligger bak den generelle innebygde funksjonen **`sleep(<ms>)`**
  som kan kalles hvor som helst; pausen er implementert portabelt (`nanosleep` /
  `Sleep`). Spesifisert i [`SLEELA.syntax` §32](../../SLEELA.syntax); API-referanse i
  [`api/sleela/3.0/LOOP_TIMING_API.md`](../../api/sleela/3.0/LOOP_TIMING_API.md);
  kjørbar
  [`examples/loop-timing-annotation-demo.sleela`](../../examples/loop-timing-annotation-demo.sleela).
  En indeks over øktens tillegg (blant annet dette, den native `OP_BIGMUL`
  for multiplikasjon av store heltall og de frittstående verktøyene
  **[local-generator](../../local-generator/README.md)** og
  **[pi-generator](../../pi-generator/README.md)**) finnes i
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](../../api/sleela/3.0/SESSION-ADDITIONS-API.md).
  De tre genererte API-flatene
  ([`sleeladoc-api/`](../../sleeladoc-api/README.md),
  [`docs/api/`](../../docs/api/README.md) og det uforanderlige
  [`api/sleela/3.0/version-014/`](../../api/sleela/3.0/version-014/index.html)-
  øyeblikksbildet) ble gjenbygd med `make api-docs` og har nå de nye
  instruksjonsoppføringene `SLISASleep` / `SLISABigMul` på sin `vm`-pakkeside.
- **SleelaDoc fra kommandolinjen — `sleela docs`.** SleelaDoc kan nå nås
  direkte fra terminalen. `sleela docs` lister alle dokumenterbare emner;
  `sleela docs crypto` viser en pakke; `sleela docs stringbuilder` løser opp en
  klasse (uavhengig av store og små bokstaver, og via `SL`-prefikset for
  standardbiblioteket → `SLStringBuilder`) og skriver ut en rik side —
  **definisjon, designformål, relasjoner, struktur, user clays og en mestringsstige
  med fem trinn** — med rangerte «mente du»-resultater ved nesten-treff. Forfattere
  beriker en side ved ganske enkelt å legge til SleelaDoc-`@`-tagger (`@intent`,
  `@thesis`, `@axiom`, `@param`, `@see`, `@mastery`). `sleela docs --generate [dir]`
  skriver et rent tekstkorpus (én side per klasse + en `TOPICS.index`) som CLI-en
  deretter leser frakoblet; **`make sleeladoc`** pakker dette korpuset **og de
  tilhørende `.sleela`-kildene** i `sleeladoc/build/sleeladoc.jar`. For emner som
  bare har fasade (f.eks. `psychiatry`, `politico`) søker
  `sleela docs --facade <topic>` i filsystemet ved kjøring og lister de faktiske
  **mappene (deretter filene, som naturlig bevegelse)**, avgrenset av
  brukeroverstyrbare grenser på **6 MB / 6000 filer / 6 minutter**.
  Enhetene i `lib/sleeladoc` vedlikeholdes på gjeldende utgave,
  **`#sleela 3.0`**. Se [`man/man1/sleela.1`](../../man/man1/sleela.1) og
  [`lib/sleeladoc/README.md`](../../lib/sleeladoc/README.md).
- **SleelaDoc — en dokumentasjonsløsning i Javadoc-stil, med et generert HTML-område.**
  **SleelaDoc** er SLeeLas dokumentkommentarkonvensjon i kildekoden
  (vinkelparentes-kommentarer markert med `@doc` / `@page`; se
  [`manifesto/SLEELADOC.md`](../../manifesto/SLEELADOC.md)), og
  [`lib/sleeladoc`](../../lib/sleeladoc/README.md) er **ekstraktoren** som gjengir en
  `.sleela`-enhet som en Markdown- eller HTML-API-side — på samme måte som `javadoc`
  gjengir Java, og dokumenterer *hensikt*. Et komplett, bla-bart **HTML-dokumentasjonsområde
  for hele `/lib`** genereres inn i [`/sleeladoc-api`](../../sleeladoc-api/README.md)
  av `tools/generate-sleeladoc-api.py` (åpne `sleeladoc-api/index.html`; en ren
  stil i blått / lyseblått / lysegrått / hvitt med et hierarki pakke → klasse →
  medlem og en ordliste skrevet for leseren *Student 2*). Kompilatoren sender
  også ut forfattermerkede dokumentutdata som biprodukt av kompileringen (`--wiki`
  MediaWiki-side + en opprinnelig kildetvilling), dokumentert i
  [`manifesto/SLEELADOC.WIKI.md`](../../manifesto/SLEELADOC.WIKI.md). Kjørbar:
  [`lib/sleeladoc/sleeladoc-demo.sleela`](../../lib/sleeladoc/sleeladoc-demo.sleela).
- **XML Document Object Model (`lib/xml`).** En XML-DOM på SLeeLa-siden — et
  dokument som eier et tre av element-/tekst-/kommentar-/CDATA-/PI-noder med
  ordnede barn og navngitte attributter, en serialiserer (kompakt/pen, XML 1.0
  §2.4-escaping), en char-cell-parser og kvalitetsregisteret `SLXmlAspect`.
  Nodelikhet er en treleddet tillitsstige (identitet → samme subjekt → struktur),
  refleksiv, symmetrisk og deterministisk. Se [`lib/xml/README.md`](../../lib/xml/README.md) og
  den kjørbare [`lib/xml/xml-dom.sleela`](../../lib/xml/xml-dom.sleela).
- **Brede signerte heltallsprimitiver (`long`, `int64`, `int128`).** `int`, `long`
  og `int64` er alle 64-bits signerte heltall som typesjekkes og kjører identisk;
  `int128` godtas som heltallstype (foreløpig 64-bits-basert, med ekte 128-bits
  som dokumentert oppfølging). Den signerte base-2-serien `int32`..`int16777216`
  (syntaks 2.7) og den tilsvarende usignerte serien `uint32`..`uint16777216`
  (syntaks 2.8) utvider dette. Se [`SLEELA.syntax`](../../SLEELA.syntax) §21, §24, §25.
- **`@mix`-konsekvensfelt (syntaks 2.9).** En klasse kan deklarere flere
  instansfelt med **samme navn** når hver gjentakelse har `@mix`; språket
  forstår gruppen som konsekvent (en tabell/liste indeksert etter nummer),
  ovenfra og ned `1,2,3,…` som standard eller manuelt via `@mix(1,2,3,...)` der
  antallet indekser må samsvare med antallet felt. Se [`SLEELA.syntax`](../../SLEELA.syntax) §28.
- **Dokumentmarkører — den naturlige prosessen, titulert Preonems
  (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).**
  Disse bare dokumentmarkørordene har samlet tittelen **Preonems**
  (én markør er en *Preonem*; settet er *Preonems* — se
  [`SLEELA.syntax`](../../SLEELA.syntax) §26.1). Førsteklasses, rent deklarative
  dokumentmarkører som navngir hva et dokument *er* i rekkefølgen for tilblivelse:
  `author`/`authorial` (syntaks 2.4; håndteres av profesjonelt råd — det eneste
  medlemmet med atferdseffekt), `material;` (avklart kilde) og `memory;`
  (syntaks 2.5), `determine;` (syntaks 2.6; et dokument som er åpent for videre
  behandling av en doktor, lege eller ghost mot ferdigstillelse ved lovlig hånd),
  og de to nyeste, **`object;`** (en avklart ting som står i katalogen over det
  som er — **reifisert, gitt konkret stilling, ved syntaks 2.8**) og
  **`field;`** (grunnen for relasjon som objektene står og beveger seg i) —
  begge sperret fra syntaks 2.6. Hver er bar, kontekstuell og versjonssperret av
  frontenden uten effekt på parsing, typing eller kodegenerering; utenfor
  markørposisjonen forblir hver enkelt en vanlig identifikator (slik at `field`
  aldri forstyrrer et klassemedlem eller en `.field`-tilgang). Se
  [`SLEELA.syntax`](../../SLEELA.syntax) §18–§20, §23 og §26.
- **Nordshrift ved syntaks 3.0 med full bibliotekrekkevidde.** Nordshrift løser opp
  gjennom den samme rekursive `/lib`-indeksen som kompilatoren og når alle
  pakker og `.sleela`-enheter; `nordshrift check` rapporterer tallene for hele treet.
- **Recursion stanchion.** Katalogen [`recursion/stanchion/`](../../recursion/stanchion/README.md) omfatter nå dedikerte C- og C++-referanseprototyper for de sju dokumenterte primitivkontraktene, pluss generiske, anvendte, vitenskapelige og pedagogiske eksempler. Dette er referansekilder; kompiler og test dem før de behandles som verifiserte.

- **Kommentarer med vinkelparenteser (`<` / `<<`).** En ny kommentarstil for
  fremtiden — `< line >` og `<< block >>` — lekset av SLeeLa-frontenden og dokumentert i
  [`SLEELA.syntax`](../../SLEELA.syntax) §1.1.
- **Heltallsliteraler med grunntall.** `int`-literaler godtar nå heksadesimale (`0x`),
  binære (`0b`) og oktale (`0o`) former med `_` som sifferseparator, både i
  SLeeLa-kildespråket ([`SLEELA.syntax`](../../SLEELA.syntax) §1.4) og i
  [`.xlt` CPU-modellformatet](../../lib/xlt/README.md).
- **Metadata Class-standarden på tvers av biblioteket.** Det beskrivende
  metadatamønsteret (proveniens, lovlighet, jurisdiksjonell rekkevidde) bæres nå av
  hver klassebærende `.sleela`-fil under `/lib`; se
  [`SLEELA.syntax`](../../SLEELA.syntax) §15 og
  [leksjonen i veiledningen](../../tutorial/13-metadata-class.md).
- **En rikere tekst-/String-familie.** `lib/text` får ekte hjelpere i språket,
  en char-cell-visning som gir ekte `charAt`/`substring`/`indexOf`/`reverse`/
  kolonnejustering, og en ekte posisjonell malmotor — se
  [veiledningen om tekst og String](../../tutorial/14-text-and-strings.md) og den
  kjørbare [`lib/text/text-string-demo.sleela`](../../lib/text/text-string-demo.sleela).
- **Førsteklasses `String` i Nordshrift-ark.** Et `.sst`-ark kan deklarere en
  typet `strings:`-blokk som bidrar til byggets komponentmanifest;
  se [Nordshrift Sheet Reference](../../wiki/Nordshrift-Sheet-Reference.md).
- **CPU-modeller kjører både via opkoder og via funksjonskall-ekvivalens.** Hver
  `SL<ARCH>CPU` arver grunnlaget for å kjøre en native instruksjonsstrøm
  gjennom sin hent-dekod-utfør-løkke *og* for å kjøre koden til en antatt
  annen prosess ved ekvivalente kall på det delte VM-substratet — se
  [`lib/cpu/EXECUTION.md`](../../lib/cpu/EXECUTION.md) §5.

## III. Bygg og test

**Forutsetninger:** en C/C++-verktøykjede (GCC eller Clang) og GNU Make; Python 3 for
manifest- og flytverktøyene; på Linux utviklingspakkene for X11/Xrandr for
visningsproduktene (en moduls `make deps` rapporterer/installerer det den trenger).
Bygget i `impl/` er fail-closed mot et pålitelig SHA-256-manifest, som rot-`Makefile`
peker på automatisk.

Fra en ren utsjekking er den raskeste starten:

```sh
make                                  # build everything from the repo root
```

Eller bygg og test bare den native kjernen:

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

`make` fra depotets rot bygger alle produkter (kjerne, VM, kodeker, churn,
antivirus, …) og avslutter med å bygge **registeret** og materialisere
grunntabellen — slik at et bygd tre leveres med et gjennomgåbart register
ferdig fra start. De viktigste målene:

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

Målet `registry` kjører [`build-registry.sh`](../../build-registry.sh): det bygger
registerkjernen, fyller grunntabellen inn i det portable tekstlageret
`registry/build/sleela.sleelareg` og eksporterer MySQL-tabellskriptet
`registry/build/registry.sql` (via `sleela registry export`). Gjennomgå det med
`sleela registry` eller `registry/build/sleela-registry review`. Se
[registerveiledningen](../../registry/README.md) og [Registry 3.0-arkitekturen](../../registry/REGISTRY-3.0-ARCHITECTURE.md).

Konsulter bygge- og testveiledningene for forutsetninger og plattformspesifikke trinn. Et vellykket bygg av én komponent beviser ikke at alle biblioteker eller plattformer er klare for utgivelse.

## IV. Installasjon

Etter at de native produktene er bygget, installerer du kommandoene og kjøretidsstøttefilene:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

Det konvensjonelle installasjonsprogrammet bruker som standard `/usr/local/bin`, `/usr/local/lib/sleela`
og `/usr/local/share/sleela`. Det bevarer en eksisterende oppstarts-
konfigurasjon og nekter å overskrive uspårede eksisterende kommandoer. Avinstallering
fjerner bare filer som er registrert av installasjonsprogrammet, og bevarer konfigurasjon og
uspårede kjøretidsdata.

For en fokusert, veiledet utrulling av bare hovedpakken — binærfilene `sleela`,
`sleelvac` og `nordshrift`, standardbiblioteket, protokollene og
støttemodulene — bruker du det rene installasjonsprogrammet:

```sh
make clean-install       # guided install of the main package only
```

Det tar ett primærvalg (installer hovedpakken), deretter et valg av omfang,
og ved suksess en valgfri velger for utvalgte utvidelsesmoduler. For den
interaktive utrullingsflyten og alle installasjonsvalg, se
[Rask og trygg installasjon](../../install/README.md).

## V. Språk og kjøretidsmiljø
Den autoritative grammatikken er [`SLEELA.syntax`](../../SLEELA.syntax). Depotet dokumenterer for tiden syntaks **3.0**; versjonen til kompilatoren/verktøyet og versjonen til kildesyntaksen er to ulike ting. Overflaten i 3.0 legger til **dokumentmarkørene `pure` og `gasoline`** (§30) — et par bare, kontekstuelle, rent deklarative Preonems som betyr nesten det samme, der hver markerer dokumentets stilling gjennom sin egen ærlighet. Syntaks 3.0 konsoliderer språkoverflaten 1.0–2.9 til den gjeldende normative utgaven og bevarer alle funksjonsspesifikke minimumssperrer. Den foregående 2.9-overflaten legger til **`@mix`-konsekvensfelt** (§28) — en klasse kan deklarere flere instansfelt med samme navn når hvert har `@mix`, forstått som én indeksert gruppe (ovenfra og ned `1,2,3,…` eller manuelt `@mix(1,2,3,...)` der antallet indekser må samsvare med gruppen). Den viderefører de base-2 signerte/usignerte brede heltallsfamiliene `int32`..`int16777216` (§24) og `uint32`..`uint16777216` (§25) over de tidligere `long`/`int64`/`int128` (§21); **støttedokumenter for nøkkelord** (§22); og **Preonems** — de rent deklarative dokumentmarkørene `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) og `artifact` (§35, stilling på lovgivende nivå gradert mot tid og rom), samlet titulert Preonems (§26.1). Konstruktoroverlastoppløsning og `this(...)`/delegering til basekonstruktør er uttrykkelig utenfor den verifiserte kontrakten for syntaks 1.8. Konsulter grammatikken og implementasjonsnotatene før du stoler på en funksjon. SleelaDocs ni kildeenheter deklarerer nå `#sleela 3.0`; den delte kompilatorsperren godtar kildesyntaks 1.0–3.0. Det nye API-bygget produseres med `make api-docs`.

Depotet inneholder en native C/C++-implementasjon, SLVM-genereringsreferanser, standardbibliotekmoduler, tester, eksempler og plattformintegrasjoner. Implementasjons- og valideringsstatus varierer fra modul til modul; konsulter modulens spesifikasjon og gjeldende CI-resultater i stedet for å anta jevn fullstendighet.

## VI. Ingeniørprinsipper
- **Bevis foran påstander:** skill mellom implementert, testet, eksperimentell og planlagt atferd.
- **Én kontrakt per grense:** dokumenter syntaks, API-er, filformater, plattformforutsetninger og feilatferd.
- **Portabelt av design:** isoler plattformspesifikke backender og test på native runnere.
- **Trygg feil:** test ugyldig inndata, ressursgrenser, prosessfeil og opprydningsstier.
- **Reproduserbare utgivelser:** bygg rent på nytt og publiser artefakt-hasher og utgivelsesnotater.

## VII. Antivirus-bygg og integritet

Antivirus-pakken Sleelavirin inneholder en native SHA-256-skanner, en valgfri
ClamAV-adapter, integritetsmanifester for kilde/indeks og sjekksummer for
plattformpakker. Bygg og test pakken fra depotets rot:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Integritetsmanifestet genereres under `antivirus/build/`; det dekker sporede
kilde- og dokumentasjonsinndata pluss beskyttede signaturindeksfiler, men
utelater generert utdata og den muterbare ClamAV-bufferen. Øyeblikksbilder av
ClamAV-databasen har sine egne manifester. Utgivelses-ZIP-filer inneholder `INTEGRITY-MANIFEST.json`,
`SHA256SUMS` og en `.sha256`-sidefil. Verifiser hasher for nedlastede arkiver mot
en pålitelig verdi som er innhentet uavhengig: en sjekksum som er lagret ved siden av et arkiv er
ikke en utgiversignatur. Se [Sleelavirin-veiledningen](../../antivirus/README.md),
[sikkerhetssjekklisten](../../antivirus/SECURITY.md) og
[arbeidsflyten for antivirus-bygg](../../.github/workflows/antivirus-binaries.yml).

En utgivelse regnes ikke som verifisert før de relevante CI-jobbene består på
den nøyaktige utgivelsescommiten og de resulterende artefaktene er kontrollert.


## VIII. SLeeLa-editortemaer — Vim og Emacs

SLeeLa inneholder et samordnet sett av profesjonelle mørke fargeskjemaer for **Vim** og **Emacs**, pluss et native C/C++-verktøy for paletter. Det opprinnelige skjemaet **American Professional** er fortsatt standard. Ti ledsagende paletter er tilgjengelige for ulike kulturelle og institusjonelle designinspirasjoner; dette er fortolkende designpaletter og ikke offisielle representasjoner av noen nasjon, kultur, domstol eller institusjon.

### De elleve skjemaene og fargene deres

| Skjema | Bakgrunn | Forgrunn | Nøkkelord | Typer | Strenger | Kommentarer |
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

#### Nasjonale flaggfarger (kildepalett)

De greske og tyrkiske skjemaene er avledet fra de to landenes **offisielle nasjonale flaggfarger**, vist her med deres nøyaktige publiserte hex-verdier. Skjemaenes bakgrunner/aksenter ovenfor er mørkere, editorvennlige fortolkninger av disse kildefargene — tabellen nedenfor er referansen for «fargene slik de er nasjonalt uttrykt».

| Nasjon | Farge | Hex | Grunnlag og betydning |
|---|---|---|---|
| **Hellas** | Blå | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | Nasjonal flaggblå (lov 851/1978). Himmelen og havene; blasonert heraldisk som *azure*. |
| **Hellas** | Hvit | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Nasjonal flagghvit. Renheten i uavhengighetskampen; korset betegner den østortodokse kristendommen. |
| **Türkiye** | Rød (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | Nasjonal flaggrød (den tyrkiske flaggloven). Feltet; etter tradisjonen martyrenes blod og den osmanske suvereniteten. |
| **Türkiye** | Hvit | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Nasjonal flagghvit. Halvmånen og stjernen — fred, renhet og symbolet for den turkiske/islamske arven. |

Skjemaene forankrer hver editors forgrunn i den nasjonale **hvite** fargen, og nøkkelord, markør, markering og statuslinje i den nøyaktige nasjonale **blå** (`#0D5EAF`) eller **røde** (`#E30A17`), slik at fargene som betyr noe, bokstavelig talt er de nasjonale fargene. Dette er fortolkende designpaletter; flaggfargereferansen er oppgitt for proveniensens skyld og er ikke en offisiell representasjon av noen av statene. Fargebetydningene følger beskrivelsen av statssymboler fra det greske utenriksdepartementet (Hellenic Republic MFA) og tradisjonen i den tyrkiske flaggloven.


### Fargeprøvetabell

Palettcellene nedenfor bruker **SVG-fargeprøver**, slik at GitHub gjengir de faktiske fargene i stedet for tomme HTML-fargeceller. Hver stripe viser de seks fargene i denne rekkefølgen: bakgrunn, forgrunn, nøkkelord, typer, strenger og kommentarer. De nøyaktige hex-kodene er oppført ved siden av hver fargeprøve, i samme skjemarekkefølge som referansetabellen ovenfor.

| Skjema | Gjengitte palettfarger | Hex-koder (bakgrunn · forgrunn · nøkkelord · typer · strenger · kommentarer) |
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

Ytterligere aksentfarger brukes til funksjoner, konstanter, markeringer, diagnostikk og preprosessordirektiver. Paletten Russian and Slavic bruker bevisst skifer, blått, salvie, rubin og antikt gull med middels intensitet for et roligere, profesjonelt uttrykk.

### Installer Vim-skjemaene

Fra depotets rot:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Legg til følgende i `~/.vimrc` for å slå på SLeeLa-syntaksutheving og velge et skjema:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Velg nøyaktig ett av disse fargeskjemanavnene:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Bruk for eksempel `colorscheme sleela-russian-slavic` for den russiske og slaviske paletten med middels intensitet, eller `colorscheme sleela-greek` / `colorscheme sleela-turkish` for de hellenske og Al Bayrak-palettene. Start Vim på nytt, eller last inn konfigurasjonen med `:source $MYVIMRC`.

### Installer Emacs-skjemaene

Opprett en lokal temakatalog, og kopier modus- og temafilene fra depotets rot:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Legg til dette i `~/.emacs` eller `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Bytt ut `sleela-american` med `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` eller `sleela-sleela` for å velge et annet skjema. Hvis Emacs spør om du stoler på temaet, må du inspisere den lokale temafilen før du godtar.

### Forhåndsvis de native C/C++-palettene

Det lille native palettverktøyet kan liste skjemaer, skrive ut terminalfargene deres og slå opp en enkeltfarge:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Av hensyn til bakoverkompatibilitet velger `--palette` og `--hex COLOR` uten `--theme` American Professional. Bygge- og testresultater bør kontrolleres på målsystemet og i CI; dokumentasjon av en kommando er ikke i seg selv bevis for at den besto.

Se [editorintegrasjonsveiledningen](../../linux-editors/README.md) for de fullstendige palettbeskrivelsene og detaljer om verktøyet.

## IX. Bidra
1. Les dokumentasjonsindeksen og testpolicyen.
2. Gjør fokuserte endringer med positive og negative tester.
3. Kjør målrettede tester, deretter hele testserien.
4. Rapporter OS- og verktøykjededetaljer; ikke beskriv hoppede over kontroller som bestått.

SLeeLa utvikles av **MEARVK LLC**. Historisk landingssidemateriale er bevart i [`docs/README-LEGACY.md`](../../docs/README-LEGACY.md).

## X. AE6E66 — forventning om avgift på Round Public-e-post

For initiativet AE6E66 er prosjektets uttalte forventning **$300,000,000.00 USD per Round Public-e-post** som et foreslått avgiftsbeløp knyttet til hver kvalifiserende Round Public-e-post.

Denne uttalelsen registrerer bare en prosjektforventning. Den er **ikke** en rettslig skattefastsettelse, et offentlig pålegg, en rettsavgjørelse eller en uavhengig fastslått skatteplikt. Enhver faktisk avgift ville kreve den relevante rettslige myndigheten, jurisdiksjonen, definisjonene, prosedyren og en kompetent offentlig avgjørelse.


## XI. Alfabet og tegnsettkjøretid

SLeeLas alfabetdokumentasjon og native tegnkjøretid vedlikeholdes i alphabet/sleela/ og lib/alphabet/native/. De native C++17-testene dekker streng UTF-8-skalardekoding/-koding, C<n>-kapasitetssignaturer og avgrenset medlemskap i tegnsett:

    make -C lib/alphabet/native test

Den eksperimentelle UTF-4088-katalogen og dens SLeeLa-oppslagsadapter er dokumentert i utf-4088/SLeeLa-ALPHABET-INTEGRATION.md. UTF-4088-ID-er er et eget navnerom, ikke Unicode-skalarverdier. Arbeidet med kompilator- og SLVM-binding spores uttrykkelig separat fra disse native grunnmurene.

## XII. AutoCAD Creator™

[AutoCAD Creator™](../../autocad/creator/README.md) er et interaktivt 2D-CAD-program
skrevet helt og holdent i SLeeLa. Der søsterprogrammet AutoCAD Renderer gjør en
plan om til en tegning i ett steg, lar Creator deg **opprette og endre** et 2D-
AutoCAD-dokument direkte — legg til vegger, rom, dimensjoner og notater, og flytt,
gi nytt navn til, bytt lag på, slett og angre det du har plassert — og **lagre**
resultatet som en ekte AutoCAD DXF-tegning (R12-tekst, åpnes i AutoCAD / LibreCAD).

Det grafiske grensesnittet bygger på **SleelaUI™**, SLeeLas eget plattformuavhengige
UI-verktøysett (under [`lib/user-interface`](../../lib/user-interface)) — ikke JavaFX — og
åpner et ekte native vindu på X11 / Cocoa / Win32. Designet følger én regel,
**«SleelaUI presenterer; SLeeLa bestemmer»**: GUI-et beregner aldri geometri og skriver aldri
DXF. Hvert verktøy utsteder en intensjon om å opprette / endre / slette / angre / lagre til den
autoritative [`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela), som eier det
redigerbare dokumentet og en reversibel kommandohistorikk, mens lerretet bare forhåndsviser
geometrien som Creator holder.

Den gjenbrukbare redigeringsmotoren ligger i [`lib/autocad`](../../lib/autocad) —
[`SLCadEntity`](../../lib/autocad/SLCadEntity.sleela),
[`SLCadDocument`](../../lib/autocad/SLCadDocument.sleela),
[`SLCadCommand`](../../lib/autocad/SLCadCommand.sleela) og
[`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela) — og kjører fra ende til annen headless.
GUI-skallet og demoen ligger under [`autocad/creator`](../../autocad/creator):

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

På en skjerm åpner dette det levende redigeringsvinduet; på en headless vert bygger det
tegningen i minnet, skriver ut en rapport pluss DXF-en og avslutter rent.


## XIII. Rekkefølge for UTF-4088-inndatakart

Den spesialiserte UTF-4088-katalogen definerer sin romlige gjennomgang av inndatakartet og abstrakte trykkiterasjoner i [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). Policyen for National Lien-emneindeksen finnes i [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Kartkoordinater, trykknivåer (1–50), permanente katalog-ID-er og alfabetiske rangeringer er uavhengige felt; ingen av dem skal i det stille erstattes av en annen. Trykk er et algoritmisk iterasjonsnivå, ikke en fysisk størrelse.


## XIV. Nylig utvidet pakkeindeks

Denne tilføyde indeksen fremhever nylig utvidede pakker, domenebiblioteker og
kildesamlinger. Oppføringene er gruppert alfabetisk etter pakkenavn eller emne.
Beskrivelsene angir hvor i depotet noe ligger; implementasjons- og
valideringsstatus bør kontrolleres i hver pakkes egen dokumentasjon og tester.

### A–C

- **Advantage** — `lib/advantage/` og `advantage/`: å advantage et entydig
  utbytte i forhold til en modell som en lagdelt Sequitur-løk, med fem
  fagmapper — `math/` (felles profittmaksimering, Nash-forhandling,
  bilateral newsvendor), `physics/` (metrikk/geodet/kontakt/SDE for tillitsfelt,
  en markert modellanalogi), `integration/` (vertikale integrasjons-
  kompartmenter og en sofistikasjonsscore), `grading/` (en A–D-
  sofistikasjonskretsgradering for moduler og sett) og `output/` (terminal / CommonRails /
  socket-emit). En native C/C++-forbruker (`advantage/`) kobler modellen til
  utarbeidede handelseksempler. Se [`lib/advantage/README.md`](../../lib/advantage/README.md)
  og [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md).
- **Alfabet og UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` og
  `utf-4088/`: alfabetkataloger, native støtte for tegnsett, rekkefølge for
  inndatakart og dokumentasjon for UTF-4088-integrasjon.
- **AutoCAD Creator** — `autocad/creator/` og `lib/autocad/`: interaktive
  2D-tegningsarbeidsflyter og gjenbrukbare CAD-dokument-/redigeringskomponenter.
- **Ballum** — kontroll av visning på pikselnivå: sett/hent per piksel, blit av region,
  lysstyrke/varme/gamma for hele skjermen, peker-/tastaturinndata og en
  **fullskjerms framebuffer-flate** for total, eid og rask kontroll per piksel
  (override-redirect-vindu + én enkelt `XPutImage`/MIT-SHM-presentasjon på Linux/X11).
  Native kjerne under `ballum/` med SLeeLa-fasaden i `lib/ballum/` og en VM-
  bro under `impl/`; se [`ballum/README.md`](../../ballum/README.md) og det
  kjørbare [`ballum/example/`](../../ballum/example/).
- **CDMA** — `cdma/` og `lib/cdma/`: CDMA-relatert teknisk materiale og
  arbeid på SLeeLa-kildebiblioteket.
- **Character** — `lib/character/`: karakterprofiler og gjenbrukbar
  karakterstatistikk, blant annet eksempler på biografiske profiler.
- **Coorenagraph** — `lib/coorenagraph/`: etisk resonnement, moralsk vokabular,
  overveielse og beslektede begreper; se pakkens README og kildefiler.

### D–G

- **Governance-kildemodeller** — [lib/governance](../../lib/governance/README.md) definerer deklarative SLeeLa-kildeformer for forslag, anonyme manuelle stemmesedler og aggregerte opptellinger. Den foreslåtte @vote-annotasjonen og den manuelle prosessen er dokumentert i [registry/DEMOCRATIC-VOTING-SYNTAX.md](../../registry/DEMOCRATIC-VOTING-SYNTAX.md); gjeldende kompilatorer behandler den som inaktive kommentarmetadata.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` til og med de tilgjengelige versjonerte HTTP-
  pakkekatalogene: versjonsspesifikke protokollspesifikasjoner og
  implementasjoner. Konsulter hver versjons dokumentasjon for nøyaktig omfang
  og verifikasjonsstatus.
- **International Criminal Court** — `lib/international-criminal-court/` og
  `INTERNATIONAL.CRIMINAL.COURTS.md`: beslektet domenespesifikk kildekode og
  dokumentasjon.

### O–S

- **OFDMA** — `ofdma/`: teknisk materiale om ortogonal frekvensdelt
  flertilgang og C/C++-utviklingsressurser.
- **Philosophy og modulen Conversation Mask** — [`philosophy/`](../../philosophy/README.md)
  inneholder de native C/C++-grunnmurene, og [`philosophy/module/`](../../philosophy/module/README.md)
  organiserer det neste modullaget deres i tre eksplisitte områder:
  [`data/`](../../philosophy/module/data/README.md) for typede, daterte observasjoner,
  hendelsesavslutninger, halveringstidsinnstillinger, proveniens og samtykke; [`helper/`](../../philosophy/module/helper/README.md)
  for deterministisk validering, relevansforfall, kompatible sammenligninger og
  samtykkefiltrert presentasjon; og [`psychiatry-friend/`](../../philosophy/module/psychiatry-friend/README.md)
  for et støttende, ikke-klinisk samtalegrensesnitt. De SLeeLa-vendte
  håndtakene og det sammensatte Tree ligger i [`lib/philosophy/`](../../lib/philosophy/README.md),
  ved siden av den bredere filosofipakken. Modulen er eksplisitt-inndata og
  samtykke-først: den overvåker ikke mennesker eller medier passivt, stiller ikke diagnoser, utleder ikke
  privat hensikt, skårer ikke menneskeverd og anbefaler ikke endringer i medisinering. Dens
  hendelseslenker bevarer proveniens og navigasjon, ikke uavhengig bekreftelse.
- **Synchro** — native integrasjon i kjøretidsmiljø/kompilator samt pakkedistribusjons-
  og målingsrelatert funksjonalitet; se den relevante `impl/`-kilden og
  kompilatorimplementasjonen.
- **SleelaUI / terminal-UI** — `lib/user-interface/` og prosjektet
  `sleela-terminal`: SLeeLa UI-integrasjon og terminalorienterte
  interaksjonskomponenter.

### U–W

- **United Nations** — `united-nations/`: domenespesifikt depotmateriale.
- **Wi-Fi 4, Wi-Fi 5 og Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`,
  `wi-fi-6/` og tilhørende `lib/wifi/`-kildekataloger: generasjonsspesifikt
  materiale og konfigurasjonsressurser for trådløse nettverk.

### Vedlikehold av pakkeindeksen

Når en pakke legges til eller utvides vesentlig, legg til oppføringen her i den
riktige alfabetiske gruppen, lenk til dens kanoniske katalog og README,
og beskriv bare funksjonalitet som støttes av de innsjekkede kildene. Hold denne
indeksen additiv: ikke fjern eller skriv om de eksisterende README-delene som
ledd i oppdateringer av pakkeindeksen.


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)


bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

US Democratic and US Policy.

---

## XVI. Nasjonalt vitenskapelig merkevarebildemateriale

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. Eksperimentell UTF-4088-katalog med en million oppføringer

[UTF-4088-kataloggeneratoren](../../utf-4088/tools/generate_catalogue.py) bygger deterministisk en eksperimentell symbolkatalog med 1 048 576 oppføringer. Det dokumenterte kompakte formatet er beskrevet i [utf-4088/FORMAT.md](../../utf-4088/FORMAT.md); kontrakten for generering og validering finnes i [utf-4088/GENERATION-RESULTS.md](../../utf-4088/GENERATION-RESULTS.md).

Kjør generatoren med Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Den genererte binærfilen er 16 777 244 byte og er utformet for å holde seg under 20 000 000 byte. Enhetstester og en GitHub Actions-arbeidsflyt verifiserer antall oppføringer, glyfenes unikhet, størrelse og SHA-256; vellykkede kjøringer publiserer binærfilen og sammendraget som arbeidsflytartefakten `utf4088-million-catalogue`. Den genererte binærfilen holdes bevisst utenfor Git-historikken. UTF-4088 er fortsatt eksperimentell og er ikke Unicode-kompatibel.

---

# SLeeLa
<p align="center"><img src="../../images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Nyere tillegg — Pi-konstant, oppstart av Modium/Registry, sleelactl Math og `@native`

Denne delen registrerer funksjoner som ble lagt til under arbeidet i 2026-11, de nyeste gruppene først. De lenkede kildefilene og deres valideringsstatus er autoritative; endringer under gjennomgang beskrives ikke som flettet før de når `master`.

### Pi-katalogkonstanten med 100 000 000 sifre

Depotet leverer en verifisert verdi av π med **hundre millioner sifre** i [`pi-generator/pi.100000000.digits`](../../pi-generator/pi.100000000.digits) (et velformet desimaltall, `3.14159…`), ved siden av en verdi med 57 344 sifre i [`pi-generator/pi.57344.digits`](../../pi-generator/pi.57344.digits). Begge er uavhengig verifisert som ekte π — se [`pi-generator/VERIFIED.md`](../../pi-generator/VERIFIED.md), som dokumenterer kontrollene (lærebokprefikset, Feynman-punktet ved brøkposisjon 762, det berømte en-millionte sifferet, samsvar med `lib/math/SLPiReference.sleela` og en frekvensanalyse av sifrene i hele filen) og et reproduksjonsskript som kjøres i ett trinn.

### Modium™ / Registry™ laster Pi-konstanten ved oppstart av delsystemet

På de dedikerte delsystem-inngangspunktene — `sleela registry` og `sleela modium` (og første kjøring av installasjonsprogrammet), **ikke** ved hver `sleela run` — laster delsystemene Modium og Registry den store π-katalogkonstanten. På en interaktiv terminal skriver de ut en forbigående rød linje `Loading Modium™` / `Loading Registry™` som deretter slettes på stedet før normal utdata begynner; effekten er strengt TTY-styrt, slik at utdata som sendes gjennom rør og testutdata forblir byte for byte uendret. Lokatoren (ikke bytene) legges inn som registerets `Math`-nøkkelstandarder `piReferenceLargePath` og `piReferenceLargeDigits`, både i SLeeLa-modellen ([`lib/registry/SLRegistry.sleela`](../../lib/registry/SLRegistry.sleela)) og i den native seederen ([`registry/sleela_registry.c`](../../registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` eksponerer `largePath()` / `largeDigits()`. π-konstanten installeres i OS-ets SLeeLa-plassering (`<data>/pi/`) av **både** standard `make`-installasjonsprogram og det veiledede «vakre» installasjonsprogrammet. Detaljer: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](../../pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — det vakre installasjonsprogrammet og et native-støttet `math`-subjekt

- Det veiledede («vakre») installasjonsprogrammet installerer nå `sleelactl` (SLeeLa-kontrollplanet) og dets modulregister, tilsvarende `make`-installasjonsprogrammet. Se [`bin/SLEELACTL.md`](../../bin/SLEELACTL.md).
- `sleelactl` fikk en ny **`subject`**-natur for de native C/C++-subjektbibliotekene som er kompilert inn i kjøretidsmiljøet, og registrerer det første av dem: **`math`**. `sleelactl run math` kjører den native `__NativeMath`-backenden (`impl/subjects/math/math_native.cpp`) fra ende til annen via subjektets referansedemo; `sleelactl info math` forklarer hvordan den nås (`import math;`). Subjektnaturen tilbyr bare `run info` — det finnes bevisst ikke noe `check`-verb, fordi `sleela check` ikke laster de SHEET-registrerte native subjektene.

### `@native`, Native Drive og kjøring på flere språk

En ny metodeannotasjon **`@native`** ber om forhåndskompilering (ahead-of-time) til native kode for en hot metode. Den nye kommandoen `sleela aot` finner `@native`-metoder, verifiserer at hver ligger innenfor en støttet *avlastbar delmengde*, og sender ut C/C++ — og **feiler lukket** (avviser, med en presis begrunnelse) ved enhver konstruksjon den ennå ikke kan oversette, slik at en halvveis oversatt metode aldri produseres.

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

`@native` er fortsatt en forespørsel om AOT-behandling, der konstruksjoner som ikke støttes avvises før utsendelse. [Native Drive](../../impl/native-drive/README.md) støtter bygg av kjørbare filer og delte biblioteker i flere enheter i C/C++ gjennom vertskompilatorer (inkludert MSVC), støttet AOT-utdata fra SLeeLa, symbolinspeksjon mot ABI-et til den native lasteren, prosesskjøring uten shell, håndtering av stdin/stdout/stderr, flertrinns rørføring og XClass-delegering til produksjonslasteren. `sleelactl` eksponerer sine bygge-/kjørekommandoer og ruter XClass-inspeksjon og eksplisitt gjengivelse/installasjon av systemd-enheter gjennom `inspect native-drive`. VM-en eier en avgrenset håndtakstabell for native moduler med obligatorisk vertsautorisasjon og generasjonskontrollerte håndtak. Syntaks 3.0 legger nå til `nativeOpen`, `nativeCall` og `nativeClose`: append-only bytekodeforsendelse krysser et separat versjonert null/bool/int64/double/UTF-8-ABI, mens VM-eide tabeller, strukturer og andre sammensatte verdier blir værende inne i tolken og kan bruke bytekode-reserve med samme navn. Plugins kan valgfritt motta en versjonert vertstilbakekallstabell for å påkalle eksakte SLeeLa-funksjoner gjennom nestet VM-kjøring, med GC-rooting av suspendert stakk, en eksplisitt tillatelsesliste over vertstilbakekallsnavn og avvisning av rekursive native API-kall. Native kode forblir pålitelig kode i samme prosess, ikke en sandkasse. Annotasjoner for kildeprosesspolicy har et versjonert skjema, men blir foreløpig ikke parset/håndhevet. Detaljer, sikkerhetsgrenser og hovedboken over de sju arbeidsstrømmene: [Native Drive](../../impl/native-drive/README.md) og [`pi-generator/AT-NATIVE.md`](../../pi-generator/AT-NATIVE.md).


## XIX. Internasjonale README-utgaver

SLeeLa tilbyr landsspesifikke README-inngangspunkter for de 50 markedene i sin internasjonale utgave. Hvert land har én fil med navnet `README.<CC>.md`; der flere språk er relevante, er de gruppert i det aktuelle landets fil.


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

**Oversettelsespolicy:** Disse filene er innledningsvis lokaliserte landsinngangspunkter. Introduksjonene og språkoverskriftene er lokalisert, mens den fullstendige tekniske referansen forblir i den kanoniske engelske [README.md](../../README.md). En landsfil må ikke beskrives som en fullstendig oversettelse før hver del er oversatt, tekniske identifikatorer og lenker er kontrollert, og utgaven har gjennomgått språkgjennomgang. Språkdekningen i denne første omgangen er representativ og er ikke en påstand om at alle konstitusjonelt anerkjente, regionale, urfolks- eller tegnspråk er fullstendig oversatt.

## Appendiks — bevart regional veiledning

Dette appendikset bevarer praktisk veiledning fra den tidligere korte norske veiledningen som ikke finnes som egne deler i den kanoniske README-en. Den kanoniske engelske [README.md](../../README.md) har forrang ved enhver uoverensstemmelse.

### Norsk språkform

Norsk bokmål brukes i denne utgaven. Den er ikke ment å hevde at nynorsk, samiske språk eller norsk tegnspråk er fullstendig dekket. Bidrag til flere norske språkformer og tilgjengelige formater er velkomne.

### Mål, profiler og grenser for kjøring

SLeeLa skiller kildekode fra bygge- og kjøringsmål. Den dokumenterte målfamilien omfatter `SLVM`, `SLJVM` og `SLIR`; `NATIVE` er et avgrenset mål med vilkår og må ikke antas å være tilgjengelig på alle plattformer. Profilene `BASIC_COMPLETE` og `ADVANCED_TOTAL` angir ulikt omfang. Kontroller faktisk mål, profil og plattformstøtte for hver bygging.

En referanse identifiserer en fil, modul eller ressurs, men gir ikke i seg selv tillatelse til å kjøre den. Når et program bruker filer, eksterne biblioteker, native bindings eller systemressurser, må autorisasjonsgrensene og ansvaret for kjøringen defineres tydelig. En beskrivelse i dokumentasjonen er ikke alene bevis for en sikkerhetsgaranti i runtime; kontroller implementasjonen og relevante tester.

### Ressurslivssyklus og feilsøking

Dokumenter eierskap, levetid og synkroniseringsansvar for ressurser som deles mellom funksjoner eller tråder. Skill mellom local variables, shared object state, VM handles og serializable data. Et VM-local handle er ikke et stabilt filformat eller en nettverksmelding. Bruk modulens definerte serialiseringsgrensesnitt og test invalid, missing og boundary values.

Ved feilsøking registrerer du commit, OS, architecture og toolchain. Bygg først det minste relevante målet, ta vare på hele feilutskriften, skill compiler errors fra runtime errors og lag et minimalt eksempel som kan gjenskapes. En vellykket bygging beviser ikke alene at runtime-oppførselen er korrekt.

En verifikasjonsrapport bør oppgi exact commit, OS/architecture, compiler versions, fullstendige build/test commands, antall pass/fail/skip og kjente begrensninger. Tester som er hoppet over eller ikke kjørt, skal ikke regnes som bestått.

### Merknader om henvisninger og adresser

Henvisninger i README-en til emner som CSPML, Character, Governance, Synchro, SleelaUI og United Nations beskriver innholdet i depotet og er ikke offisielle godkjenninger fra myndigheter eller institusjoner.

Tilstedeværelsen av adresser eller bilder i delen **Bitcoin Conjegeum** (XV) er ikke en finansiell anbefaling, en bekreftelse av eierskap eller en instruksjon om å overføre midler. Kontroller opplysningene uavhengig før bruk.

**Oversettelsesstatus:** Del I–XIX er oversatt til norsk bokmål. Tekniske identifikatorer, kommandoer, kodeeksempler, filnavn, lenker og adresser er beholdt. En avsluttende språkgjennomgang av morsmålsbrukere anbefales.
