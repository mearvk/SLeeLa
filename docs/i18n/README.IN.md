<img align="right" src="../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — भारत (IN)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>एक native systems भाषा, compiler, runtime और virtual-machine ecosystem।</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="../../docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **International देश संस्करण।** यह संस्करण हिन्दी में है। तकनीकी identifiers, commands, code उदाहरण, फ़ाइल नाम, paths और links कैनोनिकल अंग्रेज़ी [README.md](../../README.md) की तुलना में अपरिवर्तित हैं।

**The Now. The Ingredient.** भाषा केवल syntax नहीं है: वह compiler, runtime, libraries, tools और platform contracts का समुच्चय है, जो software को उपयोगी बनाते हैं।

**SLeeLa किन क्षेत्रों से जुड़ा है।** SLeeLa **Math**, **Object**, **Science** और **Reach** से जुड़ा है — ये उसके कार्य के चार पक्ष हैं: गणना-योग्य (Math), जो कुछ है उसकी सूची में खड़ी स्थिर वस्तु (Object), सत्य का अनुशासित अध्ययन (Science), और किसी system के हाथ का अन्य systems तथा लोगों तक विस्तार (Reach)। **राष्ट्रीय हित बने रहते हैं** — इन चारों में से कोई भी उन्हें परे नहीं हटाता; जहाँ कोई विषय किसी राष्ट्र के हित से संबंधित है, वहाँ वह स्थिति सुरक्षित रखी जाती है (निष्ठापूर्वक दर्ज की जाती है, उदाहरणतः प्रत्येक class के Metadata `metaOrigin` / `metaNationalSchedule` में), कभी विलीन नहीं की जाती।

