# SLeeLa 3.0 — Norge (NO)

## Norsk utgave

SLeeLa er et systemprogrammeringsspråk med egen kompilator, kjøretidsmiljø og et økosystem av virtuelle maskiner. Prosjektet omfatter også biblioteker, utviklerverktøy, dokumentasjon, byggeprosesser og integrasjonsavtaler for støttede plattformer. Tekniske identifikatorer, kommandoer, filstier, filnavn, kodeeksempler og API-navn beholdes uendret slik at veiledningene kan brukes direkte.

**The Now. The Ingredient.** Et programmeringsspråk er mer enn syntaks: det omfatter kompilatoren, kjøretidsmiljøet, bibliotekene, verktøyene og plattformavtalene som gjør programvare anvendelig.

SLeeLa beskriver arbeidsområdene sine gjennom **Math**, **Object**, **Science** og **Reach**. Nasjonale interesser som er relevante for et emne, skal fortsatt være uttrykkelig registrert i metadata og skal ikke forsvinne som følge av denne inndelingen.

## I. Kom i gang

- [Bygg den native implementasjonen](impl/README.md).
- [Brukerveiledning for virtuelle maskiner og Modium](virtual-machine/USER-GUIDE.md), inkludert `sleela vm` og `sleela modium`.
- [CPU-arkitektur i SLeeLa-kildekode](lib/cpu/CPU.md): fra logiske porter og ALU til prosessorkjerner, operativsystem og programmer. Katalogen inneholder mer enn 130 CPU-profiler. Se også [tidtaking, spenninger og frekvenser](lib/cpu/TIMING.md).
- [Dokumentasjonsindeks](docs/README.md).
- [CSPML-datakilde og forskningsreferanse for Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md).
- [Generert API-dokumentasjon](docs/api/README.md) og [API-katalog](api/README.md).
- [Standarder for beregningsmessig sammenligning](comparison/README.md): typer, minne, ABI og samsvar mellom SLeeLa VM, Java/JVM og native C/C++.
- [C- og C++-referanseprototyper for Recursion stanchion](recursion/stanchion/README.md).
- [Plattformstøtte og valideringsmatrise](docs/PLATFORM-SUPPORT.md).
- [Merkevareveiledning](docs/BRAND-GUIDE.md).
- [Testpolicy og krav for utgivelser](markdown/TESTING.md).

## II. Funksjoner og nyheter

Standardbibliotekpakken **Advantage** (`lib/advantage/`) modellerer vurderingen av et utbytte i forhold til en modell. Den er organisert i lag med gjennomgang, hensyn, matematikk, tillegg, subtraksjon, omgruppering og iterasjon. Temaene omfatter bilateral handelsøkonomi, en strukturell fysikkanalogi for tillitsfelt (uttrykkelig merket som en modellanalogi), vertikal integrasjon, modulgradering og rutet utdata. Se [Advantage-dokumentasjonen](lib/advantage/README.md).

Andre dokumenterte funksjoner omfatter:

