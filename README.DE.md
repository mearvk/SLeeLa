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

## Noch ausstehende Abschnitte

Die folgenden Abschnitte sind noch nicht übersetzt und werden in weiteren Batches ergänzt: **VIII. SLeeLa-Editor-Themes; XIV–XVII. Weitere Projektbereiche; XVIII. Neueste Ergänzungen; XIX. Internationale README-Ausgaben.** Bis zur Fertigstellung bleibt die englische [kanonische README.md](README.md) die vollständige Referenz.

---
**Übersetzungsstatus:** Teilübersetzung – Abschnitte I–VII und IX–XIII übersetzt. Technische Kennungen, Befehle, Codebeispiele, Dateinamen und Links wurden unverändert beibehalten.
