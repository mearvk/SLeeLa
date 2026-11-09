# SLeeLa 3.0 — Ísland (IS)

<p align="center"><strong>The Now. The Ingredient.</strong></p>

## Íslenskt yfirlit

SLeeLa er innbyggt kerfisforritunarmál með eigin þýðanda, keyrsluumhverfi og vistkerfi sýndarvéla. Verkefnið nær lengra en málskipanin ein: það felur einnig í sér söfn, þróunarverkfæri, viðmót, skjölun, byggingarferli og samninga fyrir studd stýrikerfi.

Alþjóðlega útgáfan 3.0 sameinar lýsingu á málinu, keyrsluumhverfinu, verkfærunum og söfnunum. Tæknileg auðkenni, frumkóði, skipanir, skráarnöfn og API-heiti haldast óbreytt svo að dæmi og leiðbeiningar nýtist beint.

## 1. Byrjað

- [Aðal-README og tæknileg viðmiðunarskjöl](README.md).
- [Byggingarleiðbeiningar fyrir innbyggðu útfærsluna](impl/README.md).
- [Notendahandbók sýndarvélar og Modium](virtual-machine/USER-GUIDE.md).
- [Yfirlit yfir skjölun](docs/README.md).
- [Mynduð API-skjöl](docs/api/README.md) og [API-skrá](api/README.md).
- [Stuðningur við stýrikerfi og sannprófunartafla](docs/PLATFORM-SUPPORT.md).
- [Málfræðilýsing](SLEELA.syntax).
- [Prófunarstefna](markdown/TESTING.md).

CPU-skjölin fjalla um byggingu frá rökhliðum og ALU til örgjörvakjarna, stýrikerfis og forrita. Sjá [CPU.md](lib/cpu/CPU.md) og [TIMING.md](lib/cpu/TIMING.md).

## 2. Bygging og prófanir

Kröfur ráðast af því hvaða byggingarmarkmið er valið. Fyrir innbyggðu útfærsluna þarf yfirleitt C/C++ þýðanda, svo sem GCC eða Clang, og GNU Make. Python 3 er notað af sumum skrár- og verkflæðisverkfærum. Grafískir Linux-hlutar kunna einnig að krefjast þróunarpakka fyrir X11 og XRandR.

Keyrið eftirfarandi í rót verkefnisins:

```sh
make
```

Bygging innbyggðu útfærslunnar og keyrsla alls prófunarsafnsins:

```sh
make -C impl all
bash ./test-suites/run-all.sh --all
```

Algeng Make-markmið:

```sh
make all
make clean
make help
sudo make install
```

Fylgið alltaf leiðbeiningum fyrir hvert markmið. Það að einn hluti byggist án villu sannar ekki að öll söfn eða öll stýrikerfi séu tilbúin til útgáfu. Prófunarskýrslur eiga að greina á milli prófana sem voru keyrðar, sleppt og féllu.

## 3. Uppsetning

Sjálfgefið forskeyti fyrir uppsetningu á kerfisstigi er yfirleitt `/usr/local`:

```sh
sudo make install
```

Uppsetning fyrir einn notanda, yfirleitt án kerfisstjóraheimilda:

```sh
make install PREFIX="$HOME/.local"
```

Til að búa til pakkarót og sannreyna uppsetningu:

```sh
make install DESTDIR="$PWD/package-root"
make install-check
```

Hreint uppsetningarmarkmið er í boði með `make clean-install`. Sjá [uppsetningarhandbókina](install/README.md) fyrir kröfur, öryggismörk og fleiri valkosti. Gætið þess að PATH innihaldi `bin`-möppu þeirrar uppsetningar sem valin var.

## 4. Forritunarmálið, þýðandinn og keyrsluumhverfið

Staðlaða málfræðilýsingin er í [SLEELA.syntax](SLEELA.syntax). Skjöl þessa safns miða við málfræðiútgáfu 3.0. Útgáfa þýðandans og útgáfa málfræðinnar eru ekki það sama; lágmarksútgáfa hvers eiginleika gildir áfram.

