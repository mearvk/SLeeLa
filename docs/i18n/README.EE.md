<img align="right" src="../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — Eesti (EE)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Natiivne süsteemide programmeerimiskeel, kompilaator, käituskeskkond ja virtuaalmasinate ökosüsteem.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="../../docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Rahvusvaheline riigiväljaanne.** See väljaanne on eesti keeles. Tehnilised identifikaatorid, käsud, koodinäited, failinimed, failiteed ja lingid on samad mis ingliskeelses normatiivses [README.md](../../README.md)-s.

**The Now. The Ingredient.** Keel on enamat kui süntaks: see on kompilaator, käituskeskkond, teegid, tööriistad ja platvormilepingud, mis muudavad tarkvara kasutatavaks.

**Millega SLeeLa tegeleb.** SLeeLa tegeleb valdkondadega **Math**, **Object**,
**Science** ja **Reach** — tema töö neli palet: arvutatav
(Math), kindlaks tehtud asi, mis seisab olevate asjade kataloogis (Object),
tõe distsiplineeritud uurimine (Science) ning süsteemi käe sirutamine teiste
süsteemide ja inimeste poole (Reach). **Riiklikud huvid jäävad püsima** —
ükski neist neljast ei jäta neid kõrvale; kui teema puudutab riigi huve,
siis see seisund säilitatakse (kirjendatakse ustavalt, nt iga klassi
Metadata väljades `metaOrigin` / `metaNationalSchedule`) ega lahustu kunagi.

## I. Alustamine
- [Natiivse teostuse ehitamine](../../impl/README.md)
- [Virtuaalmasinad ja Modium — praktiline kasutusjuhend](../../virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [CPU virn SLeeLa lähtekoodis](../../lib/cpu/CPU.md) — täielik masin loogikaväravatest ALU-ni, tuumadeni, operatsioonisüsteemini ja programmideni, üle 130 CPU-profiili ning reguleeritavad [pinged, muutuvad taktsagedused ja eksperttasemel ajastused](../../lib/cpu/TIMING.md), mida saab [käitada spetsifikatsiooni järgi ja ülekiirendada](../../lib/cpu/components/README.md)
- [Dokumentatsiooni register](../../docs/README.md)
- [CSPML-i andmeallikas ja Salt & Salary uurimisviide](../reference/CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — välised USA kohtade, sotsiaalmajanduslike, tööjõu-, haridus- ja avalike koolide andmete päritolu ning integreerimisjuhised.
- [Genereeritud API-dokumentatsioon](../../docs/api/README.md) — praegune HTML-viide, versioonitud API väljaanne ja regenereerimisjuhised
- [API-kataloog](../../api/README.md)
- [Arvutusliku võrdluse standardid](../../comparison/README.md) — SLeeLa VM-i, Java/JVM-i ja natiivse C/C++ tüüpide, mälu, ABI ja vastavuse standardid.
- [Recursion stanchion — C ja C++ prototüübid](../../recursion/stanchion/README.md) — primitiivipõhised näited tüüpidele Boolean, Byte, Character, Integer, Long, Float ja Double ning üldised, rakenduslikud, teaduslikud ja õpetlikud rekursioonimustrid.
- [Platvormitugi ja valideerimismaatriks](../../docs/PLATFORM-SUPPORT.md)
- [Brändijuhend](../../docs/BRAND-GUIDE.md)
- [Kaubakontseptsioonid ja materjalid](../../merchandise/README.md)
- [NitroWebExpress logo](../../images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Testimispoliitika ja väljalaske tingimused](../../markdown/TESTING.md)

## II. Uudised

Hiljutised täiendused keelele, draiveritele ja standardteegile:

- **Standardteegi pakett `advantage` — dividendi eelisseisu arvutamine
  matemaatika, füüsika, integratsiooni, hindamise ja marsruuditava väljundiga.**
  [`lib/advantage/`](../../lib/advantage/README.md) kapseldab *unikaalse dividendi
  eelisseisu arvutamise mudeli suhtes* kihilise **Sequitur'i sibulana**
  (dividend → review → concerns → maths → additions → subtraction → regrouping
  → iteration), mida saab liikuda sissepoole ja väljapoole täieliku
  dünaamilise uuenduse ja taastamisega ning mis on avatud lihtsa
  **kasutaja mudeli** ja täieliku kontrolliga **arendaja mudeli** kaudu. Sellel on
  viis teemavaldkonda: **`math/`** kahepoolse kauplemise majandus (ühise kasumi
  maksimeerimine, Nashi läbirääkimine, kahepoolse newsvendor'i kriitiline
  suhe), **`physics/`** struktuurse majanduse „usaldusväljade füüsika"
  (usaldus kui meetrikatensor, tehingu geodeesia, teadaande kontaktstruktuur,
  CIR+hüppe usalduse hääbumise SDE — mudelianaloogia, mis on selliseks märgitud),
  **`integration/`** rekursiivselt komponeeritav vertikaalse integratsiooni
  mudel kapitalismi/jaekaubanduse jaoks (kambrid, inseneriavad, katsekambrid,
  newtoni mediaanalused ja süsteemi keerukuse skoor), **`grading/`**
  keerukuse skeemihindaja, mis hindab iga moodulit ja mis tahes võitvate/kaotavate/hindaja/absoluutsete
  moodulite hulka skaalal A–D, ning **`output/`**
  marsruuditav väljastussüsteem, mis saadab mooduli väljundi terminali,
  **CommonRails**-vormingus pinnale või sokli/ühenduse kaudu (raamistatud natiivse
  `slnet_write` jaoks). Iga klass kannab Metadata-Class kirjet; viis käivitatavat
  demot valideerivad teemasid ning kaasdokumendid
  [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md),
  [`MATH.md`](../../lib/advantage/MATH.md) ja
  [`PHYSICS.md`](../../lib/advantage/PHYSICS.md) kirjeldavad neid. Natiivne C/C++
  tarbija kataloogis [`advantage/`](../../advantage/README.md) (C tuum
  `advantage_value`/`advantage_mean`, XML-teadmistesalv, turvaline
  HTTPS/SFTP/SSH hankimise + võtmehoidla + DH/ECDH transpordi teek) seob
  mudeli töönäidetega
  [kauplemisnäited](../../advantage/examples/README.md) — positiivsed tehingud
  eeldatavatest internetiallikatest, sihtrühma asjakohasuse jada (Traders,
  Consumers, Helpers, Healers) ja kaheksa mudelit järjestatuna A–D (D = kõige rohkem
  Cement, A = kõige rohkem Brand).
- **Tsüklite ajastuse annotatsioonid — `@loopDelay` / `@loopBudget` (+ sisseehitatud
  `sleep(ms)`).** SLeeLa esimesed **lausetasandi, käitumuslikud** annotatsioonid:
  kirjutatuna eraldi reale enne `for` / `while` / `do` tsüklit lisavad nad tsükli
  täitmisse tegeliku pausi. `@loopDelay(ms = 250)` /
  `@loopDelay(seconds = 2)` ootab iteratsiooni kohta fikseeritud aja (paljas
  `@loopDelay(250)` tähendab vaikimisi millisekundeid); `@loopBudget(seconds = 10,
  iterations = 1000)` jaotab teadaoleva iteratsioonide arvu ühtlaselt kogu
  seinakella eelarve peale. Mõlemad taanduvad uueks lisatav-ainult operatsioonikoodiks **`OP_SLEEP`**
  (ISA 157), mis toetab ka üldist, kõikjal kutsutavat sisseehitatud funktsiooni **`sleep(<ms>)`**; paus on
  teostatud teisaldatavalt (`nanosleep` / `Sleep`). Määratletud
  [`SLEELA.syntax` §32](../../SLEELA.syntax); API viide:
  [`api/sleela/3.0/LOOP_TIMING_API.md`](../../api/sleela/3.0/LOOP_TIMING_API.md);
  käivitatav
  [`examples/loop-timing-annotation-demo.sleela`](../../examples/loop-timing-annotation-demo.sleela).
  Sessioonilisandite register (sealhulgas see, natiivne `OP_BIGMUL`
  suurte täisarvude korrutamine ning iseseisvad **[local-generator](../../local-generator/README.md)**
  ja **[pi-generator](../../pi-generator/README.md)** tööriistad) asub aadressil
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](../../api/sleela/3.0/SESSION-ADDITIONS-API.md).
  Kolm genereeritud API pinda
  ([`sleeladoc-api/`](../../sleeladoc-api/README.md),
  [`docs/api/`](../../docs/api/README.md) ja muutumatu
  [`api/sleela/3.0/version-014/`](../../api/sleela/3.0/version-014/index.html)
  hetktõmmis) ehitati uuesti käsuga `make api-docs` ning kannavad nüüd uusi `SLISASleep`
  / `SLISABigMul` käsukirjeid oma `vm` paketi lehel.
- **SleelaDoc käsurealt — `sleela docs`.** SleelaDoc on nüüd kättesaadav
  otse terminalist. `sleela docs` loetleb kõik dokumenteeritavad teemad;
  `sleela docs crypto` näitab paketti; `sleela docs stringbuilder` lahendab
  klassi (tõstutundetult ja standardteegi `SL` prefiksi kaudu →
  `SLStringBuilder`) ning prindib sisuka lehe — **definitsioon, kavandi eesmärk,
  seosed, struktuur, kasutaja savid ja viieastmeline meisterlikkuse redel** —
  koos järjestatud „kas mõtlesid" tulemustega möödalaskmise korral. Autorid rikastavad iga lehte lihtsalt
  SleelaDoc'i `@`-märgendite lisamisega (`@intent`, `@thesis`, `@axiom`, `@param`,
  `@see`, `@mastery`). `sleela docs --generate [dir]` kirjutab tavatekstikorpuse
  (üks leht klassi kohta + `TOPICS.index`), mida CLI seejärel võrguühenduseta loeb;
  **`make sleeladoc`** koondab selle korpuse **ja vastavad `.sleela` lähtefailid**
  failiks `sleeladoc/build/sleeladoc.jar`. Ainult fassaadiga teemade jaoks (nt
  `psychiatry`, `politico`) otsib `sleela docs --facade <topic>`
  käitusajal failisüsteemist ning loetleb tegelikud **kaustad (seejärel failid, loomuliku
  liikumisena)**, piiratuna kasutaja poolt muudetavate piirmääradega: **6 MB / 6000 faili / 6
  minutit**. Üksused `lib/sleeladoc` hoitakse praeguses väljaandes,
  **`#sleela 3.0`**. Vaata [`man/man1/sleela.1`](../../man/man1/sleela.1) ja
  [`lib/sleeladoc/README.md`](../../lib/sleeladoc/README.md).
