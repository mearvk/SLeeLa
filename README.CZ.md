# SLeeLa 3.0 — Česká republika (CZ)

## Česká edice

SLeeLa je nativní systémový programovací jazyk s překladačem, běhovým prostředím a ekosystémem virtuálních strojů. Projekt zahrnuje také knihovny, vývojářské nástroje a smlouvy pro integraci s platformami. Technické identifikátory, příkazy, cesty, názvy souborů, ukázky kódu a názvy API zůstávají beze změny, aby bylo možné pokyny přímo použít.

**The Now. The Ingredient.** Jazyk není jen syntaxe. Patří k němu překladač, runtime, knihovny, nástroje a platformní smlouvy, které umožňují software sestavovat a provozovat.

SLeeLa popisuje své oblasti působnosti pomocí čtyř dimenzí: **Math**, **Object**, **Science** a **Reach**. Příslušné národní zájmy zůstávají výslovně zaznamenány v metadatech a nejsou tímto členěním potlačeny.

## I. Začínáme

- [Sestavení nativní implementace](impl/README.md).
- [Uživatelská příručka virtuálních strojů a Modium](virtual-machine/USER-GUIDE.md), včetně příkazů `sleela vm` a `sleela modium`.
- [Architektura CPU ve zdrojovém kódu SLeeLa](lib/cpu/CPU.md): od logických hradel a ALU přes jádra a operační systém až po programy; katalog obsahuje více než 130 profilů CPU. Viz také [časování, napětí a frekvence](lib/cpu/TIMING.md).
- [Rejstřík dokumentace](docs/README.md).
- [Zdroj dat CSPML a výzkumný referenční materiál Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md).
- [Generovaná dokumentace API](docs/api/README.md) a [katalog API](api/README.md).
- [Standardy výpočetního porovnání](comparison/README.md): typy, paměť, ABI a shoda mezi SLeeLa VM, Java/JVM a nativním C/C++.
- [Referenční prototypy C a C++ pro Recursion stanchion](recursion/stanchion/README.md).
- [Podpora platforem a validační matice](docs/PLATFORM-SUPPORT.md).
- [Průvodce značkou](docs/BRAND-GUIDE.md).
- [Zásady testování a podmínky vydání](markdown/TESTING.md).

## II. Funkce a novinky

Balíček standardní knihovny **Advantage** (`lib/advantage/`) modeluje analýzu dividendy vzhledem k modelu. Je uspořádán do vrstev zahrnujících review, concerns, maths, additions, subtraction, regrouping a iteration. Oblasti zahrnují ekonomiku bilaterálního obchodu, strukturální analogii polí důvěry ve fyzice (výslovně označenou jako modelovací analogie), vertikální integraci, hodnocení modulů a směrování výstupu. Viz [dokumentace Advantage](lib/advantage/README.md).

Mezi další zdokumentované funkce patří:

- **Časování smyček:** `@loopDelay` a `@loopBudget` vyjadřují prodlevy a časové rozpočty smyček; implementace používá `OP_SLEEP` a vestavěnou funkci `sleep(ms)`. Viz [specifikace §32](SLEELA.syntax) a [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md).
- **SleelaDoc:** dokumentační konvence ve zdrojovém kódu a nástroje pro generování stránek Markdown/HTML, rejstříků témat a dokumentace API. Příkaz `sleela docs` vypisuje témata a zobrazuje dokumentaci balíčků a tříd. Viz [SleelaDoc](lib/sleeladoc/README.md).
- **XML DOM:** strom XML uzlů včetně elementů, textu, komentářů, CDATA a instrukcí pro zpracování, s uspořádanými potomky a atributy, parserem a serializérem. Viz [XML DOM](lib/xml/README.md).
- **Rozšířené celočíselné typy:** dokumentace zahrnuje `long`, `int64`, `int128` a rodiny znaménkových i bezznaménkových celých čísel. Skutečnou šířku typu je nutné ověřit ve specifikaci; nelze ji odvodit pouze z názvu.
- **Navazující pole `@mix`:** opakovaná instanční pole se stejným názvem lze seskupit do indexované posloupnosti podle pravidel specifikace.
- **Dokumentační značky Preonem:** například `author`, `authorial`, `material`, `memory`, `determine`, `object` a `field`. Význam závisí na kontextové pozici a verzi syntaxe; mimo pozici značky zůstávají běžnými identifikátory.
- **Nordshrift:** vyhledává balíčky a zdrojové jednotky pomocí stejného rekurzivního indexu `/lib` jako překladač.
- **Komentáře v úhlových závorkách:** `< ... >` a `<< ... >>` jsou zdokumentované formy komentářů.
- **Číselné literály s různým základem:** syntaxe podporuje šestnáctkový, dvojkový a osmičkový zápis a oddělovače číslic.

