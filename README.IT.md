# SLeeLa 3.0 — Italia (IT)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>

<p align="center"><strong>Un linguaggio nativo per sistemi, con compilatore, ambiente di esecuzione ed ecosistema di macchine virtuali.</strong></p>

> **Edizione internazionale per paese.** Questo file raccoglie le versioni linguistiche previste per l’Italia. La traduzione tecnica viene realizzata per fasi. Le sezioni tradotte sono riportate integralmente; quelle ancora da tradurre sono indicate in modo esplicito.

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

**Traduzione in sospeso.** Gli annunci completi delle modifiche e delle funzionalità della sezione II inglese saranno tradotti in una prossima fase. Fonte di riferimento: [README.md — sezione II](README.md).

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

## Sezioni ancora da tradurre

Le seguenti sezioni non sono ancora tradotte e saranno aggiunte nelle prossime fasi: **II. Novità; VIII. Temi dell’editor SLeeLa; IX. Contribuire; X–XVII. Altre aree del progetto; XVIII. Aggiunte recenti; XIX. Edizioni internazionali del README.** Fino al completamento della traduzione, il [README.md canonico in inglese](README.md) rimane il riferimento completo.

---
**Stato della traduzione:** traduzione parziale — sezioni I e III–VII tradotte. Identificatori tecnici, comandi, esempi di codice, nomi di file e collegamenti sono stati mantenuti invariati.
