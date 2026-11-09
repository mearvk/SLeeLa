# SLeeLa 3.0 — Malaysia (MY)

## Bahasa Melayu

SLeeLa ialah bahasa pengaturcaraan sistem natif yang merangkumi pengkompil, persekitaran masa jalan dan ekosistem mesin maya. Bahasa, pengkompil, pustaka, alat pembangun dan kontrak platformnya bekerjasama untuk membina serta menjalankan perisian.

**The Now. The Ingredient.** Bahasa pengaturcaraan bukan sekadar sintaks. Komponen toolchain dan persekitaran masa jalan turut membentuk sistem perisian yang boleh digunakan.

## I. Mula di sini

- [Bina pelaksanaan natif](impl/README.md)
- [Panduan pengguna mesin maya dan Modium](virtual-machine/USER-GUIDE.md) — `sleela vm`, `sleela modium`
- [Seni bina CPU dalam kod sumber SLeeLa](lib/cpu/CPU.md) — lebih daripada 130 profil CPU; rujuk [tetapan pemasaan](lib/cpu/TIMING.md).
- [Indeks dokumentasi](docs/README.md)
- [Dokumentasi API terjana](docs/api/README.md) dan [katalog API](api/README.md)
- [Sokongan platform dan pengesahan](docs/PLATFORM-SUPPORT.md)
- [Dasar pengujian](markdown/TESTING.md)

## II. Bina dan uji

Keperluan termasuk toolchain C/C++ (GCC atau Clang), GNU Make dan Python 3. Komponen grafik Linux mungkin turut memerlukan pakej pembangunan X11/Xrandr.

Jalankan dari direktori akar repositori:

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

Kejayaan membina satu komponen tidak membuktikan bahawa semua pustaka atau platform sudah bersedia untuk keluaran. Rujuk dokumentasi binaan dan platform bagi keperluan setiap sasaran.

## III. Pemasangan

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

Awalan pemasangan sistem lazimnya ialah `/usr/local`. Tetapan `PREFIX="$HOME/.local"` menyediakan pemasangan setempat pengguna dan biasanya tidak memerlukan keistimewaan pentadbir. Untuk aliran pemasangan berpandu dan butiran pakej, lihat [panduan pemasangan](install/README.md).

## IV. Bahasa dan persekitaran masa jalan

Tatabahasa normatif ditakrifkan dalam [`SLEELA.syntax`](SLEELA.syntax). Dokumentasi repositori menggunakan sintaks **3.0**. Versi pengkompil dan versi sintaks bahasa sumber ialah dua perkara yang berbeza.

Rujukan teknikal merangkumi medan berjujukan `@mix`, keluarga integer lanjutan, penanda dokumen Preonem, SleelaDoc, penjanaan dokumentasi API dan alat Nordshrift. Pengecam teknikal, arahan, nama fail, nama API dan contoh kod dikekalkan tanpa perubahan supaya boleh digunakan terus.

## V. Prinsip kejuruteraan dan pengesahan

SLeeLa mengutamakan bukti, kontrak komponen yang jelas, keboleh alihan, tingkah laku selamat apabila berlaku kegagalan dan keluaran yang boleh dihasilkan semula. Status pengesahan hendaklah dinilai mengikut modul dan platform. Ujian yang dilangkau tidak boleh dilaporkan sebagai lulus; kejayaan satu komponen bukan pengesahan keluaran sepenuhnya.

Alat perlindungan Sleelavirin merangkumi penyemak SHA-256 setempat, penyesuai ClamAV pilihan dan manifes integriti. Sesuatu keluaran hanya boleh dianggap disahkan selepas pemeriksaan CI bagi commit keluaran selesai dan artifak yang terhasil telah disemak.

## VI. Editor dan sumbangan

SLeeLa menyediakan tema dan integrasi sintaks untuk **Vim** dan **Emacs**, serta utiliti palet natif C/C++. Lihat [panduan integrasi editor](linux-editors/README.md).

