# SLeeLa 3.0 — Slovensko (SK)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Natívny systémový jazyk, kompilátor, runtime prostredie a ekosystém virtuálnych strojov.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Medzinárodné krajinské vydanie.** Toto vydanie je v slovenčine. Technické identifikátory, príkazy, príklady kódu, názvy súborov, cesty a odkazy zostávajú rovnaké ako v kanonickom anglickom [README.md](README.md).

**The Now. The Ingredient.** Jazyk je viac než syntax: tvorí ho kompilátor, runtime prostredie, knižnice, nástroje a platformové zmluvy, vďaka ktorým je softvér použiteľný.

**Čím sa SLeeLa zaoberá.** SLeeLa sa zaoberá oblasťami **Math**, **Object**,
**Science** a **Reach** — štyrmi tvárami svojej práce: vypočítateľným
(Math), ustálenou vecou stojacou v katalógu toho, čo jestvuje (Object),
disciplinovaným štúdiom pravdivého (Science) a rozšírením ruky systému k iným
systémom a ľuďom (Reach). **Národné záujmy zostávajú** — žiadna zo štyroch
oblastí ich neodsúva stranou; ak sa predmet dotýka záujmu národa, toto
postavenie sa zachováva (verne zaznamenané, napr. v metadátach každej triedy
`metaOrigin` / `metaNationalSchedule`), nikdy sa nezrušuje.

