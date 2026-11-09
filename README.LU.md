# SLeeLa 3.0 — Lëtzebuerg (LU)

## Lëtzebuergesch Editioun

SLeeLa ass eng nativ Systemprogramméiersprooch mat engem eegene Compiler, Runtime an engem Ökosystem vu virtuelle Maschinnen. De Projet ëmfaasst och Bibliothéiken, Entwécklerinstrumenter, Dokumentatioun, Build-Prozesser an Integratiounskontrakter fir ënnerstëtzte Plattformen. Technesch Identifikateuren, Kommandoen, Weeër, Dateinimm, Codebeispiller an API-Nimm bleiwen onverännert, fir datt d'Instruktiounen direkt benotzt kënne ginn.

**The Now. The Ingredient.** Eng Programméiersprooch ass méi wéi Syntax: si ëmfaasst de Compiler, de Runtime, d'Bibliothéiken, d'Instrumenter an d'Plattformkontrakter, déi Software notzbar maachen.

SLeeLa beschreift seng Aarbechtsberäicher mat **Math**, **Object**, **Science** a **Reach**. National Interessen, déi fir en Thema relevant sinn, bleiwen ausdrécklech an de Metadate festgehalen a ginn duerch dës Klassifikatioun net op d'Säit gesat.

## I. Ufank

- [Déi nativ Implementatioun bauen](impl/README.md).
- [Benotzerhandbuch fir virtuell Maschinnen a Modium](virtual-machine/USER-GUIDE.md), dorënner `sleela vm` a `sleela modium`.
- [CPU-Architektur am SLeeLa-Quellcode](lib/cpu/CPU.md): vu logesche Paaren an der ALU iwwer Prozessorkären an de Betribssystem bis bei Programmer. De Katalog enthält méi wéi 130 CPU-Profiler. Kuckt och [Timing, Spannungen a Frequenzen](lib/cpu/TIMING.md).
- [Dokumentatiounsindex](docs/README.md).
- [CSPML-Datenquell a Fuerschungsreferenz Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md).
- [Generéiert API-Dokumentatioun](docs/api/README.md) an [API-Katalog](api/README.md).
- [Standarde fir computational Vergläicher](comparison/README.md): Typen, Erënnerung, ABI a Konformitéit tëscht SLeeLa VM, Java/JVM an nativem C/C++.
- [C- a C++-Referenzprototypen fir Recursion stanchion](recursion/stanchion/README.md).
- [Plattformënnerstëtzung a Validéierungsmatrix](docs/PLATFORM-SUPPORT.md).
- [Markeguide](docs/BRAND-GUIDE.md).
- [Testpolitik a Release-Krittären](markdown/TESTING.md).

## II. Funktiounen an Neiegkeeten

De Standardbibliothéik-Pak **Advantage** (`lib/advantage/`) modelléiert d'Bewäertung vun engem Resultat am Bezuch op e Modell. Seng Schichtestruktur ëmfaasst Iwwerpréiwung, Faktoren, Mathematik, Ergänzungen, Subtraktioun, Ëmgruppéierung an Iteratioun. D'Themen ëmfaassen bilateral Handelswirtschaft, eng strukturell physikalesch Analogie fir Vertrauensfelder (ausdrécklech als Modellanalogie markéiert), vertikal Integratioun, Modulbewäertung a geroutten Output. Kuckt d'[Advantage-Dokumentatioun](lib/advantage/README.md).

Aner dokumentéiert Funktiounen:

