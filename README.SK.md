# SLeeLa 3.0 — Slovensko (SK)

## Slovenské vydanie

SLeeLa je systémový programovací jazyk s vlastným kompilátorom, runtime prostredím a ekosystémom virtuálnych strojov. Projekt zahŕňa aj knižnice, vývojárske nástroje, dokumentáciu, procesy zostavovania a integračné zmluvy pre podporované platformy. Technické identifikátory, príkazy, cesty, názvy súborov, príklady kódu a názvy API zostávajú nezmenené, aby bolo možné návody priamo používať.

**The Now. The Ingredient.** Programovací jazyk je viac než syntax: tvorí ho kompilátor, runtime prostredie, knižnice, nástroje a platformové zmluvy, vďaka ktorým možno softvér vytvárať a používať.

SLeeLa opisuje svoje pracovné oblasti pojmami **Math**, **Object**, **Science** a **Reach**. Národné záujmy súvisiace s konkrétnou témou zostávajú výslovne zaznamenané v metadátach a táto klasifikácia ich neodstraňuje.

## I. Začíname

- [Zostavenie natívnej implementácie](impl/README.md).
- [Používateľská príručka virtuálnych strojov a Modium](virtual-machine/USER-GUIDE.md), vrátane príkazov `sleela vm` a `sleela modium`.
- [Architektúra CPU v zdrojovom kóde SLeeLa](lib/cpu/CPU.md): od logických hradiel a ALU až po procesorové jadrá, operačný systém a programy. Katalóg obsahuje viac než 130 profilov CPU. Pozrite si aj [časovanie, napätia a frekvencie](lib/cpu/TIMING.md).
- [Register dokumentácie](docs/README.md).
- [Zdroj údajov CSPML a výskumná referencia Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md).
- [Generovaná dokumentácia API](docs/api/README.md) a [katalóg API](api/README.md).
- [Štandardy výpočtového porovnávania](comparison/README.md): typy, pamäť, ABI a zhoda medzi SLeeLa VM, Java/JVM a natívnym C/C++.
- [Referenčné prototypy C a C++ pre Recursion stanchion](recursion/stanchion/README.md).
- [Podpora platforiem a validačná matica](docs/PLATFORM-SUPPORT.md).
- [Príručka značky](docs/BRAND-GUIDE.md).
- [Zásady testovania a podmienky vydania](markdown/TESTING.md).

## II. Funkcie a novinky

Balík štandardnej knižnice **Advantage** (`lib/advantage/`) modeluje vyhodnotenie výsledku vzhľadom na model. Jeho vrstvená štruktúra zahŕňa kontrolu, faktory, matematiku, prírastky, odčítanie, preskupovanie a iteráciu. Témy zahŕňajú bilaterálnu obchodnú ekonomiku, štrukturálnu fyzikálnu analógiu polí dôvery (výslovne označenú ako modelová analógia), vertikálnu integráciu, hodnotenie modulov a smerovaný výstup. Pozrite si [dokumentáciu Advantage](lib/advantage/README.md).

Ďalšie zdokumentované funkcie:

