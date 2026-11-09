# SLeeLa 3.0 — Nederland (NL)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>

<p align="center"><strong>Een native systeemprogrammeertaal met compiler, runtime en ecosysteem van virtuele machines.</strong></p>

> **Internationale landeneditie.** Dit bestand bundelt de taalversies die voor Nederland zijn geselecteerd. De technische vertaling wordt stapsgewijs uitgevoerd. Vertaalde onderdelen zijn volledig opgenomen; onderdelen die nog moeten worden vertaald, zijn uitdrukkelijk gemarkeerd.

**The Now. The Ingredient.** Een programmeertaal is meer dan alleen de syntaxis: ook de compiler, runtime, bibliotheken, hulpmiddelen en platformcontracten die software bruikbaar maken, horen erbij.

**Werkterreinen van SLeeLa.** SLeeLa omvat **Math**, **Object**, **Science** en **Reach**, de vier dimensies van het werk: wat berekenbaar is (Math), het vastgestelde object in de catalogus van wat bestaat (Object), het gedisciplineerde onderzoek naar waarheid (Science) en het uitbreiden van de mogelijkheden van een systeem naar andere systemen en mensen (Reach). **Nationale belangen blijven bestaan**; geen van deze vier dimensies vervangt ze. Wanneer een onderwerp de belangen van een land raakt, blijft die relatie behouden en wordt deze zorgvuldig vastgelegd, bijvoorbeeld in de metadatavelden `metaOrigin` en `metaNationalSchedule` van een klasse.

## I. Aan de slag

