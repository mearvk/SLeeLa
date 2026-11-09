# SLeeLa 3.0 — Danmark (DK)

## Dansk udgave

SLeeLa er et native systemprogrammeringssprog med compiler, runtime og et økosystem af virtuelle maskiner. Projektet omfatter desuden biblioteker, udviklingsværktøjer og integrationsaftaler med platforme. Tekniske identifikatorer, kommandoer, stier, filnavne, kodeeksempler og API-navne bevares, så vejledningerne kan anvendes direkte.

**The Now. The Ingredient.** Et sprog er mere end syntaks: Det omfatter compiler, runtime, biblioteker, værktøjer og platformaftaler, som gør software anvendelig.

SLeeLa beskriver sine arbejdsområder gennem **Math**, **Object**, **Science** og **Reach**. Relevante nationale interesser bevares som udtrykkelige metadata og sættes ikke til side af denne opdeling.

## I. Kom godt i gang

- [Byg den native implementering](impl/README.md).
- [Brugervejledning til virtuelle maskiner og Modium](virtual-machine/USER-GUIDE.md), herunder `sleela vm` og `sleela modium`.
- [CPU-arkitektur i SLeeLa-kildekode](lib/cpu/CPU.md): fra logiske porte og ALU til kerner, operativsystem og programmer; kataloget indeholder over 130 CPU-profiler. Se også [timing, spændinger og frekvenser](lib/cpu/TIMING.md).
- [Dokumentationsindeks](docs/README.md).
- [CSPML-datakilde og Salt & Salary-forskningsreference](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md).
- [Genereret API-dokumentation](docs/api/README.md) og [API-katalog](api/README.md).
- [Standarder for beregningsmæssig sammenligning](comparison/README.md): typer, hukommelse, ABI og overensstemmelse mellem SLeeLa VM, Java/JVM og native C/C++.
- [C- og C++-referenceprototyper til Recursion stanchion](recursion/stanchion/README.md).
- [Platformunderstøttelse og valideringsmatrix](docs/PLATFORM-SUPPORT.md).
- [Brandguide](docs/BRAND-GUIDE.md).
- [Testpolitik og releasekrav](markdown/TESTING.md).

## II. Funktioner og nyheder

Standardbibliotekspakken **Advantage** (`lib/advantage/`) modellerer analyse af et udbytte i forhold til en model. Den er organiseret i lag med review, concerns, maths, additions, subtraction, regrouping og iteration. Emnerne omfatter bilateral handelsøkonomi, en strukturel analogi for tillidsfelter i fysik (udtrykkeligt markeret som en modelanalogi), vertikal integration, modulbedømmelse og routet output. Se [Advantage-dokumentationen](lib/advantage/README.md).

Andre dokumenterede funktioner omfatter:

- **Løkketiming:** `@loopDelay` og `@loopBudget` angiver pauser og tidsbudgetter for løkker; implementeringen bruger `OP_SLEEP` og den indbyggede funktion `sleep(ms)`. Se [specifikation §32](SLEELA.syntax) og [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md).
- **SleelaDoc:** dokumentationskonventioner i kildekoden og værktøjer til at generere Markdown/HTML-sider, emneindeks og API-dokumentation. Kommandoen `sleela docs` viser emner og dokumentation for pakker og klasser. Se [SleelaDoc](lib/sleeladoc/README.md).
- **XML DOM:** XML-nodetræ med elementer, tekst, kommentarer, CDATA, behandlingsinstruktioner, ordnede børn og attributter samt parser og serializer. Se [XML DOM](lib/xml/README.md).
- **Udvidede heltalstyper:** dokumentationen omfatter `long`, `int64`, `int128` og signerede og usignerede heltalsfamilier. Den faktiske bredde skal kontrolleres i specifikationen og må ikke udledes af navnet alene.
- **`@mix`-felter:** gentagne instansfelter med samme navn kan samles i en indekseret sekvens efter specifikationens regler.
- **Preonem-dokumentmarkører:** blandt andet `author`, `authorial`, `material`, `memory`, `determine`, `object` og `field`. Betydningen afhænger af kontekst og syntaksversion; uden for markørpositionen er ordene almindelige identifikatorer.
- **Nordshrift:** finder pakker og kildeenheder gennem det samme rekursive `/lib`-indeks som compileren.
- **Vinkelparenteskommentarer:** `< ... >` og `<< ... >>` er dokumenterede kommentarformer.
- **Tal med radix:** syntaksen understøtter hexadecimal, binær og oktal notation samt cifferseparatorer.