- **Loop-Timing:** `@loopDelay` an `@loopBudget` definéieren Pausen an Zäitbudgeten fir Schleifen. Si ginn op d'append-only VM-Instruktioun `OP_SLEEP` (ISA 157) ofgeleet, déi och d'allgemeng Funktioun `sleep(ms)` ënnerstëtzt. Kuckt [SLEELA.syntax](SLEELA.syntax) §32 an d'[API-Referenz](api/sleela/3.0/LOOP_TIMING_API.md).
- **SleelaDoc:** `sleela docs` weist Dokumentatioun fir Themen, Paketen a Klassen. `sleela docs --generate [dir]` erstellt e lokale Textkorpus fir offline Notzung. Kuckt [lib/sleeladoc](lib/sleeladoc/README.md).
- **XML DOM:** `lib/xml` stellt en XML-Dokumentmodell mat Element-, Text-, Kommentar-, CDATA- a Processing-Instruction-Noden, Attributer, Serialiséierung a Parsing zur Verfügung. Kuckt [lib/xml](lib/xml/README.md).
- **Ganzzueltypen:** `int`, `long` an `int64` si 64-Bit signéiert Ganzzuelen. `int128` gëtt als Ganzzueltyp akzeptéiert, baséiert aktuell awer op 64 Bit; dat ass keng vollstänneg 128-Bit-Implementatioun.
- **`@mix`-Felder:** Widderholl Instanzfelder mam selwechten Numm kënnen als indexéiert Sequenz gruppéiert ginn, wann all Feld mat `@mix` markéiert ass. Kuckt d'Syntax 2.9 an [SLEELA.syntax](SLEELA.syntax).
- **Preonems:** D'Dokumentmarkéierer `author`, `authorial`, `material;`, `memory;`, `determine;`, `object;` a `field;` beschreiwen de Status an d'Hierkonft vun engem Dokument. Si sinn deklarativ Markéierer an net eng Erlaabnis fir Code auszeféieren.
- **Nordshrift:** D'Instrument benotzt den rekursiven `/lib`-Index fir Packagen an `.sleela`-Eenheeten ze kontrolléieren.
- **Wénkelklammer-Kommentaren:** D'Formen `< line >` an `<< block >>` sinn dokumentéiert Kommentar-Syntaxen.
- **Zuelerliteraler:** Ganzzuele kënnen hexadezimal (`0x`), binär (`0b`) an oktal (`0o`) geschriwwe ginn; `_` kann als Zifferentrenner benotzt ginn.
- **Metadata Class:** Quellmoduler mat Klassen benotzen e Metadate-Muster fir Hierkonft, Legalitéit a jurisdiktionelle Beräich.
- **Text a String:** `lib/text` enthält Zeechenzell-baséiert Text- a Stringoperatiounen, dorënner `charAt`, `substring`, `indexOf`, `reverse` a positionell Formatéierung.

Eng Beschreiwung vun enger Funktioun beweist net eleng, datt si op all Zilplattform implementéiert a validéiert ass. Kontrolléiert ëmmer déi aktuell Implementatioun an d'Testresultater.

## III. Build a Testen

D'Ufuerderunge hänke vum gewielte Build-Zil of. Fir déi nativ Implementatioun brauch een normalerweis eng C/C++-Toolchain, zum Beispill GCC oder Clang, an GNU Make. Verschidde Manifest- a Workflow-Tools benotzen Python 3. Grafesch Linux-Komponente kënnen d'Entwécklungspakete fir X11 an XRandR verlaangen.

Am Root-Verzeechnes vum Repository:

```sh
make
```

D'nativ Kär bauen an d'Test-Suite starten:

```sh
make -C impl all
bash ./test-suites/run-all.sh --all
```

Nëtzlech Make-Ziler:

```sh
make all
make clean
make help
make registry
make api-docs
```

E erfollegräiche Build vun engem eenzege Bestanddeel beweist net, datt all Bibliothéiken oder Plattformen releasefäerdeg sinn. Testberichter mussen Tester, déi ausgefouert, iwwersprongen oder net bestanen goufen, getrennt weisen. Liest d'[Testpolitik](markdown/TESTING.md), ier Dir e Build-Status als Release-Freigab interpretéiert.

## IV. Installatioun

Eng systemwäit Installatioun benotzt normalerweis de Prefix `/usr/local`:

```sh
sudo make install
```

Eng Installatioun nëmme fir den aktuelle Benotzer, normalerweis ouni Administratiounsrechter:

```sh
make install PREFIX="$HOME/.local"
```

E Package-Root erstellen an d'Installatioun kontrolléieren:

```sh
make install DESTDIR="$PWD/package-root"
make install-check
```

Follegt den Instruktioune fir déi gewielte Plattform an dat gewielte Zil. Bei enger Benotzerinstallatioun muss de passende `bin`-Verzeechnes am `PATH` stoen. Gitt net dovun aus, datt dee selwechte Installatiounskommando op Linux, macOS a Windows identesch funktionéiert; kontrolléiert d'Plattformdokumentatioun an déi disponibel Installatiounsskripten.

- [Installatiounsguide](install/README.md)
- [Plattformënnerstëtzung](docs/PLATFORM-SUPPORT.md)
- [Build vun der nativer Implementatioun](impl/README.md)

## V. Sprooch, Compiler a Runtime

