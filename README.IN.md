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
