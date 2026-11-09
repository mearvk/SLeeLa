# SLeeLa 3.0 — Nederland (NL)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>

<p align="center"><strong>Een native systeemprogrammeertaal met compiler, runtime en ecosysteem van virtuele machines.</strong></p>

> **Internationale landeneditie — vertaalconcept.** Dit Nederlandse bestand is een werkversie. De oorspronkelijke inhofdsstructuur en technische verwijzingen zijn behouden, maar verschillende onderdelen bevatten nog brontekst of moeten taalkundig worden herzien. Behandel deze editie nog niet als een volledige Nederlandse vertaling.

**The Now. The Ingredient.** Een programmeertaal is meer dan alleen de syntaxis: ook de compiler, runtime, bibliotheken, hulpmiddelen en platformcontracten die software bruikbaar maken, horen erbij.

**Werkterreinen van SLeeLa.** SLeeLa omvat **Math**, **Object**, **Science** en **Reach**, de vier dimensies van het werk: wat berekenbaar is (Math), het vastgestelde object in de catalogus van wat bestaat (Object), het gedisciplineerde onderzoek naar waarheid (Science) en het uitbreiden van de mogelijkheden van een systeem naar andere systemen en mensen (Reach). **Nationale belangen blijven bestaan**; geen van deze vier dimensies vervangt ze. Wanneer een onderwerp de belangen van een land raakt, blijft die relatie behouden en wordt deze zorgvuldig vastgelegd, bijvoorbeeld in de metadatavelden `metaOrigin` en `metaNationalSchedule` van een klasse.

## I. Aan de slag