Déi normativ Sproochespezifikatioun steet an [SLEELA.syntax](SLEELA.syntax). D'SLeeLa-3.0-Dokumentatioun behandelt d'Syntax, de Compiler, de Runtime, virtuell Maschinnen a Bibliothéiken. Compiler-Versioun, Syntax-Versioun a VM-/ISA-Versioun si getrennte Begrëffer a däerfen net verwiesselt ginn.

Eng typesch Toolchain ëmfaasst:

1. **Quellcode:** `.sleela`-Dateie mat Sproochversioun an Deklaratiounen.
2. **Frontend:** lexikalesch Analyse, Parsing, Symbolbehandlung a semantesch Kontrollen.
3. **Zwëschevertriedung:** eng strukturéiert Form fir déi nächst Transformatiounsschrëtt.
4. **Codegeneratioun:** Output fir ënnerstëtzt VM- oder Zwëschenformat-Ziler.
5. **Runtime:** Ausféierung vu kompiléiertem Inhalt mat Mechanisme fir Erënnerung a Kontrollfloss.
6. **Bibliothéiken an Tools:** wiederverwendbar Packagen, Dokumentatioun, Build a Verifikatioun.

Déi genee Fäegkeete hänke vun der aktueller Implementatioun of. Eng Funktioun an der Spezifikatioun heescht net automatesch, datt si op all Zil ënnerstëtzt gëtt.

## VI. C, C++ an nativ Kompilatioun

De SLeeLa-Projet enthält C- a C++-Komponenten an dokumentéiert en ofgegrenzten native Wee. De Marker `@native` an de Kommando `sleela aot` beschreiwen en Entwécklungsberäich fir native Codegeneratioun. Si sinn keng allgemeng Erlaabnis, fir arbiträre native Code auszeféieren.

Wann SLeeLa mat C oder C++ verbonnen ass, muss d'Interface-Grenz explizit sinn. ABI-Kontrakter, Erënnerungsbesëtz, Datentypen, Feelerbehandlung a Plattformofhängegkeete mussen dokumentéiert a getest ginn. Native Code ass als eng eege Vertrauensgrenz ze behandelen; eng Referenz oder Deklaratioun ass eleng keng Autorisatioun fir d'Ausféierung.

Kuckt [native Implementatioun](impl/README.md), [Plattformmatrix](docs/PLATFORM-SUPPORT.md) an [Vergläichsstandarden](comparison/README.md).

## VII. Bibliothéiken a Subsystemer

De Repository enthält e breede Bibliothéiks- an Tool-Ökosystem. Wichteg Entréeën:

- `lib/advantage` — Advantage-Bibliothéik mat Definitiounen, Mathematik, Physik, Integratioun, Bewäertung an Output.
- `lib/xml` — XML-Dokumentmodell a Serialiséierung.
- `lib/text` — Text- a Stringoperatiounen.
- `lib/sleeladoc` — Dokumentatiounsextraktioun an CLI-Ënnerstëtzung.
- `lib/cpu` — CPU-Modeller, Komponenten an Timing-Referenzen.
- `lib/xlt` — CPU-Modellformat an entspriechend Dokumentatioun.
- `recursion/stanchion` — C- a C++-Referenzprototypen a Beispiller.
- `virtual-machine` — VM-Handbicher a Modium-Material.
- `api` an `docs/api` — API-Katalog a generéiert Referenz.

Eng Entrée am Katalog bedeit net automatesch, datt e Modul fir de Produktivbetrib prett ass. Kontrolléiert de README vum Modul, d'Testofdeckung, d'Plattformlimiten an d'Validéierungsdokumentatioun, ier Dir en asetzt.

## VIII. Integritéit, Sécherheet a Verifikatioun

SLeeLa leet Wäert op d'Verfollegbarkeet tëscht Quellcode, Dokumentatioun, Build an Testresultater. Hash-Manifester a Verifikatiounsinstrumenter kënnen iwwerpréiwen, ob Dateie mam erwaarten Inhalt iwwereneestëmmen. Eng erfollegräich Kontroll gëllt nëmme fir den uginnene Beräich an den Zäitpunkt, wou se ausgefouert gouf.

Grondprinzipien:

- Referenze identifizéieren Ressourcen; si erlaben net automatesch hir Ausféierung.
- Native Interfaces mussen explizit a limitéiert sinn.
- Testresultater mussen pass, fail a skip getrennt weisen.
- Aussoe iwwer Plattformënnerstëtzung mussen op konkret Builds an Tester zeréckgefouert ginn.
- Generéiert Dokumentatioun muss op Quell a Generator zeréckverfollegt kënne ginn.
- Sécherheetskontrolle mussen zoumaachen, wann néideg Integritéitsinformatioun feelt.

