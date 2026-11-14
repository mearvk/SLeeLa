<img align="right" src="../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — Nederland (NL)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Een native systeemtaal, compiler, runtime en ecosysteem van virtuele machines.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="../../docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Internationale landeneditie.** Dit bestand bevat de Nederlandstalige editie voor Nederland. Technische identifiers, opdrachten, code, bestandsnamen en links zijn ongewijzigd gebleven.

**The Now. The Ingredient.** Een taal is meer dan syntaxis: het zijn de compiler, de runtime, de bibliotheken, de hulpmiddelen en de platformcontracten die software bruikbaar maken.

**Waarmee SLeeLa zich bezighoudt.** SLeeLa houdt zich bezig met **Math**, **Object**, **Science** en **Reach** — de vier gezichten van het werk: het berekenbare (Math), het vastgestelde ding dat in de catalogus van wat bestaat staat (Object), de gedisciplineerde studie van het ware (Science) en de uitbreiding van de hand van een systeem naar andere systemen en mensen (Reach). **Nationale belangen blijven bestaan** — geen van de vier zet ze opzij; waar een onderwerp een nationaal belang raakt, blijft die positie behouden (getrouw vastgelegd, bijvoorbeeld in de Metadata-velden `metaOrigin` / `metaNationalSchedule` van elke klasse) en wordt ze nooit opgelost.

## I. Aan de slag

