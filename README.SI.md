# SLeeLa 3.0 — Slovenija (SI)

## Slovenska izdaja

SLeeLa je sistemski programski jezik z lastnim prevajalnikom, izvajalnim okoljem in ekosistemom navideznih strojev. Projekt vključuje tudi knjižnice, razvojna orodja, dokumentacijo, postopke gradnje in integracijske pogodbe za podprte platforme. Tehnični identifikatorji, ukazi, poti, imena datotek, primeri kode in imena API ostanejo nespremenjeni, da je navodila mogoče neposredno uporabiti.

**The Now. The Ingredient.** Programski jezik je več kot sintaksa: sestavljajo ga prevajalnik, izvajalno okolje, knjižnice, orodja in platformne pogodbe, zaradi katerih je programsko opremo mogoče razvijati in uporabljati.

SLeeLa svoja delovna področja opisuje s pojmi **Math**, **Object**, **Science** in **Reach**. Nacionalni interesi, ki so pomembni za določeno področje, ostanejo izrecno zapisani v metapodatkih in jih ta razvrstitev ne izključuje.

## I. Začetek

- [Gradnja izvorne implementacije](impl/README.md).
- [Uporabniški priročnik za navidezne stroje in Modium](virtual-machine/USER-GUIDE.md), vključno z ukazoma `sleela vm` in `sleela modium`.
- [Arhitektura CPU v izvorni kodi SLeeLa](lib/cpu/CPU.md): od logičnih vrat in ALU do procesorskih jeder, operacijskega sistema in programov. Katalog vsebuje več kot 130 profilov CPU. Glejte tudi [časovne parametre, napetosti in frekvence](lib/cpu/TIMING.md).
- [Kazalo dokumentacije](docs/README.md).
- [Vir podatkov CSPML in raziskovalni referenčni dokument Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md).
- [Ustvarjena dokumentacija API](docs/api/README.md) in [katalog API](api/README.md).
- [Standardi računalniških primerjav](comparison/README.md): tipi, pomnilnik, ABI in skladnost med SLeeLa VM, Java/JVM ter izvornim C/C++.
- [Referenčni prototipi C in C++ za Recursion stanchion](recursion/stanchion/README.md).
- [Podpora platform in matrika preverjanja](docs/PLATFORM-SUPPORT.md).
- [Vodnik po blagovni znamki](docs/BRAND-GUIDE.md).
- [Politika testiranja in pogoji za izdajo](markdown/TESTING.md).

## II. Funkcije in novosti

Paket standardne knjižnice **Advantage** (`lib/advantage/`) modelira vrednotenje rezultata glede na model. Njegova večplastna struktura vključuje pregled, dejavnike, matematiko, dodatke, odštevanje, združevanje in iteracijo. Teme vključujejo dvostransko trgovinsko ekonomijo, strukturno fizikalno analogijo polj zaupanja (izrecno označeno kot modelna analogija), vertikalno integracijo, ocenjevanje modulov in usmerjen izhod. Glejte [dokumentacijo Advantage](lib/advantage/README.md).

Druge dokumentirane funkcije:

- **Časovno upravljanje zank:** anotaciji `@loopDelay` in `@loopBudget` določata premore in časovne proračune zank. Prevedeta se v append-only ukaz VM `OP_SLEEP` (ISA 157), na katerem temelji tudi splošna funkcija `sleep(ms)`. Glejte [specifikacijo sintakse](SLEELA.syntax) §32 in [referenco API](api/sleela/3.0/LOOP_TIMING_API.md).
- **SleelaDoc:** ukaz `sleela docs` prikaže dokumentacijo tem, paketov in razredov. `sleela docs --generate [dir]` ustvari lokalni besedilni korpus za uporabo brez povezave. Glejte [knjižnico SleelaDoc](lib/sleeladoc/README.md).
- **XML DOM:** `lib/xml` zagotavlja model dokumenta XML z vozlišči elementov, besedila, komentarjev, CDATA in procesnih navodil, atributi, serializacijo in razčlenjevanjem. Glejte [dokumentacijo XML](lib/xml/README.md).
- **Celoštevilski tipi:** `int`, `long` in `int64` so 64-bitna predznačena cela števila. `int128` je sprejet kot celoštevilski tip, vendar je za zdaj podprt na 64-bitni osnovi; to ni popolna 128-bitna implementacija.
- **Polja `@mix`:** ponovljena polja primerkov z istim imenom je mogoče združiti v indeksirano zaporedje, če je vsako polje označeno z `@mix`. Glejte sintakso 2.9 v [SLEELA.syntax](SLEELA.syntax).
- **Preonems:** označevalniki dokumentov `author`, `authorial`, `material;`, `memory;`, `determine;`, `object;` in `field;` opisujejo status in izvor dokumenta. Gre za deklarativne označevalnike, ne za dovoljenja za izvajanje kode.
- **Nordshrift:** orodje uporablja rekurzivni indeks `/lib` za preverjanje paketov in enot `.sleela`.
- **Komentarji v kotnih oklepajih:** obliki `< line >` in `<< block >>` sta dokumentirani sintaksi komentarjev.
- **Številski literali:** cela števila podpirajo šestnajstiško (`0x`), dvojiško (`0b`) in osmiško (`0o`) obliko z ločilom števk `_`.
- **Metadata Class:** izvorni moduli knjižnice, ki vsebujejo razrede, uporabljajo vzorec metapodatkov za izvor, zakonitost in jurisdikcijski obseg.
- **Text in String:** `lib/text` vključuje operacije nad besedilom in nizi na osnovi znakovnih celic, vključno z `charAt`, `substring`, `indexOf`, `reverse` in pozicijskim oblikovanjem.