Kuckt [Testpolitik](markdown/TESTING.md), [Plattformënnerstëtzung](docs/PLATFORM-SUPPORT.md) an [Installatiounsguide](install/README.md).

## IX. Editoren an Entwécklungsprozess

De Repository enthält Integratiounen an Instruktioune fir Code-Editoren. Kuckt [Linux-Editorintegratiounen](linux-editors/README.md) fir Ressourcen iwwer Vim an Emacs. Kontrolléiert virun der Installatioun, ob d'Instruktioune mat Ärem Betribssystem an Ärer Editor-Versioun iwwereneestëmmen.

Recommandéierte Prozess fir Contributiounen:

1. Liest den Dokumentatiounsindex an d'Dokumentatioun vum Modul, deen Dir ännert.
2. Grenzt d'Ännerung of an erkläert hiren Zweck.
3. Füügt positiv an negativ Tester dobäi.
4. Start déi relevant Tester an, wann néideg, déi ganz Test-Suite.
5. Dokumentéiert Betribssystem, Toolchain, Kommandoen an tatsächlech Resultater.
6. Aktualiséiert README an API-Dokumentatioun, wann ëffentlecht Verhalen ännert.
7. Markéiert keng Tester als bestanen, wann se iwwersprongen oder net ausgefouert goufen.

## X. Weider Projetberäicher

De SLeeLa-Repository enthält och CPU- a Hardwaremodeller, grafesch Interfaces, Netzwierkkomponenten, Textveraarbechtung, Dokumentatioun, Internationaliséierung an experimentell Katalogen. Dës Beräicher kënnen ënnerschiddlech Reifegrade hunn. En experimentelle Katalog oder Beispill soll net ouni dokumentéiert Validéierung als stabil Produktivfunktioun bezeechent ginn.

Den UTF-4088-Katalog, d'π-Tools, Modium, Registry an `sleelactl` hunn separat Entréeën an technesch Viraussetzungen. Kontrolléiert de kanonesche README an d'Dokumentatioun vun de jeeweilege Moduler fir den aktuelle Status.

## XI. API- a Referenzdokumentatioun

- [Generéiert API-Dokumentatioun](docs/api/README.md)
- [API-Katalog](api/README.md)
- [SleelaDoc](lib/sleeladoc/README.md)
- [Syntaxspezifikatioun](SLEELA.syntax)
- [Dokumentatiounsindex](docs/README.md)
- [CPU-Dokumentatioun](lib/cpu/CPU.md)
- [CPU-Timing](lib/cpu/TIMING.md)

## XII. AutoCAD Creator™

AutoCAD Creator™ gëtt am technesche Katalog vum Projet ernimmt. Liest d'Dokumentatioun vum entspriechende Modul, fir beschriwwe Funktiounen, Beispiller a verifizéiert Resultater auserneenzehalen. Gitt keng Kompatibilitéit mat enger bestëmmter AutoCAD-Versioun dovun aus, ouni eng ausdrécklech Kompatibilitéitsmatrix an Testresultater.

## XIII. Experimentellen UTF-4088-Katalog

D'UTF-4088-Material ass en experimentelle Beräich fir Zeechen a Kataloge vum Projet. Gréisst, Unzuel vun Entréeën, Erënnerungsbedarf a Formatdetailer musse mat der Dokumentatioun an der aktueller Implementatioun zesumme gekuckt ginn. Eng grouss Unzuel vun Entréeën beweist eleng weder voll Unicode-Kompatibilitéit nach Produktivreife.

## XIV. π, Modium, Registry a sleelactl

De Projet enthält separat Tools an Dokumentatiounsberäicher fir π-Generatioun, Modium, Registry an `sleelactl`. Si hunn ënnerschiddlech Zwecker a mussen no hiren eegene Instruktioune gebaut a kontrolléiert ginn. Aktuell Kommandoen a Limite stinn am kanonesche README an an de jeeweilege Verzeechnesser.

## XV. International README-Editiounen

D'Länner-Editioune si lokaliséiert Entréeën an d'SLeeLa-Dokumentatioun. Si sollen zentral technesch Themen an de lokale Sprooche vermëttelen an dobäi technesch Identifikateuren, Kommandoen, Dateinimm, Code a Linken erhalen. Eng Editioun däerf net als komplett iwwersat bezeechent ginn, bis all relevant technesch Sektiounen iwwersat a kontrolléiert goufen.

