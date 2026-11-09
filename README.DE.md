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

**Übersetzung ausstehend.** Die vollständigen Änderungs- und Funktionsmeldungen aus dem englischen Abschnitt II werden in einem folgenden Übersetzungsbatch übertragen. Maßgebliche Quelle: [README.md – Abschnitt II](README.md).

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

## Noch ausstehende Abschnitte

Die folgenden Abschnitte sind noch nicht übersetzt und werden in weiteren Batches ergänzt: **II. Neuigkeiten; VIII. SLeeLa-Editor-Themes; IX. Mitwirken; X–XVII. Weitere Projektbereiche; XVIII. Neueste Ergänzungen; XIX. Internationale README-Ausgaben.** Bis zur Fertigstellung bleibt die englische [kanonische README.md](README.md) die vollständige Referenz.

---
**Übersetzungsstatus:** Teilübersetzung – Abschnitte I und III–VII übersetzt. Technische Kennungen, Befehle, Codebeispiele, Dateinamen und Links wurden unverändert beibehalten.