## I. Začíname
- [Zostavenie natívnej implementácie](impl/README.md)
- [Virtuálne stroje a Modium — praktická používateľská príručka](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [Zásobník CPU v zdrojovom kóde SLeeLa](lib/cpu/CPU.md) — kompletný stroj od hradiel cez ALU, jadrá a OS až po programy, zoznam viac než 130 profilov CPU a nastaviteľné [napätia, premenlivé takty a odborné časovania](lib/cpu/TIMING.md), ktoré môžete [spustiť podľa špecifikácie aj pretaktovať](lib/cpu/components/README.md)
- [Register dokumentácie](docs/README.md)
- [Zdroj údajov CSPML a výskumná referencia Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — pôvod údajov a pokyny na integráciu externých amerických údajov o miestach, socioekonomike, pracovnej sile, vzdelávaní a verejných školách.
- [Generovaná dokumentácia API](docs/api/README.md) — aktuálna HTML referencia, verzované vydanie API a pokyny na opätovné generovanie
- [Katalóg API](api/README.md)
- [Štandardy výpočtového porovnávania](comparison/README.md) — štandardy typov, pamäte, ABI a zhody pre SLeeLa VM, Java/JVM a natívny C/C++.
- [Recursion stanchion — prototypy C a C++](recursion/stanchion/README.md) — príklady špecializované na primitíva Boolean, Byte, Character, Integer, Long, Float a Double spolu so všeobecnými, aplikovanými, vedeckými a výukovými vzormi rekurzie.
- [Podpora platforiem a validačná matica](docs/PLATFORM-SUPPORT.md)
- [Príručka značky](docs/BRAND-GUIDE.md)
- [Koncepty a podklady propagačných predmetov](merchandise/README.md)
- [Logo NitroWebExpress](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Zásady testovania a podmienky vydania](markdown/TESTING.md)

## II. Novinky

Nedávne doplnenia jazyka, ovládačov a štandardnej knižnice:

- **Balík štandardnej knižnice `advantage` — zvýhodnenie dividendy
  s matematikou, fyzikou, integráciou, hodnotením a smerovateľným výstupom.**
  [`lib/advantage/`](lib/advantage/README.md) zapuzdruje *zvýhodnenie
  jedinečnej dividendy vzhľadom na model* ako vrstvenú **cibuľu Sequitur**
  (dividenda → preskúmanie → obavy → matematika → prírastky → odčítanie
  → preskupenie → iterácia), po ktorej sa dá pohybovať dovnútra aj navonok
  s úplnou dynamickou aktualizáciou a navrátením (restitute); je sprístupnená
  cez jednoduchý **model používateľa** a plne ovládateľný **model vývojára**.
  Zahŕňa päť tematických oblastí: **`math/`** — bilaterálna obchodná ekonómia
  (spoločná maximalizácia zisku, Nashovo vyjednávanie, kritický pomer
  bilaterálneho newsvendora), **`physics/`** — štrukturálna ekonomická
  „fyzika polí dôvery“ (dôvera ako metrický tenzor, transakčná geodetika,
  kontaktná štruktúra oznámenia, SDE rozpadu dôvery CIR+jump — modelová
  analógia, ako taká označená), **`integration/`** — rekurzívne skladateľný
  model vertikálnej integrácie pre kapitalizmus/maloobchod (oddelenia,
  inžinierske zálivy, skúšobné oddelenia, newtonove medián-podnosy a skóre
  sofistikovanosti systému), **`grading/`** — hodnotiteľ sofistikovanosti
  (circuit grader), ktorý hodnotí každý modul a ľubovoľnú množinu
  víťazných/porazených/hodnotiacich/absolútnych modulov stupňami A–D, a
  **`output/`** — smerovateľný emisný šev, ktorý posiela výstup modulu do
  terminálu, na povrch formátovaný pre **CommonRails** alebo na socket/konektor
  (zarámovaný pre natívny `slnet_write`). Každá trieda nesie záznam
  Metadata-Class; päť spustiteľných ukážok overuje jednotlivé oblasti a
  sprievodné dokumenty [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md),
  [`MATH.md`](lib/advantage/MATH.md) a
  [`PHYSICS.md`](lib/advantage/PHYSICS.md) ich opisujú. Natívny C/C++
  konzument v [`advantage/`](advantage/README.md) (jadro C
  `advantage_value`/`advantage_mean`, XML úložisko znalostí, knižnica na
  bezpečné získavanie cez HTTPS/SFTP/SSH + úložisko kľúčov + transport DH/ECDH)
  prepája model s konkrétnymi
  [obchodnými príkladmi](advantage/examples/README.md) — pozitívne obchody
  z predpokladaných internetových zdrojov, postupnosť relevantnosti pre
  publikum (Obchodníci, Spotrebitelia, Pomocníci, Liečitelia) a osem modelov
  zoradených A–D (D = najviac Cement, A = najviac Brand).
- **Anotácie časovania slučiek — `@loopDelay` / `@loopBudget` (+ vstavaná
  funkcia `sleep(ms)`).** Prvé **príkazové, behaviorálne** anotácie v SLeeLa:
  píšu sa na samostatný riadok pred slučku `for` / `while` / `do` a vkladajú do
  jej vykonávania skutočnú pauzu. `@loopDelay(ms = 250)` /
  `@loopDelay(seconds = 2)` uspí slučku na pevný čas v každej iterácii (holé
  `@loopDelay(250)` predvolene znamená milisekundy); `@loopBudget(seconds = 10,
  iterations = 1000)` rovnomerne rozloží známy počet iterácií na celkový
  rozpočet reálneho času. Obe sa prekladajú do nového append-only operačného
  kódu **`OP_SLEEP`** (ISA 157), ktorý podporuje aj všeobecnú vstavanú funkciu
  **`sleep(<ms>)`** volateľnú kdekoľvek; pauza je implementovaná prenosne
  (`nanosleep` / `Sleep`). Špecifikované v
  [`SLEELA.syntax` §32](SLEELA.syntax); referencia API v
  [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md);
  spustiteľná ukážka
  [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela).
  Index doplnení z tejto etapy (vrátane tohto, natívneho násobenia veľkých
  celých čísel `OP_BIGMUL` a samostatných nástrojov
  **[local-generator](local-generator/README.md)** a
  **[pi-generator](pi-generator/README.md)**) sa nachádza v
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md).
  Tri generované povrchy API
  ([`sleeladoc-api/`](sleeladoc-api/README.md),
  [`docs/api/`](docs/api/README.md) a nemenný
  snímok [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html))
  boli znovu vytvorené pomocou `make api-docs` a na stránke balíka `vm` teraz
  obsahujú nové záznamy inštrukcií `SLISASleep` / `SLISABigMul`.
- **SleelaDoc z príkazového riadka — `sleela docs`.** K SleelaDoc sa teraz dá
  dostať priamo z terminálu. `sleela docs` vypíše všetky zdokumentovateľné
  témy; `sleela docs crypto` zobrazí balík; `sleela docs stringbuilder`
  vyhľadá triedu (bez ohľadu na veľkosť písmen a podľa prefixu štandardnej
  knižnice `SL` → `SLStringBuilder`) a vypíše bohatú stránku — **definíciu,
  zámer návrhu, vzťahy, štruktúru, používateľské clays a päťstupňový rebrík
  majstrovstva** — s hodnotenými návrhmi „mali ste na mysli“ pri blízkej
  zhode. Autori môžu ktorúkoľvek stránku obohatiť jednoducho pridaním značiek
  SleelaDoc `@` (`@intent`, `@thesis`, `@axiom`, `@param`, `@see`,
  `@mastery`). `sleela docs --generate [dir]` zapíše textový korpus (jedna
  stránka na triedu + `TOPICS.index`), ktorý CLI potom číta offline;
  **`make sleeladoc`** zbalí tento korpus **a zodpovedajúce zdrojové súbory
  `.sleela`** do `sleeladoc/build/sleeladoc.jar`. Pre predmety len s fasádou
  (napr. `psychiatry`, `politico`) `sleela docs --facade <topic>` za behu
  prehľadá súborový systém a vypíše skutočné **priečinky (potom súbory ako
  prirodzený pohyb)**, ohraničené používateľom prepísateľnými limitmi **6 MB /
  6000 súborov / 6 minút**. Jednotky `lib/sleeladoc` sa udržiavajú na
  aktuálnom vydaní, **`#sleela 3.0`**. Pozrite si
  [`man/man1/sleela.1`](man/man1/sleela.1) a
  [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md).
- **SleelaDoc — dokumentačné riešenie v štýle Javadoc s generovanou HTML
  stránkou.** **SleelaDoc** je konvencia dokumentačných komentárov v zdrojovom
  kóde SLeeLa (komentáre v uhlových zátvorkách označené `@doc` / `@page`;
  pozri [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)) a
  [`lib/sleeladoc`](lib/sleeladoc/README.md) je **extraktor**, ktorý
  vykreslí jednotku `.sleela` ako stránku API v Markdown alebo HTML — tak, ako
  `javadoc` vykresľuje Javu — a dokumentuje *zámer*. Kompletná, prehľadávateľná
  **HTML dokumentačná stránka pre celý `/lib`** sa generuje do
  [`/sleeladoc-api`](sleeladoc-api/README.md) pomocou
  `tools/generate-sleeladoc-api.py` (otvorte `sleeladoc-api/index.html`; čistý
  štýl modrej / svetlomodrej / svetlosivej / bielej s hierarchiou balík →
  trieda → člen a slovníkom písaným pre čitateľa *Student 2*). Kompilátor
  tiež ako vedľajší produkt kompilácie vydáva dokumentačné výstupy označené
  autorom (stránka MediaWiki `--wiki` + pôvodný zdrojový dvojník),
  zdokumentované v [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md).
  Spustiteľná ukážka:
  [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela).
- **Objektový model dokumentu XML (`lib/xml`).** XML DOM na strane SLeeLa —
  dokument vlastniaci strom uzlov element/text/komentár/CDATA/PI s usporiadanými
  potomkami a pomenovanými atribútmi, serializátor (kompaktný/pekný, escapovanie
  podľa XML 1.0 §2.4), parser nad znakovými bunkami a register kvality
  `SLXmlAspect`. Rovnosť uzlov je trojstupňová väzba dôvery (identita → ten
  istý subjekt → štruktúra), reflexívna, symetrická a deterministická. Pozrite
  si [`lib/xml/README.md`](lib/xml/README.md) a spustiteľnú ukážku
  [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela).
- **Široké znamienkové celočíselné primitíva (`long`, `int64`, `int128`).**
  `int`, `long` a `int64` sú všetko 64-bitové znamienkové celé čísla, ktoré sa
  typovo kontrolujú a vykonávajú rovnako; `int128` sa prijíma ako celočíselný
  typ (zatiaľ na 64-bitovom základe, pričom skutočné 128-bitové riešenie je
  zdokumentované ako ďalší krok). Rozširujú ich dvojkové znamienkové rady
  `int32`..`int16777216` (syntax 2.7) a zodpovedajúci neznamienkový rad
  `uint32`..`uint16777216` (syntax 2.8). Pozrite si
  [`SLEELA.syntax`](SLEELA.syntax) §21, §24, §25.
- **Následné polia `@mix` (syntax 2.9).** Trieda môže deklarovať viacero
  inštančných polí s **rovnakým názvom**, ak každé opakovanie nesie `@mix`;
  jazyk takúto skupinu chápe ako následnú (pole / zoznam indexovaný indexom),
  zhora nadol `1,2,3,…` predvolene alebo ručne cez `@mix(1,2,3,...)`, pričom
  počet indexov musí zodpovedať počtu polí. Pozrite si
  [`SLEELA.syntax`](SLEELA.syntax) §28.
- **Značky dokumentu — Prirodzený proces, titulovaný ako Preonems
  (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).**
  Tieto holé slová značiek dokumentu sa súhrnne titulujú **Preonems**
  (jedna značka je *Preonem*; množina, *Preonems* — pozri
  [`SLEELA.syntax`](SLEELA.syntax) §26.1). Prvotriedne, výlučne deklaratívne
  značky dokumentu pomenúvajúce, čím dokument *je*, v poradí tvorby:
  `author`/`authorial` (syntax 2.4; spracúva ich odborná rada — jediný člen s
  behaviorálnym účinkom), `material;` (ustálený Zdroj) a `memory;`
  (syntax 2.5), `determine;` (syntax 2.6; dokument otvorený na ďalšie
  spracovanie lekárom, lekárnikom alebo duchom smerom k dokončeniu zákonnou
  rukou) a dve najnovšie, **`object;`** (ustálená vec stojaca v katalógu
  toho, čo jestvuje — **zreifikovaná, s konkrétnym postavením, v syntaxi
  2.8**) a **`field;`** (základ vzťahu, v ktorom objekty stoja a pohybujú sa)
  — obe podmienené syntaxou 2.6. Každá je holá, kontextová a verziovo
  podmienená frontendom, bez vplyvu na parsovanie, typovanie či generovanie
  kódu; mimo pozície značky zostáva každá bežným identifikátorom (takže
  `field` nikdy neruší člena triedy ani prístup `.field`). Pozrite si
  [`SLEELA.syntax`](SLEELA.syntax) §18–§20, §23 a §26.
- **Nordshrift v syntaxi 3.0 s plným dosahom na knižnice.** Nordshrift sa
  rozlišuje cez ten istý rekurzívny index `/lib` ako kompilátor a dosahuje
  každý balík a jednotku `.sleela`; `nordshrift check` hlási počty pre celý
  strom.
- **Recursion stanchion.** Adresár [`recursion/stanchion/`](recursion/stanchion/README.md) teraz obsahuje samostatné referenčné prototypy v C a C++ pre sedem zdokumentovaných primitívnych zmlúv, ako aj všeobecné, aplikované, vedecké a výukové príklady. Ide o referenčné zdroje; pred ich pokladaním za overené ich skompilujte a otestujte.

- **Komentáre v uhlových zátvorkách (`<` / `<<`).** Nový štýl komentárov
  na ďalšie používanie — `< line >` a `<< block >>` — lexikálne spracúvaný
  frontendom SLeeLa a zdokumentovaný v
  [`SLEELA.syntax`](SLEELA.syntax) §1.1.
- **Celočíselné literály v rôznych sústavách.** Literály `int` teraz
  akceptujú šestnástkový (`0x`), binárny (`0b`) a osmičkový (`0o`) tvar
  s oddeľovačmi číslic `_`, a to v zdrojovom jazyku SLeeLa
  ([`SLEELA.syntax`](SLEELA.syntax) §1.4) aj vo
  [formáte modelov CPU `.xlt`](lib/xlt/README.md).
- **Štandard Metadata Class v celej knižnici.** Metavzor opisných metadát
  (pôvod, zákonnosť, jurisdikčný rozsah) nesie teraz každý súbor `.sleela`
  s triedami pod `/lib`; pozrite si
  [`SLEELA.syntax`](SLEELA.syntax) §15 a
  [lekciu v tutoriáli](tutorial/13-metadata-class.md).
- **Bohatšia rodina text / String.** `lib/text` získava skutočných pomocníkov
  v jazyku, pohľad na znakové bunky poskytujúci skutočné `charAt`/`substring`/
  `indexOf`/`reverse`/zarovnávanie stĺpcov a skutočný pozičný šablónovač —
  pozrite si [tutoriál o texte a String](tutorial/14-text-and-strings.md) a
  spustiteľnú ukážku
  [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela).
- **Prvotriedny `String` v hárkoch Nordshrift.** Hárok `.sst` môže deklarovať
  typovaný blok `strings:`, ktorý prispieva do manifestu komponentov
  zostavenia; pozrite si [Referenciu hárkov Nordshrift](wiki/Nordshrift-Sheet-Reference.md).
- **Modely CPU sa vykonávajú operačnými kódmi aj ekvivalenciou volania
  funkcií.** Každé `SL<ARCH>CPU` zdedí základ vykonávania natívneho toku
  inštrukcií cez svoju slučku fetch-decode-execute *a zároveň* vykonávania
  kódu predpokladaného iného procesu ekvivalentnými volaniami na zdieľanom
  substráte VM — pozrite si [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5.

## III. Zostavenie a testovanie

**Požiadavky:** nástrojový reťazec C/C++ (GCC alebo Clang) a GNU Make; Python 3
pre nástroje manifestov a tokov; v Linuxe vývojové balíky X11/Xrandr pre
zobrazovacie produkty (`make deps` modulu ohlási/nainštaluje, čo potrebuje).
Zostavenie `impl/` je fail-closed voči dôveryhodnému manifestu SHA-256, na ktorý
koreňový `Makefile` automaticky odkazuje.

Z čistého checkoutu je najrýchlejší štart:

```sh
make                                  # build everything from the repo root
```

Alebo zostavte a otestujte iba natívne jadro:

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

Koreňový `make` v repozitári zostaví každý produkt (jadro, VM, kodeky, churn,
antivírus, …) a končí zostavením **registra** a vytvorením jeho základnej
tabuľky — takže zostavený strom ponúka revidovateľný register hneď po
zostavení. Základné ciele:

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

Ďalšie užitočné ciele:

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

Cieľ `registry` spúšťa [`build-registry.sh`](build-registry.sh): zostaví
jadro registra, nasype základnú tabuľku do prenosného textového úložiska
`registry/build/sleela.sleelareg` a exportuje skript tabuľky MySQL
`registry/build/registry.sql` (cez `sleela registry export`). Skontrolujte ju
pomocou `sleela registry` alebo `registry/build/sleela-registry review`. Pozrite
si [príručku registra](registry/README.md) a [architektúru Registry 3.0](registry/REGISTRY-3.0-ARCHITECTURE.md).

Prerekvizity a kroky špecifické pre platformu nájdete v príručkách zostavovania a testovania. Úspešné zostavenie jednej súčasti nedokazuje, že každá knižnica alebo platforma je pripravená na vydanie.
## IV. Inštalácia

Po zostavení natívnych produktov nainštalujte príkazy a súbory podpory runtime prostredia:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

Bežný inštalátor predvolene používa `/usr/local/bin`, `/usr/local/lib/sleela`
a `/usr/local/share/sleela`. Zachováva existujúcu úvodnú konfiguráciu
a odmieta prepísať existujúce príkazy, ktoré nie sú sledované. Odinštalovanie
odstráni iba súbory zaznamenané inštalátorom a zachová konfiguráciu a
nesledované údaje runtime prostredia.

Na cielené, riadené nasadenie iba hlavného balíka — binárnych súborov `sleela`,
`sleelvac` a `nordshrift`, štandardnej knižnice, protokolov a podporných
modulov — použite čistý inštalátor:

```sh
make clean-install       # guided install of the main package only
```

Urobí jedno primárne rozhodnutie (nainštalovať hlavný balík), potom voľbu
rozsahu a po úspechu voliteľný výber vybraných rozširujúcich modulov. Interaktívny
postup nasadenia a všetky možnosti inštalácie nájdete v dokumente
[Rýchla a bezpečná inštalácia](install/README.md).

## V. Jazyk a runtime prostredie
Záväznou gramatikou je [`SLEELA.syntax`](SLEELA.syntax). Aktuálny repozitár dokumentuje syntax **3.0**; verzia kompilátora/nástroja a verzia zdrojovej syntaxe sú odlišné pojmy. Povrch 3.0 pridáva **značky dokumentu `pure` a `gasoline`** (§30) — dvojicu holých, kontextových, výlučne deklaratívnych Preonems, ktoré znamenajú takmer to isté, pričom každá označuje postavenie dokumentu svojou vlastnou úprimnosťou. Syntax 3.0 zjednocuje jazykový povrch 1.0–2.9 do súčasného normatívneho vydania a zachováva všetky minimálne brány špecifické pre jednotlivé funkcie. Predchádzajúci povrch 2.9 pridáva **následné polia `@mix`** (§28) — trieda môže deklarovať viacero rovnomenných inštančných polí, ak každé nesie `@mix`, chápaných ako jedna indexovaná skupina (zhora nadol `1,2,3,…` alebo ručne `@mix(1,2,3,...)`, pričom počet indexov musí zodpovedať skupine). Prenáša ďalej dvojkové rodiny širokých znamienkových/neznamienkových celých čísel `int32`..`int16777216` (§24) a `uint32`..`uint16777216` (§25) nad skoršími `long`/`int64`/`int128` (§21); **dokumenty podporovateľov kľúčových slov** (§22); a **Preonems** — výlučne deklaratívne značky dokumentu `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) a `artifact` (§35, postavenie na úrovni zákonodarstva odstupňované vzhľadom na čas a priestor), súhrnne titulované Preonems (§26.1). Rozlíšenie preťaženia konštruktorov a delegovanie `this(...)`/konštruktora bázy sú výslovne mimo overenej zmluvy syntaxe 1.8. Skôr než sa na funkciu spoľahnete, pozrite si gramatiku a poznámky k implementácii. Deväť zdrojových jednotiek SleelaDoc teraz deklaruje `#sleela 3.0`; spoločná brána kompilátora akceptuje zdrojovú syntax 1.0–3.0. Nové zostavenie API vzniká pomocou `make api-docs`.

Repozitár obsahuje natívnu implementáciu C/C++, referencie generovania SLVM, moduly štandardnej knižnice, testy, príklady a platformové integrácie. Stav implementácie a validácie sa líši podľa modulu; namiesto predpokladu rovnomernej úplnosti si pozrite jeho špecifikáciu a aktuálne výsledky CI.

## VI. Inžinierske princípy
- **Dôkazy namiesto tvrdení:** rozlišujte implementované, testované, experimentálne a plánované správanie.
- **Jedna zmluva na každú hranicu:** zdokumentujte syntax, API, formáty súborov, predpoklady o platforme a správanie pri zlyhaní.
- **Prenosnosť návrhom:** izolujte backendy špecifické pre platformu a testujte na natívnych runneroch.
- **Bezpečné zlyhanie:** testujte chybný vstup, limity zdrojov, zlyhania procesov a cesty upratovania.
- **Reprodukovateľné vydania:** zostavujte znovu načisto a zverejňujte hashe artefaktov a poznámky k vydaniu.

## VII. Zostavenie antivírusu a integrita

Antivírusový balík Sleelavirin obsahuje natívny skener SHA-256, voliteľný
adaptér ClamAV, manifesty integrity zdrojov/indexov a kontrolné súčty
platformových balíkov. Balík zostavte a otestujte z koreňa repozitára:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Manifest integrity sa generuje pod `antivirus/build/`; zahŕňa sledované
vstupy zdrojov a dokumentácie plus chránené súbory indexu podpisov, pričom
vynecháva generovaný výstup a meniteľnú vyrovnávaciu pamäť ClamAV. Snímky
databázy ClamAV majú vlastné manifesty. Archívy ZIP vydania obsahujú
`INTEGRITY-MANIFEST.json`, `SHA256SUMS` a doplnkový súbor `.sha256`. Hashe
stiahnutých archívov overte voči dôveryhodnej hodnote získanej nezávisle:
kontrolný súčet uložený vedľa archívu nie je podpisom vydavateľa. Pozrite si
[príručku Sleelavirin](antivirus/README.md),
[bezpečnostný kontrolný zoznam](antivirus/SECURITY.md) a
[pracovný postup zostavenia antivírusu](.github/workflows/antivirus-binaries.yml).

Vydanie sa nepovažuje za overené, kým relevantné úlohy CI neprejdú na presnom
commite vydania a výsledné artefakty nie sú skontrolované.


## VIII. Editorové témy SLeeLa — Vim a Emacs

SLeeLa obsahuje koordinovanú sadu profesionálnych tmavých farebných schém pre **Vim** a **Emacs** a natívny nástroj na palety v C/C++. Pôvodná schéma **American Professional** zostáva predvolená. Desať sprievodných paliet je dostupných pre rozdielne kultúrne a inštitucionálne dizajnové inšpirácie; ide o interpretačné dizajnové palety, nie o oficiálne zobrazenia ktoréhokoľvek národa, kultúry, súdu alebo inštitúcie.

### Jedenásť schém a ich farby

| Schéma | Pozadie | Popredie | Kľúčové slová | Typy | Reťazce | Komentáre |
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

#### Národné vlajkové farby (zdrojová paleta)

Grécka a turecká schéma sú odvodené z **oficiálnych národných vlajkových farieb** oboch krajín, tu uvedených s ich presnými zverejnenými hodnotami hex. Pozadia/akcenty schém vyššie sú stmavené interpretácie týchto zdrojových farieb vhodné pre editor — tabuľka nižšie je referenciou „farieb tak, ako sú národne vystavené“.

| Štát | Farba | Hex | Základ a význam |
|---|---|---|---|
| **Grécko** | Modrá | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | Modrá národnej vlajky (zákon 851/1978). Nebo a moria; heraldicky blazonovaná ako *azure*. |
| **Grécko** | Biela | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Biela národnej vlajky. Čistota boja za nezávislosť; kríž označuje východné pravoslávne kresťanstvo. |
| **Turecko (Türkiye)** | Červená (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | Červená národnej vlajky (turecký zákon o vlajke). Pole; podľa tradície krv mučeníkov a osmanská zvrchovanosť. |
| **Turecko (Türkiye)** | Biela | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Biela národnej vlajky. Polmesiac a hviezda — mier, čistota a symbol tureckého/islamského dedičstva. |

Schémy ukotvujú popredie každého editora na národnej **bielej** a jeho kľúčové slová, kurzor, výber a stavový riadok na presnej národnej **modrej** (`#0D5EAF`) alebo **červenej** (`#E30A17`), takže farby, na ktorých záleží, sú doslova národnými farbami. Ide o interpretačné dizajnové palety; referencia vlajkových farieb sa poskytuje kvôli pôvodu a nie je oficiálnym zobrazením ani jedného z oboch štátov. Významy farieb vychádzajú z opisu štátnych symbolov ministerstva zahraničných vecí Helénskej republiky a z tradície tureckého zákona o vlajke.


### Tabuľka farebných vzoriek

Bunky paliet nižšie používajú **vzorky SVG**, takže GitHub vykreslí skutočné farby namiesto prázdnych buniek HTML. Každý pásik zobrazuje šesť farieb v tomto poradí: pozadie, popredie, kľúčové slová, typy, reťazce a komentáre. Presné kódy hex sú uvedené vedľa každej vzorky v rovnakom poradí schém ako v referenčnej tabuľke vyššie.

| Schéma | Vykreslené farby palety | Kódy hex (pozadie · popredie · kľúčové slová · typy · reťazce · komentáre) |
|---|---|---|
| **American Professional** | ![American Professional palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Pan-Eurasian palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russian and Slavic — Medium** | ![Russian and Slavic Medium palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Central Asian / Silk Road** | ![Central Asian Silk Road palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **East Asian** | ![East Asian palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC — International Criminal Court** | ![ICC palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS — U.S. Supreme Court** | ![SCOTUS palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Greek (Hellenic)** | ![Greek Hellenic palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turkish (Al Bayrak)** | ![Turkish Al Bayrak palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Kiro palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![SLeeLa palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

Ďalšie akcentové farby sa používajú pre funkcie, konštanty, výbery, diagnostiku a direktívy preprocesora. Ruská a slovanská paleta zámerne používa bridlicovú, modrú, šalviovú, rubínovú a staroželtú zlatú strednej intenzity pre pokojnejší profesionálny vzhľad.

### Inštalácia schém pre Vim

Z koreňa repozitára:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Do `~/.vimrc` pridajte nasledujúce, aby ste zapli zvýrazňovanie syntaxe SLeeLa a zvolili schému:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Zvoľte presne jeden z týchto názvov colorscheme:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Napríklad `colorscheme sleela-russian-slavic` použite pre strednú ruskú a slovanskú paletu alebo `colorscheme sleela-greek` / `colorscheme sleela-turkish` pre helénsku paletu a paletu Al Bayrak. Vim reštartujte alebo načítajte konfiguráciu pomocou `:source $MYVIMRC`.

### Inštalácia schém pre Emacs

Vytvorte lokálny adresár tém a skopírujte súbory režimu a tém z koreňa repozitára:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Pridajte toto do `~/.emacs` alebo `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Nahraďte `sleela-american` hodnotou `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` alebo `sleela-sleela`, aby ste zvolili inú schému. Ak sa Emacs spýta, či dôverovať téme, pred prijatím skontrolujte lokálny súbor témy.

### Náhľad natívnych paliet C/C++

Malý natívny nástroj na palety dokáže vypísať schémy, vytlačiť ich farby pre terminál a vyhľadať jednotlivú farbu:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Pre spätnú kompatibilitu `--palette` a `--hex COLOR` bez `--theme` vyberú American Professional. Výsledky zostavenia a testov treba overiť na cieľovom systéme a v CI; dokumentácia príkazu sama osebe nie je dôkazom, že prešiel.

Úplné opisy paliet a podrobnosti o nástroji nájdete v [príručke integrácie editorov](linux-editors/README.md).
## IX. Prispievanie
1. Prečítajte si register dokumentácie a zásady testovania.
2. Robte cielené zmeny s pozitívnymi aj negatívnymi testami.
3. Spustite cielené testy a potom širokú sadu.
4. Uveďte údaje o OS a nástrojovom reťazci; preskočené kontroly neoznačujte za úspešné.

SLeeLa vyvíja **MEARVK LLC**. Historický materiál úvodnej stránky je zachovaný v [`docs/README-LEGACY.md`](docs/README-LEGACY.md).

## X. AE6E66 — Očakávanie odvodu z Round Public e-mailu

Pre iniciatívu AE6E66 je deklarovaným očakávaním projektu **$300,000,000.00 USD za jeden Round Public e-mail** ako navrhovaná výška daňového odvodu spojeného s každým kvalifikujúcim sa Round Public e-mailom.

Toto vyhlásenie zaznamenáva iba očakávanie projektu. **Nie je** to právne daňové vyrubenie, vládny príkaz, súdny rozsudok ani nezávisle preukázaná daňová povinnosť. Akýkoľvek skutočný odvod by vyžadoval príslušnú právnu autoritu, jurisdikciu, definície, postup a rozhodnutie spôsobilého vládneho orgánu.


## XI. Abeceda a runtime znakových sád

Dokumentácia abecedy SLeeLa a natívny znakový runtime sa udržiavajú v alphabet/sleela/ a lib/alphabet/native/. Natívne testy v C++17 pokrývajú striktné dekódovanie/kódovanie skalárov UTF-8, podpisy kapacity C<n> a ohraničenú príslušnosť k znakovej sade:

    make -C lib/alphabet/native test

Experimentálny katalóg UTF-4088 a jeho vyhľadávací adaptér pre SLeeLa sú zdokumentované v utf-4088/SLeeLa-ALPHABET-INTEGRATION.md. Identifikátory UTF-4088 sú samostatný menný priestor, nie skalárne hodnoty Unicode. Práca na väzbách kompilátora a SLVM sa sleduje výslovne oddelene od týchto natívnych základov.

## XII. AutoCAD Creator™

[AutoCAD Creator™](autocad/creator/README.md) je interaktívna 2D CAD
aplikácia napísaná celá v SLeeLa. Zatiaľ čo súrodenecký AutoCAD Renderer
premení plán na výkres jedným ťahom, Creator vám umožňuje 2D dokument
AutoCAD naživo **vytvárať a meniť** — pridávať steny, miestnosti, kóty
a poznámky, potom ich presúvať, premenovávať, preradiť do inej vrstvy,
mazať a vrátiť späť, čo ste umiestnili — a výsledok **uložiť** ako
skutočný výkres AutoCAD DXF (text R12, otvorí sa v AutoCADe / LibreCADe).

Jeho grafické rozhranie je postavené na **SleelaUI™**, vlastnom multiplatformovom
UI toolkite SLeeLa (pod [`lib/user-interface`](lib/user-interface)) — nie na JavaFX —
a otvára skutočné natívne okno na X11 / Cocoa / Win32. Návrh sa riadi jedným
pravidlom, **„SleelaUI predstavuje; SLeeLa rozhoduje“**: GUI nikdy nepočíta
geometriu ani nezapisuje DXF. Každý nástroj posiela zámer vytvoriť / zmeniť /
zmazať / vrátiť späť / uložiť autoritatívnej triede
[`SLCadCreator`](lib/autocad/SLCadCreator.sleela), ktorá vlastní upraviteľný
dokument a vratnú históriu príkazov, kým plátno iba zobrazuje náhľad
geometrie, ktorú Creator drží.

Opakovane použiteľný editačný engine sa nachádza v [`lib/autocad`](lib/autocad) —
[`SLCadEntity`](lib/autocad/SLCadEntity.sleela),
[`SLCadDocument`](lib/autocad/SLCadDocument.sleela),
[`SLCadCommand`](lib/autocad/SLCadCommand.sleela) a
[`SLCadCreator`](lib/autocad/SLCadCreator.sleela) — a beží od začiatku do konca
bez displeja. Obal GUI a ukážka sú v [`autocad/creator`](autocad/creator):

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

Na displeji sa otvorí živé editačné okno; na hostiteľovi bez displeja sa výkres vytvorí v pamäti, vypíše sa správa plus DXF a program sa čisto ukončí.


## XIII. Poradie vstupnej mapy UTF-4088

Špecializovaný katalóg UTF-4088 definuje prechod svojej priestorovej vstupnej mapy a abstraktné iterácie tlaku v [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). Politika predmetového indexu National Lien je v [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Súradnice mapy, úrovne tlaku (1–50), trvalé identifikátory katalógu a abecedné poradia sú nezávislé polia; žiadne z nich sa nesmie potichu nahradiť iným. Tlak je algoritmická úroveň iterácie, nie fyzikálna veličina.


## XIV. Nedávny a rozšírený index balíkov

Tento pripojený index zvýrazňuje nedávno rozšírené balíky, doménové knižnice
a zbierky zdrojov. Položky sú zoskupené abecedne podľa názvu balíka alebo
témy. Opisy určujú umiestnenie v repozitári; stav implementácie a validácie
treba overiť v dokumentácii a testoch každého balíka.

### A–C

- **Advantage** — `lib/advantage/` a `advantage/`: zvýhodnenie jedinečnej
  dividendy vzhľadom na model ako vrstvená cibuľa Sequitur, s piatimi
  tematickými podpriečinkami — `math/` (spoločná maximalizácia zisku,
  Nashovo vyjednávanie, bilaterálny newsvendor), `physics/` (metrika/geodetika/
  kontakt/SDE poľa dôvery, označená modelová analógia), `integration/`
  (oddelenia vertikálnej integrácie a skóre sofistikovanosti), `grading/`
  (hodnotiteľ sofistikovanosti A–D pre moduly a množiny) a `output/`
  (emisia do terminálu / CommonRails / socketu). Natívny konzument C/C++
  (`advantage/`) prepája model s konkrétnymi obchodnými príkladmi. Pozrite si
  [`lib/advantage/README.md`](lib/advantage/README.md)
  a [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alphabet a UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/`
  a `utf-4088/`: katalógy abecied, natívna podpora znakových sád, poradie
  vstupnej mapy a dokumentácia integrácie UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` a `lib/autocad/`: interaktívne
  pracovné postupy 2D kreslenia a opakovane použiteľné komponenty dokumentu
  CAD a úprav.
- **Ballum** — riadenie displeja na úrovni pixelov: nastavenie/čítanie
  po pixeloch, blit oblasti, jas/teplota/gama celého displeja, vstup z
  ukazovateľa/klávesnice a **povrch fullscreen framebuffera** pre úplné,
  vlastnené, rýchle riadenie po pixeloch (okno override-redirect + jediné
  predloženie `XPutImage`/MIT-SHM v Linuxe/X11). Natívne jadro pod `ballum/`
  s fasádou SLeeLa v `lib/ballum/` a mostom VM pod `impl/`; pozrite si
  [`ballum/README.md`](ballum/README.md) a spustiteľné
  [`ballum/example/`](ballum/example/).
- **CDMA** — `cdma/` a `lib/cdma/`: technické materiály súvisiace s CDMA
  a práca na knižnici zdrojov SLeeLa.
- **Character** — `lib/character/`: profily postáv a opakovane použiteľné
  štatistiky postáv vrátane príkladov biografických profilov.
- **Coorenagraph** — `lib/coorenagraph/`: etické uvažovanie, morálna
  slovná zásoba, deliberácia a súvisiace pojmy; pozrite si README balíka
  a jeho zdrojové súbory.

### D–G

- **Governance source models** — [lib/governance](lib/governance/README.md) definuje deklaratívne zdrojové tvary SLeeLa pre návrhy, anonymné ručné hlasovacie lístky a súhrnné sčítania. Navrhovaná anotácia @vote a ručný proces sú zdokumentované v [registry/DEMOCRATIC-VOTING-SYNTAX.md](registry/DEMOCRATIC-VOTING-SYNTAX.md); súčasné kompilátory ju považujú za inertné metadáta komentára.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` až dostupné verzované adresáre
  balíkov HTTP: špecifikácie protokolov a implementácie špecifické pre
  verziu. Presný rozsah a stav overenia zistíte v dokumentácii každej verzie.
- **International Criminal Court** — `lib/international-criminal-court/`
  a `INTERNATIONAL.CRIMINAL.COURTS.md`: súvisiaci doménovo špecifický zdroj
  a dokumentácia.

### O–S

- **OFDMA** — `ofdma/`: technické materiály o viacnásobnom prístupe s
  ortogonálnym frekvenčným delením a vývojové zdroje C/C++.
- **Philosophy a modul Conversation Mask** — [`philosophy/`](philosophy/README.md)
  obsahuje natívne základy v C/C++ a [`philosophy/module/`](philosophy/module/README.md)
  organizuje ich ďalšiu modulovú vrstvu do troch výslovných oblastí:
  [`data/`](philosophy/module/data/README.md) pre typované, datované pozorovania,
  konce udalostí, nastavenia polčasu, pôvod a súhlas; [`helper/`](philosophy/module/helper/README.md)
  pre deterministickú validáciu, útlm relevantnosti, kompatibilné porovnania
  a prezentáciu filtrovanú súhlasom; a [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md)
  pre podporné, neklinické konverzačné rozhranie. Rukoväte obrátené k SLeeLa
  a zložený Tree sa nachádzajú v [`lib/philosophy/`](lib/philosophy/README.md),
  spolu so širším balíkom philosophy. Modul stavia na výslovnom vstupe a na
  súhlase: pasívne nesleduje ľudí ani médiá, nediagnostikuje, nevyvodzuje
  súkromné zámery, nehodnotí ľudskú hodnotu ani neodporúča zmeny liekov.
  Odkazy medzi udalosťami zachovávajú pôvod a navigáciu, nie nezávislé
  potvrdenie.
- **Synchro** — integrácia natívneho runtime/kompilátora a funkcionalita
  súvisiaca s odosielaním paketov a meraním; pozrite si príslušný zdroj
  v `impl/` a implementáciu kompilátora.
- **SleelaUI / terminálové UI** — `lib/user-interface/` a projekt
  `sleela-terminal`: integrácia UI SLeeLa a komponenty interakcie
  orientované na terminál.

### U–W

- **United Nations** — `united-nations/`: doménovo špecifické materiály repozitára.
- **Wi-Fi 4, Wi-Fi 5 a Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`,
  `wi-fi-6/` a súvisiace zdrojové adresáre `lib/wifi/`: materiály o
  bezdrôtových sieťach špecifické pre generáciu a konfiguračné zdroje.

### Údržba indexu balíkov

Keď sa balík pridá alebo podstatne rozšíri, pridajte jeho položku sem do
príslušnej abecednej skupiny, odkážte na jeho kanonický adresár a README
a opíšte iba možnosti podporené zdrojmi v repozitári. Tento index udržiavajte
aditívne: pri aktualizáciách indexu balíkov neodstraňujte ani neprepisujte
existujúce sekcie README.
---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

US Democratic and US Policy.

---

## XVI. Národné obrazové materiály značky Science

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. Experimentálny katalóg UTF-4088 s miliónom položiek

[Generátor katalógu UTF-4088](utf-4088/tools/generate_catalogue.py) deterministicky vytvára experimentálny katalóg symbolov s 1 048 576 položkami. Zdokumentovaný kompaktný formát je opísaný v [utf-4088/FORMAT.md](utf-4088/FORMAT.md); zmluva o generovaní a validácii je v [utf-4088/GENERATION-RESULTS.md](utf-4088/GENERATION-RESULTS.md).

Generátor spustite pomocou Pythonu 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Vygenerovaný binárny súbor má 16 777 244 bajtov a je navrhnutý tak, aby zostal pod 20 000 000 bajtov. Jednotkové testy a pracovný postup GitHub Actions overujú počet položiek, jedinečnosť glyfov, veľkosť a SHA-256; úspešné behy zverejňujú binárny súbor a súhrn ako artefakt pracovného postupu `utf4088-million-catalogue`. Vygenerovaný binárny súbor sa zámerne uchováva mimo histórie Gitu. UTF-4088 zostáva experimentálny a nie je kompatibilný s Unicode.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---


## XVIII. Nedávne doplnenia — konštanta Pi, štart Modium/Registry, matematika sleelactl a `@native`

Táto sekcia zaznamenáva funkcie pridané počas práce v 2026-11, najnovšie skupiny na začiatku. Prepojené zdrojové súbory a stav ich validácie sú záväzné; zmeny v revízii sa nepovažujú za zlúčené, kým sa nedostanú do `master`.

### Konštanta katalógu Pi so 100 000 000 číslicami

Repozitár dodáva overenú hodnotu π so **sto miliónmi číslic** v súbore [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (správne utvorené desatinné číslo, `3.14159…`) spolu s hodnotou s 57 344 číslicami [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Obe sú nezávisle overené ako skutočné π — pozrite si [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md), ktorý dokumentuje kontroly (učebnicový prefix, Feynmanov bod na desatinnej pozícii 762, slávnu miliónitú číslicu, zhodu s `lib/math/SLPiReference.sleela` a analýzu frekvencie číslic celého súboru) a jednorazový skript na reprodukciu.

### Modium™ / Registry™ načítavajú konštantu Pi pri štarte subsystému

Na vyhradených vstupných bodoch subsystémov — `sleela registry` a `sleela modium` (a pri prvom spustení inštalátora), **nie** pri každom `sleela run` — subsystémy Modium a Registry načítavajú veľkú konštantu katalógu π. V interaktívnom termináli vypíšu prechodný červený riadok `Loading Modium™` / `Loading Registry™`, ktorý sa potom na mieste vymaže ešte pred začiatkom bežného výstupu; účinok je prísne viazaný na TTY, takže výstup cez rúru a testovací výstup zostávajú bajt po bajte nezmenené. Lokátor (nie bajty) je nasadený ako predvolené hodnoty kľúča registra `Math` `piReferenceLargePath` a `piReferenceLargeDigits`, a to v modeli SLeeLa ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) aj v natívnom seederi ([`registry/sleela_registry.c`](registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` sprístupňuje `largePath()` / `largeDigits()`. Konštanta π sa inštaluje do umiestnenia SLeeLa v OS (`<data>/pi/`) **oboma** štandardným inštalátorom `make` aj riadeným „krásnym“ inštalátorom. Podrobnosti: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — krásny inštalátor a `math` s natívnym pozadím

- Riadený („krásny“) inštalátor teraz inštaluje `sleelactl` (riadiacu rovinu SLeeLa) a jeho register modulov, v súlade s inštalátorom `make`. Pozrite si [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- `sleelactl` získal novú povahu **`subject`** pre natívne knižnice predmetov v C/C++ skompilované do runtime prostredia a registruje prvú z nich: **`math`**. `sleelactl run math` vykoná natívny backend `__NativeMath` (`impl/subjects/math/math_native.cpp`) od začiatku do konca pomocou referenčnej ukážky predmetu; `sleelactl info math` vysvetľuje, ako sa k nemu pristupuje (`import math;`). Povaha subject ponúka iba `run info` — zámerne tu nie je sloveso `check`, pretože `sleela check` nenačíta natívne predmety zaregistrované cez SHEET.

### `@native`, Native Drive a vykonávanie vo viacerých jazykoch

Nová anotácia metódy **`@native`** žiada o ahead-of-time natívnu kompiláciu horúcej metódy. Nový príkaz `sleela aot` nájde metódy `@native`, overí, že každá patrí do podporovanej *odovzdateľnej podmnožiny*, a vydá C/C++ — **zlyháva uzavreto** (odmietne s presným dôvodom) pri každom konštrukte, ktorý zatiaľ nevie preložiť, takže sa nikdy nevytvorí napoly preložená metóda.

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

`@native` zostáva žiadosťou o spracovanie AOT, pričom nepodporované konštrukty sa odmietnu pred emisiou. [Native Drive](impl/native-drive/README.md) podporuje viacjednotkové zostavenia spustiteľných súborov a zdieľaných knižníc v C/C++ cez hostiteľské kompilátory (vrátane MSVC), podporovaný výstup SLeeLa AOT, kontrolu symbolov voči ABI natívneho loadera, vykonávanie procesov bez shellu, spracovanie stdin/stdout/stderr, viacstupňové rúrovanie a delegovanie XClass na produkčný loader. `sleelactl` sprístupňuje jeho príkazy zostavenia/spustenia a cez `inspect native-drive` smeruje kontrolu XClass a výslovné vykresľovanie/inštaláciu jednotiek systemd. VM vlastní ohraničenú tabuľku rukovätí natívnych modulov s povinnou autorizáciou hostiteľa a rukoväťami kontrolovanými podľa generácie. Syntax 3.0 teraz pridáva `nativeOpen`, `nativeCall` a `nativeClose`: append-only bytecode dispatch prekračuje samostatne verzované ABI null/bool/int64/double/UTF-8, kým polia, štruktúry a iné zložené hodnoty vlastnené VM zostávajú vo vnútri interpretera a môžu používať záložné riešenie bytecode podľa rovnakého názvu. Pluginy môžu voliteľne dostať verzovanú tabuľku spätných volaní hostiteľa na vyvolanie presných funkcií SLeeLa cez vnorené vykonávanie VM, s rootovaním pozastaveného zásobníka pre GC, výslovným zoznamom povolených názvov spätných volaní hostiteľa a odmietnutím rekurzívnych volaní natívneho API. Natívny kód zostáva dôveryhodným kódom v procese, nie sandboxom. Anotácie politiky procesov v zdroji majú verzovanú schému, ale zatiaľ sa neparsujú ani nevynucujú. Podrobnosti, bezpečnostné hranice a ledger siedmich pracovných tokov: [Native Drive](impl/native-drive/README.md) a [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).


## XIX. Medzinárodné vydania README

SLeeLa poskytuje krajinské vstupné body README pre 50 trhov svojho medzinárodného vydania. Každá krajina má jeden súbor s názvom `README.<CC>.md`; ak je relevantných viac jazykov, sú zoskupené v súbore danej krajiny.

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

**Zásady prekladu:** Tieto súbory sú úvodné lokalizované krajinské vstupné body. Ich úvody a jazykové nadpisy sú lokalizované, zatiaľ čo úplná technická referencia zostáva v kanonickom anglickom [README.md](README.md). Krajinský súbor sa nesmie označiť za úplný preklad, kým nie je preložená každá sekcia, skontrolované technické identifikátory a odkazy a kým vydanie neprejde jazykovou revíziou. Pokrytie jazykov v tejto prvej fáze je reprezentatívne a nie je tvrdením, že každý ústavne uznaný, regionálny, domorodý alebo posunkový jazyk bol úplne preložený.

## Príloha — zachované regionálne usmernenie

Táto príloha zachováva praktické usmernenie z predchádzajúceho krátkeho slovenského sprievodcu, ktoré sa v kanonickom README nenachádza ako samostatné sekcie. Pri akomkoľvek rozpore má prednosť kanonické anglické [README.md](README.md).

### Ciele, profily a hranice vykonávania

SLeeLa oddeľuje zdrojový kód od cieľov zostavenia a vykonávania. Dokumentovaná rodina cieľov zahŕňa `SLVM`, `SLJVM` a `SLIR`; `NATIVE` je obmedzený cieľ podmienený podporou, preto nemožno predpokladať jeho dostupnosť na každej platforme. Profily `BASIC_COMPLETE` a `ADVANCED_TOTAL` označujú rozdielny rozsah. Pri každom zostavení overte skutočný cieľ, profil a podporu platformy.

Referencia identifikuje súbor, modul alebo zdroj, ale sama osebe neposkytuje oprávnenie na jeho vykonanie. Pri používaní súborov, externých knižníc, natívnych väzieb alebo systémových zdrojov musia byť jasne určené hranice oprávnení a zodpovednosť za vykonanie. Opis v dokumentácii sám osebe nedokazuje bezpečnostnú záruku runtime prostredia; overte implementáciu a príslušné testy.

### Životný cyklus zdrojov a ladenie

Zdokumentujte vlastníctvo, životnosť a zodpovednosť za synchronizáciu zdrojov zdieľaných medzi funkciami alebo vláknami. Rozlišujte lokálne premenné, zdieľaný stav objektov, rukoväte VM a serializovateľné údaje. Lokálna rukoväť VM nie je stabilný formát súboru ani sieťová správa. Používajte serializačné rozhranie definované modulom a testujte neplatné, chýbajúce a hraničné hodnoty.

Pri ladení zaznamenajte commit, OS, architektúru a nástrojový reťazec. Najprv zostavte najmenší relevantný cieľ, uchovajte úplný chybový výstup, rozlišujte chyby kompilátora od chýb runtime prostredia a vytvorte minimálny reprodukovateľný príklad. Úspešné zostavenie samo osebe nedokazuje správne správanie runtime prostredia.

Validačná správa má uvádzať presný commit, OS/architektúru, verzie kompilátora, úplné príkazy zostavenia a testov, počty pass/fail/skip a známe obmedzenia. Preskočené alebo nespustené testy sa nesmú počítať ako úspešné.

### Zmluvy modulových API, kompatibilita a testovacia stratégia

Pri zavádzaní nového API zdokumentujte vstupy, výstupy, chybové podmienky, vlastníctvo údajov a prípadné vedľajšie účinky. Príklady majú ukazovať bežné použitie aj spracovanie neplatného alebo chýbajúceho vstupu. Správanie neoznačujte za zaručené, pokiaľ ho nepotvrdzuje implementácia a príslušné testy.

Testy rozdeľte podľa etáp kompilačného reťazca: lexikálna a syntaktická analýza, sémantická analýza, znižovanie do IR, generovanie kódu, vykonávanie vo VM a natívna integrácia. Pre každý regresný prípad uchovajte najmenší reprodukovateľný vstup, očakávaný výsledok a skutočný výsledok. Zmeny API musia opísať spätnú kompatibilitu, kroky migrácie a vplyv na existujúce príklady.

### Lokalizácia, kódovanie a čitateľnosť

Technické identifikátory, príkazy, cesty, syntax a bloky kódu zachovávajte vo všetkých jazykových verziách. Prekladajte vysvetľujúci text, nie tokeny alebo názvy používané v zdrojovom kóde. Jazykové a regionálne označenie uvádzajte presne; slovenskú terminológiu nezamieňajte s českou.

Po každej aktualizácii skontrolujte odkazy Markdown, hierarchiu nadpisov, bloky kódu a príklady. Pri zmene kanonických anglických dokumentov porovnajte príslušné časti a aktualizujte celý preklad, nielen úvod. Obsah bez jazykovej alebo technickej kontroly musí byť označený.

### Medzinárodné vydania a slovenské jazykové formy

Toto slovenské vydanie je určené predovšetkým čitateľom slovenčiny. Netvrdí, že pokrýva všetky regionálne, menšinové alebo posunkové jazyky používané na Slovensku. Príspevky pre ďalšie jazykové formy a prístupné formáty sú vítané.

### Hlavné odkazy

- [Kanonický anglický README](README.md)
- [Špecifikácia syntaxe SLeeLa](SLEELA.syntax)
- [Register dokumentácie](docs/README.md)
- [Dokumentácia API](docs/api/README.md)
- [Podpora platforiem](docs/PLATFORM-SUPPORT.md)
- [Natívna implementácia](impl/README.md)
- [Inštalačná príručka](install/README.md)
- [Zásady testovania](markdown/TESTING.md)
- [Repozitár SLeeLa na GitHub](https://github.com/mearvk/SLeeLa)

**Stav prekladu:** Sekcie I–XIX sú preložené do slovenčiny. Technické identifikátory, príkazy, ukážky kódu, názvy súborov, odkazy a adresy sú zachované. Odporúča sa záverečná jazyková revízia rodenými hovorcami.