- **Časovanie slučiek:** anotácie `@loopDelay` a `@loopBudget` určujú prestávky a časové rozpočty slučiek. Prekladajú sa do append-only inštrukcie VM `OP_SLEEP` (ISA 157), ktorá podporuje aj všeobecnú funkciu `sleep(ms)`. Pozrite si [špecifikáciu syntaxe](SLEELA.syntax) §32 a [referenciu API](api/sleela/3.0/LOOP_TIMING_API.md).
- **SleelaDoc:** príkaz `sleela docs` zobrazuje dokumentáciu tém, balíkov a tried. Príkaz `sleela docs --generate [dir]` vytvorí lokálny textový korpus na použitie offline. Pozrite si [knižnicu SleelaDoc](lib/sleeladoc/README.md).
- **XML DOM:** `lib/xml` poskytuje model XML dokumentu s uzlami prvkov, textu, komentárov, CDATA a spracovateľských inštrukcií, atribútmi, serializáciou a parsovaním. Pozrite si [dokumentáciu XML](lib/xml/README.md).
- **Celočíselné typy:** `int`, `long` a `int64` sú 64-bitové znamienkové celé čísla. `int128` sa prijíma ako celočíselný typ, ale zatiaľ je založený na 64-bitovej reprezentácii; nejde o úplnú 128-bitovú implementáciu.
- **Polia `@mix`:** opakované inštančné polia s rovnakým názvom možno zoskupiť do indexovanej sekvencie, ak je každé pole označené `@mix`. Pozrite si syntax 2.9 v [SLEELA.syntax](SLEELA.syntax).
- **Preonems:** značky dokumentu `author`, `authorial`, `material;`, `memory;`, `determine;`, `object;` a `field;` opisujú stav a pôvod dokumentu. Sú to deklaratívne značky, nie oprávnenia na vykonávanie kódu.
- **Nordshrift:** nástroj používa rekurzívny index `/lib` na kontrolu balíkov a jednotiek `.sleela`.
- **Komentáre v uhlových zátvorkách:** formy `< line >` a `<< block >>` sú zdokumentované syntaxe komentárov.
- **Číselné literály:** celé čísla podporujú šestnástkový (`0x`), binárny (`0b`) a osmičkový (`0o`) zápis s oddeľovačom číslic `_`.
- **Metadata Class:** zdrojové moduly knižnice obsahujúce triedy používajú vzor metadát pre pôvod, zákonnosť a jurisdikčný rozsah.
- **Text a String:** `lib/text` zahŕňa operácie s textom a reťazcami založené na znakových bunkách vrátane `charAt`, `substring`, `indexOf`, `reverse` a pozičného formátovania.

Opis funkcie sám osebe nedokazuje, že je implementovaná a overená pre všetky ciele alebo platformy. Vždy skontrolujte aktuálnu implementáciu a výsledky testov.

## III. Zostavenie a testovanie

Požiadavky závisia od zvoleného cieľa zostavenia. Pre natívnu implementáciu je zvyčajne potrebný nástrojový reťazec C/C++, napríklad GCC alebo Clang, a GNU Make. Niektoré nástroje manifestov a pracovných postupov používajú Python 3. Grafické komponenty Linuxu môžu vyžadovať vývojové balíky X11 a XRandR.

Z koreňového adresára repozitára:

```sh
make
```

Zostavenie natívneho jadra a spustenie testovacej sady:

```sh
make -C impl all
bash ./test-suites/run-all.sh --all
```

Užitočné ciele Make:

```sh
make all
make clean
make help
make registry
make api-docs
```

Úspešné zostavenie jednej súčasti nedokazuje pripravenosť všetkých knižníc alebo platforiem na vydanie. Výsledky musia samostatne uvádzať testy, ktoré boli spustené, preskočené alebo zlyhali. Pred posúdením pripravenosti na vydanie si prečítajte [zásady testovania](markdown/TESTING.md).

## IV. Inštalácia

Systémová inštalácia zvyčajne používa prefix `/usr/local`:

```sh
sudo make install
```

Inštalácia iba pre aktuálneho používateľa, spravidla bez oprávnení správcu:

```sh
make install PREFIX="$HOME/.local"
```

Vytvorenie koreňového adresára balíka a kontrola inštalácie:

```sh
make install DESTDIR="$PWD/package-root"
make install-check
```

Dodržiavajte pokyny pre zvolenú platformu a cieľ. Pri používateľskej inštalácii musí byť príslušný adresár `bin` dostupný v premennej `PATH`. Nepredpokladajte, že rovnaký inštalačný príkaz má rovnaký účinok v Linuxe, macOS a Windows; overte si dokumentáciu platformy a dostupné inštalačné skripty.

- [Inštalačná príručka](install/README.md)
- [Podpora platforiem](docs/PLATFORM-SUPPORT.md)
- [Zostavenie natívnej implementácie](impl/README.md)

## V. Jazyk, kompilátor a runtime prostredie

Normatívna špecifikácia jazyka sa nachádza v súbore [SLEELA.syntax](SLEELA.syntax). Dokumentácia SLeeLa 3.0 pokrýva syntax, kompilátor, runtime prostredie, virtuálne stroje a knižnice. Verzia kompilátora, verzia syntaxe a verzia VM/ISA sú samostatné pojmy a nesmú sa zamieňať.

