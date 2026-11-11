# SLeeLa 3.0 — Magyarország (HU)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Natív rendszernyelv, fordítóprogram, futtatókörnyezet és virtuálisgép-ökoszisztéma.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Nemzetközi országkiadás.** Ez a kiadás magyar nyelven készült. A műszaki azonosítók, parancsok, kódpéldák, fájlnevek, elérési utak és hivatkozások változatlanok a kanonikus angol [README.md](README.md) fájlhoz képest.

**The Now. The Ingredient.** A nyelv több a szintaxisnál: a fordítóprogram, a futtatókörnyezet, a könyvtárak, az eszközök és a platformszerződések együtt teszik használhatóvá a szoftvert.

**Mivel foglalkozik a SLeeLa.** A SLeeLa a **Math**, az **Object**, a
**Science** és a **Reach** területével foglalkozik — munkájának négy arcával: a
kiszámíthatóval (Math), a megállapodott dologgal, amely a létezők katalógusában
áll (Object), az igaz fegyelmezett tanulmányozásával (Science), valamint egy
rendszer kezének más rendszerek és emberek felé való kinyújtásával (Reach). **A
nemzeti érdekek fennmaradnak** — a négy terület egyike sem helyezi őket
mellőzésre; ahol egy téma egy nemzet érdekét érinti, ez az álláspont megmarad
(hűen rögzítve, például az egyes osztályok Metadata `metaOrigin` /
`metaNationalSchedule` mezőiben), és soha nem oldódik fel.

