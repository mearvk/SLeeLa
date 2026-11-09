# SLeeLa 3.0 — Türkiye (TR)

## Türkçe

SLeeLa; yerel bir sistem programlama dili, derleyici, çalışma zamanı ve sanal makine ekosistemidir.

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>

**The Now. The Ingredient.** Bir programlama dili yalnızca sözdiziminden ibaret değildir. Yazılımı kullanılabilir kılan derleyici, çalışma zamanı, kütüphaneler, araçlar ve platform sözleşmeleri de dilin parçasıdır.

**SLeeLa'nın çalışma alanları.** SLeeLa dört boyutu ele alır: **Math**, **Object**, **Science** ve **Reach**. Bunlar sırasıyla hesaplanabilir olanı; var olanların kataloğunda tanımlanan nesneyi; gerçeğin disiplinli biçimde araştırılmasını; ve bir sistemin yeteneklerinin başka sistemlere ve insanlara ulaştırılmasını ifade eder. **Ulusal çıkarlar ayrı bir konu olarak korunur**; bu dört boyut ulusal çıkarların yerine geçmez. Bir konu bir ulusun çıkarlarını etkiliyorsa bu ilişki korunmalı ve örneğin sınıf meta veri alanları `metaOrigin` ve `metaNationalSchedule` içinde dikkatle belgelenmelidir.

## Başlangıç

