# SLeeLa 3.0 — ประเทศไทย (TH)

## ภาษาไทย

SLeeLa เป็นภาษาโปรแกรมระบบแบบเนทีฟ พร้อมคอมไพเลอร์ สภาพแวดล้อมขณะทำงาน และระบบนิเวศของเครื่องเสมือน ภาษา คอมไพเลอร์ ไลบรารี เครื่องมือสำหรับนักพัฒนา และข้อกำหนดของแพลตฟอร์มทำงานร่วมกันเพื่อสร้างและเรียกใช้ซอฟต์แวร์

**The Now. The Ingredient.** ภาษาโปรแกรมไม่ได้มีเพียงไวยากรณ์เท่านั้น แต่ยังรวมถึงเครื่องมือสำหรับการคอมไพล์และระบบขณะทำงานที่ทำให้ซอฟต์แวร์ใช้งานได้จริง

## I. เริ่มต้นใช้งาน

- [สร้างการทำงานแบบ native](impl/README.md)
- [คู่มือผู้ใช้ Virtual Machine และ Modium](virtual-machine/USER-GUIDE.md) — `sleela vm`, `sleela modium`
- [สถาปัตยกรรม CPU ในซอร์สโค้ด SLeeLa](lib/cpu/CPU.md) — โปรไฟล์ CPU มากกว่า 130 แบบ ดู[การตั้งค่าจังหวะเวลา](lib/cpu/TIMING.md)
- [ดัชนีเอกสาร](docs/README.md)
- [เอกสาร API ที่สร้างขึ้น](docs/api/README.md) และ [แค็ตตาล็อก API](api/README.md)
- [การรองรับแพลตฟอร์มและการตรวจสอบ](docs/PLATFORM-SUPPORT.md)
- [นโยบายการทดสอบ](markdown/TESTING.md)

## II. การ build และทดสอบ

ข้อกำหนดประกอบด้วย toolchain C/C++ (GCC หรือ Clang), GNU Make และ Python 3 ส่วนประกอบกราฟิกบน Linux อาจต้องใช้แพ็กเกจพัฒนา X11/Xrandr เพิ่มเติม

เรียกใช้จากไดเรกทอรีรากของ repository:

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

การ build สำเร็จเพียงองค์ประกอบเดียวไม่ได้ยืนยันว่าไลบรารีหรือแพลตฟอร์มทั้งหมดพร้อมสำหรับการเผยแพร่ โปรดตรวจสอบเอกสาร build และแพลตฟอร์มสำหรับข้อกำหนดของแต่ละ target

## III. การติดตั้ง

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

prefix สำหรับการติดตั้งระดับระบบโดยทั่วไปคือ `/usr/local` การกำหนด `PREFIX="$HOME/.local"` จะติดตั้งในพื้นที่ของผู้ใช้ และโดยปกติไม่ต้องใช้สิทธิ์ผู้ดูแลระบบ ดูรายละเอียดขั้นตอนและแพ็กเกจได้ที่[คู่มือการติดตั้ง](install/README.md)

## IV. ภาษาและ runtime

ไวยากรณ์มาตรฐานกำหนดไว้ใน [`SLEELA.syntax`](SLEELA.syntax) เอกสารใน repository ใช้ไวยากรณ์เวอร์ชัน **3.0** เวอร์ชันของคอมไพเลอร์และเวอร์ชันไวยากรณ์ของซอร์สโค้ดเป็นคนละแนวคิดกัน

เอกสารทางเทคนิคครอบคลุมฟิลด์ต่อเนื่อง `@mix`, ตระกูลชนิด integer แบบขยาย, เครื่องหมายเอกสาร Preonem, SleelaDoc, การสร้างเอกสาร API และเครื่องมือ Nordshrift ชื่อ identifier คำสั่ง ชื่อไฟล์ ชื่อ API และตัวอย่างโค้ดยังคงเดิมเพื่อให้ใช้งานได้โดยตรง

## V. หลักวิศวกรรมและการตรวจสอบ

SLeeLa ให้ความสำคัญกับหลักฐาน สัญญาระหว่างคอมโพเนนต์ที่ชัดเจน ความสามารถในการพกพา พฤติกรรมที่ปลอดภัยเมื่อเกิดข้อผิดพลาด และ release ที่สร้างซ้ำได้ สถานะการตรวจสอบต้องประเมินแยกตามโมดูลและแพลตฟอร์ม การทดสอบที่ข้ามไปต้องไม่รายงานว่าผ่าน และการ build สำเร็จเพียงคอมโพเนนต์เดียวไม่ใช่การตรวจสอบ release อย่างครบถ้วน

