<img align="right" src="../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — Россия (RU)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="Маркетинговый логотип SLeeLa" width="560"></p>

<p align="center"><strong>Нативный системный язык, компилятор, среда выполнения и экосистема виртуальных машин.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="../../docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Международное страновое издание.** Это издание подготовлено на русском языке. Технические идентификаторы, команды, примеры кода, имена файлов, пути и ссылки остаются без изменений по сравнению с каноническим английским [README.md](../../README.md).

**The Now. The Ingredient.** Язык — это больше, чем синтаксис: это компилятор, среда выполнения, библиотеки, инструменты и платформенные соглашения, благодаря которым программное обеспечение становится пригодным к использованию.

**Чем занимается SLeeLa.** SLeeLa занимается направлениями **Math**,
**Object**, **Science** и **Reach** — четырьмя гранями своей работы: исчислимым
(Math), устоявшейся вещью, занимающей место в каталоге существующего (Object),
дисциплинированным изучением истинного (Science) и простиранием «руки» системы
к другим системам и людям (Reach). **Национальные интересы сохраняются** —
ни одно из четырёх направлений не отодвигает их в сторону; если предмет
затрагивает интерес государства, это положение сохраняется (достоверно
фиксируется, например, в `metaOrigin` / `metaNationalSchedule` метаданных
каждого класса) и никогда не растворяется.

## I. Начало работы
- [Сборка native-реализации](../../impl/README.md)
- [Виртуальные машины и Modium — практическое руководство пользователя](../../virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [Стек CPU в исходном коде SLeeLa](../../lib/cpu/CPU.md) — полноценная машина от логических вентилей → ALU → ядер → ОС → программ, перечень из 130+ профилей CPU и настраиваемые [напряжения, переменные тактовые частоты и экспертные тайминги](../../lib/cpu/TIMING.md), которые можно [запускать по спецификации и разгонять](../../lib/cpu/components/README.md)
- [Указатель документации](../../docs/README.md)
- [Источник данных CSPML и исследовательская справка Salt & Salary](../../CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md) — происхождение и рекомендации по интеграции внешних данных США о местностях, социально-экономических показателях, рабочей силе, образовании и государственных школах.
- [Сгенерированная документация API](../../docs/api/README.md) — актуальный HTML-справочник, версионное издание API и инструкции по повторной генерации
- [Каталог API](../../api/README.md)
- [Стандарты вычислительного сравнения](../../comparison/README.md) — стандарты типов, памяти, ABI и соответствия для SLeeLa VM, Java/JVM и native C/C++.
- [Recursion stanchion — прототипы на C и C++](../../recursion/stanchion/README.md) — примеры, специализированные по примитивам Boolean, Byte, Character, Integer, Long, Float и Double, а также обобщённые, прикладные, научные и учебные шаблоны рекурсии.
- [Поддержка платформ и матрица проверки](../../docs/PLATFORM-SUPPORT.md)
- [Руководство по стилю бренда](../../docs/BRAND-GUIDE.md)
- [Концепции и материалы сувенирной продукции](../../merchandise/README.md)
- [Логотип NitroWebExpress](../../images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Политика тестирования и критерии выпуска](../../markdown/TESTING.md)

## II. Новости

Недавние дополнения к языку, драйверам и стандартной библиотеке:

- **Пакет стандартной библиотеки `advantage` — «улучшение» дивиденда с
  математикой, физикой, интеграцией, оцениванием и маршрутизируемым выводом.**
  [`lib/advantage/`](../../lib/advantage/README.md) инкапсулирует *«улучшение»
  уникального дивиденда относительно модели* в виде слоистой **луковицы
  Sequitur** (дивиденд → обзор → замечания → математика → добавления →
  вычитание → перегруппировка → итерация), по которой можно перемещаться внутрь
  и наружу с полным динамическим обновлением и восстановлением; она доступна
  через простую **модель пользователя** и **модель разработчика** с полным
  контролем. Пакет содержит пять предметных областей: **`math/`** — экономика
  двусторонней торговли (совместная максимизация прибыли, торг по Нэшу,
  критическое отношение двустороннего «продавца газет»), **`physics/`** —
  структурно-экономическая «физика полей доверия» (доверие как метрический
  тензор, транзакционная геодезическая, контактная структура объявления,
  SDE затухания доверия CIR+скачки — модельная аналогия, помеченная как
  таковая), **`integration/`** — рекурсивно составляемая модель вертикальной
  интеграции для капитализма/розничной торговли (отсеки, инженерные отсеки,
  пробные отсеки, лотки ньютоновской медианы и оценка системной
  изощрённости), **`grading/`** — оценщик «контура изощрённости», который
  оценивает каждый модуль и любой набор выигрывающих/проигрывающих/оценивающих/
  абсолютных модулей по шкале A–D, и **`output/`** — маршрутизируемый шов
  вывода, направляющий вывод модуля в терминал, на поверхность в формате
  **CommonRails** или в сокет/коннектор (с оформлением кадров для нативного
  `slnet_write`). Каждый класс несёт запись Metadata-Class; пять запускаемых
  демонстраций проверяют предметные области, а сопутствующие документы
  [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md),
  [`MATH.md`](../../lib/advantage/MATH.md) и
  [`PHYSICS.md`](../../lib/advantage/PHYSICS.md) описывают их. Нативный потребитель на
  C/C++ в каталоге [`advantage/`](../../advantage/README.md) (ядро на C
  `advantage_value`/`advantage_mean`, XML-хранилище знаний, библиотека
  защищённого получения данных по HTTPS/SFTP/SSH, хранилище ключей и транспорт
  DH/ECDH) подключает модель к разобранным
  [торговым примерам](../../advantage/examples/README.md) — положительным сделкам
  по предполагаемым интернет-источникам, последовательности по релевантности
  для аудитории (Traders, Consumers, Helpers, Healers) и восьми моделям,
  ранжированным A–D (D — наибольшая доля Cement, A — наибольшая доля Brand).
- **Аннотации времени циклов — `@loopDelay` / `@loopBudget` (+ встроенная
  функция `sleep(ms)`).** Первые **уровня оператора, поведенческие** аннотации
  в SLeeLa: записываются в отдельной строке перед циклом `for` / `while` / `do`
  и вносят в выполнение цикла реальную паузу. `@loopDelay(ms = 250)` /
  `@loopDelay(seconds = 2)` засыпает на фиксированное время на каждой итерации
  (простое `@loopDelay(250)` по умолчанию означает миллисекунды);
  `@loopBudget(seconds = 10, iterations = 1000)` равномерно распределяет
  известное число итераций на заданный общий бюджет реального времени. Обе
  аннотации понижаются до нового append-only опкода **`OP_SLEEP`** (ISA 157),
  который также лежит в основе универсальной встроенной функции
  **`sleep(<ms>)`**, вызываемой где угодно; пауза реализована переносимо
  (`nanosleep` / `Sleep`). Определено в [`SLEELA.syntax` §32](../../SLEELA.syntax);
  справочник API — [`api/sleela/3.0/LOOP_TIMING_API.md`](../../api/sleela/3.0/LOOP_TIMING_API.md);
  запускаемый пример —
  [`examples/loop-timing-annotation-demo.sleela`](../../examples/loop-timing-annotation-demo.sleela).
  Указатель дополнений сессии (включая это, нативное умножение больших целых
  `OP_BIGMUL` и автономные инструменты **[local-generator](../../local-generator/README.md)**
  и **[pi-generator](../../pi-generator/README.md)**) находится в
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](../../api/sleela/3.0/SESSION-ADDITIONS-API.md).
  Три сгенерированные поверхности API
  ([`sleeladoc-api/`](../../sleeladoc-api/README.md),
  [`docs/api/`](../../docs/api/README.md) и неизменяемый снимок
  [`api/sleela/3.0/version-014/`](../../api/sleela/3.0/version-014/index.html))
  были пересобраны командой `make api-docs` и теперь содержат новые записи
  инструкций `SLISASleep` / `SLISABigMul` на странице пакета `vm`.