- [De native implementatie bouwen](impl/README.md)
- [Virtuele machines en Modium — praktische gebruikershandleiding](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [CPU-stack in de SLeeLa-broncode](lib/cpu/CPU.md): een volledige machineketen van logische poorten via ALU, cores en besturingssysteem tot programma's; met meer dan 130 CPU-profielen en instelbare [spanningen, variabele kloksnelheden en geavanceerde timingparameters](lib/cpu/TIMING.md), voor uitvoering [volgens specificatie of met overklokken](lib/cpu/components/README.md).
- [Documentatie-index](docs/README.md)
- [CSPML-gegevensbron en onderzoeksreferentie Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md): herkomst en integratierichtlijnen voor externe Amerikaanse gegevens over locaties, sociaaleconomische omstandigheden, arbeidskrachten, onderwijs en openbare scholen.
- [Gegenereerde API-documentatie](docs/api/README.md): actuele HTML-referentie, versiegebonden API-uitgave en instructies voor het opnieuw genereren.
- [API-catalogus](api/README.md)
- [Normen voor computationele vergelijkingen](comparison/README.md): normen voor typen, geheugen, ABI en conformiteit voor SLeeLa VM, Java/JVM en native C/C++.
- [Recursie-Stanchion — C- en C++-prototypes](recursion/stanchion/README.md): voorbeelden gericht op de primitieve typen Boolean, Byte, Character, Integer, Long, Float en Double, plus generieke, toegepaste, wetenschappelijke en educatieve recursiepatronen.
- [Matrix voor platformondersteuning en validatie](docs/PLATFORM-SUPPORT.md)
- [Merkrichtlijnen](docs/BRAND-GUIDE.md)
- [Merchandiseconcepten en grafische middelen](merchandise/README.md)
- [NitroWebExpress-logo](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Testbeleid en releasecriteria](markdown/TESTING.md)

## II. Nieuws

**Vertaling in behandeling.** De volledige wijzigingsberichten en functieaankondigingen uit sectie II van de Engelse README worden in een volgende batch vertaald. Normatieve bron: [README.md — sectie II](README.md).

## III. Bouwen en testen

**Vereisten:** een C/C++-toolchain (GCC of Clang) en GNU Make; Python 3 voor manifest- en workflowhulpmiddelen; op Linux ook de X11/Xrandr-ontwikkelpakketten voor weergaveproducten. Het commando `make deps` van een module meldt of installeert de vereiste afhankelijkheden. De build onder `impl/` stopt veilig wanneer niet aan het vertrouwde SHA-256-manifest wordt voldaan; het Makefile in de repository-root verwijst hier automatisch naar.

De snelste start vanuit een schone clone:

```sh
make                                  # alles bouwen vanuit de repository-root
```

Of alleen de native kern bouwen en testen:

```sh
make -C impl all                      # alleen de compiler- en runtimekern bouwen
bash ./test-suites/run-all.sh --all   # de volledige testsuite uitvoeren
```

Het commando `make` in de repository-root bouwt alle producten (kern, VM, codecs, churn, antivirus enzovoort) en maakt als laatste het **register** met de bijbehorende basistabel. Een gebouwde werkboom bevat daardoor direct een controleerbaar register.

Belangrijke doelen:

```sh
make                     # alles bouwen (alias voor make all; register als laatste)
make all                 # dezelfde volledige build, expliciet benoemd
sudo make install        # opdrachten en runtime-ondersteuningsbestanden installeren (zie § IV)
make clean               # builduitvoer van alle geïntegreerde buildsysteemonderdelen verwijderen
make help                # alle doelen van het root-Makefile tonen
```

Andere nuttige doelen:

```sh
make registry            # alleen het register bouwen en de tabel materialiseren
./build-registry.sh      # dezelfde bewerking via een rechtstreeks script
make tests               # de testsuite bouwen en uitvoeren
make ballum              # de Ballum-weergavebibliotheek voor dit besturingssysteem bouwen
bin/install-modules      # modules gefaseerd installeren/bouwen gedurende ongeveer 10–20 minuten
```

Het doel `registry` voert [`build-registry.sh`](build-registry.sh) uit: het bouwt de registerkern, schrijft de basistabel naar de draagbare tekstopslag `registry/build/sleela.sleelareg` en exporteert het MySQL-tabelscript `registry/build/registry.sql` (via `sleela registry export`). Het register kan worden gecontroleerd met `sleela registry` of `registry/build/sleela-registry review`. Zie de [registerhandleiding](registry/README.md) en de [Registry 3.0-architectuur](registry/REGISTRY-3.0-ARCHITECTURE.md).

Raadpleeg de build- en testhandleidingen voor vereisten en platformgebonden stappen. Een geslaagde build van één component bewijst niet dat elke bibliotheek of elk platform gereed is voor een release.

## IV. Installatie

Installeer na het bouwen van de native producten de opdrachten en runtime-ondersteuningsbestanden:

```sh
sudo make install
make install PREFIX="$HOME/.local"        # installatie voor alleen de gebruiker, zonder sudo
make install DESTDIR="$PWD/package-root" # installatie in een stagingmap voor pakketten
make install-check
```

Het conventionele installatieprogramma gebruikt standaard `/usr/local/bin`, `/usr/local/lib/sleela` en `/usr/local/share/sleela`. Bestaande opstartconfiguratie blijft behouden en bestaande opdrachten die niet door de repository worden bijgehouden, worden niet overschreven. Bij het verwijderen worden alleen door het installatieprogramma geregistreerde bestanden verwijderd; configuratie en niet-geregistreerde runtimegegevens blijven behouden.

Gebruik voor een gerichte, begeleide installatie van alleen het hoofdpakket — de binaries `sleela`, `sleelvac` en `nordshrift`, de standaardbibliotheek, protocollen en ondersteuningsmodules — het schone installatieprogramma:

```sh
make clean-install       # begeleide installatie van alleen het hoofdpakket
```

Eerst wordt gevraagd of het hoofdpakket moet worden geïnstalleerd; daarna wordt de omvang van de installatie gekozen. Bij succes kan optioneel een selectie van aanvullende modules worden aangeboden. De volledige interactieve procedure en alle opties staan beschreven in [Snelle en veilige installatie](install/README.md).

## V. Taal en runtime

De normatieve grammatica staat in [`SLEELA.syntax`](SLEELA.syntax). De repository documenteert momenteel syntaxis **3.0**; de versie van compiler/hulpmiddelen en de broncode-syntaxisversie zijn afzonderlijk. Syntaxis 3.0 voegt de documentmarkeringen **`pure` en `gasoline`** (§30) toe: twee contextuele, declaratieve Preonems zonder zelfstandig uitvoeringseffect, die elk de positie van een document uitdrukken door de eigen eerlijkheid ervan. Syntaxis 3.0 brengt de taallagen 1.0 tot en met 2.9 samen in één actuele normatieve editie en behoudt de minimale versievereisten per functie.

De voorgaande laag, 2.9, voegt **opeenvolgende `@mix`-velden** (§28) toe: een klasse mag meerdere instantievelden met dezelfde naam declareren wanneer elk veld is gemarkeerd met `@mix`. Ze worden behandeld als één geïndexeerde groep (standaard oplopend: `1,2,3,…`, of handmatig ingesteld via `@mix(1,2,3,...)`; het aantal indexen moet overeenkomen met het aantal velden). Ook opgenomen zijn de uitgebreide binaire gehele-getalfamilies met en zonder teken, van `int32` tot en met `int16777216` (§24) en van `uint32` tot en met `uint16777216` (§25), als uitbreiding op `long`, `int64` en `int128` (§21); documentatie over trefwoordondersteuning (§22); en de **Preonems**: declaratieve documentmarkeringen `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) en `artifact` (§35; een status van wetgevende kwaliteit die in relatie tot tijd en ruimte wordt beoordeeld). Samen worden ze Preonems genoemd (§26.1).

Overloadresolutie voor constructors en delegatie naar `this(...)`- en basisklasseconstructors vallen uitdrukkelijk buiten het geverifieerde syntaxiscontract 1.8. Raadpleeg de grammatica en implementatienotities voordat u op een functie vertrouwt. De negen SleelaDoc-broneenheden declareren nu `#sleela 3.0`; de gemeenschappelijke compilercontrole accepteert broncode-syntaxis 1.0 tot en met 3.0. De nieuwe API-documentatie wordt gegenereerd met `make api-docs`.

De repository bevat een native C/C++-implementatie, verwijzingen voor SLVM-generatie, standaardbibliotheekmodules, tests, voorbeelden en platformintegraties. De implementatie- en validatiestatus verschilt per module. Raadpleeg de toepasselijke specificatie en actuele CI-resultaten; ga niet uit van een uniform voltooiingsniveau.

## VI. Engineeringprincipes

- **Bewijs vóór beweringen:** maak duidelijk onderscheid tussen geïmplementeerd, getest, experimenteel en gepland gedrag.
- **Eén contract per grensvlak:** documenteer syntaxis, API's, bestandsindelingen, platformaannames en foutgedrag.
- **Portabiliteit vanaf het ontwerp:** isoleer platformspecifieke backends en test ze op native runners.
- **Veilig falen:** test ongeldige invoer, resourcelimieten, procesfouten en opruimpaden.
- **Reproduceerbare releases:** bouw opnieuw vanuit een schone omgeving en publiceer artefact-hashes en releasenotities.

## VII. Antivirusbuild en integriteit

Het antiviruspakket Sleelavirin omvat een native SHA-256-scanner, een optionele ClamAV-adapter, integriteitsmanifesten voor bronnen en indexen en controlesommen voor platformpakketten. Bouw en test het pakket vanuit de repository-root:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Het integriteitsmanifest wordt gegenereerd onder `antivirus/build/`. Het omvat bijgehouden broncode- en documentatiebestanden en beschermde bestanden met handtekeningindexen; gegenereerde uitvoer en de veranderlijke ClamAV-cache zijn uitgesloten. ClamAV-databasesnapshots hebben eigen manifesten. Release-ZIP-bestanden bevatten `INTEGRITY-MANIFEST.json`, `SHA256SUMS` en een bijbehorend `.sha256`-bestand. Vergelijk hashes van gedownloade archieven met een onafhankelijk verkregen vertrouwenswaarde: een controlesom naast een archief is geen uitgevershandtekening. Zie de [Sleelavirin-handleiding](antivirus/README.md), de [beveiligingschecklist](antivirus/SECURITY.md) en de [antivirus-buildworkflow](.github/workflows/antivirus-binaries.yml).

Een release geldt pas als geverifieerd wanneer de relevante CI-taken voor exact de commit van die release slagen en de resulterende artefacten zijn gecontroleerd.

## Nog te vertalen secties

De volgende secties zijn nog niet vertaald en worden in volgende batches toegevoegd: **II. Nieuws; VIII. SLeeLa-editorthema's; IX. Bijdragen; X–XVII. Overige projectgebieden; XVIII. Recente toevoegingen; XIX. Internationale README-edities.** Totdat de vertaling is voltooid, blijft de [canonieke Engelse README.md](README.md) de volledige referentie.

---
**Vertaalstatus:** gedeeltelijke vertaling — secties I en III–VII zijn vertaald. Technische identificaties, opdrachten, codevoorbeelden, bestandsnamen en links zijn ongewijzigd gebleven.
