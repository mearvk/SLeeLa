<img align="right" src="../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — Türkiye (TR)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Yerel bir sistem programlama dili, derleyici, çalışma zamanı ve sanal makine ekosistemi.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="../../docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Uluslararası ülke sürümü.** Bu sürüm Türkçedir. Teknik tanımlayıcılar, komutlar, kod örnekleri, dosya adları, yollar ve bağlantılar, kanonik İngilizce [README.md](../../README.md) ile aynıdır ve değiştirilmemiştir.

**The Now. The Ingredient.** Bir dil yalnızca sözdiziminden ibaret değildir: yazılımı kullanılabilir kılan derleyici, çalışma zamanı, kütüphaneler, araçlar ve platform sözleşmeleridir.

**SLeeLa'nın ilgilendiği alanlar.** SLeeLa; **Math**, **Object**,
**Science** ve **Reach** ile ilgilenir — çalışmasının dört yüzü: hesaplanabilir
olan (Math), var olanların kataloğunda duran yerleşik şey (Object), doğrunun
disiplinli incelenmesi (Science) ve bir sistemin elinin başka sistemlere ve
insanlara uzatılması (Reach). **Ulusal çıkarlar varlığını korur** — dördünden
hiçbiri onları bir kenara itmez; bir konu bir ulusun çıkarıyla ilgili olduğunda
bu konum korunur (örneğin her sınıfın Metadata alanları `metaOrigin` /
`metaNationalSchedule` içinde sadakatle kaydedilir) ve asla ortadan kaldırılmaz.

