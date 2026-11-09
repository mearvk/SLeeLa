# SLeeLa 3.0 — Suomi (FI)

<p align="center"><strong>The Now. The Ingredient.</strong></p>

## Suomenkielinen esittely

SLeeLa on natiivi järjestelmäohjelmointikieli sekä siihen kuuluva kääntäjä-, suoritusympäristö- ja virtuaalikonekokonaisuus. Se ei tarkoita pelkästään ohjelmointikielen syntaksia: kokonaisuuteen kuuluvat myös työkalut, kirjastot, rajapinnat, dokumentaatio, rakennusjärjestelmä ja alustakohtaiset sopimukset.

SLeeLa 3.0:n kansainvälinen dokumentaatio kokoaa yhteen kielen, ajonaikaisen ympäristön, kehitystyökalut ja laajenevan kirjastovalikoiman. Teknisiä tunnisteita, lähdekoodia, komentoja, tiedostonimiä ja API-nimiä ei lokalisoida, jotta esimerkit ja ohjeet säilyvät suoraan käytettävinä.

## 1. Aloittaminen

Keskeiset lähtökohdat:

- [Kanoninen tekninen README](README.md) — koko projektin normatiivinen tekninen kuvaus.
- [Natiivin toteutuksen rakennusohje](impl/README.md).
- [Virtuaalikoneen ja Modiumin käyttöopas](virtual-machine/USER-GUIDE.md).
- [Dokumentaation hakemisto](docs/README.md).
- [API-dokumentaation hakemisto](docs/api/README.md) ja [API-luettelo](api/README.md).
- [Alustatuki ja varmennusmatriisi](docs/PLATFORM-SUPPORT.md).
- [Syntaksimääritys](SLEELA.syntax).
- [Testauskäytäntö](markdown/TESTING.md).

SLeeLa sisältää myös lähdekoodissa kuvatun CPU-rakenteen logiikkaporteista ja ALU:sta prosessoriytimeen, käyttöjärjestelmään ja ohjelmiin. Katso [CPU-dokumentaatio](lib/cpu/CPU.md) ja [ajoitusparametrit](lib/cpu/TIMING.md).

## 2. Kääntäminen ja testaus

Rakennustyökalujen vaatimukset riippuvat kohteesta. Natiivin toteutuksen perusympäristöön kuuluvat C/C++-kääntäjä (esimerkiksi GCC tai Clang) ja GNU Make. Python 3:a käytetään useissa luettelo- ja työnkulkutyökaluissa. Linuxin graafiset komponentit voivat edellyttää X11- ja XRandR-kehityspaketteja.

Suorita projektin juurihakemistossa:

```sh
make
```

Natiivin toteutuksen rakennus ja koko testikokonaisuus:

```sh
make -C impl all
bash ./test-suites/run-all.sh --all
```

Tavallisia Make-kohteita:

```sh
make all
make clean
make help
sudo make install
```

Tarkista aina kunkin kohteen omat ohjeet. Yhden komponentin onnistunut käännös ei todista kaikkien kirjastojen tai käyttöjärjestelmien olevan julkaisukunnossa. Testituloksissa on erotettava suoritetut, ohitetut ja epäonnistuneet tarkistukset.

## 3. Asennus

Järjestelmäasennus käyttää tavallisesti etuliitettä `/usr/local`:

```sh
sudo make install
```

Käyttäjäkohtainen asennus ei yleensä vaadi järjestelmänvalvojan oikeuksia:

```sh
make install PREFIX="$HOME/.local"
```

Pakettihakemiston muodostaminen ja asennuksen tarkistus:

```sh
make install DESTDIR="$PWD/package-root"
make install-check
```

Puhdas asennuskohde on käytettävissä komennolla `make clean-install`. Asennusohjeet, turvallisuusrajat ja muut vaihtoehdot on kuvattu [asennusoppaassa](install/README.md). Tarkista, että käyttäjän PATH sisältää valitun asennuksen binäärihakemiston.

## 4. Kieli, kääntäjä ja suoritusympäristö

Kielen normatiivinen syntaksimääritys on tiedostossa [SLEELA.syntax](SLEELA.syntax). Tämän arkiston dokumentaatio käyttää syntaksiversiota 3.0. Kääntäjän ohjelmistoversio ja lähdekielen syntaksiversio ovat eri asioita: dokumentaatio ilmaisee kielen ominaisuuksien vähimmäisversiot eikä oleta, että jokainen kääntäjäversio vastaa yhtä syntaksinumeroa.

Projektin työkaluketjuun kuuluvat `sleelvac`-kääntäjä ja `sleela`-suoritusympäristö. Dokumentaatiossa käsitellään muun muassa `@mix`-kenttäjärjestelyä, laajennettuja kokonaislukutyyppejä, Preonem-dokumenttimerkintöjä, SleelaDocia, API-dokumentaation tuottamista ja Nordshrift-työkaluja.

Virtuaalikone- ja välikohteet sisältävät SLVM-, SLJVM- ja SLIR-polut. Natiivin kohteen käyttö on rajattu erikseen määriteltyyn rajapintaan. Keskeinen turvallisuusperiaate on: **viitteet yksilöivät kohteen; ne eivät anna lupaa suorittaa sitä**. Tunnistaminen, luottamus ja suoritusoikeus ovat erillisiä päätöksiä.