[`SLEELA.syntax`](SLEELA.syntax) er den autoritative kilde til sprogets syntaks. At en funktion omtales i dokumentationen, betyder ikke nødvendigvis, at alle compilerversioner understøtter den.

## III. Byg og test

**Forudsætninger:** C/C++-værktøjskæde (GCC eller Clang), GNU Make og Python 3 til manifest- og workflowværktøjer. Grafiske produkter på Linux kan kræve X11/Xrandr-udviklingspakker. Se det enkelte moduls dokumentation for dets konkrete afhængigheder.

Kør fra repositoryts rodmappe:

```sh
make
```

Byg kun den native kerne og kør hele testsuiten:

```sh
make -C impl all
bash ./test-suites/run-all.sh --all
```

Almindelige Make-mål:

```sh
make all
make registry
make tests
make ballum
make help
make clean
```

Rodmålet koordinerer bygningen af repositoryts produkter. Målet `registry` bygger registry-kernen og opretter dens baseline-tabel. Se [Registry-guiden](registry/README.md) og [Registry 3.0-arkitekturen](registry/REGISTRY-3.0-ARCHITECTURE.md).

En vellykket bygning af én komponent beviser ikke, at alle biblioteker, platforme eller konfigurationer er klar til release. Validering skal vurderes for hvert modul og hver platform. Tests, der er sprunget over eller ikke er kørt, må ikke rapporteres som bestået.

## IV. Installation

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

En almindelig systeminstallation bruger typisk `/usr/local/bin`, `/usr/local/lib/sleela` og `/usr/local/share/sleela`. Med `PREFIX="$HOME/.local"` kan installationen normalt foretages i brugerens eget område uden `sudo`. `DESTDIR` gør det muligt at forberede en pakkestruktur uden direkte installation i systemet.

Til en guidet installation af hovedpakken alene — kommandoerne `sleela`, `sleelvac`, `nordshrift`, standardbiblioteket, protokoller og supportmoduler — bruges:

```sh
make clean-install
```

Se [Quick and Safe Install](install/README.md). Installationsprogrammet bevarer eksisterende opstartskonfiguration og undgår at overskrive kommandoer, det ikke administrerer. Afinstallation bør begrænses til filer, der er registreret af installationsprogrammet.

## V. Sprog og runtime

Repositoryts dokumentation følger **SLeeLa 3.0**-syntaksen. Compilerens version og den syntaksversion, som kildekoden erklærer, er to forskellige begreber. Version 3.0 samler sprogets funktioner fra version 1.0 til 2.9 og bevarer hver funktions minimumskrav til syntaksversion.

SLeeLa omfatter en native implementering, compiler, runtime, virtuelle maskiner, dokumentationsværktøjer og et bibliotek organiseret i pakker. Repositoryt indeholder både SLeeLa-kildekode og native C/C++-komponenter. Grænsen mellem native kode og runtime skal overholde de dokumenterede ABI-aftaler.

## VI. C, C++ og native kompilering

Annotationen `@native` og kommandoen `sleela aot` repræsenterer en tidlig og konservativ fase af ahead-of-time-kompilering. De kan opregne annoterede metoder, kontrollere om de tilhører en understøttet delmængde og udsende C/C++ for de understøttede tilfælde:

```sh
sleela aot --list <file.sleela>
sleela aot --emit <file.sleela>
```

Konstruktioner, der ikke understøttes, afvises med en begrundelse i stedet for at skabe en misvisende delvis oversættelse. `@native` ændrer ikke i sig selv programmets adfærd. Bygning af delte biblioteker og dispatch fra VM må ikke beskrives som færdig uden implementerings- og testbevis. Se [AT-NATIVE](pi-generator/AT-NATIVE.md).

