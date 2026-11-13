<img align="right" src="images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — Schweiz (CH)

## Deutsch

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Eine native Systemsprache mit Compiler, Laufzeitumgebung und Ökosystem virtueller Maschinen.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Schweizer Sprachfassung.** Vollständige deutsche technische Übersetzung für die deutschsprachige Schweiz. Technische Bezeichner und Code bleiben unverändert.

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

# SLeeLa 3.0 — Suisse (CH)

## Français

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Un langage système natif, son compilateur, son environnement d’exécution et son écosystème de machines virtuelles.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Édition suisse en français.** Traduction technique complète destinée aux lecteurs francophones de Suisse. Les identifiants techniques, le code, les commandes et les liens restent inchangés.

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

# SLeeLa 3.0 — Svizzera (CH)

## Italiano

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Un linguaggio nativo per sistemi, con compilatore, ambiente di esecuzione ed ecosistema di macchine virtuali.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Edizione svizzera in italiano.** Traduzione tecnica completa per i lettori italofoni della Svizzera. Identificatori tecnici, codice, comandi e collegamenti restano invariati.

**The Now. The Ingredient.** Un linguaggio di programmazione è più della sua sintassi: comprende il compilatore, l’ambiente di esecuzione, le librerie, gli strumenti e i contratti di piattaforma che rendono il software concretamente utilizzabile.

**Aree di lavoro di SLeeLa.** SLeeLa affronta **Math**, **Object**, **Science** e **Reach**, le quattro dimensioni del suo lavoro: ciò che è calcolabile (Math), l’oggetto definito nel catalogo di ciò che esiste (Object), lo studio disciplinato del vero (Science) e l’estensione delle capacità di un sistema verso altri sistemi e persone (Reach). **Gli interessi nazionali restano validi**: nessuna di queste quattro dimensioni li sostituisce. Quando un argomento riguarda gli interessi di una nazione, tale relazione viene mantenuta e documentata fedelmente, ad esempio nei campi di metadati di una classe `metaOrigin` e `metaNationalSchedule`.

## I. Per iniziare

