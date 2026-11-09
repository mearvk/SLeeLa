# SLeeLa 3.0 — Magyarország (HU)

<p align="center"><strong>The Now. The Ingredient.</strong></p>

## Magyar nyelvű áttekintés

A SLeeLa natív rendszerprogramozási nyelv, saját fordítóval, futtatókörnyezettel és virtuálisgép-ökoszisztémával. A projekt nem csupán nyelvtant jelent: könyvtárakat, fejlesztőeszközöket, interfészeket, dokumentációt, buildfolyamatokat és a támogatott platformokhoz tartozó szerződéseket is magában foglal.

A 3.0-s nemzetközi kiadás a nyelvet, a runtime-ot, a fejlesztőeszközöket és a könyvtárakat egységes dokumentációban mutatja be. A technikai azonosítók, a forráskód, a parancsok, a fájlnevek és az API-nevek változatlanok maradnak, hogy a példák közvetlenül használhatók legyenek.

## 1. Első lépések

- [Kanonikus README és teljes műszaki referencia](README.md).
- [A natív implementáció build-útmutatója](impl/README.md).
- [Virtuális gép és Modium használati útmutató](virtual-machine/USER-GUIDE.md).
- [Dokumentációs index](docs/README.md).
- [Generált API-dokumentáció](docs/api/README.md) és [API-katalógus](api/README.md).
- [Platformtámogatás és ellenőrzési mátrix](docs/PLATFORM-SUPPORT.md).
- [Nyelvi szintaxisleírás](SLEELA.syntax).
- [Tesztelési szabályzat](markdown/TESTING.md).

A CPU-dokumentáció a logikai kapuktól és az ALU-tól a processzormagon, az operációs rendszeren és a programokon át mutatja be az architektúrát. Lásd: [CPU.md](lib/cpu/CPU.md) és [TIMING.md](lib/cpu/TIMING.md).

## 2. Fordítás és tesztelés

A szükséges eszközök a kiválasztott buildcéltól függenek. A natív implementációhoz rendszerint C/C++ fordító (például GCC vagy Clang) és GNU Make szükséges. Egyes katalógus- és munkafolyamat-eszközök Python 3-at használnak. Linuxon a grafikus összetevők X11- és XRandR-fejlesztőcsomagokat is igényelhetnek.

A repository gyökérkönyvtárából:

```sh
make
```

A natív implementáció fordítása és a teljes tesztcsomag futtatása:

```sh
make -C impl all
bash ./test-suites/run-all.sh --all
```

Gyakori Make-célok:

```sh
make all
make clean
make help
sudo make install
```

Mindig az adott cél saját útmutatóját kell követni. Egyetlen komponens sikeres fordítása nem bizonyítja, hogy minden könyvtár vagy minden platform kiadásra kész. A tesztjelentések különböztessék meg a lefuttatott, kihagyott és sikertelen ellenőrzéseket.

## 3. Telepítés

Az alapértelmezett rendszerszintű telepítési prefix általában `/usr/local`:

```sh
sudo make install
```

Felhasználói telepítéshez, rendszeradminisztrátori jogosultság nélkül:

```sh
make install PREFIX="$HOME/.local"
```

Csomaggyökér létrehozása és telepítésellenőrzés:

```sh
make install DESTDIR="$PWD/package-root"
make install-check
```

A tiszta telepítési cél a `make clean-install` paranccsal érhető el. A feltételeket, biztonsági határokat és további lehetőségeket a [telepítési útmutató](install/README.md) ismerteti. Ellenőrizze, hogy a PATH tartalmazza a kiválasztott telepítés `bin` könyvtárát.

## 4. A nyelv, a fordító és a futtatókörnyezet

A nyelv normatív szintaxisleírása a [SLEELA.syntax](SLEELA.syntax) fájlban található. A repository dokumentációja a 3.0-s szintaxist használja. A fordító szoftververziója és a forrásnyelv szintaxisverziója két külön fogalom; az egyes nyelvi funkciókhoz tartozó minimális verziókövetelmény továbbra is irányadó.

Az eszközkészlet része a `sleelvac` fordító és a `sleela` futtatókörnyezet. A dokumentáció többek között az `@mix` mezőelrendezést, a kiterjesztett egész típusokat, a Preonem dokumentumjelölőket, a SleelaDoc rendszert, az API-dokumentáció előállítását és a Nordshrift eszközöket tárgyalja.