## VII. Biblioteker og delsystemer

- **CPU** (`lib/cpu/`): processormodeller, profiler, eksekvering og timing.
- **Ballum** (`ballum/`, `lib/ballum/`): pixelbaseret skærmstyring, områder, lysstyrke, gamma samt tastatur- og markørinput; tilgængeligheden afhænger af platform og backend. Se [Ballum](ballum/README.md).
- **AutoCAD Creator™** (`autocad/creator/`): interaktiv 2D-CAD i SLeeLa til oprettelse af entiteter, lag, mål og noter, fortrydelse og lagring af DXF-tegninger. Redigeringsmodellen ejer geometrien og historikken; brugergrænsefladen præsenterer dem. Se [AutoCAD Creator](autocad/creator/README.md).
- **Alphabet og UTF-4088:** tegnkatalog og eksperimentelle værktøjer. UTF-4088 bruger sit eget identifikatorrum og er **ikke kompatibelt med Unicode**; dets identifikatorer må ikke forveksles med Unicode-skalærværdier.
- **Pi Generator:** kataloger over π-konstanter samt værktøjer til verifikation og reproduktion.
- **Modium og Registry:** registreringsdelsystemer og inspektionsværktøjer.
- **Philosophy:** native grundlag samt data-, hjælpe- og samtalemoduler. Samtalekomponenten beskrives som ikke-klinisk og baseret på eksplicit input og samtykke; den stiller ikke diagnoser.
- **CDMA, OFDMA, HTTP og Wi-Fi:** tekniske områder og versionsopdelte biblioteker. Kontrollér hvert moduls dokumentation for omfang og valideringsstatus.

Pakkekataloget er beskrivende og erstatter ikke modulets kildekode, README og tests.

## VIII. Integritet, sikkerhed og verifikation

Projektet dokumenterer SHA-256-manifester, lokale integritetskontroller og en valgfri ClamAV-adapter via Sleelavirin. At et værktøj eller manifest findes, er ikke i sig selv en sikkerhedscertificering.

Tekniske principper:

1. Udtrykkelige aftaler mellem komponenter.
2. Portabilitet indtænkt fra designfasen.
3. Sikker adfærd, når en operation fejler.
4. Reproducerbare beviser knyttet til det testede Git-commit.
5. Skelnen mellem at identificere en reference og at godkende udførelse.
6. Korrekt rapportering af tests, der ikke er kørt, er sprunget over eller er blokeret.

Ved vurdering af en release skal CI-resultater for det konkrete commit, genererede artefakter, relevante tests og platformsmatricen gennemgås. Et resultat fra Linux kan ikke overføres til Windows eller macOS uden særskilt dokumentation.

## IX. Editor og bidrag

SLeeLa leverer syntaksintegration og temaer til **Vim** og **Emacs** samt et native C/C++-værktøj til farvepaletter. Se [vejledningen til editorintegration](linux-editors/README.md).

Bidrag:

1. Læs [dokumentationsindekset](docs/README.md) og [testpolitikken](markdown/TESTING.md).
2. Hold ændringerne afgrænsede, og tilføj positive og negative tests.
3. Kør relevante tests og derefter den passende bredere testsuite.
4. Angiv operativsystem, compiler og anvendt værktøjskæde.
5. Beskriv ikke tests, der er sprunget over, som bestået.

SLeeLa udvikles af **MEARVK LLC**.

## X. Andre projektemner

Hoved-README'en omtaler også CSPML-datakilder og integrationsvejledning, forskningsreferencer, merchandise-koncepter og visuelle aktiver samt emner som Character, Governance, Synchro, SleelaUI og United Nations. Disse henvisninger beskriver repositoryts indhold og er ikke officielle godkendelser fra myndigheder eller institutioner.

Repositoryt indeholder også et afsnit med titlen **Bitcoin Conjegeum**, som omfatter adresser og grafisk materiale. Tilstedeværelsen af adresser eller billeder er ikke en finansiel anbefaling, en bekræftelse af ejerskab eller en instruktion om at overføre midler. Kontrollér oplysningerne uafhængigt før brug.