Opis funkcije sam po sebi ne dokazuje, da je funkcija izvedena in preverjena za vse cilje ali platforme. Vedno preverite trenutno implementacijo in rezultate testov.

## III. Gradnja in testiranje

Zahteve so odvisne od izbranega cilja gradnje. Za izvorno implementacijo sta običajno potrebna orodna veriga C/C++, na primer GCC ali Clang, in GNU Make. Nekatera orodja za manifeste in delovne tokove uporabljajo Python 3. Grafične komponente Linux lahko zahtevajo razvojne pakete X11 in XRandR.

V korenskem imeniku repozitorija:

```sh
make
```

Gradnja izvornega jedra in zagon zbirke testov:

```sh
make -C impl all
bash ./test-suites/run-all.sh --all
```

Uporabni cilji Make:

```sh
make all
make clean
make help
make registry
make api-docs
```

Uspešna gradnja ene komponente ne dokazuje, da so vse knjižnice ali platforme pripravljene za izdajo. Poročila morajo ločeno navesti teste, ki so bili izvedeni, preskočeni ali neuspešni. Pred presojo pripravljenosti za izdajo preberite [politiko testiranja](markdown/TESTING.md).

## IV. Namestitev

Sistemska namestitev običajno uporablja predpono `/usr/local`:

```sh
sudo make install
```

Namestitev samo za trenutnega uporabnika, običajno brez skrbniških pravic:

```sh
make install PREFIX="$HOME/.local"
```

Ustvarjanje imenika paketa in preverjanje namestitve:

```sh
make install DESTDIR="$PWD/package-root"
make install-check
```

Upoštevajte navodila za izbrano platformo in cilj. Pri uporabniški namestitvi mora biti ustrezni imenik `bin` v spremenljivki `PATH`. Ne predpostavljajte, da ima isti ukaz enak učinek v Linuxu, macOS in Windows; preverite dokumentacijo platforme in razpoložljive namestitvene skripte.

- [Navodila za namestitev](install/README.md)
- [Podpora platform](docs/PLATFORM-SUPPORT.md)
- [Gradnja izvorne implementacije](impl/README.md)

## V. Jezik, prevajalnik in izvajalno okolje

Normativna specifikacija jezika je v datoteki [SLEELA.syntax](SLEELA.syntax). Dokumentacija SLeeLa 3.0 obravnava sintakso, prevajalnik, izvajalno okolje, navidezne stroje in knjižnice. Različica prevajalnika, različica sintakse in različica VM/ISA so ločeni pojmi in jih ne smemo enačiti.

Običajna veriga orodij vključuje:

1. **Izvorno kodo:** datoteke `.sleela` z različico jezika in deklaracijami.
2. **Frontend:** leksikalno analizo, razčlenjevanje, obdelavo simbolov in semantična preverjanja.
3. **Vmesno predstavitev:** strukturirano obliko za nadaljnje pretvorbe.
4. **Generiranje kode:** izhod za podprte cilje VM ali vmesne cilje.
5. **Izvajalno okolje:** izvajanje prevedene vsebine z mehanizmi za pomnilnik in nadzor poteka.
6. **Knjižnice in orodja:** ponovno uporabne pakete, dokumentacijo, gradnjo in preverjanje.

Natančne zmožnosti so odvisne od trenutne implementacije. Funkcija, navedena v specifikaciji, ni samodejno podprta na vsakem cilju.

## VI. C, C++ in izvorno prevajanje

Projekt SLeeLa vsebuje komponente C in C++ ter dokumentira omejeno izvorno pot. Oznaka `@native` in ukaz `sleela aot` opisujeta razvojno področje za generiranje izvorne kode. Ne pomenita splošnega dovoljenja za izvajanje poljubne izvorne kode.