## I. Első lépések
- [A natív implementáció fordítása](impl/README.md)
- [Virtuális gépek és Modium — gyakorlati felhasználói útmutató](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [CPU-verem SLeeLa-forráskódban](lib/cpu/CPU.md) — teljes gép a logikai kapuktól → ALU → magok → operációs rendszer → programok vonalon, 130+ CPU-profil jegyzéke, valamint állítható [feszültségek, változtatható órajelek és szakértői időzítések](lib/cpu/TIMING.md), amelyeket [specifikáció szerint futtathat, vagy túlhajthat](lib/cpu/components/README.md)
- [Dokumentációs index](docs/README.md)
- [CSPML-adatforrás és Salt & Salary kutatási hivatkozás](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — külső, amerikai helyekre, társadalmi-gazdasági helyzetre, munkaerőre, oktatásra és állami iskolákra vonatkozó adatok eredete és integrációs útmutatója.
- [Generált API-dokumentáció](docs/api/README.md) — aktuális HTML-referencia, verziózott API-kiadás és újragenerálási utasítások
- [API-katalógus](api/README.md)
- [Számítási összehasonlítási szabványok](comparison/README.md) — a SLeeLa VM, a Java/JVM és a natív C/C++ típus-, memória-, ABI- és megfelelőségi szabványai.
- [Recursion stanchion — C és C++ prototípusok](recursion/stanchion/README.md) — primitív típusokra specializált példák Boolean, Byte, Character, Integer, Long, Float és Double típusokhoz, általános, alkalmazott, tudományos és oktatási rekurziós mintákkal kiegészítve.
- [Platformtámogatási és érvényesítési mátrix](docs/PLATFORM-SUPPORT.md)
- [Arculati útmutató](docs/BRAND-GUIDE.md)
- [Merchandise-koncepciók és -anyagok](merchandise/README.md)
- [NitroWebExpress-logó](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Tesztelési szabályzat és kiadási kapuk](markdown/TESTING.md)

## II. Hírek

A nyelv, az illesztőprogramok és a szabványos könyvtár legutóbbi újdonságai:

- **Az `advantage` szabványkönyvtári csomag — egy osztalék „advantage”-elése
  matematikával, fizikával, integrációval, osztályozással és irányítható
  kimenettel.**
  A [`lib/advantage/`](lib/advantage/README.md) az *egyedi osztalék modellhez
  viszonyított advantage-elését* egy rétegzett **Sequitur-hagymaként** foglalja
  magába (osztalék → felülvizsgálat → aggályok → matematika → kiegészítések →
  kivonás → átcsoportosítás → iteráció), amelyben befelé/kifelé lehet
  navigálni, teljes dinamikus frissítéssel és restitúcióval; egy egyszerű
  **felhasználói modellen** és egy teljes kontrollt adó **fejlesztői modellen**
  keresztül érhető el. Öt témakört tartalmaz: a **`math/`** a kétoldalú
  kereskedelmi közgazdaságtant (közös profitmaximalizálás, Nash-alku, a
  kétoldalú newsvendor kritikus hányados), a **`physics/`** a „bizalmi mezők
  fizikájának” strukturális közgazdaságtanát (a bizalom mint metrikus tenzor,
  a tranzakciós geodetikus, egy bejelentési kontaktstruktúra, egy CIR+jump
  bizalomcsökkenési SDE — modellezési analógia, annak is jelölve), az
  **`integration/`** a kapitalizmusra/kiskereskedelemre vonatkozó, rekurzívan
  összeállítható vertikális integrációs modellt (rekeszek, mérnöki öblök,
  próbarekeszek, newton-medián tálcák és egy rendszerkifinomultsági pontszám),
  a **`grading/`** egy kifinomultsági áramköri osztályozót, amely minden modult
  és nyertes/vesztes/osztályozó/abszolút modulok bármely halmazát A–D skálán
  értékeli, az **`output/`** pedig egy irányítható kibocsátási csatlakozót,
  amely a modul kimenetét a terminálra, egy **CommonRails**-formátumú
  felületre vagy egy socketre/csatlakozóra küldi (a natív `slnet_write`
  számára keretezve). Minden osztály hordozza a Metadata-Class rekordot; öt
  futtatható demó validálja a témaköröket, és a kísérő
  [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md),
  [`MATH.md`](lib/advantage/MATH.md) és
  [`PHYSICS.md`](lib/advantage/PHYSICS.md) dokumentálja őket. Egy natív C/C++
  fogyasztó az [`advantage/`](advantage/README.md) alatt (a C mag
  `advantage_value`/`advantage_mean`, egy XML tudástár, valamint egy biztonságos
  HTTPS/SFTP/SSH-beszerzési + kulcstár + DH/ECDH szállítási könyvtár) a modellt
  kidolgozott
  [kereskedelmi példákhoz](advantage/examples/README.md) köti — feltételezett
  internetes forrásokból származó pozitív ügyletek, egy közönségrelevancia-sorozat
  (Traders, Consumers, Helpers, Healers), valamint nyolc A–D skálán rangsorolt
  modell (D = a legtöbb Cement, A = a legtöbb Brand).
- **Ciklusidőzítő annotációk — `@loopDelay` / `@loopBudget` (+ beépített
  `sleep(ms)`).** A SLeeLa első **utasításszintű, viselkedési** annotációi:
  egy `for` / `while` / `do` ciklus előtt, saját sorban írva valódi szünetet
  iktatnak a ciklus végrehajtásába. A `@loopDelay(ms = 250)` /
  `@loopDelay(seconds = 2)` iterációnként rögzített ideig alszik (a puszta
  `@loopDelay(250)` alapértelmezésben ezredmásodpercet jelent);
  a `@loopBudget(seconds = 10,
  iterations = 1000)` egy ismert iterációszámot egyenletesen elosztja egy teljes
  falióra-keretre. Mindkettő az új, csak hozzáfűzhető **`OP_SLEEP`** opkódra
  (ISA 157) fordul, amely egy általános, bárhol hívható **`sleep(<ms>)`**
  beépített függvényt is kiszolgál; a szünet hordozhatóan valósul meg
  (`nanosleep` / `Sleep`). Specifikáció: a
  [`SLEELA.syntax` §32](SLEELA.syntax); API-referencia:
  [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md);
  futtatható:
  [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela).
  A munkamenet-kiegészítések indexe (ebben ez, a natív `OP_BIGMUL`
  nagy egész szorzás, valamint az önálló **[local-generator](local-generator/README.md)**
  és **[pi-generator](pi-generator/README.md)** eszközök) itt található:
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md).
  A három generált API-felületet
  ([`sleeladoc-api/`](sleeladoc-api/README.md),
  [`docs/api/`](docs/api/README.md) és a megváltoztathatatlan
  [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html)
  pillanatkép) a `make api-docs` paranccsal újraépítettük, és a `vm` csomagoldalukon
  már szerepelnek az új `SLISASleep`
  / `SLISABigMul` utasításbejegyzések.
- **SleelaDoc parancssorból — `sleela docs`.** A SleelaDoc mostantól közvetlenül
  a terminálból elérhető. A `sleela docs` felsorolja az összes dokumentálható
  témát; a `sleela docs crypto` egy csomagot mutat; a `sleela docs stringbuilder`
  feloldja az osztályt (kis- és nagybetűtől függetlenül, valamint az `SL`
  szabványkönyvtári előtag alapján →
  `SLStringBuilder`), és gazdag oldalt nyomtat — **definíció, tervezési cél,
  kapcsolatok, szerkezet, felhasználói clay-k és ötfokozatú mesterségi
  létra** —, közel eltaláló keresésnél rangsorolt „úgy értette” találatokkal.
  A szerzők bármely oldalt egyszerűen gazdagíthatják SleelaDoc `@`-címkék
  hozzáadásával (`@intent`, `@thesis`, `@axiom`, `@param`,
  `@see`, `@mastery`). A `sleela docs --generate [dir]` egyszerű szöveges
  korpuszt ír (osztályonként egy oldal + egy `TOPICS.index`), amelyet a CLI
  ezután offline olvas; a **`make sleeladoc`** ezt a korpuszt **és a hozzá
  tartozó `.sleela` forrásokat** a `sleeladoc/build/sleeladoc.jar` fájlba
  csomagolja. Csak homlokzatú témáknál (pl.
  `psychiatry`, `politico`) a `sleela docs --facade <topic>` futásidőben
  átkeresi a fájlrendszert, és felsorolja a valódi **mappákat (majd a fájlokat,
  természetes mozgásként)**, felhasználó által felülírható korlátokkal:
  **6 MB / 6000 fájl / 6 perc**. A `lib/sleeladoc` egységeket a jelenlegi kiadáson,
  a **`#sleela 3.0`** szinten tartjuk karban. Lásd:
  [`man/man1/sleela.1`](man/man1/sleela.1) és
  [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md).
- **SleelaDoc — Javadoc-stílusú dokumentációs megoldás generált HTML-oldallal.**
  A **SleelaDoc** a SLeeLa forráson belüli dokumentációs megjegyzési
  konvenciója (`@doc` / `@page` jelölővel ellátott, hegyes zárójeles
  megjegyzések; lásd
  [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)), a
  [`lib/sleeladoc`](lib/sleeladoc/README.md) pedig a **kivonatoló**, amely egy
  `.sleela` egységet Markdown vagy HTML API-oldalként jelenít meg — úgy, ahogy a
  `javadoc` a Javát, a *szándékot* dokumentálva. A teljes `/lib` könyvtárhoz
  tartozó, böngészhető **HTML-dokumentációs oldal** a
  [`/sleeladoc-api`](sleeladoc-api/README.md) könyvtárba generálódik a
  `tools/generate-sleeladoc-api.py` segítségével (nyissa meg a
  `sleeladoc-api/index.html` fájlt; letisztult kék / világoskék / világosszürke /
  fehér stílus, csomag → osztály → tag hierarchiával és a *Student 2* olvasó
  számára írt szószedettel). A fordító a fordítás melléktermékeként a
  szerző által jelölt dokumentációs kimeneteket is előállítja (`--wiki`
  MediaWiki-oldal + egy származtatott forrásikertárs), amelyeket a
  [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md) dokumentál.
  Futtatható:
  [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela).
- **XML dokumentumobjektum-modell (`lib/xml`).** SLeeLa-oldali XML DOM — egy
  dokumentum, amely elem-/szöveg-/megjegyzés-/CDATA-/PI-csomópontok fáját birtokolja
  rendezett gyermekekkel és elnevezett attribútumokkal, egy szerializáló
  (kompakt/szépített, XML 1.0 §2.4 szerinti escape-elés), egy karaktercellás
  értelmező és az `SLXmlAspect` minőségi regiszter. A csomópontok egyenlősége
  háromfokú bizalmi lépcső (azonosság → azonos tárgy → szerkezet), reflexív,
  szimmetrikus és determinisztikus. Lásd: [`lib/xml/README.md`](lib/xml/README.md)
  és a futtatható [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela).
- **Széles előjeles egész primitívek (`long`, `int64`, `int128`).** Az `int`,
  a `long` és az `int64` mind 64 bites előjeles egész, amelyek azonosan
  típusellenőrizhetők és futtathatók; az `int128` egész típusként elfogadott
  (egyelőre 64 bittel megvalósítva, a valódi 128 bit dokumentált folytatás). A
  2-es alapú előjeles `int32`..`int16777216` sorozat (2.7-es szintaxis) és a
  megfelelő előjel nélküli `uint32`..`uint16777216` sorozat (2.8-as szintaxis)
  ezt bővíti. Lásd: [`SLEELA.syntax`](SLEELA.syntax) §21, §24, §25.
- **`@mix` következményes mezők (2.9-es szintaxis).** Egy osztály több,
  **azonos nevű** példánymezőt is deklarálhat, ha minden ismétlés `@mix`-et
  hordoz; a nyelv a csoportot következményesnek (index szerinti tömbnek / listának)
  értelmezi: alapértelmezésben fentről lefelé `1,2,3,…`, vagy kézzel a
  `@mix(1,2,3,...)` formával, amelynek indexszámának egyeznie kell a mezők
  számával. Lásd: [`SLEELA.syntax`](SLEELA.syntax) §28.
- **Dokumentumjelölők — a Természetes Folyamat, a Preonemek néven
  (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).**
  Ezeket a puszta dokumentumjelölő szavakat együttesen **Preonemeknek**
  nevezzük (egy jelölő egy *Preonem*; a halmaz a *Preonemek* — lásd:
  [`SLEELA.syntax`](SLEELA.syntax) §26.1). Elsőrendű, kizárólag deklaratív
  dokumentumjelölők, amelyek azt nevezik meg, hogy egy dokumentum *micsoda* az
  alkotás rendjében:
  `author`/`authorial` (2.4-es szintaxis; szakmai tanács kezeli — az egyetlen
  tag, amelynek viselkedési hatása van), `material;` (megállapodott Forrás) és
  `memory;` (2.5-ös szintaxis), `determine;` (2.6-os szintaxis; olyan dokumentum,
  amelyet orvos, doktor vagy kísértet tovább feldolgozhat a törvényes kéz általi
  befejezés felé), valamint a két legújabb, az **`object;`** (a létezők
  katalógusában álló, megállapodott dolog — **reifikálva, konkrét állást kapva a
  2.8-as szintaxisnál**) és a **`field;`** (a viszony alapja, amelyben a tárgyak
  állnak és mozognak) — mindkettő a 2.6-os szintaxistól kapuzott. Mindegyik puszta,
  szövegkörnyezetfüggő, és a frontend verzió szerint kapuzza, az elemzésre, a
  típusellenőrzésre és a kódgenerálásra nincs hatása; a jelölő helyzetén kívül
  mindegyik közönséges azonosító marad (így a `field` soha nem zavar egy
  osztálytagot vagy egy `.field` hozzáférést). Lásd: [`SLEELA.syntax`](SLEELA.syntax)
  §18–§20, §23 és §26.
- **A Nordshrift a 3.0-s szintaxison, teljes könyvtáreléréssel.** A Nordshrift
  ugyanazon a rekurzív `/lib` indexen keresztül old fel, mint a fordító, és
  minden csomagot és `.sleela` egységet elér; a `nordshrift check` a teljes fa
  darabszámait jelenti.
- **Recursion stanchion.** A [`recursion/stanchion/`](recursion/stanchion/README.md) könyvtár most már külön C és C++ referenciaprototípusokat tartalmaz a hét dokumentált primitív szerződéshez, valamint általános, alkalmazott, tudományos és oktatási példákat. Ezek referenciaforrások; az ellenőrzöttként való kezelésük előtt fordítsa le és tesztelje őket.

- **Hegyes zárójeles megjegyzések (`<` / `<<`).** Új, előremutató megjegyzésstílus —
  `< sor >` és `<< blokk >>` — amelyet a SLeeLa frontend lexikálisan elemez, és a
  [`SLEELA.syntax`](SLEELA.syntax) §1.1 dokumentál.
- **Radix egész literálok.** Az `int` literálok mostantól elfogadják a
  hexadecimális (`0x`), bináris (`0b`) és oktális (`0o`) alakot `_`
  számjegyelválasztókkal, mind a SLeeLa forrásnyelvben ([`SLEELA.syntax`](SLEELA.syntax) §1.4),
  mind a [`.xlt` CPU-modellformátumban](lib/xlt/README.md).
- **A Metadata Class szabvány az egész könyvtárban.** A leíró metaadat-
  metaminta (eredet, jogszerűség, joghatósági hatály) mostantól minden, a
  `/lib` alatti osztályt tartalmazó `.sleela` fájlban megtalálható; lásd:
  [`SLEELA.syntax`](SLEELA.syntax) §15 és
  [az oktatóanyag-lecke](tutorial/13-metadata-class.md).
- **Gazdagabb szöveg- / String-család.** A `lib/text` valódi nyelven belüli
  segédfüggvényekkel, valódi `charAt`/`substring`/`indexOf`/`reverse`/
  oszlopigazítást adó karaktercellás nézettel és valódi pozicionális
  sablonkezelővel bővül — lásd a
  [szöveg- és String-oktatóanyagot](tutorial/14-text-and-strings.md) és a futtatható
  [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela) fájlt.
- **Elsőrendű `String` a Nordshrift lapokon.** Egy `.sst` lap deklarálhat egy
  típusos `strings:` blokkot, amely hozzájárul a build komponensjegyzékéhez;
  lásd a [Nordshrift Sheet Reference](wiki/Nordshrift-Sheet-Reference.md) dokumentumot.
- **A CPU-modellek opkódokkal és függvényhívás-ekvivalenciával is futnak.**
  Minden `SL<ARCH>CPU` örökli azt az alapot, hogy egy natív utasításfolyamot a
  fetch-decode-execute ciklusán keresztül futtat, *és* egy feltételezett másik
  folyamat kódját egyenértékű hívásokkal futtatja a közös VM-alapzaton — lásd:
  [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5.

## III. Fordítás és tesztelés

**Előfeltételek:** C/C++ eszközlánc (GCC vagy Clang) és GNU Make; Python 3 a
jegyzék-/folyamateszközökhöz; Linuxon az X11/Xrandr fejlesztőcsomagok a
megjelenítési termékekhez (egy modul `make deps` parancsa jelzi/telepíti, amire
szüksége van). Az `impl/` build megbízható SHA-256 jegyzékre épül és hiba esetén
lezár (fail-closed); a gyökér `Makefile` ezt automatikusan beállítja.

Tiszta checkoutból a leggyorsabb indulás:

```sh
make                                  # build everything from the repo root
```


Vagy csak a natív magot fordítsa és tesztelje:

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

A gyökérszintű `make` minden terméket lefordít (mag, VM, kodekek, churn,
antivírus, …), és a **registry** felépítésével, valamint az alaptáblázat
előállításával fejeződik be — így a lefordított fa kapásból egy
átvizsgálható registryt szállít. A legfontosabb célok:

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

További hasznos célok:

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

A `registry` cél a [`build-registry.sh`](build-registry.sh) szkriptet futtatja:
felépíti a registry magját, az alaptáblázatot betölti a hordozható szöveges
tárba (`registry/build/sleela.sleelareg`), és exportálja a MySQL-táblaszkriptet
(`registry/build/registry.sql`) a `sleela registry export` parancson keresztül.
Átvizsgálható a `sleela registry` vagy a `registry/build/sleela-registry review`
paranccsal. Lásd a [registry-útmutatót](registry/README.md) és a
[Registry 3.0 architektúrát](registry/REGISTRY-3.0-ARCHITECTURE.md).

Az előfeltételekért és a platformspecifikus lépésekért olvassa el a build- és tesztelési útmutatókat. Egyetlen komponens sikeres fordítása nem bizonyítja, hogy minden könyvtár vagy platform kiadásra kész.

## IV. Telepítés

A natív termékek lefordítása után telepítse a parancsokat és a futtatókörnyezet támogatófájljait:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

A hagyományos telepítő alapértelmezés szerint a `/usr/local/bin`,
a `/usr/local/lib/sleela` és a `/usr/local/share/sleela` könyvtárat használja.
Megőrzi a meglévő indítási konfigurációt, és megtagadja a nem követett, már
létező parancsok felülírását. Az eltávolítás kizárólag a telepítő által
rögzített fájlokat törli, a konfigurációt és a nem követett futásidejű adatokat
megőrzi.

A kizárólag a fő csomagra — a `sleela`, `sleelvac` és `nordshrift`
binárisokra, a szabványos könyvtárra, a protokollokra és a támogató modulokra —
összpontosító, vezetett telepítéshez használja a tiszta telepítőt:

```sh
make clean-install       # guided install of the main package only
```

Egyetlen elsődleges döntést kér (a fő csomag telepítése), majd hatókört kell
választani, sikeres telepítés után pedig opcionálisan kiválasztható néhány
gondosan válogatott kiegészítő modul. Az interaktív telepítési munkafolyamatról és
az összes telepítési lehetőségről lásd:
[Gyors és biztonságos telepítés](install/README.md).

## V. Nyelv és futtatókörnyezet
A mérvadó nyelvtan a [`SLEELA.syntax`](SLEELA.syntax). A repository jelenleg a **3.0-s** szintaxist dokumentálja; a fordító/eszköz verziója és a forrásszintaxis verziója két különböző fogalom. A 3.0-s felület hozzáadja a **`pure` és `gasoline` dokumentumjelölőket** (§30) — a Preonemek egy puszta, szövegkörnyezetfüggő, kizárólag deklaratív párját, amelyek majdnem ugyanazt jelentik, és mindegyik a saját őszintesége szerint jelöli a dokumentum állását. A 3.0-s szintaxis az 1.0–2.9-es nyelvi felületet egyetlen, jelenlegi normatív kiadássá vonja össze, megőrizve az összes funkcióspecifikus minimális kaput. Az előző, 2.9-es felület hozzáadja a **`@mix` következményes mezőket** (§28) — egy osztály több azonos nevű példánymezőt deklarálhat, ha mindegyik `@mix`-et hordoz, egyetlen indexelt csoportként értelmezve (fentről lefelé `1,2,3,…` vagy kézzel `@mix(1,2,3,...)`, amelynek indexszámának egyeznie kell a csoporttal). Továbbviszi a 2-es alapú előjeles/előjel nélküli széles egész családokat: `int32`..`int16777216` (§24) és `uint32`..`uint16777216` (§25) a korábbi `long`/`int64`/`int128` (§21) felett; a **kulcsszótámogató dokumentumokat** (§22); valamint a **Preonemeket** — a kizárólag deklaratív dokumentumjelölőket: `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) és `artifact` (§35, időre és térre fokozott törvényhozási szintű állás), amelyeket együttesen Preonemeknek nevezünk (§26.1). A konstruktor-túlterhelés feloldása és a `this(...)`/alaskonstruktor-delegálás kifejezetten az ellenőrzött 1.8-as szintaxisszerződésen kívül esik. Egy funkció használata előtt olvassa el a nyelvtant és a megvalósítási jegyzeteket. A SleelaDoc kilenc forrásegysége mostantól a `#sleela 3.0` verziót deklarálja; a megosztott fordítói kapu az 1.0–3.0 forrásszintaxist fogadja el. Az új API-build a `make api-docs` paranccsal készül.

A repository natív C/C++ implementációt, SLVM-generálási referenciákat, szabványkönyvtári modulokat, teszteket, példákat és platformintegrációkat tartalmaz. A megvalósítás és az érvényesítés állapota modulonként eltérő; ne feltételezzen egységes teljességet, hanem olvassa el az adott modul specifikációját és az aktuális CI-eredményeket.

## VI. Mérnöki alapelvek
- **Bizonyíték az állítások helyett:** különböztesse meg a megvalósított, tesztelt, kísérleti és tervezett viselkedést.
- **Egy szerződés határonként:** dokumentálja a szintaxist, az API-kat, a fájlformátumokat, a platformfeltevéseket és a hibaviselkedést.
- **Tervezetten hordozható:** különítse el a platformspecifikus háttérrendszereket, és natív futtatókon tesztelje őket.
- **Biztonságos hibakezelés:** tesztelje a hibás bemenetet, az erőforráskorlátokat, a folyamathibákat és a takarítási útvonalakat.
- **Reprodukálható kiadások:** építse újra tisztán, és tegye közzé az artefaktumok hash-értékeit és a kiadási megjegyzéseket.

## VII. Antivírus-build és integritás

A Sleelavirin antivírus-csomag natív SHA-256 szkennert, opcionális ClamAV-
adaptert, forrás-/indexintegritási jegyzékeket és platformcsomag-ellenőrzőösszegeket
tartalmaz. A csomagot a repository gyökeréből lehet lefordítani és tesztelni:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Az integritási jegyzék az `antivirus/build/` alatt jön létre; lefedi a követett
forrás- és dokumentációs bemeneteket, valamint a védett aláírásindex-fájlokat,
a generált kimenetet és a módosítható ClamAV-gyorsítótárat viszont kihagyja. A ClamAV
adatbázis-pillanatképeknek saját jegyzékük van. A kiadási ZIP-ek tartalmazzák az
`INTEGRITY-MANIFEST.json` és a `SHA256SUMS` fájlt, valamint egy `.sha256` kísérőfájlt.
A letöltött archívumok hash-értékét egy függetlenül beszerzett, megbízható
értékkel vesse össze: az archívum mellett tárolt ellenőrzőösszeg nem a kiadó
aláírása. Lásd a [Sleelavirin-útmutatót](antivirus/README.md), a
[biztonsági ellenőrzőlistát](antivirus/SECURITY.md) és az
[antivírus build-munkafolyamatot](.github/workflows/antivirus-binaries.yml).

Egy kiadás addig nem tekinthető ellenőrzöttnek, amíg a vonatkozó CI-feladatok
nem sikeresek pontosan a kiadási commiton, és az eredményül kapott artefaktumokat
nem vizsgálták át.



## VIII. A SLeeLa szerkesztőtémái — Vim és Emacs

A SLeeLa összehangolt, professzionális sötét színsémákat tartalmaz a **Vimhez** és az **Emacshez**, valamint egy natív C/C++ palettaeszközt. Az eredeti **American Professional** séma marad az alapértelmezett. Tíz kísérő paletta áll rendelkezésre különböző kulturális és intézményi tervezési ihletforrásokhoz; ezek értelmező tervezési paletták, nem pedig egy nemzet, kultúra, bíróság vagy intézmény hivatalos megjelenítései.

### A tizenegy séma és színeik

| Séma | Háttér | Előtér | Kulcsszavak | Típusok | Karakterláncok | Megjegyzések |
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

#### Nemzeti zászlószínek (forrás paletta)

A görög és a török séma a két ország **hivatalos nemzeti zászlószíneiből** származik, itt a pontos, közzétett hexadecimális értékekkel. A fenti sémák hátterei/kiemelései e forrásszínek sötétített, szerkesztőbarát értelmezései — az alábbi táblázat a „nemzetileg megjelenített színek” referenciája.

| Nemzet | Szín | Hex | Alap és jelentés |
|---|---|---|---|
| **Görögország** | Kék | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | Nemzeti zászlókék (851/1978. sz. törvény). Az ég és a tengerek; heraldikai nyelven *azúr*. |
| **Görögország** | Fehér | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Nemzeti zászlófehér. A függetlenségi harc tisztasága; a kereszt a keleti ortodox kereszténységet jelöli. |
| **Türkiye** | Vörös (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | Nemzeti zászlóvörös (török zászlótörvény). A mező; hagyomány szerint a vértanúk vére és az oszmán szuverenitás. |
| **Türkiye** | Fehér | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Nemzeti zászlófehér. A félhold és a csillag — béke, tisztaság, valamint a türk/iszlám örökség szimbóluma. |

A sémák az egyes szerkesztők előterét a nemzeti **fehérre**, kulcsszavait, kurzorát, kijelölését és állapotsorát pedig a pontos nemzeti **kékre** (`#0D5EAF`) vagy **vörösre** (`#E30A17`) horgonyozzák, így a lényeges színek szó szerint a nemzeti színek. Ezek értelmező tervezési paletták; a zászlószín-hivatkozás az eredet igazolására szolgál, és egyik állam hivatalos megjelenítése sem. A színek jelentése a Helléni Köztársaság külügyminisztériumának állami szimbólumokról szóló leírását és a török zászlótörvény hagyományát követi.


### Színminta-táblázat

Az alábbi palettacellák **SVG-mintákat** használnak, így a GitHub a tényleges színeket jeleníti meg az üres HTML színcellák helyett. Minden csík a hat színt ebben a sorrendben mutatja: háttér, előtér, kulcsszavak, típusok, karakterláncok és megjegyzések. A pontos hexadecimális kódok az egyes minták mellett állnak, ugyanabban a sémasorrendben, mint a fenti referenciatáblázatban.

| Séma | Megjelenített palettaszínek | Hex kódok (háttér · előtér · kulcsszavak · típusok · karakterláncok · megjegyzések) |
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

További kiemelőszíneket használnak a függvényekhez, konstansokhoz, kijelölésekhez, diagnosztikához és előfeldolgozói direktívákhoz. Az orosz és szláv paletta szándékosan közepes intenzitású palát-, kék-, zsálya-, rubin- és antik aranyszínt használ a nyugodtabb, professzionálisabb megjelenésért.

### A Vim-sémák telepítése

A repository gyökeréből:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Adja hozzá a következőt a `~/.vimrc` fájlhoz a SLeeLa szintaxiskiemelés engedélyezéséhez és egy séma kiválasztásához:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Pontosan az alábbi colorscheme-nevek egyikét válassza:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Például a `colorscheme sleela-russian-slavic` a közepes orosz és szláv palettát, a `colorscheme sleela-greek` / `colorscheme sleela-turkish` pedig a helléni és az Al Bayrak palettát választja. Indítsa újra a Vimet, vagy töltse be újra a konfigurációt a `:source $MYVIMRC` paranccsal.

### Az Emacs-sémák telepítése

Hozzon létre egy helyi témakönyvtárat, és másolja át a mód- és témafájlokat a repository gyökeréből:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Adja hozzá ezt az `~/.emacs` vagy az `~/.emacs.d/init.el` fájlhoz:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Másik séma kiválasztásához cserélje le a `sleela-american` nevet a következők valamelyikére: `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` vagy `sleela-sleela`. Ha az Emacs megkérdezi, hogy megbízik-e a témában, elfogadás előtt vizsgálja meg a helyi témafájlt.

### A natív C/C++ paletták előnézete

A kis natív paletta-segédprogram képes listázni a sémákat, kiírni azok terminálszíneit, és lekérdezni egy-egy színt:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

A visszamenőleges kompatibilitás érdekében a `--theme` nélküli `--palette` és `--hex COLOR` az American Professional sémát választja. A build- és tesztredményeket a célrendszeren és a CI-ben kell ellenőrizni; egy parancs dokumentálása önmagában nem bizonyíték arra, hogy sikeres volt.

A teljes palettaleírásokat és a segédprogram részleteit lásd a [szerkesztő-integrációs útmutatóban](linux-editors/README.md).

## IX. Közreműködés
1. Olvassa el a dokumentációs indexet és a tesztelési szabályzatot.
2. Végezzen célzott módosításokat pozitív és negatív tesztekkel.
3. Futtasson célzott teszteket, majd a teljes csomagot.
4. Adja meg az operációs rendszer/eszközlánc adatait; a kihagyott ellenőrzéseket ne írja le sikeresként.

A SLeeLa fejlesztője a **MEARVK LLC**. A korábbi nyitóoldali anyagot a [`docs/README-LEGACY.md`](docs/README-LEGACY.md) őrzi.

## X. AE6E66 — a Round Public e-mailekre vonatkozó illetékelvárás

Az AE6E66 kezdeményezés esetében a projekt kinyilvánított elvárása **$300,000,000.00 USD minden egyes Round Public e-mailre**, javasolt adóilletékként, minden egyes jogosult Round Public e-mailhez társítva.

Ez az állítás kizárólag a projekt elvárását rögzíti. **Nem** jogi adókivetés, kormányzati rendelet, bírósági ítélet vagy önállóan megállapított adókötelezettség. Bármely tényleges illeték az alkalmazandó jogi felhatalmazást, joghatóságot, fogalommeghatározásokat, eljárást és az illetékes kormányzati megállapítást igényelné.


## XI. Ábécé- és karakterkészlet-futtatókörnyezet

A SLeeLa ábécé-dokumentációját és natív karakter-futtatókörnyezetét az alphabet/sleela/ és a lib/alphabet/native/ tartja karban. A natív C++17 tesztek a szigorú UTF-8 skalárdekódolást/-kódolást, a C<n> kapacitásaláírásokat és a korlátozott karakterkészlet-tagságot fedik le:

    make -C lib/alphabet/native test

A kísérleti UTF-4088 katalógust és annak SLeeLa-keresőadapterét a utf-4088/SLeeLa-ALPHABET-INTEGRATION.md dokumentálja. Az UTF-4088 azonosítók külön névteret alkotnak, nem Unicode skalárértékek. A fordítói és SLVM-kötési munkát ezektől a natív alapoktól kifejezetten külön követjük.

## XII. AutoCAD Creator™

Az [AutoCAD Creator™](autocad/creator/README.md) egy teljes egészében SLeeLa nyelven
írt, interaktív 2D CAD-alkalmazás. Míg a testvér AutoCAD Renderer egy tervet egy
lépésben rajzzá alakít, a Creator lehetővé teszi egy 2D AutoCAD-dokumentum élő
**létrehozását és módosítását** — falak, helyiségek, méretezések és jegyzetek
hozzáadását, majd az elhelyezettek mozgatását, átcímkézését, rétegváltását,
törlését és visszavonását — és az eredmény **mentését** valódi AutoCAD DXF
rajzként (R12 szöveg, megnyílik AutoCAD-ben / LibreCAD-ben).

Grafikus felülete a **SleelaUI™**-ra épül, a SLeeLa saját, többplatformos
UI-eszköztárára (a [`lib/user-interface`](lib/user-interface) alatt) — nem JavaFX-re —,
amely valódi natív ablakot nyit X11 / Cocoa / Win32 alatt. A tervezés egyetlen
szabályt követ: **„a SleelaUI megjelenít; a SLeeLa dönt”**: a GUI soha nem számol
geometriát és nem ír DXF-et. Minden eszköz létrehozási / módosítási / törlési /
visszavonási / mentési szándékot küld a mérvadó
[`SLCadCreator`](lib/autocad/SLCadCreator.sleela) felé, amely a szerkeszthető
dokumentumot és a visszafordítható parancselőzményt birtokolja, miközben a vászon
csak a Creator által tartott geometriát jeleníti meg előnézetként.

Az újrahasznosítható szerkesztőmotor a [`lib/autocad`](lib/autocad) alatt található —
[`SLCadEntity`](lib/autocad/SLCadEntity.sleela),
[`SLCadDocument`](lib/autocad/SLCadDocument.sleela),
[`SLCadCommand`](lib/autocad/SLCadCommand.sleela) és
[`SLCadCreator`](lib/autocad/SLCadCreator.sleela) — és teljesen fej nélkül
(headless) is végigfut. A GUI-héj és a demó az [`autocad/creator`](autocad/creator) alatt található:

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

Kijelzővel ez megnyitja az élő szerkesztőablakot; fej nélküli gazdagépen a rajzot
a memóriában építi fel, kiírja a jelentést és a DXF-et, majd tisztán kilép.


## XIII. UTF-4088 bemeneti térkép sorrendje

A specializált UTF-4088 katalógus a térbeli bemeneti térkép bejárását és az elvont nyomás-iterációkat az [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md) fájlban határozza meg. A National Lien tárgymutató-szabályzat az [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md) fájlban található. A térképkoordináták, a nyomásszintek (1–50), az állandó katalógusazonosítók és az ábécé szerinti rangok egymástól független mezők; egyiket sem szabad csendben a másik helyére tenni. A nyomás algoritmikus iterációs szint, nem fizikai mennyiség.



## XIV. Újabb és bővített csomagindex

Ez a hozzáfűzött index az újabban bővített csomagokat, doménkönyvtárakat és
forrásgyűjteményeket emeli ki. A bejegyzések csomagnév vagy téma szerint
ábécésorrendben csoportosítva szerepelnek. A leírások a repositorybeli helyet
azonosítják; a megvalósítás és az érvényesítés állapotát az egyes csomagok saját
dokumentációjában és tesztjeiben kell ellenőrizni.

### A–C

- **Advantage** — `lib/advantage/` és `advantage/`: egy egyedi osztalék
  modellhez viszonyított „advantage”-elése rétegzett Sequitur-hagymaként, öt
  tárgyi almappával — `math/` (közös profitmaximalizálás, Nash-alku,
  kétoldalú newsvendor), `physics/` (bizalmi mező metrika/geodetikus/kontaktus/SDE,
  jelölten modellezési analógia), `integration/` (vertikális integrációs
  rekeszek és kifinomultsági pontszám), `grading/` (A–D kifinomultsági
  áramköri osztályozó modulokhoz és halmazokhoz) és `output/` (terminál /
  CommonRails / socket kibocsátás). Egy natív C/C++ fogyasztó (`advantage/`)
  a modellt kidolgozott kereskedelmi példákhoz köti. Lásd: [`lib/advantage/README.md`](lib/advantage/README.md)
  és [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alphabet és UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` és
  `utf-4088/`: ábécékatalógusok, natív karakterkészlet-támogatás, bemeneti
  térkép sorrendje és UTF-4088 integrációs dokumentáció.
- **AutoCAD Creator** — `autocad/creator/` és `lib/autocad/`: interaktív
  2D rajzolási munkafolyamatok és újrahasznosítható CAD dokumentum-/szerkesztőkomponensek.
- **Ballum** — pixelszintű kijelzővezérlés: pixelenkénti set/get, régió-blit,
  az egész kijelző fényerejének/melegségének/gammájának állítása, mutató-/billentyűzetbemenet,
  valamint **teljes képernyős framebuffer-felület** a teljes, saját tulajdonú,
  gyors pixelenkénti vezérléshez (override-redirect ablak + egyetlen
  `XPutImage`/MIT-SHM megjelenítés Linux/X11 alatt).
  Natív mag a `ballum/` alatt, SLeeLa-homlokzat a `lib/ballum/` könyvtárban és VM-híd
  az `impl/` alatt; lásd: [`ballum/README.md`](ballum/README.md) és a
  futtatható [`ballum/example/`](ballum/example/).
- **CDMA** — `cdma/` és `lib/cdma/`: CDMA-hoz kapcsolódó műszaki anyag és
  SLeeLa forráskönyvtári munka.
- **Character** — `lib/character/`: karakterprofilok és újrahasznosítható
  karakterstatisztikák, életrajzi profilpéldákkal együtt.
- **Coorenagraph** — `lib/coorenagraph/`: etikai gondolkodás, erkölcsi szókincs,
  mérlegelés és kapcsolódó fogalmak; lásd a csomag README-jét és forrásfájljait.

### D–G

- **Kormányzati forrásmodellek** — a [lib/governance](lib/governance/README.md) deklaratív SLeeLa-forrásalakokat határoz meg javaslatokhoz, névtelen kézi szavazatokhoz és összesített eredményekhez. A javasolt @vote annotációt és a kézi folyamatot a [registry/DEMOCRATIC-VOTING-SYNTAX.md](registry/DEMOCRATIC-VOTING-SYNTAX.md) dokumentálja; a jelenlegi fordítók inert megjegyzés-metaadatként kezelik.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/`-tól a rendelkezésre álló verziózott
  HTTP-csomagkönyvtárakig: verzióspecifikus protokollspecifikációk és
  megvalósítások. A pontos hatókörért és az ellenőrzési állapotért olvassa el az
  egyes verziók dokumentációját.
- **Nemzetközi Büntetőbíróság** — `lib/international-criminal-court/` és
  `INTERNATIONAL.CRIMINAL.COURTS.md`: kapcsolódó, doménspecifikus forrás és
  dokumentáció.

### O–S

- **OFDMA** — `ofdma/`: ortogonális frekvenciaosztásos többszörös hozzáférésű
  műszaki anyag és C/C++ fejlesztési erőforrások.
- **Philosophy és a Conversation Mask modul** — a [`philosophy/`](philosophy/README.md)
  a natív C/C++ alapokat tartalmazza, a [`philosophy/module/`](philosophy/module/README.md)
  pedig a következő modulréteget három explicit területre szervezi:
  [`data/`](philosophy/module/data/README.md) a típusos, dátumozott megfigyelésekhez,
  eseményvégződésekhez, felezési idő-beállításokhoz, eredethez és hozzájáruláshoz; [`helper/`](philosophy/module/helper/README.md)
  a determinisztikus validáláshoz, relevanciacsökkenéshez, kompatibilis összehasonlításokhoz és
  hozzájárulással szűrt megjelenítéshez; valamint [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md)
  egy támogató, nem klinikai beszélgetési felülethez. A SLeeLa felé néző
  kezelők és az összeállított Tree a [`lib/philosophy/`](lib/philosophy/README.md) alatt találhatók,
  a tágabb philosophy-csomag mellett. A modul explicit bemenetre és
  a hozzájárulásra épít: nem figyel meg passzívan embereket vagy médiát,
  nem diagnosztizál, nem következtet magánjellegű szándékra, nem pontozza az emberi értéket,
  és nem javasol gyógyszerváltoztatást. Eseményhivatkozásai az eredetet és a
  navigációt őrzik, nem független megerősítést.
- **Synchro** — natív futtatókörnyezeti/fordítói integráció, valamint
  csomagkiosztással és méréssel kapcsolatos funkciók; lásd a vonatkozó `impl/`
  forrást és a fordító megvalósítását.
- **SleelaUI / terminál-UI** — `lib/user-interface/` és a
  `sleela-terminal` projekt: SLeeLa UI-integráció és terminálközpontú
  interakciós komponensek.

### U–W

- **Egyesült Nemzetek** — `united-nations/`: doménspecifikus repository-anyagok.
- **Wi-Fi 4, Wi-Fi 5 és Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`,
  `wi-fi-6/` és a kapcsolódó `lib/wifi/` forráskönyvtárak: generációspecifikus
  vezeték nélküli hálózati anyagok és konfigurációs erőforrások.

### A csomagindex karbantartása

Amikor egy csomagot hozzáadnak vagy érdemben bővítenek, vegye fel a bejegyzését
ide a megfelelő ábécé szerinti csoportba, hivatkozzon a kanonikus könyvtárára és
README-jére, és csak a becheckolt források által támogatott képességeket írja le.
Tartsa ezt az indexet additívnak: a csomagindex frissítésének részeként ne
távolítsa el és ne írja át a meglévő README-szakaszokat.


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

US Democratic and US Policy.

---
## XVI. Nemzeti tudományos arculati képanyag

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>

## XVII. UTF-4088 egymillió bejegyzéses kísérleti katalógus

A [UTF-4088 katalógusgenerátor](utf-4088/tools/generate_catalogue.py) determinisztikusan épít fel egy 1 048 576 bejegyzéses kísérleti szimbólumkatalógust. A dokumentált tömör formátumot az [utf-4088/FORMAT.md](utf-4088/FORMAT.md) írja le; a generálási és validálási szerződés az [utf-4088/GENERATION-RESULTS.md](utf-4088/GENERATION-RESULTS.md) fájlban található.

Futtassa a generátort Python 3-mal:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

A generált bináris 16 777 244 bájt, és úgy tervezték, hogy 20 000 000 bájt alatt maradjon. Az egységtesztek és egy GitHub Actions munkafolyamat ellenőrzik a bejegyzések számát, a glifák egyediségét, a méretet és az SHA-256 értéket; a sikeres futások a binárist és az összegzést a `utf4088-million-catalogue` munkafolyamat-artefaktumként teszik közzé. A generált bináris szándékosan kimarad a Git-előzményekből. Az UTF-4088 kísérleti marad, és nem Unicode-kompatibilis.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Legutóbbi kiegészítések — Pi-állandó, Modium/Registry indítás, sleelactl Math és `@native`

Ez a szakasz a 2026-11-es munka során hozzáadott funkciókat rögzíti, a legújabb csoportokkal kezdve. A hivatkozott forrásfájlok és azok érvényesítési állapota mérvadó; az áttekintés alatt álló módosításokat nem írjuk le egyesítettként, amíg el nem érik a `master` ágat.

### A 100 000 000 számjegyű Pi katalógusállandó

A repository egy ellenőrzött, **száz-millió-számjegyű** π-értéket szállít a [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) fájlban (jól formált tizedes tört, `3.14159…`), egy 57 344 számjegyű érték, a [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits) mellett. Mindkettő egymástól függetlenül ellenőrzött, valódi π — lásd a [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md) fájlt, amely dokumentálja az ellenőrzéseket (tankönyvi előtag, a Feynman-pont a 762. törtjegy-pozíciónál, a híres egymilliomodik számjegy, egyezés a `lib/math/SLPiReference.sleela` fájllal és egy teljes fájlra kiterjedő számjegygyakoriság-elemzés), valamint egy egylépéses reprodukáló szkriptet.

### A Modium™ / Registry™ az alrendszer indításakor betölti a Pi-állandót

A dedikált alrendszer-belépési pontokon — `sleela registry` és `sleela modium` (és a telepítő első futtatásakor), **nem** minden `sleela run` esetén — a Modium és a Registry alrendszer betölti a nagy π katalógusállandót. Interaktív terminálon átmeneti, piros `Loading Modium™` / `Loading Registry™` sort írnak ki, amelyet a normál kimenet megkezdése előtt a helyén törölnek; a hatás szigorúan TTY-hez kötött, így a csővezetékbe irányított és a tesztkimenet bájtról bájtra változatlan marad. A lelőhelyet (nem a bájtokat) a registry `Math` kulcs alapértékeiként, `piReferenceLargePath` és `piReferenceLargeDigits` néven töltik be, mind a SLeeLa-modellben ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)), mind a natív betöltőben ([`registry/sleela_registry.c`](registry/sleela_registry.c)); a `lib/math/SLPiReference.sleela` a `largePath()` / `largeDigits()` függvényeket teszi elérhetővé. A π-állandót az operációs rendszer SLeeLa-helyére (`<data>/pi/`) a szabványos `make` telepítő **és** a vezetett, „beautiful” telepítő is telepíti. Részletek: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — a „beautiful” telepítő és egy natív hátterű `math` tárgy

- A vezetett („beautiful”) telepítő mostantól telepíti a `sleelactl` eszközt (a SLeeLa vezérlősíkját) és modul-registryjét, a `make` telepítővel egyezően. Lásd: [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- A `sleelactl` új **`subject`** természetet kapott a futtatókörnyezetbe fordított natív C/C++ tárgykönyvtárakhoz, és regisztrálja az elsőt: a **`math`** tárgyat. A `sleelactl run math` a natív `__NativeMath` hátteret (`impl/subjects/math/math_native.cpp`) gyakoroltatja végpontól végpontig a tárgy referenciademójával; a `sleelactl info math` elmagyarázza, hogyan érhető el (`import math;`). A subject természet kizárólag a `run info` parancsokat kínálja — szándékosan nincs `check` ige, mert a `sleela check` nem tölti be a SHEET-ben regisztrált natív tárgyakat.

### `@native`, Native Drive és többnyelvű végrehajtás

Az új **`@native`** metódusannotáció egy forró metódus ahead-of-time natív fordítását kéri. Az új `sleela aot` parancs megkeresi a `@native` metódusokat, ellenőrzi, hogy mindegyik egy támogatott *kiszervezhető részhalmazon* belül van-e, és C/C++ kódot bocsát ki — **zárt hibamóddal (fail closed)**: pontos indoklással megtagad minden olyan szerkezetet, amelyet még nem tud lefordítani, így félig lefordított metódus soha nem keletkezik.

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

Az `@native` továbbra is AOT-kezelés iránti kérés, a nem támogatott szerkezeteket a kibocsátás előtt elutasítják. A [Native Drive](impl/native-drive/README.md) többegységes C/C++ futtatható és megosztott könyvtár buildeket támogat gazdafordítókon keresztül (az MSVC-t is beleértve), támogatott SLeeLa AOT-kimenetet, szimbólumvizsgálatot a natív betöltő ABI-jával szemben, shell nélküli folyamatvégrehajtást, stdin/stdout/stderr kezelést, többlépcsős csővezetékezést, valamint az XClass delegálását a produkciós betöltőnek. A `sleelactl` elérhetővé teszi a build-/futtatási parancsait, és az XClass-vizsgálatot, valamint az explicit systemd-unit renderelést/telepítést az `inspect native-drive` parancson keresztül irányítja. A VM egy korlátozott méretű natív modul-handle táblát birtokol kötelező gazdaengedélyezéssel és generációellenőrzött handle-ökkel. A 3.0-s szintaxis most hozzáadja a `nativeOpen`, `nativeCall` és `nativeClose` hívásokat: a csak hozzáfűzhető bájtkód-diszpécselés egy külön verziózott null/bool/int64/double/UTF-8 ABI-n halad át, miközben a VM által birtokolt tömbök, struktúrák és más összetett értékek az interpreteren belül maradnak, és használhatnak azonos nevű bájtkód-tartalékot. A bővítmények opcionálisan kaphatnak egy verziózott gazda-visszahívási táblát, amellyel pontos SLeeLa-függvényeket hívhatnak meg beágyazott VM-végrehajtáson keresztül, felfüggesztett verem GC-gyökerezéssel, explicit gazda-visszahívásnév engedélyezőlistával, a rekurzív natív API-hívások pedig elutasításra kerülnek. A natív kód továbbra is megbízható, folyamaton belüli kód, nem homokozó. A forrásszintű folyamatpolitikai annotációknak van verziózott sémájuk, de még nem értelmezi és nem kényszeríti ki őket semmi. Részletek, biztonsági határok és a hét munkafolyamatos főkönyv: [Native Drive](impl/native-drive/README.md) és [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).


## XIX. Nemzetközi README-kiadások

A SLeeLa országspecifikus README-belépési pontokat biztosít nemzetközi kiadásának 50 piacára. Minden országhoz egy `README.<CC>.md` nevű fájl tartozik; ahol több nyelv is releváns, azok az adott ország fájljában vannak csoportosítva.

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

**Fordítási szabályzat:** Ezek a fájlok kezdeti, lokalizált országos belépési pontok. Bevezetőik és nyelvi címsoraik lokalizáltak, a teljes műszaki referencia azonban a kanonikus angol [README.md](README.md) fájlban marad. Egy országfájl nem írható le teljes fordításként, amíg minden szakaszt le nem fordítottak, a műszaki azonosítókat és hivatkozásokat nem ellenőrizték, és a kiadás nyelvi felülvizsgálaton nem esett át. A nyelvi lefedettség ebben az első körben reprezentatív, és nem állítja, hogy minden alkotmányosan elismert, regionális, őshonos vagy jelnyelv teljesen le van fordítva.



## Függelék — megőrzött regionális útmutatás

Az alábbi gyakorlati szakaszok a korábbi, rövidebb magyar útmutatóból származnak, és a kanonikus README mellett maradnak meg. Nem helyettesítik a fentebbi szakaszokat; ahol eltérés mutatkozik, a kanonikus tartalom az irányadó.

### Célplatformok, profilok és a natív végrehajtás határai

A SLeeLa külön kezeli a forráskódot és a build-/végrehajtási célokat. A dokumentált célcsalád része az `SLVM`, az `SLJVM` és az `SLIR`; a `NATIVE` korlátozott, feltételekhez kötött cél, ezért nem tekinthető minden platformon általánosan elérhetőnek. A `BASIC_COMPLETE` és az `ADVANCED_TOTAL` profil eltérő hatókört jelöl (lásd az `api/sleela/*/package/vm.html` oldalakat). Minden buildnél ellenőrizze a tényleges célt, profilt és platformtámogatást.

A hivatkozás fájlt, modult vagy erőforrást azonosít, de önmagában nem ad végrehajtási engedélyt. Ez különösen fontos fájlok, külső könyvtárak, natív kötések és rendszererőforrások használatakor. Ellenőrizze a bemeneteket, és határozza meg, melyik komponens jogosult az adott műveletre. A dokumentációban szereplő állítás önmagában nem bizonyít futásidejű biztonsági garanciát; ehhez megvalósítási és tesztbizonyíték szükséges.

### Erőforrás-életciklus és hibakeresés

Dokumentálja a függvények vagy szálak között megosztott erőforrások tulajdonjogát, élettartamát és szinkronizációját. Különböztesse meg a lokális változókat, a megosztott objektumállapotot, a VM-handle-öket és a szerializálható adatokat. A VM-lokális handle nem stabil fájlformátum vagy hálózati üzenet. Használja a modul dokumentált szerializációs felületét, és tesztelje az érvénytelen, hiányzó és határértékű bemeneteket.

Hibakereséskor rögzítse a commitot, az operációs rendszert, az architektúrát és az eszközláncot. Először a legkisebb releváns célt fordítsa le, őrizze meg a teljes hibaüzenetet, különítse el a fordítási hibákat a futásidejű hibáktól, és készítsen minimális, reprodukálható példát. A sikeres build önmagában nem bizonyítja a helyes futást.

A megfelelő ellenőrzési jelentés tartalmazza a pontos commitot, az operációs rendszer és az architektúra adatait, a fordítóverziókat, a teljes build- és tesztparancsokat, a sikeres/sikertelen/kihagyott tesztek számát és az ismert korlátozásokat. A kihagyott vagy le nem futtatott teszt nem számíthat sikeresnek.

### Modul-API-szerződések, kompatibilitás és tesztelés

Új API bevezetésekor dokumentálni kell a bemeneteket, kimeneteket, hibafeltételeket, az adatok tulajdonjogát és az esetleges mellékhatásokat. A példáknak a szokásos használat mellett az érvénytelen vagy hiányzó bemenet kezelését is be kell mutatniuk. Olyan viselkedést ne állítsunk garanciaként, amelyet a megvalósítás és a tesztek nem igazolnak.

A teszteket a fordítási folyamat rétegei szerint érdemes csoportosítani: lexikai elemzés/szintaktikai elemzés, szemantikai elemzés, IR-leképezés, kódgenerálás, VM-végrehajtás és natív integráció. Minden regressziós esetnél őrizzük meg a legkisebb reprodukálható bemenetet, az elvárt és a tényleges eredményt. API-változásoknál ismertetni kell a visszamenőleges kompatibilitás kérdéseit, a migrációs lépéseket és a meglévő példákra gyakorolt hatást.

### Nemzetközi dokumentáció karbantartása és lokalizáció

Az országonkénti README-k helyi belépési pontok a műszaki dokumentációhoz; nem helyettesítik a normatív nyelvi specifikációt vagy a modulok API-dokumentációját. Az angol forrás változásakor ellenőrizze a fordítás elavulását. Az azonosítókat, parancsokat, útvonalakat, szintaxist és kódpéldákat minden nyelvi változatban változatlanul kell hagyni; a magyarázó szöveget kell fordítani, a forráskód tokenjeit és neveit nem. A magyar terminológiát következetesen kell használni.

Minden frissítés után ellenőrizze a Markdown-hivatkozásokat, a címsorhierarchiát, a kódkerítések párosítását és a példákat. Az angol kanonikus dokumentumok módosításakor szakaszonként hasonlítsa össze és frissítse a fordítást, ne csak a bevezetőt. A nyelvi vagy műszaki ellenőrzésen még át nem esett tartalmat jelölje.

**Fordítási állapot:** Az I–XIX. szakasz magyar nyelvre lefordítva. A műszaki azonosítók, parancsok, kódpéldák, fájlnevek, hivatkozások és címek megmaradtak. Anyanyelvi beszélők általi záró nyelvi ellenőrzés ajánlott.
