<img align="right" src="images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — Belgien (BE)

## Deutsch

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Eine native Systemsprache mit Compiler, Laufzeitumgebung und Ökosystem virtueller Maschinen.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Belgische deutschsprachige Ausgabe.** Vollständige deutsche technische Übersetzung für deutschsprachige Leserinnen und Leser in Belgien. Technische Bezeichner, Quellcode, Befehle, Dateinamen und API-Namen bleiben unverändert.

**The Now. The Ingredient.** Eine Programmiersprache ist mehr als ihre Syntax: Dazu gehören Compiler, Laufzeitumgebung, Bibliotheken, Werkzeuge und Plattformverträge, durch die Software praktisch nutzbar wird.

**Womit sich SLeeLa befasst.** SLeeLa befasst sich mit **Math**, **Object**, **Science** und **Reach** – den vier Seiten seiner Arbeit: dem Berechenbaren (Math), dem feststehenden Gegenstand im Katalog dessen, was ist (Object), der disziplinierten Untersuchung des Wahren (Science) und der Ausdehnung der Möglichkeiten eines Systems auf andere Systeme und Menschen (Reach). **Nationale Interessen bleiben bestehen**; sie werden durch keine dieser vier Seiten aufgehoben. Wenn ein Thema die Interessen einer Nation berührt, bleibt diese Zuordnung erhalten und wird zuverlässig dokumentiert, beispielsweise in den Metadatenfeldern `metaOrigin` und `metaNationalSchedule` einer Klasse.

## I. Hier beginnen