Pri povezovanju SLeeLa s C ali C++ mora biti meja vmesnika izrecna. Pogodbe ABI, lastništvo pomnilnika, podatkovni tipi, obravnava napak in odvisnosti od platforme morajo biti dokumentirani in testirani. Izvorno kodo obravnavajte kot ločeno mejo zaupanja; sklic ali deklaracija sama po sebi ne dovoljuje izvajanja.

Glejte [izvorno implementacijo](impl/README.md), [matriko platform](docs/PLATFORM-SUPPORT.md) in [standarde primerjave](comparison/README.md).

## VII. Knjižnice in podsistemi

Repozitorij vsebuje obsežen nabor knjižnic in orodij. Glavne vstopne točke:

- `lib/advantage` — knjižnica Advantage ter definicije, matematika, fizika, integracija, ocenjevanje in izhod.
- `lib/xml` — model dokumenta XML in serializacija.
- `lib/text` — operacije z besedilom in nizi.
- `lib/sleeladoc` — ekstrakcija dokumentacije in podpora CLI.
- `lib/cpu` — modeli CPU, komponente in časovne reference.
- `lib/xlt` — format modelov CPU in pripadajoča dokumentacija.
- `recursion/stanchion` — referenčni prototipi in primeri C/C++.
- `virtual-machine` — priročniki VM in gradivo Modium.
- `api` in `docs/api` — katalog API in ustvarjena referenca.

Vnos v katalog ne pomeni nujno, da je modul pripravljen za produkcijsko uporabo. Pred uporabo preverite README modula, testno pokritost, omejitve platform in dokumentacijo preverjanja.

## VIII. Celovitost, varnost in preverjanje

SLeeLa poudarja sledljivost med izvorno kodo, dokumentacijo, gradnjo in rezultati testov. Zgoščevalni manifesti in orodja za preverjanje lahko potrdijo, da se datoteke ujemajo s pričakovano vsebino. Uspešno preverjanje velja samo za navedeni obseg in trenutek, ko je bilo izvedeno.

Ključna načela:

- Sklici identificirajo vire; sami po sebi ne dovoljujejo njihovega izvajanja.
- Izvorni vmesniki morajo biti izrecni in omejeni.
- Rezultati testov morajo ločeno prikazati pass, fail in skip.
- Trditve o podpori platform morajo biti povezane s konkretnimi gradnjami in testi.
- Ustvarjeno dokumentacijo mora biti mogoče povezati z izvorom in generatorjem.
- Varnostna preverjanja morajo ob manjkajočih zahtevanih podatkih odpovedati varno.

Glejte [politiko testiranja](markdown/TESTING.md), [podporo platform](docs/PLATFORM-SUPPORT.md) in [navodila za namestitev](install/README.md).

## IX. Urejevalniki in razvojni potek

Repozitorij vsebuje integracije in navodila za urejevalnike kode. Glejte [integracije urejevalnikov Linux](linux-editors/README.md), kjer so na voljo viri za Vim in Emacs. Pred namestitvijo preverite, ali navodila ustrezajo vašemu operacijskemu sistemu in različici urejevalnika.

Priporočen potek prispevanja:

1. Preberite kazalo dokumentacije in dokumentacijo modula, ki ga spreminjate.
2. Omejite obseg spremembe in pojasnite njen namen.
3. Dodajte pozitivne in negativne teste.
4. Zaženite ustrezne teste in po potrebi celotno zbirko.
5. Zabeležite operacijski sistem, orodno verigo, ukaze in dejanske rezultate.
6. Ob spremembi javnega vedenja posodobite README in dokumentacijo API.
7. Testov, ki so bili preskočeni ali niso bili izvedeni, ne označite kot uspešne.

## X. Dodatna projektna področja

Repozitorij SLeeLa vključuje tudi modele CPU in strojne opreme, grafične vmesnike, omrežne komponente, obdelavo besedila, dokumentacijo, internacionalizacijo in eksperimentalne kataloge. Ta področja imajo lahko različne stopnje zrelosti. Eksperimentalnega kataloga ali primera ne predstavljajte kot stabilne produkcijske funkcije brez dokumentiranega preverjanja.

Katalog UTF-4088, orodja za π, Modium, Registry in `sleelactl` imajo ločene vstopne točke in tehnične zahteve. Za aktualno stanje preverite kanonični README in dokumentacijo posameznih modulov.

## XI. API in referenčna dokumentacija

- [Ustvarjena dokumentacija API](docs/api/README.md)
- [Katalog API](api/README.md)
- [SleelaDoc](lib/sleeladoc/README.md)
- [Specifikacija sintakse](SLEELA.syntax)
- [Kazalo dokumentacije](docs/README.md)
- [Dokumentacija CPU](lib/cpu/CPU.md)
- [Časovne značilnosti CPU](lib/cpu/TIMING.md)

## XII. AutoCAD Creator™

