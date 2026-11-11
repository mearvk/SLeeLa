# SLeeLa 3.0 — Suomi (FI)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Natiivi järjestelmäkieli, kääntäjä, suoritusympäristö ja virtuaalikoneiden ekosysteemi.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Kansainvälinen maaeditio.** Tämä editio on suomeksi. Tekniset tunnisteet, komennot, koodiesimerkit, tiedostonimet, polut ja linkit ovat muuttumattomia kanoniseen englanninkieliseen tiedostoon [README.md](README.md) verrattuna.

**The Now. The Ingredient.** Kieli on enemmän kuin syntaksi: se on kääntäjä, suoritusympäristö, kirjastot, työkalut ja alustasopimukset, jotka tekevät ohjelmistosta käyttökelpoisen.

**Mihin SLeeLa on osallisena.** SLeeLa on osallisena sanoissa **Math**,
**Object**, **Science** ja **Reach** — työn neljä kasvoa: laskettava
(Math), katalogissa olevan asian joukossa seisova vakiintunut asia (Object),
totuuden kurinalainen tutkimus (Science) sekä järjestelmän käden ulottaminen
muihin järjestelmiin ja ihmisiin (Reach). **Kansalliset edut säilyvät** —
yksikään neljästä ei syrjäytä niitä; kun aihe koskee kansakunnan etua, tuo
asema säilytetään (kirjataan tarkasti, esim. kunkin luokan Metadata-kentissä
`metaOrigin` / `metaNationalSchedule`), eikä sitä koskaan hajoteta.