- **Løkketiming:** `@loopDelay` og `@loopBudget` angir pauser og tidsbudsjetter for løkker. De senkes til den append-only VM-instruksjonen `OP_SLEEP` (ISA 157), som også brukes av den generelle funksjonen `sleep(ms)`. Se [syntaksreferansen](SLEELA.syntax) §32 og [API-referansen](api/sleela/3.0/LOOP_TIMING_API.md).
- **SleelaDoc:** `sleela docs` viser dokumentasjon for emner, pakker og klasser. `sleela docs --generate [dir]` oppretter et lokalt tekstkorpus for frakoblet bruk. Se [SleelaDoc-biblioteket](lib/sleeladoc/README.md) og [API-dokumentasjonen](docs/api/README.md).
- **XML DOM:** `lib/xml` inneholder en dokumentmodell med element-, tekst-, kommentar-, CDATA- og prosesseringsinstruksjonsnoder, attributter, serialisering og parsing. Se [XML-dokumentasjonen](lib/xml/README.md).
- **Heltallstyper:** `int`, `long` og `int64` er 64-bits signerte heltall. `int128` godtas som heltallstype, men er foreløpig 64-bits-basert; dette er ikke det samme som en full 128-bits implementasjon.
- **`@mix`-felt:** gjentatte instansfelt med samme navn kan grupperes som en indeksbasert sekvens når hvert felt er merket med `@mix`. Se syntaks 2.9 og [SLEELA.syntax](SLEELA.syntax).
- **Preonems:** dokumentmarkørene `author`, `authorial`, `material;`, `memory;`, `determine;`, `object;` og `field;` beskriver dokumentets status og opprinnelse. De er deklarative markører, ikke tillatelser til å kjøre kode.
- **Nordshrift:** verktøyet bruker den rekursive `/lib`-indeksen og kan kontrollere pakker og `.sleela`-enheter.
- **Kommentarer med vinkelparenteser:** `< line >` og `<< block >>` er dokumenterte kommentarsyntakser.
- **Tall med grunntall:** heltallsliteraler støtter heksadesimale (`0x`), binære (`0b`) og oktale (`0o`) former med understrek som sifferseparator.
- **Metadata Class:** bibliotekets klassebærende kildemoduler bruker et metadata-mønster for proveniens, lovlighet og jurisdiksjonell rekkevidde.
- **Tekst og String:** `lib/text` inneholder tegncellebaserte operasjoner, blant annet `charAt`, `substring`, `indexOf`, `reverse` og posisjonell formatering.

Funksjonsbeskrivelser er dokumentasjon, ikke en generell garanti for at alle mål, plattformer eller moduler er ferdig validert. Kontroller alltid den gjeldende implementasjonen og testresultatene.

## III. Bygging og testing

Kravene avhenger av byggemålet. For den native implementasjonen trengs vanligvis en C/C++-verktøykjede, for eksempel GCC eller Clang, samt GNU Make. Python 3 brukes av enkelte manifest- og arbeidsflytverktøy. Grafiske Linux-komponenter kan kreve utviklingspakker for X11 og XRandR.

Fra rotkatalogen i depotet:

```sh
make
```

Bygg den native kjernen og kjør testserien:

```sh
make -C impl all
bash ./test-suites/run-all.sh --all
```

Vanlige Make-mål:

```sh
make all
make clean
make help
make registry
make api-docs
```

En vellykket bygging av én komponent beviser ikke at alle biblioteker eller plattformer er klare for produksjon. Testresultater skal skille mellom tester som er kjørt, hoppet over og mislyktes. Se [testpolicyen](markdown/TESTING.md) før du tolker en byggestatus som en utgivelsesgodkjenning.

## IV. Installasjon

Systemomfattende installasjon bruker vanligvis prefikset `/usr/local`:

```sh
sudo make install
```

For installasjon bare for gjeldende bruker, vanligvis uten administratorrettigheter:

```sh
make install PREFIX="$HOME/.local"
```

For å opprette en pakkekatalog og kontrollere installasjonen:

```sh
make install DESTDIR="$PWD/package-root"
make install-check
```

Følg installasjonsveiledningen for det valgte målet. En installasjon for én bruker krever at den aktuelle `bin`-katalogen er tilgjengelig i `PATH`. Ikke anta at en installasjonskommando har samme virkning på Linux, macOS og Windows; kontroller plattformveiledningen og de tilgjengelige installasjonsskriptene.

- [Installasjonsveiledning](install/README.md)
- [Plattformstøtte](docs/PLATFORM-SUPPORT.md)
- [Bygging av native implementasjon](impl/README.md)

## V. Språk, kompilator og kjøretidsmiljø

Den normative språkbeskrivelsen ligger i [SLEELA.syntax](SLEELA.syntax). SLeeLa 3.0-dokumentasjonen dekker språkets syntaks, kompilator, kjøretidsmiljø, virtuelle maskiner og biblioteker. Kompilatorversjon, syntaksversjon og VM-/ISA-versjon er separate begreper og må ikke blandes sammen.

En typisk verktøykjede består av:

1. **Kildekode:** `.sleela`-filer med språkversjon og deklarasjoner.
2. **Frontend:** leksikalsk analyse, parsing, symbolbehandling og semantiske kontroller.
3. **Mellomrepresentasjon:** strukturert representasjon som brukes i videre senking.
4. **Kodegenerering:** generering for støttede VM- eller mellomformatmål.
5. **Kjøretidsmiljø:** utførelse av kompilert innhold med minne- og kontrollmekanismer.
6. **Biblioteker og verktøy:** gjenbrukbare pakker, dokumentasjon, bygging og verifikasjon.

