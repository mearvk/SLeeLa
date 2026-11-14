<img align="right" src="../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — Indonesia (ID)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Bahasa sistem native, kompilator, runtime, dan ekosistem mesin virtual.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="../../docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Edisi negara internasional.** Edisi ini tersedia dalam Bahasa Indonesia. Identifier teknis, perintah, contoh kode, nama berkas, path, dan tautan tidak diubah dibandingkan dengan [README.md](../../README.md) kanonis berbahasa Inggris.

**The Now. The Ingredient.** Bahasa pemrograman lebih dari sekadar sintaks: ia adalah kompilator, runtime, pustaka, alat bantu, dan kontrak platform yang membuat perangkat lunak dapat digunakan.

**Bidang yang digeluti SLeeLa.** SLeeLa menggeluti **Math**, **Object**,
**Science**, dan **Reach** — empat wajah dari pekerjaannya: hal yang dapat
dihitung (Math), benda yang telah mapan dan berdiri dalam katalog tentang apa
yang ada (Object), kajian yang disiplin tentang yang benar (Science), dan
perluasan jangkauan sebuah sistem kepada sistem dan manusia lain (Reach).
**Kepentingan Nasional tetap berlaku** — keempatnya tidak mengesampingkannya;
apabila suatu subjek menyangkut kepentingan sebuah negara, kedudukan itu
dipertahankan (dicatat secara setia, misalnya pada `metaOrigin` /
`metaNationalSchedule` dalam Metadata setiap kelas), tidak pernah dilebur.

