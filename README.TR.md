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
