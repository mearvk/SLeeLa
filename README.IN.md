# SLeeLa 3.0 — भारत (IN)

## भाषा संस्करण / Language editions

यह भारतीय संस्करण English, हिन्दी, বাংলা, తెలుగు, मराठी, தமிழ், ગુજરાતી, ಕನ್ನಡ, മലയാളം, ਪੰਜਾਬੀ, ଓଡ଼ିଆ, অসমীয়া, اردو और संस्कृतम् के पाठकों के लिए है। नीचे दिया गया व्यावहारिक तकनीकी मार्गदर्शन हिन्दी और English में है; अन्य भाषाओं के परिचय सुरक्षित रखे गए हैं। तकनीकी identifiers, source code, commands, filenames और API names को अपरिवर्तित रखा जाता है।

SLeeLa एक native systems programming भाषा, compiler, runtime और virtual-machine ecosystem है। भाषा, compiler, libraries, developer tools और platform contracts मिलकर software बनाने और चलाने की व्यवस्था प्रदान करते हैं।

SLeeLa একটি নেটিভ সিস্টেম প্রোগ্রামিং ভাষা। / SLeeLa ఒక స్థానిక సిస్టమ్ ప్రోగ్రామింగ్ భాష. / SLeeLa ही नेटिव्ह सिस्टम प्रोग्रामिंग भाषा आहे. / SLeeLa ஒரு இயல்பான கணினி அமைப்பு நிரலாக்க மொழி. / SLeeLa એક નેટિવ સિસ્ટમ પ્રોગ્રામિંગ ભાષા છે. / SLeeLa ಸ್ಥಳೀಯ ಸಿಸ್ಟಮ್ ಪ್ರೋಗ್ರಾಮಿಂಗ್ ಭಾಷೆಯಾಗಿದೆ. / SLeeLa ഒരു നേറ്റീവ് സിസ്റ്റം പ്രോഗ്രാമിങ് ഭാഷയാണ്. / SLeeLa ਇੱਕ ਨੇਟਿਵ ਸਿਸਟਮ ਪ੍ਰੋਗਰਾਮਿੰਗ ਭਾਸ਼ਾ ਹੈ. / SLeeLa ଏକ ନେଟିଭ୍ ସିଷ୍ଟମ୍ ପ୍ରୋଗ୍ରାମିଂ ଭାଷା। / SLeeLa এটা নেটিভ ছিষ্টেম প্ৰগ্ৰামিং ভাষা। / SLeeLa ایک مقامی نظامی پروگرامنگ زبان ہے۔

## I. शुरुआत करें

- [Native implementation बनाएँ](impl/README.md)
- [Virtual machine और Modium उपयोगकर्ता मार्गदर्शिका](virtual-machine/USER-GUIDE.md) — `sleela vm`, `sleela modium`
- [SLeeLa source में CPU architecture](lib/cpu/CPU.md) — 130 से अधिक CPU profiles; [timing controls](lib/cpu/TIMING.md) देखें।
- [दस्तावेज़ सूची](docs/README.md)
- [निर्मित API दस्तावेज़](docs/api/README.md) और [API catalogue](api/README.md)
- [Platform support और validation](docs/PLATFORM-SUPPORT.md)
- [Testing policy](markdown/TESTING.md)

## II. निर्माण और परीक्षण

**आवश्यकताएँ:** C/C++ toolchain (GCC या Clang), GNU Make और Python 3। Linux के graphical components के लिए X11/Xrandr development packages भी आवश्यक हो सकते हैं।

Repository के root directory से चलाएँ:

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

किसी एक component का सफल build यह प्रमाणित नहीं करता कि सभी libraries या platforms release के लिए तैयार हैं। प्रत्येक target की आवश्यकताओं के लिए build और platform documentation देखें।

## III. स्थापना

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

सामान्य system installation prefix `/usr/local` है। `PREFIX="$HOME/.local"` उपयोगकर्ता के अपने स्थान पर installation करता है और सामान्यतः administrator privileges की आवश्यकता नहीं होती। अधिक जानकारी के लिए [installation guide](install/README.md) देखें।

## IV. भाषा और runtime

