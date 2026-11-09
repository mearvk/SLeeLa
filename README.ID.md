# SLeeLa 3.0 — Indonesia (ID)

## Bahasa Indonesia

SLeeLa adalah bahasa pemrograman sistem native yang mencakup kompilator, runtime, dan ekosistem mesin virtual. Bahasa, kompilator, pustaka, alat pengembang, serta kontrak platformnya bekerja bersama untuk membangun dan menjalankan perangkat lunak.

**The Now. The Ingredient.** Bahasa pemrograman bukan hanya sintaks. Komponen toolchain dan runtime juga merupakan bagian dari sistem perangkat lunak yang dapat digunakan.

## I. Mulai di sini

- [Membangun implementasi native](impl/README.md)
- [Panduan pengguna mesin virtual dan Modium](virtual-machine/USER-GUIDE.md) — `sleela vm`, `sleela modium`
- [Arsitektur CPU dalam kode sumber SLeeLa](lib/cpu/CPU.md) — lebih dari 130 profil CPU; lihat [pengaturan timing](lib/cpu/TIMING.md).
- [Indeks dokumentasi](docs/README.md)
- [Dokumentasi API yang dihasilkan](docs/api/README.md) dan [katalog API](api/README.md)
- [Dukungan platform dan validasi](docs/PLATFORM-SUPPORT.md)
- [Kebijakan pengujian](markdown/TESTING.md)

## II. Build dan pengujian

Persyaratan meliputi toolchain C/C++ (GCC atau Clang), GNU Make, dan Python 3. Komponen grafis Linux mungkin juga memerlukan paket pengembangan X11/Xrandr.

Jalankan dari direktori root repositori:

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

Keberhasilan build satu komponen tidak membuktikan bahwa semua pustaka atau platform siap dirilis. Periksa dokumentasi build dan platform untuk persyaratan setiap target.

## III. Instalasi

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

Awalan instalasi sistem biasanya `/usr/local`. Menggunakan `PREFIX="$HOME/.local"` menyediakan instalasi lokal pengguna dan biasanya tidak memerlukan hak administrator. Untuk alur instalasi terpandu dan rincian paket, lihat [panduan instalasi](install/README.md).

## IV. Bahasa dan runtime

Tata bahasa normatif ditetapkan dalam [`SLEELA.syntax`](SLEELA.syntax). Dokumentasi repositori menggunakan sintaks **3.0**. Versi kompilator dan versi sintaks bahasa sumber merupakan dua hal yang berbeda.

Referensi teknis mencakup field berurutan `@mix`, keluarga integer yang diperluas, penanda dokumen Preonem, SleelaDoc, pembuatan dokumentasi API, dan alat Nordshrift. Identifier teknis, perintah, nama berkas, nama API, dan contoh kode dipertahankan tanpa perubahan agar dapat digunakan secara langsung.

## V. Prinsip rekayasa dan validasi

SLeeLa mengutamakan bukti, kontrak komponen yang eksplisit, portabilitas, perilaku aman saat terjadi kegagalan, dan rilis yang dapat direproduksi. Status validasi harus dinilai per modul dan platform. Pemeriksaan yang dilewati tidak boleh dilaporkan sebagai lulus; keberhasilan satu komponen bukan validasi rilis penuh.

Perangkat perlindungan Sleelavirin mencakup pemeriksa SHA-256 lokal, adaptor ClamAV opsional, dan manifes integritas. Rilis hanya boleh dianggap tervalidasi setelah pemeriksaan CI untuk commit rilis selesai dan artefak yang dihasilkan telah ditinjau.

## VI. Editor dan kontribusi

SLeeLa menyediakan tema dan integrasi sintaks untuk **Vim** dan **Emacs**, serta utilitas palet native C/C++. Lihat [panduan integrasi editor](linux-editors/README.md).

Untuk berkontribusi:

1. Baca indeks dokumentasi dan kebijakan pengujian.
2. Buat perubahan dengan cakupan jelas dan sertakan pengujian positif serta negatif.
3. Jalankan pengujian terkait, lalu rangkaian pengujian yang lebih lengkap.
4. Catat sistem operasi dan toolchain yang digunakan. Jangan menyebut pemeriksaan yang dilewati sebagai berhasil.

SLeeLa dikembangkan oleh **MEARVK LLC**.

## VII. Referensi teknis lainnya