## I. यहाँ से शुरू करें
- [Native implementation बनाएँ](../../impl/README.md)
- [Virtual machines और Modium — व्यावहारिक उपयोगकर्ता मार्गदर्शिका](../../virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [SLeeLa source में CPU stack](../../lib/cpu/CPU.md) — gates→ALU→cores→OS→programs तक की पूरी machine, 130+ CPU profiles की सूची, और समायोज्य [voltages, variable clocks तथा expert timings](../../lib/cpu/TIMING.md), जिन्हें आप [spec पर चला सकते हैं और overclock कर सकते हैं](../../lib/cpu/components/README.md)
- [दस्तावेज़ सूची](../../docs/README.md)
- [CSPML डेटा स्रोत और Salt & Salary शोध संदर्भ](../reference/CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — बाहरी अमेरिकी स्थान, सामाजिक-आर्थिक, कार्यबल, शिक्षा और सरकारी विद्यालय डेटा की उत्पत्ति तथा integration संबंधी मार्गदर्शन।
- [निर्मित API दस्तावेज़](../../docs/api/README.md) — वर्तमान HTML संदर्भ, versioned API संस्करण और पुनः-निर्माण (regeneration) के निर्देश
- [API catalogue](../../api/README.md)
- [Computational comparison standards](../../comparison/README.md) — SLeeLa VM, Java/JVM और native C/C++ के type, memory, ABI और conformance मानक।
- [Recursion stanchion — C और C++ prototypes](../../recursion/stanchion/README.md) — Boolean, Byte, Character, Integer, Long, Float और Double के लिए primitive-विशेषीकृत उदाहरण, साथ ही generic, applied, scientific और शिक्षण-उद्देश्य वाले recursion patterns।
- [Platform support और validation matrix](../../docs/PLATFORM-SUPPORT.md)
- [Brand guide](../../docs/BRAND-GUIDE.md)
- [Merchandise की अवधारणाएँ और assets](../../merchandise/README.md)
- [NitroWebExpress logo](../../images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Testing policy और release gates](../../markdown/TESTING.md)

## II. समाचार

भाषा, drivers और standard library में हाल की वृद्धियाँ:

- **`advantage` standard-library package — math, physics, integration, grading और routable output के साथ किसी dividend को advantage देना।**
  [`lib/advantage/`](../../lib/advantage/README.md) *किसी model के सापेक्ष एक अद्वितीय dividend को advantage देने* को परतदार **Sequitur onion** (dividend → review → concerns → maths → additions → subtraction → regrouping → iteration) के रूप में समाहित करता है, जिसमें पूर्ण dynamic update और restitute के साथ भीतर/बाहर की ओर navigate किया जा सकता है; इसे सरल **user's model** और पूर्ण-नियंत्रण वाले **developer's model** के माध्यम से प्रस्तुत किया गया है। इसमें पाँच विषय-क्षेत्र हैं: **`math/`** द्विपक्षीय व्यापार अर्थशास्त्र (संयुक्त लाभ अधिकतमीकरण, Nash bargaining, द्विपक्षीय newsvendor critical ratio), **`physics/`** "trust fields की physics" नामक संरचनात्मक अर्थशास्त्र (trust एक metric tensor के रूप में, transactional geodesic, announcement contact structure, CIR+jump trust-decay SDE — एक modelling analogy, जिसे इसी रूप में चिह्नित किया गया है), **`integration/`** पूँजीवाद/खुदरा व्यापार के लिए पुनरावर्ती रूप से संयोज्य vertical-integration model (compartments, engineering bays, trial compartments, newton median trays और system-sophistication score), **`grading/`** एक sophistication circuit grader, जो प्रत्येक module और winning/losing/grader/absolute modules के किसी भी समूह को A–D श्रेणी देता है, तथा **`output/`** एक routable emit seam, जो module output को terminal, **CommonRails**-स्वरूपित सतह, या socket/connector तक भेजता है (native `slnet_write` के लिए framed)। प्रत्येक class में Metadata-Class record होता है; पाँच चलाने योग्य demos इन विषयों का validation करते हैं, और सहायक [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md), [`MATH.md`](../../lib/advantage/MATH.md) तथा [`PHYSICS.md`](../../lib/advantage/PHYSICS.md) उन्हें दस्तावेज़ित करते हैं। [`advantage/`](../../advantage/README.md) के अंतर्गत एक native C/C++ consumer (C core `advantage_value`/`advantage_mean`, एक XML knowledge store, तथा सुरक्षित HTTPS/SFTP/SSH acquisition + keystore + DH/ECDH transport library) इस model को व्यावहारिक [trade उदाहरणों](../../advantage/examples/README.md) से जोड़ता है — अनुमानित internet स्रोतों से सकारात्मक trades, एक audience-relevance क्रम (Traders, Consumers, Helpers, Healers), और A–D श्रेणी में क्रमबद्ध आठ models (D = सर्वाधिक Cement, A = सर्वाधिक Brand)।
- **Loop-timing annotations — `@loopDelay` / `@loopBudget` (साथ में `sleep(ms)` built-in)।** SLeeLa के पहले **statement-level, behavioral** annotations: किसी `for` / `while` / `do` loop से पहले अपनी अलग पंक्ति में लिखे जाने पर ये loop के execution में वास्तविक pause डालते हैं। `@loopDelay(ms = 250)` / `@loopDelay(seconds = 2)` प्रत्येक iteration में निश्चित समय के लिए रुकता है (केवल `@loopDelay(250)` लिखने पर default इकाई milliseconds होती है); `@loopBudget(seconds = 10, iterations = 1000)` ज्ञात iteration संख्या को कुल wall-clock budget में समान रूप से फैलाता है। दोनों नए append-only opcode **`OP_SLEEP`** (ISA 157) तक lower होते हैं, जो कहीं भी बुलाए जा सकने वाले सामान्य **`sleep(<ms>)`** built-in का भी आधार है; pause को portable रूप से (`nanosleep` / `Sleep`) लागू किया गया है। विनिर्देश [`SLEELA.syntax` §32](../../SLEELA.syntax) में; API संदर्भ [`api/sleela/3.0/LOOP_TIMING_API.md`](../../api/sleela/3.0/LOOP_TIMING_API.md) में; चलाने योग्य [`examples/loop-timing-annotation-demo.sleela`](../../examples/loop-timing-annotation-demo.sleela)। एक session-additions सूची (जिसमें यह, native `OP_BIGMUL` big-integer गुणन, और स्वतंत्र **[local-generator](../../local-generator/README.md)** तथा **[pi-generator](../../pi-generator/README.md)** tools शामिल हैं) [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](../../api/sleela/3.0/SESSION-ADDITIONS-API.md) पर है। तीनों निर्मित API सतहें ([`sleeladoc-api/`](../../sleeladoc-api/README.md), [`docs/api/`](../../docs/api/README.md) और अपरिवर्तनीय [`api/sleela/3.0/version-014/`](../../api/sleela/3.0/version-014/index.html) snapshot) `make api-docs` से पुनः बनाई गईं और अब अपने `vm` package पृष्ठ पर नई `SLISASleep` / `SLISABigMul` instruction प्रविष्टियाँ रखती हैं।
- **command line से SleelaDoc — `sleela docs`।** SleelaDoc अब सीधे terminal से उपलब्ध है। `sleela docs` प्रत्येक दस्तावेज़ित किए जा सकने वाले विषय की सूची देता है; `sleela docs crypto` एक package दिखाता है; `sleela docs stringbuilder` एक class को हल करता है (case-insensitive रूप से, और `SL` standard-library prefix के द्वारा → `SLStringBuilder`) और एक समृद्ध पृष्ठ छापता है — **परिभाषा, design उद्देश्य, संबंध, संरचना, user clays और पाँच-सीढ़ी वाली mastery ladder** — निकट-मिलान की स्थिति में क्रमबद्ध "did you mean" परिणामों के साथ। लेखक किसी भी पृष्ठ को केवल SleelaDoc `@`-tags (`@intent`, `@thesis`, `@axiom`, `@param`, `@see`, `@mastery`) जोड़कर समृद्ध कर सकते हैं। `sleela docs --generate [dir]` एक plain-text corpus लिखता है (प्रत्येक class के लिए एक पृष्ठ + एक `TOPICS.index`), जिसे CLI फिर offline पढ़ता है; **`make sleeladoc`** उस corpus **और मेल खाते `.sleela` sources** को `sleeladoc/build/sleeladoc.jar` में बाँध देता है। केवल facade वाले विषयों (जैसे `psychiatry`, `politico`) के लिए `sleela docs --facade <topic>` runtime पर filesystem में खोजता है और वास्तविक **folders (फिर files, स्वाभाविक गति के रूप में)** सूचीबद्ध करता है, जो उपयोगकर्ता द्वारा बदली जा सकने वाली सीमाओं **6 MB / 6000 files / 6 मिनट** से बँधा है। `lib/sleeladoc` units वर्तमान संस्करण **`#sleela 3.0`** पर बनाए रखी जाती हैं। देखें [`man/man1/sleela.1`](../../man/man1/sleela.1) और [`lib/sleeladoc/README.md`](../../lib/sleeladoc/README.md)।
- **SleelaDoc — Javadoc-शैली का दस्तावेज़ीकरण समाधान, निर्मित HTML साइट के साथ।** **SleelaDoc** SLeeLa की source-भीतर doc-comment परंपरा है (angle-bracket comments, जो `@doc` / `@page` से चिह्नित होते हैं; देखें [`manifesto/SLEELADOC.md`](../../manifesto/SLEELADOC.md)), और [`lib/sleeladoc`](../../lib/sleeladoc/README.md) वह **extractor** है जो `.sleela` unit को Markdown या HTML API पृष्ठ के रूप में प्रस्तुत करता है — जैसे `javadoc` Java को करता है, *आशय (intent)* को दस्तावेज़ित करते हुए। संपूर्ण `/lib` के लिए एक पूर्ण, browse करने योग्य **HTML दस्तावेज़ साइट** `tools/generate-sleeladoc-api.py` द्वारा [`/sleeladoc-api`](../../sleeladoc-api/README.md) में निर्मित होती है (`sleeladoc-api/index.html` खोलें; साफ़ नीला / हल्का नीला / हल्का धूसर / सफ़ेद शैली, package → class → member पदानुक्रम, और *Student 2* पाठक के लिए लिखी गई शब्दावली के साथ)। Compiler compilation के उप-उत्पाद के रूप में लेखक-चिह्नित doc outputs भी उत्पन्न करता है (`--wiki` MediaWiki पृष्ठ + एक originated source twin), जो [`manifesto/SLEELADOC.WIKI.md`](../../manifesto/SLEELADOC.WIKI.md) में दस्तावेज़ित है। चलाने योग्य: [`lib/sleeladoc/sleeladoc-demo.sleela`](../../lib/sleeladoc/sleeladoc-demo.sleela)।
- **XML Document Object Model (`lib/xml`)।** SLeeLa-पक्ष का XML DOM — एक document, जो क्रमबद्ध children और नामित attributes वाले element/text/comment/CDATA/PI nodes का वृक्ष धारण करता है, एक serializer (compact/pretty, XML 1.0 §2.4 escaping), एक char-cell parser, और `SLXmlAspect` quality register। Node equality विश्वास की त्रि-सीढ़ी वाली कड़ी है (identity → वही विषय → संरचना), जो reflexive, symmetric और deterministic है। देखें [`lib/xml/README.md`](../../lib/xml/README.md) और चलाने योग्य [`lib/xml/xml-dom.sleela`](../../lib/xml/xml-dom.sleela)।
- **विस्तृत signed integer primitives (`long`, `int64`, `int128`)।** `int`, `long` और `int64` सभी 64-bit signed integers हैं, जो एक जैसे type-check होते और चलते हैं; `int128` को integer type के रूप में स्वीकार किया जाता है (अभी 64-bit-आधारित, वास्तविक 128-bit एक दस्तावेज़ित आगामी कार्य है)। base-2 signed श्रृंखला `int32`..`int16777216` (syntax 2.7) और मेल खाती unsigned श्रृंखला `uint32`..`uint16777216` (syntax 2.8) इसे आगे बढ़ाती हैं। देखें [`SLEELA.syntax`](../../SLEELA.syntax) §21, §24, §25।
- **`@mix` consequent fields (syntax 2.9)।** कोई class **एक ही नाम** वाले कई instance fields घोषित कर सकती है, बशर्ते प्रत्येक दोहराव पर `@mix` हो; भाषा इस समूह को consequent (index से कुंजित array / list) के रूप में समझती है, डिफ़ॉल्ट रूप से ऊपर से नीचे `1,2,3,…` या `@mix(1,2,3,...)` द्वारा मैन्युअल, जिसकी index संख्या fields की संख्या से मेल खानी चाहिए। देखें [`SLEELA.syntax`](../../SLEELA.syntax) §28।
- **Document markers — Natural Process, जिसे Preonems कहा गया है (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`)।** इन bare document-marker शब्दों को सामूहिक रूप से **Preonems** कहा जाता है (एक marker एक *Preonem* है; समुच्चय, *Preonems* — देखें [`SLEELA.syntax`](../../SLEELA.syntax) §26.1)। ये प्रथम-श्रेणी, केवल-घोषणात्मक document markers हैं, जो यह नाम देते हैं कि कोई document रचना के क्रम में *क्या है*: `author`/`authorial` (syntax 2.4; पेशेवर परिषद द्वारा संचालित — व्यवहारगत प्रभाव वाला एकमात्र सदस्य), `material;` (स्थिर Source) और `memory;` (syntax 2.5), `determine;` (syntax 2.6; ऐसा document जो किसी डॉक्टर, चिकित्सक या ghost द्वारा आगे संसाधित किए जाने के लिए खुला है, ताकि वह वैध हाथ से पूर्णता तक पहुँचे), तथा दो सबसे नए, **`object;`** (जो है उसकी सूची में खड़ी स्थिर वस्तु — **syntax 2.8 पर reified, ठोस स्थान प्राप्त**) और **`field;`** (संबंध का वह आधार जिसमें objects खड़े होते और गति करते हैं) — दोनों syntax 2.6 से gated हैं। प्रत्येक bare, संदर्भाश्रित और front end द्वारा version-gated है, जिसका parsing, typing या codegen पर कोई प्रभाव नहीं पड़ता; marker स्थिति के बाहर हर एक सामान्य identifier बना रहता है (इसलिए `field` कभी किसी class member या `.field` access को बाधित नहीं करता)। देखें [`SLEELA.syntax`](../../SLEELA.syntax) §18–§20, §23 और §26।
- **Syntax 3.0 पर Nordshrift, पूर्ण library पहुँच के साथ।** Nordshrift उसी पुनरावर्ती `/lib` index के माध्यम से हल करता है जिससे compiler करता है, और प्रत्येक package तथा `.sleela` unit तक पहुँचता है; `nordshrift check` पूरे वृक्ष की गणनाएँ बताता है।
- **Recursion stanchion।** [`recursion/stanchion/`](../../recursion/stanchion/README.md) directory में अब सात दस्तावेज़ित primitive contracts के लिए समर्पित C और C++ reference prototypes, साथ ही generic, applied, scientific और शिक्षण उदाहरण शामिल हैं। ये reference sources हैं; इन्हें verified मानने से पहले compile और test करें।

- **Angle-bracket comments (`<` / `<<`)।** एक नई आगे-के-लिए comment शैली — `< line >` और `<< block >>` — जिसे SLeeLa frontend lex करता है और जो [`SLEELA.syntax`](../../SLEELA.syntax) §1.1 में दस्तावेज़ित है।
- **Radix integer literals।** `int` literals अब `_` अंक-विभाजकों के साथ हेक्साडेसिमल (`0x`), बाइनरी (`0b`) और ऑक्टल (`0o`) रूप स्वीकार करते हैं, SLeeLa source भाषा ([`SLEELA.syntax`](../../SLEELA.syntax) §1.4) और [`.xlt` CPU-model format](../../lib/xlt/README.md) दोनों में।
- **पूरी library में Metadata Class मानक।** वर्णनात्मक-metadata metapattern (उत्पत्ति, वैधता, क्षेत्राधिकारीय दायरा) अब `/lib` के अंतर्गत प्रत्येक class-युक्त `.sleela` फ़ाइल में रखा गया है; देखें [`SLEELA.syntax`](../../SLEELA.syntax) §15 और [tutorial पाठ](../../tutorial/13-metadata-class.md)।
- **अधिक समृद्ध text / String परिवार।** `lib/text` को वास्तविक in-language helpers मिलते हैं, एक char-cell दृश्य जो वास्तविक `charAt`/`substring`/`indexOf`/`reverse`/column-alignment देता है, और एक वास्तविक positional templater — देखें [text & String tutorial](../../tutorial/14-text-and-strings.md) और चलाने योग्य [`lib/text/text-string-demo.sleela`](../../lib/text/text-string-demo.sleela)।
- **Nordshrift sheets में प्रथम-श्रेणी `String`।** `.sst` sheet एक typed `strings:` block घोषित कर सकती है, जो build के component manifest में योगदान देता है; देखें [Nordshrift Sheet Reference](../../wiki/Nordshrift-Sheet-Reference.md)।
- **CPU models opcodes और function-call equivalence दोनों से चलते हैं।** प्रत्येक `SL<ARCH>CPU` अपने fetch-decode-execute loop के माध्यम से native instruction stream चलाने *और* साझा VM substrate पर समतुल्य calls द्वारा किसी अनुमानित अन्य process का code चलाने का आधार विरासत में पाता है — देखें [`lib/cpu/EXECUTION.md`](../../lib/cpu/EXECUTION.md) §5।

## III. Build और Test

**पूर्वापेक्षाएँ:** एक C/C++ toolchain (GCC या Clang) और GNU Make; manifest/flow tooling के लिए Python 3; Linux पर display products के लिए X11/Xrandr development packages (किसी module का `make deps` बताता/स्थापित करता है कि उसे क्या चाहिए)। `impl/` build विश्वसनीय SHA-256 manifest पर fail-closed है, जिसकी ओर root `Makefile` स्वतः संकेत करती है।

साफ़ checkout से सबसे तेज़ शुरुआत:

```sh
make                                  # build everything from the repo root
```

या केवल native core को build और test करें:

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

Repo-root का `make` प्रत्येक product (core, VM, codecs, churn, antivirus, …) बनाता है और अंत में **registry** बनाकर उसकी baseline table तैयार करता है — इस तरह बना हुआ tree अपने साथ समीक्षा-योग्य registry तैयार रूप में देता है। आवश्यक targets:

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

अन्य उपयोगी targets:

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

`registry` target [`build-registry.sh`](../../build-registry.sh) चलाता है: यह registry core बनाता है, baseline table को portable text store `registry/build/sleela.sleelareg` में भरता है, और MySQL table script `registry/build/registry.sql` निर्यात करता है (`sleela registry export` के माध्यम से)। इसकी समीक्षा `sleela registry` या `registry/build/sleela-registry review` से करें। देखें [registry guide](../../registry/README.md) और [Registry 3.0 architecture](../../registry/REGISTRY-3.0-ARCHITECTURE.md)।

पूर्वापेक्षाओं और platform-विशिष्ट चरणों के लिए build तथा testing guides देखें। किसी एक component का सफल build यह सिद्ध नहीं करता कि प्रत्येक library या platform release के लिए तैयार है।

## IV. Install

Native products बनाने के बाद commands और runtime support files install करें:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

परंपरागत installer डिफ़ॉल्ट रूप से `/usr/local/bin`, `/usr/local/lib/sleela` और `/usr/local/share/sleela` का उपयोग करता है। यह मौजूदा startup configuration को सुरक्षित रखता है और बिना track किए मौजूद commands को overwrite करने से इनकार करता है। Uninstall केवल installer द्वारा दर्ज फ़ाइलें हटाता है, configuration और untracked runtime data को सुरक्षित रखते हुए।

केवल मुख्य package — `sleela`, `sleelvac` और `nordshrift` binaries, standard library, protocols और support modules — की केंद्रित, निर्देशित deployment के लिए clean installer का उपयोग करें:

```sh
make clean-install       # guided install of the main package only
```

यह एक प्राथमिक निर्णय लेता है (मुख्य package install करना), फिर दायरे का चयन, और सफल होने पर चुने हुए enhancement modules के लिए वैकल्पिक picker। interactive deployment workflow और सभी installation विकल्पों के लिए [Quick and Safe Install](../../install/README.md) देखें।

## V. भाषा और Runtime
प्रामाणिक grammar [`SLEELA.syntax`](../../SLEELA.syntax) है। वर्तमान repository syntax **3.0** को दस्तावेज़ित करता है; compiler/tool version और source syntax version अलग-अलग हैं। 3.0 surface **`pure` और `gasoline` document markers** (§30) जोड़ता है — bare, संदर्भाश्रित, केवल-घोषणात्मक Preonems की एक जोड़ी, जिनका अर्थ लगभग समान है, और प्रत्येक अपनी ईमानदारी से document की स्थिति चिह्नित करता है। Syntax 3.0 1.0–2.9 भाषा surface को वर्तमान normative संस्करण में समेकित करता है, और सभी feature-विशिष्ट न्यूनतम gates सुरक्षित रखता है। पूर्ववर्ती 2.9 surface **`@mix` consequent fields** (§28) जोड़ता है — कोई class एक ही नाम के कई instance fields घोषित कर सकती है, यदि प्रत्येक पर `@mix` हो, जिसे एक अनुक्रमित समूह के रूप में समझा जाता है (ऊपर से नीचे `1,2,3,…` या मैन्युअल `@mix(1,2,3,...)`, जिसकी index संख्या समूह से मेल खानी चाहिए)। यह पहले के `long`/`int64`/`int128` (§21) के ऊपर base-2 signed/unsigned wide-integer परिवार `int32`..`int16777216` (§24) और `uint32`..`uint16777216` (§25) को आगे ले जाता है; **keyword supporter documents** (§22); और **Preonems** — केवल-घोषणात्मक document markers `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) और `artifact` (§35, समय और स्थान के अनुसार श्रेणीबद्ध विधायिका-स्तरीय स्थिति), जिन्हें सामूहिक रूप से Preonems कहा गया है (§26.1)। Constructor overload resolution और `this(...)`/base-constructor delegation स्पष्ट रूप से verified syntax 1.8 contract के बाहर हैं। किसी feature पर निर्भर होने से पहले grammar और implementation notes देखें। SleelaDoc की नौ source units अब `#sleela 3.0` घोषित करती हैं; साझा compiler gate source syntax 1.0–3.0 स्वीकार करता है। नया API build `make api-docs` से बनता है।

Repository में native C/C++ implementation, SLVM generation references, standard-library modules, tests, examples और platform integrations शामिल हैं। Implementation और validation की स्थिति module के अनुसार भिन्न होती है; समान पूर्णता मान लेने के बजाय उसका specification और वर्तमान CI परिणाम देखें।

## VI. Engineering सिद्धांत
- **दावों से ऊपर प्रमाण:** implemented, tested, experimental और planned व्यवहार में अंतर करें।
- **प्रत्येक सीमा पर एक contract:** syntax, APIs, file formats, platform मान्यताओं और failure व्यवहार को दस्तावेज़ित करें।
- **डिज़ाइन से portable:** platform-विशिष्ट backends को अलग रखें और native runners पर test करें।
- **सुरक्षित विफलता:** malformed input, resource limits, process failures और cleanup paths का test करें।
- **Reproducible releases:** साफ़ तरीके से पुनः build करें और artifact hashes तथा release notes प्रकाशित करें।

## VII. Antivirus Build और Integrity

Sleelavirin antivirus package में native SHA-256 scanner, वैकल्पिक ClamAV adapter, source/index integrity manifests और platform-package checksums शामिल हैं। Repository root से package को build और test करें:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Integrity manifest `antivirus/build/` के अंतर्गत बनता है; इसमें tracked source और documentation inputs तथा संरक्षित signature-index फ़ाइलें आती हैं, जबकि generated output और परिवर्तनीय ClamAV cache को छोड़ा जाता है। ClamAV database snapshots के अपने manifests होते हैं। Release ZIPs में `INTEGRITY-MANIFEST.json`, `SHA256SUMS` और `.sha256` sidecar शामिल होते हैं। डाउनलोड किए गए archive के hashes को स्वतंत्र रूप से प्राप्त विश्वसनीय मान से मिलाएँ: archive के बगल में रखा checksum publisher का signature नहीं है। देखें [Sleelavirin guide](../../antivirus/README.md), [security checklist](../../antivirus/SECURITY.md) और [antivirus build workflow](../../.github/workflows/antivirus-binaries.yml)।

कोई release तब तक verified नहीं माना जाता जब तक संबंधित CI jobs ठीक उसी release commit पर पास न हों और परिणामी artifacts की जाँच न हो चुकी हो।


## VIII. SLeeLa Editor Themes — Vim और Emacs

SLeeLa में **Vim** और **Emacs** के लिए पेशेवर dark color schemes का एक समन्वित समूह तथा एक native C/C++ palette utility शामिल है। मूल **American Professional** scheme डिफ़ॉल्ट बना रहता है। विभिन्न सांस्कृतिक और संस्थागत design प्रेरणाओं के लिए दस सहयोगी palettes उपलब्ध हैं; ये व्याख्यात्मक design palettes हैं, किसी राष्ट्र, संस्कृति, न्यायालय या संस्था का आधिकारिक प्रतिनिधित्व नहीं।

### ग्यारह Schemes और उनके रंग

| Scheme | Background | Foreground | Keywords | Types | Strings | Comments |
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


#### राष्ट्रीय ध्वज के रंग (स्रोत palette)

Greek और Turkish schemes दोनों देशों के **आधिकारिक राष्ट्रीय ध्वज रंगों** से व्युत्पन्न हैं, जिन्हें यहाँ उनके प्रकाशित सटीक hex मानों के साथ दिखाया गया है। ऊपर के scheme backgrounds/accents इन स्रोत रंगों की गहरी, editor-अनुकूल व्याख्याएँ हैं — नीचे की तालिका "राष्ट्रीय रूप से प्रदर्शित रंगों" का संदर्भ है।

| राष्ट्र | रंग | Hex | आधार और अर्थ |
|---|---|---|---|
| **Greece** | नीला | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | राष्ट्रीय ध्वज का नीला (Law 851/1978)। आकाश और समुद्र; heraldry में *azure* के रूप में वर्णित। |
| **Greece** | सफ़ेद | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | राष्ट्रीय ध्वज का सफ़ेद। स्वतंत्रता संग्राम की पवित्रता; क्रॉस पूर्वी ऑर्थोडॉक्स ईसाई धर्म का प्रतीक है। |
| **Türkiye** | लाल (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | राष्ट्रीय ध्वज का लाल (Turkish Flag Law)। पृष्ठभूमि; परंपरा से शहीदों का रक्त और Ottoman संप्रभुता। |
| **Türkiye** | सफ़ेद | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | राष्ट्रीय ध्वज का सफ़ेद। अर्धचंद्र और तारा — शांति, पवित्रता और तुर्क/इस्लामी विरासत का प्रतीक। |

ये schemes प्रत्येक editor के foreground को राष्ट्रीय **सफ़ेद** पर, तथा उसके keywords, cursor, selection और status line को सटीक राष्ट्रीय **नीले** (`#0D5EAF`) या **लाल** (`#E30A17`) पर आधारित करती हैं, ताकि जो रंग महत्त्वपूर्ण हैं वे सचमुच राष्ट्रीय रंग हों। ये व्याख्यात्मक design palettes हैं; ध्वज-रंग संदर्भ उत्पत्ति (provenance) के लिए दिया गया है और किसी भी राज्य का आधिकारिक प्रतिनिधित्व नहीं है। रंगों के अर्थ Hellenic Republic MFA के राज्य-प्रतीकों के वर्णन और Turkish Flag Law की परंपरा का अनुसरण करते हैं।


### Color Swatch तालिका

नीचे की palette कोशिकाएँ **SVG swatches** का उपयोग करती हैं, ताकि GitHub खाली HTML color cells के बजाय वास्तविक रंग दिखाए। प्रत्येक पट्टी छह रंगों को इस क्रम में दिखाती है: background, foreground, keywords, types, strings और comments। सटीक hex codes प्रत्येक swatch के पास, ऊपर की संदर्भ तालिका के समान scheme क्रम में सूचीबद्ध हैं।

| Scheme | प्रदर्शित palette रंग | Hex codes (background · foreground · keywords · types · strings · comments) |
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

अतिरिक्त accent रंग functions, constants, selections, diagnostics और preprocessor directives के लिए उपयोग किए जाते हैं। Russian and Slavic palette जान-बूझकर मध्यम-तीव्रता वाले slate, नीले, sage, माणिक (ruby) और प्राचीन स्वर्ण रंगों का उपयोग करती है, ताकि अधिक शांत पेशेवर रूप मिले।

### Vim Schemes install करें

Repository root से:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

SLeeLa syntax highlighting सक्षम करने और scheme चुनने के लिए `~/.vimrc` में निम्नलिखित जोड़ें:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

इन colorscheme नामों में से ठीक एक चुनें:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

उदाहरण के लिए, मध्यम Russian and Slavic palette के लिए `colorscheme sleela-russian-slavic` उपयोग करें, या Hellenic और Al Bayrak palettes के लिए `colorscheme sleela-greek` / `colorscheme sleela-turkish`। Vim को पुनः आरंभ करें या `:source $MYVIMRC` से अपना configuration source करें।

### Emacs Schemes install करें

एक स्थानीय theme directory बनाएँ और repository root से mode तथा theme फ़ाइलें कॉपी करें:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

इसे `~/.emacs` या `~/.emacs.d/init.el` में जोड़ें:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

दूसरा scheme चुनने के लिए `sleela-american` को `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` या `sleela-sleela` से बदलें। यदि Emacs पूछे कि theme पर भरोसा करना है या नहीं, तो स्वीकार करने से पहले स्थानीय theme फ़ाइल का निरीक्षण करें।

### Native C/C++ Palettes का पूर्वावलोकन करें

छोटी native palette utility schemes की सूची दे सकती है, उनके terminal रंग छाप सकती है और किसी एक रंग को खोज सकती है:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

पश्चगामी संगतता (backward compatibility) के लिए, `--theme` के बिना `--palette` और `--hex COLOR` American Professional चुनते हैं। Build और test परिणामों को लक्ष्य system और CI पर जाँचा जाना चाहिए; किसी command का दस्तावेज़ होना स्वयं इस बात का प्रमाण नहीं है कि वह पास हुआ।

पूर्ण palette विवरणों और utility की जानकारी के लिए [editor integration guide](../../linux-editors/README.md) देखें।

## IX. योगदान
1. Documentation index और testing policy पढ़ें।
2. Positive और negative tests के साथ केंद्रित बदलाव करें।
3. पहले लक्षित tests चलाएँ, फिर व्यापक suite।
4. OS/toolchain का विवरण बताएँ; छोड़े गए checks को पास होना न बताएँ।

SLeeLa का विकास **MEARVK LLC** द्वारा किया जाता है। ऐतिहासिक landing-page सामग्री [`docs/README-LEGACY.md`](../../docs/README-LEGACY.md) में सुरक्षित है।

## X. AE6E66 — Round Public Email Levy अपेक्षा

AE6E66 पहल के लिए, परियोजना की घोषित अपेक्षा प्रत्येक योग्य Round Public email से जुड़ी प्रस्तावित कर-levy राशि के रूप में **प्रति Round Public email $300,000,000.00 USD** है।

यह कथन केवल परियोजना की अपेक्षा दर्ज करता है। यह कानूनी कर निर्धारण, सरकारी आदेश, न्यायालय का निर्णय या स्वतंत्र रूप से स्थापित कर देनदारी **नहीं** है। किसी भी वास्तविक levy के लिए संबंधित वैधानिक प्राधिकारी, क्षेत्राधिकार, परिभाषाएँ, प्रक्रिया और सक्षम सरकारी निर्धारण आवश्यक होंगे।


## XI. Alphabet और Character-set Runtime

SLeeLa का alphabet दस्तावेज़ीकरण और native character runtime alphabet/sleela/ और lib/alphabet/native/ में बनाए रखे जाते हैं। Native C++17 tests strict UTF-8 scalar decoding/encoding, C<n> capacity signatures और सीमित character-set membership को कवर करते हैं:

    make -C lib/alphabet/native test

प्रायोगिक UTF-4088 catalogue और उसका SLeeLa lookup adapter utf-4088/SLeeLa-ALPHABET-INTEGRATION.md में दस्तावेज़ित हैं। UTF-4088 IDs एक अलग namespace हैं, Unicode scalar values नहीं। Compiler और SLVM binding कार्य को इन native आधारों से अलग स्पष्ट रूप से track किया जाता है।

## XII. AutoCAD Creator™

[AutoCAD Creator™](../../autocad/creator/README.md) पूरी तरह SLeeLa में लिखा गया एक interactive 2D CAD application है। जहाँ सहयोगी AutoCAD Renderer एक योजना को एक ही बार में चित्र में बदल देता है, वहीं Creator आपको 2D AutoCAD document को लाइव **बनाने और बदलने** देता है — दीवारें, कमरे, dimensions और notes जोड़ें, फिर जो रखा है उसे हिलाएँ, पुनः-लेबल करें, पुनः-layer करें, हटाएँ और undo करें — और परिणाम को वास्तविक AutoCAD DXF चित्र (R12 text, AutoCAD / LibreCAD में खुलता है) के रूप में **सहेजें**।

इसका graphical interface **SleelaUI™** पर बना है, जो SLeeLa का अपना cross-platform UI toolkit है ([`lib/user-interface`](../../lib/user-interface) के अंतर्गत) — JavaFX नहीं — और X11 / Cocoa / Win32 पर वास्तविक native window उठाता है। Design एक नियम का पालन करता है, **"SleelaUI प्रस्तुत करता है; SLeeLa निर्णय लेता है"**: GUI कभी geometry की गणना नहीं करता और न DXF लिखता है। प्रत्येक tool प्रामाणिक [`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela) को create / alter / delete / undo / save का आशय (intent) भेजता है, जो संपादन-योग्य document और उलटने-योग्य command history का स्वामी है, जबकि canvas केवल उस geometry का पूर्वावलोकन करता है जो Creator धारण करता है।

पुनः-उपयोग योग्य संपादन engine [`lib/autocad`](../../lib/autocad) में है — [`SLCadEntity`](../../lib/autocad/SLCadEntity.sleela), [`SLCadDocument`](../../lib/autocad/SLCadDocument.sleela), [`SLCadCommand`](../../lib/autocad/SLCadCommand.sleela) और [`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela) — और headless रूप से शुरू से अंत तक चलता है। GUI shell और demo [`autocad/creator`](../../autocad/creator) के अंतर्गत हैं:

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

Display पर यह लाइव संपादन window खोलता है; headless host पर यह चित्र को memory में बनाता है, एक रिपोर्ट के साथ DXF छापता है और साफ़-सुथरे ढंग से समाप्त हो जाता है।


## XIII. UTF-4088 Input-map क्रम

विशिष्ट UTF-4088 catalogue अपनी स्थानिक input-map traversal और अमूर्त pressure iterations को [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md) में परिभाषित करता है। National Lien subject-index नीति [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md) में है। Map coordinates, pressure levels (1–50), स्थायी catalogue IDs और अक्षर-क्रम रैंक स्वतंत्र fields हैं; इनमें से किसी को भी चुपचाप दूसरे के स्थान पर नहीं रखा जाना चाहिए। Pressure एक algorithmic iteration level है, भौतिक राशि नहीं।


## XIV. हाल का और विस्तारित Package Index

यह जोड़ा गया index हाल ही में विस्तारित packages, domain libraries और source collections को रेखांकित करता है। प्रविष्टियाँ package नाम या विषय के वर्णानुक्रम में समूहित हैं। विवरण repository में स्थान बताते हैं; implementation और validation की स्थिति प्रत्येक package के अपने दस्तावेज़ों और tests में जाँची जानी चाहिए।

### A–C

- **Advantage** — `lib/advantage/` और `advantage/`: किसी model के सापेक्ष एक अद्वितीय dividend को advantage देना, परतदार Sequitur onion के रूप में, पाँच विषय उप-folders के साथ — `math/` (संयुक्त लाभ अधिकतमीकरण, Nash bargaining, द्विपक्षीय newsvendor), `physics/` (trust-field metric/geodesic/contact/SDE, एक चिह्नित modelling analogy), `integration/` (vertical-integration compartments और sophistication score), `grading/` (modules और समूहों के लिए A–D sophistication circuit grader), और `output/` (terminal / CommonRails / socket emit)। एक native C/C++ consumer (`advantage/`) model को व्यावहारिक trade उदाहरणों से जोड़ता है। देखें [`lib/advantage/README.md`](../../lib/advantage/README.md) और [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md)।
- **Alphabet और UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` और `utf-4088/`: alphabet catalogues, native character-set समर्थन, input-map क्रम और UTF-4088 integration दस्तावेज़।
- **AutoCAD Creator** — `autocad/creator/` और `lib/autocad/`: interactive 2D drawing workflows और पुनः-उपयोग योग्य CAD document/editing components।
- **Ballum** — pixel-स्तरीय display नियंत्रण: प्रति-pixel set/get, region blit, पूरे display की brightness/warmth/gamma, pointer/keyboard input, और कुल, स्वामित्वयुक्त, तेज़ प्रति-pixel नियंत्रण के लिए **fullscreen framebuffer surface** (Linux/X11 पर override-redirect window + एकल `XPutImage`/MIT-SHM present)। Native core `ballum/` के अंतर्गत, SLeeLa facade `lib/ballum/` में और VM bridge `impl/` के अंतर्गत; देखें [`ballum/README.md`](../../ballum/README.md) और चलाने योग्य [`ballum/example/`](../../ballum/example/)।
- **CDMA** — `cdma/` और `lib/cdma/`: CDMA से संबंधित तकनीकी सामग्री और SLeeLa source-library कार्य।
- **Character** — `lib/character/`: character profiles और पुनः-उपयोग योग्य character statistics, जिनमें जीवनी-संबंधी profile उदाहरण शामिल हैं।
- **Coorenagraph** — `lib/coorenagraph/`: नैतिक तर्क, नैतिक शब्दावली, विचार-विमर्श और संबंधित अवधारणाएँ; इसका package README और source फ़ाइलें देखें।

### D–G

- **Governance source models** — [lib/governance](../../lib/governance/README.md) प्रस्तावों, अज्ञात मैन्युअल मतपत्रों और समेकित गणनाओं (tallies) के लिए declarative SLeeLa source आकार परिभाषित करता है। प्रस्तावित @vote annotation और मैन्युअल प्रक्रिया [registry/DEMOCRATIC-VOTING-SYNTAX.md](../../registry/DEMOCRATIC-VOTING-SYNTAX.md) में दस्तावेज़ित हैं; वर्तमान compilers इसे निष्क्रिय comment metadata मानते हैं।

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` से लेकर उपलब्ध versioned HTTP package directories तक: version-विशिष्ट protocol विनिर्देश और implementations। इनके सटीक दायरे और verification स्थिति के लिए प्रत्येक version का दस्तावेज़ देखें।
- **International Criminal Court** — `lib/international-criminal-court/` और `INTERNATIONAL.CRIMINAL.COURTS.md`: संबंधित domain-विशिष्ट source और दस्तावेज़।

### O–S

- **OFDMA** — `ofdma/`: orthogonal frequency-division multiple-access तकनीकी सामग्री और C/C++ विकास संसाधन।
- **Philosophy और Conversation Mask module** — [`philosophy/`](../../philosophy/README.md) में native C/C++ आधार हैं और [`philosophy/module/`](../../philosophy/module/README.md) उनकी अगली module परत को तीन स्पष्ट क्षेत्रों में व्यवस्थित करता है: [`data/`](../../philosophy/module/data/README.md) typed, दिनांकित observations, event endings, half-life settings, provenance और consent के लिए; [`helper/`](../../philosophy/module/helper/README.md) deterministic validation, relevance decay, संगत तुलनाओं और consent-filtered प्रस्तुति के लिए; और [`psychiatry-friend/`](../../philosophy/module/psychiatry-friend/README.md) सहायक, गैर-क्लिनिकल संवाद interface के लिए। SLeeLa-सम्मुख handles और संयोजित Tree व्यापक philosophy package के साथ [`lib/philosophy/`](../../lib/philosophy/README.md) में हैं। यह module explicit-input और consent-first है: यह लोगों या media की निष्क्रिय निगरानी नहीं करता, निदान नहीं करता, निजी आशय का अनुमान नहीं लगाता, मानव मूल्य का अंकन नहीं करता, और दवा बदलने की सिफ़ारिश नहीं करता। इसके event links provenance और navigation सुरक्षित रखते हैं, स्वतंत्र पुष्टि नहीं।
- **Synchro** — native runtime/compiler integration और packet-dispatch तथा measurement-संबंधी कार्यक्षमता; संबंधित `impl/` source और compiler implementation देखें।
- **SleelaUI / terminal UI** — `lib/user-interface/` और `sleela-terminal` project: SLeeLa UI integration और terminal-उन्मुख interaction components।

### U–W

- **United Nations** — `united-nations/`: domain-विशिष्ट repository सामग्री।
- **Wi-Fi 4, Wi-Fi 5 और Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`, `wi-fi-6/` और संबंधित `lib/wifi/` source directories: पीढ़ी-विशिष्ट वायरलेस नेटवर्किंग सामग्री और configuration संसाधन।

### Package-index रखरखाव

जब कोई package जोड़ा जाए या उसका सारगर्भित विस्तार हो, तो उसकी प्रविष्टि यहाँ उपयुक्त वर्णानुक्रम समूह में जोड़ें, उसकी canonical directory और README से link करें, और केवल वही क्षमताएँ वर्णित करें जिन्हें checked-in sources समर्थन देते हैं। इस index को additive रखें: package-index अद्यतनों के भाग के रूप में मौजूदा README sections को हटाएँ या पुनर्लिखित न करें।


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

US Democratic और US Policy।

---

## XVI. National Science Brand Imagery

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. UTF-4088 दस-लाख-प्रविष्टि प्रायोगिक Catalogue

[UTF-4088 catalogue generator](../../utf-4088/tools/generate_catalogue.py) निश्चयात्मक (deterministic) रूप से 1,048,576 प्रविष्टियों वाला प्रायोगिक symbol catalogue बनाता है। दस्तावेज़ित compact format का वर्णन [utf-4088/FORMAT.md](../../utf-4088/FORMAT.md) में है; generation और validation contract [utf-4088/GENERATION-RESULTS.md](../../utf-4088/GENERATION-RESULTS.md) में है।

Generator को Python 3 से चलाएँ:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

निर्मित binary 16,777,244 bytes की है और 20,000,000 bytes से नीचे रहने के लिए डिज़ाइन की गई है। Unit tests और एक GitHub Actions workflow प्रविष्टि संख्या, glyph अद्वितीयता, आकार और SHA-256 को सत्यापित करते हैं; सफल runs binary और summary को `utf4088-million-catalogue` workflow artifact के रूप में प्रकाशित करते हैं। निर्मित binary को जान-बूझकर Git history से बाहर रखा गया है। UTF-4088 प्रायोगिक बना रहता है और Unicode-संगत नहीं है।

---

# SLeeLa
<p align="center"><img src="../../images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. हाल की वृद्धियाँ — Pi Constant, Modium/Registry Startup, sleelactl Math और `@native`

यह खंड 2026-11 के कार्य में जोड़ी गई सुविधाओं को दर्ज करता है, नवीनतम समूह पहले। जुड़ी हुई source फ़ाइलें और उनकी validation स्थिति प्रामाणिक हैं; समीक्षाधीन बदलावों को तब तक merged नहीं बताया जाता जब तक वे `master` तक न पहुँचें।

### 100,000,000-अंकों वाला Pi catalog constant

Repository π का सत्यापित **दस-करोड़-अंकों** वाला मान [`pi-generator/pi.100000000.digits`](../../pi-generator/pi.100000000.digits) पर उपलब्ध कराता है (एक सुगठित दशमलव, `3.14159…`), साथ ही 57,344-अंकों वाला मान [`pi-generator/pi.57344.digits`](../../pi-generator/pi.57344.digits)। दोनों स्वतंत्र रूप से सत्यापित वास्तविक π हैं — देखें [`pi-generator/VERIFIED.md`](../../pi-generator/VERIFIED.md), जो जाँचों (पाठ्यपुस्तक उपसर्ग, भिन्नात्मक स्थान 762 पर Feynman बिंदु, प्रसिद्ध दस-लाखवाँ अंक, `lib/math/SLPiReference.sleela` से मेल, और पूरी-फ़ाइल का अंक-आवृत्ति विश्लेषण) तथा एक one-shot पुनरुत्पादन script को दस्तावेज़ित करता है।

### Modium™ / Registry™ subsystem startup पर Pi constant लोड करते हैं

समर्पित subsystem entry points पर — `sleela registry` और `sleela modium` (तथा installer का पहला run), हर `sleela run` पर **नहीं** — Modium और Registry subsystems बड़ा π catalog constant लोड करते हैं। Interactive terminal पर वे अस्थायी लाल `Loading Modium™` / `Loading Registry™` पंक्ति छापते हैं, जिसे सामान्य output शुरू होने से पहले उसी स्थान पर मिटा दिया जाता है; यह प्रभाव सख्ती से TTY-gated है, इसलिए piped और test output बाइट-दर-बाइट अपरिवर्तित रहता है। Locator (bytes नहीं) को registry `Math` key defaults `piReferenceLargePath` और `piReferenceLargeDigits` के रूप में seed किया जाता है, SLeeLa model ([`lib/registry/SLRegistry.sleela`](../../lib/registry/SLRegistry.sleela)) और native seeder ([`registry/sleela_registry.c`](../../registry/sleela_registry.c)) दोनों में; `lib/math/SLPiReference.sleela` `largePath()` / `largeDigits()` प्रस्तुत करता है। π constant को OS के SLeeLa स्थान (`<data>/pi/`) में मानक `make` installer **और** निर्देशित "beautiful" installer दोनों द्वारा install किया जाता है। विवरण: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](../../pi-generator/MODIUM-REGISTRY-PI-STARTUP.md)।

### `sleelactl` — beautiful installer और native-backed `math` subject

- निर्देशित ("beautiful") installer अब `sleelactl` (SLeeLa control plane) और उसकी module registry install करता है, `make` installer के अनुरूप। देखें [`bin/SLEELACTL.md`](../../bin/SLEELACTL.md)।
- `sleelactl` को runtime में compile किए गए native C/C++ subject libraries के लिए एक नई **`subject`** nature मिली है, और यह पहली को पंजीकृत करती है: **`math`**। `sleelactl run math` subject के reference demo के माध्यम से native `__NativeMath` backend (`impl/subjects/math/math_native.cpp`) को शुरू से अंत तक चलाता है; `sleelactl info math` बताता है कि उस तक कैसे पहुँचा जाता है (`import math;`)। Subject nature केवल `run info` प्रदान करती है — जान-बूझकर कोई `check` verb नहीं है, क्योंकि `sleela check` SHEET-पंजीकृत native subjects को लोड नहीं करता।

### `@native`, Native Drive और बहु-भाषा execution

एक नई **`@native`** method annotation किसी hot method के ahead-of-time native compilation का अनुरोध करती है। नया `sleela aot` command `@native` methods खोजता है, सत्यापित करता है कि प्रत्येक समर्थित *offloadable subset* के भीतर है, और C/C++ उत्पन्न करता है — किसी भी ऐसे construct पर **fail-closed** रहते हुए (सटीक कारण के साथ मना करते हुए) जिसका वह अभी अनुवाद नहीं कर सकता, ताकि आधा-अनूदित method कभी उत्पन्न न हो।

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

`@native` AOT व्यवहार का अनुरोध बना रहता है, और असमर्थित constructs को emission से पहले अस्वीकार कर दिया जाता है। [Native Drive](../../impl/native-drive/README.md) host compilers (MSVC सहित) के माध्यम से बहु-unit C/C++ executable और shared-library builds, समर्थित SLeeLa AOT output, native-loader ABI के विरुद्ध symbol निरीक्षण, shell-रहित process execution, stdin/stdout/stderr संचालन, बहु-चरणीय piping, और production loader को XClass delegation का समर्थन करता है। `sleelactl` इसके build/run commands प्रस्तुत करता है और XClass निरीक्षण तथा स्पष्ट systemd unit rendering/installation को `inspect native-drive` के माध्यम से route करता है। VM अनिवार्य host authorization और generation-checked handles के साथ सीमित native-module handle table का स्वामी है। Syntax 3.0 अब `nativeOpen`, `nativeCall` और `nativeClose` जोड़ता है: append-only bytecode dispatch अलग से versioned null/bool/int64/double/UTF-8 ABI को पार करता है, जबकि VM-स्वामित्व वाले arrays, structs और अन्य composite values interpreter के भीतर रहते हैं और same-name bytecode fallback का उपयोग कर सकते हैं। Plugins वैकल्पिक रूप से versioned host callback table प्राप्त कर सकते हैं, ताकि वे nested VM execution के माध्यम से सटीक SLeeLa functions बुला सकें; इसमें suspended-stack GC rooting, स्पष्ट host callback-name allowlist है, और पुनरावर्ती native API calls अस्वीकार की जाती हैं। Native code विश्वसनीय in-process code बना रहता है, sandbox नहीं। Source process-policy annotations का versioned schema है परंतु वे अभी parse/enforce नहीं होतीं। विवरण, सुरक्षा सीमाएँ और सात-workstream ledger: [Native Drive](../../impl/native-drive/README.md) और [`pi-generator/AT-NATIVE.md`](../../pi-generator/AT-NATIVE.md)।


## XIX. अंतरराष्ट्रीय README संस्करण

SLeeLa अपने अंतरराष्ट्रीय संस्करण के 50 बाज़ारों के लिए देश-विशिष्ट README entry points प्रदान करता है। प्रत्येक देश की एक फ़ाइल है जिसका नाम `README.<CC>.md` है; जहाँ एकाधिक भाषाएँ प्रासंगिक हैं, वहाँ उन्हें उस देश की फ़ाइल में समूहित किया गया है।

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

**अनुवाद नीति:** ये फ़ाइलें प्रारंभिक रूप से स्थानीयकृत देश entry points हैं। इनके परिचय और भाषा शीर्षक स्थानीयकृत हैं, जबकि पूर्ण तकनीकी संदर्भ कैनोनिकल अंग्रेज़ी [README.md](../../README.md) में ही रहता है। किसी देश की फ़ाइल को तब तक पूर्ण अनुवाद नहीं बताया जाना चाहिए जब तक प्रत्येक खंड का अनुवाद न हो गया हो, तकनीकी identifiers और links की जाँच न हो गई हो, और संस्करण की भाषाई समीक्षा न हो गई हो। इस पहले चरण में भाषा-कवरेज प्रतिनिधिक है और यह दावा नहीं है कि प्रत्येक संवैधानिक रूप से मान्यता-प्राप्त, क्षेत्रीय, आदिवासी या सांकेतिक भाषा का पूर्ण अनुवाद हो चुका है।

## Appendix — सुरक्षित रखा गया क्षेत्रीय मार्गदर्शन

यह appendix भारतीय संस्करण की पूर्ववर्ती संक्षिप्त मार्गदर्शिका की वह सामग्री सुरक्षित रखता है जो कैनोनिकल README में अलग खंडों के रूप में नहीं है। किसी भी असंगति की स्थिति में कैनोनिकल अंग्रेज़ी [README.md](../../README.md) को प्राथमिकता प्राप्त है।

### भाषा संस्करण / Language editions

यह भारतीय संस्करण English, हिन्दी, বাংলা, తెలుగు, मराठी, தமிழ், ગુજરાતી, ಕನ್ನಡ, മലയാളം, ਪੰਜਾਬੀ, ଓଡ଼ିଆ, অসমীয়া, اردو और संस्कृतम् के पाठकों के लिए है। इस संस्करण का पूर्ण पाठ हिन्दी में है और तकनीकी identifiers, source code, commands, filenames और API names अपरिवर्तित रखे गए हैं। अन्य भाषाओं के संक्षिप्त परिचय नीचे सुरक्षित रखे गए हैं:

- বাংলা: SLeeLa একটি নেটিভ সিস্টেম প্রোগ্রামিং ভাষা।
- తెలుగు: SLeeLa ఒక స్థానిక సిస్టమ్ ప్రోగ్రామింగ్ భాష.
- मराठी: SLeeLa ही नेटिव्ह सिस्टम प्रोग्रामिंग भाषा आहे.
- தமிழ்: SLeeLa ஒரு இயல்பான கணினி அமைப்பு நிரலாக்க மொழி.
- ગુજરાતી: SLeeLa એક નેટિવ સિસ્ટમ પ્રોગ્રામિંગ ભાષા છે.
- ಕನ್ನಡ: SLeeLa ಸ್ಥಳೀಯ ಸಿಸ್ಟಮ್ ಪ್ರೋಗ್ರಾಮಿಂಗ್ ಭಾಷೆಯಾಗಿದೆ.
- മലയാളം: SLeeLa ഒരു നേറ്റീവ് സിസ്റ്റം പ്രോഗ്രാമിങ് ഭാഷയാണ്.
- ਪੰਜਾਬੀ: SLeeLa ਇੱਕ ਨੇਟਿਵ ਸਿਸਟਮ ਪ੍ਰੋਗਰਾਮਿੰਗ ਭਾਸ਼ਾ ਹੈ.
- ଓଡ଼ିଆ: SLeeLa ଏକ ନେଟିଭ୍ ସିଷ୍ଟମ୍ ପ୍ରୋଗ୍ରାମିଂ ଭାଷା।
- অসমীয়া: SLeeLa এটা নেটিভ ছিষ্টেম প্ৰগ্ৰামিং ভাষা।
- اردو: SLeeLa ایک مقامی نظامی پروگرامنگ زبان ہے۔

### Compilation pipeline का सारांश

SLeeLa में source language, compiler frontend, intermediate representation, code generation और runtime अलग-अलग स्तर हैं। Compiler release, syntax edition और VM/ISA version को एक ही version number न मानें। किसी feature का specification में होना यह सिद्ध नहीं करता कि वह हर target पर समान रूप से उपलब्ध और validated है। सामान्य pipeline:

1. **Source:** version pragma और declarations वाली UTF-8 `.sleela` फ़ाइलें।
2. **Lexing और parsing:** source को tokens और syntax tree में बदलना।
3. **Semantic analysis:** names, symbols, types और language rules की जाँच।
4. **Intermediate representation:** आगे lowering और optimization के लिए संरचित program form।
5. **Code generation:** समर्थित VM या intermediate targets के लिए output।
6. **Runtime:** उपलब्ध memory-management और control-flow सुविधाओं के साथ compiled program का execution।

Version pragma, source lifecycle और compiler व्यवहार के लिए [source documentation](../../markdown/SOURCE.md) तथा [compiler documentation](../../markdown/COMPILER.md) देखें।

### Types और object model पर टिप्पणियाँ

SLeeLa की source vocabulary में `int`, `double`, `boolean`, `String`, `void`, घोषित `struct` प्रकार और समर्थित reference-type signatures शामिल हैं। `int128` को integer type के रूप में स्वीकार किया जाता है, लेकिन इसे पूर्ण 128-bit arithmetic न मानें।

एक `struct` named fields वाला aggregate है। VM-managed struct handles raw pointers नहीं हैं। Assignment और parameter passing उसी instance को साझा करते हैं; equality identity-based है। Portable interchange के लिए `structPack(instance)` और `structUnpack(TypeName, json)` जैसे documented serialization helpers उपयोग करें; VM-local handle को सीधे network पर भेजने योग्य मानना गलत होगा।

Classes में fields और methods होते हैं। वर्तमान execution model में fields shared class state हैं और method-local variables thread-local होते हैं। Inheritance, nested classes और generics syntax-version gates के अधीन हैं। विवरण [International Language Reference](../reference/LANGUAGE.INTERNATIONAL.3.0.md) में है।

### Loop timing का नमूना

`@loopDelay` और `@loopBudget` loop से पहले लिखे जाते हैं। निम्न उदाहरण केवल syntax का नमूना है; वास्तविक identifiers और enclosing class आपके program में परिभाषित होने चाहिए।

```sleela
@loopDelay(ms = 250)
while (running) {
    processNext();
}
```

### C/C++ और native boundary

`@native` और `sleela aot` जैसे नाम native development से संबंधित हैं; इन्हें arbitrary native code चलाने की सामान्य अनुमति न समझें। C/C++ integration में ABI, calling convention, memory ownership, data layout, error handling और platform dependencies स्पष्ट होने चाहिए। Native code को अलग trust boundary मानें। कोई reference या declaration अपने-आप execution authorization नहीं देता। देखें [Native implementation](../../impl/README.md), [platform matrix](../../docs/PLATFORM-SUPPORT.md) और [comparison standards](../../comparison/README.md)।

### सुरक्षा, integrity और testing के मुख्य नियम

Hash manifests और verification tools repository files की अपेक्षित सामग्री से तुलना कर सकते हैं। PASS परिणाम केवल जाँचे गए scope और समय पर लागू होता है। Test reports में pass, fail और skip को अलग-अलग दिखाना चाहिए। छोड़े गए या न चलाए गए tests को सफल न लिखें।

- References resources की पहचान करते हैं; वे execution की अनुमति नहीं देते।
- Native interfaces स्पष्ट और सीमित हों।
- Platform-support दावे वास्तविक builds और tests पर आधारित हों।
- Generated documentation को source और generator तक trace किया जा सके।
- आवश्यक integrity data अनुपस्थित हो तो security checks सुरक्षित रूप से fail हों।

Linux, macOS और Windows पर एक ही command या dependency set मानकर न चलें; platform-विशिष्ट prerequisites के लिए [installation guide](../../install/README.md) और [platform support matrix](../../docs/PLATFORM-SUPPORT.md) देखें।

### Project domains पर सावधानी

CPU और hardware models, GUI, networking, text processing, UTF-4088, π tools, Modium, Registry, `sleelactl` और AutoCAD Creator™ अलग-अलग project areas हैं। इनके उद्देश्यों, dependencies और maturity में अंतर हो सकता है। Experimental directory या example को documented validation के बिना stable production feature न कहें। UTF-4088 के आकार या entry count से पूर्ण Unicode compatibility सिद्ध नहीं होती। AutoCAD compatibility के लिए version-specific matrix और tests आवश्यक हैं।

### भारत के लिए भाषा और योगदान

भारत में SLeeLa सामग्री को हिन्दी और अंग्रेज़ी के साथ বাংলা, తెలుగు, मराठी, தமிழ், ગુજરાતી, ಕನ್ನಡ, മലയാളം, ਪੰਜਾਬੀ, ଓଡ଼ିଆ, অসমীয়া, اردو तथा संस्कृतम् जैसे पाठक-समुदायों तक पहुँचाने का लक्ष्य है। यह README इन सभी भाषाओं का पूर्ण तकनीकी अनुवाद नहीं है; पूर्ण पाठ हिन्दी में है। तकनीकी identifiers, commands, filenames और code को मूल रूप में रखना आवश्यक है; स्थानीय भाषा के technical terms का native-speaker review भी आवश्यक है। योगदानकर्ता अपने भाषा-क्षेत्र के लिए अनुवाद, terminology glossary और build/test walkthrough जोड़ सकते हैं।

**अनुवाद स्थिति:** खंड I–XIX का हिन्दी में अनुवाद किया गया है। तकनीकी identifiers, commands, code उदाहरण, फ़ाइल नाम, links और addresses यथावत रखे गए हैं। अन्य भारतीय भाषाओं में पूर्ण तकनीकी अनुवाद अभी शेष है; हिन्दी पाठ की मातृभाषा-भाषियों द्वारा अंतिम भाषाई समीक्षा और संबंधित भाषाओं के तकनीकी समीक्षकों द्वारा जाँच अनुशंसित है।