- **SleelaDoc — Javadoc'i stiilis dokumenteerimislahendus koos genereeritud HTML-saidiga.**
  **SleelaDoc** on SLeeLa lähtekoodisisene dokumentatsioonikommentaaride tava
  (nurksulgudes kommentaarid, mis on märgitud `@doc` / `@page`; vaata
  [`manifesto/SLEELADOC.md`](../../manifesto/SLEELADOC.md)) ning
  [`lib/sleeladoc`](../../lib/sleeladoc/README.md) on **ekstraktor**, mis renderdab
  `.sleela` üksuse Markdown- või HTML-API lehena — nii nagu `javadoc` renderdab Java,
  dokumenteerides *kavatsust*. Täielik, sirvitav **HTML-dokumentatsiooni sait kogu
  `/lib` kohta** genereeritakse kataloogi [`/sleeladoc-api`](../../sleeladoc-api/README.md)
  skriptiga `tools/generate-sleeladoc-api.py` (ava `sleeladoc-api/index.html`; puhas
  sinise / helesinise / heleda halli / valge stiil paketi → klassi → liikme
  hierarhiaga ning sõnastik, mis on kirjutatud *Student 2* lugejale). Kompilaator
  väljastab autori märgitud dokumendiväljundid ka kompileerimise kõrvalproduktina (`--wiki`
  MediaWiki leht + algupärastatud lähtekoodi kaksik), mida kirjeldab
  [`manifesto/SLEELADOC.WIKI.md`](../../manifesto/SLEELADOC.WIKI.md). Käivitatav:
  [`lib/sleeladoc/sleeladoc-demo.sleela`](../../lib/sleeladoc/sleeladoc-demo.sleela).
- **XML dokumendiobjektimudel (`lib/xml`).** SLeeLa-poolne XML DOM — dokument,
  mis omab element-/tekst-/kommentaari-/CDATA-/PI-sõlmede puud järjestatud lastega ja
  nimeliste atribuutidega, serialiseerija (kompaktne/ilus, XML 1.0 §2.4 paosüsteem),
  märgiraku parser ja kvaliteediregister `SLXmlAspect`. Sõlmede võrdsus on
  kolmeastmeline usaldusseos (identiteet → sama subjekt → struktuur), refleksiivne,
  sümmeetriline ja deterministlik. Vaata [`lib/xml/README.md`](../../lib/xml/README.md) ja
  käivitatavat [`lib/xml/xml-dom.sleela`](../../lib/xml/xml-dom.sleela).
- **Laiad märgiga täisarvu primitiivid (`long`, `int64`, `int128`).** `int`, `long`
  ja `int64` on kõik 64-bitised märgiga täisarvud, mis tüübikontrollitakse ja käivituvad identselt;
  `int128` aktsepteeritakse täisarvutüübina (praegu 64-bitise toega, tõeline
  128-bitine on dokumenteeritud järgmine samm). Kahendsüsteemi märgiga seeria `int32`..`int16777216`
  (süntaks 2.7) ja vastav märgita seeria `uint32`..`uint16777216`
  (süntaks 2.8) laiendavad seda. Vaata [`SLEELA.syntax`](../../SLEELA.syntax) §21, §24, §25.
- **`@mix` järgnevad väljad (süntaks 2.9).** Klass võib deklareerida mitu
  eksemplariväljad **sama nimega**, kui iga kordus kannab `@mix`; keel
  mõistab rühma järgnevana (indeksi järgi võtmestatud massiiv / loend),
  vaikimisi ülalt alla `1,2,3,…` või käsitsi `@mix(1,2,3,...)` kaudu, mille indeksite
  arv peab võrduma väljade arvuga. Vaata [`SLEELA.syntax`](../../SLEELA.syntax) §28.
- **Dokumendimarkerid — Loomulik Protsess, tiitliga Preonemid
  (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).**
  Neid paljaid dokumendimarkeri sõnu kutsutakse ühiselt **Preonemideks**
  (üks marker on *Preonem*; hulk — *Preonemid* — vaata
  [`SLEELA.syntax`](../../SLEELA.syntax) §26.1). Esimese klassi, ainult deklaratiivsed
  dokumendimarkerid, mis nimetavad, mis dokument on tegemise järjekorras:
  `author`/`authorial` (süntaks 2.4; käsitleb professionaalne nõukogu — ainus
  liige käitumusliku mõjuga), `material;` (kindlaks tehtud Allikas) ja `memory;`
  (süntaks 2.5), `determine;` (süntaks 2.6; dokument, mida arst, doktor
  või kummitus võib edasi töödelda õiguspärase käe all lõpuni viimiseks)
  ning kaks uusimat, **`object;`** (olevate asjade kataloogis seisev kindlaks tehtud asi —
  **reifitseeritud, antud konkreetne seisund, süntaksis 2.8**) ja
  **`field;`** (seose alus, milles objektid seisavad ja liiguvad) —
  mõlemad lubatud alates süntaksist 2.6. Iga marker on paljas, kontekstuaalne ja
  esiosa poolt versioonipiiratud ilma mõjuta parsimisele, tüpiseerimisele või koodigeneratsioonile;
  markeripositsioonist väljaspool jääb igaüks tavaliseks identifikaatoriks (nii et `field` ei häiri kunagi
  klassiliiget ega `.field` juurdepääsu). Vaata [`SLEELA.syntax`](../../SLEELA.syntax)
  §18–§20, §23 ja §26.
- **Nordshrift süntaksil 3.0 täieliku teegi ulatusega.** Nordshrift lahendab
  sama rekursiivse `/lib` indeksi kaudu nagu kompilaator, jõudes igasse
  paketti ja `.sleela` üksusesse; `nordshrift check` raporteerib kogu puu arvud.