- [De native implementatie bouwen](../../impl/README.md)
- [Virtuele machines en Modium — praktische gebruikershandleiding](../../virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [CPU-stack in SLeeLa-broncode](../../lib/cpu/CPU.md) — een volledige machine van poorten → ALU → cores → besturingssysteem → programma's, een lijst van meer dan 130 CPU-profielen en instelbare [spanningen, variabele klokken en expert-timings](../../lib/cpu/TIMING.md) die u [op specificatie of overgeklokt kunt uitvoeren](../../lib/cpu/components/README.md)
- [Documentatie-index](../../docs/README.md)
- [CSPML-gegevensbron en onderzoeksreferentie Salt & Salary](../../CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — herkomst en integratierichtlijnen voor externe Amerikaanse gegevens over plaatsen, sociaaleconomische omstandigheden, beroepsbevolking, onderwijs en openbare scholen.
- [Gegenereerde API-documentatie](../../docs/api/README.md) — actuele HTML-referentie, versiegebonden API-editie en instructies voor het opnieuw genereren
- [API-catalogus](../../api/README.md)
- [Normen voor computationele vergelijking](../../comparison/README.md) — normen voor typen, geheugen, ABI en conformiteit van de SLeeLa VM, Java/JVM en native C/C++.
- [Recursie-stanchion — C- en C++-prototypes](../../recursion/stanchion/README.md) — voorbeelden gespecialiseerd voor Boolean, Byte, Character, Integer, Long, Float en Double, naast generieke, toegepaste, wetenschappelijke en didactische recursiepatronen.
- [Platformondersteuning en validatiematrix](../../docs/PLATFORM-SUPPORT.md)
- [Merkgids](../../docs/BRAND-GUIDE.md)
- [Merchandiseconcepten en -materiaal](../../merchandise/README.md)
- [NitroWebExpress-logo](../../images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Testbeleid en releasecriteria](../../markdown/TESTING.md)

## II. Nieuws

Recente toevoegingen aan de taal, de drivers en de standaardbibliotheek:

- **Het standaardbibliotheekpakket `advantage` — een dividend benutten, met wiskunde, natuurkunde, integratie, beoordeling en routeerbare uitvoer.** [`lib/advantage/`](../../lib/advantage/README.md) kapselt *het benutten van een uniek dividend ten opzichte van een model* in als een gelaagde **Sequitur onion** (dividend → beoordeling → aandachtspunten → wiskunde → toevoegingen → aftrek → hergroepering → iteratie), naar binnen en naar buiten te doorlopen, met volledige dynamische bijwerking en herstel (restitute), beschikbaar via een eenvoudig **gebruikersmodel** en een **ontwikkelaarsmodel** met volledige controle. Het omvat vijf vakgebieden: **`math/`** bilaterale handelseconomie (gezamenlijke winstmaximalisatie, Nash-onderhandelingen, de kritieke ratio van de bilaterale newsvendor), **`physics/`** een structureel-economische „natuurkunde van vertrouwensvelden” (vertrouwen als metrische tensor, de transactionele geodeet, een aankondigingscontactstructuur, een CIR+sprong-SDE voor vertrouwensverval — een modelanalogie, als zodanig aangeduid), **`integration/`** een recursief samenstelbaar model van verticale integratie voor kapitalisme/detailhandel (compartimenten, engineeringbays, proefcompartimenten, newton-mediaanbakken en een systeemverfijningsscore), **`grading/`** een verfijningscircuitbeoordelaar die elke module en elke verzameling winnende/verliezende/beoordelende/absolute modules van A tot D beoordeelt, en **`output/`** een routeerbare emissienaad die moduluitvoer naar de terminal, een **CommonRails**-geformatteerd oppervlak of een socket/connector stuurt (geframed voor een native `slnet_write`). Elke klasse draagt het Metadata-Class-record; vijf uitvoerbare demo's valideren de vakgebieden, en de bijbehorende [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md), [`MATH.md`](../../lib/advantage/MATH.md) en [`PHYSICS.md`](../../lib/advantage/PHYSICS.md) documenteren ze. Een native C/C++-consument onder [`advantage/`](../../advantage/README.md) (de C-kern `advantage_value`/`advantage_mean`, een XML-kennisopslag, een beveiligde HTTPS/SFTP/SSH-verwervings- + sleutelopslag- + DH/ECDH-transportbibliotheek) koppelt het model aan uitgewerkte [handelsvoorbeelden](../../advantage/examples/README.md) — positieve transacties uit vermeende internetbronnen, een relevantiereeks voor het publiek (Handelaren, Consumenten, Helpers, Genezers) en acht modellen gerangschikt van A tot D (D = meest Cement, A = meest Brand).
- **Lustijdannotaties — `@loopDelay` / `@loopBudget` (+ een ingebouwde functie `sleep(ms)`).** De eerste **gedragsmatige annotaties op instructieniveau** in SLeeLa: ze worden op een eigen regel vóór een `for`-, `while`- of `do`-lus geschreven en voegen een echte pauze toe aan de uitvoering van de lus. `@loopDelay(ms = 250)` / `@loopDelay(seconds = 2)` slaapt per iteratie een vaste tijd (een kale `@loopDelay(250)` gebruikt standaard milliseconden); `@loopBudget(seconds = 10, iterations = 1000)` spreidt een bekend aantal iteraties gelijkmatig over een totaal klokbudget. Beide worden vertaald naar de nieuwe, uitsluitend toegevoegde opcode **`OP_SLEEP`** (ISA 157), die ook een algemene, overal aanroepbare ingebouwde functie **`sleep(<ms>)`** ondersteunt; de pauze is draagbaar geïmplementeerd (`nanosleep` / `Sleep`). Gespecificeerd in [`SLEELA.syntax` §32](../../SLEELA.syntax); API-referentie in [`api/sleela/3.0/LOOP_TIMING_API.md`](../../api/sleela/3.0/LOOP_TIMING_API.md); uitvoerbaar [`examples/loop-timing-annotation-demo.sleela`](../../examples/loop-timing-annotation-demo.sleela). Een index van sessietoevoegingen (waaronder deze, de native grote-gehele-getallenvermenigvuldiging `OP_BIGMUL` en de zelfstandige hulpmiddelen **[local-generator](../../local-generator/README.md)** en **[pi-generator](../../pi-generator/README.md)**) staat in [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](../../api/sleela/3.0/SESSION-ADDITIONS-API.md). De drie gegenereerde API-oppervlakken ([`sleeladoc-api/`](../../sleeladoc-api/README.md), [`docs/api/`](../../docs/api/README.md) en de onveranderlijke momentopname [`api/sleela/3.0/version-014/`](../../api/sleela/3.0/version-014/index.html)) zijn opnieuw opgebouwd met `make api-docs` en bevatten nu de nieuwe instructievermeldingen `SLISASleep` / `SLISABigMul` op hun pakketpagina `vm`.
- **SleelaDoc vanaf de opdrachtregel — `sleela docs`.** SleelaDoc is nu rechtstreeks vanuit de terminal bereikbaar. `sleela docs` toont alle documenteerbare onderwerpen; `sleela docs crypto` toont een pakket; `sleela docs stringbuilder` herleidt een klasse (hoofdletterongevoelig en via het `SL`-voorvoegsel van de standaardbibliotheek → `SLStringBuilder`) en drukt een uitgebreide pagina af — **definitie, ontwerpdoel, relaties, structuur, gebruikersconcepten en een beheersingsladder met vijf treden** — met gerangschikte „bedoelde u”-resultaten bij een bijna-treffer. Auteurs verrijken elke pagina eenvoudig door SleelaDoc-`@`-tags toe te voegen (`@intent`, `@thesis`, `@axiom`, `@param`, `@see`, `@mastery`). `sleela docs --generate [dir]` schrijft een platte-tekstcorpus (één pagina per klasse + een `TOPICS.index`) dat de CLI vervolgens offline leest; **`make sleeladoc`** bundelt dat corpus **en de bijbehorende `.sleela`-bronnen** in `sleeladoc/build/sleeladoc.jar`. Voor uitsluitend-façadeonderwerpen (bijv. `psychiatry`, `politico`) doorzoekt `sleela docs --facade <topic>` tijdens de uitvoering het bestandssysteem en toont de werkelijke **mappen (daarna bestanden, als natuurlijke beweging)**, begrensd door door de gebruiker te overschrijven limieten van **6 MB / 6000 bestanden / 6 minuten**. De `lib/sleeladoc`-eenheden worden onderhouden in de huidige editie, **`#sleela 3.0`**. Zie [`man/man1/sleela.1`](../../man/man1/sleela.1) en [`lib/sleeladoc/README.md`](../../lib/sleeladoc/README.md).
- **SleelaDoc — een documentatieoplossing in Javadoc-stijl, met een gegenereerde HTML-site.** **SleelaDoc** is de conventie van SLeeLa voor documentatiecommentaar in de broncode (commentaar tussen punthaken, afgebakend met `@doc` / `@page`; zie [`manifesto/SLEELADOC.md`](../../manifesto/SLEELADOC.md)), en [`lib/sleeladoc`](../../lib/sleeladoc/README.md) is de **extractor** die een `.sleela`-eenheid weergeeft als Markdown- of HTML-API-pagina — zoals `javadoc` Java weergeeft, met documentatie van de *bedoeling*. Een complete, doorbladerbare **HTML-documentatiesite voor de hele `/lib`** wordt door `tools/generate-sleeladoc-api.py` gegenereerd in [`/sleeladoc-api`](../../sleeladoc-api/README.md) (open `sleeladoc-api/index.html`; een schone stijl in blauw / lichtblauw / lichtgrijs / wit met een hiërarchie pakket → klasse → lid en een woordenlijst geschreven voor de lezer *Student 2*). De compiler produceert bovendien als bijproduct van de compilatie door de auteur gemarkeerde documentatie-uitvoer (een MediaWiki-pagina via `--wiki` + een afgeleide brontweeling), gedocumenteerd in [`manifesto/SLEELADOC.WIKI.md`](../../manifesto/SLEELADOC.WIKI.md). Uitvoerbaar: [`lib/sleeladoc/sleeladoc-demo.sleela`](../../lib/sleeladoc/sleeladoc-demo.sleela).
- **XML Document Object Model (`lib/xml`).** Een XML-DOM aan SLeeLa-zijde — een document dat een boom van element-/tekst-/commentaar-/CDATA-/PI-knooppunten bezit, met geordende kinderen en benoemde attributen, een serializer (compact/opgemaakt, XML 1.0 §2.4-escaping), een parser op tekencelbasis en het kwaliteitsregister `SLXmlAspect`. Knooppuntgelijkheid is een driestaps vertrouwensband (identiteit → hetzelfde onderwerp → structuur), reflexief, symmetrisch en deterministisch. Zie [`lib/xml/README.md`](../../lib/xml/README.md) en de uitvoerbare [`lib/xml/xml-dom.sleela`](../../lib/xml/xml-dom.sleela).
- **Brede gehele primitieven met teken (`long`, `int64`, `int128`).** `int`, `long` en `int64` zijn allemaal 64-bits gehele getallen met teken die identiek worden getypecontroleerd en uitgevoerd; `int128` wordt geaccepteerd als geheel type (voorlopig 64-bits ondersteund, met echte 128 bits als gedocumenteerde vervolgstap). De binaire reeks met teken `int32`..`int16777216` (syntaxis 2.7) en de bijbehorende reeks zonder teken `uint32`..`uint16777216` (syntaxis 2.8) breiden dit uit. Zie [`SLEELA.syntax`](../../SLEELA.syntax) §21, §24, §25.
- **`@mix` opeenvolgende velden (syntaxis 2.9).** Een klasse mag meerdere instantievelden met **dezelfde naam** declareren wanneer elke herhaling `@mix` draagt; de taal begrijpt de groep als opeenvolgend (een array / lijst op index), standaard van boven naar beneden `1,2,3,…` of handmatig via `@mix(1,2,3,...)`, waarvan het aantal indexen moet overeenkomen met het aantal velden. Zie [`SLEELA.syntax`](../../SLEELA.syntax) §28.
- **Documentmarkeringen — het Natuurlijke Proces, getiteld de Preonems (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).** Deze kale documentmarkeringswoorden heten gezamenlijk de **Preonems** (één markering is een *Preonem*; de verzameling, de *Preonems* — zie [`SLEELA.syntax`](../../SLEELA.syntax) §26.1). Eersteklas, uitsluitend declaratieve documentmarkeringen die benoemen wat een document *is* in de volgorde van het maken: `author`/`authorial` (syntaxis 2.4; behandeld door een beroepsraad — het enige lid met een gedragseffect), `material;` (vastgestelde Bron) en `memory;` (syntaxis 2.5), `determine;` (syntaxis 2.6; een document dat openstaat om door een arts, geneesheer of geest verder te worden verwerkt tot voltooiing door wettige hand) en de twee nieuwste, **`object;`** (een vastgesteld ding dat in de catalogus van wat bestaat staat — **gereïficeerd, met concreet bestaansrecht, in syntaxis 2.8**) en **`field;`** (de grond van betrekking waarin objecten staan en bewegen) — beide afgeschermd vanaf syntaxis 2.6. Elk is kaal, contextueel en door de front-end versiegebonden, zonder effect op parsing, typecontrole of codegeneratie; buiten de markeringspositie blijft elk een gewone identifier (zodat `field` nooit een klasselid of een `.field`-toegang verstoort). Zie [`SLEELA.syntax`](../../SLEELA.syntax) §18–§20, §23 en §26.
- **Nordshrift met syntaxis 3.0 en volledig bibliotheekbereik.** Nordshrift lost namen op via dezelfde recursieve `/lib`-index als de compiler en bereikt elk pakket en elke `.sleela`-eenheid; `nordshrift check` meldt de aantallen voor de hele boom.
- **Recursie-stanchion.** De map [`recursion/stanchion/`](../../recursion/stanchion/README.md) bevat nu speciale C- en C++-referentieprototypes voor de zeven gedocumenteerde primitieve contracten, plus generieke, toegepaste, wetenschappelijke en didactische voorbeelden. Dit zijn referentiebronnen; compileer en test ze voordat u ze als geverifieerd beschouwt.

- **Commentaar met punthaken (`<` / `<<`).** Een nieuwe commentaarstijl voor de toekomst — `< regel >` en `<< blok >>` — lexicaal verwerkt door de SLeeLa-frontend en gedocumenteerd in [`SLEELA.syntax`](../../SLEELA.syntax) §1.1.
- **Gehele literalen met radix.** `int`-literalen accepteren nu hexadecimale (`0x`), binaire (`0b`) en octale (`0o`) vormen met `_` als cijferscheidingsteken, zowel in de SLeeLa-brontaal ([`SLEELA.syntax`](../../SLEELA.syntax) §1.4) als in het [`.xlt`-CPU-modelformaat](../../lib/xlt/README.md).
- **De Metadata Class-standaard in de hele bibliotheek.** Het metapatroon van beschrijvende metadata (herkomst, rechtmatigheid, jurisdictioneel bereik) wordt nu gedragen door elk klassendragend `.sleela`-bestand onder `/lib`; zie [`SLEELA.syntax`](../../SLEELA.syntax) §15 en [de tutorialles](../../tutorial/13-metadata-class.md).
- **Een rijkere tekst-/String-familie.** `lib/text` krijgt echte hulpfuncties in de taal zelf, een tekencelweergave met echte `charAt`/`substring`/`indexOf`/`reverse`/kolomuitlijning en een echte positionele sjabloonverwerker — zie de [tutorial over tekst en String](../../tutorial/14-text-and-strings.md) en de uitvoerbare [`lib/text/text-string-demo.sleela`](../../lib/text/text-string-demo.sleela).
- **`String` als eersteklas type in Nordshrift-sheets.** Een `.sst`-sheet kan een getypeerd `strings:`-blok declareren dat bijdraagt aan het componentenmanifest van de build; zie de [Nordshrift Sheet Reference](../../wiki/Nordshrift-Sheet-Reference.md).
- **CPU-modellen die zowel via opcodes als via functieaanroepequivalentie draaien.** Elke `SL<ARCH>CPU` erft de grondslag om een native instructiestroom via zijn fetch-decode-execute-lus uit te voeren *en* om de code van een verondersteld ander proces uit te voeren via gelijkwaardige aanroepen op het gedeelde VM-substraat — zie [`lib/cpu/EXECUTION.md`](../../lib/cpu/EXECUTION.md) §5.

## III. Bouwen en testen

**Vereisten:** een C/C++-toolchain (GCC of Clang) en GNU Make; Python 3 voor de manifest- en flowtooling; onder Linux de X11/Xrandr-ontwikkelpakketten voor de weergaveproducten (`make deps` van een module meldt/installeert wat nodig is). De build in `impl/` is fail-closed op een vertrouwd SHA-256-manifest, waar het Makefile in de hoofdmap automatisch naar verwijst.

Vanaf een schone checkout is dit de snelste start:

```sh
make                                  # alles bouwen vanuit de repository-root
```

Of bouw en test alleen de native kern:

```sh
make -C impl all                      # alleen de kerncompiler/runtime bouwen
bash ./test-suites/run-all.sh --all   # de volledige testsuite uitvoeren
```

De `make` in de repository-root bouwt elk product (kern, VM, codecs, churn, antivirus, …) en eindigt met het bouwen van het **register** en het materialiseren van de basistabel — zodat een gebouwde boom direct een controleerbaar register meelevert. De belangrijkste doelen:

```sh
make                     # alles bouwen (alias van `make all`; register als laatste)
make all                 # dezelfde volledige build, expliciet benoemd
sudo make install        # opdrachten + runtime-ondersteuning installeren (zie §IV)
make clean               # buildresultaten van elk aangestuurd buildsysteem verwijderen
make help                # elk doel van het Makefile in de hoofdmap tonen
```

Andere nuttige doelen:

```sh
make registry            # alleen het register: bouwen + de tabel materialiseren
./build-registry.sh      # hetzelfde, rechtstreeks aangeroepen
make tests               # de testsuite bouwen en uitvoeren
make ballum              # de Ballum-weergavebibliotheek voor dit besturingssysteem bouwen (zie ballum/)
bin/install-modules      # gefaseerde installatie/build van elke module in ~10-20 min (zie bin/)
```

Het doel `registry` voert [`build-registry.sh`](../../build-registry.sh) uit: het bouwt de registerkern, zet de basistabel in de draagbare tekstopslag `registry/build/sleela.sleelareg` en exporteert het MySQL-tabelscript `registry/build/registry.sql` (via `sleela registry export`). Bekijk het met `sleela registry` of `registry/build/sleela-registry review`. Zie de [registerhandleiding](../../registry/README.md) en de [Registry 3.0-architectuur](../../registry/REGISTRY-3.0-ARCHITECTURE.md).

Raadpleeg de build- en testhandleidingen voor vereisten en platformspecifieke stappen. Een geslaagde build van één component bewijst niet dat elke bibliotheek of elk platform gereed is voor release.

## IV. Installatie

Installeer na het bouwen van de native producten de opdrachten en runtime-ondersteuningsbestanden:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # lokale installatie voor de gebruiker, zonder sudo
make install DESTDIR="$PWD/package-root" # gefaseerde pakketinstallatie
make install-check
```

Het conventionele installatieprogramma gebruikt standaard `/usr/local/bin`, `/usr/local/lib/sleela` en `/usr/local/share/sleela`. Het behoudt een bestaande opstartconfiguratie en weigert niet-getrackte bestaande opdrachten te overschrijven. Verwijderen wist alleen de door het installatieprogramma vastgelegde bestanden en laat configuratie en niet-getrackte runtimegegevens ongemoeid.

Voor een gerichte, begeleide installatie van alleen het hoofdpakket — de binaire bestanden `sleela`, `sleelvac` en `nordshrift`, de standaardbibliotheek, protocollen en ondersteunende modules — gebruikt u het schone installatieprogramma:

```sh
make clean-install       # begeleide installatie van alleen het hoofdpakket
```

Het vraagt om één hoofdbeslissing (het hoofdpakket installeren), daarna om een keuze van het bereik en bij succes om een optionele keuzelijst met geselecteerde uitbreidingsmodules. Zie [Quick and Safe Install](../../install/README.md) voor de interactieve implementatieprocedure en alle installatieopties.

## V. Taal en runtime

De gezaghebbende grammatica is [`SLEELA.syntax`](../../SLEELA.syntax). De huidige repository documenteert syntaxis **3.0**; de versie van compiler/tools en de versie van de brontaalsyntaxis zijn verschillend. Het 3.0-oppervlak voegt de **documentmarkeringen `pure` en `gasoline`** toe (§30) — een paar kale, contextuele, uitsluitend declaratieve Preonems die vrijwel hetzelfde betekenen en die elk de status van het document markeren door de eigen oprechtheid. Syntaxis 3.0 bundelt het taaloppervlak 1.0–2.9 tot de huidige normatieve editie en behoudt alle functiespecifieke minimumdrempels. Het voorgaande 2.9-oppervlak voegt **opeenvolgende `@mix`-velden** toe (§28) — een klasse mag meerdere instantievelden met dezelfde naam declareren wanneer elk veld `@mix` draagt, te begrijpen als één geïndexeerde groep (van boven naar beneden `1,2,3,…` of handmatig `@mix(1,2,3,...)`, waarvan het aantal indexen met de groep moet overeenkomen). Het zet de basis-2-families van brede gehele getallen met en zonder teken voort, `int32`..`int16777216` (§24) en `uint32`..`uint16777216` (§25), bovenop de eerdere `long`/`int64`/`int128` (§21); **documenten ter ondersteuning van trefwoorden** (§22); en de **Preonems** — de uitsluitend declaratieve documentmarkeringen `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) en `artifact` (§35, wetgevingswaardige status gegradeerd naar tijd en ruimte), gezamenlijk de Preonems genoemd (§26.1). Overload-resolutie van constructors en delegatie via `this(...)`/basisconstructors vallen uitdrukkelijk buiten het geverifieerde syntaxiscontract 1.8. Raadpleeg de grammatica en de implementatienotities voordat u op een functie vertrouwt. De negen broneenheden van SleelaDoc declareren nu `#sleela 3.0`; de gedeelde compilerpoort accepteert brontaalsyntaxis 1.0–3.0. De nieuwe API-build wordt geproduceerd met `make api-docs`.

De repository bevat een native C/C++-implementatie, SLVM-generatiereferenties, standaardbibliotheekmodules, tests, voorbeelden en platformintegraties. De implementatie- en validatiestatus verschilt per module; raadpleeg de specificatie en de actuele CI-resultaten in plaats van uit te gaan van uniforme volledigheid.

## VI. Engineeringprincipes

- **Bewijs boven beweringen:** onderscheid geïmplementeerd, getest, experimenteel en gepland gedrag.
- **Eén contract per grens:** documenteer syntaxis, API's, bestandsformaten, platformaannames en foutgedrag.
- **Draagbaar door ontwerp:** isoleer platformspecifieke backends en test op native runners.
- **Veilig falen:** test misvormde invoer, resourcelimieten, procesfouten en opruimpaden.
- **Reproduceerbare releases:** bouw schoon opnieuw en publiceer artefacthashes en releasenotities.

## VII. Antivirusbuild en integriteit

Het antiviruspakket Sleelavirin bevat een native SHA-256-scanner, een optionele ClamAV-adapter, integriteitsmanifesten voor bron en index en controlesommen voor platformpakketten. Bouw en test het pakket vanuit de repository-root:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Het integriteitsmanifest wordt gegenereerd onder `antivirus/build/`; het omvat gevolgde bron- en documentatie-invoer plus beschermde handtekeningindexbestanden, en sluit gegenereerde uitvoer en de veranderlijke ClamAV-cache uit. Momentopnamen van ClamAV-databases hebben hun eigen manifesten. Release-ZIP's bevatten `INTEGRITY-MANIFEST.json`, `SHA256SUMS` en een `.sha256`-zijbestand. Controleer hashes van gedownloade archieven aan de hand van een onafhankelijk verkregen, vertrouwde waarde: een controlesom die naast een archief is opgeslagen is geen handtekening van de uitgever. Zie de [Sleelavirin-handleiding](../../antivirus/README.md), de [beveiligingschecklist](../../antivirus/SECURITY.md) en de [antivirus-buildworkflow](../../.github/workflows/antivirus-binaries.yml).

Een release geldt pas als geverifieerd wanneer de relevante CI-taken op de exacte releasecommit zijn geslaagd en de resulterende artefacten zijn gecontroleerd.


## VIII. SLeeLa-editorthema's — Vim en Emacs

SLeeLa bevat een samenhangende reeks professionele donkere kleurenschema's voor **Vim** en **Emacs**, plus een native C/C++-paletprogramma. Het oorspronkelijke schema **American Professional** blijft de standaard. Tien aanvullende paletten zijn beschikbaar, geïnspireerd op uiteenlopende culturele en institutionele ontwerptradities; dit zijn interpretatieve ontwerppaletten, geen officiële weergaven van enig land, enige cultuur, rechtbank of instelling.

### De elf schema's en hun kleuren

| Schema | Achtergrond | Voorgrond | Trefwoorden | Typen | Strings | Commentaar |
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

#### Nationale vlagkleuren (bronpalet)

De Griekse en Turkse schema's zijn afgeleid van de **officiële nationale vlagkleuren** van beide landen, hier getoond met hun exacte gepubliceerde hexwaarden. De achtergronden en accenten van de bovenstaande schema's zijn verdonkerde, voor editors geschikte interpretaties van deze bronkleuren — de onderstaande tabel is de referentie van de „kleuren zoals nationaal vastgesteld”.

| Land | Kleur | Hex | Grondslag en betekenis |
|---|---|---|---|
| **Griekenland** | Blauw | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | Blauw van de nationale vlag (wet 851/1978). De hemel en de zeeën; heraldisch omschreven als *azuur*. |
| **Griekenland** | Wit | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Wit van de nationale vlag. De zuiverheid van de onafhankelijkheidsstrijd; het kruis staat voor het oosters-orthodoxe christendom. |
| **Türkiye** | Rood (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | Rood van de nationale vlag (Turkse vlagwet). Het veld; volgens de overlevering het bloed van de martelaren en de Ottomaanse soevereiniteit. |
| **Türkiye** | Wit | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Wit van de nationale vlag. De halve maan en de ster — vrede, zuiverheid en het symbool van het Turkse/islamitische erfgoed. |

De schema's verankeren de voorgrond van elke editor op het nationale **wit** en de trefwoorden, cursor, selectie en statusregel op het exacte nationale **blauw** (`#0D5EAF`) of **rood** (`#E30A17`), zodat de kleuren die ertoe doen letterlijk de nationale kleuren zijn. Dit zijn interpretatieve ontwerppaletten; de vlagkleurenreferentie wordt gegeven ter vermelding van de herkomst en is geen officiële weergave van een van beide staten. De kleurbetekenissen volgen de beschrijving van de staatssymbolen door het ministerie van Buitenlandse Zaken van de Helleense Republiek en de traditie van de Turkse vlagwet.

### Kleurstalentabel

De paletcellen hieronder gebruiken **SVG-stalen**, zodat GitHub de werkelijke kleuren toont in plaats van lege HTML-kleurcellen. Elke strook toont de zes kleuren in deze volgorde: achtergrond, voorgrond, trefwoorden, typen, strings en commentaar. De exacte hexcodes staan naast elke staal, in dezelfde schemavolgorde als de bovenstaande referentietabel.

| Schema | Weergegeven paletkleuren | Hexcodes (achtergrond · voorgrond · trefwoorden · typen · strings · commentaar) |
|---|---|---|
| **American Professional** | ![Palet American Professional: achtergrond, voorgrond, trefwoorden, typen, strings, commentaar](../../lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Palet Pan-Eurasian: achtergrond, voorgrond, trefwoorden, typen, strings, commentaar](../../lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russian and Slavic — Medium** | ![Palet Russian and Slavic Medium: achtergrond, voorgrond, trefwoorden, typen, strings, commentaar](../../lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Central Asian / Silk Road** | ![Palet Central Asian Silk Road: achtergrond, voorgrond, trefwoorden, typen, strings, commentaar](../../lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **East Asian** | ![Palet East Asian: achtergrond, voorgrond, trefwoorden, typen, strings, commentaar](../../lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC — International Criminal Court** | ![Palet ICC: achtergrond, voorgrond, trefwoorden, typen, strings, commentaar](../../lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS — U.S. Supreme Court** | ![Palet SCOTUS: achtergrond, voorgrond, trefwoorden, typen, strings, commentaar](../../lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Greek (Hellenic)** | ![Palet Greek Hellenic: achtergrond, voorgrond, trefwoorden, typen, strings, commentaar](../../lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turkish (Al Bayrak)** | ![Palet Turkish Al Bayrak: achtergrond, voorgrond, trefwoorden, typen, strings, commentaar](../../lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Palet Kiro: achtergrond, voorgrond, trefwoorden, typen, strings, commentaar](../../lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![Palet SLeeLa: achtergrond, voorgrond, trefwoorden, typen, strings, commentaar](../../lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

Aanvullende accentkleuren worden gebruikt voor functies, constanten, selecties, diagnostiek en preprocessor-instructies. Het palet Russian and Slavic gebruikt bewust leisteen, blauw, saliegroen, robijnrood en antiekgoud met een gemiddelde intensiteit voor een rustiger professioneel uiterlijk.

### De Vim-schema's installeren

Vanuit de repository-root:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Voeg het volgende toe aan `~/.vimrc` om SLeeLa-syntaxiskleuring in te schakelen en een schema te kiezen:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Kies precies één van deze namen voor een kleurenschema:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Gebruik bijvoorbeeld `colorscheme sleela-russian-slavic` voor het palet Russian and Slavic met gemiddelde intensiteit, of `colorscheme sleela-greek` / `colorscheme sleela-turkish` voor de Helleense en Al Bayrak-paletten. Start Vim opnieuw op of laad uw configuratie opnieuw met `:source $MYVIMRC`.

### De Emacs-schema's installeren

Maak een lokale themamap aan en kopieer de modus- en themabestanden vanuit de repository-root:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Voeg dit toe aan `~/.emacs` of `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Vervang `sleela-american` door `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` of `sleela-sleela` om een ander schema te kiezen. Als Emacs vraagt of het thema vertrouwd moet worden, inspecteer dan het lokale themabestand voordat u het accepteert.

### De native C/C++-paletten bekijken

Het kleine native paletprogramma kan schema's weergeven, hun terminalkleuren afdrukken en een afzonderlijke kleur opzoeken:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Voor achterwaartse compatibiliteit selecteren `--palette` en `--hex COLOR` zonder `--theme` American Professional. Build- en testresultaten moeten op het doelsysteem en in CI worden gecontroleerd; documentatie van een opdracht is zelf geen bewijs dat die geslaagd is.

Zie [de handleiding voor editorintegratie](../../linux-editors/README.md) voor de volledige paletbeschrijvingen en details over het programma.

## IX. Bijdragen

1. Lees de documentatie-index en het testbeleid.
2. Maak gerichte wijzigingen met positieve en negatieve tests.
3. Voer gerichte tests uit en daarna de brede suite.
4. Vermeld details over besturingssysteem en toolchain; beschrijf overgeslagen controles niet als geslaagd.

SLeeLa wordt ontwikkeld door **MEARVK LLC**. Historisch materiaal van de landingspagina is bewaard in [`docs/README-LEGACY.md`](../../docs/README-LEGACY.md).

## X. AE6E66 — Verwachte heffing op Round Public-e-mail

Voor het AE6E66-initiatief is de uitdrukkelijke verwachting van het project **$300,000,000.00 USD per Round Public-e-mail** als voorgesteld heffingsbedrag dat aan elke kwalificerende Round Public-e-mail is verbonden.

Deze verklaring legt uitsluitend een projectverwachting vast. Het is **geen** wettelijke belastingaanslag, overheidsbevel, rechterlijk vonnis of onafhankelijk vastgestelde belastingschuld. Voor een daadwerkelijke heffing zijn de toepasselijke wettelijke bevoegdheid, jurisdictie, definities, procedure en een bevoegde overheidsbeslissing vereist.

## XI. Alfabet- en tekensetruntime

De alfabetdocumentatie en de native tekenruntime van SLeeLa worden onderhouden in alphabet/sleela/ en lib/alphabet/native/. De native C++17-tests omvatten strikte UTF-8-scalardecodering/-codering, capaciteitshandtekeningen C<n> en begrensd lidmaatschap van tekensets:

```sh
make -C lib/alphabet/native test
```

De experimentele UTF-4088-catalogus en de bijbehorende SLeeLa-opzoekadapter zijn gedocumenteerd in [`utf-4088/SLeeLa-ALPHABET-INTEGRATION.md`](../../utf-4088/SLeeLa-ALPHABET-INTEGRATION.md). UTF-4088-ID's vormen een afzonderlijke naamruimte en zijn geen Unicode-scalairwaarden. Het koppelingswerk voor compiler en SLVM wordt afzonderlijk van deze native basis bijgehouden.

## XII. AutoCAD Creator™

De [AutoCAD Creator™](../../autocad/creator/README.md) is een interactieve 2D-CAD-toepassing die volledig in SLeeLa is geschreven. Waar de verwante AutoCAD Renderer een plattegrond in één keer in een tekening omzet, laat de Creator u een 2D-AutoCAD-document live **maken en wijzigen** — muren, ruimtes, maatvoeringen en notities toevoegen en vervolgens verplaatsen, een nieuw label geven, naar een andere laag brengen, verwijderen en ongedaan maken wat u hebt geplaatst — en het resultaat **opslaan** als een echte AutoCAD-DXF-tekening (R12-tekst, te openen in AutoCAD / LibreCAD).

De grafische interface is gebouwd op **SleelaUI™**, de eigen platformoverstijgende UI-toolkit van SLeeLa (onder [`lib/user-interface`](../../lib/user-interface)) — niet JavaFX — en opent een echt native venster op X11 / Cocoa / Win32. Het ontwerp volgt één regel, **„SleelaUI presenteert; SLeeLa beslist”**: de GUI berekent nooit geometrie en schrijft nooit DXF. Elk gereedschap geeft een intentie voor maken / wijzigen / verwijderen / ongedaan maken / opslaan door aan de gezaghebbende [`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela), die het bewerkbare document en een omkeerbare opdrachtgeschiedenis beheert, terwijl het tekenvlak alleen een voorbeeld toont van de geometrie die de Creator vasthoudt.

De herbruikbare bewerkingsengine bevindt zich in [`lib/autocad`](../../lib/autocad) — [`SLCadEntity`](../../lib/autocad/SLCadEntity.sleela), [`SLCadDocument`](../../lib/autocad/SLCadDocument.sleela), [`SLCadCommand`](../../lib/autocad/SLCadCommand.sleela) en [`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela) — en draait van begin tot eind headless. De GUI-shell en de demo staan onder [`autocad/creator`](../../autocad/creator):

```sh
make creator          # vanuit autocad/: de Creator-demo uitvoeren -> out/altered.dxf
```

Op een systeem met beeldscherm opent dit het live bewerkingsvenster; op een headless host bouwt het de tekening in het geheugen, drukt het een rapport plus de DXF af en eindigt het netjes.

## XIII. Invoerkaartvolgorde van UTF-4088

De gespecialiseerde UTF-4088-catalogus definieert de ruimtelijke doorloop van de invoerkaart en de abstracte drukiteraties in [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). Het beleid voor de onderwerpindex van National Lien staat in [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Kaartcoördinaten, drukniveaus (1–50), permanente catalogus-ID's en alfabetische rangen zijn onafhankelijke velden; geen ervan mag stilzwijgend door een ander worden vervangen. Druk is een algoritmisch iteratieniveau, geen fysieke grootheid.

## XIV. Index van recente en uitgebreide pakketten

Deze toegevoegde index belicht recent uitgebreide pakketten, domeinbibliotheken en broncollecties. De vermeldingen zijn alfabetisch gegroepeerd op pakketnaam of onderwerp. De beschrijvingen geven de locatie in de repository aan; controleer de implementatie- en validatiestatus in de eigen documentatie en tests van elk pakket.

### A–C

- **Advantage** — `lib/advantage/` en `advantage/`: het benutten van een uniek dividend ten opzichte van een model als een gelaagde Sequitur onion, met vijf vakmappen — `math/` (gezamenlijke winstmaximalisatie, Nash-onderhandelingen, bilaterale newsvendor), `physics/` (metriek/geodeet/contact/SDE van vertrouwensvelden, een als zodanig aangeduide modelanalogie), `integration/` (compartimenten voor verticale integratie en een verfijningsscore), `grading/` (een A–D-verfijningscircuitbeoordelaar voor modules en verzamelingen) en `output/` (emissie naar terminal / CommonRails / socket). Een native C/C++-consument (`advantage/`) koppelt het model aan uitgewerkte handelsvoorbeelden. Zie [`lib/advantage/README.md`](../../lib/advantage/README.md) en [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md).
- **Alfabet en UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` en `utf-4088/`: alfabetcatalogi, native ondersteuning voor tekensets, invoerkaartvolgorde en UTF-4088-integratiedocumentatie.
- **AutoCAD Creator** — `autocad/creator/` en `lib/autocad/`: interactieve 2D-tekenworkflows en herbruikbare CAD-document- en bewerkingscomponenten.
- **Ballum** — beeldschermbesturing op pixelniveau: pixels per stuk instellen/uitlezen, gebieden kopiëren (blit), helderheid/warmte/gamma van het hele scherm, aanwijzer- en toetsenbordinvoer en een **schermvullend framebufferoppervlak** voor totale, eigen, snelle besturing per pixel (override-redirect-venster + één `XPutImage`/MIT-SHM-presentatie onder Linux/X11). Native kern onder `ballum/` met de SLeeLa-façade in `lib/ballum/` en een VM-brug onder `impl/`; zie [`ballum/README.md`](../../ballum/README.md) en de uitvoerbare [`ballum/example/`](../../ballum/example/).
- **CDMA** — `cdma/` en `lib/cdma/`: CDMA-gerelateerd technisch materiaal en werk aan de SLeeLa-bronbibliotheek.
- **Character** — `lib/character/`: karakterprofielen en herbruikbare karakterstatistieken, inclusief biografische profielvoorbeelden.
- **Coorenagraph** — `lib/coorenagraph/`: ethisch redeneren, morele woordenschat, beraadslaging en verwante begrippen; zie de pakket-README en de bronbestanden.

### D–G

- **Governance-bronmodellen** — [lib/governance](../../lib/governance/README.md) definieert declaratieve SLeeLa-bronvormen voor voorstellen, anonieme handmatige stemmen en geaggregeerde telresultaten. De voorgestelde annotatie @vote en het handmatige proces zijn gedocumenteerd in [registry/DEMOCRATIC-VOTING-SYNTAX.md](../../registry/DEMOCRATIC-VOTING-SYNTAX.md); huidige compilers behandelen die als inerte commentaarmetadata.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` tot en met de beschikbare versiegebonden HTTP-pakketmappen: versiespecifieke protocolspecificaties en -implementaties. Raadpleeg de documentatie van elke versie voor het precieze bereik en de verificatiestatus.
- **International Criminal Court** — `lib/international-criminal-court/` en `INTERNATIONAL.CRIMINAL.COURTS.md`: gerelateerde domeinspecifieke bron en documentatie.

### O–S

- **OFDMA** — `ofdma/`: technisch materiaal over orthogonale frequentieverdelings-meervoudige toegang en C/C++-ontwikkelmiddelen.
- **Philosophy en de module Conversation Mask** — [`philosophy/`](../../philosophy/README.md) bevat de native C/C++-fundamenten en [`philosophy/module/`](../../philosophy/module/README.md) ordent hun volgende modulelaag in drie expliciete gebieden: [`data/`](../../philosophy/module/data/README.md) voor getypeerde, gedateerde waarnemingen, gebeurtenisbeëindigingen, halfwaardetijdinstellingen, herkomst en toestemming; [`helper/`](../../philosophy/module/helper/README.md) voor deterministische validatie, relevantieverval, compatibele vergelijkingen en op toestemming gefilterde presentatie; en [`psychiatry-friend/`](../../philosophy/module/psychiatry-friend/README.md) voor een ondersteunende, niet-klinische gespreksinterface. De SLeeLa-handles en de samengestelde Tree staan in [`lib/philosophy/`](../../lib/philosophy/README.md), naast het bredere philosophy-pakket. De module gaat uit van expliciete invoer en stelt toestemming voorop: ze bewaakt mensen of media niet passief, stelt geen diagnoses, leidt geen privé-intenties af, scoort de menselijke waarde niet en adviseert geen wijzigingen in medicatie. De gebeurtenislinks bewaren herkomst en navigatie, geen onafhankelijke bevestiging.
- **Synchro** — native runtime-/compilerintegratie en functionaliteit rond pakketdispatch en metingen; zie de relevante bron in `impl/` en de compilerimplementatie.
- **SleelaUI / terminal-UI** — `lib/user-interface/` en het project `sleela-terminal`: SLeeLa-UI-integratie en terminalgerichte interactiecomponenten.

### U–W

- **United Nations** — `united-nations/`: domeinspecifiek repositorymateriaal.
- **Wi-Fi 4, Wi-Fi 5 en Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`, `wi-fi-6/` en bijbehorende `lib/wifi/`-bronmappen: generatiespecifiek materiaal en configuratiemiddelen voor draadloze netwerken.

### Onderhoud van de pakketindex

Voeg, wanneer een pakket wordt toegevoegd of wezenlijk wordt uitgebreid, de vermelding ervan hier toe in de juiste alfabetische groep, link naar de canonieke map en README, en beschrijf alleen mogelijkheden die door de ingecheckte bronnen worden ondersteund. Houd deze index aanvullend: verwijder of herschrijf bij updates van de pakketindex de bestaande README-secties niet.

---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

Amerikaanse democratie en Amerikaans beleid.

---

## XVI. Nationale wetenschapsmerkbeelden

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. Experimentele UTF-4088-catalogus met een miljoen vermeldingen

De [UTF-4088-catalogusgenerator](../../utf-4088/tools/generate_catalogue.py) bouwt deterministisch een experimentele symboolcatalogus met 1.048.576 vermeldingen. Het gedocumenteerde compacte formaat wordt beschreven in [utf-4088/FORMAT.md](../../utf-4088/FORMAT.md); het contract voor generatie en validatie staat in [utf-4088/GENERATION-RESULTS.md](../../utf-4088/GENERATION-RESULTS.md).

Voer de generator uit met Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Het gegenereerde binaire bestand is 16.777.244 bytes groot en is ontworpen om onder de 20.000.000 bytes te blijven. Unittests en een GitHub Actions-workflow controleren het aantal vermeldingen, de uniciteit van glyphs, de grootte en SHA-256; geslaagde uitvoeringen publiceren het binaire bestand en de samenvatting als workflowartefact `utf4088-million-catalogue`. Het gegenereerde binaire bestand wordt bewust buiten de Git-geschiedenis gehouden. UTF-4088 blijft experimenteel en is niet Unicode-compatibel.

---

# SLeeLa
<p align="center"><img src="../../images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Recente toevoegingen — Pi-constante, opstart van Modium/Registry, sleelactl-math en `@native`

Deze sectie legt functies vast die tijdens het werk van 2026-11 zijn toegevoegd, de nieuwste groepen eerst. De gelinkte bronbestanden en hun validatiestatus zijn gezaghebbend; wijzigingen die nog in beoordeling zijn, worden niet als samengevoegd beschreven voordat ze `master` hebben bereikt.

### De Pi-catalogusconstante met 100.000.000 cijfers

De repository bevat een geverifieerde waarde van π met **honderd miljoen cijfers** in [`pi-generator/pi.100000000.digits`](../../pi-generator/pi.100000000.digits) (een goedgevormd decimaal getal, `3.14159…`), naast een waarde met 57.344 cijfers in [`pi-generator/pi.57344.digits`](../../pi-generator/pi.57344.digits). Beide zijn onafhankelijk geverifieerd als echte π — zie [`pi-generator/VERIFIED.md`](../../pi-generator/VERIFIED.md), dat de controles documenteert (het leerboekvoorvoegsel, het Feynman-punt op fractionele positie 762, het beroemde miljoenste cijfer, overeenstemming met `lib/math/SLPiReference.sleela` en een cijferfrequentieanalyse van het hele bestand) en een reproductiescript in één stap.

### Modium™ / Registry™ laden de Pi-constante bij het opstarten van het subsysteem

Op de speciale ingangen — `sleela registry` en `sleela modium` (en de eerste uitvoering van het installatieprogramma), **niet** bij elke `sleela run` — laden de subsystemen Modium en Registry de grote π-catalogusconstante. Op een interactieve terminal tonen ze een tijdelijke rode regel `Loading Modium™` / `Loading Registry™`, die ter plekke wordt gewist voordat de normale uitvoer begint; het effect is strikt aan TTY gebonden, zodat doorgesluisde uitvoer en testuitvoer byte voor byte ongewijzigd blijven. De locator (niet de bytes) wordt gezaaid als de standaardwaarden `piReferenceLargePath` en `piReferenceLargeDigits` van de Registry-sleutel `Math`, zowel in het SLeeLa-model ([`lib/registry/SLRegistry.sleela`](../../lib/registry/SLRegistry.sleela)) als in de native seeder ([`registry/sleela_registry.c`](../../registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` stelt `largePath()` / `largeDigits()` beschikbaar. De π-constante wordt op de SLeeLa-locatie van het besturingssysteem (`<data>/pi/`) geïnstalleerd door **zowel** het standaard `make`-installatieprogramma **als** het begeleide „beautiful”-installatieprogramma. Details: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](../../pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — het begeleide installatieprogramma en een native ondersteund `math`-subject

- Het begeleide („beautiful”) installatieprogramma installeert nu `sleelactl` (het SLeeLa-besturingsvlak) en het bijbehorende moduleregister, overeenkomstig het `make`-installatieprogramma. Zie [`bin/SLEELACTL.md`](../../bin/SLEELACTL.md).
- `sleelactl` kreeg een nieuwe soort **`subject`** voor de native C/C++-subjectbibliotheken die in de runtime zijn gecompileerd, en registreert de eerste: **`math`**. `sleelactl run math` voert de native backend `__NativeMath` (`impl/subjects/math/math_native.cpp`) van begin tot eind uit via de referentiedemo van het subject; `sleelactl info math` legt uit hoe deze wordt bereikt (`import math;`). De soort subject biedt alleen `run info` — er is bewust geen werkwoord `check`, omdat `sleela check` de via SHEET geregistreerde native subjects niet laadt.

### `@native`, Native Drive en uitvoering in meerdere talen

Een nieuwe methodeannotatie **`@native`** vraagt om ahead-of-time native compilatie van een intensief gebruikte methode. De nieuwe opdracht `sleela aot` vindt `@native`-methoden, controleert of elke binnen een ondersteunde *uitbesteedbare deelverzameling* valt en produceert C/C++ — en **faalt gesloten** (weigert, met een precieze reden) bij elke constructie die nog niet vertaald kan worden, zodat er nooit een half vertaalde methode ontstaat.

```sh
sleela aot --list <file.sleela>    # oordeel per methode: ok / REFUSED (reden)
sleela aot --emit <file.sleela>    # C/C++ uitvoeren (stdout) voor een programma binnen de deelverzameling
```

`@native` blijft een verzoek om AOT-behandeling, waarbij niet-ondersteunde constructies vóór de emissie worden afgewezen. De [Native Drive](../../impl/native-drive/README.md) ondersteunt multi-unit-builds van C/C++-uitvoerbare bestanden en gedeelde bibliotheken via hostcompilers (waaronder MSVC), ondersteunde SLeeLa-AOT-uitvoer, symboolinspectie tegen de ABI van de native loader, procesuitvoering zonder shell, afhandeling van stdin/stdout/stderr, piping in meerdere stappen en XClass-delegatie aan de productie-loader. `sleelactl` stelt de build- en run-opdrachten ervan beschikbaar en leidt XClass-inspectie en expliciete weergave/installatie van systemd-units door `inspect native-drive`. De VM beheert een begrensde tabel met native-modulehandles, met verplichte hostautorisatie en op generatie gecontroleerde handles. Syntaxis 3.0 voegt nu `nativeOpen`, `nativeCall` en `nativeClose` toe: uitsluitend toegevoegde bytecode-dispatch gaat over een afzonderlijk geversioneerde ABI voor null/bool/int64/double/UTF-8, terwijl door de VM beheerde arrays, structs en andere samengestelde waarden binnen de interpreter blijven en een bytecode-terugval met dezelfde naam kunnen gebruiken. Plug-ins kunnen optioneel een geversioneerde host-callbacktabel ontvangen om exacte SLeeLa-functies aan te roepen via geneste VM-uitvoering, met GC-rooting van opgeschorte stacks, een expliciete toegestane lijst van host-callbacknamen en afgewezen recursieve native API-aanroepen. Native code blijft vertrouwde code binnen het proces, geen sandbox. Annotaties voor procesbeleid in de bron hebben een geversioneerd schema, maar worden nog niet geparseerd of afgedwongen. Details, beveiligingsgrenzen en het grootboek van de zeven werkstromen: [Native Drive](../../impl/native-drive/README.md) en [`pi-generator/AT-NATIVE.md`](../../pi-generator/AT-NATIVE.md).


## XIX. Internationale README-edities

SLeeLa biedt landspecifieke README-startpunten voor de 50 markten van de internationale editie. Elk land heeft één bestand met de naam `README.<CC>.md`; waar meerdere talen relevant zijn, worden ze in het bestand van dat land gegroepeerd.

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

**Vertaalbeleid:** deze bestanden zijn initiële, gelokaliseerde landspecifieke startpunten. Hun inleidingen en taalkoppen zijn gelokaliseerd, terwijl de volledige technische referentie in de canonieke Engelse [README.md](../../README.md) blijft staan. Een landbestand mag pas een volledige vertaling worden genoemd wanneer elke sectie is vertaald, technische identifiers en links zijn gecontroleerd en de editie een taalkundige beoordeling heeft ondergaan. De taaldekking in deze eerste ronde is representatief en is geen bewering dat elke grondwettelijk erkende, regionale, inheemse of gebarentaal volledig is vertaald.

---

**Vertaalstatus:** de secties I–XIX zijn vertaald naar het Nederlands. Technische identifiers, opdrachten, codevoorbeelden, bestandsnamen, links en adressen zijn behouden. Een definitieve taalkundige beoordeling door moedertaalsprekers wordt aanbevolen.