Typický reťazec nástrojov zahŕňa:

1. **Zdrojový kód:** súbory `.sleela` s verziou jazyka a deklaráciami.
2. **Frontend:** lexikálnu analýzu, parsovanie, spracovanie symbolov a sémantické kontroly.
3. **Medzireprezentáciu:** štruktúrovanú podobu používanú pri ďalšom znižovaní.
4. **Generovanie kódu:** výstup pre podporované ciele VM alebo medziformáty.
5. **Runtime prostredie:** vykonávanie skompilovaného obsahu s mechanizmami správy pamäte a riadenia toku.
6. **Knižnice a nástroje:** opakovane použiteľné balíky, dokumentáciu, zostavovanie a overovanie.

Presné možnosti závisia od aktuálnej implementácie. Funkcia uvedená v špecifikácii automaticky neznamená, že je podporovaná na každom cieli.

## VI. C, C++ a natívna kompilácia

Projekt SLeeLa obsahuje komponenty C a C++ a dokumentuje ohraničenú natívnu cestu. Značka `@native` a príkaz `sleela aot` opisujú vývojovú oblasť natívneho generovania. Nepredstavujú všeobecné povolenie na vykonávanie ľubovoľného natívneho kódu.

Pri prepájaní SLeeLa s C alebo C++ musí byť hranica rozhrania výslovná. Zmluvy ABI, vlastníctvo pamäte, dátové typy, spracovanie chýb a závislosti od platformy musia byť zdokumentované a testované. Natívny kód považujte za samostatnú hranicu dôvery; odkaz alebo deklarácia samy osebe neoprávňujú na vykonanie.

Pozrite si [natívnu implementáciu](impl/README.md), [maticu platforiem](docs/PLATFORM-SUPPORT.md) a [štandardy porovnávania](comparison/README.md).

## VII. Knižnice a podsystémy

Repozitár obsahuje rozsiahly ekosystém knižníc a nástrojov. Hlavné vstupné body:

- `lib/advantage` — knižnica Advantage a súvisiace definície, matematika, fyzika, integrácia, hodnotenie a výstup.
- `lib/xml` — model XML dokumentu a serializácia.
- `lib/text` — operácie s textom a reťazcami.
- `lib/sleeladoc` — extrakcia dokumentácie a podpora CLI.
- `lib/cpu` — modely CPU, komponenty a časové referencie.
- `lib/xlt` — formát modelov CPU a súvisiaca dokumentácia.
- `recursion/stanchion` — referenčné prototypy a príklady C/C++.
- `virtual-machine` — príručky VM a materiály Modium.
- `api` a `docs/api` — katalóg API a generovaná referencia.

Záznam v katalógu neznamená, že je modul pripravený na produkčné použitie. Pred použitím skontrolujte README modulu, pokrytie testami, obmedzenia platforiem a validačnú dokumentáciu.

## VIII. Integrita, bezpečnosť a overovanie

SLeeLa kladie dôraz na sledovateľnosť medzi zdrojovým kódom, dokumentáciou, zostavením a výsledkami testov. Hash manifesty a overovacie nástroje môžu potvrdiť, že súbory zodpovedajú očakávanému obsahu. Úspešná kontrola sa vzťahuje iba na uvedený rozsah a okamih, keď bola vykonaná.

Základné princípy:

- Odkazy identifikujú zdroje; samy osebe nepovoľujú ich vykonanie.
- Natívne rozhrania musia byť výslovné a obmedzené.
- Výsledky testov musia samostatne uvádzať pass, fail a skip.
- Tvrdenia o podpore platforiem musia byť podložené konkrétnymi zostaveniami a testami.
- Generovaná dokumentácia musí byť spätne prepojiteľná so zdrojom a generátorom.
- Bezpečnostné kontroly musia pri chýbajúcich požadovaných údajoch zlyhať bezpečným spôsobom.

Pozrite si [zásady testovania](markdown/TESTING.md), [podporu platforiem](docs/PLATFORM-SUPPORT.md) a [inštalačnú príručku](install/README.md).

## IX. Editory a vývojový proces

