<img align="right" src="images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — ישראל (IL)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>שפת מערכות מקורית, מהדר, סביבת ריצה ומערכת אקולוגית של מכונות וירטואליות.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **מהדורה בין-לאומית לפי מדינה.** מהדורה זו מוגשת בשתי שפות: **עברית** (גוף המסמך המלא, בעברית טכנית מודרנית) ו**ערבית** (מבוא קצר בראש המסמך וצירוף טכני בסופו) עבור קוראים דוברי ערבית בישראל. מזהים טכניים, פקודות, דוגמאות קוד, שמות קבצים, נתיבים, קישורים וכתובות נותרים ללא שינוי ביחס ל-[README.md](README.md) הקנוני באנגלית; מתורגם רק הטקסט המסביר שסביבם.

## العربية

**لغة أنظمة أصلية ومنظومة متكاملة تضم مُصرّفًا وبيئة تشغيل وآلات افتراضية.** SLeeLa لغة برمجة أنظمة أصلية تضم مُصرّفًا وبيئة تشغيل ومنظومة آلات افتراضية. هذه النسخة موجَّهة أيضًا إلى القرّاء الناطقين بالعربية في إسرائيل. اللغة أكثر من مجرد صياغة نحوية: فهي المُصرّف وبيئة التشغيل والمكتبات والأدوات وعقود المنصات التي تجعل البرمجيات قابلة للاستخدام. تبقى المعرّفات التقنية والأوامر وأمثلة الشيفرة والروابط دون تغيير؛ وفي نهاية هذا الملف «ملحق عربي» (ضمن الملحق العبري) يعرض نظرة تقنية موجزة. العودة إلى النص العبري الكامل أدناه.

## עברית

**The Now. The Ingredient.** שפה היא יותר מתחביר: היא המהדר, סביבת הריצה, הספריות, הכלים וחוזי הפלטפורמה שהופכים תוכנה לשמישה.

**במה SLeeLa עוסקת.** SLeeLa עוסקת ב-**Math**, ב-**Object**, ב-**Science** וב-**Reach** — ארבעת פניה של עבודתה: המחושב (Math), הדבר המסוכם העומד בקטלוג של מה שקיים (Object), המחקר המשמעתי של האמת (Science), והושטת ידה של מערכת אל מערכות אחרות ואל בני אדם (Reach). **האינטרסים הלאומיים נשארים בעינם** — אף אחד מהארבעה אינו מבטל אותם; כאשר נושא נוגע באינטרס של מדינה, מעמד זה נשמר (ונרשם בנאמנות, למשל ב-Metadata של כל מחלקה ב-`metaOrigin` / `metaNationalSchedule`), ואינו מתמוסס.