De nøyaktige funksjonene avhenger av den aktuelle implementasjonen. En språkfunksjon som er beskrevet i spesifikasjonen, skal ikke automatisk tolkes som implementert på alle mål.

## VI. C, C++ og native kompilering

SLeeLa-prosjektet inneholder C- og C++-komponenter og dokumenterer en avgrenset native vei. Markøren `@native` og kommandoen `sleela aot` beskriver et utviklingsområde for native generering. De gir ikke en generell tillatelse til å kjøre vilkårlig native kode.

Når SLeeLa kobles til C eller C++, må grensesnittet være eksplisitt. ABI-kontrakter, eierskap til minne, datatyper, feilhåndtering og plattformavhengigheter må dokumenteres og testes. Native kode skal behandles som en egen tillitsgrense; en referanse eller deklarasjon er ikke i seg selv en autorisasjon til å utføre kode.

Se [native implementasjon](impl/README.md), [plattformmatrisen](docs/PLATFORM-SUPPORT.md) og [sammenligningsstandardene](comparison/README.md).

## VII. Biblioteker og delsystemer

Depotet inneholder et bredt bibliotek- og verktøyområde. Viktige innganger omfatter:

- `lib/advantage` — Advantage-biblioteket og tilhørende definisjoner, matematikk, fysikk, integrasjon, gradering og utdata.
- `lib/xml` — XML-dokumentmodell og serialisering.
- `lib/text` — tekst- og strengoperasjoner.
- `lib/sleeladoc` — dokumentasjonsekstraksjon og CLI-støtte.
- `lib/cpu` — CPU-modeller, komponenter og tidsreferanser.
- `lib/xlt` — CPU-modellformat og tilhørende dokumentasjon.
- `recursion/stanchion` — C- og C++-referanseprototyper og eksempler.
- `virtual-machine` — VM-veiledninger og Modium-materiale.
- `api` og `docs/api` — API-katalog og generert referanse.

Et katalogoppføringspunkt betyr ikke nødvendigvis at modulen er produksjonsklar. Kontroller modulens egen README, testdekning, plattformbegrensninger og valideringsdokumentasjon før bruk.

## VIII. Integritet, sikkerhet og verifikasjon

SLeeLa legger vekt på sporbarhet mellom kildekode, dokumentasjon, bygging og testresultater. Hash-manifester og verifikasjonsverktøy kan brukes til å kontrollere at filer samsvarer med forventet innhold. En bestått kontroll gjelder bare det angitte omfanget og det tidspunktet kontrollen ble kjørt.

Viktige prinsipper:

- Referanser identifiserer ressurser; de gir ikke i seg selv tillatelse til å utføre dem.
- Native grensesnitt skal være eksplisitte og begrensede.
- Testresultater skal rapportere pass, fail og skip separat.
- Plattformpåstander skal knyttes til konkrete bygg og tester.
- Genererte dokumenter skal kunne spores tilbake til kilde og generator.
- Sikkerhetskontroller skal feile lukket når nødvendig integritetsinformasjon mangler.

Se [testpolicyen](markdown/TESTING.md), [plattformstøtten](docs/PLATFORM-SUPPORT.md) og [installasjonsveiledningen](install/README.md).

## IX. Redaktører og utviklingsarbeid

Depotet inneholder integrasjoner og veiledning for utviklingsredaktører. Se [Linux-editorer](linux-editors/README.md) for tilgjengelige Vim- og Emacs-ressurser. Kontroller at instruksjonene passer til operativsystemet og redaktørversjonen før installasjon.

En anbefalt arbeidsflyt for bidrag:

1. Les dokumentasjonsindeksen og dokumentasjonen for modulen du skal endre.
2. Avgrens endringen og beskriv hvorfor den er nødvendig.
3. Legg til positive tester og negative tester.
4. Kjør relevante tester, og kjør hele testserien når endringens omfang krever det.
5. Registrer operativsystem, verktøykjede, kommandoer og faktiske resultater.
6. Oppdater README og API-dokumentasjon når offentlig oppførsel endres.
7. Ikke merk tester som bestått dersom de ble hoppet over eller ikke ble kjørt.