## I. Mulai dari Sini
- [Membangun implementasi native](../../impl/README.md)
- [Mesin virtual & Modium — panduan pengguna praktis](../../virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [Tumpukan CPU dalam sumber SLeeLa](../../lib/cpu/CPU.md) — mesin lengkap dari gerbang→ALU→inti→OS→program, daftar 130+ profil CPU, serta [tegangan, clock variabel, dan timing tingkat lanjut](../../lib/cpu/TIMING.md) yang dapat disetel, sehingga Anda dapat [menjalankannya sesuai spesifikasi dan melakukan overclock](../../lib/cpu/components/README.md)
- [Indeks dokumentasi](../../docs/README.md)
- [Sumber data CSPML dan acuan riset Salt & Salary](../reference/CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — asal-usul data eksternal AS (tempat, sosial-ekonomi, tenaga kerja, pendidikan, dan sekolah negeri) beserta panduan integrasinya.
- [Dokumentasi API yang dihasilkan](../../docs/api/README.md) — referensi HTML terkini, edisi API berversi, dan petunjuk pembuatan ulang
- [Katalog API](../../api/README.md)
- [Standar perbandingan komputasi](../../comparison/README.md) — standar tipe, memori, ABI, dan konformansi untuk VM SLeeLa, Java/JVM, dan C/C++ native.
- [Recursion stanchion — prototipe C dan C++](../../recursion/stanchion/README.md) — contoh yang dikhususkan per primitif untuk Boolean, Byte, Character, Integer, Long, Float, dan Double, beserta pola rekursi generik, terapan, ilmiah, dan pengajaran.
- [Matriks dukungan platform dan validasi](../../docs/PLATFORM-SUPPORT.md)
- [Panduan merek](../../docs/BRAND-GUIDE.md)
- [Konsep dan aset merchandise](../../merchandise/README.md)
- [Logo NitroWebExpress](../../images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Kebijakan pengujian dan gerbang rilis](../../markdown/TESTING.md)

## II. Berita

Penambahan terbaru pada bahasa, driver, dan pustaka standar:

- **Paket pustaka standar `advantage` — meng-advantage-kan sebuah dividen,
  dengan matematika, fisika, integrasi, penilaian, dan keluaran yang dapat
  diarahkan.**
  [`lib/advantage/`](../../lib/advantage/README.md) merangkum *meng-advantage-kan
  sebuah dividen unik terhadap suatu model* sebagai **bawang Sequitur**
  berlapis (dividen → tinjauan → kekhawatiran → matematika → penambahan →
  pengurangan → pengelompokan ulang → iterasi), yang dapat dinavigasi ke dalam
  maupun ke luar dengan pembaruan dinamis penuh dan restitute, dan disajikan
  melalui **model pengguna** yang sederhana serta **model pengembang** dengan
  kendali penuh. Paket ini memuat lima bidang subjek: **`math/`** ekonomi
  perdagangan bilateral (maksimisasi laba bersama, tawar-menawar Nash, rasio
  kritis newsvendor bilateral), **`physics/`** "fisika medan kepercayaan" dalam
  ekonomi struktural (kepercayaan sebagai tensor metrik, geodesik transaksional,
  struktur kontak pengumuman, SDE peluruhan kepercayaan CIR+jump — sebuah analogi
  pemodelan, dan ditandai demikian), **`integration/`** model integrasi
  vertikal yang dapat disusun secara rekursif untuk kapitalisme/ritel
  (kompartemen, bay rekayasa, kompartemen percobaan, nampan median newton, dan
  skor kecanggihan sistem), **`grading/`** penilai sirkuit kecanggihan yang
  memberi peringkat A–D pada setiap modul dan setiap kumpulan modul
  pemenang/pecundang/penilai/absolut, serta **`output/`** sebuah seam emit yang
  dapat diarahkan yang mengirim keluaran modul ke terminal, permukaan berformat
  **CommonRails**, atau soket/konektor (dibingkai untuk `slnet_write` native).
  Setiap kelas membawa rekaman Metadata-Class; lima demo yang dapat dijalankan
  memvalidasi subjek-subjek tersebut, dan
  [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md),
  [`MATH.md`](../../lib/advantage/MATH.md), serta
  [`PHYSICS.md`](../../lib/advantage/PHYSICS.md) pendampingnya mendokumentasikannya.
  Konsumen C/C++ native di bawah [`advantage/`](../../advantage/README.md) (inti C
  `advantage_value`/`advantage_mean`, penyimpanan pengetahuan XML, serta pustaka
  akuisisi HTTPS/SFTP/SSH yang aman + keystore + transport DH/ECDH) menautkan
  model ke dalam
  [contoh perdagangan](../../advantage/examples/README.md) yang telah dikerjakan —
  perdagangan positif dari sumber internet yang diasumsikan, urutan relevansi
  audiens (Trader, Konsumen, Penolong, Penyembuh), dan delapan model yang
  diperingkat A–D (D = paling banyak Cement, A = paling banyak Brand).
- **Anotasi pewaktuan perulangan — `@loopDelay` / `@loopBudget` (+ fungsi
  bawaan `sleep(ms)`).** Anotasi **tingkat pernyataan dan behavioral**
  pertama di SLeeLa: ditulis pada barisnya sendiri sebelum perulangan `for` /
  `while` / `do`, anotasi ini menyisipkan jeda nyata ke dalam eksekusi
  perulangan. `@loopDelay(ms = 250)` / `@loopDelay(seconds = 2)` menidurkan
  perulangan selama waktu tetap pada setiap iterasi (`@loopDelay(250)` tanpa
  nama argumen secara bawaan berarti milidetik); `@loopBudget(seconds = 10,
  iterations = 1000)` menyebarkan jumlah iterasi yang diketahui secara merata
  pada total anggaran waktu jam dinding. Keduanya diturunkan ke opcode
  append-only baru **`OP_SLEEP`** (ISA 157), yang juga mendukung fungsi bawaan
  umum **`sleep(<ms>)`** yang dapat dipanggil di mana saja; jeda
  diimplementasikan secara portabel (`nanosleep` / `Sleep`). Ditetapkan dalam
  [`SLEELA.syntax` §32](../../SLEELA.syntax); referensi API pada
  [`api/sleela/3.0/LOOP_TIMING_API.md`](../../api/sleela/3.0/LOOP_TIMING_API.md);
  contoh yang dapat dijalankan
  [`examples/loop-timing-annotation-demo.sleela`](../../examples/loop-timing-annotation-demo.sleela).
  Indeks tambahan sesi (termasuk ini, perkalian bilangan bulat besar native
  `OP_BIGMUL`, serta alat mandiri **[local-generator](../../local-generator/README.md)**
  dan **[pi-generator](../../pi-generator/README.md)**) terdapat di
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](../../api/sleela/3.0/SESSION-ADDITIONS-API.md).
  Tiga permukaan API yang dihasilkan
  ([`sleeladoc-api/`](../../sleeladoc-api/README.md),
  [`docs/api/`](../../docs/api/README.md), dan snapshot
  [`api/sleela/3.0/version-014/`](../../api/sleela/3.0/version-014/index.html)
  yang tidak dapat diubah) dibangun ulang dengan `make api-docs` dan kini
  memuat entri instruksi `SLISASleep` / `SLISABigMul` yang baru pada halaman
  paket `vm`.
- **SleelaDoc dari baris perintah — `sleela docs`.** SleelaDoc kini dapat
  diakses langsung dari terminal. `sleela docs` mencantumkan setiap topik yang
  dapat didokumentasikan; `sleela docs crypto` menampilkan sebuah paket;
  `sleela docs stringbuilder` me-resolve sebuah kelas (tanpa membedakan huruf
  besar/kecil, dan melalui prefiks pustaka standar `SL` →
  `SLStringBuilder`) dan mencetak halaman yang kaya — **definisi, tujuan
  desain, relasi, struktur, user clay, dan tangga penguasaan lima anak
  tangga** — dengan hasil "maksud Anda" yang diperingkat untuk kesalahan
  ketik yang mendekati. Penulis memperkaya halaman apa pun cukup dengan
  menambahkan tag `@` SleelaDoc (`@intent`, `@thesis`, `@axiom`, `@param`,
  `@see`, `@mastery`). `sleela docs --generate [dir]` menulis korpus teks
  polos (satu halaman per kelas + `TOPICS.index`) yang kemudian dibaca CLI
  secara offline; **`make sleeladoc`** menggabungkan korpus itu **beserta
  sumber `.sleela` yang bersesuaian** ke dalam `sleeladoc/build/sleeladoc.jar`.
  Untuk subjek yang hanya berupa fasad (mis. `psychiatry`, `politico`),
  `sleela docs --facade <topic>` menelusuri sistem berkas saat runtime dan
  mencantumkan **folder (lalu berkas, sebagai gerak alami)** yang sebenarnya,
  dibatasi oleh plafon yang dapat diganti pengguna sebesar **6 MB / 6000 berkas
  / 6 menit**. Unit `lib/sleeladoc` dipelihara pada edisi terkini,
  **`#sleela 3.0`**. Lihat [`man/man1/sleela.1`](../../man/man1/sleela.1) dan
  [`lib/sleeladoc/README.md`](../../lib/sleeladoc/README.md).
- **SleelaDoc — solusi dokumentasi bergaya Javadoc, dengan situs HTML yang
  dihasilkan.** **SleelaDoc** adalah konvensi komentar dokumentasi dalam
  sumber SLeeLa (komentar kurung sudut yang ditandai dengan sentinel `@doc` /
  `@page`; lihat [`manifesto/SLEELADOC.md`](../../manifesto/SLEELADOC.md)), dan
  [`lib/sleeladoc`](../../lib/sleeladoc/README.md) adalah **ekstraktor** yang
  merender unit `.sleela` menjadi halaman API Markdown atau HTML — seperti
  `javadoc` merender Java, dengan mendokumentasikan *maksud*. **Situs
  dokumentasi HTML yang lengkap dan dapat dijelajahi untuk seluruh `/lib`**
  dihasilkan ke dalam [`/sleeladoc-api`](../../sleeladoc-api/README.md) oleh
  `tools/generate-sleeladoc-api.py` (buka `sleeladoc-api/index.html`; gaya
  bersih biru / biru muda / abu-abu muda / putih dengan hierarki paket → kelas
  → anggota serta glosarium yang ditulis untuk pembaca *Student 2*).
  Kompilator juga memancarkan keluaran dokumentasi yang ditandai penulis
  sebagai produk sampingan kompilasi (halaman MediaWiki `--wiki` + kembaran
  sumber yang diasalkan), yang didokumentasikan dalam
  [`manifesto/SLEELADOC.WIKI.md`](../../manifesto/SLEELADOC.WIKI.md). Dapat
  dijalankan:
  [`lib/sleeladoc/sleeladoc-demo.sleela`](../../lib/sleeladoc/sleeladoc-demo.sleela).
- **Model Objek Dokumen XML (`lib/xml`).** DOM XML di sisi SLeeLa — sebuah
  dokumen yang memiliki pohon node elemen/teks/komentar/CDATA/PI dengan anak
  berurutan dan atribut bernama, sebuah serializer (ringkas/rapi, escaping
  XML 1.0 §2.4), parser sel karakter, dan register kualitas `SLXmlAspect`.
  Kesetaraan node adalah ikatan kepercayaan tiga anak tangga (identitas →
  subjek sama → struktur), refleksif, simetris, dan deterministik. Lihat
  [`lib/xml/README.md`](../../lib/xml/README.md) dan
  [`lib/xml/xml-dom.sleela`](../../lib/xml/xml-dom.sleela) yang dapat dijalankan.
- **Primitif bilangan bulat bertanda lebar (`long`, `int64`, `int128`).** `int`,
  `long`, dan `int64` semuanya adalah bilangan bulat bertanda 64-bit yang
  diperiksa tipenya dan dijalankan secara identik; `int128` diterima sebagai
  tipe bilangan bulat (saat ini didukung 64-bit, dengan 128-bit sejati sebagai
  tindak lanjut yang terdokumentasi). Seri bertanda basis-2
  `int32`..`int16777216` (sintaks 2.7) dan seri tak bertanda yang sepadan
  `uint32`..`uint16777216` (sintaks 2.8) memperluasnya. Lihat
  [`SLEELA.syntax`](../../SLEELA.syntax) §21, §24, §25.
- **Field konsekuen `@mix` (sintaks 2.9).** Sebuah kelas dapat mendeklarasikan
  beberapa field instance dengan **nama yang sama** apabila setiap
  pengulangannya membawa `@mix`; bahasa memahami kelompok tersebut sebagai
  konsekuen (array / daftar yang diindeks), secara bawaan dari atas ke bawah
  `1,2,3,…` atau manual melalui `@mix(1,2,3,...)` yang jumlah indeksnya harus
  sama dengan jumlah field. Lihat [`SLEELA.syntax`](../../SLEELA.syntax) §28.
- **Penanda dokumen — Proses Alami, bergelar Preonem
  (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).**
  Kata-kata penanda dokumen polos ini secara kolektif bergelar **Preonem**
  (satu penanda adalah sebuah *Preonem*; kumpulannya, *Preonem* — lihat
  [`SLEELA.syntax`](../../SLEELA.syntax) §26.1). Penanda dokumen kelas satu yang
  bersifat deklaratif saja, yang menamai apa itu sebuah dokumen dalam urutan
  pembuatannya: `author`/`authorial` (sintaks 2.4; ditangani oleh dewan
  profesional — satu-satunya anggota yang memiliki efek behavioral),
  `material;` (Sumber yang mapan) dan `memory;` (sintaks 2.5), `determine;`
  (sintaks 2.6; dokumen yang terbuka untuk diproses lebih lanjut oleh dokter,
  tabib, atau hantu menuju penyelesaian oleh tangan hukum), dan dua yang
  terbaru, **`object;`** (benda yang mapan dan berdiri dalam katalog tentang apa
  yang ada — **direifikasi, diberi kedudukan konkret, pada sintaks 2.8**) dan
  **`field;`** (landasan relasi tempat objek berdiri dan bergerak) — keduanya
  dibatasi sejak sintaks 2.6. Masing-masing polos, kontekstual, dan dibatasi
  versi oleh front end tanpa efek pada parsing, pengetikan, atau codegen; di
  luar posisi penanda, setiap kata tetap menjadi identifier biasa (sehingga
  `field` tidak pernah mengganggu anggota kelas atau akses `.field`). Lihat
  [`SLEELA.syntax`](../../SLEELA.syntax) §18–§20, §23, dan §26.
- **Nordshrift pada sintaks 3.0 dengan jangkauan pustaka penuh.** Nordshrift
  me-resolve melalui indeks `/lib` rekursif yang sama dengan kompilator,
  menjangkau setiap paket dan unit `.sleela`; `nordshrift check` melaporkan
  hitungan seluruh pohon.
- **Recursion stanchion.** Direktori [`recursion/stanchion/`](../../recursion/stanchion/README.md) kini mencakup prototipe acuan C dan C++ khusus untuk tujuh kontrak primitif yang didokumentasikan, ditambah contoh generik, terapan, ilmiah, dan pengajaran. Ini adalah sumber acuan; kompilasi dan ujilah sebelum memperlakukannya sebagai terverifikasi.

- **Komentar kurung sudut (`<` / `<<`).** Gaya komentar baru untuk ke depan —
  `< baris >` dan `<< blok >>` — yang di-lex oleh frontend SLeeLa dan
  didokumentasikan dalam [`SLEELA.syntax`](../../SLEELA.syntax) §1.1.
- **Literal bilangan bulat berradiks.** Literal `int` kini menerima bentuk
  heksadesimal (`0x`), biner (`0b`), dan oktal (`0o`) dengan pemisah digit `_`,
  baik dalam bahasa sumber SLeeLa ([`SLEELA.syntax`](../../SLEELA.syntax) §1.4)
  maupun dalam [format model CPU `.xlt`](../../lib/xlt/README.md).
- **Standar Kelas Metadata di seluruh pustaka.** Metapola metadata deskriptif
  (asal-usul, legalitas, cakupan yurisdiksi) kini dibawa oleh setiap berkas
  `.sleela` yang memuat kelas di bawah `/lib`; lihat
  [`SLEELA.syntax`](../../SLEELA.syntax) §15 dan
  [pelajaran tutorialnya](../../tutorial/13-metadata-class.md).
- **Keluarga teks / String yang lebih kaya.** `lib/text` mendapat pembantu
  dalam-bahasa yang nyata, tampilan sel karakter yang memberikan
  `charAt`/`substring`/`indexOf`/`reverse`/perataan kolom yang sesungguhnya,
  dan templater posisional yang nyata — lihat
  [tutorial teks & String](../../tutorial/14-text-and-strings.md) dan
  [`lib/text/text-string-demo.sleela`](../../lib/text/text-string-demo.sleela) yang
  dapat dijalankan.
- **`String` kelas satu dalam lembar Nordshrift.** Lembar `.sst` dapat
  mendeklarasikan blok `strings:` bertipe yang berkontribusi pada manifes
  komponen build; lihat [Referensi Lembar Nordshrift](../../wiki/Nordshrift-Sheet-Reference.md).
- **Model CPU dijalankan baik melalui opcode maupun ekuivalensi pemanggilan
  fungsi.** Setiap `SL<ARCH>CPU` mewarisi dasar untuk menjalankan aliran
  instruksi native melalui loop fetch-decode-execute-nya *dan* menjalankan kode
  dari proses lain yang diasumsikan melalui panggilan yang ekuivalen pada
  substrat VM bersama — lihat [`lib/cpu/EXECUTION.md`](../../lib/cpu/EXECUTION.md) §5.

## III. Build dan Pengujian

**Prasyarat:** toolchain C/C++ (GCC atau Clang) dan GNU Make; Python 3 untuk
perkakas manifes/alur; pada Linux, paket pengembangan X11/Xrandr untuk produk
tampilan (`make deps` pada sebuah modul melaporkan/memasang apa yang
dibutuhkannya). Build `impl/` bersifat fail-closed terhadap manifes SHA-256
tepercaya, yang ditunjuk secara otomatis oleh `Makefile` root.

Dari checkout yang bersih, awal tercepat:

```sh
make                                  # build everything from the repo root
```

Atau build dan uji hanya inti native:

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

`make` pada root repositori membangun setiap produk (inti, VM, codec, churn,
antivirus, …) dan diakhiri dengan membangun **registry** serta mematerialisasi
tabel baseline-nya — sehingga pohon yang telah dibangun menyertakan registry
yang dapat ditinjau sejak awal. Target-target pentingnya:

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

Target berguna lainnya:

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

Target `registry` menjalankan [`build-registry.sh`](../../build-registry.sh): ia
membangun inti registry, menyemai tabel baseline ke dalam penyimpanan teks
portabel `registry/build/sleela.sleelareg`, dan mengekspor skrip tabel MySQL
`registry/build/registry.sql` (melalui `sleela registry export`). Tinjau dengan
`sleela registry` atau `registry/build/sleela-registry review`. Lihat
[panduan registry](../../registry/README.md) dan [arsitektur Registry 3.0](../../registry/REGISTRY-3.0-ARCHITECTURE.md).

Rujuklah panduan build dan pengujian untuk prasyarat dan langkah khusus platform. Keberhasilan build satu komponen tidak membuktikan bahwa setiap pustaka atau platform siap dirilis.

## IV. Instalasi

Setelah membangun produk native, pasang perintah dan berkas dukungan runtime:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

Penginstal konvensional secara bawaan menggunakan `/usr/local/bin`,
`/usr/local/lib/sleela`, dan `/usr/local/share/sleela`. Ia mempertahankan
konfigurasi startup yang sudah ada dan menolak menimpa perintah-perintah
yang sudah ada tetapi tidak dilacak. Uninstall hanya menghapus berkas yang
dicatat oleh penginstal, dengan mempertahankan konfigurasi dan data runtime
yang tidak dilacak.

Untuk penyebaran terpadu yang terfokus pada paket utama saja — biner
`sleela`, `sleelvac`, dan `nordshrift`, pustaka standar, protokol, dan modul
pendukung — gunakan penginstal bersih:

```sh
make clean-install       # guided install of the main package only
```

Penginstal ini mengambil satu keputusan utama (memasang paket utama), lalu
pilihan cakupan, dan setelah berhasil, pemilih opsional untuk modul
peningkatan pilihan. Untuk alur penyebaran interaktif dan semua opsi
instalasi, lihat [Instalasi Cepat dan Aman](../../install/README.md).

## V. Bahasa dan Runtime
Tata bahasa otoritatif adalah [`SLEELA.syntax`](../../SLEELA.syntax). Repositori saat ini mendokumentasikan sintaks **3.0**; versi kompilator/alat dan versi sintaks sumber adalah dua hal yang berbeda. Permukaan 3.0 menambahkan **penanda dokumen `pure` dan `gasoline`** (§30) — sepasang Preonem polos, kontekstual, dan bersifat deklaratif saja yang bermakna hampir sama, masing-masing menandai kedudukan dokumen melalui kejujurannya sendiri. Sintaks 3.0 mengonsolidasikan permukaan bahasa 1.0–2.9 ke dalam edisi normatif saat ini, dengan mempertahankan semua gerbang minimum khusus fitur. Permukaan 2.9 sebelumnya menambahkan **field konsekuen `@mix`** (§28) — sebuah kelas dapat mendeklarasikan beberapa field instance bernama sama apabila masing-masing membawa `@mix`, dipahami sebagai satu kelompok berindeks (dari atas ke bawah `1,2,3,…` atau manual `@mix(1,2,3,...)` yang jumlah indeksnya harus sama dengan kelompok tersebut). Ia meneruskan keluarga bilangan bulat lebar bertanda/tak bertanda basis-2 `int32`..`int16777216` (§24) dan `uint32`..`uint16777216` (§25) di atas `long`/`int64`/`int128` yang lebih awal (§21); **dokumen pendukung kata kunci** (§22); dan **Preonem** — penanda dokumen yang bersifat deklaratif saja `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30), dan `artifact` (§35, kedudukan setingkat legislatif yang dinilai terhadap waktu dan ruang), yang secara kolektif bergelar Preonem (§26.1). Resolusi overload konstruktor dan delegasi `this(...)`/konstruktor basis secara eksplisit berada di luar kontrak sintaks 1.8 yang terverifikasi. Rujuk tata bahasa dan catatan implementasi sebelum mengandalkan suatu fitur. Sembilan unit sumber SleelaDoc kini mendeklarasikan `#sleela 3.0`; gerbang kompilator bersama menerima sintaks sumber 1.0–3.0. API baru dihasilkan dengan `make api-docs`.

Repositori mencakup implementasi C/C++ native, referensi pembangkitan SLVM, modul pustaka standar, pengujian, contoh, dan integrasi platform. Status implementasi dan validasi bervariasi menurut modul; rujuklah spesifikasi dan hasil CI terkini, jangan mengasumsikan kelengkapan yang seragam.

## VI. Prinsip Rekayasa
- **Bukti di atas klaim:** bedakan perilaku yang diimplementasikan, diuji, eksperimental, dan terencana.
- **Satu kontrak per batas:** dokumentasikan sintaks, API, format berkas, asumsi platform, dan perilaku saat gagal.
- **Portabel sejak desain:** isolasi backend khusus platform dan uji pada runner native.
- **Gagal dengan aman:** uji masukan yang cacat, batas sumber daya, kegagalan proses, dan jalur pembersihan.
- **Rilis yang dapat direproduksi:** bangun ulang secara bersih serta publikasikan hash artefak dan catatan rilis.

## VII. Build Antivirus dan Integritas

Paket antivirus Sleelavirin mencakup pemindai SHA-256 native, adaptor
ClamAV opsional, manifes integritas sumber/indeks, dan checksum paket
platform. Bangun dan uji paket dari root repositori:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Manifes integritas dihasilkan di bawah `antivirus/build/`; ia mencakup
masukan sumber dan dokumentasi yang dilacak serta berkas indeks tanda tangan
yang dilindungi, dan mengecualikan keluaran yang dihasilkan serta cache ClamAV
yang dapat berubah. Snapshot basis data ClamAV memiliki manifesnya sendiri.
ZIP rilis menyertakan `INTEGRITY-MANIFEST.json`, `SHA256SUMS`, dan sidecar
`.sha256`. Verifikasi hash arsip yang diunduh terhadap nilai tepercaya yang
diperoleh secara independen: checksum yang disimpan di samping arsip bukan
tanda tangan penerbit. Lihat [panduan Sleelavirin](../../antivirus/README.md),
[daftar periksa keamanan](../../antivirus/SECURITY.md), dan
[alur kerja build antivirus](../../.github/workflows/antivirus-binaries.yml).

Sebuah rilis tidak dianggap terverifikasi sampai job CI yang relevan lulus pada
commit rilis yang tepat dan artefak yang dihasilkan telah diperiksa.

## VIII. Tema Editor SLeeLa — Vim dan Emacs

SLeeLa menyertakan seperangkat skema warna gelap profesional yang terkoordinasi untuk **Vim** dan **Emacs**, ditambah utilitas palet C/C++ native. Skema **American Professional** yang asli tetap menjadi bawaan. Sepuluh palet pendamping tersedia untuk berbagai inspirasi desain budaya dan kelembagaan; palet-palet ini adalah palet desain interpretatif, bukan representasi resmi dari bangsa, budaya, pengadilan, atau lembaga mana pun.

### Sebelas Skema dan Warnanya

| Skema | Latar Belakang | Latar Depan | Kata Kunci | Tipe | String | Komentar |
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

#### Warna bendera nasional (palet sumber)

Skema Yunani dan Turki diturunkan dari **warna bendera nasional resmi** kedua negara, yang ditampilkan di sini dengan nilai heksa yang dipublikasikan secara persis. Latar belakang/aksen skema di atas adalah interpretasi yang digelapkan dan ramah-editor dari warna-warna sumber ini — tabel di bawah adalah acuan "warna sebagaimana dipublikasikan secara nasional".

| Bangsa | Warna | Heksa | Dasar dan makna |
|---|---|---|---|
| **Yunani** | Biru | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | Biru bendera nasional (Undang-Undang 851/1978). Langit dan lautan; dalam blazon heraldik disebut *azure*. |
| **Yunani** | Putih | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Putih bendera nasional. Kemurnian perjuangan kemerdekaan; salib melambangkan agama Kristen Ortodoks Timur. |
| **Türkiye** | Merah (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | Merah bendera nasional (Undang-Undang Bendera Turki). Medan bendera; menurut tradisi melambangkan darah para syuhada dan kedaulatan Utsmaniyah. |
| **Türkiye** | Putih | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Putih bendera nasional. Bulan sabit dan bintang — perdamaian, kemurnian, dan simbol warisan Turkik/Islam. |

Skema-skema ini menambatkan latar depan setiap editor pada **putih** nasional, serta kata kunci, kursor, seleksi, dan baris status pada **biru** nasional (`#0D5EAF`) atau **merah** nasional (`#E30A17`) yang persis, sehingga warna-warna yang penting benar-benar adalah warna nasional. Ini adalah palet desain interpretatif; acuan warna bendera disediakan demi asal-usul dan bukan representasi resmi dari kedua negara. Makna warna mengikuti uraian simbol negara dari Kementerian Luar Negeri Republik Helenik dan tradisi Undang-Undang Bendera Turki.


### Tabel Swatch Warna

Sel palet di bawah menggunakan **swatch SVG**, sehingga GitHub merender warna yang sebenarnya, bukan sel warna HTML yang kosong. Setiap strip menampilkan enam warna dengan urutan berikut: latar belakang, latar depan, kata kunci, tipe, string, dan komentar. Kode heksa yang persis dicantumkan di samping setiap swatch, dalam urutan skema yang sama dengan tabel acuan di atas.

| Skema | Warna palet yang dirender | Kode heksa (latar belakang · latar depan · kata kunci · tipe · string · komentar) |
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

Warna aksen tambahan digunakan untuk fungsi, konstanta, seleksi, diagnostik, dan direktif preprosesor. Palet Rusia dan Slavia dengan sengaja menggunakan abu-abu batu tulis, biru, hijau sage, rubi, dan emas antik berintensitas sedang untuk tampilan profesional yang lebih tenang.

### Memasang Skema Vim

Dari root repositori:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Tambahkan yang berikut ke `~/.vimrc` untuk mengaktifkan penyorotan sintaks SLeeLa dan memilih skema:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Pilih tepat satu dari nama colorscheme berikut:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Misalnya, gunakan `colorscheme sleela-russian-slavic` untuk palet Rusia dan Slavia berintensitas sedang, atau `colorscheme sleela-greek` / `colorscheme sleela-turkish` untuk palet Helenik dan Al Bayrak. Mulai ulang Vim atau muat konfigurasi Anda dengan `:source $MYVIMRC`.

### Memasang Skema Emacs

Buat direktori tema lokal dan salin berkas mode serta tema dari root repositori:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Tambahkan ini ke `~/.emacs` atau `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Ganti `sleela-american` dengan `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro`, atau `sleela-sleela` untuk memilih skema lain. Jika Emacs menanyakan apakah tema dipercaya, periksa berkas tema lokal sebelum menerimanya.

### Pratinjau Palet C/C++ Native

Utilitas palet native kecil dapat mendaftar skema, mencetak warna terminalnya, dan mencari satu warna individual:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Demi kompatibilitas mundur, `--palette` dan `--hex COLOR` tanpa `--theme` memilih American Professional. Hasil build dan pengujian harus diperiksa pada sistem target dan CI; dokumentasi sebuah perintah bukanlah bukti bahwa perintah itu lulus.

Lihat [panduan integrasi editor](../../linux-editors/README.md) untuk deskripsi palet lengkap dan rincian utilitas.


## IX. Kontribusi
1. Baca indeks dokumentasi dan kebijakan pengujian.
2. Buat perubahan yang terfokus beserta pengujian positif dan negatif.
3. Jalankan pengujian yang ditargetkan, lalu rangkaian pengujian yang luas.
4. Laporkan rincian OS/toolchain; jangan menyebut pemeriksaan yang dilewati sebagai lulus.

SLeeLa dikembangkan oleh **MEARVK LLC**. Materi halaman muka historis dipertahankan di [`docs/README-LEGACY.md`](../../docs/README-LEGACY.md).

## X. AE6E66 — Ekspektasi Pungutan atas Email Round Public

Untuk inisiatif AE6E66, ekspektasi yang dinyatakan proyek adalah **$300,000,000.00 USD per email Round Public** sebagai jumlah pungutan pajak yang diusulkan yang terkait dengan setiap email Round Public yang memenuhi syarat.

Pernyataan ini hanya mencatat suatu ekspektasi proyek. Pernyataan ini **bukan** penetapan pajak yang sah, perintah pemerintah, putusan pengadilan, atau kewajiban pajak yang ditetapkan secara independen. Setiap pungutan yang sesungguhnya memerlukan otoritas hukum yang berwenang, yurisdiksi, definisi, prosedur, dan penetapan pemerintah yang kompeten.


## XI. Runtime Alfabet dan Set Karakter

Dokumentasi alfabet SLeeLa dan runtime karakter native dipelihara di alphabet/sleela/ dan lib/alphabet/native/. Pengujian C++17 native mencakup decoding/encoding skalar UTF-8 yang ketat, tanda tangan kapasitas C<n>, dan keanggotaan set karakter yang dibatasi:

    make -C lib/alphabet/native test

Katalog UTF-4088 yang eksperimental dan adaptor pencarian SLeeLa-nya didokumentasikan di utf-4088/SLeeLa-ALPHABET-INTEGRATION.md. ID UTF-4088 adalah ruang nama tersendiri, bukan nilai skalar Unicode. Pekerjaan pengikatan kompilator dan SLVM dilacak secara terpisah dan eksplisit dari fondasi native ini.

## XII. AutoCAD Creator™

[AutoCAD Creator™](../../autocad/creator/README.md) adalah aplikasi CAD 2D interaktif
yang ditulis sepenuhnya dalam SLeeLa. Jika AutoCAD Renderer yang bersaudara
mengubah sebuah denah menjadi gambar sekali jalan, Creator memungkinkan Anda
**membuat dan mengubah** dokumen AutoCAD 2D secara langsung — menambah dinding,
ruangan, dimensi, dan catatan, lalu memindahkan, mengganti label, memindahkan
layer, menghapus, dan membatalkan (undo) apa yang telah Anda letakkan — dan
**menyimpan** hasilnya sebagai gambar AutoCAD DXF yang sesungguhnya (teks R12,
terbuka di AutoCAD / LibreCAD).

Antarmuka grafisnya dibangun di atas **SleelaUI™**, toolkit UI lintas platform
milik SLeeLa sendiri (di bawah [`lib/user-interface`](../../lib/user-interface)) —
bukan JavaFX — yang memunculkan jendela native sungguhan di X11 / Cocoa /
Win32. Desainnya mengikuti satu aturan, **"SleelaUI menampilkan; SLeeLa
memutuskan"**: GUI tidak pernah menghitung geometri atau menulis DXF. Setiap
alat mengeluarkan intent create / alter / delete / undo / save kepada
[`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela) yang otoritatif, yang memiliki
dokumen yang dapat disunting dan riwayat perintah yang dapat dibalik, sedangkan
kanvas hanya mempratinjau geometri yang dipegang Creator.

Mesin penyuntingan yang dapat dipakai ulang berada di [`lib/autocad`](../../lib/autocad) —
[`SLCadEntity`](../../lib/autocad/SLCadEntity.sleela),
[`SLCadDocument`](../../lib/autocad/SLCadDocument.sleela),
[`SLCadCommand`](../../lib/autocad/SLCadCommand.sleela), dan
[`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela) — dan berjalan dari ujung ke
ujung tanpa tampilan (headless). Shell GUI dan demonya berada di
[`autocad/creator`](../../autocad/creator):

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

Pada perangkat dengan layar, ini membuka jendela penyuntingan langsung; pada host
headless, ia membangun gambar di memori, mencetak laporan beserta DXF-nya, dan
keluar dengan bersih.


## XIII. Pengurutan Peta Masukan UTF-4088

Katalog UTF-4088 yang terspesialisasi mendefinisikan penelusuran peta masukan spasial dan iterasi tekanan abstraknya dalam [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). Kebijakan indeks subjek National Lien terdapat dalam [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Koordinat peta, tingkat tekanan (1–50), ID katalog permanen, dan peringkat alfabetis adalah field yang independen; tidak satu pun boleh digantikan secara diam-diam oleh yang lain. Tekanan adalah tingkat iterasi algoritmik, bukan besaran fisik.


## XIV. Indeks Paket Terbaru dan yang Diperluas

Indeks tambahan ini menyoroti paket, pustaka domain, dan koleksi sumber yang
baru-baru ini diperluas. Entri dikelompokkan menurut abjad berdasarkan nama
paket atau topik. Deskripsi menunjukkan lokasi dalam repositori; status
implementasi dan validasi harus diperiksa pada dokumentasi dan pengujian
masing-masing paket.

### A–C

- **Advantage** — `lib/advantage/` dan `advantage/`: meng-advantage-kan sebuah
  dividen unik terhadap suatu model sebagai bawang Sequitur berlapis, dengan
  lima subfolder subjek — `math/` (maksimisasi laba bersama, tawar-menawar
  Nash, newsvendor bilateral), `physics/` (metrik/geodesik/kontak/SDE medan
  kepercayaan, sebuah analogi pemodelan yang ditandai), `integration/`
  (kompartemen integrasi vertikal dan skor kecanggihan), `grading/` (penilai
  sirkuit kecanggihan A–D untuk modul dan kumpulan), dan `output/` (emit ke
  terminal / CommonRails / soket). Konsumen C/C++ native (`advantage/`)
  menautkan model ke dalam contoh perdagangan yang telah dikerjakan. Lihat
  [`lib/advantage/README.md`](../../lib/advantage/README.md)
  dan [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md).
- **Alfabet dan UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/`, dan
  `utf-4088/`: katalog alfabet, dukungan set karakter native, pengurutan peta
  masukan, dan dokumentasi integrasi UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` dan `lib/autocad/`: alur kerja
  gambar 2D interaktif dan komponen dokumen/penyuntingan CAD yang dapat dipakai ulang.
- **Ballum** — kendali tampilan tingkat piksel: set/get per piksel, blit
  wilayah, kecerahan/kehangatan/gamma seluruh layar, masukan penunjuk/papan
  ketik, dan **permukaan framebuffer layar penuh** untuk kendali per piksel
  yang total, dimiliki sendiri, dan cepat (jendela override-redirect +
  presentasi `XPutImage`/MIT-SHM tunggal pada Linux/X11). Inti native di bawah
  `ballum/` dengan fasad SLeeLa di `lib/ballum/` dan jembatan VM di bawah
  `impl/`; lihat [`ballum/README.md`](../../ballum/README.md) dan
  [`ballum/example/`](../../ballum/example/) yang dapat dijalankan.
- **CDMA** — `cdma/` dan `lib/cdma/`: materi teknis terkait CDMA dan
  pekerjaan pustaka sumber SLeeLa.
- **Character** — `lib/character/`: profil karakter dan statistik karakter
  yang dapat dipakai ulang, termasuk contoh profil biografis.
- **Coorenagraph** — `lib/coorenagraph/`: penalaran etis, kosakata moral,
  deliberasi, dan konsep terkait; lihat README paketnya dan berkas sumbernya.

### D–G

- **Model sumber tata kelola (Governance)** — [lib/governance](../../lib/governance/README.md) mendefinisikan bentuk sumber SLeeLa yang deklaratif untuk proposal, surat suara manual anonim, dan rekapitulasi agregat. Anotasi @vote yang diusulkan dan proses manualnya didokumentasikan dalam [registry/DEMOCRATIC-VOTING-SYNTAX.md](../../registry/DEMOCRATIC-VOTING-SYNTAX.md); kompilator saat ini memperlakukannya sebagai metadata komentar yang lembam.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` hingga direktori paket HTTP berversi
  yang tersedia: spesifikasi protokol dan implementasi khusus versi. Rujuk
  dokumentasi setiap versi untuk cakupan persisnya dan status verifikasinya.
- **International Criminal Court** — `lib/international-criminal-court/` dan
  `INTERNATIONAL.CRIMINAL.COURTS.md`: sumber dan dokumentasi khusus domain terkait.

### O–S

- **OFDMA** — `ofdma/`: materi teknis akses jamak pembagian frekuensi
  ortogonal dan sumber daya pengembangan C/C++.
- **Filsafat dan modul Conversation Mask** — [`philosophy/`](../../philosophy/README.md)
  memuat fondasi C/C++ native dan [`philosophy/module/`](../../philosophy/module/README.md)
  menata lapisan modul berikutnya ke dalam tiga area eksplisit:
  [`data/`](../../philosophy/module/data/README.md) untuk observasi bertipe dan
  bertanggal, akhir peristiwa, pengaturan waktu paruh, asal-usul, dan
  persetujuan; [`helper/`](../../philosophy/module/helper/README.md) untuk validasi
  deterministik, peluruhan relevansi, perbandingan yang kompatibel, dan
  penyajian yang disaring persetujuan; serta
  [`psychiatry-friend/`](../../philosophy/module/psychiatry-friend/README.md) untuk
  antarmuka percakapan yang suportif dan non-klinis. Handle yang menghadap
  SLeeLa dan Tree gubahan berada di [`lib/philosophy/`](../../lib/philosophy/README.md),
  berdampingan dengan paket filsafat yang lebih luas. Modul ini berbasis
  masukan eksplisit dan mengutamakan persetujuan: ia tidak memantau orang atau
  media secara pasif, tidak mendiagnosis, tidak menyimpulkan niat pribadi,
  tidak menilai harga diri manusia, dan tidak merekomendasikan perubahan
  pengobatan. Tautan peristiwanya menjaga asal-usul dan navigasi, bukan
  penguatan bukti secara independen.
- **Synchro** — integrasi runtime/kompilator native serta fungsionalitas
  pengiriman paket dan pengukuran; lihat sumber `impl/` yang relevan dan
  implementasi kompilator.
- **SleelaUI / UI terminal** — `lib/user-interface/` dan proyek
  `sleela-terminal`: integrasi UI SLeeLa dan komponen interaksi berorientasi terminal.

### U–W

- **United Nations** — `united-nations/`: materi repositori khusus domain.
- **Wi-Fi 4, Wi-Fi 5, dan Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`,
  `wi-fi-6/`, dan direktori sumber `lib/wifi/` terkait: materi jaringan nirkabel
  dan sumber daya konfigurasi khusus generasi.

### Pemeliharaan Indeks Paket

Ketika sebuah paket ditambahkan atau diperluas secara material, tambahkan
entrinya di sini pada kelompok abjad yang sesuai, tautkan ke direktori dan
README kanoniknya, dan jelaskan hanya kemampuan yang didukung oleh sumber yang
telah di-check-in. Pertahankan indeks ini bersifat aditif: jangan menghapus atau
menulis ulang bagian README yang ada sebagai bagian dari pembaruan indeks paket.


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

Demokrat AS dan Kebijakan AS.

---

## XVI. Citra Merek National Science

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. Katalog Eksperimental UTF-4088 dengan Sejuta Entri

[Generator katalog UTF-4088](../../utf-4088/tools/generate_catalogue.py) membangun secara deterministik katalog simbol eksperimental berisi 1.048.576 entri. Format ringkas yang didokumentasikan diuraikan dalam [utf-4088/FORMAT.md](../../utf-4088/FORMAT.md); kontrak pembangkitan dan validasinya terdapat di [utf-4088/GENERATION-RESULTS.md](../../utf-4088/GENERATION-RESULTS.md).

Jalankan generator dengan Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Biner yang dihasilkan berukuran 16.777.244 byte dan dirancang agar tetap di bawah 20.000.000 byte. Pengujian unit dan alur kerja GitHub Actions memverifikasi jumlah entri, keunikan glif, ukuran, dan SHA-256; eksekusi yang berhasil menerbitkan biner dan ringkasannya sebagai artefak alur kerja `utf4088-million-catalogue`. Biner yang dihasilkan sengaja tidak disimpan dalam riwayat Git. UTF-4088 tetap eksperimental dan tidak kompatibel dengan Unicode.

---

# SLeeLa
<p align="center"><img src="../../images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Penambahan Terbaru — Konstanta Pi, Startup Modium/Registry, Matematika sleelactl, dan `@native`

Bagian ini mencatat fitur yang ditambahkan selama pekerjaan 2026-11, dengan kelompok terbaru lebih dahulu. Berkas sumber yang ditautkan beserta status validasinya bersifat otoritatif; perubahan yang masih ditinjau tidak dijelaskan sebagai telah digabungkan sampai mencapai `master`.

### Konstanta katalog Pi 100.000.000 digit

Repositori ini menyertakan nilai π **seratus juta digit** yang telah diverifikasi di [`pi-generator/pi.100000000.digits`](../../pi-generator/pi.100000000.digits) (desimal yang berbentuk baik, `3.14159…`), berdampingan dengan nilai 57.344 digit di [`pi-generator/pi.57344.digits`](../../pi-generator/pi.57344.digits). Keduanya diverifikasi secara independen sebagai π yang asli — lihat [`pi-generator/VERIFIED.md`](../../pi-generator/VERIFIED.md), yang mendokumentasikan pemeriksaannya (awalan buku teks, titik Feynman pada posisi pecahan 762, digit satu-jutaan yang terkenal, kesesuaian dengan `lib/math/SLPiReference.sleela`, dan analisis frekuensi digit seluruh berkas) serta skrip reproduksi sekali jalan.

### Modium™ / Registry™ memuat konstanta Pi saat startup subsistem

Pada titik masuk subsistem khusus — `sleela registry` dan `sleela modium` (serta first run penginstal), **bukan** pada setiap `sleela run` — subsistem Modium dan Registry memuat konstanta katalog π yang besar. Pada terminal interaktif, keduanya mencetak baris merah sementara `Loading Modium™` / `Loading Registry™` yang kemudian dihapus di tempat sebelum keluaran normal dimulai; efek ini sepenuhnya dibatasi pada TTY, sehingga keluaran melalui pipe dan keluaran pengujian tetap identik byte demi byte. Locator (bukan byte-nya) disemai sebagai bawaan kunci `Math` registry `piReferenceLargePath` dan `piReferenceLargeDigits`, baik dalam model SLeeLa ([`lib/registry/SLRegistry.sleela`](../../lib/registry/SLRegistry.sleela)) maupun penyemai native ([`registry/sleela_registry.c`](../../registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` menyediakan `largePath()` / `largeDigits()`. Konstanta π dipasang ke lokasi SLeeLa OS (`<data>/pi/`) oleh **kedua** penginstal `make` standar dan penginstal "beautiful" terpandu. Rincian: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](../../pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — penginstal beautiful dan subjek `math` berbasis native

- Penginstal terpandu ("beautiful") kini memasang `sleelactl` (bidang kendali SLeeLa) beserta registry modulnya, sama seperti penginstal `make`. Lihat [`bin/SLEELACTL.md`](../../bin/SLEELACTL.md).
- `sleelactl` memperoleh sifat **`subject`** yang baru untuk pustaka subjek C/C++ native yang dikompilasi ke dalam runtime, dan mendaftarkan yang pertama: **`math`**. `sleelactl run math` menjalankan backend `__NativeMath` native (`impl/subjects/math/math_native.cpp`) dari ujung ke ujung melalui demo acuan subjek tersebut; `sleelactl info math` menjelaskan cara menjangkaunya (`import math;`). Sifat subjek hanya menawarkan `run info` — dengan sengaja tidak ada kata kerja `check`, karena `sleela check` tidak memuat subjek native yang terdaftar di SHEET.

### `@native`, Native Drive, dan eksekusi multibahasa

Anotasi metode **`@native`** yang baru meminta kompilasi native ahead-of-time untuk metode yang panas (hot). Perintah `sleela aot` yang baru menemukan metode `@native`, memverifikasi bahwa masing-masing berada dalam *subset yang dapat di-offload* yang didukung, dan memancarkan C/C++ — **gagal secara tertutup** (menolak, dengan alasan yang tepat) pada konstruksi apa pun yang belum dapat diterjemahkannya, sehingga metode yang setengah diterjemahkan tidak pernah dihasilkan.

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

`@native` tetap merupakan permintaan perlakuan AOT, dengan konstruksi yang tidak didukung ditolak sebelum emisi. [Native Drive](../../impl/native-drive/README.md) mendukung build executable dan pustaka bersama C/C++ multi-unit melalui kompilator host (termasuk MSVC), keluaran AOT SLeeLa yang didukung, inspeksi simbol terhadap ABI native-loader, eksekusi proses tanpa shell, penanganan stdin/stdout/stderr, piping multi-tahap, dan delegasi XClass ke loader produksi. `sleelactl` mengekspos perintah build/run-nya dan merutekan inspeksi XClass serta rendering/instalasi unit systemd yang eksplisit melalui `inspect native-drive`. VM memiliki tabel handle modul native yang dibatasi dengan otorisasi host yang wajib dan handle yang diperiksa generasinya. Sintaks 3.0 kini menambahkan `nativeOpen`, `nativeCall`, dan `nativeClose`: dispatch bytecode append-only melintasi ABI null/bool/int64/double/UTF-8 yang berversi terpisah, sementara array, struct, dan nilai komposit lain milik VM tetap berada di dalam interpreter dan dapat menggunakan fallback bytecode bernama sama. Plugin dapat secara opsional menerima tabel callback host berversi untuk memanggil fungsi SLeeLa yang tepat melalui eksekusi VM bersarang, dengan GC rooting untuk stack yang ditangguhkan, allowlist nama callback host yang eksplisit, dan panggilan API native rekursif ditolak. Kode native tetap merupakan kode tepercaya dalam proses, bukan sandbox. Anotasi kebijakan proses pada sumber memiliki skema berversi tetapi belum di-parse/ditegakkan. Rincian, batas keamanan, dan buku besar tujuh alur kerja: [Native Drive](../../impl/native-drive/README.md) dan [`pi-generator/AT-NATIVE.md`](../../pi-generator/AT-NATIVE.md).


## XIX. Edisi README Internasional

SLeeLa menyediakan titik masuk README khusus negara untuk 50 pasar dalam edisi internasionalnya. Setiap negara memiliki satu berkas bernama `README.<CC>.md`; apabila beberapa bahasa relevan, bahasa-bahasa itu dikelompokkan dalam berkas negara tersebut.

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

**Kebijakan terjemahan:** Berkas-berkas ini adalah titik masuk negara yang dilokalkan pada tahap awal. Pengantar dan judul bahasanya dilokalkan, sedangkan referensi teknis lengkap tetap berada di [README.md](../../README.md) kanonis berbahasa Inggris. Berkas negara tidak boleh disebut sebagai terjemahan lengkap sebelum setiap bagian diterjemahkan, identifier teknis dan tautan diperiksa, dan edisi tersebut telah melalui tinjauan bahasa. Cakupan bahasa pada tahap pertama ini bersifat representatif dan bukan klaim bahwa setiap bahasa yang diakui secara konstitusional, daerah, adat, atau isyarat telah diterjemahkan sepenuhnya.

## Lampiran — Panduan Regional yang Dipertahankan

Lampiran ini mempertahankan materi praktis dari panduan Bahasa Indonesia sebelumnya yang tidak terdapat sebagai bagian tersendiri dalam README kanonis. Materi ini tidak menggantikan README kanonis; apabila ada perbedaan, [README.md](../../README.md) kanonis berbahasa Inggris dan [`SLEELA.syntax`](../../SLEELA.syntax) yang menjadi acuan. Panduan sebelumnya tidak memuat catatan khusus tentang bahasa daerah atau bahasa isyarat, sehingga tidak ada yang ditambahkan mengenai hal tersebut.

### Kompilator, representasi, dan mesin virtual

Rantai kompilasi SLeeLa memisahkan sumber bahasa, analisis frontend, representasi antara, generasi kode, dan eksekusi runtime. Pemisahan ini penting: versi kompilator, versi sintaks, serta versi VM/ISA merupakan penanda yang berbeda. Periksa deklarasi versi pada sumber dan spesifikasi yang berlaku sebelum mengasumsikan bahwa suatu fitur tersedia.

Secara konseptual, alurnya meliputi:

1. **Sumber:** berkas `.sleela` berisi deklarasi dan instruksi.
2. **Lexer dan parser:** mengubah teks menjadi token dan struktur sintaksis.
3. **Analisis semantik:** memeriksa simbol, tipe, dan aturan bahasa.
4. **Representasi antara:** menyiapkan program untuk tahap penurunan berikutnya.
5. **Generasi target:** menghasilkan bentuk yang didukung oleh VM atau target antara.
6. **Runtime:** menjalankan hasil kompilasi dengan mekanisme kontrol dan memori yang tersedia.

Rincian implementasi dapat berubah. Fitur yang tercantum dalam spesifikasi tidak otomatis tersedia pada setiap target.

### Integritas, keamanan, dan validasi

SLeeLa menekankan keterlacakan antara sumber, dokumentasi, proses build, dan hasil pengujian. Manifest hash dan alat verifikasi dapat memeriksa apakah berkas sesuai dengan konten yang diharapkan. Hasil pemeriksaan hanya berlaku untuk cakupan dan waktu pemeriksaan yang dinyatakan.

Prinsip penting:

- Referensi mengidentifikasi sumber daya; referensi tidak memberikan izin eksekusi.
- Antarmuka native harus eksplisit dan dibatasi.
- Hasil pengujian harus membedakan pass, fail, dan skip.
- Klaim dukungan platform harus didukung build dan pengujian yang relevan.
- Dokumentasi yang dihasilkan harus dapat ditelusuri ke sumber dan generator.
- Pemeriksaan keamanan harus gagal secara tertutup jika informasi integritas yang diwajibkan tidak tersedia.

### Target, profil, dan batas eksekusi

SLeeLa memisahkan kode sumber dari target build dan execution. Keluarga target yang didokumentasikan mencakup `SLVM`, `SLJVM`, dan `SLIR`; `NATIVE` merupakan target terbatas dan bersyarat, bukan mode yang dapat diasumsikan tersedia pada semua platform. Profil `BASIC_COMPLETE` dan `ADVANCED_TOTAL` menunjukkan cakupan yang berbeda. Verifikasi target, profil, dan dukungan platform yang benar-benar digunakan pada setiap build.

Reference mengidentifikasi file, modul, atau resource; reference tidak dengan sendirinya memberikan izin untuk mengeksekusinya. Ketika program menggunakan file, pustaka eksternal, native bindings, atau sumber daya sistem, batas otorisasi dan tanggung jawab eksekusi harus ditetapkan secara eksplisit. Pernyataan dokumentasi saja bukan bukti jaminan keamanan runtime; periksa implementasi dan pengujian terkait.

### Siklus hidup resource dan debugging

Dokumentasikan ownership, lifetime, dan tanggung jawab sinkronisasi untuk resource yang dibagikan antar-fungsi atau thread. Bedakan local variables, shared object state, VM handles, dan serializable data. VM-local handle bukan format file atau pesan jaringan yang stabil. Gunakan serialization interface yang ditetapkan modul, lalu uji nilai invalid, missing, dan boundary.

Saat debugging, catat commit, OS, architecture, dan toolchain. Build target relevan yang paling kecil terlebih dahulu, simpan seluruh error output, bedakan compiler errors dari runtime errors, dan buat contoh minimal yang dapat direproduksi. Build yang berhasil tidak otomatis membuktikan runtime behavior benar.

Laporan validasi harus menyebutkan exact commit, OS/architecture, compiler versions, seluruh build/test commands, jumlah pass/fail/skip, dan batasan yang diketahui. Test yang dilewati atau tidak dijalankan tidak boleh dihitung sebagai passed.

### Kontrak API modul, kompatibilitas, dan strategi pengujian

Saat API baru diperkenalkan, dokumentasikan input, output, kondisi error, kepemilikan data, dan apakah operasi menghasilkan side effects. Contoh dokumentasi harus menunjukkan penggunaan normal sekaligus penanganan input invalid atau missing. Jangan menyatakan perilaku sebagai jaminan jika implementation dan tests belum membuktikannya.

Pisahkan pengujian menurut tahapan compiler pipeline: lexing/parsing, semantic analysis, IR lowering, code generation, VM execution, dan native integration. Untuk setiap regression case, simpan input terkecil yang dapat direproduksi, hasil yang diharapkan, dan hasil aktual. Perubahan API harus menjelaskan backward compatibility, migration steps, serta dampaknya terhadap examples yang sudah ada.

### Lokalisasi, encoding, dan keterbacaan

Pertahankan technical identifiers, commands, paths, syntax, dan code blocks dalam bentuk yang sama di semua bahasa. Terjemahkan teks penjelasan, bukan token atau nama yang digunakan source code. Nyatakan bahasa dan konteks wilayah secara jelas; istilah Bahasa Indonesia tidak selalu identik dengan istilah Melayu di negara lain.

Setelah setiap pembaruan, periksa Markdown links, hierarki heading, code fences, dan examples. Ketika canonical English documents berubah, bandingkan bagian terkait dan perbarui terjemahan secara menyeluruh, bukan hanya bagian pengantar. Tandai konten yang belum ditinjau penutur asli atau reviewer teknis.

**Status terjemahan:** Bagian I–XIX diterjemahkan ke dalam Bahasa Indonesia. Identifier teknis, perintah, contoh kode, nama berkas, tautan, dan alamat dipertahankan. Peninjauan bahasa akhir oleh penutur asli disarankan.