A virtuális gépi és köztes célok között szerepel az SLVM, az SLJVM és az SLIR. A natív cél külön meghatározott interfészhatárokhoz kötött. Alapvető biztonsági szabály: **a hivatkozások azonosítanak; nem adnak felhatalmazást a végrehajtásra**. Egy objektum azonosítása, a belé vetett bizalom és a végrehajtás engedélyezése külön döntés.

## 5. Mérnöki alapelvek és ellenőrzés

A fejlesztés bizonyítékokra, világos komponenshatárokra, hordozhatóságra, biztonságos hibakezelésre és reprodukálható kiadásokra épít. Az interfészszerződéseknek rögzíteniük kell az elvárásokat és a hibafeltételeket. A dokumentáció nem ígérhet olyan képességet, amelyet a megvalósítás vagy a tesztek nem támasztanak alá.

Az ellenőrzést modulonként és platformonként kell értékelni. A le nem futtatott vizsgálat nem jelölhető sikeresnek. Egyetlen komponens sikere nem helyettesíti a teljes kiadás ellenőrzését. A SHA-256 ellenőrzések és integritási manifestek segítenek a változások észlelésében, de önmagukban nem bizonyítják a szoftver biztonságát.

A Sleelavirin védelmi eszközei közé tartozik a helyi SHA-256 ellenőrző, az opcionális ClamAV-integráció és az integritási manifest. Egy kiadás csak akkor nevezhető ellenőrzöttnek, ha az adott commit kötelező CI-vizsgálatai befejeződtek, és a létrejött artefaktumokat felülvizsgálták.

## 6. Szerkesztők és közreműködés

A SLeeLa Vim- és Emacs-témákat, valamint szintaxisintegrációt biztosít; natív C/C++ színpaletta-eszközök is rendelkezésre állnak. Részletek a [szerkesztő-integrációs útmutatóban](linux-editors/README.md).

A jó közreműködés világosan körülhatárolt módosítást, indoklást, pozitív és negatív teszteket, valamint a használt operációs rendszer és eszközlánc megadását tartalmazza. A kihagyott ellenőrzéseket kifejezetten jelezni kell.

## 7. Könyvtárak és projektterületek

A fő README ismerteti a csomagkatalógust, valamint az Advantage, a SleelaDoc és az XML DOM könyvtárakat. További dokumentált területek: egész számok alaptípusai, karakter-runtime, AutoCAD Creator™, a kísérleti UTF-4088 katalógus, a Pi-állandó, a Modium™, a Registry™ és a `sleelactl` eszköz.

Az egymillió bejegyzést tartalmazó UTF-4088 kísérleti katalógust és a kapcsolódó kísérleti adatokat a saját dokumentációjuk szerint kell értelmezni. Egy katalógus létezése önmagában nem jelenti azt, hogy minden bejegyzés éles üzemben használatos. A csomagjegyzék karbantartásakor meg kell őrizni az útvonalak, a felelősségi körök, a függőségek és a tesztállapot nyomon követhetőségét.

## 8. `@native` és a C/C++ irányú natív út

A dokumentáció az `@native` attribútumot és a `sleela aot` parancsot a SLeeLa-forráskódtól a natív C/C++ irányába vezető út első lépéseként írja le. Ez nem jelent korlátlan natív végrehajtási jogosultságot. Az interfész- és ABI-határok, a kifejezett jóváhagyás és a biztonsági kapuk továbbra is kötelező szempontok. A natív cél tényleges állapotát a megvalósítás és a tesztek alapján kell megítélni.

## 9. Kiadások és nemzetközi README-k

A hírek, kiadási megjegyzések és a nemzetközi README-fájlok jegyzéke a fő README-ben található. Az ország- vagy nyelvspecifikus dokumentum lokalizált belépési pont; nem tekinthető automatikusan valamennyi műszaki fejezet teljes fordításának.

## 10. Hivatkozások

