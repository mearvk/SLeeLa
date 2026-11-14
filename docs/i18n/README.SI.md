<img align="right" src="../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — Slovenija (SI)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Izvorni sistemski jezik, prevajalnik, izvajalno okolje in ekosistem navideznih strojev.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="../../docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Mednarodna državna izdaja.** Ta izdaja je v slovenščini. Tehnični identifikatorji, ukazi, primeri kode, imena datotek, poti in povezave so nespremenjeni glede na kanonični angleški [README.md](../../README.md).

**The Now. The Ingredient.** Jezik je več kot sintaksa: to so prevajalnik, izvajalno okolje, knjižnice, orodja in platformne pogodbe, zaradi katerih je programska oprema uporabna.

**S čim se SLeeLa ukvarja.** SLeeLa se ukvarja z **Math**, **Object**,
**Science** in **Reach** — štirimi obrazi svojega dela: izračunljivim
(Math), uveljavljeno stvarjo, ki stoji v katalogu obstoječega (Object),
disciplinirano preučitvijo resničnega (Science) in razširitvijo roke sistema
k drugim sistemom in ljudem (Reach). **Nacionalni interesi ostajajo** — nobeno
od teh štirih jih ne odstrani; kjer se tema dotika interesa naroda, se ta
položaj ohrani (zvesto zabeležen, npr. v poljih `metaOrigin` /
`metaNationalSchedule` metapodatkov vsakega razreda), nikoli pa ni raztopljen.