Normativním zdrojem syntaxe je [`SLEELA.syntax`](SLEELA.syntax). Popis funkce v dokumentaci sám o sobě nedokazuje, že ji podporují všechny verze překladače.

## III. Sestavení a testování

**Požadavky:** nástroje C/C++ (GCC nebo Clang), GNU Make a Python 3 pro nástroje manifestů a workflow. Grafické komponenty na Linuxu mohou vyžadovat vývojové balíčky X11/Xrandr. Konkrétní závislosti uvádí dokumentace příslušného modulu.

V kořenovém adresáři repozitáře spusťte:

```sh
make
```

Pouze nativní jádro a úplná testovací sada:

```sh
make -C impl all
bash ./test-suites/run-all.sh --all
```

Běžné cíle Make:

```sh
make all
make registry
make tests
make ballum
make help
make clean
```

Kořenový cíl sestavuje produkty spravované repozitářem. Cíl `registry` sestaví jádro registru a vytvoří jeho výchozí referenční tabulku. Viz [příručka Registry](registry/README.md) a [architektura Registry 3.0](registry/REGISTRY-3.0-ARCHITECTURE.md).

Úspěšné sestavení jedné komponenty nedokazuje připravenost všech knihoven, platforem nebo konfigurací k vydání. Validaci je nutné posuzovat pro každý modul a platformu zvlášť. Přeskočené nebo neprovedené kontroly se nesmějí označovat jako úspěšné.

## IV. Instalace

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

Standardní systémová instalace obvykle používá `/usr/local/bin`, `/usr/local/lib/sleela` a `/usr/local/share/sleela`. Nastavení `PREFIX="$HOME/.local"` umožňuje instalaci pro uživatele, zpravidla bez `sudo`. `DESTDIR` připraví strom balíčku bez přímé instalace do systému.

Pro řízenou instalaci pouze hlavního balíčku — příkazů `sleela`, `sleelvac`, `nordshrift`, standardní knihovny, protokolů a podpůrných modulů — použijte:

```sh
make clean-install
```

Další podrobnosti uvádí [příručka rychlé a bezpečné instalace](install/README.md). Instalátor zachovává existující konfiguraci spouštění a nepřepisuje příkazy, které nespravuje. Odinstalace má odstranit pouze soubory evidované instalátorem.

## V. Jazyk a runtime

Dokumentace repozitáře odpovídá syntaxi **SLeeLa 3.0**. Verze překladače a verze syntaxe deklarovaná zdrojovým kódem jsou odlišné pojmy. Verze 3.0 sjednocuje jazykové prvky verzí 1.0 až 2.9 a zachovává minimální požadovanou verzi každé funkce.

SLeeLa zahrnuje nativní implementaci, překladač, runtime, virtuální stroje, dokumentační nástroje a knihovnu členěnou do balíčků. Vedle zdrojového kódu SLeeLa jsou v repozitáři i nativní komponenty C a C++. Hranice mezi nativním kódem a runtime musí dodržovat zdokumentované smlouvy ABI.

## VI. C, C++ a nativní překlad

Anotace `@native` a příkaz `sleela aot` představují ranou, konzervativní fázi předběžného překladu. Umožňují vypsat anotované metody, ověřit, zda patří do podporované podmnožiny, a pro podporované případy emitovat C/C++:

```sh
sleela aot --list <file.sleela>
sleela aot --emit <file.sleela>
```

Nepodporované konstrukce jsou odmítnuty s vysvětlením místo zavádějícího částečného překladu. Samotná anotace `@native` nemění chování programu. Dokončení tvorby sdílených knihoven nebo jejich volání z VM nelze tvrdit bez důkazů implementace a testů. Viz [AT-NATIVE](pi-generator/AT-NATIVE.md).

## VII. Knihovny a subsystémy