- [Kanonikus README (angol)](README.md)
- [Szintaxisleírás](SLEELA.syntax)
- [Dokumentációs index](docs/README.md)
- [Platformtámogatás](docs/PLATFORM-SUPPORT.md)
- [Natív implementáció](impl/README.md)
- [Telepítési útmutató](install/README.md)
- [Tesztelési szabályzat](markdown/TESTING.md)
- [SLeeLa GitHub repository](https://github.com/mearvk/SLeeLa)

## Fordítási állapot

Ez a magyar kiadás kibővített műszaki útmutató: bemutatja a projektet, a build- és telepítési folyamatot, a nyelvet és a runtime-ot, az ellenőrzési elveket, a fejlesztőeszközöket és a fő projektterületeket. Még nem a kanonikus angol README teljes, sorról sorra készült fordítása. A teljes fordítás és szakmai ellenőrzés befejezéséig a [README.md](README.md) marad a normatív műszaki forrás.

A SLeeLa fejlesztője a MEARVK LLC.


## 11. A fordító felépítése és a végrehajtási folyamat

A SLeeLa külön rétegként kezeli a forrásnyelvet, a fordító frontendjét, a köztes reprezentációt, a kódgenerálást és a runtime végrehajtást. A fordító kiadása, a szintaxisverzió és a VM/ISA verzió külön verziótengelyek. Egy funkció specifikációban való szereplése nem bizonyítja, hogy minden célplatformon megvalósították és ellenőrizték.

A szokásos folyamat UTF-8 kódolású `.sleela` forrásfájlokat olvas be, lexikai és szintaktikai elemzést végez, ellenőrzi a szimbólumokat és típusokat, strukturált köztes reprezentációt alakít át, kódot állít elő a támogatott VM- vagy köztes célokhoz, majd a runtime-ban végrehajtja az eredményt. A normatív nyelvtan a [SLEELA.syntax](SLEELA.syntax); lásd a [source documentation](markdown/SOURCE.md) és a [compiler documentation](markdown/COMPILER.md) anyagát.

## 12. Típusok, objektummodell és könyvtárak

A dokumentált forrástípusok közé tartozik az `int`, `double`, `boolean`, `String`, `void`, a deklarált `struct` típusok és a támogatott reference-type signatures. Az `int`, `long` és `int64` előjeles, 64 bites egész számok. Az `int128` egész számtípusként elfogadott, de a jelenleg dokumentált implementáció továbbra is 64 bitre épül; nem szabad teljes 128 bites aritmetikát feltételezni.

A `struct` névvel ellátott összetett típus. A VM által kezelt struct handle nem nyers pointer; az értékadás és a paraméterátadás ugyanazt az instance-t osztja meg, az egyenlőség pedig identitásalapú. Hordozható szerializáláshoz a dokumentált `structPack(instance)` és `structUnpack(TypeName, json)` segédfüggvényeket kell használni. Az osztályok fields és methods elemeket tartalmaznak; az öröklés, a beágyazott osztályok és a generics szintaxisverzióhoz kötöttek.

A fontosabb modulok: `lib/advantage` (modellalapú értékelés, matematika, integráció, osztályozás és irányítható kimenet), `lib/xml` (XML DOM), `lib/text` (szöveg- és karakterlánc-műveletek), `lib/sleeladoc` (forrásdokumentáció és CLI), `lib/cpu`, `lib/xlt` (CPU-profilok és időzítési hivatkozások), valamint `recursion/stanchion` (C/C++ rekurziós prototípusok). Egy modul jelenléte nem bizonyítja a production readiness állapotot; ellenőrizni kell a teszteket és a platformkorlátozásokat.

## 13. Ciklusok időzítése, natív integráció és tesztelés

A `@loopDelay` és `@loopBudget` annotációk ciklusszünetet vagy teljes időkeretet határoznak meg. A VM `OP_SLEEP` utasítására (ISA 157) fordulnak le; a beépített `sleep(ms)` ugyanazt az utasítást használja. Lásd [SLEELA.syntax §32](SLEELA.syntax) és [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md).

A repository C/C++ komponenseket és korlátozott natív kódgenerálási útvonalat tartalmaz. Az `@native` és a `sleela aot` nem ad általános engedélyt tetszőleges natív kód futtatására. A C/C++ határoknak egyértelműen meg kell határozniuk az ABI-t, a hívási konvenciót, a memóriatulajdonlást, az adatelrendezést, a hibakezelést és a platformfüggőségeket.

Tipikus parancsok:

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
sudo make install
make install PREFIX="$HOME/.local"
```

A build rendszerint GCC-t vagy Clanget és GNU Make-et igényel; egyes eszközök Python 3-at használnak. Lásd az [installation guide](install/README.md), a [platform matrix](docs/PLATFORM-SUPPORT.md) és a [testing policy](markdown/TESTING.md) dokumentumokat. A tesztjelentéseknek külön kell kezelniük a pass, fail és skip állapotokat; a kihagyott teszt nem jelent sikert.

**Frissített fordítási állapot:** A magyar útmutató immár a fordítási folyamatot, a típus- és objektummodellt, a könyvtárakat, a ciklusok időzítését, a natív integrációt, a buildet, a telepítést és a tesztelést is lefedi. Ez még nem a teljes kanonikus angol README és az összes kapcsolódó specifikáció teljes, sorról sorra készült fordítása.


## 14. Célplatformok, profilok és a natív végrehajtás határai

A SLeeLa külön kezeli a forráskódot és a build/végrehajtási célokat. A dokumentált célcsalád része a `SLVM`, `SLJVM` és `SLIR`; a `NATIVE` korlátozott, feltételekhez kötött cél, ezért nem tekinthető minden platformon általánosan elérhetőnek. A `BASIC_COMPLETE` és `ADVANCED_TOTAL` profilok eltérő hatókört jelölnek. Minden buildnél ellenőrizd a tényleges célt, profilt és platformtámogatást.

A referencia fájlt, modult vagy erőforrást azonosít, de önmagában nem ad végrehajtási engedélyt. Ez különösen fontos fájlok, külső könyvtárak, native bindingok és rendszererőforrások használatakor. Ellenőrizd a bemeneteket, és határozd meg, melyik komponens jogosult az adott műveletre. A dokumentációban szereplő állítás önmagában nem bizonyít runtime-biztonsági garanciát; ehhez implementációs és tesztbizonyíték szükséges.

## 15. Erőforrás-életciklus és hibakeresés

Dokumentáld a függvények vagy szálak között megosztott erőforrások tulajdonjogát, élettartamát és szinkronizációját. Különböztesd meg a lokális változókat, a megosztott objektumállapotot, a VM-handle-öket és a szerializálható adatokat. A VM-local handle nem stabil fájlformátum vagy hálózati üzenet. Használd a modul dokumentált szerializációs felületét, és teszteld az érvénytelen, hiányzó és határértékű bemeneteket.

Hibakereséskor rögzítsd a commitot, az operációs rendszert, az architektúrát és a toolchaint. Először a legkisebb releváns targetet buildeld, őrizd meg a teljes hibaüzenetet, különítsd el a fordítóhibákat a runtime-hibáktól, és készíts minimális reprodukálható példát. A sikeres build önmagában nem bizonyítja a helyes futást.

A megfelelő ellenőrzési jelentés tartalmazza a pontos commitot, az OS/architektúra adatokat, a compiler verziókat, a teljes build- és tesztparancsokat, a pass/fail/skip darabszámokat és az ismert korlátozásokat. A kihagyott vagy le nem futtatott teszt nem számíthat sikeresnek.

## 16. Nemzetközi dokumentáció karbantartása

Az országonkénti README-k helyi belépési pontok a műszaki dokumentációhoz; nem helyettesítik a normatív nyelvi specifikációt vagy a modulok API-dokumentációját. Az angol forrás változásakor ellenőrizd a fordítás elavulását. Az azonosítókat, parancsokat, útvonalakat és kódpéldákat változatlanul kell hagyni; a nyelvi vagy műszaki ellenőrzésre váró részeket egyértelműen jelölni kell.

**Bővített fordítási állapot:** A magyar műszaki útmutató most már a célplatformokat és profilokat, a natív végrehajtás határait, az erőforrások tulajdonjogát, a hibakeresést, a tesztjelentéseket és a nemzetközi dokumentáció karbantartását is tárgyalja.


## 17. Modul-API-szerződések, kompatibilitás és tesztelés

Új API bevezetésekor dokumentálni kell a bemeneteket, kimeneteket, hibafeltételeket, az adatok tulajdonjogát és az esetleges side effects működést. A példáknak a szokásos használat mellett az invalid vagy missing input kezelését is be kell mutatniuk. Olyan viselkedést ne állítsunk garanciaként, amelyet az implementation és a tests nem igazol.

A teszteket a compiler pipeline rétegei szerint kell csoportosítani: lexing/parsing, semantic analysis, IR lowering, code generation, VM execution és native integration. Minden regression case esetén őrizzük meg a legkisebb reprodukálható bemenetet, az elvárt és a tényleges eredményt. Az API-változásoknál ismertetni kell a backward compatibility kérdéseit, a migration steps lépéseit és a meglévő examples elemekre gyakorolt hatást.

## 18. Lokalizáció, karakterkódolás és olvashatóság

A technical identifiers, commands, paths, syntax és code blocks minden nyelvi változatban maradjanak változatlanok. A magyarázó szöveget fordítsuk, ne a source code tokenjeit vagy neveit. A nyelvi és regionális környezetet egyértelműen jelöljük; a magyar terminológiát következetesen használjuk.

Minden frissítés után ellenőrizzük a Markdown links elemeket, a címsorok hierarchiáját, a code fences párokat és a példákat. Az angol canonical documents módosításakor szakaszonként hasonlítsuk össze és frissítsük a fordítást, ne csak a bevezetőt. A nyelvi vagy műszaki ellenőrzésen még át nem esett tartalmat jelöljük.