Dës Editioun ass op Lëtzebuergesch geschriwwen. Si behaapt net, datt all offiziell Sproochen, regional Sproochen oder Zeechesproochen zu Lëtzebuerg komplett ofgedeckt sinn. Ergänzunge fir Franséisch an Däitsch kënnen op déi jeeweileg international SLeeLa-Editioune verweisen; eng lokal Iwwerpréiwung vun de Fachbegrëffer bleift recommandéiert.

## Wichteg Referenzen

- [Kanonesche README op Englesch](README.md)
- [SLeeLa-Syntaxspezifikatioun](SLEELA.syntax)
- [Dokumentatiounsindex](docs/README.md)
- [API-Dokumentatioun](docs/api/README.md)
- [Plattformënnerstëtzung](docs/PLATFORM-SUPPORT.md)
- [Nativ Implementatioun](impl/README.md)
- [Installatiounsguide](install/README.md)
- [Testpolitik](markdown/TESTING.md)
- [SLeeLa op GitHub](https://github.com/mearvk/SLeeLa)

**Iwwersetzungsstatus:** Dës lëtzebuergesch Editioun gouf zu enger technescher Aféierung ausgebaut, déi den Ufank, de Build, d'Installatioun, d'Sprooch an de Runtime, d'nativ Integratioun, Bibliothéiken, Sécherheet, Entwécklungsaarbecht an déi wichtegst Projetberäicher ofdeckt. Si ass nach keng Zeil-fir-Zeil-Iwwersetzung vun allen Detailer am kanoneschen englesche README. Eng technesch Kontroll an eng sproochlech Iwwerpréiwung vun engem lëtzebuergesche Fachlieser si virun enger definitiver Publikatioun recommandéiert.

---

## Français — Présentation technique

SLeeLa est un langage de programmation système natif accompagné de son compilateur, de son environnement d'exécution et d'un écosystème de machines virtuelles. Le dépôt comprend également des bibliothèques, des outils de développement, de la documentation et des procédures de construction.

Les identifiants techniques, commandes, chemins, noms de fichiers, exemples de code et noms d'API restent inchangés. Pour commencer, consultez le [README canonique](README.md), la [spécification du langage](SLEELA.syntax), le [guide de compilation native](impl/README.md), la [documentation d'installation](install/README.md) et la [matrice des plateformes](docs/PLATFORM-SUPPORT.md).

La construction de base utilise généralement une chaîne d'outils C/C++ (GCC ou Clang) et GNU Make. Les commandes de référence sont :

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

Une installation système utilise généralement `sudo make install`; une installation utilisateur peut utiliser `make install PREFIX="$HOME/.local"`. Vérifiez les instructions propres à la plateforme avant d'exécuter ces commandes.

Les domaines documentés comprennent Advantage, XML DOM, Text/String, SleelaDoc, CPU, XLT, les machines virtuelles et les outils Modium. Les fonctions natives et les interfaces C/C++ doivent respecter des frontières ABI explicites et des contrôles de sécurité. Une référence identifie une ressource, mais ne constitue pas une autorisation d'exécution.

Cette présentation française est un complément d'accès pour les lecteurs luxembourgeois francophones. Elle ne constitue pas encore une traduction intégrale, section par section, du README anglais. Consultez le [README canonique](README.md) pour les détails techniques normatifs.


## Lëtzebuergesch — Ergänzende technesch Referenz

### 1. API-Kontrakter a Moduler

All ëffentlech API soll dokumentéieren, wéi eng Inputen akzeptéiert ginn, wéi eng Outputen zeréckkommen, wéi eng Feeler méiglech sinn, wien d'Erënnerung besëtzt a wéi eng Säitewierkungen optrieden. D'Dokumentatioun vun engem Modul soll Virbedingungen, Nokonditiounen a Plattformlimite kloer beschreiwen.

E Symbolnumm oder eng Modulreferenz ass keng Erlaabnis, fir Code auszeféieren. D'Autorisatioun muss un der richteger Ausféierungsgrenz kontrolléiert ginn. Bei ongültegen Inputen, feelende Ressourcen oder net ënnerstëtzte Funktioune soll d'Operatioun sécher ofbriechen an eng diagnostizéierbar Fehlermeldung liwweren.

### 2. Compiler-Pipeline an Teststrategie

