# SLeeLa 3.0 — Sverige (SE)

## Svensk utgåva

SLeeLa är ett systemprogrammeringsspråk med egen kompilator, körmiljö och ett ekosystem av virtuella maskiner. Projektet omfattar även bibliotek, utvecklarverktyg, dokumentation, byggprocesser och integrationsavtal för plattformar som stöds. Tekniska identifierare, kommandon, sökvägar, filnamn, kodexempel och API-namn behålls oförändrade så att instruktionerna kan användas direkt.

**The Now. The Ingredient.** Ett programmeringsspråk är mer än syntax: det består av kompilatorn, körmiljön, biblioteken, verktygen och plattformskontrakten som gör programvara användbar.

SLeeLa beskriver sina arbetsområden genom **Math**, **Object**, **Science** och **Reach**. Nationella intressen som är relevanta för ett ämne ska fortsatt anges uttryckligen i metadata och sätts inte åt sidan av denna indelning.

## I. Kom igång

- [Bygg den inbyggda implementationen](impl/README.md).
- [Användarhandbok för virtuella maskiner och Modium](virtual-machine/USER-GUIDE.md), inklusive `sleela vm` och `sleela modium`.
- [CPU-arkitektur i SLeeLa-källkod](lib/cpu/CPU.md): från logiska grindar och ALU till processorkärnor, operativsystem och program. Katalogen innehåller mer än 130 CPU-profiler. Se även [timing, spänningar och frekvenser](lib/cpu/TIMING.md).
- [Dokumentationsindex](docs/README.md).
- [CSPML-datakälla och forskningsreferens för Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md).
- [Genererad API-dokumentation](docs/api/README.md) och [API-katalog](api/README.md).
- [Standarder för beräkningsjämförelser](comparison/README.md): typer, minne, ABI och överensstämmelse mellan SLeeLa VM, Java/JVM och inbyggd C/C++.
- [C- och C++-referensprototyper för Recursion stanchion](recursion/stanchion/README.md).
- [Plattformsstöd och valideringsmatris](docs/PLATFORM-SUPPORT.md).
- [Varumärkesguide](docs/BRAND-GUIDE.md).
- [Testpolicy och krav för utgåvor](markdown/TESTING.md).

## II. Funktioner och nyheter

Standardbibliotekspaketet **Advantage** (`lib/advantage/`) modellerar bedömningen av ett resultat i förhållande till en modell. Den lagerindelade strukturen omfattar granskning, faktorer, matematik, tillägg, subtraktion, omgruppering och iteration. Ämnena omfattar bilateral handelsekonomi, en strukturell fysikanalogi för förtroendefält (uttryckligen markerad som en modellanalogi), vertikal integration, modulgradering och routad utdata. Se [Advantage-dokumentationen](lib/advantage/README.md).

Andra dokumenterade funktioner:

- **Loop-timing:** `@loopDelay` och `@loopBudget` anger pauser och tidsbudgetar för loopar. De sänks till den append-only VM-instruktionen `OP_SLEEP` (ISA 157), som även används av den allmänna funktionen `sleep(ms)`. Se [syntaxspecifikationen](SLEELA.syntax) §32 och [API-referensen](api/sleela/3.0/LOOP_TIMING_API.md).
- **SleelaDoc:** `sleela docs` visar dokumentation för ämnen, paket och klasser. `sleela docs --generate [dir]` skapar en lokal textkorpus för offlineanvändning. Se [SleelaDoc-biblioteket](lib/sleeladoc/README.md).
- **XML DOM:** `lib/xml` innehåller en XML-dokumentmodell med noder för element, text, kommentarer, CDATA och processinstruktioner, attribut, serialisering och parsning. Se [XML-dokumentationen](lib/xml/README.md).
- **Heltalstyper:** `int`, `long` och `int64` är signerade 64-bitars heltal. `int128` accepteras som heltalstyp men är för närvarande 64-bitarsbaserad; det är inte en fullständig 128-bitarsimplementation.
- **`@mix`-fält:** upprepade instansfält med samma namn kan grupperas som en indexerad sekvens när varje fält är märkt med `@mix`. Se syntax 2.9 i [SLEELA.syntax](SLEELA.syntax).
- **Preonems:** dokumentmarkörerna `author`, `authorial`, `material;`, `memory;`, `determine;`, `object;` och `field;` beskriver dokumentets status och ursprung. De är deklarativa markörer, inte tillstånd att köra kod.
- **Nordshrift:** verktyget använder det rekursiva `/lib`-indexet för att kontrollera paket och `.sleela`-enheter.
- **Vinkelparenteskommentarer:** formerna `< line >` och `<< block >>` är dokumenterade kommentarsyntaxer.
- **Radixliteraler:** heltalsliteraler stöder hexadecimal (`0x`), binär (`0b`) och oktal (`0o`) notation med `_` som sifferavgränsare.
- **Metadata Class:** klassbärande källmoduler i biblioteket använder ett metadatamönster för ursprung, laglighet och jurisdiktionell omfattning.
- **Text och String:** `lib/text` innehåller teckencellsbaserade text- och strängoperationer, bland annat `charAt`, `substring`, `indexOf`, `reverse` och positionsbaserad formatering.

