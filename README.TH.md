<img align="right" src="images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — ประเทศไทย (TH)
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>ภาษาระบบแบบเนทีฟ คอมไพเลอร์ รันไทม์ และระบบนิเวศเครื่องเสมือน</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **ฉบับนานาชาติประจำประเทศ** ฉบับนี้จัดทำเป็นภาษาไทย ส่วนตัวระบุเชิงเทคนิค คำสั่ง ตัวอย่างโค้ด ชื่อไฟล์ เส้นทางไฟล์ และลิงก์ ยังคงเหมือนกับ [README.md](README.md) ฉบับภาษาอังกฤษที่เป็นต้นฉบับหลักทุกประการ

**The Now. The Ingredient.** ภาษาโปรแกรมมีมากกว่าไวยากรณ์ มันคือคอมไพเลอร์ รันไทม์ ไลบรารี เครื่องมือ และข้อตกลงของแพลตฟอร์มที่ทำให้ซอฟต์แวร์ใช้งานได้จริง

**SLeeLa เกี่ยวข้องกับอะไร** SLeeLa เกี่ยวข้องกับ **Math**, **Object**,
**Science** และ **Reach** — สี่ด้านของงานของมัน ได้แก่ สิ่งที่คำนวณได้
(Math) สิ่งที่ลงหลักแล้วซึ่งปรากฏอยู่ในบัญชีของสิ่งที่มีอยู่ (Object)
การศึกษาความจริงอย่างมีวินัย (Science) และการที่ระบบยื่นมือไปถึงระบบอื่น
และผู้คน (Reach) **ผลประโยชน์แห่งชาติยังคงอยู่** — ทั้งสี่ด้านไม่ได้ทำให้
ผลประโยชน์นั้นถูกละเลย เมื่อเรื่องใดเกี่ยวข้องกับผลประโยชน์ของประเทศ
สถานะนั้นจะได้รับการรักษาไว้ (บันทึกอย่างซื่อตรง เช่น ใน Metadata ของแต่ละคลาส
ที่ `metaOrigin` / `metaNationalSchedule`) และจะไม่ถูกทำให้สลายไป