- **Recursion stanchion.** Kataloog [`recursion/stanchion/`](../../recursion/stanchion/README.md) sisaldab nüüd pühendatud C ja C++ viiteprototüüpe seitsmele dokumenteeritud primitiivilepingule ning üldiseid, rakenduslikke, teaduslikke ja õpetlikke näiteid. Need on viitelähtekoodid; kompileeri ja testi neid enne, kui neid kontrollituks pead.

- **Nurksulgudes kommentaarid (`<` / `<<`).** Uus edasisuunatud kommentaaristiil —
  `< rida >` ja `<< plokk >>` — mida SLeeLa esiosa lekseerib ja mis on dokumenteeritud
  [`SLEELA.syntax`](../../SLEELA.syntax) §1.1.
- **Radix-täisarvuliteraalid.** `int` literaalid aktsepteerivad nüüd kuueteistkümnend- (`0x`),
  kahend- (`0b`) ja kaheksandvorme (`0o`) koos `_` numbrieraldajatega, nii SLeeLa
  lähtekeeles ([`SLEELA.syntax`](../../SLEELA.syntax) §1.4) kui ka
  [`.xlt` CPU-mudeli vormingus](../../lib/xlt/README.md).
- **Metadata Class standard kogu teegis.** Kirjeldava metaandme
  metamuster (päritolu, õiguspärasus, jurisdiktsiooniline ulatus) on nüüd kaasas
  igas klassi sisaldavas `.sleela` failis kataloogis `/lib`; vaata
  [`SLEELA.syntax`](../../SLEELA.syntax) §15 ja
  [õppetundi](../../tutorial/13-metadata-class.md).