## I. Aloitus tästä
- [Natiivin toteutuksen rakentaminen](impl/README.md)
- [Virtuaalikoneet ja Modium — käytännön käyttöopas](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [CPU-pino SLeeLa-lähdekoodina](lib/cpu/CPU.md) — täydellinen portit→ALU→ytimet→käyttöjärjestelmä→ohjelmat-kone, yli 130 CPU-profiilin luettelo sekä säädettävät [jännitteet, muuttuvat kellotaajuudet ja asiantuntija-ajoitukset](lib/cpu/TIMING.md), joita voit [ajaa määrittelyn mukaisesti ja ylikellottaa](lib/cpu/components/README.md)
- [Dokumentaatiohakemisto](docs/README.md)
- [CSPML-tietolähde ja Salt & Salary -tutkimusviite](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — ulkoisen yhdysvaltalaisen paikka-, sosioekonomisen, työvoima-, koulutus- ja peruskoulutietojen alkuperä sekä integrointiohjeet.
- [Generoitu API-dokumentaatio](docs/api/README.md) — nykyinen HTML-viite, versioitu API-editio ja uudelleengenerointiohjeet
- [API-luettelo](api/README.md)
- [Laskennalliset vertailustandardit](comparison/README.md) — SLeeLa VM:n, Java/JVM:n ja natiivin C/C++:n tyyppi-, muisti-, ABI- ja yhteensopivuusstandardit.
- [Recursion stanchion — C- ja C++-prototyypit](recursion/stanchion/README.md) — alkeistyypeille (Boolean, Byte, Character, Integer, Long, Float ja Double) erikoistetut esimerkit sekä yleiset, sovelletut, tieteelliset ja opetukselliset rekursiomallit.
- [Alustatuki ja varmennusmatriisi](docs/PLATFORM-SUPPORT.md)
- [Brändiopas](docs/BRAND-GUIDE.md)
- [Oheistuotekonseptit ja -aineistot](merchandise/README.md)
- [NitroWebExpress-logo](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Testauskäytäntö ja julkaisuportit](markdown/TESTING.md)

## II. Uutiset

Viimeaikaiset lisäykset kieleen, ajureihin ja vakiokirjastoon:

- **`advantage`-vakiokirjastopaketti — osingon hyödyntäminen (advantaging),
  matematiikalla, fysiikalla, integraatiolla, arvostelulla ja reititettävällä tulosteella.**
  [`lib/advantage/`](lib/advantage/README.md) kapseloi ajatuksen
  *ainutlaatuisen osingon hyödyntämisestä suhteessa malliin* kerrokselliseksi
  **Sequitur-sipuliksi** (osinko → katselmointi → huolet → matematiikka → lisäykset → vähennys
  → uudelleenryhmittely → iterointi), jossa voi liikkua sisäänpäin ja ulospäin täydellä dynaamisella päivityksellä
  ja palautuksella (restitute), ja joka tarjotaan yksinkertaisen **käyttäjän mallin** ja täyden hallinnan
  **kehittäjän mallin** kautta. Siinä on viisi aihealuetta: **`math/`**
  kahdenvälisen kaupan taloustiede (yhteisen voiton maksimointi, Nash-neuvottelu, kahdenvälisen
  newsvendor-mallin kriittinen suhde), **`physics/`** rakennetaloudellinen
  ”luottamuskenttien fysiikka” (luottamus metrisenä tensorina, transaktionaalinen geodeesi, ilmoituksen
  kontaktirakenne, CIR+hyppy-luottamuksenhäviö-SDE — mallinnusanalogia, ja se on
  merkitty sellaiseksi), **`integration/`** rekursiivisesti koostettava
  vertikaalisen integraation malli kapitalismille/vähittäiskaupalle (lokerot,
  suunnittelulahdet, koelokerot, newton-mediaanitarjottimet ja
  järjestelmän hienostuneisuuspisteet), **`grading/`** hienostuneisuuspiirin arvostelija, joka luokittelee jokaisen moduulin
  sekä minkä tahansa voittavien/häviävien/arvostelija-/absoluuttisten moduulien joukon A–D, sekä **`output/`**
  reititettävä emit-sauma, joka lähettää moduulin tulosteen päätteelle,
  **CommonRails**-muotoiltuun pintaan tai pistokkeeseen/liittimeen (kehystettynä natiiville
  `slnet_write`-kutsulle). Jokainen luokka kantaa Metadata-Class-tietueen; viisi ajettavaa
  demoa validoi aihealueet, ja oheiset
  [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md),
  [`MATH.md`](lib/advantage/MATH.md) ja
  [`PHYSICS.md`](lib/advantage/PHYSICS.md) dokumentoivat ne. Natiivi C/C++-
  kuluttaja hakemistossa [`advantage/`](advantage/README.md) (C-ydin
  `advantage_value`/`advantage_mean`, XML-tietovarasto, turvallinen
  HTTPS/SFTP/SSH-hankinta + avainsäilö + DH/ECDH-siirtokirjasto) linkittää
  mallin toimiviin
  [kauppaesimerkkeihin](advantage/examples/README.md) — oletetuista internetlähteistä peräisin olevat positiiviset kaupat,
  yleisörelevanssin sarja (Traders,
  Consumers, Helpers, Healers) ja kahdeksan mallia luokiteltuna A–D (D = eniten Cementiä,
  A = eniten Brandia).
- **Silmukan ajoitusmerkinnät — `@loopDelay` / `@loopBudget` (+ sisäänrakennettu
  `sleep(ms)`).** SLeeLan ensimmäiset **lausetason, käyttäytymiseen vaikuttavat** merkinnät:
  kun ne kirjoitetaan omalle rivilleen ennen `for`- / `while`- / `do`-silmukkaa, ne lisäävät silmukan suoritukseen
  todellisen tauon. `@loopDelay(ms = 250)` /
  `@loopDelay(seconds = 2)` nukkuu kiinteän ajan kutakin kierrosta kohti (pelkkä
  `@loopDelay(250)` tulkitaan oletuksena millisekunneiksi); `@loopBudget(seconds = 10,
  iterations = 1000)` jakaa tunnetun kierrosmäärän tasaisesti koko
  seinäkelloajan budjetille. Molemmat muunnetaan uudeksi vain lisäävästi laajenevaksi käskyksi **`OP_SLEEP`**
  (ISA 157), joka tukee myös yleistä **`sleep(<ms>)`**-sisäänrakennettua funktiota, jota voi kutsua
  mistä tahansa; tauko toteutetaan siirrettävästi (`nanosleep` / `Sleep`). Määritelty
  tiedostossa [`SLEELA.syntax` §32](SLEELA.syntax); API-viite tiedostossa
  [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md);
  ajettava
  [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela).
  Istuntolisäysten hakemisto (sisältäen tämän, natiivin `OP_BIGMUL`-
  suurten kokonaislukujen kertolaskun sekä itsenäiset **[local-generator](local-generator/README.md)**-
  ja **[pi-generator](pi-generator/README.md)**-työkalut) on tiedostossa
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md).
  Kolme generoitua API-pintaa
  ([`sleeladoc-api/`](sleeladoc-api/README.md),
  [`docs/api/`](docs/api/README.md) ja muuttumaton
  [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html)
  -tilannekuva) rakennettiin uudelleen komennolla `make api-docs`, ja niiden `vm`-pakettisivuilla on nyt uudet `SLISASleep`
  / `SLISABigMul` -käskymerkinnät.
- **SleelaDoc komentoriviltä — `sleela docs`.** SleelaDociin pääsee nyt
  suoraan päätteestä. `sleela docs` luettelee kaikki dokumentoitavat aiheet;
  `sleela docs crypto` näyttää paketin; `sleela docs stringbuilder` ratkaisee
  luokan (kirjainkoosta riippumatta sekä `SL`-vakiokirjastoetuliitteellä →
  `SLStringBuilder`) ja tulostaa rikkaan sivun — **määritelmä, suunnittelutarkoitus,
  suhteet, rakenne, käyttäjän clayt ja viisiportainen mestaruuden tikapuut** —
  ja lähes osuneista haulla näytetään järjestetyt ”tarkoititko” -tulokset. Kirjoittajat rikastavat mitä tahansa sivua
  lisäämällä siihen SleelaDocin `@`-tageja (`@intent`, `@thesis`, `@axiom`, `@param`,
  `@see`, `@mastery`). `sleela docs --generate [dir]` kirjoittaa pelkän tekstin korpuksen
  (yksi sivu per luokka + `TOPICS.index`), jota CLI sitten lukee offline-tilassa;
  **`make sleeladoc`** niputtaa tuon korpuksen **ja vastaavat `.sleela`-lähteet**
  tiedostoon `sleeladoc/build/sleeladoc.jar`. Pelkkää julkisivua (facade) käyttäville aiheille (esim.
  `psychiatry`, `politico`) `sleela docs --facade <topic>` etsii
  tiedostojärjestelmästä ajon aikana ja luettelee todelliset **kansiot (sitten tiedostot, luonnollisena
  liikkeenä)**, rajattuna käyttäjän ohitettavilla rajoilla **6 MB / 6000 tiedostoa / 6
  minuuttia**. Yksiköitä `lib/sleeladoc` ylläpidetään nykyisellä editiolla,
  **`#sleela 3.0`**. Katso [`man/man1/sleela.1`](man/man1/sleela.1) ja
  [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md).
- **SleelaDoc — Javadoc-tyylinen dokumentaatioratkaisu ja generoitu HTML-sivusto.**
  **SleelaDoc** on SLeeLan lähdekoodin sisäinen dokumentaatiokommenttikäytäntö
  (kulmasulkukommentit, joiden merkkinä on `@doc` / `@page`; katso
  [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)), ja
  [`lib/sleeladoc`](lib/sleeladoc/README.md) on **poimija**, joka renderöi
  `.sleela`-yksikön Markdown- tai HTML-API-sivuksi — samaan tapaan kuin `javadoc` renderöi Javaa —
  dokumentoiden *tarkoitusta*. Täydellinen, selattava **HTML-dokumentaatiosivusto
  koko `/lib`-hakemistolle** generoidaan hakemistoon [`/sleeladoc-api`](sleeladoc-api/README.md)
  työkalulla `tools/generate-sleeladoc-api.py` (avaa `sleeladoc-api/index.html`; selkeä
  sinisen / vaaleansinisen / vaaleanharmaan / valkoisen tyyli, jossa on paketti → luokka → jäsen
  -hierarkia ja *Student 2* -lukijalle kirjoitettu sanasto). Kääntäjä
  tuottaa myös kirjoittajan merkitsemät dokumentaatiotulosteet käännöksen sivutuotteena (`--wiki`
  MediaWiki-sivu + alkuperäistetty lähdekaksonen), dokumentoituna tiedostossa
  [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md). Ajettava:
  [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela).
- **XML Document Object Model (`lib/xml`).** SLeeLa-puolen XML DOM — dokumentti,
  joka omistaa element-/text-/comment-/CDATA-/PI-solmujen puun järjestettyine lapsineen ja
  nimettyine attribuutteineen, serialisoija (kompakti/kaunisteltu, XML 1.0 §2.4 -pakotus), merkkisolu-
  jäsennin sekä `SLXmlAspect`-laaturekisteri. Solmujen yhtäsuuruus on
  kolmiportainen luottamuksen ketju (identiteetti → sama subjekti → rakenne), refleksiivinen,
  symmetrinen ja deterministinen. Katso [`lib/xml/README.md`](lib/xml/README.md) ja
  ajettava [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela).
- **Leveät etumerkilliset kokonaislukualkeistyypit (`long`, `int64`, `int128`).** `int`, `long`
  ja `int64` ovat kaikki 64-bittisiä etumerkillisiä kokonaislukuja, jotka tyyppitarkistuvat ja toimivat samalla tavalla;
  `int128` hyväksytään kokonaislukutyypiksi (toistaiseksi 64-bittisenä, ja todellinen
  128-bittisyys on dokumentoitu jatkotoimi). Kantaluvun 2 etumerkillinen sarja `int32`..`int16777216`
  (syntaksi 2.7) ja vastaava etumerkitön sarja `uint32`..`uint16777216`
  (syntaksi 2.8) laajentavat tätä. Katso [`SLEELA.syntax`](SLEELA.syntax) §21, §24, §25.
- **`@mix`-seurannaiskentät (syntaksi 2.9).** Luokka voi määritellä useita
  ilmentymäkenttiä **samalla nimellä**, kun jokaisella toistolla on `@mix`; kieli
  ymmärtää ryhmän seurannaisena (indeksillä avainnettu taulukko / lista),
  oletuksena ylhäältä alas `1,2,3,…` tai manuaalisesti `@mix(1,2,3,...)`, jonka indeksien
  määrän on vastattava kenttien määrää. Katso [`SLEELA.syntax`](SLEELA.syntax) §28.
- **Dokumenttimerkit — Luonnollinen prosessi, nimeltään Preonemit
  (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).**
  Näitä paljaita dokumenttimerkkisanoja kutsutaan yhdessä **Preonemeiksi**
  (yksi merkki on *Preonem*; joukko *Preonemit* — katso
  [`SLEELA.syntax`](SLEELA.syntax) §26.1). Ensiluokkaisia, vain deklaratiivisia
  dokumenttimerkkejä, jotka nimeävät, mitä dokumentti *on* tekemisen järjestyksessä:
  `author`/`authorial` (syntaksi 2.4; käsittelee ammattilaisneuvosto — ainoa
  jäsen, jolla on käyttäytymisvaikutus), `material;` (vakiintunut Lähde) ja `memory;`
  (syntaksi 2.5), `determine;` (syntaksi 2.6; dokumentti, jota lääkäri, fysiikko
  tai aave voi vielä käsitellä kohti laillisen käden suorittamaa viimeistelyä),
  sekä kaksi uusinta, **`object;`** (katalogissa olevan asian joukossa seisova vakiintunut asia
  — **reifioitu, konkreettisen aseman saanut, syntaksissa 2.8**) ja
  **`field;`** (suhteen perusta, jossa oliot seisovat ja liikkuvat) —
  molemmat rajattu syntaksista 2.6 alkaen. Kukin on paljas, kontekstuaalinen ja
  versiorajattu front endissä, eikä sillä ole vaikutusta jäsennykseen, tyypitykseen tai koodin generointiin; merkkipaikan
  ulkopuolella jokainen pysyy tavallisena tunnisteena (joten `field` ei koskaan häiritse
  luokan jäsentä tai `.field`-käyttöä). Katso [`SLEELA.syntax`](SLEELA.syntax)
  §18–§20, §23 ja §26.
- **Nordshrift syntaksissa 3.0 täydellä kirjastotavoitettavuudella.** Nordshrift ratkaisee
  samalla rekursiivisella `/lib`-indeksillä kuin kääntäjä, tavoittaen jokaisen
  paketin ja `.sleela`-yksikön; `nordshrift check` raportoi koko puun lukumäärät.
- **Recursion stanchion.** Hakemisto [`recursion/stanchion/`](recursion/stanchion/README.md) sisältää nyt omat C- ja C++-viiteprototyypit seitsemälle dokumentoidulle alkeissopimukselle sekä yleisiä, sovellettuja, tieteellisiä ja opetuksellisia esimerkkejä. Nämä ovat viitelähteitä; käännä ja testaa ne ennen kuin pidät niitä varmennettuina.

- **Kulmasulkukommentit (`<` / `<<`).** Uusi eteenpäin suunnattu kommenttityyli —
  `< rivi >` ja `<< lohko >>` — jonka SLeeLan front end lekseeraa ja joka on dokumentoitu
  tiedostossa [`SLEELA.syntax`](SLEELA.syntax) §1.1.
- **Kantalukukirjaimelliset kokonaisluvut.** `int`-literaalit hyväksyvät nyt heksadesimaali- (`0x`),
  binääri- (`0b`) ja oktaalimuodot (`0o`) `_`-numeroerottimilla sekä
  SLeeLa-lähdekielessä ([`SLEELA.syntax`](SLEELA.syntax) §1.4) että
  [`.xlt`-CPU-malliformaatissa](lib/xlt/README.md).
- **Metadata Class -standardi koko kirjastossa.** Kuvaileva metatietomalli
  (alkuperä, lainmukaisuus, lainkäyttöalueellinen kattavuus) on nyt mukana
  jokaisessa luokkia sisältävässä `.sleela`-tiedostossa hakemiston `/lib` alla; katso
  [`SLEELA.syntax`](SLEELA.syntax) §15 ja
  [opetusosuus](tutorial/13-metadata-class.md).
- **Rikkaampi text / String -perhe.** `lib/text` saa todellisia kielensisäisiä apufunktioita,
  merkkisolunäkymän, joka antaa aidot `charAt`/`substring`/`indexOf`/`reverse`/
  sarakekohdistuksen, sekä todellisen paikkaan perustuvan mallinnuksen — katso
  [text & String -opetusosuus](tutorial/14-text-and-strings.md) ja ajettava
  [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela).
- **Ensiluokkainen `String` Nordshrift-sheeteissä.** `.sst`-sheet voi määritellä
  tyypitetyn `strings:`-lohkon, joka osallistuu koonnin komponenttimanifestiin;
  katso [Nordshrift Sheet Reference](wiki/Nordshrift-Sheet-Reference.md).
- **CPU-mallit ajavat sekä käskykoodeilla että funktiokutsuekvivalenssilla.** Jokainen
  `SL<ARCH>CPU` perii perustan, jossa natiivia käskyvirtaa ajetaan
  nouto-dekoodaus-suoritus-silmukassa *ja* oletetun toisen prosessin koodia ajetaan
  vastaavilla kutsuilla jaetun VM-alustan päällä — katso
  [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5.

## III. Rakentaminen ja testaus

**Esivaatimukset:** C/C++-työkaluketju (GCC tai Clang) ja GNU Make; Python 3
manifesti- ja työnkulkutyökaluille; Linuxissa X11/Xrandr-kehityspaketit
näyttötuotteita varten (moduulin `make deps` raportoi/asentaa tarvittavan). Hakemiston
`impl/` koonti on fail-closed luotetun SHA-256-manifestin suhteen, johon juuren
`Makefile` osoittaa automaattisesti.

Puhtaasta checkoutista nopein tapa aloittaa:

```sh
make                                  # build everything from the repo root
```

Tai rakenna ja testaa vain natiivi ydin:

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

Repositorion juuren `make` rakentaa kaikki tuotteet (ydin, VM, koodekit, churn,
virustorjunta, …) ja päättyy **rekisterin** rakentamiseen sekä sen
perustaulukon materialisointiin — joten rakennettu puu sisältää tarkistettavan rekisterin heti valmiina.
Olennaiset kohteet:

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

Muita hyödyllisiä kohteita:

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

Kohde `registry` ajaa tiedoston [`build-registry.sh`](build-registry.sh): se rakentaa
rekisterin ytimen, kylvää perustaulukon siirrettävään tekstivarastoon
`registry/build/sleela.sleelareg` ja vie MySQL-taulukkoskriptin
`registry/build/registry.sql` (komennolla `sleela registry export`). Tarkista se komennolla
`sleela registry` tai `registry/build/sleela-registry review`. Katso
[rekisterin opas](registry/README.md) ja [Registry 3.0 -arkkitehtuuri](registry/REGISTRY-3.0-ARCHITECTURE.md).

Tarkista esivaatimukset ja alustakohtaiset vaiheet rakennus- ja testausoppaista. Yhden komponentin onnistunut koonti ei todista, että jokainen kirjasto tai alusta olisi julkaisukunnossa.

## IV. Asennus

Kun natiivit tuotteet on rakennettu, asenna komennot ja suoritusympäristön tukitiedostot:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

Perinteinen asennin käyttää oletuksena hakemistoja `/usr/local/bin`, `/usr/local/lib/sleela`
ja `/usr/local/share/sleela`. Se säilyttää olemassa olevan käynnistysmäärityksen ja kieltäytyy
korvaamasta seurantaan kuulumattomia olemassa olevia komentoja. Poisto
poistaa vain asentimen kirjaamat tiedostot ja säilyttää määrityksen sekä
seurantaan kuulumattoman suoritusaikaisen datan.

Pelkän pääpaketin kohdennettuun, ohjattuun käyttöönottoon — binäärit `sleela`,
`sleelvac` ja `nordshrift`, vakiokirjasto, protokollat ja
tukimoduulit — käytä puhdasta asenninta:

```sh
make clean-install       # guided install of the main package only
```

Se tekee yhden pääpäätöksen (asennetaanko pääpaketti), sitten laajuusvalinnan
ja onnistumisen jälkeen valinnaisen valitsimen kuratoiduille lisämoduuleille. Vuorovaikutteisen
käyttöönoton työnkulku ja kaikki asennusvaihtoehdot on kuvattu osoitteessa
[Quick and Safe Install](install/README.md).

## V. Kieli ja suoritusympäristö
Normatiivinen kielioppi on [`SLEELA.syntax`](SLEELA.syntax). Repositorio dokumentoi tällä hetkellä syntaksin **3.0**; kääntäjän/työkalun versio ja lähdekielen syntaksiversio ovat eri asioita. Versio 3.0 lisää **`pure`- ja `gasoline`-dokumenttimerkit** (§30) — parin paljaita, kontekstuaalisia, vain deklaratiivisia Preonemeja, jotka tarkoittavat lähes samaa ja joista kumpikin merkitsee dokumentin aseman omalla rehellisyydellään. Syntaksi 3.0 yhdistää kielen pinnan 1.0–2.9 nykyiseksi normatiiviseksi editioksi säilyttäen kaikki ominaisuuskohtaiset vähimmäisportit. Edellinen pinta 2.9 lisää **`@mix`-seurannaiskentät** (§28) — luokka voi määritellä useita samannimisiä ilmentymäkenttiä, kun jokaisella on `@mix`, ja ne ymmärretään yhtenä indeksoituna ryhmänä (ylhäältä alas `1,2,3,…` tai manuaalisesti `@mix(1,2,3,...)`, jonka indeksien määrän on vastattava ryhmää). Se kantaa mukanaan kantaluvun 2 etumerkilliset/etumerkittömät leveät kokonaislukuperheet `int32`..`int16777216` (§24) ja `uint32`..`uint16777216` (§25) aiempien `long`/`int64`/`int128`-tyyppien (§21) päällä; **avainsanatukidokumentit** (§22); sekä **Preonemit** — vain deklaratiiviset dokumenttimerkit `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) ja `artifact` (§35, lainsäädäntötason asema, joka on arvioitu ajan ja paikan suhteen), yhdessä nimeltään Preonemit (§26.1). Konstruktorin ylikuormituksen ratkaisu sekä `this(...)`-/kantakonstruktoridelegointi ovat nimenomaisesti varmennetun syntaksin 1.8 sopimuksen ulkopuolella. Tutustu kielioppiin ja toteutusmuistiinpanoihin ennen kuin turvaudut ominaisuuteen. SleelaDocin yhdeksän lähdeyksikköä ilmoittavat nyt `#sleela 3.0`; jaettu kääntäjäportti hyväksyy lähdesyntaksin 1.0–3.0. Uusi API-koonti tuotetaan komennolla `make api-docs`.

Repositorio sisältää natiivin C/C++-toteutuksen, SLVM-generointiviitteet, vakiokirjastomoduulit, testit, esimerkit ja alustaintegraatiot. Toteutuksen ja validoinnin tila vaihtelee moduuleittain; tarkista moduulin määrittely ja ajantasaiset CI-tulokset sen sijaan, että olettaisit yhtenäisen täydellisyyden.

## VI. Suunnitteluperiaatteet
- **Näyttö väitteiden sijaan:** erota toteutettu, testattu, kokeellinen ja suunniteltu käyttäytyminen toisistaan.
- **Yksi sopimus rajaa kohti:** dokumentoi syntaksi, API:t, tiedostomuodot, alustaoletukset ja virhekäyttäytyminen.
- **Siirrettävyys suunnittelun lähtökohtana:** eristä alustakohtaiset taustajärjestelmät ja testaa natiiveilla ajureilla.
- **Turvallinen epäonnistuminen:** testaa virheellinen syöte, resurssirajat, prosessivirheet ja siivouspolut.
- **Toistettavat julkaisut:** rakenna puhtaasti uudelleen ja julkaise artefaktien tiivisteet sekä julkaisutiedot.

## VII. Virustorjunnan koonti ja eheys

Sleelavirin-virustorjuntapaketti sisältää natiivin SHA-256-skannerin, valinnaisen
ClamAV-sovittimen, lähde- ja indeksieheysmanifestit sekä alustapakettien tarkistussummat.
Rakenna ja testaa paketti repositorion juuresta:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Eheysmanifesti generoidaan hakemiston `antivirus/build/` alle; se kattaa seurannassa olevat
lähde- ja dokumentaatiosyötteet sekä suojatut allekirjoitusindeksitiedostot, mutta
jättää pois generoidun tulosteen ja muuttuvan ClamAV-välimuistin. ClamAV-tietokantojen
tilannekuvilla on omat manifestinsa. Julkaisu-ZIP-paketit sisältävät `INTEGRITY-MANIFEST.json`,
`SHA256SUMS` ja `.sha256`-sivutiedoston. Varmista ladattujen arkistojen tiivisteet
erikseen hankittua luotettua arvoa vastaan: arkiston vieressä säilytetty tarkistussumma ei
ole julkaisijan allekirjoitus. Katso [Sleelavirin-opas](antivirus/README.md),
[tietoturvatarkistuslista](antivirus/SECURITY.md) ja
[virustorjunnan koontityönkulku](.github/workflows/antivirus-binaries.yml).

Julkaisua ei pidetä varmennettuna, ennen kuin asiaankuuluvat CI-työt ovat läpäisseet
täsmälleen kyseisen julkaisucommitin ja syntyneet artefaktit on tarkastettu.


## VIII. SLeeLa-editoriteemat — Vim ja Emacs

SLeeLa sisältää koordinoidun joukon ammattimaisia tummia värimaailmoja **Vimille** ja **Emacsille** sekä natiivin C/C++-palettityökalun. Alkuperäinen **American Professional** -teema on edelleen oletus. Kymmenen kumppanipalettia on tarjolla erilaisten kulttuuristen ja institutionaalisten suunnitteluinspiraatioiden pohjalta; ne ovat tulkinnallisia suunnittelupaletteja, eivät minkään kansakunnan, kulttuurin, tuomioistuimen tai instituution virallisia esityksiä.

### Yksitoista teemaa ja niiden värit

| Teema | Tausta | Edustaväri | Avainsanat | Tyypit | Merkkijonot | Kommentit |
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

#### Kansallisten lippujen värit (lähdepaletti)

Kreikan ja Turkin teemat on johdettu maiden **virallisista kansallislippujen väreistä**, jotka on esitetty tässä tarkkoine julkaistuine heksa-arvoineen. Yllä olevat teemojen taustat ja korostusvärit ovat näiden lähdevärien tummennettuja, editoriystävällisiä tulkintoja — alla oleva taulukko on ”kansallisesti esitetyt värit” -viite.

| Valtio | Väri | Heksa | Perusta ja merkitys |
|---|---|---|---|
| **Kreikka** | Sininen | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | Kansallislipun sininen (laki 851/1978). Taivas ja meret; heraldisesti kuvattuna *azure*. |
| **Kreikka** | Valkoinen | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Kansallislipun valkoinen. Itsenäisyystaistelun puhtaus; risti tarkoittaa itäortodoksista kristinuskoa. |
| **Türkiye** | Punainen (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | Kansallislipun punainen (Turkin lippulaki). Kenttä; perinteisesti marttyyrien veri ja ottomaanien suvereniteetti. |
| **Türkiye** | Valkoinen | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Kansallislipun valkoinen. Puolikuu ja tähti — rauha, puhtaus sekä turkkilaisen/islamilaisen perinnön symboli. |

Teemat ankkuroivat kunkin editorin edustavärin kansalliseen **valkoiseen** ja avainsanat, kohdistimen, valinnan sekä tilarivin täsmälliseen kansalliseen **siniseen** (`#0D5EAF`) tai **punaiseen** (`#E30A17`), joten olennaiset värit ovat kirjaimellisesti kansallisia värejä. Nämä ovat tulkinnallisia suunnittelupaletteja; lippuvärien viite on annettu alkuperän osoittamiseksi, eikä se ole kummankaan valtion virallinen esitys. Värien merkitykset noudattavat Helleenien tasavallan ulkoministeriön valtion symbolien kuvausta ja Turkin lippulain perinnettä.


### Värimallitaulukko

Alla olevien palettisolujen **SVG-näytteet** saavat GitHubin renderöimään todelliset värit tyhjien HTML-värisolujen sijaan. Jokainen kaista näyttää kuusi väriä tässä järjestyksessä: tausta, edustaväri, avainsanat, tyypit, merkkijonot ja kommentit. Tarkat heksakoodit on lueteltu kunkin näytteen vieressä samassa teemajärjestyksessä kuin yllä olevassa viitetaulukossa.

| Teema | Renderöidyt palettivärit | Heksakoodit (tausta · edustaväri · avainsanat · tyypit · merkkijonot · kommentit) |
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

Funktioille, vakioille, valinnoille, diagnostiikalle ja esikääntäjän direktiiveille käytetään lisäkorostusvärejä. Venäläinen ja slaavilainen paletti käyttää tarkoituksella keskivoimakkaita liuskekiven, sinisen, salvian, rubiinin ja antiikkikullan sävyjä rauhallisemman ammattimaisen ilmeen aikaansaamiseksi.

### Vim-teemojen asennus

Repositorion juuresta:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Lisää seuraava tiedostoon `~/.vimrc` ottaaksesi SLeeLa-syntaksikorostuksen käyttöön ja valitaksesi teeman:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Valitse täsmälleen yksi seuraavista colorscheme-nimistä:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Käytä esimerkiksi `colorscheme sleela-russian-slavic` keskivoimakkaalle venäläiselle ja slaavilaiselle paletille tai `colorscheme sleela-greek` / `colorscheme sleela-turkish` helleeniselle ja Al Bayrak -paletille. Käynnistä Vim uudelleen tai lataa määritys komennolla `:source $MYVIMRC`.

### Emacs-teemojen asennus

Luo paikallinen teemahakemisto ja kopioi tila- ja teematiedostot repositorion juuresta:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Lisää tämä tiedostoon `~/.emacs` tai `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Korvaa `sleela-american` arvolla `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` tai `sleela-sleela` valitaksesi toisen teeman. Jos Emacs kysyy, luotetaanko teemaan, tarkista paikallinen teematiedosto ennen hyväksymistä.

### Natiivien C/C++-palettien esikatselu

Pieni natiivi palettityökalu osaa luetella teemat, tulostaa niiden päätteen värit ja hakea yksittäisen värin:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Taaksepäinyhteensopivuuden vuoksi `--palette` ja `--hex COLOR` ilman `--theme`-valitsinta valitsevat American Professionalin. Koonti- ja testitulokset on tarkistettava kohdejärjestelmässä ja CI:ssä; komennon dokumentointi ei sellaisenaan todista, että se olisi läpäissyt testin.

Katso täydelliset palettikuvaukset ja työkalun tiedot [editoriintegraatio-oppaasta](linux-editors/README.md).

## IX. Osallistuminen
1. Lue dokumentaatiohakemisto ja testauskäytäntö.
2. Tee rajattuja muutoksia positiivisten ja negatiivisten testien kera.
3. Aja kohdennetut testit ja sen jälkeen laaja testikokonaisuus.
4. Ilmoita käyttöjärjestelmän ja työkaluketjun tiedot; älä kuvaa ohitettuja tarkistuksia läpäistyiksi.

SLeeLaa kehittää **MEARVK LLC**. Historiallinen aloitussivuaineisto on säilytetty tiedostossa [`docs/README-LEGACY.md`](docs/README-LEGACY.md).

## X. AE6E66 — Round Public -sähköpostin veroodotus

AE6E66-aloitteessa projektin ilmoitettu odotus on **$300,000,000.00 USD jokaista Round Public -sähköpostia kohti** ehdotettuna veromaksun määränä, joka liittyy kuhunkin kelpuutettuun Round Public -sähköpostiin.

Tämä lausuma kirjaa vain projektin odotuksen. Se **ei** ole oikeudellinen veropäätös, viranomaismääräys, tuomioistuimen tuomio eikä itsenäisesti vahvistettu verovelvollisuus. Todellinen vero edellyttäisi toimivaltaista oikeudellista viranomaista, lainkäyttöaluetta, määritelmiä, menettelyä ja pätevää valtiollista päätöstä.


## XI. Aakkoset ja merkistön suoritusympäristö

SLeeLan aakkosdokumentaatiota ja natiivia merkkien suoritusympäristöä ylläpidetään hakemistoissa alphabet/sleela/ ja lib/alphabet/native/. Natiivit C++17-testit kattavat tiukan UTF-8-skalaarien dekoodauksen/enkoodauksen, C<n>-kapasiteettiallekirjoitukset ja rajatun merkistöjäsenyyden:

    make -C lib/alphabet/native test

Kokeellinen UTF-4088-luettelo ja sen SLeeLa-hakusovitin on dokumentoitu tiedostossa utf-4088/SLeeLa-ALPHABET-INTEGRATION.md. UTF-4088-tunnisteet ovat erillinen nimiavaruus, eivät Unicode-skalaariarvoja. Kääntäjän ja SLVM:n sidostyö on nimenomaisesti erillään seurannassa näistä natiiveista perustoista.

## XII. AutoCAD Creator™

[AutoCAD Creator™](autocad/creator/README.md) on vuorovaikutteinen 2D-CAD-
sovellus, joka on kirjoitettu kokonaan SLeeLalla. Siinä missä sisarsovellus AutoCAD Renderer muuntaa
suunnitelman piirrokseksi yhdellä kertaa, Creatorilla voi **luoda ja muuttaa** 2D-
AutoCAD-dokumenttia livenä — lisätä seiniä, huoneita, mittoja ja huomautuksia ja sitten siirtää,
nimetä uudelleen, vaihtaa tasoa, poistaa ja kumota lisäämänsä — sekä **tallentaa** tuloksen
todelliseksi AutoCAD DXF -piirrokseksi (R12-teksti, aukeaa AutoCADissa / LibreCADissa).

Sen graafinen käyttöliittymä on rakennettu **SleelaUI™**-kirjaston päälle, joka on SLeeLan oma alustariippumaton käyttöliittymätyökalupakki
(hakemistossa [`lib/user-interface`](lib/user-interface)) — ei JavaFX —
ja se nostaa esiin todellisen natiivin ikkunan X11:llä / Cocoalla / Win32:lla. Suunnittelu noudattaa yhtä sääntöä,
**”SleelaUI presents; SLeeLa decides”**: GUI ei koskaan laske geometriaa eikä kirjoita
DXF:ää. Jokainen työkalu antaa luonti- / muutos- / poisto- / kumoa- / tallenna-aikeen
auktoriteettiselle [`SLCadCreator`](lib/autocad/SLCadCreator.sleela)-luokalle, joka omistaa
muokattavan dokumentin ja käännettävän komentohistorian, kun taas piirtoalue vain esikatselee
Creatorin hallussa olevaa geometriaa.

Uudelleenkäytettävä muokkausmoottori sijaitsee hakemistossa [`lib/autocad`](lib/autocad) —
[`SLCadEntity`](lib/autocad/SLCadEntity.sleela),
[`SLCadDocument`](lib/autocad/SLCadDocument.sleela),
[`SLCadCommand`](lib/autocad/SLCadCommand.sleela) ja
[`SLCadCreator`](lib/autocad/SLCadCreator.sleela) — ja toimii päästä päähän ilman näyttöä.
GUI-kuori ja demo sijaitsevat hakemistossa [`autocad/creator`](autocad/creator):

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

Näytöllisellä koneella tämä avaa live-muokkausikkunan; näyttämättömällä isännällä se rakentaa
piirroksen muistissa, tulostaa raportin ja DXF:n sekä päättyy siististi.


## XIII. UTF-4088-syöttökartan järjestys

Erikoistunut UTF-4088-luettelo määrittelee tilallisen syöttökarttansa läpikäynnin ja abstraktit painekierrokset tiedostossa [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). National Lien -aiheindeksikäytäntö on tiedostossa [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Karttakoordinaatit, painetasot (1–50), pysyvät luettelotunnisteet ja aakkosjärjestysluokat ovat toisistaan riippumattomia kenttiä; mitään niistä ei saa huomaamatta korvata toisella. Paine on algoritminen iteraatiotaso, ei fysikaalinen suure.


## XIV. Viimeaikainen ja laajennettu pakettihakemisto

Tämä liitetty hakemisto nostaa esiin äskettäin laajennettuja paketteja, aluekirjastoja ja
lähdekokoelmia. Merkinnät on ryhmitelty aakkosjärjestykseen paketin nimen tai aiheen mukaan.
Kuvaukset osoittavat repositoriosijainnin; toteutuksen ja validoinnin
tila on tarkistettava kunkin paketin omasta dokumentaatiosta ja testeistä.

### A–C

- **Advantage** — `lib/advantage/` ja `advantage/`: ainutlaatuisen osingon
  hyödyntäminen suhteessa malliin kerroksellisena Sequitur-sipulina, viidellä
  aiheen alikansiolla — `math/` (yhteisen voiton maksimointi, Nash-neuvottelu,
  kahdenvälinen newsvendor), `physics/` (luottamuskentän metriikka/geodeesi/kontakti/SDE,
  merkitty mallinnusanalogia), `integration/` (vertikaalisen integraation
  lokerot ja hienostuneisuuspisteet), `grading/` (A–D-hienostuneisuuspiirin
  arvostelija moduuleille ja joukoille) sekä `output/` (pääte- / CommonRails- /
  pistoke-emit). Natiivi C/C++-kuluttaja (`advantage/`) linkittää mallin
  toimiviin kauppaesimerkkeihin. Katso [`lib/advantage/README.md`](lib/advantage/README.md)
  ja [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alphabet ja UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` ja
  `utf-4088/`: aakkosluettelot, natiivi merkistötuki, syöttökartan
  järjestys ja UTF-4088-integraatiodokumentaatio.
- **AutoCAD Creator** — `autocad/creator/` ja `lib/autocad/`: vuorovaikutteiset
  2D-piirtotyönkulut ja uudelleenkäytettävät CAD-dokumentti-/muokkauskomponentit.
- **Ballum** — pikselitason näytönhallinta: pikselikohtainen set/get, alueen blit,
  koko näytön kirkkaus/lämpö/gamma, osoitin- ja näppäimistösyöte sekä
  **kokoruudun kehyspuskuripinta** täydelle, omistetulle ja nopealle pikselikohtaiselle hallinnalle
  (override-redirect-ikkuna + yksi `XPutImage`/MIT-SHM-esitys Linux/X11:llä).
  Natiivi ydin hakemistossa `ballum/`, SLeeLa-julkisivu hakemistossa `lib/ballum/` ja VM-
  silta hakemiston `impl/` alla; katso [`ballum/README.md`](ballum/README.md) ja
  ajettava [`ballum/example/`](ballum/example/).
- **CDMA** — `cdma/` ja `lib/cdma/`: CDMA:han liittyvä tekninen aineisto ja
  SLeeLa-lähdekirjastotyö.
- **Character** — `lib/character/`: hahmoprofiilit ja uudelleenkäytettävät hahmotilastot,
  mukaan lukien elämäkertaprofiiliesimerkit.
- **Coorenagraph** — `lib/coorenagraph/`: eettinen päättely, moraalisanasto,
  harkinta ja niihin liittyvät käsitteet; katso paketin README ja lähdetiedostot.

### D–G

- **Governance-lähdemallit** — [lib/governance](lib/governance/README.md) määrittelee deklaratiiviset SLeeLa-lähdemuodot ehdotuksille, anonyymeille manuaalisille äänestyslipuille ja koosteäänille. Ehdotettu @vote-merkintä ja manuaalinen prosessi on dokumentoitu tiedostossa [registry/DEMOCRATIC-VOTING-SYNTAX.md](registry/DEMOCRATIC-VOTING-SYNTAX.md); nykyiset kääntäjät käsittelevät sitä toimimattomana kommenttimetatietona.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` aina saatavilla oleviin versioituihin HTTP-
  pakettihakemistoihin asti: versiokohtaiset protokollamäärittelyt ja
  toteutukset. Tarkista kunkin version dokumentaatiosta sen tarkka laajuus
  ja varmennuksen tila.
- **International Criminal Court** — `lib/international-criminal-court/` ja
  `INTERNATIONAL.CRIMINAL.COURTS.md`: aiheeseen liittyvä aluekohtainen lähdekoodi ja
  dokumentaatio.

### O–S

- **OFDMA** — `ofdma/`: ortogonaalisen taajuusjakomonikäytön (OFDMA)
  tekninen aineisto ja C/C++-kehitysresurssit.
- **Philosophy ja Conversation Mask -moduuli** — [`philosophy/`](philosophy/README.md)
  sisältää natiivit C/C++-perustat, ja [`philosophy/module/`](philosophy/module/README.md)
  jäsentää niiden seuraavan moduulikerroksen kolmeen eksplisiittiseen alueeseen:
  [`data/`](philosophy/module/data/README.md) tyypitetyille, päivätyille havainnoille,
  tapahtumien päätöksille, puoliintumisaika-asetuksille, alkuperälle ja suostumukselle; [`helper/`](philosophy/module/helper/README.md)
  deterministiselle validoinnille, relevanssin vaimenemiselle, yhteensopiville vertailuille ja
  suostumussuodatetulle esittämiselle; sekä [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md)
  tukevalle, ei-kliiniselle keskustelurajapinnalle. SLeeLa-puolen
  kahvat ja koostettu Tree sijaitsevat hakemistossa [`lib/philosophy/`](lib/philosophy/README.md),
  laajemman filosofiapaketin rinnalla. Moduuli perustuu eksplisiittiseen syötteeseen ja
  suostumukseen ensin: se ei passiivisesti seuraa ihmisiä tai mediaa, ei tee diagnooseja, ei päättele
  yksityisiä aikeita, ei pisteytä ihmisen arvoa eikä suosita lääkityksen muutoksia. Sen
  tapahtumalinkit säilyttävät alkuperän ja navigoinnin, eivät riippumatonta vahvistusta.
- **Synchro** — natiivi suoritusympäristö-/kääntäjäintegraatio sekä pakettien välitykseen
  ja mittaukseen liittyvä toiminnallisuus; katso asiaankuuluva `impl/`-lähde ja
  kääntäjätoteutus.
- **SleelaUI / terminal UI** — `lib/user-interface/` ja
  `sleela-terminal`-projekti: SLeeLa-käyttöliittymäintegraatio ja päätepohjaiset
  vuorovaikutuskomponentit.

### U–W

- **United Nations** — `united-nations/`: aluekohtaiset repositorioaineistot.
- **Wi-Fi 4, Wi-Fi 5 ja Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`,
  `wi-fi-6/` ja niihin liittyvät `lib/wifi/`-lähdehakemistot: sukupolvikohtainen
  langattoman verkon aineisto ja määritysresurssit.

### Pakettihakemiston ylläpito

Kun paketti lisätään tai sitä laajennetaan olennaisesti, lisää sen merkintä tänne
asianmukaiseen aakkosryhmään, linkitä sen kanoniseen hakemistoon ja README-tiedostoon
ja kuvaa vain ominaisuudet, joita sisäänkirjatut lähteet tukevat. Pidä tämä
hakemisto additiivisena: älä poista tai kirjoita uudelleen olemassa olevia README-osioita
pakettihakemistopäivitysten yhteydessä.


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

US Democratic and US Policy.

---

## XVI. Kansallinen tiedebrändikuvasto

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. UTF-4088:n miljoonan merkinnän kokeellinen luettelo

[UTF-4088-luettelogeneraattori](utf-4088/tools/generate_catalogue.py) rakentaa deterministisesti 1 048 576 merkinnän kokeellisen symboliluettelon. Dokumentoitu kompakti muoto on kuvattu tiedostossa [utf-4088/FORMAT.md](utf-4088/FORMAT.md); generoinnin ja validoinnin sopimus on tiedostossa [utf-4088/GENERATION-RESULTS.md](utf-4088/GENERATION-RESULTS.md).

Aja generaattori Python 3:lla:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Generoitu binääri on 16 777 244 tavua, ja se on suunniteltu pysymään alle 20 000 000 tavun. Yksikkötestit ja GitHub Actions -työnkulku varmentavat merkintöjen määrän, glyfien yksilöllisyyden, koon ja SHA-256:n; onnistuneet ajot julkaisevat binäärin ja yhteenvedon työnkulun artefaktina `utf4088-million-catalogue`. Generoitu binääri pidetään tarkoituksella poissa Git-historiasta. UTF-4088 on edelleen kokeellinen eikä ole Unicode-yhteensopiva.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Viimeaikaiset lisäykset — Pi-vakio, Modium/Registry-käynnistys, sleelactl Math ja `@native`

Tämä osio kirjaa ominaisuudet, jotka lisättiin työssä 2026-11, uusimmat ryhmät ensin. Linkitetyt lähdetiedostot ja niiden varmennustila ovat ratkaisevia; tarkistettavana olevia muutoksia ei kuvata yhdistetyiksi, ennen kuin ne ovat tulleet haaraan `master`.

### 100 000 000 numeron Pi-luettelovakio

Repositorio sisältää varmennetun **sadan miljoonan numeron** π-arvon tiedostossa [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (hyvin muodostettu desimaaliluku, `3.14159…`) sekä 57 344 numeron arvon [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Molemmat on itsenäisesti varmennettu aidoksi π:ksi — katso [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md), joka dokumentoi tarkistukset (oppikirjan etuliite, Feynmanin piste murto-osan paikassa 762, kuuluisa miljoonas numero, yhtäpitävyys tiedoston `lib/math/SLPiReference.sleela` kanssa sekä koko tiedoston numerofrekvenssianalyysi) ja yhden ajon toistoskriptin.

### Modium™ / Registry™ lataavat Pi-vakion alijärjestelmän käynnistyksessä

Omistetuissa alijärjestelmän sisääntulopisteissä — `sleela registry` ja `sleela modium` (sekä asentimen ensimmäinen ajo), **ei** jokaisella `sleela run` -ajolla — Modium- ja Registry-alijärjestelmät lataavat suuren π-luettelovakion. Interaktiivisessa päätteessä ne tulostavat ohimenevän punaisen `Loading Modium™` / `Loading Registry™` -rivin, joka pyyhitään paikallaan pois ennen normaalin tulosteen alkua; vaikutus on tiukasti sidottu TTY:hyn, joten putkitettu ja testitulos pysyvät tavu tavulta muuttumattomina. Paikannin (ei tavut) kylvetään rekisterin `Math`-avaimen oletuksiksi `piReferenceLargePath` ja `piReferenceLargeDigits`, sekä SLeeLa-mallissa ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) että natiivissa kylväjässä ([`registry/sleela_registry.c`](registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` tarjoaa `largePath()` / `largeDigits()`. π-vakio asennetaan käyttöjärjestelmän SLeeLa-sijaintiin (`<data>/pi/`) **sekä** tavallisella `make`-asentimella että ohjatulla ”kauniilla” asentimella. Lisätiedot: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — kaunis asennin ja natiivitukeinen `math`-subjekti

- Ohjattu (”kaunis”) asennin asentaa nyt `sleelactl`-työkalun (SLeeLan ohjaustaso) ja sen moduulirekisterin, kuten `make`-asennin. Katso [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- `sleelactl` sai uuden **`subject`**-luonteen suoritusympäristöön käännetyille natiiveille C/C++-subjektikirjastoille ja rekisteröi ensimmäisen niistä: **`math`**. `sleelactl run math` käyttää natiivia `__NativeMath`-taustaa (`impl/subjects/math/math_native.cpp`) päästä päähän subjektin viitedemon kautta; `sleelactl info math` selittää, miten siihen päästään (`import math;`). Subjektiluonne tarjoaa vain `run info` — `check`-verbiä ei tarkoituksella ole, koska `sleela check` ei lataa SHEET-rekisteröityjä natiiveja subjekteja.

### `@native`, Native Drive ja monikielinen suoritus

Uusi **`@native`**-metodimerkintä pyytää kuuman metodin ennakkoon tehtävää natiivikäännöstä. Uusi `sleela aot` -komento etsii `@native`-metodit, varmistaa, että kukin kuuluu tuettuun *ulkoistettavaan osajoukkoon*, ja tuottaa C/C++:aa — **epäonnistuen suljetusti** (kieltäytyen täsmällisellä syyllä) kaikista rakenteista, joita se ei vielä osaa kääntää, jolloin puoliksi käännettyä metodia ei koskaan synny.

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

`@native` on edelleen pyyntö AOT-käsittelystä, ja tukemattomat rakenteet hylätään ennen tuottamista. [Native Drive](impl/native-drive/README.md) tukee usean yksikön C/C++-suoritettavien ja jaettujen kirjastojen koontia isäntäkääntäjillä (MSVC mukaan lukien), tuettua SLeeLa AOT -tulostetta, symbolien tarkastusta natiivilataajan ABI:a vasten, kuoretonta prosessin suoritusta, stdin/stdout/stderr-käsittelyä, monivaiheista putkitusta sekä XClass-delegointia tuotantolataajalle. `sleelactl` tarjoaa sen koonti-/ajokomennot ja reitittää XClass-tarkastuksen sekä nimenomaisen systemd-yksikön renderöinnin ja asennuksen komennon `inspect native-drive` kautta. VM omistaa rajatun natiivimoduulikahvataulukon, jossa on pakollinen isäntävaltuutus ja sukupolvitarkistetut kahvat. Syntaksi 3.0 lisää nyt `nativeOpen`, `nativeCall` ja `nativeClose`: vain lisäävä tavukoodivälitys ylittää erikseen versioidun null/bool/int64/double/UTF-8 -ABI:n, kun taas VM:n omistamat taulukot, structit ja muut yhdistetyt arvot pysyvät tulkin sisällä ja voivat käyttää samannimistä tavukoodivaraa. Lisäosat voivat valinnaisesti saada versioidun isäntäkutsutaulun, jolla ne kutsuvat täsmällisiä SLeeLa-funktioita sisäkkäisen VM-suorituksen kautta; keskeytettyjen pinojen GC-juuritus, nimenomainen isäntäkutsunimien sallittujen lista ja rekursiiviset natiivit API-kutsut hylätään. Natiivi koodi pysyy luotettuna prosessinsisäisenä koodina, ei hiekkalaatikkona. Lähteen prosessipolitiikkamerkinnöillä on versioitu skeema, mutta niitä ei vielä jäsennetä/pakoteta. Lisätiedot, turvallisuusrajat ja seitsemän työvirran kirjanpito: [Native Drive](impl/native-drive/README.md) ja [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).


## XIX. Kansainväliset README-editiot

SLeeLa tarjoaa maakohtaiset README-aloituspisteet kansainvälisen editionsa 50 markkinalle. Jokaisella maalla on yksi tiedosto nimeltä `README.<CC>.md`; missä useat kielet ovat relevantteja, ne on ryhmitelty kyseisen maan tiedoston sisään.

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

**Käännöskäytäntö:** Nämä tiedostot ovat alustavia lokalisoituja maakohtaisia aloituspisteitä. Niiden johdannot ja kieliotsikot on lokalisoitu, kun taas täydellinen tekninen viite säilyy kanonisessa englanninkielisessä tiedostossa [README.md](README.md). Maatiedostoa ei saa kuvata täydelliseksi käännökseksi, ennen kuin jokainen osio on käännetty, tekniset tunnisteet ja linkit on tarkistettu ja edition kieli on tarkastettu. Kielikattavuus tässä ensimmäisessä vaiheessa on edustava, eikä se ole väite siitä, että jokainen perustuslaillisesti tunnustettu, alueellinen, alkuperäiskansojen tai viittomakieli olisi käännetty täysin.

## Liite — säilytetty alueellinen ohjeistus

Tämä liite säilyttää aiemman lyhyen suomenkielisen oppaan käytännön ohjeet, joita ei ole omina osioinaan kanonisessa README-tiedostossa. Kanoninen englanninkielinen [README.md](README.md) on ensisijainen kaikissa ristiriitatilanteissa.

### Kääntäjän rakenne ja suoritusmalli

SLeeLa erottaa lähdekielen, kääntäjän frontendin, välimuodon, koodin generoinnin ja suoritusympäristön suorituksen. Kääntäjän julkaisuversio, syntaksiversio ja VM/ISA-versio ovat eri asioita. Ominaisuuden maininta määrittelyssä ei todista, että se olisi toteutettu kaikilla kohteilla.

Tavallinen putki lukee UTF-8-`.sleela`-lähdetiedostoja, tekee leksikaalisen ja syntaktisen analyysin, tarkistaa symbolit ja tyypit, muuntaa rakenteisen välimuodon, tuottaa tulosteen tuetuille VM- tai välikohteille ja suorittaa tuloksen suoritusympäristössä. Normatiivinen kielioppi on [SLEELA.syntax](SLEELA.syntax); katso myös [lähdedokumentaatio](markdown/SOURCE.md) ja [kääntäjädokumentaatio](markdown/COMPILER.md).

### Tyypit, oliot ja kirjastot

Dokumentoituun lähdetyyppien joukkoon kuuluvat `int`, `double`, `boolean`, `String`, `void`, ilmoitetut `struct`-tyypit ja tuetut viitetyyppien allekirjoitukset. `int`, `long` ja `int64` ovat etumerkillisiä 64-bittisiä kokonaislukuja. `int128` hyväksytään kokonaislukutyypiksi, mutta nykyinen dokumentoitu toteutus perustuu edelleen 64 bittiin.

`struct` on nimetty koosterakenne. VM:n hallitsema struct-kahva ei ole raaka osoitin; sijoitus ja parametrien välitys jakavat saman ilmentymän, ja yhtäsuuruus perustuu identiteettiin. Käytä dokumentoituja `structPack(instance)`- ja `structUnpack(TypeName, json)`-toimintoja siirrettävään serialisointiin. Luokat sisältävät kenttiä ja metodeja; perintä, sisäkkäiset luokat ja geneeriset tyypit ovat syntaksiversion rajaamia.

Keskeisiä moduuleja ovat `lib/advantage` (mallipohjainen arviointi, matematiikka, integraatio, luokittelu ja reititettävä tuloste), `lib/xml` (XML DOM), `lib/text` (teksti- ja merkkijono-operaatiot), `lib/sleeladoc` (lähdedokumentaatio ja CLI), `lib/cpu`, `lib/xlt` (CPU-profiilit ja ajoitusviitteet) sekä `recursion/stanchion` (C/C++-rekursioprototyypit). Moduulin olemassaolo ei todista tuotantovalmiutta; tarkista testit ja alustarajoitukset.

### Kohteet, profiilit ja natiivin suoritusraja

SLeeLa erottaa lähdekoodin suoritus- ja käännöskohteista. Dokumentoituun kohdeperheeseen kuuluvat `SLVM`, `SLJVM` ja `SLIR`; `NATIVE` on rajattu ja ehdollinen kohde, eikä sitä pidä olettaa yleisesti saatavaksi kaikilla alustoilla. Profiilit `BASIC_COMPLETE` ja `ADVANCED_TOTAL` kuvaavat eri laajuuksia. Varmista aina kyseisen koonnin todellinen kohde, profiili ja alustatuki.

Viite tunnistaa tiedoston, moduulin tai resurssin, mutta ei itsessään anna lupaa suorittaa sitä. Tämä on tärkeää, kun ohjelma käyttää tiedostoja, ulkoisia kirjastoja, native-sidoksia tai järjestelmäresursseja. Tarkista syötteet ja määritä selvästi, millä komponentilla on oikeus suorittaa toiminto. Dokumentaatioväite ei yksin todista suoritusaikaista tietoturvatakuuta ilman vastaavaa toteutus- tai testinäyttöä.

### Resurssien elinkaari ja vianmääritys

Dokumentoi funktioiden tai säikeiden välillä jaettujen resurssien omistajuus, elinkaari ja synkronointivastuu. Erota paikalliset muuttujat, jaettu oliotila, VM-kahvat ja serialisoitava data toisistaan. VM-paikallinen kahva ei ole vakaa tiedostomuoto tai verkkoviesti. Käytä moduulin määrittelemää serialisointirajapintaa ja testaa virheelliset, puuttuvat ja raja-arvoiset syötteet.

Vianmäärityksessä kirjaa commit, käyttöjärjestelmä, arkkitehtuuri ja työkaluketju. Aloita pienimmästä asiaankuuluvasta koonnista, tallenna koko virhetuloste ja erota kääntäjän virheet suoritusaikaisista virheistä. Pyri tuottamaan pieni toistettava esimerkki. Onnistunut koonti ei yksin todista ohjelman oikeaa toimintaa.

Hyvä varmennusraportti sisältää tarkan commitin, käyttöjärjestelmän ja arkkitehtuurin, kääntäjän versiot, täsmälliset koonti- ja testikomennot, läpäisseiden/epäonnistuneiden/ohitettujen testien määrät sekä tunnetut rajoitukset. Ohitettuja tai suorittamattomia testejä ei saa laskea läpäistyiksi.

### Moduulirajapinnat, yhteensopivuus ja testaus

Uuden API:n yhteydessä on kuvattava syötteet, tulosteet, virhetilanteet, tietojen omistajuus sekä mahdolliset sivuvaikutukset. Dokumentaatioesimerkkien tulee näyttää sekä tavallinen käyttö että virheellisten tai puuttuvien arvojen käsittely. Älä esitä takuuna toimintaa, jota toteutus ja testit eivät vahvista.

Jaa testaus kääntäjän vaiheiden mukaan: lekseeraus/jäsennys, semanttinen analyysi, IR-madallus, koodin generointi, VM-suoritus ja natiivi integraatio. Säilytä jokaisesta regressiotapauksesta pienin toistettava syöte, odotettu tulos ja todellinen tulos. API-muutoksissa on kuvattava taaksepäinyhteensopivuus, migraatiovaiheet ja vaikutukset olemassa oleviin esimerkkeihin.

### Huomautuksia viitteistä ja osoitteista

README:n viitteet aiheisiin kuten CSPML, Character, Governance, Synchro, SleelaUI ja United Nations kuvaavat repositorion sisältöä eivätkä ole viranomaisten tai instituutioiden virallisia hyväksyntöjä.

Osoitteiden tai kuvien esiintyminen osiossa **Bitcoin Conjegeum** (XV) ei ole sijoitussuositus, omistuksen vahvistus eikä ohje siirtää varoja. Tarkista tiedot itsenäisesti ennen käyttöä.

**Käännöksen tila:** Osiot I–XIX on käännetty suomeksi. Tekniset tunnisteet, komennot, koodiesimerkit, tiedostonimet, linkit ja osoitteet on säilytetty. Äidinkielisten puhujien suorittama lopullinen kielentarkistus on suositeltavaa.