README utama juga membahas pustaka Advantage, SleelaDoc, dan XML DOM; tipe integer dasar; runtime karakter; AutoCAD Creator™; katalog eksperimental UTF-4088; alat Pi, Modium, Registry, dan `sleelactl`; serta tahap awal jalur kompilasi C/C++ native yang diaktifkan oleh penanda `@native`.

## Referensi teknis lengkap

[README kanonis](README.md) memuat katalog fitur lengkap, referensi bahasa dan runtime, petunjuk build dan instalasi, tautan API, contoh, serta catatan rilis. Edisi Bahasa Indonesia ini telah memperluas panduan praktis, tetapi belum merupakan terjemahan baris demi baris dari seluruh README teknis. README bahasa Inggris tetap menjadi sumber teknis normatif sampai terjemahan lengkap ditinjau.

- [README kanonis](README.md)
- [Spesifikasi sintaks SLeeLa](SLEELA.syntax)
- [Indeks dokumentasi](docs/README.md)
- [Dukungan platform](docs/PLATFORM-SUPPORT.md)
- [Repositori GitHub](https://github.com/mearvk/SLeeLa)

**Status terjemahan:** Pengantar, panduan mulai, build, instalasi, bahasa/runtime, prinsip rekayasa, validasi, dan kontribusi telah dilokalkan. Terjemahan lengkap seluruh bagian teknis masih perlu dikerjakan dan ditinjau oleh penutur Bahasa Indonesia yang memahami bidang teknis.


## V. Kompilator, representasi, dan mesin virtual

Rantai kompilasi SLeeLa memisahkan sumber bahasa, analisis frontend, representasi antara, generasi kode, dan eksekusi runtime. Pemisahan ini penting: versi kompilator, versi sintaks, serta versi VM/ISA merupakan penanda yang berbeda. Periksa deklarasi versi pada sumber dan spesifikasi yang berlaku sebelum mengasumsikan bahwa suatu fitur tersedia.

Secara konseptual, alurnya meliputi:

1. **Sumber:** berkas `.sleela` berisi deklarasi dan instruksi.
2. **Lexer dan parser:** mengubah teks menjadi token dan struktur sintaksis.
3. **Analisis semantik:** memeriksa simbol, tipe, dan aturan bahasa.
4. **Representasi antara:** menyiapkan program untuk tahap penurunan berikutnya.
5. **Generasi target:** menghasilkan bentuk yang didukung oleh VM atau target antara.
6. **Runtime:** menjalankan hasil kompilasi dengan mekanisme kontrol dan memori yang tersedia.

Rincian implementasi dapat berubah. Fitur yang tercantum dalam spesifikasi tidak otomatis tersedia pada setiap target.

## VI. Fitur bahasa dan pustaka

Pustaka standar dan dokumentasi SLeeLa mencakup sejumlah bidang:

- **Advantage** (`lib/advantage/`) — model berlapis untuk menilai hasil terhadap suatu model, dengan topik ekonomi perdagangan bilateral, integrasi vertikal, penilaian modul, dan output yang dapat diarahkan. Analogi fisika yang digunakan dalam dokumentasi harus dipahami sebagai analogi pemodelan, bukan klaim hukum fisika.
- **XML DOM** (`lib/xml/`) — model dokumen XML dengan node elemen, teks, komentar, CDATA, instruksi pemrosesan, atribut, parsing, dan serialisasi.
- **Text/String** (`lib/text/`) — operasi teks berbasis sel karakter seperti `charAt`, `substring`, `indexOf`, `reverse`, dan pemformatan posisi.
- **SleelaDoc** (`lib/sleeladoc/`) — ekstraksi komentar dokumentasi dan halaman referensi. CLI `sleela docs` dapat menelusuri topik, paket, dan kelas; `sleela docs --generate [dir]` menghasilkan korpus teks lokal.
- **CPU dan XLT** (`lib/cpu/`, `lib/xlt/`) — dokumentasi arsitektur, profil CPU, komponen, model, dan timing.
- **Recursion stanchion** (`recursion/stanchion/`) — prototipe referensi C/C++ dan contoh pola rekursi.

Dokumentasi modul adalah sumber terbaik untuk mengetahui cakupan, batasan, dan contoh aktual. Entri katalog tidak dengan sendirinya menyatakan bahwa modul siap untuk produksi.

## VII. Anotasi, tipe, dan sintaks

Beberapa fitur yang dijelaskan dalam dokumentasi:

- `@loopDelay` dan `@loopBudget` mengatur jeda dan anggaran waktu perulangan. Keduanya diturunkan ke instruksi VM `OP_SLEEP` (ISA 157), yang juga mendukung fungsi `sleep(ms)`. Lihat [SLEELA.syntax §32](SLEELA.syntax) dan [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md).
- `@mix` memungkinkan pengelompokan field instance bernama sama sebagai urutan berbasis indeks apabila setiap field diberi anotasi tersebut.
- Preonems — `author`, `authorial`, `material;`, `memory;`, `determine;`, `object;`, dan `field;` — adalah penanda deklaratif untuk status dan asal dokumen. Penanda tersebut bukan otorisasi untuk mengeksekusi kode.
- Komentar kurung sudut menggunakan bentuk `< line >` dan `<< block >>`.
- Literal integer mendukung heksadesimal (`0x`), biner (`0b`), dan oktal (`0o`), termasuk pemisah digit `_`.
- `int`, `long`, dan `int64` adalah integer bertanda 64-bit. `int128` diterima sebagai tipe integer tetapi implementasi yang didokumentasikan masih berbasis 64-bit; jangan menganggapnya sebagai aritmetika 128-bit penuh.

Lihat [spesifikasi sintaks](SLEELA.syntax) untuk aturan normatif dan batasan versi.

## VIII. C, C++, dan jalur native

Repositori mencakup komponen C dan C++ serta jalur native yang dibatasi. Penanda `@native` dan perintah `sleela aot` berkaitan dengan pengembangan generasi kode native; keduanya bukan izin umum untuk menjalankan kode native sembarang.

Integrasi C/C++ harus memiliki batas antarmuka yang eksplisit. Kontrak ABI, kepemilikan memori, tipe data, penanganan kesalahan, dan ketergantungan platform perlu didokumentasikan dan diuji. Referensi hanya mengidentifikasi sumber daya; referensi tidak memberi otorisasi eksekusi.

Lihat [implementasi native](impl/README.md), [matriks platform](docs/PLATFORM-SUPPORT.md), dan [standar perbandingan](comparison/README.md).

## IX. Integritas, keamanan, dan validasi

SLeeLa menekankan keterlacakan antara sumber, dokumentasi, proses build, dan hasil pengujian. Manifest hash dan alat verifikasi dapat memeriksa apakah berkas sesuai dengan konten yang diharapkan. Hasil pemeriksaan hanya berlaku untuk cakupan dan waktu pemeriksaan yang dinyatakan.

Prinsip penting:

- Referensi mengidentifikasi sumber daya; referensi tidak memberikan izin eksekusi.
- Antarmuka native harus eksplisit dan dibatasi.
- Hasil pengujian harus membedakan pass, fail, dan skip.
- Klaim dukungan platform harus didukung build dan pengujian yang relevan.
- Dokumentasi yang dihasilkan harus dapat ditelusuri ke sumber dan generator.
- Pemeriksaan keamanan harus gagal secara tertutup jika informasi integritas yang diwajibkan tidak tersedia.

Lihat [kebijakan pengujian](markdown/TESTING.md), [dukungan platform](docs/PLATFORM-SUPPORT.md), dan [panduan instalasi](install/README.md).

## X. Editor dan kontribusi

Lihat [integrasi editor Linux](linux-editors/README.md) untuk sumber daya Vim dan Emacs. Pastikan petunjuk sesuai dengan sistem operasi dan versi editor yang digunakan.

Alur kontribusi yang disarankan:

1. Baca indeks dokumentasi dan README modul yang akan diubah.
2. Batasi ruang lingkup perubahan dan jelaskan alasannya.
3. Tambahkan pengujian positif dan negatif.
4. Jalankan pengujian terkait dan, bila perlu, seluruh test suite.
5. Catat OS, toolchain, perintah, dan hasil aktual.
6. Perbarui README dan dokumentasi API jika perilaku publik berubah.
7. Jangan menyebut pengujian yang dilewati atau tidak dijalankan sebagai lulus.

## XI. Bidang proyek lainnya

Repositori juga mencakup model CPU dan perangkat keras, antarmuka grafis, komponen jaringan, pemrosesan teks, dokumentasi, internasionalisasi, dan katalog eksperimental. Tingkat kematangan tiap bidang dapat berbeda. Katalog eksperimental atau contoh tidak boleh disebut fitur produksi yang stabil tanpa validasi terdokumentasi.

Katalog UTF-4088, alat π, Modium, Registry, dan `sleelactl` memiliki dokumentasi serta persyaratan masing-masing. Periksa README kanonis dan dokumentasi modul untuk status terkini.

## XII. AutoCAD Creator™

AutoCAD Creator™ tercantum dalam katalog teknis proyek. Baca dokumentasi modul terkait untuk membedakan fitur yang dijelaskan, contoh, dan hasil yang telah diverifikasi. Jangan mengasumsikan kompatibilitas dengan versi AutoCAD tertentu tanpa matriks kompatibilitas dan hasil pengujian yang eksplisit.

## XIII. Katalog eksperimental UTF-4088

Materi UTF-4088 merupakan bidang eksperimental terkait karakter dan katalog. Ukuran, jumlah entri, kebutuhan memori, dan rincian format harus ditafsirkan berdasarkan dokumentasi dan implementasi saat ini. Jumlah entri yang besar tidak dengan sendirinya membuktikan kompatibilitas Unicode penuh atau kesiapan produksi.

## XIV. π, Modium, Registry, dan sleelactl

Proyek memiliki alat dan dokumentasi terpisah untuk pembuatan π, Modium, Registry, dan `sleelactl`. Tujuan masing-masing berbeda; ikuti instruksi build dan validasi pada direktori terkait. Jangan menyamakan keberadaan dokumentasi dengan status produksi.

## XV. Referensi utama

- [README kanonis berbahasa Inggris](README.md)
- [Spesifikasi sintaks SLeeLa](SLEELA.syntax)
- [Indeks dokumentasi](docs/README.md)
- [Dokumentasi API](docs/api/README.md)
- [Dukungan platform](docs/PLATFORM-SUPPORT.md)
- [Implementasi native](impl/README.md)
- [Panduan instalasi](install/README.md)
- [Kebijakan pengujian](markdown/TESTING.md)
- [Repositori SLeeLa di GitHub](https://github.com/mearvk/SLeeLa)

**Status terjemahan:** Edisi Bahasa Indonesia ini telah diperluas menjadi panduan teknis yang mencakup pengenalan, build, instalasi, bahasa dan runtime, fitur, pustaka, integrasi native, keamanan, pengembangan, dan area utama proyek. Dokumen ini belum merupakan terjemahan baris demi baris dari seluruh README kanonis berbahasa Inggris. Pemeriksaan teknis dan penyuntingan bahasa oleh penutur Bahasa Indonesia yang memahami bidang teknis tetap disarankan.


## XVI. Target, profil, dan batas eksekusi

SLeeLa memisahkan kode sumber dari target build dan execution. Keluarga target yang didokumentasikan mencakup `SLVM`, `SLJVM`, dan `SLIR`; `NATIVE` merupakan target terbatas dan bersyarat, bukan mode yang dapat diasumsikan tersedia pada semua platform. Profil `BASIC_COMPLETE` dan `ADVANCED_TOTAL` menunjukkan cakupan yang berbeda. Verifikasi target, profil, dan dukungan platform yang benar-benar digunakan pada setiap build.

Reference mengidentifikasi file, modul, atau resource; reference tidak dengan sendirinya memberikan izin untuk mengeksekusinya. Ketika program menggunakan file, pustaka eksternal, native bindings, atau sumber daya sistem, batas otorisasi dan tanggung jawab eksekusi harus ditetapkan secara eksplisit. Pernyataan dokumentasi saja bukan bukti jaminan keamanan runtime; periksa implementasi dan pengujian terkait.

## XVII. Siklus hidup resource dan debugging

Dokumentasikan ownership, lifetime, dan tanggung jawab sinkronisasi untuk resource yang dibagikan antar-fungsi atau thread. Bedakan local variables, shared object state, VM handles, dan serializable data. VM-local handle bukan format file atau pesan jaringan yang stabil. Gunakan serialization interface yang ditetapkan modul, lalu uji nilai invalid, missing, dan boundary.

Saat debugging, catat commit, OS, architecture, dan toolchain. Build target relevan yang paling kecil terlebih dahulu, simpan seluruh error output, bedakan compiler errors dari runtime errors, dan buat contoh minimal yang dapat direproduksi. Build yang berhasil tidak otomatis membuktikan runtime behavior benar.

Laporan validasi harus menyebutkan exact commit, OS/architecture, compiler versions, seluruh build/test commands, jumlah pass/fail/skip, dan batasan yang diketahui. Test yang dilewati atau tidak dijalankan tidak boleh dihitung sebagai passed.

**Status terjemahan diperluas:** Panduan Bahasa Indonesia kini juga membahas target/profil, batas otorisasi eksekusi, siklus hidup resource, debugging, dan pelaporan validasi.