- **CPU** (`lib/cpu/`): modely procesorů, profily, vykonávání a časování.
- **Ballum** (`ballum/`, `lib/ballum/`): pixelové řízení displeje, oblasti, jas, gamma a vstup z klávesnice a ukazatele; dostupnost závisí na platformě a backendu. Viz [Ballum](ballum/README.md).
- **AutoCAD Creator™** (`autocad/creator/`): interaktivní 2D CAD aplikace v SLeeLa pro entity, vrstvy, kóty, poznámky, vracení změn a ukládání výkresů DXF. Geometrii a historii spravuje editační model, nikoli zobrazovací vrstva. Viz [AutoCAD Creator](autocad/creator/README.md).
- **Alphabet a UTF-4088:** katalog znaků a experimentální nástroje. UTF-4088 používá vlastní prostor identifikátorů a **není kompatibilní s Unicode**; jeho identifikátory nejsou skalární hodnoty Unicode.
- **Pi Generator:** katalog hodnot π a nástroje pro ověření a reprodukci.
- **Modium a Registry:** subsystémy registru a inspekční nástroje.
- **Philosophy:** nativní základy a datové, pomocné a konverzační moduly. Konverzační komponenta je popisována jako neklinická, založená na výslovných vstupech a souhlasu; neposkytuje diagnózy.
- **CDMA, OFDMA, HTTP a Wi-Fi:** technické oblasti a verzované knihovny. Rozsah a stav validace ověřte v dokumentaci každého balíčku.

Katalog balíčků je orientační; nenahrazuje zdrojový kód, README a testy konkrétního modulu.

## VIII. Integrita, bezpečnost a ověřování

Projekt dokumentuje manifesty SHA-256, místní kontroly integrity a volitelný adaptér ClamAV prostřednictvím Sleelavirin. Samotná existence nástroje nebo manifestu nepředstavuje bezpečnostní certifikaci.

Zásady vývoje:

1. Výslovné smlouvy mezi komponentami.
2. Přenositelnost zohledněná už při návrhu.
3. Bezpečné chování při selhání operace.
4. Reprodukovatelné důkazy spojené s testovaným commitem.
5. Rozlišení mezi identifikací odkazu a oprávněním k provedení.
6. Přesné vykazování neprovedených, přeskočených nebo blokovaných testů.

Při hodnocení vydání zkontrolujte CI pro konkrétní commit, vytvořené artefakty, příslušné testy a matici platforem. Výsledky z Linuxu nelze bez samostatných důkazů přenášet na Windows nebo macOS.

## IX. Editory a přispívání

SLeeLa poskytuje syntaktickou integraci a motivy pro **Vim** a **Emacs** a nativní nástroj barevných palet C/C++. Viz [průvodce integrací editorů](linux-editors/README.md).

Doporučený postup přispívání:

1. Přečtěte si [rejstřík dokumentace](docs/README.md) a [zásady testování](markdown/TESTING.md).
2. Provádějte úzce vymezené změny a přidávejte pozitivní i negativní testy.
3. Spusťte cílené testy a poté odpovídající širší sadu.
4. Uveďte operační systém, překladač a použitý toolchain.
5. Neoznačujte přeskočené kontroly za úspěšné.

SLeeLa vyvíjí **MEARVK LLC**.

## X. Další projektová témata

Hlavní README uvádí také zdroje dat CSPML a pokyny k integraci, výzkumné odkazy, návrhy merchandisingu a vizuální materiály a témata jako Character, Governance, Synchro, SleelaUI a United Nations. Tyto položky popisují obsah repozitáře a nepředstavují oficiální schválení vládou nebo institucí.

Repozitář obsahuje rovněž oddíl **Bitcoin Conjegeum** s adresami a obrazovými materiály. Jejich přítomnost není finančním doporučením, ověřením vlastnictví ani pokynem k převodu prostředků. Před použitím je nezávisle ověřte.

## XI. Dokumentace a nástroje API

Příkaz `sleela docs` vypisuje dokumentovatelná témata a zobrazuje stránky balíčků a tříd. Umí také vytvořit textový korpus pro offline použití. SleelaDoc podporuje značky jako `@intent`, `@thesis`, `@axiom`, `@param`, `@see` a `@mastery`. Viz [SleelaDoc](lib/sleeladoc/README.md) a [generovaná dokumentace API](sleeladoc-api/README.md).

## XII. AutoCAD Creator™

AutoCAD Creator™ je interaktivní 2D CAD aplikace napsaná v SLeeLa. Umožňuje vytvářet a měnit dokumenty, přidávat stěny, místnosti, kóty a poznámky, přesouvat popisky, měnit vrstvy, mazat prvky, vracet operace a ukládat textové výkresy DXF R12. Rozhraní používá SleelaUI™ a podle platformy může otevírat nativní okna přes X11, Cocoa nebo Win32.

Architektonické pravidlo zní: **SleelaUI zobrazuje; SLeeLa rozhoduje.** Model `SLCadCreator` vlastní geometrii dokumentu a vratnou historii úprav; uživatelské rozhraní není autoritou geometrických dat.

```sh
make creator
```