Normative grammar [`SLEELA.syntax`](SLEELA.syntax) में परिभाषित है। Repository documentation में syntax version **3.0** का उपयोग होता है। Compiler version और source-language syntax version अलग-अलग अवधारणाएँ हैं।

तकनीकी संदर्भ में `@mix` contiguous fields, विस्तारित integer families, Preonem document markers, SleelaDoc, API documentation generation और Nordshrift tools शामिल हैं। Technical identifiers, commands, filenames, API names और code examples को ज्यों का त्यों रखा जाता है।

## V. Engineering सिद्धांत और validation

SLeeLa evidence, स्पष्ट component contracts, portability, failure के समय सुरक्षित व्यवहार और reproducible releases पर बल देता है। Validation को module और platform के अनुसार जाँचना चाहिए। छोड़े गए checks को सफल नहीं बताया जाना चाहिए; एक component का सफल build पूर्ण release validation नहीं है।

Sleelavirin protection tooling में local SHA-256 verifier, वैकल्पिक ClamAV adapter और integrity manifest शामिल हैं। किसी release को validated तभी माना जाना चाहिए जब release commit के CI checks पूरे हों और बने हुए artifacts की समीक्षा हो चुकी हो।

## VI. Editors और योगदान

SLeeLa **Vim** और **Emacs** के लिए themes तथा syntax integration और native C/C++ palette utility प्रदान करता है। [Editor integration guide](linux-editors/README.md) देखें।

योगदान के लिए:

1. Documentation index और testing policy पढ़ें।
2. स्पष्ट दायरे वाले बदलाव करें और positive तथा negative tests जोड़ें।
3. संबंधित tests चलाएँ, फिर व्यापक test suite चलाएँ।
4. प्रयुक्त operating system और toolchain बताएँ। छोड़े गए checks को सफल न लिखें।

SLeeLa का विकास **MEARVK LLC** द्वारा किया जाता है।

## VII. अन्य तकनीकी विषय

मुख्य README में Advantage, SleelaDoc और XML DOM libraries; मूल integer types; character runtime; AutoCAD Creator™; experimental UTF-4088 catalogue; Pi, Modium, Registry और `sleelactl` tools; तथा `@native` marker से सक्रिय native C/C++ compilation path का प्रारंभिक चरण भी शामिल है।

## पूर्ण तकनीकी संदर्भ

[Canonical README](README.md) में संपूर्ण feature catalogue, language और runtime reference, build तथा installation निर्देश, API links, examples और release notes हैं। यह भारतीय संस्करण हिन्दी में व्यावहारिक तकनीकी मार्गदर्शन जोड़ता है और अन्य सूचीबद्ध भाषाओं के परिचय सुरक्षित रखता है; यह अभी पूरे canonical README का पंक्ति-दर-पंक्ति अनुवाद नहीं है। पूर्ण अनुवाद और तकनीकी समीक्षा होने तक English README ही normative technical source रहेगा।