เครื่องมือป้องกัน Sleelavirin ประกอบด้วยตัวตรวจสอบ SHA-256 ในเครื่อง ตัวเชื่อมต่อ ClamAV ที่เลือกใช้ได้ และ manifest ความสมบูรณ์ จะถือว่า release ผ่านการตรวจสอบได้ก็ต่อเมื่อ CI สำหรับ commit ของ release เสร็จสมบูรณ์และมีการตรวจทาน artifact แล้ว

## VI. Editor และการมีส่วนร่วม

SLeeLa มีธีมและการเชื่อมต่อไวยากรณ์สำหรับ **Vim** และ **Emacs** รวมถึงเครื่องมือชุดสี native C/C++ ดู[คู่มือการเชื่อมต่อ editor](linux-editors/README.md)

แนวทางการส่ง contribution:

1. อ่านดัชนีเอกสารและนโยบายการทดสอบ
2. ส่งการเปลี่ยนแปลงที่มีขอบเขตชัดเจน พร้อมการทดสอบทั้งกรณีที่ควรผ่านและไม่ควรผ่าน
3. รันการทดสอบที่เกี่ยวข้อง แล้วจึงรันชุดทดสอบที่ครอบคลุมยิ่งขึ้น
4. ระบุระบบปฏิบัติการและ toolchain ที่ใช้ ห้ามรายงานการตรวจสอบที่ข้ามไปว่าเสร็จสมบูรณ์

SLeeLa พัฒนาโดย **MEARVK LLC**

## VII. เนื้อหาทางเทคนิคอื่น

README หลักยังกล่าวถึงไลบรารี Advantage, SleelaDoc และ XML DOM; ชนิด integer พื้นฐาน; runtime สำหรับอักขระ; AutoCAD Creator™; แค็ตตาล็อก UTF-4088 เชิงทดลอง; เครื่องมือ Pi, Modium, Registry และ `sleelactl`; รวมถึงระยะแรกของเส้นทางคอมไพล์ C/C++ native ที่เปิดใช้ด้วยเครื่องหมาย `@native`

## เอกสารอ้างอิงทางเทคนิคฉบับเต็ม

[README หลัก](README.md) มีแค็ตตาล็อกคุณสมบัติทั้งหมด เอกสารอ้างอิงภาษาและ runtime คำแนะนำการ build และติดตั้ง ลิงก์ API ตัวอย่าง และบันทึก release ฉบับภาษาไทยนี้ได้ขยายคำแนะนำการใช้งานแล้ว แต่ยังไม่ได้แปล README ทางเทคนิคทุกส่วนแบบบรรทัดต่อบรรทัด จนกว่าจะมีการแปลและตรวจทานทางเทคนิคครบถ้วน README ภาษาอังกฤษยังคงเป็นแหล่งอ้างอิงทางเทคนิคหลัก

