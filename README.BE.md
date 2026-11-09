# SLeeLa 3.0 — Belgien (BE)

## Deutsch

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>

<p align="center"><strong>Eine native Systemsprache mit Compiler, Laufzeitumgebung und Ökosystem virtueller Maschinen.</strong></p>

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

Für eine gezielte, geführte Bereitstellung nur des Hauptpakets – der Binärdateien `sleela`, `sleelvac` und `nordshrift), der Standardbibliothek, Protokolle und Unterstützungsmodulе – dient der saubere Installer:

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

## XV. Bitcoin Conjegeum

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

## XVIII. Neueste Ergänzungen – Pi-Konstante, Modium-/Registry-Start, sleelactl-Math und `@native`

Dieser Abschnitt dokumentiert Funktionen aus den Arbeiten von 2026-11, beginnend mit den neuesten Gruppen. Alle sind in `master` eingecheckt.

### Die Pi-Katalogkonstante mit 100.000.000 Stellen

Das Repository enthält einen verifizierten **hundertmillionenstelligen** Wert von π unter [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (eine korrekt formatierte Dezimalzahl, `3.14159…`) sowie einen Wert mit 57.344 Stellen unter [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Beide sind unabhängig verifizierte echte π-Werte. [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md) dokumentiert die Prüfungen (Lehrbuch-Präfix, Feynman-Punkt an der Nachkommastellenposition 762, die bekannte Millionstel-Ziffer, Übereinstimmung mit `lib/math/SLPiReference.sleela`, eine Ziffernhäufigkeitsanalyse der gesamten Datei) sowie ein Ein-Schritt-Reproduktionsskript.

### Modium™ / Registry™ laden die Pi-Konstante beim Start des Subsystems

An den dedizierten Subsystem-Einstiegspunkten – `sleela registry` und `sleela modium` (sowie beim ersten Installationsstart), **nicht** bei jedem `sleela run`, laden Modium und Registry die große π-Katalogkonstante. In einem interaktiven Terminal erscheint eine vorübergehende rote Zeile `Loading Modium™` / `Loading Registry™`, die vor der normalen Ausgabe an Ort und Stelle gelöscht wird. Der Effekt ist strikt an TTY gebunden; umgeleitete und Testausgaben bleiben bytegenau unverändert. Der Locator (nicht die Bytes) wird als Standardwert der Registry-`Math`-Schlüssel `piReferenceLargePath` und `piReferenceLargeDigits` gesetzt, sowohl im SLeeLa-Modell ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) als auch im nativen Seeder ([`registry/sleela_registry.c`](registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` stellt `largePath()` / `largeDigits()` bereit. Die π-Konstante wird durch den Standard-`make`-Installer und den geführten „beautiful“-Installer am SLeeLa-Betriebssystempfad (`<data>/pi/`) installiert. Details: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` – der „beautiful“-Installer und ein nativ gestütztes `math`-Subject

- Der geführte („beautiful“) Installer installiert jetzt `sleelactl` (die SLeeLa-Steuerungsebene) und seine Modulregistrierung, entsprechend dem `make`-Installer. Siehe [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- `sleelactl` erhielt eine neue **`subject`**-Natur für native C/C++-Subject-Bibliotheken, die in die Laufzeitumgebung kompiliert werden, und registriert das erste Subject: **`math`**. `sleelactl run math` führt das native Backend `__NativeMath` (`impl/subjects/math/math_native.cpp`) über die Referenzdemo des Subjects vollständig aus; `sleelactl info math` erklärt den Zugriff (`import math;`). Diese Subject-Natur bietet nur `run` und `info`; ein `check`-Verb gibt es bewusst nicht, da `sleela check` keine über SHEET registrierten nativen Subjects lädt.

### `@native` und `sleela aot` – Teil 1 des SLeeLa-zu-nativem-C/C++-Pfads

Eine neue Methodenannotation **`@native`** fordert die Ahead-of-Time-Kompilierung einer rechenintensiven Methode in nativen Code an. Der neue Befehl `sleela aot` findet `@native`-Methoden, prüft, ob jede davon innerhalb einer unterstützten *Offload-Teilmenge* liegt, und gibt C/C++ aus. Der Vorgang **bricht geschlossen ab**: Jede noch nicht übersetzbare Konstruktion wird mit genauer Begründung abgelehnt, sodass niemals eine teilweise übersetzte Methode entsteht.

```sh
sleela aot --list <file.sleela>    # Urteil je Methode: ok / REFUSED (Grund)
sleela aot --emit <file.sleela>    # C/C++ (stdout) für ein Programm innerhalb der Teilmenge ausgeben
```

`@native` ist derzeit verhaltensneutral: Programme werden weiterhin unverändert geprüft und ausgeführt; die Annotation *fordert* lediglich native Behandlung an. Dies ist Teil 1: Er etabliert die Annotationsoberfläche und die klare Grenze der unterstützten Teilmenge. Die Kompilierung des ausgegebenen C/C++ in ein Shared Object und das Dispatching der VM dorthin (mit Interpreter-Fallback) sind spätere Schritte. Details, unterstützte Teilmenge und Exitcodes: [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).

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

<p align="center"><strong>Un langage système natif, son compilateur, son environnement d’exécution et son écosystème de machines virtuelles.</strong></p>

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

## XV. Bitcoin Conjegeum

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

## XVIII. Ajouts récents : constante Pi, démarrage Modium/Registry, math de sleelactl et `@native`

Cette section consigne les fonctions ajoutées lors des travaux de 2026-11, en commençant par les groupes les plus récents. Toutes sont intégrées à `master`.

### La constante de catalogue Pi à 100 000 000 chiffres

Le dépôt contient une valeur vérifiée de π à **cent millions de chiffres** dans [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (un nombre décimal bien formé, `3.14159…`), ainsi qu’une valeur à 57 344 chiffres dans [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Les deux sont des valeurs authentiques de π vérifiées indépendamment. Voir [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md), qui décrit les contrôles (préfixe de référence, point de Feynman à la position fractionnaire 762, célèbre chiffre à la position un million, concordance avec `lib/math/SLPiReference.sleela`, analyse de fréquence des chiffres sur le fichier entier) et un script de reproduction en une seule exécution.

### Modium™ / Registry™ chargent la constante Pi au démarrage du sous-système

Aux points d’entrée dédiés — `sleela registry` et `sleela modium` (ainsi qu’au premier lancement de l’installateur), et **non** à chaque `sleela run`, les sous-systèmes Modium et Registry chargent la grande constante du catalogue π. Dans un terminal interactif, une ligne rouge transitoire `Loading Modium™` / `Loading Registry™` s’affiche puis est effacée sur place avant le début de la sortie normale. Cet effet est strictement réservé aux TTY ; les sorties redirigées et celles des tests restent identiques octet pour octet. Le localisateur (et non les octets) est initialisé comme valeurs par défaut des clés `Math` du registre `piReferenceLargePath` et `piReferenceLargeDigits`, dans le modèle SLeeLa ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) et dans l’initialiseur natif ([`registry/sleela_registry.c`](registry/sleela_registry.c)) ; `lib/math/SLPiReference.sleela` expose `largePath()` / `largeDigits()`. La constante π est installée à l’emplacement SLeeLa du système d’exploitation (`<data>/pi/`) par l’installateur standard `make` et par l’installateur guidé « beautiful ». Détails : [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — l’installateur « beautiful » et un sujet `math` adossé au natif

- L’installateur guidé (« beautiful ») installe désormais `sleelactl` (le plan de contrôle SLeeLa) et son registre de modules, comme l’installateur `make`. Voir [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- `sleelactl` a reçu une nouvelle nature **`subject`** pour les bibliothèques de sujets natifs C/C++ compilées dans le runtime, et enregistre le premier : **`math`**. `sleelactl run math` exécute de bout en bout le backend natif `__NativeMath` (`impl/subjects/math/math_native.cpp`) au moyen de la démonstration de référence du sujet ; `sleelactl info math` explique comment y accéder (`import math;`). Cette nature de sujet ne propose que `run` et `info` : il n’existe volontairement pas de verbe `check`, car `sleela check` ne charge pas les sujets natifs enregistrés par SHEET.

### `@native` et `sleela aot` — première étape de la voie SLeeLa → C/C++ natif

Une nouvelle annotation de méthode **`@native`** demande la compilation native anticipée (*ahead-of-time*) d’une méthode intensive. La nouvelle commande `sleela aot` repère les méthodes `@native`, vérifie que chacune appartient au *sous-ensemble déchargeable* pris en charge et émet du C/C++. Elle **échoue de manière fermée** : toute construction qui ne peut pas encore être traduite est refusée avec une raison précise, afin qu’aucune méthode partiellement traduite ne soit produite.

```sh
sleela aot --list <file.sleela>    # verdict par méthode : ok / REFUSED (motif)
sleela aot --emit <file.sleela>    # émettre du C/C++ (stdout) pour un programme du sous-ensemble
```

Aujourd’hui, `@native` est neutre du point de vue du comportement : les programmes continuent à être vérifiés et exécutés sans changement ; l’annotation ne fait que *demander* un traitement natif. Il s’agit de la Partie 1 : elle établit la surface de l’annotation et la limite explicite du sous-ensemble pris en charge. La compilation du C/C++ émis en objet partagé et le dispatch de la VM vers celui-ci (avec repli sur l’interpréteur) constituent des étapes ultérieures. Détails, sous-ensemble pris en charge et codes de sortie : [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).

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

## Nederlandse vertaling — werkversie

> Deze Nederlandstalige sectie is nog niet volledig vertaald. De volledige Nederlandse technische tekst moet nog worden aangevuld en taalkundig worden nagekeken.

# SLeeLa 3.0 — België (BE)

## Nederlands

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>

<p align="center"><strong>Een native systeemprogrammeertaal met compiler, runtime en ecosysteem van virtuele machines.</strong></p>

> **Internationale landeneditie — vertaalconcept.** Dit Nederlandse bestand is een werkversie. De oorspronkelijke inhoudsstructuur en technische verwijzingen zijn behouden, maar verschillende onderdelen bevatten nog brontekst of moeten taalkundig worden herzien. Behandel deze editie nog niet als een volledige Nederlandse vertaling.

**The Now. The Ingredient.** Een programmeertaal is meer dan alleen de syntaxis: ook de compiler, runtime, bibliotheken, hulpmiddelen en platformcontracten die software bruikbaar maken, horen erbij.

**Werkterreinen van SLeeLa.** SLeeLa omvat **Math**, **Object**, **Science** en **Reach**, de vier dimensies van het werk: wat berekenbaar is (Math), het vastgestelde object in de catalogus van wat bestaat (Object), het gedisciplineerde onderzoek naar waarheid (Science) en het uitbreiden van de mogelijkheden van een systeem naar andere systemen en mensen (Reach). **Nationale belangen blijven bestaan**; geen van deze vier dimensies vervangt ze. Wanneer een onderwerp de belangen van een land raakt, blijft die relatie behouden en wordt deze zorgvuldig vastgelegd, bijvoorbeeld in de metadatavelden `metaOrigin` en `metaNationalSchedule` van een klasse.

## I. Aan de slag

- [De native implementatie bouwen](impl/README.md)
- [Virtuele machines en Modium — praktische gebruikershandleiding](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [CPU-stack in de SLeeLa-broncode](lib/cpu/CPU.md): een volledige machineketen van logische poorten via ALU, cores en besturingssysteem tot programma's; met meer dan 130 CPU-profielen en instelbare [spanningen, variabele kloksnelheden en geavanceerde timingvoormeters](lib/cpu/TIMING.md), voor uitvoering [volgens specificatie of met overklokken](lib/cpu/components/README.md).
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

Recente toevoegingen aan de taal, drivers en standaardbibliotheek:

- **Standaardbibliotheekpakket `advantage`.** [`lib/advantage/`](lib/advantage/README.md) modelleert het optimaliseren van een uniek dividend ten opzichte van een model met een gelaagde structuur die **Sequitur onion** (dividend → beoordeling → aandachtspunten → wiskunde → toevoegingen → aftrek → hergroepering → iteratie). Het omvat de domeinen `math/` (bilaterale handelseconomie, gezamenlijke winstmaximalisatie, Nash-onderhandelingen en de kritieke newsvendor-ratio), `physics/` (structureel-economische modelanalogie van vertrouwensvelden), `integration/` (recursieve verticale integratie), `grading/` (A–D-classificatie van complexiteit) e `oftput/` (rofteerbare uitvoer naar terminal, CommonRails of socket/connector). Het pakket bevat uitvoerbare voorbeelden en documentatie over definities, wiskunde en natuurkunde. De C/C++-consument in [`advantage/`](advantage/README.md) verbindt het model met handelsvoorbeelden.
- **Anotações de temdoorização de ciclos — `@loopDelay` / `@loopBudget`.** Deze gedragsannotaties worden vóór `for`, `while` of `do`, -lussen geplaatst en voegen echte pauzes aan de uitvoering toe. `@loopDelay(ms = 250)` define een pauze door iteração; `@loopBudget(seconds = 10, iterations = 1000)` verdeelt een bekend aantal iteraties over een totaal tijdsbudget. Beide gebruiken de aanvullende opcode `OP_SLEEP` (ISA 157), die ook de ingebouwde functie ondersteunt `sleep(<ms>)`. De implementatie gebruikt `nanosleep` / `Sleep`. Zie [`SLEELA.syntax` §32](SLEELA.syntax), de [API-referentie](api/sleela/3.0/LOOP_TIMING_API.md) en het [voorbeeld executável](examples/loop-timing-annotation-demo.sleela).
- **SleelaDoc na linha de comandos — `sleela docs`.** De opdracht somt documenteerbare onderwerpen op, toont pakketten en zoekt klassen op, ook hoofdletterongevoelig en met het standaardvoorvoegsel `SL`. De pagina's bevatten een definitie, ontwerpdoel, relaties, structuur, gebruiksconcepten en een beheersingsschaal met vijf niveaus. As etiquetas `@intent`, `@thesis`, `@axiom`, `@param`, `@see` e `@mastery` enriquecem a documentação. `sleela docs --generate [dir]` maakt een tekstcorpus voor offline raadpleging; `make sleeladoc` bundelt het corpus en de bijbehorende bronbestanden in `sleeladoc/build/sleeladoc.jar`. Zie [`lib/sleeladoc/`](lib/sleeladoc/README.md) e [`sleela(1)`](man/man1/sleela.1).
- **SleelaDoc e documentação HTML.** De extractor [`lib/sleeladoc`](lib/sleeladoc/README.md) zet documentatiecommentaar uit `.sleela`-eenheden om in Markdown- of HTML-pagina's. De doorzoekbare documentatiesite voor de volledige bibliotheek `/lib` wordt gegenereerd in [`sleeladoc-api/`](sleeladoc-api/README.md). De compiler kan ook MediaWiki-pagina's genereren via `--wiki`. Zie [SLEELADOC](manifesto/SLEELADOC.md), [SLEELADOC.WIKI](manifesto/SLEELADOC.WIKI.md) en het [voorbeeld](lib/sleeladoc/sleeladoc-demo.sleela).
- **Modelo de objetos de documento XML (`lib/xml`).** De bibliotheek bevat een DOM-boomstructuur com nós de elemento, texto, comentário, CDATA e instrução de processamento, atributos nomeados, serialização compacta/formatada, escape XML 1.0 §2.4, análise door células de caracteres e registo de qualidade `SLXmlAspect`. Gelijkheid van knooppunten is reflexief, symmetrisch en deterministisch, met niveaus voor identiteit, onderwerp en structuur. Zie [`lib/xml/README.md`](lib/xml/README.md) en het [voorbeeld executável](lib/xml/xml-dom.sleela).
- **Primitivos inteiros alargados.** `int`, `long` e `int64` zijn 64-bits signed integers; `int128` wordt momenteel ondersteund via een 64-bits representatie; echte 128-bits ondersteuning staat als toekomstig werk gedocumenteerd. As famílias binárias assinadas `int32`–`int16777216` (sintaxe 2.7) en unsigned `uint32`–`uint16777216` (sintaxe 2.8) breiden deze reeks uit. Zie [`SLEELA.syntax`](SLEELA.syntax) §§21, 24 e 25.
- **Campos consequentes `@mix` (sintaxe 2.9).** Een klasse mag herhaalde instantievelden met dezelfde naam declareren wanneer elke declaratie is voorzien van `@mix`. O grupo é indexado door ordem crescente (`1,2,3,…`) of door índices explícitos em `@mix(1,2,3,...)`; het aantal indexen moet overeenkomen met het aantal velden. Zie [§28](SLEELA.syntax).
- **Preonems — marcadores documentais do Processo Natural.** De declaratieve markeringen omvatten `author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`, `pure`, `gasoline` en `artifact`. Ze zijn contextueel en versiegebonden en veranderen de parsing, typecontrole of codegeneratie niet; buiten de markeringspositie blijven het gewone identifiers. Zie [`SLEELA.syntax`](SLEELA.syntax) §§18–20, 23, 26, 30 e 35.
- **Nordshrift met volledige bibliotheektoegang.** In syntaxis 3.0 gebruikt Nordshrift dezelfde recursieve `/lib`-index als de compiler, met alle pakketten en `.sleela`-eenheden; `nordshrift check` toont de aantallen voor de volledige boomstructuur.
- **Stanchion de recursão.** [`recursion/stanchion/`](recursion/stanchion/README.md) bevat C- en C++-referentieprototypes voor zeven gedocumenteerde primitieve contracten, plus generieke, toegepaste, wetenschappelijke en educatieve voorbeelden. De referentiebronnen moeten worden gecompileerd en getest voordat ze als geverifieerd mogen gelden.
- **Comentários angulares (`<` / `<<`).** O frontend reconhece comentários de linha `< line >` e de bloco `<< block >>`; raadpleeg [§1.1](SLEELA.syntax).
- **Literais inteiros door radix.** Os literais `int` aceitam hexadecimal (`0x`), binário (`0b`) e octal (`0o`), com scheidingstekens `_`, tanto na linguagem como no formato de modelos CPU [`.xlt`](lib/xlt/README.md). Zie [§1.4](SLEELA.syntax).
- **Norma Metadata Class.** De standaard voor beschrijvende metadata — herkomst, wettigheid en jurisdictiebereik — geldt voor alle `.sleela`-bestanden met klassen onder `/lib`. Zie [§15](SLEELA.syntax) e a [lição 13](tutorial/13-metadata-class.md).
- **Família de texto e String.** `lib/text` fornece operações de caracteres e cadeias, incluindo `charAt`, `substring`, `indexOf`, `reverse`, alinhamento de colunas e modelos posicionais. Zie de [tutorial](tutorial/14-text-and-strings.md) en het [voorbeeld](lib/text/text-string-demo.sleela).
- **String em folhas Nordshrift.** Uma folha `.sst` pode declarar um bloco tipado `strings:` que contribui voor o manifesto de componentes da compilação. Zie a [referência Nordshrift](wiki/Nordshrift-Sheet-Reference.md).
- **Modelos CPU door opcodes e equivalência de chamadas.** Cada `SL<ARCH>CPU` ondersteunderta a execução de um fluxo nativo de instruções através do ciclo fetch-decode-execute en gelijkwaardige uitvoering van code van een ander proces op de gedeelde VM-infrastructuur. Zie [`lib/cpu/EXECUTION.md` §5](lib/cpu/EXECUTION.md).

## III. Bouwen en testen

**Vereisten:** een toolchain C/C++ (GCC of Clang) en GNU Make; Python 3 voor ferramentas de manifesto e fluxo de trabalho; em Linux, também os pacotes de desenvolvimento X11/Xrandr voor produtos de visualização. De opdracht `make deps` van elke module toont of installeert de benodigde afhankelijkheden. De build in `impl/` stopt veilig wanneer niet aan het geverifieerde SHA-256-manifest wordt voldaan; het Makefile in de hoofdmap neemt deze controle automatisch op.

De snelste start vanuit een schone clone:

```sh
make                                  # alles bouwen vanuit de repository-root
```

Of alleen de native kern bouwen en testen:

```sh
make -C impl all                      # alleen de compilerkern en runtime bouwen
bash ./test-suites/run-all.sh --all   # de volledige testsuite uitvoeren
```

De opdracht `make` in de hoofdmap bofwt alle producten (kern, VM, codecs, churn, antivirus enzovoort) en sluit af met het aanmaken van het **register** en het materialiseren van de basistabel. Zo bevat een gebouwde werkboom direct een verifieerbaar register.

Belangrijke doelen:

```sh
make                     # alles bouwen (alias voor make all; het register wordt aan het einde aangemaakt)
make all                 # dezelfde volledige build, expliciet vermeld
sudo make install        # runtime-opdrachten en ondersteunende bestanden installeren (zie § IV)
make clean               # buildresultaten van alle geïntegreerde systemen verwijderen
make help                # alle doelen van het Makefile in de hoofdmap tonen
```

Andere nuttige doelen:

```sh
make registry            # alleen het register bouwen en de tabel materialiseren
./build-registry.sh      # dezelfde bewerking via een rechtstreeks script
make tests               # de testsuite bouwen en uitvoeren
make ballum              # de Ballum-weergavebibliotheek voor dit besturingssysteem bouwen
bin/install-modules      # modules gefaseerd installeren/bouwen; dit duurt ongeveer 10–20 minuten
```

Het doel `registry` voert [`build-registry.sh`](build-registry.sh): de registerkern bouwen, de basistabel opslaan in de draagbare tekstindeling `registry/build/sleela.sleelareg` en het MySQL-tabellenscript exporteren `registry/build/registry.sql` (via `sleela registry exdoort`). Het register kan worden bekeken met `sleela registry` of `registry/build/sleela-registry review`. Zie de [guia do registo](registry/README.md) e a [arquitetura Registry 3.0](registry/REGISTRY-3.0-ARCHITECTURE.md).

Zie de handleidingen de compilação e testes voor conhecer os pré-requisitos e os passos específicos de cada plataforma. Een geslaagde build van één component bewijst niet dat alle bibliotheken of platforms gereed zijn voor release.

## IV. Installatie

Installeer na het bouwen van de native producten de runtime-opdrachten en ondersteunende bestanden:

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

Overbelasting van constructors en delegatie naar constructors `this(...)` e de classes-base vallen uitdrukkelijk buiten het geverifieerde syntaxiscontract 1.8. Raadpleeg de grammatica en implementatienotities voordat u op een functie vertrouwt. De negen SleelaDoc-broneenheden declareren nu `#sleela 3.0`; a verificação comum do compilador aceita sintaxe de código-fonte de 1.0 a 3.0. De nieuwe API-documentatie wordt gegenereerd met `make api-docs`.