- **SleelaDoc из командной строки — `sleela docs`.** Теперь SleelaDoc доступен
  прямо из терминала. `sleela docs` перечисляет все документируемые темы;
  `sleela docs crypto` показывает пакет; `sleela docs stringbuilder` находит
  класс (без учёта регистра и по префиксу стандартной библиотеки `SL` →
  `SLStringBuilder`) и выводит богатую страницу — **определение, замысел
  проектирования, отношения, структуру, пользовательские «глины» и
  пятиступенчатую лестницу мастерства** — с ранжированными подсказками «возможно,
  вы имели в виду» при близком промахе. Авторы обогащают любую страницу простым
  добавлением `@`-тегов SleelaDoc (`@intent`, `@thesis`, `@axiom`, `@param`,
  `@see`, `@mastery`). `sleela docs --generate [dir]` записывает текстовый
  корпус (одна страница на класс + `TOPICS.index`), который CLI затем читает
  автономно; **`make sleeladoc`** упаковывает этот корпус **и соответствующие
  исходники `.sleela`** в `sleeladoc/build/sleeladoc.jar`. Для тем, имеющих
  только фасад (например, `psychiatry`, `politico`), `sleela docs --facade <topic>`
  во время выполнения ищет в файловой системе и перечисляет реальные **папки
  (затем файлы, как естественное движение)** в рамках переопределяемых
  пользователем ограничений **6 МБ / 6000 файлов / 6 минут**. Модули
  `lib/sleeladoc` поддерживаются в текущей редакции **`#sleela 3.0`**. См.
  [`man/man1/sleela.1`](../../man/man1/sleela.1) и
  [`lib/sleeladoc/README.md`](../../lib/sleeladoc/README.md).
- **SleelaDoc — решение для документации в стиле Javadoc со сгенерированным
  HTML-сайтом.** **SleelaDoc** — это принятое в SLeeLa соглашение о
  документирующих комментариях в исходном коде (комментарии в угловых скобках
  с сентинелами `@doc` / `@page`; см.
  [`manifesto/SLEELADOC.md`](../../manifesto/SLEELADOC.md)), а
  [`lib/sleeladoc`](../../lib/sleeladoc/README.md) — **экстрактор**, который
  оформляет модуль `.sleela` в виде страницы API в Markdown или HTML — так же,
  как `javadoc` оформляет Java, документируя *замысел*. Полный, доступный для
  просмотра **HTML-сайт документации для всего `/lib`** генерируется в
  [`/sleeladoc-api`](../../sleeladoc-api/README.md) командой
  `tools/generate-sleeladoc-api.py` (откройте `sleeladoc-api/index.html`;
  строгий сине-голубо-светло-серо-белый стиль с иерархией пакет → класс →
  член и глоссарием, написанным для читателя *Student 2*). Компилятор также
  выдаёт размеченные автором документационные результаты как побочный продукт
  компиляции (страницу MediaWiki `--wiki` + порождённый двойник исходника);
  это описано в [`manifesto/SLEELADOC.WIKI.md`](../../manifesto/SLEELADOC.WIKI.md).
  Запускаемый пример:
  [`lib/sleeladoc/sleeladoc-demo.sleela`](../../lib/sleeladoc/sleeladoc-demo.sleela).
- **Объектная модель XML-документа (`lib/xml`).** XML DOM на стороне SLeeLa —
  документ, владеющий деревом узлов элементов/текста/комментариев/CDATA/PI с
  упорядоченными потомками и именованными атрибутами, сериализатор
  (компактный/форматированный, экранирование по XML 1.0 §2.4), разборщик на
  символьных ячейках и регистр качества `SLXmlAspect`. Равенство узлов — это
  трёхступенчатая связка доверия (тождество → тот же субъект → структура),
  рефлексивная, симметричная и детерминированная. См.
  [`lib/xml/README.md`](../../lib/xml/README.md) и запускаемый пример
  [`lib/xml/xml-dom.sleela`](../../lib/xml/xml-dom.sleela).
- **Широкие знаковые целочисленные примитивы (`long`, `int64`, `int128`).**
  `int`, `long` и `int64` — 64-битные знаковые целые, которые одинаково
  проверяются по типам и выполняются; `int128` принимается как целочисленный
  тип (пока на 64-битной основе, а настоящие 128 бит — задокументированное
  последующее развитие). Дополняют это знаковая серия по основанию 2
  `int32`..`int16777216` (синтаксис 2.7) и соответствующая беззнаковая серия
  `uint32`..`uint16777216` (синтаксис 2.8). См.
  [`SLEELA.syntax`](../../SLEELA.syntax) §21, §24, §25.
- **Консеквентные поля `@mix` (синтаксис 2.9).** Класс может объявлять несколько
  полей экземпляра с **одним и тем же именем**, если каждое повторение несёт
  `@mix`; язык воспринимает группу как консеквентную (массив/список с
  индексами): сверху вниз `1,2,3,…` по умолчанию либо вручную через
  `@mix(1,2,3,...)`, число индексов в котором должно совпадать с числом полей.
  См. [`SLEELA.syntax`](../../SLEELA.syntax) §28.
- **Маркеры документа — Естественный процесс, названный Preonems
  (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).**
  Эти «голые» слова-маркеры документа вместе называются **Preonems** (один
  маркер — *Preonem*; набор — *Preonems*; см. [`SLEELA.syntax`](../../SLEELA.syntax)
  §26.1). Это полноправные, только декларативные маркеры документа, называющие
  то, чем документ *является* в порядке создания: `author`/`authorial`
  (синтаксис 2.4; обрабатывается профессиональным советом — единственный член с
  поведенческим эффектом), `material;` (устоявшийся Source) и `memory;`
  (синтаксис 2.5), `determine;` (синтаксис 2.6; документ, открытый для
  дальнейшей обработки врачом, физиком или призраком вплоть до завершения
  законной рукой), а также два новейших — **`object;`** (устоявшаяся вещь,
  занимающая место в каталоге существующего — **овеществлённая, получившая
  конкретный статус, в синтаксисе 2.8**) и **`field;`** (основа отношения, в
  которой объекты стоят и движутся) — оба с ограничением, начиная с синтаксиса
  2.6. Каждый из них «голый», контекстный и ограничен версией на уровне
  frontend без влияния на разбор, типизацию или генерацию кода; вне позиции
  маркера каждый остаётся обычным идентификатором (так что `field` никогда не
  мешает члену класса или обращению `.field`). См.
  [`SLEELA.syntax`](../../SLEELA.syntax) §18–§20, §23 и §26.
- **Nordshrift на синтаксисе 3.0 с полным охватом библиотеки.** Nordshrift
  разрешает имена через тот же рекурсивный индекс `/lib`, что и компилятор,
  достигая каждого пакета и модуля `.sleela`; `nordshrift check` сообщает
  количество по всему дереву.