- **Rikkalikum tekstide / String'i perekond.** `lib/text` saab päris keelesiseseid abilisi,
  märgirakuvaate, mis annab tõelised `charAt`/`substring`/`indexOf`/`reverse`/
  veerujoonduse, ning tõelise positsioonilise mallitaja — vaata
  [teksti ja String'i õppetundi](../../tutorial/14-text-and-strings.md) ja käivitatavat
  [`lib/text/text-string-demo.sleela`](../../lib/text/text-string-demo.sleela).
- **Esimese klassi `String` Nordshrifti lehtedes.** `.sst` leht võib deklareerida
  tüpiseeritud `strings:` ploki, mis panustab ehituse komponendimanifesti;
  vaata [Nordshrifti lehe viidet](../../wiki/Nordshrift-Sheet-Reference.md).
- **CPU mudelid käivituvad nii operatsioonikoodide kui ka funktsioonikutsete ekvivalentsi kaudu.** Iga
  `SL<ARCH>CPU` pärib aluse, mille järgi ta käitab natiivset käsuvoogu
  oma fetch-decode-execute tsüklis *ja* käitab eeldatava
  teise protsessi koodi ekvivalentsete kutsete kaudu jagatud VM-i substraadil — vaata
  [`lib/cpu/EXECUTION.md`](../../lib/cpu/EXECUTION.md) §5.

## III. Ehitamine ja testimine

**Eeltingimused:** C/C++ tööriistaahel (GCC või Clang) ja GNU Make; Python 3
manifesti-/voogude tööriistade jaoks; Linuxis X11/Xrandr arenduspaketid
ekraanitoodete jaoks (mooduli `make deps` teatab/paigaldab vajaliku). Kataloogi
`impl/` ehitus on usaldatud SHA-256 manifesti suhtes tõrkesuletud (fail-closed) ning juurkataloogi
`Makefile` viitab sellele automaatselt.

Puhtast väljavõtust kiireim algus:

```sh
make                                  # build everything from the repo root
```

Või ehita ja testi ainult natiivset tuuma:

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

Hoidla juurkataloogi `make` ehitab iga toote (tuum, VM, koodekid, churn,
viirusetõrje, …) ja lõpetab **registri** ehitamise ning selle
baastabeli materialiseerimisega — nii sisaldab ehitatud puu valmis läbivaadatavat registrit.
Olulisemad sihid:

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

Muud kasulikud sihid:

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

Siht `registry` käivitab skripti [`build-registry.sh`](../../build-registry.sh): see ehitab
registri tuuma, külvab baastabeli teisaldatavasse tekstisalve
`registry/build/sleela.sleelareg` ning ekspordib MySQL tabeliskripti
`registry/build/registry.sql` (käsuga `sleela registry export`). Vaata see üle käsuga
`sleela registry` või `registry/build/sleela-registry review`. Vaata
[registri juhendit](../../registry/README.md) ja [Registry 3.0 arhitektuuri](../../registry/REGISTRY-3.0-ARCHITECTURE.md).

Eeltingimuste ja platvormispetsiifiliste sammude jaoks vaata ehitus- ja testimisjuhendeid. Ühe komponendi edukas ehitus ei tõenda, et kõik teegid või platvormid on väljalaskeks valmis.

## IV. Paigaldamine

Pärast natiivsete toodete ehitamist paigalda käsud ja käituskeskkonna tugifailid:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

Tavaline paigaldaja kasutab vaikimisi katalooge `/usr/local/bin`, `/usr/local/lib/sleela`
ja `/usr/local/share/sleela`. See säilitab olemasoleva käivituskonfiguratsiooni
ning keeldub kirjutamast üle jälgimata olemasolevaid käske. Eemaldamine
kustutab ainult paigaldaja registreeritud failid, säilitades konfiguratsiooni ja
jälgimata käitusandmed.

Ainult põhipaketi sihipäraseks, juhendatud paigalduseks — binaarfailid `sleela`,
`sleelvac` ja `nordshrift`, standardteek, protokollid ja
tugimoodulid — kasuta puhast paigaldajat:

```sh
make clean-install       # guided install of the main package only
```

See teeb ühe peamise otsuse (paigalda põhipakett), seejärel ulatuse valiku
ning edu korral valikulise valija kureeritud täiendusmoodulite jaoks. Interaktiivse
paigalduse töövoo ja kõigi paigaldusvalikute kohta vaata
[kiiret ja turvalist paigaldust](../../install/README.md).

## V. Keel ja käituskeskkond
Autoriteetne grammatika on [`SLEELA.syntax`](../../SLEELA.syntax). Praegune hoidla dokumenteerib süntaksit **3.0**; kompilaatori/tööriista versioon ja lähtekoodi süntaksiversioon on erinevad. Versioon 3.0 lisab **dokumendimarkerid `pure` ja `gasoline`** (§30) — paar paljaid, kontekstuaalseid, ainult deklaratiivseid Preoneme, mis tähendavad peaaegu sama, kumbki tähistab dokumendi seisundit oma ausameelsusega. Süntaks 3.0 koondab keelepinna 1.0–2.9 praeguseks normatiivseks väljaandeks, säilitades kõik funktsioonipõhised minimaalsed lävendid. Eelmine pind 2.9 lisab **`@mix` järgnevad väljad** (§28) — klass võib deklareerida mitu sama nimega eksemplarivälja, kui igaüks kannab `@mix`, mõistetuna ühe indekseeritud rühmana (ülalt alla `1,2,3,…` või käsitsi `@mix(1,2,3,...)`, mille indeksite arv peab vastama rühmale). See kannab edasi kahendsüsteemi märgiga/märgita laiad täisarvuperekonnad `int32`..`int16777216` (§24) ja `uint32`..`uint16777216` (§25) varasemate `long`/`int64`/`int128` (§21) kohal; **märksõnatoe dokumendid** (§22); ning **Preonemid** — ainult deklaratiivsed dokumendimarkerid `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) ja `artifact` (§35, seadusandliku taseme seisund, astmestatud aja ja ruumi järgi), mida ühiselt kutsutakse Preonemideks (§26.1). Konstruktori ülelaadimise lahendamine ning `this(...)`/baaskonstruktori delegeerimine jäävad selgesõnaliselt kontrollitud süntaksi 1.8 lepingust välja. Enne funktsioonile toetumist vaata grammatikat ja teostusmärkmeid. SleelaDoc'i üheksa lähtekoodiüksust deklareerivad nüüd `#sleela 3.0`; jagatud kompilaatori värav aktsepteerib lähtesüntaksit 1.0–3.0. Uus API ehitus luuakse käsuga `make api-docs`.

Hoidla sisaldab natiivset C/C++ teostust, SLVM-i genereerimise viiteid, standardteegi mooduleid, teste, näiteid ja platvormiintegratsioone. Teostuse ja valideerimise olek varieerub mooduliti; ühtlase täielikkuse eeldamise asemel vaata mooduli spetsifikatsiooni ja praeguseid CI tulemusi.

## VI. Inseneripõhimõtted
- **Tõendid väidete asemel:** erista teostatud, testitud, eksperimentaalset ja kavandatud käitumist.
- **Üks leping piiri kohta:** dokumenteeri süntaks, API-d, failivormingud, platvormi eeldused ja tõrkekäitumine.
- **Teisaldatav juba kavandamisel:** isoleeri platvormispetsiifilised taustsüsteemid ja testi natiivsetel jooksutajatel.
- **Turvaline tõrge:** testi väärvormitud sisendit, ressursipiiranguid, protsessitõrkeid ja koristusteid.
- **Taasesitatavad väljalasked:** ehita puhtalt uuesti ning avalda artefaktide räsid ja väljalaskemärkmed.

## VII. Viirusetõrje ehitus ja terviklus

Viirusetõrjepakett Sleelavirin sisaldab natiivset SHA-256 skannerit, valikulist
ClamAV adapterit, lähtekoodi-/indeksi tervikluse manifeste ja platvormipakettide kontrollsummasid.
Ehita ja testi paketti hoidla juurkataloogist:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Tervikluse manifest luuakse kataloogi `antivirus/build/`; see hõlmab jälgitavaid
lähtekoodi- ja dokumentatsioonisisendeid ning kaitstud allkirjaindeksi faile,
jättes välja genereeritud väljundi ja muutuva ClamAV vahemälu. ClamAV andmebaasi
hetktõmmistel on omad manifestid. Väljalaske ZIP-id sisaldavad `INTEGRITY-MANIFEST.json`,
`SHA256SUMS` ja `.sha256` kõrvalfaili. Kontrolli allalaaditud arhiivide räsisid
sõltumatult hangitud usaldatava väärtuse vastu: arhiivi kõrval hoitav kontrollsumma
ei ole avaldaja allkiri. Vaata [Sleelavirini juhendit](../../antivirus/README.md),
[turvakontrollnimekirja](../../antivirus/SECURITY.md) ja
[viirusetõrje ehitustöövoogu](../../.github/workflows/antivirus-binaries.yml).

Väljalaset ei loeta kontrollituks enne, kui asjakohased CI ülesanded on läbitud
täpsel väljalaske commit'il ja saadud artefaktid on üle vaadatud.


## VIII. SLeeLa redaktoriteemad — Vim ja Emacs

SLeeLa sisaldab kooskõlastatud professionaalsete tumedate värviskeemide komplekti **Vimile** ja **Emacsile** ning natiivset C/C++ paletitööriista. Algne **American Professional** skeem jääb vaikimisi valituks. Kümme kaaspaletti on saadaval erinevate kultuuriliste ja institutsionaalsete disaini-inspiratsioonide jaoks; need on tõlgendavad disainipaletid, mitte ühegi rahva, kultuuri, kohtu ega institutsiooni ametlikud esitused.

### Üksteist skeemi ja nende värvid

| Skeem | Taust | Esiplaan | Märksõnad | Tüübid | Stringid | Kommentaarid |
|---|---|---|---|---|---|---|
| **American Professional** | <span style="display:inline-block;width:1em;height:1em;background:#101820;border:1px solid #888"></span> `#101820` | <span style="display:inline-block;width:1em;height:1em;background:#F4F7FA;border:1px solid #888"></span> `#F4F7FA` | <span style="display:inline-block;width:1em;height:1em;background:#7FB3FF;border:1px solid #888"></span> `#7FB3FF` | <span style="display:inline-block;width:1em;height:1em;background:#73DACA;border:1px solid #888"></span> `#73DACA` | <span style="display:inline-block;width:1em;height:1em;background:#F2C078;border:1px solid #888"></span> `#F2C078` | <span style="display:inline-block;width:1em;height:1em;background:#8A96A3;border:1px solid #888"></span> `#8A96A3` |
| **Pan-Eurasian** | <span style="display:inline-block;width:1em;height:1em;background:#111827;border:1px solid #888"></span> `#111827` | <span style="display:inline-block;width:1em;height:1em;background:#F0F2F5;border:1px solid #888"></span> `#F0F2F5` | <span style="display:inline-block;width:1em;height:1em;background:#7AA7E8;border:1px solid #888"></span> `#7AA7E8` | <span style="display:inline-block;width:1em;height:1em;background:#69C9B3;border:1px solid #888"></span> `#69C9B3` | <span style="display:inline-block;width:1em;height:1em;background:#D7B56D;border:1px solid #888"></span> `#D7B56D` | <span style="display:inline-block;width:1em;height:1em;background:#8995A5;border:1px solid #888"></span> `#8995A5` |
| **Russian and Slavic — Medium** | <span style="display:inline-block;width:1em;height:1em;background:#202733;border:1px solid #888"></span> `#202733` | <span style="display:inline-block;width:1em;height:1em;background:#E8EAF0;border:1px solid #888"></span> `#E8EAF0` | <span style="display:inline-block;width:1em;height:1em;background:#A9C0DD;border:1px solid #888"></span> `#A9C0DD` | <span style="display:inline-block;width:1em;height:1em;background:#8FC4B2;border:1px solid #888"></span> `#8FC4B2` | <span style="display:inline-block;width:1em;height:1em;background:#D6BD8B;border:1px solid #888"></span> `#D6BD8B` | <span style="display:inline-block;width:1em;height:1em;background:#9AA3AF;border:1px solid #888"></span> `#9AA3AF` |
| **Central Asian / Silk Road** | <span style="display:inline-block;width:1em;height:1em;background:#1C2529;border:1px solid #888"></span> `#1C2529` | <span style="display:inline-block;width:1em;height:1em;background:#F5F0E5;border:1px solid #888"></span> `#F5F0E5` | <span style="display:inline-block;width:1em;height:1em;background:#7FADE0;border:1px solid #888"></span> `#7FADE0` | <span style="display:inline-block;width:1em;height:1em;background:#45C6C2;border:1px solid #888"></span> `#45C6C2` | <span style="display:inline-block;width:1em;height:1em;background:#D9B56D;border:1px solid #888"></span> `#D9B56D` | <span style="display:inline-block;width:1em;height:1em;background:#92A3A0;border:1px solid #888"></span> `#92A3A0` |
| **East Asian** | <span style="display:inline-block;width:1em;height:1em;background:#171A1C;border:1px solid #888"></span> `#171A1C` | <span style="display:inline-block;width:1em;height:1em;background:#F2EFE7;border:1px solid #888"></span> `#F2EFE7` | <span style="display:inline-block;width:1em;height:1em;background:#D97878;border:1px solid #888"></span> `#D97878` | <span style="display:inline-block;width:1em;height:1em;background:#72C7A5;border:1px solid #888"></span> `#72C7A5` | <span style="display:inline-block;width:1em;height:1em;background:#D8B76A;border:1px solid #888"></span> `#D8B76A` | <span style="display:inline-block;width:1em;height:1em;background:#969F9B;border:1px solid #888"></span> `#969F9B` |
| **ICC — International Criminal Court** | <span style="display:inline-block;width:1em;height:1em;background:#0E1A2B;border:1px solid #888"></span> `#0E1A2B` | <span style="display:inline-block;width:1em;height:1em;background:#EAF0F7;border:1px solid #888"></span> `#EAF0F7` | <span style="display:inline-block;width:1em;height:1em;background:#5B8FD6;border:1px solid #888"></span> `#5B8FD6` | <span style="display:inline-block;width:1em;height:1em;background:#7FB7C9;border:1px solid #888"></span> `#7FB7C9` | <span style="display:inline-block;width:1em;height:1em;background:#D9B25A;border:1px solid #888"></span> `#D9B25A` | <span style="display:inline-block;width:1em;height:1em;background:#7E8CA0;border:1px solid #888"></span> `#7E8CA0` |
| **SCOTUS — U.S. Supreme Court** | <span style="display:inline-block;width:1em;height:1em;background:#1A1416;border:1px solid #888"></span> `#1A1416` | <span style="display:inline-block;width:1em;height:1em;background:#F3EEE6;border:1px solid #888"></span> `#F3EEE6` | <span style="display:inline-block;width:1em;height:1em;background:#9E2B35;border:1px solid #888"></span> `#9E2B35` | <span style="display:inline-block;width:1em;height:1em;background:#8FA9B8;border:1px solid #888"></span> `#8FA9B8` | <span style="display:inline-block;width:1em;height:1em;background:#C9A24B;border:1px solid #888"></span> `#C9A24B` | <span style="display:inline-block;width:1em;height:1em;background:#8C8178;border:1px solid #888"></span> `#8C8178` |
| **Greek (Hellenic)** | <span style="display:inline-block;width:1em;height:1em;background:#0A1A30;border:1px solid #888"></span> `#0A1A30` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#2E7BD6;border:1px solid #888"></span> `#2E7BD6` | <span style="display:inline-block;width:1em;height:1em;background:#5E9FE0;border:1px solid #888"></span> `#5E9FE0` | <span style="display:inline-block;width:1em;height:1em;background:#9FC3EC;border:1px solid #888"></span> `#9FC3EC` | <span style="display:inline-block;width:1em;height:1em;background:#6E86A6;border:1px solid #888"></span> `#6E86A6` |
| **Turkish (Al Bayrak)** | <span style="display:inline-block;width:1em;height:1em;background:#2A0609;border:1px solid #888"></span> `#2A0609` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#F23D48;border:1px solid #888"></span> `#F23D48` | <span style="display:inline-block;width:1em;height:1em;background:#F0A9AE;border:1px solid #888"></span> `#F0A9AE` | <span style="display:inline-block;width:1em;height:1em;background:#F0C9A9;border:1px solid #888"></span> `#F0C9A9` | <span style="display:inline-block;width:1em;height:1em;background:#A8767A;border:1px solid #888"></span> `#A8767A` |
| **Kiro** | <span style="display:inline-block;width:1em;height:1em;background:#0B0B12;border:1px solid #888"></span> `#0B0B12` | <span style="display:inline-block;width:1em;height:1em;background:#F5F5FA;border:1px solid #888"></span> `#F5F5FA` | <span style="display:inline-block;width:1em;height:1em;background:#A98EDA;border:1px solid #888"></span> `#A98EDA` | <span style="display:inline-block;width:1em;height:1em;background:#6E8BE0;border:1px solid #888"></span> `#6E8BE0` | <span style="display:inline-block;width:1em;height:1em;background:#E8C547;border:1px solid #888"></span> `#E8C547` | <span style="display:inline-block;width:1em;height:1em;background:#6A6482;border:1px solid #888"></span> `#6A6482` |
| **SLeeLa** | <span style="display:inline-block;width:1em;height:1em;background:#08080E;border:1px solid #888"></span> `#08080E` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#B07CFF;border:1px solid #888"></span> `#B07CFF` | <span style="display:inline-block;width:1em;height:1em;background:#5A7CFF;border:1px solid #888"></span> `#5A7CFF` | <span style="display:inline-block;width:1em;height:1em;background:#FFD21E;border:1px solid #888"></span> `#FFD21E` | <span style="display:inline-block;width:1em;height:1em;background:#6E5F9E;border:1px solid #888"></span> `#6E5F9E` |

#### Rahvuslipu värvid (lähtepalett)

Kreeka ja Türgi skeemid on tuletatud kahe riigi **ametlikest rahvuslipu värvidest**, mis on siin esitatud nende täpsete avaldatud hex-väärtustega. Ülaltoodud skeemide taustad/rõhuvärvid on nende lähtevärvide tumendatud, redaktorisõbralikud tõlgendused — allolev tabel on „rahvuslikult esitatud värvide" viide.

| Riik | Värv | Hex | Alus ja tähendus |
|---|---|---|---|
| **Kreeka** | Sinine | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | Rahvuslipu sinine (seadus 851/1978). Taevas ja mered; heraldiliselt blasoneeritud kui *azure*. |
| **Kreeka** | Valge | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Rahvuslipu valge. Iseseisvusvõitluse puhtus; rist tähistab idapoolset õigeusku. |
| **Türkiye** | Punane (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | Rahvuslipu punane (Türgi lipuseadus). Lipuväli; traditsiooni järgi märtrite veri ja Osmani suveräänsus. |
| **Türkiye** | Valge | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Rahvuslipu valge. Poolkuu ja täht — rahu, puhtus ning türgi/islami pärandi sümbol. |

Skeemid ankurdavad iga redaktori esiplaani rahvuslikule **valgele** ning selle märksõnad, kursori, valiku ja olekuriba täpsele rahvuslikule **sinisele** (`#0D5EAF`) või **punasele** (`#E30A17`), nii et olulised värvid on sõna otseses mõttes rahvuslikud värvid. Need on tõlgendavad disainipaletid; lipuvärvide viide on esitatud päritolu tõendamiseks ega ole kummagi riigi ametlik esitus. Värvide tähendused järgivad Kreeka Vabariigi välisministeeriumi riigisümbolite kirjeldust ja Türgi lipuseaduse traditsiooni.


### Värviproovide tabel

Allolevad paletirakud kasutavad **SVG proove**, nii et GitHub renderdab tegelikud värvid, mitte tühje HTML-värvirakke. Iga riba näitab kuut värvi selles järjekorras: taust, esiplaan, märksõnad, tüübid, stringid ja kommentaarid. Täpsed hex-koodid on loetletud iga proovi kõrval, samas skeemide järjekorras nagu ülaltoodud viitetabelis.

| Skeem | Renderdatud paletivärvid | Hex-koodid (taust · esiplaan · märksõnad · tüübid · stringid · kommentaarid) |
|---|---|---|
| **American Professional** | ![American Professional palett: taust, esiplaan, märksõnad, tüübid, stringid, kommentaarid](../../lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Pan-Eurasian palett: taust, esiplaan, märksõnad, tüübid, stringid, kommentaarid](../../lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russian and Slavic — Medium** | ![Russian and Slavic Medium palett: taust, esiplaan, märksõnad, tüübid, stringid, kommentaarid](../../lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Central Asian / Silk Road** | ![Central Asian Silk Road palett: taust, esiplaan, märksõnad, tüübid, stringid, kommentaarid](../../lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **East Asian** | ![East Asian palett: taust, esiplaan, märksõnad, tüübid, stringid, kommentaarid](../../lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC — International Criminal Court** | ![ICC palett: taust, esiplaan, märksõnad, tüübid, stringid, kommentaarid](../../lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS — U.S. Supreme Court** | ![SCOTUS palett: taust, esiplaan, märksõnad, tüübid, stringid, kommentaarid](../../lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Greek (Hellenic)** | ![Greek Hellenic palett: taust, esiplaan, märksõnad, tüübid, stringid, kommentaarid](../../lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turkish (Al Bayrak)** | ![Turkish Al Bayrak palett: taust, esiplaan, märksõnad, tüübid, stringid, kommentaarid](../../lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Kiro palett: taust, esiplaan, märksõnad, tüübid, stringid, kommentaarid](../../lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![SLeeLa palett: taust, esiplaan, märksõnad, tüübid, stringid, kommentaarid](../../lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

Funktsioonide, konstantide, valikute, diagnostika ja eeltöötlusdirektiivide jaoks kasutatakse täiendavaid rõhuvärve. Vene ja slaavi palett kasutab tahtlikult keskmise intensiivsusega kiltkivi-, sinist, salvei-, rubiin- ja antiikkuldset tooni rahulikuma professionaalse ilme jaoks.

### Vim-i skeemide paigaldamine

Hoidla juurkataloogist:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Lisa faili `~/.vimrc` järgmine, et lubada SLeeLa süntaksi esiletõstmine ja valida skeem:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Vali täpselt üks neist värviskeemi nimedest:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Näiteks kasuta keskmise Vene ja slaavi paleti jaoks `colorscheme sleela-russian-slavic` või Hellenic ja Al Bayrak paletite jaoks `colorscheme sleela-greek` / `colorscheme sleela-turkish`. Taaskäivita Vim või laadi oma konfiguratsioon käsuga `:source $MYVIMRC`.

### Emacsi skeemide paigaldamine

Loo kohalik teemakataloog ja kopeeri režiimi- ja teemafailid hoidla juurkataloogist:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Lisa see faili `~/.emacs` või `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Teise skeemi valimiseks asenda `sleela-american` nimega `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` või `sleela-sleela`. Kui Emacs küsib, kas teemat usaldada, uuri kohalikku teemafaili enne nõustumist.

### Natiivsete C/C++ paletite eelvaade

Väike natiivne paletitööriist oskab loetleda skeeme, printida nende terminalivärve ja otsida üksikut värvi:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Tagasiühilduvuse huvides valivad `--palette` ja `--hex COLOR` ilma `--theme`-ta American Professional'i. Ehituse ja testide tulemusi tuleb kontrollida sihtsüsteemis ja CI-s; käsu dokumenteerimine ei ole iseenesest tõend, et see läbis.

Paletikirjelduste ja tööriista üksikasjade kohta vaata [redaktorite integratsioonijuhendit](../../linux-editors/README.md).

## IX. Kaastöö
1. Loe dokumentatsiooni registrit ja testimispoliitikat.
2. Tee piiritletud muudatusi positiivsete ja negatiivsete testidega.
3. Käivita sihitud testid, seejärel lai komplekt.
4. Märgi OS-i/tööriistaahela üksikasjad; ära kirjelda vahele jäetud kontrolle läbituna.

SLeeLat arendab **MEARVK LLC**. Ajaloolised avalehe materjalid on säilitatud failis [`docs/README-LEGACY.md`](../../docs/README-LEGACY.md).

## X. AE6E66 — Round Public e-kirja maksukohustuse ootus

AE6E66 algatuse puhul on projekti väljendatud ootus **$300,000,000.00 USD ühe Round Public e-kirja kohta** kui kavandatav maksukohustuse summa, mis on seotud iga nõuetele vastava Round Public e-kirjaga.

See avaldus kirjendab ainult projekti ootust. See **ei ole** õiguslik maksuhindamine, valitsuse korraldus, kohtuotsus ega sõltumatult kindlaks tehtud maksukohustus. Mis tahes tegelik maksukohustus eeldaks asjaomast õiguslikku volitust, jurisdiktsiooni, määratlusi, menetlust ja pädevat valitsuse otsust.


## XI. Tähestiku ja märgistiku käituskeskkond

SLeeLa tähestiku dokumentatsiooni ja natiivset märgikäituskeskkonda hoitakse kataloogides alphabet/sleela/ ja lib/alphabet/native/. Natiivsed C++17 testid katavad range UTF-8 skalaaride dekodeerimise/kodeerimise, C<n> mahuallkirjad ja piiratud märgistiku kuulumise:

    make -C lib/alphabet/native test

Eksperimentaalne UTF-4088 kataloog ja selle SLeeLa otsinguadapter on dokumenteeritud failis utf-4088/SLeeLa-ALPHABET-INTEGRATION.md. UTF-4088 ID-d on eraldi nimeruum, mitte Unicode'i skalaarväärtused. Kompilaatori ja SLVM-i sidumistööd jälgitakse nendest natiivsetest alustest selgesõnaliselt eraldi.

## XII. AutoCAD Creator™

[AutoCAD Creator™](../../autocad/creator/README.md) on interaktiivne 2D CAD
rakendus, mis on kirjutatud täielikult SLeeLa-s. Kui sõsarrakendus AutoCAD Renderer muudab
plaani joonisekski ühe hoobiga, siis Creator võimaldab **luua ja muuta** 2D
AutoCAD dokumenti reaalajas — lisada seinu, ruume, mõõte ja märkusi, seejärel liigutada,
ümber nimetada, kihti vahetada, kustutada ja tagasi võtta, mis oled paigutanud — ning **salvestada** tulemus
päris AutoCAD DXF-joonisena (R12 tekst, avaneb AutoCAD-is / LibreCAD-is).

Selle graafiline liides on ehitatud **SleelaUI™** peale, SLeeLa enda platvormideülesele
UI tööriistakomplektile (kataloogis [`lib/user-interface`](../../lib/user-interface)) — mitte JavaFX peale — ning
avab päris natiivse akna X11 / Cocoa / Win32 all. Kavand järgib ühte reeglit,
**„SleelaUI presents; SLeeLa decides"**: GUI ei arvuta kunagi geomeetriat ega kirjuta
DXF-i. Iga tööriist esitab loomise / muutmise / kustutamise / tagasivõtmise / salvestamise kavatsuse
autoriteetsele [`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela)-ile, mis omab
muudetavat dokumenti ja pööratavat käsuajalugu, samal ajal kui lõuend ainult eelvaatab
Creatori hoitavat geomeetriat.

Taaskasutatav redigeerimismootor asub kataloogis [`lib/autocad`](../../lib/autocad) —
[`SLCadEntity`](../../lib/autocad/SLCadEntity.sleela),
[`SLCadDocument`](../../lib/autocad/SLCadDocument.sleela),
[`SLCadCommand`](../../lib/autocad/SLCadCommand.sleela) ja
[`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela) — ning töötab otsast lõpuni ekraanita.
GUI kest ja demo asuvad kataloogis [`autocad/creator`](../../autocad/creator):

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

Ekraaniga masinas avab see reaalajas redigeerimisakna; ekraanita hostis ehitab see
joonise mällu, prindib aruande ja DXF-i ning lõpetab puhtalt.


## XIII. UTF-4088 sisendkaardi järjestus

Spetsialiseeritud UTF-4088 kataloog määratleb oma ruumilise sisendkaardi läbimise ja abstraktsed rõhu iteratsioonid failis [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). National Lien'i teemaindeksi poliitika on failis [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Kaardikoordinaadid, rõhutasemed (1–50), püsivad kataloogi ID-d ja tähestikulised järgud on sõltumatud väljad; ühtegi neist ei tohi vaikselt teisega asendada. Rõhk on algoritmiline iteratsioonitase, mitte füüsikaline suurus.


## XIV. Hiljutiste ja laiendatud pakettide register

See lisatud register toob esile hiljuti laiendatud pakette, valdkonnateeke ja
lähtekogumeid. Kirjed on rühmitatud tähestikuliselt paketi nime või teema järgi.
Kirjeldused näitavad hoidlas asukohta; teostuse ja valideerimise olekut
tuleb kontrollida iga paketi enda dokumentatsioonist ja testidest.

### A–C

- **Advantage** — `lib/advantage/` ja `advantage/`: unikaalse
  dividendi eelisseisu arvutamine mudeli suhtes kihilise Sequitur'i sibulana, viie
  teemaalamkaustaga — `math/` (ühise kasumi maksimeerimine, Nashi läbirääkimine,
  kahepoolne newsvendor), `physics/` (usaldusvälja meetrika/geodeesia/kontakt/SDE,
  märgitud mudelianaloogia), `integration/` (vertikaalse integratsiooni
  kambrid ja keerukuse skoor), `grading/` (A–D keerukuse
  skeemihindaja moodulitele ja hulkadele) ning `output/` (terminal / CommonRails /
  sokli väljastus). Natiivne C/C++ tarbija (`advantage/`) seob mudeli
  töönäidete kauplemisnäidetega. Vaata [`lib/advantage/README.md`](../../lib/advantage/README.md)
  ja [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md).
- **Alphabet ja UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` ja
  `utf-4088/`: tähestikukataloogid, natiivne märgistiku tugi, sisendkaardi
  järjestus ja UTF-4088 integratsioonidokumentatsioon.
- **AutoCAD Creator** — `autocad/creator/` ja `lib/autocad/`: interaktiivsed
  2D joonistamise töövood ning taaskasutatavad CAD-dokumendi/redigeerimise komponendid.
- **Ballum** — pikslitaseme ekraanijuhtimine: piksli kaupa set/get, piirkonna blit,
  kogu ekraani heledus/soojus/gamma, osuti-/klaviatuurisisend ning
  **täisekraani kaadripuhvri pind** täielikuks, omatuks ja kiireks piksli kaupa juhtimiseks
  (override-redirect aken + üks `XPutImage`/MIT-SHM esitus Linuxis/X11).
  Natiivne tuum kataloogis `ballum/`, SLeeLa fassaad kataloogis `lib/ballum/` ja VM-sild
  kataloogis `impl/`; vaata [`ballum/README.md`](../../ballum/README.md) ja
  käivitatavat [`ballum/example/`](../../ballum/example/).
- **CDMA** — `cdma/` ja `lib/cdma/`: CDMA-ga seotud tehniline materjal ja
  SLeeLa lähtekoodiraamatukogu töö.
- **Character** — `lib/character/`: tegelasprofiilid ja taaskasutatav tegelaste
  statistika, sealhulgas biograafiliste profiilide näited.
- **Coorenagraph** — `lib/coorenagraph/`: eetiline arutlus, moraalisõnavara,
  kaalutlemine ja seotud mõisted; vaata paketi README-d ja lähtefaile.

### D–G

- **Valitsemise lähtekoodimudelid** — [lib/governance](../../lib/governance/README.md) määratleb deklaratiivsed SLeeLa lähtekoodi kujud ettepanekute, anonüümsete käsitsi hääletuste ja koondtulemuste jaoks. Kavandatud @vote annotatsioon ja käsitsi protsess on dokumenteeritud failis [registry/DEMOCRATIC-VOTING-SYNTAX.md](../../registry/DEMOCRATIC-VOTING-SYNTAX.md); praegused kompilaatorid käsitlevad seda inertse kommentaarimetaandmena.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` kuni saadaolevate versioonitud HTTP
  paketikataloogideni: versioonipõhised protokollispetsifikatsioonid ja
  teostused. Täpse ulatuse ja kontrolli oleku kohta vaata iga versiooni dokumentatsiooni.
- **International Criminal Court** — `lib/international-criminal-court/` ja
  `INTERNATIONAL.CRIMINAL.COURTS.md`: seotud valdkonnaspetsiifiline lähtekood ja
  dokumentatsioon.

### O–S

- **OFDMA** — `ofdma/`: ortogonaalse sagedusjaotusega hajusjuurdepääsu
  tehniline materjal ja C/C++ arendusressursid.
- **Philosophy ja Conversation Mask moodul** — [`philosophy/`](../../philosophy/README.md)
  sisaldab natiivseid C/C++ aluseid ning [`philosophy/module/`](../../philosophy/module/README.md)
  korraldab nende järgmise mooduli kihi kolmeks selgeks valdkonnaks:
  [`data/`](../../philosophy/module/data/README.md) tüpiseeritud, dateeritud vaatluste,
  sündmuste lõppude, poolestusaja seadete, päritolu ja nõusoleku jaoks; [`helper/`](../../philosophy/module/helper/README.md)
  deterministliku valideerimise, asjakohasuse hääbumise, ühilduvate võrdluste ja
  nõusolekuga filtreeritud esituse jaoks; ning [`psychiatry-friend/`](../../philosophy/module/psychiatry-friend/README.md)
  toetava, mittekliinilise vestlusliidese jaoks. SLeeLa-poolsed
  käepidemed ja komponeeritud puu asuvad kataloogis [`lib/philosophy/`](../../lib/philosophy/README.md),
  laiema philosophy paketi kõrval. Moodul põhineb selgesõnalisel sisendil ja
  nõusolekul: see ei jälgi passiivselt inimesi ega meediat, ei diagnoosi, ei järelda
  privaatseid kavatsusi, ei hinda inimese väärtust ega soovita ravimimuudatusi. Selle
  sündmuste lingid säilitavad päritolu ja navigeerimise, mitte sõltumatut kinnitust.
- **Synchro** — natiivne käituskeskkonna/kompilaatori integratsioon ning paketisaatmise ja
  mõõtmisega seotud funktsionaalsus; vaata asjakohast `impl/` lähtekoodi ja
  kompilaatori teostust.
- **SleelaUI / terminali UI** — `lib/user-interface/` ja
  projekt `sleela-terminal`: SLeeLa UI integratsioon ja terminalile suunatud
  interaktsioonikomponendid.

### U–W

- **United Nations** — `united-nations/`: valdkonnaspetsiifilised hoidla materjalid.
- **Wi-Fi 4, Wi-Fi 5 ja Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`,
  `wi-fi-6/` ja seotud `lib/wifi/` lähtekoodikataloogid: põlvkonnapõhine
  juhtmevaba võrgu materjal ja konfiguratsiooniressursid.

### Paketiregistri hooldus

Kui pakett lisatakse või seda oluliselt laiendatakse, lisa selle kirje siia
sobivasse tähestikulisse rühma, lingi selle kanoonilisele kataloogile ja README-le
ning kirjelda ainult sisseviidud lähtekoodi toetatavaid võimalusi. Hoia see
register täiendavana: ära eemalda ega kirjuta ümber olemasolevaid README jaotisi
paketiregistri uuenduste osana.


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

USA demokraatlik ja USA poliitika.

---

## XVI. Rahvuslik teaduse bränditunnuspilt

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. UTF-4088 miljoni kirjega eksperimentaalne kataloog

[UTF-4088 kataloogigeneraator](../../utf-4088/tools/generate_catalogue.py) ehitab deterministlikult 1 048 576 kirjega eksperimentaalse sümbolikataloogi. Dokumenteeritud kompaktvorming on kirjeldatud failis [utf-4088/FORMAT.md](../../utf-4088/FORMAT.md); genereerimise ja valideerimise leping on failis [utf-4088/GENERATION-RESULTS.md](../../utf-4088/GENERATION-RESULTS.md).

Käivita generaator Python 3-ga:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Genereeritud binaarfail on 16 777 244 baiti ja on kavandatud jääma alla 20 000 000 baidi. Ühiktestid ja GitHub Actions töövoog kontrollivad kirjete arvu, glüüfide unikaalsust, suurust ja SHA-256; edukad jooksud avaldavad binaarfaili ja kokkuvõtte töövoo artefaktina `utf4088-million-catalogue`. Genereeritud binaarfail hoitakse tahtlikult Git'i ajaloost väljas. UTF-4088 jääb eksperimentaalseks ega ole Unicode'iga ühilduv.

---

# SLeeLa
<p align="center"><img src="../../images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Hiljutised täiendused — Pi konstant, Modium/Registry käivitus, sleelactl Math ja `@native`

See jaotis kirjendab 2026-11 töö käigus lisatud funktsioone, uusimad rühmad eespool. Lingitud lähtefailid ja nende valideerimisolek on autoriteetsed; läbivaatuse all olevaid muudatusi ei kirjeldata ühendatuna enne, kui need jõuavad harusse `master`.

### 100 000 000-kohaline Pi kataloogikonstant

Hoidla sisaldab kontrollitud **saja miljoni numbriga** π väärtust failis [`pi-generator/pi.100000000.digits`](../../pi-generator/pi.100000000.digits) (korrektne kümnendarv, `3.14159…`) ning 57 344-kohalist väärtust [`pi-generator/pi.57344.digits`](../../pi-generator/pi.57344.digits). Mõlemad on sõltumatult kontrollitud tõelise π-na — vaata [`pi-generator/VERIFIED.md`](../../pi-generator/VERIFIED.md), mis dokumenteerib kontrollid (õpikuprefiks, Feynmani punkt murdkoha positsioonil 762, kuulus miljones number, kokkulangevus failiga `lib/math/SLPiReference.sleela` ning kogu faili numbrisageduse analüüs) ja ühekordse taasesitamise skripti.

### Modium™ / Registry™ laadivad Pi konstandi delsüsteemi käivitusel

Spetsiaalsetes delsüsteemi sisenemispunktides — `sleela registry` ja `sleela modium` (ning paigaldaja esimene käivitus), **mitte** igal `sleela run` korral — laadivad Modium ja Registry delsüsteemid suure π kataloogikonstandi. Interaktiivses terminalis prindivad nad mööduva punase rea `Loading Modium™` / `Loading Registry™`, mis kustutatakse kohapeal enne tavalise väljundi algust; efekt on rangelt TTY-piiratud, nii et torustatud ja testväljund jäävad bait-baidi haaval muutmata. Asukoht (mitte baidid) külvatakse registri `Math` võtme vaikeväärtustena `piReferenceLargePath` ja `piReferenceLargeDigits`, nii SLeeLa mudelis ([`lib/registry/SLRegistry.sleela`](../../lib/registry/SLRegistry.sleela)) kui ka natiivses külvajas ([`registry/sleela_registry.c`](../../registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` pakub `largePath()` / `largeDigits()`. π konstant paigaldatakse OS-i SLeeLa asukohta (`<data>/pi/`) **nii** tavalise `make` paigaldaja kui ka juhendatud „ilusa" paigaldaja poolt. Üksikasjad: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](../../pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — ilus paigaldaja ja natiivse toega `math` subject

- Juhendatud („ilus") paigaldaja paigaldab nüüd `sleelactl`-i (SLeeLa juhtimistasand) ja selle mooduliregistri, vastavalt `make` paigaldajale. Vaata [`bin/SLEELACTL.md`](../../bin/SLEELACTL.md).
- `sleelactl` sai uue **`subject`** olemuse käituskeskkonda kompileeritud natiivsetele C/C++ subject-teekidele ning registreerib esimese: **`math`**. `sleelactl run math` käivitab natiivse `__NativeMath` taustsüsteemi (`impl/subjects/math/math_native.cpp`) otsast lõpuni subjecti viitedemo kaudu; `sleelactl info math` selgitab, kuidas sellele ligi pääseb (`import math;`). Subject-olemus pakub ainult `run info` — `check` verbi tahtlikult ei ole, sest `sleela check` ei laadi SHEET-is registreeritud natiivseid subjecte.

### `@native`, Native Drive ja mitmekeelne täitmine

Uus **`@native`** meetodiannotatsioon taotleb kuuma meetodi eel-kompileerimist natiivkoodiks (ahead-of-time). Uus käsk `sleela aot` leiab `@native` meetodid, kontrollib, et iga neist jääb toetatud *kõrvalepandava alamhulga* sisse, ning väljastab C/C++ — **tõrkesuletult** (keeldudes täpse põhjusega) mis tahes konstruktsiooni korral, mida ta veel tõlkida ei suuda, nii et pooltõlgitud meetodit ei looda kunagi.

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

`@native` jääb AOT-töötluse taotluseks, toetamata konstruktsioonid lükatakse tagasi enne väljastamist. [Native Drive](../../impl/native-drive/README.md) toetab mitme üksusega C/C++ käivitatavate ja jagatud teekide ehitusi hostkompilaatorite kaudu (sealhulgas MSVC), toetatud SLeeLa AOT väljundit, sümbolite kontrolli natiivlaaduri ABI suhtes, kestavaba protsessi käivitust, stdin/stdout/stderr käsitlust, mitmeastmelist torustamist ning XClass'i delegeerimist tootmislaadurile. `sleelactl` pakub selle ehitus-/käivituskäske ning suunab XClass'i kontrolli ja selgesõnalise systemd üksuse renderdamise/paigalduse käsu `inspect native-drive` kaudu. VM omab piiratud natiivmoodulite käepidemetabelit kohustusliku hostiloa ja põlvkonnakontrollitud käepidemetega. Süntaks 3.0 lisab nüüd `nativeOpen`, `nativeCall` ja `nativeClose`: lisatav-ainult baitkoodi dispetšer ületab eraldi versioonitud null/bool/int64/double/UTF-8 ABI, samas kui VM-i omatud massiivid, struktuurid ja muud liitväärtused jäävad interpretaatorisse ning võivad kasutada sama nimega baitkoodi varuvarianti. Pluginad võivad valikuliselt saada versioonitud hosti tagasikutsetabeli, et kutsuda täpseid SLeeLa funktsioone pesastatud VM-täituse kaudu, peatatud virna GC juurimisega, selgesõnalise hosti tagasikutsenimede lubatud loendiga ning rekursiivsed natiivsed API kutsed lükatakse tagasi. Natiivkood jääb usaldatud protsessisiseseks koodiks, mitte liivakastiks. Lähtekoodi protsessipoliitika annotatsioonidel on versioonitud skeem, kuid neid ei parsita ega jõustata veel. Üksikasjad, turvapiirid ja seitsme tööjoone register: [Native Drive](../../impl/native-drive/README.md) ja [`pi-generator/AT-NATIVE.md`](../../pi-generator/AT-NATIVE.md).


## XIX. Rahvusvahelised README väljaanded

SLeeLa pakub riigipõhiseid README sisenemispunkte oma rahvusvahelise väljaande 50 turu jaoks. Igal riigil on üks fail nimega `README.<CC>.md`; kus on asjakohased mitu keelt, on need rühmitatud selle riigi faili sees.

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

**Tõlkepoliitika:** Need failid on esialgsed lokaliseeritud riigipõhised sisenemispunktid. Nende sissejuhatused ja keelepealkirjad on lokaliseeritud, samal ajal kui täielik tehniline viide jääb ingliskeelsesse normatiivsesse [README.md](../../README.md)-sse. Riigifaili ei tohi kirjeldada täieliku tõlkena enne, kui iga jaotis on tõlgitud, tehnilised identifikaatorid ja lingid on kontrollitud ning väljaanne on läbinud keeleülevaatuse. Keelekate selles esimeses läbimises on esindav ega ole väide, et iga põhiseaduslikult tunnustatud, piirkondlik, põlisrahvaste või viipekeel oleks täielikult tõlgitud.

## Lisa — säilitatud piirkondlik juhendmaterjal

Allpool on säilitatud eestikeelse lühijuhendi praktiline sisu, mida normatiivses README-s eraldi jaotisena ei ole. See täiendab jaotisi I–XIX ega asenda neid; vastuolu korral on autoriteetne ingliskeelne [README.md](../../README.md).

### Sihid, profiilid ja käivitamise piirid

SLeeLa eristab lähtekoodi ehituse ja käivituse sihtidest. Dokumenteeritud sihtide hulka kuuluvad `SLVM`, `SLJVM` ja `SLIR`; `NATIVE` on piiratud ja tingimuslik siht ning seda ei tohi eeldada kõigil platvormidel kättesaadavaks. Profiilid `BASIC_COMPLETE` ja `ADVANCED_TOTAL` tähistavad erinevat ulatust. Kontrolli iga ehituse tegelikku sihti, profiili ja platvormituge.

Viide tuvastab faili, mooduli või ressursi, kuid ei anna iseenesest õigust seda käivitada. Failide, väliste teekide, natiivsete sidumisliideste (native bindings) või süsteemiressursside kasutamisel tuleb selgelt määrata volituste piirid ja käivitamise eest vastutav komponent. Dokumentatsioonikirjeldus üksi ei tõenda käituskeskkonna turvagarantiid; kontrolli teostust ja asjakohaseid teste.

### Ressursi elutsükkel ja silumine

Dokumenteeri funktsioonide või lõimede vahel jagatud ressursside omanik, eluiga ja sünkroonimise vastutus. Erista lokaalseid muutujaid, jagatud objekti olekut, VM-i käepidemeid ja serialiseeritavaid andmeid. VM-lokaalne käepide ei ole stabiilne failivorming ega võrgusõnum. Kasuta mooduli määratletud serialiseerimisliidest ning testi vigaseid, puuduvaid ja piirväärtusi.

Silumisel märgi commit, OS, arhitektuur ja tööriistaahel. Ehita esmalt väikseim asjakohane siht, säilita täielik veaväljund, erista kompilaatori vead ja käituse vead ning koosta minimaalne korratav näide. Edukas ehitus ei tõenda iseenesest käitumise õigsust käitusajal.

Valideerimisaruandes esita täpne commit, OS/arhitektuur, kompilaatori versioonid, täielikud ehitus- ja testikäsud, läbitud/ebaõnnestunud/vahele jäetud testide arvud ja teadaolevad piirangud. Vahele jäetud või käivitamata teste ei tohi lugeda läbituks.

### Märkused viidete ja aadresside kohta

Teemade Character, Governance, Synchro, SleelaUI ja United Nations viited kirjeldavad hoidla sisu ega tähenda valitsuse või institutsiooni ametlikku heakskiitu.

Jaotise **Bitcoin Conjegeum** (XV) aadresside või piltide olemasolu ei ole finantssoovitus, omandiõiguse kinnitus ega juhis raha ülekandmiseks. Kontrolli andmeid enne kasutamist sõltumatult.

**Tõlke olek:** Jaotised I–XIX on tõlgitud eesti keelde. Tehnilised identifikaatorid, käsud, koodinäited, failinimed, failiteed, lingid ja aadressid on säilitatud. Soovitatav on emakeelsete kõnelejate lõplik keeleline ülevaatus.
