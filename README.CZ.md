# SLeeLa 3.0 — Česká republika (CZ)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="Marketingové logo SLeeLa" width="560"></p>

<p align="center"><strong>Nativní systémový jazyk, překladač, runtime a ekosystém virtuálních strojů.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Mezinárodní národní edice.** Tato edice je v češtině. Technické identifikátory, příkazy, ukázky kódu, názvy souborů, cesty a odkazy zůstávají beze změny oproti kanonickému anglickému [README.md](README.md).

**The Now. The Ingredient.** Jazyk je víc než syntaxe: je to překladač, runtime, knihovny, nástroje a platformní smlouvy, díky nimž je software použitelný.

**Čím se SLeeLa zabývá.** SLeeLa se zabývá oblastmi **Math**, **Object**,
**Science** a **Reach** — čtyřmi tvářemi své práce: počitatelným
(Math), usazenou věcí stojící v katalogu toho, co jest (Object),
kázněným studiem pravdivého (Science) a rozšířením ruky systému
k jiným systémům a lidem (Reach). **Národní zájmy zůstávají** — žádná
ze čtyř oblastí je neodsouvá stranou; kde se téma dotýká zájmu některého národa,
toto postavení se zachovává (věrně se zaznamenává např. v metadatech každé třídy
`metaOrigin` / `metaNationalSchedule`) a nikdy se nerozpouští.