Panduan sumbangan:

1. Baca indeks dokumentasi dan dasar pengujian.
2. Hantar perubahan yang skopnya jelas bersama ujian positif dan negatif.
3. Jalankan ujian berkaitan, kemudian rangkaian ujian yang lebih menyeluruh.
4. Nyatakan sistem pengendalian dan toolchain yang digunakan. Jangan laporkan ujian yang dilangkau sebagai berjaya.

SLeeLa dibangunkan oleh **MEARVK LLC**.

## VII. Rujukan teknikal lain

README utama turut membincangkan pustaka Advantage, SleelaDoc dan XML DOM; jenis integer asas; persekitaran masa jalan aksara; AutoCAD Creator™; katalog eksperimen UTF-4088; alat Pi, Modium, Registry dan `sleelactl`; serta peringkat awal laluan kompilasi C/C++ natif yang diaktifkan oleh penanda `@native`.

## Rujukan teknikal lengkap

[README kanonik](README.md) mengandungi katalog ciri penuh, rujukan bahasa dan persekitaran masa jalan, arahan binaan dan pemasangan, pautan API, contoh serta nota keluaran. Edisi Bahasa Melayu ini memperluas panduan praktikal tetapi belum menjadi terjemahan baris demi baris bagi keseluruhan README teknikal. README bahasa Inggeris kekal sebagai sumber teknikal normatif sehingga terjemahan lengkap disemak.

