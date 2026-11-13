# SLeeLa 3.0 — Sverige (SE)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Ett inbyggt systemspråk, en kompilator, en körmiljö och ett ekosystem av virtuella maskiner.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Internationell landsutgåva.** Den här utgåvan är på svenska. Tekniska identifierare, kommandon, kodexempel, filnamn, sökvägar och länkar är oförändrade jämfört med den kanoniska engelska [README.md](README.md).

**The Now. The Ingredient.** Ett språk är mer än syntax: det är kompilatorn, körmiljön, biblioteken, verktygen och plattformskontrakten som gör programvara användbar.

**Vad SLeeLa arbetar med.** SLeeLa arbetar med **Math**, **Object**,
**Science** och **Reach** — de fyra sidorna av dess verksamhet: det
beräkningsbara (Math), den fastställda sak som står i förteckningen över det
som finns (Object), det disciplinerade studiet av det sanna (Science) och
utsträckandet av ett systems hand till andra system och människor (Reach).
**Nationella intressen kvarstår** — de sätts inte åt sidan av någon av de fyra;
när ett ämne berör en nations intresse bevaras den ställningen (och
dokumenteras troget, t.ex. i varje klass Metadata `metaOrigin` /
`metaNationalSchedule`), den upplöses aldrig.

## I. Kom igång
- [Bygg den inbyggda implementationen](impl/README.md)
- [Virtuella maskiner och Modium — praktisk användarhandbok](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [CPU-stack i SLeeLa-källkod](lib/cpu/CPU.md) — en komplett maskin från grindar→ALU→kärnor→operativsystem→program, en förteckning över 130+ CPU-profiler samt justerbara [spänningar, variabla klockfrekvenser och expertinställda timingar](lib/cpu/TIMING.md) som du kan [köra enligt specifikation och överklocka](lib/cpu/components/README.md)
- [Dokumentationsindex](docs/README.md)
- [CSPML-datakälla och forskningsreferens för Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — ursprung och integrationsvägledning för externa amerikanska data om orter, socioekonomi, arbetskraft, utbildning och offentliga skolor.
- [Genererad API-dokumentation](docs/api/README.md) — aktuell HTML-referens, versionsindelad API-utgåva och instruktioner för att generera om den
- [API-katalog](api/README.md)
- [Standarder för beräkningsjämförelser](comparison/README.md) — standarder för typer, minne, ABI och överensstämmelse mellan SLeeLa VM, Java/JVM och inbyggd C/C++.
- [Recursion stanchion — C- och C++-prototyper](recursion/stanchion/README.md) — primitivspecialiserade exempel för Boolean, Byte, Character, Integer, Long, Float och Double, vid sidan av generiska, tillämpade, vetenskapliga och pedagogiska rekursionsmönster.
- [Plattformsstöd och valideringsmatris](docs/PLATFORM-SUPPORT.md)
- [Varumärkesguide](docs/BRAND-GUIDE.md)
- [Varukoncept och material](merchandise/README.md)
- [NitroWebExpress-logotyp](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Testpolicy och krav för utgåvor](markdown/TESTING.md)

## II. Nyheter

Senaste tillägg till språket, drivrutinerna och standardbiblioteket:

- **Standardbibliotekspaketet `advantage` — att ”advantaga” en utdelning,
  med matematik, fysik, integration, gradering och routbar utdata.**
  [`lib/advantage/`](lib/advantage/README.md) kapslar in *att advantaga en
  unik utdelning i förhållande till en modell* som en lagerindelad **Sequitur-lök**
  (utdelning → granskning → farhågor → matematik → tillägg → subtraktion →
  omgruppering → iteration), som kan navigeras inåt/utåt med fullständig dynamisk
  uppdatering och återställning, och som exponeras genom en enkel
  **användarmodell** och en **utvecklarmodell** med full kontroll. Paketet har
  fem ämnesområden: **`math/`** bilateral handelsekonomi (gemensam
  vinstmaximering, Nash-förhandling, den bilaterala newsvendor-kritiska kvoten),
  **`physics/`** en strukturell ekonomisk ”fysik för förtroendefält”
  (förtroende som metrisk tensor, den transaktionella geodeten, en
  kontaktstruktur för tillkännagivanden, en SDE av typen CIR+hopp för
  förtroendeförfall — en modellanalogi, uttryckligen markerad som sådan),
  **`integration/`** en rekursivt sammansättbar modell för vertikal integration
  inom kapitalism/detaljhandel (fack, ingenjörsfack, provfack, newton-medianfack
  och ett mått på systemets sofistikering), **`grading/`** en
  sofistikeringskrets-gradering som betygsätter varje modul och valfri
  uppsättning vinnande/förlorande/graderande/absoluta moduler A–D, samt
  **`output/`** en routbar emitteringssöm som skickar modulens utdata till
  terminalen, en **CommonRails**-formaterad yta eller en socket/kontakt (inramad
  för en inbyggd `slnet_write`). Varje klass bär Metadata-Class-posten; fem
  körbara demonstrationer validerar ämnena, och de medföljande dokumenten
  [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md),
  [`MATH.md`](lib/advantage/MATH.md) och
  [`PHYSICS.md`](lib/advantage/PHYSICS.md) beskriver dem. En inbyggd C/C++-konsument
  under [`advantage/`](advantage/README.md) (C-kärnan
  `advantage_value`/`advantage_mean`, ett XML-kunskapslager och ett säkert
  bibliotek för HTTPS/SFTP/SSH-inhämtning + nyckellager + DH/ECDH-transport)
  länkar in modellen i genomarbetade
  [handelsexempel](advantage/examples/README.md) — positiva affärer från
  förmodade internetkällor, en målgruppsrelevant sekvens (Traders,
  Consumers, Helpers, Healers) och åtta modeller rangordnade A–D (D = mest
  Cement, A = mest Brand).
- **Loop-timingannoteringar — `@loopDelay` / `@loopBudget` (+ den inbyggda
  funktionen `sleep(ms)`).** De första **satsnivå-, beteendebaserade**
  annoteringarna i SLeeLa: skrivna på en egen rad före en `for`-, `while`- eller
  `do`-loop infogar de en verklig paus i loopens exekvering.
  `@loopDelay(ms = 250)` / `@loopDelay(seconds = 2)` sover en fast tid per
  iteration (ett blott `@loopDelay(250)` använder millisekunder som standard);
  `@loopBudget(seconds = 10, iterations = 1000)` sprider ett känt antal
  iterationer jämnt över en total tidsbudget i väggklocktid. Båda sänks till den
  nya append-only-opkoden **`OP_SLEEP`** (ISA 157), som även ligger bakom den
  allmänna inbyggda funktionen **`sleep(<ms>)`** som kan anropas var som helst;
  pausen är implementerad portabelt (`nanosleep` / `Sleep`). Specificerad i
  [`SLEELA.syntax` §32](SLEELA.syntax); API-referens i
  [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md);
  körbart exempel:
  [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela).
  Ett index över sessionens tillägg (inklusive detta, den inbyggda
  storhetsmultiplikationen `OP_BIGMUL` för stora heltal samt de fristående
  verktygen **[local-generator](local-generator/README.md)**
  och **[pi-generator](pi-generator/README.md)**) finns i
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md).
  De tre genererade API-ytorna
  ([`sleeladoc-api/`](sleeladoc-api/README.md),
  [`docs/api/`](docs/api/README.md) och den oföränderliga
  ögonblicksbilden [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html))
  byggdes om med `make api-docs` och innehåller nu de nya instruktionsposterna
  `SLISASleep` / `SLISABigMul` på sin `vm`-paketsida.
- **SleelaDoc från kommandoraden — `sleela docs`.** SleelaDoc kan nu nås direkt
  från terminalen. `sleela docs` listar alla dokumenterbara ämnen;
  `sleela docs crypto` visar ett paket; `sleela docs stringbuilder` slår upp en
  klass (oberoende av gemener/versaler, och via standardbibliotekets prefix `SL` →
  `SLStringBuilder`) och skriver ut en rik sida — **definition, designsyfte,
  relationer, struktur, användar-clays och en mästerskapsstege i fem steg** — med
  rangordnade ”menade du”-förslag vid nära missar. Författare berikar vilken sida
  som helst genom att lägga till SleelaDoc-`@`-taggar (`@intent`, `@thesis`,
  `@axiom`, `@param`, `@see`, `@mastery`). `sleela docs --generate [dir]` skriver
  en textkorpus (en sida per klass + en `TOPICS.index`) som kommandoraden sedan
  läser offline; **`make sleeladoc`** paketerar den korpusen **och motsvarande
  `.sleela`-källor** i `sleeladoc/build/sleeladoc.jar`. För ämnen som enbart
  har fasad (t.ex. `psychiatry`, `politico`) söker `sleela docs --facade <topic>`
  igenom filsystemet vid körning och listar de verkliga **mapparna (därefter
  filerna, som naturlig rörelse)**, begränsat av användaröverstyrbara tak på
  **6 MB / 6000 filer / 6 minuter**. Enheterna i `lib/sleeladoc` underhålls i den
  aktuella utgåvan, **`#sleela 3.0`**. Se [`man/man1/sleela.1`](man/man1/sleela.1) och
  [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md).
- **SleelaDoc — en dokumentationslösning i Javadoc-stil, med en genererad
  HTML-webbplats.** **SleelaDoc** är SLeeLas dokumentkommentarskonvention i
  källkoden (vinkelparenteskommentarer med sentinellerna `@doc` / `@page`; se
  [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)), och
  [`lib/sleeladoc`](lib/sleeladoc/README.md) är **extraktorn** som återger en
  `.sleela`-enhet som en Markdown- eller HTML-API-sida — på samma sätt som
  `javadoc` återger Java, och dokumenterar *avsikt*. En komplett, bläddringsbar
  **HTML-dokumentationswebbplats för hela `/lib`** genereras i
  [`/sleeladoc-api`](sleeladoc-api/README.md) av
  `tools/generate-sleeladoc-api.py` (öppna `sleeladoc-api/index.html`; en ren
  stil i blått / ljusblått / ljusgrått / vitt med hierarkin paket → klass →
  medlem och en ordlista skriven för läsaren *Student 2*). Kompilatorn
  producerar också författarmarkerade dokumentutdata som en biprodukt av
  kompileringen (`--wiki` MediaWiki-sida + en ursprungsmärkt källkodstvilling),
  dokumenterat i [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md).
  Körbart: [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela).
- **XML Document Object Model (`lib/xml`).** En XML-DOM på SLeeLa-sidan — ett
  dokument som äger ett träd av element-/text-/kommentars-/CDATA-/PI-noder med
  ordnade barn och namngivna attribut, en serialiserare (kompakt/formaterad,
  escaping enligt XML 1.0 §2.4), en parser för teckenceller samt kvalitetsregistret
  `SLXmlAspect`. Nodlikhet är en trestegs förtroendetrappa (identitet → samma
  subjekt → struktur), reflexiv, symmetrisk och deterministisk. Se
  [`lib/xml/README.md`](lib/xml/README.md) och det körbara
  [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela).
- **Breda signerade heltalsprimitiver (`long`, `int64`, `int128`).** `int`, `long`
  och `int64` är alla 64-bitars signerade heltal som typkontrolleras och körs
  identiskt; `int128` accepteras som heltalstyp (tills vidare 64-bitarsbaserad,
  med äkta 128 bitar som dokumenterad uppföljning). Den signerade bas-2-serien
  `int32`..`int16777216` (syntax 2.7) och motsvarande osignerade serie
  `uint32`..`uint16777216` (syntax 2.8) utvidgar detta. Se
  [`SLEELA.syntax`](SLEELA.syntax) §21, §24, §25.
- **`@mix`-följdfält (syntax 2.9).** En klass kan deklarera flera
  instansfält med **samma namn** när varje upprepning bär `@mix`; språket tolkar
  gruppen som följdfält (en array / lista nycklad med index), uppifrån och ned
  `1,2,3,…` som standard eller manuellt via `@mix(1,2,3,...)` vars indexantal måste
  stämma med antalet fält. Se [`SLEELA.syntax`](SLEELA.syntax) §28.
- **Dokumentmarkörer — den naturliga processen, benämnda Preonems
  (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).**
  Dessa bara dokumentmarkörsord benämns tillsammans **Preonems**
  (en markör är en *Preonem*; mängden, *Preonems* — se
  [`SLEELA.syntax`](SLEELA.syntax) §26.1). Förstklassiga, enbart deklarativa
  dokumentmarkörer som anger vad ett dokument *är* i tillblivelsens ordning:
  `author`/`authorial` (syntax 2.4; hanteras av yrkesråd — den enda medlemmen med
  beteendeeffekt), `material;` (fastställd Källa) och `memory;`
  (syntax 2.5), `determine;` (syntax 2.6; ett dokument öppet för vidare
  bearbetning av en läkare, en fysiker eller ett spöke mot fullbordan genom laglig
  hand), samt de två nyaste, **`object;`** (en fastställd sak som står i
  förteckningen över det som finns — **förverkligad, given konkret ställning, i
  syntax 2.8**) och **`field;`** (den relationsgrund i vilken objekt står och
  rör sig) — båda spärrade från syntax 2.6. Var och en är bar, kontextuell och
  versionsspärrad av frontend utan påverkan på parsning, typning eller
  kodgenerering; utanför markörpositionen förblir var och en en vanlig
  identifierare (så `field` stör aldrig en klassmedlem eller en `.field`-åtkomst).
  Se [`SLEELA.syntax`](SLEELA.syntax) §18–§20, §23 och §26.
- **Nordshrift vid syntax 3.0 med full biblioteksräckvidd.** Nordshrift löser upp
  namn genom samma rekursiva `/lib`-index som kompilatorn och når varje paket och
  varje `.sleela`-enhet; `nordshrift check` rapporterar antalen för hela trädet.
- **Recursion stanchion.** Katalogen [`recursion/stanchion/`](recursion/stanchion/README.md) innehåller nu dedikerade C- och C++-referensprototyper för de sju dokumenterade primitivkontrakten, samt generiska, tillämpade, vetenskapliga och pedagogiska exempel. Det här är referenskällor; kompilera och testa dem innan de betraktas som verifierade.

- **Vinkelparenteskommentarer (`<` / `<<`).** En ny kommentarstil för framtiden —
  `< rad >` och `<< block >>` — lexas av SLeeLas frontend och dokumenteras i
  [`SLEELA.syntax`](SLEELA.syntax) §1.1.
- **Radixheltalsliteraler.** `int`-literaler accepterar nu hexadecimal (`0x`),
  binär (`0b`) och oktal (`0o`) form med `_` som sifferavgränsare, både i
  SLeeLas källspråk ([`SLEELA.syntax`](SLEELA.syntax) §1.4) och i
  [CPU-modellformatet `.xlt`](lib/xlt/README.md).
- **Standarden Metadata Class i hela biblioteket.** Det beskrivande
  metadatamönstret (ursprung, laglighet, jurisdiktionellt omfång) bärs nu av
  varje klassbärande `.sleela`-fil under `/lib`; se
  [`SLEELA.syntax`](SLEELA.syntax) §15 och
  [handledningslektionen](tutorial/13-metadata-class.md).
- **En rikare text-/String-familj.** `lib/text` får verkliga hjälpfunktioner i
  språket, en teckencellsvy som ger äkta `charAt`/`substring`/`indexOf`/`reverse`/
  kolumnjustering samt en verklig positionsbaserad mallhanterare — se
  [handledningen om text och String](tutorial/14-text-and-strings.md) och det
  körbara [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela).
- **Förstklassig `String` i Nordshrift-blad.** Ett `.sst`-blad kan deklarera ett
  typat `strings:`-block som bidrar till byggets komponentmanifest;
  se [Nordshrift Sheet Reference](wiki/Nordshrift-Sheet-Reference.md).
- **CPU-modeller körs både via opkoder och via funktionsanropsekvivalens.** Varje
  `SL<ARCH>CPU` ärver grunden att köra en inbyggd instruktionsström genom sin
  hämta-avkoda-exekvera-loop *och* att köra koden för en förmodad annan process
  genom ekvivalenta anrop på det delade VM-substratet — se
  [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5.

## III. Bygg och testa

**Förutsättningar:** en C/C++-verktygskedja (GCC eller Clang) och GNU Make; Python 3 för
manifest- och flödesverktygen; på Linux utvecklingspaketen för X11/Xrandr för
displayprodukterna (en moduls `make deps` rapporterar/installerar det den behöver). Bygget av
`impl/` är feltätt (fail-closed) mot ett betrott SHA-256-manifest, som rot-
`Makefile` pekar ut automatiskt.

Från en ren utcheckning, den snabbaste starten:

```sh
make                                  # build everything from the repo root
```

Eller bygg och testa bara den inbyggda kärnan:

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

`make` i repositoryts rot bygger varje produkt (kärna, VM, kodekar, churn,
antivirus, …) och avslutar med att bygga **registret** och materialisera dess
baslinjetabell — så ett byggt träd levereras med ett granskningsbart register direkt.
De viktigaste målen:

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

Andra användbara mål:

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

Målet `registry` kör [`build-registry.sh`](build-registry.sh): det bygger
registerkärnan, sår baslinjetabellen i det portabla textlagret
`registry/build/sleela.sleelareg` och exporterar MySQL-tabellskriptet
`registry/build/registry.sql` (via `sleela registry export`). Granska det med
`sleela registry` eller `registry/build/sleela-registry review`. Se
[registerguiden](registry/README.md) och [Registry 3.0-arkitekturen](registry/REGISTRY-3.0-ARCHITECTURE.md).

Läs bygg- och testguiderna för förutsättningar och plattformsspecifika steg. Att en komponent byggs utan fel bevisar inte att alla bibliotek eller plattformar är redo för release.

## IV. Installation

Efter att de inbyggda produkterna har byggts, installera kommandona och körmiljöns stödfiler:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

Det konventionella installationsprogrammet använder som standard `/usr/local/bin`, `/usr/local/lib/sleela`
och `/usr/local/share/sleela`. Det bevarar en befintlig startkonfiguration och
vägrar skriva över ospårade befintliga kommandon. Avinstallation tar bara bort
filer som installationsprogrammet har registrerat och bevarar konfiguration och
ospårade körningsdata.

För en fokuserad, guidad installation av enbart huvudpaketet — binärerna `sleela`,
`sleelvac` och `nordshrift`, standardbiblioteket, protokollen och
stödmodulerna — använd det rena installationsprogrammet:

```sh
make clean-install       # guided install of the main package only
```

Det kräver ett enda huvudbeslut (installera huvudpaketet), därefter ett val av omfång,
och vid framgång en valfri väljare för utvalda förbättringsmoduler. För det
interaktiva installationsflödet och alla installationsalternativ, se
[Snabb och säker installation](install/README.md).

## V. Språk och körmiljö
Den auktoritativa grammatiken är [`SLEELA.syntax`](SLEELA.syntax). Repositoryt dokumenterar för närvarande syntax **3.0**; kompilator-/verktygsversion och källsyntaxversion är åtskilda begrepp. Ytan i 3.0 lägger till **dokumentmarkörerna `pure` och `gasoline`** (§30) — ett par bara, kontextuella, enbart deklarativa Preonems som betyder nästan samma sak och som var och en markerar dokumentets ställning genom sin egen ärlighet. Syntax 3.0 sammanför språkytan 1.0–2.9 till den aktuella normativa utgåvan och bevarar alla funktionsspecifika minimispärrar. Den föregående ytan 2.9 lägger till **`@mix`-följdfält** (§28) — en klass kan deklarera flera instansfält med samma namn när vart och ett bär `@mix`, förstådda som en indexerad grupp (uppifrån och ned `1,2,3,…` eller manuellt `@mix(1,2,3,...)` vars indexantal måste stämma med gruppen). Den för vidare de bas-2-baserade familjerna av signerade/osignerade breda heltal `int32`..`int16777216` (§24) och `uint32`..`uint16777216` (§25) ovanpå de tidigare `long`/`int64`/`int128` (§21); **dokument för nyckelordsstöd** (§22); samt **Preonems** — de enbart deklarativa dokumentmarkörerna `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) och `artifact` (§35, ställning av lagstiftande klass graderad i tid och rum), tillsammans benämnda Preonems (§26.1). Konstruktoröverlagringsupplösning och delegering med `this(...)`/basklasskonstruktor ligger uttryckligen utanför det verifierade syntax 1.8-kontraktet. Läs grammatiken och implementationsanteckningarna innan du förlitar dig på en funktion. SleelaDocs nio källenheter deklarerar nu `#sleela 3.0`; den gemensamma kompilatorspärren accepterar källsyntax 1.0–3.0. Det nya API-bygget produceras med `make api-docs`.

Repositoryt innehåller en inbyggd C/C++-implementation, referenser för SLVM-generering, standardbiblioteksmoduler, tester, exempel och plattformsintegrationer. Implementations- och valideringsstatus varierar mellan moduler; läs modulens specifikation och aktuella CI-resultat i stället för att anta enhetlig fullständighet.

## VI. Ingenjörsprinciper
- **Belägg framför påståenden:** skilj mellan implementerat, testat, experimentellt och planerat beteende.
- **Ett kontrakt per gräns:** dokumentera syntax, API:er, filformat, plattformsantaganden och felbeteende.
- **Portabelt genom design:** isolera plattformsspecifika bakändar och testa på inbyggda körmiljöer.
- **Säkert fel:** testa felaktig indata, resursgränser, processfel och städvägar.
- **Reproducerbara utgåvor:** bygg om rent och publicera artefakternas hashar och utgåveanteckningar.

## VII. Antivirusbygge och integritet

Antiviruspaketet Sleelavirin innehåller en inbyggd SHA-256-skanner, en valfri
ClamAV-adapter, integritetsmanifest för källkod/index och kontrollsummor för plattformspaket.
Bygg och testa paketet från repositoryts rot:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Integritetsmanifestet genereras under `antivirus/build/`; det omfattar spårade
käll- och dokumentationsindata samt skyddade signaturindexfiler, men
utesluter genererad utdata och den föränderliga ClamAV-cachen. ClamAV-databasens
ögonblicksbilder har egna manifest. Utgåvornas ZIP-filer innehåller `INTEGRITY-MANIFEST.json`,
`SHA256SUMS` och en `.sha256`-sidofil. Verifiera nedladdade arkivhashar mot
ett betrott värde som hämtats oberoende: en kontrollsumma som lagras bredvid ett arkiv är
inte en utgivarsignatur. Se [Sleelavirin-guiden](antivirus/README.md),
[säkerhetschecklistan](antivirus/SECURITY.md) och
[bygg-arbetsflödet för antivirus](.github/workflows/antivirus-binaries.yml).

En utgåva anses inte vara verifierad förrän de relevanta CI-jobben har godkänts på
exakt den utgåvans commit och de resulterande artefakterna har kontrollerats.


## VIII. SLeeLa-editorteman — Vim och Emacs

SLeeLa innehåller en samordnad uppsättning professionella mörka färgscheman för **Vim** och **Emacs**, samt ett inbyggt C/C++-palettverktyg. Det ursprungliga schemat **American Professional** förblir standard. Tio kompletterande paletter finns för olika kulturella och institutionella designinspirationer; dessa är tolkande designpaletter och inte officiella representationer av någon nation, kultur, domstol eller institution.

### De elva schemana och deras färger

| Schema | Bakgrund | Förgrund | Nyckelord | Typer | Strängar | Kommentarer |
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

#### Nationella flaggfärger (källpalett)

De grekiska och turkiska schemana är härledda från de två ländernas **officiella nationella flaggfärger**, som här visas med sina exakta publicerade hexvärden. Schemats bakgrunder och accenter ovan är mörkade, redigeringsvänliga tolkningar av dessa källfärger — tabellen nedan är referensen ”färger såsom de nationellt anges”.

| Nation | Färg | Hex | Grund och betydelse |
|---|---|---|---|
| **Grekland** | Blå | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | Nationalflaggans blå (lag 851/1978). Himlen och haven; heraldiskt blasonerad som *azur*. |
| **Grekland** | Vit | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Nationalflaggans vita. Självständighetskampens renhet; korset betecknar den östortodoxa kristendomen. |
| **Türkiye** | Röd (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | Nationalflaggans röda (turkiska flaggslagen). Fältet; enligt tradition martyrernas blod och den osmanska suveräniteten. |
| **Türkiye** | Vit | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Nationalflaggans vita. Halvmånen och stjärnan — fred, renhet och symbolen för det turkiska/islamiska arvet. |

Schemana förankrar varje redigerares förgrund i den nationella **vita** färgen och dess nyckelord, markör, markering och statusrad i exakt den nationella **blå** (`#0D5EAF`) eller **röda** (`#E30A17`) färgen, så att de färger som betyder något bokstavligen är nationalfärgerna. Dessa är tolkande designpaletter; flaggfärgsreferensen tillhandahålls för proveniens och är inte en officiell representation av någon av staterna. Färgbetydelserna följer beskrivningen av statssymboler från Hellenska republikens utrikesministerium och den turkiska flaggslagens tradition.


### Färgprovstabell

Palettcellerna nedan använder **SVG-prover**, så att GitHub återger de verkliga färgerna i stället för tomma HTML-färgceller. Varje remsa visar de sex färgerna i denna ordning: bakgrund, förgrund, nyckelord, typer, strängar och kommentarer. De exakta hexkoderna anges bredvid varje prov, i samma schemaordning som i referenstabellen ovan.

| Schema | Återgivna palettfärger | Hexkoder (bakgrund · förgrund · nyckelord · typer · strängar · kommentarer) |
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

Ytterligare accentfärger används för funktioner, konstanter, markeringar, diagnostik och preprocessordirektiv. Den ryska och slaviska paletten använder avsiktligt skiffer, blått, salvia, rubin och antikt guld med medelstark intensitet för ett lugnare professionellt intryck.

### Installera Vim-schemana

Från repositoryts rot:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Lägg till följande i `~/.vimrc` för att aktivera SLeeLas syntaxmarkering och välja ett schema:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Välj exakt ett av dessa färgschemanamn:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Använd till exempel `colorscheme sleela-russian-slavic` för den medelstarka ryska och slaviska paletten, eller `colorscheme sleela-greek` / `colorscheme sleela-turkish` för de hellenska och Al Bayrak-paletterna. Starta om Vim eller läs in konfigurationen på nytt med `:source $MYVIMRC`.

### Installera Emacs-schemana

Skapa en lokal temakatalog och kopiera lägesfilen och temafilerna från repositoryts rot:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Lägg till detta i `~/.emacs` eller `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Ersätt `sleela-american` med `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` eller `sleela-sleela` för att välja ett annat schema. Om Emacs frågar om temat ska betros, granska den lokala temafilen innan du godkänner det.

### Förhandsgranska de inbyggda C/C++-paletterna

Det lilla inbyggda palettverktyget kan lista scheman, skriva ut deras terminalfärger och slå upp en enskild färg:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Av bakåtkompatibilitetsskäl väljer `--palette` och `--hex COLOR` utan `--theme` American Professional. Bygg- och testresultat bör kontrolleras på målsystemet och i CI; dokumentationen av ett kommando är i sig inget belägg för att det har godkänts.

Se [guiden för editorintegration](linux-editors/README.md) för fullständiga palettbeskrivningar och verktygsdetaljer.

## IX. Bidra
1. Läs dokumentationsindexet och testpolicyn.
2. Gör fokuserade ändringar med positiva och negativa tester.
3. Kör riktade tester och därefter hela sviten.
4. Rapportera uppgifter om operativsystem och verktygskedja; beskriv inte överhoppade kontroller som godkända.

SLeeLa utvecklas av **MEARVK LLC**. Historiskt startsidesmaterial bevaras i [`docs/README-LEGACY.md`](docs/README-LEGACY.md).

## X. AE6E66 — Förväntan om skattepålaga på Round Public-e-post

För initiativet AE6E66 är projektets uttalade förväntan **$300,000,000.00 USD per Round Public-e-post** som ett föreslaget skattepålagebelopp kopplat till varje kvalificerande Round Public-e-post.

Detta uttalande dokumenterar enbart en projektförväntan. Det är **inte** en juridisk skatteberäkning, myndighetsbeslut, domstolsavgörande eller en självständigt fastställd skatteskyldighet. En faktisk pålaga skulle kräva behörig rättslig myndighet, jurisdiktion, definitioner, förfarande och ett beslut av behörig statlig instans.


## XI. Alfabet och teckenuppsättningens körmiljö

SLeeLas alfabetsdokumentation och inbyggda teckenkörmiljö underhålls i alphabet/sleela/ och lib/alphabet/native/. De inbyggda C++17-testerna täcker strikt avkodning/kodning av UTF-8-skalärvärden, kapacitetssignaturer av typen C<n> och begränsat medlemskap i teckenuppsättningar:

    make -C lib/alphabet/native test

Den experimentella UTF-4088-katalogen och dess SLeeLa-uppslagsadapter dokumenteras i utf-4088/SLeeLa-ALPHABET-INTEGRATION.md. UTF-4088-ID:n är en separat namnrymd och inte Unicode-skalärvärden. Arbetet med kompilator- och SLVM-bindningar spåras uttryckligen separat från dessa inbyggda grunder.

## XII. AutoCAD Creator™

[AutoCAD Creator™](autocad/creator/README.md) är ett interaktivt 2D-CAD-program
skrivet helt i SLeeLa. Där systerprogrammet AutoCAD Renderer gör om en plan till
en ritning i ett enda steg låter Creator dig **skapa och ändra** ett 2D-
AutoCAD-dokument direkt — lägga till väggar, rum, måttsättningar och anteckningar, därefter flytta,
byta namn på, byta lager för, ta bort och ångra det du placerat — och **spara** resultatet som en
riktig AutoCAD DXF-ritning (R12-text, öppnas i AutoCAD / LibreCAD).

Dess grafiska gränssnitt bygger på **SleelaUI™**, SLeeLas egna plattformsoberoende
gränssnittsverktygslåda (under [`lib/user-interface`](lib/user-interface)) — inte JavaFX — som
öppnar ett riktigt inbyggt fönster på X11 / Cocoa / Win32. Designen följer en enda regel,
**”SleelaUI presenterar; SLeeLa avgör”**: gränssnittet beräknar aldrig geometri och skriver aldrig
DXF. Varje verktyg utfärdar en avsikt att skapa / ändra / ta bort / ångra / spara till den
auktoritativa [`SLCadCreator`](lib/autocad/SLCadCreator.sleela), som äger det
redigerbara dokumentet och en reversibel kommandohistorik, medan ritytan bara förhandsvisar
den geometri som Creator håller.

Den återanvändbara redigeringsmotorn finns i [`lib/autocad`](lib/autocad) —
[`SLCadEntity`](lib/autocad/SLCadEntity.sleela),
[`SLCadDocument`](lib/autocad/SLCadDocument.sleela),
[`SLCadCommand`](lib/autocad/SLCadCommand.sleela) och
[`SLCadCreator`](lib/autocad/SLCadCreator.sleela) — och körs från början till slut utan skärm.
Gränssnittsskalet och demonstrationen finns under [`autocad/creator`](autocad/creator):

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

På en bildskärm öppnas det levande redigeringsfönstret; på en värd utan skärm byggs
ritningen i minnet, en rapport plus DXF skrivs ut och programmet avslutas rent.


## XIII. Ordning för UTF-4088-inmatningskartan

Den specialiserade UTF-4088-katalogen definierar sin rumsliga genomgång av inmatningskartan och abstrakta tryckiterationer i [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). Policyn för ämnesindex i National Lien finns i [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Kartkoordinater, trycknivåer (1–50), permanenta katalog-ID:n och alfabetiska rangordningar är oberoende fält; inget av dem får i tysthet ersättas med ett annat. Tryck är en algoritmisk iterationsnivå, inte en fysisk storhet.


## XIV. Senaste och utökade paketindex

Det här tillagda indexet lyfter fram nyligen utökade paket, domänbibliotek och
källsamlingar. Posterna är grupperade alfabetiskt efter paket- eller ämnesnamn.
Beskrivningarna anger var i repositoryt något finns; implementations- och valideringsstatus
bör kontrolleras i respektive pakets egen dokumentation och tester.

### A–C

- **Advantage** — `lib/advantage/` och `advantage/`: att advantaga en unik
  utdelning i förhållande till en modell som en lagerindelad Sequitur-lök, med fem
  ämnesundermappar — `math/` (gemensam vinstmaximering, Nash-förhandling,
  bilateral newsvendor), `physics/` (förtroendefältets metrik/geodet/kontakt/SDE,
  en markerad modellanalogi), `integration/` (fack för vertikal integration
  och ett sofistikeringsmått), `grading/` (en A–D-gradering av sofistikeringskretsar
  för moduler och uppsättningar) och `output/` (terminal / CommonRails /
  socket-emittering). En inbyggd C/C++-konsument (`advantage/`) länkar in modellen i
  genomarbetade handelsexempel. Se [`lib/advantage/README.md`](lib/advantage/README.md)
  och [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alfabet och UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` och
  `utf-4088/`: alfabetskataloger, inbyggt stöd för teckenuppsättningar, ordning för
  inmatningskartor och integrationsdokumentation för UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` och `lib/autocad/`: interaktiva
  2D-ritningsflöden och återanvändbara CAD-dokument-/redigeringskomponenter.
- **Ballum** — displaykontroll på pixelnivå: set/get per pixel, regionsblit,
  ljusstyrka/värme/gamma för hela displayen, pekar-/tangentbordsinmatning och en
  **helskärmsframebuffertyta** för total, ägd och snabb kontroll per pixel
  (override-redirect-fönster + en enda `XPutImage`/MIT-SHM-presentation på Linux/X11).
  Inbyggd kärna under `ballum/` med SLeeLa-fasaden i `lib/ballum/` och en VM-brygga
  under `impl/`; se [`ballum/README.md`](ballum/README.md) och det
  körbara [`ballum/example/`](ballum/example/).
- **CDMA** — `cdma/` och `lib/cdma/`: CDMA-relaterat tekniskt material och
  arbete med SLeeLa-källbibliotek.
- **Character** — `lib/character/`: karaktärsprofiler och återanvändbar
  karaktärsstatistik, inklusive exempel på biografiska profiler.
- **Coorenagraph** — `lib/coorenagraph/`: etiskt resonemang, moraliskt ordförråd,
  överläggning och relaterade begrepp; se paketets README och källfiler.

### D–G

- **Styrningskällmodeller** — [lib/governance](lib/governance/README.md) definierar deklarativa SLeeLa-källformer för förslag, anonyma manuella röster och aggregerade sammanräkningar. Den föreslagna annoteringen @vote och den manuella processen dokumenteras i [registry/DEMOCRATIC-VOTING-SYNTAX.md](registry/DEMOCRATIC-VOTING-SYNTAX.md); nuvarande kompilatorer behandlar den som overksam kommentarsmetadata.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` till och med de tillgängliga versionsindelade
  HTTP-paketkatalogerna: versionsspecifika protokollspecifikationer och
  implementationer. Läs varje versions dokumentation för dess exakta omfattning
  och verifieringsstatus.
- **International Criminal Court** — `lib/international-criminal-court/` och
  `INTERNATIONAL.CRIMINAL.COURTS.md`: relaterad domänspecifik källkod och
  dokumentation.

### O–S

- **OFDMA** — `ofdma/`: tekniskt material om ortogonal
  frekvensdelningsmultipelåtkomst och C/C++-utvecklingsresurser.
- **Philosophy och modulen Conversation Mask** — [`philosophy/`](philosophy/README.md)
  innehåller de inbyggda C/C++-grunderna och [`philosophy/module/`](philosophy/module/README.md)
  organiserar deras nästa modulskikt i tre uttryckliga områden:
  [`data/`](philosophy/module/data/README.md) för typade, daterade observationer,
  händelseavslut, halveringstidsinställningar, proveniens och samtycke; [`helper/`](philosophy/module/helper/README.md)
  för deterministisk validering, relevansavtagande, kompatibla jämförelser och
  samtyckesfiltrerad presentation; samt [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md)
  för ett stödjande, icke-kliniskt samtalsgränssnitt. De SLeeLa-vända
  hanteringarna och det sammansatta trädet finns i [`lib/philosophy/`](lib/philosophy/README.md),
  tillsammans med det bredare philosophy-paketet. Modulen bygger på uttrycklig indata
  och samtycke först: den övervakar inte passivt människor eller medier, ställer inte
  diagnoser, härleder inte privata avsikter, poängsätter inte människors värde och
  rekommenderar inte ändringar av medicinering. Dess
  händelselänkar bevarar proveniens och navigering, inte oberoende bekräftelse.
- **Synchro** — inbyggd integration av körmiljö/kompilator samt funktionalitet för
  paketutskick och mätning; se relevant `impl/`-källkod och
  kompilatorimplementationen.
- **SleelaUI / terminalgränssnitt** — `lib/user-interface/` och
  `sleela-terminal`-projektet: SLeeLa-gränssnittsintegration och terminalorienterade
  interaktionskomponenter.

### U–W

- **United Nations** — `united-nations/`: domänspecifikt repositorymaterial.
- **Wi-Fi 4, Wi-Fi 5 och Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`,
  `wi-fi-6/` och tillhörande källkataloger i `lib/wifi/`: generationsspecifikt
  material om trådlösa nätverk och konfigurationsresurser.

### Underhåll av paketindexet

När ett paket läggs till eller väsentligt utökas, lägg till dess post här i
lämplig alfabetisk grupp, länka till dess kanoniska katalog och README,
och beskriv bara förmågor som stöds av de incheckade källorna. Håll detta
index additivt: ta inte bort eller skriv om de befintliga README-avsnitten
som en del av uppdateringar av paketindexet.


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

US Democratic and US Policy.

---

## XVI. Nationell vetenskaplig varumärkesbild

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. Experimentell UTF-4088-katalog med en miljon poster

[UTF-4088-katalogsgeneratorn](utf-4088/tools/generate_catalogue.py) bygger deterministiskt en experimentell symbolkatalog med 1 048 576 poster. Det dokumenterade kompakta formatet beskrivs i [utf-4088/FORMAT.md](utf-4088/FORMAT.md); kontraktet för generering och validering finns i [utf-4088/GENERATION-RESULTS.md](utf-4088/GENERATION-RESULTS.md).

Kör generatorn med Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Den genererade binärfilen är 16 777 244 byte och är utformad för att förbli under 20 000 000 byte. Enhetstester och ett GitHub Actions-arbetsflöde verifierar antal poster, glyfers unikhet, storlek och SHA-256; lyckade körningar publicerar binärfilen och sammanfattningen som arbetsflödesartefakten `utf4088-million-catalogue`. Den genererade binärfilen hålls medvetet utanför Git-historiken. UTF-4088 förblir experimentellt och är inte Unicode-kompatibelt.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Senaste tillägg — Pi-konstanten, start av Modium/Registry, sleelactl Math och `@native`

Det här avsnittet dokumenterar funktioner som lades till under arbetet i 2026-11, de nyaste grupperna först. De länkade källfilerna och deras valideringsstatus är auktoritativa; ändringar under granskning beskrivs inte som sammanfogade förrän de når `master`.

### Pi-katalogkonstanten med 100 000 000 siffror

Repositoryt levereras med ett verifierat värde på **hundra miljoner siffror** av π i [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (ett välformat decimaltal, `3.14159…`), vid sidan av ett värde med 57 344 siffror i [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Båda är oberoende verifierade som äkta π — se [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md), som dokumenterar kontrollerna (läroboksprefixet, Feynman-punkten vid decimalposition 762, den berömda miljonte siffran, överensstämmelse med `lib/math/SLPiReference.sleela` och en frekvensanalys av siffrorna i hela filen) samt ett reproduktionsskript för en enda körning.

### Modium™ / Registry™ läser in Pi-konstanten vid delsystemets start

Vid de dedikerade delsystemens ingångspunkter — `sleela registry` och `sleela modium` (samt installationsprogrammets första körning), **inte** vid varje `sleela run` — läser delsystemen Modium och Registry in den stora π-katalogkonstanten. I en interaktiv terminal skriver de ut en övergående röd rad `Loading Modium™` / `Loading Registry™` som sedan raderas på plats innan den normala utdatan börjar; effekten är strikt TTY-spärrad, så utdata via rör och testutdata förblir byte för byte oförändrade. Sökvägen (inte bytena) sås som registrets `Math`-nyckelstandardvärden `piReferenceLargePath` och `piReferenceLargeDigits`, både i SLeeLa-modellen ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) och i den inbyggda sådden ([`registry/sleela_registry.c`](registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` exponerar `largePath()` / `largeDigits()`. π-konstanten installeras på operativsystemets SLeeLa-plats (`<data>/pi/`) av **både** det vanliga `make`-installationsprogrammet och det guidade ”vackra” installationsprogrammet. Detaljer: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — det vackra installationsprogrammet och ett inbyggt stött `math`-ämne

- Det guidade (”vackra”) installationsprogrammet installerar nu `sleelactl` (SLeeLas kontrollplan) och dess modulregister, i likhet med `make`-installationsprogrammet. Se [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- `sleelactl` fick en ny **`subject`**-natur för de inbyggda C/C++-ämnesbiblioteken som kompileras in i körmiljön, och registrerar det första: **`math`**. `sleelactl run math` kör den inbyggda bakänden `__NativeMath` (`impl/subjects/math/math_native.cpp`) från början till slut via ämnets referensdemonstration; `sleelactl info math` förklarar hur den nås (`import math;`). Ämnesnaturen erbjuder bara `run info` — det finns medvetet inget `check`-verb, eftersom `sleela check` inte läser in de SHEET-registrerade inbyggda ämnena.

### `@native`, Native Drive och körning på flera språk

En ny metodannotering **`@native`** begär kompilering i förväg (AOT) till inbyggd kod av en het metod. Det nya kommandot `sleela aot` hittar `@native`-metoder, verifierar att var och en ligger inom en understödd *avlastningsbar delmängd* och genererar C/C++ — **felsäkert** (det avvisar, med en exakt orsak) varje konstruktion det ännu inte kan översätta, så att en halvöversatt metod aldrig produceras.

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

`@native` förblir en begäran om AOT-behandling, där konstruktioner som inte stöds avvisas före generering. [Native Drive](impl/native-drive/README.md) stöder byggen av körbara filer och delade bibliotek i flera enheter med C/C++ via värdkompilatorer (inklusive MSVC), understödd SLeeLa AOT-utdata, symbolinspektion mot den inbyggda laddarens ABI, skalfri processkörning, hantering av stdin/stdout/stderr, flerstegs rörledningar och XClass-delegering till produktionsladdaren. `sleelactl` exponerar dess bygg-/körkommandon och dirigerar XClass-inspektion samt uttrycklig rendering/installation av systemd-enheter via `inspect native-drive`. VM:en äger en begränsad hanteringstabell för inbyggda moduler med obligatorisk värdauktorisering och generationskontrollerade hanterare. Syntax 3.0 lägger nu till `nativeOpen`, `nativeCall` och `nativeClose`: append-only-bytekodsutskick korsar ett separat versionerat ABI för null/bool/int64/double/UTF-8, medan VM-ägda arrayer, structar och andra sammansatta värden stannar inne i interpretatorn och kan använda bytekodsreserv med samma namn. Insticksmoduler kan valfritt få en versionerad återanropstabell från värden för att anropa exakta SLeeLa-funktioner genom kapslad VM-exekvering, med GC-rotning av avbrutna stackar, en uttrycklig tillåtelselista för värdens återanropsnamn och avvisade rekursiva anrop av inbyggda API:er. Inbyggd kod förblir betrodd kod i processen, inte en sandlåda. Annoteringar för processpolicy i källkoden har ett versionerat schema men tolkas och upprätthålls ännu inte. Detaljer, säkerhetsgränser och liggaren över de sju arbetsströmmarna: [Native Drive](impl/native-drive/README.md) och [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).


## XIX. Internationella README-utgåvor

SLeeLa tillhandahåller landsspecifika README-ingångar för de 50 marknaderna i sin internationella utgåva. Varje land har en fil med namnet `README.<CC>.md`; där flera språk är relevanta grupperas de inom landets fil.

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

**Översättningspolicy:** Dessa filer är inledande lokaliserade landsingångar. Deras inledningar och språkrubriker är lokaliserade, medan den fullständiga tekniska referensen finns kvar i den kanoniska engelska [README.md](README.md). En landsfil får inte beskrivas som en fullständig översättning förrän varje avsnitt har översatts, tekniska identifierare och länkar har kontrollerats och utgåvan har genomgått språkgranskning. Språktäckningen i denna första omgång är representativ och utgör inte något påstående om att varje författningsenligt erkänt, regionalt, urfolks- eller teckenspråk har översatts fullständigt.

## Bilaga — bevarad regional vägledning

Följande praktiska avsnitt fanns i den tidigare, kortare svenska guiden och finns inte i den kanoniska README-filen. De bevaras här som regional vägledning och ersätter inte de kanoniska avsnitten ovan; vid avvikelser gäller den kanoniska [README.md](README.md).

### Mål, profiler och gränser för körning

SLeeLa skiljer källkod från bygg- och körmål. Den dokumenterade målfamiljen omfattar `SLVM`, `SLJVM` och `SLIR`; `NATIVE` är ett begränsat mål med villkor och ska inte antas vara tillgängligt på alla plattformar. Profilerna `BASIC_COMPLETE` och `ADVANCED_TOTAL` anger olika omfattning. Kontrollera faktiskt mål, profil och plattformsstöd för varje bygge.

En referens identifierar en fil, modul eller resurs men ger inte i sig tillstånd att köra den. När ett program använder filer, externa bibliotek, inbyggda bindningar (native bindings) eller systemresurser måste behörighetsgränser och ansvar för körningen definieras tydligt. En beskrivning i dokumentationen är inte ensam ett bevis för en säkerhetsgaranti vid körning; granska implementationen och relevanta tester.

### Resursers livscykel och felsökning

Dokumentera ägarskap, livslängd och synkroniseringsansvar för resurser som delas mellan funktioner eller trådar. Skilj mellan lokala variabler, delat objekttillstånd, VM-hanterare (VM handles) och serialiserbara data. En VM-lokal hanterare är inte ett stabilt filformat eller nätverksmeddelande. Använd modulens definierade serialiseringsgränssnitt och testa ogiltiga värden, saknade värden och gränsvärden.

Vid felsökning ska commit, operativsystem, arkitektur och verktygskedja registreras. Bygg först det minsta relevanta målet, spara hela felutskriften, skilj kompileringsfel från körningsfel och skapa ett minimalt reproducerbart exempel. Ett lyckat bygge bevisar inte ensamt att körningsbeteendet är korrekt.

En verifieringsrapport bör ange exakt commit, operativsystem/arkitektur, kompilatorversioner, fullständiga bygg- och testkommandon, antal godkända/misslyckade/överhoppade tester och kända begränsningar. Tester som hoppats över eller inte körts får inte räknas som godkända.

**Översättningsstatus:** Avsnitt I–XIX är översatta till svenska. Tekniska identifierare, kommandon, kodexempel, filnamn, länkar och adresser är bevarade. En avslutande språkgranskning av personer med svenska som modersmål rekommenderas.