Repozitár obsahuje integrácie a návody pre editory kódu. Pozrite si [integráciu editorov Linux](linux-editors/README.md), ktorá poskytuje zdroje pre Vim a Emacs. Pred inštaláciou overte, či pokyny zodpovedajú vášmu operačnému systému a verzii editora.

Odporúčaný postup prispievania:

1. Prečítajte si register dokumentácie a dokumentáciu upravovaného modulu.
2. Obmedzte rozsah zmeny a vysvetlite jej dôvod.
3. Pridajte pozitívne aj negatívne testy.
4. Spustite relevantné testy a podľa potreby celú testovaciu sadu.
5. Zaznamenajte operačný systém, nástrojový reťazec, príkazy a skutočné výsledky.
6. Pri zmene verejného správania aktualizujte README a dokumentáciu API.
7. Testy, ktoré boli preskočené alebo sa nespustili, neoznačujte ako úspešné.

## X. Ďalšie oblasti projektu

Repozitár SLeeLa zahŕňa aj modely CPU a hardvéru, grafické rozhrania, sieťové komponenty, spracovanie textu, dokumentáciu, internacionalizáciu a experimentálne katalógy. Tieto oblasti môžu mať rôznu úroveň vyspelosti. Experimentálny katalóg alebo príklad neoznačujte za stabilnú produkčnú funkciu bez zdokumentovaného overenia.

Katalóg UTF-4088, nástroje π, Modium, Registry a `sleelactl` majú samostatné vstupné body a technické požiadavky. Aktuálny stav si overte v kanonickom README a v dokumentácii príslušných modulov.

## XI. API a referenčná dokumentácia

- [Generovaná dokumentácia API](docs/api/README.md)
- [Katalóg API](api/README.md)
- [SleelaDoc](lib/sleeladoc/README.md)
- [Špecifikácia syntaxe](SLEELA.syntax)
- [Register dokumentácie](docs/README.md)
- [Dokumentácia CPU](lib/cpu/CPU.md)
- [Časovanie CPU](lib/cpu/TIMING.md)

## XII. AutoCAD Creator™

AutoCAD Creator™ je uvedený v technickom katalógu projektu. Prečítajte si dokumentáciu príslušného modulu, aby ste rozlíšili opísané možnosti, príklady a overené výsledky. Kompatibilitu s konkrétnou verziou AutoCAD nepredpokladajte bez výslovnej matice kompatibility a výsledkov testov.

## XIII. Experimentálny katalóg UTF-4088

Materiály UTF-4088 patria do experimentálnej oblasti znakov a katalógov projektu. Veľkosť, počet záznamov, požiadavky na pamäť a podrobnosti formátu treba posudzovať spolu s dokumentáciou a aktuálnou implementáciou. Veľký počet záznamov sám osebe nedokazuje úplnú kompatibilitu s Unicode ani pripravenosť na produkciu.

## XIV. π, Modium, Registry a sleelactl

Projekt obsahuje samostatné nástroje a dokumentačné oblasti pre generovanie π, Modium, Registry a `sleelactl`. Majú odlišné účely a treba ich zostavovať a overovať podľa vlastných pokynov. Aktuálne príkazy a obmedzenia nájdete v kanonickom README a v príslušných adresároch.

## XV. Medzinárodné vydania README

Krajinské vydania sú lokalizované vstupné body do dokumentácie SLeeLa. Majú sprístupniť hlavné technické témy v miestnych jazykoch a pritom zachovať technické identifikátory, príkazy, názvy súborov, kód a odkazy. Vydanie nemožno označiť za úplne preložené, kým nie sú preložené a skontrolované všetky príslušné technické časti.

Toto slovenské vydanie je určené predovšetkým čitateľom slovenčiny. Netvrdí, že pokrýva všetky regionálne, menšinové alebo posunkové jazyky používané na Slovensku. Príspevky pre ďalšie jazykové formy a prístupné formáty sú vítané.

## Hlavné odkazy