## I. Buradan Başlayın
- [Yerel uygulamayı derleme](../../impl/README.md)
- [Sanal makineler ve Modium — uygulamalı kullanıcı kılavuzu](../../virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [SLeeLa kaynak kodunda CPU yığını](../../lib/cpu/CPU.md) — mantık kapılarından → ALU → çekirdekler → işletim sistemi → programlara uzanan eksiksiz bir makine, 130'dan fazla CPU profilinin listesi ve [spesifikasyonda çalıştırıp hız aşırtabileceğiniz](../../lib/cpu/components/README.md) ayarlanabilir [voltajlar, değişken saatler ve uzman zamanlamaları](../../lib/cpu/TIMING.md)
- [Dokümantasyon dizini](../../docs/README.md)
- [CSPML veri kaynağı ve Salt & Salary araştırma referansı](../../CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — ABD'deki yerler, sosyoekonomik koşullar, iş gücü, eğitim ve devlet okulları hakkındaki harici verilerin kökeni ve entegrasyon rehberi.
- [Oluşturulmuş API belgeleri](../../docs/api/README.md) — güncel HTML başvurusu, sürümlü API sürümü ve yeniden oluşturma yönergeleri
- [API kataloğu](../../api/README.md)
- [Hesaplamalı karşılaştırma standartları](../../comparison/README.md) — SLeeLa VM, Java/JVM ve yerel C/C++ için tür, bellek, ABI ve uyumluluk standartları.
- [Recursion stanchion — C ve C++ prototipleri](../../recursion/stanchion/README.md) — Boolean, Byte, Character, Integer, Long, Float ve Double için ilkel türe özelleşmiş örnekler; bunların yanında genel, uygulamalı, bilimsel ve öğretici özyineleme kalıpları.
- [Platform desteği ve doğrulama matrisi](../../docs/PLATFORM-SUPPORT.md)
- [Marka kılavuzu](../../docs/BRAND-GUIDE.md)
- [Ürün (merchandise) kavramları ve varlıkları](../../merchandise/README.md)
- [NitroWebExpress logosu](../../images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Test politikası ve sürüm ölçütleri](../../markdown/TESTING.md)

## II. Haberler

Dilde, sürücülerde ve standart kütüphanede son eklemeler:

- **`advantage` standart kütüphane paketi — bir temettünün avantajlandırılması;
  matematik, fizik, entegrasyon, derecelendirme ve yönlendirilebilir çıktı ile.**
  [`lib/advantage/`](../../lib/advantage/README.md), *bir modele göre eşsiz bir
  temettünün avantajlandırılmasını* katmanlı bir **Sequitur soğanı** olarak
  (temettü → gözden geçirme → kaygılar → matematik → eklemeler → çıkarma →
  yeniden gruplama → yineleme) kapsüller; içe/dışa doğru tam dinamik güncelleme
  ve restitüsyonla gezilebilir ve basit bir **kullanıcı modeli** ile tam kontrollü
  bir **geliştirici modeli** aracılığıyla sunulur. Beş konu alanı taşır:
  **`math/`** ikili ticaret ekonomisi (ortak kâr maksimizasyonu, Nash
  pazarlığı, ikili newsvendor kritik oranı), **`physics/`** yapısal ekonominin
  "güven alanları fiziği" (metrik tensör olarak güven, işlemsel jeodezik, bir
  duyuru temas yapısı, CIR+sıçrama güven bozunumu SDE'si — bir modelleme
  benzetmesi olarak işaretlenmiştir), **`integration/`** kapitalizm/perakende
  için özyinelemeli olarak birleştirilebilen bir dikey entegrasyon modeli
  (bölmeler, mühendislik gözleri, deneme bölmeleri, newton medyan tepsileri ve
  bir sistem sofistikasyon puanı), **`grading/`** her modülü ve herhangi bir
  kazanan/kaybeden/derecelendirici/mutlak modül kümesini A–D ile derecelendiren
  bir sofistikasyon devresi derecelendiricisi ve **`output/`** modül çıktısını
  terminale, **CommonRails** biçimli bir yüzeye veya bir sokete/bağlayıcıya
  (yerel `slnet_write` için çerçevelenmiş) gönderen yönlendirilebilir bir yayın
  dikişi. Her sınıf Metadata-Class kaydını taşır; çalıştırılabilir beş demo
  konuları doğrular ve eşlik eden
  [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md),
  [`MATH.md`](../../lib/advantage/MATH.md) ve
  [`PHYSICS.md`](../../lib/advantage/PHYSICS.md) bunları belgeler.
  [`advantage/`](../../advantage/README.md) altındaki yerel bir C/C++ tüketicisi (C
  çekirdeği `advantage_value`/`advantage_mean`, bir XML bilgi deposu, güvenli bir
  HTTPS/SFTP/SSH edinimi + anahtar deposu + DH/ECDH aktarım kütüphanesi) modeli
  işlenmiş
  [ticaret örneklerine](../../advantage/examples/README.md) bağlar — varsayılan
  internet kaynaklarından olumlu ticaretler, bir hedef kitle ilgisi dizisi
  (Tüccarlar, Tüketiciler, Yardımcılar, Şifacılar) ve A–D olarak sıralanmış sekiz
  model (D = en çok Cement, A = en çok Brand).
- **Döngü zamanlama ek açıklamaları — `@loopDelay` / `@loopBudget` (+ yerleşik
  `sleep(ms)`).** SLeeLa'nın ilk **ifade düzeyinde, davranışsal** ek açıklamaları:
  bir `for` / `while` / `do` döngüsünden önce kendi satırlarında yazıldığında,
  döngünün yürütülmesine gerçek bir duraklama eklerler. `@loopDelay(ms = 250)` /
  `@loopDelay(seconds = 2)` her yinelemede sabit bir süre uyur (yalın
  `@loopDelay(250)` varsayılan olarak milisaniyedir); `@loopBudget(seconds = 10,
  iterations = 1000)` bilinen bir yineleme sayısını toplam duvar saati bütçesine
  eşit olarak yayar. Her ikisi de yeni, yalnızca sona eklenen **`OP_SLEEP`**
  işlem koduna (ISA 157) dönüştürülür; bu kod her yerden çağrılabilen genel bir
  **`sleep(<ms>)`** yerleşik işlevini de destekler; duraklama taşınabilir biçimde
  (`nanosleep` / `Sleep`) gerçeklenmiştir. [`SLEELA.syntax` §32](../../SLEELA.syntax)
  içinde belirtilmiştir; API başvurusu
  [`api/sleela/3.0/LOOP_TIMING_API.md`](../../api/sleela/3.0/LOOP_TIMING_API.md)
  dosyasındadır; çalıştırılabilir örnek:
  [`examples/loop-timing-annotation-demo.sleela`](../../examples/loop-timing-annotation-demo.sleela).
  Oturum eklemeleri dizini (bu özellik, yerel `OP_BIGMUL` büyük tamsayı çarpımı
  ve bağımsız **[local-generator](../../local-generator/README.md)** ile
  **[pi-generator](../../pi-generator/README.md)** araçları dahil)
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](../../api/sleela/3.0/SESSION-ADDITIONS-API.md)
  dosyasındadır. Oluşturulan üç API yüzeyi
  ([`sleeladoc-api/`](../../sleeladoc-api/README.md),
  [`docs/api/`](../../docs/api/README.md) ve değişmez
  [`api/sleela/3.0/version-014/`](../../api/sleela/3.0/version-014/index.html)
  anlık görüntüsü) `make api-docs` ile yeniden oluşturuldu ve artık `vm` paket
  sayfalarında yeni `SLISASleep` / `SLISABigMul` komut girdilerini taşıyor.
- **Komut satırından SleelaDoc — `sleela docs`.** SleelaDoc artık doğrudan
  terminalden erişilebilir. `sleela docs` belgelenebilir her konuyu listeler;
  `sleela docs crypto` bir paketi gösterir; `sleela docs stringbuilder` bir sınıfı
  çözümler (büyük/küçük harfe duyarsız ve `SL` standart kütüphane önekiyle →
  `SLStringBuilder`) ve zengin bir sayfa yazdırır — **tanım, tasarım amacı,
  ilişkiler, yapı, kullanıcı kilitleri ve beş basamaklı bir ustalık merdiveni** —
  ıskalanan aramalar için sıralı "bunu mu demek istediniz" sonuçlarıyla. Yazarlar
  herhangi bir sayfayı yalnızca SleelaDoc `@` etiketleri (`@intent`, `@thesis`,
  `@axiom`, `@param`, `@see`, `@mastery`) ekleyerek zenginleştirir.
  `sleela docs --generate [dir]` düz metin bir külliyat yazar (sınıf başına bir
  sayfa + bir `TOPICS.index`); CLI bunu daha sonra çevrimdışı okur;
  **`make sleeladoc`** bu külliyatı **ve eşleşen `.sleela` kaynaklarını**
  `sleeladoc/build/sleeladoc.jar` içinde paketler. Yalnızca cephe (facade)
  niteliğindeki konular için (örn. `psychiatry`, `politico`),
  `sleela docs --facade <topic>` çalışma zamanında dosya sistemini arar ve gerçek
  **klasörleri (ardından doğal hareket olarak dosyaları)** listeler; sınırlar
  kullanıcı tarafından değiştirilebilen **6 MB / 6000 dosya / 6 dakika**
  üst sınırlarıdır. `lib/sleeladoc` birimleri güncel sürümde, **`#sleela 3.0`**
  ile tutulur. Bkz. [`man/man1/sleela.1`](../../man/man1/sleela.1) ve
  [`lib/sleeladoc/README.md`](../../lib/sleeladoc/README.md).
- **SleelaDoc — Javadoc tarzı bir dokümantasyon çözümü ve oluşturulmuş bir HTML
  sitesi.** **SleelaDoc**, SLeeLa'nın kaynak içi belge yorumu geleneğidir
  (`@doc` / `@page` ile işaretlenmiş açılı ayraç yorumları; bkz.
  [`manifesto/SLEELADOC.md`](../../manifesto/SLEELADOC.md)) ve
  [`lib/sleeladoc`](../../lib/sleeladoc/README.md), bir `.sleela` birimini Markdown
  veya HTML API sayfası olarak işleyen **çıkarıcıdır** — `javadoc`'un Java'yı
  işlemesi gibi, *niyeti* belgeleyerek. **Tüm `/lib` için** eksiksiz, gezilebilir
  bir **HTML dokümantasyon sitesi**, `tools/generate-sleeladoc-api.py` tarafından
  [`/sleeladoc-api`](../../sleeladoc-api/README.md) altında oluşturulur
  (`sleeladoc-api/index.html` dosyasını açın; paket → sınıf → üye hiyerarşisi ve
  *Student 2* okuyucusu için yazılmış bir sözlükle, sade mavi / açık mavi / açık
  gri / beyaz bir stil). Derleyici ayrıca yazar tarafından işaretlenmiş belge
  çıktılarını derlemenin yan ürünü olarak üretir (`--wiki` MediaWiki sayfası +
  kökenlendirilmiş bir kaynak ikizi); bunlar
  [`manifesto/SLEELADOC.WIKI.md`](../../manifesto/SLEELADOC.WIKI.md) dosyasında
  belgelenmiştir. Çalıştırılabilir:
  [`lib/sleeladoc/sleeladoc-demo.sleela`](../../lib/sleeladoc/sleeladoc-demo.sleela).
- **XML Belge Nesne Modeli (`lib/xml`).** SLeeLa tarafında bir XML DOM — sıralı
  çocukları ve adlandırılmış öznitelikleri olan element/metin/yorum/CDATA/PI
  düğümlerinden oluşan bir ağaca sahip bir belge, bir serileştirici
  (sıkıştırılmış/güzel biçimli, XML 1.0 §2.4 kaçışı), bir karakter hücresi
  ayrıştırıcısı ve `SLXmlAspect` kalite kaydı. Düğüm eşitliği, güvenin üç
  basamaklı bir bağıdır (kimlik → aynı özne → yapı); yansımalı, simetrik ve
  belirlenimcidir. Bkz. [`lib/xml/README.md`](../../lib/xml/README.md) ve çalıştırılabilir
  [`lib/xml/xml-dom.sleela`](../../lib/xml/xml-dom.sleela).
- **Geniş işaretli tamsayı ilkelleri (`long`, `int64`, `int128`).** `int`, `long`
  ve `int64` türleri, aynı biçimde tür denetiminden geçen ve çalışan 64-bit
  işaretli tamsayılardır; `int128` bir tamsayı türü olarak kabul edilir (şimdilik
  64-bit tabanlıdır, gerçek 128-bit belgelenmiş bir sonraki adımdır). 2 tabanlı
  işaretli `int32`..`int16777216` serisi (sözdizimi 2.7) ve eşleşen işaretsiz
  `uint32`..`uint16777216` serisi (sözdizimi 2.8) bunu genişletir. Bkz.
  [`SLEELA.syntax`](../../SLEELA.syntax) §21, §24, §25.
- **`@mix` ardıl alanları (sözdizimi 2.9).** Bir sınıf, her tekrar `@mix`
  taşıdığında **aynı ada** sahip birden çok örnek alanı bildirebilir; dil bu
  grubu ardıl (indekse göre anahtarlanan bir dizi / liste) olarak anlar: varsayılan
  olarak yukarıdan aşağı `1,2,3,…` ya da elle `@mix(1,2,3,...)` ile; indeks sayısı
  alan sayısıyla eşleşmelidir. Bkz. [`SLEELA.syntax`](../../SLEELA.syntax) §28.
- **Belge işaretleyicileri — Doğal Süreç, Preonem'ler olarak adlandırılır
  (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).**
  Bu yalın belge işaretleyici sözcükler topluca **Preonem'ler** olarak
  adlandırılır (bir işaretleyici bir *Preonem*; küme, *Preonem'ler* — bkz.
  [`SLEELA.syntax`](../../SLEELA.syntax) §26.1). Bir belgenin yapılış sırasında *ne
  olduğunu* adlandıran birinci sınıf, yalnızca bildirimsel belge işaretleyicileri:
  `author`/`authorial` (sözdizimi 2.4; profesyonel konsey tarafından ele alınır —
  davranışsal etkisi olan tek üye), `material;` (yerleşik Kaynak) ve `memory;`
  (sözdizimi 2.5), `determine;` (sözdizimi 2.6; bir doktor, hekim veya hayalet
  tarafından yasal elle tamamlanmaya doğru daha fazla işlenmeye açık bir belge)
  ve en yeni ikisi, **`object;`** (var olanların kataloğunda duran yerleşik bir
  şey — **somutlaştırılmış, somut bir konum verilmiş, sözdizimi 2.8'de**) ve
  **`field;`** (nesnelerin içinde durduğu ve hareket ettiği ilişki zemini) —
  her ikisi de sözdizimi 2.6'dan itibaren kapılıdır. Her biri yalın, bağlamsal ve
  ön uç tarafından sürüme göre kapılanmıştır; ayrıştırma, tür belirleme veya kod
  üretimi üzerinde hiçbir etkisi yoktur; işaretleyici konumunun dışında her biri
  sıradan bir tanımlayıcı olarak kalır (dolayısıyla `field` hiçbir zaman bir sınıf
  üyesini veya bir `.field` erişimini bozmaz). Bkz. [`SLEELA.syntax`](../../SLEELA.syntax)
  §18–§20, §23 ve §26.
- **Sözdizimi 3.0'da Nordshrift ve tam kütüphane erişimi.** Nordshrift, derleyiciyle
  aynı özyinelemeli `/lib` dizini üzerinden çözümleme yapar ve her pakete ve
  `.sleela` birimine ulaşır; `nordshrift check` tüm ağacın sayımlarını raporlar.
- **Recursion stanchion.** [`recursion/stanchion/`](../../recursion/stanchion/README.md)
  dizini artık belgelenmiş yedi ilkel sözleşme için ayrılmış C ve C++ referans prototiplerinin yanı sıra genel, uygulamalı, bilimsel ve öğretici örnekler içerir. Bunlar referans kaynaklardır; doğrulanmış saymadan önce derleyin ve test edin.

- **Açılı ayraç yorumları (`<` / `<<`).** Yeni, bundan sonrası için bir yorum stili —
  `< satır >` ve `<< blok >>` — SLeeLa ön ucu tarafından sözcüklere ayrılır ve
  [`SLEELA.syntax`](../../SLEELA.syntax) §1.1 içinde belgelenmiştir.
- **Taban (radix) tamsayı sabitleri.** `int` sabitleri artık hem SLeeLa kaynak
  dilinde ([`SLEELA.syntax`](../../SLEELA.syntax) §1.4) hem de
  [`.xlt` CPU modeli biçiminde](../../lib/xlt/README.md) `_` basamak ayırıcılarıyla
  onaltılık (`0x`), ikilik (`0b`) ve sekizlik (`0o`) biçimleri kabul eder.
- **Kütüphane genelinde Metadata Class standardı.** Betimleyici üstveri
  metadeseni (köken, yasallık, yargı yetkisi kapsamı) artık `/lib` altındaki sınıf
  içeren her `.sleela` dosyasında taşınır; bkz. [`SLEELA.syntax`](../../SLEELA.syntax)
  §15 ve [öğretici ders](../../tutorial/13-metadata-class.md).
- **Daha zengin bir metin / String ailesi.** `lib/text`, gerçek dil içi
  yardımcılar, gerçek `charAt`/`substring`/`indexOf`/`reverse`/sütun hizalaması
  sağlayan bir karakter hücresi görünümü ve gerçek bir konumsal şablonlayıcı
  kazanır — bkz. [metin ve String öğreticisi](../../tutorial/14-text-and-strings.md) ve
  çalıştırılabilir [`lib/text/text-string-demo.sleela`](../../lib/text/text-string-demo.sleela).
- **Nordshrift sayfalarında birinci sınıf `String`.** Bir `.sst` sayfası, derlemenin
  bileşen bildirimine katkıda bulunan türlü bir `strings:` bloğu bildirebilir;
  bkz. [Nordshrift Sheet Reference](../../wiki/Nordshrift-Sheet-Reference.md).
- **İşlem kodlarıyla ve işlev çağrısı denkliğiyle çalışan CPU modelleri.** Her
  `SL<ARCH>CPU`, hem yerel bir komut akışını getir-çöz-yürüt döngüsünden
  geçirerek çalıştırma *hem de* varsayılan başka bir sürecin kodunu paylaşılan VM
  altyapısı üzerinde eşdeğer çağrılarla çalıştırma temelini devralır — bkz.
  [`lib/cpu/EXECUTION.md`](../../lib/cpu/EXECUTION.md) §5.

## III. Derleme ve Test

**Önkoşullar:** bir C/C++ araç zinciri (GCC veya Clang) ve GNU Make; bildirim/akış
araçları için Python 3; Linux'ta görüntü ürünleri için X11/Xrandr geliştirme
paketleri (bir modülün `make deps` komutu ihtiyaç duyduklarını raporlar/kurar).
`impl/` derlemesi güvenilir bir SHA-256 bildirimine göre güvenli-başarısızlık
(fail-closed) ilkesiyle çalışır; kök `Makefile` bunu otomatik olarak gösterir.

Temiz bir çıkarımdan en hızlı başlangıç:

```sh
make                                  # build everything from the repo root
```

Ya da yalnızca yerel çekirdeği derleyip test edin:

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

Depo kökündeki `make` her ürünü (çekirdek, VM, kodekler, churn, antivirüs, …)
derler ve **kayıt defterini (registry)** derleyip temel tablosunu
gerçekleştirerek biter — böylece derlenmiş bir ağaç, kutudan çıktığı gibi
incelenebilir bir kayıt defteri sunar. Temel hedefler:

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

Diğer yararlı hedefler:

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

`registry` hedefi [`build-registry.sh`](../../build-registry.sh) betiğini çalıştırır:
kayıt defteri çekirdeğini derler, temel tabloyu taşınabilir metin deposu
`registry/build/sleela.sleelareg` içine yerleştirir ve MySQL tablo betiği
`registry/build/registry.sql` dosyasını (`sleela registry export` aracılığıyla)
dışa aktarır. Bunu `sleela registry` veya
`registry/build/sleela-registry review` ile inceleyin. Bkz.
[kayıt defteri kılavuzu](../../registry/README.md) ve
[Registry 3.0 mimarisi](../../registry/REGISTRY-3.0-ARCHITECTURE.md).

Önkoşullar ve platforma özgü adımlar için derleme ve test kılavuzlarına bakın. Tek bir bileşenin başarıyla derlenmesi, her kütüphanenin veya platformun sürüme hazır olduğunu kanıtlamaz.

## IV. Kurulum

Yerel ürünleri derledikten sonra komutları ve çalışma zamanı destek dosyalarını kurun:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

Geleneksel kurucu varsayılan olarak `/usr/local/bin`, `/usr/local/lib/sleela` ve
`/usr/local/share/sleela` konumlarını kullanır. Mevcut bir başlangıç
yapılandırmasını korur ve izlenmeyen mevcut komutların üzerine yazmayı reddeder.
Kaldırma işlemi yalnızca kurucu tarafından kaydedilen dosyaları siler; yapılandırmayı
ve izlenmeyen çalışma zamanı verilerini korur.

Yalnızca ana paketin — `sleela`, `sleelvac` ve `nordshrift` ikili dosyaları,
standart kütüphane, protokoller ve destek modülleri — odaklı, rehberli
dağıtımı için temiz kurucuyu kullanın:

```sh
make clean-install       # guided install of the main package only
```

Tek bir birincil karar (ana paketi kurmak), ardından bir kapsam seçimi ve başarı
halinde seçilmiş geliştirme (enhancement) modülleri için isteğe bağlı bir seçici sunar.
Etkileşimli dağıtım iş akışı ve tüm kurulum seçenekleri için bkz.
[Hızlı ve Güvenli Kurulum](../../install/README.md).

## V. Dil ve Çalışma Zamanı
Normatif dilbilgisi [`SLEELA.syntax`](../../SLEELA.syntax) dosyasındadır. Depo şu anda **3.0** sözdizimini belgeler; derleyici/araç sürümü ile kaynak sözdizimi sürümü birbirinden ayrıdır. 3.0 yüzeyi **`pure` ve `gasoline` belge işaretleyicilerini** (§30) ekler — her biri belgenin konumunu kendi dürüstlüğüyle işaretleyen, neredeyse aynı anlama gelen bir çift yalın, bağlamsal, yalnızca bildirimsel Preonem. Sözdizimi 3.0, 1.0–2.9 dil yüzeyini güncel normatif sürümde birleştirir ve özelliğe özgü tüm asgari kapıları korur. Önceki 2.9 yüzeyi **`@mix` ardıl alanlarını** (§28) ekler — her biri `@mix` taşıdığında bir sınıf aynı adlı birden çok örnek alanı bildirebilir ve bu, tek bir indeksli grup olarak anlaşılır (yukarıdan aşağı `1,2,3,…` veya indeks sayısı grupla eşleşmesi gereken elle `@mix(1,2,3,...)`). Daha önceki `long`/`int64`/`int128` (§21) üzerine 2 tabanlı işaretli `int32`..`int16777216` (§24) ve işaretsiz `uint32`..`uint16777216` (§25) geniş tamsayı ailelerini; **anahtar sözcük destekleyici belgelerini** (§22); ve **Preonem'leri** devralır — bildirimsel belge işaretleyicileri `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) ve `artifact` (§35, zamana ve uzaya göre derecelendirilmiş yasama düzeyinde konum); bunlar topluca Preonem'ler olarak adlandırılır (§26.1). Yapıcı (constructor) aşırı yükleme çözümlemesi ile `this(...)`/taban yapıcıya devretme, doğrulanmış sözdizimi 1.8 sözleşmesinin açıkça dışındadır. Bir özelliğe güvenmeden önce dilbilgisine ve uygulama notlarına başvurun. SleelaDoc'un dokuz kaynak birimi artık `#sleela 3.0` bildirir; ortak derleyici kapısı 1.0–3.0 kaynak sözdizimini kabul eder. Yeni API derlemesi `make api-docs` ile üretilir.

Depo; yerel bir C/C++ uygulaması, SLVM üretim başvuruları, standart kütüphane modülleri, testler, örnekler ve platform entegrasyonları içerir. Uygulama ve doğrulama durumu modüle göre değişir; tekdüze bir bütünlük varsaymak yerine ilgili belirtime ve güncel CI sonuçlarına başvurun.

## VI. Mühendislik İlkeleri
- **İddia yerine kanıt:** uygulanmış, test edilmiş, deneysel ve planlanmış davranışı birbirinden ayırın.
- **Her sınır için tek sözleşme:** sözdizimini, API'leri, dosya biçimlerini, platform varsayımlarını ve hata davranışını belgeleyin.
- **Tasarımdan taşınabilir:** platforma özgü arka uçları yalıtın ve yerel çalıştırıcılarda test edin.
- **Güvenli başarısızlık:** bozuk girdiyi, kaynak sınırlarını, süreç hatalarını ve temizlik yollarını test edin.
- **Tekrarlanabilir sürümler:** temiz biçimde yeniden derleyin; yapıt (artifact) özetlerini ve sürüm notlarını yayımlayın.

## VII. Antivirüs Derlemesi ve Bütünlük

Sleelavirin antivirüs paketi; yerel bir SHA-256 tarayıcısı, isteğe bağlı bir
ClamAV bağdaştırıcısı, kaynak/dizin bütünlük bildirimleri ve platform paketi
sağlama toplamları içerir. Paketi depo kökünden derleyip test edin:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Bütünlük bildirimi `antivirus/build/` altında oluşturulur; izlenen kaynak ve
dokümantasyon girdilerini ile korunan imza dizini dosyalarını kapsar, oluşturulan
çıktıyı ve değişken ClamAV önbelleğini dışarıda bırakır. ClamAV veritabanı anlık
görüntülerinin kendi bildirimleri vardır. Sürüm ZIP'leri `INTEGRITY-MANIFEST.json`,
`SHA256SUMS` ve bir `.sha256` yan dosyası içerir. İndirilen arşivlerin özetlerini
bağımsız olarak elde edilmiş güvenilir bir değere karşı doğrulayın: arşivin
yanında duran bir sağlama toplamı, yayımcı imzası değildir. Bkz.
[Sleelavirin kılavuzu](../../antivirus/README.md),
[güvenlik kontrol listesi](../../antivirus/SECURITY.md) ve
[antivirüs derleme iş akışı](../../.github/workflows/antivirus-binaries.yml).

Bir sürüm, ilgili CI işleri tam sürüm commit'i üzerinde başarıyla tamamlanana ve
ortaya çıkan yapıtlar denetlenene kadar doğrulanmış sayılmaz.


## VIII. SLeeLa Editör Temaları — Vim ve Emacs

SLeeLa, **Vim** ve **Emacs** için profesyonel koyu renk şemalarından oluşan koordineli bir takım ve yerel bir C/C++ palet aracı içerir. Özgün **American Professional** şeması varsayılan olarak kalır. Farklı kültürel ve kurumsal tasarım ilhamları için on tamamlayıcı palet sunulur; bunlar yorumlayıcı tasarım paletleridir, herhangi bir ulusun, kültürün, mahkemenin veya kurumun resmî temsili değildir.

### On Bir Şema ve Renkleri

| Şema | Arka plan | Ön plan | Anahtar sözcükler | Türler | Dizgiler | Yorumlar |
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

#### Ulusal bayrak renkleri (kaynak palet)

Yunan ve Türk şemaları, iki ülkenin **resmî ulusal bayrak renklerinden** türetilmiştir ve burada yayımlanmış tam hex değerleriyle gösterilmiştir. Yukarıdaki şema arka planları/vurguları, bu kaynak renklerin koyulaştırılmış, editöre uygun yorumlarıdır — aşağıdaki tablo "ulusal olarak ilan edildiği haliyle renkler" başvurusudur.

| Ülke | Renk | Hex | Dayanak ve anlam |
|---|---|---|---|
| **Yunanistan** | Mavi | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | Ulusal bayrak mavisi (851/1978 sayılı Yasa). Gökyüzü ve denizler; hanedan armacılığında *azure* olarak tanımlanır. |
| **Yunanistan** | Beyaz | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Ulusal bayrak beyazı. Bağımsızlık mücadelesinin saflığı; haç, Doğu Ortodoks Hristiyanlığını belirtir. |
| **Türkiye** | Kırmızı (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | Ulusal bayrak kırmızısı (Türk Bayrağı Kanunu). Zemin; gelenekte şehitlerin kanı ve Osmanlı egemenliği. |
| **Türkiye** | Beyaz | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Ulusal bayrak beyazı. Ay ve yıldız — barış, saflık ve Türk/İslam mirasının simgesi. |

Şemalar, her editörün ön planını ulusal **beyaz** üzerine; anahtar sözcüklerini, imlecini, seçimini ve durum satırını ise tam ulusal **mavi** (`#0D5EAF`) veya **kırmızı** (`#E30A17`) üzerine oturtur; böylece önemli olan renkler tam anlamıyla ulusal renklerdir. Bunlar yorumlayıcı tasarım paletleridir; bayrak renkleri başvurusu köken göstermek içindir ve iki devletten hiçbirinin resmî temsili değildir. Renk anlamları, Helen Cumhuriyeti Dışişleri Bakanlığı'nın devlet sembolleri açıklamasını ve Türk Bayrağı Kanunu geleneğini izler.


### Renk Örneği Tablosu

Aşağıdaki palet hücreleri **SVG örnekleri** kullanır; böylece GitHub boş HTML renk hücreleri yerine gerçek renkleri gösterir. Her şerit altı rengi şu sırayla gösterir: arka plan, ön plan, anahtar sözcükler, türler, dizgiler ve yorumlar. Tam hex kodları, yukarıdaki başvuru tablosuyla aynı şema sırasıyla her örneğin yanında listelenmiştir.

| Şema | Gösterilen palet renkleri | Hex kodları (arka plan · ön plan · anahtar sözcükler · türler · dizgiler · yorumlar) |
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

İşlevler, sabitler, seçimler, tanılamalar ve önişlemci yönergeleri için ek vurgu renkleri kullanılır. Rus ve Slav paleti, daha sakin ve profesyonel bir görünüm için kasıtlı olarak orta yoğunlukta arduvaz, mavi, adaçayı, yakut ve antik altın tonlarını kullanır.

### Vim Şemalarını Kurma

Depo kökünden:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

SLeeLa sözdizimi vurgulamasını etkinleştirmek ve bir şema seçmek için `~/.vimrc` dosyasına aşağıdakini ekleyin:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Bu renk şeması adlarından tam olarak birini seçin:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Örneğin orta yoğunluklu Rus ve Slav paleti için `colorscheme sleela-russian-slavic`, Helen ve Al Bayrak paletleri için ise `colorscheme sleela-greek` / `colorscheme sleela-turkish` kullanın. Vim'i yeniden başlatın veya yapılandırmanızı `:source $MYVIMRC` ile yeniden yükleyin.

### Emacs Şemalarını Kurma

Yerel bir tema dizini oluşturun ve mod ile tema dosyalarını depo kökünden kopyalayın:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Bunu `~/.emacs` veya `~/.emacs.d/init.el` dosyasına ekleyin:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Başka bir şema seçmek için `sleela-american` yerine `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` veya `sleela-sleela` yazın. Emacs temaya güvenip güvenmediğinizi sorarsa, kabul etmeden önce yerel tema dosyasını inceleyin.

### Yerel C/C++ Paletlerini Önizleme

Küçük yerel palet aracı şemaları listeleyebilir, terminal renklerini yazdırabilir ve tek bir rengi sorgulayabilir:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Geriye dönük uyumluluk için `--theme` olmadan `--palette` ve `--hex COLOR` American Professional şemasını seçer. Derleme ve test sonuçları hedef sistemde ve CI'da denetlenmelidir; bir komutun belgelenmesi, onun başarıyla geçtiğinin kanıtı değildir.

Tam palet açıklamaları ve araç ayrıntıları için [editör entegrasyon kılavuzuna](../../linux-editors/README.md) bakın.

## IX. Katkı
1. Dokümantasyon dizinini ve test politikasını okuyun.
2. Olumlu ve olumsuz testlerle odaklı değişiklikler yapın.
3. Önce hedefli testleri, ardından geniş paketi çalıştırın.
4. İşletim sistemi/araç zinciri ayrıntılarını bildirin; atlanan kontrolleri başarılı gibi tanımlamayın.

SLeeLa, **MEARVK LLC** tarafından geliştirilmektedir. Tarihsel açılış sayfası içeriği [`docs/README-LEGACY.md`](../../docs/README-LEGACY.md) dosyasında korunmuştur.

## X. AE6E66 — Round Public E-posta Vergisi Beklentisi

AE6E66 girişimi için projenin ifade ettiği beklenti, her nitelikli Round Public e-postasıyla ilişkilendirilen önerilmiş bir vergi tutarı olarak **Round Public e-postası başına $300,000,000.00 USD**'dir.

Bu ifade yalnızca bir proje beklentisini kaydeder. Bir yasal vergi tarhiyatı, hükümet emri, mahkeme kararı veya bağımsız olarak tespit edilmiş bir vergi borcu **değildir**. Herhangi bir fiilî vergi için yetkili hukuki merci, yargı yetkisi, tanımlar, usul ve yetkili bir hükümet kararı gerekir.


## XI. Alfabe ve Karakter Kümesi Çalışma Zamanı

SLeeLa'nın alfabe dokümantasyonu ve yerel karakter çalışma zamanı alphabet/sleela/ ve lib/alphabet/native/ içinde tutulur. Yerel C++17 testleri, katı UTF-8 skaler kodunu çözme/kodlama, C<n> kapasite imzaları ve sınırlı karakter kümesi üyeliğini kapsar:

    make -C lib/alphabet/native test

Deneysel UTF-4088 kataloğu ve SLeeLa arama bağdaştırıcısı utf-4088/SLeeLa-ALPHABET-INTEGRATION.md dosyasında belgelenmiştir. UTF-4088 kimlikleri ayrı bir ad alanıdır; Unicode skaler değerleri değildir. Derleyici ve SLVM bağlama çalışmaları bu yerel temellerden ayrı olarak açıkça izlenir.

## XII. AutoCAD Creator™

[AutoCAD Creator™](../../autocad/creator/README.md), tamamen SLeeLa ile yazılmış etkileşimli bir 2B CAD
uygulamasıdır. Kardeş AutoCAD Renderer bir planı tek seferde çizime dönüştürürken, Creator
canlı bir 2B AutoCAD belgesini **oluşturmanıza ve değiştirmenize** olanak tanır — duvarlar,
odalar, ölçüler ve notlar ekleyin, ardından yerleştirdiklerinizi taşıyın, yeniden
etiketleyin, katmanını değiştirin, silin ve geri alın — ve sonucu gerçek bir AutoCAD DXF çizimi olarak
(R12 metin; AutoCAD / LibreCAD'de açılır) **kaydedin**.

Grafik arayüzü, JavaFX değil, SLeeLa'nın kendi platformlar arası UI araç takımı olan
**SleelaUI™** üzerine kuruludur ([`lib/user-interface`](../../lib/user-interface) altında) ve
X11 / Cocoa / Win32 üzerinde gerçek bir yerel pencere açar. Tasarım tek bir kuralı izler:
**"SleelaUI sunar; SLeeLa karar verir"**: GUI hiçbir zaman geometri hesaplamaz veya
DXF yazmaz. Her araç, düzenlenebilir belgeye ve geri alınabilir bir komut geçmişine sahip olan yetkili
[`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela) bileşenine bir oluştur / değiştir / sil / geri al / kaydet
niyeti iletir; tuval ise yalnızca Creator'ın tuttuğu geometriyi önizler.

Yeniden kullanılabilir düzenleme motoru [`lib/autocad`](../../lib/autocad) içinde bulunur —
[`SLCadEntity`](../../lib/autocad/SLCadEntity.sleela),
[`SLCadDocument`](../../lib/autocad/SLCadDocument.sleela),
[`SLCadCommand`](../../lib/autocad/SLCadCommand.sleela) ve
[`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela) — ve uçtan uca başsız (headless) çalışır.
GUI kabuğu ve demo [`autocad/creator`](../../autocad/creator) altındadır:

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

Bir ekranda canlı düzenleme penceresi açılır; başsız bir sunucuda ise çizimi bellekte oluşturur, bir rapor ve DXF yazdırır ve temiz biçimde çıkar.


## XIII. UTF-4088 Girdi Haritası Sıralaması

Özelleşmiş UTF-4088 kataloğu, uzamsal girdi haritası dolaşımını ve soyut basınç yinelemelerini [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md) dosyasında tanımlar. National Lien konu dizini politikası [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md) dosyasındadır. Harita koordinatları, basınç düzeyleri (1–50), kalıcı katalog kimlikleri ve alfabetik sıralar bağımsız alanlardır; hiçbiri sessizce bir diğerinin yerine kullanılmamalıdır. Basınç algoritmik bir yineleme düzeyidir, fiziksel bir nicelik değildir.


## XIV. Yeni ve Genişletilmiş Paket Dizini

Bu eklenmiş dizin, yakın zamanda genişletilen paketleri, alan kütüphanelerini ve
kaynak koleksiyonlarını öne çıkarır. Girdiler paket adına veya konuya göre
alfabetik olarak gruplanmıştır. Açıklamalar deponun konumunu belirtir; uygulama ve
doğrulama durumu her paketin kendi dokümantasyonunda ve testlerinde denetlenmelidir.

### A–C

- **Advantage** — `lib/advantage/` ve `advantage/`: bir modele göre eşsiz bir
  temettünün avantajlandırılması, katmanlı bir Sequitur soğanı olarak; beş konu
  alt klasörüyle — `math/` (ortak kâr maksimizasyonu, Nash pazarlığı, ikili
  newsvendor), `physics/` (güven alanı metriği/jeodeziği/teması/SDE'si,
  işaretlenmiş bir modelleme benzetmesi), `integration/` (dikey entegrasyon
  bölmeleri ve bir sofistikasyon puanı), `grading/` (modüller ve kümeler için
  A–D sofistikasyon devresi derecelendiricisi) ve `output/` (terminal /
  CommonRails / soket yayını). Yerel bir C/C++ tüketicisi (`advantage/`) modeli
  işlenmiş ticaret örneklerine bağlar. Bkz. [`lib/advantage/README.md`](../../lib/advantage/README.md)
  ve [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md).
- **Alphabet ve UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` ve
  `utf-4088/`: alfabe katalogları, yerel karakter kümesi desteği, girdi haritası
  sıralaması ve UTF-4088 entegrasyon dokümantasyonu.
- **AutoCAD Creator** — `autocad/creator/` ve `lib/autocad/`: etkileşimli
  2B çizim iş akışları ve yeniden kullanılabilir CAD belge/düzenleme bileşenleri.
- **Ballum** — piksel düzeyinde görüntü denetimi: piksel başına ayarla/oku, bölge
  blit, tüm ekran için parlaklık/sıcaklık/gama, işaretçi/klavye girdisi ve toplam,
  sahipli, hızlı piksel başına denetim için bir **tam ekran çerçeve tamponu
  yüzeyi** (Linux/X11'de override-redirect pencere + tek `XPutImage`/MIT-SHM
  sunumu). Yerel çekirdek `ballum/` altında, SLeeLa cephesi `lib/ballum/` içinde
  ve `impl/` altında bir VM köprüsü bulunur; bkz. [`ballum/README.md`](../../ballum/README.md)
  ve çalıştırılabilir [`ballum/example/`](../../ballum/example/).
- **CDMA** — `cdma/` ve `lib/cdma/`: CDMA ile ilgili teknik malzeme ve
  SLeeLa kaynak kütüphanesi çalışmaları.
- **Character** — `lib/character/`: karakter profilleri ve yeniden kullanılabilir
  karakter istatistikleri; biyografik profil örnekleri dahil.
- **Coorenagraph** — `lib/coorenagraph/`: etik muhakeme, ahlaki sözlük,
  müzakere ve ilgili kavramlar; paketin README'sine ve kaynak dosyalarına bakın.

### D–G

- **Yönetişim kaynak modelleri** — [lib/governance](../../lib/governance/README.md), öneriler, anonim elle oylar ve toplu sayımlar için bildirimsel SLeeLa kaynak biçimlerini tanımlar. Önerilen @vote ek açıklaması ve elle süreç [registry/DEMOCRATIC-VOTING-SYNTAX.md](../../registry/DEMOCRATIC-VOTING-SYNTAX.md) dosyasında belgelenmiştir; güncel derleyiciler bunu etkisiz yorum üstverisi olarak ele alır.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` dizininden başlayarak mevcut sürümlü HTTP
  paket dizinleri: sürüme özgü protokol belirtimleri ve uygulamaları. Tam
  kapsam ve doğrulama durumu için her sürümün dokümantasyonuna bakın.
- **Uluslararası Ceza Mahkemesi** — `lib/international-criminal-court/` ve
  `INTERNATIONAL.CRIMINAL.COURTS.md`: ilgili alana özgü kaynak ve
  dokümantasyon.

### O–S

- **OFDMA** — `ofdma/`: dikgen frekans bölmeli çoklu erişim ile ilgili
  teknik malzeme ve C/C++ geliştirme kaynakları.
- **Felsefe ve Conversation Mask modülü** — [`philosophy/`](../../philosophy/README.md)
  yerel C/C++ temellerini içerir ve [`philosophy/module/`](../../philosophy/module/README.md)
  bir sonraki modül katmanlarını üç açık alanda düzenler:
  türlü, tarihli gözlemler, olay sonları, yarı ömür ayarları, köken ve rıza için
  [`data/`](../../philosophy/module/data/README.md); belirlenimci doğrulama, ilgi
  bozunumu, uyumlu karşılaştırmalar ve rızayla süzülmüş sunum için
  [`helper/`](../../philosophy/module/helper/README.md); ve destekleyici, klinik
  olmayan bir konuşma arayüzü için
  [`psychiatry-friend/`](../../philosophy/module/psychiatry-friend/README.md).
  SLeeLa'ya dönük tutamaçlar ve oluşturulmuş Ağaç, daha geniş felsefe
  paketiyle birlikte [`lib/philosophy/`](../../lib/philosophy/README.md) içinde yer
  alır. Modül açık girdiye ve rıza önceliğine dayanır: insanları veya medyayı
  edilgen biçimde izlemez, tanı koymaz, özel niyeti çıkarsamaz, insan değerini
  puanlamaz ve ilaç değişikliği önermez. Olay bağlantıları köken ve gezinmeyi
  korur; bağımsız doğrulamayı değil.
- **Synchro** — yerel çalışma zamanı/derleyici entegrasyonu ve paket dağıtımı ile
  ölçümle ilgili işlevsellik; ilgili `impl/` kaynağına ve derleyici
  uygulamasına bakın.
- **SleelaUI / terminal arayüzü** — `lib/user-interface/` ve
  `sleela-terminal` projesi: SLeeLa UI entegrasyonu ve terminal odaklı
  etkileşim bileşenleri.

### U–W

- **Birleşmiş Milletler** — `united-nations/`: alana özgü depo malzemeleri.
- **Wi-Fi 4, Wi-Fi 5 ve Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`,
  `wi-fi-6/` ve ilgili `lib/wifi/` kaynak dizinleri: nesle özgü
  kablosuz ağ malzemesi ve yapılandırma kaynakları.

### Paket Dizini Bakımı

Bir paket eklendiğinde veya önemli ölçüde genişletildiğinde, girdisini burada
uygun alfabetik gruba ekleyin, kanonik dizinine ve README'sine bağlantı verin ve
yalnızca işlenmiş kaynaklarca desteklenen yetenekleri açıklayın. Bu dizini
eklemeli tutun: paket dizini güncellemelerinin bir parçası olarak mevcut README
bölümlerini kaldırmayın veya yeniden yazmayın.


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

US Democratic and US Policy.

---


## XVI. Ulusal Bilim Marka Görselleri

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. UTF-4088 Milyon Girdili Deneysel Katalog

[UTF-4088 katalog üreticisi](../../utf-4088/tools/generate_catalogue.py), 1.048.576 girdili deneysel bir simge kataloğunu belirlenimci biçimde oluşturur. Belgelenmiş sıkıştırılmış biçim [utf-4088/FORMAT.md](../../utf-4088/FORMAT.md) dosyasında, üretim ve doğrulama sözleşmesi ise [utf-4088/GENERATION-RESULTS.md](../../utf-4088/GENERATION-RESULTS.md) dosyasında açıklanmıştır.

Üreticiyi Python 3 ile çalıştırın:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Üretilen ikili dosya 16.777.244 bayttır ve 20.000.000 baytın altında kalacak şekilde tasarlanmıştır. Birim testleri ve bir GitHub Actions iş akışı girdi sayısını, glif benzersizliğini, boyutu ve SHA-256'yı doğrular; başarılı çalıştırmalar ikili dosyayı ve özeti `utf4088-million-catalogue` iş akışı yapıtı olarak yayımlar. Üretilen ikili dosya kasıtlı olarak Git geçmişinin dışında tutulur. UTF-4088 deneysel olarak kalır ve Unicode uyumlu değildir.

---

# SLeeLa
<p align="center"><img src="../../images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Son Eklemeler — Pi Sabiti, Modium/Registry Başlangıcı, sleelactl Math ve `@native`

Bu bölüm, 2026-11 çalışması sırasında eklenen özellikleri, en yeni gruplar başta olmak üzere kaydeder. Bağlantılı kaynak dosyaları ve doğrulama durumları belirleyicidir; inceleme altındaki değişiklikler `master` dalına ulaşana kadar birleştirilmiş olarak tanımlanmaz.

### 100.000.000 basamaklı Pi katalog sabiti

Depo, [`pi-generator/pi.100000000.digits`](../../pi-generator/pi.100000000.digits) konumunda π'nin doğrulanmış **yüz milyon basamaklı** bir değerini (iyi biçimlendirilmiş bir ondalık, `3.14159…`) ve onun yanında 57.344 basamaklı bir değeri [`pi-generator/pi.57344.digits`](../../pi-generator/pi.57344.digits) içerir. Her ikisi de bağımsız olarak gerçek π olduğu doğrulanmıştır — bkz. [`pi-generator/VERIFIED.md`](../../pi-generator/VERIFIED.md); bu dosya kontrolleri (ders kitabı öneki, kesirli konum 762'deki Feynman noktası, ünlü milyonuncu basamak, `lib/math/SLPiReference.sleela` ile uyum ve tüm dosya üzerinde basamak sıklığı çözümlemesi) ve tek seferlik bir yeniden üretim betiğini belgeler.

### Modium™ / Registry™, Pi sabitini alt sistem başlangıcında yükler

Ayrılmış alt sistem giriş noktalarında — `sleela registry` ve `sleela modium` (ve kurucunun ilk çalıştırması), her `sleela run` çağrısında **değil** — Modium ve Registry alt sistemleri büyük π katalog sabitini yükler. Etkileşimli bir terminalde, normal çıktı başlamadan önce yerinde silinen geçici kırmızı bir `Loading Modium™` / `Loading Registry™` satırı yazdırırlar; etki kesinlikle TTY ile sınırlandırılmıştır, dolayısıyla borulanmış ve test çıktıları bayt bayt değişmeden kalır. Baytlar değil, bulucu (locator), `Math` kayıt anahtarı varsayılanları `piReferenceLargePath` ve `piReferenceLargeDigits` olarak tohumlanır; hem SLeeLa modelinde ([`lib/registry/SLRegistry.sleela`](../../lib/registry/SLRegistry.sleela)) hem de yerel tohumlayıcıda ([`registry/sleela_registry.c`](../../registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` ise `largePath()` / `largeDigits()` sunar. π sabiti, hem standart `make` kurucusu hem de rehberli "güzel" kurucu tarafından işletim sisteminin SLeeLa konumuna (`<data>/pi/`) kurulur. Ayrıntılar: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](../../pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — güzel kurucu ve yerel destekli bir `math` konusu

- Rehberli ("güzel") kurucu artık `make` kurucusuyla uyumlu olarak `sleelactl` (SLeeLa kontrol düzlemi) ve modül kayıt defterini kurar. Bkz. [`bin/SLEELACTL.md`](../../bin/SLEELACTL.md).
- `sleelactl`, çalışma zamanına derlenmiş yerel C/C++ konu kütüphaneleri için yeni bir **`subject`** (konu) doğası kazandı ve ilkini kaydeder: **`math`**. `sleelactl run math`, konunun referans demosu aracılığıyla yerel `__NativeMath` arka ucunu (`impl/subjects/math/math_native.cpp`) uçtan uca çalıştırır; `sleelactl info math` ona nasıl ulaşıldığını (`import math;`) açıklar. Konu doğası yalnızca `run info` sunar — kasıtlı olarak bir `check` fiili yoktur, çünkü `sleela check`, SHEET ile kaydedilmiş yerel konuları yüklemez.

### `@native`, Native Drive ve çok dilli yürütme

Yeni bir **`@native`** metot ek açıklaması, sık çalışan bir metodun önceden (ahead-of-time) yerel derlenmesini ister. Yeni `sleela aot` komutu `@native` metotlarını bulur, her birinin desteklenen *aktarılabilir alt küme* içinde olduğunu doğrular ve C/C++ üretir — henüz çeviremediği herhangi bir yapıda (kesin bir gerekçeyle reddederek) **güvenli biçimde başarısız olur (fail-closed)**; böylece yarım çevrilmiş bir metot asla üretilmez.

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

`@native`, AOT işlemi için bir istek olarak kalır; desteklenmeyen yapılar üretimden önce reddedilir. [Native Drive](../../impl/native-drive/README.md); ana bilgisayar derleyicileri (MSVC dahil) aracılığıyla çok birimli C/C++ yürütülebilir dosya ve paylaşımlı kütüphane derlemelerini, desteklenen SLeeLa AOT çıktısını, yerel yükleyici ABI'sine karşı sembol incelemesini, kabuksuz süreç yürütmeyi, stdin/stdout/stderr işlemeyi, çok aşamalı boru hatlarını ve XClass'ın üretim yükleyicisine devredilmesini destekler. `sleelactl`, derleme/çalıştırma komutlarını sunar ve XClass incelemesini ile açık systemd birimi oluşturma/kurmayı `inspect native-drive` üzerinden yönlendirir. VM, zorunlu ana bilgisayar yetkilendirmesi ve nesil denetimli tutamaçlarla sınırlı bir yerel modül tutamaç tablosuna sahiptir. Sözdizimi 3.0 artık `nativeOpen`, `nativeCall` ve `nativeClose` ekler: yalnızca sona eklenen bayt kodu dağıtımı, ayrıca sürümlenmiş bir null/bool/int64/double/UTF-8 ABI'sini aşar; VM'nin sahip olduğu diziler, yapılar (struct) ve diğer bileşik değerler yorumlayıcının içinde kalır ve aynı ada sahip bayt kodu geri dönüşünü kullanabilir. Eklentiler isteğe bağlı olarak, iç içe VM yürütmesi aracılığıyla tam SLeeLa işlevlerini çağırmak için sürümlenmiş bir ana bilgisayar geri çağrı tablosu alabilir; bunun için askıya alınmış yığın GC köklendirmesi, açık bir ana bilgisayar geri çağrı adı izin listesi vardır ve özyinelemeli yerel API çağrıları reddedilir. Yerel kod, bir kum havuzu (sandbox) değil, güvenilen süreç içi kod olarak kalır. Kaynak süreç-politikası ek açıklamalarının sürümlenmiş bir şeması vardır ancak henüz ayrıştırılmaz/uygulanmaz. Ayrıntılar, güvenlik sınırları ve yedi iş akışı defteri: [Native Drive](../../impl/native-drive/README.md) ve [`pi-generator/AT-NATIVE.md`](../../pi-generator/AT-NATIVE.md).


## XIX. Uluslararası README Sürümleri

SLeeLa, uluslararası sürümündeki 50 pazar için ülkeye özgü README giriş noktaları sağlar. Her ülkenin `README.<CC>.md` adlı tek bir dosyası vardır; birden çok dilin söz konusu olduğu yerlerde bunlar o ülkenin dosyası içinde gruplanır.

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

**Çeviri politikası:** Bu dosyalar başlangıçta yerelleştirilmiş ülke giriş noktalarıdır. Girişleri ve dil başlıkları yerelleştirilmiştir; eksiksiz teknik başvuru ise kanonik İngilizce [README.md](../../README.md) içinde kalır. Bir ülke dosyası, her bölüm çevrilene, teknik tanımlayıcılar ve bağlantılar denetlenene ve sürüm dil incelemesinden geçene kadar eksiksiz bir çeviri olarak tanımlanmamalıdır. Bu ilk aşamadaki dil kapsamı temsilidir ve anayasal olarak tanınan, bölgesel, yerli veya işaret dillerinin her birinin tam olarak çevrildiği iddiası değildir.

## Ek — Korunan Bölgesel Rehberlik

Bu ek, önceki kısa Türkçe kılavuzdan, kanonik README'de ayrı bölümler olarak bulunmayan pratik rehberliği korur. Herhangi bir uyuşmazlıkta kanonik İngilizce [README.md](../../README.md) önceliklidir.

### Derleyici mimarisi ve derleme hattı

SLeeLa; kaynak dilini, derleyici ön ucunu, ara gösterimi, kod üretimini ve çalışma zamanını ayrı katmanlar olarak ele alır. Derleyici sürümü, sözdizimi sürümü ve VM/ISA sürümü birbirinden farklı sürüm eksenleridir. Bir özelliğin belirtimde yer alması, her hedefte uygulandığı ve doğrulandığı anlamına gelmez.

Tipik işlem hattı:

1. **Kaynak:** sürüm bildirimi ve program bildirimlerini içeren UTF-8 `.sleela` dosyaları.
2. **Sözcük ve sözdizimi çözümlemesi (lexing ve parsing):** metnin token'lara ve sözdizimsel yapıya dönüştürülmesi.
3. **Anlamsal çözümleme:** adların, sembollerin, türlerin ve dil kurallarının denetlenmesi.
4. **Ara gösterim:** sonraki indirgeme (lowering) aşamaları için yapılandırılmış program biçimi.
5. **Kod üretimi:** desteklenen VM veya ara hedefler için çıktı oluşturulması.
6. **Çalışma zamanı:** derlenmiş programın mevcut bellek ve kontrol akışı mekanizmalarıyla yürütülmesi.

Kaynak biçimi ve dosya yaşam döngüsü için [source documentation](../../markdown/SOURCE.md) ve [compiler documentation](../../markdown/COMPILER.md) belgelerine bakın.

### Hedefler, profiller ve yürütme sınırları

SLeeLa, kaynak kodunu derleme ve yürütme hedeflerinden ayırır. Belgelenen hedef ailesi `SLVM`, `SLJVM` ve `SLIR` içerir; `NATIVE` kısıtlı ve koşullu bir hedeftir, her platformda kullanılabilir olduğu varsayılmamalıdır. `BASIC_COMPLETE` ve `ADVANCED_TOTAL` profilleri farklı kapsamları belirtir. Her derlemede gerçek hedefi, profili ve platform desteğini doğrulayın.

Bir referans (reference) bir dosyayı, modülü veya kaynağı tanımlar; tek başına onu yürütme yetkisi vermez. Program dosyalar, harici kütüphaneler, yerel bağlamalar (native bindings) veya sistem kaynakları kullanıyorsa yetkilendirme sınırları ve yürütme sorumluluğu açıkça belirlenmelidir. Dokümantasyondaki bir açıklama tek başına çalışma zamanı güvenlik garantisinin kanıtı değildir; uygulamayı ve ilgili testleri inceleyin.

### Kaynak yaşam döngüsü ve hata ayıklama

İşlevler veya iş parçacıkları arasında paylaşılan kaynakların sahipliğini, yaşam süresini ve eşzamanlama sorumluluğunu belgeleyin. Yerel değişkenleri, paylaşılan nesne durumunu, VM tutamaçlarını (handles) ve serileştirilebilir verileri birbirinden ayırın. VM'ye yerel bir tutamaç, kararlı bir dosya biçimi veya ağ iletisi değildir. Modülün tanımladığı serileştirme arayüzünü kullanın; geçersiz, eksik ve sınır değerlerini test edin.

Hata ayıklarken commit, işletim sistemi, mimari ve araç zinciri bilgilerini kaydedin. Önce ilgili en küçük hedefi derleyin, tam hata çıktısını saklayın, derleyici hatalarını çalışma zamanı hatalarından ayırın ve yeniden üretilebilir en küçük örneği hazırlayın. Başarılı bir derleme tek başına çalışma zamanı davranışının doğru olduğunu kanıtlamaz.

Doğrulama raporu tam commit'i, işletim sistemi/mimariyi, derleyici sürümlerini, tam derleme/test komutlarını, geçen/başarısız/atlanan sayılarını ve bilinen kısıtlamaları içermelidir. Atlanan veya çalıştırılmayan testler başarılı sayılmamalıdır.

### Modül API sözleşmeleri, uyumluluk ve test stratejisi

Yeni bir API eklenirken girdiler, çıktılar, hata koşulları, veri sahipliği ve olası yan etkiler belgelenmelidir. Örnekler hem olağan kullanımı hem de geçersiz veya eksik girdinin ele alınışını göstermelidir. Uygulama ve ilgili testlerce doğrulanmayan davranışlar garanti olarak sunulmamalıdır.

Testleri derleyici hattı aşamalarına göre ayırın: sözcük/sözdizimi çözümlemesi, anlamsal çözümleme, IR indirgemesi, kod üretimi, VM yürütmesi ve yerel entegrasyon. Her gerileme (regression) durumu için yeniden üretilebilen en küçük girdiyi, beklenen sonucu ve gerçek sonucu saklayın. API değişiklikleri geriye dönük uyumluluğu, geçiş adımlarını ve mevcut örnekler üzerindeki etkileri açıklamalıdır.

### Yerelleştirme ve teknik tutarlılık

Teknik tanımlayıcılar, komutlar, yollar, sözdizimi ve kod blokları tüm dil sürümlerinde aynı kalmalıdır. Açıklayıcı metni çevirin; kaynak kodda kullanılan token'ları veya adları çevirmeyin. Dil ve bölgesel kullanım açıkça belirtilmelidir.

Her güncellemeden sonra Markdown bağlantılarını, başlık hiyerarşisini, kod çitlerini (code fences) ve örnekleri denetleyin. Kanonik İngilizce belgeler değiştiğinde yalnızca giriş bölümünü değil, ilgili bölümlerin tamamını karşılaştırıp güncelleyin. Dil veya teknik incelemeden geçmemiş içerikleri açıkça işaretleyin.

**Çeviri durumu:** I–XIX bölümleri Türkçeye çevrilmiştir. Teknik tanımlayıcılar, komutlar, kod örnekleri, dosya adları, bağlantılar ve adresler korunmuştur. Ana dili Türkçe olanlar tarafından son bir dil incelemesi önerilir.
