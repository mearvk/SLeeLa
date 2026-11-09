# SLeeLa 3.0 — Pilipinas (PH)

## Filipino · English

Ang SLeeLa ay isang katutubong wika sa pagprograma ng sistema na may compiler, runtime, at ekosistema ng mga virtual machine. Pinagsasama nito ang wika, compiler, mga library, tool para sa developer, at mga kasunduan ng platform upang makabuo at magpatakbo ng software.

**The Now. The Ingredient.** Hindi lamang syntax ang isang programming language. Bahagi rin ng isang magagamit na software system ang mga toolchain at runtime nito.

## I. Magsimula rito

- [Buuin ang native implementation](impl/README.md)
- [Gabay sa virtual machine at Modium](virtual-machine/USER-GUIDE.md) — `sleela vm`, `sleela modium`
- [Arkitektura ng CPU sa SLeeLa source](lib/cpu/CPU.md) — mahigit 130 CPU profile; tingnan ang [mga timing control](lib/cpu/TIMING.md).
- [Indeks ng dokumentasyon](docs/README.md)
- [Nabuong dokumentasyon ng API](docs/api/README.md) at [katalogo ng API](api/README.md)
- [Suporta sa platform at validation](docs/PLATFORM-SUPPORT.md)
- [Patakaran sa testing](markdown/TESTING.md)

## II. Pagbuo at pagsubok

Kabilang sa mga kailangan ang C/C++ toolchain (GCC o Clang), GNU Make, at Python 3. Maaaring mangailangan din ang mga graphical component sa Linux ng X11/Xrandr development package.

Patakbuhin mula sa root directory ng repository:

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

Hindi nangangahulugan ang matagumpay na build ng isang component na handa na para sa release ang lahat ng library o platform. Tingnan ang dokumentasyon ng build at platform para sa mga requirement ng bawat target.

## III. Pag-install

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

Karaniwang `/usr/local` ang system installation prefix. Sa paggamit ng `PREFIX="$HOME/.local"`, makakapag-install ang user sa sariling directory at karaniwang hindi kailangan ang administrator privileges. Para sa gabay sa installation at detalye ng package, tingnan ang [installation guide](install/README.md).

## IV. Wika at runtime

Nasa [`SLEELA.syntax`](SLEELA.syntax) ang normative grammar. Syntax version **3.0** ang ginagamit sa dokumentasyon ng repository. Magkaiba ang compiler version at source-language syntax version.

Kabilang sa mga teknikal na paksa ang mga contiguous field na `@mix`, pinalawak na pamilya ng integer, Preonem document marker, SleelaDoc, API documentation generation, at mga tool ng Nordshrift. Hindi binabago ang mga technical identifier, command, filename, API name, at code example upang magamit nang direkta.

## V. Mga prinsipyo sa engineering at validation

Binibigyang-diin ng SLeeLa ang ebidensiya, malinaw na kontrata sa pagitan ng mga component, portability, ligtas na pag-uugali kapag may failure, at reproducible release. Dapat suriin ang validation ayon sa module at platform. Hindi dapat iulat na pumasa ang mga test na nilaktawan; hindi rin katumbas ng buong release validation ang matagumpay na build ng iisang component.

Kabilang sa Sleelavirin protection tooling ang lokal na SHA-256 verifier, opsyonal na ClamAV adapter, at integrity manifest. Maituturing lamang na validated ang isang release kapag kumpleto na ang CI checks para sa release commit at nasuri na ang mga artifact.

## VI. Mga editor at kontribusyon

May mga theme at syntax integration ang SLeeLa para sa **Vim** at **Emacs**, pati native C/C++ palette utility. Tingnan ang [gabay sa editor integration](linux-editors/README.md).

Mga mungkahi para sa kontribusyon:

1. Basahin ang documentation index at testing policy.
2. Gumawa ng malinaw ang saklaw na pagbabago at magsama ng positive at negative test.
3. Patakbuhin ang mga kaugnay na test, pagkatapos ay ang mas malawak na test suite.
4. Ilahad ang operating system at toolchain na ginamit. Huwag sabihing pumasa ang mga check na nilaktawan.

Binuo ang SLeeLa ng **MEARVK LLC**.

## VII. Iba pang teknikal na paksa

Tinatalakay rin ng pangunahing README ang mga library ng Advantage, SleelaDoc, at XML DOM; mga pangunahing integer type; character runtime; AutoCAD Creator™; eksperimental na UTF-4088 catalogue; mga tool na Pi, Modium, Registry, at `sleelactl`; at ang unang yugto ng native C/C++ compilation path na pinapagana ng `@native` marker.

## Kumpletong teknikal na sanggunian

Nasa [canonical README](README.md) ang buong feature catalogue, sanggunian sa wika at runtime, mga tagubilin sa build at installation, mga link sa API, halimbawa, at release notes. Pinalawak ng edisyong Filipino ang praktikal na gabay, ngunit hindi pa ito kumpletong salin, bawat linya, ng lahat ng teknikal na seksyon. Mananatiling normative technical source ang English README hanggang masuri ang buong salin.

- [Canonical README](README.md)
- [SLeeLa syntax specification](SLEELA.syntax)
- [Documentation index](docs/README.md)
- [Platform support](docs/PLATFORM-SUPPORT.md)
- [GitHub repository](https://github.com/mearvk/SLeeLa)

**Katayuan ng salin:** Isinalin ang panimula, pagsisimula, build, installation, pangkalahatang paliwanag sa wika at runtime, mga prinsipyo sa engineering, validation, at kontribusyon. Kailangan pang isalin at repasuhin ng mga teknikal na tagasuri ang lahat ng natitirang seksyon.
