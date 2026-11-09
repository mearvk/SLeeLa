# SLeeLa 3.0 — Deutschland (DE)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>

<p align="center"><strong>Eine native Systemsprache mit Compiler, Laufzeitumgebung und Ökosystem virtueller Maschinen.</strong></p>

> **Internationale Länderausgabe.** Diese Datei bündelt die für Deutschland vorgesehenen Sprachfassungen. Die technische Übersetzung wird schrittweise erstellt. Bereits übersetzte Abschnitte sind unten vollständig wiedergegeben; noch nicht übersetzte Abschnitte werden ausdrücklich als ausstehend gekennzeichnet.

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
