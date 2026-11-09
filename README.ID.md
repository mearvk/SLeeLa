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
