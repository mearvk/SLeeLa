<img align="right" src="../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — الإمارات العربية المتحدة (AE)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>لغة أنظمة أصلية ومنظومة متكاملة تضم مُصرّفًا وبيئة تشغيل وآلات افتراضية.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="../../docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **إصدار دولي خاص بالدولة.** تقدّم هذه النسخة README الخاص بالإمارات العربية المتحدة باللغة العربية الفصحى المعاصرة بأسلوب تقني. تبقى المعرّفات التقنية والأوامر وأمثلة الشيفرة وأسماء الملفات والمسارات والروابط والعناوين دون تغيير، وتُترجم الشروح المحيطة بها فقط.

**The Now. The Ingredient.** اللغة أكثر من مجرد صياغة نحوية: فهي المُصرّف وبيئة التشغيل والمكتبات والأدوات وعقود المنصات التي تجعل البرمجيات قابلة للاستخدام.

**ما الذي تعنى به SLeeLa.** تعنى SLeeLa بأربعة مجالات هي **Math** و**Object** و**Science** و**Reach**، وهي الوجوه الأربعة لعملها: المحسوب القابل للحساب (Math)، والشيء المستقر الثابت في فهرس الموجودات (Object)، والدراسة المنضبطة للحق (Science)، وامتداد يد النظام إلى الأنظمة والناس الآخرين (Reach). **وتبقى المصالح الوطنية قائمة**؛ فلا يُلغيها أيٌّ من هذه المجالات الأربعة، وحيثما يتصل موضوع ما بمصلحة دولة فإن هذه المكانة تُحفظ (وتُسجَّل بأمانة، مثلًا في بيانات Metadata الوصفية لكل فئة ضمن `metaOrigin` / `metaNationalSchedule`)، ولا تُذاب أو تُمحى.