## I. נקודת התחלה
- [בניית המימוש המקורי](impl/README.md)
- [מכונות וירטואליות ו-Modium — מדריך משתמש מעשי](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [מחסנית ה-CPU בקוד המקור של SLeeLa](lib/cpu/CPU.md) — מכונה שלמה משערים לוגיים דרך ALU, ליבות ומערכת הפעלה ועד תוכניות, רשימה של יותר מ-130 פרופילי CPU, ו[מתחי חשמל, שעונים משתנים ותזמונים מתקדמים](lib/cpu/TIMING.md) הניתנים לכוונון, שאפשר [להריץ לפי המפרט או בעל-שעון (overclock)](lib/cpu/components/README.md)
- [אינדקס התיעוד](docs/README.md)
- [מקור הנתונים CSPML והפניית המחקר Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — מקורם של נתונים חיצוניים בארצות הברית על מקומות, מצב חברתי-כלכלי, כוח עבודה, חינוך ובתי ספר ציבוריים, והנחיות לשילובם.
- [תיעוד ה-API שנוצר](docs/api/README.md) — ההפניה העדכנית בפורמט HTML, מהדורת API מנוהלת-גרסאות והוראות ליצירה מחדש
- [קטלוג ה-API](api/README.md)
- [תקני השוואה חישוביים](comparison/README.md) — תקני טיפוסים, זיכרון, ABI ותאימות עבור SLeeLa VM, Java/JVM ו-C/C++ מקורי.
- [Recursion stanchion — אבות-טיפוס ב-C וב-C++](recursion/stanchion/README.md) — דוגמאות המותאמות לטיפוסים הפרימיטיביים Boolean, Byte, Character, Integer, Long, Float ו-Double, לצד תבניות רקורסיה כלליות, יישומיות, מדעיות ולימודיות.
- [תמיכה בפלטפורמות ומטריצת אימות](docs/PLATFORM-SUPPORT.md)
- [מדריך המותג](docs/BRAND-GUIDE.md)
- [מושגי סחורה ונכסים](merchandise/README.md)
- [סמל NitroWebExpress](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [מדיניות בדיקות ושערי שחרור](markdown/TESTING.md)

## II. חדשות

תוספות אחרונות לשפה, לדרייברים ולספרייה הסטנדרטית:

- **חבילת הספרייה הסטנדרטית `advantage` — יתרון על דיבידנד, עם מתמטיקה, פיזיקה, אינטגרציה, דירוג ופלט ניתן לניתוב.**
  [`lib/advantage/`](lib/advantage/README.md) מכמסת את *מתן יתרון לדיבידנד ייחודי ביחס למודל* כ**בצל Sequitur** שכבתי
  (דיבידנד ← סקירה ← חששות ← מתמטיקה ← תוספות ← חיסור ← קיבוץ מחדש
  ← איטרציה), שאפשר לנווט בו פנימה והחוצה עם עדכון דינמי מלא
  ו-restitute, והיא נחשפת דרך **מודל משתמש** פשוט ו**מודל מפתח** בעל שליטה מלאה. היא כוללת חמישה תחומי נושא: **`math/`** כלכלת סחר דו-צדדית
  (מקסום רווח משותף, מיקוח נאש, היחס הקריטי של ה-newsvendor הדו-צדדי), **`physics/`** "פיזיקה של שדות אמון" בכלכלה מבנית
  (אמון כטנזור מטרי, הגאודזית העסקית, מבנה מגע של הכרזה, SDE של דעיכת אמון מסוג CIR+jump — אנלוגיית מידול, המסומנת ככזו), **`integration/`** מודל אינטגרציה אנכית הניתן להרכבה רקורסיבית לקפיטליזם/קמעונאות
  (תאים, מפרצי הנדסה, תאי ניסוי, מגשי חציון של ניוטון וציון תחכום מערכתי), **`grading/`** מדרג מעגלי תחכום המדרג כל מודול
  וכל קבוצה של מודולים מנצחים/מפסידים/מדרגים/מוחלטים בסולם A–D, ו-**`output/`**
  נקודת פליטה הניתנת לניתוב ששולחת את פלט המודול אל הטרמינל, אל משטח בפורמט **CommonRails**, או אל socket/מחבר (ממוסגר עבור `slnet_write` מקורי). כל מחלקה נושאת את רשומת Metadata-Class; חמש הדגמות ניתנות להרצה מאמתות את הנושאים, והמסמכים הנלווים
  [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md),
  [`MATH.md`](lib/advantage/MATH.md) ו-
  [`PHYSICS.md`](lib/advantage/PHYSICS.md) מתעדים אותם. צרכן C/C++ מקורי תחת [`advantage/`](advantage/README.md) (ליבת ה-C
  `advantage_value`/`advantage_mean`, מאגר ידע XML, וספריית רכישה מאובטחת
  של HTTPS/SFTP/SSH + keystore + תעבורת DH/ECDH) מקשר את המודל אל
  [דוגמאות סחר](advantage/examples/README.md) מעשיות — סחרים חיוביים ממקורות אינטרנט משוערים, רצף רלוונטיות לקהל (סוחרים, צרכנים, עוזרים, מרפאים), ושמונה מודלים מדורגים A–D (D = הכי הרבה Cement,
  A = הכי הרבה Brand).
- **הערות תזמון לולאות — `@loopDelay` / `@loopBudget` (+ פונקציה מובנית `sleep(ms)`).** ההערות הראשונות ברמת **משפט** וב**התנהגות** ב-SLeeLa:
  נכתבות בשורה נפרדת לפני לולאת `for` / `while` / `do`, והן מוסיפות
  השהיה ממשית לביצוע הלולאה. `@loopDelay(ms = 250)` /
  `@loopDelay(seconds = 2)` ממתינה פרק זמן קבוע בכל איטרציה (`@loopDelay(250)` ללא שם ברירת המחדל שלו היא מילי-שניות); `@loopBudget(seconds = 10,
  iterations = 1000)` מפזרת באופן שווה מספר איטרציות ידוע על פני תקציב זמן כולל של שעון הקיר. שתיהן מונמכות לקוד הפעולה החדש מסוג append-only **`OP_SLEEP`**
  (ISA 157), שגם מגבה פונקציה מובנית כללית **`sleep(<ms>)`** שאפשר לקרוא לה
  בכל מקום; ההשהיה ממומשת באופן נייד (`nanosleep` / `Sleep`). מפורט
  ב-[`SLEELA.syntax` §32](SLEELA.syntax); הפניית ה-API ב-
  [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md);
  דוגמה להרצה:
  [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela).
  אינדקס תוספות-הסשן (כולל זו, את הפעולה המקורית `OP_BIGMUL`
  לכפל מספרים שלמים גדולים, ואת הכלים העצמאיים **[local-generator](local-generator/README.md)**
  ו-**[pi-generator](pi-generator/README.md)**) נמצא ב-
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md).
  שלושת משטחי ה-API שנוצרו
  ([`sleeladoc-api/`](sleeladoc-api/README.md),
  [`docs/api/`](docs/api/README.md), וצילום המצב הבלתי-משתנה
  [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html))
  נבנו מחדש באמצעות `make api-docs` וכוללים כעת את ערכי ההוראות `SLISASleep`
  / `SLISABigMul` בדף החבילה `vm` שלהם.
- **SleelaDoc משורת הפקודה — `sleela docs`.** כעת אפשר לגשת ל-SleelaDoc
  ישירות מהטרמינל. `sleela docs` מפרטת כל נושא שניתן לתעד;
  `sleela docs crypto` מציגה חבילה; `sleela docs stringbuilder` פותרת
  מחלקה (ללא תלות באותיות רישיות, ולפי קידומת הספרייה הסטנדרטית `SL` ←
  `SLStringBuilder`) ומדפיסה דף עשיר — **הגדרה, תכלית עיצובית,
  יחסים, מבנה, user clays וסולם שליטה בן חמש דרגות** — עם תוצאות "האם התכוונת ל…" מדורגות במקרה של החטאה קלה.
  מחברים מעשירים כל דף פשוט על ידי הוספת תגיות `@` של SleelaDoc (`@intent`, `@thesis`, `@axiom`, `@param`,
  `@see`, `@mastery`). `sleela docs --generate [dir]` כותבת קורפוס טקסט פשוט
  (דף אחד לכל מחלקה + `TOPICS.index`) שה-CLI קורא לאחר מכן במצב לא מקוון;
  **`make sleeladoc`** אורז את הקורפוס **ואת מקורות `.sleela` התואמים**
  לתוך `sleeladoc/build/sleeladoc.jar`. עבור נושאים שהם חזית בלבד (למשל
  `psychiatry`, `politico`), הפקודה `sleela docs --facade <topic>` סורקת
  את מערכת הקבצים בזמן ריצה ומפרטת את **התיקיות (ואחר כך הקבצים, כתנועה טבעית)** האמיתיות,
  בכפוף למגבלות הניתנות לעקיפה על ידי המשתמש: **6 MB / 6000 קבצים / 6
  דקות**. יחידות `lib/sleeladoc` מתוחזקות במהדורה הנוכחית,
  **`#sleela 3.0`**. ראו [`man/man1/sleela.1`](man/man1/sleela.1) ו-
  [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md).
- **SleelaDoc — פתרון תיעוד בסגנון Javadoc, עם אתר HTML שנוצר.** **SleelaDoc** היא מוסכמת הערות התיעוד בתוך המקור של SLeeLa
  (הערות בסוגריים משולשים עם סימון `@doc` / `@page`; ראו
  [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)), ו-
  [`lib/sleeladoc`](lib/sleeladoc/README.md) הוא ה**מחלץ** שמעבד יחידת
  `.sleela` לדף API בפורמט Markdown או HTML — כפי ש-`javadoc` מעבד Java,
  ומתעד *כוונה*. **אתר תיעוד HTML מלא וניתן לעיון עבור
  כל `/lib`** נוצר אל [`/sleeladoc-api`](sleeladoc-api/README.md)
  על ידי `tools/generate-sleeladoc-api.py` (פתחו את `sleeladoc-api/index.html`; סגנון נקי
  של כחול / תכלת / אפור בהיר / לבן עם היררכיית חבילה ← מחלקה ← חבר ומילון מונחים שנכתב עבור הקורא *Student 2*). המהדר גם
  פולט פלטי תיעוד שסומנו על ידי המחבר כתוצר לוואי של הקומפילציה (דף MediaWiki ב-`--wiki`
  + תאום מקור שמקורו בקוד), המתועדים ב-
  [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md). להרצה:
  [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela).
- **מודל אובייקטי המסמך של XML (`lib/xml`).** DOM של XML בצד SLeeLa — מסמך
  המחזיק עץ של צמתי element/text/comment/CDATA/PI עם ילדים מסודרים
  ותכונות בעלות שם, מסדרן (קומפקטי/מעוצב, בריחה לפי XML 1.0 §2.4), מנתח מבוסס
  תאי תווים, ורישום האיכות `SLXmlAspect`. שוויון בין צמתים הוא
  שרשרת אמון תלת-דרגתית (זהות ← אותו נושא ← מבנה), רפלקסיבית,
  סימטרית ודטרמיניסטית. ראו [`lib/xml/README.md`](lib/xml/README.md) ואת
  הדוגמה להרצה [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela).
- **פרימיטיבים רחבים של מספרים שלמים חתומים (`long`, `int64`, `int128`).** `int`, `long`
  ו-`int64` כולם מספרים שלמים חתומים בני 64 סיביות שעוברים בדיקת טיפוסים ורצים באופן זהה;
  `int128` מתקבל כטיפוס שלם (נתמך כרגע על 64 סיביות, כאשר 128 סיביות אמיתיות הן המשך מתועד).
  הסדרה החתומה בבסיס 2 `int32`..`int16777216`
  (תחביר 2.7) והסדרה הלא-חתומה התואמת `uint32`..`uint16777216`
  (תחביר 2.8) מרחיבות זאת. ראו [`SLEELA.syntax`](SLEELA.syntax) §21, §24, §25.
- **שדות תוצאתיים `@mix` (תחביר 2.9).** מחלקה רשאית להצהיר על מספר
  שדות מופע עם **אותו שם** כאשר כל חזרה נושאת `@mix`; השפה מבינה את הקבוצה כתוצאתית (מערך / רשימה לפי אינדקס),
  מלמעלה למטה `1,2,3,…` כברירת מחדל או ידנית באמצעות `@mix(1,2,3,...)` שמספר האינדקסים בו חייב להתאים למספר השדות. ראו [`SLEELA.syntax`](SLEELA.syntax) §28.
- **סמני מסמך — התהליך הטבעי, הנקרא Preonems
  (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).**
  מילות סימון מסמך חשופות אלה נקראות יחד **Preonems**
  (סמן אחד הוא *Preonem*; הקבוצה — *Preonems*; ראו
  [`SLEELA.syntax`](SLEELA.syntax) §26.1). סמני מסמך מדרגה ראשונה, הצהרתיים בלבד, המציינים מה המסמך *הוא* בסדר היצירה:
  `author`/`authorial` (תחביר 2.4; מטופל על ידי מועצה מקצועית — החבר היחיד בעל השפעה התנהגותית),
  `material;` (מקור מסוכם) ו-`memory;`
  (תחביר 2.5), `determine;` (תחביר 2.6; מסמך הפתוח לעיבוד נוסף בידי רופא, רופא מומחה או רוח לקראת השלמה בידי יד חוקית),
  ושני החדשים ביותר, **`object;`** (דבר מסוכם העומד בקטלוג
  של מה שקיים — **הופשט והוענק לו מעמד קונקרטי, בתחביר 2.8**) ו-
  **`field;`** (קרקע היחס שבה עומדים ונעים אובייקטים) —
  שניהם חסומים מתחביר 2.6. כל אחד מהם חשוף, תלוי הקשר וחסום לפי גרסה על ידי החזית,
  ללא השפעה על ניתוח, טיפוסים או יצירת קוד; מחוץ למיקום הסמן
  כל אחד נשאר מזהה רגיל (כך ש-`field` לעולם אינו פוגע בחבר מחלקה
  או בגישת `.field`). ראו [`SLEELA.syntax`](SLEELA.syntax)
  §18–§20, §23 ו-§26.
- **Nordshrift בתחביר 3.0 עם הישג מלא לספריות.** Nordshrift נפתר
  דרך אותו אינדקס רקורסיבי של `/lib` כמו המהדר, ומגיע לכל
  חבילה ולכל יחידת `.sleela`; `nordshrift check` מדווחת את הספירות של העץ כולו.
- **Recursion stanchion.** התיקייה [`recursion/stanchion/`](recursion/stanchion/README.md) כוללת כעת אבות-טיפוס ייחוס ייעודיים ב-C וב-C++ עבור שבעת החוזים הפרימיטיביים המתועדים, וכן דוגמאות כלליות, יישומיות, מדעיות ולימודיות. אלה מקורות ייחוס; יש לקמפל ולבדוק אותם לפני שמתייחסים אליהם כמאומתים.

- **הערות בסוגריים משולשים (`<` / `<<`).** סגנון הערות חדש להמשך הדרך —
  `< שורה >` ו-`<< בלוק >>` — המנותח על ידי חזית SLeeLa ומתועד ב-
  [`SLEELA.syntax`](SLEELA.syntax) §1.1.
- **ליטרלים שלמים בבסיסים שונים.** ליטרלי `int` מקבלים כעת צורות הקסדצימליות (`0x`),
  בינאריות (`0b`) ואוקטליות (`0o`) עם מפרידי ספרות `_`, הן בשפת המקור של SLeeLa ([`SLEELA.syntax`](SLEELA.syntax) §1.4) והן
  ב[פורמט מודל ה-CPU `.xlt`](lib/xlt/README.md).
- **תקן Metadata Class ברחבי הספרייה.** תבנית-העל של מטא-נתונים תיאוריים
  (מקור, חוקיות, היקף שיפוטי) נישאת כעת
  בכל קובץ `.sleela` נושא מחלקה תחת `/lib`; ראו
  [`SLEELA.syntax`](SLEELA.syntax) §15 ו-
  [שיעור המדריך](tutorial/13-metadata-class.md).
- **משפחת text / String עשירה יותר.** `lib/text` מוסיפה פונקציות עזר אמיתיות בתוך השפה,
  מבט תאי-תווים המספק `charAt`/`substring`/`indexOf`/`reverse`/
  יישור עמודות אמיתיים, ותבניתן מיקומי אמיתי — ראו את
  [מדריך text ו-String](tutorial/14-text-and-strings.md) ואת הדוגמה להרצה
  [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela).
- **`String` מדרגה ראשונה בגיליונות Nordshrift.** גיליון `.sst` יכול להצהיר על בלוק `strings:` מוטיפס שתורם למניפסט הרכיבים של הבנייה;
  ראו את [הפניית גיליונות Nordshrift](wiki/Nordshrift-Sheet-Reference.md).
- **מודלי CPU רצים הן לפי קודי פעולה והן לפי שקילות קריאות לפונקציות.** כל
  `SL<ARCH>CPU` יורש את הבסיס של הרצת זרם הוראות מקורי
  דרך לולאת ה-fetch-decode-execute שלו *וגם* הרצת הקוד של תהליך אחר משוער באמצעות קריאות שקולות על מצע ה-VM המשותף — ראו
  [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5.

## III. בנייה ובדיקה

**דרישות מוקדמות:** שרשרת כלים של C/C++ (GCC או Clang) ו-GNU Make; Python 3 עבור
כלי המניפסט והזרימה; ב-Linux חבילות הפיתוח של X11/Xrandr עבור
מוצרי התצוגה (הפקודה `make deps` של מודול מדווחת/מתקינה את מה שהוא צריך). בניית `impl/` היא fail-closed ביחס למניפסט SHA-256 מהימן, שה-`Makefile` שבשורש מצביע עליו אוטומטית.

מעותק נקי, ההתחלה המהירה ביותר:

```sh
make                                  # build everything from the repo root
```

או לבנות ולבדוק רק את הליבה המקורית:

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

הפקודה `make` בשורש המאגר בונה כל מוצר (ליבה, VM, קודקים, churn,
antivirus ועוד) ומסיימת בבניית ה-**registry** ובהחייאת טבלת הבסיס שלו — כך שעץ בנוי מגיע עם registry הניתן לסקירה מן המוכן.
היעדים החיוניים:

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

יעדים שימושיים נוספים:

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

היעד `registry` מריץ את [`build-registry.sh`](build-registry.sh): הוא בונה
את ליבת ה-registry, זורע את טבלת הבסיס אל מאגר הטקסט הנייד
`registry/build/sleela.sleelareg`, ומייצא את סקריפט הטבלה של MySQL
`registry/build/registry.sql` (באמצעות `sleela registry export`). סקרו אותו בעזרת
`sleela registry` או `registry/build/sleela-registry review`. ראו את
[מדריך ה-registry](registry/README.md) ואת [ארכיטקטורת Registry 3.0](registry/REGISTRY-3.0-ARCHITECTURE.md).

עיינו במדריכי הבנייה והבדיקות לדרישות המוקדמות ולשלבים הייחודיים לכל פלטפורמה. בנייה מוצלחת של רכיב אחד אינה מוכיחה שכל ספרייה או פלטפורמה מוכנה לשחרור.

## IV. התקנה

לאחר בניית המוצרים המקוריים, התקינו את הפקודות וקובצי התמיכה של סביבת הריצה:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

מתקין ברירת המחדל משתמש ב-`/usr/local/bin`, ב-`/usr/local/lib/sleela`
וב-`/usr/local/share/sleela`. הוא שומר על תצורת אתחול קיימת ומסרב לדרוס פקודות קיימות שאינן במעקב. הסרת ההתקנה
מסירה רק קבצים שנרשמו על ידי המתקין, ושומרת על התצורה
ועל נתוני ריצה שאינם במעקב.

להתקנה ממוקדת ומודרכת של החבילה הראשית בלבד — הבינאריים `sleela`,
`sleelvac` ו-`nordshrift`, הספרייה הסטנדרטית, הפרוטוקולים ומודולי
התמיכה — השתמשו במתקין הנקי:

```sh
make clean-install       # guided install of the main package only
```

הוא מבקש החלטה ראשית אחת (התקנת החבילה הראשית), אחר כך בחירת היקף,
ולאחר הצלחה בורר אופציונלי של מודולי שיפור נבחרים. לתהליך ההתקנה האינטראקטיבי ולכל אפשרויות ההתקנה ראו
[התקנה מהירה ובטוחה](install/README.md).

## V. השפה וסביבת הריצה
הדקדוק המחייב הוא [`SLEELA.syntax`](SLEELA.syntax). המאגר הנוכחי מתעד תחביר **3.0**; גרסת המהדר/הכלים וגרסת תחביר המקור הן מושגים נפרדים. משטח 3.0 מוסיף את **סמני המסמך `pure` ו-`gasoline`** (§30) — זוג Preonems חשופים, תלויי הקשר והצהרתיים בלבד, שמשמעותם כמעט זהה, וכל אחד מהם מסמן את מעמד המסמך לפי הכנות שלו. תחביר 3.0 מאחד את משטח השפה 1.0–2.9 למהדורה הנורמטיבית הנוכחית, תוך שמירה על כל שערי המינימום הייחודיים לכל תכונה. משטח 2.9 הקודם מוסיף **שדות תוצאתיים `@mix`** (§28) — מחלקה רשאית להצהיר על מספר שדות מופע בעלי אותו שם כאשר כל אחד נושא `@mix`, המובנים כקבוצה אחת מאונדקסת (מלמעלה למטה `1,2,3,…` או ידנית `@mix(1,2,3,...)` שמספר האינדקסים בו חייב להתאים לקבוצה). הוא נושא הלאה את משפחות המספרים השלמים הרחבים החתומים/הלא-חתומים בבסיס 2: `int32`..`int16777216` (§24) ו-`uint32`..`uint16777216` (§25) מעל `long`/`int64`/`int128` הקודמים (§21); **מסמכי תומכי מילות מפתח** (§22); וכן את ה-**Preonems** — סמני המסמך ההצהריים בלבד `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30), ו-`artifact` (§35, מעמד ברמת חקיקה המדורג ביחס לזמן ולמרחב), הנקראים יחד Preonems (§26.1). פתרון עומס יתר של בנאים והאצלת `this(...)`/בנאי בסיס אינם חלק מחוזה תחביר 1.8 המאומת, במפורש. עיינו בדקדוק ובהערות המימוש לפני הסתמכות על תכונה. תשע יחידות המקור של SleelaDoc מצהירות כעת על `#sleela 3.0`; שער המהדר המשותף מקבל תחביר מקור 1.0–3.0. בניית ה-API החדשה מופקת באמצעות `make api-docs`.

המאגר כולל מימוש C/C++ מקורי, הפניות ליצירת SLVM, מודולים של הספרייה הסטנדרטית, בדיקות, דוגמאות ושילובי פלטפורמה. מצב המימוש והאימות משתנה ממודול למודול; עיינו במפרט שלו ובתוצאות ה-CI העדכניות ולא בהנחה של שלמות אחידה.

## VI. עקרונות הנדסיים
- **ראיות לפני הצהרות:** יש להבחין בין התנהגות ממומשת, נבדקת, ניסיונית ומתוכננת.
- **חוזה אחד לכל גבול:** יש לתעד תחביר, ממשקי API, פורמטי קבצים, הנחות פלטפורמה והתנהגות בכשל.
- **נייד כבר בתכנון:** יש לבודד מנגנונים ייעודיים לפלטפורמה ולבדוק על מריצים מקוריים.
- **כשל בטוח:** יש לבדוק קלט פגום, מגבלות משאבים, כשלי תהליכים ונתיבי ניקוי.
- **שחרורים הניתנים לשחזור:** יש לבנות מחדש בצורה נקייה ולפרסם גיבובי תוצרים והערות שחרור.

## VII. בניית אנטי-וירוס ושלמות

חבילת האנטי-וירוס Sleelavirin כוללת סורק SHA-256 מקורי, מתאם
ClamAV אופציונלי, מניפסטים של שלמות מקור/אינדקס וסכומי ביקורת של חבילות פלטפורמה.
בנו ובדקו את החבילה משורש המאגר:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

מניפסט השלמות נוצר תחת `antivirus/build/`; הוא מכסה קלטי מקור
ותיעוד במעקב וכן קובצי אינדקס חתימות מוגנים,
ואינו כולל פלט שנוצר ואת מטמון ClamAV המשתנה. לצילומי מצב של מסד הנתונים של ClamAV
יש מניפסטים משלהם. קובצי ZIP של שחרור כוללים `INTEGRITY-MANIFEST.json`,
`SHA256SUMS` וקובץ נלווה `.sha256`. אמתו את גיבובי הארכיונים שהורדתם מול
ערך מהימן שהושג באופן בלתי תלוי: סכום ביקורת שמאוחסן לצד ארכיון
אינו חתימת מפרסם. ראו את [מדריך Sleelavirin](antivirus/README.md),
[רשימת הבדיקה לאבטחה](antivirus/SECURITY.md) ואת
[תהליך העבודה לבניית אנטי-וירוס](.github/workflows/antivirus-binaries.yml).

שחרור אינו נחשב מאומת עד שמשימות ה-CI הרלוונטיות עוברות על
ה-commit המדויק של השחרור והתוצרים שנוצרו נבדקו.


## VIII. ערכות נושא לעורכים של SLeeLa — Vim ו-Emacs

SLeeLa כוללת קבוצה מתואמת של ערכות צבעים כהות מקצועיות עבור **Vim** ו-**Emacs**, וכן כלי פלטות מקורי ב-C/C++. ערכת **American Professional** המקורית נשארת ברירת המחדל. עשר פלטות נלוות זמינות בהשראות עיצוב תרבותיות ומוסדיות שונות; אלה פלטות עיצוב פרשניות ואינן ייצוג רשמי של אף מדינה, תרבות, בית משפט או מוסד.

### אחת-עשרה הערכות וצבעיהן

| ערכה | רקע | חזית | מילות מפתח | טיפוסים | מחרוזות | הערות |
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

#### צבעי דגלי המדינות (פלטת מקור)

הערכות היווניות והטורקיות נגזרות מ**צבעי הדגל הלאומי הרשמיים** של שתי המדינות, המוצגים כאן עם ערכי ה-hex המדויקים שפורסמו. רקעי הערכות והדגשיהן שלמעלה הם פרשנויות מוחשכות ונוחות לעורך של צבעי מקור אלה — הטבלה שלהלן היא הפניית "הצבעים כפי שהם נחשפים לאומית".

| מדינה | צבע | Hex | בסיס ומשמעות |
|---|---|---|---|
| **יוון** | כחול | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | כחול הדגל הלאומי (חוק 851/1978). השמיים והימים; מכונה בהרלדיקה *azure*. |
| **יוון** | לבן | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | לבן הדגל הלאומי. טוהר מאבק העצמאות; הצלב מציין את הנצרות האורתודוקסית המזרחית. |
| **טורקיה (Türkiye)** | אדום (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | אדום הדגל הלאומי (חוק הדגל הטורקי). השדה; לפי המסורת דם השהידים וריבונות העות'מאנים. |
| **טורקיה (Türkiye)** | לבן | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | לבן הדגל הלאומי. הסהר והכוכב — שלום, טוהר וסמל המורשת הטורקית/האסלאמית. |

הערכות מעגנות את צבע החזית של כל עורך ב**לבן** הלאומי, ואת מילות המפתח, הסמן, הבחירה ושורת המצב ב**כחול** הלאומי המדויק (`#0D5EAF`) או ב**אדום** הלאומי המדויק (`#E30A17`), כך שהצבעים החשובים הם ממש הצבעים הלאומיים. אלה פלטות עיצוב פרשניות; הפניית צבעי הדגלים ניתנת לצורך מקוריות המידע ואינה ייצוג רשמי של אף אחת משתי המדינות. משמעויות הצבעים נשענות על תיאור סמלי המדינה של משרד החוץ של הרפובליקה ההלנית ועל מסורת חוק הדגל הטורקי.


### טבלת דוגמיות צבע

תאי הפלטה שלהלן משתמשים ב**דוגמיות SVG**, כך ש-GitHub מציג את הצבעים עצמם ולא תאי צבע ריקים של HTML. כל רצועה מציגה את ששת הצבעים בסדר זה: רקע, חזית, מילות מפתח, טיפוסים, מחרוזות והערות. קודי ה-hex המדויקים מופיעים לצד כל דוגמית, באותו סדר ערכות כמו בטבלת ההפניה שלמעלה.

| ערכה | צבעי הפלטה כפי שהם מוצגים | קודי Hex (רקע · חזית · מילות מפתח · טיפוסים · מחרוזות · הערות) |
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


צבעי הדגשה נוספים משמשים לפונקציות, קבועים, בחירות, אבחונים והנחיות מקדם-מעבד (preprocessor). הפלטה הרוסית-הסלאבית משתמשת במכוון בצפחה, כחול, מרווה, אודם וזהב עתיק בעוצמה בינונית, לקבלת מראה מקצועי רגוע יותר.

### התקנת ערכות Vim

משורש המאגר:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

הוסיפו את השורות הבאות ל-`~/.vimrc` כדי להפעיל הדגשת תחביר של SLeeLa ולבחור ערכה:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

בחרו שם `colorscheme` אחד בדיוק מבין אלה:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

לדוגמה, השתמשו ב-`colorscheme sleela-russian-slavic` עבור הפלטה הבינונית הרוסית-הסלאבית, או ב-`colorscheme sleela-greek` / `colorscheme sleela-turkish` עבור הפלטות ההלניות וה-Al Bayrak. הפעילו מחדש את Vim או טענו את התצורה בעזרת `:source $MYVIMRC`.

### התקנת ערכות Emacs

צרו תיקיית ערכות נושא מקומית והעתיקו אליה את קובצי המצב והערכות משורש המאגר:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

הוסיפו זאת ל-`~/.emacs` או ל-`~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

החליפו את `sleela-american` ב-`sleela-eurasia`, ב-`sleela-russian-slavic`, ב-`sleela-silk-road`, ב-`sleela-east-asian`, ב-`sleela-greek`, ב-`sleela-turkish`, ב-`sleela-kiro` או ב-`sleela-sleela` כדי לבחור ערכה אחרת. אם Emacs שואל אם לתת אמון בערכת הנושא, בדקו את קובץ הערכה המקומי לפני שאתם מאשרים.

### תצוגה מקדימה של הפלטות המקוריות ב-C/C++

כלי הפלטות המקורי הקטן יכול לפרט ערכות, להדפיס את צבעי הטרמינל שלהן ולחפש צבע בודד:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

לשם תאימות לאחור, `--palette` ו-`--hex COLOR` ללא `--theme` בוחרים ב-American Professional. יש לבדוק את תוצאות הבנייה והבדיקה במערכת היעד וב-CI; תיעוד של פקודה אינו ראיה לכך שהיא עברה.

לתיאורים המלאים של הפלטות ולפרטי הכלי ראו [מדריך שילוב העורכים](linux-editors/README.md).

## IX. תרומה
1. קראו את אינדקס התיעוד ואת מדיניות הבדיקות.
2. בצעו שינויים ממוקדים עם בדיקות חיוביות ושליליות.
3. הריצו בדיקות ממוקדות, ואז את המערך הרחב.
4. דווחו על פרטי מערכת ההפעלה ושרשרת הכלים; אל תתארו בדיקות שדולגו כעוברות.

SLeeLa מפותחת על ידי **MEARVK LLC**. חומר היסטורי של דף הנחיתה נשמר ב-[`docs/README-LEGACY.md`](docs/README-LEGACY.md).

## X. AE6E66 — ציפיית מס על דוא"ל Round Public

ביוזמת AE6E66, הציפייה המוצהרת של הפרויקט היא **$300,000,000.00 USD לכל דוא"ל Round Public**, כסכום מס מוצע הקשור לכל דוא"ל Round Public מזכה.

הצהרה זו מתעדת ציפיית פרויקט בלבד. היא **אינה** שומת מס משפטית, צו ממשלתי, פסק דין או חבות מס שנקבעה באופן עצמאי. כל מס בפועל ידרוש את הסמכות המשפטית הרלוונטית, שיפוט, הגדרות, הליך וקביעה ממשלתית מוסמכת.


## XI. זמן ריצה של אלפבית וערכות תווים

תיעוד האלפבית של SLeeLa וסביבת הריצה המקורית לתווים מתוחזקים ב-alphabet/sleela/ וב-lib/alphabet/native/. בדיקות C++17 המקוריות מכסות פענוח/קידוד קפדני של סקלרי UTF-8, חתימות קיבולת C<n>, וחברות מוגבלת בערכת תווים:

    make -C lib/alphabet/native test

קטלוג UTF-4088 הניסיוני ומתאם החיפוש שלו ב-SLeeLa מתועדים ב-utf-4088/SLeeLa-ALPHABET-INTEGRATION.md. מזהי UTF-4088 הם מרחב שמות נפרד, ולא ערכים סקלריים של Unicode. העבודה על קישור המהדר ו-SLVM נעקבת בנפרד במפורש מיסודות מקוריים אלה.

## XII. AutoCAD Creator™

[AutoCAD Creator™](autocad/creator/README.md) הוא יישום CAD דו-ממדי אינטראקטיבי
הכתוב כולו ב-SLeeLa. בעוד AutoCAD Renderer האחות הופך תוכנית לשרטוט בבת אחת, ה-Creator מאפשר **ליצור ולשנות** מסמך AutoCAD דו-ממדי בזמן אמת — להוסיף קירות, חדרים, מידות והערות, ואז להזיז, לשנות תוויות, לשנות שכבות, למחוק ולבטל את מה שהוצב — ו**לשמור** את התוצאה כשרטוט AutoCAD DXF אמיתי (טקסט R12, נפתח ב-AutoCAD / LibreCAD).

הממשק הגרפי שלו בנוי על **SleelaUI™**, ערכת ה-UI הרב-פלטפורמית של SLeeLa עצמה (תחת [`lib/user-interface`](lib/user-interface)) — לא JavaFX — המעלה
חלון מקורי אמיתי על X11 / Cocoa / Win32. העיצוב פועל לפי כלל אחד,
**"SleelaUI מציגה; SLeeLa מחליטה"**: ה-GUI לעולם אינו מחשב גיאומטריה וכותב
DXF. כל כלי מוציא כוונת יצירה / שינוי / מחיקה / ביטול / שמירה אל
[`SLCadCreator`](lib/autocad/SLCadCreator.sleela) הסמכותי, שמחזיק במסמך
הניתן לעריכה ובהיסטוריית פקודות הפיכה, בעוד הקנבס רק מציג תצוגה מקדימה
של הגיאומטריה שה-Creator מחזיק.

מנוע העריכה לשימוש חוזר שוכן ב-[`lib/autocad`](lib/autocad) —
[`SLCadEntity`](lib/autocad/SLCadEntity.sleela),
[`SLCadDocument`](lib/autocad/SLCadDocument.sleela),
[`SLCadCommand`](lib/autocad/SLCadCommand.sleela) ו-
[`SLCadCreator`](lib/autocad/SLCadCreator.sleela) — ורץ מקצה לקצה ללא ממשק (headless).
מעטפת ה-GUI וההדגמה שוכנות תחת [`autocad/creator`](autocad/creator):

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

על מסך נפתח חלון העריכה החי; על מארח ללא מסך הוא בונה את
השרטוט בזיכרון, מדפיס דוח וכן את ה-DXF, ויוצא בצורה נקייה.


## XIII. סדר מפת הקלט של UTF-4088

קטלוג UTF-4088 המיוחד מגדיר את סריקת מפת הקלט המרחבית ואת איטרציות הלחץ המופשטות שלו ב-[`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). מדיניות אינדקס הנושאים National Lien נמצאת ב-[`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). קואורדינטות מפה, רמות לחץ (1–50), מזהי קטלוג קבועים ודרגות אלפביתיות הם שדות בלתי תלויים; אין להחליף אחד מהם באחר בשקט. לחץ הוא רמת איטרציה אלגוריתמית, לא גודל פיזיקלי.


## XIV. אינדקס חבילות עדכני ומורחב

אינדקס מצורף זה מדגיש חבילות שהורחבו לאחרונה, ספריות תחום
ואוספי מקורות. הערכים מקובצים לפי סדר אלפביתי לפי שם החבילה או הנושא.
התיאורים מציינים את מיקום המאגר; מצב המימוש והאימות
יש לבדוק בתיעוד ובבדיקות של כל חבילה.

### A–C

- **Advantage** — `lib/advantage/` ו-`advantage/`: מתן יתרון לדיבידנד ייחודי ביחס למודל כבצל Sequitur שכבתי, עם חמש
  תיקיות משנה לנושאים — `math/` (מקסום רווח משותף, מיקוח נאש,
  newsvendor דו-צדדי), `physics/` (מטריקה/גאודזית/מגע/SDE של שדה אמון,
  אנלוגיית מידול מסומנת), `integration/` (תאי אינטגרציה אנכית
  וציון תחכום), `grading/` (מדרג מעגלי תחכום A–D
  למודולים ולקבוצות), ו-`output/` (פליטה לטרמינל / CommonRails /
  socket). צרכן C/C++ מקורי (`advantage/`) מקשר את המודל אל
  דוגמאות סחר מעשיות. ראו [`lib/advantage/README.md`](lib/advantage/README.md)
  ו-[`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alphabet ו-UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` ו-
  `utf-4088/`: קטלוגי אלפבית, תמיכה מקורית בערכות תווים, סדר מפת קלט
  ותיעוד שילוב UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` ו-`lib/autocad/`: תהליכי עבודה אינטראקטיביים
  לשרטוט דו-ממדי ורכיבי מסמך/עריכה CAD לשימוש חוזר.
- **Ballum** — שליטה בתצוגה ברמת הפיקסל: set/get לכל פיקסל, blit לאזור,
  בהירות/חמימות/גמא לכל התצוגה, קלט מצביע/מקלדת, ו**משטח framebuffer במסך מלא** לשליטה מלאה, ייעודית ומהירה בכל פיקסל
  (חלון override-redirect + הצגת `XPutImage`/MIT-SHM יחידה ב-Linux/X11).
  ליבה מקורית תחת `ballum/` עם חזית SLeeLa ב-`lib/ballum/` וגשר VM תחת `impl/`; ראו [`ballum/README.md`](ballum/README.md) ואת
  [`ballum/example/`](ballum/example/) להרצה.
- **CDMA** — `cdma/` ו-`lib/cdma/`: חומר טכני הקשור ל-CDMA ועבודת ספריית מקור של SLeeLa.
- **Character** — `lib/character/`: פרופילי דמויות וסטטיסטיקות דמות לשימוש חוזר,
  כולל דוגמאות לפרופילים ביוגרפיים.
- **Coorenagraph** — `lib/coorenagraph/`: חשיבה אתית, אוצר מילים מוסרי,
  שקילה ומושגים קשורים; ראו את ה-README של החבילה ואת קובצי המקור שלה.

### D–G

- **מודלי מקור לממשל (Governance)** — [lib/governance](lib/governance/README.md) מגדירה צורות מקור הצהרתיות של SLeeLa להצעות, קלפיות ידניות אנונימיות וספירות מצטברות. ההערה המוצעת @vote והתהליך הידני מתועדים ב-[registry/DEMOCRATIC-VOTING-SYNTAX.md](registry/DEMOCRATIC-VOTING-SYNTAX.md); המהדרים הנוכחיים מתייחסים אליה כמטא-נתוני הערה בלבד.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` ועד לתיקיות חבילות HTTP
  הממוספרות לפי גרסה הזמינות: מפרטי פרוטוקול ומימושים ספציפיים לגרסה. עיינו בתיעוד של כל גרסה
  להיקפה המדויק ולמצב האימות שלה.
- **בית הדין הפלילי הבין-לאומי (International Criminal Court)** — `lib/international-criminal-court/` ו-
  `INTERNATIONAL.CRIMINAL.COURTS.md`: מקורות ותיעוד ייעודיים לתחום.

### O–S

- **OFDMA** — `ofdma/`: חומר טכני על גישה מרובה בחלוקת תדר אורתוגונלית
  ומשאבי פיתוח ב-C/C++.
- **Philosophy ומודול Conversation Mask** — [`philosophy/`](philosophy/README.md)
  מכילה את היסודות המקוריים ב-C/C++ ו-[`philosophy/module/`](philosophy/module/README.md)
  מארגנת את שכבת המודול הבאה שלהם בשלושה תחומים מפורשים:
  [`data/`](philosophy/module/data/README.md) עבור תצפיות מוטיפסות ומתוארכות,
  סיומי אירועים, הגדרות מחצית חיים, מקוריות והסכמה; [`helper/`](philosophy/module/helper/README.md)
  עבור אימות דטרמיניסטי, דעיכת רלוונטיות, השוואות תואמות והצגה מסוננת לפי הסכמה; ו-[`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md)
  עבור ממשק שיחה תומך ולא-קליני. הידיות הפונות ל-SLeeLa
  והעץ המורכב נמצאים ב-[`lib/philosophy/`](lib/philosophy/README.md),
  לצד חבילת הפילוסופיה הרחבה יותר. המודול מבוסס קלט מפורש
  והסכמה תחילה: הוא אינו עוקב באופן פסיבי אחר אנשים או מדיה, אינו מאבחן, אינו מסיק כוונה פרטית, אינו מדרג ערך אנושי ואינו ממליץ על שינויי תרופות. קישורי האירועים שלו שומרים על מקוריות וניווט, לא על אימות עצמאי.
- **Synchro** — שילוב סביבת ריצה/מהדר מקורי ופונקציונליות של שיגור חבילות
  ומדידה; ראו את מקור `impl/` הרלוונטי ואת מימוש המהדר.
- **SleelaUI / ממשק טרמינל** — `lib/user-interface/` ופרויקט
  `sleela-terminal`: שילוב ממשק משתמש של SLeeLa ורכיבי אינטראקציה
  מכווני טרמינל.

### U–W

- **האומות המאוחדות (United Nations)** — `united-nations/`: חומרי מאגר ייעודיים לתחום.
- **Wi-Fi 4, Wi-Fi 5 ו-Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`,
  `wi-fi-6/` ותיקיות המקור הקשורות `lib/wifi/`: חומרי רשת אלחוטית
  וקובצי תצורה ייעודיים לכל דור.

### תחזוקת אינדקס החבילות

כאשר חבילה נוספת או מורחבת באופן מהותי, הוסיפו את הערך שלה כאן
בקבוצה האלפביתית המתאימה, קשרו לספרייה הקנונית שלה ול-README שלה,
ותארו רק יכולות הנתמכות על ידי המקורות שבמאגר. שמרו על האינדקס
תוספתי: אל תסירו ואל תשכתבו את סעיפי ה-README הקיימים
במסגרת עדכוני אינדקס החבילות.


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

US Democratic and US Policy.

---


## XVI. דימויי המותג של National Science

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. קטלוג ניסיוני UTF-4088 בן מיליון רשומות

[מחולל הקטלוג של UTF-4088](utf-4088/tools/generate_catalogue.py) בונה באופן דטרמיניסטי קטלוג סמלים ניסיוני בן 1,048,576 רשומות. הפורמט הקומפקטי המתועד מתואר ב-[utf-4088/FORMAT.md](utf-4088/FORMAT.md); חוזה היצירה והאימות נמצא ב-[utf-4088/GENERATION-RESULTS.md](utf-4088/GENERATION-RESULTS.md).

הריצו את המחולל באמצעות Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

הבינארי שנוצר הוא בגודל 16,777,244 בתים ומתוכנן להישאר מתחת ל-20,000,000 בתים. בדיקות יחידה ותהליך עבודה של GitHub Actions מאמתים את מספר הרשומות, ייחודיות הגליפים, הגודל ו-SHA-256; ריצות מוצלחות מפרסמות את הבינארי ואת הסיכום כתוצר העבודה `utf4088-million-catalogue`. הבינארי שנוצר נשמר במכוון מחוץ להיסטוריית Git. UTF-4088 נשאר ניסיוני ואינו תואם Unicode.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. תוספות אחרונות — קבוע Pi, אתחול Modium/Registry, מתמטיקה ב-sleelactl ו-`@native`

סעיף זה מתעד תכונות שנוספו במהלך העבודה של 2026-11, החדשות ביותר תחילה. קובצי המקור המקושרים ומצב האימות שלהם הם הסמכותיים; שינויים בבדיקה אינם מתוארים כממוזגים עד שהגיעו ל-`master`.

### קבוע קטלוג Pi בן 100,000,000 ספרות

המאגר כולל ערך **מאומת של מאה מיליון ספרות** של π ב-[`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (עשרוני תקין, `3.14159…`), לצד ערך בן 57,344 ספרות ב-[`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). שני הערכים אומתו באופן בלתי תלוי כ-π אמיתי — ראו [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md), המתעד את הבדיקות (תחילית ספר לימוד, נקודת פיינמן במיקום השברי 762, הספרה המיליונית הידועה, התאמה ל-`lib/math/SLPiReference.sleela`, וניתוח תדירות ספרות של הקובץ כולו) וסקריפט שחזור חד-פעמי.

### Modium™ / Registry™ טוענים את קבוע Pi באתחול תת-המערכת

בנקודות הכניסה הייעודיות של תת-המערכות — `sleela registry` ו-`sleela modium` (והרצה ראשונה של המתקין), **ולא** בכל `sleela run` — תת-המערכות Modium ו-Registry טוענות את קבוע הקטלוג הגדול של π. בטרמינל אינטראקטיבי הן מדפיסות שורה אדומה חולפת `Loading Modium™` / `Loading Registry™` שנמחקת במקום לפני תחילת הפלט הרגיל; האפקט מותנה בקפדנות ב-TTY, כך שפלט מצינור ופלט בדיקות נשארים זהים בייט-לבייט. המאתר (ולא הבתים) נזרע כברירות המחדל של מפתח ה-`Math` ב-registry, `piReferenceLargePath` ו-`piReferenceLargeDigits`, הן במודל SLeeLa ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) והן בזורע המקורי ([`registry/sleela_registry.c`](registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` חושף את `largePath()` / `largeDigits()`. קבוע π מותקן במיקום SLeeLa של מערכת ההפעלה (`<data>/pi/`) על ידי **שני** המתקינים: מתקין `make` הסטנדרטי והמתקין המודרך "היפה". פרטים: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — המתקין היפה ונושא `math` מגובה-מקורי

- המתקין המודרך ("היפה") מתקין כעת את `sleelactl` (מישור הבקרה של SLeeLa) ואת רישום המודולים שלו, בהתאם למתקין `make`. ראו [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- ל-`sleelactl` נוספה **טבע** חדש בשם **`subject`** עבור ספריות הנושא המקוריות ב-C/C++ המהודרות לתוך סביבת הריצה, והוא רושם את הראשונה: **`math`**. `sleelactl run math` מפעיל את הליבה המקורית `__NativeMath` (`impl/subjects/math/math_native.cpp`) מקצה לקצה באמצעות ההדגמה של הנושא; `sleelactl info math` מסביר כיצד מגיעים אליה (`import math;`). טבע ה-subject מציע `run info` בלבד — אין במכוון פעולת `check`, משום ש-`sleela check` אינה טוענת את הנושאים המקוריים הרשומים ב-SHEET.

### `@native`, Native Drive והרצה רב-לשונית

הערת מתודה חדשה **`@native`** מבקשת קומפילציה מקורית מראש (ahead-of-time) של מתודה חמה. הפקודה החדשה `sleela aot` מוצאת מתודות `@native`, מוודאת שכל אחת נמצאת ב*תת-קבוצה הניתנת להעברה* הנתמכת, ופולטת C/C++ — **נכשלת באופן סגור** (מסרבת, עם סיבה מדויקת) בכל מבנה שאינה יכולה עדיין לתרגם, כך שלעולם לא נוצרת מתודה מתורגמת למחצה.

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

`@native` נשארת בקשה לטיפול AOT, כאשר מבנים שאינם נתמכים נדחים לפני הפליטה. ה-[Native Drive](impl/native-drive/README.md) תומך בבניית קובצי הרצה וספריות משותפות של C/C++ מרובי יחידות באמצעות מהדרי המארח (כולל MSVC), בפלט AOT נתמך של SLeeLa, בבדיקת סמלים מול ה-ABI של הטוען המקורי, בהרצת תהליכים ללא shell, בטיפול ב-stdin/stdout/stderr, בצנרת רב-שלבית ובהאצלת XClass לטוען הייצור. `sleelactl` חושף את פקודות הבנייה/ההרצה שלו ומנתב בדיקת XClass והצגה/התקנה מפורשות של יחידות systemd דרך `inspect native-drive`. ה-VM מחזיק טבלת ידיות מודולים מקוריים מוגבלת עם הרשאת מארח חובה וידיות הנבדקות לפי דור. תחביר 3.0 מוסיף כעת את `nativeOpen`, `nativeCall` ו-`nativeClose`: שיגור bytecode מסוג append-only חוצה ABI בעל גרסה נפרדת של null/bool/int64/double/UTF-8, בעוד מערכים, structs וערכים מורכבים אחרים בבעלות ה-VM נשארים בתוך המפרש ויכולים להשתמש בנפילה לאחור ל-bytecode בעל אותו שם. תוספים יכולים לקבל באופן אופציונלי טבלת callback של המארח בעלת גרסה כדי להפעיל פונקציות SLeeLa מדויקות דרך הרצת VM מקוננת, עם שורשי GC למחסנית מושעית, רשימת היתרים מפורשת לשמות callback של המארח, וקריאות API מקוריות רקורסיביות נדחות. קוד מקורי נשאר קוד מהימן בתוך התהליך, לא sandbox. להערות מדיניות תהליך במקור יש סכמה בעלת גרסה אך הן עדיין אינן מנותחות/נאכפות. פרטים, גבולות אבטחה ורשימת שבעת זרמי העבודה: [Native Drive](impl/native-drive/README.md) ו-[`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).


## XIX. מהדורות README בין-לאומיות

SLeeLa מספקת נקודות כניסה ל-README לפי מדינה עבור 50 השווקים של המהדורה הבין-לאומית שלה. לכל מדינה קובץ אחד בשם `README.<CC>.md`; כאשר רלוונטיות מספר שפות, הן מקובצות בתוך הקובץ של אותה מדינה.

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

**מדיניות תרגום:** קובצי README אלה הם נקודות כניסה מקומיות ראשוניות לפי מדינה. המבואות וכותרות השפה בהם מותאמים מקומית, בעוד ההפניה הטכנית המלאה נשארת ב-[README.md](README.md) הקנוני באנגלית. אין לתאר קובץ מדינה כתרגום מלא עד שכל סעיף תורגם, המזהים הטכניים והקישורים נבדקו והמהדורה עברה סקירה לשונית. כיסוי השפות בסבב ראשון זה הוא מייצג ואינו טענה שכל שפה מוכרת חוקתית, אזורית, ילידית או שפת סימנים תורגמה במלואה.

## נספח — הנחיות אזוריות שנשמרו

הסעיפים שלהלן נשמרו מהמדריך העברי-ערבי הקצר הקודם של מהדורת ישראל. הם משלימים את הסעיפים הקנוניים I–XIX ואינם מחליפים אותם; במקרה של סתירה, [README.md](README.md) הקנוני הוא המחייב.

### ארכיטקטורת המהדר וזרימת הקומפילציה

SLeeLa מפרידה בין שפת המקור, חזית המהדר, ייצוג ביניים, יצירת קוד וסביבת הריצה. גרסת המהדר, מהדורת התחביר וגרסת VM/ISA הן צירי גרסאות נפרדים. הופעת תכונה במפרט אינה מוכיחה שהיא ממומשת ומאומתת בכל יעד.

צינור העבודה הטיפוסי:

1. **מקור:** קובצי UTF-8 מסוג `.sleela` עם הצהרת גרסה והצהרות תוכנית.
2. **ניתוח לקסיקלי ותחבירי:** המרת הטקסט לאסימונים ולמבנה תחבירי.
3. **ניתוח סמנטי:** בדיקת שמות, סמלים, טיפוסים וכללי השפה.
4. **ייצוג ביניים:** צורה מובנית של התוכנית לשלבי lowering נוספים.
5. **יצירת קוד:** הפקת פלט ליעדי VM או יעדי ביניים נתמכים.
6. **Runtime:** הרצת התוכנית המקובצת באמצעות מנגנוני הזיכרון וזרימת הבקרה הזמינים.

המפרט הנורמטיבי הוא [SLEELA.syntax](SLEELA.syntax). לפרטי פורמט המקור ומחזור החיים של קובץ, ראו [source documentation](markdown/SOURCE.md) ו-[compiler documentation](markdown/COMPILER.md).

### טיפוסים ומודל האובייקטים

אוצר הטיפוסים הבסיסי כולל `int`, `double`, `boolean`, `String`, `void`, טיפוסי `struct` שהוגדרו בקוד וחתימות נתמכות של טיפוסי reference. לפי התיעוד הנוכחי, `int`, `long` ו-`int64` הם מספרים שלמים חתומים בני 64 סיביות. `int128` מתקבל כטיפוס שלם, אך המימוש המתועד עדיין מבוסס על 64 סיביות; אין להתייחס אליו כאל אריתמטיקה מלאה של 128 סיביות.

`struct` הוא מבנה מצטבר בעל שדות בעלי שם. ידיות struct המנוהלות על ידי ה-VM אינן מצביעים גולמיים החשופים למשתמש. השמה והעברת פרמטרים משתפות את אותו מופע, והשוויון מבוסס זהות. להעברה ניידת יש להשתמש בסריאליזציה המתועדת, כגון `structPack(instance)` ו-`structUnpack(TypeName, json)`; ידית מקומית של VM אינה ערך רשת נייד.

מחלקות מכילות שדות ומתודות. במודל הביצוע הנוכחי השדות הם מצב מחלקה משותף, ואילו משתנים מקומיים של מתודה שייכים ל-thread. ירושה, מחלקות מקוננות ו-generics כפופים לשערי גרסת תחביר. ראו [International Language Reference](LANGUAGE.INTERNATIONAL.3.0.md).

### זרימת בקרה ותזמון לולאות

התחביר כולל תנאים, לולאות, קריאות למתודות ורקורסיה. ההערות ההתנהגותיות `@loopDelay` ו-`@loopBudget` מגדירות השהיה או תקציב זמן ללולאה. הן נכתבות לפני הלולאה ומונמכות להוראת VM בשם `OP_SLEEP` (ISA 157). גם הפונקציה המובנית `sleep(ms)` משתמשת בהוראה זו.

```sleela
@loopDelay(ms = 250)
while (running) {
    processNext();
}
```

הדוגמה מציגה את צורת התחביר בלבד; יש להגדיר בתוכנית בפועל את המזהים ואת המחלקה העוטפת. ראו [SLEELA.syntax §32](SLEELA.syntax) ואת [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md).

### ספריות ורכיבים מרכזיים

המאגר כולל רכיבים ברמות בשלות שונות:

- `lib/advantage` — הערכה שכבתית של תוצאה ביחס למודל, לרבות מתמטיקה, כלכלת סחר דו-צדדי, אינטגרציה, דירוג ופלט מנותב. הדיון בשדות אמון בהשראת הפיזיקה הוא אנלוגיית מידול, לא טענה לחוק פיזיקלי.
- `lib/xml` — עץ מסמכי XML עם צמתי element, text, comment, CDATA ו-processing instruction, מאפיינים, parser ו-serializer.
- `lib/text` — פעולות טקסט ומחרוזות מבוססות תאי תווים, כגון `charAt`, `substring`, `indexOf`, `reverse` ועיצוב לפי מיקום.
- `lib/sleeladoc` — חילוץ תיעוד מקוד המקור ודפי עיון ב-CLI. הפקודה `sleela docs` מציגה נושאים, חבילות ומחלקות; `sleela docs --generate [dir]` יוצרת מאגר טקסט מקומי לשימוש לא מקוון.
- `lib/cpu` ו-`lib/xlt` — פרופילי CPU, רכיבי ארכיטקטורה, פורמט מודלים והפניות לתזמון.
- `recursion/stanchion` — אבות-טיפוס לדוגמה ב-C/C++ ודוגמאות לרקורסיה.

הופעת רכיב בקטלוג אינה מעידה שהוא מוכן לשימוש בייצור. יש לבדוק את README של המודול, כיסוי הבדיקות, מגבלות הפלטפורמה ורישומי האימות.

### שילוב C/C++ וגבול native

המאגר כולל רכיבי C ו-C++ ונתיב מוגבל ליצירת קוד native. הסמן `@native` והפקודה `sleela aot` קשורים לפיתוח native; הם אינם הרשאה כללית להריץ קוד native שרירותי. פרטי ה-Native Drive מופיעים בסעיף XVIII ובמסמך [Native Drive](impl/native-drive/README.md).

שילוב C/C++ חייב להגדיר במפורש ABI, מוסכמות קריאה, בעלות על זיכרון, פריסת נתונים, טיפול בשגיאות ותלויות פלטפורמה. יש להתייחס לקוד native כאל גבול אמון נפרד. reference או declaration מזהים משאב, אך אינם מעניקים הרשאת ביצוע. ראו [native implementation](impl/README.md), [platform matrix](docs/PLATFORM-SUPPORT.md) ו-[comparison standards](comparison/README.md).

### אבטחה, שלמות ובדיקות

Hash manifests וכלי אימות יכולים להשוות קבצים לתוכן הצפוי. תוצאת PASS חלה רק על ההיקף ועל מועד הבדיקה שצוינו. דוחות בדיקה חייבים להבחין בין pass, fail ו-skip; אין לסמן בדיקה שדולגה או לא הופעלה כהצלחה.

עקרונות מרכזיים:

- References מזהים משאבים; הם אינם מעניקים הרשאת ביצוע.
- ממשקי native חייבים להיות מפורשים ומוגבלים.
- טענות לתמיכת פלטפורמה צריכות להיתמך בבניות ובבדיקות ממשיות.
- תיעוד שנוצר אוטומטית חייב להיות ניתן למעקב עד המקור וה-generator.
- כאשר חסר מידע שלמות חיוני, בדיקות אבטחה צריכות להיכשל באופן סגור ובטוח.

ראו [testing policy](markdown/TESTING.md) ו-[platform validation](docs/PLATFORM-SUPPORT.md).

### חוזי API של מודולים, תאימות ואסטרטגיית בדיקות

בעת הוספת API חדש, יש לתעד קלט, פלט, תנאי שגיאה, בעלות על נתונים והאם הפעולה יוצרת side effects. דוגמאות התיעוד צריכות להציג שימוש רגיל וגם טיפול ב-invalid או missing input. אין להציג התנהגות כהתחייבות אם היא לא אומתה באמצעות ה-implementation וה-tests המתאימים.

יש לחלק בדיקות לפי שלבי compiler pipeline: lexing/parsing, semantic analysis, IR lowering, code generation, VM execution ו-native integration. לכל regression case יש לשמור קלט מינימלי שניתן לשחזור, תוצאה צפויה ותוצאה בפועל. שינויים ב-API צריכים להסביר backward compatibility, migration steps והשפעה על examples קיימים.

### לוקליזציה, קידוד ותחזוקת תרגום

יש לשמור על technical identifiers, commands, paths, syntax ו-code blocks ללא שינוי בין גרסאות השפה. יש לתרגם את הטקסט המסביר, ולא tokens או שמות המשמשים ב-source code. יש לציין בבירור את ההקשר הלשוני והאזורי, ולשמור על מינוח עברי עקבי.

לאחר כל עדכון יש לבדוק Markdown links, היררכיית כותרות, code fences ודוגמאות. כאשר canonical English documents משתנים, יש להשוות ולעדכן את הסעיפים המקבילים במלואם ולא רק את המבוא. יש לסמן תוכן שטרם עבר בדיקה לשונית או טכנית.

### ملحق عربي — نظرة تقنية

يفصل SLeeLa بين لغة المصدر وواجهة المترجم والتمثيل الوسيط وتوليد الشيفرة وبيئة التشغيل. إصدار المترجم وإصدار الصياغة وإصدار VM/ISA مسارات مستقلة، ووجود ميزة في المواصفة لا يثبت أنها منفذة ومتحقق منها على كل هدف.

تتضمن سلسلة البناء قراءة ملفات `.sleela`، والتحليل المعجمي والنحوي، والتحليل الدلالي، والتمثيل الوسيط، وتوليد المخرجات للأهداف المدعومة، ثم التنفيذ في runtime. المرجع المعياري هو [SLEELA.syntax](SLEELA.syntax)، كما توضح [وثيقة اللغة الدولية](LANGUAGE.INTERNATIONAL.3.0.md) تفاصيل الأنواع والكائنات والوراثة والأنواع العامة.

تضم المكتبات الموثقة Advantage وXML DOM وText/String وSleelaDoc وCPU وXLT. وجود وحدة في المستودع لا يعني تلقائياً أنها جاهزة للإنتاج؛ ينبغي مراجعة README والاختبارات وحدود المنصات وسجل التحقق الخاص بها.

يتطلب دمج C/C++ تحديد ABI وملكية الذاكرة وتخطيط البيانات ومعالجة الأخطاء واعتماديات المنصة. الشيفرة native حد ثقة مستقل، والمرجع يحدد المورد ولا يمنح إذناً بالتنفيذ. راجع [native implementation](impl/README.md) و[platform matrix](docs/PLATFORM-SUPPORT.md).

تتطلب عملية البناء عادةً GCC أو Clang وGNU Make، وقد تحتاج بعض الأدوات إلى Python 3. راجع [دليل التثبيت](install/README.md) قبل تثبيت النظام أو تثبيت المستخدم. يجب أن تميز تقارير الاختبار بين pass وfail وskip، وألا تعتبر الاختبارات المتروكة ناجحة. راجع [سياسة الاختبار](markdown/TESTING.md). أوامر البناء الأساسية مذكورة في القسم III أعلاه.

**حالة الترجمة:** أضيف هذا الملحق العربي إلى جانب الدليل العبري الموسع. وهو مدخل تقني موجز، وليس ترجمة عربية كاملة سطراً بسطر لجميع وثائق المشروع.

**סטטוס התרגום:** סעיפים I–XIX תורגמו לעברית (בתוספת מבוא קצר וצירוף טכני בערבית). מזהים טכניים, פקודות, דוגמאות קוד, שמות קבצים, קישורים וכתובות נשמרו ללא שינוי. מומלצת סקירה לשונית סופית בידי דוברי עברית ילידיים בעלי רקע טכני (ובמקטעים בערבית — בידי דוברי ערבית).