- [README หลัก](README.md)
- [ข้อกำหนดไวยากรณ์ SLeeLa](SLEELA.syntax)
- [ดัชนีเอกสาร](docs/README.md)
- [การรองรับแพลตฟอร์ม](docs/PLATFORM-SUPPORT.md)
- [GitHub repository](https://github.com/mearvk/SLeeLa)

**สถานะการแปล:** แปลบทนำ การเริ่มต้นใช้งาน การ build การติดตั้ง ภาพรวมภาษาและ runtime หลักวิศวกรรม การตรวจสอบ และแนวทาง contribution แล้ว ส่วนทางเทคนิคทั้งหมดที่เหลือยังต้องแปลและให้ผู้ตรวจทานภาษาไทยที่มีความรู้ด้านเทคนิคตรวจสอบ


## VIII. สถาปัตยกรรมคอมไพเลอร์และขั้นตอนการแปลโปรแกรม

SLeeLa แยกภาษาต้นฉบับ ส่วน frontend ของคอมไพเลอร์ intermediate representation การสร้างโค้ด และ runtime ออกจากกัน รุ่นของคอมไพเลอร์ รุ่นของ syntax และรุ่นของ VM/ISA เป็นข้อมูลคนละส่วน การมีคุณลักษณะอยู่ใน specification ไม่ได้ยืนยันว่าคุณลักษณะนั้นได้รับการพัฒนาและตรวจสอบครบทุก target

ขั้นตอนทั่วไปประกอบด้วย:

1. **Source:** ไฟล์ UTF-8 นามสกุล `.sleela` พร้อม version pragma และ declarations
2. **Lexing และ parsing:** แปลงข้อความเป็น tokens และโครงสร้าง syntax
3. **Semantic analysis:** ตรวจสอบชื่อ symbols types และกฎของภาษา
4. **Intermediate representation:** รูปแบบโปรแกรมที่มีโครงสร้างสำหรับ lowering ขั้นต่อไป
5. **Code generation:** สร้าง output สำหรับ VM หรือ intermediate targets ที่รองรับ
6. **Runtime:** ประมวลผลโปรแกรมที่ compile แล้วด้วยกลไก memory และ control flow ที่มีอยู่

เอกสารข้อกำหนดหลักคือ [SLEELA.syntax](SLEELA.syntax) ส่วนรูปแบบ source และวงจรชีวิตไฟล์ดูที่ [source documentation](markdown/SOURCE.md) และ [compiler documentation](markdown/COMPILER.md)

## IX. ชนิดข้อมูลและแบบจำลองออบเจ็กต์

ชนิดข้อมูลหลักประกอบด้วย `int`, `double`, `boolean`, `String`, `void`, ชนิด `struct` ที่ประกาศเอง และ reference-type signatures ที่รองรับ ตามเอกสารปัจจุบัน `int`, `long` และ `int64` เป็น signed 64-bit integers ส่วน `int128` รับเป็นชนิด integer ได้ แต่ implementation ที่ระบุยังอิง 64-bit จึงไม่ควรถือว่าเป็น arithmetic 128-bit อย่างสมบูรณ์

`struct` คือ aggregate ที่มี fields ระบุชื่อ VM-managed struct handles ไม่ใช่ raw pointers ที่เปิดเผยโดยตรง การกำหนดค่าและส่งพารามิเตอร์ใช้ instance เดียวกัน และ equality อิง identity สำหรับการแลกเปลี่ยนข้อมูลแบบ portable ให้ใช้ serialization helpers ที่ระบุ เช่น `structPack(instance)` และ `structUnpack(TypeName, json)`; VM-local handle ไม่ใช่ข้อมูลเครือข่ายที่ส่งต่อได้โดยตรง

Class ประกอบด้วย fields และ methods ใน execution model ปัจจุบัน fields เป็น shared class state ส่วน method-local variables เป็นของแต่ละ thread การสืบทอดคลาส nested classes และ generics ขึ้นกับ syntax-version gates ดูรายละเอียดที่ [International Language Reference](LANGUAGE.INTERNATIONAL.3.0.md)

## X. Control flow และการกำหนดเวลา loop

ภาษาให้ syntax สำหรับเงื่อนไข loops การเรียก methods และ recursion annotations `@loopDelay` และ `@loopBudget` ใช้กำหนดเวลาหยุดหรือ time budget ของ loop โดยเขียนไว้ก่อน loop และจะ lower ไปเป็น VM instruction `OP_SLEEP` (ISA 157) ฟังก์ชัน built-in `sleep(ms)` ใช้ instruction เดียวกัน

```sleela
@loopDelay(ms = 250)
while (running) {
    processNext();
}
```

ตัวอย่างนี้แสดงรูปแบบ syntax เท่านั้น โปรแกรมจริงต้องประกาศ identifiers และ enclosing class ให้ครบ ดู [SLEELA.syntax §32](SLEELA.syntax) และ [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md)

## XI. Standard library และโมดูลหลัก

Repository มีโมดูลที่มีระดับความพร้อมแตกต่างกัน:

- `lib/advantage` — การประเมินผลแบบหลายชั้นเทียบกับ model ครอบคลุม mathematics, bilateral trade economics, integration, grading และ routable output การกล่าวถึง trust fields ในเชิงฟิสิกส์เป็น modeling analogy ไม่ใช่ข้ออ้างว่าเป็นกฎฟิสิกส์
- `lib/xml` — XML document tree ที่มี element, text, comment, CDATA, processing-instruction nodes, attributes, parser และ serializer
- `lib/text` — การจัดการ text/string แบบ character-cell เช่น `charAt`, `substring`, `indexOf`, `reverse` และ positional formatting
- `lib/sleeladoc` — การดึงเอกสารจาก source และหน้าอ้างอิง CLI คำสั่ง `sleela docs` แสดง topics, packages และ classes ส่วน `sleela docs --generate [dir]` สร้าง text corpus สำหรับใช้งานแบบ offline
- `lib/cpu` และ `lib/xlt` — CPU profiles, architecture components, model format และ timing references
- `recursion/stanchion` — C/C++ reference prototypes และตัวอย่าง recursion

การมีชื่อโมดูลใน catalog ไม่ได้แปลว่าพร้อมใช้ใน production ควรตรวจสอบ README ของโมดูล test coverage ข้อจำกัดของ platform และเอกสาร validation ก่อนนำไปใช้

## XII. การเชื่อมต่อ C/C++ และขอบเขต native

Repository มี C และ C++ components รวมถึงเส้นทาง native code generation ที่มีขอบเขตจำกัด `@native` และ `sleela aot` เกี่ยวข้องกับการพัฒนา native ไม่ใช่การอนุญาตทั่วไปให้รัน native code ใดก็ได้

การเชื่อมต่อ C/C++ ต้องระบุ ABI, calling convention, memory ownership, data layout, error handling และ platform dependencies อย่างชัดเจน ให้ถือ native code เป็น trust boundary แยกต่างหาก reference หรือ declaration ระบุ resource ได้ แต่ไม่ได้ให้สิทธิ์ในการ execute ดู [native implementation](impl/README.md), [platform matrix](docs/PLATFORM-SUPPORT.md) และ [comparison standards](comparison/README.md)

## XIII. Build, installation และ platform support

Native build โดยทั่วไปต้องใช้ C/C++ toolchain เช่น GCC หรือ Clang และ GNU Make เครื่องมือบางส่วนใช้ Python 3 ส่วน Linux GUI components อาจต้องใช้ development packages ของ X11 และ XRandR

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

การติดตั้งระดับระบบโดยทั่วไป:

```sh
sudo make install
```

การติดตั้งสำหรับผู้ใช้ปัจจุบัน:

```sh
make install PREFIX="$HOME/.local"
```

ตรวจสอบ [installation guide](install/README.md) และ [platform support matrix](docs/PLATFORM-SUPPORT.md) ก่อนติดตั้ง อย่าสมมติว่า dependencies และคำสั่งเดียวกันใช้ได้เหมือนกันบน Linux, macOS และ Windows

## XIV. ความปลอดภัย ความถูกต้อง และการทดสอบ

Hash manifests และ verification tools ใช้ตรวจว่าไฟล์ตรงกับเนื้อหาที่คาดหวังได้ ผล PASS ใช้ได้เฉพาะ scope และเวลาที่ตรวจสอบเท่านั้น รายงานทดสอบต้องแยก pass, fail และ skip ออกจากกัน ห้ามรายงานการทดสอบที่ skip หรือไม่ได้รันว่า pass

หลักการสำคัญ:

- References ใช้ระบุ resources ไม่ได้อนุญาตให้ execute
- Native interfaces ต้องชัดเจนและมีขอบเขต
- ข้ออ้างเรื่อง platform support ต้องมี builds และ tests จริงรองรับ
- Generated documentation ต้องตรวจสอบย้อนกลับไปยัง source และ generator ได้
- เมื่อไม่มี integrity data ที่จำเป็น security checks ต้อง fail-closed อย่างปลอดภัย

ดู [testing policy](markdown/TESTING.md) และ [platform validation](docs/PLATFORM-SUPPORT.md)

## XV. เอกสาร เครื่องมือแก้ไข และการมีส่วนร่วม

เริ่มจาก [documentation index](docs/README.md), [generated API reference](docs/api/README.md), [API catalogue](api/README.md) และ [SleelaDoc](lib/sleeladoc/README.md) สำหรับ editor resources บน Linux ดู [linux-editors](linux-editors/README.md)

ก่อนส่ง contribution ให้อ่าน README ของโมดูล กำหนดขอบเขตการเปลี่ยนแปลง เพิ่ม positive และ negative tests รันการทดสอบที่เกี่ยวข้อง บันทึก OS/toolchain/commands/results และปรับ README กับ API documentation เมื่อ public behavior เปลี่ยน

## XVI. ขอบเขตโครงการอื่น

CPU และ hardware models, GUI, networking, text processing, UTF-4088, π tools, Modium, Registry, `sleelactl` และ AutoCAD Creator™ เป็น project areas แยกกัน ซึ่งมีวัตถุประสงค์ dependencies และ maturity ต่างกัน ห้ามเรียก experimental directory หรือ example ว่าเป็น stable production feature หากไม่มี validation ที่บันทึกไว้ จำนวน entries หรือขนาดของ UTF-4088 ไม่ได้พิสูจน์ Unicode compatibility อย่างสมบูรณ์ ส่วน AutoCAD compatibility ต้องมี version matrix และผลการทดสอบชัดเจน

**สถานะการแปล:** ฉบับภาษาไทยนี้ขยายเนื้อหาให้ครอบคลุม compiler pipeline, type/object model, loop timing, libraries, native integration, build/install, security, testing และ contribution workflow แล้ว แต่ยังไม่ใช่การแปลแบบครบทุกบรรทัดของ README ภาษาอังกฤษและ specification ที่เชื่อมโยงทั้งหมด