D'Tests solle sech op d'Etappe vun der Compiler-Pipeline verdeelen: lexikalesch Analyse, Parsing, semantesch Analyse, IR-Lowering, Codegeneratioun, VM-Ausféierung an native Integratioun. Test all Etapp separat an och d'Iwwergäng tëscht den Etappen.

E Regressionstest soll de klengste reproduzéierbare Input, dat erwaart Resultat an dat tatsächlecht Resultat enthalen. Negativ Fäll solle Syntax- a Typfeeler, feelend Inputen, Ressourcefeeler an ABI-Inkompatibilitéit ofdecken. Tests, déi iwwerspronge goufen oder net ausgefouert goufen, däerfen net als erfollegräich gemellt ginn.

### 3. Kompatibilitéit a Migratioun

Wann eng ëffentlech API, e Dateiformat oder d'Verhale vum Compiler geännert gëtt, dokumentéiert d'Auswierkungen op d'Backward Compatibility. Gitt Migratiounsschrëtt an Beispiller virun an no der Ännerung un. Erkläert, ob d'Ännerung Quellcode, kompiléiert Artefakter oder Runtime-Konfiguratiounen betrëfft.

Fir e Build- oder Testresultat reproduzéierbar ze maachen, notéiert de Git-Commit, de Betribssystem, d'Architektur, d'Compiler-Versioun, d'Toolchain an déi genee Kommandoen. E Resultat op enger Plattform ass kee Beweis fir eng aner Plattform. Kuckt d'[Plattformmatrix](docs/PLATFORM-SUPPORT.md) an d'[Testpolitik](markdown/TESTING.md).

### 4. Lokaliséierung an Ënnerhalt

Dateinimm, Identifikatoren, Kommandoen, Weeër, URLen, Syntax an Codebeispiller mussen onverännert bleiwen. Iwwersat gëtt den Erklärungstext, net d'Tokens, déi de Compiler oder d'Shell erkennen muss. Benotzt konsequent Begrëffer fir Compiler, Runtime, virtuell Maschinn, Modul an API; wann e Lëtzebuergesche Fachbegrëff net eendeiteg ass, kann den engleschen Term bei der éischter Ernimmung dobäistoen.

Wann d'kanonesch englesch README aktualiséiert gëtt, vergläicht déi entspriechend Kapitelen a passt dës Editioun un. Kontrolléiert Markdown-Linken, Iwwerschrëften a Code-Fences. Dës Ergänzung mécht d'lëtzebuergesch technesch Aféierung méi ëmfaassend, ass awer keng komplett Iwwersetzung vun all Kapitel a brauch nach eng fachlech a sproochlech Schlussrevisioun.


---

## Deutsch — Technische Einführung

SLeeLa ist eine native Systemprogrammiersprache mit eigenem Compiler, eigener Laufzeitumgebung und einem Ökosystem virtueller Maschinen. Das Repository umfasst außerdem Bibliotheken, Entwicklungswerkzeuge, Dokumentation und Build-Prozesse.

Technische Bezeichner, Befehle, Pfade, Dateinamen, Codebeispiele und API-Namen bleiben unverändert. Für den Einstieg dienen die [kanonische README](README.md), die [Sprachspezifikation](SLEELA.syntax), die [Anleitung zur nativen Implementierung](impl/README.md), die [Installationsdokumentation](install/README.md) und die [Plattformmatrix](docs/PLATFORM-SUPPORT.md).

Für den grundlegenden Build werden üblicherweise eine C/C++-Toolchain (GCC oder Clang) und GNU Make benötigt. Zentrale Befehle sind:

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

Eine systemweite Installation erfolgt üblicherweise mit `sudo make install`; eine benutzerspezifische Installation kann `make install PREFIX="$HOME/.local"` verwenden. Vorher sind die plattformspezifischen Hinweise zu prüfen.

Die dokumentierten Bereiche umfassen Advantage, XML DOM, Text/String, SleelaDoc, CPU, XLT, virtuelle Maschinen und Modium-Werkzeuge. Native Funktionen und C/C++-Schnittstellen benötigen explizite ABI-Grenzen und Sicherheitsprüfungen. Eine Referenz identifiziert eine Ressource, erteilt aber keine Ausführungserlaubnis.

Diese deutsche Einführung ergänzt den Zugang für deutschsprachige Leser in Luxemburg. Sie ist noch keine vollständige, abschnittsweise Übersetzung der englischen README. Für normative technische Einzelheiten ist die [kanonische README](README.md) maßgeblich.
