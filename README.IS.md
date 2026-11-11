# SLeeLa 3.0 — Ísland (IS)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Innbyggt kerfisforritunarmál, þýðandi, keyrsluumhverfi og vistkerfi sýndarvéla.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Alþjóðleg landsútgáfa.** Þessi útgáfa er á íslensku. Tæknileg auðkenni, skipanir, kóðadæmi, skráarnöfn, slóðir og tenglar eru óbreytt frá hinu staðlaða enska [README.md](README.md).

**The Now. The Ingredient.** Tungumál er meira en setningafræði: það er þýðandinn, keyrsluumhverfið, söfnin, verkfærin og samningar um vettvang sem gera hugbúnað nothæfan.

**Hvað SLeeLa tekur þátt í.** SLeeLa tekur þátt í **Math**, **Object**,
**Science** og **Reach** — fjórum hliðum starfs þess: hinu reiknanlega
(Math), hinum afgreidda hlut sem stendur í skrá yfir það sem er (Object),
hinni agaðri rannsókn á hinu sanna (Science) og útréttri hönd kerfis til
annarra kerfa og fólks (Reach). **Þjóðarhagsmunir haldast** — enginn þessara
fjögurra þátta setur þá til hliðar; þegar viðfangsefni snertir hagsmuni þjóðar
er sú staða varðveitt (skráð af trúmennsku, t.d. í `metaOrigin` /
`metaNationalSchedule` í lýsigögnum hvers flokks), aldrei leyst upp.