## X. Flere prosjektområder

SLeeLa-depotet omfatter også CPU- og maskinvaremodeller, grafiske grensesnitt, nettverk, tekstbehandling, dokumentasjon, internasjonalisering og eksperimentelle kataloger. Disse områdene kan ha forskjellige modenhetsnivåer. En eksperimentell katalog eller et eksempel skal ikke omtales som en stabil produksjonsfunksjon uten dokumentert validering.

UTF-4088-katalogen, π-verktøyene, Modium, Registry og `sleelactl` har egne innganger og tekniske forutsetninger. Se den kanoniske README-en og modulenes dokumentasjon for oppdatert status.

## XI. API-dokumentasjon og referanser

- [Generert API-dokumentasjon](docs/api/README.md)
- [API-katalog](api/README.md)
- [SleelaDoc](lib/sleeladoc/README.md)
- [Syntaksreferanse](SLEELA.syntax)
- [Dokumentasjonsindeks](docs/README.md)
- [CPU-dokumentasjon](lib/cpu/CPU.md)
- [CPU-timing](lib/cpu/TIMING.md)

## XII. AutoCAD Creator™

AutoCAD Creator™ er omtalt i prosjektets tekniske katalog. Les den tilhørende moduldokumentasjonen for å skille mellom beskrevet funksjonalitet, eksempler og verifiserte leveranser. Ikke anta kompatibilitet med en bestemt AutoCAD-versjon uten en uttrykkelig kompatibilitetsmatrise og testresultater.

## XIII. UTF-4088 eksperimentell katalog

UTF-4088-materialet er et eksperimentelt tegn- og katalogområde i prosjektet. Størrelser, oppføringer, minnebehov og formatdetaljer må leses sammen med den aktuelle implementasjonen og dens dokumentasjon. At en katalog inneholder et stort antall oppføringer, er ikke i seg selv bevis på full Unicode-kompatibilitet eller produksjonsmodenhet.

## XIV. π, Modium, Registry og sleelactl

Prosjektet inneholder separate verktøy og dokumentasjonsområder for π-generering, Modium, Registry og `sleelactl`. De har ulike formål og skal bygges og kontrolleres etter sine egne instruksjoner. Se den kanoniske README-en og de respektive katalogene for gjeldende kommandoer og begrensninger.

## XV. Internasjonale README-utgaver

Landutgavene er lokale innganger til SLeeLa-dokumentasjonen. De skal gjøre sentrale tekniske emner forståelige på lokale språk, samtidig som tekniske identifikatorer, kommandoer, filnavn, kode og lenker bevares. En landutgave skal ikke kalles fullstendig oversatt før alle relevante tekniske deler er oversatt og kontrollert.

Norsk bokmål brukes i denne utgaven. Den er ikke ment å hevde at nynorsk, samiske språk eller norsk tegnspråk er fullstendig dekket. Bidrag til flere norske språkformer og tilgjengelige formater er velkomne.

## Viktige referanser

- [Kanonisk README på engelsk](README.md)
- [SLeeLa-syntaks](SLEELA.syntax)
- [Dokumentasjonsindeks](docs/README.md)
- [API-dokumentasjon](docs/api/README.md)
- [Plattformstøtte](docs/PLATFORM-SUPPORT.md)
- [Native implementasjon](impl/README.md)
- [Installasjonsveiledning](install/README.md)
- [Testpolicy](markdown/TESTING.md)
- [SLeeLa på GitHub](https://github.com/mearvk/SLeeLa)

**Oversettelsesstatus:** Denne norske utgaven er utvidet til en teknisk introduksjon som dekker oppstart, bygging, installasjon, språk og kjøretidsmiljø, native integrasjon, biblioteker, sikkerhet, utviklingsarbeid og sentrale prosjektområder. Den er ikke en linje-for-linje-oversettelse av samtlige detaljer i den kanoniske engelske README-en. Teknisk kontroll og språklig gjennomgang av en norskspråklig fagperson anbefales før utgaven betegnes som endelig.