Příkaz spusťte z adresáře `autocad/`. V prostředí bez displeje může příklad vytvořit výkres v paměti, vypsat zprávu a skončit.

## XIII. Experimentální katalog UTF-4088

Generátor `utf-4088/tools/generate_catalogue.py` deterministicky vytváří experimentální katalog s **1 048 576** položkami. Dokumentovaný binární soubor má 16 777 244 bajtů. Testy a workflow GitHub Actions ověřují počet položek, jedinečnost glyfů, velikost a SHA-256. Binární soubor se zveřejňuje jako artefakt workflow a neukládá se do historie Git. UTF-4088 zůstává experimentální a není kompatibilní s Unicode.

## XIV. π, Modium, Registry a sleelactl

Repozitář uvádí desetinné katalogy π, včetně verze se **100 000 000** číslicemi a verze s 57 344 číslicemi. Dokument [VERIFIED](pi-generator/VERIFIED.md) popisuje kontroly a reprodukční postup.

Vstupní body `sleela registry` a `sleela modium` načítají rozsáhlý katalog π. Průvodce instalací instaluje `sleelactl` a registr modulů. Projekt popisuje `math` jako první subject nativní knihovny C/C++ integrované do runtime; konkrétní chování ověřte v dokumentaci a testech modulu.

## XV. Mezinárodní README

Národní edice používají názvy `README.<CC>.md`. Jeden soubor země může obsahovat více jazykových oddílů. Uvedení jazyka neznamená, že jsou přeloženy všechny regionální, původní, úřední nebo znakové jazyky.

**Zásady překladu:** Technické názvy, identifikátory, příkazy, cesty, odkazy a ukázky kódu se zachovávají. Edici lze označit za úplný překlad pouze tehdy, když jsou přeloženy všechny oddíly a prošly jazykovou i technickou revizí. Nepřeložený nebo nerevidovaný technický text se řídí anglickým normativním README.

## Hlavní reference

- [Normativní README](README.md)
- [Specifikace syntaxe SLeeLa](SLEELA.syntax)
- [Rejstřík dokumentace](docs/README.md)
- [Dokumentace API](docs/api/README.md)
- [Podpora platforem](docs/PLATFORM-SUPPORT.md)
- [Zásady testování](markdown/TESTING.md)
- [Repozitář GitHub](https://github.com/mearvk/SLeeLa)

**Stav překladu:** Tato česká edice nyní rozšiřuje úvod, začátky, sestavení, instalaci, jazyk, funkce, architekturu, knihovny, bezpečnost, přispívání a hlavní technická témata. Stále je nutná odborná jazyková revize a následné porovnávání s každou změnou anglického README.


## XVI. Cíle, profily a hranice spouštění

SLeeLa odděluje zdrojový kód od cílů sestavení a spuštění. Dokumentovaná rodina cílů zahrnuje `SLVM`, `SLJVM` a `SLIR`; `NATIVE` je omezený cíl podmíněný konkrétní podporou a nelze předpokládat jeho dostupnost na všech platformách. Profily `BASIC_COMPLETE` a `ADVANCED_TOTAL` označují rozdílný rozsah. U každého sestavení ověřte skutečný cíl, profil a podporu platformy.

Reference identifikuje soubor, modul nebo prostředek, ale sama o sobě neopravňuje k jeho spuštění. Pokud program používá soubory, externí knihovny, native bindings nebo systémové prostředky, musí být jasně určeny hranice oprávnění a odpovědnost za spuštění. Popis v dokumentaci sám o sobě nedokazuje bezpečnostní záruku runtime; ověřte implementaci a související testy.

## XVII. Životní cyklus prostředků a ladění

Dokumentujte vlastnictví, životnost a odpovědnost za synchronizaci prostředků sdílených mezi funkcemi nebo vlákny. Rozlišujte local variables, shared object state, VM handles a serializable data. VM-local handle není stabilní formát souboru ani síťová zpráva. Používejte serializační rozhraní definované modulem a testujte invalid, missing a boundary values.

Při ladění zaznamenejte commit, OS, architecture a toolchain. Nejprve sestavte nejmenší relevantní cíl, uchovejte úplný chybový výstup, rozlišujte compiler errors od runtime errors a vytvořte minimální reprodukovatelný příklad. Úspěšné sestavení samo o sobě nedokazuje správné chování runtime.

Validační zpráva má uvádět exact commit, OS/architecture, compiler versions, úplné build/test commands, počty pass/fail/skip a známá omezení. Přeskočené nebo nespouštěné testy se nesmějí počítat jako úspěšné.