## I. Byrjaðu hér
- [Smíðaðu innbyggðu útfærsluna](impl/README.md)
- [Sýndarvélar og Modium — hagnýt notendahandbók](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [CPU-stafli í SLeeLa-frumkóða](lib/cpu/CPU.md) — heil vél frá rökhliðum → ALU → kjarna → stýrikerfi → forrit, skrá yfir 130+ CPU-snið og stillanleg [spenna, breytileg klukka og sérfræðitímasetningar](lib/cpu/TIMING.md) sem þú getur [keyrt samkvæmt forskrift og yfirklukkað](lib/cpu/components/README.md)
- [Skjalayfirlit](docs/README.md)
- [CSPML-gagnalind og tilvísun í Salt & Salary-rannsókn](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — upprunaskráning og leiðbeiningar um samþættingu ytri bandarískra gagna um staði, félags- og efnahagsmál, vinnuafl, menntun og opinbera skóla.
- [Mynduð API-skjöl](docs/api/README.md) — núverandi HTML-viðmiðun, útgáfubundin API-útgáfa og leiðbeiningar um endurmyndun
- [API-skrá](api/README.md)
- [Samanburðarstaðlar fyrir útreikninga](comparison/README.md) — staðlar fyrir gerðir, minni, ABI og samræmi hjá SLeeLa VM, Java/JVM og innbyggðu C/C++.
- [Recursion stanchion — C- og C++-frumgerðir](recursion/stanchion/README.md) — dæmi sérhæfð fyrir frumgerðirnar Boolean, Byte, Character, Integer, Long, Float og Double, ásamt almennum, hagnýtum, vísindalegum og kennslutengdum endurkvæmnimynstrum.
- [Stuðningur við vettvang og sannprófunartafla](docs/PLATFORM-SUPPORT.md)
- [Vörumerkjahandbók](docs/BRAND-GUIDE.md)
- [Söluvöruhugmyndir og gögn](merchandise/README.md)
- [NitroWebExpress-merki](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Prófunarstefna og útgáfuhlið](markdown/TESTING.md)

## II. Fréttir

Nýlegar viðbætur við málið, rekla og staðalsafn:

- **`advantage`-staðalsafnspakkinn — að „advantage-a“ arðgreiðslu, með
  stærðfræði, eðlisfræði, samþættingu, einkunnagjöf og framsendanlegu úttaki.**
  [`lib/advantage/`](lib/advantage/README.md) hjúpar *að advantage-a einstaka
  arðgreiðslu með tilliti til líkans* sem lagskipta **Sequitur-lauk**
  (arðgreiðsla → yfirferð → áhyggjuefni → stærðfræði → viðbætur → frádráttur
  → endurflokkun → ítrun), sem hægt er að rata um inn á við og út á við með
  fullri kvikri uppfærslu og endurheimt, og er aðgengilegur í gegnum einfalt
  **líkan notandans** og **líkan þróunaraðilans** með fullri stjórn. Hann
  geymir fimm viðfangssvið: **`math/`** tvíhliða viðskiptahagfræði (sameiginleg
  hagnaðarhámörkun, Nash-samningagerð, mikilvægt hlutfall tvíhliða
  newsvendor-líkansins), **`physics/`** byggingarhagfræðilega „eðlisfræði
  traustsviða“ (traust sem metrískur tensor, viðskiptageódesían,
  tilkynningarsnertibygging, CIR+stökk SDE fyrir traustrýrnun — líkanssamlíking,
  merkt sem slík), **`integration/`** endurkvæmt samsetjanlegt líkan lóðréttrar
  samþættingar fyrir kapítalisma/smásölu (hólf, verkfræðiflóar, tilraunahólf,
  newton-miðgildisbakkar og einkunn fyrir fágun kerfis), **`grading/`**
  fágunarrásar-einkunnagjafa sem metur hverja einingu og hvaða safn
  sigurvegara/taparaeininga/gjafaeininga/algildra eininga sem er á kvarðanum
  A–D, og **`output/`** framsendanlegt úttakssamskeyti sem sendir úttak
  eininga á skjáinn, á **CommonRails**-sniðið yfirborð eða á
  tengil/innstungu (rammað fyrir innbyggt `slnet_write`). Hver flokkur ber
  Metadata-Class-færsluna; fimm keyranleg sýnidæmi sannreyna viðfangssviðin og
  fylgiskjölin [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md),
  [`MATH.md`](lib/advantage/MATH.md) og
  [`PHYSICS.md`](lib/advantage/PHYSICS.md) lýsa þeim. Innbyggður C/C++-notandi
  undir [`advantage/`](advantage/README.md) (C-kjarninn
  `advantage_value`/`advantage_mean`, XML-þekkingargeymsla, örugg
  HTTPS/SFTP/SSH-öflun + lyklageymsla + DH/ECDH-flutningssafn) tengir líkanið
  við útfærð
  [viðskiptadæmi](advantage/examples/README.md) — jákvæð viðskipti frá
  gefnum netheimildum, röð sniðin að áheyrendum (Traders, Consumers, Helpers,
  Healers) og átta líkön raðað A–D (D = mest Cement, A = mest Brand).
- **Lykkjutímasetningar-merkingar — `@loopDelay` / `@loopBudget` (+ innbyggt
  fall `sleep(ms)`).** Fyrstu **setningastigs-, hegðunar**merkingarnar í
  SLeeLa: þær eru skrifaðar á sérlínu á undan `for` / `while` / `do`-lykkju og
  setja raunverulega bið inn í keyrslu lykkjunnar. `@loopDelay(ms = 250)` /
  `@loopDelay(seconds = 2)` sefur fastan tíma í hverri ítrun (berskjölduð
  `@loopDelay(250)` miðast sjálfgefið við millisekúndur);
  `@loopBudget(seconds = 10, iterations = 1000)` dreifir þekktum fjölda ítrana
  jafnt yfir heildartímaáætlun á vegg-klukku. Báðar lækkast í nýju
  viðbótaraðgerðakóðana **`OP_SLEEP`** (ISA 157), sem stendur einnig að baki
  almennu innbyggðu falli **`sleep(<ms>)`** sem hægt er að kalla á hvar sem er;
  biðin er útfærð færanlega (`nanosleep` / `Sleep`). Skilgreint í
  [`SLEELA.syntax` §32](SLEELA.syntax); API-viðmiðun í
  [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md);
  keyranlegt
  [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela).
  Yfirlit yfir viðbætur lotunnar (þar á meðal þessa, innbyggðu
  stórtölumargföldunina `OP_BIGMUL` og sjálfstæðu verkfærin
  **[local-generator](local-generator/README.md)** og
  **[pi-generator](pi-generator/README.md)**) er í
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md).
  API-yfirborðin þrjú sem mynduð eru
  ([`sleeladoc-api/`](sleeladoc-api/README.md),
  [`docs/api/`](docs/api/README.md) og óbreytanlega
  [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html)
  skyndimyndin) voru endurbyggð með `make api-docs` og bera nú nýju
  `SLISASleep` / `SLISABigMul`-skipanafærslurnar á `vm`-pakkasíðu sinni.
- **SleelaDoc af skipanalínu — `sleela docs`.** Nú er hægt að ná í SleelaDoc
  beint úr skelinni. `sleela docs` listar öll skjalanleg viðfangsefni;
  `sleela docs crypto` sýnir pakka; `sleela docs stringbuilder` leysir upp
  flokk (án tillits til há-/lágstafa og með `SL`-forskeyti staðalsafnsins →
  `SLStringBuilder`) og prentar ríkulega síðu — **skilgreining, hönnunartilgang,
  tengsl, byggingu, notendaleir og fimm þrepa leikni-stiga** — með röðuðum
  „áttirðu við“-niðurstöðum þegar næstum hittist. Höfundar auðga hverja síðu
  einfaldlega með því að bæta við SleelaDoc-`@`-merkjum (`@intent`, `@thesis`,
  `@axiom`, `@param`, `@see`, `@mastery`). `sleela docs --generate [dir]`
  skrifar hreinan textaforða (eina síðu fyrir hvern flokk + `TOPICS.index`)
  sem skipanalínan les síðan án nettengingar; **`make sleeladoc`** pakkar þeim
  forða **og samsvarandi `.sleela`-frumkóða** í
  `sleeladoc/build/sleeladoc.jar`. Fyrir viðfangsefni sem eingöngu eru framhlið
  (t.d. `psychiatry`, `politico`) leitar `sleela docs --facade <topic>` í
  skráakerfinu á keyrslutíma og listar raunverulegar **möppur (síðan skrár,
  sem eðlilega hreyfingu)**, afmarkað af mörkum sem notandi getur breytt:
  **6 MB / 6000 skrár / 6 mínútur**. `lib/sleeladoc`-einingarnar eru
  viðhaldið í núverandi útgáfu, **`#sleela 3.0`**. Sjá
  [`man/man1/sleela.1`](man/man1/sleela.1) og
  [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md).
- **SleelaDoc — skjalalausn að hætti Javadoc, með myndaðri HTML-vefsíðu.**
  **SleelaDoc** er skjalaathugasemdavenja SLeeLa í frumkóða
  (hornklofaathugasemdir með vörðunum `@doc` / `@page`; sjá
  [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)) og
  [`lib/sleeladoc`](lib/sleeladoc/README.md) er **útdráttarverkfærið** sem
  birtir `.sleela`-einingu sem Markdown- eða HTML-API-síðu — á sama hátt og
  `javadoc` birtir Java, með því að skjalfesta *ásetning*. Heil,
  vafranleg **HTML-skjalavefsíða fyrir alla `/lib`** er mynduð í
  [`/sleeladoc-api`](sleeladoc-api/README.md) með
  `tools/generate-sleeladoc-api.py` (opnaðu `sleeladoc-api/index.html`; hreinn
  stíll í bláu / ljósbláu / ljósgráu / hvítu með stigveldi pakki → flokkur →
  meðlimur og orðasafni skrifuðu fyrir lesandann *Student 2*). Þýðandinn
  sendir einnig frá sér höfundarmerkt skjalaúttak sem aukaafurð þýðingar
  (`--wiki` MediaWiki-síða + upprunninn frumkóðatvíburi), skjalfest í
  [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md). Keyranlegt:
  [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela).
- **XML-skjalahlutalíkan (`lib/xml`).** XML DOM SLeeLa-megin — skjal sem á tré
  af element-/texta-/athugasemda-/CDATA-/PI-hnútum með röðuðum börnum og nefndum
  eigindum, raðgerðari (þéttur/fallegur hamur, XML 1.0 §2.4 flóttar), 
  stafahólfa-þáttara og gæðaskránni `SLXmlAspect`. Jafngildi hnúta er þriggja
  þrepa trausts-jafntefli (auðkenni → sama viðfang → bygging), sjálfhverft,
  samhverft og ákvarðandi. Sjá [`lib/xml/README.md`](lib/xml/README.md) og
  keyranlega [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela).
- **Víðar 64-bita formerktar heiltölu-frumgerðir (`long`, `int64`, `int128`).**
  `int`, `long` og `int64` eru allar 64-bita formerktar heiltölur sem gerðatékkast
  og keyra eins; `int128` er samþykkt sem heiltölugerð (að svo stöddu studd af
  64 bitum, með sönnu 128-bita sem skjalfest framhaldsverkefni). Tvíundar-2
  formerkta röðin `int32`..`int16777216` (málskipan 2.7) og samsvarandi
  óformerkta röðin `uint32`..`uint16777216` (málskipan 2.8) auka þetta. Sjá
  [`SLEELA.syntax`](SLEELA.syntax) §21, §24, §25.
- **`@mix` afleidd svið (málskipan 2.9).** Flokkur má lýsa yfir mörgum
  tilviksreitum með **sama nafni** ef hver endurtekning ber `@mix`; málið
  skilur hópinn sem afleiddan (fylki / lista með vísi), ofanfrá og niður
  `1,2,3,…` sjálfgefið eða handvirkt með `@mix(1,2,3,...)` þar sem fjöldi vísa
  þarf að passa við fjölda reita. Sjá [`SLEELA.syntax`](SLEELA.syntax) §28.
- **Skjalamerki — hið náttúrulega ferli, titlað Preonems
  (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).**
  Þessi berskjölduðu skjalamerkisorð eru sameiginlega titluð **Preonems**
  (eitt merki er *Preonem*; safnið *Preonems* — sjá
  [`SLEELA.syntax`](SLEELA.syntax) §26.1). Fyrsta flokks skjalamerki, eingöngu
  yfirlýsandi, sem nefna hvað skjal *er* í röð gerðar: `author`/`authorial`
  (málskipan 2.4; meðhöndlað af faglegu ráði — eini meðlimurinn með
  hegðunaráhrif), `material;` (afgreitt Source) og `memory;` (málskipan 2.5),
  `determine;` (málskipan 2.6; skjal opið til frekari vinnslu af lækni,
  lyflækni eða draugi í átt að lúkningu með löglegri hendi), og tvö hin nýjustu,
  **`object;`** (afgreiddur hlutur í skrá yfir það sem er — **raungerður, gefin
  áþreifanleg staða, í málskipan 2.8**) og **`field;`** (grunnur tengsla þar
  sem hlutir standa og hreyfast) — bæði hliðuð frá málskipan 2.6. Hvert er
  berskjaldað, samhengisbundið og útgáfuhliðað af framendanum án áhrifa á
  þáttun, tegundagreiningu eða kóðagerð; utan merkisstöðu er hvert þeirra
  áfram venjulegt auðkenni (svo `field` truflar aldrei flokksmeðlim eða
  `.field`-aðgang). Sjá [`SLEELA.syntax`](SLEELA.syntax)
  §18–§20, §23 og §26.
- **Nordshrift í málskipan 3.0 með fullri ná til safna.** Nordshrift leysir
  upp í gegnum sömu endurkvæmu `/lib`-vísitöluna og þýðandinn og nær til hvers
  pakka og hverrar `.sleela`-einingar; `nordshrift check` skilar heildartölum
  alls trésins.
- **Recursion stanchion.** Skráin [`recursion/stanchion/`](recursion/stanchion/README.md) inniheldur nú sérstakar C- og C++-viðmiðunarfrumgerðir fyrir samningana sjö um frumgerðir sem skjalfestir eru, auk almennra, hagnýtra, vísindalegra og kennslutengdra dæma. Þetta eru viðmiðunarfrumkóðar; þýddu þá og prófaðu áður en þeir eru taldir sannprófaðir.

- **Hornklofaathugasemdir (`<` / `<<`).** Nýr athugasemdastíll héðan í frá —
  `< lína >` og `<< blokk >>` — lesinn af SLeeLa-framendanum og skjalfestur í
  [`SLEELA.syntax`](SLEELA.syntax) §1.1.
- **Heiltölustaðvirði í mismunandi grunntölum.** `int`-staðvirði taka nú við
  sextándakerfi (`0x`), tvíundarkerfi (`0b`) og áttundakerfi (`0o`) með `_`
  sem tölustafaskilju, bæði í SLeeLa-frumkóðamálinu
  ([`SLEELA.syntax`](SLEELA.syntax) §1.4) og í
  [`.xlt` CPU-líkanasniðinu](lib/xlt/README.md).
- **Metadata Class-staðallinn um allt safnið.** Lýsigagnamynstrið
  (uppruni, lögmæti, lögsagnarumdæmi) er nú borið af hverri `.sleela`-skrá með
  flokkum undir `/lib`; sjá [`SLEELA.syntax`](SLEELA.syntax) §15 og
  [kennslustundina](tutorial/13-metadata-class.md).
- **Ríkulegri texta- / String-fjölskylda.** `lib/text` fær raunveruleg
  hjálparföll í málinu, stafahólfasýn sem veitir ósvikin
  `charAt`/`substring`/`indexOf`/`reverse`/dálkajöfnun og raunverulegt
  staðsetningarbundið sniðmátskerfi — sjá
  [texta- og String-kennsluna](tutorial/14-text-and-strings.md) og keyranlega
  [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela).
- **Fyrsta flokks `String` í Nordshrift-blöðum.** `.sst`-blað getur lýst yfir
  tegundaðri `strings:`-blokk sem leggur til íhlutaskrá smíðinnar; sjá
  [Nordshrift Sheet Reference](wiki/Nordshrift-Sheet-Reference.md).
- **CPU-líkön keyrð bæði með aðgerðakóðum og jafngildi fallakalla.** Hvert
  `SL<ARCH>CPU` erfir þann grunn að keyra innbyggða skipanarunu í gegnum
  sækja-afkóða-framkvæma-lykkju sína *og* keyra kóða ætlaðs annars ferlis með
  jafngildum köllum á sameiginlega VM-undirlaginu — sjá
  [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5.

## III. Smíði og prófanir

**Forkröfur:** C/C++-verkfærakeðja (GCC eða Clang) og GNU Make; Python 3 fyrir
manifest-/flæðiverkfærin; á Linux X11/Xrandr-þróunarpakkar fyrir
skjávörurnar (`make deps` einingar greinir frá / setur upp það sem hún þarf).
Smíði `impl/` er lokuð við bilun (fail-closed) gagnvart traustu SHA-256-manifesti,
sem `Makefile` í rót vísar sjálfkrafa á.

Hraðasta byrjun úr hreinni útteknu afriti:

```sh
make                                  # build everything from the repo root
```

Eða smíðaðu og prófaðu aðeins innbyggða kjarnann:

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

Make-skipunin í rót hugbúnaðarsafnsins smíðar sérhverja vöru (kjarna, VM,
kóðunarsöfn, churn, vírusvörn, …) og lýkur með því að smíða **registry** og
efnisgera grunntöflu þess — þannig fylgir smíðuðu tré yfirferðarhæft registry
beint úr kassanum. Mikilvægustu markmiðin:

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

Önnur gagnleg markmið:

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

`registry`-markmiðið keyrir [`build-registry.sh`](build-registry.sh): það
smíðar registry-kjarnann, sáir grunntöflunni í færanlegu textageymsluna
`registry/build/sleela.sleelareg` og flytur út MySQL-töfluskriftuna
`registry/build/registry.sql` (með `sleela registry export`). Yfirfarðu það
með `sleela registry` eða `registry/build/sleela-registry review`. Sjá
[registry-handbókina](registry/README.md) og [Registry 3.0-arkitektúr](registry/REGISTRY-3.0-ARCHITECTURE.md).

Lestu smíða- og prófunarleiðbeiningarnar um forkröfur og skref sem eru háð vettvangi. Vel heppnuð smíði eins íhlutar sannar ekki að sérhvert safn eða vettvangur sé tilbúinn til útgáfu.

## IV. Uppsetning

Eftir að innbyggðu vörurnar hafa verið smíðaðar skaltu setja upp skipanirnar og stuðningsskrár keyrsluumhverfisins:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

Hefðbundna uppsetningarforritið notar sjálfgefið `/usr/local/bin`,
`/usr/local/lib/sleela` og `/usr/local/share/sleela`. Það varðveitir
fyrirliggjandi ræsistillingar og neitar að skrifa yfir fyrirliggjandi skipanir
sem eru ekki raktar. Afuppsetning fjarlægir aðeins skrár sem uppsetningarforritið
skráði og varðveitir stillingar og órakin keyrslugögn.

Fyrir einbeitta, leiðbeinda uppsetningu á aðalpakkanum einum — tvíundunum
`sleela`, `sleelvac` og `nordshrift`, staðalsafninu, samskiptareglunum og
stuðningseiningunum — notaðu hreina uppsetningarforritið:

```sh
make clean-install       # guided install of the main package only
```

Það krefst einnar aðalákvörðunar (að setja upp aðalpakkann), síðan vals á
umfangi og að því loknu valfrjáls veljari fyrir valdar viðbótareiningar. Fyrir
gagnvirka uppsetningarferlið og alla uppsetningarvalkosti, sjá
[Hröð og örugg uppsetning](install/README.md).

## V. Mál og keyrsluumhverfi
Opinbera málskipanin er [`SLEELA.syntax`](SLEELA.syntax). Núverandi hugbúnaðarsafn skjalfestir málskipan **3.0**; útgáfa þýðanda/verkfæris og útgáfa frumkóðamálskipanar eru aðskildar. Útgáfa 3.0 bætir við **skjalamerkjunum `pure` og `gasoline`** (§30) — pari berskjaldaðra, samhengisbundinna Preonems, eingöngu yfirlýsandi, sem þýða nánast það sama og merkja hvort um sig stöðu skjalsins með eigin heiðarleika. Málskipan 3.0 sameinar yfirborð málsins 1.0–2.9 í núgildandi staðalútgáfu og varðveitir öll lágmarkshlið einstakra eiginleika. Fyrra 2.9-yfirborðið bætir við **`@mix` afleiddum sviðum** (§28) — flokkur má lýsa yfir mörgum samnefndum tilviksreitum þegar hver ber `@mix`, skilið sem einn vísaður hópur (ofanfrá og niður `1,2,3,…` eða handvirkt `@mix(1,2,3,...)` þar sem fjöldi vísa þarf að passa við hópinn). Það heldur áfram með tvíundar-2 formerktu/óformerktu víðu heiltölufjölskyldurnar `int32`..`int16777216` (§24) og `uint32`..`uint16777216` (§25) ofan á fyrri `long`/`int64`/`int128` (§21); **lykilorðastuðningsskjöl** (§22); og **Preonems** — eingöngu yfirlýsandi skjalamerkin `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) og `artifact` (§35, staða á löggjafarstigi flokkuð eftir tíma og rúmi), sameiginlega titluð Preonems (§26.1). Yfirhleðsluupplausn smiða og framsal `this(...)`/grunnsmiðs eru sérstaklega utan staðfests samnings málskipanar 1.8. Skoðaðu málskipanina og útfærsluathugasemdir áður en þú reiðir þig á eiginleika. Níu frumkóðaeiningar SleelaDoc lýsa nú yfir `#sleela 3.0`; sameiginlega þýðandahliðið tekur við frumkóðamálskipan 1.0–3.0. Nýja API-smíðin er framleidd með `make api-docs`.

Hugbúnaðarsafnið inniheldur innbyggða C/C++-útfærslu, viðmiðanir um SLVM-myndun, staðalsafnseiningar, prófanir, dæmi og samþættingar við vettvanga. Útfærslu- og sannprófunarstaða er breytileg eftir einingum; styðstu við forskrift hennar og núverandi CI-niðurstöður frekar en að gera ráð fyrir einsleitri fullkomnun.

## VI. Verkfræðilegar meginreglur
- **Gögn framar fullyrðingum:** greindu á milli útfærðrar, prófaðrar, tilraunakenndrar og fyrirhugaðrar hegðunar.
- **Einn samningur á hver mörk:** skjalfestu málskipan, API, skráarsnið, forsendur um vettvang og hegðun við bilun.
- **Færanlegt í hönnun:** einangraðu vettvangsháð bakendi og prófaðu á innbyggðum keyrurum.
- **Örugg bilun:** prófaðu gallað inntak, auðlindamörk, ferlabilanir og hreinsunarleiðir.
- **Endurtakanlegar útgáfur:** endursmíðaðu hreint og birtu tætigildi afurða og útgáfuathugasemdir.

## VII. Vírusvarnarsmíði og heilleiki

Vírusvarnarpakkinn Sleelavirin inniheldur innbyggðan SHA-256-skanna, valfrjálsan
ClamAV-millistykki, heilleikamanifest fyrir frumkóða/vísitölur og
yfirsummur vettvangspakka. Smíðaðu og prófaðu pakkann úr rót hugbúnaðarsafnsins:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Heilleikamanifestið er myndað undir `antivirus/build/`; það nær yfir raktan
frumkóða og skjalainntak auk varinna undirskriftavísitöluskráa, en undanskilur
myndað úttak og breytanlegt ClamAV-skyndiminni. Skyndimyndir ClamAV-gagnagrunna
hafa eigin manifest. Útgáfu-ZIP innihalda `INTEGRITY-MANIFEST.json`,
`SHA256SUMS` og `.sha256`-hliðarskrá. Sannreyndu tætigildi sóttra skjalasafna
gegn traustu gildi sem fengið er óháð: yfirsumma sem geymd er við hlið skjalasafns
er ekki undirskrift útgefanda. Sjá [Sleelavirin-handbókina](antivirus/README.md),
[öryggisgátlistann](antivirus/SECURITY.md) og
[smíðaflæði vírusvarnar](.github/workflows/antivirus-binaries.yml).

Útgáfa telst ekki sannprófuð fyrr en viðeigandi CI-verk standast á nákvæmlega
útgáfu-commitinu og afurðirnar sem af því hljótast hafa verið athugaðar.


## VIII. SLeeLa ritlaþemu — Vim og Emacs

SLeeLa inniheldur samræmt safn faglegra dökkra litaþema fyrir **Vim** og **Emacs**, auk innbyggðs C/C++-litaveituforrits. Upprunalega þemað **American Professional** er áfram sjálfgefið. Tíu fylgipalettur eru í boði, innblásnar af ólíkri menningar- og stofnanahönnun; þetta eru túlkandi hönnunarpalettur, ekki opinber framsetning neinnar þjóðar, menningar, dómstóls eða stofnunar.

### Þemun ellefu og litir þeirra

| Þema | Bakgrunnur | Forgrunnur | Lykilorð | Gerðir | Strengir | Athugasemdir |
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

#### Þjóðfánalitir (upprunapaletta)

Gríska og tyrkneska þemað eru leidd af **opinberum þjóðfánalitum** landanna tveggja, sýndum með nákvæmum útgefnum sextándagildum. Bakgrunnar og áherslulitir þemanna hér að ofan eru dekktar túlkanir á þessum upprunalitum, sniðnar að ritlum — taflan hér að neðan er viðmiðunin „litir eins og þeir eru birtir af þjóðunum“.

| Þjóð | Litur | Hex | Grunnur og merking |
|---|---|---|---|
| **Grikkland** | Blár | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | Blár litur þjóðfánans (lög 851/1978). Himinninn og höfin; í skjaldarmerkjafræði nefndur *azure*. |
| **Grikkland** | Hvítur | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Hvítur litur þjóðfánans. Hreinleiki sjálfstæðisbaráttunnar; krossinn táknar grísk-rétttrúnaðarkristni. |
| **Türkiye** | Rauður (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | Rauður litur þjóðfánans (tyrknesku fánalögin). Flöturinn; samkvæmt hefð blóð píslarvotta og fullveldi Ottómana. |
| **Türkiye** | Hvítur | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Hvítur litur þjóðfánans. Hálfmáninn og stjarnan — friður, hreinleiki og tákn tyrkneskrar/íslamskrar arfleifðar. |

Þemun festa forgrunn hvers ritils á þjóðarlitinn **hvítan** og lykilorð, bendil, val og stöðulínu á nákvæmlega þjóðarlitinn **bláan** (`#0D5EAF`) eða **rauðan** (`#E30A17`), þannig að mikilvægu litirnir eru bókstaflega þjóðarlitirnir. Þetta eru túlkandi hönnunarpalettur; fánalitaviðmiðunin er veitt til að sýna uppruna og er ekki opinber framsetning neins ríkis. Merking litanna fylgir lýsingu utanríkisráðuneytis Hellenska lýðveldisins á ríkistáknum og hefð tyrknesku fánalaganna.


### Litasýnistafla

Palettureitirnir hér að neðan nota **SVG-sýni**, þannig að GitHub birtir raunverulega liti í stað tómra HTML-litareita. Hver rönd sýnir litina sex í þessari röð: bakgrunnur, forgrunnur, lykilorð, gerðir, strengir og athugasemdir. Nákvæm sextándagildi eru talin upp við hlið hvers sýnis, í sömu þemaröð og í viðmiðunartöflunni hér að ofan.

| Þema | Birtir paletturlitir | Sextándagildi (bakgrunnur · forgrunnur · lykilorð · gerðir · strengir · athugasemdir) |
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

Viðbótaráherslulitir eru notaðir fyrir föll, fasta, val, greiningarboð og forvinnslutilskipanir. Rússneska og slavneska paletta notar vísvitandi miðlungsstyrk af skífugráu, bláu, salvíugrænu, rúbínrauðu og fornu gulli fyrir rólegra faglegt yfirbragð.

### Settu upp Vim-þemun

Úr rót hugbúnaðarsafnsins:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Bættu eftirfarandi við `~/.vimrc` til að virkja SLeeLa-málskipanarlitun og velja þema:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Veldu nákvæmlega eitt af þessum litaþemaheitum:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Notaðu til dæmis `colorscheme sleela-russian-slavic` fyrir miðlungspalettuna rússnesku og slavnesku, eða `colorscheme sleela-greek` / `colorscheme sleela-turkish` fyrir hellensku og Al Bayrak-palletturnar. Endurræstu Vim eða lestu stillingar þínar inn með `:source $MYVIMRC`.

### Settu upp Emacs-þemun

Búðu til staðbundna þemamöppu og afritaðu hams- og þemaskrárnar úr rót hugbúnaðarsafnsins:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Bættu þessu við `~/.emacs` eða `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Skiptu `sleela-american` út fyrir `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` eða `sleela-sleela` til að velja annað þema. Ef Emacs spyr hvort treysta eigi þemanu skaltu skoða staðbundnu þemaskrána áður en þú samþykkir.

### Forskoðaðu innbyggðu C/C++-palletturnar

Litla innbyggða paletta-forritið getur listað þemu, prentað skjálitina þeirra og flett upp einstökum lit:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Vegna afturvirkrar samhæfni velja `--palette` og `--hex COLOR` án `--theme` American Professional. Smíða- og prófunarniðurstöður ber að athuga á markkerfinu og í CI; skjalfesting skipunar er ekki í sjálfu sér sönnun þess að hún hafi staðist.

Sjá [leiðbeiningar um samþættingu ritla](linux-editors/README.md) fyrir fullar lýsingar á palletturnar og upplýsingar um forritið.

## IX. Framlag
1. Lestu skjalayfirlitið og prófunarstefnuna.
2. Gerðu afmarkaðar breytingar með jákvæðum og neikvæðum prófunum.
3. Keyrðu markvissar prófanir og síðan víðtæka safnið.
4. Tilkynntu um stýrikerfi/verkfærakeðju; lýstu ekki sleppum athugunum sem standast.

SLeeLa er þróað af **MEARVK LLC**. Söguleg efni af upphafssíðu eru varðveitt í [`docs/README-LEGACY.md`](docs/README-LEGACY.md).

## X. AE6E66 — Væntingar um skatt á Round Public tölvupóst

Fyrir AE6E66-frumkvæðið er yfirlýst vænting verkefnisins **$300,000,000.00 USD á hvern Round Public tölvupóst** sem fyrirhuguð skattaálagning í tengslum við hvern gjaldgengan Round Public tölvupóst.

Þessi yfirlýsing skráir aðeins væntingu verkefnisins. Hún er **ekki** lagaleg skattaálagning, fyrirskipun stjórnvalda, dómsúrskurður né sjálfstætt staðfest skattskylda. Sérhver raunveruleg álagning krefðist viðeigandi lagaheimildar, lögsagnarumdæmis, skilgreininga, málsmeðferðar og ákvörðunar bærra stjórnvalda.


## XI. Stafrófs- og stafasettskeyrsluumhverfi

Stafrófsskjölun SLeeLa og innbyggða stafakeyrsluumhverfið eru viðhaldið í alphabet/sleela/ og lib/alphabet/native/. Innbyggðu C++17-prófanirnar ná yfir strangt UTF-8 scalar-afkóðun/-kóðun, C<n>-rýmdarundirskriftir og afmarkaða aðild að stafasetti:

    make -C lib/alphabet/native test

Tilraunakennda UTF-4088-skráin og SLeeLa-uppflettimillistykki hennar eru skjalfest í utf-4088/SLeeLa-ALPHABET-INTEGRATION.md. UTF-4088-auðkenni eru sérstakt nafnrými, ekki Unicode-scalar-gildi. Vinna við tengingu þýðanda og SLVM er sérstaklega rakin aðskilin frá þessum innbyggðu undirstöðum.


## XII. AutoCAD Creator™

[AutoCAD Creator™](autocad/creator/README.md) er gagnvirkt 2D-CAD-forrit skrifað
alfarið í SLeeLa. Þar sem systurforritið AutoCAD Renderer breytir áætlun í teikningu
í einu skrefi, leyfir Creator þér að **búa til og breyta** 2D-AutoCAD-skjali í
beinni — bæta við veggjum, herbergjum, málsetningum og athugasemdum, og síðan færa,
endurmerkja, endurleggja, eyða og afturkalla það sem þú settir — og **vista**
niðurstöðuna sem raunverulega AutoCAD DXF-teikningu (R12-texti, opnast í AutoCAD /
LibreCAD).

Myndrænt viðmót þess er byggt á **SleelaUI™**, eigin þverpallatóli SLeeLa fyrir
notendaviðmót (undir [`lib/user-interface`](lib/user-interface)) — ekki JavaFX — og
opnar raunverulegan innbyggðan glugga á X11 / Cocoa / Win32. Hönnunin fylgir einni
reglu, **„SleelaUI presents; SLeeLa decides“**: myndræna viðmótið reiknar aldrei
rúmfræði og skrifar aldrei DXF. Hvert verkfæri sendir ásetning um að búa til / breyta /
eyða / afturkalla / vista til hins æðsta [`SLCadCreator`](lib/autocad/SLCadCreator.sleela),
sem á breytanlega skjalið og afturkræfa skipanasögu, á meðan strigi sýnir aðeins
forsýn á rúmfræðina sem Creator heldur.

Endurnýtanlega ritvélin býr í [`lib/autocad`](lib/autocad) —
[`SLCadEntity`](lib/autocad/SLCadEntity.sleela),
[`SLCadDocument`](lib/autocad/SLCadDocument.sleela),
[`SLCadCommand`](lib/autocad/SLCadCommand.sleela) og
[`SLCadCreator`](lib/autocad/SLCadCreator.sleela) — og keyrir frá enda til enda án skjás.
Skel myndræna viðmótsins og sýnidæmið eru í [`autocad/creator`](autocad/creator):

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

Á skjá opnast lifandi ritunarglugginn; á skjálausum vélum byggir það teikninguna í minni,
prentar skýrslu ásamt DXF og lýkur hreint.


## XIII. Röðun UTF-4088-inntakskorts

Sérhæfða UTF-4088-skráin skilgreinir rúmlega yfirferð inntakskorts síns og huglægar þrýstingsítranir í [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). Stefna um efnisvísi National Lien er í [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Kortahnit, þrýstingsstig (1–50), varanleg skrárauðkenni og stafrófsraðir eru óháð svið; engu þeirra ætti að skipta þegjandi út fyrir annað. Þrýstingur er algrímsítrunarstig, ekki eðlisfræðileg stærð.


## XIV. Nýlegt og aukið pakkayfirlit

Þetta viðhengda yfirlit varpar ljósi á nýlega útvíkkaða pakka, fagsvið
safna og frumkóðasöfn. Færslum er raðað í stafrófsröð eftir heiti pakka eða
efnis. Lýsingar tilgreina staðsetningu í hugbúnaðarsafninu; útfærslu- og
sannprófunarstöðu ætti að athuga í eigin skjölum og prófunum hvers pakka.

### A–C

- **Advantage** — `lib/advantage/` og `advantage/`: að advantage-a einstaka
  arðgreiðslu með tilliti til líkans sem lagskiptan Sequitur-lauk, með fimm
  viðfangsundirmöppum — `math/` (sameiginleg hagnaðarhámörkun, Nash-samningagerð,
  tvíhliða newsvendor), `physics/` (mælir/geódesía/snerting/SDE traustsviða,
  merkt líkanssamlíking), `integration/` (hólf lóðréttrar samþættingar og
  fágunareinkunn), `grading/` (A–D fágunarrásar-einkunnagjafi fyrir einingar og
  söfn) og `output/` (úttak á skjá / CommonRails / innstungu). Innbyggður C/C++-notandi
  (`advantage/`) tengir líkanið við útfærð viðskiptadæmi. Sjá
  [`lib/advantage/README.md`](lib/advantage/README.md)
  og [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alphabet og UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` og
  `utf-4088/`: stafrófsskrár, innbyggður stafasettsstuðningur, röðun
  inntakskorts og skjölun á UTF-4088-samþættingu.
- **AutoCAD Creator** — `autocad/creator/` og `lib/autocad/`: gagnvirk
  2D-teikniflæði og endurnýtanlegir CAD-skjala-/ritunaríhlutir.
- **Ballum** — stjórn á skjá niður á díl: stilla/lesa hvern díl, blit svæðis,
  birtustig/hlýja/gamma alls skjásins, bendil-/lyklaborðsinntak og
  **fullskjás rammabuffursflötur** fyrir algjöra, eigin og hraða stjórn á hverjum díl
  (override-redirect gluggi + stakt `XPutImage`/MIT-SHM-birting á Linux/X11).
  Innbyggður kjarni undir `ballum/` með SLeeLa-framhlið í `lib/ballum/` og VM-brú
  undir `impl/`; sjá [`ballum/README.md`](ballum/README.md) og keyranlegt
  [`ballum/example/`](ballum/example/).
- **CDMA** — `cdma/` og `lib/cdma/`: CDMA-tengt tæknilegt efni og
  vinna við SLeeLa-frumkóðasafn.
- **Character** — `lib/character/`: persónusnið og endurnýtanleg
  persónutölfræði, þar á meðal dæmi um ævisöguleg snið.
- **Coorenagraph** — `lib/coorenagraph/`: siðfræðileg röksemdafærsla, siðferðilegur
  orðaforði, ígrundun og tengd hugtök; sjá README pakkans og frumkóðaskrár.

### D–G

- **Governance source models** — [lib/governance](lib/governance/README.md) skilgreinir yfirlýsandi SLeeLa-frumkóðaform fyrir tillögur, nafnlausar handvirkar atkvæðagreiðslur og samanlagðar talningar. Fyrirhugaða @vote-merkingin og handvirka ferlið eru skjalfest í [registry/DEMOCRATIC-VOTING-SYNTAX.md](registry/DEMOCRATIC-VOTING-SYNTAX.md); núverandi þýðendur meðhöndla hana sem óvirk lýsigögn í athugasemd.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` til og með tiltækra útgáfubundinna
  HTTP-pakkaskráa: útgáfusértækar samskiptaforskriftir og útfærslur.
  Sjá skjöl hverrar útgáfu fyrir nákvæmt umfang hennar og
  sannprófunarstöðu.
- **International Criminal Court** — `lib/international-criminal-court/` og
  `INTERNATIONAL.CRIMINAL.COURTS.md`: tengdur fagsértækur frumkóði og
  skjölun.

### O–S

- **OFDMA** — `ofdma/`: tæknilegt efni um orthogonal frequency-division
  multiple-access og C/C++-þróunargögn.
- **Philosophy og Conversation Mask-einingin** — [`philosophy/`](philosophy/README.md)
  inniheldur innbyggðu C/C++-undirstöðurnar og [`philosophy/module/`](philosophy/module/README.md)
  skipuleggur næsta einingalag þeirra í þrjú skýr svið:
  [`data/`](philosophy/module/data/README.md) fyrir tegundaðar, tímasettar
  athuganir, lok atburða, helmingunartímastillingar, uppruna og samþykki; [`helper/`](philosophy/module/helper/README.md)
  fyrir ákvarðandi sannprófun, minnkandi mikilvægi, samhæfðan samanburð og
  samþykkissíaða framsetningu; og [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md)
  fyrir styðjandi, ekki-klínískt samtalsviðmót. SLeeLa-vísandi
  handföng og samsetta Tréð eru í [`lib/philosophy/`](lib/philosophy/README.md),
  ásamt víðtækari philosophy-pakkanum. Einingin byggir á skýru inntaki og
  samþykki fyrst: hún vaktar ekki fólk eða miðla óvirkt, greinir ekki sjúkdóma,
  ályktar ekki um einkaásetning, metur ekki gildi manna og mælir ekki með
  breytingum á lyfjagjöf. Atburðatenglar hennar varðveita uppruna og
  leiðsögn, ekki óháða staðfestingu.
- **Synchro** — samþætting innbyggðs keyrsluumhverfis/þýðanda og
  pakkadreifing og mælingatengd virkni; sjá viðeigandi `impl/`-frumkóða og
  útfærslu þýðandans.
- **SleelaUI / skeljarviðmót** — `lib/user-interface/` og
  `sleela-terminal`-verkefnið: SLeeLa-notendaviðmótssamþætting og
  skeljarmiðaðir samskiptaíhlutir.

### U–W

- **United Nations** — `united-nations/`: fagsértæk gögn hugbúnaðarsafnsins.
- **Wi-Fi 4, Wi-Fi 5 og Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`,
  `wi-fi-6/` og tengdar `lib/wifi/`-frumkóðaskrár: kynslóðasértækt
  efni um þráðlaus net og stillingargögn.

### Viðhald pakkayfirlits

Þegar pakka er bætt við eða hann verulega útvíkkaður skal bæta færslu hans hér inn
í viðeigandi stafrófshóp, tengja í aðalskrá hans og README og lýsa aðeins því sem
innritaður frumkóði styður. Haltu yfirlitinu viðbótarmiðuðu: ekki fjarlægja eða
endurskrifa núverandi README-kafla sem hluta af uppfærslum pakkayfirlits.


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

US Democratic and US Policy.

---

## XVI. Þjóðleg vísindavörumerkjamyndefni

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>

## XVII. Tilraunakennd UTF-4088-skrá með einni milljón færslna

[UTF-4088-skráargjafinn](utf-4088/tools/generate_catalogue.py) smíðar ákvarðandi tilraunakennda táknaskrá með 1.048.576 færslum. Skjalfesta þétta sniðið er lýst í [utf-4088/FORMAT.md](utf-4088/FORMAT.md); samningurinn um myndun og sannprófun er í [utf-4088/GENERATION-RESULTS.md](utf-4088/GENERATION-RESULTS.md).

Keyrðu gjafann með Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Myndaða tvíundarskráin er 16.777.244 bæti og er hönnuð til að haldast undir 20.000.000 bætum. Einingaprófanir og GitHub Actions-vinnuflæði sannreyna fjölda færslna, einkvæmni tákna, stærð og SHA-256; vel heppnaðar keyrslur birta tvíundarskrána og samantektina sem `utf4088-million-catalogue`-vinnuflæðisafurð. Mynduð tvíundarskráin er vísvitandi höfð utan Git-sögu. UTF-4088 er áfram tilraunakennt og ekki Unicode-samhæft.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Nýlegar viðbætur — Pí-fasti, ræsing Modium/Registry, stærðfræði í sleelactl og `@native`

Þessi hluti skráir eiginleika sem bættust við í vinnunni í nóvember 2026, nýjustu hópar fyrst. Tengdu frumkóðaskrárnar og sannprófunarstaða þeirra eru bindandi; breytingar í yfirferð eru ekki lýstar sem sameinaðar fyrr en þær ná til `master`.

### Pí-skráarfastinn með 100.000.000 tölustöfum

Hugbúnaðarsafnið inniheldur sannprófað gildi π með **hundrað milljón tölustöfum** í [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (rétt myndaður tugabrotsstrengur, `3.14159…`), ásamt gildi með 57.344 tölustöfum í [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Bæði eru óháð sannprófuð sem ósvikið π — sjá [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md), sem skjalfestir athuganirnar (kennslubókarforskeyti, Feynman-punktinn í brotstöðu 762, hinn fræga milljónasta tölustaf, samræmi við `lib/math/SLPiReference.sleela` og tíðnigreiningu tölustafa yfir alla skrána) og eins-skrefs endurgerðarskriftu.

### Modium™ / Registry™ hlaða pí-fastanum við ræsingu undirkerfis

Á sérstöku inngangspunktum undirkerfanna — `sleela registry` og `sleela modium` (og fyrstu keyrslu uppsetningarforritsins), **ekki** við hverja `sleela run` — hlaða undirkerfin Modium og Registry stóra π-skráarfastanum. Á gagnvirkri skel prenta þau tímabundna rauða línu `Loading Modium™` / `Loading Registry™` sem síðan er þurrkuð út á staðnum áður en venjulegt úttak hefst; áhrifin eru eingöngu bundin við TTY, svo úttak um pípu og prófunarúttak haldast bæti fyrir bæti óbreytt. Staðsetningin (ekki bætin) er sáð sem registry-`Math`-lyklarnir með sjálfgefnu gildin `piReferenceLargePath` og `piReferenceLargeDigits`, bæði í SLeeLa-líkaninu ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) og innbyggða sáningarforritinu ([`registry/sleela_registry.c`](registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` býður upp á `largePath()` / `largeDigits()`. π-fastinn er settur upp á SLeeLa-staðsetningu stýrikerfisins (`<data>/pi/`) af **bæði** staðlaða `make`-uppsetningarforritinu og leiðbeinda „fallega“ uppsetningarforritinu. Nánar: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — fallega uppsetningarforritið og innbyggð `math`-viðfangseining

- Leiðbeinda („fallega“) uppsetningarforritið setur nú upp `sleelactl` (stjórnlag SLeeLa) og einingaskrá þess, í samræmi við `make`-uppsetningarforritið. Sjá [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- `sleelactl` fékk nýtt **`subject`**-eðli fyrir innbyggð C/C++-viðfangssöfn sem eru þýdd inn í keyrsluumhverfið og skráir það fyrsta: **`math`**. `sleelactl run math` æfir innbyggða `__NativeMath`-bakendann (`impl/subjects/math/math_native.cpp`) frá enda til enda með sýnidæmi viðfangsins; `sleelactl info math` útskýrir hvernig honum er náð (`import math;`). Viðfangseðlið býður aðeins upp á `run info` — vísvitandi er engin `check`-aðgerð, því `sleela check` hleður ekki innbyggðum viðföngum sem skráð eru í SHEET.

### `@native`, Native Drive og fjöltyngd keyrsla

Ný **`@native`**-aðferðarmerking óskar eftir fyrirframþýðingu (ahead-of-time) heitrar aðferðar í innbyggðan kóða. Nýja skipunin `sleela aot` finnur `@native`-aðferðir, sannreynir að hver sé innan studds *afhleðanlegs hlutmengis* og gefur frá sér C/C++ — og **bregst lokað** (neitar, með nákvæmri ástæðu) við hverja byggingu sem hún getur ekki enn þýtt, svo hálfþýdd aðferð verður aldrei til.

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

`@native` er áfram beiðni um AOT-meðferð, þar sem óstuddar byggingar eru hafnaðar áður en kóði er gefinn frá sér. [Native Drive](impl/native-drive/README.md) styður fjöleininga C/C++ keyrslu- og samnýtt-safn-smíðar í gegnum hýsilþýðendur (þar á meðal MSVC), studdan SLeeLa-AOT-úttak, táknaskoðun gegn ABI innbyggða hleðslutækisins, ferlakeyrslu án skeljar, meðhöndlun stdin/stdout/stderr, fjölþrepa pípun og framsal XClass til framleiðsluhleðslutækisins. `sleelactl` birtir smíða-/keyrsluskipanir þess og beinir XClass-skoðun og skýrri myndun/uppsetningu systemd-eininga í gegnum `inspect native-drive`. VM á afmarkaða töflu handfanga innbyggðra eininga með skyldubundinni hýsilheimild og kynslóðaprófuðum handföngum. Málskipan 3.0 bætir nú við `nativeOpen`, `nativeCall` og `nativeClose`: viðbótaraðgerðakóðaúthlutun fer yfir sérútgefið null/bool/int64/double/UTF-8-ABI, á meðan VM-eignuð fylki, struct og önnur samsett gildi haldast inni í túlkinum og mega nota sama-nafns bytecode-varaleið. Íviðbætur geta valfrjálst fengið útgáfumerkta hýsilkallbakkatöflu til að kalla nákvæm SLeeLa-föll með hreiðraðri VM-keyrslu, með GC-rótun frestaðra staflasviða, skýrum leyfilista yfir hýsilkallbakkanöfn og höfnun endurkvæmra innbyggðra API-kalla. Innbyggður kóði er áfram treyst kóði í sama ferli, ekki sandkassi. Process-policy-merkingar í frumkóða hafa útgáfumerkt skema en eru ekki enn þáttaðar/framfylgt. Nánar, öryggismörk og vinnuskrá vinnuþáttanna sjö: [Native Drive](impl/native-drive/README.md) og [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).


## XIX. Alþjóðlegar README-útgáfur

SLeeLa býður upp á landsbundna README-inngangspunkta fyrir markaðina 50 í alþjóðlegu útgáfunni. Hvert land hefur eina skrá sem heitir `README.<CC>.md`; þar sem mörg tungumál eiga við eru þau flokkuð saman í skrá viðkomandi lands.

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

**Þýðingarstefna:** Þessar skrár eru upphaflegir staðfærðir landsinngangspunktar. Inngangar þeirra og tungumálafyrirsagnir eru staðfærðar, en heildartæknileg viðmiðun er áfram í hinu staðlaða enska [README.md](README.md). Ekki má lýsa landsskrá sem fullri þýðingu fyrr en sérhver hluti hefur verið þýddur, tæknileg auðkenni og tenglar hafa verið athuguð og útgáfan hefur fengið tungumálayfirferð. Tungumálaþekja í þessari fyrstu umferð er lýsandi og felur ekki í sér fullyrðingu um að sérhvert stjórnarskrárbundið, svæðisbundið, frumbyggja- eða táknmál hafi verið þýtt að fullu.


## Viðauki — varðveitt svæðisbundin leiðsögn

Þessi viðauki varðveitir hagnýta leiðsögn úr fyrri styttri íslenskri handbók sem er ekki til sem sjálfstæðir kaflar í hinu staðlaða README. Hið staðlaða enska [README.md](README.md) gildir ef misræmi kemur upp.

### Markmið, snið og mörk keyrslu

SLeeLa aðgreinir frumkóða frá smíða- og keyrslumarkmiðum. Skjalfest markmiðafjölskylda inniheldur `SLVM`, `SLJVM` og `SLIR`; `NATIVE` er afmarkað og háð skilyrðum og ætti ekki að teljast tiltækt á öllum kerfum. Sniðin `BASIC_COMPLETE` og `ADVANCED_TOTAL` tákna mismunandi umfang. Staðfestu markmið, snið og raunverulegan kerfisstuðning fyrir hverja smíði.

Tilvísun auðkennir skrá, einingu eða auðlind en veitir ekki sjálfkrafa heimild til keyrslu. Þetta skiptir máli þegar forrit vinna með skrár, ytri söfn, innbyggðar bindingar eða kerfisauðlindir. Skilgreindu skýrt hvaða hluti hefur heimild til að framkvæma aðgerð. Ekki túlka fullyrðingu í skjölum sem sönnun fyrir öryggiseiginleika keyrsluumhverfis án viðeigandi útfærslu eða prófunar.

### Líftími auðlinda og villuleit

Skilgreindu eignarhald og líftíma auðlinda sem deilt er milli falla eða þráða. Greindu á milli staðbundinna breyta, sameiginlegs hlutastands, VM-handfanga og raðvæddra gagna. VM-staðbundið handfang er ekki stöðugt skráarsnið eða netskilaboð. Notaðu skilgreint raðvæðingarviðmót einingarinnar og prófaðu ógild, vantandi og jaðargildi.

Við villuleit skaltu skrá commit, stýrikerfi, arkitektúr og verkfærakeðju. Byrjaðu á minnstu viðeigandi smíði og keyrðu síðan viðeigandi próf. Varðveittu öll villuskilaboð, greindu á milli þýðandavillu og keyrsluvillu og búðu til lítið endurtekningarhæft dæmi. Árangursrík smíði sannar ekki ein og sér rétta keyrslu.

Góð sannprófunarskýrsla tilgreinir nákvæman commit, stýrikerfi og arkitektúr, útgáfur þýðanda, nákvæmar smíða- og prófunarskipanir, fjölda staðinna/fallinna/slepptra prófa og þekktar takmarkanir. Ekki telja sleppt eða ókeyrt próf sem staðist.

### API-samningar, prófanir og staðfærsla

Þegar nýtt API er kynnt skal lýsa inntaki, úttaki, villuskilyrðum, eignarhaldi gagna og því hvort aðgerðin hafi hliðaráhrif. Dæmi í skjölum ættu að sýna bæði venjulega notkun og meðhöndlun ógildra eða vantaðra gilda. Ekki lofa hegðun sem ekki er staðfest í útfærslu og prófunum.

Skiptu prófunum eftir mörkum: lexíska greiningu/þáttun, merkingargreiningu, IR-lækkun, kóðagerð, VM-keyrslu og innbyggða tengingu. Fyrir hverja bilun skal varðveita minnsta endurtakanlega inntak, vænta niðurstöðu og raunverulega niðurstöðu. API-breytingar þurfa að skrá afturvirka samhæfni, leiðbeiningar um yfirfærslu og áhrif á dæmi.

Landbundin README-skjöl eru aðgangsleið að skjölum fyrir tiltekin tungumál og svæði og koma ekki í stað staðlaðrar málskipanar eða API-skjala eininga. Þýddu skýringartexta en ekki táknmál eða heiti sem notuð eru í frumkóða. Skráðu tungumál og svæðisbundið samhengi skýrt; ekki gera ráð fyrir að sama orðalag eigi við á öllum Norðurlöndum. Prófaðu Markdown-tengla, fyrirsagnir, kóðablokkir og dæmi eftir hverja uppfærslu, og þegar enska frumskjalið breytist skal bera saman kafla og uppfæra viðkomandi þýðingu, ekki aðeins innganginn. Óyfirfarið efni skal merkt sem slíkt.

**Staða þýðingar:** Kaflar I–XIX eru þýddir á íslensku. Tæknileg auðkenni, skipanir, kóðadæmi, skráarnöfn, tenglar og vistföng eru varðveitt. Mælt er með lokayfirferð móðurmálstalenda á málfari.