- [De native implementatie bofwen](impl/README.md)
- [Virtuele machines en Modium — praktische gebruikershandleiding](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [CPU-stack in de SLeeLa-broncode](lib/cpu/CPU.md): een volledige machineketen van logische poorten via ALU, cores en besturingssysteem tot programma's; met meer dan 130 CPU-profielen en instelbare [spanningen, variabele kloksnelheden en geavanceerde timingvoormeters](lib/cpu/TIMING.md), voor uitvoering [volgens specificatie of met overklokken](lib/cpu/components/README.md).
- [Documentatie-index](docs/README.md)
- [CSPML-gegevensbron en onderzoeksreferentie Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md): herkomst en integratierichtlijnen voor externe Amerikaanse gegevens over locaties, sociaaleconomische omstandigheden, arbeidskrachten, onderwijs en openbare scholen.
- [Gegenereerde API-documentatie](docs/api/README.md): actuele HTML-referentie, versiegebonden API-uitgave en instructies voor het opnieuw genereren.
- [API-catalogus](api/README.md)
- [Normen voor computationele vergelijkingen](comparison/README.md): normen voor typen, geheugen, ABI en conformiteit voor SLeeLa VM, Java/JVM en native C/C++.
- [Recursie-Stanchion — C- en C++-prototypes](recursion/stanchion/README.md): voorbeelden gericht op de primitieve typen Boolean, Byte, Character, Integer, Long, Float en Dofble, plus generieke, toegepaste, wetenschappelijke en educatieve recursiepatronen.
- [Matrix voor platformondersteuning en validatie](docs/PLATFORM-SUPPORT.md)
- [Merkrichtlijnen](docs/BRAND-GUIDE.md)
- [Merchandiseconcepten en grafische middelen](merchandise/README.md)
- [NitroWebExpress-logo](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Testbeleid en releasecriteria](markdown/TESTING.md)

## II. Nieuws

Recente toevoegingen aan de taal, drivers en standaardbibliotheek:

- **Standaardbibliotheekpakket `advantage`.** [`lib/advantage/`](lib/advantage/README.md) modelleert het optimaliseren van een uniek dividend ten opzichte van een model met een gelaagde structuur die **Sequitur onion** (dividend → beoordeling → aandachtspunten → wiskunde → toevoegingen → aftrek → hergroepering → iteratie). Het omvat de domeinen `math/` (bilaterale handelseconomie, gezamenlijke winstmaximalisatie, Nash-onderhandelingen en de kritieke newsvendor-ratio), `physics/` (structureel-economische modelanalogie van vertrofwensvelden), `integration/` (recursieve verticale integratie), `grading/` (A–D-classificatie van complexiteit) e `oftput/` (rofteerbare uitvoer naar terminal, CommonRails of socket/connector). Het pakket bevat uitvoerbare voorbeelden en documentatie over definities, wiskunde en natuurkunde. De C/C++-consument in [`advantage/`](advantage/README.md) verbindt het model met handelsvoorbeelden.
- **Anotações de temdoorização de ciclos — `@loopDelay` / `@loopBudget`.** Deze gedragsannotaties worden vóór `for`, `while` of `do`, -lussen geplaatst en voegen echte pauzes aan de uitvoering toe. `@loopDelay(ms = 250)` define een pauze door iteração; `@loopBudget(seconds = 10, iterations = 1000)` distribui um número conhecido de iterações door um orçamento de tempo total. Ambas utilizam o opcode aditivo `OP_SLEEP` (ISA 157), que também ondersteunderta a função integrada `sleep(<ms>)`. A implementação usa `nanosleep` / `Sleep`. Zie [`SLEELA.syntax` §32](SLEELA.syntax), de [API-referentie](api/sleela/3.0/LOOP_TIMING_API.md) e o [exemplo executável](examples/loop-timing-annotation-demo.sleela).
- **SleelaDoc na linha de comandos — `sleela docs`.** De opdracht somt documenteerbare onderwerpen op, toont pakketten en zoekt klassen op, ook hoofdletterongevoelig en met het standaardvoorvoegsel `SL`. De pagina's bevatten een definitie, ontwerpdoel, relaties, structuur, gebruiksconcepten en een beheersingsschaal met vijf niveaus. As etiquetas `@intent`, `@thesis`, `@axiom`, `@param`, `@see` e `@mastery` enriquecem a documentação. `sleela docs --generate [dir]` maakt een tekstcorpus voor offline raadpleging; `make sleeladoc` bundelt het corpus en de bijbehorende bronbestanden in `sleeladoc/build/sleeladoc.jar`. Zie [`lib/sleeladoc/`](lib/sleeladoc/README.md) e [`sleela(1)`](man/man1/sleela.1).
- **SleelaDoc e documentação HTML.** De extractor [`lib/sleeladoc`](lib/sleeladoc/README.md) zet documentatiecommentaar uit `.sleela`-eenheden om in Markdown- of HTML-pagina's. De doorzoekbare documentatiesite voor de volledige bibliotheek `/lib` wordt gegenereerd in [`sleeladoc-api/`](sleeladoc-api/README.md). De compiler kan ook MediaWiki-pagina's genereren via `--wiki`. Zie [SLEELADOC](manifesto/SLEELADOC.md), [SLEELADOC.WIKI](manifesto/SLEELADOC.WIKI.md) e o [exemplo](lib/sleeladoc/sleeladoc-demo.sleela).
- **Modelo de objetos de documento XML (`lib/xml`).** De bibliotheek bevat een DOM-boomstructuur com nós de elemento, texto, comentário, CDATA e instrução de processamento, atributos nomeados, serialização compacta/formatada, escape XML 1.0 §2.4, análise door células de caracteres e registo de qualidade `SLXmlAspect`. Gelijkheid van knooppunten is reflexief, symmetrisch en deterministisch, met niveaus voor identiteit, onderwerp en structuur. Zie [`lib/xml/README.md`](lib/xml/README.md) e o [exemplo executável](lib/xml/xml-dom.sleela).
- **Primitivos inteiros alargados.** `int`, `long` e `int64` zijn 64-bits signed integers; `int128` wordt momenteel ondersteund via een 64-bits representatie; echte 128-bits ondersteuning staat als toekomstig werk gedocumenteerd. As famílias binárias assinadas `int32`–`int16777216` (sintaxe 2.7) en unsigned `uint32`–`uint16777216` (sintaxe 2.8) breiden deze reeks uit. Zie [`SLEELA.syntax`](SLEELA.syntax) §§21, 24 e 25.
- **Campos consequentes `@mix` (sintaxe 2.9).** Een klasse mag herhaalde instantievelden met dezelfde naam declareren wanneer elke declaratie is voorzien van `@mix`. O grupo é indexado door ordem crescente (`1,2,3,…`) of door índices explícitos em `@mix(1,2,3,...)`; het aantal indexen moet overeenkomen met het aantal velden. Zie [§28](SLEELA.syntax).
- **Preonems — marcadores documentais do Processo Natural.** De declaratieve markeringen omvatten `author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`, `pure`, `gasoline` en `artifact`. Ze zijn contextueel en versiegebonden en veranderen de parsing, typecontrole of codegeneratie niet; buiten de markeringspositie blijven het gewone identifiers. Zie [`SLEELA.syntax`](SLEELA.syntax) §§18–20, 23, 26, 30 e 35.
- **Nordshrift met volledige bibliotheektoegang.** In syntaxis 3.0 gebruikt Nordshrift dezelfde recursieve `/lib`-index als de compiler, met alle pakketten en `.sleela`-eenheden; `nordshrift check` toont de aantallen voor de volledige boomstructuur.
- **Stanchion de recursão.** [`recursion/stanchion/`](recursion/stanchion/README.md) bevat C- en C++-referentieprototypes voor zeven gedocumenteerde primitieve contracten, plus generieke, toegepaste, wetenschappelijke en educatieve voorbeelden. De referentiebronnen moeten worden gecompileerd en getest voordat ze als geverifieerd mogen gelden.
- **Comentários angulares (`<` / `<<`).** O frontend reconhece comentários de linha `< line >` e de bloco `<< block >>`; raadpleeg [§1.1](SLEELA.syntax).
- **Literais inteiros door radix.** Os literais `int` aceitam hexadecimal (`0x`), binário (`0b`) e octal (`0o`), com scheidingstekens `_`, tanto na linguagem como no formato de modelos CPU [`.xlt`](lib/xlt/README.md). Zie [§1.4](SLEELA.syntax).
- **Norma Metadata Class.** De standaard voor beschrijvende metadata — herkomst, wettigheid en jurisdictiebereik — geldt voor alle `.sleela`-bestanden met klassen onder `/lib`. Zie [§15](SLEELA.syntax) e a [lição 13](tutorial/13-metadata-class.md).
- **Família de texto e String.** `lib/text` fornece operações de caracteres e cadeias, incluindo `charAt`, `substring`, `indexOf`, `reverse`, alinhamento de colunas e modelos posicionais. Zie o [tutorial](tutorial/14-text-and-strings.md) e o [exemplo](lib/text/text-string-demo.sleela).
- **String em folhas Nordshrift.** Uma folha `.sst` pode declarar um bloco tipado `strings:` que contribui voor o manifesto de componentes da compilação. Zie a [referência Nordshrift](wiki/Nordshrift-Sheet-Reference.md).
- **Modelos CPU door opcodes e equivalência de chamadas.** Cada `SL<ARCH>CPU` ondersteunderta a execução de um fluxo nativo de instruções através do ciclo fetch-decode-execute en gelijkwaardige uitvoering van code van een ander proces op de gedeelde VM-infrastructuur. Zie [`lib/cpu/EXECUTION.md` §5](lib/cpu/EXECUTION.md).

## III. Bouwen en testen

**Vereisten:** een toolchain C/C++ (GCC of Clang) en GNU Make; Python 3 voor ferramentas de manifesto e fluxo de trabalho; em Linux, também os pacotes de desenvolvimento X11/Xrandr voor produtos de visualização. De opdracht `make deps` van elke module toont of installeert de benodigde afhankelijkheden. De build in `impl/` stopt veilig wanneer niet aan het vertrofwde SHA-256-manifest wordt voldaan; het Makefile in de hoofdmap neemt deze controle automatisch op.

De snelste start vanuit een schone clone:

```sh
make                                  # alles bofwen vanuit de repository-root
```

Of alleen de native kern bofwen en testen:

```sh
make -C impl all                      # alleen de compilerkern en runtime bofwen
bash ./test-suites/run-all.sh --all   # de volledige testsuite uitvoeren
```

De opdracht `make` in de hoofdmap bofwt alle producten (kern, VM, codecs, churn, antivirus enzovoort) en sluit af met het aanmaken van het **register** en het materialiseren van de basistabel. Zo bevat een gebouwde werkboom direct een verifieerbaar register.

Belangrijke doelen:

```sh
make                     # alles bofwen (alias voor make all; het register wordt aan het einde aangemaakt)
make all                 # dezelfde volledige build, expliciet vermeld
sudo make install        # runtime-opdrachten en ondersteunende bestanden installeren (zie § IV)
make clean               # buildresultaten van alle geïntegreerde systemen verwijderen
make help                # alle doelen van het Makefile in de hoofdmap tonen
```

Andere nuttige doelen:

```sh
make registry            # alleen het register bofwen en de tabel materialiseren
./build-registry.sh      # dezelfde bewerking via een rechtstreeks script
make tests               # de testsuite bofwen en uitvoeren
make ballum              # de Ballum-weergavebibliotheek voor dit besturingssysteem bofwen
bin/install-modules      # modules gefaseerd installeren/bofwen; dit duurt ongeveer 10–20 minuten
```

Het doel `registry` voert [`build-registry.sh`](build-registry.sh): de registerkern bofwen, de basistabel opslaan in de draagbare tekstindeling `registry/build/sleela.sleelareg` en het MySQL-tabellenscript exdoorteren `registry/build/registry.sql` (via `sleela registry exdoort`). Het register kan worden bekeken met `sleela registry` of `registry/build/sleela-registry review`. Zie o [guia do registo](registry/README.md) e a [arquitetura Registry 3.0](registry/REGISTRY-3.0-ARCHITECTURE.md).

Zie os guias de compilação e testes voor conhecer os pré-requisitos e os passos específicos de cada plataforma. Een geslaagde build van één component bewijst niet dat alle bibliotheken of platforms gereed zijn voor release.

## IV. Installatie

Installeer na het bofwen van de native producten de runtime-opdrachten en ondersteunende bestanden:

```sh
sudo make install
make install PREFIX="$HOME/.local"        # instalação apenas voor o utilizador, zonder sudo
make install DESTDIR="$PWD/package-root" # instalação num diretório de voorbereidings de pacotes
make install-check
```

Het conventionele installatieprogramma gebruikt standaard `/usr/local/bin`, `/usr/local/lib/sleela` e `/usr/local/share/sleela`. Het behofdt een bestaande opstartconfiguratie en vervangt geen bestaande opdrachten die niet onder versiebeheer vallen. Verwijderen wist uitsluitend de door het installatieprogramma geregistreerde bestanden; niet-geregistreerde configuratie en runtimegegevens blijven behouden.

Gebruik voor een gerichte installatie van uitsluitend het hoofdpakket — de binaire bestanden `sleela`, `sleelvac` en `nordshrift`, de standaardbibliotheek, protocollen en ondersteunende modules — gebruik het schone installatieprogramma:

```sh
make clean-install       # instalação orientada apenas do pacote principal
```

Eerst wordt gevraagd of het hoofdpakket moet worden geïnstalleerd; daarna kiest u de reikwijdte van de installatie. Als de installatie slaagt, kan optioneel een keuzescherm voor geselecteerde uitbreidingsmodules verschijnen. De volledige interactieve procedure en alle opties staan beschreven in [Instalação rápida e segura](install/README.md).

## V. Taal en runtime

De normatieve grammatica staat in [`SLEELA.syntax`](SLEELA.syntax). De repository documenteert momenteel syntaxis **3.0**; a versão do compilador/ferramentas e a versão da sintaxe do código-fonte são independentes. Syntaxis 3.0 voegt de documentatiemarkeringen toe **`pure` en `gasoline`** (§30): twee declaratieve, contextuele Preonems zonder zelfstandig uitvoeringseffect; ze drukken de positie van een document uit via de eigen aard ervan. Syntaxis 3.0 brengt de taalfuncties van versie 1.0 tot en met 2.9 samen in één actuele normatieve editie en behoudt de minimale versievereisten per functie.

De voorgaande laag, 2.9, voegt de **opeenvolgende `@mix`-velden** toe (§28): een klasse mag meerdere instantievelden met dezelfde naam declareren, mits elk veld is gemarkeerd met `@mix`. Ze worden behandeld als een geïndexeerde groep (standaard oplopend: `1,2,3,…`, of definido manualmente via `@mix(1,2,3,...)`; het aantal indexen moet overeenkomen met het aantal velden). Ook opgenomen zijn de uitgebreide signed en unsigned binaire integerreeksen, de `int32` a `int16777216` (§24) e de `uint32` a `uint16777216` (§25), die `long`, `int64` en `int128` uitbreiden (§21); documentatie over trefwoordondersteuning (§22); e os **Preonems**: marcadores documentais declarativos `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) en `artifact` (§35; um estatuto de qualidade legislativa avaliado em relação ao tempo e ao espaço). Samen worden deze markeringen Preonems genoemd (§26.1).

Overbelasting van constructors en delegatie naar constructors `this(...)` e de classes-base vallen uitdrukkelijk buiten het geverifieerde syntaxiscontract 1.8. Raadpleeg de grammatica en implementatienotities voordat u op een functie vertrofwt. De negen SleelaDoc-broneenheden declareren nu `#sleela 3.0`; a verificação comum do compilador aceita sintaxe de código-fonte de 1.0 a 3.0. De nieuwe API-documentatie wordt gegenereerd met `make api-docs`.

De repository bevat uma implementação nativa C/C++, referências voor geração de SLVM, módulos da biblioteca padrão, testes, exemplos e integrações de plataforma. De implementatie- en validatiestatus verschilt per module. Zie a especificação aplicável e os resultados atuais de CI; ga er niet van uit dat alle componenten dezelfde mate van voltooiing hebben.

## VI. Engineeringprincipes

- **Bewijs vóór beweringen:** maak duidelijk onderscheid tussen wat is geïmplementeerd, getest, experimenteel en gepland.
- **Eén contract per grensvlak:** documenteer syntaxis, API's, bestandsindelingen, platformaannames en foftgedrag.
- **Portabiliteit vanaf het ontwerp:** isoleer platformgebonden backends en test ze op native runners.
- **Veilig falen:** test misvormde invoer, resofrcelimieten, processtoringen en opruimpaden.
- **Reproduceerbare releases:** voer schone builds uit en publiceer hashes van artefacten en releasenotities.

## VII. Antivirusbuild en integriteit

Het antiviruspakket Sleelavirin bevat um verificador SHA-256 nativo, um adaptador ClamAV opcional, manifestos de integridade voor fontes e índices e somas de verificação voor pacotes de plataforma. Bouw en test het pakket vanuit de hoofdmap van de repository:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Het integriteitsmanifest wordt gegenereerd onder `antivirus/build/`. Het manifest omvat versiebeheerde broncode- en documentatiebestanden en beschermde handtekeningindexbestanden; gegenereerde resultaten en de veranderlijke ClamAV-cache zijn uitgesloten. Snapshots van ClamAV-databases hebben eigen manifesten. Os ZIP de lançamento incluem `INTEGRITY-MANIFEST.json`, `SHA256SUMS` e um ficheiro complementar `.sha256`. Vergelijk hashes van gedownloade archieven met een onafhankelijk verkregen vertrofwenswaarde: een controlesom die naast een archief is opgeslagen, is geen handtekening van de uitgever. Zie o [guia Sleelavirin](antivirus/README.md), a [lista de verificação de segurança](antivirus/SECURITY.md) e o [workflow de compilação do antivírus](.github/workflows/antivirus-binaries.yml).

Een release geldt pas als geverifieerd wanneer de relevante CI-taken voor de exacte releasecommit succesvol zijn afgerond en de resulterende artefacten zijn geïnspecteerd.

## VIII. SLeeLa-editorthema's — Vim en Emacs

SLeeLa bevat een samenhangende reeks professionele donkere kleurenschema's voor **Vim** en **Emacs**, plus een native C/C++-paletprogramma. Het oorspronkelijke schema **American Professional** blijft de standaard. Er zijn tien aanvullende paletten geïnspireerd op uiteenlopende culturele en institutionele tradities; het zijn ontwerpinterpretaties en geen officiële weergaven van landen, culturen, rechtbanken of instellingen.

### De elf schema's en hun kleuren

| Schema | Achtergrond | Voorgrond | Trefwoorden | Typen | Strings | Commentaar |
|---|---|---|---|---|---|---|
| American Professional | `#101820` | `#F4F7FA` | `#7FB3FF` | `#73DACA` | `#F2C078` | `#8A96A3` |
| Pan-Eurasian | `#111827` | `#F0F2F5` | `#7AA7E8` | `#69C9B3` | `#D7B56D` | `#8995A5` |
| Russian and Slavic — Medium | `#202733` | `#E8EAF0` | `#A9C0DD` | `#8FC4B2` | `#D6BD8B` | `#9AA3AF` |
| Central Asian / Silk Road | `#1C2529` | `#F5F0E5` | `#7FADE0` | `#45C6C2` | `#D9B56D` | `#92A3A0` |
| East Asian | `#171A1C` | `#F2EFE7` | `#D97878` | `#72C7A5` | `#D8B76A` | `#969F9B` |
| ICC — International Criminal Cofrt | `#0E1A2B` | `#EAF0F7` | `#5B8FD6` | `#7FB7C9` | `#D9B25A` | `#7E8CA0` |
| SCOTUS — U.S. Supreme Cofrt | `#1A1416` | `#F3EEE6` | `#9E2B35` | `#8FA9B8` | `#C9A24B` | `#8C8178` |
| Greek (Hellenic) | `#0A1A30` | `#FFFFFF` | `#2E7BD6` | `#5E9FE0` | `#9FC3EC` | `#6E86A6` |
| Turkish (Al Bayrak) | `#2A0609` | `#FFFFFF` | `#F23D48` | `#F0A9AE` | `#F0C9A9` | `#A8767A` |
| Kiro | `#0B0B12` | `#F5F5FA` | `#A98EDA` | `#6E8BE0` | `#E8C547` | `#6A6482` |
| SLeeLa | `#08080E` | `#FFFFFF` | `#B07CFF` | `#5A7CFF` | `#FFD21E` | `#6E5F9E` |

De Griekse en Turkse paletten zijn afgeleid van de officiële nationale vlagkleuren: Grieks blauw `#0D5EAF), wit `#FFFFFF), Turks rood `#E30A17` e wit `#FFFFFF`. De getoonde achtergronden en accenten zijn donkerdere, editorvriendelijke interpretaties. De kleurreferenties documenteren de herkomst en vormen geen officiële staatsrepresentatie. De paletten gebruiken daarnaast kleuren voor functies, constanten, selecties, diagnostiek en preprocessorinstructies.

### Vim-schema's installeren

Voer dit uit vanuit de repository-root:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Voeg toe aan `~/.vimrc`:

```vim
syntax enable
set backgrofnd=dark
colorscheme sleela-american
```

Kies een schema: `sleela-american`, `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` of `sleela-sleela`. Herstart Vim of voer uit `:sofrce $MYVIMRC`.

### Emacs-schema's installeren

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Voeg toe aan `~/.emacs` of `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Vervang `sleela-american` door `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` of `sleela-sleela`. Als Emacs vraagt om het thema te vertrofwen, inspecteer dan eerst het lokale themabestand.

### Native C/C++-paletten bekijken

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Voor achterwaartse compatibiliteit `--palette` e `--hex COLOR` zonder `--theme` selecionam American Professional. Bouw- en testresultaten moeten op het doelsysteem en in CI worden gecontroleerd; documentatie van een opdracht bewijst niet dat deze geslaagd is. Zie de [gids voor editorintegratie](linux-editors/README.md).

## IX. Bijdragen

1. Lees de documentatie-index en het testbeleid.
2. Maak gerichte wijzigingen met positieve en negatieve tests.
3. Voer eerst gerichte tests uit en daarna de volledige suite.
4. Vermeld het besturingssysteem en de toolchain; presenteer overgeslagen controles niet als geslaagd.

SLeeLa wordt ontwikkeld door **MEARVK LLC**. Historisch materiaal van de startpagina is bewaard in [`docs/README-LEGACY.md`](docs/README-LEGACY.md).

## X. AE6E66 — Verwachting van Rofnd Public-e-mailheffing

Voor het AE6E66-initiatief is de door het project uitgesproken verwachting **300 000 000,00 USD door cada mensagem de correio eletrónico Rofnd Public**, como montante de taxa fiscal proposto associado a cada mensagem que se qualifique.

Deze verklaring legt uitsluitend een projectverwachting vast. **Não** constitui uma avaliação fiscal legal, ordem governamental, decisão judicial of obrigação tributária estabelecida de forma independente. Een daadwerkelijke heffing vereist a autoridade jurídica aplicável, a jurisdição, definições, procedimento e determinação door uma entidade governamental competente.

## XI. Alfabet en runtime voor tekensets

De SLeeLa-alfabetdocumentatie en de native character-runtime worden onderhofden in `alphabet/sleela/` e `lib/alphabet/native/`. De native C++17-tests behandelen descodificação e codificação estritas de escalares UTF-8, assinaturas de capacidade `C<n>` e pertença limitada a conjuntos de caracteres:

```sh
make -C lib/alphabet/native test
```

O catálogo experimental UTF-4088 e o respetivo adaptador de consulta SLeeLa estão documentados em [`utf-4088/SLeeLa-ALPHABET-INTEGRATION.md`](utf-4088/SLeeLa-ALPHABET-INTEGRATION.md). UTF-4088-ID's behoren tot een afzonderlijke naamruimte en zijn geen Unicode-scalarwaarden. Het compiler- en SLVM-bindingswerk wordt afzonderlijk van deze native basis bijgehouden.

## XII. AutoCAD Creator™

O [AutoCAD Creator™](autocad/creator/README.md) is een interactieve 2D-CAD-toepassing die volledig in SLeeLa is geschreven. Ao contrário do AutoCAD Renderer, que transforma um plano num desenho de uma só vez, o Creator maakt het mogelijk om live een 2D-document te **maken en te wijzigen** um documento 2D em tempo real: adicionar paredes, divisões, cotas e notas; mover, renomear, mudar de camada, eliminar e anular operações; e **guardar** o resultado como um desenho AutoCAD DXF real (texto R12, compatível com AutoCAD / LibreCAD).

De grafische interface gebruikt **SleelaUI™**, o toolkit de interface multiplataforma da SLeeLa em [`lib/user-interface`](lib/user-interface), en niet JavaFX. Het opent een native venster op X11 / Cocoa / Win32. Het ontwerpprincipe is **«SleelaUI presenteert; SLeeLa beslist»**: de GUI berekent geen geometrie en schrijft geen DXF. Elk hulpmiddel stuurt een intentie om te maken, wijzigen, verwijderen, ongedaan te maken of op te slaan naar [`SLCadCreator`](lib/autocad/SLCadCreator.sleela), que detém o documento editável e o histórico reversível de comandos; o canvas limita-se a pré-visualizar a geometria.

O motor de edição reutilizável encontra-se em [`lib/autocad`](lib/autocad): [`SLCadEntity`](lib/autocad/SLCadEntity.sleela), [`SLCadDocument`](lib/autocad/SLCadDocument.sleela), [`SLCadCommand`](lib/autocad/SLCadCommand.sleela) e [`SLCadCreator`](lib/autocad/SLCadCreator.sleela). O shell gráfico e a demonstração estão em [`autocad/creator`](autocad/creator):

```sh
make creator          # a partir de autocad/: executar a demonstração -> oft/altered.dxf
```

Op een systeem met beeldscherm opent a janela de edição em tempo real; op een headless host, o desenho é construído em memória, é apresentado um relatório com o DXF e o processo termina normalmente.

## XIII. UTF-4088-invoermapvolgorde

O catálogo especializado UTF-4088 definieert de ruimtelijke traversal van de invoermap en de abstracte drukiteraties in [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](utf-4088/INPUT-MAP-AND-PRESSURE.md). Het beleid voor de onderwerpindex van National Lien staat in [`utf-4088/NATIONAL-LIEN-REGISTER.md`](utf-4088/NATIONAL-LIEN-REGISTER.md). Kaartcoördinaten, drukniveaus (1–50), os IDs permanentes do catálogo e as classificações alfabéticas zijn onafhankelijke velden; ze mogen niet stilzwijgend door elkaar worden vervangen. A pressão is een algoritmisch iteratieniveau, geen fysieke grootheid.

## XIV. Index van recente en uitgebreide pakketten

Deze aanvullende index belicht recent uitgebreide pakketten, domeinbibliotheken en broncollecties. De vermeldingen zijn alfabetisch gegroepeerd op pakketnaam of onderwerp. De beschrijvingen geven de locatie in de repository aan; controleer de implementatie- en validatiestatus in de documentatie en tests van elk pakket.

### A–C

- **Advantage** — `lib/advantage/` e `advantage/`: het modelleren van een uniek dividend met een Sequitur-onionstructuur en vijf gebieden: `math/` (gezamenlijke winstmaximalisatie, Nash-onderhandelingen en de bilaterale newsvendor), `physics/` (metriek, geodeten, contactstructuur en SDE van vertrouwensvelden, aangeduid als modelanalogie), `integration/` (compartimenten voor verticale integratie en een complexiteitsscore), `grading/` (A–D-circuitbeoordelaar) e `oftput/` (saída voor terminal, CommonRails of socket). Een C/C++-consument in `advantage/` verbindt het model met handelsvoorbeelden. Zie [`lib/advantage/README.md`](lib/advantage/README.md) e [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alfabeto e UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` e `utf-4088/`: catálogos alfabéticos, ondersteunderte nativo de conjuntos de caracteres, ordenação de mapas de entrada e documentação de integração UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` e `lib/autocad/`: fluxos de desenho 2D interativos e componentes reutilizáveis voor documentos e edição CAD.
- **Ballum** — schermbesturing op pixelniveau: pixels lezen/schrijven, regio's kopiëren, algemene helderheid/kleurtemperatuur/gamma, muis- en toetsenbordinvoer en een volledig schermvullend framebufferoppervlak. De native kern staat in `ballum/`, de SLeeLa-façade in `lib/ballum/` en de VM-brug in `impl/`. Zie [`ballum/README.md`](ballum/README.md) e os [exemplos](ballum/example/).
- **CDMA** — `cdma/` e `lib/cdma/`: technisch materiaal over CDMA en SLeeLa-bronbibliotheekwerk.
- **Character** — `lib/character/`: perfis de caracteres e estatísticas reutilizáveis, incluindo exemplos biográficos.
- **Coorenagraph** — `lib/coorenagraph/`: ethisch redeneren, morele woordenschat, beraadslaging en aanverwante begrippen.

### D–G

- **Modelos de código-fonte de governação** — [`lib/governance`](lib/governance/README.md) define estruturas declarativas SLeeLa voor propostas, votações manuais anónimas e apuramentos agregados. A anotação proposta `@vote` e o processo manual estão documentados em [`registry/DEMOCRATIC-VOTING-SYNTAX.md`](registry/DEMOCRATIC-VOTING-SYNTAX.md); huidige compilers behandelen deze als inerte commentaarmetadata.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` en de beschikbare versiegebonden HTTP-pakketmappen: protocolspecificaties en implementaties per versie. Zie a documentação de cada versão voor determinar o âmbito e o estado de verificação.
- **Tribunal Penal Internacional** — `lib/international-criminal-cofrt/` e `INTERNATIONAL.CRIMINAL.COURTS.md`: bronnen en documentatie voor het gerelateerde domein.

### O–S

- **OFDMA** — `ofdma/`: material técnico de acesso múltiplo door divisão ortogonal de frequências e recursos de desenvolvimento C/C++.
- **Filosofia e módulo Conversation Mask** — [`philosophy/`](philosophy/README.md) bevat de native C/C++-basis; [`philosophy/module/`](philosophy/module/README.md) ordent de volgende modulelaag in drie gebieden: [`data/`](philosophy/module/data/README.md), voor observações tipadas e datadas, terminações de eventos, meia-vida, proveniência e consentimento; [`helper/`](philosophy/module/helper/README.md), voor validação determinística, decaimento de relevância, comvoorções compatíveis e apresentação filtrada door consentimento; e [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md), voor uma interface de conversa de apoio, não clínica. De SLeeLa-handgrepen en de samengestelde Tree staan in [`lib/philosophy/`](lib/philosophy/README.md). O módulo exige entradas explícitas e dá prioridade ao consentimento: não monitoriza pessoas of meios passivamente, não diagnostica, não infere intenções privadas, não classifica o valor humano nem recomenda alterações de medicação. Gebeurtenislinks bewaren herkomst en navigatie, maar vormen geen onafhankelijke bevestiging.
- **Synchro** — integração nativa de runtime/compilador e funcionalidades relacionadas com despacho de pacotes e medição; raadpleeg de bronnen relevantes em `impl/` en de compilerimplementatie.
- **SleelaUI / interface de terminal** — `lib/user-interface/` e o projeto `sleela-terminal`: SLeeLa-interface-integratie en terminalgerichte interactiecomponenten.

### U–W

- **Nações Unidas** — `united-nations/`: domeinspecifieke repositorymaterialen.
- **Wi-Fi 4, Wi-Fi 5 e Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`, `wi-fi-6/` e diretórios de fontes relacionados em `lib/wifi/`: material de redes zonder fios door geração e recursos de configuração.

### Onderhofd van de pakketindex

Wanneer een pakket wordt toegevoegd of wezenlijk uitgebreid, voeg dan hier een vermelding toe in de juiste alfabetische groep, link naar de canonieke map en README en beschrijf alleen mogelijkheden die door de aanwezige bronbestanden worden ondersteund. Hofd deze index aanvullend: verwijder of herschrijf geen bestaande README-secties bij het bijwerken van de pakketindex.

## XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

`bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh` :: `⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓`

`bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc` :: `⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉`

Amerikaanse democratische en beleidscontext.

## XVI. Nationale wetenschapsmerkbeelden

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>

## XVII. Experimentele UTF-4088-catalogus met één miljoen items

De [UTF-4088-catalogusgenerator](utf-4088/tools/generate_catalogue.py) bofwt deterministisch een experimentele symbolencatalogus met 1.048.576 vermeldingen. De compacte indeling wordt beschreven in [`utf-4088/FORMAT.md`](utf-4088/FORMAT.md); het contract voor generatie en validatie staat in [`utf-4088/GENERATION-RESULTS.md`](utf-4088/GENERATION-RESULTS.md).

Voer de generator uit met Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --oftput utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Het gegenereerde binaire bestand is 16 777 244 bytes en is ontworpen om onder 20 000 000 bytes. Unittests en een GitHub Actions-workflow controleren het aantal vermeldingen, de uniciteit van glyphs, de bestandsgrootte en SHA-256; succesvolle uitvoeringen publiceren het binaire bestand en de samenvatting als workflowartefact `utf4088-million-catalogue`. Het gegenereerde binaire bestand wordt bewust buiten de Git-geschiedenis gehofden. UTF-4088 blijft experimenteel en is niet Unicode-compatibel.

---

# SLeeLa

<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Recente toevoegingen — Pi-constante, Modium/Registry-start, sleelactl-math en `@native`

Deze sectie registreert functies die tijdens het werk van 2026-11 zijn toegevoegd, met de nieuwste groepen eerst. Alles is vastgelegd op `master`.

### Pi-catalogusconstante met 100.000.000 cijfers

De repository bevat een geverifieerde π-waarde met **honderd miljoen cijfers** in [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits), naast een waarde met 57.344 cijfers in [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Beide zijn onafhankelijk geverifieerd als echte π-waarden. [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md) beschrijft de controles: de bekende beginreeks, het Feynman-punt op positie 762 na de komma, het bekende cijfer op positie één miljoen, overeenstemming met `lib/math/SLPiReference.sleela`, análise da frequência de dígitos van het volledige bestand en een script om de resultaten opnieuw te produceren.

### Modium™ / Registry™ laden de Pi-constante bij het starten van het subsysteem

Bij de speciale toegangspunten — `sleela registry` en `sleela modium` (en bij de eerste uitvoering van het installatieprogramma), **não** em cada `sleela run` — laden de subsystemen Modium en Registry de grote π-catalogusconstante. In een interactieve terminal tonen ze tijdelijk uma linha vermelha `Loading Modium™` / `Loading Registry™`, die wordt gewist voordat de normale uitvoer begint. O gedrag depende estritamente de TTY, pelo que a saída canalizada e os testes permanecem inalterados byte a byte. De locatieaanduiding, niet de bytes zelf, wordt als standaardwaarde voor de Registry-sleutel `Math` van Registry: `piReferenceLargePath` e `piReferenceLargeDigits`, zowel in het SLeeLa-model ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) como no zondereador nativo ([`registry/sleela_registry.c`](registry/sleela_registry.c)). `lib/math/SLPiReference.sleela` expõe `largePath()` e `largeDigits()`. De constante wordt geïnstalleerd op de SLeeLa-locatie van het besturingssysteem (`<data>/pi/`) door zowel het standaardinstallatieprogramma `make` als het begeleide installatieprogramma. Zie [`MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — begeleid installatieprogramma en native math-subject

- Het begeleide ('beautiful') installatieprogramma installeert nu `sleelactl` (het SLeeLa-besturingsvlak) en het bijbehorende module-register, overeenkomstig het `make`-installatieprogramma. Zie [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- `sleelactl` kreeg een nieuw type `subject` voor bibliotecas de domínio C/C++ nativas compiladas no runtime e regista a primeira: `math`. `sleelactl run math` voert de backend `__NativeMath` (`impl/subjects/math/math_native.cpp`) via de referentiedemonstratie uit; `sleelactl info math` legt uit hoe deze wordt bereikt (`imdoort math;`). Dit subjecttype biedt alleen de werkwoorden `run` e `info`; não existe `check`, doorque `sleela check` não carrega os domínios nativos registados door SHEET.

### `@native` en `sleela aot` — deel 1 van het SLeeLa → native C/C++-traject

De nieuwe methodeannotatie **`@native`** vraagt om ahead-of-time native compilatie van een intensieve methode. Het commando `sleela aot` encontra os métodos anotados, verifica se pertencem a um subconjunto ondersteundertado voor transferência e emite C/C++. De bewerking **faalt gesloten** — recusa construções ainda não traduzíveis e apresenta uma razão precisa, evitando produzir métodos parcialmente traduzidos.

```sh
sleela aot --list <file.sleela>    # veredito door método: ok / REFUSED (motivo)
sleela aot --emit <file.sleela>    # emitir C/C++ (stdoft) voor um programa nde subset
```

Momenteel `@native` verandert het gedrag niet: os programas continuam a ser verificados e executados zonder alterações; a anotação apenas solicita tratamento nativo. Dit is deel 1: het legt de annotatie en de eerlijke subsetgrens vast. A compilação do C/C++ emitido voor uma biblioteca partilhada e o encaminhamento da VM voor essa biblioteca (com fallback voor de interpreter) são etapas posteriores. Zie [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md) voor de subset ondersteundertado e os códigos de saída.

## XIX. Internationale README-edities

SLeeLa biedt landspecifieke README-startpunten voor de 50 markten van de internationale editie. Elk land heeft een bestand `README.<CC>.md`; waarbij relevante talen in het bestand van dat land worden gegroepeerd.

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

**Vertaalbeleid:** deze bestanden zijn gelokaliseerde startpunten. Een editie mag pas een volledige vertaling worden genoemd wanneer alle secties zijn vertaald, technische identifiers en links zijn gecontroleerd en de editie taalkundig is nagekeken. De taaldekking is representatief en beweert niet dat alle regionale, inheemse, gebaren- of grondwettelijk erkende talen volledig zijn vertaald.

---

**Vertaalstatus:** werkversie — de sectiekoppen I–XIX zijn aanwezig, maar de lopende tekst is nog niet volledig naar het Nederlands vertaald. Technische identifiers, opdrachten, codevoorbeelden, bestandsnamen en links zijn behouden. Voltooi de vertaling en laat deze daarna door een moedertaalspreker nakijken voordat de editie als volledig wordt aangeduid.