- **Recursion stanchion.** Каталог [`recursion/stanchion/`](../../recursion/stanchion/README.md) теперь включает отдельные эталонные прототипы на C и C++ для семи документированных контрактов примитивов, а также обобщённые, прикладные, научные и учебные примеры. Это эталонные исходники; скомпилируйте и протестируйте их, прежде чем считать проверенными.

- **Комментарии в угловых скобках (`<` / `<<`).** Новый перспективный стиль
  комментариев — `< line >` и `<< block >>` — разбирается лексером frontend
  SLeeLa и документирован в [`SLEELA.syntax`](../../SLEELA.syntax) §1.1.
- **Целочисленные литералы с основанием.** Литералы `int` теперь принимают
  шестнадцатеричную (`0x`), двоичную (`0b`) и восьмеричную (`0o`) формы с
  разделителями цифр `_` как в исходном языке SLeeLa
  ([`SLEELA.syntax`](../../SLEELA.syntax) §1.4), так и в
  [формате моделей CPU `.xlt`](../../lib/xlt/README.md).
- **Стандарт Metadata Class во всей библиотеке.** Метапаттерн описательных
  метаданных (происхождение, правомерность, юрисдикционная область) теперь
  присутствует в каждом файле `.sleela` с классами в `/lib`; см.
  [`SLEELA.syntax`](../../SLEELA.syntax) §15 и
  [урок учебника](../../tutorial/13-metadata-class.md).
- **Более богатое семейство text / String.** `lib/text` получает настоящие
  вспомогательные средства на самом языке, представление в виде символьных
  ячеек с подлинными `charAt`/`substring`/`indexOf`/`reverse`/выравниванием по
  столбцам и настоящий позиционный шаблонизатор — см.
  [учебник по text и String](../../tutorial/14-text-and-strings.md) и запускаемый
  пример [`lib/text/text-string-demo.sleela`](../../lib/text/text-string-demo.sleela).
- **`String` первого класса в листах Nordshrift.** Лист `.sst` может объявлять
  типизированный блок `strings:`, вносящий вклад в манифест компонентов сборки;
  см. [справочник по листам Nordshrift](../../wiki/Nordshrift-Sheet-Reference.md).
- **Модели CPU работают как по опкодам, так и по эквивалентности вызовов функций.**
  Каждый `SL<ARCH>CPU` наследует основу выполнения нативного потока инструкций
  через свой цикл выборки-декодирования-исполнения *и* выполнения кода
  предполагаемого другого процесса эквивалентными вызовами на общем субстрате
  VM — см. [`lib/cpu/EXECUTION.md`](../../lib/cpu/EXECUTION.md) §5.

## III. Сборка и тестирование

**Требования:** набор инструментов C/C++ (GCC или Clang) и GNU Make; Python 3
для инструментов манифестов и потоков; в Linux — пакеты разработки X11/Xrandr
для графических продуктов (команда `make deps` модуля сообщает и устанавливает
то, что ему нужно). Сборка `impl/` работает в режиме fail-closed по доверенному
манифесту SHA-256, на который корневой `Makefile` указывает автоматически.

Из чистой копии быстрее всего начать так:

```sh
make                                  # build everything from the repo root
```

Либо собрать и протестировать только native-ядро:

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

Корневая команда `make` собирает каждый продукт (ядро, VM, кодеки, churn,
антивирус, …) и завершается сборкой **реестра** и материализацией его базовой
таблицы — так что собранное дерево сразу поставляется с реестром, пригодным
для проверки. Основные цели:

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

Другие полезные цели:

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

Цель `registry` запускает [`build-registry.sh`](../../build-registry.sh): он собирает
ядро реестра, загружает базовую таблицу в переносимое текстовое хранилище
`registry/build/sleela.sleelareg` и экспортирует скрипт таблицы MySQL
`registry/build/registry.sql` (через `sleela registry export`). Просмотрите её
командой `sleela registry` или `registry/build/sleela-registry review`. См.
[руководство по реестру](../../registry/README.md) и [архитектуру Registry 3.0](../../registry/REGISTRY-3.0-ARCHITECTURE.md).

Требования и платформенные шаги уточняйте в руководствах по сборке и тестированию. Успешная сборка одного компонента не доказывает, что все библиотеки или платформы готовы к выпуску.

## IV. Установка

После сборки native-продуктов установите команды и вспомогательные файлы среды выполнения:

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

Обычный установщик по умолчанию использует `/usr/local/bin`, `/usr/local/lib/sleela`
и `/usr/local/share/sleela`. Он сохраняет существующую стартовую конфигурацию и
отказывается перезаписывать существующие команды, не отслеживаемые установщиком.
Удаление затрагивает только файлы, записанные установщиком, сохраняя
конфигурацию и неотслеживаемые данные среды выполнения.

Для целевого пошагового развёртывания только основного пакета — бинарных файлов
`sleela`, `sleelvac` и `nordshrift`, стандартной библиотеки, протоколов и
вспомогательных модулей — используйте чистый установщик:

```sh
make clean-install       # guided install of the main package only
```

Он требует одного основного решения (установить основной пакет), затем выбора
области установки, а при успехе — необязательного выбора из подобранных
модулей расширения. Об интерактивном процессе развёртывания и всех параметрах
установки см. [Быструю и безопасную установку](../../install/README.md).

## V. Язык и среда выполнения
Авторитетная грамматика — [`SLEELA.syntax`](../../SLEELA.syntax). В репозитории сейчас документирован синтаксис **3.0**; версия компилятора/инструмента и версия синтаксиса исходного кода — разные понятия. Поверхность 3.0 добавляет **маркеры документа `pure` и `gasoline`** (§30) — пару «голых», контекстных, только декларативных Preonems, значащих почти одно и то же, каждый из которых отмечает статус документа по-своему честно. Синтаксис 3.0 объединяет языковую поверхность 1.0–2.9 в текущую нормативную редакцию, сохраняя все минимальные пороги, привязанные к отдельным возможностям. Предыдущая поверхность 2.9 добавляет **консеквентные поля `@mix`** (§28) — класс может объявлять несколько одноимённых полей экземпляра, если каждое несёт `@mix`, понимаемых как одна индексированная группа (сверху вниз `1,2,3,…` или вручную `@mix(1,2,3,...)`, число индексов которого должно совпадать с группой). Она переносит дальше семейства широких знаковых/беззнаковых целых по основанию 2 `int32`..`int16777216` (§24) и `uint32`..`uint16777216` (§25) поверх более ранних `long`/`int64`/`int128` (§21); **документы-поддержки ключевых слов** (§22); и **Preonems** — только декларативные маркеры документа `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) и `artifact` (§35, статус законодательного уровня, градуированный по времени и пространству), совместно называемые Preonems (§26.1). Разрешение перегрузок конструкторов и делегирование `this(...)`/конструктору базового класса явно находятся за пределами проверенного контракта синтаксиса 1.8. Прежде чем полагаться на возможность, сверьтесь с грамматикой и примечаниями по реализации. Девять исходных модулей SleelaDoc теперь объявляют `#sleela 3.0`; общий шлюз компилятора принимает исходный синтаксис 1.0–3.0. Новая сборка API создаётся командой `make api-docs`.

Репозиторий включает native-реализацию на C/C++, справочные материалы по генерации SLVM, модули стандартной библиотеки, тесты, примеры и платформенные интеграции. Статус реализации и проверки различается по модулям; сверяйтесь со спецификацией модуля и текущими результатами CI, не предполагая единообразной полноты.