## I. Začínáme
- [Sestavení nativní implementace](impl/README.md)
- [Virtuální stroje a Modium — praktická uživatelská příručka](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [Zásobník CPU ve zdrojovém kódu SLeeLa](lib/cpu/CPU.md) — kompletní stroj od hradel přes ALU a jádra a OS až po programy, soupis více než 130 profilů CPU a nastavitelná [napětí, proměnné takty a expertní časování](lib/cpu/TIMING.md), která můžete [provozovat podle specifikace i přetaktovat](lib/cpu/components/README.md)
- [Rejstřík dokumentace](docs/README.md)
- [Zdroj dat CSPML a výzkumný referenční materiál Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — původ dat a pokyny k integraci pro externí americká data o místech, socioekonomice, pracovní síle, vzdělávání a veřejných školách.
- [Generovaná dokumentace API](docs/api/README.md) — aktuální HTML reference, verzované vydání API a pokyny k opětovnému generování
- [Katalog API](api/README.md)
- [Standardy výpočetního porovnání](comparison/README.md) — standardy typů, paměti, ABI a shody pro SLeeLa VM, Java/JVM a nativní C/C++.
- [Recursion stanchion — prototypy v C a C++](recursion/stanchion/README.md) — příklady specializované na primitivní typy Boolean, Byte, Character, Integer, Long, Float a Double vedle obecných, aplikovaných, vědeckých a výukových rekurzivních vzorů.
- [Podpora platforem a validační matice](docs/PLATFORM-SUPPORT.md)
- [Průvodce značkou](docs/BRAND-GUIDE.md)
- [Koncepty a podklady merchandisingu](merchandise/README.md)
- [Logo NitroWebExpress](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Zásady testování a podmínky vydání](markdown/TESTING.md)

## II. Novinky

Nedávné přírůstky v jazyce, ovladačích a standardní knihovně:

- **Balíček standardní knihovny `advantage` — zvýhodnění dividendy s
  matematikou, fyzikou, integrací, hodnocením a směrovatelným výstupem.**
  [`lib/advantage/`](lib/advantage/README.md) zapouzdřuje *zvýhodnění jedinečné
  dividendy vzhledem k modelu* jako vrstvenou **cibuli Sequitur**
  (dividenda → review → concerns → maths → additions → subtraction → regrouping
  → iteration), po které lze procházet dovnitř i ven s plnou dynamickou
  aktualizací a obnovou (restitute) a která je zpřístupněna prostřednictvím
  jednoduchého **modelu uživatele** a plně řiditelného **modelu vývojáře**.
  Zahrnuje pět tematických oblastí: **`math/`** — ekonomika bilaterálního
  obchodu (společná maximalizace zisku, Nashovo vyjednávání, kritický poměr
  bilaterálního newsvendora), **`physics/`** — strukturální ekonomická „fyzika
  polí důvěry“ (důvěra jako metrický tenzor, transakční geodetika, kontaktní
  struktura oznámení, SDE rozpadu důvěry typu CIR+skok — modelovací analogie,
  jako taková výslovně označená), **`integration/`** — rekurzivně skládatelný
  model vertikální integrace pro kapitalismus/maloobchod (kompartmenty,
  inženýrské zálivy, zkušební kompartmenty, newton median trays a skóre
  sofistikovanosti systému), **`grading/`** — hodnotitel sofistikovanosti
  (circuit grader), který známkuje každý modul i libovolnou množinu modulů
  winning/losing/grader/absolute stupni A–D, a **`output/`** — směrovatelné
  emisní rozhraní, které posílá výstup modulů na terminál, do povrchu
  formátovaného pomocí **CommonRails** nebo do soketu/konektoru (zarámováno pro
  nativní `slnet_write`). Každá třída nese záznam Metadata-Class; pět
  spustitelných ukázek ověřuje jednotlivé oblasti a doprovodné dokumenty
  [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md),
  [`MATH.md`](lib/advantage/MATH.md) a
  [`PHYSICS.md`](lib/advantage/PHYSICS.md) je popisují. Nativní konzument v C/C++
  v adresáři [`advantage/`](advantage/README.md) (jádro v C
  `advantage_value`/`advantage_mean`, úložiště znalostí v XML, knihovna pro
  zabezpečené získávání dat přes HTTPS/SFTP/SSH + úložiště klíčů + přenos
  DH/ECDH) propojuje model s propracovanými
  [obchodními příklady](advantage/examples/README.md) — pozitivní obchody z
  předpokládaných internetových zdrojů, posloupnost relevance pro publikum
  (Traders, Consumers, Helpers, Healers) a osm modelů seřazených A–D (D = nejvíce
  Cement, A = nejvíce Brand).
- **Anotace časování smyček — `@loopDelay` / `@loopBudget` (+ vestavěná funkce
  `sleep(ms)`).** První **příkazové, behaviorální** anotace v SLeeLa:
  zapisují se na samostatný řádek před smyčku `for` / `while` / `do` a vkládají
  do provádění smyčky skutečnou pauzu. `@loopDelay(ms = 250)` /
  `@loopDelay(seconds = 2)` uspí smyčku o pevnou dobu při každé iteraci (holé
  `@loopDelay(250)` se vykládá jako milisekundy); `@loopBudget(seconds = 10,
  iterations = 1000)` rozloží známý počet iterací rovnoměrně na celkový
  rozpočet skutečného času. Obě se převádějí na nový, pouze přidávaný opkód
  **`OP_SLEEP`** (ISA 157), který zároveň stojí za obecnou vestavěnou funkcí
  **`sleep(<ms>)`** volatelnou odkudkoli; pauza je implementována přenositelně
  (`nanosleep` / `Sleep`). Specifikováno v
  [`SLEELA.syntax` §32](SLEELA.syntax); reference API v
  [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md);
  spustitelný příklad
  [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela).
  Rejstřík přírůstků ze sezení (včetně tohoto, nativního násobení velkých
  celých čísel `OP_BIGMUL` a samostatných nástrojů **[local-generator](local-generator/README.md)**
  a **[pi-generator](pi-generator/README.md)**) je v
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md).
  Tři generované povrchy API
  ([`sleeladoc-api/`](sleeladoc-api/README.md),
  [`docs/api/`](docs/api/README.md) a neměnný snímek
  [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html))
  byly znovu sestaveny příkazem `make api-docs` a na stránce balíčku `vm` nyní
  nesou nové položky instrukcí `SLISASleep` / `SLISABigMul`.
- **SleelaDoc z příkazového řádku — `sleela docs`.** K SleelaDoc se nyní lze
  dostat přímo z terminálu. `sleela docs` vypíše všechna dokumentovatelná témata;
  `sleela docs crypto` zobrazí balíček; `sleela docs stringbuilder` vyhledá třídu
  (bez ohledu na velikost písmen a podle prefixu standardní knihovny `SL` →
  `SLStringBuilder`) a vypíše bohatou stránku — **definici, návrhový účel,
  vztahy, strukturu, uživatelské clays a pětistupňový žebříček zvládnutí
  (mastery)** — s pořadím návrhů „měli jste na mysli“ při překlepu. Autoři
  stránky obohacují prostě přidáním značek SleelaDoc `@` (`@intent`, `@thesis`,
  `@axiom`, `@param`, `@see`, `@mastery`). `sleela docs --generate [dir]` zapíše
  textový korpus (jedna stránka na třídu + `TOPICS.index`), který CLI poté čte
  offline; **`make sleeladoc`** zabalí tento korpus **i odpovídající zdroje
  `.sleela`** do `sleeladoc/build/sleeladoc.jar`. Pro subjekty pouze s fasádou
  (např. `psychiatry`, `politico`) `sleela docs --facade <topic>` za běhu
  prohledá souborový systém a vypíše skutečné **složky (poté soubory, jako
  přirozený pohyb)**, v mezích uživatelsky přepsatelných limitů
  **6 MB / 6000 souborů / 6 minut**. Jednotky `lib/sleeladoc` jsou udržovány
  v aktuální edici, **`#sleela 3.0`**. Viz [`man/man1/sleela.1`](man/man1/sleela.1) a
  [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md).
- **SleelaDoc — dokumentační řešení ve stylu Javadoc s generovaným webem HTML.**
  **SleelaDoc** je konvence dokumentačních komentářů SLeeLa přímo ve zdrojovém
  kódu (komentáře v úhlových závorkách označené `@doc` / `@page`; viz
  [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)) a
  [`lib/sleeladoc`](lib/sleeladoc/README.md) je **extraktor**, který vykreslí
  jednotku `.sleela` jako stránku API v Markdownu nebo HTML — tak, jak `javadoc`
  vykresluje Javu, a dokumentuje přitom *záměr*. Kompletní, procházitelný
  **web s HTML dokumentací celého `/lib`** generuje do
  [`/sleeladoc-api`](sleeladoc-api/README.md) skript
  `tools/generate-sleeladoc-api.py` (otevřete `sleeladoc-api/index.html`; čistý
  styl modré / světle modré / světle šedé / bílé s hierarchií balíček → třída →
  člen a glosářem psaným pro čtenáře *Student 2*). Překladač také jako
  vedlejší produkt překladu vydává výstupy dokumentace označené autorem
  (stránku MediaWiki `--wiki` + originovaného zdrojového dvojníka), popsané v
  [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md). Spustitelný příklad:
  [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela).
- **Objektový model dokumentu XML (`lib/xml`).** XML DOM na straně SLeeLa —
  dokument vlastnící strom uzlů element/text/komentář/CDATA/PI s uspořádanými
  potomky a pojmenovanými atributy, serializér (kompaktní/formátovaný, escapování
  podle XML 1.0 §2.4), parser po znakových buňkách a rejstřík kvality
  `SLXmlAspect`. Rovnost uzlů je trojstupňový svazek důvěry (identita → stejný
  subjekt → struktura), reflexivní, symetrický a deterministický. Viz
  [`lib/xml/README.md`](lib/xml/README.md) a spustitelný
  [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela).
- **Široké znaménkové celočíselné primitivy (`long`, `int64`, `int128`).** `int`,
  `long` a `int64` jsou všechny 64bitová znaménková celá čísla, která se
  typově kontrolují a spouštějí shodně; `int128` je přijímán jako celočíselný
  typ (zatím s 64bitovým podkladem, skutečných 128 bitů je zdokumentované
  navazující úsilí). Rozšiřují je dvojkově založená znaménková řada
  `int32`..`int16777216` (syntaxe 2.7) a odpovídající bezznaménková řada
  `uint32`..`uint16777216` (syntaxe 2.8). Viz [`SLEELA.syntax`](SLEELA.syntax) §21, §24, §25.
- **Navazující pole `@mix` (syntaxe 2.9).** Třída může deklarovat více
  instančních polí se **stejným názvem**, pokud každé opakování nese `@mix`;
  jazyk chápe skupinu jako navazující (pole / seznam indexovaný pořadím),
  shora dolů `1,2,3,…` ve výchozím stavu nebo ručně pomocí `@mix(1,2,3,...)`,
  jehož počet indexů musí odpovídat počtu polí. Viz [`SLEELA.syntax`](SLEELA.syntax) §28.
- **Dokumentové značky — Přirozený proces, nazývané Preonems
  (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).**
  Tato holá slova dokumentových značek se souhrnně nazývají **Preonems**
  (jedna značka je *Preonem*; množina, *Preonems* — viz
  [`SLEELA.syntax`](SLEELA.syntax) §26.1). Prvotřídní, čistě deklarativní
  dokumentové značky pojmenovávající, čím dokument *je* v pořadí tvorby:
  `author`/`authorial` (syntaxe 2.4; zpracovává odborná rada — jediný člen s
  behaviorálním účinkem), `material;` (usazený Zdroj) a `memory;`
  (syntaxe 2.5), `determine;` (syntaxe 2.6; dokument otevřený k dalšímu
  zpracování lékařem, fyzikem nebo duchem směrem k dokončení právní rukou)
  a dvě nejnovější, **`object;`** (usazená věc stojící v katalogu toho, co
  jest — **reifikovaná, s dodaným konkrétním postavením, v syntaxi 2.8**)
  a **`field;`** (základ vztahu, v němž objekty stojí a pohybují se) —
  obě podmíněné syntaxí od 2.6. Každá je holá, kontextová a verzovaně
  podmíněná front-endem, bez vlivu na parsování, typování ani generování kódu;
  mimo pozici značky zůstává každá běžným identifikátorem (takže `field`
  nikdy neruší člen třídy ani přístup `.field`). Viz [`SLEELA.syntax`](SLEELA.syntax)
  §18–§20, §23 a §26.
- **Nordshrift v syntaxi 3.0 s plným dosahem do knihovny.** Nordshrift vyhledává
  prostřednictvím stejného rekurzivního indexu `/lib` jako překladač a dosáhne
  na každý balíček a jednotku `.sleela`; `nordshrift check` hlásí počty pro celý strom.
- **Recursion stanchion.** Adresář [`recursion/stanchion/`](recursion/stanchion/README.md) nyní obsahuje vyhrazené referenční prototypy v C a C++ pro sedm zdokumentovaných primitivních smluv a také obecné, aplikované, vědecké a výukové příklady. Jsou to referenční zdroje; než je budete považovat za ověřené, přeložte je a otestujte.

- **Komentáře v úhlových závorkách (`<` / `<<`).** Nový styl komentářů pro další
  vývoj — `< řádek >` a `<< blok >>` — lexikálně zpracovaný front-endem SLeeLa
  a zdokumentovaný v [`SLEELA.syntax`](SLEELA.syntax) §1.1.
- **Celočíselné literály s radixem.** Literály `int` nyní přijímají šestnáctkový
  (`0x`), dvojkový (`0b`) a osmičkový (`0o`) zápis s oddělovači číslic `_`, a to
  jak ve zdrojovém jazyce SLeeLa ([`SLEELA.syntax`](SLEELA.syntax) §1.4), tak ve
  [formátu modelů CPU `.xlt`](lib/xlt/README.md).
- **Standard Metadata Class v celé knihovně.** Metavzor popisných metadat
  (původ, legalita, jurisdikční rozsah) nyní nese každý soubor `.sleela` s
  třídami pod `/lib`; viz [`SLEELA.syntax`](SLEELA.syntax) §15 a
  [výukovou lekci](tutorial/13-metadata-class.md).
- **Bohatší rodina text / String.** `lib/text` získává skutečné pomocné funkce
  přímo v jazyce, znakově-buňkový pohled poskytující skutečné `charAt`/`substring`/
  `indexOf`/`reverse`/zarovnání sloupců a skutečný poziční šablonovač — viz
  [výukový text o textu a String](tutorial/14-text-and-strings.md) a spustitelný
  [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela).
- **Prvotřídní `String` v listech Nordshrift.** List `.sst` může deklarovat
  typovaný blok `strings:`, který přispívá do manifestu komponent sestavení;
  viz [Nordshrift Sheet Reference](wiki/Nordshrift-Sheet-Reference.md).
- **Modely CPU se spouštějí jak operačními kódy, tak ekvivalencí volání funkcí.**
  Každé `SL<ARCH>CPU` dědí základ spouštění nativního proudu instrukcí svou
  smyčkou fetch-decode-execute *a také* spouštění kódu předpokládaného jiného
  procesu ekvivalentními voláními na sdíleném substrátu VM — viz
  [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5.

## III. Sestavení a testování

**Požadavky:** toolchain C/C++ (GCC nebo Clang) a GNU Make; Python 3 pro
nástroje manifestů a toků; na Linuxu vývojové balíčky X11/Xrandr pro
zobrazovací produkty (`make deps` daného modulu ohlásí/nainstaluje, co
potřebuje). Sestavení `impl/` je fail-closed vůči důvěryhodnému manifestu
SHA-256, na který kořenový `Makefile` odkazuje automaticky.

Z čistého checkoutu nejrychleji začnete takto:

```sh
make                                  # sestaví vše z kořene repozitáře
```

Nebo sestavte a otestujte pouze nativní jádro:

```sh
make -C impl all                      # sestaví pouze jádro překladače/runtime
bash ./test-suites/run-all.sh --all   # spustí celou testovací sadu
```

Kořenový `make` sestavuje každý produkt (jádro, VM, kodeky, churn,
antivirus, …) a končí sestavením **registru** a materializací jeho
základní tabulky — takže sestavený strom dodává revidovatelný registr rovnou
po vybalení. Základní cíle:

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

Další užitečné cíle:

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

Cíl `registry` spouští [`build-registry.sh`](build-registry.sh): sestaví
jádro registru, nasází základní tabulku do přenositelného textového úložiště
`registry/build/sleela.sleelareg` a exportuje skript tabulky MySQL
`registry/build/registry.sql` (pomocí `sleela registry export`). Zkontrolujte jej
příkazem `sleela registry` nebo `registry/build/sleela-registry review`. Viz
[příručka Registry](registry/README.md) a [architektura Registry 3.0](registry/REGISTRY-3.0-ARCHITECTURE.md).

Požadavky a kroky specifické pro platformu najdete v průvodcích sestavením a testováním. Úspěšné sestavení jedné komponenty nedokazuje, že jsou všechny knihovny nebo platformy připraveny k vydání.

## IV. Instalace

Po sestavení nativních produktů nainstalujte příkazy a soubory podpory runtime:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

Konvenční instalátor ve výchozím stavu používá `/usr/local/bin`, `/usr/local/lib/sleela`
a `/usr/local/share/sleela`. Zachovává existující konfiguraci spouštění
a odmítá přepsat existující příkazy, které nejsou pod jeho správou. Odinstalace
odstraňuje pouze soubory zaznamenané instalátorem a zachovává konfiguraci
i nesledovaná data runtime.

Pro cílené, řízené nasazení pouze hlavního balíčku — binárních souborů `sleela`,
`sleelvac` a `nordshrift`, standardní knihovny, protokolů a podpůrných modulů —
použijte čistý instalátor:

```sh
make clean-install       # guided install of the main package only
```

Činí jedno primární rozhodnutí (nainstalovat hlavní balíček), poté volbu
rozsahu a po úspěchu nabídne volitelný výběr z kurátorských rozšiřujících
modulů. Interaktivní postup nasazení a všechny možnosti instalace viz
[Rychlá a bezpečná instalace](install/README.md).

## V. Jazyk a runtime
Závaznou gramatikou je [`SLEELA.syntax`](SLEELA.syntax). Aktuální repozitář dokumentuje syntaxi **3.0**; verze překladače/nástrojů a verze zdrojové syntaxe jsou odlišné pojmy. Povrch 3.0 přidává **dokumentové značky `pure` a `gasoline`** (§30) — dvojici holých, kontextových, čistě deklarativních Preonems, které znamenají téměř totéž a každá označuje postavení dokumentu svou vlastní upřímností. Syntaxe 3.0 sjednocuje jazykový povrch 1.0–2.9 do aktuální normativní edice a zachovává všechny minimální verzní podmínky jednotlivých funkcí. Předchozí povrch 2.9 přidává **navazující pole `@mix`** (§28) — třída může deklarovat více instančních polí se stejným názvem, pokud každé nese `@mix`, chápaných jako jedna indexovaná skupina (shora dolů `1,2,3,…` nebo ručně `@mix(1,2,3,...)`, jehož počet indexů musí odpovídat skupině). Přenáší dále dvojkově založené rodiny širokých celých čísel se znaménkem a bez něj `int32`..`int16777216` (§24) a `uint32`..`uint16777216` (§25) nad dřívějšími `long`/`int64`/`int128` (§21); **dokumenty podporující klíčová slova** (§22); a **Preonems** — čistě deklarativní dokumentové značky `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) a `artifact` (§35, postavení legislativní úrovně odstupňované k času a prostoru), souhrnně nazývané Preonems (§26.1). Rozlišení přetížení konstruktorů a delegování `this(...)`/konstruktoru báze jsou výslovně mimo ověřený kontrakt syntaxe 1.8. Než se na některou funkci spolehnete, prostudujte gramatiku a poznámky k implementaci. Devět zdrojových jednotek SleelaDoc nyní deklaruje `#sleela 3.0`; sdílená brána překladače přijímá zdrojovou syntaxi 1.0–3.0. Nové sestavení API vzniká příkazem `make api-docs`.

Repozitář obsahuje nativní implementaci v C/C++, reference generování SLVM, moduly standardní knihovny, testy, příklady a platformní integrace. Stav implementace a validace se u jednotlivých modulů liší; řiďte se jeho specifikací a aktuálními výsledky CI a nepředpokládejte jednotnou úplnost.

## VI. Zásady inženýrství
- **Důkazy před tvrzeními:** rozlišujte implementované, testované, experimentální a plánované chování.
- **Jedna smlouva na každé rozhraní:** dokumentujte syntaxi, API, formáty souborů, předpoklady o platformě a chování při selhání.
- **Přenositelnost již v návrhu:** izolujte backendy specifické pro platformu a testujte na nativních runnerech.
- **Bezpečné selhání:** testujte nevalidní vstup, limity zdrojů, selhání procesů a cesty úklidu.
- **Reprodukovatelná vydání:** sestavujte znovu načisto a zveřejňujte hashe artefaktů a poznámky k vydání.

## VII. Sestavení a integrita antiviru

Antivirový balíček Sleelavirin obsahuje nativní skener SHA-256, volitelný
adaptér ClamAV, manifesty integrity zdrojů/indexů a kontrolní součty
platformních balíčků. Balíček sestavte a otestujte z kořene repozitáře:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Manifest integrity se generuje pod `antivirus/build/`; pokrývá sledované
zdrojové a dokumentační vstupy i chráněné soubory indexu signatur, přičemž
vylučuje generovaný výstup a proměnlivou cache ClamAV. Snímky databáze ClamAV
mají vlastní manifesty. Vydané archivy ZIP obsahují `INTEGRITY-MANIFEST.json`,
`SHA256SUMS` a doprovodný soubor `.sha256`. Hashe stažených archivů ověřujte
proti důvěryhodné hodnotě získané nezávisle: kontrolní součet uložený vedle
archivu není podpisem vydavatele. Viz [příručka Sleelavirin](antivirus/README.md),
[bezpečnostní kontrolní seznam](antivirus/SECURITY.md) a
[workflow sestavení antiviru](.github/workflows/antivirus-binaries.yml).

Vydání se nepovažuje za ověřené, dokud příslušné úlohy CI neprojdou na
přesném commitu vydání a výsledné artefakty nebyly zkontrolovány.


## VIII. Editorová témata SLeeLa — Vim a Emacs

SLeeLa obsahuje koordinovanou sadu profesionálních tmavých barevných schémat pro **Vim** a **Emacs** a nativní palettovou utilitu v C/C++. Původní schéma **American Professional** zůstává výchozí. K dispozici je deset doprovodných palet inspirovaných různými kulturními a institucionálními designovými tradicemi; jde o interpretační designové palety, nikoli o oficiální reprezentaci jakéhokoli národa, kultury, soudu či instituce.

### Jedenáct schémat a jejich barvy

| Schéma | Pozadí | Popředí | Klíčová slova | Typy | Řetězce | Komentáře |
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

#### Barvy národních vlajek (zdrojová paleta)

Řecké a turecké schéma jsou odvozena z **oficiálních barev národních vlajek** obou zemí, zde uvedených s jejich přesnými publikovanými hodnotami hex. Pozadí a akcenty schémat výše jsou ztmavené, pro editor přívětivé interpretace těchto zdrojových barev — tabulka níže je referencí „barev tak, jak jsou národně vystaveny“.

| Národ | Barva | Hex | Základ a význam |
|---|---|---|---|
| **Řecko** | Modrá | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | Modř národní vlajky (zákon 851/1978). Nebe a moře; heraldicky blasonována jako *azure*. |
| **Řecko** | Bílá | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Bělost národní vlajky. Čistota boje za nezávislost; kříž označuje východní pravoslavné křesťanství. |
| **Türkiye** | Červená (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | Červeň národní vlajky (turecký zákon o vlajce). Pole; podle tradice krev mučedníků a osmanská suverenita. |
| **Türkiye** | Bílá | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Bělost národní vlajky. Půlměsíc a hvězda — mír, čistota a symbol turkického/islámského dědictví. |

Schémata ukotvují popředí každého editoru na národní **bílé** a jeho klíčová slova, kurzor, výběr a stavový řádek na přesné národní **modré** (`#0D5EAF`) nebo **červené** (`#E30A17`), takže důležité barvy jsou doslova barvami národními. Jde o interpretační designové palety; reference barev vlajek je uvedena kvůli původu a není oficiální reprezentací žádného ze států. Významy barev vycházejí z popisu státních symbolů ministerstva zahraničních věcí Helénské republiky a z tradice tureckého zákona o vlajce.


### Tabulka barevných vzorků

Buňky palet níže používají **vzorky SVG**, takže GitHub vykreslí skutečné barvy místo prázdných buněk HTML. Každý proužek ukazuje šest barev v tomto pořadí: pozadí, popředí, klíčová slova, typy, řetězce a komentáře. Přesné kódy hex jsou uvedeny vedle každého vzorku, ve stejném pořadí schémat jako ve výše uvedené referenční tabulce.

| Schéma | Vykreslené barvy palety | Kódy hex (pozadí · popředí · klíčová slova · typy · řetězce · komentáře) |
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

Další akcentní barvy se používají pro funkce, konstanty, výběry, diagnostiku a direktivy preprocesoru. Ruská a slovanská paleta záměrně používá středně intenzivní břidlicovou, modrou, šalvějovou, rubínovou a starozlatou pro klidnější profesionální vzhled.

### Instalace schémat pro Vim

Z kořene repozitáře:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Do `~/.vimrc` přidejte následující, abyste zapnuli zvýrazňování syntaxe SLeeLa a vybrali schéma:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Zvolte přesně jeden z těchto názvů colorscheme:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Například použijte `colorscheme sleela-russian-slavic` pro střední ruskou a slovanskou paletu, nebo `colorscheme sleela-greek` / `colorscheme sleela-turkish` pro palety Hellenic a Al Bayrak. Restartujte Vim nebo načtěte konfiguraci příkazem `:source $MYVIMRC`.

### Instalace schémat pro Emacs

Vytvořte místní adresář témat a zkopírujte soubory režimu a témat z kořene repozitáře:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Přidejte toto do `~/.emacs` nebo `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Chcete-li vybrat jiné schéma, nahraďte `sleela-american` hodnotou `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` nebo `sleela-sleela`. Pokud se Emacs zeptá, zda tématu důvěřovat, před potvrzením zkontrolujte místní soubor tématu.

### Náhled nativních palet C/C++

Malá nativní palettová utilita umí vypsat schémata, vytisknout jejich barvy pro terminál a vyhledat jednotlivou barvu:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Kvůli zpětné kompatibilitě volby `--palette` a `--hex COLOR` bez `--theme` vybírají American Professional. Výsledky sestavení a testů je třeba ověřit na cílovém systému a v CI; dokumentace příkazu sama o sobě není důkazem, že prošel.

Úplné popisy palet a podrobnosti o utilitě viz [průvodce integrací editorů](linux-editors/README.md).


## IX. Přispívání
1. Přečtěte si rejstřík dokumentace a zásady testování.
2. Provádějte cílené změny s pozitivními i negativními testy.
3. Spusťte cílené testy, poté širokou sadu.
4. Uveďte podrobnosti o OS a toolchainu; přeskočené kontroly nepopisujte jako úspěšné.

SLeeLa vyvíjí **MEARVK LLC**. Historický materiál úvodní stránky je zachován v [`docs/README-LEGACY.md`](docs/README-LEGACY.md).

## X. AE6E66 — Očekávání odvodu z Round Public e-mailu

U iniciativy AE6E66 je deklarovaným očekáváním projektu **$300,000,000.00 USD za každý Round Public e-mail** jako navrhovaná výše daňového odvodu spojená s každým kvalifikujícím Round Public e-mailem.

Toto prohlášení zaznamenává pouze očekávání projektu. **Není** to právní daňový výměr, vládní nařízení, soudní rozsudek ani nezávisle stanovená daňová povinnost. Jakýkoli skutečný odvod by vyžadoval příslušný právní podklad, jurisdikci, definice, postup a rozhodnutí příslušného vládního orgánu.


## XI. Abeceda a runtime znakových sad

Dokumentace abecedy a nativní znakový runtime SLeeLa jsou udržovány v alphabet/sleela/ a lib/alphabet/native/. Nativní testy v C++17 pokrývají striktní dekódování/kódování skalárních hodnot UTF-8, podpisy kapacity C<n> a ohraničenou příslušnost ke znakové sadě:

    make -C lib/alphabet/native test

Experimentální katalog UTF-4088 a jeho vyhledávací adaptér pro SLeeLa jsou zdokumentovány v utf-4088/SLeeLa-ALPHABET-INTEGRATION.md. Identifikátory UTF-4088 tvoří samostatný jmenný prostor, nikoli skalární hodnoty Unicode. Práce na vazbách překladače a SLVM je výslovně sledována odděleně od těchto nativních základů.

## XII. AutoCAD Creator™

[AutoCAD Creator™](autocad/creator/README.md) je interaktivní 2D CAD
aplikace napsaná celá v SLeeLa. Zatímco sesterský AutoCAD Renderer promění
plán ve výkres jedním tahem, Creator vám umožňuje 2D dokument AutoCAD **vytvářet
a měnit** za běhu — přidávat stěny, místnosti, kóty a poznámky, pak umístěné
prvky přesouvat, přejmenovávat popisky, přeřazovat do vrstev, mazat a vracet
zpět — a výsledek **uložit** jako skutečný výkres AutoCAD DXF (text R12, otevře se
v AutoCADu / LibreCADu).

Jeho grafické rozhraní je postaveno na **SleelaUI™**, vlastní multiplatformní
sadě nástrojů UI SLeeLa (pod [`lib/user-interface`](lib/user-interface)) — nikoli
JavaFX — a otevírá skutečné nativní okno na X11 / Cocoa / Win32. Návrh se řídí
jediným pravidlem, **„SleelaUI zobrazuje; SLeeLa rozhoduje“**: GUI nikdy
nepočítá geometrii ani nezapisuje DXF. Každý nástroj zasílá záměr
vytvořit / změnit / smazat / vrátit / uložit autoritativnímu
[`SLCadCreator`](lib/autocad/SLCadCreator.sleela), který vlastní editovatelný
dokument a vratnou historii příkazů, zatímco plátno pouze zobrazuje náhled
geometrie, kterou Creator drží.

Znovupoužitelný editační engine žije v [`lib/autocad`](lib/autocad) —
[`SLCadEntity`](lib/autocad/SLCadEntity.sleela),
[`SLCadDocument`](lib/autocad/SLCadDocument.sleela),
[`SLCadCommand`](lib/autocad/SLCadCommand.sleela) a
[`SLCadCreator`](lib/autocad/SLCadCreator.sleela) — a běží celý bez displeje.
Grafický shell a ukázka jsou v [`autocad/creator`](autocad/creator):

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

Na displeji se otevře živé editační okno; na hostiteli bez displeje se výkres
sestaví v paměti, vypíše se zpráva spolu s DXF a program čistě skončí.


## XIII. Řazení vstupní mapy UTF-4088

Specializovaný katalog UTF-4088 definuje průchod svou prostorovou vstupní mapou a abstraktní iterace tlaku v [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). Zásady rejstříku subjektů National Lien jsou v [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Souřadnice mapy, úrovně tlaku (1–50), trvalé identifikátory katalogu a abecední pořadí jsou nezávislá pole; žádné z nich se nesmí potichu zaměňovat za jiné. Tlak je algoritmická iterační úroveň, nikoli fyzikální veličina.


## XIV. Nedávný a rozšířený rejstřík balíčků

Tento připojený rejstřík zdůrazňuje nedávno rozšířené balíčky, doménové
knihovny a sbírky zdrojů. Položky jsou seskupeny abecedně podle názvu balíčku
nebo tématu. Popisy určují umístění v repozitáři; stav implementace a validace
je třeba ověřit v dokumentaci a testech jednotlivých balíčků.

### A–C

- **Advantage** — `lib/advantage/` a `advantage/`: zvýhodnění jedinečné
  dividendy vzhledem k modelu jako vrstvená cibule Sequitur, s pěti
  tematickými podsložkami — `math/` (společná maximalizace zisku, Nashovo
  vyjednávání, bilaterální newsvendor), `physics/` (metrika/geodetika/kontakt/SDE
  polí důvěry, označená modelovací analogie), `integration/` (kompartmenty
  vertikální integrace a skóre sofistikovanosti), `grading/` (hodnotitel
  sofistikovanosti A–D pro moduly a množiny) a `output/` (emise na terminál /
  CommonRails / soket). Nativní konzument v C/C++ (`advantage/`) propojuje model
  s propracovanými obchodními příklady. Viz [`lib/advantage/README.md`](lib/advantage/README.md)
  a [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alphabet a UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` a
  `utf-4088/`: katalogy abeced, nativní podpora znakových sad, řazení
  vstupních map a dokumentace integrace UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` a `lib/autocad/`: interaktivní
  pracovní postupy 2D kreslení a znovupoužitelné komponenty CAD dokumentu/editace.
- **Ballum** — pixelové řízení displeje: nastavení/čtení jednotlivých pixelů,
  blit oblastí, jas/teplota/gamma celého displeje, vstup ukazatele/klávesnice
  a **povrch framebufferu na celou obrazovku** pro úplné, vlastněné, rychlé
  řízení po pixelech (okno override-redirect + jediné předání `XPutImage`/MIT-SHM
  na Linuxu/X11). Nativní jádro pod `ballum/` s fasádou SLeeLa v `lib/ballum/`
  a mostem VM pod `impl/`; viz [`ballum/README.md`](ballum/README.md) a
  spustitelný [`ballum/example/`](ballum/example/).
- **CDMA** — `cdma/` a `lib/cdma/`: technické materiály související s CDMA a
  práce na zdrojové knihovně SLeeLa.
- **Character** — `lib/character/`: profily postav a znovupoužitelné statistiky
  postav, včetně příkladů biografických profilů.
- **Coorenagraph** — `lib/coorenagraph/`: etické uvažování, morální slovník,
  deliberace a související pojmy; viz README balíčku a zdrojové soubory.

### D–G

- **Zdrojové modely správy (Governance)** — [lib/governance](lib/governance/README.md) definuje deklarativní zdrojové tvary SLeeLa pro návrhy, anonymní ruční hlasovací lístky a souhrnná sčítání. Navrhovaná anotace @vote a ruční proces jsou zdokumentovány v [registry/DEMOCRATIC-VOTING-SYNTAX.md](registry/DEMOCRATIC-VOTING-SYNTAX.md); současné překladače ji považují za netečná metadata v komentáři.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` až dostupné verzované adresáře balíčků
  HTTP: specifikace protokolu a implementace specifické pro verzi. Přesný
  rozsah a stav ověření viz dokumentace jednotlivých verzí.
- **International Criminal Court** — `lib/international-criminal-court/` a
  `INTERNATIONAL.CRIMINAL.COURTS.md`: související doménově specifický zdroj a
  dokumentace.

### O–S

- **OFDMA** — `ofdma/`: technické materiály o ortogonálním frekvenčním
  vícenásobném přístupu a vývojové zdroje v C/C++.
- **Philosophy a modul Conversation Mask** — [`philosophy/`](philosophy/README.md)
  obsahuje nativní základy v C/C++ a [`philosophy/module/`](philosophy/module/README.md)
  organizuje jejich další vrstvu modulu do tří výslovných oblastí:
  [`data/`](philosophy/module/data/README.md) pro typovaná, datovaná pozorování,
  konce událostí, nastavení poločasu, původ a souhlas; [`helper/`](philosophy/module/helper/README.md)
  pro deterministickou validaci, úbytek relevance, kompatibilní porovnání a
  prezentaci filtrovanou souhlasem; a [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md)
  pro podpůrné, neklinické konverzační rozhraní. Úchyty (handles) obrácené k
  SLeeLa a složený strom (Tree) žijí v [`lib/philosophy/`](lib/philosophy/README.md),
  vedle širšího balíčku philosophy. Modul pracuje s výslovnými vstupy a klade
  důraz na souhlas: pasivně nesleduje lidi ani média, nestanovuje diagnózy,
  nevyvozuje soukromé úmysly, neohodnocuje lidskou hodnotu a nedoporučuje změny
  medikace. Jeho odkazy na události uchovávají původ a navigaci, nikoli
  nezávislé potvrzení.
- **Synchro** — nativní integrace runtime/překladače a funkcionalita pro
  odesílání paketů a související s měřením; viz příslušný zdroj v `impl/` a
  implementace překladače.
- **SleelaUI / terminálové UI** — `lib/user-interface/` a projekt
  `sleela-terminal`: integrace UI SLeeLa a komponenty pro interakci orientované
  na terminál.

### U–W

- **United Nations** — `united-nations/`: doménově specifické materiály repozitáře.
- **Wi-Fi 4, Wi-Fi 5 a Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`,
  `wi-fi-6/` a související zdrojové adresáře `lib/wifi/`: materiály a
  konfigurační zdroje pro bezdrátové sítě podle generací.

### Údržba rejstříku balíčků

Když je balíček přidán nebo podstatně rozšířen, přidejte jeho položku sem do
příslušné abecední skupiny, odkažte na jeho kanonický adresář a README
a popište pouze schopnosti podporované vloženými zdroji. Udržujte tento
rejstřík přírůstkový: v rámci aktualizací rejstříku balíčků neodstraňujte ani
nepřepisujte existující oddíly README.


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

US Democratic and US Policy.

---

## XVI. Národní vědecké brandové obrazové materiály

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. Experimentální katalog UTF-4088 s milionem položek

[Generátor katalogu UTF-4088](utf-4088/tools/generate_catalogue.py) deterministicky sestavuje experimentální katalog symbolů s 1 048 576 položkami. Zdokumentovaný kompaktní formát je popsán v [utf-4088/FORMAT.md](utf-4088/FORMAT.md); smlouva o generování a validaci je v [utf-4088/GENERATION-RESULTS.md](utf-4088/GENERATION-RESULTS.md).

Generátor spusťte v Pythonu 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Vygenerovaný binární soubor má 16 777 244 bajtů a je navržen tak, aby zůstal pod 20 000 000 bajtů. Jednotkové testy a workflow GitHub Actions ověřují počet položek, jedinečnost glyfů, velikost a SHA-256; úspěšné běhy zveřejňují binární soubor a souhrn jako artefakt workflow `utf4088-million-catalogue`. Vygenerovaný binární soubor je záměrně udržován mimo historii Gitu. UTF-4088 zůstává experimentální a není kompatibilní s Unicode.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="Marketingové logo SLeeLa" width="560"></p>

---

## XVIII. Nedávné přírůstky — konstanta Pi, spouštění Modium/Registry, matematika sleelactl a `@native`

Tato část zaznamenává funkce přidané během prací 2026-11, nejnovější skupiny první. Autoritativní jsou odkazované zdrojové soubory a jejich stav validace; změny v revizi nejsou popisovány jako začleněné, dokud se nedostanou do `master`.

### Konstanta katalogu Pi se 100 000 000 číslic

Repozitář dodává ověřenou hodnotu π o **sto milionech číslic** v [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (dobře utvořené desetinné číslo, `3.14159…`) vedle hodnoty o 57 344 číslicích [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Obě jsou nezávisle ověřeny jako skutečné π — viz [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md), který dokumentuje kontroly (učebnicový prefix, Feynmanův bod na desetinné pozici 762, slavná miliontá číslice, shoda s `lib/math/SLPiReference.sleela` a analýza četnosti číslic v celém souboru) a jednorázový skript pro reprodukci.

### Modium™ / Registry™ načítají konstantu Pi při spuštění subsystému

Na vyhrazených vstupních bodech subsystémů — `sleela registry` a `sleela modium` (a při prvním spuštění instalátoru), **nikoli** při každém `sleela run` — subsystémy Modium a Registry načítají velkou konstantu katalogu π. Na interaktivním terminálu vypíší přechodný červený řádek `Loading Modium™` / `Loading Registry™`, který je poté na místě smazán, než začne běžný výstup; efekt je přísně podmíněn TTY, takže výstup do roury a testovací výstup zůstávají byte po bytu beze změny. Lokátor (nikoli bajty) je zaset jako výchozí hodnoty klíče `Math` registru `piReferenceLargePath` a `piReferenceLargeDigits`, a to jak v modelu SLeeLa ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)), tak v nativním seederu ([`registry/sleela_registry.c`](registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` zpřístupňuje `largePath()` / `largeDigits()`. Konstanta π se instaluje do umístění SLeeLa v OS (`<data>/pi/`) **jak** standardním instalátorem `make`, tak průvodcovským „beautiful“ instalátorem. Podrobnosti: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — beautiful installer a nativně podložený subjekt `math`

- Průvodcovský („beautiful“) instalátor nyní instaluje `sleelactl` (řídicí rovinu SLeeLa) a jeho registr modulů, shodně s instalátorem `make`. Viz [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- `sleelactl` získal novou povahu **`subject`** pro nativní knihovny subjektů v C/C++ zkompilované do runtime a registruje první z nich: **`math`**. `sleelactl run math` provede nativní backend `__NativeMath` (`impl/subjects/math/math_native.cpp`) od začátku do konce prostřednictvím referenčního dema subjektu; `sleelactl info math` vysvětluje, jak se k němu přistupuje (`import math;`). Povaha subject nabízí pouze `run info` — příkaz `check` záměrně neexistuje, protože `sleela check` nenačítá nativní subjekty registrované v SHEET.

### `@native`, Native Drive a vícejazyčné provádění

Nová metodová anotace **`@native`** žádá o předběžnou nativní kompilaci (AOT) horké metody. Nový příkaz `sleela aot` najde metody s `@native`, ověří, že každá spadá do podporované *offloadovatelné podmnožiny*, a vyemituje C/C++ — **selhává uzavřeně** (odmítne s přesným důvodem) u každé konstrukce, kterou zatím nedokáže přeložit, takže se nikdy nevytvoří napůl přeložená metoda.

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

`@native` zůstává žádostí o AOT zpracování, přičemž nepodporované konstrukce se odmítnou před emisí. [Native Drive](impl/native-drive/README.md) podporuje vícejednotková sestavení spustitelných souborů a sdílených knihoven v C/C++ pomocí hostitelských překladačů (včetně MSVC), podporovaný výstup SLeeLa AOT, inspekci symbolů vůči ABI nativního loaderu, spouštění procesů bez shellu, zpracování stdin/stdout/stderr, vícestupňové zřetězení rour a delegování XClass na produkční loader. `sleelactl` zpřístupňuje jeho příkazy sestavení/spuštění a směruje inspekci XClass a výslovné vykreslování/instalaci jednotek systemd přes `inspect native-drive`. VM vlastní omezenou tabulku úchytů nativních modulů s povinnou autorizací hostitele a úchyty kontrolovanými generací. Syntaxe 3.0 nyní přidává `nativeOpen`, `nativeCall` a `nativeClose`: bytecodové dispečování pouze s přidáváním překračuje samostatně verzované ABI null/bool/int64/double/UTF-8, zatímco pole, struktury a další složené hodnoty vlastněné VM zůstávají uvnitř interpretu a mohou používat bytecodový záložní postup se stejným názvem. Pluginy mohou volitelně obdržet verzovanou tabulku zpětných volání hostitele pro vyvolání přesných funkcí SLeeLa prostřednictvím vnořeného provádění VM, s rootováním pozastaveného zásobníku pro GC, výslovným seznamem povolených názvů zpětných volání hostitele a odmítáním rekurzivních volání nativního API. Nativní kód zůstává důvěryhodným kódem v témže procesu, nikoli sandboxem. Anotace zásad procesů ve zdroji mají verzované schéma, ale zatím se neparsují ani nevynucují. Podrobnosti, bezpečnostní hranice a evidenci sedmi pracovních proudů viz [Native Drive](impl/native-drive/README.md) a [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).


## XIX. Mezinárodní edice README

SLeeLa poskytuje vstupní body README specifické pro jednotlivé země pro 50 trhů své mezinárodní edice. Každá země má jeden soubor pojmenovaný `README.<CC>.md`; kde je relevantních více jazyků, jsou seskupeny v souboru dané země.

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

**Zásady překladu:** Tyto soubory jsou počátečními lokalizovanými národními vstupními body. Jejich úvody a jazykové nadpisy jsou lokalizovány, zatímco úplná technická reference zůstává v kanonickém anglickém [README.md](README.md). Soubor země nelze označit za úplný překlad, dokud není přeložen každý oddíl, nejsou zkontrolovány technické identifikátory a odkazy a edice neprošla jazykovou revizí. Jazykové pokrytí v tomto prvním průchodu je reprezentativní a není tvrzením, že byly plně přeloženy všechny ústavně uznané, regionální, původní či znakové jazyky.

## Dodatek — zachované regionální pokyny

Tento dodatek zachovává praktické pokyny z dřívější krátké české příručky, které v kanonickém README nemají vlastní oddíly. Při jakémkoli rozporu má přednost kanonické anglické [README.md](README.md).

### Cíle, profily a hranice spouštění

SLeeLa odděluje zdrojový kód od cílů sestavení a spuštění. Dokumentovaná rodina cílů zahrnuje `SLVM`, `SLJVM` a `SLIR`; `NATIVE` je omezený cíl podmíněný konkrétní podporou a nelze předpokládat jeho dostupnost na všech platformách. Profily `BASIC_COMPLETE` a `ADVANCED_TOTAL` označují rozdílný rozsah. U každého sestavení ověřte skutečný cíl, profil a podporu platformy.

Reference identifikuje soubor, modul nebo prostředek, ale sama o sobě neopravňuje k jeho spuštění. Pokud program používá soubory, externí knihovny, native bindings nebo systémové prostředky, musí být jasně určeny hranice oprávnění a odpovědnost za spuštění. Popis v dokumentaci sám o sobě nedokazuje bezpečnostní záruku runtime; ověřte implementaci a související testy.

### Životní cyklus prostředků a ladění

Dokumentujte vlastnictví, životnost a odpovědnost za synchronizaci prostředků sdílených mezi funkcemi nebo vlákny. Rozlišujte local variables, shared object state, VM handles a serializable data. VM-local handle není stabilní formát souboru ani síťová zpráva. Používejte serializační rozhraní definované modulem a testujte invalid, missing a boundary values.

Při ladění zaznamenejte commit, OS, architecture a toolchain. Nejprve sestavte nejmenší relevantní cíl, uchovejte úplný chybový výstup, rozlišujte compiler errors od runtime errors a vytvořte minimální reprodukovatelný příklad. Úspěšné sestavení samo o sobě nedokazuje správné chování runtime.

Validační zpráva má uvádět exact commit, OS/architecture, compiler versions, úplné build/test commands, počty pass/fail/skip a známá omezení. Přeskočené nebo nespouštěné testy se nesmějí počítat jako úspěšné.

### Smlouvy modulových API, kompatibilita a strategie testování

Při zavádění nového API zdokumentujte vstupy, výstupy, chybové podmínky, vlastnictví dat a případné side effects. Příklady mají ukazovat běžné použití i zpracování invalid nebo missing input. Chování neoznačujte za zaručené, pokud je nepotvrzuje implementation a související tests.

Testy rozdělte podle etap compiler pipeline: lexing/parsing, semantic analysis, IR lowering, code generation, VM execution a native integration. Pro každý regression case uchovejte nejmenší reprodukovatelný vstup, očekávaný výsledek a skutečný výsledek. Změny API musí popsat backward compatibility, migration steps a dopad na existující examples.

### Lokalizace, kódování a čitelnost

Technical identifiers, commands, paths, syntax a code blocks zachovávejte ve všech jazykových verzích. Překládejte vysvětlující text, nikoli tokeny nebo názvy používané ve source code. Jazykové a regionální označení uvádějte přesně; českou a slovenskou terminologii nezaměňujte. Po každé aktualizaci zkontrolujte Markdown links, hierarchii nadpisů, code fences a příklady. Při změně canonical English documents porovnejte příslušné části a aktualizujte celý překlad, nejen úvod. Obsah bez jazykové nebo technické kontroly musí být označen.

### Poznámky k odkazům a adresám

Odkazy v README na témata jako CSPML, Character, Governance, Synchro, SleelaUI a United Nations popisují obsah repozitáře a nepředstavují oficiální schválení vládou nebo institucí.

Přítomnost adres nebo obrázků v oddílu **Bitcoin Conjegeum** (XV) není finančním doporučením, ověřením vlastnictví ani pokynem k převodu prostředků. Před použitím je nezávisle ověřte.

**Stav překladu:** Oddíly I–XIX jsou přeloženy do češtiny. Technické identifikátory, příkazy, ukázky kódu, názvy souborů, odkazy a adresy jsou zachovány. Doporučuje se závěrečná jazyková revize rodilými mluvčími.