## XI. Dokumentation og API-værktøjer

`sleela docs` kan vise dokumenterbare emner, pakker og klasser og generere et tekstkorpus til offline brug. SleelaDoc understøtter mærker som `@intent`, `@thesis`, `@axiom`, `@param`, `@see` og `@mastery`. Se [SleelaDoc](lib/sleeladoc/README.md) og [den genererede API-dokumentation](sleeladoc-api/README.md).

## XII. AutoCAD Creator™

AutoCAD Creator™ er en interaktiv 2D-CAD-applikation skrevet i SLeeLa. Den kan oprette og ændre dokumenter, tilføje vægge, rum, mål og noter, flytte etiketter, ændre lag, slette elementer, fortryde handlinger og gemme R12-tekstbaserede DXF-tegninger. Brugergrænsefladen bruger SleelaUI™ og kan åbne native vinduer via X11, Cocoa eller Win32 afhængigt af platformen.

Det dokumenterede arkitekturprincip er: **SleelaUI præsenterer; SLeeLa beslutter.** Modellen `SLCadCreator` ejer dokumentets geometri og den reversible redigeringshistorik; brugergrænsefladen er ikke den autoritative kilde til geometrien.

```sh
make creator
```

Kør kommandoen fra `autocad/`. I et miljø uden display kan eksemplet oprette tegningen i hukommelsen, udskrive en rapport og afslutte.

## XIII. Eksperimentelt UTF-4088-katalog

`utf-4088/tools/generate_catalogue.py` genererer deterministisk et eksperimentelt katalog med **1.048.576** poster. Den dokumenterede binærfil fylder 16.777.244 bytes. Tests og et GitHub Actions-workflow kontrollerer antal poster, glyffers entydighed, størrelse og SHA-256. Binærfilen offentliggøres som workflow-artefakt og holdes uden for Git-historikken. UTF-4088 er fortsat eksperimentelt og ikke kompatibelt med Unicode.

## XIV. π, Modium, Registry og sleelactl

Repositoryt dokumenterer decimal-kataloger for π, herunder en udgave med **100.000.000** cifre og en med 57.344 cifre. [VERIFIED](pi-generator/VERIFIED.md) beskriver kontrollerne og reproduktionsproceduren.

Indgangene `sleela registry` og `sleela modium` indlæser det store π-katalog. Den guidede installer installerer `sleelactl` og dets modulregister. Projektet beskriver `math` som det første subject for et native C/C++-bibliotek integreret med runtime; den konkrete funktionalitet skal kontrolleres i modulets dokumentation og tests.

## XV. Internationale README-udgaver

Landeudgaver bruger navnemønstret `README.<CC>.md`. En lande-fil kan indeholde flere sprogafsnit. At et sprog er nævnt, betyder ikke, at alle regionale, oprindelige, officielle eller tegnsprog er oversat.

**Oversættelsespolitik:** Tekniske navne, identifikatorer, kommandoer, stier, links og kodeeksempler bevares. En udgave bør kun kaldes en fuldstændig oversættelse, når alle afsnit er oversat og har gennemgået sproglig og teknisk kontrol. Ikke-oversat eller ikke-gennemgået teknisk indhold følger fortsat den engelske normative README.

## Centrale referencer

- [Normativ README](README.md)
- [SLeeLa-syntaksspecifikation](SLEELA.syntax)
- [Dokumentationsindeks](docs/README.md)
- [API-dokumentation](docs/api/README.md)
- [Platformunderstøttelse](docs/PLATFORM-SUPPORT.md)
- [Testpolitik](markdown/TESTING.md)
- [GitHub-repository](https://github.com/mearvk/SLeeLa)

**Oversættelsesstatus:** Denne danske udgave er udvidet med introduktion, startvejledning, bygning, installation, sprog, funktioner, arkitektur, biblioteker, sikkerhed, bidrag og centrale tekniske emner. Faglig sproglig gennemgang og løbende sammenligning med ændringer i den engelske README mangler fortsat.