## VI. Инженерные принципы
- **Доказательства важнее заявлений:** различайте реализованное, протестированное, экспериментальное и запланированное поведение.
- **Один контракт на границу:** документируйте синтаксис, API, форматы файлов, допущения о платформе и поведение при сбоях.
- **Переносимость по замыслу:** изолируйте платформенно-зависимые бэкенды и тестируйте на native-раннерах.
- **Безопасный отказ:** тестируйте некорректный ввод, ограничения ресурсов, сбои процессов и пути очистки.
- **Воспроизводимые выпуски:** пересобирайте с нуля и публикуйте хеши артефактов и примечания к выпуску.

## VII. Сборка антивируса и целостность

Пакет антивируса Sleelavirin включает native-сканер SHA-256, необязательный
адаптер ClamAV, манифесты целостности исходников/индексов и контрольные суммы
платформенных пакетов. Соберите и протестируйте пакет из корня репозитория:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Манифест целостности создаётся в `antivirus/build/`; он охватывает
отслеживаемые исходные и документационные входные данные, а также защищённые
файлы индекса сигнатур, исключая сгенерированный вывод и изменяемый кэш ClamAV.
У снимков баз ClamAV есть собственные манифесты. ZIP-архивы выпуска включают
`INTEGRITY-MANIFEST.json`, `SHA256SUMS` и сопутствующий файл `.sha256`.
Проверяйте хеши скачанных архивов по доверенному значению, полученному
независимо: контрольная сумма, хранящаяся рядом с архивом, не является подписью
издателя. См. [руководство по Sleelavirin](../../antivirus/README.md),
[контрольный список безопасности](../../antivirus/SECURITY.md) и
[рабочий процесс сборки антивируса](../../.github/workflows/antivirus-binaries.yml).

Выпуск не считается проверенным, пока соответствующие задания CI не пройдены на
точном коммите выпуска, а полученные артефакты не проверены.


## VIII. Цветовые темы редакторов SLeeLa — Vim и Emacs

SLeeLa включает согласованный набор профессиональных тёмных цветовых схем для **Vim** и **Emacs**, а также native-утилиту палитр на C/C++. Исходная схема **American Professional** остаётся используемой по умолчанию. Десять сопутствующих палитр доступны для разных культурных и институциональных дизайнерских вдохновений; это интерпретационные дизайнерские палитры, а не официальные представления какой-либо нации, культуры, суда или учреждения.

### Одиннадцать схем и их цвета

| Схема | Фон | Передний план | Ключевые слова | Типы | Строки | Комментарии |
|---|---|---|---|---|---|---|
| **Американская профессиональная** | <span style="display:inline-block;width:1em;height:1em;background:#101820;border:1px solid #888"></span> `#101820` | <span style="display:inline-block;width:1em;height:1em;background:#F4F7FA;border:1px solid #888"></span> `#F4F7FA` | <span style="display:inline-block;width:1em;height:1em;background:#7FB3FF;border:1px solid #888"></span> `#7FB3FF` | <span style="display:inline-block;width:1em;height:1em;background:#73DACA;border:1px solid #888"></span> `#73DACA` | <span style="display:inline-block;width:1em;height:1em;background:#F2C078;border:1px solid #888"></span> `#F2C078` | <span style="display:inline-block;width:1em;height:1em;background:#8A96A3;border:1px solid #888"></span> `#8A96A3` |
| **Паневразийская** | <span style="display:inline-block;width:1em;height:1em;background:#111827;border:1px solid #888"></span> `#111827` | <span style="display:inline-block;width:1em;height:1em;background:#F0F2F5;border:1px solid #888"></span> `#F0F2F5` | <span style="display:inline-block;width:1em;height:1em;background:#7AA7E8;border:1px solid #888"></span> `#7AA7E8` | <span style="display:inline-block;width:1em;height:1em;background:#69C9B3;border:1px solid #888"></span> `#69C9B3` | <span style="display:inline-block;width:1em;height:1em;background:#D7B56D;border:1px solid #888"></span> `#D7B56D` | <span style="display:inline-block;width:1em;height:1em;background:#8995A5;border:1px solid #888"></span> `#8995A5` |
| **Русская и славянская — средняя** | <span style="display:inline-block;width:1em;height:1em;background:#202733;border:1px solid #888"></span> `#202733` | <span style="display:inline-block;width:1em;height:1em;background:#E8EAF0;border:1px solid #888"></span> `#E8EAF0` | <span style="display:inline-block;width:1em;height:1em;background:#A9C0DD;border:1px solid #888"></span> `#A9C0DD` | <span style="display:inline-block;width:1em;height:1em;background:#8FC4B2;border:1px solid #888"></span> `#8FC4B2` | <span style="display:inline-block;width:1em;height:1em;background:#D6BD8B;border:1px solid #888"></span> `#D6BD8B` | <span style="display:inline-block;width:1em;height:1em;background:#9AA3AF;border:1px solid #888"></span> `#9AA3AF` |
| **Центральноазиатская / Шёлковый путь** | <span style="display:inline-block;width:1em;height:1em;background:#1C2529;border:1px solid #888"></span> `#1C2529` | <span style="display:inline-block;width:1em;height:1em;background:#F5F0E5;border:1px solid #888"></span> `#F5F0E5` | <span style="display:inline-block;width:1em;height:1em;background:#7FADE0;border:1px solid #888"></span> `#7FADE0` | <span style="display:inline-block;width:1em;height:1em;background:#45C6C2;border:1px solid #888"></span> `#45C6C2` | <span style="display:inline-block;width:1em;height:1em;background:#D9B56D;border:1px solid #888"></span> `#D9B56D` | <span style="display:inline-block;width:1em;height:1em;background:#92A3A0;border:1px solid #888"></span> `#92A3A0` |
| **Восточноазиатская** | <span style="display:inline-block;width:1em;height:1em;background:#171A1C;border:1px solid #888"></span> `#171A1C` | <span style="display:inline-block;width:1em;height:1em;background:#F2EFE7;border:1px solid #888"></span> `#F2EFE7` | <span style="display:inline-block;width:1em;height:1em;background:#D97878;border:1px solid #888"></span> `#D97878` | <span style="display:inline-block;width:1em;height:1em;background:#72C7A5;border:1px solid #888"></span> `#72C7A5` | <span style="display:inline-block;width:1em;height:1em;background:#D8B76A;border:1px solid #888"></span> `#D8B76A` | <span style="display:inline-block;width:1em;height:1em;background:#969F9B;border:1px solid #888"></span> `#969F9B` |
| **ICC — Международный уголовный суд** | <span style="display:inline-block;width:1em;height:1em;background:#0E1A2B;border:1px solid #888"></span> `#0E1A2B` | <span style="display:inline-block;width:1em;height:1em;background:#EAF0F7;border:1px solid #888"></span> `#EAF0F7` | <span style="display:inline-block;width:1em;height:1em;background:#5B8FD6;border:1px solid #888"></span> `#5B8FD6` | <span style="display:inline-block;width:1em;height:1em;background:#7FB7C9;border:1px solid #888"></span> `#7FB7C9` | <span style="display:inline-block;width:1em;height:1em;background:#D9B25A;border:1px solid #888"></span> `#D9B25A` | <span style="display:inline-block;width:1em;height:1em;background:#7E8CA0;border:1px solid #888"></span> `#7E8CA0` |
| **SCOTUS — Верховный суд США** | <span style="display:inline-block;width:1em;height:1em;background:#1A1416;border:1px solid #888"></span> `#1A1416` | <span style="display:inline-block;width:1em;height:1em;background:#F3EEE6;border:1px solid #888"></span> `#F3EEE6` | <span style="display:inline-block;width:1em;height:1em;background:#9E2B35;border:1px solid #888"></span> `#9E2B35` | <span style="display:inline-block;width:1em;height:1em;background:#8FA9B8;border:1px solid #888"></span> `#8FA9B8` | <span style="display:inline-block;width:1em;height:1em;background:#C9A24B;border:1px solid #888"></span> `#C9A24B` | <span style="display:inline-block;width:1em;height:1em;background:#8C8178;border:1px solid #888"></span> `#8C8178` |
| **Греческая (эллинская)** | <span style="display:inline-block;width:1em;height:1em;background:#0A1A30;border:1px solid #888"></span> `#0A1A30` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#2E7BD6;border:1px solid #888"></span> `#2E7BD6` | <span style="display:inline-block;width:1em;height:1em;background:#5E9FE0;border:1px solid #888"></span> `#5E9FE0` | <span style="display:inline-block;width:1em;height:1em;background:#9FC3EC;border:1px solid #888"></span> `#9FC3EC` | <span style="display:inline-block;width:1em;height:1em;background:#6E86A6;border:1px solid #888"></span> `#6E86A6` |
| **Турецкая (Al Bayrak)** | <span style="display:inline-block;width:1em;height:1em;background:#2A0609;border:1px solid #888"></span> `#2A0609` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#F23D48;border:1px solid #888"></span> `#F23D48` | <span style="display:inline-block;width:1em;height:1em;background:#F0A9AE;border:1px solid #888"></span> `#F0A9AE` | <span style="display:inline-block;width:1em;height:1em;background:#F0C9A9;border:1px solid #888"></span> `#F0C9A9` | <span style="display:inline-block;width:1em;height:1em;background:#A8767A;border:1px solid #888"></span> `#A8767A` |
| **Kiro** | <span style="display:inline-block;width:1em;height:1em;background:#0B0B12;border:1px solid #888"></span> `#0B0B12` | <span style="display:inline-block;width:1em;height:1em;background:#F5F5FA;border:1px solid #888"></span> `#F5F5FA` | <span style="display:inline-block;width:1em;height:1em;background:#A98EDA;border:1px solid #888"></span> `#A98EDA` | <span style="display:inline-block;width:1em;height:1em;background:#6E8BE0;border:1px solid #888"></span> `#6E8BE0` | <span style="display:inline-block;width:1em;height:1em;background:#E8C547;border:1px solid #888"></span> `#E8C547` | <span style="display:inline-block;width:1em;height:1em;background:#6A6482;border:1px solid #888"></span> `#6A6482` |
| **SLeeLa** | <span style="display:inline-block;width:1em;height:1em;background:#08080E;border:1px solid #888"></span> `#08080E` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#B07CFF;border:1px solid #888"></span> `#B07CFF` | <span style="display:inline-block;width:1em;height:1em;background:#5A7CFF;border:1px solid #888"></span> `#5A7CFF` | <span style="display:inline-block;width:1em;height:1em;background:#FFD21E;border:1px solid #888"></span> `#FFD21E` | <span style="display:inline-block;width:1em;height:1em;background:#6E5F9E;border:1px solid #888"></span> `#6E5F9E` |

