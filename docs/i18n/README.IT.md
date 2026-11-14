<img align="right" src="../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — Italia (IT)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Un linguaggio nativo per sistemi, con compilatore, ambiente di esecuzione ed ecosistema di macchine virtuali.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="../../docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Edizione internazionale per paese.** Questo file raccoglie le versioni linguistiche previste per l’Italia. La traduzione tecnica viene realizzata per fasi. Le sezioni tradotte sono riportate integralmente; quelle ancora da tradurre sono indicate in modo esplicito.

**The Now. The Ingredient.** Un linguaggio di programmazione è più della sua sintassi: comprende il compilatore, l’ambiente di esecuzione, le librerie, gli strumenti e i contratti di piattaforma che rendono il software concretamente utilizzabile.

**Aree di lavoro di SLeeLa.** SLeeLa affronta **Math**, **Object**, **Science** e **Reach**, le quattro dimensioni del suo lavoro: ciò che è calcolabile (Math), l’oggetto definito nel catalogo di ciò che esiste (Object), lo studio disciplinato del vero (Science) e l’estensione delle capacità di un sistema verso altri sistemi e persone (Reach). **Gli interessi nazionali restano validi**: nessuna di queste quattro dimensioni li sostituisce. Quando un argomento riguarda gli interessi di una nazione, tale relazione viene mantenuta e documentata fedelmente, ad esempio nei campi di metadati di una classe `metaOrigin` e `metaNationalSchedule`.

## I. Per iniziare

