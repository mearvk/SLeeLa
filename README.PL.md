# SLeeLa 3.0 — Polska (PL)

## Polski

SLeeLa to natywny język programowania systemowego, kompilator, środowisko uruchomieniowe oraz ekosystem maszyn wirtualnych.

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>

**The Now. The Ingredient.** Język programowania to coś więcej niż składnia. Obejmuje również kompilator, środowisko uruchomieniowe, biblioteki, narzędzia i kontrakty platformowe, które umożliwiają praktyczne wykorzystanie oprogramowania.

**Obszary działania SLeeLa.** SLeeLa rozwija cztery wymiary pracy: **Math**, **Object**, **Science** i **Reach** — odpowiednio to, co można obliczyć; obiekt ujęty w katalogu tego, co istnieje; zdyscyplinowane badanie prawdy; oraz rozszerzanie możliwości systemu na inne systemy i ludzi. **Interesy narodowe pozostają odrębnym zagadnieniem** i nie są zastępowane przez te cztery wymiary. Jeżeli temat dotyczy interesów danego państwa, zależność ta powinna być zachowana i precyzyjnie udokumentowana, na przykład w polach metadanych klasy `metaOrigin` i `metaNationalSchedule`.

## Rozpocznij tutaj

- [Budowanie implementacji natywnej](impl/README.md)
- [Maszyny wirtualne i Modium — praktyczny przewodnik](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [Stos CPU w kodzie źródłowym SLeeLa](lib/cpu/CPU.md): od bramek logicznych i ALU po rdzenie, system operacyjny i programy; obejmuje ponad 130 profili CPU oraz [napięcia, zmienne częstotliwości i zaawansowane parametry taktowania](lib/cpu/TIMING.md).
- [Indeks dokumentacji](docs/README.md)
- [Źródło danych CSPML i opracowanie Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md): pochodzenie i wskazówki integracyjne dla zewnętrznych danych USA dotyczących lokalizacji, warunków społeczno-ekonomicznych, rynku pracy, edukacji i szkół publicznych.
- [Wygenerowana dokumentacja API](docs/api/README.md) oraz [katalog API](api/README.md)
- [Standardy porównawcze](comparison/README.md): typy, pamięć, ABI i zgodność SLeeLa VM, Java/JVM oraz natywnego C/C++.
- [Stanchion rekurencji — prototypy C i C++](recursion/stanchion/README.md)
- [Macierz obsługi i walidacji platform](docs/PLATFORM-SUPPORT.md)
- [Przewodnik po marce](docs/BRAND-GUIDE.md)
- [Merchandising i zasoby graficzne](merchandise/README.md)
- [Polityka testów i kryteria wydania](markdown/TESTING.md)

## Zakres dokumentacji technicznej

Kanoniczny [README.md](README.md) zawiera pełny katalog funkcji, opis języka i środowiska uruchomieniowego, instrukcje budowania i instalacji, odnośniki do API, przykłady oraz informacje o wydaniach. Zawiera również najnowsze materiały dotyczące biblioteki `advantage`, adnotacji czasu pętli, SleelaDoc, modelu XML DOM, rozszerzonych typów całkowitych, Preonems, edytorów Vim i Emacs, Sleelavirin, UTF-4088, AutoCAD Creator, katalogu pakietów oraz nowych funkcji Modium/Registry i `@native`.

## Budowanie i testowanie

Wymagane są kompilator C/C++ (GCC lub Clang), GNU Make oraz Python 3 dla narzędzi manifestu i przepływu pracy. W systemie Linux produkty graficzne mogą dodatkowo wymagać pakietów deweloperskich X11/Xrandr.

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

Pełne instrukcje, cele Makefile, instalacja systemowa i instalacja lokalna bez uprawnień administratora są opisane w kanonicznym README oraz w [przewodniku instalacji](install/README.md). Pomyślne zbudowanie jednego komponentu nie oznacza automatycznie, że wszystkie biblioteki i platformy są gotowe do wydania.

## Język, runtime i zasady inżynierskie

Normatywna gramatyka znajduje się w [`SLEELA.syntax`](SLEELA.syntax). Repozytorium dokumentuje składnię **3.0**; wersja kompilatora i wersja składni źródłowej są odrębnymi pojęciami. Dokumentacja opisuje między innymi adnotacje `@mix`, rozszerzone rodziny typów całkowitych, Preonems, obsługę dokumentacji SleelaDoc i generowanie API. Stan implementacji i walidacji należy oceniać osobno dla każdego modułu.

Zasady projektu obejmują: dowody przed deklaracjami, jawne kontrakty na granicach komponentów, przenośność od początku projektowania, bezpieczne zachowanie w przypadku błędów oraz powtarzalne wydania. Pakiet antywirusowy Sleelavirin udostępnia natywną kontrolę SHA-256, opcjonalny adapter ClamAV i manifesty integralności. Weryfikacja wydania wymaga pomyślnego CI dla dokładnego commitu wydania oraz sprawdzenia artefaktów.

## Edytory i wkład w projekt

SLeeLa dostarcza ciemne schematy kolorów i integrację składniową dla **Vim** oraz **Emacs**, a także natywne narzędzie palet C/C++. Instalację opisano w [przewodniku integracji edytorów](linux-editors/README.md). Wkład powinien obejmować ukierunkowane zmiany, testy pozytywne i negatywne oraz jasne wskazanie systemu operacyjnego i toolchainu. Pominiętych kontroli nie należy przedstawiać jako zaliczonych.

## Pozostałe dziedziny i materiały

Dokumentacja obejmuje także inicjatywę AE6E66 i jej deklarowaną oczekiwaną stawkę — wyłącznie jako oczekiwanie projektu, nie jako obowiązujące zobowiązanie prawne; natywne środowisko znaków i katalog eksperymentalny UTF-4088; AutoCAD Creator™; mapę wejściową UTF-4088; rozszerzony indeks pakietów; materiały Bitcoin Conjegeum i grafiki National SLeeLa Science. Szczegóły, zastrzeżenia i odnośniki znajdują się w odpowiednich sekcjach kanonicznego README.

## Ostatnie dodatki — Pi, Modium/Registry i `@native`

Repozytorium opisuje zweryfikowane pliki referencyjne π o 100 000 000 i 57 344 cyfrach. Punkty wejścia `sleela registry` i `sleela modium` ładują katalogową stałą Pi podczas uruchamiania odpowiedniego podsystemu, a nie przy każdym `sleela run`. Przewodnik instalacyjny instaluje również `sleelactl`, plan sterowania SLeeLa, wraz z rejestrem modułów i natywnym obszarem `math`.

Adnotacja metody `@native` oraz `sleela aot` stanowią pierwszą część ścieżki tłumaczenia SLeeLa do natywnego C/C++. Narzędzie sprawdza, czy oznaczona metoda mieści się w obsługiwanym podzbiorze; odrzuca nieobsługiwane konstrukcje zamiast emitować częściowo przetłumaczony kod. Adnotacja na tym etapie nie zmienia semantyki wykonywania programu.

## Międzynarodowe wydania README

Repozytorium zawiera pliki wejściowe README dla 50 rynków. Lista krajów i kodów znajduje się w [kanonicznym indeksie międzynarodowym](README.md#xix-international-readme-editions).

---

**Status tłumaczenia:** polska edycja lokalizuje wprowadzenie i główne obszary dokumentacji. Pełny tekst techniczny sekcji I–XIX nie został jeszcze przetłumaczony w całości; do czasu zakończenia tłumaczenia kanoniczny README pozostaje źródłem normatywnym.