## I. Začnite tukaj
- [Gradnja izvorne implementacije](../../impl/README.md)
- [Navidezni stroji in Modium — praktični uporabniški priročnik](../../virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [Sklad CPU v izvorni kodi SLeeLa](../../lib/cpu/CPU.md) — popoln stroj od logičnih vrat prek ALU, jeder in OS do programov, seznam več kot 130 profilov CPU ter nastavljive [napetosti, spremenljive takte in strokovne časovne parametre](../../lib/cpu/TIMING.md), ki jih lahko [zaženete po specifikaciji ali s prekomernim taktiranjem](../../lib/cpu/components/README.md)
- [Kazalo dokumentacije](../../docs/README.md)
- [Vir podatkov CSPML in raziskovalni referenčni dokument Salt & Salary](../reference/CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — izvor podatkov in navodila za integracijo zunanjih ameriških podatkov o krajih, družbenoekonomskih razmerah, delovni sili, izobraževanju in javnih šolah.
- [Ustvarjena dokumentacija API](../../docs/api/README.md) — trenutna referenca HTML, verzionirana izdaja API in navodila za ponovno ustvarjanje
- [Katalog API](../../api/README.md)
- [Standardi računalniških primerjav](../../comparison/README.md) — standardi tipov, pomnilnika, ABI in skladnosti za SLeeLa VM, Java/JVM ter izvorni C/C++.
- [Recursion stanchion — prototipi C in C++](../../recursion/stanchion/README.md) — primeri, specializirani za primitive Boolean, Byte, Character, Integer, Long, Float in Double, ob splošnih, uporabnih, znanstvenih in učnih vzorcih rekurzije.
- [Podpora platform in matrika preverjanja](../../docs/PLATFORM-SUPPORT.md)
- [Vodnik po blagovni znamki](../../docs/BRAND-GUIDE.md)
- [Zamisli in gradiva za blago](../../merchandise/README.md)
- [Logotip NitroWebExpress](../../images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Politika testiranja in pogoji za izdajo](../../markdown/TESTING.md)

## II. Novice

Nedavni dodatki jeziku, gonilnikom in standardni knjižnici:

- **Paket standardne knjižnice `advantage` — izkoriščanje dividende z
  matematiko, fiziko, integracijo, ocenjevanjem in usmerljivim izhodom.**
  [`lib/advantage/`](../../lib/advantage/README.md) zajema *izkoriščanje edinstvene
  dividende glede na model* kot večplastno **čebulo Sequitur**
  (dividenda → pregled → pomisleki → matematika → dodatki → odštevanje →
  prerazvrstitev → iteracija), po kateri se je mogoče pomikati navznoter in
  navzven s polno dinamično posodobitvijo in obnovitvijo (restitute); na voljo
  je prek preprostega **uporabniškega modela** in **razvijalskega modela** s
  polnim nadzorom. Vsebuje pet predmetnih področij: **`math/`** — dvostransko
  trgovinsko ekonomijo (skupna maksimizacija dobička, Nashevo pogajanje,
  kritično razmerje dvostranskega newsvendorja), **`physics/`** — strukturno
  ekonomsko »fiziko polj zaupanja« (zaupanje kot metrični tenzor,
  transakcijska geodetka, kontaktna struktura obvestila, SDE razpada zaupanja
  CIR+skoki — modelna analogija, kot taka tudi označena), **`integration/`** —
  rekurzivno sestavljiv model vertikalne integracije za kapitalizem/trgovino na
  drobno (predelki, inženirski zalivi, preizkusni predelki, newtonski mediani
  pladnji in ocena sofisticiranosti sistema), **`grading/`** — ocenjevalnik
  sofisticiranosti, ki vsak modul in vsak niz zmagovalnih/poraženih/ocenjevalnih/
  absolutnih modulov uvrsti v razrede A–D, ter **`output/`** — usmerljivo
  točko oddaje, ki izhod modula pošlje v terminal, na površino v formatu
  **CommonRails** ali na vtičnico/priključek (uokvirjeno za izvorni
  `slnet_write`). Vsak razred nosi zapis Metadata-Class; pet zagonljivih
  predstavitev potrjuje predmetna področja, spremljajoči dokumenti
  [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md),
  [`MATH.md`](../../lib/advantage/MATH.md) in
  [`PHYSICS.md`](../../lib/advantage/PHYSICS.md) pa jih dokumentirajo. Izvorni
  potrošnik C/C++ v [`advantage/`](../../advantage/README.md) (jedro C
  `advantage_value`/`advantage_mean`, shramba znanja XML, varna knjižnica za
  pridobivanje prek HTTPS/SFTP/SSH + shramba ključev + prenos DH/ECDH) povezuje
  model z izdelanimi
  [primeri trgovanja](../../advantage/examples/README.md) — pozitivne trgovine iz
  domnevnih internetnih virov, zaporedje relevantnosti za občinstvo (trgovci,
  potrošniki, pomočniki, zdravilci) in osem modelov, razvrščenih A–D (D =
  največ cementa, A = največ blagovne znamke).
- **Časovne anotacije zank — `@loopDelay` / `@loopBudget` (+ vgrajena funkcija
  `sleep(ms)`).** Prvi **stavčni, vedenjski** anotaciji v SLeeLa:
  zapisani v lastni vrstici pred zanko `for` / `while` / `do` vstavita
  pravi premor v izvajanje zanke. `@loopDelay(ms = 250)` /
  `@loopDelay(seconds = 2)` miruje fiksen čas ob vsaki iteraciji (goli
  `@loopDelay(250)` privzeto pomeni milisekunde); `@loopBudget(seconds = 10,
  iterations = 1000)` znano število iteracij enakomerno razporedi čez skupni
  proračun realnega časa. Obe se prevedeta v nov append-only ukaz
  **`OP_SLEEP`** (ISA 157), ki podpira tudi splošno vgrajeno funkcijo
  **`sleep(<ms>)`**, ki jo je mogoče poklicati kjerkoli; premor je izveden
  prenosljivo (`nanosleep` / `Sleep`). Specificirano v
  [`SLEELA.syntax` §32](../../SLEELA.syntax); referenca API v
  [`api/sleela/3.0/LOOP_TIMING_API.md`](../../api/sleela/3.0/LOOP_TIMING_API.md);
  zagonljiv
  [`examples/loop-timing-annotation-demo.sleela`](../../examples/loop-timing-annotation-demo.sleela).
  Kazalo dodatkov seje (vključno s tem, izvornim `OP_BIGMUL` za množenje
  velikih celih števil ter samostojnima orodjema **[local-generator](../../local-generator/README.md)**
  in **[pi-generator](../../pi-generator/README.md)**) je v
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](../../api/sleela/3.0/SESSION-ADDITIONS-API.md).
  Tri ustvarjene površine API
  ([`sleeladoc-api/`](../../sleeladoc-api/README.md),
  [`docs/api/`](../../docs/api/README.md) in nespremenljivi posnetek
  [`api/sleela/3.0/version-014/`](../../api/sleela/3.0/version-014/index.html))
  so bile ponovno zgrajene z `make api-docs` in na strani paketa `vm` zdaj
  vsebujejo nova vnosa ukazov `SLISASleep` / `SLISABigMul`.
- **SleelaDoc iz ukazne vrstice — `sleela docs`.** Do SleelaDoc je zdaj mogoče
  priti neposredno iz terminala. `sleela docs` našteje vse dokumentirljive teme;
  `sleela docs crypto` prikaže paket; `sleela docs stringbuilder` razreši
  razred (neodvisno od velikosti črk in prek predpone standardne knjižnice `SL` →
  `SLStringBuilder`) in izpiše bogato stran — **definicijo, oblikovalski namen,
  razmerja, zgradbo, uporabniške gline (user clays) in petstopenjsko lestvico
  mojstrstva** — z razvrščenimi rezultati »ste mislili« ob zgrešku. Avtorji
  stran obogatijo preprosto z dodajanjem oznak SleelaDoc `@` (`@intent`,
  `@thesis`, `@axiom`, `@param`, `@see`, `@mastery`). `sleela docs --generate
  [dir]` zapiše besedilni korpus (ena stran na razred + `TOPICS.index`), ki ga
  CLI nato bere brez povezave; **`make sleeladoc`** v
  `sleeladoc/build/sleeladoc.jar` zapakira ta korpus **in ustrezne izvore
  `.sleela`**. Za teme samo s fasado (npr. `psychiatry`, `politico`)
  `sleela docs --facade <topic>` med izvajanjem preišče datotečni sistem in
  našteje prave **mape (nato datoteke, kot naravno gibanje)**, omejene z
  uporabnik-preglasljivimi zgornjimi mejami **6 MB / 6000 datotek / 6 minut**.
  Enote `lib/sleeladoc` se vzdržujejo na trenutni izdaji, **`#sleela 3.0`**.
  Glejte [`man/man1/sleela.1`](../../man/man1/sleela.1) in
  [`lib/sleeladoc/README.md`](../../lib/sleeladoc/README.md).
- **SleelaDoc — rešitev za dokumentacijo v slogu Javadoc z ustvarjenim
  spletiščem HTML.** **SleelaDoc** je SLeeLina konvencija dokumentacijskih
  komentarjev v izvorni kodi (komentarji v kotnih oklepajih, označeni z
  `@doc` / `@page`; glejte
  [`manifesto/SLEELADOC.md`](../../manifesto/SLEELADOC.md)), [`lib/sleeladoc`](../../lib/sleeladoc/README.md)
  pa je **ekstraktor**, ki enoto `.sleela` upodobi kot stran API v Markdownu ali
  HTML — tako kot `javadoc` upodobi Javo, pri čemer dokumentira *namen*.
  Popolno, brskljivo **spletišče dokumentacije HTML za celoten `/lib`** ustvari
  [`/sleeladoc-api`](../../sleeladoc-api/README.md) z
  `tools/generate-sleeladoc-api.py` (odprite `sleeladoc-api/index.html`; čist
  slog modre / svetlomodre / svetlosive / bele z hierarhijo paket → razred →
  član in glosarjem, napisanim za bralca *Student 2*). Prevajalnik kot
  stranski produkt prevajanja izdaja tudi avtorsko označene dokumentacijske
  izhode (`--wiki` stran MediaWiki + izvorni dvojnik z navedenim poreklom),
  dokumentirane v
  [`manifesto/SLEELADOC.WIKI.md`](../../manifesto/SLEELADOC.WIKI.md). Zagonljivo:
  [`lib/sleeladoc/sleeladoc-demo.sleela`](../../lib/sleeladoc/sleeladoc-demo.sleela).
- **Objektni model dokumenta XML (`lib/xml`).** DOM XML na strani SLeeLa —
  dokument, ki ima drevo vozlišč elementov/besedila/komentarjev/CDATA/PI z
  urejenimi otroki in poimenovanimi atributi, serializator (kompaktno/lepo,
  ubežanje po XML 1.0 §2.4), razčlenjevalnik na osnovi znakovnih celic in
  register kakovosti `SLXmlAspect`. Enakost vozlišč je tristopenjska vez
  zaupanja (identiteta → isti subjekt → struktura), refleksivna, simetrična in
  deterministična. Glejte [`lib/xml/README.md`](../../lib/xml/README.md) in
  zagonljivi [`lib/xml/xml-dom.sleela`](../../lib/xml/xml-dom.sleela).
- **Široki predznačeni celoštevilski primitivi (`long`, `int64`, `int128`).**
  `int`, `long` in `int64` so vsi 64-bitna predznačena cela števila, ki se
  tipsko preverijo in izvajajo enako; `int128` je sprejet kot celoštevilski tip
  (za zdaj podprt s 64 biti, pravi 128-bitni tip pa je dokumentiran nadaljnji
  korak). Razširjata ju predznačena serija z osnovo 2 `int32`..`int16777216`
  (sintaksa 2.7) in ustrezna nepredznačena serija `uint32`..`uint16777216`
  (sintaksa 2.8). Glejte [`SLEELA.syntax`](../../SLEELA.syntax) §21, §24, §25.
- **Posledična polja `@mix` (sintaksa 2.9).** Razred lahko deklarira več
  polj primerka z **istim imenom**, če vsaka ponovitev nosi `@mix`; jezik
  skupino razume kot posledično (polje/seznam, indeksiran po indeksu),
  privzeto od zgoraj navzdol `1,2,3,…` ali ročno prek `@mix(1,2,3,...)`, katerega
  število indeksov mora ustrezati številu polj. Glejte
  [`SLEELA.syntax`](../../SLEELA.syntax) §28.
- **Označevalniki dokumentov — Naravni proces, naslovljen Preonems
  (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).**
  Te goli besedi označevalnikov dokumentov se skupaj imenujejo **Preonems**
  (en označevalnik je *Preonem*; množica pa *Preonems* — glejte
  [`SLEELA.syntax`](../../SLEELA.syntax) §26.1). Prvorazredni, zgolj deklarativni
  označevalniki dokumentov, ki poimenujejo, kaj dokument *je* v vrstnem redu
  nastajanja: `author`/`authorial` (sintaksa 2.4; obravnava jih strokovni svet
  — edini član z vedenjskim učinkom), `material;` (uveljavljeni vir) in
  `memory;` (sintaksa 2.5), `determine;` (sintaksa 2.6; dokument, odprt za
  nadaljnjo obdelavo po zdravniku, fiziku ali duhu do dokončanja po zakoniti
  roki) ter najnovejša dva, **`object;`** (uveljavljena stvar, ki stoji v
  katalogu obstoječega — **reificirana, s konkretnim položajem, pri sintaksi
  2.8**) in **`field;`** (podlaga razmerja, v kateri objekti stojijo in se
  gibljejo) — oba omejena od sintakse 2.6. Vsak je gol, kontekstualen in ga
  sprednji del prevajalnika omejuje po različici, brez vpliva na razčlenjevanje,
  tipiziranje ali generiranje kode; zunaj položaja označevalnika ostane vsak
  navaden identifikator (zato `field` nikoli ne moti člana razreda ali dostopa
  `.field`). Glejte [`SLEELA.syntax`](../../SLEELA.syntax)
  §18–§20, §23 in §26.
- **Nordshrift pri sintaksi 3.0 s polnim dosegom knjižnice.** Nordshrift
  razrešuje prek istega rekurzivnega indeksa `/lib` kot prevajalnik in doseže
  vsak paket in enoto `.sleela`; `nordshrift check` poroča števila za celotno
  drevo.
- **Recursion stanchion.** Imenik [`recursion/stanchion/`](../../recursion/stanchion/README.md) zdaj vključuje namenske referenčne prototipe C in C++ za sedem dokumentiranih pogodb primitivov ter splošne, uporabne, znanstvene in učne primere. To so referenčni viri; prevedite in preizkusite jih, preden jih obravnavate kot preverjene.

- **Komentarji v kotnih oklepajih (`<` / `<<`).** Nov komentarni slog za naprej —
  `< line >` in `<< block >>` — ki ga leksira sprednji del SLeeLa in je
  dokumentiran v [`SLEELA.syntax`](../../SLEELA.syntax) §1.1.
- **Radiksni celoštevilski literali.** Literali `int` zdaj sprejemajo
  šestnajstiško (`0x`), dvojiško (`0b`) in osmiško (`0o`) obliko z ločili
  števk `_`, tako v izvornem jeziku SLeeLa ([`SLEELA.syntax`](../../SLEELA.syntax) §1.4)
  kot v [formatu modelov CPU `.xlt`](../../lib/xlt/README.md).
- **Standard Metadata Class v celotni knjižnici.** Metavzorec opisnih
  metapodatkov (poreklo, zakonitost, jurisdikcijski obseg) zdaj nosi vsaka
  datoteka `.sleela` pod `/lib`, ki vsebuje razrede; glejte
  [`SLEELA.syntax`](../../SLEELA.syntax) §15 in
  [učno lekcijo](../../tutorial/13-metadata-class.md).
- **Bogatejša družina text / String.** `lib/text` pridobi prave pomočnike v
  jeziku, pogled na znakovne celice, ki daje pravi
  `charAt`/`substring`/`indexOf`/`reverse`/poravnavo stolpcev, in pravi
  pozicijski predlogovnik — glejte
  [učbenik za text in String](../../tutorial/14-text-and-strings.md) in zagonljivi
  [`lib/text/text-string-demo.sleela`](../../lib/text/text-string-demo.sleela).
- **Prvorazredni `String` v listih Nordshrift.** List `.sst` lahko deklarira
  tipiziran blok `strings:`, ki prispeva k manifestu komponent gradnje;
  glejte [Nordshrift Sheet Reference](../../wiki/Nordshrift-Sheet-Reference.md).
- **Modeli CPU se izvajajo prek ukazov in prek enakovrednosti klicev funkcij.**
  Vsak `SL<ARCH>CPU` podeduje osnovo izvajanja izvornega toka ukazov prek
  svoje zanke prevzemi-dekodiraj-izvedi *in* izvajanja kode domnevnega drugega
  procesa z enakovrednimi klici na skupni podlagi VM — glejte
  [`lib/cpu/EXECUTION.md`](../../lib/cpu/EXECUTION.md) §5.

## III. Gradnja in testiranje

**Zahteve:** orodna veriga C/C++ (GCC ali Clang) in GNU Make; Python 3 za
orodja za manifeste/tok dela; v Linuxu razvojni paketi X11/Xrandr za
izdelke za prikaz (modulov `make deps` sporoči/namesti, kar potrebuje).
Gradnja `impl/` je zaprta ob napaki (fail-closed) glede na zaupanja vreden
manifest SHA-256, na katerega korenski `Makefile` kaže samodejno.

Iz čiste izvorne kopije najhitrejši začetek:

```sh
make                                  # build everything from the repo root
```

Ali zgradite in preizkusite samo izvorno jedro:

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

Korenski `make` zgradi vse izdelke (jedro, VM, kodeke, churn,
antivirus, …) in se konča z gradnjo **registra** ter materializacijo njegove
izhodiščne tabele — zato zgrajeno drevo takoj ponuja pregledljiv register.
Bistveni cilji:

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

Drugi uporabni cilji:

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

Cilj `registry` zažene [`build-registry.sh`](../../build-registry.sh): zgradi
jedro registra, izhodiščno tabelo vnese v prenosljivo besedilno shrambo
`registry/build/sleela.sleelareg` in izvozi skript tabele MySQL
`registry/build/registry.sql` (prek `sleela registry export`). Pregledate ga z
`sleela registry` ali `registry/build/sleela-registry review`. Glejte
[vodnik po registru](../../registry/README.md) in [arhitekturo Registry 3.0](../../registry/REGISTRY-3.0-ARCHITECTURE.md).

Zahteve in korake, specifične za platformo, poiščite v vodnikih za gradnjo in testiranje. Uspešna gradnja ene komponente ne dokazuje, da so vse knjižnice ali platforme pripravljene za izdajo.

## IV. Namestitev

Po gradnji izvornih izdelkov namestite ukaze in podporne datoteke izvajalnega okolja:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

Običajni namestitveni program privzeto uporablja `/usr/local/bin`, `/usr/local/lib/sleela`
in `/usr/local/share/sleela`. Ohrani obstoječo zagonsko
konfiguracijo in noče prepisati obstoječih ukazov, ki jih ne sledi.
Odstranitev odstrani le datoteke, ki jih je zabeležil namestitveni program, ter
ohrani konfiguracijo in nesledene podatke izvajalnega okolja.

Za osredotočeno, vodeno namestitev samo glavnega paketa — dvojiških datotek `sleela`,
`sleelvac` in `nordshrift`, standardne knjižnice, protokolov in
podpornih modulov — uporabite čisti namestitveni program:

```sh
make clean-install       # guided install of the main package only
```

Sprejme eno primarno odločitev (namestitev glavnega paketa), nato izbiro
obsega, ob uspehu pa neobvezen izbirnik za izbrane razširitvene module.
Za interaktivni potek namestitve in vse možnosti glejte
[Hitra in varna namestitev](../../install/README.md).

## V. Jezik in izvajalno okolje
Avtoritativna slovnica je [`SLEELA.syntax`](../../SLEELA.syntax). Trenutni repozitorij dokumentira sintakso **3.0**; različica prevajalnika/orodja in različica izvorne sintakse sta ločeni. Površina 3.0 doda **označevalnika dokumentov `pure` in `gasoline`** (§30) — par golih, kontekstualnih, zgolj deklarativnih Preonems, ki pomenita skoraj isto, vsak pa označuje položaj dokumenta po svoji lastni poštenosti. Sintaksa 3.0 poveže površino jezika 1.0–2.9 v trenutno normativno izdajo, pri čemer ohrani vse minimalne omejitve posameznih funkcij. Prejšnja površina 2.9 doda **posledična polja `@mix`** (§28) — razred lahko deklarira več istoimenskih polj primerka, če vsako nosi `@mix`, razumljena kot ena indeksirana skupina (od zgoraj navzdol `1,2,3,…` ali ročno `@mix(1,2,3,...)`, katerega število indeksov mora ustrezati skupini). Prenaša naprej družini predznačenih/nepredznačenih širokih celih števil z osnovo 2 `int32`..`int16777216` (§24) in `uint32`..`uint16777216` (§25) nad prejšnjimi `long`/`int64`/`int128` (§21); **dokumente podpornike ključnih besed** (§22); in **Preonems** — zgolj deklarativne označevalnike dokumentov `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) in `artifact` (§35, položaj zakonodajne stopnje, odmerjen glede na čas in prostor), skupaj imenovane Preonems (§26.1). Razreševanje preobremenjenih konstruktorjev in delegiranje `this(...)`/konstruktorja osnovnega razreda sta izrecno zunaj preverjene pogodbe sintakse 1.8. Pred zanašanjem na funkcijo preberite slovnico in opombe o implementaciji. Devet izvornih enot SleelaDoc zdaj deklarira `#sleela 3.0`; skupna vrata prevajalnika sprejmejo izvorno sintakso 1.0–3.0. Nova gradnja API nastane z `make api-docs`.

Repozitorij vključuje izvorno implementacijo C/C++, reference za generiranje SLVM, module standardne knjižnice, teste, primere in platformne integracije. Stanje implementacije in preverjanja se po modulih razlikuje; ne predpostavljajte enotne popolnosti, temveč preglejte specifikacijo modula in trenutne rezultate CI.

## VI. Inženirska načela
- **Dokazi pred trditvami:** ločujte implementirano, preizkušeno, eksperimentalno in načrtovano vedenje.
- **Ena pogodba na mejo:** dokumentirajte sintakso, API-je, formate datotek, platformne predpostavke in vedenje ob napakah.
- **Prenosljivo že v zasnovi:** izolirajte zaledja, specifična za platformo, in testirajte na izvornih izvajalcih.
- **Varna odpoved:** preizkusite napačno oblikovan vnos, omejitve virov, odpovedi procesov in poti čiščenja.
- **Ponovljive izdaje:** gradite znova iz čistega ter objavite zgoščene vrednosti artefaktov in opombe ob izdaji.

## VII. Gradnja antivirusa in celovitost

Antivirusni paket Sleelavirin vključuje izvorni pregledovalnik SHA-256, neobvezen
adapter ClamAV, manifeste celovitosti izvora/indeksa in kontrolne vsote
paketov za platforme. Paket zgradite in preizkusite iz korena repozitorija:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Manifest celovitosti se ustvari pod `antivirus/build/`; zajema sledene
vhode izvorne kode in dokumentacije ter zaščitene datoteke indeksa podpisov,
ustvarjen izhod in spremenljivi predpomnilnik ClamAV pa izključuje. Posnetki
zbirk podatkov ClamAV imajo lastne manifeste. Izdajni ZIP-i vključujejo `INTEGRITY-MANIFEST.json`,
`SHA256SUMS` in prikolico `.sha256`. Zgoščene vrednosti prenesenih arhivov preverite glede na
zaupanja vredno vrednost, pridobljeno neodvisno: kontrolna vsota, shranjena ob arhivu, ni
podpis izdajatelja. Glejte [vodnik Sleelavirin](../../antivirus/README.md),
[varnostni kontrolni seznam](../../antivirus/SECURITY.md) in
[potek dela gradnje antivirusa](../../.github/workflows/antivirus-binaries.yml).

Izdaja ne velja za preverjeno, dokler ustrezna opravila CI ne uspejo na
natančnem izdajnem commitu in dokler niso preverjeni nastali artefakti.

## VIII. Urejevalniške teme SLeeLa — Vim in Emacs

SLeeLa vključuje usklajen nabor profesionalnih temnih barvnih shem za **Vim** in **Emacs** ter izvorno pripomoček za palete v C/C++. Prvotna shema **American Professional** ostaja privzeta. Na voljo je deset spremljevalnih palet za različne kulturne in institucionalne oblikovalske navdihe; to so interpretativne oblikovalske palete, ne uradni prikazi katerega koli naroda, kulture, sodišča ali ustanove.

### Enajst shem in njihove barve

| Shema | Ozadje | Ospredje | Ključne besede | Tipi | Nizi | Komentarji |
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

#### Barve nacionalnih zastav (izvorna paleta)

Grška in turška shema izhajata iz **uradnih barv nacionalnih zastav** obeh držav, prikazanih z njunimi natančnimi objavljenimi šestnajstiškimi vrednostmi. Ozadja/poudarki shem zgoraj so potemnjene, urejevalniku prijazne interpretacije teh izvornih barv — spodnja tabela je referenca »barv, kakor so nacionalno izpostavljene«.

| Narod | Barva | Hex | Podlaga in pomen |
|---|---|---|---|
| **Grčija** | Modra | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | Modra barva nacionalne zastave (zakon 851/1978). Nebo in morja; heraldično opisana kot *azure*. |
| **Grčija** | Bela | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Bela barva nacionalne zastave. Čistost boja za neodvisnost; križ označuje vzhodnopravoslavno krščanstvo. |
| **Türkiye** | Rdeča (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | Rdeča barva nacionalne zastave (turški zakon o zastavi). Polje; po izročilu kri mučencev in osmanska suverenost. |
| **Türkiye** | Bela | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Bela barva nacionalne zastave. Polmesec in zvezda — mir, čistost in simbol turško-islamske dediščine. |

Sheme zasidrajo ospredje vsakega urejevalnika v nacionalno **belo**, ključne besede, kazalko, izbor in vrstico stanja pa v natančno nacionalno **modro** (`#0D5EAF`) ali **rdečo** (`#E30A17`), tako da so pomembne barve dobesedno nacionalne barve. To so interpretativne oblikovalske palete; referenca barv zastav je podana zaradi porekla in ni uradni prikaz nobene od obeh držav. Pomeni barv sledijo opisu državnih simbolov ministrstva za zunanje zadeve Helenske republike in izročilu turškega zakona o zastavi.

### Tabela barvnih vzorcev

Celice palet spodaj uporabljajo **vzorce SVG**, tako da GitHub upodobi dejanske barve in ne praznih celic barv HTML. Vsak trak prikazuje šest barv v tem vrstnem redu: ozadje, ospredje, ključne besede, tipi, nizi in komentarji. Natančne šestnajstiške kode so navedene ob vsakem vzorcu, v istem vrstnem redu shem kot zgoraj.

| Shema | Upodobljene barve palete | Šestnajstiške kode (ozadje · ospredje · ključne besede · tipi · nizi · komentarji) |
|---|---|---|
| **American Professional** | ![American Professional palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Pan-Eurasian palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russian and Slavic — Medium** | ![Russian and Slavic Medium palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Central Asian / Silk Road** | ![Central Asian Silk Road palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **East Asian** | ![East Asian palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC — International Criminal Court** | ![ICC palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS — U.S. Supreme Court** | ![SCOTUS palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Greek (Hellenic)** | ![Greek Hellenic palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turkish (Al Bayrak)** | ![Turkish Al Bayrak palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Kiro palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![SLeeLa palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

Za funkcije, konstante, izbore, diagnostiko in direktive predprocesorja se uporabljajo dodatne poudarjene barve. Ruska in slovanska paleta namenoma uporablja srednje intenzivne barve skrilavca, modre, žajblja, rubina in starega zlata za mirnejši profesionalen videz.

### Namestitev shem za Vim

Iz korena repozitorija:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

V `~/.vimrc` dodajte naslednje, da omogočite barvanje sintakse SLeeLa in izberete shemo:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Izberite natančno eno od teh imen barvnih shem:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Na primer, za srednjo rusko in slovansko paleto uporabite `colorscheme sleela-russian-slavic`, za helensko in al bayrak paleto pa `colorscheme sleela-greek` oziroma `colorscheme sleela-turkish`. Znova zaženite Vim ali naložite konfiguracijo z `:source $MYVIMRC`.

### Namestitev shem za Emacs

Ustvarite lokalni imenik tem in iz korena repozitorija kopirajte datoteke načina in tem:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Dodajte to v `~/.emacs` ali `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Za drugo shemo zamenjajte `sleela-american` s `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` ali `sleela-sleela`. Če Emacs vpraša, ali naj temi zaupa, pred potrditvijo preglejte lokalno datoteko teme.

### Predogled izvornih palet C/C++

Majhen izvorni pripomoček za palete lahko našteje sheme, izpiše njihove barve terminala in poišče posamezno barvo:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Zaradi povratne združljivosti `--palette` in `--hex COLOR` brez `--theme` izbereta American Professional. Rezultate gradnje in testov je treba preveriti na ciljnem sistemu in v CI; dokumentacija ukaza sama ni dokaz, da je uspel.

Celotne opise palet in podrobnosti o pripomočku glejte v [vodniku za integracijo urejevalnikov](../../linux-editors/README.md).

## IX. Prispevanje
1. Preberite kazalo dokumentacije in politiko testiranja.
2. Naredite osredotočene spremembe s pozitivnimi in negativnimi testi.
3. Zaženite ciljane teste, nato široko zbirko.
4. Sporočite podrobnosti o OS/orodni verigi; preskočenih preverjanj ne opisujte kot uspešnih.

SLeeLo razvija **MEARVK LLC**. Zgodovinsko gradivo ciljne strani je ohranjeno v [`docs/README-LEGACY.md`](../../docs/README-LEGACY.md).

## X. AE6E66 — Pričakovanje davčne dajatve na javno e-pošto Round

Za pobudo AE6E66 je izrecno pričakovanje projekta **$300,000,000.00 USD na javno e-pošto Round** kot predlagani znesek davčne dajatve, povezan z vsako kvalificirano javno e-pošto Round.

Ta izjava beleži zgolj pričakovanje projekta. **Ni** pravna davčna odmera, vladni nalog, sodna odločba ali neodvisno ugotovljena davčna obveznost. Vsaka dejanska dajatev bi zahtevala ustrezen pravni organ, jurisdikcijo, opredelitve, postopek in pristojno vladno odločitev.


## XI. Izvajalno okolje abecede in nabora znakov

Dokumentacija abecede in izvorno izvajalno okolje znakov SLeeLa se vzdržujeta v alphabet/sleela/ in lib/alphabet/native/. Izvorni testi C++17 zajemajo strogo dekodiranje/kodiranje skalarjev UTF-8, podpise zmogljivosti C<n> in omejeno članstvo v naboru znakov:

    make -C lib/alphabet/native test

Eksperimentalni katalog UTF-4088 in njegov iskalni vmesnik SLeeLa sta dokumentirana v utf-4088/SLeeLa-ALPHABET-INTEGRATION.md. ID-ji UTF-4088 so ločen imenski prostor, ne skalarne vrednosti Unicode. Delo na vezavah prevajalnika in SLVM se izrecno vodi ločeno od teh izvornih temeljev.

## XII. AutoCAD Creator™

[AutoCAD Creator™](../../autocad/creator/README.md) je interaktivna 2D aplikacija CAD,
napisana v celoti v SLeeLa. Kjer sorodni AutoCAD Renderer načrt v enem koraku
spremeni v risbo, vam Creator omogoča, da dokument 2D AutoCAD **ustvarjate in
spreminjate** sproti — dodajate stene, sobe, mere in opombe, nato pa jih
premikate, preimenujete, prestavljate med plasti, brišete in razveljavljate —
ter rezultat **shranite** kot pravo risbo AutoCAD DXF (besedilo R12, odpre se v
AutoCAD / LibreCAD).

Njegov grafični vmesnik je zgrajen na **SleelaUI™**, SLeeLinem lastnem
medplatformnem orodju za uporabniški vmesnik (pod [`lib/user-interface`](../../lib/user-interface)) — ne na JavaFX —
ki odpre pravo izvorno okno na X11 / Cocoa / Win32. Zasnova sledi enemu pravilu,
**»SleelaUI predstavlja; SLeeLa odloča«**: GUI nikoli ne izračunava geometrije
in ne zapisuje DXF. Vsako orodje izda namero ustvari / spremeni / izbriši /
razveljavi / shrani avtoritativnemu
[`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela), ki je lastnik
urejljivega dokumenta in obrnljive zgodovine ukazov, medtem ko platno le
predogleduje geometrijo, ki jo hrani Creator.

Ponovno uporabni urejevalni mehanizem je v [`lib/autocad`](../../lib/autocad) —
[`SLCadEntity`](../../lib/autocad/SLCadEntity.sleela),
[`SLCadDocument`](../../lib/autocad/SLCadDocument.sleela),
[`SLCadCommand`](../../lib/autocad/SLCadCommand.sleela) in
[`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela) — in teče od konca do konca brez zaslona.
Lupina GUI in predstavitev sta v [`autocad/creator`](../../autocad/creator):

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

Na zaslonu to odpre živo urejevalno okno; na gostitelju brez zaslona risbo zgradi v
pomnilniku, izpiše poročilo in DXF ter se čisto konča.


## XIII. Vrstni red vhodnega zemljevida UTF-4088

Specializirani katalog UTF-4088 definira prostorsko prečkanje vhodnega zemljevida in abstraktne iteracije pritiska v [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). Politika predmetnega kazala National Lien je v [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Koordinate zemljevida, ravni pritiska (1–50), stalni ID-ji kataloga in abecedni razredi so neodvisna polja; nobenega ni dovoljeno tiho nadomestiti z drugim. Pritisk je algoritmična raven iteracije, ne fizikalna količina.


## XIV. Nedavni in razširjeni indeks paketov

Ta dodani indeks izpostavlja nedavno razširjene pakete, domenske knjižnice in
zbirke izvorne kode. Vnosi so razvrščeni po abecedi imena paketa ali teme.
Opisi navajajo lokacijo v repozitoriju; stanje implementacije in preverjanja
je treba preveriti v dokumentaciji in testih posameznega paketa.

### A–C

- **Advantage** — `lib/advantage/` in `advantage/`: izkoriščanje edinstvene
  dividende glede na model kot večplastna čebula Sequitur s petimi
  podmapami predmetnih področij — `math/` (skupna maksimizacija dobička,
  Nashevo pogajanje, dvostranski newsvendor), `physics/` (metrika/geodetka/
  kontakt/SDE polj zaupanja, označena modelna analogija), `integration/`
  (predelki vertikalne integracije in ocena sofisticiranosti), `grading/`
  (ocenjevalnik sofisticiranosti A–D za module in nize) ter `output/`
  (oddaja v terminal / CommonRails / vtičnico). Izvorni potrošnik C/C++
  (`advantage/`) povezuje model z izdelanimi primeri trgovanja. Glejte
  [`lib/advantage/README.md`](../../lib/advantage/README.md)
  in [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md).
- **Alphabet in UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` in
  `utf-4088/`: katalogi abeced, izvorna podpora naborom znakov, vrstni red
  vhodnih zemljevidov in dokumentacija integracije UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` in `lib/autocad/`: interaktivni
  poteki dela 2D risanja in ponovno uporabne komponente dokumenta/urejanja CAD.
- **Ballum** — nadzor zaslona na ravni slikovnih pik: nastavljanje/branje po
  posamezni piki, blit območja, svetlost/toplina/gama celotnega zaslona,
  vnos kazalca/tipkovnice in **površina medpomnilnika celotnega zaslona** za
  popoln, lasten, hiter nadzor po slikovnih pikah (okno override-redirect +
  en sam prikaz `XPutImage`/MIT-SHM v Linuxu/X11). Izvorno jedro je pod
  `ballum/`, fasada SLeeLa v `lib/ballum/`, most VM pa pod `impl/`; glejte
  [`ballum/README.md`](../../ballum/README.md) in zagonljivi
  [`ballum/example/`](../../ballum/example/).
- **CDMA** — `cdma/` in `lib/cdma/`: tehnično gradivo, povezano s CDMA, in
  delo na knjižnici izvorne kode SLeeLa.
- **Character** — `lib/character/`: profili likov in ponovno uporabna
  statistika likov, vključno s primeri biografskih profilov.
- **Coorenagraph** — `lib/coorenagraph/`: etično sklepanje, moralno
  besedišče, razmislek in sorodni pojmi; glejte README paketa in izvorne datoteke.

### D–G

- **Governance source models** — [lib/governance](../../lib/governance/README.md) definira deklarativne izvorne oblike SLeeLa za predloge, anonimne ročne glasovnice in agregatne seštevke. Predlagana anotacija @vote in ročni postopek sta dokumentirana v [registry/DEMOCRATIC-VOTING-SYNTAX.md](../../registry/DEMOCRATIC-VOTING-SYNTAX.md); trenutni prevajalniki jo obravnavajo kot inertne metapodatke komentarja.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` do razpoložljivih verzioniranih
  imenikov paketov HTTP: protokolne specifikacije in implementacije,
  specifične za posamezno različico. Natančen obseg in stanje preverjanja
  poiščite v dokumentaciji posamezne različice.
- **International Criminal Court** — `lib/international-criminal-court/` in
  `INTERNATIONAL.CRIMINAL.COURTS.md`: sorodna domensko specifična izvorna koda
  in dokumentacija.

### O–S

- **OFDMA** — `ofdma/`: tehnično gradivo o ortogonalnem frekvenčnem
  večkratnem dostopu in razvojni viri C/C++.
- **Philosophy in modul Conversation Mask** — [`philosophy/`](../../philosophy/README.md)
  vsebuje izvorne temelje C/C++, [`philosophy/module/`](../../philosophy/module/README.md)
  pa njihovo naslednjo plast modula organizira v tri izrecna področja:
  [`data/`](../../philosophy/module/data/README.md) za tipizirana, datirana opažanja,
  konce dogodkov, nastavitve razpolovnega časa, poreklo in privolitev; [`helper/`](../../philosophy/module/helper/README.md)
  za deterministično preverjanje, upadanje relevantnosti, združljive primerjave in
  predstavitev, filtrirano glede na privolitev; ter [`psychiatry-friend/`](../../philosophy/module/psychiatry-friend/README.md)
  za podporen, neklinični pogovorni vmesnik. Ročaji, obrnjeni k SLeeLa, in
  sestavljeno drevo (Tree) so v [`lib/philosophy/`](../../lib/philosophy/README.md),
  ob širšem paketu philosophy. Modul temelji na izrecnem vnosu in
  najprej na privolitvi: ne spremlja pasivno ljudi ali medijev, ne postavlja
  diagnoz, ne sklepa o zasebnih namenih, ne ocenjuje človekove vrednosti in ne
  priporoča sprememb zdravil. Povezave dogodkov ohranjajo poreklo in
  navigacijo, ne neodvisne potrditve.
- **Synchro** — integracija izvornega izvajalnega okolja/prevajalnika ter
  funkcionalnost razpošiljanja paketov in meritev; glejte ustrezno izvorno kodo
  v `impl/` in implementacijo prevajalnika.
- **SleelaUI / terminalski UI** — `lib/user-interface/` in projekt
  `sleela-terminal`: integracija UI SLeeLa in komponente za interakcijo,
  usmerjene v terminal.

### U–W

- **United Nations** — `united-nations/`: domensko specifično gradivo repozitorija.
- **Wi-Fi 4, Wi-Fi 5 in Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`,
  `wi-fi-6/` in sorodni izvorni imeniki `lib/wifi/`: gradivo o brezžičnem
  omrežju in konfiguracijski viri za posamezno generacijo.

### Vzdrževanje indeksa paketov

Ko je paket dodan ali bistveno razširjen, v ta indeks dodajte njegov vnos v
ustrezno abecedno skupino, povežite njegov kanonični imenik in README ter
opišite le zmožnosti, ki jih podpirajo vstavljeni viri. Indeks ohranjajte
aditiven: pri posodobitvah indeksa paketov ne odstranjujte ali prepisujte
obstoječih razdelkov README.


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

US Democratic and US Policy.

## XVI. Nacionalna znanstvena blagovna podoba

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. Eksperimentalni katalog UTF-4088 z milijonom vnosov

[Generator kataloga UTF-4088](../../utf-4088/tools/generate_catalogue.py) deterministično zgradi eksperimentalni katalog simbolov s 1.048.576 vnosi. Dokumentirani kompaktni format je opisan v [utf-4088/FORMAT.md](../../utf-4088/FORMAT.md); pogodba o generiranju in preverjanju je v [utf-4088/GENERATION-RESULTS.md](../../utf-4088/GENERATION-RESULTS.md).

Generator zaženite s Pythonom 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Ustvarjena dvojiška datoteka meri 16.777.244 bajtov in je zasnovana tako, da ostane pod 20.000.000 bajti. Enotni testi in potek dela GitHub Actions preverjajo število vnosov, edinstvenost glifov, velikost in SHA-256; uspešni zagoni objavijo dvojiško datoteko in povzetek kot artefakt poteka dela `utf4088-million-catalogue`. Ustvarjena dvojiška datoteka je namenoma izven zgodovine Gita. UTF-4088 ostaja eksperimentalen in ni združljiv z Unicode.

---

# SLeeLa
<p align="center"><img src="../../images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

---

## XVIII. Nedavni dodatki — konstanta Pi, zagon Modium/Registry, matematika sleelactl in `@native`

Ta razdelek beleži funkcije, dodane med delom 2026-11, najnovejše skupine najprej. Povezane izvorne datoteke in stanje njihovega preverjanja so avtoritativni; spremembe v pregledu niso opisane kot združene, dokler ne dosežejo `master`.

### Katalogna konstanta Pi s 100.000.000 števkami

Repozitorij vsebuje preverjeno vrednost števila π s **sto milijoni števk** v [`pi-generator/pi.100000000.digits`](../../pi-generator/pi.100000000.digits) (veljavno decimalno število, `3.14159…`) ob vrednosti s 57.344 števkami v [`pi-generator/pi.57344.digits`](../../pi-generator/pi.57344.digits). Obe sta neodvisno preverjeno pristni vrednosti π — glejte [`pi-generator/VERIFIED.md`](../../pi-generator/VERIFIED.md), ki dokumentira preverjanja (učbeniška predpona, Feynmanova točka na ulomljenem mestu 762, znana milijontna števka, ujemanje z `lib/math/SLPiReference.sleela` in analiza frekvenc števk celotne datoteke) ter enkratni reprodukcijski skript.

### Modium™ / Registry™ naložita konstanto Pi ob zagonu podsistema

Na namenskih vstopnih točkah podsistemov — `sleela registry` in `sleela modium` (ter ob prvem zagonu namestitvenega programa), **ne** ob vsakem `sleela run` — podsistema Modium in Registry naložita veliko katalogno konstanto π. Na interaktivnem terminalu izpišeta prehodno rdečo vrstico `Loading Modium™` / `Loading Registry™`, ki se nato izbriše na mestu, preden se začne običajni izpis; učinek je strogo vezan na TTY, zato izhod prek cevi in izhod testov ostaneta bajt za bajtom nespremenjena. Lokator (ne bajti) je vnesen kot privzeti vrednosti ključa registra `Math`, `piReferenceLargePath` in `piReferenceLargeDigits`, tako v modelu SLeeLa ([`lib/registry/SLRegistry.sleela`](../../lib/registry/SLRegistry.sleela)) kot v izvornem seederju ([`registry/sleela_registry.c`](../../registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` izpostavlja `largePath()` / `largeDigits()`. Konstanta π se v lokacijo SLeeLa operacijskega sistema (`<data>/pi/`) namesti z **obema** — standardnim namestitvenim programom `make` in vodenim »lepim« namestitvenim programom. Podrobnosti: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](../../pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — lepi namestitveni program in izvorno podprt predmet `math`

- Vodeni (»lepi«) namestitveni program zdaj namesti `sleelactl` (nadzorno ravnino SLeeLa) in njegov register modulov, enako kot namestitveni program `make`. Glejte [`bin/SLEELACTL.md`](../../bin/SLEELACTL.md).
- `sleelactl` je pridobil novo naravo **`subject`** za izvorne knjižnice predmetov C/C++, prevedene v izvajalno okolje, in registrira prvo od njih: **`math`**. `sleelactl run math` od konca do konca preizkusi izvorno zaledje `__NativeMath` (`impl/subjects/math/math_native.cpp`) prek referenčne predstavitve predmeta; `sleelactl info math` pojasni, kako se do njega pride (`import math;`). Narava subject ponuja le `run info` — namenoma ni ukaza `check`, ker `sleela check` ne naloži izvornih predmetov, registriranih v SHEET.

### `@native`, Native Drive in večjezično izvajanje

Nova anotacija metod **`@native`** zahteva vnaprejšnje izvorno prevajanje (AOT) vroče metode. Nov ukaz `sleela aot` poišče metode `@native`, preveri, da je vsaka znotraj podprte *razbremenljive podmnožice*, in odda C/C++ — **zaprto ob napaki (failing closed)**: ob vsaki konstrukciji, ki je še ne zna prevesti, zavrne (s točnim razlogom), tako da napol prevedena metoda ni nikoli ustvarjena.

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

`@native` ostaja zahteva za obravnavo AOT, nepodprte konstrukcije pa se zavrnejo pred oddajo. [Native Drive](../../impl/native-drive/README.md) podpira gradnje večenotnih izvedljivih datotek in deljenih knjižnic C/C++ prek gostiteljskih prevajalnikov (vključno z MSVC), podprt izhod AOT SLeeLa, pregled simbolov glede na ABI izvornega nalagalnika, izvajanje procesov brez lupine, obravnavo stdin/stdout/stderr, večstopenjsko povezovanje s cevmi in delegiranje XClass produkcijskemu nalagalniku. `sleelactl` izpostavlja svoje ukaze za gradnjo/zagon ter prek `inspect native-drive` usmerja pregled XClass in izrecno upodabljanje/namestitev enot systemd. VM je lastnik omejene tabele ročajev izvornih modulov z obvezno gostiteljsko avtorizacijo in ročaji, preverjenimi po generaciji. Sintaksa 3.0 zdaj dodaja `nativeOpen`, `nativeCall` in `nativeClose`: razpošiljanje bytecode samo z dodajanjem (append-only) prečka ločeno verzioniran ABI null/bool/int64/double/UTF-8, medtem ko VM-lastna polja, strukture in druge sestavljene vrednosti ostanejo znotraj interpreterja in lahko uporabijo rezervo bytecode z istim imenom. Vtičniki lahko neobvezno prejmejo verzionirano tabelo gostiteljskih povratnih klicev za klicanje natančnih funkcij SLeeLa prek ugnezdenega izvajanja VM, z zakoreninjenjem GC za začasno ustavljene sklade, izrecnim seznamom dovoljenih imen gostiteljskih povratnih klicev in zavrnitvijo rekurzivnih klicev izvornega API. Izvorna koda ostaja zaupanja vredna koda v procesu, ne peskovnik. Anotacije politike procesov v izvorni kodi imajo verzionirano shemo, vendar se še ne razčlenjujejo/uveljavljajo. Podrobnosti, varnostne meje in dnevnik sedmih delovnih tokov: [Native Drive](../../impl/native-drive/README.md) in [`pi-generator/AT-NATIVE.md`](../../pi-generator/AT-NATIVE.md).


## XIX. Mednarodne izdaje README

SLeeLa ponuja državne vstopne točke README za 50 trgov svoje mednarodne izdaje. Vsaka država ima eno datoteko z imenom `README.<CC>.md`; kjer je relevantnih več jezikov, so združeni v datoteki te države.

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

**Politika prevajanja:** Te datoteke so začetne lokalizirane državne vstopne točke. Njihovi uvodi in jezikovni naslovi so lokalizirani, popolna tehnična referenca pa ostaja v kanoničnem angleškem [README.md](../../README.md). Državne datoteke ni dovoljeno opisati kot popolnega prevoda, dokler ni preveden vsak razdelek, preverjeni tehnični identifikatorji in povezave ter opravljen jezikovni pregled izdaje. Jezikovna pokritost v tem prvem prehodu je reprezentativna in ni trditev, da so bili vsi ustavno priznani, regionalni, avtohtoni ali znakovni jeziki v celoti prevedeni.

## Dodatek — ohranjena regionalna navodila

Ta dodatek ohranja praktična navodila iz prejšnjega kratkega slovenskega vodnika, ki jih kanonični README nima kot samostojnih razdelkov. Pri vsakem neskladju ima prednost kanonični angleški [README.md](../../README.md).

### Cilji, profili in meje izvajanja

SLeeLa ločuje izvorno kodo od ciljev gradnje in izvajanja. Dokumentirana družina ciljev vključuje `SLVM`, `SLJVM` in `SLIR`; `NATIVE` je omejen cilj s pogoji in ni samoumevno na voljo na vseh platformah. Profila `BASIC_COMPLETE` in `ADVANCED_TOTAL` določata različen obseg. Pri vsaki gradnji preverite dejanski cilj, profil in podporo platforme.

Referenca identificira datoteko, modul ali vir, vendar sama po sebi ne dovoljuje njegovega izvajanja. Pri uporabi datotek, zunanjih knjižnic, native bindings ali sistemskih virov morajo biti meje pooblastil in odgovornost za izvajanje izrecno določene. Opis v dokumentaciji sam po sebi ni dokaz varnostnega jamstva runtime; preverite implementacijo in ustrezne teste.

### Življenjski cikel virov in odpravljanje napak

Dokumentirajte lastništvo, življenjsko dobo in odgovornosti za sinhronizacijo virov, ki si jih delijo funkcije ali niti. Ločujte local variables, shared object state, VM handles in serializable data. VM-local handle ni stabilna oblika datoteke ali omrežno sporočilo. Uporabite serializacijski vmesnik modula ter preizkusite invalid, missing in boundary values.

Pri odpravljanju napak zabeležite commit, OS, architecture in toolchain. Najprej zgradite najmanjši ustrezni cilj, shranite celoten izpis napak, ločite compiler errors od runtime errors in pripravite minimalen ponovljiv primer. Uspešna gradnja sama po sebi ne dokazuje pravilnega delovanja runtime.

Poročilo o preverjanju mora navesti exact commit, OS/architecture, compiler versions, vse build/test commands, število pass/fail/skip in znane omejitve. Preskočeni ali neizvedeni testi ne štejejo kot uspešni.

**Stanje prevoda:** Razdelki I–XIX so prevedeni v slovenščino. Tehnični identifikatorji, ukazi, primeri kode, imena datotek, povezave in naslovi so ohranjeni. Priporočen je končni jezikovni pregled, ki ga opravijo naravni govorci.
