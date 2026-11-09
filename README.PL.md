# SLeeLa 3.0 — Polska (PL)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="Logo SLeeLa" width="560"></p>

<p align="center"><strong>Natywny język programowania systemowego, kompilator, środowisko uruchomieniowe i ekosystem maszyn wirtualnych.</strong></p>

**The Now. The Ingredient.** Język to coś więcej niż składnia: obejmuje kompilator, środowisko uruchomieniowe, biblioteki, narzędzia i kontrakty platformowe, które sprawiają, że oprogramowanie nadaje się do praktycznego użycia.

**Zakres działania SLeeLa.** SLeeLa obejmuje **Math**, **Object**, **Science** i **Reach** — cztery wymiary pracy: to, co można obliczyć (Math); ustalony obiekt w katalogu tego, co istnieje (Object); zdyscyplinowane badanie prawdy (Science); oraz rozszerzanie możliwości systemu na inne systemy i ludzi (Reach). **Interesy narodowe pozostają odrębne** i nie są pomijane przez żaden z tych wymiarów. Jeżeli temat dotyczy interesów danego państwa, zależność ta jest zachowywana i wiernie dokumentowana, na przykład w metadanych klasy `metaOrigin` i `metaNationalSchedule`.

## I. Rozpocznij tutaj

- [Budowanie implementacji natywnej](impl/README.md)
- [Maszyny wirtualne i Modium — praktyczny przewodnik](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [Stos CPU w kodzie źródłowym SLeeLa](lib/cpu/CPU.md) — pełny łańcuch od bramek logicznych przez ALU i rdzenie po system operacyjny oraz programy; ponad 130 profili CPU i regulowane [napięcia, zmienne częstotliwości oraz zaawansowane parametry taktowania](lib/cpu/TIMING.md), z możliwością pracy [zgodnie ze specyfikacją lub po podkręceniu](lib/cpu/components/README.md).
- [Indeks dokumentacji](docs/README.md)
- [Źródło danych CSPML i opracowanie Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — pochodzenie i wskazówki integracyjne dotyczące zewnętrznych danych USA o lokalizacji, warunkach społeczno-ekonomicznych, rynku pracy, edukacji i szkołach publicznych.
- [Wygenerowana dokumentacja API](docs/api/README.md) — aktualna dokumentacja HTML, wersjonowane wydanie API i instrukcje ponownego generowania.
- [Katalog API](api/README.md)
- [Standardy porównań obliczeniowych](comparison/README.md) — standardy typów, pamięci, ABI i zgodności dla SLeeLa VM, Java/JVM oraz natywnego C/C++.
- [Stanchion rekurencji — prototypy C i C++](recursion/stanchion/README.md) — przykłady dla typów Boolean, Byte, Character, Integer, Long, Float i Double, a także wzorce rekurencji ogólnej, stosowanej, naukowej i dydaktycznej.
- [Macierz obsługi i walidacji platform](docs/PLATFORM-SUPPORT.md)
- [Przewodnik po marce](docs/BRAND-GUIDE.md)
- [Koncepcje merchandisingowe i zasoby graficzne](merchandise/README.md)
- [Logo NitroWebExpress](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Polityka testów i kryteria wydania](markdown/TESTING.md)

## II. Aktualności

Najnowsze rozszerzenia języka, sterowników i biblioteki standardowej:

- **Pakiet biblioteki standardowej `advantage`.** [`lib/advantage/`](lib/advantage/README.md) modeluje optymalizację unikatowej dywidendy względem modelu jako warstwową strukturę **Sequitur onion** (dywidenda → przegląd → zagadnienia → matematyka → dodatki → odejmowanie → ponowne grupowanie → iteracja). Obejmuje pięć obszarów: `math/` (ekonomia handlu dwustronnego, wspólna maksymalizacja zysku, negocjacje Nasha i krytyczny wskaźnik newsvendor), `physics/` (analogia modelowania strukturalno-ekonomicznego pól zaufania), `integration/` (rekurencyjna integracja pionowa), `grading/` (ocena złożoności A–D) oraz `output/` (kierowalne wyjście do terminala, CommonRails lub gniazda/łącznika). Pakiet zawiera uruchamialne przykłady i dokumentację definicji, matematyki oraz fizyki. Natywny odbiorca C/C++ w [`advantage/`](advantage/README.md) łączy model z przykładami handlowymi.
- **Adnotacje czasu pętli — `@loopDelay` / `@loopBudget` i funkcja `sleep(ms)`.** Są to adnotacje behawioralne umieszczane w osobnym wierszu przed pętlami `for`, `while` lub `do`. Wprowadzają rzeczywistą przerwę w wykonaniu pętli. `@loopDelay(ms = 250)` wstrzymuje każdą iterację na określony czas; `@loopBudget(seconds = 10, iterations = 1000)` rozkłada znaną liczbę iteracji na całkowity budżet czasu. Obie używają dodatkowego kodu operacji `OP_SLEEP` (ISA 157), który obsługuje również wbudowane `sleep(<ms>)`. Implementacja korzysta z `nanosleep` / `Sleep`. Zob. [`SLEELA.syntax` §32](SLEELA.syntax), [referencję API](api/sleela/3.0/LOOP_TIMING_API.md) i [uruchamialny przykład](examples/loop-timing-annotation-demo.sleela).
- **SleelaDoc z wiersza poleceń — `sleela docs`.** Polecenie wyświetla dokumentowalne tematy, pakiety i klasy; wyszukiwanie klas nie uwzględnia wielkości liter i obsługuje standardowy prefiks `SL`. Strony obejmują definicję, cel projektu, relacje, strukturę, koncepcje użycia i pięciostopniową skalę opanowania. Dokumentację wzbogacają znaczniki `@intent`, `@thesis`, `@axiom`, `@param`, `@see` i `@mastery`. `sleela docs --generate [dir]` tworzy korpus tekstowy do pracy offline; `make sleeladoc` pakuje korpus i odpowiadające mu pliki źródłowe do `sleeladoc/build/sleeladoc.jar`. Zob. [`lib/sleeladoc/`](lib/sleeladoc/README.md) i [`sleela(1)`](man/man1/sleela.1).
- **SleelaDoc i dokumentacja HTML.** Ekstraktor [`lib/sleeladoc`](lib/sleeladoc/README.md) przekształca komentarze dokumentacyjne w jednostkach `.sleela` w strony Markdown lub HTML. Przeszukiwalna witryna dokumentacji całej biblioteki `/lib` jest generowana w [`sleeladoc-api/`](sleeladoc-api/README.md). Kompilator może również generować strony MediaWiki za pomocą `--wiki`. Zob. [SLEELADOC](manifesto/SLEELADOC.md), [SLEELADOC.WIKI](manifesto/SLEELADOC.WIKI.md) i [przykład](lib/sleeladoc/sleeladoc-demo.sleela).
- **Model obiektowy dokumentu XML (`lib/xml`).** Biblioteka udostępnia drzewo DOM z węzłami elementów, tekstu, komentarzy, CDATA i instrukcji przetwarzania, nazwanymi atrybutami, serializacją zwartą i czytelną, escapowaniem XML 1.0 §2.4, parserem komórek znakowych oraz rejestrem jakości `SLXmlAspect`. Równość węzłów jest refleksyjna, symetryczna i deterministyczna, z poziomami tożsamości, tematu i struktury. Zob. [`lib/xml/README.md`](lib/xml/README.md) i [przykład](lib/xml/xml-dom.sleela).
- **Rozszerzone typy całkowite.** `int`, `long` i `int64` są 64-bitowymi liczbami całkowitymi ze znakiem; `int128` jest obecnie obsługiwany przez reprezentację 64-bitową, a pełna obsługa 128-bitowa pozostaje zaplanowana. Rodziny binarne ze znakiem `int32`–`int16777216` (składnia 2.7) i bez znaku `uint32`–`uint16777216` (składnia 2.8) rozszerzają ten zestaw. Zob. [`SLEELA.syntax`](SLEELA.syntax) §§21, 24 i 25.
- **Pola następcze `@mix` (składnia 2.9).** Klasa może deklarować powtarzające się pola instancji o tej samej nazwie, jeżeli każda deklaracja ma oznaczenie `@mix`. Grupa jest indeksowana rosnąco (`1,2,3,…)) albo jawnie przez `@mix(1,2,3,...)`; liczba indeksów musi odpowiadać liczbie pól. Zob. [§28](SLEELA.syntax).
- **Preonems — znaczniki dokumentacyjne Procesu Naturalnego.** Deklaratywne znaczniki obejmują `author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`, `pure`, `gasoline` i `artifact`. Są zależne od kontekstu i wersji; nie zmieniają parsowania, kontroli typów ani generowania kodu. Poza pozycją znacznika pozostają zwykłymi identyfikatorami. Zob. [`SLEELA.syntax`](SLEELA.syntax) §§18–20, 23, 26, 30 i 35.
- **Nordshrift z pełnym dostępem do biblioteki.** W składni 3.0 Nordshrift korzysta z tego samego rekurencyjnego indeksu `/lib` co kompilator, obejmując wszystkie pakiety i jednostki `.sleela`; `nordshrift check` podaje liczbę elementów w całym drzewie.
- **Stanchion rekurencji.** [`recursion/stanchion/`](recursion/stanchion/README.md) zawiera referencyjne prototypy C i C++ dla siedmiu udokumentowanych kontraktów typów pierwotnych oraz przykłady ogólne, stosowane, naukowe i dydaktyczne. Źródła referencyjne należy skompilować i przetestować przed uznaniem ich za zweryfikowane.
- **Komentarze kątowe (`<` / `<<`).** Frontend rozpoznaje komentarze wierszowe `< line >` i blokowe `<< block >>`. Zob. [§1.1](SLEELA.syntax).
- **Literały całkowite według podstawy.** Literały `int` obsługują zapis szesnastkowy (`0x`), binarny (`0b`) i ósemkowy (`0o`), z separatorami `_`; dotyczy to języka i formatu modeli CPU [`.xlt`](lib/xlt/README.md). Zob. [§1.4](SLEELA.syntax).
- **Standard Metadata Class.** Standard metadanych opisowych — pochodzenie, legalność i zakres jurysdykcji — dotyczy wszystkich plików `.sleela` zawierających klasy w `/lib`. Zob. [§15](SLEELA.syntax) i [lekcję 13](tutorial/13-metadata-class.md).
- **Rodzina tekstu i String.** `lib/text` udostępnia operacje na znakach i łańcuchach, w tym `charAt`, `substring`, `indexOf`, `reverse`, wyrównywanie kolumn i szablony pozycyjne. Zob. [samouczek](tutorial/14-text-and-strings.md) i [przykład](lib/text/text-string-demo.sleela).
- **Łańcuchy w arkuszach Nordshrift.** Arkusz `.sst` może deklarować typowany blok `strings:`, który współtworzy manifest komponentów kompilacji. Zob. [referencję Nordshrift](wiki/Nordshrift-Sheet-Reference.md).
- **Modele CPU według kodów operacji i równoważność wywołań.** Każdy `SL<ARCH>CPU` obsługuje wykonanie natywnego strumienia instrukcji przez cykl pobranie–dekodowanie–wykonanie oraz równoważne wykonywanie kodu innego procesu na współdzielonej infrastrukturze VM. Zob. [`lib/cpu/EXECUTION.md` §5](lib/cpu/EXECUTION.md).

## III. Budowanie i testowanie

**Wymagania:** kompilator C/C++ (GCC lub Clang) i GNU Make; Python 3 do obsługi manifestów i przepływu pracy; w systemie Linux pakiety deweloperskie X11/Xrandr dla produktów wyświetlania. Polecenie `make deps` modułu informuje o wymaganych zależnościach i może je zainstalować. Kompilacja w `impl/` działa w trybie fail-closed względem zaufanego manifestu SHA-256, do którego główny `Makefile` odwołuje się automatycznie.

Najszybszy start z czystej kopii repozytorium:

```sh
make                                  # zbuduj wszystko z katalogu głównego repozytorium
```

Aby zbudować i przetestować wyłącznie rdzeń natywny:

```sh
make -C impl all                      # zbuduj tylko rdzeń kompilatora i runtime
bash ./test-suites/run-all.sh --all   # uruchom pełny zestaw testów
```

Polecenie `make` w katalogu głównym buduje wszystkie produkty (rdzeń, VM, kodeki, churn, antywirus itd.), a na końcu buduje **rejestr** i materializuje jego tabelę bazową. Dzięki temu gotowe drzewo zawiera rejestr, który można od razu przeglądać.

Podstawowe cele:

```sh
make                     # pełna kompilacja (alias make all; rejestr jest budowany na końcu)
make all                 # jawnie wskazana pełna kompilacja
sudo make install        # instalacja poleceń i plików runtime (zob. § IV)
make clean               # usuń wyniki kompilacji ze wszystkich zintegrowanych systemów
make help                # wyświetl cele głównego Makefile
```

Inne przydatne cele:

```sh
make registry            # zbuduj rejestr i materializuj tabelę
./build-registry.sh      # wykonaj tę samą operację bezpośrednio
make tests               # zbuduj i uruchom zestaw testów
make ballum              # zbuduj bibliotekę wyświetlania Ballum dla bieżącego systemu
bin/install-modules      # etapowa instalacja/budowanie modułów (około 10–20 minut)
```

Cel `registry` uruchamia [`build-registry.sh`](build-registry.sh): buduje rdzeń rejestru, zapisuje tabelę bazową w przenośnym magazynie tekstowym `registry/build/sleela.sleelareg` i eksportuje skrypt tabeli MySQL do `registry/build/registry.sql` przez `sleela registry export`. Rejestr można przeglądać poleceniem `sleela registry` lub `registry/build/sleela-registry review`. Zob. [przewodnik rejestru](registry/README.md) i [architekturę Registry 3.0](registry/REGISTRY-3.0-ARCHITECTURE.md).

Instrukcje dotyczące wymagań i kroków specyficznych dla platformy znajdują się w przewodnikach budowania i testowania. Pomyślne zbudowanie jednego komponentu nie dowodzi, że każda biblioteka i platforma jest gotowa do wydania.

## IV. Instalacja

Po zbudowaniu produktów natywnych zainstaluj polecenia i pliki pomocnicze środowiska uruchomieniowego:

```sh
sudo make install
make install PREFIX="$HOME/.local"        # instalacja dla bieżącego użytkownika, bez sudo
make install DESTDIR="$PWD/package-root" # instalacja etapowa do katalogu pakietu
make install-check
```

Domyślnie standardowy instalator używa katalogów `/usr/local/bin`, `/usr/local/lib/sleela` i `/usr/local/share/sleela`. Zachowuje istniejącą konfigurację startową i odmawia nadpisania istniejących poleceń nieśledzonych przez instalator. Odinstalowanie usuwa wyłącznie pliki zarejestrowane przez instalator; zachowuje konfigurację i nieśledzone dane runtime.

Do ukierunkowanego wdrożenia wyłącznie głównego pakietu — plików binarnych `sleela`, `sleelvac` i `nordshrift`, biblioteki standardowej, protokołów i modułów pomocniczych — użyj instalatora czystego:

```sh
make clean-install       # prowadzone instalowanie wyłącznie głównego pakietu
```

Najpierw podejmowana jest główna decyzja o instalacji pakietu, następnie wybierany jest zakres, a po pomyślnej instalacji może pojawić się opcjonalny wybór wyselekcjonowanych modułów rozszerzeń. Pełny interaktywny proces i wszystkie opcje opisano w [przewodniku Quick and Safe Install](install/README.md).

## V. Język i środowisko uruchomieniowe

Normatywna gramatyka znajduje się w [`SLEELA.syntax`](SLEELA.syntax). Repozytorium dokumentuje obecnie składnię **3.0**; wersja kompilatora/narzędzi i wersja składni kodu źródłowego są odrębne. Składnia 3.0 dodaje znaczniki dokumentacyjne **`pure` i `gasoline`** (§30) — dwa kontekstowe, deklaratywne Preonems, które oznaczają status dokumentu zgodnie z jego własnym charakterem. Składnia 3.0 łączy zakres języka od 1.0 do 2.9 w bieżącą edycję normatywną, zachowując minimalne wymagania wersji dla poszczególnych funkcji.

Poprzednia wersja 2.9 wprowadza **następcze pola `@mix`** (§28): klasa może deklarować wiele pól instancji o tej samej nazwie, jeśli każde ma oznaczenie `@mix`. Pola tworzą jedną indeksowaną grupę — domyślnie od góry `1,2,3,…` albo ręcznie przez `@mix(1,2,3,...)`; liczba indeksów musi odpowiadać liczbie pól. Włączono również szerokie rodziny liczb całkowitych ze znakiem i bez znaku: `int32`–`int16777216` (§24) i `uint32`–`uint16777216` (§25), rozszerzające wcześniejsze `long`, `int64` i `int128` (§21); dokumenty wspierające słowa kluczowe (§22); oraz **Preonems**: deklaratywne znaczniki dokumentacyjne `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) i `artifact` (§35; status jakości o randze legislacyjnej oceniany w czasie i przestrzeni). Zbiorcza nazwa Preonems została zdefiniowana w §26.1. Rozstrzyganie przeciążenia konstruktorów oraz delegowanie do `this(...)` i konstruktorów klas bazowych pozostają wyraźnie poza zweryfikowanym kontraktem składni 1.8. Przed poleganiem na funkcji należy sprawdzić gramatykę i notatki implementacyjne. Dziewięć jednostek źródłowych SleelaDoc deklaruje obecnie `#sleela 3.0`; wspólna kontrola kompilatora akceptuje składnię źródłową 1.0–3.0. Nową dokumentację API generuje `make api-docs`.

Repozytorium zawiera natywną implementację C/C++, odniesienia do generowania SLVM, moduły biblioteki standardowej, testy, przykłady i integracje platformowe. Stan implementacji i walidacji różni się między modułami; należy sprawdzać właściwą specyfikację i aktualne wyniki CI, a nie zakładać jednakowy poziom kompletności.

## VI. Zasady inżynierskie

- **Dowody zamiast deklaracji:** rozróżniaj zachowanie zaimplementowane, przetestowane, eksperymentalne i planowane.
- **Jeden kontrakt na granicę:** dokumentuj składnię, API, formaty plików, założenia platformowe i zachowanie w razie błędu.
- **Przenośność z założenia:** izoluj backendy zależne od platformy i testuj je na natywnych runnerach.
- **Bezpieczne niepowodzenie:** testuj błędne dane wejściowe, limity zasobów, awarie procesów i ścieżki sprzątania.
- **Powtarzalne wydania:** wykonuj czyste kompilacje i publikuj skróty artefaktów oraz informacje o wydaniu.

## VII. Budowanie i integralność antywirusa

Pakiet antywirusowy Sleelavirin zawiera natywny skaner SHA-256, opcjonalny adapter ClamAV, manifesty integralności źródeł i indeksów oraz sumy kontrolne pakietów platformowych. Zbuduj i przetestuj pakiet z katalogu głównego repozytorium:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Manifest integralności jest generowany w `antivirus/build/`. Obejmuje śledzone pliki źródłowe i dokumentacyjne oraz chronione pliki indeksów sygnatur, wykluczając wygenerowane wyniki i zmienną pamięć podręczną ClamAV. Migawki baz danych ClamAV mają własne manifesty. Archiwa ZIP wydań zawierają `INTEGRITY-MANIFEST.json`, `SHA256SUMS` i plik towarzyszący `.sha256`. Skróty pobranych archiwów należy porównywać z zaufaną wartością uzyskaną niezależnie: suma kontrolna zapisana obok archiwum nie jest podpisem wydawcy. Zob. [przewodnik Sleelavirin](antivirus/README.md), [listę kontrolną bezpieczeństwa](antivirus/SECURITY.md) i [workflow budowania antywirusa](.github/workflows/antivirus-binaries.yml).

Wydanie nie jest uznawane za zweryfikowane, dopóki odpowiednie zadania CI nie zakończą się powodzeniem dla dokładnego commitu wydania, a wynikowe artefakty nie zostaną sprawdzone.

## VIII. Motywy edytorów SLeeLa — Vim i Emacs

SLeeLa zawiera spójny zestaw profesjonalnych ciemnych schematów kolorów dla **Vim** i **Emacs**, a także natywne narzędzie palet C/C++. Oryginalny schemat **American Professional** pozostaje domyślny. Dostępnych jest dziesięć dodatkowych palet inspirowanych różnymi tradycjami kulturowymi i instytucjonalnymi; są to interpretacje projektowe, a nie oficjalne reprezentacje państw, kultur, sądów ani instytucji.

### Jedenaście schematów i ich kolory

| Schemat | Tło | Tekst | Słowa kluczowe | Typy | Łańcuchy | Komentarze |
|---|---|---|---|---|---|---|
| American Professional | `#101820` | `#F4F7FA` | `#7FB3FF` | `#73DACA` | `#F2C078` | `#8A96A3` |
| Pan-Eurasian | `#111827` | `#F0F2F5` | `#7AA7E8` | `#69C9B3` | `#D7B56D` | `#8995A5` |
| Russian and Slavic — Medium | `#202733` | `#E8EAF0` | `#A9C0DD` | `#8FC4B2` | `#D6BD8B` | `#9AA3AF` |
| Central Asian / Silk Road | `#1C2529` | `#F5F0E5` | `#7FADE0` | `#45C6C2` | `#D9B56D` | `#92A3A0` |
| East Asian | `#171A1C` | `#F2EFE7` | `#D97878` | `#72C7A5` | `#D8B76A` | `#969F9B` |
| ICC — International Criminal Court | `#0E1A2B` | `#EAF0F7` | `#5B8FD6` | `#7FB7C9` | `#D9B25A` | `#7E8CA0` |
| SCOTUS — U.S. Supreme Court | `#1A1416` | `#F3EEE6` | `#9E2B35` | `#8FA9B8` | `#C9A24B` | `#8C8178` |
| Greek (Hellenic) | `#0A1A30` | `#FFFFFF` | `#2E7BD6` | `#5E9FE0` | `#9FC3EC` | `#6E86A6` |
| Turkish (Al Bayrak) | `#2A0609` | `#FFFFFF` | `#F23D48` | `#F0A9AE` | `#F0C9A9` | `#A8767A` |
| Kiro | `#0B0B12` | `#F5F5FA` | `#A98EDA` | `#6E8BE0` | `#E8C547` | `#6A6482` |
| SLeeLa | `#08080E` | `#FFFFFF` | `#B07CFF` | `#5A7CFF` | `#FFD21E` | `#6E5F9E` |

### Kolory flag narodowych — paleta źródłowa

Schematy grecki i turecki wywodzą się z oficjalnych barw flag narodowych, podanych tu z dokładnymi wartościami szesnastkowymi. Tła i akcenty schematów powyżej są przyciemnionymi, dostosowanymi do edytorów interpretacjami tych barw.

| Państwo | Kolor | Kod | Podstawa i znaczenie |
|---|---|---|---|
| Grecja | Niebieski | `#0D5EAF` | Niebieski flagi narodowej (ustawa 851/1978); niebo i morze. |
| Grecja | Biały | `#FFFFFF` | Biel flagi narodowej; symbol czystości walki o niepodległość; krzyż odnosi się do prawosławia wschodniego. |
| Türkiye | Czerwony (*al bayrak*) | `#E30A17` | Czerwień flagi narodowej (turecka ustawa o fladze); tradycyjnie krew męczenników i suwerenność osmańska. |
| Türkiye | Biały | `#FFFFFF` | Biel flagi narodowej; półksiężyc i gwiazda — pokój, czystość oraz dziedzictwo turkijskie i islamskie. |

Schematy opierają tekst edytora na narodowej bieli, a słowa kluczowe, kursor, zaznaczenie i pasek stanu na dokładnym kolorze narodowym — niebieskim (`#0D5EAF`) lub czerwonym (`#E30A17`). Są to interpretacje projektowe; tabela barw służy wskazaniu pochodzenia, a nie oficjalnej reprezentacji któregokolwiek państwa. Opisy znaczeń odwołują się do materiałów o symbolach państwowych greckiego MSZ i tradycji tureckiej ustawy o fladze.

### Tabela próbek kolorów

Poniższe pola palet używają próbek SVG, dzięki czemu GitHub wyświetla rzeczywiste kolory. Każdy pasek pokazuje sześć kolorów w kolejności: tło, tekst, słowa kluczowe, typy, łańcuchy i komentarze. Dokładne kody HEX podano obok.

| Schemat | Paleta | Kody HEX (tło · tekst · słowa kluczowe · typy · łańcuchy · komentarze) |
|---|---|---|
| American Professional | ![Paleta American Professional](lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| Pan-Eurasian | ![Paleta Pan-Eurasian](lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| Russian and Slavic — Medium | ![Paleta Russian and Slavic Medium](lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| Central Asian / Silk Road | ![Paleta Central Asian Silk Road](lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| East Asian | ![Paleta East Asian](lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| ICC | ![Paleta ICC](lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| SCOTUS | ![Paleta SCOTUS](lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| Greek (Hellenic) | ![Paleta Greek](lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| Turkish (Al Bayrak) | ![Paleta Turkish](lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| Kiro | ![Paleta Kiro](lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| SLeeLa | ![Paleta SLeeLa](lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

Dodatkowe kolory akcentów służą do oznaczania funkcji, stałych, zaznaczeń, diagnostyki i dyrektyw preprocesora. Paleta Russian and Slavic celowo używa umiarkowanej intensywności łupkowych szarości, błękitów, szałwii, rubinu i antycznego złota.

### Instalacja schematów Vim

W katalogu głównym repozytorium:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Dodaj do `~/.vimrc`, aby włączyć podświetlanie składni SLeeLa i wybrać schemat:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Wybierz jedną nazwę schematu: `sleela-american`, `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` lub `sleela-sleela`. Na przykład `colorscheme sleela-russian-slavic` wybiera umiarkowaną paletę rosyjsko-słowiańską. Uruchom ponownie Vim lub wczytaj konfigurację poleceniem `:source $MYVIMRC`.

### Instalacja schematów Emacs

Utwórz lokalny katalog motywów i skopiuj pliki trybu oraz motywów z katalogu głównego repozytorium:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Dodaj do `~/.emacs` lub `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Zastąp `sleela-american` nazwą `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` lub `sleela-sleela`, aby wybrać inny schemat. Jeśli Emacs zapyta o zaufanie do motywu, przed zaakceptowaniem sprawdź lokalny plik motywu.

### Podgląd natywnych palet C/C++

Niewielkie natywne narzędzie palet może wyświetlać listę schematów, drukować kolory terminala i wyszukiwać pojedynczy kolor:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Dla zgodności wstecznej `--palette` i `--hex COLOR` bez `--theme` wybierają American Professional. Wyniki budowania i testów należy sprawdzać w systemie docelowym i CI; sama obecność polecenia w dokumentacji nie dowodzi, że zostało pomyślnie wykonane. Pełne opisy palet i narzędzia znajdują się w [przewodniku integracji edytorów](linux-editors/README.md).

## IX. Współtworzenie projektu

1. Przeczytaj indeks dokumentacji i politykę testów.
2. Wprowadzaj ukierunkowane zmiany wraz z testami pozytywnymi i negatywnymi.
3. Uruchom testy ukierunkowane, a następnie pełniejszy zestaw.
4. Podaj system operacyjny i szczegóły toolchainu; nie przedstawiaj pominiętych kontroli jako zaliczonych.

SLeeLa jest rozwijana przez **MEARVK LLC**. Materiały historycznej strony startowej zachowano w [`docs/README-LEGACY.md`](docs/README-LEGACY.md).

## X. AE6E66 — oczekiwana opłata podatkowa Round Public Email

W ramach inicjatywy AE6E66 deklarowane oczekiwanie projektu wynosi **300 000 000,00 USD za każdą wiadomość Round Public email**, jako proponowana kwota opłaty podatkowej związanej z każdą kwalifikującą się wiadomością.

To stwierdzenie odnotowuje wyłącznie oczekiwanie projektu. **Nie jest** prawnym wymiarem podatku, nakazem rządowym, wyrokiem sądu ani niezależnie ustalonym zobowiązaniem podatkowym. Każda rzeczywista opłata wymagałaby właściwej podstawy prawnej, jurysdykcji, definicji, procedury i rozstrzygnięcia właściwego organu.

## XI. Środowisko alfabetu i zestawu znaków

Dokumentacja alfabetu SLeeLa i natywne środowisko znaków są utrzymywane w `alphabet/sleela/` oraz `lib/alphabet/native/`. Natywne testy C++17 obejmują ścisłe dekodowanie i kodowanie skalarów UTF-8, sygnatury pojemności C<n> oraz ograniczoną przynależność do zestawu znaków:

```sh
make -C lib/alphabet/native test
```

Eksperymentalny katalog UTF-4088 i adapter wyszukiwania SLeeLa opisano w [`utf-4088/SLeeLa-ALPHABET-INTEGRATION.md`](utf-4088/SLeeLa-ALPHABET-INTEGRATION.md). Identyfikatory UTF-4088 należą do odrębnej przestrzeni nazw i nie są wartościami skalarnymi Unicode. Prace nad powiązaniem kompilatora i SLVM są śledzone osobno od tych natywnych podstaw.

## XII. AutoCAD Creator™

[AutoCAD Creator™](autocad/creator/README.md) to interaktywna aplikacja CAD 2D napisana w całości w SLeeLa. Podczas gdy powiązany AutoCAD Renderer przekształca plan w rysunek jednorazowo, Creator pozwala **tworzyć i modyfikować** dokument AutoCAD na żywo — dodawać ściany, pomieszczenia, wymiary i notatki, a następnie przesuwać elementy, zmieniać ich etykiety i warstwy, usuwać je oraz cofać operacje — i **zapisywać** wynik jako rzeczywisty rysunek AutoCAD DXF (tekstowy R12, otwierany w AutoCAD / LibreCAD).

Interfejs graficzny opiera się na **SleelaUI™**, własnym wieloplatformowym zestawie narzędzi UI SLeeLa (w [`lib/user-interface`](lib/user-interface)), a nie na JavaFX; tworzy natywne okno w X11 / Cocoa / Win32. Obowiązuje zasada **„SleelaUI prezentuje; SLeeLa decyduje”**: GUI nie oblicza geometrii ani nie zapisuje DXF. Każde narzędzie przekazuje zamiar utworzenia, zmiany, usunięcia, cofnięcia lub zapisu do nadrzędnego [`SLCadCreator`](lib/autocad/SLCadCreator.sleela), który zarządza edytowalnym dokumentem i odwracalną historią poleceń; obszar roboczy jedynie podgląda geometrię przechowywaną przez Creator.

Wielokrotnego użytku silnik edycji znajduje się w [`lib/autocad`](lib/autocad): [`SLCadEntity`](lib/autocad/SLCadEntity.sleela), [`SLCadDocument`](lib/autocad/SLCadDocument.sleela), [`SLCadCommand`](lib/autocad/SLCadCommand.sleela) i [`SLCadCreator`](lib/autocad/SLCadCreator.sleela). Działa od początku do końca również bez interfejsu graficznego. Powłoka GUI i demonstracja znajdują się w [`autocad/creator`](autocad/creator):

```sh
make creator          # z katalogu autocad/: uruchom demo Creator -> out/altered.dxf
```

W środowisku z ekranem polecenie otwiera aktywne okno edycji; na hoście bez interfejsu graficznego tworzy rysunek w pamięci, drukuje raport i plik DXF, po czym kończy działanie.

## XIII. Porządkowanie mapy wejściowej UTF-4088

Specjalistyczny katalog UTF-4088 definiuje przestrzenny przebieg mapy wejściowej i abstrakcyjne iteracje nacisku w [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). Politykę indeksu tematycznego National Lien opisano w [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Współrzędne mapy, poziomy nacisku (1–50), trwałe identyfikatory katalogowe i rangi alfabetyczne są niezależnymi polami; nie należy ich po cichu zastępować jednymi drugimi. Nacisk jest poziomem iteracji algorytmicznej, a nie wielkością fizyczną.

## XIV. Nowszy i rozszerzony indeks pakietów

Poniższy indeks przedstawia ostatnio rozszerzone pakiety, biblioteki dziedzinowe i zbiory źródeł. Wpisy są pogrupowane alfabetycznie według nazwy pakietu lub tematu. Opisy wskazują lokalizację w repozytorium; stan implementacji i walidacji należy sprawdzać w dokumentacji i testach danego pakietu.

### A–C

- **Advantage** — `lib/advantage/` i `advantage/`: modelowanie unikatowej dywidendy względem modelu jako warstwowej struktury Sequitur onion. Pięć podkatalogów: `math/` (wspólna maksymalizacja zysku, negocjacje Nasha, dwustronny newsvendor), `physics/` (metryka/pole zaufania, geodezyjna, kontakt i SDE — oznaczona analogia modelowania), `integration/` (przedziały integracji pionowej i wynik złożoności), `grading/` (ocena obwodowa A–D dla modułów i zestawów) oraz `output/` (wyjście do terminala, CommonRails lub gniazda). Natywny odbiorca C/C++ łączy model z przykładami handlowymi. Zob. [README](lib/advantage/README.md) i [DEFINITIONS.md](lib/advantage/DEFINITIONS.md).
- **Alfabet i UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` i `utf-4088/`: katalogi alfabetów, natywna obsługa zestawów znaków, porządkowanie mapy wejściowej i dokumentacja integracji UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` i `lib/autocad/`: interaktywne procesy rysowania 2D oraz wielokrotnego użytku komponenty dokumentów i edycji CAD.
- **Ballum** — sterowanie wyświetlaczem na poziomie pikseli: ustawianie i odczyt pikseli, kopiowanie obszarów, jasność/temperatura/gamma całego ekranu, wejście wskaźnika i klawiatury oraz pełnoekranowa powierzchnia bufora ramki do szybkiej, bezpośredniej kontroli pikseli (okno override-redirect i pojedyncza prezentacja `XPutImage`/MIT-SHM w Linux/X11). Rdzeń natywny znajduje się w `ballum/`, fasada SLeeLa w `lib/ballum/`, a most VM w `impl/`. Zob. [Ballum README](ballum/README.md) i [przykład](ballum/example/).
- **CDMA** — `cdma/` i `lib/cdma/`: materiały techniczne związane z CDMA oraz prace nad biblioteką źródłową SLeeLa.
- **Character** — `lib/character/`: profile postaci i wielokrotnego użytku statystyki postaci, w tym przykłady profili biograficznych.
- **Coorenagraph** — `lib/coorenagraph/`: rozumowanie etyczne, słownictwo moralne, deliberacja i pojęcia pokrewne; zob. README pakietu i pliki źródłowe.

### D–G

- **Modele źródłowe zarządzania** — [`lib/governance`](lib/governance/README.md) definiuje deklaratywne kształty źródeł SLeeLa dla wniosków, anonimowych głosowań ręcznych i zestawień zbiorczych. Proponowana adnotacja `@vote` i proces ręczny są opisane w [`registry/DEMOCRATIC-VOTING-SYNTAX.md`](registry/DEMOCRATIC-VOTING-SYNTAX.md); obecne kompilatory traktują ją jako nieaktywną metadane komentarza.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` i dostępne katalogi wersjonowanych pakietów HTTP: specyfikacje i implementacje protokołów dla poszczególnych wersji. Dokładny zakres i status weryfikacji należy sprawdzać w dokumentacji każdej wersji.
- **Międzynarodowy Trybunał Karny** — `lib/international-criminal-court/` i `INTERNATIONAL.CRIMINAL.COURTS.md`: powiązane źródła i dokumentacja dziedzinowa.

### O–S

- **OFDMA** — `ofdma/`: materiały techniczne dotyczące ortogonalnego wielodostępu z podziałem częstotliwości oraz zasoby programistyczne C/C++.
- **Filozofia i moduł Conversation Mask** — [`philosophy/`](philosophy/README.md) zawiera natywne podstawy C/C++, a [`philosophy/module/`](philosophy/module/README.md) dzieli kolejną warstwę na trzy obszary: [`data/`](philosophy/module/data/README.md) dla typowanych, datowanych obserwacji, zakończeń zdarzeń, ustawień okresu półtrwania, pochodzenia i zgody; [`helper/`](philosophy/module/helper/README.md) dla deterministycznej walidacji, zaniku istotności, zgodnych porównań i prezentacji filtrowanej według zgody; oraz [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md) dla wspierającego, nieklinicznego interfejsu rozmowy. Uchwyty SLeeLa i złożone drzewo znajdują się w [`lib/philosophy/`](lib/philosophy/README.md). Moduł wymaga jawnych danych wejściowych i stawia zgodę na pierwszym miejscu: nie monitoruje biernie osób ani mediów, nie diagnozuje, nie wnioskuje o prywatnych zamiarach, nie ocenia wartości człowieka ani nie zaleca zmian leków. Łącza zdarzeń zachowują pochodzenie i nawigację, a nie niezależne potwierdzenie.
- **Synchro** — natywna integracja runtime/kompilatora oraz funkcje związane z przekazywaniem pakietów i pomiarami; zob. odpowiednie źródła `impl/` i implementację kompilatora.
- **SleelaUI / terminal UI** — `lib/user-interface/` i projekt `sleela-terminal`: integracja interfejsu SLeeLa i komponenty interakcji terminalowej.

### U–W

- **Organizacja Narodów Zjednoczonych** — `united-nations/`: materiały repozytorium dotyczące tej dziedziny.
- **Wi-Fi 4, Wi-Fi 5 i Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`, `wi-fi-6/` oraz powiązane katalogi źródeł `lib/wifi/`: materiały i zasoby konfiguracyjne dotyczące kolejnych generacji sieci bezprzewodowych.

### Utrzymanie indeksu pakietów

Gdy pakiet zostaje dodany lub istotnie rozszerzony, dodaj tutaj jego wpis w odpowiedniej grupie alfabetycznej, podaj odnośnik do kanonicznego katalogu i README oraz opisuj wyłącznie możliwości potwierdzone przez źródła znajdujące się w repozytorium. Indeks ma charakter addytywny: aktualizacje nie powinny usuwać ani przepisywać istniejących sekcji README.

## XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

`bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh` :: `⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓`

`bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc` :: `⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉`

Materiały dotyczą amerykańskiej demokracji i polityki USA.

## XVI. Grafika marki National SLeeLa Science

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>

## XVII. Eksperymentalny katalog UTF-4088 z milionem wpisów

[Generator katalogu UTF-4088](utf-4088/tools/generate_catalogue.py) deterministycznie tworzy eksperymentalny katalog symboli zawierający 1 048 576 wpisów. Udokumentowany format kompaktowy opisano w [`utf-4088/FORMAT.md`](utf-4088/FORMAT.md), a kontrakt generowania i walidacji w [`utf-4088/GENERATION-RESULTS.md`](utf-4088/GENERATION-RESULTS.md).

Uruchom generator za pomocą Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Wygenerowany plik binarny ma 16 777 244 bajty i został zaprojektowany tak, aby pozostać poniżej 20 000 000 bajtów. Testy jednostkowe i workflow GitHub Actions sprawdzają liczbę wpisów, unikatowość glifów, rozmiar i SHA-256; udane uruchomienia publikują plik binarny i podsumowanie jako artefakt workflow `utf4088-million-catalogue`. Wygenerowany plik binarny celowo nie jest umieszczany w historii Git. UTF-4088 pozostaje eksperymentalny i nie jest zgodny z Unicode.

## XVIII. Ostatnie dodatki — stała Pi, uruchamianie Modium/Registry, matematyka sleelactl i `@native`

Ta sekcja opisuje funkcje dodane w pracach z 2026-11, zaczynając od najnowszych grup. Wszystkie zostały zatwierdzone w gałęzi `master`.

### Katalogowa stała Pi o 100 000 000 cyfr

Repozytorium zawiera zweryfikowaną wartość π o **stu milionach cyfr** w [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (poprawny zapis dziesiętny `3.14159…`) oraz wartość o 57 344 cyfrach w [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Obie wartości zostały niezależnie zweryfikowane jako rzeczywiste rozwinięcia π. [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md) opisuje kontrole: prefiks podręcznikowy, punkt Feynmana na pozycji ułamkowej 762, znaną cyfrę na pozycji milionowej, zgodność z `lib/math/SLPiReference.sleela`, analizę częstości cyfr w całym pliku oraz jednorazowy skrypt odtwarzający.

### Modium™ / Registry™ ładują stałą Pi przy uruchamianiu podsystemu

Dedykowane punkty wejścia podsystemów — `sleela registry` i `sleela modium` (oraz pierwsze uruchomienie instalatora) — ładują dużą katalogową stałą π; nie dzieje się to przy każdym `sleela run`. W interaktywnym terminalu wyświetlany jest chwilowy czerwony wiersz `Loading Modium™` / `Loading Registry™`, który następnie jest usuwany w miejscu przed rozpoczęciem zwykłego wyjścia. Efekt jest ograniczony do TTY, więc wyjście potoków i testów pozostaje identyczne bajt po bajcie. Lokalizator (nie same bajty) jest zapisywany jako domyślne klucze rejestru `Math`: `piReferenceLargePath` i `piReferenceLargeDigits`, zarówno w modelu SLeeLa ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)), jak i w natywnym seederze ([`registry/sleela_registry.c`](registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` udostępnia `largePath()` / `largeDigits()`. Stała π jest instalowana w lokalizacji SLeeLa systemu operacyjnego (`<data>/pi/`) przez standardowy instalator `make` i instalator prowadzony („beautiful”). Szczegóły: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — instalator prowadzony i natywny obszar `math`

- Instalator prowadzony („beautiful”) instaluje teraz `sleelactl` (płaszczyznę sterowania SLeeLa) i jego rejestr modułów, zgodnie z instalatorem `make`. Zob. [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- `sleelactl` zyskał nową naturę `subject` dla natywnych bibliotek tematycznych C/C++ kompilowanych do runtime; pierwszą rejestrowaną jest `math`. `sleelactl run math` uruchamia backend `__NativeMath` (`impl/subjects/math/math_native.cpp`) od początku do końca przez demonstrację referencyjną; `sleelactl info math` wyjaśnia sposób dostępu (`import math;`). Natura `subject` udostępnia wyłącznie polecenia `run` i `info` — celowo nie ma polecenia `check`, ponieważ `sleela check` nie ładuje natywnych podmiotów zarejestrowanych w SHEET.

### `@native` i `sleela aot` — część 1 ścieżki SLeeLa → natywne C/C++

Nowa adnotacja metody `@native` żąda kompilacji natywnej AOT dla gorącej metody. Nowe polecenie `sleela aot` wyszukuje metody oznaczone `@native`, sprawdza, czy każda mieści się w obsługiwanym *podzbiorze możliwym do odciążenia*, i emituje C/C++. Działa w trybie **fail-closed**: odmawia translacji konstrukcji, których jeszcze nie potrafi obsłużyć, podając precyzyjny powód; nie tworzy kodu przetłumaczonego tylko częściowo.

```sh
sleela aot --list <file.sleela>    # werdykt dla każdej metody: ok / REFUSED (powód)
sleela aot --emit <file.sleela>    # emituje C/C++ (stdout) dla programu z obsługiwanego podzbioru
```

Obecnie `@native` nie zmienia zachowania programu: programy nadal przechodzą kontrolę i uruchamiają się bez zmian; adnotacja jedynie *żąda* obsługi natywnej. Jest to część 1 — ustanawia składnię adnotacji i uczciwą bramkę obsługiwanego podzbioru. Kompilowanie wyemitowanego C/C++ do biblioteki współdzielonej i kierowanie VM do tej biblioteki (z powrotem do interpretera w razie potrzeby) to późniejsze etapy. Szczegóły, obsługiwany podzbiór i kody wyjścia: [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).

## XIX. Międzynarodowe wydania README

SLeeLa udostępnia krajowe pliki README dla 50 rynków edycji międzynarodowej. Każdy kraj ma plik `README.<CC>.md`; jeśli istotnych jest kilka języków, są one grupowane w pliku danego kraju.

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

**Polityka tłumaczeń:** Pliki krajowe stanowią lokalne punkty wejścia. W tej edycji polskiej przetłumaczono pełną treść sekcji I–XIX; identyfikatory techniczne, polecenia, przykłady kodu, nazwy plików, adresy i nazwy produktów zachowano w oryginalnej postaci. Przed uznaniem wydania za ostateczne zalecana jest niezależna korekta językowa oraz końcowa kontrola odnośników. Zakres językowy edycji krajowych ma charakter reprezentatywny i nie oznacza, że w pełni przetłumaczono każdy język urzędowy, regionalny, rdzenny lub migowy.