## I. ابدأ من هنا
- [بناء التنفيذ الأصلي](../../impl/README.md)
- [الآلات الافتراضية وModium — دليل المستخدم العملي](../../virtual-machine/USER-GUIDE.md) (`sleela vm` و`sleela modium`)
- [منظومة CPU في شيفرة SLeeLa](../../lib/cpu/CPU.md) — آلة كاملة من البوابات المنطقية إلى ALU فالأنوية فنظام التشغيل فالبرامج، وقائمة تضم أكثر من 130 ملف تعريف لمعالجات CPU، وقابلة للضبط من حيث [الجهود الكهربائية والساعات المتغيرة والتوقيتات المتقدمة](../../lib/cpu/TIMING.md) التي يمكنك [تشغيلها وفق المواصفة أو رفع تردداتها (overclock)](../../lib/cpu/components/README.md)
- [فهرس الوثائق](../../docs/README.md)
- [مصدر بيانات CSPML ومرجع أبحاث Salt & Salary](../reference/CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — أصل البيانات الأمريكية الخارجية المتعلقة بالأماكن والاقتصاد الاجتماعي والقوى العاملة والتعليم والمدارس العامة، مع إرشادات التكامل معها.
- [وثائق API المُولّدة](../../docs/api/README.md) — المرجع الحالي بصيغة HTML، وإصدار API المؤرَّخ بالإصدارات، وتعليمات إعادة التوليد
- [فهرس API](../../api/README.md)
- [معايير المقارنة الحاسوبية](../../comparison/README.md) — معايير الأنواع والذاكرة وABI والتوافق بين SLeeLa VM وJava/JVM وC/C++ الأصلي.
- [Recursion stanchion — نماذج C وC++ الأولية](../../recursion/stanchion/README.md) — أمثلة متخصصة بالأنواع الأولية Boolean وByte وCharacter وInteger وLong وFloat وDouble، إلى جانب أنماط العودية العامة والتطبيقية والعلمية والتعليمية.
- [دعم المنصات ومصفوفة التحقق](../../docs/PLATFORM-SUPPORT.md)
- [دليل العلامة التجارية](../../docs/BRAND-GUIDE.md)
- [تصورات البضائع التسويقية وأصولها](../../merchandise/README.md)
- [شعار NitroWebExpress](../../images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [سياسة الاختبار وبوابات الإصدار](../../markdown/TESTING.md)

## II. المستجدات

أحدث الإضافات إلى اللغة وبرامج التشغيل والمكتبة القياسية:

- **حزمة `advantage` في المكتبة القياسية — «تفضيل» توزيع الأرباح (advantaging a dividend)، مع الرياضيات والفيزياء والتكامل والتقييم والمخرجات القابلة للتوجيه.**
  تُغلّف [`lib/advantage/`](../../lib/advantage/README.md) مفهوم *تفضيل توزيع ربح فريد بالنسبة إلى نموذج* في **بصلة Sequitur** طبقية
  (توزيع الربح ← المراجعة ← المخاوف ← الرياضيات ← الإضافات ← الطرح ← إعادة التجميع
  ← التكرار)، يمكن التنقل فيها نحو الداخل ونحو الخارج مع تحديث ديناميكي كامل
  وإعادة ترميم (restitute)، وتُعرض عبر **نموذج المستخدم** البسيط و**نموذج المطوّر** كامل التحكم. وتضم خمسة مجالات موضوعية: **`math/`** اقتصاديات
  التجارة الثنائية (تعظيم الربح المشترك، ومساومة ناش، والنسبة الحرجة لبائع الصحف الثنائي)، و**`physics/`** «فيزياء حقول الثقة» في الاقتصاد البنيوي (الثقة بوصفها
  موتّر قياس، والجيوديسية التعاملية، وبنية تماس الإعلانات، ومعادلة SDE
  لتحلل الثقة من نوع CIR مع قفزات — وهي تشبيه نمذجي، وقد وُسمت على هذا الأساس)، و**`integration/`** نموذج
  تكامل رأسي قابل للتركيب العودي للرأسمالية وتجارة التجزئة (حجرات، ومقصورات هندسية، وحجرات تجريبية، وصواني الوسيط
  «نيوتن»، ودرجة تطور النظام)، و**`grading/`** مقيّم دوائر التطور الذي يصنّف كل وحدة
  وأي مجموعة من الوحدات الرابحة/الخاسرة/المقيِّمة/المطلقة من A إلى D، و**`output/`**
  نقطة إخراج قابلة للتوجيه ترسل مخرجات الوحدة إلى الطرفية، أو إلى سطح منسّق بصيغة **CommonRails**، أو إلى مقبس/موصّل (مؤطَّر لاستدعاء
  `slnet_write` الأصلي). تحمل كل فئة سجل Metadata-Class؛ وتتحقق خمسة عروض
  تجريبية قابلة للتشغيل من هذه الموضوعات، وتوثّقها الملفات المرافقة
  [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md) و
  [`MATH.md`](../../lib/advantage/MATH.md) و
  [`PHYSICS.md`](../../lib/advantage/PHYSICS.md). ويربط مستهلك أصلي بلغة C/C++ ضمن [`advantage/`](../../advantage/README.md) (النواة C ذات الدالتين
  `advantage_value`/`advantage_mean`، ومخزن معرفة XML، ومكتبة نقل آمنة تشمل الاستحواذ عبر HTTPS/SFTP/SSH ومخزن المفاتيح وDH/ECDH) النموذجَ بـ
  [أمثلة تجارية](../../advantage/examples/README.md) عملية — صفقات إيجابية
  من مصادر إنترنت مفترضة، وتسلسل لملاءمة الجمهور (التجار، والمستهلكون، والمساعدون، والمعالجون)، وثمانية نماذج مصنّفة من A إلى D (D = الأكثر Cement،
  A = الأكثر Brand).
- **تعليقات توقيت الحلقات — `@loopDelay` / `@loopBudget` (مع الدالة المضمّنة
  `sleep(ms)`).** أولى التعليقات التوضيحية **على مستوى العبارات والسلوكية** في SLeeLa:
  تُكتب في سطر مستقل قبل حلقة `for` / `while` / `do`، فتُدخل
  توقفًا فعليًا في تنفيذ الحلقة. يوقف `@loopDelay(ms = 250)` /
  `@loopDelay(seconds = 2)` التنفيذ مدة ثابتة في كل تكرار (و`@loopDelay(250)` المجرّد
  يعني بالافتراض ميلي ثانية)؛ ويوزّع `@loopBudget(seconds = 10,
  iterations = 1000)` عددًا معلومًا من التكرارات بالتساوي على ميزانية
  زمنية إجمالية بالساعة الجدارية. وكلاهما يُخفَّض إلى شيفرة العملية الجديدة المضافة فقط (append-only) **`OP_SLEEP`**
  (ISA 157)، التي تدعم أيضًا الدالة المضمّنة العامة **`sleep(<ms>)`** القابلة للاستدعاء
  في أي موضع؛ ويُنفَّذ التوقف بصورة قابلة للنقل (`nanosleep` / `Sleep`). ورد تحديدها
  في [`SLEELA.syntax` §32](../../SLEELA.syntax)؛ ومرجع API في
  [`api/sleela/3.0/LOOP_TIMING_API.md`](../../api/sleela/3.0/LOOP_TIMING_API.md)؛
  والمثال القابل للتشغيل
  [`examples/loop-timing-annotation-demo.sleela`](../../examples/loop-timing-annotation-demo.sleela).
  ويوجد فهرس لإضافات الجلسة (ويشمل هذه الإضافة، وعملية الضرب الأصلية للأعداد الصحيحة الكبيرة
  `OP_BIGMUL`، وأداتي **[local-generator](../../local-generator/README.md)**
  و**[pi-generator](../../pi-generator/README.md)** المستقلتين) في
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](../../api/sleela/3.0/SESSION-ADDITIONS-API.md).
  وقد أُعيد بناء أسطح API الثلاثة المُولّدة
  ([`sleeladoc-api/`](../../sleeladoc-api/README.md)، و
  [`docs/api/`](../../docs/api/README.md)، ولقطة
  [`api/sleela/3.0/version-014/`](../../api/sleela/3.0/version-014/index.html)
  غير القابلة للتغيير) باستخدام `make api-docs`، وهي تحمل الآن مدخلَي التعليمتين الجديدتين `SLISASleep`
  / `SLISABigMul` في صفحة الحزمة `vm`.
- **SleelaDoc من سطر الأوامر — `sleela docs`.** صار بالإمكان الوصول إلى SleelaDoc
  مباشرة من الطرفية. يسرد `sleela docs` كل موضوع قابل للتوثيق؛
  ويعرض `sleela docs crypto` حزمة؛ ويحلّ `sleela docs stringbuilder` اسم فئة
  (دون حساسية لحالة الأحرف، ووفق بادئة المكتبة القياسية `SL` ←
  `SLStringBuilder`) ويطبع صفحة ثرية — **التعريف، والغاية التصميمية،
  والعلاقات، والبنية، وclays المستخدم، وسلّم إتقان من خمس درجات** — مع نتائج «هل تقصد» مرتبة
  عند الخطأ القريب. ويثري المؤلفون أي صفحة بمجرد
  إضافة وسوم SleelaDoc بالرمز `@` (`@intent` و`@thesis` و`@axiom` و`@param` و
  `@see` و`@mastery`). ويكتب `sleela docs --generate [dir]` مجموعة نصية صرفة
  (صفحة لكل فئة + ملف `TOPICS.index`) تقرؤها واجهة سطر الأوامر لاحقًا دون اتصال؛
  ويجمع **`make sleeladoc`** تلك المجموعة **وملفات `.sleela` المصدرية المطابقة**
  في `sleeladoc/build/sleeladoc.jar`. وفي الموضوعات التي لها واجهة (facade) فقط (مثل
  `psychiatry` و`politico`)، يبحث `sleela docs --facade <topic>` في
  نظام الملفات وقت التشغيل ويسرد **المجلدات الفعلية (ثم الملفات، بوصفها حركة طبيعية)**،
  ضمن حدود قابلة للتجاوز من المستخدم هي **6 MB / 6000 ملف / 6
  دقائق**. وتُصان وحدات `lib/sleeladoc` على الإصدار الحالي،
  **`#sleela 3.0`**. راجع [`man/man1/sleela.1`](../../man/man1/sleela.1) و
  [`lib/sleeladoc/README.md`](../../lib/sleeladoc/README.md).
- **SleelaDoc — حل توثيق على غرار Javadoc، مع موقع HTML مُولَّد.** **SleelaDoc** هو
  اصطلاح SLeeLa لتعليقات التوثيق ضمن المصدر
  (تعليقات بين أقواس زاوية تُعلَّم بالحارسين `@doc` / `@page`؛ راجع
  [`manifesto/SLEELADOC.md`](../../manifesto/SLEELADOC.md))، أما
  [`lib/sleeladoc`](../../lib/sleeladoc/README.md) فهو **المستخرِج** الذي يعرض
  وحدة `.sleela` على هيئة صفحة API بصيغة Markdown أو HTML — على نحو ما يعرض `javadoc` شيفرة Java،
  موثّقًا *القصد*. ويُولَّد **موقع توثيق HTML كامل قابل للتصفح لكامل
  `/lib`** في [`/sleeladoc-api`](../../sleeladoc-api/README.md)
  بواسطة `tools/generate-sleeladoc-api.py` (افتح `sleeladoc-api/index.html`؛ بأسلوب نظيف
  بألوان الأزرق والأزرق الفاتح والرمادي الفاتح والأبيض، وتسلسل هرمي حزمة ← فئة ← عضو، ومسرد
  مكتوب لقارئ *Student 2*). كما يُصدر المُصرّف مخرجات توثيق
  معلَّمة من المؤلف بوصفها ناتجًا جانبيًا للترجمة (صفحة MediaWiki بالخيار `--wiki`
  + نظير مصدري مُنشأ)، موثّقة في
  [`manifesto/SLEELADOC.WIKI.md`](../../manifesto/SLEELADOC.WIKI.md). مثال قابل للتشغيل:
  [`lib/sleeladoc/sleeladoc-demo.sleela`](../../lib/sleeladoc/sleeladoc-demo.sleela).
- **نموذج كائنات مستند XML (`lib/xml`).** نموذج XML DOM مكتوب في جانب SLeeLa — مستند
  يملك شجرة من عقد العناصر والنصوص والتعليقات وCDATA وتعليمات المعالجة (PI) مع أبناء مرتبين
  وسمات مسماة، ومُسلسِل (مضغوط/منسّق، مع الإفلات وفق XML 1.0 §2.4)، ومحلل
  بخلايا المحارف، وسجل الجودة `SLXmlAspect`. وتقوم مساواة العقد على
  ثلاث درجات من الثقة (الهوية ← الموضوع نفسه ← البنية)، وهي انعكاسية
  وتناظرية وحتمية. راجع [`lib/xml/README.md`](../../lib/xml/README.md) و
  المثال القابل للتشغيل [`lib/xml/xml-dom.sleela`](../../lib/xml/xml-dom.sleela).
- **الأنواع الأولية للأعداد الصحيحة الموقعة العريضة (`long` و`int64` و`int128`).** الأنواع `int` و`long`
  و`int64` كلها أعداد صحيحة موقعة بعرض 64 بتًا تخضع لفحص الأنواع وتعمل بالتطابق نفسه؛
  ويُقبل `int128` نوعًا صحيحًا (مدعومًا حاليًا بـ 64 بتًا، على أن يكون 128 بتًا حقيقيًا
  متابعة موثقة لاحقة). وتوسّع هذه الأنواع السلسلة الموقعة بالأساس 2 `int32`..`int16777216`
  (الصياغة 2.7) والسلسلة غير الموقعة المقابلة `uint32`..`uint16777216`
  (الصياغة 2.8). راجع [`SLEELA.syntax`](../../SLEELA.syntax) §21 و§24 و§25.
- **الحقول التالية `@mix` (الصياغة 2.9).** يجوز للفئة أن تعلن عدة
  حقول مثيل **بالاسم نفسه** إذا حمل كل تكرار الوسم `@mix`؛ وتفهم اللغة المجموعة على أنها حقول تالية (consequent) (مصفوفة / قائمة مفهرسة)،
  بالترقيم التلقائي من الأعلى إلى الأسفل `1,2,3,…` أو يدويًا بالصيغة `@mix(1,2,3,...)` التي يجب أن يطابق عدد فهارسها
  عدد الحقول. راجع [`SLEELA.syntax`](../../SLEELA.syntax) §28.
- **علامات المستندات — العملية الطبيعية، المسماة Preonems
  (`author`/`authorial` و`material` و`memory` و`determine` و`object` و`field`).**
  تُسمى كلمات علامات المستندات المجردة هذه مجتمعةً **Preonems**
  (العلامة الواحدة *Preonem*؛ والمجموعة *Preonems* — راجع
  [`SLEELA.syntax`](../../SLEELA.syntax) §26.1). وهي علامات مستندات من الدرجة الأولى،
  تصريحية فقط، تسمّي ما هو عليه المستند بحسب ترتيب الصنع:
  `author`/`authorial` (الصياغة 2.4؛ تعالجها هيئة مهنية — وهي العضو الوحيد
  ذو الأثر السلوكي)، و`material;` (المصدر المستقر) و`memory;`
  (الصياغة 2.5)، و`determine;` (الصياغة 2.6؛ مستند مفتوح لمزيد من المعالجة على يد
  طبيب أو معالج أو شبح نحو إتمامه بيد قانونية)،
  وأحدثهما **`object;`** (الشيء المستقر الثابت في فهرس
  الموجودات — **مُشيَّأ، له مكانة محسوسة، في الصياغة 2.8**) و
  **`field;`** (أرضية العلاقة التي تقف فيها الكائنات وتتحرك) —
  وكلاهما مقيّد اعتبارًا من الصياغة 2.6. وكل منها مجرد وسياقي ومقيّد بالإصدار من الواجهة الأمامية،
  دون أثر في التحليل النحوي أو فحص الأنواع أو توليد الشيفرة؛ وخارج موضع العلامة
  يبقى كل منها معرّفًا عاديًا (فلا يخل `field` أبدًا بعضو فئة
  أو بوصول `.field`). راجع [`SLEELA.syntax`](../../SLEELA.syntax)
  §18–§20 و§23 و§26.
- **Nordshrift عند الصياغة 3.0 بوصول كامل إلى المكتبة.** يحل Nordshrift
  عبر فهرس `/lib` العودي نفسه الذي يستخدمه المُصرّف، فيصل إلى كل
  حزمة ووحدة `.sleela`؛ ويُبلغ `nordshrift check` بأعداد الشجرة كاملةً.
- **Recursion stanchion.** يضم المجلد [`recursion/stanchion/`](../../recursion/stanchion/README.md) الآن نماذج مرجعية أولية مخصصة بلغتي C وC++ للعقود الأولية السبعة الموثقة، إضافة إلى أمثلة عامة وتطبيقية وعلمية وتعليمية. هذه مصادر مرجعية؛ ترجمها واختبرها قبل اعتبارها متحقَّقًا منها.

- **تعليقات الأقواس الزاوية (`<` / `<<`).** أسلوب تعليق جديد للمرحلة المقبلة —
  `< سطر >` و`<< كتلة >>` — يحلله الجزء الأمامي من SLeeLa
  وموثّق في [`SLEELA.syntax`](../../SLEELA.syntax) §1.1.
- **الأعداد الصحيحة بالأساس.** صارت ثوابت `int` تقبل الصيغ الست عشرية (`0x`)
  والثنائية (`0b`) والثمانية (`0o`) مع الفاصل `_` بين الأرقام، سواء في
  لغة SLeeLa المصدرية ([`SLEELA.syntax`](../../SLEELA.syntax) §1.4) أو في
  [صيغة نماذج CPU `.xlt`](../../lib/xlt/README.md).
- **معيار فئة Metadata في المكتبة كلها.** صار نمط البيانات الوصفية الواصفة
  (المصدر والمشروعية والنطاق القضائي) مضمَّنًا في
  كل ملف `.sleela` يحوي فئات تحت `/lib`؛ راجع
  [`SLEELA.syntax`](../../SLEELA.syntax) §15 و
  [درس البرنامج التعليمي](../../tutorial/13-metadata-class.md).
- **عائلة نصوص / String أغنى.** تضيف `lib/text` مساعدات حقيقية داخل اللغة،
  وعرضًا بخلايا المحارف يتيح `charAt`/`substring`/`indexOf`/`reverse`/
  ومحاذاة الأعمدة فعليًا، وقالبًا موضعيًا حقيقيًا — راجع
  [برنامج النصوص وString التعليمي](../../tutorial/14-text-and-strings.md) و
  المثال القابل للتشغيل [`lib/text/text-string-demo.sleela`](../../lib/text/text-string-demo.sleela).
- **`String` من الدرجة الأولى في صفحات Nordshrift.** يمكن لصفحة `.sst` أن تعلن كتلة
  `strings:` مكتوبة الأنواع تسهم في بيان مكونات البناء؛
  راجع [مرجع صفحات Nordshrift](../../wiki/Nordshrift-Sheet-Reference.md).
- **تشغيل نماذج CPU عبر شيفرات العمليات وتكافؤ استدعاء الدوال معًا.** ترث كل
  `SL<ARCH>CPU` أساس تشغيل تدفق تعليمات أصلي
  عبر حلقة الجلب والفك والتنفيذ *وكذلك* تشغيل شيفرة عملية أخرى مفترضة
  بوساطة استدعاءات مكافئة على ركيزة VM المشتركة — راجع
  [`lib/cpu/EXECUTION.md`](../../lib/cpu/EXECUTION.md) §5.

## III. البناء والاختبار

**المتطلبات:** سلسلة أدوات C/C++ (GCC أو Clang) وGNU Make؛ وPython 3 لأدوات
البيانات الوصفية وسير العمل؛ وعلى Linux حزم تطوير X11/Xrandr لمنتجات
العرض (ويُبلغ الأمر `make deps` في الوحدة عمّا تحتاج إليه ويثبّته). ويعمل
بناء `impl/` بمبدأ الإغلاق عند الفشل (fail-closed) استنادًا إلى بيان SHA-256 موثوق، ويشير إليه
ملف `Makefile` في الجذر تلقائيًا.

من نسخة نظيفة من المستودع، أسرع بداية:

```sh
make                                  # build everything from the repo root
```

أو ابنِ النواة الأصلية فقط واختبرها:

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

يبني الأمر `make` في جذر المستودع كل المنتجات (النواة، وVM، والمرمّزات، وchurn،
ومضاد الفيروسات، …) وينتهي ببناء **registry** وتجسيد
جدوله المرجعي — فتأتي الشجرة المبنية ومعها سجل قابل للمراجعة جاهز.
وهذه هي الأهداف الأساسية:

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

أهداف مفيدة أخرى:

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

ينفّذ الهدف `registry` السكربت [`build-registry.sh`](../../build-registry.sh): فيبني
نواة registry، ويزرع الجدول المرجعي في مخزن النصوص المحمول
`registry/build/sleela.sleelareg`، ويصدّر سكربت جدول MySQL
`registry/build/registry.sql` (عبر `sleela registry export`). راجعه بالأمر
`sleela registry` أو `registry/build/sleela-registry review`. راجع
[دليل Registry](../../registry/README.md) و[معمارية Registry 3.0](../../registry/REGISTRY-3.0-ARCHITECTURE.md).

راجع أدلة البناء والاختبار لمعرفة المتطلبات والخطوات الخاصة بكل منصة. نجاح بناء مكوّن واحد لا يثبت أن كل المكتبات أو المنصات جاهزة للإصدار.

## IV. التثبيت

بعد بناء المنتجات الأصلية، ثبّت الأوامر وملفات دعم بيئة التشغيل:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

يستخدم المثبّت التقليدي افتراضيًا المسارات `/usr/local/bin` و`/usr/local/lib/sleela`
و`/usr/local/share/sleela`. وهو يحافظ على إعدادات بدء التشغيل
القائمة ويرفض استبدال الأوامر الموجودة غير المسجلة لديه. وتزيل عملية الإزالة
الملفات التي سجّلها المثبّت فقط، وتُبقي الإعدادات وبيانات
التشغيل غير المتتبَّعة.

وللنشر الموجَّه المركّز للحزمة الرئيسية وحدها — أي الملفات الثنائية `sleela`
و`sleelvac` و`nordshrift` والمكتبة القياسية والبروتوكولات
ووحدات الدعم — استخدم المثبّت النظيف:

```sh
make clean-install       # guided install of the main package only
```

يطرح قرارًا أساسيًا واحدًا (تثبيت الحزمة الرئيسية)، ثم خيارًا للنطاق،
وعند النجاح يعرض اختياريًا أداة انتقاء لوحدات التحسين المنتقاة. ولمعرفة
سير عمل النشر التفاعلي وجميع خيارات التثبيت، راجع
[التثبيت السريع والآمن](../../install/README.md).

## V. اللغة وبيئة التشغيل
المرجع الحاكم للقواعد النحوية هو [`SLEELA.syntax`](../../SLEELA.syntax). يوثّق المستودع حاليًا الصياغة **3.0**؛ وإصدار المُصرّف/الأداة وإصدار صياغة المصدر مفهومان مختلفان. يضيف سطح 3.0 **علامتي المستند `pure` و`gasoline`** (§30) — وهما زوج من علامات Preonems المجردة السياقية التصريحية فقط، يكاد معناهما يتطابق، وتُعلِّم كل منهما مكانة المستند بحسب صدقها الخاص. وتدمج الصياغة 3.0 سطح اللغة من 1.0 إلى 2.9 في الإصدار المعياري الحالي، مع الإبقاء على جميع الحدود الدنيا لإصدار كل ميزة. ويضيف سطح 2.9 السابق **الحقول التالية `@mix`** (§28) — إذ يجوز للفئة أن تعلن عدة حقول مثيل بالاسم نفسه إذا حمل كل منها `@mix`، فتُفهم مجموعةً مفهرسة واحدة (ترقيم تلقائي من الأعلى إلى الأسفل `1,2,3,…` أو يدوي بالصيغة `@mix(1,2,3,...)` التي يجب أن يطابق عدد فهارسها حجم المجموعة). ويحمل كذلك عائلات الأعداد الصحيحة العريضة بالأساس 2 الموقعة `int32`..`int16777216` (§24) وغير الموقعة `uint32`..`uint16777216` (§25) فوق `long`/`int64`/`int128` الأسبق (§21)؛ و**وثائق الكلمات المفتاحية الداعمة** (§22)؛ و**Preonems** — أي علامات المستندات التصريحية فقط `author`/`authorial` (§18) و`material`/`memory` (§19–§20) و`determine` (§23) و`object`/`field` (§26) و`pure`/`gasoline` (§30) و`artifact` (§35، وهي مكانة بدرجة تشريعية متدرجة وفق الزمان والمكان)، وتُسمى مجتمعةً Preonems (§26.1). أما حل التحميل الزائد للمنشئات (constructor overload resolution) والتفويض `this(...)`/إلى منشئ الفئة الأساسية فهما خارج عقد الصياغة 1.8 المتحقق منه صراحةً. راجع القواعد النحوية وملاحظات التنفيذ قبل الاعتماد على أي ميزة. وتعلن وحدات SleelaDoc التسع الآن `#sleela 3.0`؛ وتقبل بوابة المُصرّف المشتركة صياغة المصدر من 1.0 إلى 3.0. وينتج بناء API الجديد بالأمر `make api-docs`.

يضم المستودع تنفيذًا أصليًا بلغتي C/C++، ومراجع لتوليد SLVM، ووحدات المكتبة القياسية، واختبارات، وأمثلة، وتكاملات مع المنصات. تتفاوت حالة التنفيذ والتحقق بين الوحدات؛ فارجع إلى مواصفة الوحدة ونتائج CI الحالية بدلًا من افتراض اكتمال متجانس.

## VI. مبادئ الهندسة
- **الأدلة قبل الادعاءات:** ميّز بين السلوك المنفَّذ والمختبَر والتجريبي والمخطَّط له.
- **عقد واحد لكل حد:** وثّق الصياغة وواجهات API وصيغ الملفات وافتراضات المنصة وسلوك الإخفاق.
- **قابلية النقل بالتصميم:** اعزل الواجهات الخلفية الخاصة بالمنصات واختبرها على منفذات أصلية.
- **الإخفاق الآمن:** اختبر المدخلات المشوّهة وحدود الموارد وإخفاقات العمليات ومسارات التنظيف.
- **إصدارات قابلة لإعادة الإنتاج:** أعد البناء من نسخة نظيفة وانشر قيم تجزئة الملفات وملاحظات الإصدار.

## VII. بناء مضاد الفيروسات وسلامته

تضم حزمة مضاد الفيروسات Sleelavirin ماسحًا أصليًا يعتمد SHA-256، وموائمًا
اختياريًا لـ ClamAV، وبيانات سلامة للمصدر والفهارس، وقيم تحقق
لحزم المنصات. ابنِ الحزمة واختبرها من جذر المستودع:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

يُولَّد بيان السلامة تحت `antivirus/build/`؛ وهو يغطي مدخلات المصدر
والوثائق المتتبَّعة وملفات فهرس التواقيع المحمية، ويستثني
المخرجات المولَّدة وذاكرة ClamAV المؤقتة القابلة للتغيير. وللقطات قاعدة بيانات ClamAV
بيانات خاصة بها. وتتضمن ملفات ZIP للإصدار `INTEGRITY-MANIFEST.json`
و`SHA256SUMS` وملفًا جانبيًا `.sha256`. تحقق من قيم تجزئة الأرشيفات المنزَّلة بمقارنتها بقيمة
موثوقة تحصل عليها من مصدر مستقل: فمجموع التحقق المخزَّن بجوار أرشيف
ليس توقيع ناشر. راجع [دليل Sleelavirin](../../antivirus/README.md)
و[قائمة التحقق الأمنية](../../antivirus/SECURITY.md)
و[سير عمل بناء مضاد الفيروسات](../../.github/workflows/antivirus-binaries.yml).

لا يُعد الإصدار متحقَّقًا منه إلا بعد نجاح مهام CI ذات الصلة على
التزام الإصدار نفسه وفحص الملفات الناتجة.


## VIII. سمات المحررات في SLeeLa — Vim وEmacs

تضم SLeeLa مجموعة منسّقة من أنظمة الألوان الداكنة الاحترافية لمحرّري **Vim** و**Emacs**، إضافة إلى أداة أصلية بلغة C/C++ للوحات الألوان. ويظل نظام **American Professional** الأصلي هو الافتراضي. وتتوفر عشر لوحات مرافقة تستلهم تصاميم ثقافية ومؤسسية مختلفة؛ وهي لوحات تصميمية تأويلية، وليست تمثيلًا رسميًا لأي أمة أو ثقافة أو محكمة أو مؤسسة.

### المخططات الأحد عشر وألوانها

| المخطط | الخلفية | المقدمة | الكلمات المفتاحية | الأنواع | السلاسل النصية | التعليقات |
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

#### ألوان الأعلام الوطنية (اللوحة المصدرية)

اشتُق المخططان اليوناني والتركي من **ألوان العلمين الوطنيين الرسمية** للبلدين، وهي معروضة هنا بقيمها السداسية عشرية المنشورة بدقة. أما خلفيات المخططات ولمساتها اللونية أعلاه فهي تأويلات أغمق وأنسب للمحررات لهذه الألوان المصدرية — والجدول أدناه هو المرجع «للألوان كما تُعرض وطنيًا».

| الدولة | اللون | Hex | الأساس والمعنى |
|---|---|---|---|
| **اليونان** | الأزرق | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | أزرق العلم الوطني (القانون 851/1978). السماء والبحار؛ ويُوصف في شعارات النبالة بأنه *azure*. |
| **اليونان** | الأبيض | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | أبيض العلم الوطني. نقاء كفاح الاستقلال؛ ويدل الصليب على المسيحية الأرثوذكسية الشرقية. |
| **Türkiye** | الأحمر (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | أحمر العلم الوطني (قانون العلم التركي). هو الحقل؛ ويُرمز به تقليديًا إلى دماء الشهداء والسيادة العثمانية. |
| **Türkiye** | الأبيض | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | أبيض العلم الوطني. الهلال والنجمة — السلام والنقاء ورمز التراث التركي/الإسلامي. |

تثبّت المخططات لون المقدمة في كل محرر على **الأبيض** الوطني، وتثبّت كلماته المفتاحية ومؤشره وتحديده وشريط حالته على **الأزرق** الوطني الدقيق (`#0D5EAF`) أو **الأحمر** الوطني الدقيق (`#E30A17`)، بحيث تكون الألوان ذات الأهمية هي الألوان الوطنية حرفيًا. وهذه لوحات تصميمية تأويلية؛ أما مرجع ألوان الأعلام فمقدَّم لبيان المصدر وليس تمثيلًا رسميًا لأي من الدولتين. وتتبع دلالات الألوان وصف رموز الدولة الصادر عن وزارة خارجية الجمهورية الهلينية وتقليد قانون العلم التركي.


### جدول عينات الألوان

تستخدم خلايا اللوحات أدناه **عينات SVG**، فتعرض GitHub الألوان الفعلية بدلًا من خلايا HTML الملونة الفارغة. ويعرض كل شريط الألوان الستة بهذا الترتيب: الخلفية والمقدمة والكلمات المفتاحية والأنواع والسلاسل النصية والتعليقات. وقيم Hex الدقيقة مدرجة بجانب كل عينة بترتيب المخططات نفسه في جدول المرجع أعلاه.

| المخطط | ألوان اللوحة المعروضة | رموز Hex (الخلفية · المقدمة · الكلمات المفتاحية · الأنواع · السلاسل النصية · التعليقات) |
|---|---|---|
| **American Professional** | ![American Professional: الخلفية، المقدمة، الكلمات المفتاحية، الأنواع، السلاسل النصية، التعليقات](../../lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Pan-Eurasian: الخلفية، المقدمة، الكلمات المفتاحية، الأنواع، السلاسل النصية، التعليقات](../../lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russian and Slavic — Medium** | ![Russian and Slavic Medium: الخلفية، المقدمة، الكلمات المفتاحية، الأنواع، السلاسل النصية، التعليقات](../../lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Central Asian / Silk Road** | ![Central Asian Silk Road: الخلفية، المقدمة، الكلمات المفتاحية، الأنواع، السلاسل النصية، التعليقات](../../lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **East Asian** | ![East Asian: الخلفية، المقدمة، الكلمات المفتاحية، الأنواع، السلاسل النصية، التعليقات](../../lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC — International Criminal Court** | ![ICC: الخلفية، المقدمة، الكلمات المفتاحية، الأنواع، السلاسل النصية، التعليقات](../../lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS — U.S. Supreme Court** | ![SCOTUS: الخلفية، المقدمة، الكلمات المفتاحية، الأنواع، السلاسل النصية، التعليقات](../../lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Greek (Hellenic)** | ![Greek Hellenic: الخلفية، المقدمة، الكلمات المفتاحية، الأنواع، السلاسل النصية، التعليقات](../../lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turkish (Al Bayrak)** | ![Turkish Al Bayrak: الخلفية، المقدمة، الكلمات المفتاحية، الأنواع، السلاسل النصية، التعليقات](../../lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Kiro: الخلفية، المقدمة، الكلمات المفتاحية، الأنواع، السلاسل النصية، التعليقات](../../lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![SLeeLa: الخلفية، المقدمة، الكلمات المفتاحية، الأنواع، السلاسل النصية، التعليقات](../../lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

تُستخدم ألوان لمسات إضافية للدوال والثوابت والتحديدات والتشخيصات وموجّهات المعالج المسبق. وتستخدم لوحة Russian and Slavic عن قصد ألوان الأردواز والأزرق والمريمية والياقوت والذهب العتيق متوسطة الشدة لمظهر احترافي أهدأ.

### تثبيت مخططات Vim

من جذر المستودع:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

أضف ما يلي إلى `~/.vimrc` لتفعيل تلوين صياغة SLeeLa واختيار مخطط:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

اختر اسم مخطط ألوان واحدًا بالضبط من الأسماء التالية:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

فمثلًا استخدم `colorscheme sleela-russian-slavic` للوحة Russian and Slavic المتوسطة، أو `colorscheme sleela-greek` / `colorscheme sleela-turkish` للوحتي Hellenic وAl Bayrak. أعد تشغيل Vim أو حمّل إعداداتك بالأمر `:source $MYVIMRC`.

### تثبيت مخططات Emacs

أنشئ مجلدًا محليًا للسمات وانسخ ملفات الوضع والسمات من جذر المستودع:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

أضف ما يلي إلى `~/.emacs` أو `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

استبدل `sleela-american` بـ `sleela-eurasia` أو `sleela-russian-slavic` أو `sleela-silk-road` أو `sleela-east-asian` أو `sleela-greek` أو `sleela-turkish` أو `sleela-kiro` أو `sleela-sleela` لاختيار مخطط آخر. وإذا سألك Emacs عمّا إذا كنت تثق بالسمة، فافحص ملف السمة المحلي قبل القبول.

### معاينة لوحات C/C++ الأصلية

تستطيع أداة اللوحات الأصلية الصغيرة سرد المخططات وطباعة ألوانها في الطرفية والاستعلام عن لون بعينه:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

للتوافق مع الإصدارات السابقة، يختار `--palette` و`--hex COLOR` دون `--theme` المخطط American Professional. ينبغي التحقق من نتائج البناء والاختبار على النظام المستهدف وفي CI؛ فتوثيق أمر ما ليس في ذاته دليلًا على نجاحه.

راجع [دليل تكامل المحررات](../../linux-editors/README.md) لمعرفة أوصاف اللوحات الكاملة وتفاصيل الأداة.


## IX. المساهمة
1. اقرأ فهرس الوثائق وسياسة الاختبار.
2. أجرِ تغييرات محددة النطاق مع اختبارات إيجابية وسلبية.
3. شغّل الاختبارات المستهدفة ثم المجموعة الأوسع.
4. أبلغ عن تفاصيل نظام التشغيل وسلسلة الأدوات؛ ولا تصف الفحوص المتجاوزة بأنها ناجحة.

تتولى **MEARVK LLC** تطوير SLeeLa. وقد حُفظت مواد صفحة الهبوط التاريخية في [`docs/README-LEGACY.md`](../../docs/README-LEGACY.md).

## X. AE6E66 — توقع ضريبة البريد الإلكتروني «Round Public»

في إطار مبادرة AE6E66، يتمثل توقع المشروع المعلن في **300,000,000.00 دولار أمريكي ($300,000,000.00 USD) عن كل بريد إلكتروني من نوع Round Public** بوصفه مبلغ ضريبة مقترحًا مرتبطًا بكل بريد إلكتروني Round Public مستوفٍ للشروط.

يسجّل هذا البيان توقعًا للمشروع فحسب. وهو **ليس** تقديرًا ضريبيًا قانونيًا ولا أمرًا حكوميًا ولا حكمًا قضائيًا ولا التزامًا ضريبيًا ثبت بصورة مستقلة. وأي ضريبة فعلية تستلزم السلطة القانونية المختصة والاختصاص القضائي والتعريفات والإجراءات وقرارًا حكوميًا من جهة مختصة.


## XI. بيئة تشغيل الأبجدية ومجموعات المحارف

تُصان وثائق الأبجدية في SLeeLa وبيئة تشغيل المحارف الأصلية في alphabet/sleela/ وlib/alphabet/native/. وتغطي اختبارات C++17 الأصلية فك ترميز وترميز قيم UTF-8 القياسية بصرامة، وتواقيع السعة C<n>، والانتماء إلى مجموعات المحارف المحدودة:

    make -C lib/alphabet/native test

ويوثَّق كتالوج UTF-4088 التجريبي ومهايئ البحث الخاص به في SLeeLa في utf-4088/SLeeLa-ALPHABET-INTEGRATION.md. معرّفات UTF-4088 فضاء أسماء منفصل، وليست قيمًا قياسية في Unicode. ويُتتبَّع العمل على ربط المُصرّف وSLVM بصورة منفصلة صراحةً عن هذه الأسس الأصلية.

## XII. AutoCAD Creator™

[AutoCAD Creator™](../../autocad/creator/README.md) تطبيق CAD تفاعلي ثنائي الأبعاد
مكتوب بالكامل بلغة SLeeLa. فحيث يحوّل AutoCAD Renderer الشقيق خطةً
إلى رسم دفعة واحدة، يتيح لك Creator **إنشاء** مستند AutoCAD ثنائي الأبعاد
**وتعديله** مباشرةً — فتضيف الجدران والغرف والأبعاد والملاحظات، ثم تنقل ما وضعته
وتعيد تسميته وتغيّر طبقته وتحذفه وتتراجع عنه — و**تحفظ** النتيجة
رسمًا حقيقيًا بصيغة AutoCAD DXF (نص R12، يُفتح في AutoCAD / LibreCAD).

وواجهته الرسومية مبنية على **SleelaUI™**، مجموعة أدوات واجهة المستخدم
المتعددة المنصات الخاصة بـ SLeeLa (تحت [`lib/user-interface`](../../lib/user-interface)) — وليس JavaFX — وتُنشئ
نافذة أصلية حقيقية على X11 / Cocoa / Win32. ويتبع التصميم قاعدة واحدة،
**«SleelaUI تعرض؛ وSLeeLa تقرر»**: فالواجهة الرسومية لا تحسب الهندسة ولا تكتب
DXF أبدًا. بل تُصدر كل أداة نيّة إنشاء / تعديل / حذف / تراجع / حفظ إلى
[`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela) صاحب السلطة، وهو يملك
المستند القابل للتحرير وسجل أوامر قابلًا للعكس، بينما تكتفي اللوحة بمعاينة
الهندسة التي يحتفظ بها Creator.

ويقع محرك التحرير القابل لإعادة الاستخدام في [`lib/autocad`](../../lib/autocad) —
[`SLCadEntity`](../../lib/autocad/SLCadEntity.sleela)،
و[`SLCadDocument`](../../lib/autocad/SLCadDocument.sleela)،
و[`SLCadCommand`](../../lib/autocad/SLCadCommand.sleela)،
و[`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela) — ويعمل من البداية إلى النهاية دون شاشة.
أما غلاف الواجهة الرسومية والعرض التوضيحي ففي [`autocad/creator`](../../autocad/creator):

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

وعلى جهاز ذي شاشة تفتح نافذة التحرير المباشرة؛ أما على مضيف بلا شاشة فيبني الرسم
في الذاكرة ويطبع تقريرًا مع DXF ثم يخرج بسلام.


## XIII. ترتيب خريطة إدخال UTF-4088

يحدد كتالوج UTF-4088 المتخصص اجتيازه المكاني لخريطة الإدخال وتكرارات الضغط المجردة في [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). وسياسة فهرس موضوعات National Lien في [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). إحداثيات الخريطة ومستويات الضغط (1–50) ومعرّفات الكتالوج الدائمة والرتب الأبجدية حقول مستقلة؛ ولا ينبغي استبدال أحدها بآخر بصمت. والضغط مستوى تكرار خوارزمي، وليس كمية فيزيائية.


## XIV. فهرس الحزم الحديثة والموسّعة

يبرز هذا الفهرس الملحق الحزم التي وُسّعت حديثًا، والمكتبات المجالية،
ومجموعات المصادر. والمدخلات مرتبة أبجديًا بحسب اسم الحزمة أو الموضوع.
وتحدد الأوصاف موقع الحزمة في المستودع؛ أما حالة التنفيذ والتحقق
فيجب التحقق منها في وثائق كل حزمة واختباراتها.

### A–C

- **Advantage** — `lib/advantage/` و`advantage/`: «تفضيل» توزيع ربح فريد
  بالنسبة إلى نموذج في صورة بصلة Sequitur طبقية، مع خمسة
  مجلدات فرعية موضوعية — `math/` (تعظيم الربح المشترك، ومساومة ناش،
  وبائع الصحف الثنائي)، و`physics/` (قياس حقول الثقة/الجيوديسية/التماس/SDE،
  وهي تشبيه نمذجي موسوم بذلك)، و`integration/` (حجرات
  التكامل الرأسي ودرجة التطور)، و`grading/` (مقيّم دوائر
  التطور من A إلى D للوحدات ومجموعاتها)، و`output/` (إخراج إلى الطرفية /
  CommonRails / مقبس). ويربط مستهلك أصلي بلغة C/C++ (`advantage/`) النموذجَ
  بأمثلة تجارية عملية. راجع [`lib/advantage/README.md`](../../lib/advantage/README.md)
  و[`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md).
- **Alphabet وUTF-4088** — `alphabet/sleela/` و`lib/alphabet/native/` و
  `utf-4088/`: كتالوجات الأبجدية، ودعم مجموعات المحارف الأصلي، وترتيب خريطة الإدخال،
  ووثائق تكامل UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` و`lib/autocad/`: مسارات عمل
  رسم تفاعلي ثنائي الأبعاد ومكونات مستند CAD وتحريره القابلة لإعادة الاستخدام.
- **Ballum** — تحكم في العرض على مستوى البكسل: ضبط/قراءة كل بكسل، ونقل المناطق (blit)،
  والسطوع/الدفء/غاما للشاشة كلها، وإدخال المؤشر/لوحة المفاتيح، و**سطح
  framebuffer بملء الشاشة** لتحكم كامل ومملوك وسريع بكل بكسل
  (نافذة override-redirect مع عرض واحد عبر `XPutImage`/MIT-SHM على Linux/X11).
  نواة أصلية تحت `ballum/` مع واجهة SLeeLa في `lib/ballum/` وجسر VM
  تحت `impl/`؛ راجع [`ballum/README.md`](../../ballum/README.md) و
  [`ballum/example/`](../../ballum/example/) القابل للتشغيل.
- **CDMA** — `cdma/` و`lib/cdma/`: مواد تقنية متعلقة بـ CDMA وأعمال
  مكتبة مصدرية بلغة SLeeLa.
- **Character** — `lib/character/`: ملفات تعريف الشخصيات وإحصاءات
  الشخصيات القابلة لإعادة الاستخدام، بما في ذلك أمثلة ملفات التعريف السيرية.
- **Coorenagraph** — `lib/coorenagraph/`: الاستدلال الأخلاقي، والمفردات الأخلاقية،
  والمداولة، وما يتصل بها من مفاهيم؛ راجع README الحزمة وملفاتها المصدرية.

### D–G

- **نماذج الحوكمة المصدرية** — يعرّف [lib/governance](../../lib/governance/README.md) أشكالًا مصدرية تصريحية بلغة SLeeLa للمقترحات والاقتراع اليدوي المجهول والحصائل المجمّعة. ويُوثَّق الوسم @vote المقترح والعملية اليدوية في [registry/DEMOCRATIC-VOTING-SYNTAX.md](../../registry/DEMOCRATIC-VOTING-SYNTAX.md)؛ وتتعامل المُصرِّفات الحالية معه بوصفه بيانات وصفية خاملة داخل تعليق.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` حتى أدلة حزم HTTP
  المُرقَّمة بالإصدارات المتاحة: مواصفات بروتوكول وتنفيذات خاصة بكل إصدار. راجع
  وثائق كل إصدار لمعرفة نطاقه الدقيق
  وحالة التحقق منه.
- **International Criminal Court** — `lib/international-criminal-court/` و
  `INTERNATIONAL.CRIMINAL.COURTS.md`: مصدر ووثائق متخصصة بالمجال.

### O–S

- **OFDMA** — `ofdma/`: مواد تقنية عن النفاذ المتعدد بتقسيم التردد
  المتعامد وموارد تطوير C/C++.
- **Philosophy ووحدة Conversation Mask** — يضم [`philosophy/`](../../philosophy/README.md)
  الأسس الأصلية بلغتي C/C++، وينظّم [`philosophy/module/`](../../philosophy/module/README.md)
  طبقة الوحدة التالية لها في ثلاثة مجالات صريحة:
  [`data/`](../../philosophy/module/data/README.md) للملاحظات المصنّفة والمؤرَّخة،
  ونهايات الأحداث، وإعدادات نصف العمر، والمصدر، والموافقة؛ و[`helper/`](../../philosophy/module/helper/README.md)
  للتحقق الحتمي، وتناقص الصلة، والمقارنات المتوافقة،
  والعرض المصفّى بحسب الموافقة؛ و[`psychiatry-friend/`](../../philosophy/module/psychiatry-friend/README.md)
  لواجهة محادثة داعمة غير سريرية. وتقع المقابض الموجهة إلى SLeeLa
  والشجرة المركّبة في [`lib/philosophy/`](../../lib/philosophy/README.md)،
  إلى جانب حزمة الفلسفة الأوسع. والوحدة قائمة على المدخلات الصريحة
  وتبدأ بالموافقة: فهي لا ترصد الأشخاص أو الوسائط بصورة سلبية، ولا تشخّص، ولا تستنتج
  النوايا الخاصة، ولا تقيّم قيمة الإنسان، ولا توصي بتغيير الأدوية. وتحفظ
  روابط أحداثها المصدر والتنقل، لا التثبّت المستقل.
- **Synchro** — تكامل بيئة التشغيل/المُصرّف الأصلي ووظائف توزيع الحزم
  والقياس؛ راجع مصدر `impl/` ذي الصلة وتنفيذ
  المُصرّف.
- **SleelaUI / واجهة الطرفية** — `lib/user-interface/` ومشروع
  `sleela-terminal`: تكامل واجهة مستخدم SLeeLa ومكونات
  التفاعل الموجهة إلى الطرفية.

### U–W

- **United Nations** — `united-nations/`: مواد مستودع متخصصة بالمجال.
- **Wi-Fi 4 وWi-Fi 5 وWi-Fi 6** — `wi-fi-4/` و`wi-fi-5/` و
  `wi-fi-6/` وأدلة المصدر ذات الصلة `lib/wifi/`: مواد شبكات لاسلكية
  وموارد إعداد خاصة بكل جيل.

### صيانة فهرس الحزم

عند إضافة حزمة أو توسيعها توسيعًا جوهريًا، أضف مدخلها هنا في
المجموعة الأبجدية المناسبة، واربطها بدليلها وREADME الخاصين بها،
ولا تصف إلا القدرات التي تدعمها المصادر المودعة. وأبقِ هذا
الفهرس تراكميًا: فلا تحذف أقسام README الحالية ولا تعد كتابتها
ضمن تحديثات فهرس الحزم.


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

الديمقراطية الأمريكية والسياسة الأمريكية.

---

## XVI. صور العلامة التجارية الوطنية للعلوم

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>



## XVII. كتالوج UTF-4088 التجريبي المليوني المدخلات

يبني [مولّد كتالوج UTF-4088](../../utf-4088/tools/generate_catalogue.py) بصورة حتمية كتالوجًا تجريبيًا للرموز يضم 1,048,576 مدخلًا. وتوصف الصيغة المدمجة الموثقة في [utf-4088/FORMAT.md](../../utf-4088/FORMAT.md)؛ أما عقد التوليد والتحقق ففي [utf-4088/GENERATION-RESULTS.md](../../utf-4088/GENERATION-RESULTS.md).

شغّل المولّد بواسطة Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

يبلغ حجم الملف الثنائي المولَّد 16,777,244 بايتًا، وقد صُمم ليبقى دون 20,000,000 بايت. وتتحقق اختبارات الوحدات وسير عمل GitHub Actions من عدد المدخلات وتفرّد الرسوم (glyphs) والحجم وSHA-256؛ وتنشر عمليات التشغيل الناجحة الملف الثنائي والملخص بوصفهما أثر سير العمل `utf4088-million-catalogue`. ويُترك الملف الثنائي المولَّد خارج سجل Git عمدًا. ويظل UTF-4088 تجريبيًا وغير متوافق مع Unicode.

---

# SLeeLa
<p align="center"><img src="../../images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---


## XVIII. إضافات حديثة — ثابت π، وبدء Modium/Registry، ورياضيات sleelactl، و`@native`

يسجّل هذا القسم الميزات المضافة خلال أعمال 2026-11، بدءًا بأحدث المجموعات. والملفات المصدرية المرتبطة وحالة التحقق منها هي المرجع الحاكم؛ ولا توصف التغييرات قيد المراجعة بأنها دُمجت إلى أن تصل إلى `master`.

### ثابت كتالوج π ذو المئة مليون رقم

يضم المستودع قيمة موثَّقة لـ π من **مئة مليون رقم** في [`pi-generator/pi.100000000.digits`](../../pi-generator/pi.100000000.digits) (عدد عشري سليم الصياغة، `3.14159…`)، إلى جانب قيمة من 57,344 رقمًا [`pi-generator/pi.57344.digits`](../../pi-generator/pi.57344.digits). وقد جرى التحقق من كلتيهما باستقلال على أنهما π أصيلة — راجع [`pi-generator/VERIFIED.md`](../../pi-generator/VERIFIED.md) الذي يوثق الفحوص (البادئة المعروفة في الكتب، ونقطة Feynman عند الموضع الكسري 762، والرقم المليوني الشهير، والتطابق مع `lib/math/SLPiReference.sleela`، وتحليل تواتر الأرقام في الملف كاملًا) ويقدّم سكربت إعادة إنتاج بخطوة واحدة.

### تحميل Modium™ / Registry™ لثابت π عند بدء النظام الفرعي

على نقاط الدخول المخصصة للنظامين الفرعيين — `sleela registry` و`sleela modium` (وعند أول تشغيل للمثبّت)، **وليس** عند كل `sleela run` — يحمّل النظامان الفرعيان Modium وRegistry ثابت كتالوج π الكبير. وفي طرفية تفاعلية يطبعان سطرًا أحمر مؤقتًا `Loading Modium™` / `Loading Registry™` يُمحى في مكانه قبل بدء المخرجات العادية؛ والأثر مقصور على TTY، فتبقى المخرجات المنقولة عبر الأنابيب ومخرجات الاختبار دون تغيير بايتًا ببايت. ويُزرع المحدِّد (locator) لا البايتات بوصفه القيمتين الافتراضيتين لمفتاح `Math` في registry وهما `piReferenceLargePath` و`piReferenceLargeDigits`، في كل من نموذج SLeeLa ([`lib/registry/SLRegistry.sleela`](../../lib/registry/SLRegistry.sleela)) والزارع الأصلي ([`registry/sleela_registry.c`](../../registry/sleela_registry.c))؛ ويكشف `lib/math/SLPiReference.sleela` الدالتين `largePath()` / `largeDigits()`. ويُثبَّت ثابت π في موقع SLeeLa الخاص بنظام التشغيل (`<data>/pi/`) بواسطة **كل من** المثبّت القياسي `make` والمثبّت الموجَّه «الجميل». التفاصيل: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](../../pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — المثبّت الجميل وموضوع `math` المدعوم أصليًا

- يثبّت المثبّت الموجَّه («الجميل») الآن `sleelactl` (مستوى التحكم في SLeeLa) وسجل وحداته، بما يطابق مثبّت `make`. راجع [`bin/SLEELACTL.md`](../../bin/SLEELACTL.md).
- أضاف `sleelactl` طبيعة جديدة هي **`subject`** لمكتبات الموضوعات الأصلية بلغة C/C++ المترجمة داخل بيئة التشغيل، وسجّل أولها: **`math`**. فيشغّل `sleelactl run math` الواجهة الخلفية الأصلية `__NativeMath` (`impl/subjects/math/math_native.cpp`) من البداية إلى النهاية عبر العرض المرجعي للموضوع؛ ويشرح `sleelactl info math` كيفية الوصول إليها (`import math;`). وتتيح طبيعة الموضوع الأمرين `run info` فقط — وليس فيها أمر `check` عن قصد، لأن `sleela check` لا يحمّل الموضوعات الأصلية المسجَّلة في SHEET.

### `@native` وNative Drive والتنفيذ متعدد اللغات

يطلب وسم الطرق الجديد **`@native`** الترجمة الأصلية المسبقة (ahead-of-time) لطريقة ساخنة. ويعثر الأمر الجديد `sleela aot` على الطرق الموسومة بـ `@native`، ويتحقق من أن كلًّا منها ضمن *المجموعة الفرعية القابلة للتفريغ* المدعومة، ويصدر C/C++ — مع **الإخفاق المغلق** (الرفض مع بيان السبب بدقة) عند أي بنية لا يستطيع ترجمتها بعد، فلا تُنتج طريقة مترجمة جزئيًا أبدًا.

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

يبقى `@native` طلبًا لمعالجة AOT، مع رفض البنى غير المدعومة قبل الإصدار. ويدعم [Native Drive](../../impl/native-drive/README.md) عمليات بناء ملفات تنفيذية ومكتبات مشتركة متعددة الوحدات بلغتي C/C++ عبر المُصرّفات المضيفة (بما فيها MSVC)، ومخرجات SLeeLa AOT المدعومة، وفحص الرموز مقابل ABI المحمِّل الأصلي، وتنفيذ العمليات دون صدفة (shell)، ومعالجة stdin/stdout/stderr، وتوصيل الأنابيب متعدد المراحل، وتفويض XClass إلى المحمِّل الإنتاجي. ويتيح `sleelactl` أوامر البناء/التشغيل فيه، ويوجّه فحص XClass وتوليد وتثبيت وحدات systemd الصريحة عبر `inspect native-drive`. وتملك VM جدول مقابض لوحدات أصلية محدود الحجم، مع تفويض مضيف إلزامي ومقابض يُتحقق من جيلها. وتضيف الصياغة 3.0 الآن `nativeOpen` و`nativeCall` و`nativeClose`: إذ يعبر إرسال شيفرة البايت (append-only) عبر ABI مُرقَّم الإصدار بصورة مستقلة لقيم null/bool/int64/double/UTF-8، بينما تبقى المصفوفات والبنى (structs) وسائر القيم المركّبة التي تملكها VM داخل المفسّر، ويجوز لها استخدام بديل شيفرة البايت بالاسم نفسه. ويمكن للإضافات (plugins) أن تتلقى اختياريًا جدول استدعاءات مضيف مُرقَّمًا بالإصدار لاستدعاء دوال SLeeLa بأسمائها الدقيقة عبر تنفيذ VM متداخل، مع تجذير المكدس المعلّق لدى GC، وقائمة سماح صريحة بأسماء استدعاءات المضيف، ورفض استدعاءات API الأصلي العودية. وتبقى الشيفرة الأصلية شيفرة موثوقة داخل العملية نفسها، وليست بيئة معزولة (sandbox). ولتعليقات سياسة العمليات في المصدر مخطط مُرقَّم الإصدار، لكنها لا تُحلَّل ولا تُفرض بعد. التفاصيل والحدود الأمنية وسجل مسارات العمل السبعة: [Native Drive](../../impl/native-drive/README.md) و[`pi-generator/AT-NATIVE.md`](../../pi-generator/AT-NATIVE.md).


## XIX. إصدارات README الدولية

توفر SLeeLa نقاط دخول README خاصة بكل بلد للأسواق الخمسين في إصدارها الدولي. ولكل بلد ملف واحد باسم `README.<CC>.md`؛ وحيثما ترتبط عدة لغات بالبلد فإنها تُجمع داخل ملفه.

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

**سياسة الترجمة:** هذه الملفات نقاط دخول قُطرية مُوطَّنة في مرحلتها الأولى. وتُترجم مقدماتها وعناوين اللغات، بينما يظل المرجع التقني الكامل في النسخة الإنجليزية المعيارية [README.md](../../README.md). ولا يجوز وصف ملف بلد بأنه ترجمة كاملة إلا بعد ترجمة كل قسم، والتحقق من المعرّفات التقنية والروابط، وخضوع الإصدار لمراجعة لغوية. وتغطية اللغات في هذه المرحلة الأولى تمثيلية، ولا تعني أن كل لغة معترف بها دستوريًا أو إقليمية أو أصلية أو لغة إشارة قد تُرجمت بالكامل.

## ملحق — إرشادات إقليمية محتفظ بها

يحتفظ هذا الملحق بإرشادات عملية وردت في الدليل العربي المختصر السابق ولا تكرر أقسام README المعياري. وهي تكميلية، ولا تغيّر ما ورد في الأقسام I–XIX أعلاه؛ وعند أي اختلاف يُعتمد README المعياري.

### عقود الوحدات وواجهات API

يجب أن توثق كل واجهة API عامة شكل المدخلات المقبولة، والمخرجات المتوقعة، وشروط الخطأ، وملكية الذاكرة، والآثار الجانبية. وينبغي أن تحدد وثائق كل وحدة المتطلبات السابقة والنتائج اللاحقة والقيود المرتبطة بنظام التشغيل أو هدف التنفيذ.

لا تمنح أسماء الرموز أو مراجع الوحدات إذنًا تلقائيًا لتنفيذ الشيفرة. يجب التحقق من التفويض عند حد التنفيذ المناسب، مع الفصل بين اكتشاف المورد والسماح باستخدامه. وعند وجود مدخلات غير صالحة أو موارد مفقودة أو ميزة غير مدعومة، ينبغي أن تفشل العملية بطريقة آمنة وأن تقدم خطأً يمكن تشخيصه.

### استراتيجية الاختبار والانحدار

ينبغي تقسيم الاختبارات وفق مراحل المُصرّف: التحليل المعجمي، والتحليل النحوي، والتحليل الدلالي، وخفض التمثيل الوسيط (IR)، وتوليد الشيفرة، والتنفيذ على الآلة الافتراضية، والتكامل مع الشيفرة الأصلية. ويجب اختبار كل مرحلة منفردة، وكذلك نقاط الانتقال بين المراحل.

يحتوي كل اختبار انحدار على أقل مدخل يعيد إنتاج المشكلة، والنتيجة المتوقعة، والنتيجة الفعلية، ورمز الخروج عند الاقتضاء. وتشمل الحالات السلبية أخطاء الصياغة والأنواع، والمدخلات الناقصة، وفشل تخصيص الموارد، وعدم توافق ABI. ويجب ألا تُعرض الاختبارات المتجاوزة أو التي لم تُنفذ على أنها ناجحة.

### التوافق والترحيل وإعادة الإنتاج

عند تغيير واجهة عامة أو صيغة ملف أو سلوك مُصرّف، وثّق أثر التغيير على التوافق مع الإصدارات السابقة. أدرج خطوات الترحيل وأمثلة قبل التغيير وبعده، وحدد ما إذا كان التغيير يؤثر في ملفات المصدر أو المنتجات المترجمة أو إعدادات التشغيل.

لإعادة إنتاج نتيجة بناء أو اختبار، سجّل التزام Git، ونظام التشغيل، والمعمارية، وإصدار المُصرّف، وإصدارات الأدوات، وأوامر التنفيذ. لا تعمم نتيجة منصة واحدة على منصات أخرى. راجع [مصفوفة دعم المنصات](../../docs/PLATFORM-SUPPORT.md) و[سياسة الاختبار](../../markdown/TESTING.md) قبل وصف إصدار بأنه متحقق منه.

### توحيد المصطلحات وصيانة الترجمة

ينبغي توحيد المصطلحات العربية للمُصرّف وبيئة التشغيل والآلة الافتراضية والوحدة وواجهة API عبر الصفحات، مع إبقاء المصطلح الإنجليزي عند أول استعمال إذا كان ذلك يزيل الالتباس. وعند تحديث README الإنجليزي المعياري، قارن الأقسام المقابلة وحدّث هذه النسخة بالتوازي، وافحص الروابط وعناوين Markdown وتوازن كتل الشيفرة.

**حالة الترجمة:** تُرجمت الأقسام I–XIX إلى العربية الفصحى المعاصرة. وقد أُبقيت المعرّفات التقنية والأوامر وأمثلة الشيفرة وأسماء الملفات والروابط والعناوين كما هي. يوصى بإجراء مراجعة لغوية نهائية على يد متحدثين أصليين بالعربية ومراجعة تقنية متخصصة.