Verkfærasafnið inniheldur `sleelvac`-þýðandann og `sleela`-keyrsluumhverfið. Skjölin fjalla meðal annars um `@mix`-reitasnið, útvíkkaðar heiltölugerðir, Preonem-skjalam­erki, SleelaDoc, myndun API-skjala og Nordshrift-verkfæri.

Markmið sýndarvéla og milliframsetninga eru meðal annars SLVM, SLJVM og SLIR. Innbyggða markmiðið lýtur sérstökum viðmótsmörkum. Grundvallarregla öryggis er: **tilvísanir auðkenna; þær veita ekki heimild til keyrslu**. Auðkenning hlutar, traust á honum og leyfi til að keyra hann eru aðskildar ákvarðanir.

## 5. Verkfræðilegar meginreglur og sannprófun

Þróunin leggur áherslu á sannanleg gögn, skýr mörk milli eininga, færanleika, örugga hegðun við villur og endurtakanlegar útgáfur. Samningar viðmóta eiga að tilgreina væntingar og villuskilyrði. Skjölun má ekki lofa eiginleikum sem útfærslan eða prófanir styðja ekki.

Sannprófun þarf að meta fyrir hverja einingu og hvert stýrikerfi. Prófun sem ekki var keyrð má ekki merkja sem staðna. Árangur eins þáttar kemur ekki í stað sannprófunar allrar útgáfunnar. SHA-256 athuganir og heilleikaskrár hjálpa til við að finna breytingar, en sanna ekki einar og sér að hugbúnaður sé öruggur.

Sleelavirin-varnarverkfærin innihalda staðbundna SHA-256 sannprófun, valfrjálsa ClamAV-tengingu og heilleikaskrá. Útgáfu má aðeins kalla sannprófaða þegar nauðsynlegar CI-athuganir fyrir viðkomandi commit hafa lokið og útgefin gagnasöfn hafa verið yfirfarin.

## 6. Ritlar og þátttaka

SLeeLa býður upp á þemu og málfræðistuðning fyrir Vim og Emacs, auk litatóla fyrir innbyggt C/C++. Nánari upplýsingar eru í [leiðbeiningum um ritlasamþættingu](linux-editors/README.md).

Gott framlag hefur skýrt afmarkað umfang, rökstuðning, jákvæðar og neikvæðar prófanir og upplýsingar um stýrikerfi og verkfærakeðju. Greina þarf sérstaklega frá prófunum sem var sleppt.

## 7. Söfn og verkefnasvið

Aðal-README skráir pakkayfirlit og söfnin Advantage, SleelaDoc og XML DOM. Önnur skjalfest svið eru grunntölutegundir, keyrsluumhverfi tákna, AutoCAD Creator™, tilraunaskrá UTF-4088, pí-fastinn, Modium™, Registry™ og `sleelactl`-verkfærið.

Meðhöndla skal tilraunaskrá UTF-4088 með einni milljón færslna og önnur tilraunagögn samkvæmt eigin skjölun þeirra. Tilvist skrár þýðir ekki sjálfkrafa að allar færslur séu notaðar í framleiðsluumhverfi. Viðhald pakkaskrár þarf að varðveita rekjanleika slóða, ábyrgðar, háðra eininga og prófunarstöðu.

## 8. `@native` og leiðin að C/C++

Skjölin lýsa `@native`-eiginleikanum og skipuninni `sleela aot` sem fyrsta áfanga leiðar frá SLeeLa-frumkóða að innbyggðu C/C++. Þetta veitir ekki ótakmarkað leyfi til innbyggðrar keyrslu. Mörk viðmóta, ABI, skýrt samþykki og öryggishlið skipta áfram máli. Staða markmiðsins verður að byggjast á útfærslu og prófunum.

## 9. Útgáfur og alþjóðleg README-skjöl

Fréttir, útgáfuupplýsingar og skrá yfir alþjóðleg README-skjöl eru viðhaldin í aðal-README. Land- eða tungumálaskjal er staðfærður inngangur, ekki sjálfkrafa full þýðing allra tæknikafla.

## 10. Gagnlegir tenglar