En funktionsbeskrivning är inte i sig ett bevis på att funktionen är implementerad och verifierad för alla mål eller plattformar. Kontrollera alltid den aktuella implementationen och testresultaten.

## III. Bygg och testa

Kraven beror på valt byggmål. För den inbyggda implementationen behövs vanligtvis en C/C++-verktygskedja, till exempel GCC eller Clang, samt GNU Make. Vissa manifest- och arbetsflödesverktyg använder Python 3. Grafiska Linux-komponenter kan kräva utvecklingspaket för X11 och XRandR.

Från repositoryts rotkatalog:

```sh
make
```

Bygg den inbyggda kärnan och kör testsviten:

```sh
make -C impl all
bash ./test-suites/run-all.sh --all
```

Vanliga Make-mål:

```sh
make all
make clean
make help
make registry
make api-docs
```

Att en komponent byggs utan fel bevisar inte att alla bibliotek eller plattformar är redo för en release. Testresultat ska skilja mellan tester som körts, hoppats över och misslyckats. Läs [testpolicyn](markdown/TESTING.md) innan en byggstatus tolkas som ett releasegodkännande.

## IV. Installation

En systemomfattande installation använder vanligtvis prefixet `/usr/local`:

```sh
sudo make install
```

Installation för endast den aktuella användaren, normalt utan administratörsbehörighet:

```sh
make install PREFIX="$HOME/.local"
```

Skapa en paketrot och kontrollera installationen:

```sh
make install DESTDIR="$PWD/package-root"
make install-check
```

Följ instruktionerna för valt mål och operativsystem. Vid användarinstallation måste motsvarande `bin`-katalog finnas i `PATH`. Anta inte att samma installationskommando fungerar likadant i Linux, macOS och Windows; kontrollera plattformsdokumentationen och tillgängliga installationsskript.

- [Installationsguide](install/README.md)
- [Plattformsstöd](docs/PLATFORM-SUPPORT.md)
- [Bygg den inbyggda implementationen](impl/README.md)

## V. Språk, kompilator och körmiljö

Den normativa språkspecifikationen finns i [SLEELA.syntax](SLEELA.syntax). Dokumentationen för SLeeLa 3.0 omfattar syntax, kompilator, körmiljö, virtuella maskiner och bibliotek. Kompilatorversion, syntaxversion och VM-/ISA-version är separata begrepp och ska inte blandas ihop.

En typisk verktygskedja omfattar:

1. **Källkod:** `.sleela`-filer med språkversion och deklarationer.
2. **Frontend:** lexikal analys, parsning, symbolhantering och semantiska kontroller.
3. **Mellanrepresentation:** en strukturerad form för efterföljande sänkning.
4. **Kodgenerering:** utdata för mål som stöds av VM eller mellanformat.
5. **Körmiljö:** exekvering av kompilerat innehåll med mekanismer för minneshantering och kontrollflöde.
6. **Bibliotek och verktyg:** återanvändbara paket, dokumentation, byggsystem och verifiering.

De exakta funktionerna beror på den aktuella implementationen. Att en funktion finns i specifikationen innebär inte automatiskt att den stöds på varje mål.

## VI. C, C++ och inbyggd kompilering

SLeeLa-projektet innehåller C- och C++-komponenter och dokumenterar en avgränsad inbyggd väg. Markören `@native` och kommandot `sleela aot` beskriver ett utvecklingsområde för inbyggd kodgenerering. De innebär inte ett generellt tillstånd att köra godtycklig inbyggd kod.

När SLeeLa kopplas till C eller C++ måste gränssnittet vara uttryckligt. ABI-kontrakt, minnesägarskap, datatyper, felhantering och plattformsberoenden ska dokumenteras och testas. Behandla inbyggd kod som en separat tillitsgräns; en referens eller deklaration ger inte i sig tillstånd att exekvera kod.