- [Compilare l’implementazione nativa](impl/README.md)
- [Macchine virtuali e Modium — guida pratica per l’utente](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [Stack CPU nel codice sorgente SLeeLa](lib/cpu/CPU.md): una catena completa, dalle porte logiche all’ALU, ai core, al sistema operativo e ai programmi; comprende oltre 130 profili CPU e [tensioni, frequenze variabili e parametri avanzati di temporizzazione](lib/cpu/TIMING.md), con esecuzione [secondo specifica oppure in overclock](lib/cpu/components/README.md).
- [Indice della documentazione](docs/README.md)
- [Fonte dati CSPML e riferimento di ricerca Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md): provenienza e indicazioni d’integrazione per dati esterni statunitensi su località, condizioni socioeconomiche, forza lavoro, istruzione e scuole pubbliche.
- [Documentazione API generata](docs/api/README.md): riferimento HTML corrente, versione dell’API e istruzioni per rigenerarla.
- [Catalogo API](api/README.md)
- [Standard di confronto computazionale](comparison/README.md): standard per tipi, memoria, ABI e conformità di SLeeLa VM, Java/JVM e C/C++ nativo.
- [Stanchion della ricorsione — prototipi C e C++](recursion/stanchion/README.md): esempi specializzati per i tipi primitivi Boolean, Byte, Character, Integer, Long, Float e Double, oltre a modelli di ricorsione generici, applicati, scientifici e didattici.
- [Matrice di supporto e convalida delle piattaforme](docs/PLATFORM-SUPPORT.md)
- [Guida del marchio](docs/BRAND-GUIDE.md)
- [Concetti e risorse grafiche per il merchandising](merchandise/README.md)
- [Logo NitroWebExpress](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Politica di test e criteri di rilascio](markdown/TESTING.md)

## II. Novità

Aggiunte recenti al linguaggio, ai driver e alla libreria standard:

- **Pacchetto della libreria standard `advantage` — ottimizzazione di un dividendo con matematica, fisica, integrazione, valutazione e output instradabile.** [`lib/advantage/`](lib/advantage/README.md) racchiude l’*ottimizzazione di un dividendo unico rispetto a un modello* in una struttura a strati **Sequitur onion** (dividendo → revisione → questioni → matematica → aggiunte → sottrazione → raggruppamento → iterazione), percorribile verso l’interno e verso l’esterno con aggiornamento dinamico e restituzione completi, tramite un semplice **modello utente** e un **modello sviluppatore** a controllo completo. Comprende cinque aree: **`math/`**, economia del commercio bilaterale (massimizzazione congiunta del profitto, negoziazione di Nash, rapporto critico bilaterale del modello newsvendor); **`physics/`**, una «fisica dei campi di fiducia» applicata all’economia strutturale (fiducia come tensore metrico, geodetica transazionale, struttura di contatto degli annunci e SDE CIR con salti per il decadimento della fiducia, esplicitamente indicata come analogia di modellazione); **`integration/`**, modello di integrazione verticale componibile ricorsivamente per capitalismo e vendita al dettaglio (compartimenti, baie di ingegneria, compartimenti di prova, vassoi di mediana newtoniana e punteggio di sofisticazione del sistema); **`grading/`**, valutatore di circuiti di sofisticazione che assegna valutazioni A–D a ciascun modulo e a insiemi di moduli vincenti, perdenti, valutatori o assoluti; e **`output/`**, interfaccia di emissione instradabile verso terminale, superficie in formato **CommonRails** o socket/connettore (predisposta per un `slnet_write` nativo). Ogni classe contiene il record Metadata-Class; cinque demo eseguibili convalidano le aree. Documentazione complementare: [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md), [`MATH.md`](lib/advantage/MATH.md) e [`PHYSICS.md`](lib/advantage/PHYSICS.md). Un consumatore nativo C/C++ in [`advantage/`](advantage/README.md) — nucleo C `advantage_value`/`advantage_mean`, archivio di conoscenza XML e libreria sicura di acquisizione HTTPS/SFTP/SSH, keystore e trasporto DH/ECDH — collega il modello a [esempi commerciali](advantage/examples/README.md): transazioni positive da presunte fonti Internet, una sequenza di rilevanza per Traders, Consumers, Helpers e Healers e otto modelli classificati da A a D (D = massimo Cement, A = massimo Brand).
- **Annotazioni temporali dei cicli — `@loopDelay` / `@loopBudget` e funzione integrata `sleep(ms)`.** Sono le prime annotazioni **comportamentali a livello di istruzione** di SLeeLa: scritte su una riga autonoma prima di un ciclo `for`, `while` o `do`, inseriscono una pausa reale nell’esecuzione. `@loopDelay(ms = 250)` / `@loopDelay(seconds = 2)` attende una quantità fissa per iterazione (la forma abbreviata `@loopDelay(250)` usa i millisecondi per impostazione predefinita); `@loopBudget(seconds = 10, iterations = 1000)` distribuisce un numero noto di iterazioni su un budget totale di tempo reale. Entrambe vengono abbassate al nuovo opcode append-only **`OP_SLEEP`** (ISA 157), che supporta anche la funzione integrata generale **`sleep(<ms>)`**, richiamabile ovunque. La pausa è implementata in modo portabile tramite `nanosleep` / `Sleep`. Specifica: [`SLEELA.syntax` §32](SLEELA.syntax); riferimento API: [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md); demo eseguibile: [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela). L’indice delle aggiunte di sessione — che include anche la moltiplicazione nativa di grandi interi `OP_BIGMUL` e gli strumenti autonomi [local-generator](local-generator/README.md) e [pi-generator](pi-generator/README.md) — si trova in [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md). Le tre superfici API generate ([`sleeladoc-api/`](sleeladoc-api/README.md), [`docs/api/`](docs/api/README.md) e lo snapshot immutabile [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html)) sono state ricostruite con `make api-docs` e ora includono le voci `SLISASleep` / `SLISABigMul` nella pagina del pacchetto `vm`.
- **SleelaDoc dalla riga di comando — `sleela docs`.** SleelaDoc è ora disponibile direttamente dal terminale. `sleela docs` elenca tutti gli argomenti documentabili; `sleela docs crypto` mostra un pacchetto; `sleela docs stringbuilder` risolve una classe senza distinzione tra maiuscole e minuscole e riconosce il prefisso standard `SL` come `SLStringBuilder`. Visualizza una pagina dettagliata con **definizione, finalità progettuale, relazioni, struttura, user clays e una scala di competenza a cinque livelli**, oltre a suggerimenti ordinati «intendevi?» per nomi simili. Gli autori arricchiscono le pagine aggiungendo tag SleelaDoc `@` come `@intent`, `@thesis`, `@axiom`, `@param`, `@see` e `@mastery`. `sleela docs --generate [dir]` genera un corpus in testo semplice (una pagina per classe e un indice `TOPICS.index`) che la CLI può leggere offline; **`make sleeladoc`** raggruppa il corpus e i corrispondenti sorgenti `.sleela` in `sleeladoc/build/sleeladoc.jar`. Per argomenti con la sola facciata, come `psychiatry` e `politico`, `sleela docs --facade <topic>` esplora il filesystem durante l’esecuzione ed elenca le cartelle effettive e poi i file, con limiti configurabili dall’utente di **6 MB, 6000 file e 6 minuti**. Le unità di `lib/sleeladoc` adottano l’edizione corrente **`#sleela 3.0`**. Vedere [`man/man1/sleela.1`](man/man1/sleela.1) e [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md).
- **SleelaDoc — documentazione in stile Javadoc con sito HTML generato.** **SleelaDoc** è la convenzione per i commenti documentali nel sorgente SLeeLa (commenti tra parentesi angolari delimitati da `@doc` / `@page`; vedere [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)). [`lib/sleeladoc`](lib/sleeladoc/README.md) è l’**estrattore** che trasforma un’unità `.sleela` in una pagina API Markdown o HTML, come `javadoc` per Java, documentando l’*intento*. Un sito HTML completo e navigabile per l’intera directory `/lib` viene generato in [`/sleeladoc-api`](sleeladoc-api/README.md) da `tools/generate-sleeladoc-api.py` (aprire `sleeladoc-api/index.html`); utilizza uno stile pulito blu, azzurro, grigio chiaro e bianco, una gerarchia pacchetto → classe → membro e un glossario per il lettore *Student 2*. Il compilatore produce anche, come risultato collaterale della compilazione, documenti contrassegnati dall’autore (`--wiki`: pagina MediaWiki e copia gemella del sorgente originale), descritti in [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md). Demo eseguibile: [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela).
- **Modello a oggetti del documento XML (`lib/xml`).** DOM XML lato SLeeLa: un documento possiede un albero di nodi elemento/testo/commento/CDATA/istruzione di elaborazione, con figli ordinati e attributi denominati; include un serializzatore compatto o leggibile (escaping XML 1.0 §2.4), un parser basato su celle di caratteri e il registro di qualità `SLXmlAspect`. L’uguaglianza dei nodi segue tre livelli di fiducia (identità → stesso soggetto → struttura) ed è riflessiva, simmetrica e deterministica. Vedere [`lib/xml/README.md`](lib/xml/README.md) e l’esempio eseguibile [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela).
- **Primitive intere con segno a larghezza estesa (`long`, `int64`, `int128`).** `int`, `long` e `int64` sono interi con segno a 64 bit e vengono verificati dal sistema dei tipi ed eseguiti in modo identico. `int128` è accettato come tipo intero (per ora basato su 64 bit; il vero supporto a 128 bit è documentato come lavoro futuro). Le serie con segno in base 2 da `int32` a `int16777216` (sintassi 2.7) e senza segno da `uint32` a `uint16777216` (sintassi 2.8) estendono queste capacità. Vedere [`SLEELA.syntax`](SLEELA.syntax) §21, §24 e §25.
- **Campi conseguenti `@mix` (sintassi 2.9).** Una classe può dichiarare più campi di istanza con lo **stesso nome** quando ogni ripetizione include `@mix`. Il linguaggio interpreta il gruppo come sequenza indicizzata (array/lista), dall’alto verso il basso `1,2,3,…` per impostazione predefinita o manualmente tramite `@mix(1,2,3,...)`; il numero di indici deve corrispondere al numero di campi. Vedere [`SLEELA.syntax`](SLEELA.syntax) §28.
- **Marcatori documentali — il Processo Naturale, denominato Preonems (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).** Queste parole nude che fungono da marcatori documentali sono chiamate collettivamente **Preonems** (un marcatore è un *Preonem*; l’insieme è costituito dai *Preonems*; vedere [`SLEELA.syntax`](SLEELA.syntax) §26.1). Sono marcatori dichiarativi di prima classe che indicano che cosa *è* un documento nell’ordine della sua creazione: `author`/`authorial` (sintassi 2.4; gestito dal consiglio professionale, l’unico membro con effetto comportamentale), `material;` (Source stabilita) e `memory;` (sintassi 2.5), `determine;` (sintassi 2.6; documento aperto a ulteriore elaborazione da parte di un dottore, medico o ghost fino al completamento per via legale), e i due più recenti **`object;`** (cosa stabilita nel catalogo di ciò che esiste, reificata e dotata di consistenza concreta in sintassi 2.8) e **`field;`** (ambito relazionale in cui gli oggetti esistono e si muovono), entrambi abilitati dalla sintassi 2.6. Ogni marcatore è contestuale e vincolato alla versione nel frontend; non modifica parsing, tipizzazione o generazione del codice. Fuori dalla posizione di marcatore ogni parola resta un identificatore normale; pertanto `field` non interferisce con un membro di classe o con l’accesso `.field`. Vedere [`SLEELA.syntax`](SLEELA.syntax) §18–§20, §23 e §26.
- **Nordshrift in sintassi 3.0 con accesso completo alla libreria.** Nordshrift risolve i nomi tramite lo stesso indice ricorsivo `/lib` usato dal compilatore, raggiungendo ogni pacchetto e unità `.sleela`; `nordshrift check` riporta i conteggi dell’intero albero.
- **Stanchion della ricorsione.** [`recursion/stanchion/`](recursion/stanchion/README.md) include ora prototipi di riferimento C e C++ dedicati ai sette contratti primitivi documentati, oltre a esempi generici, applicati, scientifici e didattici. Sono sorgenti di riferimento: compilarle e testarle prima di considerarle verificate.
- **Commenti tra parentesi angolari (`<` / `<<`).** Un nuovo stile di commento per sviluppi futuri — `< line >` e `<< block >>` — riconosciuto dal frontend SLeeLa e documentato in [`SLEELA.syntax`](SLEELA.syntax) §1.1.
- **Letterali interi con base.** I letterali `int` accettano ora forme esadecimali (`0x`), binarie (`0b`) e ottali (`0o`), con separatori di cifre `_`, sia nel linguaggio sorgente SLeeLa ([`SLEELA.syntax`](SLEELA.syntax) §1.4) sia nel formato dei modelli CPU [`.xlt`](lib/xlt/README.md).
- **Standard Metadata Class nell’intera libreria.** Il metapattern dei metadati descrittivi (provenienza, legalità, ambito giurisdizionale) è ora presente in ogni file `.sleela` contenente classi sotto `/lib`. Vedere [`SLEELA.syntax`](SLEELA.syntax) §15 e [la lezione del tutorial](tutorial/13-metadata-class.md).
- **Famiglia Text / String ampliata.** `lib/text` aggiunge helper effettivi nel linguaggio, una vista a celle di caratteri con operazioni reali `charAt`, `substring`, `indexOf`, `reverse` e allineamento delle colonne, oltre a un motore di template posizionale. Vedere il [tutorial Text e String](tutorial/14-text-and-strings.md) e la demo eseguibile [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela).
- **`String` di prima classe nei fogli Nordshrift.** Un foglio `.sst` può dichiarare un blocco tipizzato `strings:` che contribuisce al manifesto dei componenti della compilazione. Vedere il [riferimento Nordshrift Sheet](wiki/Nordshrift-Sheet-Reference.md).
- **Modelli CPU eseguiti tramite opcode ed equivalenza delle chiamate di funzione.** Ogni `SL<ARCH>CPU` eredita la capacità di eseguire un flusso di istruzioni native tramite il ciclo fetch-decode-execute e anche il codice di un altro processo ipotizzato mediante chiamate equivalenti sull’infrastruttura VM condivisa. Vedere [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5.

## III. Compilazione e test

**Prerequisiti:** una toolchain C/C++ (GCC o Clang) e GNU Make; Python 3 per gli strumenti relativi a manifesti e flussi di lavoro; su Linux, anche i pacchetti di sviluppo X11/Xrandr per i prodotti di visualizzazione. Il comando `make deps` di ciascun modulo segnala o installa le dipendenze necessarie. La compilazione in `impl/` si interrompe in sicurezza se il manifesto SHA-256 attendibile non viene rispettato; il Makefile principale lo richiama automaticamente.

Il modo più rapido per iniziare da un clone pulito:

```sh
make                                  # compilare tutto dalla radice del repository
```

Oppure compilare e testare soltanto il nucleo nativo:

```sh
make -C impl all                      # compilare soltanto il nucleo del compilatore e del runtime
bash ./test-suites/run-all.sh --all   # eseguire l’intera suite di test
```

Il comando `make` nella radice del repository compila tutti i prodotti (core, VM, codec, churn, antivirus e così via) e termina creando il **registro** e materializzandone la tabella di base. Un albero di lavoro compilato include quindi fin dall’inizio un registro verificabile.

Obiettivi principali:

```sh
make                     # compilare tutto (alias di make all; il registro viene creato per ultimo)
make all                 # stessa compilazione completa, indicata esplicitamente
sudo make install        # installare comandi e file di supporto del runtime (vedere § IV)
make clean               # rimuovere gli output di compilazione di tutti i sistemi integrati
make help                # mostrare tutti gli obiettivi del Makefile principale
```

Altri obiettivi utili:

```sh
make registry            # compilare solo il registro e materializzare la tabella
./build-registry.sh      # stessa operazione tramite script diretto
make tests               # compilare ed eseguire la suite di test
make ballum              # compilare la libreria di visualizzazione Ballum per questo sistema operativo
bin/install-modules      # installare/compilare i moduli in modo scaglionato per circa 10–20 minuti
```

L’obiettivo `registry` esegue [`build-registry.sh`](build-registry.sh): compila il nucleo del registro, inserisce la tabella di base nell’archivio testuale portabile `registry/build/sleela.sleelareg` ed esporta lo script della tabella MySQL `registry/build/registry.sql` (tramite `sleela registry export`). Il registro può essere esaminato con `sleela registry` oppure `registry/build/sleela-registry review`. Consultare la [guida al registro](registry/README.md) e l’[architettura Registry 3.0](registry/REGISTRY-3.0-ARCHITECTURE.md).

Per i prerequisiti e le procedure specifiche di ciascuna piattaforma, fare riferimento alle guide di compilazione e test. La compilazione riuscita di un componente non dimostra che ogni libreria o piattaforma sia pronta per il rilascio.

## IV. Installazione

Dopo aver compilato i prodotti nativi, installare i comandi e i file di supporto del runtime:

```sh
sudo make install
make install PREFIX="$HOME/.local"        # installazione per il solo utente, senza sudo
make install DESTDIR="$PWD/package-root" # installazione in una directory di staging del pacchetto
make install-check
```

L’installer convenzionale utilizza per impostazione predefinita `/usr/local/bin`, `/usr/local/lib/sleela` e `/usr/local/share/sleela`. Mantiene la configurazione di avvio esistente e non sovrascrive comandi preesistenti non tracciati dal repository. La disinstallazione rimuove soltanto i file registrati dall’installer; configurazione e dati runtime non tracciati vengono conservati.

Per una distribuzione guidata e mirata del solo pacchetto principale — i binari `sleela`, `sleelvac` e `nordshrift`, la libreria standard, i protocolli e i moduli di supporto — utilizzare l’installer pulito:

```sh
make clean-install       # installazione guidata del solo pacchetto principale
```

In primo luogo viene chiesto se installare il pacchetto principale; successivamente si sceglie l’ambito dell’installazione. In caso di esito positivo, può essere proposto un selettore facoltativo di moduli aggiuntivi selezionati. Il flusso interattivo completo e tutte le opzioni sono descritti in [Installazione rapida e sicura](install/README.md).

## V. Linguaggio e runtime

La grammatica normativa si trova in [`SLEELA.syntax`](SLEELA.syntax). Il repository documenta attualmente la sintassi **3.0**; la versione del compilatore/degli strumenti e la versione della sintassi del codice sorgente sono distinte. La superficie 3.0 aggiunge i marcatori documentali **`pure` e `gasoline`** (§30): due Preonem contestuali e dichiarativi, privi di effetti di esecuzione autonomi, che esprimono ciascuno la posizione di un documento attraverso la sua stessa onestà. La sintassi 3.0 riunisce le superfici linguistiche dalla 1.0 alla 2.9 in un’unica edizione normativa corrente, mantenendo i requisiti di versione minima per ciascuna funzionalità.

La precedente superficie 2.9 aggiunge i **campi sequenziali `@mix`** (§28): una classe può dichiarare più campi di istanza con lo stesso nome se ciascuno è contrassegnato con `@mix`. Sono trattati come un gruppo indicizzato (in ordine crescente per impostazione predefinita: `1,2,3,…`, oppure definito manualmente tramite `@mix(1,2,3,...)`; il numero di indici deve corrispondere al numero di campi). Sono inoltre incluse le famiglie di interi binari estesi con e senza segno, da `int32` a `int16777216` (§24) e da `uint32` a `uint16777216` (§25), che estendono `long`, `int64` e `int128` (§21); i documenti di supporto per le parole chiave (§22); e i **Preonem**: marcatori documentali dichiarativi `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) e `artifact` (§35; uno stato di qualità legislativa valutato in relazione al tempo e allo spazio). Nel loro insieme sono denominati Preonem (§26.1).

La risoluzione degli overload dei costruttori e la delega ai costruttori `this(...)` e delle classi base sono esplicitamente escluse dal contratto di sintassi 1.8 verificato. Prima di fare affidamento su una funzionalità, consultare la grammatica e le note d’implementazione. Le nove unità sorgente SleelaDoc dichiarano ora `#sleela 3.0`; il controllo comune del compilatore accetta sintassi sorgente dalla 1.0 alla 3.0. La nuova documentazione API viene generata con `make api-docs`.

Il repository contiene un’implementazione nativa C/C++, riferimenti per la generazione SLVM, moduli della libreria standard, test, esempi e integrazioni di piattaforma. Lo stato di implementazione e convalida varia da modulo a modulo. Consultare la specifica pertinente e i risultati CI correnti, senza presumere un livello uniforme di completezza.

## VI. Principi di progettazione

- **Prove prima delle affermazioni:** distinguere chiaramente ciò che è implementato, testato, sperimentale e pianificato.
- **Un contratto per ogni confine:** documentare sintassi, API, formati dei file, presupposti di piattaforma e comportamento in caso di errore.
- **Portabilità fin dalla progettazione:** isolare i backend specifici delle piattaforme e testarli su runner nativi.
- **Gestione sicura degli errori:** verificare input malformati, limiti delle risorse, errori dei processi e percorsi di pulizia.
- **Release riproducibili:** ricostruire da un ambiente pulito e pubblicare hash degli artefatti e note di rilascio.

## VII. Build antivirus e integrità

Il pacchetto antivirus Sleelavirin comprende uno scanner SHA-256 nativo, un adattatore ClamAV facoltativo, manifesti di integrità per sorgenti e indici e checksum per i pacchetti di piattaforma. Compilare e testare il pacchetto dalla radice del repository:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Il manifesto di integrità viene generato in `antivirus/build/`. Comprende input di codice sorgente e documentazione tracciati dal repository, oltre ai file protetti degli indici delle firme; esclude gli output generati e la cache ClamAV modificabile. Gli snapshot dei database ClamAV dispongono di manifesti propri. Gli archivi ZIP di rilascio includono `INTEGRITY-MANIFEST.json`, `SHA256SUMS` e un file `.sha256` associato. Confrontare gli hash degli archivi scaricati con un valore attendibile ottenuto indipendentemente: un checksum memorizzato accanto a un archivio non costituisce una firma dell’editore. Consultare la [guida Sleelavirin](antivirus/README.md), la [lista di controllo di sicurezza](antivirus/SECURITY.md) e il [workflow di build antivirus](.github/workflows/antivirus-binaries.yml).

Una release è considerata verificata soltanto quando i job CI pertinenti hanno esito positivo per l’esatto commit della release e gli artefatti risultanti sono stati controllati.

## VIII. Temi dell’editor SLeeLa — Vim ed Emacs

SLeeLa include un insieme coordinato di schemi cromatici scuri e professionali per **Vim** ed **Emacs**, oltre a un’utilità nativa per le palette C/C++. Lo schema originale **American Professional** rimane quello predefinito. Dieci palette complementari offrono ispirazioni progettuali culturali e istituzionali diverse; sono interpretazioni grafiche, non rappresentazioni ufficiali di alcuna nazione, cultura, corte o istituzione.

### Gli undici schemi e i relativi colori

| Schema | Sfondo | Primo piano | Parole chiave | Tipi | Stringhe | Commenti |
|---|---|---|---|---|---|---|
| **American Professional** | `#101820` | `#F4F7FA` | `#7FB3FF` | `#73DACA` | `#F2C078` | `#8A96A3` |
| **Pan-Eurasian** | `#111827` | `#F0F2F5` | `#7AA7E8` | `#69C9B3` | `#D7B56D` | `#8995A5` |
| **Russian and Slavic — Medium** | `#202733` | `#E8EAF0` | `#A9C0DD` | `#8FC4B2` | `#D6BD8B` | `#9AA3AF` |
| **Central Asian / Silk Road** | `#1C2529` | `#F5F0E5` | `#7FADE0` | `#45C6C2` | `#D9B56D` | `#92A3A0` |
| **East Asian** | `#171A1C` | `#F2EFE7` | `#D97878` | `#72C7A5` | `#D8B76A` | `#969F9B` |
| **ICC — Corte penale internazionale** | `#0E1A2B` | `#EAF0F7` | `#5B8FD6` | `#7FB7C9` | `#D9B25A` | `#7E8CA0` |
| **SCOTUS — Corte Suprema degli Stati Uniti** | `#1A1416` | `#F3EEE6` | `#9E2B35` | `#8FA9B8` | `#C9A24B` | `#8C8178` |
| **Greek (Hellenic)** | `#0A1A30` | `#FFFFFF` | `#2E7BD6` | `#5E9FE0` | `#9FC3EC` | `#6E86A6` |
| **Turkish (Al Bayrak)** | `#2A0609` | `#FFFFFF` | `#F23D48` | `#F0A9AE` | `#F0C9A9` | `#A8767A` |
| **Kiro** | `#0B0B12` | `#F5F5FA` | `#A98EDA` | `#6E8BE0` | `#E8C547` | `#6A6482` |
| **SLeeLa** | `#08080E` | `#FFFFFF` | `#B07CFF` | `#5A7CFF` | `#FFD21E` | `#6E5F9E` |

### Colori delle bandiere nazionali (palette di origine)

Gli schemi greco e turco derivano dai **colori ufficiali delle bandiere nazionali** dei due paesi, qui riportati con i valori esadecimali pubblicati. Gli sfondi e gli accenti degli schemi sopra sono interpretazioni scurite e adatte agli editor; la tabella seguente conserva i colori di riferimento esposti a livello nazionale.

| Paese | Colore | Esadecimale | Fondamento e significato |
|---|---|---|---|
| **Grecia** | Blu | `#0D5EAF` | Blu della bandiera nazionale (legge 851/1978). Il cielo e i mari; in araldica, *azzurro*. |
| **Grecia** | Bianco | `#FFFFFF` | Bianco della bandiera nazionale. Purezza della lotta per l’indipendenza; la croce rappresenta il cristianesimo ortodosso orientale. |
| **Türkiye** | Rosso (*al bayrak*) | `#E30A17` | Rosso della bandiera nazionale (legge turca sulla bandiera). Il campo; per tradizione, il sangue dei martiri e la sovranità ottomana. |
| **Türkiye** | Bianco | `#FFFFFF` | Bianco della bandiera nazionale. La mezzaluna e la stella: pace, purezza e simbolo dell’eredità turco-islamica. |

Gli schemi utilizzano il **bianco** nazionale come primo piano e il **blu** nazionale esatto (`#0D5EAF`) o il **rosso** (`#E30A17`) per parole chiave, cursore, selezione e barra di stato. I colori più importanti sono quindi letteralmente quelli nazionali. Si tratta di palette interpretative; il riferimento ai colori delle bandiere ne documenta la provenienza e non costituisce una rappresentazione ufficiale di nessuno dei due Stati. I significati cromatici seguono la descrizione dei simboli statali del Ministero degli Affari Esteri della Repubblica Ellenica e la tradizione della legge turca sulla bandiera.

### Tabella dei campioni cromatici

Le celle della palette utilizzano **campioni SVG**, così GitHub mostra i colori effettivi invece di celle HTML vuote. Ogni striscia presenta sei colori in questo ordine: sfondo, primo piano, parole chiave, tipi, stringhe e commenti. I codici esadecimali esatti sono indicati accanto a ogni campione.

| Schema | Palette visualizzata | Codici esadecimali (sfondo · primo piano · parole chiave · tipi · stringhe · commenti) |
|---|---|---|
| **American Professional** | ![Palette American Professional](lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Palette Pan-Eurasian](lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russian and Slavic — Medium** | ![Palette Russian and Slavic Medium](lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Central Asian / Silk Road** | ![Palette Central Asian Silk Road](lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **East Asian** | ![Palette East Asian](lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC** | ![Palette ICC](lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS** | ![Palette SCOTUS](lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Greek (Hellenic)** | ![Palette greca ellenica](lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turkish (Al Bayrak)** | ![Palette turca Al Bayrak](lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Palette Kiro](lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![Palette SLeeLa](lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

Ulteriori colori di accento sono utilizzati per funzioni, costanti, selezioni, diagnostica e direttive del preprocessore. La palette Russian and Slavic utilizza intenzionalmente tonalità medie di ardesia, blu, salvia, rubino e oro antico per un aspetto professionale più tranquillo.

### Installare gli schemi Vim

Dalla radice del repository:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Aggiungere quanto segue a `~/.vimrc` per abilitare l’evidenziazione della sintassi SLeeLa e scegliere uno schema:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Scegliere esattamente uno dei seguenti nomi:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Ad esempio, usare `colorscheme sleela-russian-slavic` per la palette russa e slava di intensità media, oppure `colorscheme sleela-greek` / `colorscheme sleela-turkish` per le palette ellenica e Al Bayrak. Riavviare Vim oppure ricaricare la configurazione con `:source $MYVIMRC`.

### Installare gli schemi Emacs

Creare una directory locale per i temi e copiare i file della modalità e dei temi dalla radice del repository:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Aggiungere questo a `~/.emacs` o `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Sostituire `sleela-american` con `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` o `sleela-sleela` per selezionare un altro schema. Se Emacs chiede se considerare attendibile il tema, esaminare il file locale del tema prima di accettare.

### Anteprima delle palette native C/C++

La piccola utilità nativa per le palette può elencare gli schemi, stampare i relativi colori del terminale e cercare un singolo colore:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Per compatibilità con le versioni precedenti, `--palette` e `--hex COLOR` senza `--theme` selezionano American Professional. I risultati di build e test devono essere verificati sul sistema di destinazione e in CI; documentare un comando non dimostra che sia stato eseguito con successo.

Vedere la [guida all’integrazione degli editor](linux-editors/README.md) per le descrizioni complete delle palette e i dettagli dell’utilità.

## IX. Contribuire

1. Leggere l’indice della documentazione e la politica di test.
2. Apportare modifiche mirate, accompagnate da test positivi e negativi.
3. Eseguire prima i test mirati e poi la suite completa.
4. Indicare i dettagli del sistema operativo e della toolchain; non descrivere i controlli saltati come superati.

SLeeLa è sviluppato da **MEARVK LLC**. Il materiale storico della pagina iniziale è conservato in [`docs/README-LEGACY.md`](docs/README-LEGACY.md).

## X. AE6E66 — Aspettativa di prelievo sulle email pubbliche Round

Per l’iniziativa AE6E66, l’aspettativa dichiarata del progetto è di **300.000.000,00 USD per ogni email pubblica Round**, come importo di prelievo fiscale proposto associato a ciascuna email Round Public idonea.

Questa dichiarazione registra soltanto un’aspettativa del progetto. **Non** costituisce un accertamento fiscale legale, un ordine governativo, una sentenza o una passività fiscale accertata in modo indipendente. Qualsiasi prelievo effettivo richiederebbe la pertinente autorità giuridica, giurisdizione, definizioni, procedura e determinazione dell’autorità governativa competente.

## XI. Runtime di alfabeti e set di caratteri

La documentazione dell’alfabeto SLeeLa e il runtime nativo dei caratteri sono mantenuti in `alphabet/sleela/` e `lib/alphabet/native/`. I test nativi C++17 coprono la decodifica/codifica rigorosa degli scalari UTF-8, le firme di capacità `C<n>` e l’appartenenza limitata ai set di caratteri:

```sh
make -C lib/alphabet/native test
```

Il catalogo sperimentale UTF-4088 e il relativo adattatore di ricerca SLeeLa sono documentati in [`utf-4088/SLeeLa-ALPHABET-INTEGRATION.md`](utf-4088/SLeeLa-ALPHABET-INTEGRATION.md). Gli ID UTF-4088 appartengono a uno spazio dei nomi separato e non sono valori scalari Unicode. Il lavoro di binding del compilatore e di SLVM è tracciato separatamente da queste basi native.

## XII. AutoCAD Creator™

[**AutoCAD Creator™**](autocad/creator/README.md) è un’applicazione CAD 2D interattiva scritta interamente in SLeeLa. Mentre il relativo AutoCAD Renderer trasforma un piano in un disegno in un solo passaggio, Creator consente di **creare e modificare** dal vivo un documento AutoCAD 2D: aggiungere pareti, stanze, quote e note, quindi spostare, rinominare, riassegnare livelli, eliminare e annullare gli elementi inseriti, e **salvare** il risultato come vero disegno DXF AutoCAD (testo R12, apribile in AutoCAD / LibreCAD).

La sua interfaccia grafica è basata su **SleelaUI™**, il toolkit UI multipiattaforma di SLeeLa (in [`lib/user-interface`](lib/user-interface)), non su JavaFX, e apre una vera finestra nativa su X11 / Cocoa / Win32. Il design segue una regola: **«SleelaUI presenta; SLeeLa decide»**. La GUI non calcola la geometria e non scrive DXF. Ogni strumento invia un’intenzione di creazione, modifica, eliminazione, annullamento o salvataggio al componente autorevole [`SLCadCreator`](lib/autocad/SLCadCreator.sleela), che possiede il documento modificabile e una cronologia reversibile dei comandi; la tela visualizza soltanto un’anteprima della geometria posseduta da Creator.

Il motore di modifica riutilizzabile si trova in [`lib/autocad`](lib/autocad): [`SLCadEntity`](lib/autocad/SLCadEntity.sleela), [`SLCadDocument`](lib/autocad/SLCadDocument.sleela), [`SLCadCommand`](lib/autocad/SLCadCommand.sleela) e [`SLCadCreator`](lib/autocad/SLCadCreator.sleela). Funziona end-to-end senza interfaccia grafica. Il guscio GUI e la demo si trovano in [`autocad/creator`](autocad/creator):

```sh
make creator          # da autocad/: esegue la demo Creator -> out/altered.dxf
```

Su un sistema con display si apre la finestra di modifica dal vivo; su un host headless il disegno viene creato in memoria, viene stampato un rapporto insieme al DXF e il processo termina correttamente.

## XIII. Ordinamento della mappa di input UTF-4088

Il catalogo specializzato UTF-4088 definisce il percorso spaziale della mappa di input e le iterazioni astratte di pressione in [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). La politica dell’indice per materia del National Lien Register è in [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Le coordinate della mappa, i livelli di pressione (1–50), gli ID permanenti del catalogo e i ranghi alfabetici sono campi indipendenti; nessuno deve essere sostituito silenziosamente con un altro. La pressione è un livello di iterazione algoritmica, non una grandezza fisica.

## XIV. Indice dei pacchetti recenti e ampliati

Questo indice aggiunto mette in evidenza pacchetti ampliati di recente, librerie di dominio e raccolte di codice sorgente. Le voci sono raggruppate alfabeticamente per nome del pacchetto o argomento. Le descrizioni indicano la posizione nel repository; implementazione e stato di convalida devono essere verificati nella documentazione e nei test di ciascun pacchetto.

### A–C

- **Advantage** — `lib/advantage/` e `advantage/`: ottimizzazione di un dividendo unico rispetto a un modello tramite una Sequitur onion a strati, con cinque sottocartelle tematiche: `math/` (massimizzazione congiunta dei profitti, negoziazione di Nash, modello bilaterale newsvendor), `physics/` (metrica, geodetica, contatto e SDE del campo di fiducia; analogia di modellazione esplicitamente segnalata), `integration/` (compartimenti di integrazione verticale e punteggio di sofisticazione), `grading/` (valutatore di circuiti di sofisticazione A–D per moduli e insiemi) e `output/` (emissione verso terminale, CommonRails o socket). Un consumatore nativo C/C++ (`advantage/`) collega il modello a esempi commerciali elaborati. Vedere [`lib/advantage/README.md`](lib/advantage/README.md) e [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alphabet e UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` e `utf-4088/`: cataloghi di alfabeti, supporto nativo ai set di caratteri, ordinamento delle mappe di input e documentazione d’integrazione UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` e `lib/autocad/`: flussi di disegno 2D interattivi e componenti riutilizzabili per documenti e modifica CAD.
- **Ballum** — controllo del display a livello di pixel: impostazione/lettura dei pixel, blit di regioni, luminosità/temperatura/gamma dell’intero display, input di puntatore/tastiera e **superficie framebuffer a schermo intero** per un controllo totale, proprietario e rapido di ogni pixel (finestra override-redirect e presentazione con una sola chiamata `XPutImage`/MIT-SHM su Linux/X11). Il nucleo nativo è in `ballum/`, la facciata SLeeLa in `lib/ballum/` e il bridge VM in `impl/`; vedere [`ballum/README.md`](ballum/README.md) e l’[esempio eseguibile](ballum/example/).
- **CDMA** — `cdma/` e `lib/cdma/`: materiale tecnico relativo a CDMA e sviluppo della libreria sorgente SLeeLa.
- **Character** — `lib/character/`: profili dei personaggi e statistiche riutilizzabili, inclusi esempi di profili biografici.
- **Coorenagraph** — `lib/coorenagraph/`: ragionamento etico, vocabolario morale, deliberazione e concetti correlati; vedere il README del pacchetto e i file sorgente.

### D–G

- **Modelli sorgente di governance** — [`lib/governance`](lib/governance/README.md) definisce strutture dichiarative SLeeLa per proposte, schede manuali anonime e conteggi aggregati. L’annotazione proposta `@vote` e il processo manuale sono documentati in [`registry/DEMOCRATIC-VOTING-SYNTAX.md`](registry/DEMOCRATIC-VOTING-SYNTAX.md); i compilatori attuali la trattano come metadati di commento inerti.

### H–I

- **HTTP 1.0–8.0** — da `lib/http-1.0/` alle directory disponibili dei pacchetti HTTP versionati: specifiche di protocollo e implementazioni specifiche per versione. Consultare la documentazione di ciascuna versione per l’ambito preciso e lo stato di verifica.
- **Corte penale internazionale** — `lib/international-criminal-court/` e `INTERNATIONAL.CRIMINAL.COURTS.md`: sorgenti e documentazione specifiche del dominio.

### O–S

- **OFDMA** — `ofdma/`: materiale tecnico sull’accesso multiplo a divisione di frequenza ortogonale e risorse di sviluppo C/C++.
- **Filosofia e modulo Conversation Mask** — [`philosophy/`](philosophy/README.md) contiene le basi native C/C++; [`philosophy/module/`](philosophy/module/README.md) organizza il livello successivo del modulo in tre aree esplicite: [`data/`](philosophy/module/data/README.md), per osservazioni tipizzate e datate, conclusioni degli eventi, impostazioni dell’emivita, provenienza e consenso; [`helper/`](philosophy/module/helper/README.md), per convalida deterministica, decadimento della rilevanza, confronti compatibili e presentazione filtrata per consenso; e [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md), per un’interfaccia conversazionale di supporto non clinica. Gli handle rivolti a SLeeLa e il Tree composto si trovano in [`lib/philosophy/`](lib/philosophy/README.md), accanto al più ampio pacchetto philosophy. Il modulo utilizza input espliciti e dà priorità al consenso: non monitora passivamente persone o media, non diagnostica, non deduce intenzioni private, non assegna punteggi al valore umano e non raccomanda modifiche ai farmaci. I collegamenti tra eventi preservano provenienza e navigazione, ma non costituiscono corroborazione indipendente.
- **Synchro** — integrazione nativa di runtime e compilatore, oltre a funzionalità relative alla distribuzione dei pacchetti e alla misurazione; vedere le sorgenti pertinenti in `impl/` e l’implementazione del compilatore.
- **SleelaUI / UI terminale** — `lib/user-interface/` e il progetto `sleela-terminal`: integrazione dell’interfaccia utente SLeeLa e componenti di interazione orientati al terminale.

### U–W

- **Nazioni Unite** — `united-nations/`: materiali del repository specifici del dominio.
- **Wi-Fi 4, Wi-Fi 5 e Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`, `wi-fi-6/` e le directory sorgente correlate `lib/wifi/`: materiale di rete wireless e risorse di configurazione specifiche per generazione.

### Manutenzione dell’indice dei pacchetti

Quando viene aggiunto un pacchetto o ampliato in modo sostanziale, aggiungere qui la relativa voce nel gruppo alfabetico appropriato, collegare la directory canonica e il relativo README e descrivere soltanto funzionalità supportate dai sorgenti presenti nel repository. Mantenere questo indice additivo: non rimuovere né riscrivere le sezioni README esistenti durante gli aggiornamenti dell’indice.

---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

`bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh` :: `⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓`

`bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc` :: `⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉`

Democrazia statunitense e politica degli Stati Uniti.

---

## XVI. Immagini del marchio della scienza nazionale

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="Scienza nazionale SLeeLa" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="NSF nazionale SLeeLa" width="560"></p>

## XVII. Catalogo sperimentale UTF-4088 da un milione di voci

Il [generatore del catalogo UTF-4088](utf-4088/tools/generate_catalogue.py) crea in modo deterministico un catalogo sperimentale di simboli da 1.048.576 voci. Il formato compatto documentato è descritto in [`utf-4088/FORMAT.md`](utf-4088/FORMAT.md); il contratto di generazione e convalida si trova in [`utf-4088/GENERATION-RESULTS.md`](utf-4088/GENERATION-RESULTS.md).

Eseguire il generatore con Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Il binario generato è di 16.777.244 byte ed è progettato per rimanere sotto i 20.000.000 byte. I test unitari e un workflow GitHub Actions verificano il numero di voci, l’unicità dei glifi, la dimensione e SHA-256; le esecuzioni riuscite pubblicano il binario e il riepilogo come artefatto del workflow `utf4088-million-catalogue`. Il binario generato viene deliberatamente escluso dalla cronologia Git. UTF-4088 rimane sperimentale e non è compatibile con Unicode.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Aggiunte recenti — costante Pi, avvio Modium/Registry, math di sleelactl e `@native`

Questa sezione registra le funzionalità aggiunte durante il lavoro del 2026-11, a partire dai gruppi più recenti. I file sorgente collegati e il relativo stato di validazione fanno fede; le modifiche in revisione non sono descritte come integrate finché non raggiungono `master`.

### La costante di catalogo Pi da 100.000.000 di cifre

Il repository include un valore verificato di π da **cento milioni di cifre** in [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (un decimale ben formato, `3.14159…`), insieme a un valore da 57.344 cifre in [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Entrambi sono valori autentici di π verificati in modo indipendente. [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md) documenta i controlli: prefisso di riferimento, punto di Feynman alla posizione frazionaria 762, cifra famosa alla posizione un milione, corrispondenza con `lib/math/SLPiReference.sleela`, analisi della frequenza delle cifre sull’intero file e script di riproduzione in un solo passaggio.

### Modium™ / Registry™ caricano la costante Pi all’avvio del sottosistema

Nei punti di ingresso dedicati — `sleela registry` e `sleela modium` (e al primo avvio dell’installer), **non** a ogni `sleela run`, i sottosistemi Modium e Registry caricano la grande costante del catalogo π. In un terminale interattivo viene mostrata una riga rossa temporanea `Loading Modium™` / `Loading Registry™`, poi cancellata sul posto prima dell’output normale. L’effetto è strettamente limitato ai TTY, quindi l’output reindirizzato e quello dei test restano identici byte per byte. Il localizzatore (non i byte) viene inizializzato come valore predefinito delle chiavi `Math` del registro `piReferenceLargePath` e `piReferenceLargeDigits`, sia nel modello SLeeLa ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) sia nel seeder nativo ([`registry/sleela_registry.c`](registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` espone `largePath()` / `largeDigits()`. La costante π viene installata nella posizione SLeeLa del sistema operativo (`<data>/pi/`) sia dall’installer standard `make` sia dall’installer guidato «beautiful». Dettagli: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — installer «beautiful» e subject `math` supportato dal nativo

- L’installer guidato («beautiful») ora installa `sleelactl` (il control plane di SLeeLa) e il relativo registro dei moduli, come l’installer `make`. Vedere [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- `sleelactl` ha acquisito una nuova natura **`subject`** per le librerie subject native C/C++ compilate nel runtime e registra la prima: **`math`**. `sleelactl run math` esegue end-to-end il backend nativo `__NativeMath` (`impl/subjects/math/math_native.cpp`) tramite la demo di riferimento del subject; `sleelactl info math` spiega come raggiungerlo (`import math;`). La natura subject offre solo `run` e `info`: non esiste deliberatamente un verbo `check`, perché `sleela check` non carica i subject nativi registrati tramite SHEET.

### `@native`, Native Drive ed esecuzione multilingue

Una nuova annotazione di metodo **`@native`** richiede la compilazione nativa anticipata (*ahead-of-time*) di un metodo ad alto carico. Il nuovo comando `sleela aot` individua i metodi `@native`, verifica che ciascuno rientri in un *sottoinsieme offloadable* supportato ed emette C/C++. Il processo **fallisce in modo chiuso**: rifiuta con una motivazione precisa qualsiasi costrutto che non sia ancora traducibile, così da non produrre mai un metodo tradotto solo parzialmente.

```sh
sleela aot --list <file.sleela>    # verdetto per metodo: ok / REFUSED (motivo)
sleela aot --emit <file.sleela>    # emette C/C++ (stdout) per un programma nel sottoinsieme
```

`@native` resta una richiesta di trattamento AOT, con i costrutti non supportati rifiutati prima dell’emissione. Il [Native Drive](impl/native-drive/README.md) supporta build multi-unità di eseguibili e librerie condivise C/C++ tramite i compilatori dell’host (incluso MSVC), l’output AOT SLeeLa supportato, l’ispezione dei simboli rispetto all’ABI del loader nativo, l’esecuzione di processi senza shell, la gestione di stdin/stdout/stderr, il piping a più stadi e la delega XClass al loader di produzione. `sleelactl` ne espone i comandi di build ed esecuzione e instrada l’ispezione XClass e il rendering/l’installazione espliciti delle unità systemd tramite `inspect native-drive`. La VM possiede una tabella limitata di handle di moduli nativi, con autorizzazione obbligatoria dell’host e handle verificati per generazione. La sintassi 3.0 aggiunge ora `nativeOpen`, `nativeCall` e `nativeClose`: il dispatch di bytecode di sola aggiunta attraversa un’ABI con versione indipendente (null/bool/int64/double/UTF-8), mentre array, struct e altri valori compositi posseduti dalla VM restano all’interno dell’interprete e possono usare come ripiego il bytecode omonimo. I plugin possono facoltativamente ricevere una tabella di callback dell’host versionata per invocare funzioni SLeeLa esatte tramite esecuzione annidata della VM, con rooting GC degli stack sospesi, una allowlist esplicita dei nomi di callback dell’host e rifiuto delle chiamate ricorsive all’API nativa. Il codice nativo resta codice fidato eseguito nel processo, non una sandbox. Le annotazioni di policy dei processi nel sorgente hanno uno schema versionato ma non sono ancora analizzate né applicate. Dettagli, confini di sicurezza e registro dei sette flussi di lavoro: [Native Drive](impl/native-drive/README.md) e [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).

## XIX. Edizioni internazionali del README

SLeeLa fornisce punti di ingresso README specifici per paese per i 50 mercati della sua edizione internazionale. Ogni paese dispone di un file denominato `README.<CC>.md`; quando sono pertinenti più lingue, vengono raggruppate nel file del paese.

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

**Politica di traduzione:** questi file sono punti di ingresso localizzati iniziali per ciascun paese. Le introduzioni e le intestazioni linguistiche sono localizzate, mentre il riferimento tecnico completo resta il [README.md canonico in inglese](README.md). Un file nazionale non deve essere descritto come traduzione completa finché tutte le sezioni non sono state tradotte, gli identificatori tecnici e i collegamenti non sono stati verificati e l’edizione non ha ricevuto una revisione linguistica. La copertura linguistica di questa prima fase è rappresentativa e non afferma che tutte le lingue riconosciute costituzionalmente, regionali, indigene o dei segni siano state tradotte integralmente.


## Rumantsch

### Guid tecnic supplementar en rumantsch

#### 1. Construir e testar

Il file `SLEELA.syntax` è la referenza principala per la grammatica da la lingua. La versiun dal compilatur e la versiun da la specificaziun da la lingua èn duas chaussas differentas. Betg mintga versiun dal compilatur sto sustegnair tut las funcziuns descrittas en la specificaziun.

Da la ragisch dal repository pon ins executar las suandantas cumondas, tenor basegn:

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

Auters targets Make impurtants èn:

```sh
make all
make registry
make tests
make ballum
make help
make clean
```

Sche la construcziun d'in singul component reussescha, quai na signifitga betg che tut las bibliotecas u tut las plattafurmas èn prontas per ina publicaziun. Ils resultats dals tests ston vegnir colliads cun il commit Git, il sistem operativ ed ils utensils utilisads. Tests betg exequids u omess na dastgan betg vegnir rapportads sco reussids.

#### 2. Installaziun

SLeeLa sustegna l'installaziun per l'entir sistem, l'installaziun en il conto d'in singul utilisader e la preparaziun d'in directori per construir pachets:

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

In'installaziun dal sistem utilisescha per regla vias sco `/usr/local/bin`, `/usr/local/lib/sleela` e `/usr/local/share/sleela`. Cun `PREFIX="$HOME/.local"` vegnan ils files installads en il directori da l'utilisader; per regla na dovri nagin `sudo`. `DESTDIR` producescha ina structura temporara per la preparaziun da pachets e n'è betg in'installaziun directa sin il sistem ospitant.

Resguardai il [guid d'installaziun](install/README.md) e las instrucziuns specificas per la plattafurma. Na supprimai betg files che n'èn betg registrads sco files administrads dal process d'installaziun.

#### 3. Linguatg, compilatur e runtime

SLeeLa cumpiglia in compilatur, in runtime, maschinas virtualas, bibliotecas ed utensils per la documentaziun. Il repository cuntegna era code nativ C e C++ sco er moduls SLeeLa. Sche code traversa il cunfin tranter code nativ e runtime, sto el respectar il contract ABI dal component pertutgà.

La pipeline dal compilatur cumpiglia normalmain l'analisa lexicala, il parsing, l'analisa semantica, la reducziun ad ina represchentaziun intermediara, la generaziun da code e l'execuziun sin la maschina virtuala. Mintga fasa e mintga cunfin tranter fasas duai vegnir testà. In test dal parser sulet na po betg cumprovar la correctadad d'in program entir.

#### 4. Contracts API e cunfins da moduls

Mintga API publica duai documentar las entradas acceptadas, las sortidas, las pussaivlas situaziuns d'errur, la proprietad da la memoria e las consequenzas lateralas. La documentaziun d'in modul duai descriver cleramain las premissas, las cundiziuns finalas e las restricziuns da la plattafurma.

In num da simbol u ina referenza ad in modul na dat betg automaticamain la permissiun d'exequir code. L'autorisaziun e ils cunfins d'execuziun ston vegnir controllads al lieu previs. Sche las entradas èn invalidas, sch'i mancan resursas u sch'ina funcziun n'è betg sustegnida, duai l'operaziun terminar en moda segira.

#### 5. Tests da regressiun e reproducibilitad

La collecziun da tests duai cuntegnair cas positivs e negativs: files da funtauna valaivels, errurs da parsing, violaziuns da tips, errurs da reducziun IR, generaziun da code e resultats d'execuziun. Per l'integraziun nativa ston era la cumpatibilitad ABI, la gestiun d'errurs e la liberaziun da resursas vegnir testadas.

Registrai il sistem operativ, la versiun dal compilatur, las versiuns dals utensils e il commit Git. Ils resultats da tests repetids duain pudair vegnir cumparegliads; tests omess ston restar visibels en il rapport. Consultai la [politica da tests](markdown/TESTING.md) e la [matrica da sustegn da plattafurmas](docs/PLATFORM-SUPPORT.md).

#### 6. Terminologia e mantegniment

Mantegnai senza midadas ils nums da files, identificaturs, cumondas, vias, URL, exempels da code e nums d'API. Translatà duai vegnir il text explicativ, betg las lingias che il compilatur u la shell sto identifitgar. Utilisai ina terminologia constanta per compilatur, runtime, maschina virtuala, modul ed API. Sche nagin term rumantsch è cler u stabil, agiuntai il term tecnic englais a la prima utilisaziun.

La translaziun è ina lavur cuntinuanta. Cura che la README canonica vegn midada, cumparegliai las secziuns correspundentas e actualisai questa ediziun. Quest guid supplementar porscha ina basa tecnica en rumantsch, ma na remplazza betg la translaziun cumpletta da tut las secziuns existentas ni ina revisiun finala tras persunas cun cumpetenza linguistica.


---

**Stato della traduzione:** le sezioni I–XIX sono tradotte in italiano. Identificatori tecnici, comandi, esempi di codice, nomi di file, collegamenti e indirizzi sono stati conservati. Si raccomanda una revisione linguistica finale da parte di madrelingua.