## 5. Suunnitteluperiaatteet ja varmennus

SLeeLa painottaa näyttöön perustuvaa kehitystä, selkeitä komponenttirajoja, siirrettävyyttä, turvallista virhekäyttäytymistä ja toistettavia julkaisuja. Rajapintojen tulee ilmaista odotukset ja virhetilanteet, eikä dokumentaatio saa luvata ominaisuuksia, joita toteutus tai testit eivät tue.

Varmennus on arvioitava moduuli- ja alustakohtaisesti. Testiä, jota ei ole suoritettu, ei saa merkitä hyväksytyksi. Yksittäisen osan onnistuminen ei korvaa koko julkaisun tarkastusta. SHA-256-tarkistukset ja eheysmanifestit auttavat havaitsemaan muutoksia; ne eivät yksin todista ohjelmiston turvallisuutta.

Sleelavirin-suojaustyökalut sisältävät paikallisen SHA-256-varmennuksen, valinnaisen ClamAV-liitännän ja eheysluettelon. Julkaisu voidaan nimetä varmennetuksi vasta, kun kyseisen commitin vaaditut CI-tarkistukset ovat valmistuneet ja julkaistavat artefaktit on tarkastettu.

## 6. Editorit ja kehittäjätyö

SLeeLa tarjoaa Vim- ja Emacs-editoriteemoja sekä syntaksitukea. Natiivin C/C++-paletin esikatselut ja editorien asennusohjeet löytyvät [editorioppaasta](linux-editors/README.md). Asennusmenettelyt on tarkistettava käyttöjärjestelmän ja editoriversion mukaan.

Hyvässä muutosehdotuksessa on rajattu tarkoitus, selkeä kuvaus, positiiviset ja negatiiviset testit sekä tiedot käytetystä käyttöjärjestelmästä ja työkaluketjusta. Suorittamatta jääneet tarkistukset on ilmoitettava avoimesti.

## 7. Kirjastot ja projektin osa-alueet

Kanoninen README kuvaa laajaa pakettihakemistoa sekä Advantage-, SleelaDoc- ja XML DOM -kirjastoja. Muita dokumentoituja aiheita ovat kokonaislukujen perustyypit, merkistö- ja ajoympäristö, AutoCAD Creator™, UTF-4088-kokeilu, Pi-vakio, Modium™, Registry™ ja `sleelactl`.

UTF-4088:n miljoonan merkinnän luettelo ja muut kokeelliset aineistot on tulkittava niiden omien kuvausten mukaisesti: kokeellinen luettelo ei itsessään tarkoita, että kaikki merkinnät olisivat tuotantokäytössä. Pakettihakemiston ylläpidossa on säilytettävä polkujen, omistajuuden, riippuvuuksien ja testitilan jäljitettävyys.

## 8. `@native` ja natiivin C/C++-polun rajaus

Dokumentaatio kuvaa `@native`-merkinnän ja `sleela aot` -komennon ensimmäisen vaiheen SLeeLa-lähdekoodista natiivin C/C++-polun suuntaan. Tätä ei pidä tulkita rajattomaksi natiiviksi suoritusoikeudeksi: rajapinta, ABI, hyväksyntä ja turvallisuusportit ovat edelleen olennaisia. Natiivin kohteen tila on tarkistettava toteutuksen ja testien perusteella.

## 9. Julkaisut ja kansainväliset README-versiot

Projektin uutiset, julkaisutiedot ja kansainvälisten README-tiedostojen luettelo ylläpidetään kanonisessa README-tiedostossa. Maa- tai kielikohtainen tiedosto on lokalisoitu aloituspiste, ei automaattisesti täydellinen käännös kaikista teknisistä luvuista.

## 10. Linkit

- [Kanoninen README (englanti)](README.md)
- [Syntaksimääritys](SLEELA.syntax)
- [Dokumentaatio](docs/README.md)
- [Alustatuki](docs/PLATFORM-SUPPORT.md)
- [Natiivin toteutuksen ohjeet](impl/README.md)
- [Asennusopas](install/README.md)
- [Testauskäytäntö](markdown/TESTING.md)
- [SLeeLa GitHub -projekti](https://github.com/mearvk/SLeeLa)

## Suomen- ja ruotsinkielinen huomautus

**Suomi:** Tämä on laajennettu suomenkielinen tekninen opas, joka kattaa projektin tärkeimmät käsitteet, rakennuksen, asennuksen, varmennuksen ja dokumentaatiopolut. Se ei vielä ole kanonisen englanninkielisen README-tiedoston täydellinen rivi riviltä tehty käännös.

**Svenska:** Den här landsutgåvan innehåller en utökad finskspråkig teknisk introduktion och en svensk ingång. Den är ännu inte en fullständig rad-för-rad-översättning av den kanoniska engelska README-filen. Kontrollera alltid den engelska specifikationen för normativa tekniska detaljer.

SLeeLa on MEARVK LLC:n kehittämä.