Se [den inbyggda implementationen](impl/README.md), [plattformsmatrisen](docs/PLATFORM-SUPPORT.md) och [jämförelsestandarderna](comparison/README.md).

## VII. Bibliotek och delsystem

Repositoryt innehåller ett omfattande bibliotek- och verktygsekosystem. Viktiga ingångar:

- `lib/advantage` — Advantage-biblioteket med definitioner, matematik, fysik, integration, gradering och utdata.
- `lib/xml` — XML-dokumentmodell och serialisering.
- `lib/text` — text- och strängoperationer.
- `lib/sleeladoc` — dokumentationsextraktion och CLI-stöd.
- `lib/cpu` — CPU-modeller, komponenter och timingreferenser.
- `lib/xlt` — CPU-modellformat och tillhörande dokumentation.
- `recursion/stanchion` — referensprototyper och exempel i C/C++.
- `virtual-machine` — VM-guider och Modium-material.
- `api` och `docs/api` — API-katalog och genererad referens.

En katalogpost innebär inte nödvändigtvis att modulen är produktionsklar. Kontrollera modulens README, testtäckning, plattformsbegränsningar och valideringsdokumentation före användning.

## VIII. Integritet, säkerhet och verifiering

SLeeLa betonar spårbarhet mellan källkod, dokumentation, byggprocess och testresultat. Hashmanifest och verifieringsverktyg kan kontrollera att filer motsvarar förväntat innehåll. En godkänd kontroll gäller endast det angivna omfånget och den tidpunkt då den utfördes.

Grundläggande principer:

- Referenser identifierar resurser; de ger inte i sig tillstånd att exekvera dem.
- Inbyggda gränssnitt ska vara uttryckliga och begränsade.
- Testresultat ska separat visa pass, fail och skip.
- Påståenden om plattformsstöd ska kopplas till konkreta byggen och tester.
- Genererad dokumentation ska kunna spåras till källan och generatorn.
- Säkerhetskontroller ska misslyckas på ett säkert sätt när nödvändig integritetsinformation saknas.

Se [testpolicyn](markdown/TESTING.md), [plattformsstödet](docs/PLATFORM-SUPPORT.md) och [installationsguiden](install/README.md).

## IX. Redigerare och utvecklingsflöde

Repositoryt innehåller integrationer och guider för kodredigerare. Se [Linux-editorintegrationer](linux-editors/README.md) för resurser till Vim och Emacs. Kontrollera att instruktionerna passar operativsystemet och redigerarversionen innan installation.

Rekommenderat arbetsflöde för bidrag:

1. Läs dokumentationsindexet och dokumentationen för modulen som ska ändras.
2. Avgränsa ändringen och förklara dess syfte.
3. Lägg till positiva och negativa tester.
4. Kör relevanta tester och vid behov hela testsviten.
5. Dokumentera operativsystem, verktygskedja, kommandon och faktiska resultat.
6. Uppdatera README och API-dokumentation när publikt beteende ändras.
7. Markera inte tester som godkända om de hoppats över eller inte körts.

## X. Ytterligare projektområden

SLeeLa-repositoryt omfattar även CPU- och hårdvarumodeller, grafiska gränssnitt, nätverkskomponenter, textbehandling, dokumentation, internationalisering och experimentella kataloger. Dessa områden kan ha olika mognadsgrad. En experimentell katalog eller ett exempel ska inte beskrivas som en stabil produktionsfunktion utan dokumenterad verifiering.

UTF-4088-katalogen, π-verktygen, Modium, Registry och `sleelactl` har separata ingångar och tekniska förutsättningar. Kontrollera den kanoniska README-filen och respektive moduls dokumentation för aktuell status.

## XI. API och referensdokumentation

- [Genererad API-dokumentation](docs/api/README.md)
- [API-katalog](api/README.md)
- [SleelaDoc](lib/sleeladoc/README.md)
- [Syntaxspecifikation](SLEELA.syntax)
- [Dokumentationsindex](docs/README.md)
- [CPU-dokumentation](lib/cpu/CPU.md)
- [CPU-timing](lib/cpu/TIMING.md)

## XII. AutoCAD Creator™