- [Kanonický anglický README](README.md)
- [Špecifikácia syntaxe SLeeLa](SLEELA.syntax)
- [Register dokumentácie](docs/README.md)
- [Dokumentácia API](docs/api/README.md)
- [Podpora platforiem](docs/PLATFORM-SUPPORT.md)
- [Natívna implementácia](impl/README.md)
- [Inštalačná príručka](install/README.md)
- [Zásady testovania](markdown/TESTING.md)
- [Repozitár SLeeLa na GitHub](https://github.com/mearvk/SLeeLa)

**Stav prekladu:** Slovenské vydanie bolo rozšírené na technický úvod pokrývajúci začiatok práce, zostavenie, inštaláciu, jazyk a runtime prostredie, natívnu integráciu, knižnice, bezpečnosť, vývojový proces a hlavné oblasti projektu. Zatiaľ nejde o riadkový preklad všetkých podrobností kanonického anglického README. Pred označením vydania za konečné sa odporúča technická kontrola a jazyková redakcia odborníkom so znalosťou slovenčiny.


## XVI. Ciele, profily a hranice vykonávania

SLeeLa oddeľuje zdrojový kód od cieľov zostavenia a vykonávania. Dokumentovaná rodina cieľov zahŕňa `SLVM`, `SLJVM` a `SLIR`; `NATIVE` je obmedzený cieľ podmienený podporou, preto nemožno predpokladať jeho dostupnosť na každej platforme. Profily `BASIC_COMPLETE` a `ADVANCED_TOTAL` označujú rozdielny rozsah. Pri každom zostavení overte skutočný cieľ, profil a podporu platformy.

Referencia identifikuje súbor, modul alebo zdroj, ale sama osebe neposkytuje oprávnenie na jeho vykonanie. Pri používaní súborov, externých knižníc, native bindings alebo systémových zdrojov musia byť jasne určené hranice oprávnení a zodpovednosť za vykonanie. Opis v dokumentácii sám osebe nedokazuje bezpečnostnú záruku runtime; overte implementáciu a príslušné testy.

## XVII. Životný cyklus zdrojov a ladenie

Zdokumentujte vlastníctvo, životnosť a zodpovednosť za synchronizáciu zdrojov zdieľaných medzi funkciami alebo vláknami. Rozlišujte local variables, shared object state, VM handles a serializable data. VM-local handle nie je stabilný formát súboru ani sieťová správa. Používajte serializačné rozhranie definované modulom a testujte invalid, missing a boundary values.

Pri ladení zaznamenajte commit, OS, architecture a toolchain. Najprv zostavte najmenší relevantný cieľ, uchovajte úplný chybový výstup, rozlišujte compiler errors od runtime errors a vytvorte minimálny reprodukovateľný príklad. Úspešné zostavenie samo osebe nedokazuje správne správanie runtime.

Validačná správa má uvádzať exact commit, OS/architecture, compiler versions, úplné build/test commands, počty pass/fail/skip a známe obmedzenia. Preskočené alebo nespustené testy sa nesmú počítať ako úspešné.


## XVIII. Zmluvy modulových API, kompatibilita a testovacia stratégia

Pri zavádzaní nového API zdokumentujte vstupy, výstupy, chybové podmienky, vlastníctvo údajov a prípadné side effects. Príklady majú ukazovať bežné použitie aj spracovanie invalid alebo missing input. Správanie neoznačujte za zaručené, pokiaľ ho nepotvrdzuje implementation a príslušné tests.

Testy rozdeľte podľa etáp compiler pipeline: lexing/parsing, semantic analysis, IR lowering, code generation, VM execution a native integration. Pre každý regression case uchovajte najmenší reprodukovateľný vstup, očakávaný výsledok a skutočný výsledok. Zmeny API musia opísať backward compatibility, migration steps a vplyv na existujúce examples.

## XIX. Lokalizácia, kódovanie a čitateľnosť

Technical identifiers, commands, paths, syntax a code blocks zachovávajte vo všetkých jazykových verziách. Prekladajte vysvetľujúci text, nie tokeny alebo názvy používané v source code. Jazykové a regionálne označenie uvádzajte presne; slovenskú terminológiu nezamieňajte s českou.

Po každej aktualizácii skontrolujte Markdown links, hierarchiu nadpisov, code fences a príklady. Pri zmene canonical English documents porovnajte príslušné časti a aktualizujte celý preklad, nielen úvod. Obsah bez jazykovej alebo technickej kontroly musí byť označený.