- [Yerel uygulamayı derleme](impl/README.md)
- [Sanal makineler ve Modium — kullanıcı kılavuzu](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [SLeeLa kaynak kodundaki CPU yığını](lib/cpu/CPU.md): mantık kapılarından ALU'ya, çekirdeklere, işletim sistemine ve programlara uzanan zincir; 130'dan fazla CPU profili ve [voltaj, değişken frekans ve gelişmiş zamanlama parametreleri](lib/cpu/TIMING.md).
- [Dokümantasyon dizini](docs/README.md)
- [CSPML veri kaynağı ve Salt & Salary araştırma referansı](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md): ABD'deki konumlar, sosyoekonomik koşullar, iş gücü, eğitim ve devlet okulları hakkındaki harici verilerin kökeni ve entegrasyon rehberi.
- [Oluşturulmuş API belgeleri](docs/api/README.md) ve [API kataloğu](api/README.md)
- [Karşılaştırma standartları](comparison/README.md): SLeeLa VM, Java/JVM ve yerel C/C++ için tür, bellek, ABI ve uyumluluk standartları.
- [Recursion Stanchion — C ve C++ referans prototipleri](recursion/stanchion/README.md)
- [Platform desteği ve doğrulama matrisi](docs/PLATFORM-SUPPORT.md)
- [Marka kılavuzu](docs/BRAND-GUIDE.md)
- [Ürün ve görsel varlıklar](merchandise/README.md)
- [Test politikası ve sürüm ölçütleri](markdown/TESTING.md)

## Teknik dokümantasyon kapsamı

Kanonik [README.md](README.md), özellik kataloğunun tamamını, dil ve çalışma zamanı referansını, derleme ve kurulum talimatlarını, API bağlantılarını, örnekleri ve sürüm notlarını içerir. Ayrıca `advantage` kütüphanesi, döngü zamanlama ek açıklamaları, SleelaDoc, XML DOM modeli, genişletilmiş tamsayı türleri, Preonem'ler, Vim ve Emacs düzenleyicileri, Sleelavirin, UTF-4088, AutoCAD Creator, paket dizini ve Modium/Registry ile `@native` geliştirmeleri hakkında güncel bilgiler sunar.

## Derleme ve test

Gereksinimler: C/C++ araç zinciri (GCC veya Clang), GNU Make ve bildirim/iş akışı araçları için Python 3. Linux üzerinde grafik ürünleri ayrıca X11/Xrandr geliştirme paketlerine ihtiyaç duyabilir.

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

Tüm Make hedefleri, sistem geneline kurulum ve yönetici yetkisi olmadan kullanıcıya özel kurulum için kanonik README'ye ve [kurulum kılavuzuna](install/README.md) bakın. Tek bir bileşenin başarıyla derlenmesi, bütün kütüphanelerin veya platformların sürüme hazır olduğunu göstermez.

## Dil, çalışma zamanı ve mühendislik ilkeleri

Normatif dilbilgisi [`SLEELA.syntax`](SLEELA.syntax) dosyasındadır. Depo **3.0** sözdizimini belgeler; derleyici sürümü ile kaynak kodu sözdizimi sürümü birbirinden bağımsızdır. Belgelerde `@mix` alanları, genişletilmiş tamsayı aileleri, Preonem'ler, SleelaDoc ve API oluşturma gibi özellikler açıklanır. Uygulama ve doğrulama durumu her modül için ayrı değerlendirilmelidir.

Mühendislik ilkeleri şunlardır: iddialardan önce kanıt, bileşen sınırlarında açık sözleşmeler, tasarımdan itibaren taşınabilirlik, hatalarda güvenli davranış ve tekrarlanabilir sürümler. Sleelavirin antivirüs paketi yerel SHA-256 doğrulayıcısı, isteğe bağlı ClamAV bağdaştırıcısı ve bütünlük bildirimleri sağlar. Bir sürüm ancak ilgili CI işleri tam sürüm commit'i için başarıyla tamamlandığında ve oluşturulan çıktılar incelendiğinde doğrulanmış sayılır.

## Düzenleyiciler ve katkı

SLeeLa, **Vim** ve **Emacs** için profesyonel koyu renk şemaları ve sözdizimi entegrasyonu ile yerel C/C++ palet aracı sağlar. Kurulum ayrıntıları [düzenleyici entegrasyon kılavuzunda](linux-editors/README.md) yer alır. Katkılar odaklı değişiklikler, olumlu ve olumsuz testler ve kullanılan işletim sistemi ile araç zincirinin açıkça belirtilmesini içermelidir. Atlanan kontroller başarılıymış gibi sunulmamalıdır.

## Diğer alanlar ve materyaller

Dokümantasyon ayrıca AE6E66 girişimini ve proje tarafından ifade edilen beklenen tutarı — yürürlükteki bir hukuki yükümlülük değil, yalnızca proje beklentisi olarak —; yerel karakter çalışma zamanını ve deneysel UTF-4088 kataloğunu; AutoCAD Creator™'ı; UTF-4088 giriş haritasını; genişletilmiş paket dizinini; Bitcoin Conjegeum materyallerini ve National SLeeLa Science görsellerini kapsar. Ayrıntılar, sınırlamalar ve bağlantılar kanonik README'nin ilgili bölümlerindedir.

## Son eklemeler — Pi, Modium/Registry ve `@native`

Depoda 100.000.000 ve 57.344 basamaklı, doğrulanmış Pi referans dosyaları bulunur. `sleela registry` ve `sleela modium` giriş noktaları katalog Pi sabitini her `sleela run` çağrısında değil, ilgili alt sistem başlatılırken yükler. Rehberli kurulum ayrıca SLeeLa kontrol düzlemi `sleelactl` aracını, modül kaydını ve yerel `math` alanını kurar.

`@native` metot ek açıklaması ve `sleela aot`, SLeeLa'dan yerel C/C++'a çeviri yolunun ilk aşamasıdır. Araç, işaretli metodun desteklenen alt kümeye girip girmediğini denetler; desteklenmeyen yapıları kısmen çevrilmiş kod üretmek yerine reddeder. Bu aşamada ek açıklama programın yürütme davranışını değiştirmez.

## Uluslararası README sürümleri

Depoda 50 pazar için ülkeye özgü README giriş dosyaları bulunur. Ülke kodlarının listesi [kanonik uluslararası dizinde](README.md#xix-international-readme-editions) yer alır.

---

**Çeviri durumu:** Türkçe sürüm giriş bölümünü ve dokümantasyonun ana alanlarını yerelleştirir. I–XIX teknik bölümlerinin tam metni henüz bütünüyle çevrilmemiştir; çeviri tamamlanana kadar normatif kaynak kanonik README'dir.


## X. Derleyici mimarisi ve derleme hattı

SLeeLa; kaynak dili, derleyici ön ucunu, ara gösterimi, kod üretimini ve çalışma zamanını ayrı katmanlar olarak ele alır. Derleyici sürümü, sözdizimi sürümü ve VM/ISA sürümü birbirinden farklı sürüm eksenleridir. Bir özelliğin belirtimde yer alması, her hedefte uygulandığı ve doğrulandığı anlamına gelmez.

Tipik işlem hattı:

1. **Kaynak:** sürüm bildirimi ve program bildirimlerini içeren UTF-8 `.sleela` dosyaları.
2. **Lexing ve parsing:** metnin token'lara ve sözdizimsel yapıya dönüştürülmesi.
3. **Semantik analiz:** adların, sembollerin, türlerin ve dil kurallarının denetlenmesi.
4. **Ara gösterim:** sonraki lowering aşamaları için yapılandırılmış program biçimi.
5. **Kod üretimi:** desteklenen VM veya ara hedefler için çıktı oluşturulması.
6. **Runtime:** derlenmiş programın mevcut bellek ve kontrol akışı mekanizmalarıyla yürütülmesi.

Normatif başvuru [SLEELA.syntax](SLEELA.syntax) dosyasıdır. Kaynak biçimi ve dosya yaşam döngüsü için [source documentation](markdown/SOURCE.md) ve [compiler documentation](markdown/COMPILER.md) incelenmelidir.

## XI. Tür sistemi ve nesne modeli

Temel kaynak türleri `int`, `double`, `boolean`, `String`, `void`, kullanıcı tarafından tanımlanan `struct` türleri ve desteklenen reference-type signatures biçimlerini içerir. Güncel belgelenmiş modele göre `int`, `long` ve `int64` işaretli 64-bit tamsayılardır. `int128` bir tamsayı türü olarak kabul edilir; ancak mevcut uygulama 64-bit temellidir ve tam 128-bit aritmetik olarak değerlendirilmemelidir.

`struct`, adlandırılmış alanlardan oluşan bir aggregate yapıdır. VM tarafından yönetilen struct handle değerleri dışarıya açılmış ham işaretçiler değildir. Atama ve parametre aktarımı aynı instance'ı paylaşır; eşitlik kimlik temellidir. Taşınabilir veri aktarımı için `structPack(instance)` ve `structUnpack(TypeName, json)` gibi belgelenmiş serileştirme yardımcıları kullanılmalıdır. VM'ye özgü handle, doğrudan ağ üzerinden taşınabilir bir değer değildir.

Sınıflar fields ve methods içerir. Geçerli yürütme modelinde fields paylaşılan class state, method-local variables ise thread'e özgüdür. Inheritance, nested classes ve generics, syntax-version gates kurallarına tabidir. Ayrıntılar için [International Language Reference](LANGUAGE.INTERNATIONAL.3.0.md) sayfasına bakın.

## XII. Kontrol akışı ve döngü zamanlaması

Dil; koşullu ifadeler, döngüler, method çağrıları ve recursion için sözdizimi sağlar. `@loopDelay` ve `@loopBudget` açıklamaları döngüden önce yazılır ve gerçek bekleme veya toplam zaman bütçesi belirler. Her ikisi de VM komutu `OP_SLEEP` (ISA 157) biçimine dönüştürülür; yerleşik `sleep(ms)` işlevi de aynı komutu kullanır.

```sleela
@loopDelay(ms = 250)
while (running) {
    processNext();
}
```

Bu örnek yalnızca sözdizimi biçimini gösterir; gerçek program gerekli identifiers ve enclosing class tanımlarını içermelidir. [SLEELA.syntax §32](SLEELA.syntax) ve [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md) belgelerine bakın.

## XIII. Standart kütüphane ve temel modüller

Depo, farklı olgunluk seviyelerine sahip modüller içerir:

- `lib/advantage` — bir modele göre sonucun katmanlı değerlendirilmesi; matematik, ikili ticaret ekonomisi, entegrasyon, derecelendirme ve yönlendirilebilir çıktı. Güven alanlarına ilişkin fizik anlatımı bir modelleme benzetmesidir; fizik yasası iddiası değildir.
- `lib/xml` — element, text, comment, CDATA ve processing-instruction düğümleri, attributes, parser ve serializer içeren XML belge ağacı.
- `lib/text` — `charAt`, `substring`, `indexOf`, `reverse` ve konumsal biçimlendirme gibi karakter hücresi tabanlı metin/string işlemleri.
- `lib/sleeladoc` — kaynak dokümantasyonunun çıkarılması ve CLI başvuru sayfaları. `sleela docs` konuları, paketleri ve sınıfları listeler; `sleela docs --generate [dir]` çevrimdışı kullanım için yerel metin külliyatı oluşturur.
- `lib/cpu` ve `lib/xlt` — CPU profilleri, mimari bileşenleri, model biçimi ve zamanlama başvuruları.
- `recursion/stanchion` — C/C++ referans prototipleri ve recursion örnekleri.

Bir modülün katalogda bulunması üretime hazır olduğu anlamına gelmez. Kullanımdan önce modül README'si, test kapsamı, platform kısıtları ve doğrulama kayıtları incelenmelidir.

## XIV. C/C++ entegrasyonu ve native sınırı

Depo, C ve C++ bileşenleri ile sınırlandırılmış bir native kod üretim yolu içerir. `@native` işaretçisi ve `sleela aot` komutu native geliştirme alanıyla ilgilidir; keyfî native kod çalıştırmak için genel izin oluşturmaz.

C/C++ entegrasyonu ABI, çağrı kuralları, bellek sahipliği, veri yerleşimi, hata yönetimi ve platform bağımlılıklarını açıkça tanımlamalıdır. Native kod ayrı bir güven sınırı olarak ele alınmalıdır. Bir reference veya declaration kaynağı tanımlar, ancak yürütme yetkisi vermez. [Native implementation](impl/README.md), [platform matrix](docs/PLATFORM-SUPPORT.md) ve [comparison standards](comparison/README.md) belgelerine bakın.

## XV. Derleme, kurulum ve platform desteği

Native build genellikle GCC veya Clang gibi bir C/C++ toolchain ve GNU Make gerektirir. Bazı araçlar Python 3 kullanır; Linux GUI bileşenleri X11 ve XRandR geliştirme paketlerine ihtiyaç duyabilir.

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

Sistem genelinde kurulum genellikle:

```sh
sudo make install
```

Yalnızca geçerli kullanıcı için kurulum:

```sh
make install PREFIX="$HOME/.local"
```

Kurulumdan önce [installation guide](install/README.md) ve [platform support matrix](docs/PLATFORM-SUPPORT.md) incelenmelidir. Linux, macOS ve Windows için aynı bağımlılıkların ve komutların geçerli olduğu varsayılmamalıdır.

## XVI. Güvenlik, bütünlük ve testler

Hash manifestleri ve doğrulama araçları dosyaları beklenen içerikle karşılaştırabilir. PASS sonucu yalnızca belirtilen kapsam ve kontrol zamanı için geçerlidir. Test raporları pass, fail ve skip durumlarını ayrı göstermelidir; atlanan veya çalıştırılmayan testler başarılı olarak raporlanmamalıdır.

Temel ilkeler:

- References kaynakları tanımlar; yürütme izni vermez.
- Native interfaces açık ve sınırlı olmalıdır.
- Platform desteği iddiaları gerçek build ve testlerle desteklenmelidir.
- Üretilen dokümantasyon source ve generator'a kadar izlenebilmelidir.
- Gerekli integrity verisi yoksa güvenlik kontrolleri güvenli biçimde fail-closed davranmalıdır.

[Testing policy](markdown/TESTING.md) ve [platform validation](docs/PLATFORM-SUPPORT.md) belgelerine bakın.

## XVII. Dokümantasyon, editörler ve katkı süreci

Başlangıç için [documentation index](docs/README.md), [generated API reference](docs/api/README.md), [API catalogue](api/README.md) ve [SleelaDoc](lib/sleeladoc/README.md) kullanılabilir. Linux editör kaynakları [linux-editors](linux-editors/README.md) altında bulunur.

Katkı sunmadan önce ilgili modül README'sini okuyun, değişikliğin kapsamını belirleyin, olumlu ve olumsuz testler ekleyin, ilgili testleri çalıştırın, OS/toolchain/commands/actual results bilgilerini kaydedin ve genel davranış değiştiğinde README ile API belgelerini güncelleyin.

## XVIII. Diğer proje alanları

CPU ve donanım modelleri, GUI, ağ bileşenleri, metin işleme, UTF-4088, π araçları, Modium, Registry, `sleelactl` ve AutoCAD Creator™ ayrı proje alanlarıdır. Amaçları, bağımlılıkları ve olgunluk seviyeleri farklı olabilir. Belgelenmiş doğrulama olmadan deneysel bir dizin veya örnek kararlı üretim özelliği olarak tanımlanmamalıdır. UTF-4088'in boyutu veya kayıt sayısı tam Unicode uyumluluğunu kanıtlamaz; AutoCAD uyumluluğu açık bir sürüm matrisi ve test sonuçları gerektirir.

**Çeviri durumu:** Türkçe sürüm; derleyici hattı, tür ve nesne modeli, döngü zamanlaması, kütüphaneler, native entegrasyon, build/kurulum, güvenlik, test ve katkı sürecini kapsayacak şekilde genişletildi. Ancak bu metin, İngilizce kanonik README'nin ve bağlantılı tüm belirtimlerin satır satır tam çevirisi değildir.
