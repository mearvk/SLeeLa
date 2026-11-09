# SLeeLa 3.0 — Eesti (EE)

## Eestikeelne väljaanne

SLeeLa on natiivne süsteemide programmeerimiskeel koos kompilaatori, käituskeskkonna ja virtuaalmasinate ökosüsteemiga. Projekt hõlmab ka teeke, arendustööriistu ning platvormidega integreerimise lepinguid. Tehnilised identifikaatorid, käsud, failiteed, failinimed, koodinäited ja API-nimed jäävad muutmata, et juhiseid saaks vahetult kasutada.

**The Now. The Ingredient.** Keel ei ole ainult süntaks: selle juurde kuuluvad kompilaator, käituskeskkond, teegid, tööriistad ja platvormilepingud, mis muudavad tarkvara kasutatavaks.

SLeeLa kirjeldab oma tegevusvaldkondi nelja mõõtme kaudu: **Math**, **Object**, **Science** ja **Reach**. Asjakohased riiklikud huvid säilivad selgesõnaliste metaandmetena ega kao selle jaotuse tõttu.

## I. Alustamine

- [Natiivse teostuse ehitamine](impl/README.md).
- [Virtuaalmasinate ja Modiumi kasutusjuhend](virtual-machine/USER-GUIDE.md), sealhulgas käsud `sleela vm` ja `sleela modium`.
- [CPU arhitektuur SLeeLa lähtekoodis](lib/cpu/CPU.md): loogikaväravatest ja ALU-st tuumade, operatsioonisüsteemi ning programmideni; kataloogis on üle 130 CPU-profiili. Vaata ka [ajastust, pingeid ja sagedusi](lib/cpu/TIMING.md).
- [Dokumentatsiooni register](docs/README.md).
- [CSPML-i andmeallikas ja Salt & Salary uurimisviide](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md).
- [Genereeritud API-dokumentatsioon](docs/api/README.md) ja [API-kataloog](api/README.md).
- [Arvutusliku võrdluse standardid](comparison/README.md): tüübid, mälu, ABI ning SLeeLa VM-i, Java/JVM-i ja natiivse C/C++ vastavus.
- [Recursion stanchion'i C- ja C++-viiteprototüübid](recursion/stanchion/README.md).
- [Platvormitugi ja valideerimismaatriks](docs/PLATFORM-SUPPORT.md).
- [Brändijuhend](docs/BRAND-GUIDE.md).
- [Testimispoliitika ja väljalaske tingimused](markdown/TESTING.md).

## II. Funktsioonid ja uudised

Standardteegi pakett **Advantage** (`lib/advantage/`) modelleerib dividendi analüüsi mudeli suhtes. See on kihiline ning hõlmab review, concerns, maths, additions, subtraction, regrouping ja iteration osi. Teemadeks on kahepoolse kauplemise majandus, usaldusväljade struktuurne analoogia füüsikas (selgelt mudelianaloogiana märgitud), vertikaalne integratsioon, moodulite hindamine ja marsruuditav väljund. Vaata [Advantage'i dokumentatsiooni](lib/advantage/README.md).

Muud dokumenteeritud funktsioonid:

- **Tsüklite ajastus:** `@loopDelay` ja `@loopBudget` väljendavad tsüklite pause ja ajakulu eelarveid; teostus kasutab `OP_SLEEP`-i ja sisseehitatud funktsiooni `sleep(ms)`. Vaata [süntaksi §32](SLEELA.syntax) ja [Loop Timing API-t](api/sleela/3.0/LOOP_TIMING_API.md).
- **SleelaDoc:** lähtekoodi dokumenteerimise tava ja tööriistad Markdown/HTML-lehtede, teemaregistrite ning API-dokumentatsiooni loomiseks. Käsk `sleela docs` loetleb teemad ja kuvab pakettide ning klasside dokumentatsiooni. Vaata [SleelaDoc-i](lib/sleeladoc/README.md).
- **XML DOM:** XML-sõlmede puu elementide, teksti, kommentaaride, CDATA, töötlemisjuhiste, järjestatud laste ja atribuutidega ning parseri ja serialiseerijaga. Vaata [XML DOM-i](lib/xml/README.md).
- **Laiendatud täisarvutüübid:** dokumentatsioon hõlmab `long`-, `int64`- ja `int128`-tüüpe ning märgiga ja märgita täisarvuperekondi. Tegelik laius tuleb kontrollida spetsifikatsioonist, mitte järeldada üksnes nimest.
- **`@mix`-väljad:** korduvad sama nimega eksemplariväljad saab spetsifikatsiooni reeglite järgi koondada indekseeritud jadaks.
- **Preonemi dokumendimarkerid:** näiteks `author`, `authorial`, `material`, `memory`, `determine`, `object` ja `field`. Tähendus sõltub kontekstist ja süntaksiversioonist; markeripositsioonist väljaspool jäävad need tavalisteks identifikaatoriteks.
- **Nordshrift:** lahendab paketid ja lähtekoodiüksused sama rekursiivse `/lib`-indeksi abil nagu kompilaator.
- **Nurksulgudes kommentaarid:** `< ... >` ja `<< ... >>` on dokumenteeritud kommentaarivormid.
- **Radix-arvuliteraalid:** süntaks toetab kuueteistkümnend-, kahend- ja kaheksandarve ning numbrirühmade eraldajaid.

Süntaksi normatiivne allikas on [`SLEELA.syntax`](SLEELA.syntax). Funktsiooni kirjeldamine dokumentatsioonis ei tähenda, et kõik kompilaatoriversioonid seda toetavad.

## III. Ehitamine ja testimine

**Eeltingimused:** C/C++ tööriistaahel (GCC või Clang), GNU Make ning Python 3 manifesti- ja töövootööriistade jaoks. Linuxi graafikatooted võivad vajada X11/Xrandr arenduspakette. Konkreetse mooduli sõltuvused on kirjas selle dokumentatsioonis.

Käivita hoidla juurkataloogis:

```sh
make
```

Ainult natiivse tuuma ehitamiseks ja kogu testikomplekti käivitamiseks:

```sh
make -C impl all
bash ./test-suites/run-all.sh --all
```

Levinud Make'i sihid:

```sh
make all
make registry
make tests
make ballum
make help
make clean
```

Juurkataloogi ehitussiht koordineerib hoidla toodete ehitamist. `registry` ehitab registri tuuma ja loob selle lähtetabeli. Vaata [Registry juhendit](registry/README.md) ja [Registry 3.0 arhitektuuri](registry/REGISTRY-3.0-ARCHITECTURE.md).

Ühe komponendi edukas ehitamine ei tõenda, et kõik teegid, platvormid või konfiguratsioonid on väljalaskeks valmis. Valideerimist tuleb hinnata mooduli ja platvormi kaupa. Vahele jäetud või käivitamata kontrolle ei tohi märkida edukaks.

## IV. Paigaldamine

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

Tavaline süsteemipaigaldus kasutab enamasti katalooge `/usr/local/bin`, `/usr/local/lib/sleela` ja `/usr/local/share/sleela`. `PREFIX="$HOME/.local"` võimaldab paigaldada kasutaja enda alasse, tavaliselt ilma `sudo`-ta. `DESTDIR` võimaldab koostada pakenduskataloogi süsteemi otse muutmata.

Ainult põhipaketi juhendatud paigaldamiseks — käsud `sleela`, `sleelvac`, `nordshrift`, standardteek, protokollid ja tugimoodulid — kasuta:

```sh
make clean-install
```

Vaata [kiire ja turvalise paigalduse juhendit](install/README.md). Paigaldaja säilitab olemasoleva käivituskeskkonna konfiguratsiooni ega kirjuta üle käske, mida ta ei halda. Eemaldamine peaks piirduma paigaldaja registreeritud failidega.

## V. Keel ja käituskeskkond

Hoidla dokumentatsioon vastab **SLeeLa 3.0** süntaksile. Kompilaatori versioon ja lähtekoodis deklareeritud süntaksiversioon on eri mõisted. Versioon 3.0 koondab keele funktsioonid versioonidest 1.0 kuni 2.9 ning säilitab iga funktsiooni minimaalse süntaksiversiooni nõude.

SLeeLa sisaldab natiivset teostust, kompilaatorit, käituskeskkonda, virtuaalmasinaid, dokumenteerimistööriistu ja pakettideks korraldatud teeki. Hoidlas on nii SLeeLa lähtekood kui ka natiivsed C/C++ komponendid. Natiivkoodi ja käituskeskkonna piir peab järgima dokumenteeritud ABI-lepinguid.

## VI. C, C++ ja natiivne kompileerimine

Annotatsioon `@native` ja käsk `sleela aot` tähistavad eeltõlke varajast ja konservatiivset etappi. Nendega saab loetleda märgistatud meetodeid, kontrollida nende kuulumist toetatud alamhulka ning väljastada toetatud juhtude jaoks C/C++ koodi:

```sh
sleela aot --list <file.sleela>
sleela aot --emit <file.sleela>
```

Toetamata konstruktsioonid lükatakse koos põhjendusega tagasi, mitte ei väljastata eksitavat osalist tõlget. `@native` üksi ei muuda programmi käitumist. Jagatud teekide ehitamist või VM-ist natiivkoodi väljakutsumist ei tohi pidada lõpetatuks ilma teostuse ja testide tõenduseta. Vaata [AT-NATIVE](pi-generator/AT-NATIVE.md).

## VII. Teegid ja alamsüsteemid

- **CPU** (`lib/cpu/`): protsessorimudelid, profiilid, täitmine ja ajastus.
- **Ballum** (`ballum/`, `lib/ballum/`): pikslitaseme ekraanijuhtimine, piirkonnad, heledus, gamma ning klaviatuuri ja osutusseadme sisend; saadavus sõltub platvormist ja taustsüsteemist. Vaata [Ballum](ballum/README.md).
- **AutoCAD Creator™** (`autocad/creator/`): SLeeLa-s kirjutatud interaktiivne 2D CAD-rakendus objektide, kihtide, mõõtude ja märkuste loomiseks, toimingute tagasivõtmiseks ning DXF-jooniste salvestamiseks. Geomeetria ja muudatuste ajalugu kuuluvad redigeerimismudelile, mitte kuvakihile. Vaata [AutoCAD Creatorit](autocad/creator/README.md).
- **Alphabet ja UTF-4088:** märgikataloog ja eksperimentaalsed tööriistad. UTF-4088 kasutab oma identifikaatoriruumi ega ole **Unicode'iga ühilduv**; selle identifikaatoreid ei tohi pidada Unicode'i skalaarväärtusteks.
- **Pi Generator:** π väärtuste kataloogid ning kontrolli ja taasesituse tööriistad.
- **Modium ja Registry:** registri alamsüsteemid ja kontrolltööriistad.
- **Philosophy:** natiivsed alused ning andme-, abi- ja vestlusmoodulid. Vestluskomponenti kirjeldatakse mittekliinilise, selgesõnalisel sisendil ja nõusolekul põhinevana; see ei anna diagnoose.
- **CDMA, OFDMA, HTTP ja Wi-Fi:** tehnilised valdkonnad ja versioonitud teegid. Mooduli ulatuse ja valideerimisoleku kohta vaata selle dokumentatsiooni.

Pakettide kataloog on kirjeldav ega asenda konkreetse mooduli lähtekoodi, README-d ja teste.

## VIII. Terviklus, turvalisus ja kontrollimine

Projekt dokumenteerib SHA-256 manifeste, kohalikke tervikluskontrolle ja valikulist ClamAV adapterit Sleelavirini kaudu. Tööriista või manifesti olemasolu ei ole iseenesest turvasertifikaat.

Tehnilised põhimõtted:

1. Komponentide vahelised selgesõnalised lepingud.
2. Teisaldatavusega arvestamine juba kavandamisel.
3. Turvaline käitumine toimingu ebaõnnestumisel.
4. Taasesitatavad tõendid, mis on seotud testitud Git commit'iga.
5. Viite tuvastamise ja käivitamisloa eristamine.
6. Käivitamata, vahele jäetud või blokeeritud testide täpne raporteerimine.

Väljalaske hindamisel kontrolli konkreetse commit'i CI tulemusi, loodud artefakte, asjakohaseid teste ja platvormimaatriksit. Linuxi tulemust ei tohi ilma eraldi tõenduseta laiendada Windowsile või macOS-ile.

## IX. Redaktorid ja kaastöö

SLeeLa pakub süntaksiintegratsiooni ja teemasid **Vimile** ja **Emacsile** ning natiivset C/C++ värvipaleti tööriista. Vaata [redaktorite integratsioonijuhendit](linux-editors/README.md).

Kaastöö tegemisel:

1. Loe [dokumentatsiooni registrit](docs/README.md) ja [testimispoliitikat](markdown/TESTING.md).
2. Tee piiritletud muudatusi ning lisa positiivsed ja negatiivsed testid.
3. Käivita esmalt asjakohased testid ja seejärel sobiv laiem testikomplekt.
4. Märgi kasutatud operatsioonisüsteem, kompilaator ja tööriistaahel.
5. Ära kirjelda vahele jäetud kontrolle edukatena.

SLeeLat arendab **MEARVK LLC**.

## X. Muud projektiteemad

Põhi-README käsitleb ka CSPML-i andmeallikaid ja integreerimisjuhiseid, uurimisviiteid, kaubakontseptsioone ja visuaalseid materjale ning teemasid nagu Character, Governance, Synchro, SleelaUI ja United Nations. Need viited kirjeldavad hoidla sisu ega tähenda valitsuse või institutsiooni ametlikku heakskiitu.

Hoidlas on ka jaotis **Bitcoin Conjegeum**, mis sisaldab aadresse ja graafilisi materjale. Nende olemasolu ei ole finantssoovitus, omandiõiguse kinnitus ega raha ülekandmise juhis. Kontrolli andmeid enne kasutamist sõltumatult.

## XI. Dokumentatsioon ja API-tööriistad

Käsk `sleela docs` loetleb dokumenteeritavad teemad ning kuvab pakettide ja klasside lehti. Samuti saab see luua võrguühenduseta kasutamiseks tekstikorpuse. SleelaDoc toetab märgendeid `@intent`, `@thesis`, `@axiom`, `@param`, `@see` ja `@mastery`. Vaata [SleelaDoc-i](lib/sleeladoc/README.md) ja [genereeritud API-dokumentatsiooni](sleeladoc-api/README.md).

## XII. AutoCAD Creator™

AutoCAD Creator™ on SLeeLa-s kirjutatud interaktiivne 2D CAD-rakendus. Sellega saab dokumente luua ja muuta, lisada seinu, ruume, mõõte ja märkusi, liigutada silte, muuta kihte, kustutada elemente, toiminguid tagasi võtta ning salvestada R12 tekstilisi DXF-jooniseid. Liides kasutab SleelaUI™-d ja võib platvormist sõltuvalt avada natiivseid aknaid X11, Cocoa või Win32 kaudu.

Dokumenteeritud arhitektuurireegel on: **SleelaUI esitab; SLeeLa otsustab.** `SLCadCreator` mudel haldab dokumendi geomeetriat ja tagasivõetavat muudatuste ajalugu; kasutajaliides ei ole geomeetria autoriteetne allikas.

```sh
make creator
```

Käivita käsk kataloogist `autocad/`. Ekraanita keskkonnas võib näide luua joonise mälus, väljastada aruande ja lõpetada.

## XIII. Eksperimentaalne UTF-4088 kataloog

`utf-4088/tools/generate_catalogue.py` loob deterministlikult eksperimentaalse kataloogi, milles on **1 048 576** kirjet. Dokumenteeritud binaarfaili suurus on 16 777 244 baiti. Testid ja GitHub Actionsi töövoog kontrollivad kirjete arvu, glüüfide unikaalsust, faili suurust ja SHA-256 räsi. Binaarfail avaldatakse töövoo artefaktina ega kuulu Git'i ajalukku. UTF-4088 on jätkuvalt eksperimentaalne ega ühildu Unicode'iga.

## XIV. π, Modium, Registry ja sleelactl

Hoidla dokumenteerib π kümnendkatalooge, sealhulgas **100 000 000** numbriga ja 57 344 numbriga versiooni. [VERIFIED](pi-generator/VERIFIED.md) kirjeldab kontrolle ja taasesitamise protseduuri.

Käsud `sleela registry` ja `sleela modium` laadivad suure π kataloogi. Juhendatud paigaldaja paigaldab `sleelactl`-i ja selle mooduliregistri. Projekt kirjeldab `math`-i kui esimest runtime'iga integreeritud natiivse C/C++ teegi subject'i; täpne käitumine tuleb üle kontrollida mooduli dokumentatsiooni ja testide põhjal.

## XV. Rahvusvahelised README-väljaanded

Riigiväljaanded kasutavad nimekuju `README.<CC>.md`. Ühes riigifailis võib olla mitu keelejaotist. Keele mainimine ei tähenda, et kõik piirkondlikud, põlisrahvaste, ametlikud või viipekeeled oleksid tõlgitud.

**Tõlkepoliitika:** tehnilised nimed, identifikaatorid, käsud, teed, lingid ja koodinäited säilitatakse. Väljaannet võib nimetada täielikuks tõlkeks alles siis, kui kõik jaotised on tõlgitud ning läbinud keelelise ja tehnilise ülevaatuse. Tõlkimata või üle vaatamata tehnilise sisu normatiivne allikas on ingliskeelne README.

## Põhiviited

- [Normatiivne README](README.md)
- [SLeeLa süntaksi spetsifikatsioon](SLEELA.syntax)
- [Dokumentatsiooni register](docs/README.md)
- [API-dokumentatsioon](docs/api/README.md)
- [Platvormitugi](docs/PLATFORM-SUPPORT.md)
- [Testimispoliitika](markdown/TESTING.md)
- [GitHubi hoidla](https://github.com/mearvk/SLeeLa)

**Tõlke olek:** Eesti väljaanne sisaldab nüüd laiendatud sissejuhatust, alustamist, ehitamist, paigaldamist, keelt, funktsioone, arhitektuuri, teeke, turvalisust, kaastööd ja peamisi tehnilisi teemasid. Vajalik on erialane keeleline ülevaatus ning võrdlus iga ingliskeelse README muudatusega.