- [Canonical README](README.md)
- [SLeeLa syntax specification](SLEELA.syntax)
- [Documentation index](docs/README.md)
- [Platform support](docs/PLATFORM-SUPPORT.md)
- [GitHub repository](https://github.com/mearvk/SLeeLa)

**अनुवाद स्थिति:** हिन्दी में परिचय, शुरुआत, build, installation, भाषा/runtime overview, engineering, validation और contribution guidance जोड़ी गई है। अन्य भारतीय भाषाओं में पूर्ण तकनीकी अनुवाद अभी शेष है और संबंधित भाषाओं के तकनीकी समीक्षकों द्वारा जाँचा जाना चाहिए।


## VIII. SLeeLa की संरचना और compilation pipeline

SLeeLa में source language, compiler frontend, intermediate representation, code generation और runtime अलग-अलग स्तर हैं। Compiler release, syntax edition और VM/ISA version को एक ही version number न मानें। किसी feature का specification में होना यह सिद्ध नहीं करता कि वह हर target पर समान रूप से उपलब्ध और validated है।

सामान्य pipeline:

1. **Source:** version pragma और declarations वाली UTF-8 `.sleela` फ़ाइलें।
2. **Lexing और parsing:** source को tokens और syntax tree में बदलना।
3. **Semantic analysis:** names, symbols, types और language rules की जाँच।
4. **Intermediate representation:** आगे lowering और optimization के लिए संरचित program form।
5. **Code generation:** समर्थित VM या intermediate targets के लिए output।
6. **Runtime:** उपलब्ध memory-management और control-flow सुविधाओं के साथ compiled program का execution।

मुख्य भाषा-संदर्भ [SLEELA.syntax](SLEELA.syntax) है। Version pragma, source lifecycle और compiler व्यवहार के लिए [source documentation](markdown/SOURCE.md) तथा [compiler documentation](markdown/COMPILER.md) देखें।

## IX. भाषा के प्रकार और object model

SLeeLa की source vocabulary में `int`, `double`, `boolean`, `String`, `void`, घोषित `struct` प्रकार और समर्थित reference-type signatures शामिल हैं। `int`, `long` और `int64` वर्तमान दस्तावेज़ित मॉडल में signed 64-bit integers हैं। `int128` को integer type के रूप में स्वीकार किया जाता है, लेकिन इसे पूर्ण 128-bit arithmetic न मानें।

एक `struct` named fields वाला aggregate है। VM-managed struct handles raw pointers नहीं हैं। Assignment और parameter passing उसी instance को साझा करते हैं; equality identity-based है। Portable interchange के लिए `structPack(instance)` और `structUnpack(TypeName, json)` जैसे documented serialization helpers उपयोग करें; VM-local handle को सीधे network पर भेजने योग्य मानना गलत होगा।

Classes में fields और methods होते हैं। वर्तमान execution model में fields shared class state हैं और method-local variables thread-local होते हैं। Inheritance, nested classes और generics syntax-version gates के अधीन हैं। विवरण [International Language Reference](LANGUAGE.INTERNATIONAL.3.0.md) में है।

## X. नियंत्रण प्रवाह, loops और समय

भाषा में conditional statements, loops, method calls और recursion के लिए syntax उपलब्ध है। Loop timing के लिए statement-level annotations `@loopDelay` और `@loopBudget` documented हैं। ये loop के पहले लिखे जाते हैं और execution में वास्तविक pause या कुल समय-बजट लागू करते हैं। दोनों VM instruction `OP_SLEEP` (ISA 157) तक lower होते हैं; सामान्य `sleep(ms)` built-in भी उपलब्ध है।

उदाहरण:

```sleela
@loopDelay(ms = 250)
while (running) {
    processNext();
}
```

यह उदाहरण syntax का नमूना है; वास्तविक identifiers और enclosing class आपके program में परिभाषित होने चाहिए। विनिर्देश [SLEELA.syntax §32](SLEELA.syntax) और [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md) देखें।

## XI. Standard library और प्रमुख modules

Repository में अलग-अलग maturity वाले अनेक library modules हैं:

- `lib/advantage` — model के सापेक्ष परिणाम का layered मूल्यांकन; mathematics, bilateral trade economics, integration, grading और routable output। Physics संबंधी trust-field चर्चा को स्पष्ट रूप से modeling analogy समझें।
- `lib/xml` — XML document tree, elements, text, comments, CDATA, processing instructions, attributes, parser और serializer।
- `lib/text` — character-cell आधारित text और string operations, जैसे `charAt`, `substring`, `indexOf`, `reverse` और positional formatting।
- `lib/sleeladoc` — source documentation extraction और CLI reference pages। `sleela docs` topics, packages और classes खोजता है; `sleela docs --generate [dir]` offline text corpus बनाता है।
- `lib/cpu` और `lib/xlt` — CPU profiles, architecture components, model format और timing references।
- `recursion/stanchion` — C/C++ reference prototypes तथा recursion examples।

Module की मौजूदगी production readiness का प्रमाण नहीं है। उसके README, test coverage, platform limitations और validation record को अलग से जाँचें।

## XII. C/C++ और native boundary

SLeeLa repository में C तथा C++ components और सीमित native code-generation path शामिल हैं। `@native` और `sleela aot` जैसे नाम native development से संबंधित हैं; इन्हें arbitrary native code चलाने की सामान्य अनुमति न समझें।

C/C++ integration में ABI, calling convention, memory ownership, data layout, error handling और platform dependencies स्पष्ट होने चाहिए। Native code को अलग trust boundary मानें। कोई reference या declaration अपने-आप execution authorization नहीं देता। [Native implementation](impl/README.md), [platform matrix](docs/PLATFORM-SUPPORT.md) और [comparison standards](comparison/README.md) देखें।

## XIII. Build, installation और platform support

सामान्य native build के लिए C/C++ toolchain (जैसे GCC या Clang) और GNU Make की आवश्यकता होती है। कुछ tooling Python 3 का उपयोग करती है। Linux GUI components के लिए X11/XRandR development packages आवश्यक हो सकते हैं।

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

System-wide installation का सामान्य रूप:

```sh
sudo make install
```

केवल वर्तमान user के लिए:

```sh
make install PREFIX="$HOME/.local"
```

Platform-specific prerequisites और scripts के लिए [installation guide](install/README.md) तथा [platform support matrix](docs/PLATFORM-SUPPORT.md) पढ़ें। Linux, macOS और Windows पर एक ही command या dependency set मानकर न चलें।

## XIV. सुरक्षा, integrity और testing

Hash manifests और verification tools repository files की अपेक्षित सामग्री से तुलना कर सकते हैं। PASS परिणाम केवल जाँचे गए scope और समय पर लागू होता है। Test reports में pass, fail और skip को अलग-अलग दिखाना चाहिए। छोड़े गए या न चलाए गए tests को सफल न लिखें।

मुख्य नियम:

- References resources की पहचान करते हैं; वे execution की अनुमति नहीं देते।
- Native interfaces स्पष्ट और सीमित हों।
- Platform-support दावे वास्तविक builds और tests पर आधारित हों।
- Generated documentation को source और generator तक trace किया जा सके।
- आवश्यक integrity data अनुपस्थित हो तो security checks सुरक्षित रूप से fail हों।

[Testing policy](markdown/TESTING.md) और [platform validation](docs/PLATFORM-SUPPORT.md) देखें।

## XV. Documentation, editors और योगदान

[Documentation index](docs/README.md), [generated API reference](docs/api/README.md), [API catalogue](api/README.md) और [SleelaDoc](lib/sleeladoc/README.md) से शुरुआत करें। Linux पर editor resources के लिए [linux-editors](linux-editors/README.md) देखें।

योगदान से पहले संबंधित module का README पढ़ें, बदलाव का scope स्पष्ट करें, positive और negative tests जोड़ें, relevant tests चलाएँ, OS/toolchain/commands/results दर्ज करें और public behavior बदलने पर README/API documentation अपडेट करें।

## XVI. प्रमुख project domains

CPU और hardware models, GUI, networking, text processing, UTF-4088, π tools, Modium, Registry, `sleelactl` और AutoCAD Creator™ अलग-अलग project areas हैं। इनके उद्देश्यों, dependencies और maturity में अंतर हो सकता है। Experimental directory या example को documented validation के बिना stable production feature न कहें। UTF-4088 के आकार या entry count से पूर्ण Unicode compatibility सिद्ध नहीं होती। AutoCAD compatibility के लिए version-specific matrix और tests आवश्यक हैं।

## XVII. भारत के लिए भाषा और योगदान

भारत में SLeeLa सामग्री को हिन्दी और अंग्रेज़ी के साथ বাংলা, తెలుగు, मराठी, தமிழ், ગુજરાતી, ಕನ್ನಡ, മലയാളം, ਪੰਜਾਬੀ, ଓଡ଼ିଆ, অসমীয়া, اردو तथा संस्कृतम् जैसे पाठक-समुदायों तक पहुँचाने का लक्ष्य है। यह README अभी इन सभी भाषाओं का पूर्ण तकनीकी अनुवाद नहीं है। तकनीकी identifiers, commands, filenames और code को मूल रूप में रखना आवश्यक है; स्थानीय भाषा के technical terms का native-speaker review भी आवश्यक है। योगदानकर्ता अपने भाषा-क्षेत्र के लिए अनुवाद, terminology glossary और build/test walkthrough जोड़ सकते हैं।

**अनुवाद स्थिति:** हिन्दी मार्गदर्शिका को विस्तारित करके compiler pipeline, type/object model, loop timing, libraries, native integration, build/install, security, testing और contribution workflow शामिल किए गए हैं। यह अभी canonical English README और सभी linked specifications का line-by-line अनुवाद नहीं है।
