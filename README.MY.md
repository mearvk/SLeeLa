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