AutoCAD Creator™ finns med i projektets tekniska katalog. Läs dokumentationen för den aktuella modulen för att skilja mellan beskrivna funktioner, exempel och verifierade resultat. Anta inte kompatibilitet med en viss AutoCAD-version utan en uttrycklig kompatibilitetsmatris och testresultat.

## XIII. Experimentell UTF-4088-katalog

UTF-4088-materialet är ett experimentellt område för tecken och kataloger i projektet. Storlek, antal poster, minneskrav och formatdetaljer ska bedömas tillsammans med dokumentationen och den aktuella implementationen. Ett stort antal poster bevisar inte i sig fullständig Unicode-kompatibilitet eller produktionsmognad.

## XIV. π, Modium, Registry och sleelactl

Projektet innehåller separata verktyg och dokumentationsområden för π-generering, Modium, Registry och `sleelactl`. De har olika syften och ska byggas och kontrolleras enligt sina respektive instruktioner. Aktuella kommandon och begränsningar finns i den kanoniska README-filen och respektive katalog.

## XV. Internationella README-utgåvor

Landsutgåvorna är lokaliserade ingångar till SLeeLa-dokumentationen. De ska göra centrala tekniska ämnen begripliga på lokala språk samtidigt som tekniska identifierare, kommandon, filnamn, kod och länkar bevaras. En utgåva ska inte kallas fullständigt översatt förrän alla relevanta tekniska avsnitt har översatts och kontrollerats.

Den svenska utgåvan riktar sig till svenskspråkiga läsare i Sverige. Den gör inte anspråk på att täcka alla nationella minoritetsspråk, regionala språk eller svenskt teckenspråk fullständigt. Bidrag till ytterligare språk och tillgängliga format välkomnas.

## Viktiga referenser

- [Kanonisk README på engelska](README.md)
- [SLeeLa-syntaxspecifikation](SLEELA.syntax)
- [Dokumentationsindex](docs/README.md)
- [API-dokumentation](docs/api/README.md)
- [Plattformsstöd](docs/PLATFORM-SUPPORT.md)
- [Inbyggd implementation](impl/README.md)
- [Installationsguide](install/README.md)
- [Testpolicy](markdown/TESTING.md)
- [SLeeLa på GitHub](https://github.com/mearvk/SLeeLa)

**Översättningsstatus:** Den svenska utgåvan har utökats till en teknisk introduktion som täcker kom igång, byggande, installation, språk och körmiljö, inbyggd integration, bibliotek, säkerhet, utvecklingsflöde och centrala projektområden. Den är ännu inte en rad-för-rad-översättning av alla detaljer i den kanoniska engelska README-filen. Teknisk granskning och språkgranskning av en svenskspråkig fackperson rekommenderas innan utgåvan betecknas som slutgiltig.


## XVI. Mål, profiler och gränser för körning

SLeeLa skiljer källkod från bygg- och körmål. Den dokumenterade målfamiljen omfattar `SLVM`, `SLJVM` och `SLIR`; `NATIVE` är ett begränsat mål med villkor och ska inte antas vara tillgängligt på alla plattformar. Profilerna `BASIC_COMPLETE` och `ADVANCED_TOTAL` anger olika omfattning. Kontrollera faktiskt mål, profil och plattformsstöd för varje bygge.

En referens identifierar en fil, modul eller resurs men ger inte i sig tillstånd att köra den. När ett program använder filer, externa bibliotek, native bindings eller systemresurser måste behörighetsgränser och ansvar för körningen definieras tydligt. En beskrivning i dokumentationen är inte ensam ett bevis för en säkerhetsgaranti i runtime; granska implementationen och relevanta tester.

## XVII. Resurslivscykel och felsökning

Dokumentera ägarskap, livslängd och synkroniseringsansvar för resurser som delas mellan funktioner eller trådar. Skilj mellan local variables, shared object state, VM handles och serializable data. Ett VM-local handle är inte ett stabilt filformat eller nätverksmeddelande. Använd modulens definierade serialiseringsgränssnitt och testa invalid, missing och boundary values.

Vid felsökning ska commit, OS, architecture och toolchain registreras. Bygg först det minsta relevanta målet, spara hela felutskriften, skilj compiler errors från runtime errors och skapa ett minimalt reproducerbart exempel. Ett lyckat bygge bevisar inte ensamt att runtime-beteendet är korrekt.

En verifieringsrapport bör ange exact commit, OS/architecture, compiler versions, fullständiga build/test commands, antal pass/fail/skip och kända begränsningar. Tester som hoppats över eller inte körts får inte räknas som godkända.