De repository bevat uma implementação nativa C/C++, referências voor geração de SLVM, módulos da biblioteca padrão, testes, exemplos e integrações de plataforma. De implementatie- en validatiestatus verschilt per module. Zie a especificação aplicável e os resultados atuais de CI; ga er niet van uit dat alle componenten dezelfde mate van voltooiing hebben.

## VI. Engineeringprincipes

- **Bewijs vóór beweringen:** maak duidelijk onderscheid tussen wat is geïmplementeerd, getest, experimenteel en gepland.
- **Eén contract per grensvlak:** documenteer syntaxis, API's, bestandsindelingen, platformaannames en foutgedrag.
- **Portabiliteit vanaf het ontwerp:** isoleer platformgebonden backends en test ze op native runners.
- **Veilig falen:** test misvormde invoer, resourcelimieten, processtoringen en opruimpaden.
- **Reproduceerbare releases:** voer schone builds uit en publiceer hashes van artefacten en releasenotities.

## VII. Antivirusbuild en integriteit

Het antiviruspakket Sleelavirin bevat um verificador SHA-256 nativo, um adaptador ClamAV opcional, manifestos de integridade voor fontes e índices e somas de verificação voor pacotes de plataforma. Bouw en test het pakket vanuit de hoofdmap van de repository:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Het integriteitsmanifest wordt gegenereerd onder `antivirus/build/`. Het manifest omvat versiebeheerde broncode- en documentatiebestanden en beschermde handtekeningindexbestanden; gegenereerde resultaten en de veranderlijke ClamAV-cache zijn uitgesloten. Snapshots van ClamAV-databases hebben eigen manifesten. Os ZIP de lançamento incluem `INTEGRITY-MANIFEST.json`, `SHA256SUMS` e um ficheiro complementar `.sha256`. Vergelijk hashes van gedownloade archieven met een onafhankelijk verkregen vertrouwenswaarde: een controlesom die naast een archief is opgeslagen, is geen handtekening van de uitgever. Zie de [guia Sleelavirin](antivirus/README.md), a [lista de verificação de segurança](antivirus/SECURITY.md) e o [workflow de compilação do antivírus](.github/workflows/antivirus-binaries.yml).

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

Vervang `sleela-american` door `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` of `sleela-sleela`. Als Emacs vraagt om het thema te vertrouwen, inspecteer dan eerst het lokale themabestand.

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
- **Ballum** — schermbesturing op pixelniveau: pixels lezen/schrijven, regio's kopiëren, algemene helderheid/kleurtemperatuur/gamma, muis- en toetsenbordinvoer en een volledig schermvullend framebufferoppervlak. De native kern staat in `ballum/`, de SLeeLa-façade in `lib/ballum/` en de VM-brug in `impl/`. Zie [`ballum/README.md`](ballum/README.md) en de [exemplos](ballum/example/).
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


---

## Taalstatus België

De Duitse en Franse secties bevatten de volledige technische README-vertalingen. De Nederlandse sectie is opgenomen als werkversie en is nog niet volledig vertaald. Deze editie mag daarom niet als volledig drietalig worden aangeduid totdat de Nederlandse technische tekst is voltooid en nagekeken.