#### Национальные цвета флагов (исходная палитра)

Греческая и турецкая схемы выведены из **официальных цветов государственных флагов** двух стран, приведённых здесь с точными опубликованными значениями hex. Фоны и акценты схем выше — это затемнённые, удобные для редактора интерпретации этих исходных цветов; приведённая ниже таблица служит справкой «цвета в том виде, как они официально представлены».

| Государство | Цвет | Hex | Основание и значение |
|---|---|---|---|
| **Греция** | Синий | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | Синий цвет государственного флага (закон 851/1978). Небо и моря; в геральдике блазонируется как *лазурь*. |
| **Греция** | Белый | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Белый цвет государственного флага. Чистота борьбы за независимость; крест обозначает восточное православное христианство. |
| **Türkiye** | Красный (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | Красный цвет государственного флага (Закон о турецком флаге). Поле; по традиции — кровь мучеников и суверенитет Османской империи. |
| **Türkiye** | Белый | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Белый цвет государственного флага. Полумесяц и звезда — мир, чистота и символ тюркско-исламского наследия. |

Схемы привязывают передний план каждого редактора к национальному **белому**, а ключевые слова, курсор, выделение и строку состояния — к точному национальному **синему** (`#0D5EAF`) или **красному** (`#E30A17`), так что значимые цвета буквально совпадают с национальными. Это интерпретационные дизайнерские палитры; справка по цветам флагов приводится ради происхождения данных и не является официальным представлением ни одного из государств. Значения цветов следуют описанию государственной символики Министерства иностранных дел Греческой Республики и традиции Закона о турецком флаге.


### Таблица цветовых образцов

Ячейки палитры ниже используют **SVG-образцы**, поэтому GitHub отображает настоящие цвета, а не пустые HTML-ячейки цвета. Каждая полоса показывает шесть цветов в таком порядке: фон, передний план, ключевые слова, типы, строки и комментарии. Точные hex-коды перечислены рядом с каждым образцом, в том же порядке схем, что и в справочной таблице выше.

| Схема | Отображаемые цвета палитры | Hex-коды (фон · передний план · ключевые слова · типы · строки · комментарии) |
|---|---|---|
| **Американская профессиональная** | ![American Professional palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Паневразийская** | ![Pan-Eurasian palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Русская и славянская — средняя** | ![Russian and Slavic Medium palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Центральноазиатская / Шёлковый путь** | ![Central Asian Silk Road palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **Восточноазиатская** | ![East Asian palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC — Международный уголовный суд** | ![ICC palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS — Верховный суд США** | ![SCOTUS palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Греческая (эллинская)** | ![Greek Hellenic palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Турецкая (Al Bayrak)** | ![Turkish Al Bayrak palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Kiro palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![SLeeLa palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

Дополнительные акцентные цвета используются для функций, констант, выделений, диагностики и директив препроцессора. Русская и славянская палитра намеренно использует цвета средней интенсивности — сланцевый, синий, шалфейный, рубиновый и античное золото — для более спокойного профессионального облика.

### Установка схем Vim

Из корня репозитория:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Добавьте следующее в `~/.vimrc`, чтобы включить подсветку синтаксиса SLeeLa и выбрать схему:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Выберите ровно одно из этих названий colorscheme:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Например, используйте `colorscheme sleela-russian-slavic` для средней русской и славянской палитры или `colorscheme sleela-greek` / `colorscheme sleela-turkish` для эллинской палитры и палитры Al Bayrak. Перезапустите Vim или загрузите конфигурацию командой `:source $MYVIMRC`.

### Установка схем Emacs

Создайте локальный каталог тем и скопируйте файлы режима и тем из корня репозитория:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Добавьте это в `~/.emacs` или `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Замените `sleela-american` на `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` или `sleela-sleela`, чтобы выбрать другую схему. Если Emacs спросит, доверять ли теме, проверьте локальный файл темы, прежде чем соглашаться.

### Предварительный просмотр native-палитр на C/C++

Небольшая native-утилита палитр умеет перечислять схемы, выводить их цвета для терминала и находить отдельный цвет:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Для обратной совместимости `--palette` и `--hex COLOR` без `--theme` выбирают American Professional. Результаты сборки и тестов следует проверять на целевой системе и в CI; документация команды сама по себе не является свидетельством того, что она прошла.

Полное описание палитр и подробности об утилите см. в [руководстве по интеграции редакторов](../../linux-editors/README.md).

## IX. Участие в проекте
1. Прочитайте указатель документации и политику тестирования.
2. Вносите целенаправленные изменения с позитивными и негативными тестами.
3. Запускайте целевые тесты, затем широкий набор.
4. Сообщайте сведения об ОС и наборе инструментов; не описывайте пропущенные проверки как пройденные.

SLeeLa разрабатывается компанией **MEARVK LLC**. Исторические материалы целевой страницы сохранены в [`docs/README-LEGACY.md`](../../docs/README-LEGACY.md).

## X. AE6E66 — ожидаемый сбор с Round Public email

В рамках инициативы AE6E66 заявленное ожидание проекта составляет **$300,000,000.00 USD за каждое Round Public email** в качестве предлагаемой суммы налогового сбора, связанного с каждым соответствующим требованиям Round Public email.

Это заявление фиксирует лишь ожидание проекта. Оно **не** является юридическим налоговым начислением, распоряжением правительства, решением суда или независимо установленной налоговой обязанностью. Любой фактический сбор потребовал бы соответствующих правовых полномочий, юрисдикции, определений, процедуры и решения компетентного государственного органа.


## XI. Алфавит и среда выполнения наборов символов

Документация по алфавитам SLeeLa и native-среда выполнения символов поддерживаются в alphabet/sleela/ и lib/alphabet/native/. Native-тесты на C++17 охватывают строгое декодирование/кодирование скаляров UTF-8, сигнатуры ёмкости C<n> и ограниченную проверку принадлежности к набору символов:

    make -C lib/alphabet/native test

Экспериментальный каталог UTF-4088 и его адаптер поиска для SLeeLa документированы в utf-4088/SLeeLa-ALPHABET-INTEGRATION.md. Идентификаторы UTF-4088 — это отдельное пространство имён, а не скалярные значения Unicode. Работа над привязкой компилятора и SLVM отслеживается явно отдельно от этих native-основ.

## XII. AutoCAD Creator™

[AutoCAD Creator™](../../autocad/creator/README.md) — интерактивное 2D CAD-приложение,
целиком написанное на SLeeLa. Если родственный AutoCAD Renderer превращает план
в чертёж за один проход, то Creator позволяет **создавать и изменять**
2D-документ AutoCAD в реальном времени — добавлять стены, комнаты, размеры и
примечания, затем перемещать, переименовывать, переносить на другие слои,
удалять и отменять размещённое — и **сохранять** результат как настоящий
чертёж AutoCAD DXF (текстовый R12, открывается в AutoCAD / LibreCAD).

Его графический интерфейс построен на **SleelaUI™** — собственном
кроссплатформенном UI-тулките SLeeLa (в [`lib/user-interface`](../../lib/user-interface)),
а не на JavaFX — и открывает настоящее нативное окно в X11 / Cocoa / Win32.
Дизайн следует одному правилу: **«SleelaUI показывает; SLeeLa решает»**: GUI
никогда не вычисляет геометрию и не пишет DXF. Каждый инструмент выдаёт
намерение создать / изменить / удалить / отменить / сохранить авторитетному
[`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela), который владеет редактируемым
документом и обратимой историей команд, тогда как холст лишь показывает
предварительный вид геометрии, которую хранит Creator.

Многократно используемый механизм редактирования находится в
[`lib/autocad`](../../lib/autocad) —
[`SLCadEntity`](../../lib/autocad/SLCadEntity.sleela),
[`SLCadDocument`](../../lib/autocad/SLCadDocument.sleela),
[`SLCadCommand`](../../lib/autocad/SLCadCommand.sleela) и
[`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela) — и работает от начала до
конца без графической оболочки. Оболочка GUI и демонстрация находятся в
[`autocad/creator`](../../autocad/creator):

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

На машине с дисплеем это открывает окно живого редактирования; на хосте без
дисплея чертёж строится в памяти, выводятся отчёт и DXF, после чего программа
штатно завершается.


## XIII. Порядок входной карты UTF-4088

Специализированный каталог UTF-4088 определяет обход своей пространственной входной карты и абстрактные итерации давления в [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). Политика предметного индекса National Lien находится в [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Координаты карты, уровни давления (1–50), постоянные идентификаторы каталога и алфавитные ранги — независимые поля; ни одно из них не следует молча подставлять вместо другого. Давление — это алгоритмический уровень итерации, а не физическая величина.

## XIV. Недавний и расширенный указатель пакетов

Этот дополняемый указатель выделяет недавно расширенные пакеты, предметные
библиотеки и коллекции исходников. Записи сгруппированы по алфавиту названий
пакетов или тем. Описания указывают расположение в репозитории; статус
реализации и проверки следует смотреть в собственной документации и тестах
каждого пакета.

### A–C

- **Advantage** — `lib/advantage/` и `advantage/`: «улучшение» уникального
  дивиденда относительно модели в виде слоистой луковицы Sequitur с пятью
  предметными подкаталогами — `math/` (совместная максимизация прибыли, торг
  по Нэшу, двусторонний «продавец газет»), `physics/` (метрика/геодезическая/
  контакт/SDE поля доверия, помеченная модельная аналогия), `integration/`
  (отсеки вертикальной интеграции и оценка изощрённости), `grading/` (оценщик
  контура изощрённости A–D для модулей и наборов) и `output/` (вывод в
  терминал / CommonRails / сокет). Нативный потребитель на C/C++ (`advantage/`)
  подключает модель к разобранным торговым примерам. См.
  [`lib/advantage/README.md`](../../lib/advantage/README.md) и
  [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md).
- **Alphabet и UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` и
  `utf-4088/`: каталоги алфавитов, native-поддержка наборов символов, порядок
  входной карты и документация по интеграции UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` и `lib/autocad/`: интерактивные
  процессы 2D-черчения и многократно используемые компоненты CAD-документа и
  редактирования.
- **Ballum** — управление дисплеем на уровне пикселей: установка/чтение
  отдельных пикселей, блиттинг области, яркость/теплота/гамма всего дисплея,
  ввод указателем/клавиатурой и **полноэкранная поверхность framebuffer** для
  полного, принадлежащего и быстрого попиксельного управления (окно
  override-redirect + единственный вывод `XPutImage`/MIT-SHM в Linux/X11).
  Native-ядро находится в `ballum/`, фасад SLeeLa — в `lib/ballum/`, мост VM —
  в `impl/`; см. [`ballum/README.md`](../../ballum/README.md) и запускаемые
  [`ballum/example/`](../../ballum/example/).
- **CDMA** — `cdma/` и `lib/cdma/`: технические материалы, связанные с CDMA, и
  работа над библиотекой исходников SLeeLa.
- **Character** — `lib/character/`: профили персонажей и многократно
  используемая статистика персонажей, включая примеры биографических профилей.
- **Coorenagraph** — `lib/coorenagraph/`: этические рассуждения, моральная
  лексика, обдумывание и связанные понятия; см. README пакета и исходные файлы.

### D–G

- **Исходные модели управления (Governance)** — [lib/governance](../../lib/governance/README.md) определяет декларативные формы исходного кода SLeeLa для предложений, анонимных ручных бюллетеней и агрегированных подсчётов. Предлагаемая аннотация @vote и ручной процесс описаны в [registry/DEMOCRATIC-VOTING-SYNTAX.md](../../registry/DEMOCRATIC-VOTING-SYNTAX.md); нынешние компиляторы рассматривают её как инертные метаданные в комментарии.

### H–I

- **HTTP 1.0–8.0** — от `lib/http-1.0/` и далее по имеющимся версионным
  каталогам пакетов HTTP: спецификации протоколов и реализации для конкретных
  версий. Точный охват и статус проверки смотрите в документации каждой версии.
- **Международный уголовный суд** — `lib/international-criminal-court/` и
  `INTERNATIONAL.CRIMINAL.COURTS.md`: связанные предметные исходники и
  документация.

### O–S

- **OFDMA** — `ofdma/`: технические материалы по множественному доступу с
  ортогональным частотным разделением и ресурсы разработки на C/C++.
- **Philosophy и модуль Conversation Mask** — [`philosophy/`](../../philosophy/README.md)
  содержит native-основы на C/C++, а [`philosophy/module/`](../../philosophy/module/README.md)
  организует их следующий модульный слой в три явные области:
  [`data/`](../../philosophy/module/data/README.md) — для типизированных датированных
  наблюдений, окончаний событий, параметров периода полураспада, происхождения
  и согласия; [`helper/`](../../philosophy/module/helper/README.md) — для
  детерминированной проверки, затухания релевантности, совместимых сравнений и
  представления, отфильтрованного по согласию; и
  [`psychiatry-friend/`](../../philosophy/module/psychiatry-friend/README.md) — для
  поддерживающего, неклинического диалогового интерфейса. Дескрипторы для
  SLeeLa и составленное Tree находятся в [`lib/philosophy/`](../../lib/philosophy/README.md),
  рядом с более широким пакетом philosophy. Модуль строится на явном вводе и
  принципе «сначала согласие»: он не отслеживает пассивно людей или медиа, не
  ставит диагнозов, не выводит скрытых намерений, не оценивает ценность
  человека и не рекомендует изменять лекарства. Его связи событий сохраняют
  происхождение и навигацию, но не независимое подтверждение.
- **Synchro** — интеграция native-среды выполнения/компилятора, диспетчеризация
  пакетов и функциональность, связанная с измерениями; см. соответствующий
  исходный код `impl/` и реализацию компилятора.
- **SleelaUI / терминальный UI** — `lib/user-interface/` и проект
  `sleela-terminal`: интеграция UI SLeeLa и компоненты терминального
  взаимодействия.

### U–W

- **Организация Объединённых Наций** — `united-nations/`: предметные материалы репозитория.
- **Wi-Fi 4, Wi-Fi 5 и Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`,
  `wi-fi-6/` и связанные исходные каталоги `lib/wifi/`: материалы по
  беспроводным сетям и ресурсы конфигурации для конкретных поколений.

### Сопровождение указателя пакетов

Когда пакет добавляется или существенно расширяется, добавьте его запись сюда в
подходящую алфавитную группу, сославшись на его канонический каталог и README,
и описывайте только возможности, подтверждённые зафиксированными исходниками.
Сохраняйте этот указатель аддитивным: не удаляйте и не переписывайте
существующие разделы README при обновлении указателя пакетов.


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

Демократические США и политика США.

---

## XVI. Национальные брендовые изображения науки

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. Экспериментальный каталог UTF-4088 на миллион записей

[Генератор каталога UTF-4088](../../utf-4088/tools/generate_catalogue.py) детерминированно строит экспериментальный каталог символов из 1 048 576 записей. Документированный компактный формат описан в [utf-4088/FORMAT.md](../../utf-4088/FORMAT.md); контракт генерации и проверки — в [utf-4088/GENERATION-RESULTS.md](../../utf-4088/GENERATION-RESULTS.md).

Запустите генератор с помощью Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Сгенерированный бинарный файл занимает 16 777 244 байта и рассчитан так, чтобы оставаться меньше 20 000 000 байт. Модульные тесты и рабочий процесс GitHub Actions проверяют число записей, уникальность глифов, размер и SHA-256; успешные запуски публикуют бинарный файл и сводку как артефакт рабочего процесса `utf4088-million-catalogue`. Сгенерированный бинарный файл намеренно не хранится в истории Git. UTF-4088 остаётся экспериментальным и не совместим с Unicode.

---

# SLeeLa
<p align="center"><img src="../../images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="Маркетинговый логотип SLeeLa" width="560"></p>

---

## XVIII. Недавние дополнения — константа Pi, запуск Modium/Registry, математика sleelactl и `@native`

В этом разделе записаны возможности, добавленные в ходе работ 2026-11, сначала самые новые группы. Авторитетны связанные исходные файлы и статус их проверки; изменения на рассмотрении не описываются как влитые, пока не попадут в `master`.

### Каталожная константа Pi на 100 000 000 цифр

Репозиторий поставляется с проверенным значением π в **сто миллионов цифр** в [`pi-generator/pi.100000000.digits`](../../pi-generator/pi.100000000.digits) (корректная десятичная запись, `3.14159…`), а также со значением в 57 344 цифры в [`pi-generator/pi.57344.digits`](../../pi-generator/pi.57344.digits). Оба независимо подтверждены как подлинное π — см. [`pi-generator/VERIFIED.md`](../../pi-generator/VERIFIED.md), где задокументированы проверки (учебниковый префикс, точка Фейнмана в дробной позиции 762, известная миллионная цифра, совпадение с `lib/math/SLPiReference.sleela` и анализ частот цифр по всему файлу) и однократный скрипт воспроизведения.

### Modium™ / Registry™ загружают константу Pi при запуске подсистемы

На выделенных точках входа подсистем — `sleela registry` и `sleela modium` (и при первом запуске установщика), **а не** при каждом `sleela run` — подсистемы Modium и Registry загружают большую каталожную константу π. На интерактивном терминале они выводят временную красную строку `Loading Modium™` / `Loading Registry™`, которая затем стирается на месте до начала обычного вывода; эффект строго привязан к TTY, поэтому конвейерный вывод и вывод тестов остаются побайтно неизменными. Локатор (а не байты) заносится как значения по умолчанию ключей `Math` реестра `piReferenceLargePath` и `piReferenceLargeDigits` — как в модели SLeeLa ([`lib/registry/SLRegistry.sleela`](../../lib/registry/SLRegistry.sleela)), так и в native-заполнителе ([`registry/sleela_registry.c`](../../registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` предоставляет `largePath()` / `largeDigits()`. Константа π устанавливается в системное расположение SLeeLa (`<data>/pi/`) **как** стандартным установщиком `make`, **так и** управляемым «красивым» установщиком. Подробности: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](../../pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — красивый установщик и subject `math` на native-основе

- Управляемый («красивый») установщик теперь устанавливает `sleelactl` (плоскость управления SLeeLa) и его реестр модулей, как и установщик `make`. См. [`bin/SLEELACTL.md`](../../bin/SLEELACTL.md).
- В `sleelactl` появилась новая природа **`subject`** для native-библиотек предметов на C/C++, скомпилированных в среду выполнения, и зарегистрирован первый из них — **`math`**. `sleelactl run math` сквозным образом задействует native-бэкенд `__NativeMath` (`impl/subjects/math/math_native.cpp`) через эталонную демонстрацию предмета; `sleelactl info math` объясняет, как к нему обращаться (`import math;`). Природа subject предлагает только `run info` — глагола `check` намеренно нет, поскольку `sleela check` не загружает native-subjects, зарегистрированные в SHEET.

### `@native`, Native Drive и выполнение на нескольких языках

Новая аннотация метода **`@native`** запрашивает ahead-of-time native-компиляцию горячего метода. Новая команда `sleela aot` находит методы с `@native`, проверяет, что каждый находится в поддерживаемом *выгружаемом подмножестве*, и выдаёт C/C++ — **с отказом по закрытому принципу (fail closed)** (с отказом и точной причиной) для любой конструкции, которую она пока не умеет транслировать, так что наполовину транслированный метод никогда не создаётся.

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

`@native` остаётся запросом на AOT-обработку, а неподдерживаемые конструкции отвергаются до выдачи кода. [Native Drive](../../impl/native-drive/README.md) поддерживает многомодульные сборки исполняемых файлов и разделяемых библиотек C/C++ через хост-компиляторы (включая MSVC), поддерживаемый AOT-вывод SLeeLa, инспекцию символов по ABI native-загрузчика, выполнение процессов без оболочки, обработку stdin/stdout/stderr, многоступенчатые конвейеры и делегирование XClass производственному загрузчику. `sleelactl` предоставляет его команды сборки/запуска и направляет инспекцию XClass и явную генерацию/установку юнитов systemd через `inspect native-drive`. VM владеет ограниченной таблицей дескрипторов native-модулей с обязательной авторизацией хоста и дескрипторами, проверяемыми по поколению. Синтаксис 3.0 теперь добавляет `nativeOpen`, `nativeCall` и `nativeClose`: append-only диспетчеризация байт-кода пересекает отдельно версионируемый ABI null/bool/int64/double/UTF-8, тогда как принадлежащие VM массивы, структуры и другие составные значения остаются внутри интерпретатора и могут использовать резервный вызов байт-кода с тем же именем. Плагины могут по желанию получать версионируемую таблицу обратных вызовов хоста для вызова точных функций SLeeLa через вложенное выполнение VM, с корневым хранением приостановленного стека для GC, явным списком разрешённых имён обратных вызовов хоста и отклонением рекурсивных вызовов native API. Native-код остаётся доверенным кодом внутри процесса, а не песочницей. Аннотации политики процессов в исходном коде имеют версионируемую схему, но пока не разбираются и не применяются. Подробности, границы безопасности и реестр из семи направлений работ: [Native Drive](../../impl/native-drive/README.md) и [`pi-generator/AT-NATIVE.md`](../../pi-generator/AT-NATIVE.md).


## XIX. Международные издания README

SLeeLa предоставляет страновые точки входа README для 50 рынков своего международного издания. У каждой страны есть один файл с именем `README.<CC>.md`; если актуальны несколько языков, они сгруппированы в файле этой страны.

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

**Политика перевода:** Эти файлы — первоначальные локализованные страновые точки входа. Их введения и языковые заголовки локализованы, тогда как полный технический справочник остаётся в каноническом английском [README.md](../../README.md). Страновой файл нельзя называть полным переводом, пока не переведён каждый раздел, не проверены технические идентификаторы и ссылки, а издание не прошло языковую проверку. Охват языков на этом первом этапе репрезентативен и не является заявлением о том, что полностью переведены все конституционно признанные, региональные, коренные языки или языки жестов.

## Приложение — сохранённые региональные рекомендации

В этом приложении сохранены практические рекомендации из прежнего краткого русскоязычного руководства, не имеющие собственных разделов в каноническом README. При любом расхождении приоритет имеет канонический английский [README.md](../../README.md).

### Типичная цепочка инструментов

Нормативная спецификация языка находится в [SLEELA.syntax](../../SLEELA.syntax). Версию компилятора, версию синтаксиса и версию VM/ISA необходимо рассматривать как отдельные понятия. Типичная цепочка инструментов включает:

1. **Исходный код:** файлы `.sleela` с указанием версии языка и декларациями.
2. **Frontend:** лексический анализ, разбор, обработку символов и семантические проверки.
3. **Промежуточное представление:** структурированную форму для последующих этапов преобразования.
4. **Генерацию кода:** вывод для поддерживаемых VM- или промежуточных целей.
5. **Среду выполнения:** выполнение скомпилированного содержимого с механизмами управления памятью и потоком управления.
6. **Библиотеки и инструменты:** повторно используемые пакеты, документацию, сборку и верификацию.

Точные возможности зависят от текущей реализации. Наличие функции в спецификации не означает автоматически, что она поддерживается на каждой цели.

### Цели, профили и границы выполнения

SLeeLa отделяет исходный код от целей сборки и выполнения. Документированное семейство целей включает `SLVM`, `SLJVM` и `SLIR`; `NATIVE` — ограниченная цель с условиями, и нельзя считать её доступной на всех платформах. Профили `BASIC_COMPLETE` и `ADVANCED_TOTAL` обозначают разный охват. Для каждой сборки проверяйте фактическую цель, профиль и поддержку платформы.

Ссылка идентифицирует файл, модуль или ресурс, но сама по себе не даёт разрешения на его выполнение. Когда программа использует файлы, внешние библиотеки, native-привязки или системные ресурсы, границы авторизации и ответственность за выполнение должны быть определены явно. Описание в документации само по себе не доказывает гарантию безопасности во время выполнения; проверяйте реализацию и соответствующие тесты.

При взаимодействии SLeeLa с C или C++ граница интерфейса должна быть явной. Контракты ABI, владение памятью, типы данных, обработка ошибок и платформенные зависимости необходимо документировать и тестировать. Native-код следует считать отдельной границей доверия.

### Жизненный цикл ресурсов и диагностика

Документируйте владение, время жизни и ответственность за синхронизацию для ресурсов, совместно используемых функциями или потоками. Разделяйте local variables, shared object state, VM handles и serializable data. Локальный для VM дескриптор не является стабильным форматом файла или сетевым сообщением. Используйте определённый модулем интерфейс сериализации и тестируйте недопустимые, отсутствующие и граничные значения.

При диагностике записывайте коммит, ОС, архитектуру и набор инструментов. Сначала собирайте наименьшую относящуюся к делу цель, сохраняйте полный вывод ошибок, разделяйте ошибки компилятора и ошибки времени выполнения и создавайте минимальный воспроизводимый пример. Успешная сборка сама по себе не доказывает корректного поведения во время выполнения.

В отчёте о проверке следует указать точный коммит, ОС/архитектуру, версии компиляторов, полные команды сборки и тестирования, число пройденных/упавших/пропущенных тестов и известные ограничения. Пропущенные или не запускавшиеся тесты нельзя засчитывать как пройденные.

### Замечания о ссылках и адресах

Упоминания в README таких тем, как CSPML, Character, Governance, Synchro, SleelaUI и United Nations, описывают содержимое репозитория и не являются официальным одобрением со стороны государственных органов или организаций.

Наличие адресов или изображений в разделе **Bitcoin Conjegeum** (XV) не является финансовой рекомендацией, подтверждением права собственности или указанием переводить средства. Перед использованием проверяйте сведения независимо.

### Языковой охват русской редакции

Русская редакция предназначена для русскоязычных читателей. Она не заявляет о полном охвате всех языков народов Российской Федерации или всех региональных языковых форм. Вклад носителей других языков и специалистов по техническому переводу приветствуется.

**Статус перевода:** Разделы I–XIX переведены на русский язык. Технические идентификаторы, команды, примеры кода, имена файлов, ссылки и адреса сохранены. Рекомендуется заключительная языковая проверка носителями языка.