- [Compilare l’implementazione nativa](../../impl/README.md)
- [Macchine virtuali e Modium — guida pratica per l’utente](../../virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [Stack CPU nel codice sorgente SLeeLa](../../lib/cpu/CPU.md): una catena completa, dalle porte logiche all’ALU, ai core, al sistema operativo e ai programmi; comprende oltre 130 profili CPU e [tensioni, frequenze variabili e parametri avanzati di temporizzazione](../../lib/cpu/TIMING.md), con esecuzione [secondo specifica oppure in overclock](../../lib/cpu/components/README.md).
- [Indice della documentazione](../../docs/README.md)
- [Fonte dati CSPML e riferimento di ricerca Salt & Salary](../reference/CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md): provenienza e indicazioni d’integrazione per dati esterni statunitensi su località, condizioni socioeconomiche, forza lavoro, istruzione e scuole pubbliche.
- [Documentazione API generata](../../docs/api/README.md): riferimento HTML corrente, versione dell’API e istruzioni per rigenerarla.
- [Catalogo API](../../api/README.md)
- [Standard di confronto computazionale](../../comparison/README.md): standard per tipi, memoria, ABI e conformità di SLeeLa VM, Java/JVM e C/C++ nativo.
- [Stanchion della ricorsione — prototipi C e C++](../../recursion/stanchion/README.md): esempi specializzati per i tipi primitivi Boolean, Byte, Character, Integer, Long, Float e Double, oltre a modelli di ricorsione generici, applicati, scientifici e didattici.
- [Matrice di supporto e convalida delle piattaforme](../../docs/PLATFORM-SUPPORT.md)
- [Guida del marchio](../../docs/BRAND-GUIDE.md)
- [Concetti e risorse grafiche per il merchandising](../../merchandise/README.md)
- [Logo NitroWebExpress](../../images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Politica di test e criteri di rilascio](../../markdown/TESTING.md)

## II. Novità

Aggiunte recenti al linguaggio, ai driver e alla libreria standard:

- **Pacchetto della libreria standard `advantage` — ottimizzazione di un dividendo con matematica, fisica, integrazione, valutazione e output instradabile.** [`lib/advantage/`](../../lib/advantage/README.md) racchiude l’*ottimizzazione di un dividendo unico rispetto a un modello* in una struttura a strati **Sequitur onion** (dividendo → revisione → questioni → matematica → aggiunte → sottrazione → raggruppamento → iterazione), percorribile verso l’interno e verso l’esterno con aggiornamento dinamico e restituzione completi, tramite un semplice **modello utente** e un **modello sviluppatore** a controllo completo. Comprende cinque aree: **`math/`**, economia del commercio bilaterale (massimizzazione congiunta del profitto, negoziazione di Nash, rapporto critico bilaterale del modello newsvendor); **`physics/`**, una «fisica dei campi di fiducia» applicata all’economia strutturale (fiducia come tensore metrico, geodetica transazionale, struttura di contatto degli annunci e SDE CIR con salti per il decadimento della fiducia, esplicitamente indicata come analogia di modellazione); **`integration/`**, modello di integrazione verticale componibile ricorsivamente per capitalismo e vendita al dettaglio (compartimenti, baie di ingegneria, compartimenti di prova, vassoi di mediana newtoniana e punteggio di sofisticazione del sistema); **`grading/`**, valutatore di circuiti di sofisticazione che assegna valutazioni A–D a ciascun modulo e a insiemi di moduli vincenti, perdenti, valutatori o assoluti; e **`output/`**, interfaccia di emissione instradabile verso terminale, superficie in formato **CommonRails** o socket/connettore (predisposta per un `slnet_write` nativo). Ogni classe contiene il record Metadata-Class; cinque demo eseguibili convalidano le aree. Documentazione complementare: [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md), [`MATH.md`](../../lib/advantage/MATH.md) e [`PHYSICS.md`](../../lib/advantage/PHYSICS.md). Un consumatore nativo C/C++ in [`advantage/`](../../advantage/README.md) — nucleo C `advantage_value`/`advantage_mean`, archivio di conoscenza XML e libreria sicura di acquisizione HTTPS/SFTP/SSH, keystore e trasporto DH/ECDH — collega il modello a [esempi commerciali](../../advantage/examples/README.md): transazioni positive da presunte fonti Internet, una sequenza di rilevanza per Traders, Consumers, Helpers e Healers e otto modelli classificati da A a D (D = massimo Cement, A = massimo Brand).
- **Annotazioni temporali dei cicli — `@loopDelay` / `@loopBudget` e funzione integrata `sleep(ms)`.** Sono le prime annotazioni **comportamentali a livello di istruzione** di SLeeLa: scritte su una riga autonoma prima di un ciclo `for`, `while` o `do`, inseriscono una pausa reale nell’esecuzione. `@loopDelay(ms = 250)` / `@loopDelay(seconds = 2)` attende una quantità fissa per iterazione (la forma abbreviata `@loopDelay(250)` usa i millisecondi per impostazione predefinita); `@loopBudget(seconds = 10, iterations = 1000)` distribuisce un numero noto di iterazioni su un budget totale di tempo reale. Entrambe vengono abbassate al nuovo opcode append-only **`OP_SLEEP`** (ISA 157), che supporta anche la funzione integrata generale **`sleep(<ms>)`**, richiamabile ovunque. La pausa è implementata in modo portabile tramite `nanosleep` / `Sleep`. Specifica: [`SLEELA.syntax` §32](../../SLEELA.syntax); riferimento API: [`api/sleela/3.0/LOOP_TIMING_API.md`](../../api/sleela/3.0/LOOP_TIMING_API.md); demo eseguibile: [`examples/loop-timing-annotation-demo.sleela`](../../examples/loop-timing-annotation-demo.sleela). L’indice delle aggiunte di sessione — che include anche la moltiplicazione nativa di grandi interi `OP_BIGMUL` e gli strumenti autonomi [local-generator](../../local-generator/README.md) e [pi-generator](../../pi-generator/README.md) — si trova in [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](../../api/sleela/3.0/SESSION-ADDITIONS-API.md). Le tre superfici API generate ([`sleeladoc-api/`](../../sleeladoc-api/README.md), [`docs/api/`](../../docs/api/README.md) e lo snapshot immutabile [`api/sleela/3.0/version-014/`](../../api/sleela/3.0/version-014/index.html)) sono state ricostruite con `make api-docs` e ora includono le voci `SLISASleep` / `SLISABigMul` nella pagina del pacchetto `vm`.
- **SleelaDoc dalla riga di comando — `sleela docs`.** SleelaDoc è ora disponibile direttamente dal terminale. `sleela docs` elenca tutti gli argomenti documentabili; `sleela docs crypto` mostra un pacchetto; `sleela docs stringbuilder` risolve una classe senza distinzione tra maiuscole e minuscole e riconosce il prefisso standard `SL` come `SLStringBuilder`. Visualizza una pagina dettagliata con **definizione, finalità progettuale, relazioni, struttura, user clays e una scala di competenza a cinque livelli**, oltre a suggerimenti ordinati «intendevi?» per nomi simili. Gli autori arricchiscono le pagine aggiungendo tag SleelaDoc `@` come `@intent`, `@thesis`, `@axiom`, `@param`, `@see` e `@mastery`. `sleela docs --generate [dir]` genera un corpus in testo semplice (una pagina per classe e un indice `TOPICS.index`) che la CLI può leggere offline; **`make sleeladoc`** raggruppa il corpus e i corrispondenti sorgenti `.sleela` in `sleeladoc/build/sleeladoc.jar`. Per argomenti con la sola facciata, come `psychiatry` e `politico`, `sleela docs --facade <topic>` esplora il filesystem durante l’esecuzione ed elenca le cartelle effettive e poi i file, con limiti configurabili dall’utente di **6 MB, 6000 file e 6 minuti**. Le unità di `lib/sleeladoc` adottano l’edizione corrente **`#sleela 3.0`**. Vedere [`man/man1/sleela.1`](../../man/man1/sleela.1) e [`lib/sleeladoc/README.md`](../../lib/sleeladoc/README.md).
- **SleelaDoc — documentazione in stile Javadoc con sito HTML generato.** **SleelaDoc** è la convenzione per i commenti documentali nel sorgente SLeeLa (commenti tra parentesi angolari delimitati da `@doc` / `@page`; vedere [`manifesto/SLEELADOC.md`](../../manifesto/SLEELADOC.md)). [`lib/sleeladoc`](../../lib/sleeladoc/README.md) è l’**estrattore** che trasforma un’unità `.sleela` in una pagina API Markdown o HTML, come `javadoc` per Java, documentando l’*intento*. Un sito HTML completo e navigabile per l’intera directory `/lib` viene generato in [`/sleeladoc-api`](../../sleeladoc-api/README.md) da `tools/generate-sleeladoc-api.py` (aprire `sleeladoc-api/index.html`); utilizza uno stile pulito blu, azzurro, grigio chiaro e bianco, una gerarchia pacchetto → classe → membro e un glossario per il lettore *Student 2*. Il compilatore produce anche, come risultato collaterale della compilazione, documenti contrassegnati dall’autore (`--wiki`: pagina MediaWiki e copia gemella del sorgente originale), descritti in [`manifesto/SLEELADOC.WIKI.md`](../../manifesto/SLEELADOC.WIKI.md). Demo eseguibile: [`lib/sleeladoc/sleeladoc-demo.sleela`](../../lib/sleeladoc/sleeladoc-demo.sleela).
- **Modello a oggetti del documento XML (`lib/xml`).** DOM XML lato SLeeLa: un documento possiede un albero di nodi elemento/testo/commento/CDATA/istruzione di elaborazione, con figli ordinati e attributi denominati; include un serializzatore compatto o leggibile (escaping XML 1.0 §2.4), un parser basato su celle di caratteri e il registro di qualità `SLXmlAspect`. L’uguaglianza dei nodi segue tre livelli di fiducia (identità → stesso soggetto → struttura) ed è riflessiva, simmetrica e deterministica. Vedere [`lib/xml/README.md`](../../lib/xml/README.md) e l’esempio eseguibile [`lib/xml/xml-dom.sleela`](../../lib/xml/xml-dom.sleela).
- **Primitive intere con segno a larghezza estesa (`long`, `int64`, `int128`).** `int`, `long` e `int64` sono interi con segno a 64 bit e vengono verificati dal sistema dei tipi ed eseguiti in modo identico. `int128` è accettato come tipo intero (per ora basato su 64 bit; il vero supporto a 128 bit è documentato come lavoro futuro). Le serie con segno in base 2 da `int32` a `int16777216` (sintassi 2.7) e senza segno da `uint32` a `uint16777216` (sintassi 2.8) estendono queste capacità. Vedere [`SLEELA.syntax`](../../SLEELA.syntax) §21, §24 e §25.
- **Campi conseguenti `@mix` (sintassi 2.9).** Una classe può dichiarare più campi di istanza con lo **stesso nome** quando ogni ripetizione include `@mix`. Il linguaggio interpreta il gruppo come sequenza indicizzata (array/lista), dall’alto verso il basso `1,2,3,…` per impostazione predefinita o manualmente tramite `@mix(1,2,3,...)`; il numero di indici deve corrispondere al numero di campi. Vedere [`SLEELA.syntax`](../../SLEELA.syntax) §28.
- **Marcatori documentali — il Processo Naturale, denominato Preonems (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).** Queste parole nude che fungono da marcatori documentali sono chiamate collettivamente **Preonems** (un marcatore è un *Preonem*; l’insieme è costituito dai *Preonems*; vedere [`SLEELA.syntax`](../../SLEELA.syntax) §26.1). Sono marcatori dichiarativi di prima classe che indicano che cosa *è* un documento nell’ordine della sua creazione: `author`/`authorial` (sintassi 2.4; gestito dal consiglio professionale, l’unico membro con effetto comportamentale), `material;` (Source stabilita) e `memory;` (sintassi 2.5), `determine;` (sintassi 2.6; documento aperto a ulteriore elaborazione da parte di un dottore, medico o ghost fino al completamento per via legale), e i due più recenti **`object;`** (cosa stabilita nel catalogo di ciò che esiste, reificata e dotata di consistenza concreta in sintassi 2.8) e **`field;`** (ambito relazionale in cui gli oggetti esistono e si muovono), entrambi abilitati dalla sintassi 2.6. Ogni marcatore è contestuale e vincolato alla versione nel frontend; non modifica parsing, tipizzazione o generazione del codice. Fuori dalla posizione di marcatore ogni parola resta un identificatore normale; pertanto `field` non interferisce con un membro di classe o con l’accesso `.field`. Vedere [`SLEELA.syntax`](../../SLEELA.syntax) §18–§20, §23 e §26.
- **Nordshrift in sintassi 3.0 con accesso completo alla libreria.** Nordshrift risolve i nomi tramite lo stesso indice ricorsivo `/lib` usato dal compilatore, raggiungendo ogni pacchetto e unità `.sleela`; `nordshrift check` riporta i conteggi dell’intero albero.
- **Stanchion della ricorsione.** [`recursion/stanchion/`](../../recursion/stanchion/README.md) include ora prototipi di riferimento C e C++ dedicati ai sette contratti primitivi documentati, oltre a esempi generici, applicati, scientifici e didattici. Sono sorgenti di riferimento: compilarle e testarle prima di considerarle verificate.
- **Commenti tra parentesi angolari (`<` / `<<`).** Un nuovo stile di commento per sviluppi futuri — `< line >` e `<< block >>` — riconosciuto dal frontend SLeeLa e documentato in [`SLEELA.syntax`](../../SLEELA.syntax) §1.1.
- **Letterali interi con base.** I letterali `int` accettano ora forme esadecimali (`0x`), binarie (`0b`) e ottali (`0o`), con separatori di cifre `_`, sia nel linguaggio sorgente SLeeLa ([`SLEELA.syntax`](../../SLEELA.syntax) §1.4) sia nel formato dei modelli CPU [`.xlt`](../../lib/xlt/README.md).
- **Standard Metadata Class nell’intera libreria.** Il metapattern dei metadati descrittivi (provenienza, legalità, ambito giurisdizionale) è ora presente in ogni file `.sleela` contenente classi sotto `/lib`. Vedere [`SLEELA.syntax`](../../SLEELA.syntax) §15 e [la lezione del tutorial](../../tutorial/13-metadata-class.md).
- **Famiglia Text / String ampliata.** `lib/text` aggiunge helper effettivi nel linguaggio, una vista a celle di caratteri con operazioni reali `charAt`, `substring`, `indexOf`, `reverse` e allineamento delle colonne, oltre a un motore di template posizionale. Vedere il [tutorial Text e String](../../tutorial/14-text-and-strings.md) e la demo eseguibile [`lib/text/text-string-demo.sleela`](../../lib/text/text-string-demo.sleela).
- **`String` di prima classe nei fogli Nordshrift.** Un foglio `.sst` può dichiarare un blocco tipizzato `strings:` che contribuisce al manifesto dei componenti della compilazione. Vedere il [riferimento Nordshrift Sheet](../../wiki/Nordshrift-Sheet-Reference.md).
- **Modelli CPU eseguiti tramite opcode ed equivalenza delle chiamate di funzione.** Ogni `SL<ARCH>CPU` eredita la capacità di eseguire un flusso di istruzioni native tramite il ciclo fetch-decode-execute e anche il codice di un altro processo ipotizzato mediante chiamate equivalenti sull’infrastruttura VM condivisa. Vedere [`lib/cpu/EXECUTION.md`](../../lib/cpu/EXECUTION.md) §5.

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

L’obiettivo `registry` esegue [`build-registry.sh`](../../build-registry.sh): compila il nucleo del registro, inserisce la tabella di base nell’archivio testuale portabile `registry/build/sleela.sleelareg` ed esporta lo script della tabella MySQL `registry/build/registry.sql` (tramite `sleela registry export`). Il registro può essere esaminato con `sleela registry` oppure `registry/build/sleela-registry review`. Consultare la [guida al registro](../../registry/README.md) e l’[architettura Registry 3.0](../../registry/REGISTRY-3.0-ARCHITECTURE.md).

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

In primo luogo viene chiesto se installare il pacchetto principale; successivamente si sceglie l’ambito dell’installazione. In caso di esito positivo, può essere proposto un selettore facoltativo di moduli aggiuntivi selezionati. Il flusso interattivo completo e tutte le opzioni sono descritti in [Installazione rapida e sicura](../../install/README.md).

## V. Linguaggio e runtime

La grammatica normativa si trova in [`SLEELA.syntax`](../../SLEELA.syntax). Il repository documenta attualmente la sintassi **3.0**; la versione del compilatore/degli strumenti e la versione della sintassi del codice sorgente sono distinte. La superficie 3.0 aggiunge i marcatori documentali **`pure` e `gasoline`** (§30): due Preonem contestuali e dichiarativi, privi di effetti di esecuzione autonomi, che esprimono ciascuno la posizione di un documento attraverso la sua stessa onestà. La sintassi 3.0 riunisce le superfici linguistiche dalla 1.0 alla 2.9 in un’unica edizione normativa corrente, mantenendo i requisiti di versione minima per ciascuna funzionalità.

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

Il manifesto di integrità viene generato in `antivirus/build/`. Comprende input di codice sorgente e documentazione tracciati dal repository, oltre ai file protetti degli indici delle firme; esclude gli output generati e la cache ClamAV modificabile. Gli snapshot dei database ClamAV dispongono di manifesti propri. Gli archivi ZIP di rilascio includono `INTEGRITY-MANIFEST.json`, `SHA256SUMS` e un file `.sha256` associato. Confrontare gli hash degli archivi scaricati con un valore attendibile ottenuto indipendentemente: un checksum memorizzato accanto a un archivio non costituisce una firma dell’editore. Consultare la [guida Sleelavirin](../../antivirus/README.md), la [lista di controllo di sicurezza](../../antivirus/SECURITY.md) e il [workflow di build antivirus](../../.github/workflows/antivirus-binaries.yml).

Una release è considerata verificata soltanto quando i job CI pertinenti hanno esito positivo per l’esatto commit della release e gli artefatti risultanti sono stati controllati.

## VIII. Temi dell’editor SLeeLa — Vim ed Emacs

SLeeLa include un insieme coordinato di schemi cromatici scuri e professionali per **Vim** ed **Emacs**, oltre a un’utilità nativa per le palette C/C++. Lo schema originale **American Professional** rimane quello predefinito. Dieci palette complementari offrono ispirazioni progettuali culturali e istituzionali diverse; sono interpretazioni grafiche, non rappresentazioni ufficiali di alcuna nazione, cultura, corte o istituzione.

### Gli undici schemi e i relativi colori

| Schema | Sfondo | Primo piano | Parole chiave | Tipi | Stringhe | Commenti |
|---|---|---|---|---|---|---|
| **American Professional** | <span style="display:inline-block;width:1em;height:1em;background:#101820;border:1px solid #888"></span> `#101820` | <span style="display:inline-block;width:1em;height:1em;background:#F4F7FA;border:1px solid #888"></span> `#F4F7FA` | <span style="display:inline-block;width:1em;height:1em;background:#7FB3FF;border:1px solid #888"></span> `#7FB3FF` | <span style="display:inline-block;width:1em;height:1em;background:#73DACA;border:1px solid #888"></span> `#73DACA` | <span style="display:inline-block;width:1em;height:1em;background:#F2C078;border:1px solid #888"></span> `#F2C078` | <span style="display:inline-block;width:1em;height:1em;background:#8A96A3;border:1px solid #888"></span> `#8A96A3` |
| **Pan-Eurasian** | <span style="display:inline-block;width:1em;height:1em;background:#111827;border:1px solid #888"></span> `#111827` | <span style="display:inline-block;width:1em;height:1em;background:#F0F2F5;border:1px solid #888"></span> `#F0F2F5` | <span style="display:inline-block;width:1em;height:1em;background:#7AA7E8;border:1px solid #888"></span> `#7AA7E8` | <span style="display:inline-block;width:1em;height:1em;background:#69C9B3;border:1px solid #888"></span> `#69C9B3` | <span style="display:inline-block;width:1em;height:1em;background:#D7B56D;border:1px solid #888"></span> `#D7B56D` | <span style="display:inline-block;width:1em;height:1em;background:#8995A5;border:1px solid #888"></span> `#8995A5` |
| **Russian and Slavic — Medium** | <span style="display:inline-block;width:1em;height:1em;background:#202733;border:1px solid #888"></span> `#202733` | <span style="display:inline-block;width:1em;height:1em;background:#E8EAF0;border:1px solid #888"></span> `#E8EAF0` | <span style="display:inline-block;width:1em;height:1em;background:#A9C0DD;border:1px solid #888"></span> `#A9C0DD` | <span style="display:inline-block;width:1em;height:1em;background:#8FC4B2;border:1px solid #888"></span> `#8FC4B2` | <span style="display:inline-block;width:1em;height:1em;background:#D6BD8B;border:1px solid #888"></span> `#D6BD8B` | <span style="display:inline-block;width:1em;height:1em;background:#9AA3AF;border:1px solid #888"></span> `#9AA3AF` |
| **Central Asian / Silk Road** | <span style="display:inline-block;width:1em;height:1em;background:#1C2529;border:1px solid #888"></span> `#1C2529` | <span style="display:inline-block;width:1em;height:1em;background:#F5F0E5;border:1px solid #888"></span> `#F5F0E5` | <span style="display:inline-block;width:1em;height:1em;background:#7FADE0;border:1px solid #888"></span> `#7FADE0` | <span style="display:inline-block;width:1em;height:1em;background:#45C6C2;border:1px solid #888"></span> `#45C6C2` | <span style="display:inline-block;width:1em;height:1em;background:#D9B56D;border:1px solid #888"></span> `#D9B56D` | <span style="display:inline-block;width:1em;height:1em;background:#92A3A0;border:1px solid #888"></span> `#92A3A0` |
| **East Asian** | <span style="display:inline-block;width:1em;height:1em;background:#171A1C;border:1px solid #888"></span> `#171A1C` | <span style="display:inline-block;width:1em;height:1em;background:#F2EFE7;border:1px solid #888"></span> `#F2EFE7` | <span style="display:inline-block;width:1em;height:1em;background:#D97878;border:1px solid #888"></span> `#D97878` | <span style="display:inline-block;width:1em;height:1em;background:#72C7A5;border:1px solid #888"></span> `#72C7A5` | <span style="display:inline-block;width:1em;height:1em;background:#D8B76A;border:1px solid #888"></span> `#D8B76A` | <span style="display:inline-block;width:1em;height:1em;background:#969F9B;border:1px solid #888"></span> `#969F9B` |
| **ICC — Corte penale internazionale** | <span style="display:inline-block;width:1em;height:1em;background:#0E1A2B;border:1px solid #888"></span> `#0E1A2B` | <span style="display:inline-block;width:1em;height:1em;background:#EAF0F7;border:1px solid #888"></span> `#EAF0F7` | <span style="display:inline-block;width:1em;height:1em;background:#5B8FD6;border:1px solid #888"></span> `#5B8FD6` | <span style="display:inline-block;width:1em;height:1em;background:#7FB7C9;border:1px solid #888"></span> `#7FB7C9` | <span style="display:inline-block;width:1em;height:1em;background:#D9B25A;border:1px solid #888"></span> `#D9B25A` | <span style="display:inline-block;width:1em;height:1em;background:#7E8CA0;border:1px solid #888"></span> `#7E8CA0` |
| **SCOTUS — Corte Suprema degli Stati Uniti** | <span style="display:inline-block;width:1em;height:1em;background:#1A1416;border:1px solid #888"></span> `#1A1416` | <span style="display:inline-block;width:1em;height:1em;background:#F3EEE6;border:1px solid #888"></span> `#F3EEE6` | <span style="display:inline-block;width:1em;height:1em;background:#9E2B35;border:1px solid #888"></span> `#9E2B35` | <span style="display:inline-block;width:1em;height:1em;background:#8FA9B8;border:1px solid #888"></span> `#8FA9B8` | <span style="display:inline-block;width:1em;height:1em;background:#C9A24B;border:1px solid #888"></span> `#C9A24B` | <span style="display:inline-block;width:1em;height:1em;background:#8C8178;border:1px solid #888"></span> `#8C8178` |
| **Greek (Hellenic)** | <span style="display:inline-block;width:1em;height:1em;background:#0A1A30;border:1px solid #888"></span> `#0A1A30` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#2E7BD6;border:1px solid #888"></span> `#2E7BD6` | <span style="display:inline-block;width:1em;height:1em;background:#5E9FE0;border:1px solid #888"></span> `#5E9FE0` | <span style="display:inline-block;width:1em;height:1em;background:#9FC3EC;border:1px solid #888"></span> `#9FC3EC` | <span style="display:inline-block;width:1em;height:1em;background:#6E86A6;border:1px solid #888"></span> `#6E86A6` |
| **Turkish (Al Bayrak)** | <span style="display:inline-block;width:1em;height:1em;background:#2A0609;border:1px solid #888"></span> `#2A0609` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#F23D48;border:1px solid #888"></span> `#F23D48` | <span style="display:inline-block;width:1em;height:1em;background:#F0A9AE;border:1px solid #888"></span> `#F0A9AE` | <span style="display:inline-block;width:1em;height:1em;background:#F0C9A9;border:1px solid #888"></span> `#F0C9A9` | <span style="display:inline-block;width:1em;height:1em;background:#A8767A;border:1px solid #888"></span> `#A8767A` |
| **Kiro** | <span style="display:inline-block;width:1em;height:1em;background:#0B0B12;border:1px solid #888"></span> `#0B0B12` | <span style="display:inline-block;width:1em;height:1em;background:#F5F5FA;border:1px solid #888"></span> `#F5F5FA` | <span style="display:inline-block;width:1em;height:1em;background:#A98EDA;border:1px solid #888"></span> `#A98EDA` | <span style="display:inline-block;width:1em;height:1em;background:#6E8BE0;border:1px solid #888"></span> `#6E8BE0` | <span style="display:inline-block;width:1em;height:1em;background:#E8C547;border:1px solid #888"></span> `#E8C547` | <span style="display:inline-block;width:1em;height:1em;background:#6A6482;border:1px solid #888"></span> `#6A6482` |
| **SLeeLa** | <span style="display:inline-block;width:1em;height:1em;background:#08080E;border:1px solid #888"></span> `#08080E` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#B07CFF;border:1px solid #888"></span> `#B07CFF` | <span style="display:inline-block;width:1em;height:1em;background:#5A7CFF;border:1px solid #888"></span> `#5A7CFF` | <span style="display:inline-block;width:1em;height:1em;background:#FFD21E;border:1px solid #888"></span> `#FFD21E` | <span style="display:inline-block;width:1em;height:1em;background:#6E5F9E;border:1px solid #888"></span> `#6E5F9E` |

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
| **American Professional** | ![Palette American Professional](../../lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Palette Pan-Eurasian](../../lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russian and Slavic — Medium** | ![Palette Russian and Slavic Medium](../../lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Central Asian / Silk Road** | ![Palette Central Asian Silk Road](../../lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **East Asian** | ![Palette East Asian](../../lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC** | ![Palette ICC](../../lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS** | ![Palette SCOTUS](../../lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Greek (Hellenic)** | ![Palette greca ellenica](../../lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turkish (Al Bayrak)** | ![Palette turca Al Bayrak](../../lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Palette Kiro](../../lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![Palette SLeeLa](../../lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

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

Vedere la [guida all’integrazione degli editor](../../linux-editors/README.md) per le descrizioni complete delle palette e i dettagli dell’utilità.

## IX. Contribuire

1. Leggere l’indice della documentazione e la politica di test.
2. Apportare modifiche mirate, accompagnate da test positivi e negativi.
3. Eseguire prima i test mirati e poi la suite completa.
4. Indicare i dettagli del sistema operativo e della toolchain; non descrivere i controlli saltati come superati.

SLeeLa è sviluppato da **MEARVK LLC**. Il materiale storico della pagina iniziale è conservato in [`docs/README-LEGACY.md`](../../docs/README-LEGACY.md).

## X. AE6E66 — Aspettativa di prelievo sulle email pubbliche Round

Per l’iniziativa AE6E66, l’aspettativa dichiarata del progetto è di **300.000.000,00 USD per ogni email pubblica Round**, come importo di prelievo fiscale proposto associato a ciascuna email Round Public idonea.

Questa dichiarazione registra soltanto un’aspettativa del progetto. **Non** costituisce un accertamento fiscale legale, un ordine governativo, una sentenza o una passività fiscale accertata in modo indipendente. Qualsiasi prelievo effettivo richiederebbe la pertinente autorità giuridica, giurisdizione, definizioni, procedura e determinazione dell’autorità governativa competente.

## XI. Runtime di alfabeti e set di caratteri

La documentazione dell’alfabeto SLeeLa e il runtime nativo dei caratteri sono mantenuti in `alphabet/sleela/` e `lib/alphabet/native/`. I test nativi C++17 coprono la decodifica/codifica rigorosa degli scalari UTF-8, le firme di capacità `C<n>` e l’appartenenza limitata ai set di caratteri:

```sh
make -C lib/alphabet/native test
```

Il catalogo sperimentale UTF-4088 e il relativo adattatore di ricerca SLeeLa sono documentati in [`utf-4088/SLeeLa-ALPHABET-INTEGRATION.md`](../../utf-4088/SLeeLa-ALPHABET-INTEGRATION.md). Gli ID UTF-4088 appartengono a uno spazio dei nomi separato e non sono valori scalari Unicode. Il lavoro di binding del compilatore e di SLVM è tracciato separatamente da queste basi native.

## XII. AutoCAD Creator™

[**AutoCAD Creator™**](../../autocad/creator/README.md) è un’applicazione CAD 2D interattiva scritta interamente in SLeeLa. Mentre il relativo AutoCAD Renderer trasforma un piano in un disegno in un solo passaggio, Creator consente di **creare e modificare** dal vivo un documento AutoCAD 2D: aggiungere pareti, stanze, quote e note, quindi spostare, rinominare, riassegnare livelli, eliminare e annullare gli elementi inseriti, e **salvare** il risultato come vero disegno DXF AutoCAD (testo R12, apribile in AutoCAD / LibreCAD).

La sua interfaccia grafica è basata su **SleelaUI™**, il toolkit UI multipiattaforma di SLeeLa (in [`lib/user-interface`](../../lib/user-interface)), non su JavaFX, e apre una vera finestra nativa su X11 / Cocoa / Win32. Il design segue una regola: **«SleelaUI presenta; SLeeLa decide»**. La GUI non calcola la geometria e non scrive DXF. Ogni strumento invia un’intenzione di creazione, modifica, eliminazione, annullamento o salvataggio al componente autorevole [`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela), che possiede il documento modificabile e una cronologia reversibile dei comandi; la tela visualizza soltanto un’anteprima della geometria posseduta da Creator.

Il motore di modifica riutilizzabile si trova in [`lib/autocad`](../../lib/autocad): [`SLCadEntity`](../../lib/autocad/SLCadEntity.sleela), [`SLCadDocument`](../../lib/autocad/SLCadDocument.sleela), [`SLCadCommand`](../../lib/autocad/SLCadCommand.sleela) e [`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela). Funziona end-to-end senza interfaccia grafica. Il guscio GUI e la demo si trovano in [`autocad/creator`](../../autocad/creator):

```sh
make creator          # da autocad/: esegue la demo Creator -> out/altered.dxf
```

Su un sistema con display si apre la finestra di modifica dal vivo; su un host headless il disegno viene creato in memoria, viene stampato un rapporto insieme al DXF e il processo termina correttamente.

## XIII. Ordinamento della mappa di input UTF-4088

Il catalogo specializzato UTF-4088 definisce il percorso spaziale della mappa di input e le iterazioni astratte di pressione in [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). La politica dell’indice per materia del National Lien Register è in [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Le coordinate della mappa, i livelli di pressione (1–50), gli ID permanenti del catalogo e i ranghi alfabetici sono campi indipendenti; nessuno deve essere sostituito silenziosamente con un altro. La pressione è un livello di iterazione algoritmica, non una grandezza fisica.

## XIV. Indice dei pacchetti recenti e ampliati

Questo indice aggiunto mette in evidenza pacchetti ampliati di recente, librerie di dominio e raccolte di codice sorgente. Le voci sono raggruppate alfabeticamente per nome del pacchetto o argomento. Le descrizioni indicano la posizione nel repository; implementazione e stato di convalida devono essere verificati nella documentazione e nei test di ciascun pacchetto.

### A–C

- **Advantage** — `lib/advantage/` e `advantage/`: ottimizzazione di un dividendo unico rispetto a un modello tramite una Sequitur onion a strati, con cinque sottocartelle tematiche: `math/` (massimizzazione congiunta dei profitti, negoziazione di Nash, modello bilaterale newsvendor), `physics/` (metrica, geodetica, contatto e SDE del campo di fiducia; analogia di modellazione esplicitamente segnalata), `integration/` (compartimenti di integrazione verticale e punteggio di sofisticazione), `grading/` (valutatore di circuiti di sofisticazione A–D per moduli e insiemi) e `output/` (emissione verso terminale, CommonRails o socket). Un consumatore nativo C/C++ (`advantage/`) collega il modello a esempi commerciali elaborati. Vedere [`lib/advantage/README.md`](../../lib/advantage/README.md) e [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md).
- **Alphabet e UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` e `utf-4088/`: cataloghi di alfabeti, supporto nativo ai set di caratteri, ordinamento delle mappe di input e documentazione d’integrazione UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` e `lib/autocad/`: flussi di disegno 2D interattivi e componenti riutilizzabili per documenti e modifica CAD.
- **Ballum** — controllo del display a livello di pixel: impostazione/lettura dei pixel, blit di regioni, luminosità/temperatura/gamma dell’intero display, input di puntatore/tastiera e **superficie framebuffer a schermo intero** per un controllo totale, proprietario e rapido di ogni pixel (finestra override-redirect e presentazione con una sola chiamata `XPutImage`/MIT-SHM su Linux/X11). Il nucleo nativo è in `ballum/`, la facciata SLeeLa in `lib/ballum/` e il bridge VM in `impl/`; vedere [`ballum/README.md`](../../ballum/README.md) e l’[esempio eseguibile](../../ballum/example/).
- **CDMA** — `cdma/` e `lib/cdma/`: materiale tecnico relativo a CDMA e sviluppo della libreria sorgente SLeeLa.
- **Character** — `lib/character/`: profili dei personaggi e statistiche riutilizzabili, inclusi esempi di profili biografici.
- **Coorenagraph** — `lib/coorenagraph/`: ragionamento etico, vocabolario morale, deliberazione e concetti correlati; vedere il README del pacchetto e i file sorgente.

### D–G

- **Modelli sorgente di governance** — [`lib/governance`](../../lib/governance/README.md) definisce strutture dichiarative SLeeLa per proposte, schede manuali anonime e conteggi aggregati. L’annotazione proposta `@vote` e il processo manuale sono documentati in [`registry/DEMOCRATIC-VOTING-SYNTAX.md`](../../registry/DEMOCRATIC-VOTING-SYNTAX.md); i compilatori attuali la trattano come metadati di commento inerti.

### H–I

- **HTTP 1.0–8.0** — da `lib/http-1.0/` alle directory disponibili dei pacchetti HTTP versionati: specifiche di protocollo e implementazioni specifiche per versione. Consultare la documentazione di ciascuna versione per l’ambito preciso e lo stato di verifica.
- **Corte penale internazionale** — `lib/international-criminal-court/` e `INTERNATIONAL.CRIMINAL.COURTS.md`: sorgenti e documentazione specifiche del dominio.

### O–S

- **OFDMA** — `ofdma/`: materiale tecnico sull’accesso multiplo a divisione di frequenza ortogonale e risorse di sviluppo C/C++.
- **Filosofia e modulo Conversation Mask** — [`philosophy/`](../../philosophy/README.md) contiene le basi native C/C++; [`philosophy/module/`](../../philosophy/module/README.md) organizza il livello successivo del modulo in tre aree esplicite: [`data/`](../../philosophy/module/data/README.md), per osservazioni tipizzate e datate, conclusioni degli eventi, impostazioni dell’emivita, provenienza e consenso; [`helper/`](../../philosophy/module/helper/README.md), per convalida deterministica, decadimento della rilevanza, confronti compatibili e presentazione filtrata per consenso; e [`psychiatry-friend/`](../../philosophy/module/psychiatry-friend/README.md), per un’interfaccia conversazionale di supporto non clinica. Gli handle rivolti a SLeeLa e il Tree composto si trovano in [`lib/philosophy/`](../../lib/philosophy/README.md), accanto al più ampio pacchetto philosophy. Il modulo utilizza input espliciti e dà priorità al consenso: non monitora passivamente persone o media, non diagnostica, non deduce intenzioni private, non assegna punteggi al valore umano e non raccomanda modifiche ai farmaci. I collegamenti tra eventi preservano provenienza e navigazione, ma non costituiscono corroborazione indipendente.
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

Il [generatore del catalogo UTF-4088](../../utf-4088/tools/generate_catalogue.py) crea in modo deterministico un catalogo sperimentale di simboli da 1.048.576 voci. Il formato compatto documentato è descritto in [`utf-4088/FORMAT.md`](../../utf-4088/FORMAT.md); il contratto di generazione e convalida si trova in [`utf-4088/GENERATION-RESULTS.md`](../../utf-4088/GENERATION-RESULTS.md).

Eseguire il generatore con Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Il binario generato è di 16.777.244 byte ed è progettato per rimanere sotto i 20.000.000 byte. I test unitari e un workflow GitHub Actions verificano il numero di voci, l’unicità dei glifi, la dimensione e SHA-256; le esecuzioni riuscite pubblicano il binario e il riepilogo come artefatto del workflow `utf4088-million-catalogue`. Il binario generato viene deliberatamente escluso dalla cronologia Git. UTF-4088 rimane sperimentale e non è compatibile con Unicode.

---

# SLeeLa
<p align="center"><img src="../../images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Aggiunte recenti — costante Pi, avvio Modium/Registry, math di sleelactl e `@native`

Questa sezione registra le funzionalità aggiunte durante il lavoro del 2026-11, a partire dai gruppi più recenti. I file sorgente collegati e il relativo stato di validazione fanno fede; le modifiche in revisione non sono descritte come integrate finché non raggiungono `master`.

### La costante di catalogo Pi da 100.000.000 di cifre

Il repository include un valore verificato di π da **cento milioni di cifre** in [`pi-generator/pi.100000000.digits`](../../pi-generator/pi.100000000.digits) (un decimale ben formato, `3.14159…`), insieme a un valore da 57.344 cifre in [`pi-generator/pi.57344.digits`](../../pi-generator/pi.57344.digits). Entrambi sono valori autentici di π verificati in modo indipendente. [`pi-generator/VERIFIED.md`](../../pi-generator/VERIFIED.md) documenta i controlli: prefisso di riferimento, punto di Feynman alla posizione frazionaria 762, cifra famosa alla posizione un milione, corrispondenza con `lib/math/SLPiReference.sleela`, analisi della frequenza delle cifre sull’intero file e script di riproduzione in un solo passaggio.

### Modium™ / Registry™ caricano la costante Pi all’avvio del sottosistema

Nei punti di ingresso dedicati — `sleela registry` e `sleela modium` (e al primo avvio dell’installer), **non** a ogni `sleela run`, i sottosistemi Modium e Registry caricano la grande costante del catalogo π. In un terminale interattivo viene mostrata una riga rossa temporanea `Loading Modium™` / `Loading Registry™`, poi cancellata sul posto prima dell’output normale. L’effetto è strettamente limitato ai TTY, quindi l’output reindirizzato e quello dei test restano identici byte per byte. Il localizzatore (non i byte) viene inizializzato come valore predefinito delle chiavi `Math` del registro `piReferenceLargePath` e `piReferenceLargeDigits`, sia nel modello SLeeLa ([`lib/registry/SLRegistry.sleela`](../../lib/registry/SLRegistry.sleela)) sia nel seeder nativo ([`registry/sleela_registry.c`](../../registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` espone `largePath()` / `largeDigits()`. La costante π viene installata nella posizione SLeeLa del sistema operativo (`<data>/pi/`) sia dall’installer standard `make` sia dall’installer guidato «beautiful». Dettagli: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](../../pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — installer «beautiful» e subject `math` supportato dal nativo

- L’installer guidato («beautiful») ora installa `sleelactl` (il control plane di SLeeLa) e il relativo registro dei moduli, come l’installer `make`. Vedere [`bin/SLEELACTL.md`](../../bin/SLEELACTL.md).
- `sleelactl` ha acquisito una nuova natura **`subject`** per le librerie subject native C/C++ compilate nel runtime e registra la prima: **`math`**. `sleelactl run math` esegue end-to-end il backend nativo `__NativeMath` (`impl/subjects/math/math_native.cpp`) tramite la demo di riferimento del subject; `sleelactl info math` spiega come raggiungerlo (`import math;`). La natura subject offre solo `run` e `info`: non esiste deliberatamente un verbo `check`, perché `sleela check` non carica i subject nativi registrati tramite SHEET.

### `@native`, Native Drive ed esecuzione multilingue

Una nuova annotazione di metodo **`@native`** richiede la compilazione nativa anticipata (*ahead-of-time*) di un metodo ad alto carico. Il nuovo comando `sleela aot` individua i metodi `@native`, verifica che ciascuno rientri in un *sottoinsieme offloadable* supportato ed emette C/C++. Il processo **fallisce in modo chiuso**: rifiuta con una motivazione precisa qualsiasi costrutto che non sia ancora traducibile, così da non produrre mai un metodo tradotto solo parzialmente.

```sh
sleela aot --list <file.sleela>    # verdetto per metodo: ok / REFUSED (motivo)
sleela aot --emit <file.sleela>    # emette C/C++ (stdout) per un programma nel sottoinsieme
```

`@native` resta una richiesta di trattamento AOT, con i costrutti non supportati rifiutati prima dell’emissione. Il [Native Drive](../../impl/native-drive/README.md) supporta build multi-unità di eseguibili e librerie condivise C/C++ tramite i compilatori dell’host (incluso MSVC), l’output AOT SLeeLa supportato, l’ispezione dei simboli rispetto all’ABI del loader nativo, l’esecuzione di processi senza shell, la gestione di stdin/stdout/stderr, il piping a più stadi e la delega XClass al loader di produzione. `sleelactl` ne espone i comandi di build ed esecuzione e instrada l’ispezione XClass e il rendering/l’installazione espliciti delle unità systemd tramite `inspect native-drive`. La VM possiede una tabella limitata di handle di moduli nativi, con autorizzazione obbligatoria dell’host e handle verificati per generazione. La sintassi 3.0 aggiunge ora `nativeOpen`, `nativeCall` e `nativeClose`: il dispatch di bytecode di sola aggiunta attraversa un’ABI con versione indipendente (null/bool/int64/double/UTF-8), mentre array, struct e altri valori compositi posseduti dalla VM restano all’interno dell’interprete e possono usare come ripiego il bytecode omonimo. I plugin possono facoltativamente ricevere una tabella di callback dell’host versionata per invocare funzioni SLeeLa esatte tramite esecuzione annidata della VM, con rooting GC degli stack sospesi, una allowlist esplicita dei nomi di callback dell’host e rifiuto delle chiamate ricorsive all’API nativa. Il codice nativo resta codice fidato eseguito nel processo, non una sandbox. Le annotazioni di policy dei processi nel sorgente hanno uno schema versionato ma non sono ancora analizzate né applicate. Dettagli, confini di sicurezza e registro dei sette flussi di lavoro: [Native Drive](../../impl/native-drive/README.md) e [`pi-generator/AT-NATIVE.md`](../../pi-generator/AT-NATIVE.md).


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

**Politica di traduzione:** questi file sono punti di ingresso localizzati iniziali per ciascun paese. Le introduzioni e le intestazioni linguistiche sono localizzate, mentre il riferimento tecnico completo resta il [README.md canonico in inglese](../../README.md). Un file nazionale non deve essere descritto come traduzione completa finché tutte le sezioni non sono state tradotte, gli identificatori tecnici e i collegamenti non sono stati verificati e l’edizione non ha ricevuto una revisione linguistica. La copertura linguistica di questa prima fase è rappresentativa e non afferma che tutte le lingue riconosciute costituzionalmente, regionali, indigene o dei segni siano state tradotte integralmente.

**Stato della traduzione:** le sezioni I–XIX sono tradotte in italiano. Identificatori tecnici, comandi, esempi di codice, nomi di file, collegamenti e indirizzi sono stati conservati. Si raccomanda una revisione linguistica finale da parte di madrelingua.