- [Aðal-README (enska)](README.md)
- [Málfræðilýsing](SLEELA.syntax)
- [Skjalayfirlit](docs/README.md)
- [Stuðningur við stýrikerfi](docs/PLATFORM-SUPPORT.md)
- [Innbyggð útfærsla](impl/README.md)
- [Uppsetningarhandbók](install/README.md)
- [Prófunarstefna](markdown/TESTING.md)
- [SLeeLa á GitHub](https://github.com/mearvk/SLeeLa)

## Staða þýðingar

Þessi íslenska útgáfa er ítarleg tæknileg kynning sem nær yfir verkefnið, byggingu, uppsetningu, mál og keyrsluumhverfi, sannprófun, þróunarverkfæri og helstu verkefnasvið. Hún er ekki enn full þýðing línu fyrir línu á öllu staðlaða enska README-skjalinu. Þar til fullri þýðingu og tæknilegri yfirferð er lokið er [README.md](README.md) áfram staðlaða tæknilega heimildin.

SLeeLa er þróað af MEARVK LLC.


## 11. Þýðandabygging og keyrslulíkan

SLeeLa aðgreinir upprunamál, framenda þýðandans, milliframsetningu, kóðagerð og keyrsluumhverfi. Útgáfa þýðandans, málskipanarútgáfa og VM/ISA-útgáfa eru aðskilin hugtök. Það að eiginleiki sé nefndur í forskrift sannar ekki að hann sé útfærður á öllum markkerfum.

Venjulegt ferli les UTF-8 `.sleela`-skrár, framkvæmir lexíska og setningafræðilega greiningu, sannprófar tákn og tegundir, umbreytir skipulagðri milliframsetningu, býr til úttak fyrir studd VM- eða millimarkkerfi og keyrir niðurstöðuna í runtime. Staðlaða málskipanin er [SLEELA.syntax](SLEELA.syntax); sjá einnig [source documentation](markdown/SOURCE.md) og [compiler documentation](markdown/COMPILER.md).

## 12. Gagnagerðir, hlutir og söfn

Skjalfestar grunngerðir eru `int`, `double`, `boolean`, `String`, `void`, skilgreindar `struct`-gerðir og studdar reference-type signatures. `int`, `long` og `int64` eru 64-bita heiltölur með formerki. `int128` er samþykkt sem heiltölugerð, en núverandi skjalfesta útfærslan byggir enn á 64 bitum.

`struct` er nafngreind samsett gerð. Struct-handföng sem VM stýrir eru ekki hráir bendlar; úthlutun og færibreytuflutningur deila sama tilviki og jafngildi byggist á auðkenni. Notaðu `structPack(instance)` og `structUnpack(TypeName, json)` fyrir skjalfesta færanlega raðvæðingu. Classes innihalda fields og methods; erfðir, nested classes og generics lúta útgáfuhliðum málskipanar.

Helstu einingar eru `lib/advantage` (líkansbundið mat, stærðfræði, samþætting, einkunnagjöf og beint úttak), `lib/xml` (XML DOM), `lib/text` (texta- og strengjaaðgerðir), `lib/sleeladoc` (frumkóðaskjöl og CLI), `lib/cpu`, `lib/xlt` (CPU-snið og tímasetningarviðmið) og `recursion/stanchion` (C/C++-frumgerðir fyrir endurkvæmni). Tilvist einingar sannar ekki framleiðsluhæfni; skoðaðu prófanir og takmarkanir stýrikerfa.

## 13. Lykkjutímasetning, native-samþætting og prófanir

`@loopDelay` og `@loopBudget` skilgreina bið í lykkju eða heildartímamörk. Þær eru lækkaðar í VM-skipunina `OP_SLEEP` (ISA 157); innbyggða fallið `sleep(ms)` notar sömu skipun. Sjá [SLEELA.syntax §32](SLEELA.syntax) og [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md).

Geymslan inniheldur C/C++-einingar og afmarkaða leið fyrir native-kóðagerð. `@native` og `sleela aot` veita ekki almennt leyfi til að keyra hvaða native-kóða sem er. C/C++-mörk þurfa að skilgreina ABI, kallavenjur, eignarhald minnis, gagnaskipan, villumeðhöndlun og kerfisháðar forsendur.

Dæmigerðar skipanir:

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
sudo make install
make install PREFIX="$HOME/.local"
```

Bygging krefst yfirleitt GCC eða Clang og GNU Make; sum verkfæri nota Python 3. Sjá [installation guide](install/README.md), [platform matrix](docs/PLATFORM-SUPPORT.md) og [testing policy](markdown/TESTING.md). Prófunarskýrslur verða að greina á milli pass, fail og skip. PASS gildir aðeins um tilgreint umfang og tíma sannprófunar.

**Uppfærð þýðingarstaða:** Íslenski leiðarvísirinn nær nú einnig yfir þýðandapípuna, gagnagerðir, hlutalíkan, söfn, lykkjutímasetningu, native-samþættingu, byggingu, uppsetningu og prófanir. Hann er ekki fullkomin línu-fyrir-línu þýðing á öllu kanóníska enska README-skjalinu.


## 14. Markmið, snið og afmörkun native

SLeeLa aðgreinir upprunakóðann frá keyrslumarkmiðum. Skjalfest markmiðafjölskylda inniheldur `SLVM`, `SLJVM` og `SLIR`; `NATIVE` er afmarkað og háð skilyrðum og ætti ekki að teljast almennt tiltækt á öllum kerfum. Sniðin `BASIC_COMPLETE` og `ADVANCED_TOTAL` tákna mismunandi umfang. Staðfestu markmið, snið og raunverulegan kerfisstuðning fyrir hverja byggingu.

Tilvísun auðkennir skrá, einingu eða auðlind en veitir ekki sjálfkrafa heimild til keyrslu. Þetta skiptir máli þegar forrit vinna með skrár, ytri söfn, native bindingar eða kerfisauðlindir. Sannprófaðu inntak og skilgreindu skýrt hvaða hluti hefur heimild til að framkvæma aðgerð. Ekki túlka fullyrðingu í skjölum sem sönnun fyrir öryggiseiginleika runtime án viðeigandi útfærslu eða prófunar.

## 15. Hönnun gagna, villumeðhöndlun og prófanir

Skilgreindu eignarhald og líftíma auðlinda sem deilt er milli falla eða þráða. Greindu á milli staðbundinna breyta, sameiginlegs hlutastands, VM-handfanga og raðvæddra gagna. VM-local handle er ekki stöðugt skráarsnið eða netskilaboð. Notaðu skilgreint raðvæðingarviðmót einingarinnar og prófaðu ógild, vantandi og jaðargildi.

Við villuleit skaltu skrá commit, stýrikerfi, arkitektúr og verkfærakeðju. Byrjaðu á minnstu viðeigandi byggingu og keyrðu síðan viðeigandi próf. Varðveittu öll villuskilaboð, greindu á milli þýðandavillu og runtime-villu og búðu til lítið endurtekningarhæft dæmi. Árangursrík bygging sannar ekki ein og sér rétta keyrslu.

Góð sannprófunarskýrsla tilgreinir nákvæman commit, stýrikerfi og arkitektúr, útgáfur þýðanda, nákvæmar byggingar- og prófunarskipanir, fjölda pass/fail/skip og þekktar takmarkanir. Ekki telja sleppt eða ókeyrt próf sem staðist.

## 16. Alþjóðleg skjöl og útgáfustýring

Landbundin README-skjöl eru aðgangsleið að skjölum fyrir tiltekin tungumál og svæði. Þau koma ekki í stað staðlaðrar málskipanar eða API-skjala eininga. Þegar enska frumskjalið breytist skal bera þýðinguna saman við nýja útgáfu, varðveita auðkenni, skipanir, slóðir og kóða óbreytt og merkja efni sem bíður yfirferðar.

**Viðbót við þýðingarstöðu:** Íslenski leiðarvísirinn nær nú einnig yfir keyrslumarkmið og snið, heimildamörk, auðlindaeign, villuleit, prófunarskýrslur og viðhald alþjóðlegra skjala.