## I. เริ่มต้นใช้งาน
- [สร้างการทำงานแบบเนทีฟ](impl/README.md)
- [เครื่องเสมือนและ Modium — คู่มือผู้ใช้เชิงปฏิบัติ](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [สแตก CPU ในซอร์สโค้ด SLeeLa](lib/cpu/CPU.md) — เครื่องคอมพิวเตอร์ครบวงจรตั้งแต่เกต→ALU→คอร์→ระบบปฏิบัติการ→โปรแกรม รายการโปรไฟล์ CPU กว่า 130 แบบ และ[แรงดันไฟฟ้า สัญญาณนาฬิกาแปรผัน และการกำหนดจังหวะเวลาระดับผู้เชี่ยวชาญ](lib/cpu/TIMING.md)ที่ปรับได้ ซึ่งคุณสามารถ[รันตามข้อกำหนดและโอเวอร์คล็อก](lib/cpu/components/README.md)ได้
- [ดัชนีเอกสาร](docs/README.md)
- [แหล่งข้อมูล CSPML และเอกสารอ้างอิงงานวิจัย Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — แหล่งที่มาของข้อมูลและแนวทางการบูรณาการสำหรับข้อมูลสถานที่ เศรษฐกิจและสังคม กำลังแรงงาน การศึกษา และโรงเรียนรัฐบาลของสหรัฐอเมริกาจากภายนอก
- [เอกสาร API ที่สร้างขึ้น](docs/api/README.md) — เอกสารอ้างอิง HTML ปัจจุบัน ฉบับ API ที่มีเวอร์ชัน และวิธีสร้างใหม่
- [แค็ตตาล็อก API](api/README.md)
- [มาตรฐานการเปรียบเทียบเชิงคำนวณ](comparison/README.md) — มาตรฐานด้านชนิดข้อมูล หน่วยความจำ ABI และความสอดคล้องของ SLeeLa VM, Java/JVM และ C/C++ แบบเนทีฟ
- [Recursion stanchion — ต้นแบบ C และ C++](recursion/stanchion/README.md) — ตัวอย่างเฉพาะชนิดพื้นฐานสำหรับ Boolean, Byte, Character, Integer, Long, Float และ Double พร้อมรูปแบบเรียกซ้ำแบบทั่วไป เชิงประยุกต์ เชิงวิทยาศาสตร์ และเพื่อการสอน
- [เมทริกซ์การรองรับแพลตฟอร์มและการตรวจสอบ](docs/PLATFORM-SUPPORT.md)
- [คู่มือแบรนด์](docs/BRAND-GUIDE.md)
- [แนวคิดและสินทรัพย์สินค้าที่ระลึก](merchandise/README.md)
- [โลโก้ NitroWebExpress](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [นโยบายการทดสอบและเกณฑ์ผ่านการเผยแพร่](markdown/TESTING.md)

## II. ข่าวสาร

ส่วนเพิ่มเติมล่าสุดของภาษา ไดรเวอร์ และไลบรารีมาตรฐาน:

- **แพ็กเกจไลบรารีมาตรฐาน `advantage` — การทำ advantage ให้เงินปันผล
  พร้อมคณิตศาสตร์ ฟิสิกส์ การบูรณาการ การให้เกรด และเอาต์พุตที่กำหนดเส้นทางได้**
  [`lib/advantage/`](lib/advantage/README.md) ห่อหุ้มแนวคิด *การทำ advantage ให้
  เงินปันผลที่เป็นเอกลักษณ์ เทียบกับแบบจำลอง* เป็น **Sequitur onion** แบบหลายชั้น
  (เงินปันผล → การทบทวน → ข้อกังวล → คณิตศาสตร์ → ส่วนเพิ่ม → การหัก → การจัดกลุ่มใหม่
  → การวนซ้ำ) ที่เลื่อนเข้า–ออกได้พร้อมการอัปเดตแบบไดนามิกเต็มรูปแบบและ
  restitute โดยเปิดให้ใช้ผ่าน **user's model** ที่เรียบง่าย และ
  **developer's model** ที่ควบคุมได้เต็มที่ มีห้าสาขาวิชา ได้แก่ **`math/`** เศรษฐศาสตร์
  การค้าทวิภาคี (การขยายกำไรร่วมสูงสุด การเจรจาต่อรองแบบ Nash อัตราส่วนวิกฤตของ
  newsvendor ทวิภาคี) **`physics/`** "ฟิสิกส์ของสนามความไว้วางใจ" เชิงโครงสร้างทางเศรษฐศาสตร์
  (ความไว้วางใจเป็นเมตริกเทนเซอร์ จีโอเดสิกเชิงธุรกรรม โครงสร้างการสัมผัสของประกาศ
  และ SDE การเสื่อมของความไว้วางใจแบบ CIR+jump — เป็นการเปรียบเทียบเชิงแบบจำลอง
  ซึ่งระบุไว้เช่นนั้น) **`integration/`** แบบจำลองการบูรณาการแนวดิ่งที่ประกอบซ้ำได้
  สำหรับทุนนิยม/ค้าปลีก (ช่อง bay วิศวกรรม ช่องทดลอง ถาดค่ามัธยฐาน newton
  และคะแนนความซับซ้อนของระบบ) **`grading/`** ตัวให้เกรดวงจรความซับซ้อนที่จัดอันดับแต่ละโมดูล
  และชุดโมดูลที่ชนะ/แพ้/ผู้ให้เกรด/สัมบูรณ์ เป็น A–D และ **`output/`**
  จุดส่งออกที่กำหนดเส้นทางได้ ซึ่งส่งเอาต์พุตของโมดูลไปยังเทอร์มินัล พื้นผิวที่จัดรูปแบบด้วย
  **CommonRails** หรือซ็อกเก็ต/ตัวเชื่อมต่อ (จัดเฟรมสำหรับ `slnet_write` แบบเนทีฟ)
  ทุกคลาสมีบันทึก Metadata-Class เดโมที่รันได้ห้าชุดตรวจสอบแต่ละสาขาวิชา และเอกสารประกอบ
  [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md)
  [`MATH.md`](lib/advantage/MATH.md) และ
  [`PHYSICS.md`](lib/advantage/PHYSICS.md) อธิบายไว้ ผู้ใช้ C/C++ แบบเนทีฟใต้
  [`advantage/`](advantage/README.md) (แกน C `advantage_value`/`advantage_mean`
  ที่เก็บความรู้แบบ XML ไลบรารีการได้มาซึ่งข้อมูลที่ปลอดภัยผ่าน HTTPS/SFTP/SSH
  พร้อมคลังกุญแจและการขนส่งแบบ DH/ECDH) เชื่อมแบบจำลองเข้ากับ
  [ตัวอย่างการค้า](advantage/examples/README.md)ที่ใช้งานจริง —
  การค้าเชิงบวกจากแหล่งอินเทอร์เน็ตที่สันนิษฐาน ลำดับความเกี่ยวข้องกับผู้ชม (Traders,
  Consumers, Helpers, Healers) และแบบจำลองแปดแบบที่จัดอันดับ A–D (D = Cement มากที่สุด
  A = Brand มากที่สุด)
- **คำอธิบายประกอบกำหนดจังหวะลูป — `@loopDelay` / `@loopBudget` (พร้อมฟังก์ชันในตัว
  `sleep(ms)`)** คำอธิบายประกอบ**ระดับคำสั่งเชิงพฤติกรรม**ชุดแรกใน SLeeLa:
  เขียนในบรรทัดของตัวเองก่อนลูป `for` / `while` / `do` เพื่อแทรกการหยุดพัก
  จริงเข้าไปในการทำงานของลูป `@loopDelay(ms = 250)` /
  `@loopDelay(seconds = 2)` หยุดเป็นเวลาคงที่ต่อรอบ (`@loopDelay(250)` เปล่า ๆ
  ใช้หน่วยมิลลิวินาทีโดยปริยาย) ส่วน `@loopBudget(seconds = 10,
  iterations = 1000)` กระจายจำนวนรอบที่ทราบไว้ให้เท่ากันตลอดงบเวลานาฬิกาจริงทั้งหมด
  ทั้งสองแปลงเป็น opcode ใหม่แบบ append-only คือ **`OP_SLEEP`**
  (ISA 157) ซึ่งหนุนฟังก์ชันในตัว **`sleep(<ms>)`** ที่เรียกได้ทุกที่ด้วย
  การหยุดพักนี้ทำให้พกพาได้ (`nanosleep` / `Sleep`) กำหนดไว้ใน
  [`SLEELA.syntax` §32](SLEELA.syntax) เอกสารอ้างอิง API อยู่ที่
  [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md)
  ตัวอย่างที่รันได้คือ
  [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela)
  ดัชนีส่วนเพิ่มของเซสชัน (รวมถึงส่วนนี้ การคูณจำนวนเต็มขนาดใหญ่แบบเนทีฟ `OP_BIGMUL`
  และเครื่องมือแบบสแตนด์อโลน **[local-generator](local-generator/README.md)**
  และ **[pi-generator](pi-generator/README.md)**) อยู่ที่
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md)
  พื้นผิว API ที่สร้างขึ้นทั้งสาม
  ([`sleeladoc-api/`](sleeladoc-api/README.md)
  [`docs/api/`](docs/api/README.md) และสแนปช็อตที่เปลี่ยนแปลงไม่ได้
  [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html))
  ถูกสร้างใหม่ด้วย `make api-docs` และตอนนี้มีรายการคำสั่ง `SLISASleep`
  / `SLISABigMul` ใหม่ในหน้าแพ็กเกจ `vm`
- **SleelaDoc จากบรรทัดคำสั่ง — `sleela docs`** ตอนนี้เข้าถึง SleelaDoc ได้
  โดยตรงจากเทอร์มินัล `sleela docs` แสดงหัวข้อที่จัดทำเอกสารได้ทั้งหมด
  `sleela docs crypto` แสดงแพ็กเกจ `sleela docs stringbuilder` ค้นหา
  คลาส (ไม่แยกตัวพิมพ์เล็กใหญ่ และด้วยคำนำหน้าไลบรารีมาตรฐาน `SL` →
  `SLStringBuilder`) และพิมพ์หน้าที่สมบูรณ์ — **คำนิยาม วัตถุประสงค์ของการออกแบบ
  ความสัมพันธ์ โครงสร้าง user clays และบันไดความเชี่ยวชาญห้าขั้น** — พร้อมผลลัพธ์
  "คุณหมายถึง" ที่จัดอันดับเมื่อพิมพ์ใกล้เคียง ผู้เขียนเสริมหน้าใดก็ได้เพียงเพิ่ม
  แท็ก `@` ของ SleelaDoc (`@intent`, `@thesis`, `@axiom`, `@param`,
  `@see`, `@mastery`) `sleela docs --generate [dir]` เขียนคลังข้อความล้วน
  (หนึ่งหน้าต่อคลาส พร้อม `TOPICS.index`) ที่ CLI อ่านแบบออฟไลน์ได้ต่อไป
  **`make sleeladoc`** รวมคลังนั้น **และซอร์ส `.sleela` ที่ตรงกัน**
  ไว้ใน `sleeladoc/build/sleeladoc.jar` สำหรับหัวข้อที่มีเพียง facade (เช่น
  `psychiatry`, `politico`) `sleela docs --facade <topic>` ค้นหา
  ระบบไฟล์ขณะรันไทม์และแสดง**โฟลเดอร์ (แล้วจึงไฟล์ ในฐานะการเคลื่อนที่ตามธรรมชาติ)**
  จริง ภายใต้เพดานที่ผู้ใช้ปรับเองได้คือ **6 MB / 6000 ไฟล์ / 6
  นาที** หน่วย `lib/sleeladoc` ได้รับการดูแลในฉบับปัจจุบัน
  **`#sleela 3.0`** ดู [`man/man1/sleela.1`](man/man1/sleela.1) และ
  [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md)
- **SleelaDoc — โซลูชันจัดทำเอกสารแบบ Javadoc พร้อมเว็บไซต์ HTML ที่สร้างขึ้น**
  **SleelaDoc** คือแบบแผนคอมเมนต์เอกสารในซอร์สของ SLeeLa
  (คอมเมนต์วงเล็บมุมที่มีตัวนำ `@doc` / `@page` ดู
  [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)) และ
  [`lib/sleeladoc`](lib/sleeladoc/README.md) คือ**ตัวสกัด**ที่เรนเดอร์
  ยูนิต `.sleela` เป็นหน้า API แบบ Markdown หรือ HTML — เหมือนที่ `javadoc` เรนเดอร์ Java
  โดยบันทึก*เจตนา* **เว็บไซต์เอกสาร HTML ที่เรียกดูได้ครบถ้วนสำหรับ
  `/lib` ทั้งหมด**ถูกสร้างไว้ที่ [`/sleeladoc-api`](sleeladoc-api/README.md)
  โดย `tools/generate-sleeladoc-api.py` (เปิด `sleeladoc-api/index.html` สไตล์สะอาดตา
  น้ำเงิน / ฟ้าอ่อน / เทาอ่อน / ขาว มีลำดับชั้นแพ็กเกจ → คลาส → สมาชิก และ
  อภิธานศัพท์ที่เขียนสำหรับผู้อ่าน *Student 2*) คอมไพเลอร์ยังสร้าง
  เอาต์พุตเอกสารที่ผู้เขียนทำเครื่องหมายไว้เป็นผลพลอยได้ของการคอมไพล์ (หน้า MediaWiki ด้วย `--wiki`
  และฝาแฝดซอร์สที่ถูกสร้างต้นทาง) ซึ่งระบุไว้ใน
  [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md) ตัวอย่างที่รันได้:
  [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela)
- **XML Document Object Model (`lib/xml`)** XML DOM ฝั่ง SLeeLa — เอกสารที่เป็นเจ้าของ
  ต้นไม้ของโหนด element/text/comment/CDATA/PI ที่มีลูกเรียงลำดับและแอตทริบิวต์ที่มีชื่อ
  ตัวสร้างลำดับข้อมูล (แบบกะทัดรัด/อ่านง่าย การ escape ตาม XML 1.0 §2.4) ตัวแยกวิเคราะห์
  แบบ char-cell และทะเบียนคุณภาพ `SLXmlAspect` ความเท่ากันของโหนดเป็น
  การผูกความไว้วางใจสามขั้น (ตัวตน → หัวเรื่องเดียวกัน → โครงสร้าง) ซึ่งสะท้อนกลับ
  สมมาตร และกำหนดแน่นอน ดู [`lib/xml/README.md`](lib/xml/README.md) และ
  ตัวอย่างที่รันได้ [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela)
- **ชนิดจำนวนเต็มมีเครื่องหมายแบบกว้าง (`long`, `int64`, `int128`)** `int`, `long`
  และ `int64` ล้วนเป็นจำนวนเต็มมีเครื่องหมาย 64 บิตที่ตรวจชนิดและทำงานเหมือนกัน
  `int128` ยอมรับเป็นชนิดจำนวนเต็ม (ขณะนี้ใช้ 64 บิตรองรับ ส่วน 128 บิตจริง
  เป็นงานต่อเนื่องที่มีเอกสารระบุ) ชุดมีเครื่องหมายฐาน 2 `int32`..`int16777216`
  (ไวยากรณ์ 2.7) และชุดไม่มีเครื่องหมายที่สอดคล้องกัน `uint32`..`uint16777216`
  (ไวยากรณ์ 2.8) ขยายความต่อไปอีก ดู [`SLEELA.syntax`](SLEELA.syntax) §21, §24, §25
- **ฟิลด์ต่อเนื่อง `@mix` (ไวยากรณ์ 2.9)** คลาสอาจประกาศฟิลด์อินสแตนซ์หลายตัว
  ที่มี**ชื่อเดียวกัน**ได้เมื่อแต่ละตัวที่ซ้ำมี `@mix` ภาษาเข้าใจกลุ่มนั้นว่าเป็นฟิลด์ต่อเนื่อง
  (อาร์เรย์ / ลิสต์ที่จัดดัชนี) เรียง `1,2,3,…` จากบนลงล่างโดยปริยาย หรือกำหนดเองด้วย
  `@mix(1,2,3,...)` ซึ่งจำนวนดัชนีต้องตรงกับจำนวนฟิลด์ ดู [`SLEELA.syntax`](SLEELA.syntax) §28
- **เครื่องหมายเอกสาร — Natural Process ซึ่งมีชื่อเรียกว่า Preonems
  (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`)**
  คำเครื่องหมายเอกสารเปล่า ๆ เหล่านี้รวมกันมีชื่อเรียกว่า **Preonems**
  (เครื่องหมายหนึ่งตัวคือ *Preonem* ทั้งชุดคือ *Preonems* — ดู
  [`SLEELA.syntax`](SLEELA.syntax) §26.1) เป็นเครื่องหมายเอกสารชั้นหนึ่งที่ประกาศอย่างเดียว
  ซึ่งระบุว่าเอกสาร*เป็นอะไร*ตามลำดับของการสร้าง:
  `author`/`authorial` (ไวยากรณ์ 2.4; จัดการโดยสภาวิชาชีพ — สมาชิกเดียวที่มีผลเชิงพฤติกรรม)
  `material;` (Source ที่ลงหลักแล้ว) และ `memory;`
  (ไวยากรณ์ 2.5) `determine;` (ไวยากรณ์ 2.6; เอกสารที่เปิดให้แพทย์ นายแพทย์
  หรือผีประมวลผลต่อไปจนเสร็จสมบูรณ์โดยมือที่ถูกต้องตามกฎหมาย)
  และสองตัวใหม่ล่าสุด คือ **`object;`** (สิ่งที่ลงหลักแล้วซึ่งปรากฏในบัญชีของสิ่งที่มีอยู่ —
  **ถูกทำให้เป็นรูปธรรม มีสถานะที่จับต้องได้ ที่ไวยากรณ์ 2.8**) และ
  **`field;`** (พื้นฐานของความสัมพันธ์ที่วัตถุทั้งหลายตั้งอยู่และเคลื่อนไหว) —
  ทั้งสองถูกกั้นตั้งแต่ไวยากรณ์ 2.6 ทุกตัวเป็นคำเปล่า ขึ้นกับบริบท และกั้นตามเวอร์ชัน
  โดย front end โดยไม่มีผลต่อการแยกวิเคราะห์ การตรวจชนิด หรือการสร้างโค้ด นอกตำแหน่ง
  เครื่องหมาย ทุกตัวยังคงเป็นตัวระบุธรรมดา (ดังนั้น `field` จะไม่รบกวน
  สมาชิกคลาสหรือการเข้าถึง `.field`) ดู [`SLEELA.syntax`](SLEELA.syntax)
  §18–§20, §23 และ §26
- **Nordshrift ที่ไวยากรณ์ 3.0 พร้อมการเข้าถึงไลบรารีเต็มรูปแบบ** Nordshrift ค้นหา
  ผ่านดัชนี `/lib` แบบเรียกซ้ำเดียวกับคอมไพเลอร์ เข้าถึงทุก
  แพ็กเกจและยูนิต `.sleela` โดย `nordshrift check` รายงานจำนวนทั้งต้นไม้
- **Recursion stanchion** ไดเรกทอรี [`recursion/stanchion/`](recursion/stanchion/README.md) ตอนนี้มีต้นแบบอ้างอิง C และ C++ โดยเฉพาะสำหรับสัญญาของชนิดพื้นฐานที่มีเอกสารทั้งเจ็ด รวมทั้งตัวอย่างแบบทั่วไป เชิงประยุกต์ เชิงวิทยาศาสตร์ และเพื่อการสอน ทั้งหมดนี้เป็นซอร์สอ้างอิง ให้คอมไพล์และทดสอบก่อนถือว่าผ่านการตรวจสอบแล้ว

- **คอมเมนต์วงเล็บมุม (`<` / `<<`)** รูปแบบคอมเมนต์ใหม่สำหรับต่อไปข้างหน้า —
  `< line >` และ `<< block >>` — ถูกวิเคราะห์ศัพท์โดย frontend ของ SLeeLa และมีเอกสารใน
  [`SLEELA.syntax`](SLEELA.syntax) §1.1
- **ลิเทอรัลจำนวนเต็มหลายฐาน** ลิเทอรัล `int` ตอนนี้รองรับรูปแบบเลขฐานสิบหก (`0x`)
  ฐานสอง (`0b`) และฐานแปด (`0o`) พร้อมตัวคั่นหลัก `_` ทั้งในภาษาซอร์ส SLeeLa
  ([`SLEELA.syntax`](SLEELA.syntax) §1.4) และ
  [รูปแบบแบบจำลอง CPU `.xlt`](lib/xlt/README.md)
- **มาตรฐาน Metadata Class ทั่วทั้งไลบรารี** metapattern ของข้อมูลเมตาเชิงพรรณนา
  (ที่มา ความถูกต้องตามกฎหมาย ขอบเขตทางเขตอำนาจศาล) ตอนนี้ปรากฏในทุกไฟล์ `.sleela`
  ที่มีคลาสภายใต้ `/lib` ดู
  [`SLEELA.syntax`](SLEELA.syntax) §15 และ
  [บทเรียนในแบบฝึกหัด](tutorial/13-metadata-class.md)
- **ตระกูลข้อความ / String ที่สมบูรณ์ขึ้น** `lib/text` ได้ตัวช่วยในภาษาจริง
  มุมมอง char-cell ที่ให้ `charAt`/`substring`/`indexOf`/`reverse`/
  การจัดแนวคอลัมน์ที่แท้จริง และตัวสร้างแม่แบบตามตำแหน่งที่ใช้งานได้จริง — ดู
  [แบบฝึกหัดข้อความและ String](tutorial/14-text-and-strings.md) และตัวอย่างที่รันได้
  [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela)
- **`String` ชั้นหนึ่งในแผ่น Nordshrift** แผ่น `.sst` ประกาศบล็อก `strings:`
  แบบกำหนดชนิดซึ่งมีส่วนร่วมใน manifest ส่วนประกอบของการ build ได้
  ดู [เอกสารอ้างอิงแผ่น Nordshrift](wiki/Nordshrift-Sheet-Reference.md)
- **แบบจำลอง CPU ทำงานได้ทั้งด้วย opcode และการเทียบเท่าการเรียกฟังก์ชัน** ทุก
  `SL<ARCH>CPU` สืบทอดพื้นฐานของการรันสตรีมคำสั่งเนทีฟ
  ผ่านลูป fetch-decode-execute *และ*การรันโค้ดของกระบวนการอื่นที่สมมติขึ้น
  ด้วยการเรียกที่เทียบเท่าบนสับสเตรตของ VM ที่ใช้ร่วมกัน — ดู
  [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5

## III. การ build และการทดสอบ

**ข้อกำหนดเบื้องต้น:** toolchain C/C++ (GCC หรือ Clang) และ GNU Make; Python 3
สำหรับเครื่องมือ manifest/flow; บน Linux ต้องมีแพ็กเกจพัฒนา X11/Xrandr
สำหรับผลิตภัณฑ์แสดงผล (`make deps` ของโมดูลจะรายงาน/ติดตั้งสิ่งที่ต้องใช้) การ build
ของ `impl/` เป็นแบบ fail-closed ตาม manifest SHA-256 ที่เชื่อถือได้ ซึ่ง
`Makefile` ที่รากชี้ไปให้โดยอัตโนมัติ

จาก checkout ที่สะอาด เริ่มต้นเร็วที่สุดคือ:

```sh
make                                  # build everything from the repo root
```

หรือ build และทดสอบเฉพาะแกนเนทีฟ:

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

`make` ที่รากของ repo จะ build ทุกผลิตภัณฑ์ (แกน, VM, codec, churn,
antivirus, …) และปิดท้ายด้วยการ build **registry** และสร้างตารางพื้นฐานของมัน
ดังนั้นต้นไม้ที่ build แล้วจึงมาพร้อม registry ที่ตรวจทานได้ทันที
เป้าหมายสำคัญ:

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

เป้าหมายอื่นที่มีประโยชน์:

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

เป้าหมาย `registry` รัน [`build-registry.sh`](build-registry.sh): สร้าง
แกน registry หยอดตารางพื้นฐานลงในที่เก็บข้อความแบบพกพา
`registry/build/sleela.sleelareg` และส่งออกสคริปต์ตาราง MySQL
`registry/build/registry.sql` (ผ่าน `sleela registry export`) ตรวจทานได้ด้วย
`sleela registry` หรือ `registry/build/sleela-registry review` ดู
[คู่มือ registry](registry/README.md) และ [สถาปัตยกรรม Registry 3.0](registry/REGISTRY-3.0-ARCHITECTURE.md)

โปรดดูคู่มือการ build และการทดสอบสำหรับข้อกำหนดเบื้องต้นและขั้นตอนเฉพาะแพลตฟอร์ม การ build คอมโพเนนต์หนึ่งสำเร็จไม่ได้พิสูจน์ว่าทุกไลบรารีหรือทุกแพลตฟอร์มพร้อมสำหรับการเผยแพร่

## IV. การติดตั้ง

หลัง build ผลิตภัณฑ์เนทีฟแล้ว ให้ติดตั้งคำสั่งและไฟล์สนับสนุนรันไทม์:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

ตัวติดตั้งแบบดั้งเดิมใช้ `/usr/local/bin`, `/usr/local/lib/sleela`
และ `/usr/local/share/sleela` โดยปริยาย โดยจะคงการกำหนดค่าเริ่มต้นระบบที่มีอยู่
และปฏิเสธการเขียนทับคำสั่งเดิมที่ไม่ได้ถูกติดตามไว้ การถอนการติดตั้ง
จะลบเฉพาะไฟล์ที่ตัวติดตั้งบันทึกไว้ โดยคงการกำหนดค่าและ
ข้อมูลรันไทม์ที่ไม่ได้ติดตามไว้

สำหรับการติดตั้งแบบเจาะจงเฉพาะแพ็กเกจหลัก — ไบนารี `sleela`
`sleelvac` และ `nordshrift` ไลบรารีมาตรฐาน โปรโตคอล และโมดูลสนับสนุน —
ให้ใช้ตัวติดตั้งแบบสะอาด:

```sh
make clean-install       # guided install of the main package only
```

ตัวติดตั้งนี้ให้ตัดสินใจหลักเพียงครั้งเดียว (ติดตั้งแพ็กเกจหลัก) จากนั้นเลือกขอบเขต
และเมื่อสำเร็จจะมีตัวเลือกเพิ่มเติมสำหรับโมดูลเสริมที่คัดสรรไว้ สำหรับ
เวิร์กโฟลว์การติดตั้งแบบโต้ตอบและตัวเลือกการติดตั้งทั้งหมด ดู
[การติดตั้งที่รวดเร็วและปลอดภัย](install/README.md)

## V. ภาษาและรันไทม์
ไวยากรณ์ที่เป็นทางการคือ [`SLEELA.syntax`](SLEELA.syntax) ปัจจุบัน repository ระบุเอกสารไวยากรณ์เวอร์ชัน **3.0** เวอร์ชันของคอมไพเลอร์/เครื่องมือและเวอร์ชันไวยากรณ์ของซอร์สเป็นคนละอย่างกัน ไวยากรณ์ 3.0 เพิ่ม**เครื่องหมายเอกสาร `pure` และ `gasoline`** (§30) — Preonems เปล่า ๆ ขึ้นกับบริบท ประกาศอย่างเดียว คู่หนึ่งที่มีความหมายเกือบเหมือนกัน โดยแต่ละตัวระบุสถานะของเอกสารด้วยความซื่อตรงในแบบของตน ไวยากรณ์ 3.0 รวบรวมพื้นผิวภาษา 1.0–2.9 ไว้เป็นฉบับบรรทัดฐานปัจจุบัน โดยคงเกณฑ์ขั้นต่ำเฉพาะแต่ละคุณลักษณะทั้งหมดไว้ พื้นผิว 2.9 ก่อนหน้าเพิ่ม**ฟิลด์ต่อเนื่อง `@mix`** (§28) — คลาสอาจประกาศฟิลด์อินสแตนซ์ชื่อเดียวกันหลายตัวเมื่อแต่ละตัวมี `@mix` ซึ่งเข้าใจเป็นกลุ่มที่จัดดัชนีหนึ่งกลุ่ม (`1,2,3,…` จากบนลงล่าง หรือกำหนดเองด้วย `@mix(1,2,3,...)` ซึ่งจำนวนดัชนีต้องตรงกับกลุ่ม) ยังคงสืบทอดตระกูลจำนวนเต็มกว้างแบบมีเครื่องหมาย/ไม่มีเครื่องหมายฐาน 2 คือ `int32`..`int16777216` (§24) และ `uint32`..`uint16777216` (§25) ต่อจาก `long`/`int64`/`int128` ก่อนหน้า (§21); **เอกสารผู้สนับสนุนคีย์เวิร์ด** (§22); และ **Preonems** — เครื่องหมายเอกสารแบบประกาศอย่างเดียว `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) และ `artifact` (§35 สถานะระดับนิติบัญญัติที่ไล่ระดับตามเวลาและพื้นที่) ซึ่งรวมเรียกว่า Preonems (§26.1) การแก้ความกำกวมของ constructor overload และการมอบหมาย `this(...)`/base-constructor อยู่นอกสัญญาไวยากรณ์ 1.8 ที่ผ่านการตรวจสอบอย่างชัดแจ้ง โปรดศึกษาไวยากรณ์และบันทึกการ implement ก่อนพึ่งพาคุณลักษณะใด ๆ หน่วยซอร์สทั้งเก้าของ SleelaDoc ตอนนี้ประกาศ `#sleela 3.0` เกตคอมไพเลอร์ร่วมยอมรับไวยากรณ์ซอร์ส 1.0–3.0 การ build API ใหม่สร้างด้วย `make api-docs`

repository มี implementation C/C++ แบบเนทีฟ เอกสารอ้างอิงการสร้าง SLVM โมดูลไลบรารีมาตรฐาน การทดสอบ ตัวอย่าง และการเชื่อมต่อกับแพลตฟอร์ม สถานะของ implementation และการตรวจสอบแตกต่างกันตามโมดูล ให้ดูข้อกำหนดของโมดูลและผล CI ปัจจุบัน แทนที่จะสมมติว่าสมบูรณ์เท่ากันทั้งหมด

## VI. หลักวิศวกรรม
- **หลักฐานเหนือคำกล่าวอ้าง:** แยกให้ชัดระหว่างพฤติกรรมที่ implement แล้ว ทดสอบแล้ว ทดลอง และวางแผนไว้
- **หนึ่งสัญญาต่อหนึ่งขอบเขต:** ระบุไวยากรณ์ API รูปแบบไฟล์ สมมติฐานของแพลตฟอร์ม และพฤติกรรมเมื่อล้มเหลวเป็นเอกสาร
- **พกพาได้โดยการออกแบบ:** แยกแบ็กเอนด์เฉพาะแพลตฟอร์มออกมา และทดสอบบน runner เนทีฟ
- **ล้มเหลวอย่างปลอดภัย:** ทดสอบอินพุตที่ผิดรูป ขีดจำกัดทรัพยากร ความล้มเหลวของโพรเซส และเส้นทางการล้างข้อมูล
- **release ที่สร้างซ้ำได้:** build ใหม่ให้สะอาด และเผยแพร่แฮชของ artifact พร้อมบันทึกการเผยแพร่

## VII. การ build และความสมบูรณ์ของแอนติไวรัส

แพ็กเกจแอนติไวรัส Sleelavirin ประกอบด้วยตัวสแกน SHA-256 แบบเนทีฟ
ตัวเชื่อมต่อ ClamAV ที่เลือกใช้ได้ manifest ความสมบูรณ์ของซอร์ส/ดัชนี
และ checksum ของแพ็กเกจตามแพลตฟอร์ม
build และทดสอบแพ็กเกจจากรากของ repository:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

manifest ความสมบูรณ์ถูกสร้างภายใต้ `antivirus/build/` ครอบคลุมอินพุตซอร์ส
และเอกสารที่ติดตามไว้ รวมถึงไฟล์ดัชนีลายเซ็นที่ป้องกันไว้ แต่
ไม่รวมเอาต์พุตที่สร้างขึ้นและแคช ClamAV ที่เปลี่ยนแปลงได้ สแนปช็อตฐานข้อมูล
ClamAV มี manifest ของตนเอง ZIP ของ release มี `INTEGRITY-MANIFEST.json`
`SHA256SUMS` และไฟล์ข้าง `.sha256` ให้ตรวจแฮชของไฟล์เก็บถาวรที่ดาวน์โหลดกับ
ค่าที่เชื่อถือได้ซึ่งได้มาโดยอิสระ: checksum ที่เก็บไว้ข้างไฟล์เก็บถาวร
ไม่ใช่ลายเซ็นของผู้เผยแพร่ ดู[คู่มือ Sleelavirin](antivirus/README.md)
[รายการตรวจสอบความปลอดภัย](antivirus/SECURITY.md) และ
[เวิร์กโฟลว์ build แอนติไวรัส](.github/workflows/antivirus-binaries.yml)

release จะไม่ถือว่าผ่านการตรวจสอบจนกว่างาน CI ที่เกี่ยวข้องจะผ่านบน
commit ของ release นั้นพอดี และ artifact ที่ได้ถูกตรวจสอบแล้ว


## VIII. ธีม Editor ของ SLeeLa — Vim และ Emacs

SLeeLa มีชุดแผนสีเข้มระดับมืออาชีพที่ประสานกันสำหรับ **Vim** และ **Emacs** พร้อมยูทิลิตีพาเลตต์ C/C++ แบบเนทีฟ แผน **American Professional** ดั้งเดิมยังเป็นค่าเริ่มต้น พาเลตต์คู่ใจอีกสิบชุดมีให้เลือกตามแรงบันดาลใจด้านการออกแบบทางวัฒนธรรมและสถาบันที่ต่างกัน พาเลตต์เหล่านี้เป็นพาเลตต์ออกแบบเชิงตีความ ไม่ใช่ตัวแทนอย่างเป็นทางการของชาติ วัฒนธรรม ศาล หรือสถาบันใด

### แผนสีทั้งสิบเอ็ดแบบและสีของแผนเหล่านั้น

| แผนสี | พื้นหลัง | ตัวอักษร | คีย์เวิร์ด | ชนิด | สตริง | คอมเมนต์ |
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

#### สีธงชาติ (พาเลตต์ต้นทาง)

แผนสีกรีกและตุรกีได้มาจาก**สีธงชาติอย่างเป็นทางการ**ของทั้งสองประเทศ โดยแสดงไว้ที่นี่พร้อมค่าเลขฐานสิบหกที่เผยแพร่อย่างแม่นยำ พื้นหลัง/สีเน้นของแผนสีข้างต้นเป็นการตีความที่ทำให้เข้มขึ้นและเหมาะกับ editor จากสีต้นทางเหล่านี้ — ตารางด้านล่างคือเอกสารอ้างอิง "สีตามที่ชาติเปิดเผย"

| ประเทศ | สี | Hex | พื้นฐานและความหมาย |
|---|---|---|---|
| **กรีซ** | น้ำเงิน | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | สีน้ำเงินของธงชาติ (กฎหมาย 851/1978) ท้องฟ้าและท้องทะเล ในตราศาสตร์เรียกว่า *azure* |
| **กรีซ** | ขาว | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | สีขาวของธงชาติ ความบริสุทธิ์ของการต่อสู้เพื่อเอกราช กางเขนหมายถึงศาสนาคริสต์นิกายออร์โธดอกซ์ตะวันออก |
| **ตุรกี (Türkiye)** | แดง (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | สีแดงของธงชาติ (กฎหมายธงชาติตุรกี) พื้นธง ตามธรรมเนียมคือเลือดของผู้พลีชีพและอธิปไตยของออตโตมัน |
| **ตุรกี (Türkiye)** | ขาว | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | สีขาวของธงชาติ พระจันทร์เสี้ยวและดาว — สันติภาพ ความบริสุทธิ์ และสัญลักษณ์มรดกเติร์ก/อิสลาม |

แผนสีเหล่านี้ยึดตัวอักษรของแต่ละ editor ไว้กับ**สีขาว**ของชาติ และยึดคีย์เวิร์ด เคอร์เซอร์ การเลือก และแถบสถานะไว้กับ**สีน้ำเงิน**ของชาติ (`#0D5EAF`) หรือ**สีแดง**ของชาติ (`#E30A17`) ที่แม่นยำ ดังนั้นสีที่สำคัญจึงเป็นสีของชาติอย่างแท้จริง พาเลตต์เหล่านี้เป็นพาเลตต์ออกแบบเชิงตีความ เอกสารอ้างอิงสีธงมีไว้เพื่อแสดงที่มา และไม่ใช่ตัวแทนอย่างเป็นทางการของรัฐใดรัฐหนึ่ง ความหมายของสีเป็นไปตามคำอธิบายสัญลักษณ์ประจำรัฐของกระทรวงการต่างประเทศแห่งสาธารณรัฐเฮลเลนิก และขนบของกฎหมายธงชาติตุรกี


### ตารางตัวอย่างสี

เซลล์พาเลตต์ด้านล่างใช้ **swatch แบบ SVG** เพื่อให้ GitHub เรนเดอร์สีจริง แทนที่จะเป็นเซลล์สี HTML ว่างเปล่า แต่ละแถบแสดงหกสีตามลำดับนี้: พื้นหลัง ตัวอักษร คีย์เวิร์ด ชนิด สตริง และคอมเมนต์ รหัส hex ที่แน่นอนแสดงไว้ข้างแต่ละ swatch ตามลำดับแผนสีเดียวกับตารางอ้างอิงข้างต้น

| แผนสี | สีพาเลตต์ที่เรนเดอร์ | รหัส hex (พื้นหลัง · ตัวอักษร · คีย์เวิร์ด · ชนิด · สตริง · คอมเมนต์) |
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

สีเน้นเพิ่มเติมใช้สำหรับฟังก์ชัน ค่าคงที่ การเลือก การวินิจฉัย และคำสั่งพรีโปรเซสเซอร์ พาเลตต์รัสเซียและสลาฟจงใจใช้สีเทาอมน้ำเงินเข้ม น้ำเงิน เขียวซาจ แดงทับทิม และทองโบราณที่ความเข้มปานกลาง เพื่อให้ดูเป็นมืออาชีพอย่างสงบกว่า

### ติดตั้งแผนสี Vim

จากรากของ repository:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

เพิ่มข้อความต่อไปนี้ใน `~/.vimrc` เพื่อเปิดการไฮไลต์ไวยากรณ์ SLeeLa และเลือกแผนสี:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

เลือกชื่อ colorscheme เหล่านี้เพียงหนึ่งชื่อ:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

ตัวอย่างเช่น ใช้ `colorscheme sleela-russian-slavic` สำหรับพาเลตต์รัสเซียและสลาฟระดับกลาง หรือ `colorscheme sleela-greek` / `colorscheme sleela-turkish` สำหรับพาเลตต์เฮลเลนิกและ Al Bayrak เริ่ม Vim ใหม่ หรือโหลดการกำหนดค่าของคุณด้วย `:source $MYVIMRC`

### ติดตั้งแผนสี Emacs

สร้างไดเรกทอรีธีมภายในเครื่อง แล้วคัดลอกไฟล์โหมดและธีมจากรากของ repository:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

เพิ่มข้อความนี้ใน `~/.emacs` หรือ `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

แทน `sleela-american` ด้วย `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` หรือ `sleela-sleela` เพื่อเลือกแผนสีอื่น หาก Emacs ถามว่าจะเชื่อถือธีมหรือไม่ ให้ตรวจไฟล์ธีมในเครื่องก่อนยอมรับ

### ดูตัวอย่างพาเลตต์ C/C++ แบบเนทีฟ

ยูทิลิตีพาเลตต์เนทีฟขนาดเล็กสามารถแสดงรายการแผนสี พิมพ์สีเทอร์มินัลของแผนสีนั้น และค้นหาสีรายตัวได้:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

เพื่อความเข้ากันได้ย้อนหลัง `--palette` และ `--hex COLOR` ที่ไม่มี `--theme` จะเลือก American Professional ผลการ build และทดสอบควรตรวจสอบบนระบบเป้าหมายและ CI การมีเอกสารของคำสั่งไม่ใช่หลักฐานในตัวเองว่าคำสั่งนั้นผ่าน

ดู[คู่มือการเชื่อมต่อ editor](linux-editors/README.md)สำหรับคำอธิบายพาเลตต์ฉบับเต็มและรายละเอียดของยูทิลิตี

## IX. การมีส่วนร่วม
1. อ่านดัชนีเอกสารและนโยบายการทดสอบ
2. ทำการเปลี่ยนแปลงที่มีขอบเขตชัดเจน พร้อมการทดสอบทั้งกรณีบวกและกรณีลบ
3. รันการทดสอบเฉพาะจุด แล้วจึงรันชุดทดสอบที่กว้างขึ้น
4. รายงานรายละเอียดระบบปฏิบัติการ/toolchain อย่าระบุว่าการตรวจที่ข้ามไปนั้นผ่าน

SLeeLa พัฒนาโดย **MEARVK LLC** เนื้อหาหน้าแรกในอดีตเก็บรักษาไว้ใน [`docs/README-LEGACY.md`](docs/README-LEGACY.md)

## X. AE6E66 — ความคาดหวังภาษีอีเมล Round Public

สำหรับโครงการ AE6E66 ความคาดหวังที่โครงการระบุไว้คือ **$300,000,000.00 USD ต่ออีเมล Round Public หนึ่งฉบับ** ในฐานะจำนวนภาษีที่เสนอให้เรียกเก็บซึ่งเกี่ยวข้องกับอีเมล Round Public ที่เข้าเกณฑ์แต่ละฉบับ

ข้อความนี้บันทึกเพียงความคาดหวังของโครงการเท่านั้น **ไม่ใช่**การประเมินภาษีตามกฎหมาย คำสั่งของรัฐบาล คำพิพากษาของศาล หรือภาระภาษีที่ได้รับการยืนยันโดยอิสระ การเรียกเก็บจริงใด ๆ ต้องมีหน่วยงานที่มีอำนาจตามกฎหมาย เขตอำนาจศาล นิยาม ขั้นตอน และการกำหนดที่มีอำนาจโดยรัฐบาลที่มีความสามารถ


## XI. รันไทม์ตัวอักษรและชุดอักขระ

เอกสารตัวอักษรของ SLeeLa และรันไทม์อักขระเนทีฟดูแลอยู่ใน alphabet/sleela/ และ lib/alphabet/native/ การทดสอบ C++17 แบบเนทีฟครอบคลุมการถอดรหัส/เข้ารหัส UTF-8 scalar แบบเข้มงวด ลายเซ็นความจุ C<n> และการเป็นสมาชิกของชุดอักขระที่มีขอบเขต:

    make -C lib/alphabet/native test

แค็ตตาล็อก UTF-4088 เชิงทดลองและตัวแปลงสำหรับค้นหาของ SLeeLa มีเอกสารอยู่ใน utf-4088/SLeeLa-ALPHABET-INTEGRATION.md ID ของ UTF-4088 เป็นเนมสเปซแยกต่างหาก ไม่ใช่ค่า Unicode scalar งานผูกกับคอมไพเลอร์และ SLVM ถูกติดตามแยกต่างหากจากฐานรากเนทีฟเหล่านี้อย่างชัดเจน

## XII. AutoCAD Creator™

[AutoCAD Creator™](autocad/creator/README.md) เป็นแอปพลิเคชัน CAD 2 มิติแบบโต้ตอบ
ที่เขียนด้วย SLeeLa ทั้งหมด ในขณะที่ AutoCAD Renderer ซึ่งเป็นพี่น้องกันเปลี่ยนแผนผัง
เป็นแบบวาดในครั้งเดียว Creator ให้คุณ**สร้างและแก้ไข**เอกสาร 2D AutoCAD แบบสด — เพิ่มผนัง
ห้อง มิติ และบันทึก แล้วย้าย เปลี่ยนป้าย เปลี่ยนเลเยอร์ ลบ และเลิกทำสิ่งที่คุณวางไว้ — และ**บันทึก**ผลลัพธ์เป็น
ไฟล์แบบวาด AutoCAD DXF จริง (ข้อความ R12 เปิดได้ใน AutoCAD / LibreCAD)

อินเทอร์เฟซแบบกราฟิกสร้างบน **SleelaUI™** ชุดเครื่องมือ UI ข้ามแพลตฟอร์มของ SLeeLa เอง
(ภายใต้ [`lib/user-interface`](lib/user-interface)) — ไม่ใช่ JavaFX — โดยยกหน้าต่างเนทีฟจริง
ขึ้นบน X11 / Cocoa / Win32 การออกแบบยึดกฎข้อเดียว
**"SleelaUI นำเสนอ; SLeeLa ตัดสิน"**: GUI ไม่คำนวณรูปทรงเรขาคณิตและไม่เขียน
DXF เครื่องมือทุกตัวส่งเจตนา สร้าง / แก้ไข / ลบ / เลิกทำ / บันทึก ไปยัง
[`SLCadCreator`](lib/autocad/SLCadCreator.sleela) ที่เป็นผู้มีอำนาจ ซึ่งเป็นเจ้าของ
เอกสารที่แก้ไขได้และประวัติคำสั่งที่ย้อนกลับได้ ขณะที่แคนวาสเพียงแสดงตัวอย่าง
รูปทรงที่ Creator ถืออยู่

เอนจินแก้ไขที่นำกลับมาใช้ซ้ำได้อยู่ใน [`lib/autocad`](lib/autocad) —
[`SLCadEntity`](lib/autocad/SLCadEntity.sleela)
[`SLCadDocument`](lib/autocad/SLCadDocument.sleela)
[`SLCadCommand`](lib/autocad/SLCadCommand.sleela) และ
[`SLCadCreator`](lib/autocad/SLCadCreator.sleela) — และทำงานตั้งแต่ต้นจนจบแบบ headless ได้
เชลล์ GUI และเดโมอยู่ภายใต้ [`autocad/creator`](autocad/creator):

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

บนเครื่องที่มีจอแสดงผล จะเปิดหน้าต่างแก้ไขแบบสด บนโฮสต์ headless จะสร้าง
แบบวาดในหน่วยความจำ พิมพ์รายงานพร้อม DXF และออกอย่างเรียบร้อย


## XIII. ลำดับแผนที่อินพุต UTF-4088

แค็ตตาล็อก UTF-4088 เฉพาะทางกำหนดการท่องแผนที่อินพุตเชิงพื้นที่และการวนซ้ำแรงกดเชิงนามธรรมไว้ใน [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md) นโยบายดัชนีหัวเรื่อง National Lien อยู่ใน [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md) พิกัดแผนที่ ระดับแรงกด (1–50) ID แค็ตตาล็อกถาวร และลำดับตัวอักษรเป็นฟิลด์ที่เป็นอิสระต่อกัน ห้ามนำฟิลด์หนึ่งไปแทนอีกฟิลด์หนึ่งโดยไม่แจ้ง แรงกดคือระดับการวนซ้ำเชิงอัลกอริทึม ไม่ใช่ปริมาณทางกายภาพ


## XIV. ดัชนีแพ็กเกจที่เพิ่งขยายและล่าสุด

ดัชนีที่ต่อท้ายนี้เน้นแพ็กเกจที่เพิ่งขยาย ไลบรารีโดเมน และ
คอลเลกชันซอร์ส รายการจัดกลุ่มตามตัวอักษรตามชื่อแพ็กเกจหรือหัวข้อ
คำอธิบายระบุที่ตั้งใน repository สถานะการ implement และการตรวจสอบ
ควรตรวจในเอกสารและการทดสอบของแต่ละแพ็กเกจ

### A–C

- **Advantage** — `lib/advantage/` และ `advantage/`: การทำ advantage ให้เงินปันผล
  ที่เป็นเอกลักษณ์เทียบกับแบบจำลองในรูป Sequitur onion แบบหลายชั้น โดยมี
  โฟลเดอร์ย่อยห้าสาขาวิชา — `math/` (การขยายกำไรร่วมสูงสุด การเจรจาต่อรองแบบ Nash
  newsvendor ทวิภาคี) `physics/` (เมตริก/จีโอเดสิก/การสัมผัส/SDE ของสนามความไว้วางใจ
  ซึ่งเป็นการเปรียบเทียบเชิงแบบจำลองที่ระบุไว้) `integration/` (ช่องการบูรณาการแนวดิ่ง
  และคะแนนความซับซ้อน) `grading/` (ตัวให้เกรดวงจรความซับซ้อน A–D
  สำหรับโมดูลและชุดโมดูล) และ `output/` (การส่งออกไปยังเทอร์มินัล / CommonRails /
  ซ็อกเก็ต) ผู้ใช้ C/C++ แบบเนทีฟ (`advantage/`) เชื่อมแบบจำลองเข้ากับ
  ตัวอย่างการค้าที่ใช้งานจริง ดู [`lib/advantage/README.md`](lib/advantage/README.md)
  และ [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md)
- **Alphabet และ UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` และ
  `utf-4088/`: แค็ตตาล็อกตัวอักษร การรองรับชุดอักขระเนทีฟ ลำดับแผนที่อินพุต
  และเอกสารการบูรณาการ UTF-4088
- **AutoCAD Creator** — `autocad/creator/` และ `lib/autocad/`: เวิร์กโฟลว์วาดภาพ
  2 มิติแบบโต้ตอบ และคอมโพเนนต์เอกสาร/การแก้ไข CAD ที่นำกลับมาใช้ซ้ำได้
- **Ballum** — การควบคุมการแสดงผลระดับพิกเซล: set/get ต่อพิกเซล blit ตามพื้นที่
  ความสว่าง/ความอุ่น/แกมมาของทั้งจอ อินพุตตัวชี้/คีย์บอร์ด และ
  **พื้นผิว framebuffer เต็มจอ** สำหรับการควบคุมต่อพิกเซลแบบเบ็ดเสร็จ เป็นเจ้าของ และรวดเร็ว
  (หน้าต่าง override-redirect และการแสดงผลด้วย `XPutImage`/MIT-SHM เดียวบน Linux/X11)
  แกนเนทีฟอยู่ใต้ `ballum/` พร้อม facade ของ SLeeLa ใน `lib/ballum/` และสะพาน VM
  ใต้ `impl/` ดู [`ballum/README.md`](ballum/README.md) และตัวอย่างที่รันได้
  [`ballum/example/`](ballum/example/)
- **CDMA** — `cdma/` และ `lib/cdma/`: เอกสารเทคนิคที่เกี่ยวข้องกับ CDMA และ
  งานไลบรารีซอร์ส SLeeLa
- **Character** — `lib/character/`: โปรไฟล์ตัวละครและสถิติตัวละครที่นำกลับมาใช้ซ้ำได้
  รวมถึงตัวอย่างโปรไฟล์ชีวประวัติ
- **Coorenagraph** — `lib/coorenagraph/`: การให้เหตุผลเชิงจริยธรรม คำศัพท์ทางศีลธรรม
  การพิจารณาไตร่ตรอง และแนวคิดที่เกี่ยวข้อง ดู README ของแพ็กเกจและไฟล์ซอร์ส

### D–G

- **แบบจำลองซอร์สด้านธรรมาภิบาล** — [lib/governance](lib/governance/README.md) กำหนดรูปทรงซอร์ส SLeeLa แบบประกาศสำหรับข้อเสนอ บัตรลงคะแนนแบบกระดาษนิรนาม และผลรวมสะสม คำอธิบายประกอบ @vote ที่เสนอและขั้นตอนแบบกระดาษมีเอกสารอยู่ใน [registry/DEMOCRATIC-VOTING-SYNTAX.md](registry/DEMOCRATIC-VOTING-SYNTAX.md) คอมไพเลอร์ปัจจุบันถือว่าเป็นเมทาดาทาคอมเมนต์ที่ไม่มีผล

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` ไปจนถึงไดเรกทอรีแพ็กเกจ HTTP
  ตามเวอร์ชันที่มี: ข้อกำหนดโปรโตคอลและ implementation เฉพาะเวอร์ชัน
  ดูเอกสารของแต่ละเวอร์ชันเพื่อทราบขอบเขตที่แน่ชัดและสถานะ
  การตรวจสอบ
- **International Criminal Court** — `lib/international-criminal-court/` และ
  `INTERNATIONAL.CRIMINAL.COURTS.md`: ซอร์สและเอกสารเฉพาะโดเมน
  ที่เกี่ยวข้อง

### O–S

- **OFDMA** — `ofdma/`: เอกสารเทคนิคการเข้าถึงหลายทางแบบแบ่งความถี่
  ตั้งฉาก และทรัพยากรพัฒนา C/C++
- **ปรัชญาและโมดูล Conversation Mask** — [`philosophy/`](philosophy/README.md)
  มีฐานรากแบบ C/C++ เนทีฟ และ [`philosophy/module/`](philosophy/module/README.md)
  จัดชั้นโมดูลถัดไปเป็นสามส่วนที่ชัดเจน:
  [`data/`](philosophy/module/data/README.md) สำหรับข้อสังเกตที่กำหนดชนิดและลงวันที่
  การสิ้นสุดเหตุการณ์ การตั้งค่าครึ่งชีวิต แหล่งที่มา และความยินยอม [`helper/`](philosophy/module/helper/README.md)
  สำหรับการตรวจสอบแบบกำหนดแน่นอน การเสื่อมของความเกี่ยวข้อง การเปรียบเทียบที่เข้ากันได้
  และการนำเสนอที่กรองตามความยินยอม และ [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md)
  สำหรับอินเทอร์เฟซสนทนาที่ให้การสนับสนุนแต่ไม่ใช่ทางคลินิก handle ที่หันหน้าเข้าหา SLeeLa
  และ Tree ที่ประกอบขึ้นอยู่ใน [`lib/philosophy/`](lib/philosophy/README.md)
  ควบคู่กับแพ็กเกจปรัชญาที่กว้างกว่า โมดูลนี้ใช้อินพุตที่ระบุชัดและให้ความยินยอมเป็นอันดับแรก:
  ไม่เฝ้าติดตามผู้คนหรือสื่อโดยไม่แจ้ง ไม่วินิจฉัย ไม่อนุมานเจตนาส่วนตัว
  ไม่ให้คะแนนคุณค่าของมนุษย์ และไม่แนะนำการเปลี่ยนแปลงยา ลิงก์เหตุการณ์ของมัน
  รักษาแหล่งที่มาและการนำทาง ไม่ใช่การยืนยันอิสระ
- **Synchro** — การบูรณาการรันไทม์/คอมไพเลอร์แบบเนทีฟ และฟังก์ชันที่เกี่ยวกับการส่งแพ็กเก็ตและ
  การวัด ดูซอร์ส `impl/` ที่เกี่ยวข้องและ
  implementation ของคอมไพเลอร์
- **SleelaUI / terminal UI** — `lib/user-interface/` และ
  โครงการ `sleela-terminal`: การบูรณาการ UI ของ SLeeLa และคอมโพเนนต์
  การโต้ตอบที่เน้นเทอร์มินัล

### U–W

- **United Nations** — `united-nations/`: เอกสารใน repository เฉพาะโดเมน
- **Wi-Fi 4, Wi-Fi 5 และ Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`,
  `wi-fi-6/` และไดเรกทอรีซอร์ส `lib/wifi/` ที่เกี่ยวข้อง: เอกสารเครือข่ายไร้สาย
  และทรัพยากรการกำหนดค่าเฉพาะแต่ละรุ่น

### การดูแลรักษาดัชนีแพ็กเกจ

เมื่อเพิ่มแพ็กเกจหรือขยายแพ็กเกจอย่างมีนัยสำคัญ ให้เพิ่มรายการของแพ็กเกจนั้นที่นี่
ในกลุ่มตามตัวอักษรที่เหมาะสม ลิงก์ไปยังไดเรกทอรีและ README ที่เป็นหลักของแพ็กเกจ
และอธิบายเฉพาะความสามารถที่ซอร์สที่เช็กอินไว้รองรับ ให้ดัชนีนี้เพิ่มเติมได้อย่างเดียว:
อย่าลบหรือเขียนทับส่วน README ที่มีอยู่
เมื่ออัปเดตดัชนีแพ็กเกจ


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum


![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

US Democratic and US Policy.

---

## XVI. ภาพลักษณ์แบรนด์วิทยาศาสตร์แห่งชาติ

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. แค็ตตาล็อกเชิงทดลอง UTF-4088 หนึ่งล้านรายการ

[ตัวสร้างแค็ตตาล็อก UTF-4088](utf-4088/tools/generate_catalogue.py) สร้างแค็ตตาล็อกสัญลักษณ์เชิงทดลองจำนวน 1,048,576 รายการอย่างกำหนดแน่นอน รูปแบบกะทัดรัดที่มีเอกสารอธิบายไว้ใน [utf-4088/FORMAT.md](utf-4088/FORMAT.md) ส่วนสัญญาการสร้างและการตรวจสอบอยู่ใน [utf-4088/GENERATION-RESULTS.md](utf-4088/GENERATION-RESULTS.md)

รันตัวสร้างด้วย Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

ไบนารีที่สร้างขึ้นมีขนาด 16,777,244 ไบต์ และออกแบบให้ต่ำกว่า 20,000,000 ไบต์ การทดสอบหน่วยและเวิร์กโฟลว์ GitHub Actions ตรวจสอบจำนวนรายการ ความไม่ซ้ำของกลิฟ ขนาด และ SHA-256 การรันที่สำเร็จจะเผยแพร่ไบนารีและสรุปเป็น artifact ของเวิร์กโฟลว์ `utf4088-million-catalogue` ไบนารีที่สร้างขึ้นถูกเก็บไว้นอกประวัติ Git โดยเจตนา UTF-4088 ยังคงเป็นเชิงทดลองและไม่เข้ากันกับ Unicode

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. ส่วนเพิ่มล่าสุด — ค่าคงที่ Pi, การเริ่มต้น Modium/Registry, Math ของ sleelactl และ `@native`

ส่วนนี้บันทึกคุณลักษณะที่เพิ่มระหว่างงานในเดือน 2026-11 โดยเรียงกลุ่มใหม่สุดก่อน ไฟล์ซอร์สที่ลิงก์และสถานะการตรวจสอบของไฟล์เหล่านั้นเป็นข้อมูลที่เชื่อถือได้ การเปลี่ยนแปลงที่อยู่ระหว่างการตรวจทานจะไม่ถูกอธิบายว่าถูกรวมแล้ว จนกว่าจะถึง `master`

### ค่าคงที่แค็ตตาล็อก Pi 100,000,000 หลัก

repository มีค่า π **หนึ่งร้อยล้านหลัก**ที่ตรวจสอบแล้วที่ [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (ทศนิยมที่ถูกรูปแบบ `3.14159…`) ควบคู่กับค่า 57,344 หลักที่ [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits) ทั้งสองค่าได้รับการตรวจสอบอย่างอิสระว่าเป็น π จริง — ดู [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md) ซึ่งบันทึกการตรวจสอบ (คำนำหน้าตามตำรา จุด Feynman ที่ตำแหน่งทศนิยม 762 หลักที่หนึ่งล้านอันโด่งดัง ความตรงกันกับ `lib/math/SLPiReference.sleela` และการวิเคราะห์ความถี่ของหลักทั้งไฟล์) และสคริปต์สร้างซ้ำแบบครั้งเดียวจบ

### Modium™ / Registry™ โหลดค่าคงที่ Pi ตอนเริ่มระบบย่อย

ที่จุดเข้าของระบบย่อยโดยเฉพาะ — `sleela registry` และ `sleela modium` (และการรันครั้งแรกของตัวติดตั้ง) **ไม่ใช่**ทุกครั้งที่ `sleela run` — ระบบย่อย Modium และ Registry จะโหลดค่าคงที่แค็ตตาล็อก π ขนาดใหญ่ บนเทอร์มินัลแบบโต้ตอบ ระบบจะพิมพ์บรรทัด `Loading Modium™` / `Loading Registry™` สีแดงชั่วคราว แล้วลบทิ้งในที่เดิมก่อนที่เอาต์พุตปกติจะเริ่ม เอฟเฟกต์นี้จำกัดเฉพาะ TTY อย่างเคร่งครัด ดังนั้นเอาต์พุตที่ส่งผ่านไปป์และเอาต์พุตของการทดสอบจึงเหมือนเดิมทุกไบต์ ตัวระบุตำแหน่ง (ไม่ใช่ไบต์ข้อมูล) ถูกหยอดเป็นค่าเริ่มต้นของคีย์ `Math` ใน registry คือ `piReferenceLargePath` และ `piReferenceLargeDigits` ทั้งในแบบจำลอง SLeeLa ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) และตัวหยอดข้อมูลเนทีฟ ([`registry/sleela_registry.c`](registry/sleela_registry.c)) โดย `lib/math/SLPiReference.sleela` เปิดเผย `largePath()` / `largeDigits()` ค่าคงที่ π ถูกติดตั้งลงในตำแหน่ง SLeeLa ของระบบปฏิบัติการ (`<data>/pi/`) โดย**ทั้ง**ตัวติดตั้ง `make` มาตรฐานและตัวติดตั้งแบบ "beautiful" แบบมีผู้แนะนำ รายละเอียด: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md)

### `sleelactl` — ตัวติดตั้งแบบ beautiful และหัวเรื่อง `math` ที่ใช้เนทีฟรองรับ

- ตัวติดตั้งแบบมีผู้แนะนำ ("beautiful") ตอนนี้ติดตั้ง `sleelactl` (control plane ของ SLeeLa) และ registry โมดูลของมัน ให้ตรงกับตัวติดตั้ง `make` ดู [`bin/SLEELACTL.md`](bin/SLEELACTL.md)
- `sleelactl` มีลักษณะ **`subject`** ใหม่สำหรับไลบรารีหัวเรื่อง C/C++ แบบเนทีฟที่คอมไพล์รวมอยู่ในรันไทม์ และลงทะเบียนตัวแรก คือ **`math`** `sleelactl run math` ทดสอบแบ็กเอนด์เนทีฟ `__NativeMath` (`impl/subjects/math/math_native.cpp`) ตั้งแต่ต้นจนจบผ่านเดโมอ้างอิงของหัวเรื่อง `sleelactl info math` อธิบายวิธีเข้าถึง (`import math;`) ลักษณะ subject มีเพียง `run info` — จงใจไม่มีคำกริยา `check` เพราะ `sleela check` ไม่โหลดหัวเรื่องเนทีฟที่ลงทะเบียนผ่าน SHEET

### `@native`, Native Drive และการทำงานหลายภาษา

คำอธิบายประกอบเมธอดใหม่ **`@native`** ร้องขอการคอมไพล์เนทีฟล่วงหน้า (ahead-of-time) ของเมธอดที่ทำงานหนัก คำสั่งใหม่ `sleela aot` ค้นหาเมธอด `@native` ตรวจสอบว่าแต่ละเมธอดอยู่ใน*ชุดย่อยที่ออฟโหลดได้*ที่รองรับ และส่งออก C/C++ — โดย**ล้มเหลวแบบปิด** (ปฏิเสธพร้อมเหตุผลที่แม่นยำ) เมื่อพบโครงสร้างที่ยังแปลไม่ได้ เพื่อไม่ให้เกิดเมธอดที่แปลครึ่งเดียว

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

`@native` ยังคงเป็นเพียงคำขอให้ใช้การจัดการแบบ AOT โดยโครงสร้างที่ไม่รองรับจะถูกปฏิเสธก่อนการส่งออก [Native Drive](impl/native-drive/README.md) รองรับการ build ไฟล์ปฏิบัติการและไลบรารีแชร์แบบหลายยูนิต C/C++ ผ่านคอมไพเลอร์โฮสต์ (รวมถึง MSVC) เอาต์พุต AOT ของ SLeeLa ที่รองรับ การตรวจสอบสัญลักษณ์เทียบกับ ABI ของตัวโหลดเนทีฟ การรันโพรเซสแบบไม่ผ่านเชลล์ การจัดการ stdin/stdout/stderr การต่อไปป์หลายขั้น และการมอบหมาย XClass ให้ตัวโหลดระดับ production `sleelactl` เปิดเผยคำสั่ง build/run ของมัน และกำหนดเส้นทางการตรวจสอบ XClass รวมถึงการเรนเดอร์/ติดตั้งยูนิต systemd แบบชัดแจ้งผ่าน `inspect native-drive` VM เป็นเจ้าของตาราง handle ของโมดูลเนทีฟที่มีขอบเขต พร้อมการอนุญาตจากโฮสต์ที่จำเป็นและ handle ที่ตรวจสอบรุ่น ไวยากรณ์ 3.0 ตอนนี้เพิ่ม `nativeOpen`, `nativeCall` และ `nativeClose`: การจัดส่ง bytecode แบบ append-only ข้าม ABI ที่มีเวอร์ชันแยกต่างหากสำหรับ null/bool/int64/double/UTF-8 ขณะที่อาร์เรย์ struct และค่าประกอบอื่น ๆ ที่ VM เป็นเจ้าของยังอยู่ภายในอินเทอร์พรีเตอร์ และอาจใช้ bytecode fallback ชื่อเดียวกัน ปลั๊กอินอาจเลือกรับตารางเรียกกลับของโฮสต์ที่มีเวอร์ชันเพื่อเรียกฟังก์ชัน SLeeLa ที่ตรงตัวผ่านการทำงานของ VM ซ้อน โดยมีการยึด GC root ของสแตกที่ถูกพักไว้ รายการอนุญาตชื่อ callback ของโฮสต์อย่างชัดแจ้ง และปฏิเสธการเรียก API เนทีฟแบบเรียกซ้ำ โค้ดเนทีฟยังคงเป็นโค้ดในโพรเซสที่เชื่อถือได้ ไม่ใช่ sandbox คำอธิบายประกอบนโยบายโพรเซสในซอร์สมีสคีมาที่มีเวอร์ชัน แต่ยังไม่ถูกแยกวิเคราะห์/บังคับใช้ รายละเอียด ขอบเขตความปลอดภัย และบัญชีเจ็ด workstream: [Native Drive](impl/native-drive/README.md) และ [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md)


## XIX. ฉบับ README นานาชาติ

SLeeLa จัดให้มีจุดเริ่มต้น README เฉพาะประเทศสำหรับ 50 ตลาดในฉบับนานาชาติ แต่ละประเทศมีหนึ่งไฟล์ชื่อ `README.<CC>.md` ในกรณีที่เกี่ยวข้องกับหลายภาษา จะจัดกลุ่มไว้ในไฟล์ของประเทศนั้น

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

**นโยบายการแปล:** ไฟล์เหล่านี้เป็นจุดเริ่มต้นประจำประเทศที่แปลเป็นภาษาท้องถิ่นในเบื้องต้น บทนำและหัวข้อภาษาได้รับการแปลเป็นภาษาท้องถิ่น ขณะที่เอกสารอ้างอิงทางเทคนิคฉบับสมบูรณ์ยังคงอยู่ใน [README.md](README.md) ฉบับภาษาอังกฤษที่เป็นต้นฉบับหลัก ห้ามอธิบายไฟล์ประจำประเทศว่าเป็นคำแปลฉบับสมบูรณ์ จนกว่าทุกส่วนจะได้รับการแปล ตรวจสอบตัวระบุเชิงเทคนิคและลิงก์แล้ว และฉบับนั้นผ่านการตรวจทานทางภาษา การครอบคลุมภาษาในรอบแรกนี้เป็นเพียงตัวแทน และไม่ใช่การอ้างว่าทุกภาษาที่รัฐธรรมนูญรับรอง ภาษาท้องถิ่น ภาษาของชนพื้นเมือง หรือภาษามือ ได้รับการแปลครบถ้วนแล้ว

## ภาคผนวก — แนวทางประจำภูมิภาคที่เก็บรักษาไว้

ภาคผนวกนี้เก็บรักษาแนวทางปฏิบัติจากคู่มือภาษาไทยฉบับสั้นก่อนหน้า ซึ่งไม่มีเป็นหัวข้อต่างหากใน README ต้นฉบับ หากมีความไม่สอดคล้องกัน ให้ถือ [README.md](README.md) ฉบับภาษาอังกฤษที่เป็นต้นฉบับหลักเป็นเกณฑ์

### สถาปัตยกรรมคอมไพเลอร์และขั้นตอนการแปลโปรแกรม

SLeeLa แยกภาษาต้นฉบับ ส่วน frontend ของคอมไพเลอร์ intermediate representation การสร้างโค้ด และรันไทม์ออกจากกัน รุ่นของคอมไพเลอร์ รุ่นของไวยากรณ์ และรุ่นของ VM/ISA เป็นข้อมูลคนละส่วน การมีคุณลักษณะอยู่ในข้อกำหนดไม่ได้ยืนยันว่าคุณลักษณะนั้นได้รับการพัฒนาและตรวจสอบครบทุกเป้าหมาย

ขั้นตอนทั่วไปประกอบด้วย:

1. **ซอร์ส:** ไฟล์ UTF-8 นามสกุล `.sleela` พร้อม version pragma และการประกาศ
2. **การวิเคราะห์ศัพท์และไวยากรณ์ (lexing และ parsing):** แปลงข้อความเป็นโทเค็นและโครงสร้างไวยากรณ์
3. **การวิเคราะห์ความหมาย:** ตรวจสอบชื่อ สัญลักษณ์ ชนิด และกฎของภาษา
4. **Intermediate representation:** รูปแบบโปรแกรมที่มีโครงสร้างสำหรับการลดระดับขั้นต่อไป
5. **การสร้างโค้ด:** สร้างเอาต์พุตสำหรับ VM หรือเป้าหมายระหว่างกลางที่รองรับ
6. **รันไทม์:** ประมวลผลโปรแกรมที่คอมไพล์แล้วด้วยกลไกหน่วยความจำและการควบคุมการทำงานที่มีอยู่

เอกสารข้อกำหนดหลักคือ [SLEELA.syntax](SLEELA.syntax) ส่วนรูปแบบซอร์สและวงจรชีวิตไฟล์ดูที่[เอกสารซอร์ส](markdown/SOURCE.md)และ[เอกสารคอมไพเลอร์](markdown/COMPILER.md)

### เป้าหมาย โปรไฟล์ และขอบเขตการทำงาน

SLeeLa แยกซอร์สโค้ดออกจากเป้าหมายการ build และการทำงาน กลุ่มเป้าหมายที่มีเอกสารประกอบด้วย `SLVM`, `SLJVM` และ `SLIR` ส่วน `NATIVE` เป็นเป้าหมายที่มีขอบเขตและมีเงื่อนไข จึงไม่ควรสมมติว่ามีบนทุกแพลตฟอร์ม โปรไฟล์ `BASIC_COMPLETE` และ `ADVANCED_TOTAL` ระบุขอบเขตที่ต่างกัน ให้ตรวจเป้าหมาย โปรไฟล์ และการรองรับแพลตฟอร์มจริงของแต่ละ build

ข้อมูลอ้างอิง (reference) ระบุไฟล์ โมดูล หรือทรัพยากร แต่ไม่ได้ให้สิทธิ์ในการรันสิ่งนั้นในตัวเอง เมื่อโปรแกรมใช้ไฟล์ ไลบรารีภายนอก native binding หรือทรัพยากรของระบบ ต้องกำหนดขอบเขตการอนุญาตและความรับผิดชอบในการทำงานให้ชัดเจน คำอธิบายในเอกสารเพียงอย่างเดียวไม่ใช่หลักฐานของการรับประกันความปลอดภัยขณะรันไทม์ ให้ตรวจ implementation และการทดสอบที่เกี่ยวข้อง

### ชนิดข้อมูลและแบบจำลองออบเจ็กต์

ชนิดข้อมูลหลักประกอบด้วย `int`, `double`, `boolean`, `String`, `void`, ชนิด `struct` ที่ประกาศเอง และลายเซ็นชนิดอ้างอิงที่รองรับ ตามเอกสารปัจจุบัน `int`, `long` และ `int64` เป็นจำนวนเต็มมีเครื่องหมาย 64 บิต ส่วน `int128` ยอมรับเป็นชนิดจำนวนเต็มได้ แต่ implementation ที่ระบุยังอิง 64 บิต จึงไม่ควรถือว่าเป็นเลขคณิต 128 บิตอย่างสมบูรณ์

`struct` คือ aggregate ที่มีฟิลด์ระบุชื่อ struct handle ที่ VM จัดการไม่ใช่พอยน์เตอร์ดิบที่เปิดเผยโดยตรง การกำหนดค่าและการส่งพารามิเตอร์ใช้อินสแตนซ์เดียวกัน และความเท่ากันอิงตัวตน (identity) สำหรับการแลกเปลี่ยนข้อมูลแบบพกพา ให้ใช้ตัวช่วยการทำซีเรียลไลซ์ที่ระบุ เช่น `structPack(instance)` และ `structUnpack(TypeName, json)`; handle ภายใน VM ไม่ใช่ข้อมูลเครือข่ายที่ส่งต่อได้โดยตรง

คลาสประกอบด้วยฟิลด์และเมธอด ในแบบจำลองการทำงานปัจจุบัน ฟิลด์เป็นสถานะคลาสที่ใช้ร่วมกัน ส่วนตัวแปรภายในเมธอดเป็นของแต่ละเธรด การสืบทอดคลาส คลาสซ้อน และ generics ขึ้นกับเกตเวอร์ชันไวยากรณ์ ดูรายละเอียดที่ [International Language Reference](LANGUAGE.INTERNATIONAL.3.0.md)

### Control flow และการกำหนดเวลาของลูป

ภาษามีไวยากรณ์สำหรับเงื่อนไข ลูป การเรียกเมธอด และการเรียกซ้ำ คำอธิบายประกอบ `@loopDelay` และ `@loopBudget` ใช้กำหนดเวลาหยุดหรืองบเวลาของลูป โดยเขียนไว้ก่อนลูป และจะถูกลดระดับเป็นคำสั่ง VM `OP_SLEEP` (ISA 157) ฟังก์ชันในตัว `sleep(ms)` ใช้คำสั่งเดียวกัน

```sleela
@loopDelay(ms = 250)
while (running) {
    processNext();
}
```

ตัวอย่างนี้แสดงเฉพาะรูปแบบไวยากรณ์ โปรแกรมจริงต้องประกาศตัวระบุและคลาสที่ห่อหุ้มให้ครบ ดู [SLEELA.syntax §32](SLEELA.syntax) และ [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md)

### ระดับความพร้อมของโมดูลไลบรารี

repository มีโมดูลที่มีระดับความพร้อมแตกต่างกัน การมีชื่อโมดูลในแค็ตตาล็อกไม่ได้แปลว่าพร้อมใช้ใน production ควรตรวจสอบ README ของโมดูล ความครอบคลุมของการทดสอบ ข้อจำกัดของแพลตฟอร์ม และเอกสารการตรวจสอบก่อนนำไปใช้ การกล่าวถึงฟิลด์ความไว้วางใจในเชิงฟิสิกส์ของ `lib/advantage` เป็นการเปรียบเทียบเชิงแบบจำลอง ไม่ใช่ข้ออ้างว่าเป็นกฎฟิสิกส์ ส่วน `sleela docs --generate [dir]` ของ `lib/sleeladoc` สร้างคลังข้อความสำหรับใช้งานแบบออฟไลน์

### การเชื่อมต่อ C/C++ และขอบเขตเนทีฟ

การเชื่อมต่อ C/C++ ต้องระบุ ABI, calling convention, ความเป็นเจ้าของหน่วยความจำ, data layout, การจัดการข้อผิดพลาด และ dependency ของแพลตฟอร์มอย่างชัดเจน ให้ถือโค้ดเนทีฟเป็นขอบเขตความเชื่อถือแยกต่างหาก ข้อมูลอ้างอิงหรือการประกาศระบุทรัพยากรได้ แต่ไม่ได้ให้สิทธิ์ในการรัน ดู[การ implement แบบเนทีฟ](impl/README.md) [เมทริกซ์แพลตฟอร์ม](docs/PLATFORM-SUPPORT.md) และ[มาตรฐานการเปรียบเทียบ](comparison/README.md) อย่าสมมติว่า dependency และคำสั่งเดียวกันใช้ได้เหมือนกันบน Linux, macOS และ Windows

### วงจรชีวิตของทรัพยากรและการแก้ปัญหา

ให้บันทึกความเป็นเจ้าของ อายุการใช้งาน และความรับผิดชอบด้านการซิงโครไนซ์ของทรัพยากรที่ใช้ร่วมกันระหว่างฟังก์ชันหรือเธรด แยกตัวแปรภายใน สถานะออบเจ็กต์ที่ใช้ร่วมกัน VM handle และข้อมูลที่ซีเรียลไลซ์ได้ออกจากกัน VM handle ภายในไม่ใช่รูปแบบไฟล์หรือข้อความเครือข่ายที่เสถียร ให้ใช้อินเทอร์เฟซซีเรียลไลซ์ที่โมดูลกำหนด และทดสอบค่าที่ไม่ถูกต้อง ขาดหาย และค่าที่ขอบเขต

เมื่อแก้ปัญหา ให้บันทึก commit ระบบปฏิบัติการ สถาปัตยกรรม และ toolchain build เป้าหมายที่เล็กที่สุดที่เกี่ยวข้องก่อน เก็บเอาต์พุตข้อผิดพลาดทั้งหมด แยกข้อผิดพลาดของคอมไพเลอร์ออกจากข้อผิดพลาดขณะรันไทม์ และสร้างตัวอย่างขั้นต่ำที่ทำซ้ำได้ การ build ที่สำเร็จไม่ได้พิสูจน์ว่าพฤติกรรมขณะรันไทม์ถูกต้อง

รายงานการตรวจสอบควรระบุ commit ที่แน่นอน ระบบปฏิบัติการ/สถาปัตยกรรม เวอร์ชันคอมไพเลอร์ คำสั่ง build/ทดสอบแบบเต็ม จำนวนการทดสอบที่ผ่าน/ล้มเหลว/ข้าม และข้อจำกัดที่ทราบ การทดสอบที่ข้ามหรือไม่ได้รันต้องไม่นับว่าผ่าน

### ข้อสังเกตเกี่ยวกับการอ้างอิงและที่อยู่

การอ้างถึงหัวข้อต่าง ๆ ใน README เช่น CSPML, Character, Governance, Synchro, SleelaUI และ United Nations อธิบายเนื้อหาของ repository และไม่ใช่การรับรองอย่างเป็นทางการจากหน่วยงานหรือสถาบันใด

การมีที่อยู่หรือรูปภาพในส่วน **Bitcoin Conjegeum** (XV) ไม่ใช่คำแนะนำทางการเงิน การยืนยันความเป็นเจ้าของ หรือคำสั่งให้โอนเงิน โปรดตรวจสอบข้อมูลโดยอิสระก่อนใช้

**สถานะการแปล:** หัวข้อ I–XIX ได้รับการแปลเป็นภาษาไทยแล้ว ตัวระบุเชิงเทคนิค คำสั่ง ตัวอย่างโค้ด ชื่อไฟล์ ลิงก์ และที่อยู่ยังคงเดิม แนะนำให้ผู้ใช้ภาษาไทยเป็นภาษาแม่ตรวจทานทางภาษาในขั้นสุดท้าย