- [Native Implementierung erstellen](impl/README.md)
- [Virtuelle Maschinen und Modium – praktische Benutzeranleitung](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [CPU-Stack im SLeeLa-Quellcode](lib/cpu/CPU.md) – eine vollständige Maschinenkette von Logikgattern über ALU, Kerne und Betriebssystem bis zu Programmen; mit mehr als 130 CPU-Profilen sowie einstellbaren [Spannungen, variablen Taktraten und erweiterten Zeitparametern](lib/cpu/TIMING.md), die sich [spezifikationsgemäß oder übertaktet ausführen](lib/cpu/components/README.md) lassen.
- [Dokumentationsindex](docs/README.md)
- [CSPML-Datenquelle und Forschungsreferenz Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) – Herkunft und Integrationshinweise zu externen US-Daten über Orte, sozioökonomische Verhältnisse, Arbeitskräfte, Bildung und öffentliche Schulen.
- [Generierte API-Dokumentation](docs/api/README.md) – aktuelle HTML-Referenz, versionierte API-Ausgabe und Hinweise zur Neuerstellung.
- [API-Katalog](api/README.md)
- [Standards für Rechenvergleiche](comparison/README.md) – Typ-, Speicher-, ABI- und Konformitätsstandards für SLeeLa-VM, Java/JVM und natives C/C++.
- [Rekursions-Stanchion – C- und C++-Prototypen](recursion/stanchion/README.md) – auf primitive Typen spezialisierte Beispiele für Boolean, Byte, Character, Integer, Long, Float und Double sowie generische, angewandte, wissenschaftliche und didaktische Rekursionsmuster.
- [Plattformunterstützung und Validierungsmatrix](docs/PLATFORM-SUPPORT.md)
- [Markenleitfaden](docs/BRAND-GUIDE.md)
- [Merchandise-Konzepte und Grafiken](merchandise/README.md)
- [NitroWebExpress-Logo](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Testleitlinie und Freigabekriterien](markdown/TESTING.md)

## II. Neuigkeiten

Aktuelle Ergänzungen der Sprache, Treiber und Standardbibliothek:

- **Standardbibliothekspaket `advantage` – Optimierung einer Dividende mit Mathematik, Physik, Integration, Bewertung und routbarer Ausgabe.** [`lib/advantage/`](lib/advantage/README.md) kapselt die *Optimierung einer eindeutigen Dividende relativ zu einem Modell* als geschichtelte **Sequitur onion** (Dividende → Prüfung → Anliegen → Mathematik → Ergänzungen → Subtraktion → Neugruppierung → Iteration). Die Struktur lässt sich nach innen und außen durchlaufen und unterstützt vollständige dynamische Aktualisierung und Rückerstattung. Sie bietet ein einfaches **Benutzermodell** und ein vollständig steuerbares **Entwicklermodell**. Es gibt fünf Themenbereiche: **`math/`**, bilaterale Handelsökonomie (gemeinsame Gewinnmaximierung, Nash-Verhandlung, bilaterales kritisches Verhältnis des Newsvendor-Modells); **`physics/`**, eine „Physik der Vertrauensfelder“ für die strukturelle Ökonomie (Vertrauen als metrischer Tensor, transaktionale Geodäte, Ankündigungs-Kontaktstruktur und eine CIR-SDE mit Sprüngen für den Vertrauensabbau – ausdrücklich als Modellierungsanalogie gekennzeichnet); **`integration/`**, rekursiv kombinierbares Modell vertikaler Integration für Kapitalismus und Einzelhandel (Kompartimente, Engineering-Buchten, Testkompartimente, Newton-Median-Trays und ein System-Sophistication-Score); **`grading/`**, ein Sophistication-Circuit-Grader, der einzelne Module und beliebige Gruppen von Gewinner-, Verlierer-, Grader- oder Absolutmodulen mit A–D bewertet; sowie **`output/`**, eine routbare Ausgabeschnittstelle zum Terminal, zu einer **CommonRails**-formatierten Oberfläche oder zu einem Socket/Connector (für ein natives `slnet_write` vorgesehen). Jede Klasse enthält den Metadata-Class-Datensatz; fünf ausführbare Demos validieren die Themen. Ergänzende Dokumentation: [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md), [`MATH.md`](lib/advantage/MATH.md) und [`PHYSICS.md`](lib/advantage/PHYSICS.md). Ein nativer C/C++-Konsument unter [`advantage/`](advantage/README.md) – mit C-Kern `advantage_value`/`advantage_mean`, XML-Wissensspeicher sowie sicherer HTTPS/SFTP/SSH-Akquisition, Keystore und DH/ECDH-Transportbibliothek – verbindet das Modell mit [Handelsbeispielen](advantage/examples/README.md): positiven Transaktionen aus angenommenen Internetquellen, einer Relevanzsequenz für Traders, Consumers, Helpers und Healers sowie acht Modellen mit Rang A–D (D = höchster Cement-Anteil, A = höchste Brand-Ausprägung).
- **Schleifenzeit-Anmerkungen – `@loopDelay` / `@loopBudget` und integrierte Funktion `sleep(ms)`.** Dies sind die ersten **verhaltenswirksamen Anmerkungen auf Anweisungsebene** in SLeeLa: Sie stehen in einer eigenen Zeile vor einer `for`-, `while`- oder `do`-Schleife und fügen der Ausführung eine tatsächliche Pause hinzu. `@loopDelay(ms = 250)` / `@loopDelay(seconds = 2)` wartet pro Iteration eine feste Zeitspanne (die Kurzform `@loopDelay(250)` verwendet standardmäßig Millisekunden); `@loopBudget(seconds = 10, iterations = 1000)` verteilt eine bekannte Iterationszahl gleichmäßig über ein Gesamtbudget an Echtzeit. Beide werden auf den neuen append-only Opcode **`OP_SLEEP`** (ISA 157) abgebildet, der auch die überall aufrufbare allgemeine integrierte Funktion **`sleep(<ms>)`** unterstützt. Die Pause ist mit `nanosleep` / `Sleep` portabel implementiert. Spezifikation: [`SLEELA.syntax` §32](SLEELA.syntax); API-Referenz: [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md); ausführbares Beispiel: [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela). Ein Index der Sitzungs-Ergänzungen – einschließlich der nativen Großzahlmultiplikation `OP_BIGMUL` und der eigenständigen Werkzeuge [local-generator](local-generator/README.md) und [pi-generator](pi-generator/README.md) – befindet sich in [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md). Die drei generierten API-Oberflächen ([`sleeladoc-api/`](sleeladoc-api/README.md), [`docs/api/`](docs/api/README.md) und der unveränderliche Snapshot [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html)) wurden mit `make api-docs` neu erstellt und enthalten nun die Einträge `SLISASleep` / `SLISABigMul` auf der Seite des `vm`-Pakets.
- **SleelaDoc über die Kommandozeile – `sleela docs`.** SleelaDoc ist jetzt direkt im Terminal verfügbar. `sleela docs` listet alle dokumentierbaren Themen auf; `sleela docs crypto` zeigt ein Paket; `sleela docs stringbuilder` löst eine Klasse ohne Beachtung der Groß-/Kleinschreibung auf und erkennt das Standardbibliothekspräfix `SL` als `SLStringBuilder`. Angezeigt wird eine ausführliche Seite mit **Definition, Designzweck, Beziehungen, Struktur, user clays und einer fünfstufigen Kompetenzleiter** sowie geordneten „Meinten Sie?“-Vorschlägen bei ähnlichen Namen. Autoren erweitern jede Seite durch SleelaDoc-`@`-Tags wie `@intent`, `@thesis`, `@axiom`, `@param`, `@see` und `@mastery`. `sleela docs --generate [dir]` erzeugt einen Klartext-Korpus (eine Seite je Klasse plus einen Index `TOPICS.index`), den die CLI anschließend offline lesen kann; **`make sleeladoc`** bündelt diesen Korpus und die passenden `.sleela`-Quellen in `sleeladoc/build/sleeladoc.jar`. Für Themen, die nur eine Fassade besitzen, etwa `psychiatry` und `politico`, durchsucht `sleela docs --facade <topic>` das Dateisystem zur Laufzeit und listet reale Ordner und anschließend Dateien auf; benutzerseitig anpassbare Grenzen sind **6 MB, 6000 Dateien und 6 Minuten**. Die Einheiten in `lib/sleeladoc` verwenden die aktuelle Edition **`#sleela 3.0`**. Siehe [`man/man1/sleela.1`](man/man1/sleela.1) und [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md).
- **SleelaDoc – Javadoc-artige Dokumentation mit generierter HTML-Site.** **SleelaDoc** ist die Konvention für Dokumentationskommentare im SLeeLa-Quellcode (Winkelklammer-Kommentare mit `@doc` / `@page`; siehe [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)). [`lib/sleeladoc`](lib/sleeladoc/README.md) ist der **Extraktor**, der eine `.sleela`-Einheit in eine Markdown- oder HTML-API-Seite umwandelt – vergleichbar mit `javadoc` für Java – und dabei die *Absicht* dokumentiert. Eine vollständige, durchsuchbare **HTML-Dokumentationssite für ganz `/lib`** wird mit `tools/generate-sleeladoc-api.py` unter [`/sleeladoc-api`](sleeladoc-api/README.md) erzeugt (öffnen Sie `sleeladoc-api/index.html`). Sie verwendet ein klares Design in Blau, Hellblau, Hellgrau und Weiß, eine Hierarchie Paket → Klasse → Mitglied sowie ein Glossar für Leser auf dem Niveau *Student 2*. Der Compiler erzeugt außerdem als Nebenprodukt der Kompilierung vom Autor markierte Dokumente (`--wiki`: eine MediaWiki-Seite und eine Kopie der Ursprungssource); siehe [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md). Ausführbares Beispiel: [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela).
- **XML-Dokumentobjektmodell (`lib/xml`).** Ein XML-DOM auf SLeeLa-Seite: Ein Dokument besitzt einen Baum aus Element-, Text-, Kommentar-, CDATA- und Processing-Instruction-Knoten mit geordneten Kindern und benannten Attributen. Enthalten sind ein kompakter oder formatierter Serializer (XML-1.0-§2.4-Escaping), ein Parser auf Zeichen-Zellen-Basis und das Qualitätsregister `SLXmlAspect`. Die Knotengleichheit folgt drei Vertrauensstufen (Identität → dasselbe Subjekt → Struktur) und ist reflexiv, symmetrisch und deterministisch. Siehe [`lib/xml/README.md`](lib/xml/README.md) und das ausführbare Beispiel [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela).
- **Breite vorzeichenbehaftete Ganzzahlprimitive (`long`, `int64`, `int128`).** `int`, `long` und `int64` sind vorzeichenbehaftete 64-Bit-Ganzzahlen und werden identisch typgeprüft und ausgeführt. `int128` wird als Ganzzahltyp akzeptiert (derzeit noch 64-Bit-basiert; echte 128-Bit-Unterstützung ist als Folgearbeit dokumentiert). Die vorzeichenbehaftete Basis-2-Reihe `int32` bis `int16777216` (Syntax 2.7) und die entsprechende vorzeichenlose Reihe `uint32` bis `uint16777216` (Syntax 2.8) erweitern dies. Siehe [`SLEELA.syntax`](SLEELA.syntax) §21, §24 und §25.
- **Konsekutive `@mix`-Felder (Syntax 2.9).** Eine Klasse darf mehrere Instanzfelder mit **demselben Namen** deklarieren, wenn jede Wiederholung mit `@mix` versehen ist. Die Sprache behandelt die Gruppe als indexierte Folge (Array/Liste), standardmäßig von oben nach unten `1,2,3,…` oder manuell über `@mix(1,2,3,...)`; die Anzahl der Indizes muss der Feldanzahl entsprechen. Siehe [`SLEELA.syntax`](SLEELA.syntax) §28.
- **Dokumentmarker – der natürliche Prozess, bezeichnet als Preonems (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).** Diese alleinstehenden Dokumentmarker-Wörter werden zusammen als **Preonems** bezeichnet (ein Marker ist ein *Preonem*, die Menge sind die *Preonems*; siehe [`SLEELA.syntax`](SLEELA.syntax) §26.1). Es handelt sich um deklarative Dokumentmarker erster Klasse, die benennen, was ein Dokument in der Reihenfolge seiner Entstehung *ist*: `author`/`authorial` (Syntax 2.4; durch den Berufsratsprozess verwaltet – das einzige Mitglied mit Verhaltenswirkung), `material;` (festgestellte Quelle) und `memory;` (Syntax 2.5), `determine;` (Syntax 2.6; ein Dokument, das für weitere Bearbeitung durch einen Arzt, Mediziner oder Ghost bis zum rechtlichen Abschluss offen ist) sowie die beiden neuesten Marker **`object;`** (ein feststehendes Ding im Katalog dessen, was ist, reifiziert und mit konkretem Status versehen in Syntax 2.8) und **`field;`** (das Beziehungsfeld, in dem Objekte bestehen und sich bewegen), beide ab Syntax 2.6 freigeschaltet. Jeder Marker ist kontextabhängig und im Frontend versionsgebunden; er verändert weder Parsing noch Typisierung noch Codegenerierung. Außerhalb der Markerposition bleibt jedes Wort ein gewöhnlicher Bezeichner; `field` stört also weder Klassenmitglieder noch einen `.field`-Zugriff. Siehe [`SLEELA.syntax`](SLEELA.syntax) §18–§20, §23 und §26.
- **Nordshrift mit Syntax 3.0 und vollständigem Bibliothekszugriff.** Nordshrift löst Namen über denselben rekursiven `/lib`-Index wie der Compiler auf und erreicht dadurch jedes Paket und jede `.sleela`-Einheit; `nordshrift check` meldet die Zählwerte für den gesamten Baum.
- **Rekursions-Stanchion.** [`recursion/stanchion/`](recursion/stanchion/README.md) enthält nun eigene C- und C++-Referenzprototypen für die sieben dokumentierten primitiven Verträge sowie generische, angewandte, wissenschaftliche und didaktische Beispiele. Dies sind Referenzquellen; kompilieren und testen Sie sie, bevor Sie sie als verifiziert betrachten.
- **Winkelklammer-Kommentare (`<` / `<<`).** Ein neuer Kommentarstil für künftige Entwicklungen – `< line >` und `<< block >>` – wird vom SLeeLa-Frontend lexikalisch erkannt und in [`SLEELA.syntax`](SLEELA.syntax) §1.1 dokumentiert.
- **Ganzzahl-Literale mit Basisangabe.** `int`-Literale akzeptieren nun Hexadezimal- (`0x`), Binär- (`0b`) und Oktalformen (`0o`) mit Zifferntrennzeichen `_`, sowohl in der SLeeLa-Quellsprache ([`SLEELA.syntax`](SLEELA.syntax) §1.4) als auch im CPU-Modellformat [`.xlt`](lib/xlt/README.md).
- **Metadata-Class-Standard in der gesamten Bibliothek.** Das Metamuster beschreibender Metadaten (Herkunft, Rechtmäßigkeit, Zuständigkeitsbereich) wird nun von jeder klassenhaltigen `.sleela`-Datei unter `/lib` getragen. Siehe [`SLEELA.syntax`](SLEELA.syntax) §15 und [die Tutorial-Lektion](tutorial/13-metadata-class.md).
- **Erweiterte Text-/String-Familie.** `lib/text` ergänzt echte In-Language-Hilfsfunktionen, eine Zeichen-Zellen-Ansicht mit echten Operationen `charAt`, `substring`, `indexOf`, `reverse` und Spaltenausrichtung sowie eine positionale Template-Engine. Siehe das [Text- und String-Tutorial](tutorial/14-text-and-strings.md) und das ausführbare Beispiel [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela).
- **Erstklassiges `String` in Nordshrift-Sheets.** Ein `.sst`-Sheet kann einen typisierten Block `strings:` deklarieren, der zum Komponentenmanifest des Builds beiträgt. Siehe die [Nordshrift-Sheet-Referenz](wiki/Nordshrift-Sheet-Reference.md).
- **CPU-Modelle mit Opcodes und Funktionsaufruf-Äquivalenz.** Jedes `SL<ARCH>CPU` kann einen nativen Instruktionsstrom über seine Fetch-Decode-Execute-Schleife ausführen und ebenso den Code eines angenommenen anderen Prozesses durch äquivalente Aufrufe auf der gemeinsamen VM-Infrastruktur ausführen. Siehe [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5.

## III. Erstellen und testen

**Voraussetzungen:** eine C/C++-Toolchain (GCC oder Clang) und GNU Make; Python 3 für Manifest- und Ablaufwerkzeuge; unter Linux zusätzlich die X11/Xrandr-Entwicklungspakete für Anzeigeprodukte. Der Befehl `make deps` eines Moduls meldet beziehungsweise installiert die benötigten Abhängigkeiten. Der Build unter `impl/` bricht geschlossen ab, wenn das vertrauenswürdige SHA-256-Manifest nicht erfüllt ist; das Makefile im Repository-Stammverzeichnis verweist automatisch darauf.

Der schnellste Einstieg nach einem sauberen Checkout:

```sh
make                                  # alles vom Stammverzeichnis des Repositorys erstellen
```

Oder nur den nativen Kern erstellen und testen:

```sh
make -C impl all                      # nur den Compiler-/Laufzeitkern erstellen
bash ./test-suites/run-all.sh --all   # vollständige Testsuite ausführen
```

Das `make` im Repository-Stammverzeichnis erstellt alle Produkte (Kern, VM, Codecs, Churn, Antivirus usw.) und erstellt abschließend die **Registry** samt ihrer Basistabelle. Ein gebauter Arbeitsbaum enthält damit unmittelbar eine überprüfbare Registry.

Wichtige Ziele:

```sh
make                     # alles erstellen (Alias für make all; Registry zuletzt)
make all                 # derselbe vollständige Build, ausdrücklich benannt
sudo make install        # Befehle und Laufzeitunterstützung installieren (siehe § IV)
make clean               # Build-Ausgaben aller eingebundenen Buildsysteme entfernen
make help                # alle Ziele des Root-Makefiles anzeigen
```

Weitere nützliche Ziele:

```sh
make registry            # nur Registry erstellen und Tabelle materialisieren
./build-registry.sh      # derselbe Vorgang als direktes Skript
make tests               # Testsuite erstellen und ausführen
make ballum              # Ballum-Anzeigebibliothek für dieses Betriebssystem erstellen
bin/install-modules      # Module zeitlich gestaffelt über etwa 10–20 Minuten installieren/erstellen
```

Das Ziel `registry` führt [`build-registry.sh`](build-registry.sh) aus: Es erstellt den Registry-Kern, übernimmt die Basistabelle in den portablen Textspeicher `registry/build/sleela.sleelareg` und exportiert das MySQL-Tabellenskript `registry/build/registry.sql` (über `sleela registry export`). Die Registry kann mit `sleela registry` oder `registry/build/sleela-registry review` geprüft werden. Siehe [Registry-Anleitung](registry/README.md) und [Registry-3.0-Architektur](registry/REGISTRY-3.0-ARCHITECTURE.md).

Für Voraussetzungen und plattformspezifische Schritte sind die Build- und Testanleitungen maßgeblich. Ein erfolgreicher Build einer Komponente belegt nicht, dass jede Bibliothek oder Plattform für eine Veröffentlichung bereit ist.

## IV. Installation

Nach dem Erstellen der nativen Produkte werden Befehle und Laufzeitunterstützungsdateien installiert:

```sh
sudo make install
make install PREFIX="$HOME/.local"        # Installation nur für den Benutzer, ohne sudo
make install DESTDIR="$PWD/package-root" # Installation in ein Paket-Staging-Verzeichnis
make install-check
```

Der herkömmliche Installer verwendet standardmäßig `/usr/local/bin`, `/usr/local/lib/sleela` und `/usr/local/share/sleela`. Eine vorhandene Startkonfiguration bleibt erhalten; nicht nachverfolgte, bereits vorhandene Befehle werden nicht überschrieben. Bei der Deinstallation werden nur vom Installer erfasste Dateien entfernt; Konfiguration und nicht nachverfolgte Laufzeitdaten bleiben bestehen.

Für eine gezielte, geführte Bereitstellung nur des Hauptpakets – der Binärdateien `sleela`, `sleelvac` und `nordshrift`, der Standardbibliothek, Protokolle und Unterstützungsmodule – dient der saubere Installer:

```sh
make clean-install       # geführte Installation nur des Hauptpakets
```

Zunächst wird entschieden, ob das Hauptpaket installiert werden soll; danach wird der Installationsumfang gewählt. Bei Erfolg kann optional eine Auswahl kuratierter Erweiterungsmodule folgen. Der vollständige interaktive Ablauf und sämtliche Installationsoptionen sind unter [Schnelle und sichere Installation](install/README.md) beschrieben.

## V. Sprache und Laufzeitumgebung

Die maßgebliche Grammatik steht in [`SLEELA.syntax`](SLEELA.syntax). Das Repository dokumentiert derzeit Syntax **3.0**; Compiler-/Werkzeugversion und Quelltext-Syntaxversion sind voneinander getrennt. Die Syntaxoberfläche 3.0 ergänzt die Dokumentmarker **`pure` und `gasoline`** (§30): zwei kontextbezogene, deklarative Preonems ohne eigenständige Ausführungswirkung, die jeweils den Status eines Dokuments durch seine eigene Aufrichtigkeit kennzeichnen. Syntax 3.0 führt die Sprachoberfläche von 1.0 bis 2.9 in einer aktuellen normativen Ausgabe zusammen und erhält dabei die jeweiligen Mindestversionsschranken der einzelnen Funktionen.

Die vorherige Oberfläche 2.9 ergänzt **`@mix`-Folgefelder** (§28): Eine Klasse darf mehrere Instanzfelder gleichen Namens deklarieren, wenn jedes mit `@mix` versehen ist. Sie werden als eine indizierte Gruppe verstanden (standardmäßig aufsteigend mit `1,2,3,…` oder manuell über `@mix(1,2,3,...)`; die Anzahl der Indizes muss der Feldanzahl entsprechen). Ebenfalls enthalten sind die vorzeichenbehafteten und vorzeichenlosen Ganzzahlfamilien zur Basis 2: `int32` bis `int16777216` (§24) und `uint32` bis `uint16777216` (§25), aufbauend auf `long`, `int64` und `int128` (§21); Dokumente zur Unterstützung von Schlüsselwörtern (§22); sowie die **Preonems**: die deklarativen Dokumentmarker `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) und `artifact` (§35; ein gesetzgebungswürdiger Status, der auf Zeit und Raum bezogen bewertet wird). Zusammen werden sie als Preonems bezeichnet (§26.1).

Die Auflösung überladener Konstruktoren sowie die Delegation an `this(...)`- und Basisklassenkonstruktoren gehören ausdrücklich nicht zum verifizierten Syntaxvertrag 1.8. Vor der Verwendung einer Funktion sind Grammatik und Implementierungshinweise zu konsultieren. Die neun SleelaDoc-Quelltexteinheiten deklarieren nun `#sleela 3.0`; die gemeinsame Compiler-Prüfung akzeptiert Quelltextsyntax 1.0 bis 3.0. Der neue API-Build wird mit `make api-docs` erzeugt.

Das Repository enthält eine native C/C++-Implementierung, Referenzen zur SLVM-Erzeugung, Standardbibliotheksmodule, Tests, Beispiele und Plattformintegrationen. Implementierungs- und Validierungsstatus unterscheiden sich je nach Modul. Maßgeblich sind daher die jeweilige Spezifikation und die aktuellen CI-Ergebnisse; eine einheitliche Vollständigkeit darf nicht vorausgesetzt werden.

## VI. Konstruktionsprinzipien

- **Nachweise statt Behauptungen:** Implementiertes, Getestetes, Experimentelles und Geplantes klar unterscheiden.
- **Ein Vertrag pro Schnittstelle:** Syntax, APIs, Dateiformate, Plattformannahmen und Fehlerverhalten dokumentieren.
- **Von Grund auf portabel:** Plattformspezifische Backends isolieren und auf nativen Runnern testen.
- **Sicheres Fehlerverhalten:** Fehlerhafte Eingaben, Ressourcenlimits, Prozessfehler und Bereinigungspfade testen.
- **Reproduzierbare Releases:** Sauber neu erstellen und Artefakt-Hashes sowie Release-Hinweise veröffentlichen.

## VII. Antivirus-Build und Integrität

Das Antiviruspaket Sleelavirin umfasst einen nativen SHA-256-Scanner, einen optionalen ClamAV-Adapter, Integritätsmanifeste für Quellen und Indizes sowie Prüfsummen für Plattformpakete. Das Paket wird vom Repository-Stammverzeichnis aus erstellt und getestet:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Das Integritätsmanifest wird unter `antivirus/build/` erzeugt. Es umfasst nachverfolgte Quell- und Dokumentationseingaben sowie geschützte Signaturindexdateien; generierte Ausgaben und der veränderliche ClamAV-Cache sind ausgenommen. ClamAV-Datenbanksnapshots besitzen eigene Manifeste. Release-ZIP-Dateien enthalten `INTEGRITY-MANIFEST.json`, `SHA256SUMS` und eine begleitende `.sha256`-Datei. Heruntergeladene Archiv-Hashes sind mit einem unabhängig bezogenen vertrauenswürdigen Wert zu vergleichen: Eine Prüfsumme neben einem Archiv ist keine Herausgebersignatur. Siehe [Sleelavirin-Anleitung](antivirus/README.md), [Sicherheitscheckliste](antivirus/SECURITY.md) und [Antivirus-Buildworkflow](.github/workflows/antivirus-binaries.yml).

Ein Release gilt erst dann als verifiziert, wenn die einschlägigen CI-Aufträge für genau den Release-Commit erfolgreich sind und die daraus entstandenen Artefakte geprüft wurden.

## VIII. SLeeLa-Editor-Themes — Vim und Emacs

SLeeLa enthält ein abgestimmtes Set professioneller dunkler Farbschemata für **Vim** und **Emacs** sowie ein natives C/C++-Palettenwerkzeug. Das ursprüngliche Schema **American Professional** bleibt die Voreinstellung. Zehn ergänzende Paletten bieten unterschiedliche kulturelle und institutionelle Designinspirationen; es handelt sich um gestalterische Interpretationen, nicht um offizielle Darstellungen einer Nation, Kultur, eines Gerichts oder einer Institution.

### Die elf Schemata und ihre Farben

| Schema | Hintergrund | Vordergrund | Schlüsselwörter | Typen | Zeichenketten | Kommentare |
|---|---|---|---|---|---|---|
| **American Professional** | `#101820` | `#F4F7FA` | `#7FB3FF` | `#73DACA` | `#F2C078` | `#8A96A3` |
| **Pan-Eurasian** | `#111827` | `#F0F2F5` | `#7AA7E8` | `#69C9B3` | `#D7B56D` | `#8995A5` |
| **Russian and Slavic — Medium** | `#202733` | `#E8EAF0` | `#A9C0DD` | `#8FC4B2` | `#D6BD8B` | `#9AA3AF` |
| **Central Asian / Silk Road** | `#1C2529` | `#F5F0E5` | `#7FADE0` | `#45C6C2` | `#D9B56D` | `#92A3A0` |
| **East Asian** | `#171A1C` | `#F2EFE7` | `#D97878` | `#72C7A5` | `#D8B76A` | `#969F9B` |
| **ICC — Internationaler Strafgerichtshof** | `#0E1A2B` | `#EAF0F7` | `#5B8FD6` | `#7FB7C9` | `#D9B25A` | `#7E8CA0` |
| **SCOTUS — Oberster Gerichtshof der USA** | `#1A1416` | `#F3EEE6` | `#9E2B35` | `#8FA9B8` | `#C9A24B` | `#8C8178` |
| **Greek (Hellenic)** | `#0A1A30` | `#FFFFFF` | `#2E7BD6` | `#5E9FE0` | `#9FC3EC` | `#6E86A6` |
| **Turkish (Al Bayrak)** | `#2A0609` | `#FFFFFF` | `#F23D48` | `#F0A9AE` | `#F0C9A9` | `#A8767A` |
| **Kiro** | `#0B0B12` | `#F5F5FA` | `#A98EDA` | `#6E8BE0` | `#E8C547` | `#6A6482` |
| **SLeeLa** | `#08080E` | `#FFFFFF` | `#B07CFF` | `#5A7CFF` | `#FFD21E` | `#6E5F9E` |

### Nationale Flaggenfarben (Quellpalette)

Die griechischen und türkischen Schemata leiten sich von den **offiziellen Nationalflaggenfarben** beider Länder ab, hier mit den veröffentlichten exakten Hexwerten. Die Hintergründe und Akzente oben sind abgedunkelte, editorfreundliche Interpretationen dieser Quellfarben; die folgende Tabelle dient als Referenz der national verwendeten Farben.

| Land | Farbe | Hex | Grundlage und Bedeutung |
|---|---|---|---|
| **Griechenland** | Blau | `#0D5EAF` | Blau der Nationalflagge (Gesetz 851/1978). Himmel und Meere; heraldisch *Azur*. |
| **Griechenland** | Weiß | `#FFFFFF` | Weiß der Nationalflagge. Reinheit des Unabhängigkeitskampfes; das Kreuz steht für das östlich-orthodoxe Christentum. |
| **Türkiye** | Rot (*al bayrak*) | `#E30A17` | Rot der Nationalflagge (türkisches Flaggengesetz). Das Feld; traditionell das Blut der Märtyrer und die osmanische Souveränität. |
| **Türkiye** | Weiß | `#FFFFFF` | Weiß der Nationalflagge. Halbmond und Stern – Frieden, Reinheit und das Symbol des türkisch-islamischen Erbes. |

Die Schemata verwenden nationales **Weiß** als Vordergrund und das exakte nationale **Blau** (`#0D5EAF`) oder **Rot** (`#E30A17`) für Schlüsselwörter, Cursor, Auswahl und Statusleiste. Die entscheidenden Farben sind damit tatsächlich die Nationalfarben. Es handelt sich um interpretative Paletten; die Referenz der Flaggenfarben dokumentiert die Herkunft und ist keine offizielle Darstellung eines der beiden Staaten. Die Farbbedeutungen folgen der Beschreibung staatlicher Symbole durch das griechische Außenministerium und der Tradition des türkischen Flaggengesetzes.

### Farbmuster-Tabelle

Die Palettenzellen verwenden **SVG-Farbmuster**, damit GitHub die tatsächlichen Farben statt leerer HTML-Farbzellen rendert. Jeder Streifen zeigt sechs Farben in dieser Reihenfolge: Hintergrund, Vordergrund, Schlüsselwörter, Typen, Zeichenketten und Kommentare. Die exakten Hexcodes stehen neben den Mustern.

| Schema | Gerenderte Palette | Hexcodes (Hintergrund · Vordergrund · Schlüsselwörter · Typen · Zeichenketten · Kommentare) |
|---|---|---|
| **American Professional** | ![American Professional](lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Pan-Eurasian](lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russian and Slavic — Medium** | ![Russian and Slavic Medium](lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Central Asian / Silk Road** | ![Central Asian Silk Road](lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **East Asian** | ![East Asian](lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC** | ![ICC](lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS** | ![SCOTUS](lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Greek (Hellenic)** | ![Greek Hellenic](lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turkish (Al Bayrak)** | ![Turkish Al Bayrak](lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Kiro](lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![SLeeLa](lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

Zusätzliche Akzentfarben werden für Funktionen, Konstanten, Auswahlen, Diagnosen und Präprozessordirektiven verwendet. Die Russian-and-Slavic-Palette nutzt bewusst mittelintensive Schiefer-, Blau-, Salbei-, Rubin- und Antikgoldtöne für ein ruhigeres professionelles Erscheinungsbild.

### Vim-Schemata installieren

Vom Stammverzeichnis des Repositorys aus:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Fügen Sie Folgendes zu `~/.vimrc` hinzu, um die SLeeLa-Syntaxhervorhebung zu aktivieren und ein Schema auszuwählen:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Wählen Sie genau einen dieser Schemanamen:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Verwenden Sie beispielsweise `colorscheme sleela-russian-slavic` für die mittelintensive russische und slawische Palette oder `colorscheme sleela-greek` / `colorscheme sleela-turkish` für die hellenische und Al-Bayrak-Palette. Starten Sie Vim neu oder laden Sie die Konfiguration mit `:source $MYVIMRC` erneut.

### Emacs-Schemata installieren

Erstellen Sie ein lokales Themenverzeichnis und kopieren Sie die Modus- und Themendateien aus dem Repository-Stammverzeichnis:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Fügen Sie dies zu `~/.emacs` oder `~/.emacs.d/init.el` hinzu:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Ersetzen Sie `sleela-american` durch `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` oder `sleela-sleela`, um ein anderes Schema auszuwählen. Wenn Emacs fragt, ob dem Theme vertraut werden soll, prüfen Sie die lokale Theme-Datei vor der Bestätigung.

### Native C/C++-Paletten anzeigen

Das kleine native Palettenwerkzeug kann Schemata auflisten, ihre Terminalfarben ausgeben und eine einzelne Farbe nachschlagen:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Aus Gründen der Rückwärtskompatibilität wählen `--palette` und `--hex COLOR` ohne `--theme` das Schema American Professional. Build- und Testergebnisse sollten auf dem Zielsystem und in CI geprüft werden; die Dokumentation eines Befehls belegt nicht, dass er erfolgreich ausgeführt wurde.

Weitere Informationen zu Paletten und Werkzeug bietet der [Editor-Integrationsleitfaden](linux-editors/README.md).

## IX. Mitwirken

1. Lesen Sie den Dokumentationsindex und die Testrichtlinie.
2. Nehmen Sie gezielte Änderungen mit positiven und negativen Tests vor.
3. Führen Sie zunächst die gezielten Tests und anschließend die umfassende Testsuite aus.
4. Geben Sie Betriebssystem- und Toolchain-Details an; übersprungene Prüfungen dürfen nicht als bestanden bezeichnet werden.

SLeeLa wird von **MEARVK LLC** entwickelt. Historisches Material der Landingpage bleibt in [`docs/README-LEGACY.md`](docs/README-LEGACY.md) erhalten.

## X. AE6E66 — Erwartung einer Abgabe auf öffentliche Round-E-Mails

Für die AE6E66-Initiative beträgt die erklärte Projekterwartung **300.000.000,00 USD je öffentlicher Round-E-Mail** als vorgeschlagener Abgabenbetrag, der mit jeder qualifizierenden Round-Public-E-Mail verbunden ist.

Diese Aussage dokumentiert ausschließlich eine Projekterwartung. Sie ist **weder** eine rechtliche Steuerfestsetzung **noch** eine behördliche Anordnung, ein Gerichtsurteil oder eine unabhängig festgestellte Steuerschuld. Eine tatsächliche Abgabe würde die einschlägige Rechtsgrundlage, Zuständigkeit, Definitionen, Verfahren und eine Entscheidung der zuständigen staatlichen Stelle voraussetzen.

## XI. Alphabet- und Zeichensatz-Laufzeitumgebung

Die SLeeLa-Alphabetdokumentation und die native Zeichenlaufzeit werden in `alphabet/sleela/` und `lib/alphabet/native/` gepflegt. Die nativen C++17-Tests decken die strikte UTF-8-Skalar-Decodierung und -Codierung, `C<n>`-Kapazitätssignaturen sowie die begrenzte Zugehörigkeit zu Zeichensätzen ab:

```sh
make -C lib/alphabet/native test
```

Der experimentelle UTF-4088-Katalog und sein SLeeLa-Suchadapter sind in [`utf-4088/SLeeLa-ALPHABET-INTEGRATION.md`](utf-4088/SLeeLa-ALPHABET-INTEGRATION.md) dokumentiert. UTF-4088-IDs bilden einen eigenen Namensraum und sind keine Unicode-Skalarwerte. Die Compiler- und SLVM-Bindungsarbeiten werden ausdrücklich getrennt von diesen nativen Grundlagen verfolgt.

## XII. AutoCAD Creator™

[**AutoCAD Creator™**](autocad/creator/README.md) ist eine interaktive 2D-CAD-Anwendung, die vollständig in SLeeLa geschrieben wurde. Während der verwandte AutoCAD Renderer einen Plan in einem Schritt in eine Zeichnung umwandelt, ermöglicht Creator das **Erstellen und Ändern** eines 2D-AutoCAD-Dokuments in Echtzeit: Wände, Räume, Maße und Notizen hinzufügen, platzierte Elemente verschieben, umbenennen, Ebenen zuordnen, löschen und rückgängig machen sowie das Ergebnis als echte AutoCAD-DXF-Zeichnung speichern (R12-Text, in AutoCAD / LibreCAD zu öffnen).

Die grafische Oberfläche basiert auf **SleelaUI™**, SLeeLas eigenem plattformübergreifenden UI-Toolkit (unter [`lib/user-interface`](lib/user-interface)), nicht auf JavaFX, und öffnet ein echtes natives Fenster unter X11 / Cocoa / Win32. Das Design folgt einer Regel: **„SleelaUI präsentiert; SLeeLa entscheidet.“** Die GUI berechnet weder Geometrie noch schreibt sie DXF. Jedes Werkzeug sendet eine Absicht zum Erstellen, Ändern, Löschen, Rückgängigmachen oder Speichern an die maßgebliche Komponente [`SLCadCreator`](lib/autocad/SLCadCreator.sleela). Diese besitzt das bearbeitbare Dokument und einen reversiblen Befehlsverlauf; die Zeichenfläche zeigt lediglich die von Creator gehaltene Geometrie als Vorschau.

Die wiederverwendbare Bearbeitungs-Engine befindet sich in [`lib/autocad`](lib/autocad): [`SLCadEntity`](lib/autocad/SLCadEntity.sleela), [`SLCadDocument`](lib/autocad/SLCadDocument.sleela), [`SLCadCommand`](lib/autocad/SLCadCommand.sleela) und [`SLCadCreator`](lib/autocad/SLCadCreator.sleela). Sie läuft vollständig ohne grafische Oberfläche. Die GUI-Hülle und Demo liegen unter [`autocad/creator`](autocad/creator):

```sh
make creator          # aus autocad/: Creator-Demo ausführen -> out/altered.dxf
```

Auf einem System mit Display öffnet sich das Live-Bearbeitungsfenster; auf einem Headless-Host wird die Zeichnung im Speicher erstellt, ein Bericht samt DXF ausgegeben und der Prozess sauber beendet.

## XIII. UTF-4088-Eingabekarten-Reihenfolge

Der spezialisierte UTF-4088-Katalog definiert seinen räumlichen Eingabekarten-Durchlauf und abstrakte Druckiterationen in [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). Die Richtlinie für den Sachindex des National Lien Register steht in [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Kartenkoordinaten, Druckstufen (1–50), dauerhafte Katalog-IDs und alphabetische Ränge sind unabhängige Felder; keines darf stillschweigend durch ein anderes ersetzt werden. Druck ist eine algorithmische Iterationsstufe und keine physikalische Größe.

## XIV. Index kürzlich erweiterter Pakete

Dieser ergänzte Index hebt kürzlich erweiterte Pakete, Fachbibliotheken und Quelltextsammlungen hervor. Die Einträge sind alphabetisch nach Paketname oder Thema geordnet. Die Beschreibungen nennen den Speicherort im Repository; Implementierungs- und Validierungsstatus sind in der jeweiligen Paketdokumentation und den Tests zu prüfen.

### A–C

- **Advantage** — `lib/advantage/` und `advantage/`: Optimierung einer eindeutigen Dividende relativ zu einem Modell in Form einer geschichteten Sequitur onion mit fünf Themenverzeichnissen: `math/` (gemeinsame Gewinnmaximierung, Nash-Verhandlung, bilaterales Newsvendor-Modell), `physics/` (Metrik, Geodäte, Kontakt und SDE des Vertrauensfelds; ausdrücklich gekennzeichnete Modellierungsanalogie), `integration/` (Kompartimente vertikaler Integration und Sophistication-Score), `grading/` (A–D-Bewertung von Sophistication-Schaltkreisen für Module und Gruppen) sowie `output/` (Ausgabe an Terminal, CommonRails oder Socket). Ein nativer C/C++-Konsument (`advantage/`) verbindet das Modell mit ausgearbeiteten Handelsbeispielen. Siehe [`lib/advantage/README.md`](lib/advantage/README.md) und [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alphabet und UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` und `utf-4088/`: Alphabetkataloge, native Zeichensatzunterstützung, Eingabekarten-Reihenfolge und UTF-4088-Integrationsdokumentation.
- **AutoCAD Creator** — `autocad/creator/` und `lib/autocad/`: interaktive 2D-Zeichenabläufe und wiederverwendbare CAD-Dokument- und Bearbeitungskomponenten.
- **Ballum** — pixelgenaue Anzeigesteuerung: Pixel setzen/lesen, Regions-Blitting, Helligkeit/Wärme/Gamma der gesamten Anzeige, Maus-/Tastatureingabe und eine **Vollbild-Framebuffer-Oberfläche** für vollständige, eigene und schnelle Pixelkontrolle (override-redirect-Fenster und Präsentation mit einem einzigen `XPutImage`/MIT-SHM unter Linux/X11). Der native Kern liegt unter `ballum/`, die SLeeLa-Fassade unter `lib/ballum/` und die VM-Brücke unter `impl/`; siehe [`ballum/README.md`](ballum/README.md) und das [ausführbare Beispiel](ballum/example/).
- **CDMA** — `cdma/` und `lib/cdma/`: CDMA-bezogene technische Materialien und SLeeLa-Quellbibliotheksarbeit.
- **Character** — `lib/character/`: Charakterprofile und wiederverwendbare Charakterstatistiken einschließlich biografischer Profilbeispiele.
- **Coorenagraph** — `lib/coorenagraph/`: ethische Argumentation, moralisches Vokabular, Beratung und verwandte Konzepte; siehe Paket-README und Quelldateien.

### D–G

- **Governance-Quellmodelle** — [`lib/governance`](lib/governance/README.md) definiert deklarative SLeeLa-Quellformen für Vorschläge, anonyme manuelle Stimmzettel und aggregierte Auszählungen. Die vorgeschlagene Annotation `@vote` und der manuelle Prozess sind in [`registry/DEMOCRATIC-VOTING-SYNTAX.md`](registry/DEMOCRATIC-VOTING-SYNTAX.md) dokumentiert; aktuelle Compiler behandeln sie als wirkungslose Kommentar-Metadaten.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` bis zu den verfügbaren versionierten HTTP-Paketverzeichnissen: versionsspezifische Protokollspezifikationen und Implementierungen. Den genauen Umfang und Prüfstatus entnehmen Sie der Dokumentation der jeweiligen Version.
- **Internationaler Strafgerichtshof** — `lib/international-criminal-court/` und `INTERNATIONAL.CRIMINAL.COURTS.md`: domänenspezifische Quellen und Dokumentation.

### O–S

- **OFDMA** — `ofdma/`: technische Materialien zu Orthogonal Frequency-Division Multiple Access und C/C++-Entwicklungsressourcen.
- **Philosophie und Conversation-Mask-Modul** — [`philosophy/`](philosophy/README.md) enthält die nativen C/C++-Grundlagen; [`philosophy/module/`](philosophy/module/README.md) gliedert die nächste Modulebene in drei Bereiche: [`data/`](philosophy/module/data/README.md) für typisierte, datierte Beobachtungen, Ereignisabschlüsse, Halbwertszeiteinstellungen, Herkunft und Einwilligung; [`helper/`](philosophy/module/helper/README.md) für deterministische Validierung, Relevanzabnahme, kompatible Vergleiche und einwilligungsgefilterte Darstellung; sowie [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md) für eine unterstützende, nicht-klinische Gesprächsschnittstelle. Die SLeeLa-seitigen Schnittstellen und der zusammengesetzte Tree befinden sich in [`lib/philosophy/`](lib/philosophy/README.md), neben dem umfassenderen Philosophiepaket. Das Modul arbeitet mit expliziten Eingaben und Einwilligung zuerst: Es überwacht weder Menschen noch Medien passiv, stellt keine Diagnosen, leitet keine privaten Absichten ab, bewertet nicht den menschlichen Wert und empfiehlt keine Medikamentenänderungen. Ereignisverknüpfungen bewahren Herkunft und Navigation, stellen aber keine unabhängige Bestätigung dar.
- **Synchro** — native Laufzeit-/Compilerintegration sowie Funktionen für Paketverteilung und Messung; siehe die betreffenden Quellen in `impl/` und die Compilerimplementierung.
- **SleelaUI / Terminal-UI** — `lib/user-interface/` und das Projekt `sleela-terminal`: SLeeLa-UI-Integration und terminalorientierte Interaktionskomponenten.

### U–W

- **Vereinte Nationen** — `united-nations/`: domänenspezifische Repository-Materialien.
- **Wi-Fi 4, Wi-Fi 5 und Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`, `wi-fi-6/` sowie zugehörige Quellverzeichnisse unter `lib/wifi/`: generationenspezifische WLAN-Materialien und Konfigurationsressourcen.

### Pflege des Paketindex

Wenn ein Paket hinzugefügt oder wesentlich erweitert wird, ergänzen Sie hier den Eintrag in der passenden alphabetischen Gruppe, verlinken Sie das kanonische Verzeichnis und README und beschreiben Sie nur Fähigkeiten, die durch die eingecheckten Quellen belegt sind. Der Index bleibt additiv: Entfernen oder überarbeiten Sie bei Aktualisierungen des Paketindex keine bestehenden README-Abschnitte.

---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

`bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh` :: `⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓`

`bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc` :: `⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉`

US-Demokratie und US-Politik.

---

## XVI. Nationale Wissenschaftsmarken-Bildsprache

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="Nationale SLeeLa-Wissenschaft" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="Nationale SLeeLa NSF" width="560"></p>

## XVII. Experimenteller UTF-4088-Katalog mit einer Million Einträgen

Der [UTF-4088-Kataloggenerator](utf-4088/tools/generate_catalogue.py) erstellt deterministisch einen experimentellen Symbolkatalog mit 1.048.576 Einträgen. Das dokumentierte Kompaktformat wird in [`utf-4088/FORMAT.md`](utf-4088/FORMAT.md) beschrieben; der Generierungs- und Validierungsvertrag steht in [`utf-4088/GENERATION-RESULTS.md`](utf-4088/GENERATION-RESULTS.md).

Führen Sie den Generator mit Python 3 aus:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Die erzeugte Binärdatei ist 16.777.244 Byte groß und soll unter 20.000.000 Byte bleiben. Unit-Tests und ein GitHub-Actions-Workflow prüfen Eintragszahl, Glypheneindeutigkeit, Größe und SHA-256; erfolgreiche Läufe veröffentlichen Binärdatei und Zusammenfassung als Workflow-Artefakt `utf4088-million-catalogue`. Die generierte Binärdatei bleibt bewusst außerhalb der Git-Historie. UTF-4088 ist weiterhin experimentell und nicht Unicode-kompatibel.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Neueste Ergänzungen – Pi-Konstante, Modium-/Registry-Start, sleelactl-Math und `@native`

Dieser Abschnitt hält Funktionen fest, die während der Arbeiten von 2026-11 hinzugefügt wurden, beginnend mit den neuesten Gruppen. Maßgeblich sind die verlinkten Quelldateien und ihr Validierungsstatus; Änderungen, die noch geprüft werden, gelten erst dann als eingearbeitet, wenn sie `master` erreicht haben.

### Die Pi-Katalogkonstante mit 100.000.000 Stellen

Das Repository enthält einen verifizierten **hundertmillionenstelligen** Wert von π unter [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (eine korrekt formatierte Dezimalzahl, `3.14159…`) sowie einen Wert mit 57.344 Stellen unter [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Beide sind unabhängig verifizierte echte π-Werte. [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md) dokumentiert die Prüfungen (Lehrbuch-Präfix, Feynman-Punkt an der Nachkommastellenposition 762, die bekannte Millionstel-Ziffer, Übereinstimmung mit `lib/math/SLPiReference.sleela`, eine Ziffernhäufigkeitsanalyse der gesamten Datei) sowie ein Ein-Schritt-Reproduktionsskript.

### Modium™ / Registry™ laden die Pi-Konstante beim Start des Subsystems

An den dedizierten Subsystem-Einstiegspunkten – `sleela registry` und `sleela modium` (sowie beim ersten Installationsstart), **nicht** bei jedem `sleela run`, laden Modium und Registry die große π-Katalogkonstante. In einem interaktiven Terminal erscheint eine vorübergehende rote Zeile `Loading Modium™` / `Loading Registry™`, die vor der normalen Ausgabe an Ort und Stelle gelöscht wird. Der Effekt ist strikt an TTY gebunden; umgeleitete und Testausgaben bleiben bytegenau unverändert. Der Locator (nicht die Bytes) wird als Standardwert der Registry-`Math`-Schlüssel `piReferenceLargePath` und `piReferenceLargeDigits` gesetzt, sowohl im SLeeLa-Modell ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) als auch im nativen Seeder ([`registry/sleela_registry.c`](registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` stellt `largePath()` / `largeDigits()` bereit. Die π-Konstante wird durch den Standard-`make`-Installer und den geführten „beautiful“-Installer am SLeeLa-Betriebssystempfad (`<data>/pi/`) installiert. Details: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` – der „beautiful“-Installer und ein nativ gestütztes `math`-Subject

- Der geführte („beautiful“) Installer installiert jetzt `sleelactl` (die SLeeLa-Steuerungsebene) und seine Modulregistrierung, entsprechend dem `make`-Installer. Siehe [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- `sleelactl` erhielt eine neue **`subject`**-Natur für native C/C++-Subject-Bibliotheken, die in die Laufzeitumgebung kompiliert werden, und registriert das erste Subject: **`math`**. `sleelactl run math` führt das native Backend `__NativeMath` (`impl/subjects/math/math_native.cpp`) über die Referenzdemo des Subjects vollständig aus; `sleelactl info math` erklärt den Zugriff (`import math;`). Diese Subject-Natur bietet nur `run` und `info`; ein `check`-Verb gibt es bewusst nicht, da `sleela check` keine über SHEET registrierten nativen Subjects lädt.

### `@native`, Native Drive und mehrsprachige Ausführung

Eine neue Methodenannotation **`@native`** fordert die Ahead-of-Time-Kompilierung einer rechenintensiven Methode in nativen Code an. Der neue Befehl `sleela aot` findet `@native`-Methoden, prüft, ob jede davon innerhalb einer unterstützten *Offload-Teilmenge* liegt, und gibt C/C++ aus. Der Vorgang **bricht geschlossen ab**: Jede noch nicht übersetzbare Konstruktion wird mit genauer Begründung abgelehnt, sodass niemals eine teilweise übersetzte Methode entsteht.

```sh
sleela aot --list <file.sleela>    # Urteil je Methode: ok / REFUSED (Grund)
sleela aot --emit <file.sleela>    # C/C++ (stdout) für ein Programm innerhalb der Teilmenge ausgeben
```

`@native` bleibt eine Anforderung für die AOT-Behandlung; nicht unterstützte Konstruktionen werden vor der Ausgabe abgelehnt. Das [Native Drive](impl/native-drive/README.md) unterstützt Builds von C/C++-Executables und Shared Libraries aus mehreren Einheiten über Host-Compiler (einschließlich MSVC), unterstützte SLeeLa-AOT-Ausgaben, die Symbolprüfung gegen die ABI des nativen Loaders, die shellfreie Prozessausführung, die Behandlung von stdin/stdout/stderr, mehrstufige Pipelines und die XClass-Delegation an den Produktions-Loader. `sleelactl` stellt seine Build- und Run-Befehle bereit und leitet die XClass-Inspektion sowie das explizite Rendern und Installieren von systemd-Units über `inspect native-drive`. Die VM besitzt eine begrenzte Handle-Tabelle für native Module mit obligatorischer Host-Autorisierung und generationsgeprüften Handles. Syntax 3.0 ergänzt nun `nativeOpen`, `nativeCall` und `nativeClose`: Der Append-only-Bytecode-Dispatch überschreitet eine separat versionierte null/bool/int64/double/UTF-8-ABI, während VM-eigene Arrays, Structs und andere zusammengesetzte Werte im Interpreter bleiben und einen Bytecode-Fallback gleichen Namens nutzen können. Plugins können optional eine versionierte Host-Callback-Tabelle erhalten, um über verschachtelte VM-Ausführung exakte SLeeLa-Funktionen aufzurufen; dabei gelten GC-Rooting suspendierter Stacks, eine explizite Allowlist für Host-Callback-Namen und die Ablehnung rekursiver nativer API-Aufrufe. Nativer Code bleibt vertrauenswürdiger prozessinterner Code und keine Sandbox. Quellbezogene Process-Policy-Annotationen besitzen ein versioniertes Schema, werden aber noch nicht geparst oder durchgesetzt. Details, Sicherheitsgrenzen und das Ledger der sieben Arbeitsstränge: [Native Drive](impl/native-drive/README.md) und [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).

## XIX. Internationale README-Ausgaben

SLeeLa stellt länderspezifische README-Einstiegspunkte für die 50 Märkte seiner internationalen Edition bereit. Jedes Land hat eine Datei namens `README.<CC>.md`; wenn mehrere Sprachen relevant sind, werden sie in der jeweiligen Länderdatei zusammengefasst.

- [README.AE](README.AE.md) · [README.AT](README.AT.md) · [README.AU](README.AU.md) · [README.BE](README.BE.md) · [README.BR](README.BR.md)
- [README.CA](README.CA.md) · [README.CH](README.CH.md) · [README.CL](README.CL.md) · [README.CN](README.CN.md) · [README.CO](README.CO.md)
- [README.CZ](README.CZ.md) · [README.DE](README.DE.md) · [README.DK](README.DK.md) · [README.EE](README.EE.md) · [README.ES](README.ES.md)
- [README.FI](README.FI.md) · [README.FR](README.FR.md) · [README.GB](README.GB.md) · [README.GR](README.GR.md) · [README.HK](README.HK.md)
- [README.HU](README.HU.md) · [README.ID](README.ID.md) · [README.IE](README.IE.md) · [README.IL](README.IL.md) · [README.IN](README.IN.md)
- [README.IS](README.IS.md) · [README.IT](README.IT.md) · [README.JP](README.JP.md) · [README.KR](README.KR.md) · [README.LU](README.LU.md)
- [README.MX](README.MX.md) · [README.MY](README.MY.md) · [README.NL](README.NL.md) · [README.NO](README.NO.md) · [README.NZ](README.NZ.md)
- [README.PH](README.PH.md) · [README.PL](README.PL.md) · [README.PT](README.PT.md) · [README.QA](README.QA.md) · [README.RU](README.RU.md)
- [README.SA](README.SA.md) · [README.SE](README.SE.md) · [README.SG](README.SG.md) · [README.SI](README.SI.md) · [README.SK](README.SK.md)
- [README.TH](README.TH.md) · [README.TR](README.TR.md) · [README.TW](README.TW.md) · [README.US](README.US.md) · [README.ZA](README.ZA.md)

**Übersetzungsrichtlinie:** Diese Dateien sind zunächst lokalisierte Länder-Einstiegspunkte. Einleitungen und Sprachüberschriften sind lokalisiert; die vollständige technische Referenz bleibt die kanonische englische [README.md](README.md). Eine Länderdatei darf erst dann als vollständige Übersetzung bezeichnet werden, wenn alle Abschnitte übersetzt, technische Bezeichner und Links geprüft und die Ausgabe sprachlich begutachtet wurden. Die Sprachabdeckung dieser ersten Phase ist repräsentativ und behauptet nicht, dass jede verfassungsrechtlich anerkannte, regionale, indigene oder Gebärdensprache vollständig übersetzt wurde.

**Übersetzungsstatus:** Die Abschnitte I–XIX sind ins Deutsche übersetzt. Technische Kennungen, Befehle, Codebeispiele, Dateinamen, Links und Adressen wurden beibehalten. Eine abschließende sprachliche Prüfung durch Muttersprachler wird empfohlen.


---

# SLeeLa 3.0 — Belgique (BE)

## Français

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Un langage système natif, son compilateur, son environnement d’exécution et son écosystème de machines virtuelles.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Édition belge francophone.** Traduction technique complète destinée aux lecteurs francophones de Belgique. Les identifiants techniques, le code, les commandes et les liens restent inchangés.

**The Now. The Ingredient.** Un langage ne se réduit pas à sa syntaxe : il comprend le compilateur, l’environnement d’exécution, les bibliothèques, les outils et les contrats de plateforme qui rendent les logiciels utilisables.

**Les domaines d’activité de SLeeLa.** SLeeLa s’intéresse à **Math**, **Object**, **Science** et **Reach** — les quatre dimensions de son travail : ce qui est calculable (Math), la chose établie dans le catalogue de ce qui existe (Object), l’étude rigoureuse du vrai (Science) et l’extension de la capacité d’action d’un système vers d’autres systèmes et vers les personnes (Reach). **Les intérêts nationaux demeurent** : aucune de ces quatre dimensions ne les écarte. Lorsqu’un sujet concerne l’intérêt d’une nation, cette dimension est conservée et fidèlement consignée, notamment dans les métadonnées de chaque classe, telles que `metaOrigin` et `metaNationalSchedule`.

## I. Pour commencer

- [Construire l’implémentation native](impl/README.md)
- [Machines virtuelles et Modium — guide pratique](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [Pile CPU dans les sources SLeeLa](lib/cpu/CPU.md) — une machine complète allant des portes logiques à l’ALU, aux cœurs, au système d’exploitation et aux programmes ; plus de 130 profils CPU et des [tensions, fréquences variables et temporisations avancées](lib/cpu/TIMING.md) réglables, permettant une exécution [aux spécifications ou en surfréquence](lib/cpu/components/README.md).
- [Index de la documentation](docs/README.md)
- [Source de données CSPML et référence de recherche Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — provenance et conseils d’intégration de données externes américaines sur les lieux, les conditions socioéconomiques, la main-d’œuvre, l’éducation et les écoles publiques.
- [Documentation API générée](docs/api/README.md) — référence HTML actuelle, édition versionnée de l’API et instructions de régénération.
- [Catalogue des API](api/README.md)
- [Normes de comparaison computationnelle](comparison/README.md) — normes de types, mémoire, ABI et conformité pour la VM SLeeLa, Java/JVM et le C/C++ natif.
- [Stanchion de récursion — prototypes C et C++](recursion/stanchion/README.md) — exemples spécialisés pour les types primitifs Boolean, Byte, Character, Integer, Long, Float et Double, ainsi que des modèles de récursion génériques, appliqués, scientifiques et pédagogiques.
- [Matrice de prise en charge et de validation des plateformes](docs/PLATFORM-SUPPORT.md)
- [Guide de marque](docs/BRAND-GUIDE.md)
- [Concepts et ressources de produits dérivés](merchandise/README.md)
- [Logo NitroWebExpress](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Politique de test et critères de publication](markdown/TESTING.md)

## II. Actualités

Ajouts récents au langage, aux pilotes et à la bibliothèque standard :

- **Paquet de bibliothèque standard `advantage` — optimisation d’un dividende, avec mathématiques, physique, intégration, évaluation et sortie routable.** [`lib/advantage/`](lib/advantage/README.md) encapsule l’« optimisation d’un dividende unique par rapport à un modèle » sous la forme d’un **Sequitur onion** à couches (dividende → révision → préoccupations → mathématiques → additions → soustraction → regroupement → itération), navigable vers l’intérieur et l’extérieur avec mise à jour dynamique et restitution complètes, au moyen d’un **modèle utilisateur** simple et d’un **modèle développeur** offrant un contrôle intégral. Il comprend cinq domaines : **`math/`**, économie du commerce bilatéral (maximisation conjointe des profits, négociation de Nash, ratio critique bilatéral du modèle newsvendor) ; **`physics/`**, « physique des champs de confiance » appliquée à l’économie structurelle (confiance comme tenseur métrique, géodésique transactionnelle, structure de contact d’annonce et SDE de décroissance de confiance CIR avec sauts — une analogie de modélisation explicitement signalée) ; **`integration/`**, modèle d’intégration verticale composable récursivement pour le capitalisme et le commerce de détail (compartiments, baies d’ingénierie, compartiments d’essai, plateaux de médiane newtonienne et score de sophistication du système) ; **`grading/`**, évaluateur de circuits de sophistication attribuant des notes A–D à chaque module et à tout ensemble de modules gagnants, perdants, évaluateurs ou absolus ; et **`output/`**, interface d’émission routable vers le terminal, une surface au format **CommonRails** ou un socket/connecteur (prévue pour un `slnet_write` natif). Chaque classe porte l’enregistrement Metadata-Class ; cinq démonstrations exécutables valident les domaines. Les documents complémentaires [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md), [`MATH.md`](lib/advantage/MATH.md) et [`PHYSICS.md`](lib/advantage/PHYSICS.md) les décrivent. Un consommateur natif C/C++ dans [`advantage/`](advantage/README.md) — noyau C `advantage_value`/`advantage_mean`, magasin de connaissances XML et bibliothèque sécurisée d’acquisition HTTPS/SFTP/SSH, magasin de clés et transport DH/ECDH — relie le modèle à des [exemples de commerce](advantage/examples/README.md) : transactions positives issues de sources Internet supposées, séquence de pertinence pour Traders, Consumers, Helpers et Healers, et huit modèles classés de A à D (D = Cement maximal ; A = Brand maximal).
- **Annotations de temporisation des boucles — `@loopDelay` / `@loopBudget` et fonction intégrée `sleep(ms)`.** Il s’agit des premières annotations **comportementales au niveau de l’instruction** de SLeeLa : écrites sur leur propre ligne avant une boucle `for`, `while` ou `do`, elles insèrent une véritable pause dans son exécution. `@loopDelay(ms = 250)` / `@loopDelay(seconds = 2)` attend une durée fixe par itération (la forme abrégée `@loopDelay(250)` utilise les millisecondes par défaut) ; `@loopBudget(seconds = 10, iterations = 1000)` répartit un nombre connu d’itérations sur un budget total de temps réel. Les deux annotations sont abaissées vers le nouvel opcode append-only **`OP_SLEEP`** (ISA 157), qui prend également en charge la fonction intégrée générale **`sleep(<ms>)`**, appelable partout. La pause est implémentée de façon portable par `nanosleep` / `Sleep`. Spécification : [`SLEELA.syntax` §32](SLEELA.syntax) ; référence API : [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md) ; démonstration exécutable : [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela). Un index des ajouts de session — comprenant également la multiplication native de grands entiers `OP_BIGMUL` et les outils autonomes [local-generator](local-generator/README.md) et [pi-generator](pi-generator/README.md) — se trouve dans [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md). Les trois surfaces API générées ([`sleeladoc-api/`](sleeladoc-api/README.md), [`docs/api/`](docs/api/README.md) et l’instantané immuable [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html)) ont été reconstruites avec `make api-docs` et incluent désormais les entrées `SLISASleep` / `SLISABigMul` sur la page du paquet `vm`.
- **SleelaDoc depuis la ligne de commande — `sleela docs`.** SleelaDoc est désormais accessible directement depuis le terminal. `sleela docs` répertorie tous les sujets documentables ; `sleela docs crypto` affiche un paquet ; `sleela docs stringbuilder` résout une classe sans distinction de casse et reconnaît le préfixe standard `SL` comme `SLStringBuilder`. La commande affiche une page détaillée — **définition, objectif de conception, relations, structure, user clays et échelle de maîtrise à cinq niveaux** — avec des suggestions classées de type « vouliez-vous dire ? » en cas de nom proche. Les auteurs enrichissent chaque page en ajoutant des balises SleelaDoc `@` telles que `@intent`, `@thesis`, `@axiom`, `@param`, `@see` et `@mastery`. `sleela docs --generate [dir]` génère un corpus en texte brut (une page par classe et un index `TOPICS.index`) que la CLI peut ensuite lire hors ligne ; **`make sleeladoc`** regroupe ce corpus et les sources `.sleela` correspondantes dans `sleeladoc/build/sleeladoc.jar`. Pour les sujets ne disposant que d’une façade, comme `psychiatry` et `politico`, `sleela docs --facade <topic>` explore le système de fichiers à l’exécution et énumère les dossiers réels, puis les fichiers, avec des limites configurables par l’utilisateur de **6 Mo, 6 000 fichiers et 6 minutes**. Les unités de `lib/sleeladoc` utilisent l’édition actuelle **`#sleela 3.0`**. Voir [`man/man1/sleela.1`](man/man1/sleela.1) et [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md).
- **SleelaDoc — documentation de style Javadoc et site HTML généré.** **SleelaDoc** est la convention de commentaires documentaires intégrés au code source de SLeeLa (commentaires entre chevrons délimités par `@doc` / `@page` ; voir [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)). [`lib/sleeladoc`](lib/sleeladoc/README.md) est l’**extracteur** qui transforme une unité `.sleela` en page API Markdown ou HTML, à la manière de `javadoc` pour Java, en documentant l’*intention*. Un site HTML complet et navigable pour l’ensemble de `/lib` est généré dans [`/sleeladoc-api`](sleeladoc-api/README.md) par `tools/generate-sleeladoc-api.py` (ouvrir `sleeladoc-api/index.html`) ; il utilise un style bleu, bleu clair, gris clair et blanc, une hiérarchie paquet → classe → membre et un glossaire destiné au lecteur *Student 2*. Le compilateur produit également, en complément de la compilation, des documents marqués par l’auteur (`--wiki` : page MediaWiki et copie jumelle de la source d’origine), décrits dans [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md). Démonstration exécutable : [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela).
- **Modèle objet de document XML (`lib/xml`).** DOM XML côté SLeeLa : un document possède un arbre de nœuds élément/texte/commentaire/CDATA/instruction de traitement, avec enfants ordonnés et attributs nommés ; il comprend un sérialiseur compact ou lisible (échappement XML 1.0 §2.4), un analyseur par cellules de caractères et le registre qualité `SLXmlAspect`. L’égalité des nœuds suit trois niveaux de confiance (identité → même sujet → structure) et est réflexive, symétrique et déterministe. Voir [`lib/xml/README.md`](lib/xml/README.md) et l’exemple exécutable [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela).
- **Primitives d’entiers signés de grande largeur (`long`, `int64`, `int128`).** `int`, `long` et `int64` sont tous des entiers signés de 64 bits qui sont vérifiés par le système de types et exécutés de manière identique. `int128` est accepté comme type entier (adossé à 64 bits pour le moment ; un véritable support 128 bits est documenté comme travail ultérieur). Les séries signées en base 2 `int32` à `int16777216` (syntaxe 2.7) et non signées `uint32` à `uint16777216` (syntaxe 2.8) étendent ces capacités. Voir [`SLEELA.syntax`](SLEELA.syntax) §21, §24 et §25.
- **Champs conséquents `@mix` (syntaxe 2.9).** Une classe peut déclarer plusieurs champs d’instance portant le **même nom** lorsque chaque répétition comporte `@mix`. Le langage traite le groupe comme une séquence (tableau / liste indexée), de haut en bas `1,2,3,…` par défaut ou définie manuellement avec `@mix(1,2,3,...)` ; le nombre d’indices doit correspondre au nombre de champs. Voir [`SLEELA.syntax`](SLEELA.syntax) §28.
- **Marqueurs documentaires — le Processus naturel, appelé Preonems (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).** Ces mots nus servant de marqueurs documentaires sont collectivement appelés **Preonems** (un marqueur est un *Preonem* ; l’ensemble, les *Preonems* ; voir [`SLEELA.syntax`](SLEELA.syntax) §26.1). Ce sont des marqueurs documentaires déclaratifs de première classe qui expriment ce qu’un document *est* dans l’ordre de sa création : `author`/`authorial` (syntaxe 2.4 ; géré par le conseil professionnel, seul membre ayant un effet comportemental), `material;` (Source établie) et `memory;` (syntaxe 2.5), `determine;` (syntaxe 2.6 ; document ouvert à un traitement ultérieur par un médecin, praticien ou ghost jusqu’à son achèvement par voie légale), puis les deux plus récents : **`object;`** (chose établie dans le catalogue de ce qui existe, réifiée et dotée d’une existence concrète en syntaxe 2.8) et **`field;`** (champ relationnel dans lequel les objets existent et se déplacent), tous deux disponibles à partir de la syntaxe 2.6. Chaque marqueur est contextuel et conditionné par la version dans le frontend ; il ne modifie ni l’analyse syntaxique, ni le typage, ni la génération de code. En dehors de la position de marqueur, chaque mot reste un identifiant ordinaire : `field` n’interfère donc pas avec un membre de classe ou un accès `.field`. Voir [`SLEELA.syntax`](SLEELA.syntax) §18–§20, §23 et §26.
- **Nordshrift en syntaxe 3.0 avec accès complet à la bibliothèque.** Nordshrift résout les noms via le même index récursif `/lib` que le compilateur et atteint ainsi tous les paquets et unités `.sleela` ; `nordshrift check` indique les nombres pour l’ensemble de l’arborescence.
- **Stanchion de récursion.** [`recursion/stanchion/`](recursion/stanchion/README.md) comprend désormais des prototypes de référence C et C++ dédiés aux sept contrats primitifs documentés, ainsi que des exemples génériques, appliqués, scientifiques et pédagogiques. Il s’agit de sources de référence : compilez-les et testez-les avant de les considérer comme vérifiées.
- **Commentaires entre chevrons (`<` / `<<`).** Nouveau style de commentaire pour les développements futurs : `< line >` et `<< block >>`, reconnu par le frontend SLeeLa et documenté dans [`SLEELA.syntax`](SLEELA.syntax) §1.1.
- **Littéraux entiers avec base.** Les littéraux `int` acceptent désormais les formes hexadécimale (`0x`), binaire (`0b`) et octale (`0o`), avec séparateurs de chiffres `_`, dans le langage source SLeeLa ([`SLEELA.syntax`](SLEELA.syntax) §1.4) et dans le format de modèles CPU [`.xlt`](lib/xlt/README.md).
- **Norme Metadata Class dans toute la bibliothèque.** Le métapatron de métadonnées descriptives (provenance, légalité, portée juridictionnelle) est désormais présent dans chaque fichier `.sleela` contenant des classes sous `/lib`. Voir [`SLEELA.syntax`](SLEELA.syntax) §15 et [la leçon du tutoriel](tutorial/13-metadata-class.md).
- **Famille de texte / String enrichie.** `lib/text` ajoute de véritables assistants dans le langage, une vue par cellules de caractères proposant les opérations réelles `charAt`, `substring`, `indexOf`, `reverse` et l’alignement de colonnes, ainsi qu’un moteur de modèles positionnels. Voir le [tutoriel texte et String](tutorial/14-text-and-strings.md) et la démonstration exécutable [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela).
- **`String` de première classe dans les feuilles Nordshrift.** Une feuille `.sst` peut déclarer un bloc typé `strings:` qui contribue au manifeste des composants de la compilation. Voir la [référence Nordshrift Sheet](wiki/Nordshrift-Sheet-Reference.md).
- **Modèles CPU exécutés par opcodes et équivalence d’appels de fonction.** Chaque `SL<ARCH>CPU` hérite de la capacité d’exécuter un flux d’instructions natives via sa boucle fetch-decode-execute, ainsi que le code d’un autre processus supposé par des appels équivalents sur l’infrastructure VM partagée. Voir [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5.

## III. Construction et tests

**Prérequis :** une chaîne d’outils C/C++ (GCC ou Clang) et GNU Make ; Python 3 pour les outils de manifeste et de flux ; sous Linux, les paquets de développement X11/Xrandr pour les produits d’affichage. La commande `make deps` d’un module indique ou installe les dépendances nécessaires. La construction de `impl/` échoue de manière fermée si le manifeste SHA-256 de confiance n’est pas respecté ; le `Makefile` racine le référence automatiquement.

Pour démarrer rapidement depuis un clone propre :

```sh
make                                  # construire tous les éléments depuis la racine du dépôt
```

Ou construire et tester uniquement le cœur natif :

```sh
make -C impl all                      # construire uniquement le compilateur et le cœur d’exécution
bash ./test-suites/run-all.sh --all   # exécuter toute la suite de tests
```

La commande `make` à la racine du dépôt construit tous les produits (cœur, VM, codecs, churn, antivirus, etc.), puis termine par la construction du **registre** et la matérialisation de sa table de référence. Un arbre construit contient ainsi un registre vérifiable dès le départ.

Cibles essentielles :

```sh
make                     # tout construire (alias de make all ; registre exécuté en dernier)
make all                 # même construction complète, nommée explicitement
sudo make install        # installer les commandes et les fichiers d’exécution (voir § IV)
make clean               # supprimer les sorties de construction de tous les systèmes invoqués
make help                # afficher toutes les cibles du Makefile racine
```

Autres cibles utiles :

```sh
make registry            # construire uniquement le registre et matérialiser la table
./build-registry.sh      # même opération, exécutée directement
make tests               # construire et exécuter la suite de tests
make ballum              # construire la bibliothèque d’affichage Ballum pour ce système
bin/install-modules      # installation/construction cadencée des modules sur environ 10 à 20 minutes
```

La cible `registry` exécute [`build-registry.sh`](build-registry.sh) : elle construit le cœur du registre, initialise la table de référence dans le stockage texte portable `registry/build/sleela.sleelareg` et exporte le script de table MySQL `registry/build/registry.sql` (avec `sleela registry export`). Le registre peut être examiné avec `sleela registry` ou `registry/build/sleela-registry review`. Voir le [guide du registre](registry/README.md) et l’[architecture Registry 3.0](registry/REGISTRY-3.0-ARCHITECTURE.md).

Consultez les guides de construction et de test pour les prérequis et les étapes propres à chaque plateforme. La réussite de la construction d’un composant ne prouve pas que chaque bibliothèque ou plateforme est prête pour une publication.

## IV. Installation

Après la construction des produits natifs, installez les commandes et les fichiers de prise en charge de l’environnement d’exécution :

```sh
sudo make install
make install PREFIX="$HOME/.local"        # installation pour l’utilisateur, sans sudo
make install DESTDIR="$PWD/package-root" # installation dans un répertoire de préparation de paquet
make install-check
```

L’installateur conventionnel utilise par défaut `/usr/local/bin`, `/usr/local/lib/sleela` et `/usr/local/share/sleela`. Il préserve une configuration de démarrage existante et refuse d’écraser des commandes existantes non suivies par le dépôt. La désinstallation ne supprime que les fichiers enregistrés par l’installateur ; elle préserve la configuration et les données d’exécution non suivies.

Pour un déploiement ciblé et guidé du seul paquet principal — les binaires `sleela`, `sleelvac` et `nordshrift`, la bibliothèque standard, les protocoles et les modules de prise en charge — utilisez l’installateur propre :

```sh
make clean-install       # installation guidée du seul paquet principal
```

Le processus demande d’abord si le paquet principal doit être installé, puis permet de choisir le périmètre. En cas de réussite, un sélecteur facultatif propose des modules d’amélioration sélectionnés. Le déroulement interactif complet et toutes les options sont décrits dans [Installation rapide et sûre](install/README.md).

## V. Langage et environnement d’exécution

La grammaire faisant autorité est [`SLEELA.syntax`](SLEELA.syntax). Le dépôt documente actuellement la syntaxe **3.0** ; la version du compilateur/des outils et la version de syntaxe du code source sont distinctes. La surface 3.0 ajoute les marqueurs documentaires **`pure` et `gasoline`** (§30) : deux Preonems contextuels, déclaratifs et sans effet d’exécution propre, qui expriment chacun la position d’un document par sa propre honnêteté. La syntaxe 3.0 rassemble les surfaces linguistiques 1.0 à 2.9 dans l’édition normative actuelle, tout en conservant les versions minimales requises pour chaque fonctionnalité.

La surface précédente, 2.9, ajoute les **champs conséquents `@mix`** (§28) : une classe peut déclarer plusieurs champs d’instance portant le même nom lorsque chacun est marqué `@mix`. Le langage les traite comme un groupe indexé (de haut en bas `1,2,3,…` par défaut, ou défini manuellement avec `@mix(1,2,3,...)`, dont le nombre d’indices doit correspondre au nombre de champs). Elle reprend également les familles d’entiers larges signés et non signés en base 2, de `int32` à `int16777216` (§24) et de `uint32` à `uint16777216` (§25), qui prolongent `long`, `int64` et `int128` (§21) ; les documents de prise en charge des mots-clés (§22) ; ainsi que les **Preonems** : marqueurs documentaires déclaratifs `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) et `artifact` (§35 ; statut de qualité législative évalué à l’échelle du temps et de l’espace). L’ensemble porte le nom de Preonems (§26.1).

La résolution de surcharge des constructeurs ainsi que la délégation vers `this(...)` et les constructeurs de classe de base sont explicitement hors du contrat de syntaxe 1.8 vérifié. Consultez la grammaire et les notes d’implémentation avant de vous appuyer sur une fonctionnalité. Les neuf unités sources SleelaDoc déclarent désormais `#sleela 3.0` ; le contrôle commun du compilateur accepte les syntaxes source 1.0 à 3.0. La nouvelle construction de l’API est produite par `make api-docs`.

Le dépôt comprend une implémentation native C/C++, des références de génération SLVM, des modules de bibliothèque standard, des tests, des exemples et des intégrations de plateforme. L’état d’implémentation et de validation varie selon les modules ; consultez leurs spécifications et les résultats CI actuels plutôt que de supposer une complétude uniforme.

## VI. Principes d’ingénierie

- **Les preuves avant les affirmations :** distinguer les comportements implémentés, testés, expérimentaux et prévus.
- **Un contrat par frontière :** documenter la syntaxe, les API, les formats de fichiers, les hypothèses de plateforme et les comportements en cas d’échec.
- **Conception portable :** isoler les backends propres aux plateformes et tester sur des exécuteurs natifs.
- **Échec sûr :** tester les entrées mal formées, les limites de ressources, les défaillances de processus et les chemins de nettoyage.
- **Publications reproductibles :** reconstruire proprement et publier les empreintes des artefacts ainsi que les notes de version.

## VII. Construction de l’antivirus et intégrité

Le paquet antivirus Sleelavirin comprend un scanner SHA-256 natif, un adaptateur ClamAV facultatif, des manifestes d’intégrité pour les sources et les index, ainsi que des sommes de contrôle pour les paquets de plateforme. Construisez et testez le paquet depuis la racine du dépôt :

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Le manifeste d’intégrité est généré sous `antivirus/build/`. Il couvre les sources et les documents suivis par le dépôt ainsi que les fichiers d’index de signatures protégés ; les sorties générées et le cache ClamAV modifiable en sont exclus. Les instantanés de bases de données ClamAV disposent de leurs propres manifestes. Les archives ZIP de publication comprennent `INTEGRITY-MANIFEST.json`, `SHA256SUMS` et un fichier compagnon `.sha256`. Vérifiez les empreintes des archives téléchargées à l’aide d’une valeur de confiance obtenue indépendamment : une somme de contrôle stockée à côté d’une archive ne constitue pas une signature de l’éditeur. Voir le [guide Sleelavirin](antivirus/README.md), la [liste de contrôle de sécurité](antivirus/SECURITY.md) et le [workflow de construction antivirus](.github/workflows/antivirus-binaries.yml).

Une publication n’est considérée comme vérifiée que lorsque les tâches CI pertinentes réussissent sur le commit exact de cette publication et que les artefacts obtenus ont été contrôlés.

## VIII. Thèmes de l’éditeur SLeeLa — Vim et Emacs

SLeeLa comprend un ensemble coordonné de thèmes sombres professionnels pour **Vim** et **Emacs**, ainsi qu’un utilitaire natif de palettes C/C++. Le thème d’origine **American Professional** reste le thème par défaut. Dix palettes complémentaires proposent des inspirations de conception culturelles et institutionnelles variées ; il s’agit d’interprétations graphiques et non de représentations officielles d’une nation, d’une culture, d’une cour ou d’une institution.

### Les onze thèmes et leurs couleurs

| Thème | Arrière-plan | Premier plan | Mots-clés | Types | Chaînes | Commentaires |
|---|---|---|---|---|---|---|
| **American Professional** | `#101820` | `#F4F7FA` | `#7FB3FF` | `#73DACA` | `#F2C078` | `#8A96A3` |
| **Pan-Eurasian** | `#111827` | `#F0F2F5` | `#7AA7E8` | `#69C9B3` | `#D7B56D` | `#8995A5` |
| **Russian and Slavic — Medium** | `#202733` | `#E8EAF0` | `#A9C0DD` | `#8FC4B2` | `#D6BD8B` | `#9AA3AF` |
| **Central Asian / Silk Road** | `#1C2529` | `#F5F0E5` | `#7FADE0` | `#45C6C2` | `#D9B56D` | `#92A3A0` |
| **East Asian** | `#171A1C` | `#F2EFE7` | `#D97878` | `#72C7A5` | `#D8B76A` | `#969F9B` |
| **ICC — Cour pénale internationale** | `#0E1A2B` | `#EAF0F7` | `#5B8FD6` | `#7FB7C9` | `#D9B25A` | `#7E8CA0` |
| **SCOTUS — Cour suprême des États-Unis** | `#1A1416` | `#F3EEE6` | `#9E2B35` | `#8FA9B8` | `#C9A24B` | `#8C8178` |
| **Greek (Hellenic)** | `#0A1A30` | `#FFFFFF` | `#2E7BD6` | `#5E9FE0` | `#9FC3EC` | `#6E86A6` |
| **Turkish (Al Bayrak)** | `#2A0609` | `#FFFFFF` | `#F23D48` | `#F0A9AE` | `#F0C9A9` | `#A8767A` |
| **Kiro** | `#0B0B12` | `#F5F5FA` | `#A98EDA` | `#6E8BE0` | `#E8C547` | `#6A6482` |
| **SLeeLa** | `#08080E` | `#FFFFFF` | `#B07CFF` | `#5A7CFF` | `#FFD21E` | `#6E5F9E` |

### Couleurs des drapeaux nationaux (palette source)

Les thèmes grec et turc sont dérivés des **couleurs officielles des drapeaux nationaux** des deux pays, indiquées ici avec leurs valeurs hexadécimales publiées. Les arrière-plans et accents des thèmes ci-dessus sont des interprétations assombries, adaptées aux éditeurs ; le tableau ci-dessous conserve les couleurs de référence telles qu’elles sont exposées au niveau national.

| Pays | Couleur | Hexadécimal | Fondement et signification |
|---|---|---|---|
| **Grèce** | Bleu | `#0D5EAF` | Bleu du drapeau national (loi 851/1978). Le ciel et les mers ; en héraldique, l’azur. |
| **Grèce** | Blanc | `#FFFFFF` | Blanc du drapeau national. Pureté de la lutte pour l’indépendance ; la croix représente le christianisme orthodoxe oriental. |
| **Türkiye** | Rouge (*al bayrak*) | `#E30A17` | Rouge du drapeau national (loi turque sur le drapeau). Le champ ; selon la tradition, le sang des martyrs et la souveraineté ottomane. |
| **Türkiye** | Blanc | `#FFFFFF` | Blanc du drapeau national. Le croissant et l’étoile : paix, pureté et symbole de l’héritage turcique/islamique. |

Les thèmes ancrent le premier plan de chaque éditeur sur le **blanc** national et leurs mots-clés, curseur, sélection et barre d’état sur le **bleu** national exact (`#0D5EAF`) ou le **rouge** (`#E30A17`). Les couleurs importantes sont donc littéralement les couleurs nationales. Il s’agit de palettes interprétatives ; la référence aux couleurs des drapeaux documente leur provenance et ne constitue pas une représentation officielle de l’un ou l’autre État. Les significations des couleurs suivent la description des symboles d’État du ministère grec des Affaires étrangères et la tradition de la loi turque sur le drapeau.

### Tableau des échantillons de couleur

Les cellules de palette ci-dessous utilisent des **échantillons SVG**, afin que GitHub affiche les couleurs réelles plutôt que des cellules HTML vides. Chaque bande présente six couleurs dans cet ordre : arrière-plan, premier plan, mots-clés, types, chaînes et commentaires. Les codes hexadécimaux exacts figurent à côté de chaque échantillon, dans le même ordre que le tableau de référence.

| Thème | Palette affichée | Codes hexadécimaux (arrière-plan · premier plan · mots-clés · types · chaînes · commentaires) |
|---|---|---|
| **American Professional** | ![Palette American Professional](lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Palette Pan-Eurasian](lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russian and Slavic — Medium** | ![Palette Russian and Slavic Medium](lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Central Asian / Silk Road** | ![Palette Central Asian Silk Road](lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **East Asian** | ![Palette East Asian](lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC — Cour pénale internationale** | ![Palette ICC](lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS — Cour suprême des États-Unis** | ![Palette SCOTUS](lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Greek (Hellenic)** | ![Palette grecque hellénique](lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turkish (Al Bayrak)** | ![Palette turque Al Bayrak](lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Palette Kiro](lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![Palette SLeeLa](lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

Des couleurs d’accent supplémentaires sont utilisées pour les fonctions, constantes, sélections, diagnostics et directives du préprocesseur. La palette Russian and Slavic utilise volontairement des tons moyens d’ardoise, de bleu, de sauge, de rubis et d’or ancien pour une apparence professionnelle plus apaisée.

### Installer les thèmes Vim

Depuis la racine du dépôt :

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Ajoutez ce qui suit à `~/.vimrc` pour activer la coloration syntaxique SLeeLa et choisir un thème :

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Choisissez exactement l’un des noms de thèmes suivants :

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Par exemple, utilisez `colorscheme sleela-russian-slavic` pour la palette russe et slave d’intensité moyenne, ou `colorscheme sleela-greek` / `colorscheme sleela-turkish` pour les palettes hellénique et Al Bayrak. Redémarrez Vim ou rechargez la configuration avec `:source $MYVIMRC`.

### Installer les thèmes Emacs

Créez un répertoire local pour les thèmes et copiez-y les fichiers de mode et de thèmes depuis la racine du dépôt :

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Ajoutez ceci à `~/.emacs` ou `~/.emacs.d/init.el` :

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Remplacez `sleela-american` par `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` ou `sleela-sleela` pour sélectionner un autre thème. Si Emacs demande s’il doit faire confiance au thème, examinez le fichier local du thème avant de l’accepter.

### Prévisualiser les palettes natives C/C++

Le petit utilitaire natif de palettes peut répertorier les thèmes, afficher leurs couleurs de terminal et rechercher une couleur individuelle :

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Pour assurer la compatibilité ascendante, `--palette` et `--hex COLOR` sans `--theme` sélectionnent American Professional. Les résultats de compilation et de test doivent être vérifiés sur le système cible et dans la CI ; la documentation d’une commande ne prouve pas qu’elle a effectivement réussi.

Voir le [guide d’intégration des éditeurs](linux-editors/README.md) pour les descriptions complètes des palettes et les détails de l’utilitaire.

## IX. Contribution

1. Consultez l’index de documentation et la politique de test.
2. Effectuez des modifications ciblées, accompagnées de tests positifs et négatifs.
3. Exécutez les tests ciblés, puis la suite étendue.
4. Indiquez les détails du système d’exploitation et de la chaîne d’outils ; ne présentez pas les vérifications ignorées comme réussies.

SLeeLa est développé par **MEARVK LLC**. Les anciens contenus de la page d’accueil sont conservés dans [`docs/README-LEGACY.md`](docs/README-LEGACY.md).

## X. AE6E66 — Attente de prélèvement sur les courriels publics Round

Pour l’initiative AE6E66, l’attente déclarée du projet est de **300 000 000,00 USD par courriel public Round**, en tant que montant de prélèvement fiscal proposé associé à chaque courriel Round Public répondant aux critères.

Cette déclaration consigne uniquement une attente du projet. Elle ne constitue **ni** une imposition fiscale légale, **ni** un ordre gouvernemental, **ni** un jugement, **ni** une obligation fiscale établie de manière indépendante. Tout prélèvement effectif nécessiterait l’autorité juridique applicable, une juridiction, des définitions, une procédure et une décision de l’autorité gouvernementale compétente.

## XI. Environnement d’exécution des alphabets et des jeux de caractères

La documentation de l’alphabet SLeeLa et l’environnement natif des caractères sont maintenus dans `alphabet/sleela/` et `lib/alphabet/native/`. Les tests natifs C++17 couvrent le décodage et l’encodage stricts des scalaires UTF-8, les signatures de capacité `C<n>` et l’appartenance bornée aux jeux de caractères :

```sh
make -C lib/alphabet/native test
```

Le catalogue expérimental UTF-4088 et son adaptateur de recherche SLeeLa sont documentés dans [`utf-4088/SLeeLa-ALPHABET-INTEGRATION.md`](utf-4088/SLeeLa-ALPHABET-INTEGRATION.md). Les identifiants UTF-4088 appartiennent à un espace de noms distinct et ne sont pas des valeurs scalaires Unicode. Le travail de liaison au compilateur et à SLVM est suivi séparément de ces fondations natives.

## XII. AutoCAD Creator™

[**AutoCAD Creator™**](autocad/creator/README.md) est une application CAO 2D interactive entièrement écrite en SLeeLa. Alors que l’AutoCAD Renderer associé transforme un plan en dessin en une seule opération, Creator permet de **créer et modifier** en direct un document AutoCAD 2D : ajouter des murs, des pièces, des cotes et des notes, puis déplacer, renommer, changer de calque, supprimer et annuler les éléments placés, et **enregistrer** le résultat comme véritable dessin DXF AutoCAD (texte R12, ouvrable dans AutoCAD / LibreCAD).

Son interface graphique repose sur **SleelaUI™**, la propre boîte à outils d’interface multiplateforme de SLeeLa (dans [`lib/user-interface`](lib/user-interface)), et non sur JavaFX ; elle ouvre une véritable fenêtre native sous X11 / Cocoa / Win32. Sa conception suit une règle : **« SleelaUI présente ; SLeeLa décide »**. L’interface graphique ne calcule pas la géométrie et n’écrit pas le DXF. Chaque outil émet une intention de création, modification, suppression, annulation ou enregistrement au composant faisant autorité [`SLCadCreator`](lib/autocad/SLCadCreator.sleela), propriétaire du document modifiable et d’un historique réversible des commandes ; le canevas ne fait que prévisualiser la géométrie détenue par Creator.

Le moteur d’édition réutilisable se trouve dans [`lib/autocad`](lib/autocad) : [`SLCadEntity`](lib/autocad/SLCadEntity.sleela), [`SLCadDocument`](lib/autocad/SLCadDocument.sleela), [`SLCadCommand`](lib/autocad/SLCadCommand.sleela) et [`SLCadCreator`](lib/autocad/SLCadCreator.sleela). Il fonctionne de bout en bout sans interface graphique. L’enveloppe graphique et la démonstration se trouvent dans [`autocad/creator`](autocad/creator) :

```sh
make creator          # depuis autocad/ : exécuter la démo Creator -> out/altered.dxf
```

Sur un système avec affichage, la fenêtre d’édition en direct s’ouvre ; sur un hôte sans interface graphique, le dessin est construit en mémoire, un rapport et le DXF sont affichés, puis le programme se termine proprement.

## XIII. Ordonnancement de la carte d’entrée UTF-4088

Le catalogue spécialisé UTF-4088 définit son parcours spatial de la carte d’entrée et ses itérations abstraites de pression dans [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). La politique d’index des matières du Registre national des privilèges est décrite dans [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Les coordonnées de carte, les niveaux de pression (1–50), les identifiants permanents du catalogue et les rangs alphabétiques sont des champs indépendants ; aucun ne doit être substitué silencieusement à un autre. La pression est un niveau d’itération algorithmique, et non une grandeur physique.

## XIV. Index des paquets récents et enrichis

Cet index ajouté met en évidence les paquets récemment enrichis, les bibliothèques métier et les collections de code source. Les entrées sont regroupées par ordre alphabétique selon le nom du paquet ou le sujet. Les descriptions indiquent l’emplacement dans le dépôt ; l’implémentation et l’état de validation doivent être vérifiés dans la documentation et les tests propres à chaque paquet.

### A–C

- **Advantage** — `lib/advantage/` et `advantage/` : optimisation d’un dividende unique par rapport à un modèle au moyen d’un Sequitur onion à couches, avec cinq sous-dossiers thématiques : `math/` (maximisation conjointe des profits, négociation de Nash, modèle bilatéral newsvendor), `physics/` (métrique, géodésique, contact et SDE du champ de confiance ; analogie de modélisation explicitement signalée), `integration/` (compartiments d’intégration verticale et score de sophistication), `grading/` (évaluateur de circuits de sophistication A–D pour modules et ensembles) et `output/` (émission vers terminal, CommonRails ou socket). Un consommateur natif C/C++ (`advantage/`) relie le modèle à des exemples de commerce. Voir [`lib/advantage/README.md`](lib/advantage/README.md) et [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alphabet et UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` et `utf-4088/` : catalogues d’alphabets, prise en charge native des jeux de caractères, ordre des cartes d’entrée et documentation d’intégration UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` et `lib/autocad/` : flux de dessin 2D interactifs et composants réutilisables pour les documents et l’édition CAO.
- **Ballum** — contrôle de l’affichage au niveau du pixel : définition/lecture de pixels, copie de régions, luminosité/température/gamma de l’écran entier, entrée pointeur/clavier et **surface framebuffer plein écran** pour un contrôle total, détenu et rapide de chaque pixel (fenêtre override-redirect et présentation par un seul `XPutImage`/MIT-SHM sous Linux/X11). Le noyau natif se trouve dans `ballum/`, la façade SLeeLa dans `lib/ballum/` et le pont VM dans `impl/` ; voir [`ballum/README.md`](ballum/README.md) et l’[exemple exécutable](ballum/example/).
- **CDMA** — `cdma/` et `lib/cdma/` : documentation technique liée au CDMA et développement de bibliothèque source SLeeLa.
- **Character** — `lib/character/` : profils de personnages et statistiques réutilisables, y compris des exemples de profils biographiques.
- **Coorenagraph** — `lib/coorenagraph/` : raisonnement éthique, vocabulaire moral, délibération et concepts associés ; voir le README du paquet et ses fichiers source.

### D–G

- **Modèles de gouvernance en code source** — [`lib/governance`](lib/governance/README.md) définit des structures déclaratives SLeeLa pour les propositions, les bulletins manuels anonymes et les décomptes agrégés. L’annotation proposée `@vote` et le processus manuel sont documentés dans [`registry/DEMOCRATIC-VOTING-SYNTAX.md`](registry/DEMOCRATIC-VOTING-SYNTAX.md) ; les compilateurs actuels la traitent comme des métadonnées de commentaire inertes.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` jusqu’aux répertoires de paquets HTTP versionnés disponibles : spécifications et implémentations propres à chaque version du protocole. Consultez la documentation de chaque version pour connaître son périmètre précis et son état de vérification.
- **Cour pénale internationale** — `lib/international-criminal-court/` et `INTERNATIONAL.CRIMINAL.COURTS.md` : code source et documentation propres à ce domaine.

### O–S

- **OFDMA** — `ofdma/` : documentation technique sur l’accès multiple par répartition orthogonale de la fréquence et ressources de développement C/C++.
- **Philosophie et module Conversation Mask** — [`philosophy/`](philosophy/README.md) contient les fondations natives C/C++ et [`philosophy/module/`](philosophy/module/README.md) organise la couche suivante du module en trois domaines explicites : [`data/`](philosophy/module/data/README.md), pour les observations typées et datées, les fins d’événements, les paramètres de demi-vie, la provenance et le consentement ; [`helper/`](philosophy/module/helper/README.md), pour la validation déterministe, la décroissance de pertinence, les comparaisons compatibles et la présentation filtrée par consentement ; et [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md), pour une interface conversationnelle de soutien non clinique. Les points d’accès SLeeLa et le Tree composé se trouvent dans [`lib/philosophy/`](lib/philosophy/README.md), aux côtés du paquet de philosophie plus large. Le module repose sur des entrées explicites et le consentement : il ne surveille pas passivement les personnes ou les médias, ne diagnostique pas, ne déduit pas d’intention privée, n’évalue pas la valeur humaine et ne recommande pas de modification de médicaments. Les liens d’événements préservent la provenance et la navigation, mais ne constituent pas une corroboration indépendante.
- **Synchro** — intégration native du runtime et du compilateur, ainsi que fonctions de dispatch de paquets et de mesure ; voir les sources correspondantes dans `impl/` et l’implémentation du compilateur.
- **SleelaUI / interface terminal** — `lib/user-interface/` et le projet `sleela-terminal` : intégration de l’interface SLeeLa et composants d’interaction orientés terminal.

### U–W

- **Nations Unies** — `united-nations/` : ressources du dépôt propres à ce domaine.
- **Wi-Fi 4, Wi-Fi 5 et Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`, `wi-fi-6/` et les répertoires de sources associés `lib/wifi/` : documentation réseau sans fil et ressources de configuration propres à chaque génération.

### Maintenance de l’index des paquets

Lorsqu’un paquet est ajouté ou considérablement enrichi, ajoutez son entrée ici dans le groupe alphabétique approprié, liez son répertoire et son README canoniques, et ne décrivez que les capacités étayées par les sources présentes dans le dépôt. Conservez le caractère additif de cet index : ne supprimez pas et ne réécrivez pas les sections existantes du README lors des mises à jour de l’index.

---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

`bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh` :: `⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓`

`bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc` :: `⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉`

Démocratie américaine et politique des États-Unis.

---

## XVI. Imagerie de marque de la science nationale

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="Science nationale SLeeLa" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="NSF nationale SLeeLa" width="560"></p>

## XVII. Catalogue expérimental UTF-4088 d’un million d’entrées

Le [générateur de catalogue UTF-4088](utf-4088/tools/generate_catalogue.py) construit de manière déterministe un catalogue expérimental de symboles de 1 048 576 entrées. Le format compact documenté est décrit dans [`utf-4088/FORMAT.md`](utf-4088/FORMAT.md) ; le contrat de génération et de validation se trouve dans [`utf-4088/GENERATION-RESULTS.md`](utf-4088/GENERATION-RESULTS.md).

Exécutez le générateur avec Python 3 :

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Le fichier binaire généré mesure 16 777 244 octets et est conçu pour rester inférieur à 20 000 000 octets. Les tests unitaires et un workflow GitHub Actions vérifient le nombre d’entrées, l’unicité des glyphes, la taille et SHA-256 ; les exécutions réussies publient le binaire et son résumé comme artefact du workflow `utf4088-million-catalogue`. Le binaire généré est volontairement exclu de l’historique Git. UTF-4088 reste expérimental et n’est pas compatible avec Unicode.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Ajouts récents : constante Pi, démarrage Modium/Registry, math de sleelactl et `@native`

Cette section consigne les fonctions ajoutées lors des travaux de 2026-11, en commençant par les groupes les plus récents. Les fichiers sources liés et leur état de validation font foi ; les changements en cours de revue ne sont pas décrits comme intégrés tant qu’ils n’ont pas atteint `master`.

### La constante de catalogue Pi à 100 000 000 chiffres

Le dépôt contient une valeur vérifiée de π à **cent millions de chiffres** dans [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (un nombre décimal bien formé, `3.14159…`), ainsi qu’une valeur à 57 344 chiffres dans [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Les deux sont des valeurs authentiques de π vérifiées indépendamment. Voir [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md), qui décrit les contrôles (préfixe de référence, point de Feynman à la position fractionnaire 762, célèbre chiffre à la position un million, concordance avec `lib/math/SLPiReference.sleela`, analyse de fréquence des chiffres sur le fichier entier) et un script de reproduction en une seule exécution.

### Modium™ / Registry™ chargent la constante Pi au démarrage du sous-système

Aux points d’entrée dédiés — `sleela registry` et `sleela modium` (ainsi qu’au premier lancement de l’installateur), et **non** à chaque `sleela run`, les sous-systèmes Modium et Registry chargent la grande constante du catalogue π. Dans un terminal interactif, une ligne rouge transitoire `Loading Modium™` / `Loading Registry™` s’affiche puis est effacée sur place avant le début de la sortie normale. Cet effet est strictement réservé aux TTY ; les sorties redirigées et celles des tests restent identiques octet pour octet. Le localisateur (et non les octets) est initialisé comme valeurs par défaut des clés `Math` du registre `piReferenceLargePath` et `piReferenceLargeDigits`, dans le modèle SLeeLa ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) et dans l’initialiseur natif ([`registry/sleela_registry.c`](registry/sleela_registry.c)) ; `lib/math/SLPiReference.sleela` expose `largePath()` / `largeDigits()`. La constante π est installée à l’emplacement SLeeLa du système d’exploitation (`<data>/pi/`) par l’installateur standard `make` et par l’installateur guidé « beautiful ». Détails : [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — l’installateur « beautiful » et un sujet `math` adossé au natif

- L’installateur guidé (« beautiful ») installe désormais `sleelactl` (le plan de contrôle SLeeLa) et son registre de modules, comme l’installateur `make`. Voir [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- `sleelactl` a reçu une nouvelle nature **`subject`** pour les bibliothèques de sujets natifs C/C++ compilées dans le runtime, et enregistre le premier : **`math`**. `sleelactl run math` exécute de bout en bout le backend natif `__NativeMath` (`impl/subjects/math/math_native.cpp`) au moyen de la démonstration de référence du sujet ; `sleelactl info math` explique comment y accéder (`import math;`). Cette nature de sujet ne propose que `run` et `info` : il n’existe volontairement pas de verbe `check`, car `sleela check` ne charge pas les sujets natifs enregistrés par SHEET.

### `@native`, Native Drive et exécution multilingue

Une nouvelle annotation de méthode **`@native`** demande la compilation native anticipée (*ahead-of-time*) d’une méthode intensive. La nouvelle commande `sleela aot` repère les méthodes `@native`, vérifie que chacune appartient au *sous-ensemble déchargeable* pris en charge et émet du C/C++. Elle **échoue de manière fermée** : toute construction qui ne peut pas encore être traduite est refusée avec une raison précise, afin qu’aucune méthode partiellement traduite ne soit produite.

```sh
sleela aot --list <file.sleela>    # verdict par méthode : ok / REFUSED (motif)
sleela aot --emit <file.sleela>    # émettre du C/C++ (stdout) pour un programme du sous-ensemble
```

`@native` reste une demande de traitement AOT, les constructions non prises en charge étant rejetées avant l’émission. Le [Native Drive](impl/native-drive/README.md) prend en charge la construction d’exécutables et de bibliothèques partagées C/C++ multi-unités via les compilateurs de l’hôte (y compris MSVC), la sortie AOT SLeeLa prise en charge, l’inspection des symboles par rapport à l’ABI du chargeur natif, l’exécution de processus sans shell, la gestion de stdin/stdout/stderr, le chaînage en plusieurs étapes par tubes et la délégation XClass au chargeur de production. `sleelactl` expose ses commandes de construction et d’exécution, et achemine l’inspection XClass ainsi que le rendu et l’installation explicites d’unités systemd par `inspect native-drive`. La VM possède une table bornée de descripteurs de modules natifs, avec autorisation obligatoire de l’hôte et descripteurs vérifiés par génération. La syntaxe 3.0 ajoute désormais `nativeOpen`, `nativeCall` et `nativeClose` : le dispatch de bytecode en ajout seul traverse une ABI à version indépendante (null/bool/int64/double/UTF-8), tandis que les tableaux, structs et autres valeurs composites détenus par la VM restent dans l’interpréteur et peuvent recourir au bytecode de même nom en repli. Les plugins peuvent éventuellement recevoir une table de rappels de l’hôte versionnée pour invoquer des fonctions SLeeLa précises par exécution imbriquée de la VM, avec enracinement GC des piles suspendues, une liste d’autorisation explicite des noms de rappels de l’hôte et le rejet des appels récursifs à l’API native. Le code natif reste du code de confiance exécuté dans le processus, et non un bac à sable. Les annotations de politique de processus du source disposent d’un schéma versionné, mais ne sont pas encore analysées ni appliquées. Détails, frontières de sécurité et registre des sept chantiers : [Native Drive](impl/native-drive/README.md) et [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).

## XIX. Éditions internationales du README

SLeeLa fournit des points d’entrée README propres à chaque pays pour les 50 marchés de son édition internationale. Chaque pays possède un fichier nommé `README.<CC>.md` ; lorsque plusieurs langues sont pertinentes, elles sont regroupées dans le fichier du pays concerné.

- [README.AE](README.AE.md) · [README.AT](README.AT.md) · [README.AU](README.AU.md) · [README.BE](README.BE.md) · [README.BR](README.BR.md)
- [README.CA](README.CA.md) · [README.CH](README.CH.md) · [README.CL](README.CL.md) · [README.CN](README.CN.md) · [README.CO](README.CO.md)
- [README.CZ](README.CZ.md) · [README.DE](README.DE.md) · [README.DK](README.DK.md) · [README.EE](README.EE.md) · [README.ES](README.ES.md)
- [README.FI](README.FI.md) · [README.FR](README.FR.md) · [README.GB](README.GB.md) · [README.GR](README.GR.md) · [README.HK](README.HK.md)
- [README.HU](README.HU.md) · [README.ID](README.ID.md) · [README.IE](README.IE.md) · [README.IL](README.IL.md) · [README.IN](README.IN.md)
- [README.IS](README.IS.md) · [README.IT](README.IT.md) · [README.JP](README.JP.md) · [README.KR](README.KR.md) · [README.LU](README.LU.md)
- [README.MX](README.MX.md) · [README.MY](README.MY.md) · [README.NL](README.NL.md) · [README.NO](README.NO.md) · [README.NZ](README.NZ.md)
- [README.PH](README.PH.md) · [README.PL](README.PL.md) · [README.PT](README.PT.md) · [README.QA](README.QA.md) · [README.RU](README.RU.md)
- [README.SA](README.SA.md) · [README.SE](README.SE.md) · [README.SG](README.SG.md) · [README.SI](README.SI.md) · [README.SK](README.SK.md)
- [README.TH](README.TH.md) · [README.TR](README.TR.md) · [README.TW](README.TW.md) · [README.US](README.US.md) · [README.ZA](README.ZA.md)

**Politique de traduction :** Ces fichiers constituent des points d’entrée localisés initiaux par pays. Leurs introductions et intitulés de langue sont localisés, tandis que la référence technique complète reste le [README.md canonique en anglais](README.md). Un fichier pays ne doit pas être présenté comme une traduction complète tant que toutes ses sections n’ont pas été traduites, que les identifiants techniques et les liens n’ont pas été vérifiés et que l’édition n’a pas fait l’objet d’une révision linguistique. La couverture linguistique de cette première phase est représentative ; elle ne prétend pas que toutes les langues reconnues par les constitutions, régionales, autochtones ou des signes ont été entièrement traduites.

**État de traduction :** les sections I à XIX sont traduites en français. Les identifiants techniques, commandes, exemples de code, noms de fichiers, liens et adresses ont été conservés. Une révision linguistique finale par des locuteurs natifs est recommandée.


---

# SLeeLa 3.0 — België (BE)

## Nederlands

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Een native systeemtaal met compiler, runtime en ecosysteem van virtuele machines.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Internationale landeneditie voor België — Nederlands.** Deze editie bevat de Nederlandse versie van de technische README voor de Nederlandstalige lezers in België. Technische identifiers, opdrachten, code, bestandsnamen en links blijven ongewijzigd.

**The Now. The Ingredient.** Een programmeertaal is meer dan alleen de syntaxis: ook de compiler, runtime, bibliotheken, hulpmiddelen en platformcontracten die software bruikbaar maken, horen erbij.

**Werkterreinen van SLeeLa.** SLeeLa houdt zich bezig met **Math**, **Object**, **Science** en **Reach**, de vier gezichten van zijn werk: het berekenbare (Math), het vaststaande ding in de catalogus van wat is (Object), het gedisciplineerde onderzoek naar het ware (Science) en het uitstrekken van de hand van een systeem naar andere systemen en mensen (Reach). **Nationale belangen blijven bestaan**; geen van de vier heft ze op. Wanneer een onderwerp het belang van een land raakt, blijft die positie behouden en wordt ze getrouw vastgelegd, bijvoorbeeld in de metadatavelden `metaOrigin` / `metaNationalSchedule` van elke klasse, en nooit opgelost.

## I. Aan de slag

- [De native implementatie bouwen](impl/README.md)
- [Virtuele machines en Modium — praktische gebruikershandleiding](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [CPU-stack in SLeeLa-broncode](lib/cpu/CPU.md) — een volledige machine van poorten via ALU, cores en besturingssysteem tot programma's, een lijst van meer dan 130 CPU-profielen en instelbare [spanningen, variabele kloksnelheden en expert-timings](lib/cpu/TIMING.md) die u [volgens specificatie of overgeklokt kunt uitvoeren](lib/cpu/components/README.md)
- [Documentatie-index](docs/README.md)
- [CSPML-gegevensbron en onderzoeksreferentie Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — herkomst en integratierichtlijnen voor externe Amerikaanse gegevens over plaatsen, sociaaleconomische omstandigheden, arbeidskrachten, onderwijs en openbare scholen.
- [Gegenereerde API-documentatie](docs/api/README.md) — actuele HTML-referentie, versiegebonden API-uitgave en instructies om opnieuw te genereren
- [API-catalogus](api/README.md)
- [Normen voor computationele vergelijkingen](comparison/README.md) — normen voor typen, geheugen, ABI en conformiteit van de SLeeLa-VM, Java/JVM en native C/C++.
- [Recursie-stanchion — C- en C++-prototypes](recursion/stanchion/README.md) — op primitieve typen toegespitste voorbeelden voor Boolean, Byte, Character, Integer, Long, Float en Double, naast generieke, toegepaste, wetenschappelijke en didactische recursiepatronen.
- [Platformondersteuning en validatiematrix](docs/PLATFORM-SUPPORT.md)
- [Merkgids](docs/BRAND-GUIDE.md)
- [Merchandiseconcepten en -materiaal](merchandise/README.md)
- [NitroWebExpress-logo](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Testbeleid en releasepoorten](markdown/TESTING.md)

## II. Nieuws

Recente toevoegingen aan de taal, de drivers en de standaardbibliotheek:

- **Het standaardbibliotheekpakket `advantage` — een dividend benutten, met wiskunde, fysica, integratie, beoordeling en routeerbare uitvoer.** [`lib/advantage/`](lib/advantage/README.md) kapselt het *benutten van een uniek dividend ten opzichte van een model* in als een gelaagde **Sequitur onion** (dividend → beoordeling → aandachtspunten → wiskunde → toevoegingen → aftrek → hergroepering → iteratie), die naar binnen en naar buiten doorlopen kan worden met volledige dynamische bijwerking en restitutie, en wordt ontsloten via een eenvoudig **gebruikersmodel** en een volledig te besturen **ontwikkelaarsmodel**. Het omvat vijf vakgebieden: **`math/`** bilaterale handelseconomie (gezamenlijke winstmaximalisatie, Nash-onderhandeling, de kritieke ratio van de bilaterale newsvendor), **`physics/`** een structureel-economische "fysica van vertrouwensvelden" (vertrouwen als metrische tensor, de transactionele geodeet, een aankondigingscontactstructuur, een CIR+sprong-SDE voor vertrouwensverval — een modelleringsanalogie, als zodanig aangeduid), **`integration/`** een recursief samenstelbaar model voor verticale integratie in kapitalisme en detailhandel (compartimenten, engineeringbays, proefcompartimenten, newton-mediaanlades en een score voor systeemverfijning), **`grading/`** een verfijningscircuit-beoordelaar die elke module en elke verzameling winnende/verliezende/beoordelende/absolute modules met A–D beoordeelt, en **`output/`** een routeerbare uitgifteschakel die moduleuitvoer naar de terminal, een naar **CommonRails** geformatteerd oppervlak of een socket/connector stuurt (omkaderd voor een native `slnet_write`). Elke klasse draagt het Metadata-Class-record; vijf uitvoerbare demo's valideren de vakgebieden, en de bijbehorende [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md), [`MATH.md`](lib/advantage/MATH.md) en [`PHYSICS.md`](lib/advantage/PHYSICS.md) documenteren ze. Een native C/C++-consument onder [`advantage/`](advantage/README.md) (de C-kern `advantage_value`/`advantage_mean`, een XML-kennisopslag en een beveiligde HTTPS/SFTP/SSH-acquisitie-, keystore- en DH/ECDH-transportbibliotheek) koppelt het model aan uitgewerkte [handelsvoorbeelden](advantage/examples/README.md) — positieve transacties uit veronderstelde internetbronnen, een relevantiereeks voor het publiek (Traders, Consumers, Helpers, Healers) en acht modellen gerangschikt van A tot D (D = meeste Cement, A = meeste Brand).
- **Lustijdannotaties — `@loopDelay` / `@loopBudget` (+ een ingebouwde functie `sleep(ms)`).** De eerste **gedragsmatige annotaties op instructieniveau** in SLeeLa: ze staan op een eigen regel vóór een `for`-, `while`- of `do`-lus en voegen een echte pauze toe aan de uitvoering van de lus. `@loopDelay(ms = 250)` / `@loopDelay(seconds = 2)` wacht per iteratie een vaste tijd (een kale `@loopDelay(250)` gebruikt standaard milliseconden); `@loopBudget(seconds = 10, iterations = 1000)` spreidt een bekend aantal iteraties gelijkmatig over een totaal budget aan kloktijd. Beide worden vertaald naar de nieuwe append-only opcode **`OP_SLEEP`** (ISA 157), die ook de algemene, overal aanroepbare ingebouwde functie **`sleep(<ms>)`** ondersteunt; de pauze is draagbaar geïmplementeerd (`nanosleep` / `Sleep`). Gespecificeerd in [`SLEELA.syntax` §32](SLEELA.syntax); API-referentie in [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md); uitvoerbaar voorbeeld: [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela). Een index van sessietoevoegingen (waaronder deze, de native grote-gehele-getallenvermenigvuldiging `OP_BIGMUL` en de zelfstandige hulpmiddelen **[local-generator](local-generator/README.md)** en **[pi-generator](pi-generator/README.md)**) staat in [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md). De drie gegenereerde API-oppervlakken ([`sleeladoc-api/`](sleeladoc-api/README.md), [`docs/api/`](docs/api/README.md) en de onveranderlijke momentopname [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html)) zijn opnieuw opgebouwd met `make api-docs` en bevatten nu de nieuwe instructievermeldingen `SLISASleep` / `SLISABigMul` op hun paginapakket `vm`.
- **SleelaDoc vanaf de opdrachtregel — `sleela docs`.** SleelaDoc is nu rechtstreeks vanuit de terminal bereikbaar. `sleela docs` toont alle documenteerbare onderwerpen; `sleela docs crypto` toont een pakket; `sleela docs stringbuilder` herleidt een klasse (hoofdletterongevoelig en via het standaardbibliotheekvoorvoegsel `SL` → `SLStringBuilder`) en toont een uitgebreide pagina — **definitie, ontwerpdoel, relaties, structuur, user clays en een beheersingsladder met vijf sporten** — met gerangschikte "bedoelde u"-suggesties bij een bijna-treffer. Auteurs verrijken elke pagina eenvoudig door SleelaDoc-`@`-tags toe te voegen (`@intent`, `@thesis`, `@axiom`, `@param`, `@see`, `@mastery`). `sleela docs --generate [dir]` schrijft een tekstcorpus (één pagina per klasse + een `TOPICS.index`) dat de CLI vervolgens offline leest; **`make sleeladoc`** bundelt dat corpus **en de bijbehorende `.sleela`-bronbestanden** in `sleeladoc/build/sleeladoc.jar`. Voor onderwerpen met alleen een façade (bijv. `psychiatry`, `politico`) doorzoekt `sleela docs --facade <topic>` tijdens de uitvoering het bestandssysteem en toont de echte **mappen (daarna bestanden, als natuurlijke beweging)**, begrensd door door de gebruiker te overschrijven limieten van **6 MB / 6000 bestanden / 6 minuten**. De eenheden van `lib/sleeladoc` worden onderhouden in de huidige editie, **`#sleela 3.0`**. Zie [`man/man1/sleela.1`](man/man1/sleela.1) en [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md).
- **SleelaDoc — een documentatieoplossing in Javadoc-stijl, met een gegenereerde HTML-site.** **SleelaDoc** is de conventie van SLeeLa voor documentatiecommentaar in de broncode (commentaar tussen hoekhaken met de sentinel `@doc` / `@page`; zie [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)), en [`lib/sleeladoc`](lib/sleeladoc/README.md) is de **extractor** die een `.sleela`-eenheid weergeeft als Markdown- of HTML-API-pagina — zoals `javadoc` Java weergeeft — en daarbij de *bedoeling* documenteert. Een complete, doorbladerbare **HTML-documentatiesite voor de volledige map `/lib`** wordt door `tools/generate-sleeladoc-api.py` gegenereerd in [`/sleeladoc-api`](sleeladoc-api/README.md) (open `sleeladoc-api/index.html`; een heldere stijl in blauw / lichtblauw / lichtgrijs / wit met een hiërarchie pakket → klasse → lid en een woordenlijst voor de lezer van het niveau *Student 2*). De compiler produceert bovendien als bijproduct van het compileren door de auteur gemarkeerde documentuitvoer (`--wiki`: een MediaWiki-pagina + een afgeleide brontwin), gedocumenteerd in [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md). Uitvoerbaar: [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela).
- **XML Document Object Model (`lib/xml`).** Een XML-DOM aan de SLeeLa-kant — een document met een boom van element-, tekst-, commentaar-, CDATA- en PI-knopen met geordende kinderen en benoemde attributen, een serializer (compact/mooi opgemaakt, XML 1.0 §2.4-escaping), een parser op basis van tekencellen en het kwaliteitsregister `SLXmlAspect`. Knoopgelijkheid is een drietrapsband van vertrouwen (identiteit → hetzelfde onderwerp → structuur), reflexief, symmetrisch en deterministisch. Zie [`lib/xml/README.md`](lib/xml/README.md) en het uitvoerbare [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela).
- **Brede gehele getallen met teken (`long`, `int64`, `int128`).** `int`, `long` en `int64` zijn alle 64-bits gehele getallen met teken die identiek worden getypecontroleerd en uitgevoerd; `int128` wordt als geheel type geaccepteerd (voorlopig 64-bits ondersteund; echte 128 bits is een gedocumenteerd vervolg). De base-2-reeks met teken `int32`..`int16777216` (syntaxis 2.7) en de overeenkomstige reeks zonder teken `uint32`..`uint16777216` (syntaxis 2.8) breiden dit uit. Zie [`SLEELA.syntax`](SLEELA.syntax) §21, §24, §25.
- **`@mix`-opvolgvelden (syntaxis 2.9).** Een klasse mag meerdere instantievelden met dezelfde **naam** declareren wanneer elke herhaling `@mix` draagt; de taal begrijpt de groep als opvolgend (een array / lijst met index als sleutel), standaard van boven naar beneden `1,2,3,…` of handmatig via `@mix(1,2,3,...)`, waarvan het aantal indices moet overeenkomen met het aantal velden. Zie [`SLEELA.syntax`](SLEELA.syntax) §28.
- **Documentmarkeringen — het Natuurlijke Proces, getiteld de Preonems (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).** Deze kale documentmarkeringswoorden heten gezamenlijk de **Preonems** (één markering is een *Preonem*; de verzameling, de *Preonems* — zie [`SLEELA.syntax`](SLEELA.syntax) §26.1). Eersteklas, uitsluitend declaratieve documentmarkeringen die benoemen wat een document *is* in de volgorde van het maken: `author`/`authorial` (syntaxis 2.4; behandeld door een beroepsraad — het enige lid met een gedragseffect), `material;` (vastgestelde Bron) en `memory;` (syntaxis 2.5), `determine;` (syntaxis 2.6; een document dat openstaat om door een arts, medicus of ghost verder te worden verwerkt tot voltooiing door wettelijke hand), en de twee nieuwste, **`object;`** (een vaststaand ding in de catalogus van wat is — **gereïficeerd, met concrete status, in syntaxis 2.8**) en **`field;`** (de grond van relatie waarin objecten staan en bewegen) — beide vrijgegeven vanaf syntaxis 2.6. Elke markering is kaal, contextueel en door de front-end versiegebonden, zonder effect op parsing, typering of codegeneratie; buiten de markeringspositie blijft elk woord een gewone identifier (zodat `field` nooit een klasselid of een `.field`-toegang verstoort). Zie [`SLEELA.syntax`](SLEELA.syntax) §18–§20, §23 en §26.
- **Nordshrift met syntaxis 3.0 en volledig bereik over de bibliotheek.** Nordshrift herleidt namen via dezelfde recursieve `/lib`-index als de compiler en bereikt zo elk pakket en elke `.sleela`-eenheid; `nordshrift check` rapporteert de aantallen voor de hele boom.
- **Recursie-stanchion.** De map [`recursion/stanchion/`](recursion/stanchion/README.md) bevat nu eigen C- en C++-referentieprototypes voor de zeven gedocumenteerde primitieve contracten, plus generieke, toegepaste, wetenschappelijke en didactische voorbeelden. Dit zijn referentiebronnen; compileer en test ze voordat u ze als geverifieerd beschouwt.
- **Commentaar tussen hoekhaken (`<` / `<<`).** Een nieuwe commentaarstijl voor de toekomst — `< regel >` en `<< blok >>` — wordt door de SLeeLa-front-end lexicaal herkend en is gedocumenteerd in [`SLEELA.syntax`](SLEELA.syntax) §1.1.
- **Gehele getalliteralen met grondtal.** `int`-literalen accepteren nu hexadecimale (`0x`), binaire (`0b`) en octale (`0o`) vormen met `_` als cijferscheidingsteken, zowel in de SLeeLa-brontaal ([`SLEELA.syntax`](SLEELA.syntax) §1.4) als in het [`.xlt`-CPU-modelformaat](lib/xlt/README.md).
- **De Metadata-Class-standaard in de hele bibliotheek.** Het beschrijvende-metadatapatroon (herkomst, rechtmatigheid, rechtsgebied) wordt nu gedragen door elk klassebevattend `.sleela`-bestand onder `/lib`; zie [`SLEELA.syntax`](SLEELA.syntax) §15 en [de tutorialles](tutorial/13-metadata-class.md).
- **Een rijkere tekst-/String-familie.** `lib/text` krijgt echte hulpfuncties in de taal zelf, een tekencelweergave met echte `charAt`/`substring`/`indexOf`/`reverse`/kolomuitlijning en een echte positionele sjabloonverwerker — zie de [tutorial over tekst en String](tutorial/14-text-and-strings.md) en het uitvoerbare [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela).
- **Eersteklas `String` in Nordshrift-sheets.** Een `.sst`-sheet kan een getypeerd blok `strings:` declareren dat bijdraagt aan het componentenmanifest van de build; zie de [Nordshrift Sheet Reference](wiki/Nordshrift-Sheet-Reference.md).
- **CPU-modellen die zowel via opcodes als via functieaanroep-equivalentie draaien.** Elke `SL<ARCH>CPU` erft de basis om een native instructiestroom via zijn fetch-decode-execute-lus uit te voeren *en* de code van een verondersteld ander proces uit te voeren via equivalente aanroepen op het gedeelde VM-substraat — zie [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5.

## III. Bouwen en testen

**Vereisten:** een C/C++-toolchain (GCC of Clang) en GNU Make; Python 3 voor de manifest- en flowhulpmiddelen; onder Linux de X11/Xrandr-ontwikkelpakketten voor de weergaveproducten (`make deps` van een module meldt/installeert wat die nodig heeft). De build in `impl/` is fail-closed op een vertrouwd SHA-256-manifest, waarnaar de `Makefile` in de hoofdmap automatisch verwijst.

De snelste start vanaf een schone checkout:

```sh
make                                  # alles bouwen vanuit de hoofdmap van de repository
```

Of alleen de native kern bouwen en testen:

```sh
make -C impl all                      # alleen de compilerkern/runtime bouwen
bash ./test-suites/run-all.sh --all   # de volledige testsuite uitvoeren
```

De `make` in de hoofdmap bouwt elk product (kern, VM, codecs, churn, antivirus, …) en eindigt met het bouwen van het **register** en het materialiseren van de basistabel, zodat een gebouwde boom standaard een controleerbaar register meelevert. De belangrijkste doelen:

```sh
make                     # alles bouwen (alias van `make all`; het register draait als laatste)
make all                 # dezelfde volledige build, expliciet genoemd
sudo make install        # opdrachten + runtime-ondersteuning installeren (zie §IV)
make clean               # buildresultaten van elk aangestuurd buildsysteem verwijderen
make help                # elk doel van de Makefile in de hoofdmap tonen
```

Andere nuttige doelen:

```sh
make registry            # alleen het register: bouwen + de tabel materialiseren
./build-registry.sh      # hetzelfde, rechtstreeks aangeroepen
make tests               # de testsuite bouwen en uitvoeren
make ballum              # de Ballum-weergavebibliotheek voor dit besturingssysteem bouwen (zie ballum/)
bin/install-modules      # gespreide installatie/build van elke module gedurende ~10-20 min (zie bin/)
```

Het doel `registry` voert [`build-registry.sh`](build-registry.sh) uit: het bouwt de registerkern, zaait de basistabel in het draagbare tekstarchief `registry/build/sleela.sleelareg` en exporteert het MySQL-tabelscript `registry/build/registry.sql` (via `sleela registry export`). Bekijk het met `sleela registry` of `registry/build/sleela-registry review`. Zie de [registergids](registry/README.md) en de [architectuur van Registry 3.0](registry/REGISTRY-3.0-ARCHITECTURE.md).

Raadpleeg de build- en testgidsen voor vereisten en platformspecifieke stappen. Een geslaagde build van één component bewijst niet dat elke bibliotheek of elk platform klaar is voor release.

## IV. Installatie

Installeer na het bouwen van de native producten de opdrachten en de runtime-ondersteuningsbestanden:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # installatie voor de gebruiker, zonder sudo
make install DESTDIR="$PWD/package-root" # gefaseerde pakketinstallatie
make install-check
```

Het conventionele installatieprogramma gebruikt standaard `/usr/local/bin`, `/usr/local/lib/sleela` en `/usr/local/share/sleela`. Het behoudt een bestaande opstartconfiguratie en weigert bestaande opdrachten te overschrijven die het niet bijhoudt. Het verwijderen haalt alleen door het installatieprogramma vastgelegde bestanden weg en laat configuratie en niet-bijgehouden runtimegegevens ongemoeid.

Voor een gerichte, begeleide installatie van alleen het hoofdpakket — de binaire bestanden `sleela`, `sleelvac` en `nordshrift`, de standaardbibliotheek, protocollen en ondersteunende modules — gebruikt u het schone installatieprogramma:

```sh
make clean-install       # begeleide installatie van alleen het hoofdpakket
```

Het vraagt eerst één hoofdbeslissing (het hoofdpakket installeren), dan een keuze van de reikwijdte en bij succes een optionele keuzelijst met samengestelde uitbreidingsmodules. Voor de interactieve installatieprocedure en alle installatieopties, zie [Snel en veilig installeren](install/README.md).

## V. Taal en runtime

De gezaghebbende grammatica is [`SLEELA.syntax`](SLEELA.syntax). De repository documenteert momenteel syntaxis **3.0**; de versie van compiler/hulpmiddelen en de versie van de brontaalsyntaxis zijn verschillend. Het 3.0-oppervlak voegt de **documentmarkeringen `pure` en `gasoline`** toe (§30) — een paar kale, contextuele, uitsluitend declaratieve Preonems die vrijwel hetzelfde betekenen en elk de status van het document markeren aan de hand van zijn eigen eerlijkheid. Syntaxis 3.0 bundelt het taaloppervlak van 1.0–2.9 in de huidige normatieve uitgave en behoudt alle functiespecifieke minimumdrempels. Het voorgaande 2.9-oppervlak voegt **`@mix`-opvolgvelden** toe (§28) — een klasse mag meerdere instantievelden met dezelfde naam declareren wanneer elk `@mix` draagt, begrepen als één geïndexeerde groep (van boven naar beneden `1,2,3,…` of handmatig `@mix(1,2,3,...)`, waarvan het aantal indices met de groep moet overeenstemmen). Het draagt de base-2-families van brede gehele getallen met en zonder teken `int32`..`int16777216` (§24) en `uint32`..`uint16777216` (§25) voort bovenop de eerdere `long`/`int64`/`int128` (§21); de **ondersteunende sleutelwoorddocumenten** (§22); en de **Preonems** — de uitsluitend declaratieve documentmarkeringen `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) en `artifact` (§35, status op wetgevend niveau, gegradueerd naar tijd en ruimte), gezamenlijk de Preonems genoemd (§26.1). Overload-resolutie van constructors en delegatie via `this(...)`/basisconstructor vallen uitdrukkelijk buiten het geverifieerde syntaxiscontract 1.8. Raadpleeg de grammatica en de implementatienotities voordat u op een functie vertrouwt. De negen bronnenheden van SleelaDoc declareren nu `#sleela 3.0`; de gedeelde compilerpoort accepteert bronsyntaxis 1.0–3.0. De nieuwe API-build wordt geproduceerd door `make api-docs`.

De repository bevat een native C/C++-implementatie, SLVM-generatiereferenties, modules van de standaardbibliotheek, tests, voorbeelden en platformintegraties. De implementatie- en validatiestatus verschilt per module; raadpleeg de specificatie en de actuele CI-resultaten in plaats van uniforme volledigheid aan te nemen.

## VI. Engineeringprincipes

- **Bewijs boven beweringen:** onderscheid geïmplementeerd, getest, experimenteel en gepland gedrag.
- **Eén contract per grens:** documenteer syntaxis, API's, bestandsformaten, platformaannames en foutgedrag.
- **Draagbaar door ontwerp:** isoleer platformspecifieke backends en test op native runners.
- **Veilig falen:** test onjuiste invoer, resourcelimieten, procesfouten en opruimpaden.
- **Reproduceerbare releases:** bouw schoon opnieuw en publiceer artefacthashes en release-opmerkingen.

## VII. Antivirusbuild en integriteit

Het antiviruspakket Sleelavirin bevat een native SHA-256-scanner, een optionele ClamAV-adapter, integriteitsmanifesten voor bron/index en controlesommen voor platformpakketten. Bouw en test het pakket vanuit de hoofdmap van de repository:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Het integriteitsmanifest wordt gegenereerd onder `antivirus/build/`; het omvat bijgehouden bron- en documentatie-invoer plus beschermde bestanden van de handtekeningindex, en sluit gegenereerde uitvoer en de veranderlijke ClamAV-cache uit. Momentopnamen van ClamAV-databases hebben hun eigen manifesten. Release-ZIP's bevatten `INTEGRITY-MANIFEST.json`, `SHA256SUMS` en een `.sha256`-sidecar. Controleer hashes van gedownloade archieven aan de hand van een vertrouwde waarde die u onafhankelijk hebt verkregen: een controlesom naast een archief is geen handtekening van de uitgever. Zie de [Sleelavirin-gids](antivirus/README.md), de [beveiligingschecklist](antivirus/SECURITY.md) en de [antivirus-buildworkflow](.github/workflows/antivirus-binaries.yml).

Een release geldt pas als geverifieerd wanneer de relevante CI-taken slagen op de exacte releasecommit en de resulterende artefacten zijn gecontroleerd.

## VIII. SLeeLa-editorthema's — Vim en Emacs

SLeeLa bevat een op elkaar afgestemde set professionele donkere kleurenschema's voor **Vim** en **Emacs**, plus een native C/C++-paletprogramma. Het oorspronkelijke schema **American Professional** blijft de standaard. Tien aanvullende paletten zijn beschikbaar voor uiteenlopende culturele en institutionele ontwerpinspiraties; dit zijn interpretatieve ontwerppaletten en geen officiële weergave van een land, cultuur, rechtbank of instelling.

### De elf schema's en hun kleuren

| Schema | Achtergrond | Voorgrond | Sleutelwoorden | Typen | Strings | Commentaar |
|---|---|---|---|---|---|---|
| **American Professional** | <span style="display:inline-block;width:1em;height:1em;background:#101820;border:1px solid #888"></span> `#101820` | <span style="display:inline-block;width:1em;height:1em;background:#F4F7FA;border:1px solid #888"></span> `#F4F7FA` | <span style="display:inline-block;width:1em;height:1em;background:#7FB3FF;border:1px solid #888"></span> `#7FB3FF` | <span style="display:inline-block;width:1em;height:1em;background:#73DACA;border:1px solid #888"></span> `#73DACA` | <span style="display:inline-block;width:1em;height:1em;background:#F2C078;border:1px solid #888"></span> `#F2C078` | <span style="display:inline-block;width:1em;height:1em;background:#8A96A3;border:1px solid #888"></span> `#8A96A3` |
| **Pan-Eurasian** | <span style="display:inline-block;width:1em;height:1em;background:#111827;border:1px solid #888"></span> `#111827` | <span style="display:inline-block;width:1em;height:1em;background:#F0F2F5;border:1px solid #888"></span> `#F0F2F5` | <span style="display:inline-block;width:1em;height:1em;background:#7AA7E8;border:1px solid #888"></span> `#7AA7E8` | <span style="display:inline-block;width:1em;height:1em;background:#69C9B3;border:1px solid #888"></span> `#69C9B3` | <span style="display:inline-block;width:1em;height:1em;background:#D7B56D;border:1px solid #888"></span> `#D7B56D` | <span style="display:inline-block;width:1em;height:1em;background:#8995A5;border:1px solid #888"></span> `#8995A5` |
| **Russisch en Slavisch — gemiddeld** | <span style="display:inline-block;width:1em;height:1em;background:#202733;border:1px solid #888"></span> `#202733` | <span style="display:inline-block;width:1em;height:1em;background:#E8EAF0;border:1px solid #888"></span> `#E8EAF0` | <span style="display:inline-block;width:1em;height:1em;background:#A9C0DD;border:1px solid #888"></span> `#A9C0DD` | <span style="display:inline-block;width:1em;height:1em;background:#8FC4B2;border:1px solid #888"></span> `#8FC4B2` | <span style="display:inline-block;width:1em;height:1em;background:#D6BD8B;border:1px solid #888"></span> `#D6BD8B` | <span style="display:inline-block;width:1em;height:1em;background:#9AA3AF;border:1px solid #888"></span> `#9AA3AF` |
| **Centraal-Aziatisch / Zijderoute** | <span style="display:inline-block;width:1em;height:1em;background:#1C2529;border:1px solid #888"></span> `#1C2529` | <span style="display:inline-block;width:1em;height:1em;background:#F5F0E5;border:1px solid #888"></span> `#F5F0E5` | <span style="display:inline-block;width:1em;height:1em;background:#7FADE0;border:1px solid #888"></span> `#7FADE0` | <span style="display:inline-block;width:1em;height:1em;background:#45C6C2;border:1px solid #888"></span> `#45C6C2` | <span style="display:inline-block;width:1em;height:1em;background:#D9B56D;border:1px solid #888"></span> `#D9B56D` | <span style="display:inline-block;width:1em;height:1em;background:#92A3A0;border:1px solid #888"></span> `#92A3A0` |
| **Oost-Aziatisch** | <span style="display:inline-block;width:1em;height:1em;background:#171A1C;border:1px solid #888"></span> `#171A1C` | <span style="display:inline-block;width:1em;height:1em;background:#F2EFE7;border:1px solid #888"></span> `#F2EFE7` | <span style="display:inline-block;width:1em;height:1em;background:#D97878;border:1px solid #888"></span> `#D97878` | <span style="display:inline-block;width:1em;height:1em;background:#72C7A5;border:1px solid #888"></span> `#72C7A5` | <span style="display:inline-block;width:1em;height:1em;background:#D8B76A;border:1px solid #888"></span> `#D8B76A` | <span style="display:inline-block;width:1em;height:1em;background:#969F9B;border:1px solid #888"></span> `#969F9B` |
| **ICC — Internationaal Strafhof** | <span style="display:inline-block;width:1em;height:1em;background:#0E1A2B;border:1px solid #888"></span> `#0E1A2B` | <span style="display:inline-block;width:1em;height:1em;background:#EAF0F7;border:1px solid #888"></span> `#EAF0F7` | <span style="display:inline-block;width:1em;height:1em;background:#5B8FD6;border:1px solid #888"></span> `#5B8FD6` | <span style="display:inline-block;width:1em;height:1em;background:#7FB7C9;border:1px solid #888"></span> `#7FB7C9` | <span style="display:inline-block;width:1em;height:1em;background:#D9B25A;border:1px solid #888"></span> `#D9B25A` | <span style="display:inline-block;width:1em;height:1em;background:#7E8CA0;border:1px solid #888"></span> `#7E8CA0` |
| **SCOTUS — Hooggerechtshof van de VS** | <span style="display:inline-block;width:1em;height:1em;background:#1A1416;border:1px solid #888"></span> `#1A1416` | <span style="display:inline-block;width:1em;height:1em;background:#F3EEE6;border:1px solid #888"></span> `#F3EEE6` | <span style="display:inline-block;width:1em;height:1em;background:#9E2B35;border:1px solid #888"></span> `#9E2B35` | <span style="display:inline-block;width:1em;height:1em;background:#8FA9B8;border:1px solid #888"></span> `#8FA9B8` | <span style="display:inline-block;width:1em;height:1em;background:#C9A24B;border:1px solid #888"></span> `#C9A24B` | <span style="display:inline-block;width:1em;height:1em;background:#8C8178;border:1px solid #888"></span> `#8C8178` |
| **Grieks (Helleens)** | <span style="display:inline-block;width:1em;height:1em;background:#0A1A30;border:1px solid #888"></span> `#0A1A30` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#2E7BD6;border:1px solid #888"></span> `#2E7BD6` | <span style="display:inline-block;width:1em;height:1em;background:#5E9FE0;border:1px solid #888"></span> `#5E9FE0` | <span style="display:inline-block;width:1em;height:1em;background:#9FC3EC;border:1px solid #888"></span> `#9FC3EC` | <span style="display:inline-block;width:1em;height:1em;background:#6E86A6;border:1px solid #888"></span> `#6E86A6` |
| **Turks (Al Bayrak)** | <span style="display:inline-block;width:1em;height:1em;background:#2A0609;border:1px solid #888"></span> `#2A0609` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#F23D48;border:1px solid #888"></span> `#F23D48` | <span style="display:inline-block;width:1em;height:1em;background:#F0A9AE;border:1px solid #888"></span> `#F0A9AE` | <span style="display:inline-block;width:1em;height:1em;background:#F0C9A9;border:1px solid #888"></span> `#F0C9A9` | <span style="display:inline-block;width:1em;height:1em;background:#A8767A;border:1px solid #888"></span> `#A8767A` |
| **Kiro** | <span style="display:inline-block;width:1em;height:1em;background:#0B0B12;border:1px solid #888"></span> `#0B0B12` | <span style="display:inline-block;width:1em;height:1em;background:#F5F5FA;border:1px solid #888"></span> `#F5F5FA` | <span style="display:inline-block;width:1em;height:1em;background:#A98EDA;border:1px solid #888"></span> `#A98EDA` | <span style="display:inline-block;width:1em;height:1em;background:#6E8BE0;border:1px solid #888"></span> `#6E8BE0` | <span style="display:inline-block;width:1em;height:1em;background:#E8C547;border:1px solid #888"></span> `#E8C547` | <span style="display:inline-block;width:1em;height:1em;background:#6A6482;border:1px solid #888"></span> `#6A6482` |
| **SLeeLa** | <span style="display:inline-block;width:1em;height:1em;background:#08080E;border:1px solid #888"></span> `#08080E` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#B07CFF;border:1px solid #888"></span> `#B07CFF` | <span style="display:inline-block;width:1em;height:1em;background:#5A7CFF;border:1px solid #888"></span> `#5A7CFF` | <span style="display:inline-block;width:1em;height:1em;background:#FFD21E;border:1px solid #888"></span> `#FFD21E` | <span style="display:inline-block;width:1em;height:1em;background:#6E5F9E;border:1px solid #888"></span> `#6E5F9E` |

#### Kleuren van de nationale vlag (bronpalet)

De Griekse en Turkse schema's zijn afgeleid van de **officiële kleuren van de nationale vlag** van beide landen, hier getoond met hun exacte gepubliceerde hexwaarden. De achtergronden en accenten van de schema's hierboven zijn verdonkerde, voor editors geschikte interpretaties van deze bronkleuren — de tabel hieronder is de referentie voor de "kleuren zoals nationaal vastgelegd".

| Land | Kleur | Hex | Grondslag en betekenis |
|---|---|---|---|
| **Griekenland** | Blauw | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | Blauw van de nationale vlag (wet 851/1978). De hemel en de zeeën; heraldisch geblazoeneerd als *azuur*. |
| **Griekenland** | Wit | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Wit van de nationale vlag. De zuiverheid van de onafhankelijkheidsstrijd; het kruis staat voor het oosters-orthodoxe christendom. |
| **Türkiye** | Rood (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | Rood van de nationale vlag (Turkse vlagwet). Het veld; naar traditie het bloed van martelaren en de Ottomaanse soevereiniteit. |
| **Türkiye** | Wit | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Wit van de nationale vlag. De halve maan en de ster — vrede, zuiverheid en het symbool van het Turkse/islamitische erfgoed. |

De schema's verankeren de voorgrond van elke editor op het nationale **wit** en de sleutelwoorden, de cursor, de selectie en de statusbalk op het exacte nationale **blauw** (`#0D5EAF`) of **rood** (`#E30A17`), zodat de kleuren die ertoe doen letterlijk de nationale kleuren zijn. Dit zijn interpretatieve ontwerppaletten; de verwijzing naar de vlagkleuren dient om de herkomst aan te geven en is geen officiële weergave van een van beide staten. De kleurbetekenissen volgen de beschrijving van de staatssymbolen door het ministerie van Buitenlandse Zaken van de Helleense Republiek en de traditie van de Turkse vlagwet.


### Tabel met kleurstalen

De paletcellen hieronder gebruiken **SVG-kleurstalen**, zodat GitHub de werkelijke kleuren toont in plaats van lege HTML-kleurcellen. Elke strook toont de zes kleuren in deze volgorde: achtergrond, voorgrond, sleutelwoorden, typen, strings en commentaar. De exacte hexcodes staan naast elke kleurstaal, in dezelfde schemavolgorde als de referentietabel hierboven.

| Schema | Weergegeven paletkleuren | Hexcodes (achtergrond · voorgrond · sleutelwoorden · typen · strings · commentaar) |
|---|---|---|
| **American Professional** | ![American Professional palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Pan-Eurasian palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russisch en Slavisch — gemiddeld** | ![Russian and Slavic Medium palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Centraal-Aziatisch / Zijderoute** | ![Central Asian Silk Road palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **Oost-Aziatisch** | ![East Asian palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC — Internationaal Strafhof** | ![ICC palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS — Hooggerechtshof van de VS** | ![SCOTUS palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Grieks (Helleens)** | ![Greek Hellenic palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turks (Al Bayrak)** | ![Turkish Al Bayrak palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Kiro palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![SLeeLa palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

Aanvullende accentkleuren worden gebruikt voor functies, constanten, selecties, diagnostiek en preprocessordirectieven. Het Russische en Slavische palet gebruikt bewust leisteengrijs, blauw, salie, robijn en antiek goud met gemiddelde intensiteit voor een rustiger professioneel uiterlijk.

### De Vim-schema's installeren

Vanuit de hoofdmap van de repository:

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

Kies precies één van deze namen van kleurenschema's:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Gebruik bijvoorbeeld `colorscheme sleela-russian-slavic` voor het gemiddelde Russische en Slavische palet, of `colorscheme sleela-greek` / `colorscheme sleela-turkish` voor de paletten Helleens en Al Bayrak. Start Vim opnieuw of laad uw configuratie met `:source $MYVIMRC`.

### De Emacs-schema's installeren

Maak een lokale themamap en kopieer de modus- en themabestanden vanuit de hoofdmap van de repository:

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

Vervang `sleela-american` door `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` of `sleela-sleela` om een ander schema te kiezen. Als Emacs vraagt of u het thema vertrouwt, controleer dan eerst het lokale themabestand voordat u het accepteert.

### De native C/C++-paletten bekijken

Het kleine native paletprogramma kan schema's weergeven, hun terminalkleuren afdrukken en een afzonderlijke kleur opzoeken:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Voor achterwaartse compatibiliteit selecteren `--palette` en `--hex COLOR` zonder `--theme` American Professional. Build- en testresultaten moeten op het doelsysteem en in de CI worden gecontroleerd; de documentatie van een opdracht is zelf geen bewijs dat die geslaagd is.

Zie [de gids voor editorintegratie](linux-editors/README.md) voor de volledige paletbeschrijvingen en details over het programma.

## IX. Bijdragen

1. Lees de documentatie-index en het testbeleid.
2. Maak gerichte wijzigingen met positieve en negatieve tests.
3. Voer gerichte tests uit en daarna de brede suite.
4. Meld details over besturingssysteem en toolchain; beschrijf overgeslagen controles niet als geslaagd.

SLeeLa wordt ontwikkeld door **MEARVK LLC**. Historisch landingspaginamateriaal is bewaard in [`docs/README-LEGACY.md`](docs/README-LEGACY.md).

## X. AE6E66 — Verwachting van een heffing op Round-openbare e-mails

Voor het AE6E66-initiatief is de door het project uitgesproken verwachting **$300,000,000.00 USD per Round-openbare e-mail**, als voorgesteld belastingheffingsbedrag dat aan elke kwalificerende Round-openbare e-mail is verbonden.

Deze verklaring legt uitsluitend een verwachting van het project vast. Zij is **geen** wettelijke belastingaanslag, overheidsbevel, rechterlijk vonnis of onafhankelijk vastgestelde belastingschuld. Een daadwerkelijke heffing vereist de toepasselijke wettelijke autoriteit, bevoegdheid, definities, procedure en een bevoegde overheidsbeslissing.

## XI. Alfabet- en tekensetruntime

De alfabetdocumentatie en de native tekenruntime van SLeeLa worden onderhouden in `alphabet/sleela/` en `lib/alphabet/native/`. De native C++17-tests dekken strikte UTF-8-scalardecodering/-codering, capaciteitshandtekeningen `C<n>` en begrensd lidmaatschap van tekensets:

```sh
make -C lib/alphabet/native test
```

De experimentele UTF-4088-catalogus en de bijbehorende SLeeLa-opzoekadapter zijn gedocumenteerd in [`utf-4088/SLeeLa-ALPHABET-INTEGRATION.md`](utf-4088/SLeeLa-ALPHABET-INTEGRATION.md). UTF-4088-ID's vormen een afzonderlijke naamruimte en zijn geen Unicode-scalarwaarden. Het werk aan compiler- en SLVM-koppelingen wordt uitdrukkelijk apart bijgehouden van deze native fundamenten.

## XII. AutoCAD Creator™

De [AutoCAD Creator™](autocad/creator/README.md) is een interactieve 2D-CAD-toepassing die volledig in SLeeLa is geschreven. Terwijl de verwante AutoCAD Renderer een plattegrond in één keer tot een tekening omzet, laat de Creator u een 2D-AutoCAD-document live **maken en wijzigen** — voeg muren, ruimten, maatvoeringen en notities toe en verplaats, hernoem, verplaats naar een andere laag, verwijder en maak ongedaan wat u hebt geplaatst — en het resultaat **opslaan** als een echte AutoCAD-DXF-tekening (R12-tekst, opent in AutoCAD / LibreCAD).

De grafische interface is gebouwd op **SleelaUI™**, de eigen platformonafhankelijke UI-toolkit van SLeeLa (onder [`lib/user-interface`](lib/user-interface)) — niet JavaFX — en opent een echt native venster onder X11 / Cocoa / Win32. Het ontwerp volgt één regel: **"SleelaUI presenteert; SLeeLa beslist"**: de GUI berekent nooit geometrie en schrijft nooit DXF. Elk gereedschap geeft een intentie tot maken / wijzigen / verwijderen / ongedaan maken / opslaan door aan de gezaghebbende [`SLCadCreator`](lib/autocad/SLCadCreator.sleela), die het bewerkbare document en een omkeerbare opdrachtgeschiedenis beheert, terwijl het tekenvlak alleen een voorbeeld toont van de geometrie die de Creator bijhoudt.

De herbruikbare bewerkingsengine bevindt zich in [`lib/autocad`](lib/autocad) — [`SLCadEntity`](lib/autocad/SLCadEntity.sleela), [`SLCadDocument`](lib/autocad/SLCadDocument.sleela), [`SLCadCommand`](lib/autocad/SLCadCommand.sleela) en [`SLCadCreator`](lib/autocad/SLCadCreator.sleela) — en draait volledig headless van begin tot eind. De GUI-schil en de demo staan onder [`autocad/creator`](autocad/creator):

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

Op een systeem met beeldscherm opent dit het live bewerkingsvenster; op een headless host bouwt het de tekening in het geheugen, drukt een rapport plus de DXF af en sluit netjes af.

## XIII. Volgorde van de UTF-4088-invoerkaart

De gespecialiseerde UTF-4088-catalogus definieert zijn ruimtelijke doorloop van de invoerkaart en abstracte drukiteraties in [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). Het beleid voor de onderwerpindex National Lien staat in [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Kaartcoördinaten, drukniveaus (1–50), permanente catalogus-ID's en alfabetische rangen zijn onafhankelijke velden; geen ervan mag stilzwijgend door een ander worden vervangen. Druk is een algoritmisch iteratieniveau, geen fysieke grootheid.

## XIV. Index van recente en uitgebreide pakketten

Deze toegevoegde index belicht recent uitgebreide pakketten, domeinbibliotheken en bronverzamelingen. De vermeldingen zijn alfabetisch gegroepeerd op pakket- of onderwerpnaam. De beschrijvingen geven de locatie in de repository aan; de implementatie- en validatiestatus moet worden gecontroleerd in de eigen documentatie en tests van elk pakket.

### A–C

- **Advantage** — `lib/advantage/` en `advantage/`: een uniek dividend benutten ten opzichte van een model als een gelaagde Sequitur onion, met vijf vakmappen — `math/` (gezamenlijke winstmaximalisatie, Nash-onderhandeling, bilaterale newsvendor), `physics/` (metriek/geodeet/contact/SDE van vertrouwensvelden, een als zodanig aangeduide modelleringsanalogie), `integration/` (compartimenten voor verticale integratie en een verfijningsscore), `grading/` (een A–D-verfijningscircuit-beoordelaar voor modules en verzamelingen) en `output/` (uitgifte naar terminal / CommonRails / socket). Een native C/C++-consument (`advantage/`) koppelt het model aan uitgewerkte handelsvoorbeelden. Zie [`lib/advantage/README.md`](lib/advantage/README.md) en [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alfabet en UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` en `utf-4088/`: alfabetcatalogi, native ondersteuning voor tekensets, volgorde van de invoerkaart en documentatie over UTF-4088-integratie.
- **AutoCAD Creator** — `autocad/creator/` en `lib/autocad/`: interactieve 2D-tekenwerkstromen en herbruikbare CAD-document- en bewerkingscomponenten.
- **Ballum** — weergavebesturing op pixelniveau: per pixel instellen/uitlezen, regio-blit, helderheid/warmte/gamma van het hele scherm, aanwijzer-/toetsenbordinvoer en een **framebufferoppervlak op volledig scherm** voor totale, eigen, snelle besturing per pixel (override-redirect-venster + één enkele `XPutImage`/MIT-SHM-presentatie onder Linux/X11). Native kern onder `ballum/` met de SLeeLa-façade in `lib/ballum/` en een VM-brug onder `impl/`; zie [`ballum/README.md`](ballum/README.md) en het uitvoerbare [`ballum/example/`](ballum/example/).
- **CDMA** — `cdma/` en `lib/cdma/`: CDMA-gerelateerd technisch materiaal en werk aan de SLeeLa-bronbibliotheek.
- **Character** — `lib/character/`: karakterprofielen en herbruikbare karakterstatistieken, inclusief voorbeelden van biografische profielen.
- **Coorenagraph** — `lib/coorenagraph/`: ethisch redeneren, moreel vocabulaire, overleg en verwante begrippen; zie de pakket-README en de bronbestanden.

### D–G

- **Governance-bronmodellen** — [lib/governance](lib/governance/README.md) definieert declaratieve SLeeLa-bronvormen voor voorstellen, anonieme handmatige stemmen en geaggregeerde telresultaten. De voorgestelde annotatie @vote en het handmatige proces zijn gedocumenteerd in [registry/DEMOCRATIC-VOTING-SYNTAX.md](registry/DEMOCRATIC-VOTING-SYNTAX.md); huidige compilers behandelen haar als inerte commentaarmetadata.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` tot en met de beschikbare versiemappen van HTTP-pakketten: versiespecifieke protocolspecificaties en implementaties. Raadpleeg de documentatie van elke versie voor de precieze reikwijdte en verificatiestatus.
- **International Criminal Court** — `lib/international-criminal-court/` en `INTERNATIONAL.CRIMINAL.COURTS.md`: verwante domeinspecifieke broncode en documentatie.

### O–S

- **OFDMA** — `ofdma/`: technisch materiaal over orthogonale frequentiedivisie-meervoudige toegang en C/C++-ontwikkelmiddelen.
- **Filosofie en de module Conversation Mask** — [`philosophy/`](philosophy/README.md) bevat de native C/C++-fundamenten en [`philosophy/module/`](philosophy/module/README.md) organiseert hun volgende modulelaag in drie expliciete gebieden: [`data/`](philosophy/module/data/README.md) voor getypeerde, gedateerde waarnemingen, gebeurtenisbeëindigingen, halfwaardetijdinstellingen, herkomst en toestemming; [`helper/`](philosophy/module/helper/README.md) voor deterministische validatie, relevantieverval, compatibele vergelijkingen en presentatie gefilterd op toestemming; en [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md) voor een ondersteunende, niet-klinische gespreksinterface. De naar SLeeLa gerichte handles en de samengestelde Tree staan in [`lib/philosophy/`](lib/philosophy/README.md), naast het bredere filosofiepakket. De module werkt op expliciete invoer en stelt toestemming voorop: ze bewaakt mensen of media niet passief, stelt geen diagnoses, leidt geen privé-intenties af, scoort de menselijke waarde niet en beveelt geen wijzigingen in medicatie aan. De gebeurtenislinks bewaren herkomst en navigatie, geen onafhankelijke bevestiging.
- **Synchro** — native runtime-/compilerintegratie en functionaliteit rond pakketverdeling en metingen; zie de betreffende bron in `impl/` en de compilerimplementatie.
- **SleelaUI / terminal-UI** — `lib/user-interface/` en het project `sleela-terminal`: SLeeLa-UI-integratie en op de terminal gerichte interactiecomponenten.

### U–W

- **United Nations** — `united-nations/`: domeinspecifiek repositorymateriaal.
- **Wi-Fi 4, Wi-Fi 5 en Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`, `wi-fi-6/` en verwante bronmappen onder `lib/wifi/`: generatiespecifiek materiaal en configuratiebronnen voor draadloze netwerken.

### Onderhoud van de pakketindex

Wanneer een pakket wordt toegevoegd of wezenlijk uitgebreid, voeg dan hier zijn vermelding toe in de passende alfabetische groep, verwijs naar de canonieke map en README en beschrijf alleen mogelijkheden die door de ingecheckte bronnen worden ondersteund. Houd deze index additief: verwijder of herschrijf de bestaande README-secties niet als onderdeel van updates van de pakketindex.

---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

Amerikaanse democratie en Amerikaans beleid.

---

## XVI. Beeldtaal van het nationale wetenschapsmerk

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="Nationale SLeeLa-wetenschap" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="Nationale SLeeLa NSF" width="560"></p>

## XVII. Experimentele UTF-4088-catalogus met een miljoen items

De [UTF-4088-cataloguegenerator](utf-4088/tools/generate_catalogue.py) bouwt deterministisch een experimentele symboolcatalogus met 1.048.576 items. Het gedocumenteerde compacte formaat wordt beschreven in [utf-4088/FORMAT.md](utf-4088/FORMAT.md); het generatie- en validatiecontract staat in [utf-4088/GENERATION-RESULTS.md](utf-4088/GENERATION-RESULTS.md).

Voer de generator uit met Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Het gegenereerde binaire bestand is 16.777.244 bytes groot en is ontworpen om onder 20.000.000 bytes te blijven. Unittests en een GitHub Actions-workflow controleren het aantal items, de uniciteit van glyphs, de grootte en de SHA-256; geslaagde runs publiceren het binaire bestand en de samenvatting als workflowartefact `utf4088-million-catalogue`. Het gegenereerde binaire bestand wordt bewust buiten de Git-geschiedenis gehouden. UTF-4088 blijft experimenteel en is niet Unicode-compatibel.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Recente toevoegingen — Pi-constante, Modium/Registry-start, sleelactl-math en `@native`

Deze sectie legt functies vast die tijdens het werk van 2026-11 zijn toegevoegd, met de nieuwste groepen eerst. De gelinkte bronbestanden en hun validatiestatus zijn gezaghebbend; wijzigingen die nog worden beoordeeld, worden niet als samengevoegd beschreven voordat ze `master` hebben bereikt.

### De Pi-catalogusconstante met 100.000.000 cijfers

De repository bevat een geverifieerde waarde van π met **honderd miljoen cijfers** in [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (een correct gevormd decimaal getal, `3.14159…`), naast een waarde met 57.344 cijfers in [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Beide zijn onafhankelijk geverifieerd als echte π — zie [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md), dat de controles documenteert (het leerboekvoorvoegsel, het Feynman-punt op breukpositie 762, het beroemde miljoenste cijfer, overeenstemming met `lib/math/SLPiReference.sleela` en een frequentieanalyse van de cijfers over het hele bestand) en een reproductiescript in één stap.

### Modium™ / Registry™ laden de Pi-constante bij het opstarten van het subsysteem

Op de speciale ingangspunten — `sleela registry` en `sleela modium` (en de eerste uitvoering van het installatieprogramma), **niet** bij elke `sleela run` — laden de subsystemen Modium en Registry de grote π-catalogusconstante. Op een interactieve terminal tonen ze een tijdelijke rode regel `Loading Modium™` / `Loading Registry™` die ter plekke wordt gewist voordat de normale uitvoer begint; het effect is strikt aan een TTY gebonden, zodat omgeleide uitvoer en testuitvoer byte voor byte ongewijzigd blijven. De locator (niet de bytes) wordt gezaaid als de standaardwaarden `piReferenceLargePath` en `piReferenceLargeDigits` van de Registry-sleutel `Math`, zowel in het SLeeLa-model ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) als in de native seeder ([`registry/sleela_registry.c`](registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` stelt `largePath()` / `largeDigits()` beschikbaar. De π-constante wordt door **zowel** het standaard `make`-installatieprogramma als het begeleide "beautiful"-installatieprogramma geïnstalleerd op de SLeeLa-locatie van het besturingssysteem (`<data>/pi/`). Details: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — het "beautiful"-installatieprogramma en een door native code gedragen onderwerp `math`

- Het begeleide ("beautiful") installatieprogramma installeert nu `sleelactl` (het SLeeLa-besturingsvlak) en zijn modulregister, overeenkomstig het `make`-installatieprogramma. Zie [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- `sleelactl` kreeg een nieuw **`subject`**-type voor de native C/C++-onderwerpbibliotheken die in de runtime zijn gecompileerd, en registreert het eerste: **`math`**. `sleelactl run math` voert de native backend `__NativeMath` (`impl/subjects/math/math_native.cpp`) van begin tot eind uit via de referentiedemo van het onderwerp; `sleelactl info math` legt uit hoe men het bereikt (`import math;`). Het onderwerptype biedt alleen `run info` — er is bewust geen werkwoord `check`, omdat `sleela check` de door SHEET geregistreerde native onderwerpen niet laadt.

### `@native`, Native Drive en uitvoering in meerdere talen

Een nieuwe methodeannotatie **`@native`** vraagt om ahead-of-time native compilatie van een veelgebruikte methode. De nieuwe opdracht `sleela aot` vindt `@native`-methoden, controleert of elke methode binnen een ondersteunde *offloadbare deelverzameling* valt en genereert C/C++ — en **faalt gesloten** (weigert, met een nauwkeurige reden) bij elke constructie die nog niet kan worden vertaald, zodat nooit een half vertaalde methode ontstaat.

```sh
sleela aot --list <file.sleela>    # oordeel per methode: ok / REFUSED (reden)
sleela aot --emit <file.sleela>    # C/C++ (stdout) genereren voor een programma binnen de deelverzameling
```

`@native` blijft een verzoek om AOT-behandeling, waarbij niet-ondersteunde constructies vóór het genereren worden afgewezen. De [Native Drive](impl/native-drive/README.md) ondersteunt builds van C/C++-uitvoerbare bestanden en gedeelde bibliotheken uit meerdere eenheden via host-compilers (waaronder MSVC), ondersteunde SLeeLa-AOT-uitvoer, symboolinspectie tegen de ABI van de native loader, procesuitvoering zonder shell, afhandeling van stdin/stdout/stderr, meertraps-piping en XClass-delegatie aan de productieloader. `sleelactl` ontsluit zijn build-/run-opdrachten en leidt XClass-inspectie en het expliciet renderen en installeren van systemd-units door via `inspect native-drive`. De VM beheert een begrensde handletabel voor native modules met verplichte hostautorisatie en per generatie gecontroleerde handles. Syntaxis 3.0 voegt nu `nativeOpen`, `nativeCall` en `nativeClose` toe: append-only bytecodedispatch overschrijdt een afzonderlijk geversioneerde null/bool/int64/double/UTF-8-ABI, terwijl door de VM beheerde arrays, structs en andere samengestelde waarden binnen de interpreter blijven en een bytecode-terugval met dezelfde naam kunnen gebruiken. Plugins kunnen optioneel een geversioneerde host-callbacktabel ontvangen om exacte SLeeLa-functies aan te roepen via geneste VM-uitvoering, met GC-rooting van opgeschorte stacks, een expliciete toegestane lijst van host-callbacknamen en afwijzing van recursieve native API-aanroepen. Native code blijft vertrouwde code binnen hetzelfde proces, geen sandbox. Annotaties voor procesbeleid in de bron hebben een geversioneerd schema maar worden nog niet geparsed of afgedwongen. Details, beveiligingsgrenzen en het register van de zeven werkstromen: [Native Drive](impl/native-drive/README.md) en [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).

## XIX. Internationale README-edities

SLeeLa biedt landspecifieke README-ingangen voor de 50 markten van zijn internationale editie. Elk land heeft één bestand met de naam `README.<CC>.md`; waar meerdere talen relevant zijn, zijn ze gegroepeerd binnen het bestand van dat land.

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

**Vertaalbeleid:** Deze bestanden zijn aanvankelijke, gelokaliseerde landelijke ingangen. Hun inleidingen en taalkoppen zijn gelokaliseerd, terwijl de volledige technische referentie in de canonieke Engelstalige [README.md](README.md) blijft staan. Een landbestand mag niet als volledige vertaling worden beschreven totdat elke sectie is vertaald, technische identifiers en links zijn gecontroleerd en de editie taalkundig is nagekeken. De taaldekking in deze eerste ronde is representatief en beweert niet dat elke grondwettelijk erkende, regionale, inheemse of gebarentaal volledig is vertaald.

**Vertaalstatus:** De secties I–XIX zijn in het Nederlands vertaald. Technische identifiers, opdrachten, codevoorbeelden, bestandsnamen, links en adressen zijn behouden. Een definitieve taalkundige controle door moedertaalsprekers wordt aanbevolen.

---

### Aanvullende Nederlandstalige technische gids

#### 1. Bouwen en testen

Het bestand `SLEELA.syntax` is de primaire referentie voor de taalgrammatica. Houd de versie van de compiler gescheiden van de versie van de taalspecificatie: dit zijn afzonderlijke versies, en niet elke compileruitgave hoeft alle beschreven taalfuncties te ondersteunen.

Voer vanuit de hoofdmap van de repository de relevante bouw- en testopdrachten uit:

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

Veelgebruikte aanvullende Make-doelen zijn:

```sh
make all
make registry
make tests
make ballum
make help
make clean
```

Een geslaagde bouw van één onderdeel bewijst niet dat alle bibliotheken of platformen klaar zijn voor een release. Koppel testresultaten aan de Git-commit, het besturingssysteem en de gebruikte toolchain. Rapporteer overgeslagen of niet-uitgevoerde tests nooit als geslaagd.

#### 2. Installatie en bestandspaden

SLeeLa ondersteunt systeeminstallatie, installatie binnen het gebruikersaccount en staging voor pakketbouw:

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

Een systeeminstallatie gebruikt doorgaans paden zoals `/usr/local/bin`, `/usr/local/lib/sleela` en `/usr/local/share/sleela`. Met `PREFIX="$HOME/.local"` worden bestanden onder de gebruikersmap geplaatst; doorgaans is daarvoor geen `sudo` nodig. `DESTDIR` maakt een tijdelijke installatiestructuur voor pakketbouw en is geen rechtstreekse installatie op het hostsysteem.

Volg de [installatiehandleiding](install/README.md) en de platformspecifieke instructies. Verwijder geen bestanden die niet door het installatieproces als beheerde bestanden zijn geregistreerd.

#### 3. Compiler, runtime en uitvoeringsmodel

SLeeLa omvat een compiler, runtime, virtuele machines, bibliotheken en documentatiehulpmiddelen. De repository bevat daarnaast native C- en C++-code en SLeeLa-modules. Wanneer code de grens tussen native code en de runtime overschrijdt, moet het ABI-contract van de betrokken component worden gevolgd.

De gebruikelijke compilerketen bestaat uit lexicale analyse, parsing, semantische analyse, verlaging naar een intermediate representation, codegeneratie en uitvoering op de virtuele machine. Test elke fase afzonderlijk en test ook de grenzen tussen de fasen. Een parser-test alleen kan de correctheid van een volledig programma niet aantonen.

#### 4. API-contracten en modulegrenzen

Elke publieke API hoort haar invoer, uitvoer, mogelijke foutcondities, geheugenownership en neveneffecten te documenteren. Modulehandleidingen moeten precondities, postcondities en platformbeperkingen expliciet beschrijven.

Een symboolnaam of moduleverwijzing verleent op zichzelf geen toestemming om code uit te voeren. Autorisatie en uitvoeringsgrenzen moeten op de daarvoor bestemde grens worden gecontroleerd. Bij ongeldige invoer, ontbrekende resources of niet-ondersteunde functionaliteit moet de bewerking veilig falen.

#### 5. Regressietests en reproduceerbaarheid

De testsuite hoort zowel positieve als negatieve gevallen te omvatten: geldige bronbestanden, parseerfouten, typefouten, fouten bij IR-lowering, codegeneratie en uitvoeringsresultaten. Voor native integratie moeten ook ABI-compatibiliteit, foutafhandeling en het vrijgeven van resources worden getest.

Leg het besturingssysteem, de compilerversie, de toolversies en de Git-commit vast. Resultaten van herhaalde tests moeten vergelijkbaar zijn; overgeslagen tests moeten zichtbaar blijven in de rapportage. Raadpleeg het [testbeleid](markdown/TESTING.md) en de [platformondersteuningsmatrix](docs/PLATFORM-SUPPORT.md).

#### 6. Nederlandse terminologie en onderhoud

Behoud bestandsnamen, identifiers, opdrachten, paden, URL's, codevoorbeelden en API-namen exact. Vertaal de toelichting, niet de tekenreeksen die door de compiler of shell moeten worden herkend. Gebruik consistente termen voor compiler, runtime, virtuele machine, module en API. Wanneer een Nederlandse vakterm niet eenduidig is, vermeld dan bij het eerste gebruik ook de Engelse technische term.

Vertaling is doorlopend onderhoud. Vergelijk deze editie met de canonieke README wanneer die verandert en werk de overeenkomstige onderdelen bij. Deze aanvullende gids maakt de Nederlandse editie inhoudelijk uitgebreider, maar vervangt geen volledige vertaling van alle bestaande secties of een taalkundige eindcontrole door een moedertaalspreker.

---

## Taalstatus België

De Duitse, Franse en Nederlandse secties bevatten de volledige vertaling van de technische README (secties I–XIX); de Nederlandse sectie is aangevuld met regionale richtlijnen. Een definitieve taalkundige controle door moedertaalsprekers wordt aanbevolen.