AutoCAD Creator™ je naveden v tehničnem katalogu projekta. Preberite dokumentacijo ustreznega modula, da ločite opisane zmožnosti, primere in preverjene rezultate. Združljivosti z določeno različico AutoCAD ne predpostavljajte brez izrecne matrike združljivosti in rezultatov testov.

## XIII. Eksperimentalni katalog UTF-4088

Gradivo UTF-4088 je eksperimentalno področje znakov in katalogov v projektu. Velikost, število vnosov, zahteve glede pomnilnika in podrobnosti formata je treba obravnavati skupaj z dokumentacijo in trenutno implementacijo. Veliko število vnosov samo po sebi ne dokazuje popolne združljivosti z Unicode ali pripravljenosti za produkcijo.

## XIV. π, Modium, Registry in sleelactl

Projekt vsebuje ločena orodja in dokumentacijska področja za generiranje π, Modium, Registry in `sleelactl`. Njihovi nameni se razlikujejo, zato jih je treba graditi in preverjati po lastnih navodilih. Aktualne ukaze in omejitve preverite v kanoničnem README in ustreznih imenikih.

## XV. Mednarodne izdaje README

Državne izdaje so lokalizirane vstopne točke v dokumentacijo SLeeLa. Osrednje tehnične teme naj predstavijo v lokalnih jezikih, pri tem pa ohranijo tehnične identifikatorje, ukaze, imena datotek, kodo in povezave. Izdaje ni dovoljeno označiti kot v celoti prevedene, dokler niso prevedeni in preverjeni vsi ustrezni tehnični razdelki.

Ta slovenska izdaja je namenjena predvsem bralcem slovenščine. Ne trdi, da pokriva vse regionalne jezike, manjšinske jezike ali znakovne jezike v Sloveniji. Prispevki za dostopne oblike in dodatne jezikovne različice so dobrodošli.

## Glavne povezave

- [Kanonični angleški README](README.md)
- [Specifikacija sintakse SLeeLa](SLEELA.syntax)
- [Kazalo dokumentacije](docs/README.md)
- [Dokumentacija API](docs/api/README.md)
- [Podpora platform](docs/PLATFORM-SUPPORT.md)
- [Izvorna implementacija](impl/README.md)
- [Navodila za namestitev](install/README.md)
- [Politika testiranja](markdown/TESTING.md)
- [Repozitorij SLeeLa na GitHubu](https://github.com/mearvk/SLeeLa)

**Stanje prevoda:** Slovenska izdaja je razširjena v tehnični uvod, ki zajema začetek dela, gradnjo, namestitev, jezik in izvajalno okolje, izvorno integracijo, knjižnice, varnost, razvojni potek in glavna projektna področja. Še ne gre za vrstični prevod vseh podrobnosti kanoničnega angleškega README. Pred dokončno objavo sta priporočena tehnični pregled in jezikovna redakcija strokovnjaka za slovenščino.


## XVI. Cilji, profili in meje izvajanja

SLeeLa ločuje izvorno kodo od ciljev gradnje in izvajanja. Dokumentirana družina ciljev vključuje `SLVM`, `SLJVM` in `SLIR`; `NATIVE` je omejen cilj s pogoji in ni samoumevno na voljo na vseh platformah. Profila `BASIC_COMPLETE` in `ADVANCED_TOTAL` določata različen obseg. Pri vsaki gradnji preverite dejanski cilj, profil in podporo platforme.

Referenca identificira datoteko, modul ali vir, vendar sama po sebi ne dovoljuje njegovega izvajanja. Pri uporabi datotek, zunanjih knjižnic, native bindings ali sistemskih virov morajo biti meje pooblastil in odgovornost za izvajanje izrecno določene. Opis v dokumentaciji sam po sebi ni dokaz varnostnega jamstva runtime; preverite implementacijo in ustrezne teste.

## XVII. Življenjski cikel virov in odpravljanje napak

Dokumentirajte lastništvo, življenjsko dobo in odgovornosti za sinhronizacijo virov, ki si jih delijo funkcije ali niti. Ločujte local variables, shared object state, VM handles in serializable data. VM-local handle ni stabilna oblika datoteke ali omrežno sporočilo. Uporabite serializacijski vmesnik modula ter preizkusite invalid, missing in boundary values.

Pri odpravljanju napak zabeležite commit, OS, architecture in toolchain. Najprej zgradite najmanjši ustrezni cilj, shranite celoten izpis napak, ločite compiler errors od runtime errors in pripravite minimalen ponovljiv primer. Uspešna gradnja sama po sebi ne dokazuje pravilnega delovanja runtime.

Poročilo o preverjanju mora navesti exact commit, OS/architecture, compiler versions, vse build/test commands, število pass/fail/skip in znane omejitve. Preskočeni ali neizvedeni testi ne štejejo kot uspešni.