- [README kanonik](README.md)
- [Spesifikasi sintaks SLeeLa](SLEELA.syntax)
- [Indeks dokumentasi](docs/README.md)
- [Sokongan platform](docs/PLATFORM-SUPPORT.md)
- [Repositori GitHub](https://github.com/mearvk/SLeeLa)

**Status terjemahan:** Pengenalan, panduan permulaan, binaan, pemasangan, bahasa/persekitaran masa jalan, prinsip kejuruteraan, pengesahan dan sumbangan telah dilokalkan. Terjemahan penuh semua bahagian teknikal masih perlu disiapkan dan disemak oleh penutur Bahasa Melayu yang mempunyai pengetahuan teknikal.


## VIII. Seni bina bahasa dan saluran kompilasi

SLeeLa memisahkan bahasa sumber, frontend pengkompil, representasi perantaraan, penjanaan kod dan runtime. Versi pengkompil, edisi sintaks dan versi VM/ISA ialah perkara yang berbeza. Kewujudan sesuatu ciri dalam spesifikasi tidak bermakna ciri itu telah disokong dan disahkan pada setiap sasaran.

Aliran kerja lazim:

1. **Sumber:** fail UTF-8 `.sleela` dengan pragma versi dan deklarasi.
2. **Lexer dan parser:** menukarkan teks kepada token dan struktur sintaks.
3. **Analisis semantik:** menyemak nama, simbol, jenis dan peraturan bahasa.
4. **Representasi perantaraan:** bentuk program berstruktur untuk proses lowering seterusnya.
5. **Penjanaan kod:** output bagi sasaran VM atau perantaraan yang disokong.
6. **Runtime:** pelaksanaan program terkompil dengan mekanisme memori dan aliran kawalan yang tersedia.

Rujukan normatif ialah [SLEELA.syntax](SLEELA.syntax). Untuk format sumber dan kitar hayat fail, lihat [dokumentasi sumber](markdown/SOURCE.md) dan [dokumentasi pengkompil](markdown/COMPILER.md).

## IX. Jenis data dan model objek

Kosa kata jenis sumber merangkumi `int`, `double`, `boolean`, `String`, `void`, jenis `struct` yang diisytiharkan dan tandatangan jenis rujukan yang disokong. Dalam model yang didokumenkan sekarang, `int`, `long` dan `int64` ialah integer bertanda 64-bit. `int128` diterima sebagai jenis integer tetapi belum patut dianggap sebagai aritmetik 128-bit penuh.

`struct` ialah agregat bernama yang mengandungi medan. Pemegang struct yang diurus VM bukan penuding mentah. Tugasan dan penghantaran parameter berkongsi instans yang sama; kesamaan adalah berdasarkan identiti. Untuk pertukaran mudah alih, gunakan fungsi bersiri yang didokumenkan seperti `structPack(instance)` dan `structUnpack(TypeName, json)`; pemegang yang terikat kepada VM bukan nilai rangkaian mudah alih.

Kelas mengandungi medan dan kaedah. Dalam model pelaksanaan semasa, medan ialah keadaan kelas yang dikongsi manakala pemboleh ubah setempat kaedah adalah khusus kepada thread. Pewarisan, kelas bersarang dan generik tertakluk pada pintu versi sintaks. Rujuk [International Language Reference](LANGUAGE.INTERNATIONAL.3.0.md).

## X. Aliran kawalan dan pemasaan gelung

Sintaks bahasa menyediakan pernyataan bersyarat, gelung, panggilan kaedah dan rekursi. Anotasi peringkat pernyataan `@loopDelay` dan `@loopBudget` menetapkan jeda atau belanjawan masa bagi gelung. Anotasi diletakkan sebelum gelung dan diturunkan kepada arahan VM `OP_SLEEP` (ISA 157). Fungsi terbina dalam `sleep(ms)` menggunakan arahan yang sama.

```sleela
@loopDelay(ms = 250)
while (running) {
    processNext();
}
```

Contoh ini menunjukkan bentuk sintaks; pengecam dan kelas yang diperlukan mesti ditakrifkan dalam program sebenar. Lihat [SLEELA.syntax §32](SLEELA.syntax) dan [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md).

## XI. Pustaka standard dan modul

Repository mengandungi modul dengan tahap kematangan yang berbeza:

- `lib/advantage` — penilaian berlapis terhadap hasil berbanding model; meliputi matematik, ekonomi perdagangan dua hala, integrasi, penggredan dan output boleh hala. Perbincangan medan kepercayaan berasaskan fizik ialah analogi pemodelan, bukan dakwaan undang-undang fizik.
- `lib/xml` — pepohon dokumen XML, elemen, teks, komen, CDATA, arahan pemprosesan, atribut, parser dan serializer.
- `lib/text` — operasi teks dan rentetan berasaskan sel aksara seperti `charAt`, `substring`, `indexOf`, `reverse` dan pemformatan kedudukan.
- `lib/sleeladoc` — pengekstrakan dokumentasi sumber dan halaman rujukan CLI. `sleela docs` menyenaraikan topik, pakej dan kelas; `sleela docs --generate [dir]` membina korpus teks setempat untuk kegunaan luar talian.
- `lib/cpu` dan `lib/xlt` — profil CPU, komponen seni bina, format model dan rujukan pemasaan.
- `recursion/stanchion` — prototaip rujukan C/C++ dan contoh corak rekursi.

Kewujudan modul dalam katalog tidak membuktikan kesediaan produksi. Semak README modul, liputan ujian, had platform dan rekod pengesahan.

## XII. Integrasi C/C++ dan sempadan native

Repository SLeeLa mengandungi komponen C dan C++ serta laluan penjanaan kod native yang terhad. Penanda `@native` dan arahan `sleela aot` berkaitan pembangunan native; kedua-duanya bukan kebenaran umum untuk melaksanakan kod native sewenang-wenangnya.

Integrasi C/C++ mesti menyatakan kontrak ABI, konvensyen panggilan, pemilikan memori, susun atur data, pengendalian ralat dan kebergantungan platform. Anggap kod native sebagai sempadan kepercayaan yang berasingan. Rujukan atau deklarasi hanya mengenal pasti sumber; ia tidak memberikan kebenaran pelaksanaan. Lihat [native implementation](impl/README.md), [platform matrix](docs/PLATFORM-SUPPORT.md) dan [comparison standards](comparison/README.md).

## XIII. Bina, pasang dan sokongan platform

Build native lazimnya memerlukan toolchain C/C++ seperti GCC atau Clang dan GNU Make. Sesetengah alat menggunakan Python 3. Komponen GUI Linux mungkin memerlukan pakej pembangunan X11 dan XRandR.

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

Pemasangan seluruh sistem lazimnya:

```sh
sudo make install
```

Pemasangan untuk pengguna semasa sahaja:

```sh
make install PREFIX="$HOME/.local"
```

Baca [installation guide](install/README.md) dan [platform support matrix](docs/PLATFORM-SUPPORT.md) untuk prasyarat serta skrip khusus platform. Jangan anggap arahan atau kebergantungan yang sama terpakai secara seragam pada Linux, macOS dan Windows.

## XIV. Keselamatan, integriti dan ujian

Manifest hash dan alat pengesahan boleh membandingkan fail repository dengan kandungan yang dijangka. Keputusan PASS hanya meliputi skop dan masa pemeriksaan yang dinyatakan. Laporan ujian mesti membezakan pass, fail dan skip; ujian yang dilangkau atau tidak dijalankan tidak boleh dilaporkan sebagai lulus.

Prinsip utama:

- Rujukan mengenal pasti sumber; ia tidak memberikan kebenaran pelaksanaan.
- Antara muka native mesti jelas dan terhad.
- Dakwaan sokongan platform perlu disokong oleh build dan ujian sebenar.
- Dokumentasi terjana mesti boleh dijejaki kepada sumber dan generator.
- Pemeriksaan keselamatan perlu gagal secara selamat apabila data integriti wajib tiada.

Lihat [testing policy](markdown/TESTING.md) dan [platform validation](docs/PLATFORM-SUPPORT.md).

## XV. Dokumentasi, editor dan sumbangan

Mulakan dengan [documentation index](docs/README.md), [generated API reference](docs/api/README.md), [API catalogue](api/README.md) dan [SleelaDoc](lib/sleeladoc/README.md). Untuk sumber editor Linux, lihat [linux-editors](linux-editors/README.md).

Sebelum menyumbang, baca README modul, tetapkan skop perubahan, tambah ujian positif dan negatif, jalankan ujian berkaitan, rekod OS/toolchain/arahan/hasil sebenar dan kemas kini README serta dokumentasi API apabila tingkah laku awam berubah.

## XVI. Bidang projek tambahan

Model CPU dan perkakasan, GUI, rangkaian, pemprosesan teks, UTF-4088, alat π, Modium, Registry, `sleelactl` dan AutoCAD Creator™ ialah bidang projek yang berasingan. Tujuan, kebergantungan dan tahap kematangannya mungkin berbeza. Jangan label direktori eksperimen atau contoh sebagai ciri produksi stabil tanpa pengesahan yang didokumenkan. Saiz atau bilangan entri UTF-4088 tidak membuktikan keserasian Unicode penuh. Keserasian AutoCAD memerlukan matriks versi dan keputusan ujian yang jelas.

## XVII. Penggunaan di Malaysia dan penyetempatan

Edisi Malaysia ini menggunakan Bahasa Melayu sebagai bahasa utama. Istilah teknikal boleh dikekalkan dalam bahasa Inggeris apabila terjemahan akan mengaburkan nama API, konsep compiler atau arahan. Nama fail, command, identifier dan kod sumber hendaklah dikekalkan tepat. Sumbangan istilah tempatan digalakkan melalui glosari yang konsisten dan semakan penutur jati yang memahami bidang teknikal.

**Status terjemahan:** Panduan Bahasa Melayu ini telah diperluas untuk merangkumi saluran kompilasi, model jenis dan objek, pemasaan gelung, pustaka, integrasi native, build/pemasangan, keselamatan, ujian dan aliran sumbangan. Ia belum merupakan terjemahan baris demi baris bagi keseluruhan README kanonik dan spesifikasi berkaitan.


## XVIII. Sasaran, profil dan sempadan pelaksanaan

SLeeLa membezakan kod sumber daripada sasaran binaan dan pelaksanaan. Keluarga sasaran yang didokumenkan merangkumi `SLVM`, `SLJVM` dan `SLIR`; `NATIVE` ialah sasaran terhad dan bersyarat, bukan pilihan yang boleh dianggap tersedia pada semua platform. Profil `BASIC_COMPLETE` dan `ADVANCED_TOTAL` menunjukkan skop yang berbeza. Sahkan sasaran, profil dan sokongan platform sebenar bagi setiap binaan.

Reference mengenal pasti fail, modul atau sumber; ia tidak memberikan kebenaran untuk melaksanakan sesuatu tindakan. Apabila program menggunakan fail, pustaka luaran, native bindings atau sumber sistem, sempadan kebenaran dan tanggungjawab pelaksanaan mesti ditentukan dengan jelas. Pernyataan dalam dokumentasi sahaja bukan bukti jaminan keselamatan runtime; semak pelaksanaan dan ujian berkaitan.

## XIX. Kitar hayat sumber dan penyahpepijatan

Dokumentasikan pemilikan, jangka hayat dan tanggungjawab penyegerakan bagi sumber yang dikongsi antara fungsi atau thread. Bezakan local variables, shared object state, VM handles dan serializable data. VM-local handle bukan format fail atau mesej rangkaian yang stabil. Gunakan serialization interface yang ditakrifkan oleh modul dan uji nilai invalid, missing serta boundary.

Semasa debug, rekodkan commit, OS, architecture dan toolchain. Bina sasaran terkecil yang berkaitan dahulu, simpan output ralat penuh, bezakan compiler errors daripada runtime errors dan hasilkan contoh minimum yang boleh diulang. Build yang berjaya tidak dengan sendirinya membuktikan runtime behavior yang betul.

Laporan pengesahan hendaklah menyatakan exact commit, OS/architecture, compiler versions, arahan build/test penuh, bilangan pass/fail/skip dan batasan yang diketahui. Ujian yang dilangkau atau tidak dijalankan tidak boleh dikira sebagai lulus.

**Status terjemahan diperluas:** Panduan Bahasa Melayu kini turut merangkumi sasaran/profil, sempadan pelaksanaan, kitar hayat sumber, penyahpepijatan dan laporan pengesahan.


## XX. Kontrak API modul, keserasian dan strategi ujian

Apabila API baharu diperkenalkan, dokumentasikan input, output, keadaan ralat, pemilikan data dan sama ada operasi menghasilkan side effects. Contoh dokumentasi hendaklah menunjukkan penggunaan biasa serta pengendalian input invalid atau missing. Jangan menyatakan sesuatu tingkah laku sebagai jaminan jika implementation dan tests belum mengesahkannya.

Asingkan ujian mengikut peringkat compiler pipeline: lexing/parsing, semantic analysis, IR lowering, code generation, VM execution dan native integration. Bagi setiap regression case, simpan input minimum yang boleh diulang, hasil yang dijangka dan hasil sebenar. Perubahan API mesti menerangkan backward compatibility, migration steps dan kesan terhadap examples sedia ada.

## XXI. Penyetempatan, pengekodan dan kebolehbacaan

Kekalkan technical identifiers, commands, paths, syntax dan code blocks dalam bentuk yang sama merentas bahasa. Terjemahkan penerangan, bukan token atau nama yang digunakan dalam source code. Nyatakan bahasa dan konteks wilayah dengan jelas; istilah Bahasa Melayu boleh berbeza mengikut komuniti dan negara.

Selepas setiap kemas kini, periksa Markdown links, hierarki tajuk, code fences dan examples. Apabila canonical English documents berubah, bandingkan bahagian berkaitan dan kemas kini terjemahan secara menyeluruh, bukan sekadar pengenalan. Tandakan kandungan yang belum disemak oleh penutur asli atau penyemak teknikal.
