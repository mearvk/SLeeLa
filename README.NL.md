# SLeeLa 3.0 — Nederland (NL)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>

<p align="center"><strong>Een native systeemprogrammeertaal met compiler, runtime en ecosysteem van virtuele machines.</strong></p>

> **Internationale landeneditie.** Dit bestand bundelt de taalversies die voor Nederland zijn geselecteerd. De technische vertaling wordt stapsgewijs uitgevoerd. Vertaalde onderdelen zijn volledig opgenomen; onderdelen die nog moeten worden vertaald, zijn uitdrukkelijk gemarkeerd.

**The Now. The Ingredient.** Een programmeertaal is meer dan alleen de syntaxis: ook de compiler, runtime, bibliotheken, hulpmiddelen en platformcontracten die software bruikbaar maken, horen erbij.

**Werkterreinen van SLeeLa.** SLeeLa omvat **Math**, **Object**, **Science** en **Reach**, de vier dimensies van het werk: wat berekenbaar is (Math), het vastgestelde object in de catalogus van wat bestaat (Object), het gedisciplineerde onderzoek naar waarheid (Science) en het uitbreiden van de mogelijkheden van een systeem naar andere systemen en mensen (Reach). **Nationale belangen blijven bestaan**; geen van deze vier dimensies vervangt ze. Wanneer een onderwerp de belangen van een land raakt, blijft die relatie behouden en wordt deze zorgvuldig vastgelegd, bijvoorbeeld in de metadatavelden `metaOrigin` en `metaNationalSchedule` van een klasse.

## I. Aan de slag

- [De native implementatie bouwen](impl/README.md)
- [Virtuele machines en Modium — praktische gebruikershandleiding](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [CPU-stack in de SLeeLa-broncode](lib/cpu/CPU.md): een volledige machineketen van logische poorten via ALU, cores en besturingssysteem tot programma's; met meer dan 130 CPU-profielen en instelbare [spanningen, variabele kloksnelheden en geavanceerde timingparameters](lib/cpu/TIMING.md), voor uitvoering [volgens specificatie of met overklokken](lib/cpu/components/README.md).
- [Documentatie-index](docs/README.md)
- [CSPML-gegevensbron en onderzoeksreferentie Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md): herkomst en integratierichtlijnen voor externe Amerikaanse gegevens over locaties, sociaaleconomische omstandigheden, arbeidskrachten, onderwijs en openbare scholen.
- [Gegenereerde API-documentatie](docs/api/README.md): actuele HTML-referentie, versiegebonden API-uitgave en instructies voor het opnieuw genereren.
- [API-catalogus](api/README.md)
- [Normen voor computationele vergelijkingen](comparison/README.md): normen voor typen, geheugen, ABI en conformiteit voor SLeeLa VM, Java/JVM en native C/C++.
- [Recursie-Stanchion — C- en C++-prototypes](recursion/stanchion/README.md): voorbeelden gericht op de primitieve typen Boolean, Byte, Character, Integer, Long, Float en Double, plus generieke, toegepaste, wetenschappelijke en educatieve recursiepatronen.
- [Matrix voor platformondersteuning en validatie](docs/PLATFORM-SUPPORT.md)
- [Merkrichtlijnen](docs/BRAND-GUIDE.md)
- [Merchandiseconcepten en grafische middelen](merchandise/README.md)
- [NitroWebExpress-logo](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Testbeleid en releasecriteria](markdown/TESTING.md)

## II. Nieuws

Recente toevoegingen aan de taal, drivers en standaardbibliotheek:

- **Standaardbibliotheekpakket `advantage`.** [`lib/advantage/`](lib/advantage/README.md) modela a obtenção de um dividendo único relativamente a um modelo, através de uma estrutura em camadas denominada **Sequitur onion** (dividendo → revisão → preocupações → matemática → adições → subtração → reagrupamento → iteração). Het omvat de domeinen `math/` (economia de comércio bilateral, maximização conjunta do lucro, negociação de Nash e rácio crítico newsvendor), `physics/` (analogia de modelação estrutural-económica dos campos de confiança), `integration/` (integração vertical recursiva), `grading/` (classificação de sofisticação A–D) e `output/` (saída encaminhável para terminal, CommonRails ou socket/conector). Inclui exemplos executáveis e documentação de definições, matemática e física. O consumidor C/C++ em [`advantage/`](advantage/README.md) integra o modelo em exemplos de comércio.
- **Anotações de temporização de ciclos — `@loopDelay` / `@loopBudget`.** Estas anotações comportamentais, colocadas antes de ciclos `for`, `while` ou `do`, introduzem pausas reais na execução. `@loopDelay(ms = 250)` define uma pausa por iteração; `@loopBudget(seconds = 10, iterations = 1000)` distribui um número conhecido de iterações por um orçamento de tempo total. Ambas utilizam o opcode aditivo `OP_SLEEP` (ISA 157), que também suporta a função integrada `sleep(<ms>)`. A implementação usa `nanosleep` / `Sleep`. Zie [`SLEELA.syntax` §32](SLEELA.syntax), a [referência da API](api/sleela/3.0/LOOP_TIMING_API.md) e o [exemplo executável](examples/loop-timing-annotation-demo.sleela).
- **SleelaDoc na linha de comandos — `sleela docs`.** Het commando lista tópicos documentáveis, apresenta pacotes e resolve classes, incluindo nomes sem distinção entre maiúsculas e minúsculas e o prefixo padrão `SL`. As páginas incluem definição, objetivo de conceção, relações, estrutura, conceitos de utilização e uma escala de domínio de cinco níveis. As etiquetas `@intent`, `@thesis`, `@axiom`, `@param`, `@see` e `@mastery` enriquecem a documentação. `sleela docs --generate [dir]` cria um corpus de texto para consulta offline; `make sleeladoc` reúne o corpus e as fontes correspondentes em `sleeladoc/build/sleeladoc.jar`. Zie [`lib/sleeladoc/`](lib/sleeladoc/README.md) e [`sleela(1)`](man/man1/sleela.1).
- **SleelaDoc e documentação HTML.** O extrator [`lib/sleeladoc`](lib/sleeladoc/README.md) transforma comentários documentais de unidades `.sleela` em páginas Markdown ou HTML. O sítio navegável de toda a biblioteca `/lib` é gerado em [`sleeladoc-api/`](sleeladoc-api/README.md). A compilação pode também emitir páginas MediaWiki através de `--wiki`. Zie [SLEELADOC](manifesto/SLEELADOC.md), [SLEELADOC.WIKI](manifesto/SLEELADOC.WIKI.md) e o [exemplo](lib/sleeladoc/sleeladoc-demo.sleela).
- **Modelo de objetos de documento XML (`lib/xml`).** A biblioteca inclui uma árvore DOM com nós de elemento, texto, comentário, CDATA e instrução de processamento, atributos nomeados, serialização compacta/formatada, escape XML 1.0 §2.4, análise por células de caracteres e registo de qualidade `SLXmlAspect`. A igualdade entre nós é reflexiva, simétrica e determinística, com níveis de identidade, assunto e estrutura. Zie [`lib/xml/README.md`](lib/xml/README.md) e o [exemplo executável](lib/xml/xml-dom.sleela).
- **Primitivos inteiros alargados.** `int`, `long` e `int64` são inteiros assinados de 64 bits; `int128` é atualmente suportado por uma representação de 64 bits, estando o suporte real de 128 bits documentado como trabalho futuro. As famílias binárias assinadas `int32`–`int16777216` (sintaxe 2.7) e não assinadas `uint32`–`uint16777216` (sintaxe 2.8) ampliam a família. Zie [`SLEELA.syntax`](SLEELA.syntax) §§21, 24 e 25.
- **Campos consequentes `@mix` (sintaxe 2.9).** Uma classe pode declarar campos de instância repetidos com o mesmo nome se cada declaração tiver `@mix`. O grupo é indexado por ordem crescente (`1,2,3,…`) ou por índices explícitos em `@mix(1,2,3,...)`; a quantidade de índices tem de corresponder à quantidade de campos. Zie [§28](SLEELA.syntax).
- **Preonems — marcadores documentais do Processo Natural.** Os marcadores declarativos incluem `author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`, `pure`, `gasoline` e `artifact`. São contextuais, dependentes da versão e não alteram a análise sintática, a tipagem ou a geração de código; fora da posição de marcador continuam a ser identificadores normais. Zie [`SLEELA.syntax`](SLEELA.syntax) §§18–20, 23, 26, 30 e 35.
- **Nordshrift com acesso integral à biblioteca.** Na sintaxe 3.0, Nordshrift utiliza o mesmo índice recursivo de `/lib` que o compilador, abrangendo pacotes e unidades `.sleela`; `nordshrift check` apresenta as contagens da árvore completa.
- **Stanchion de recursão.** [`recursion/stanchion/`](recursion/stanchion/README.md) contém protótipos de referência C e C++ para sete contratos primitivos documentados, além de exemplos genéricos, aplicados, científicos e pedagógicos. As fontes de referência devem ser compiladas e testadas antes de serem consideradas verificadas.
- **Comentários angulares (`<` / `<<`).** O frontend reconhece comentários de linha `< line >` e de bloco `<< block >>`; consulte [§1.1](SLEELA.syntax).
- **Literais inteiros por radix.** Os literais `int` aceitam hexadecimal (`0x`), binário (`0b`) e octal (`0o`), com separadores `_`, tanto na linguagem como no formato de modelos CPU [`.xlt`](lib/xlt/README.md). Zie [§1.4](SLEELA.syntax).
- **Norma Metadata Class.** O padrão de metadados descritivos — proveniência, legalidade e âmbito jurisdicional — é aplicado aos ficheiros `.sleela` que contêm classes em `/lib`. Zie [§15](SLEELA.syntax) e a [lição 13](tutorial/13-metadata-class.md).
- **Família de texto e String.** `lib/text` fornece operações de caracteres e cadeias, incluindo `charAt`, `substring`, `indexOf`, `reverse`, alinhamento de colunas e modelos posicionais. Zie o [tutorial](tutorial/14-text-and-strings.md) e o [exemplo](lib/text/text-string-demo.sleela).
- **String em folhas Nordshrift.** Uma folha `.sst` pode declarar um bloco tipado `strings:` que contribui para o manifesto de componentes da compilação. Zie a [referência Nordshrift](wiki/Nordshrift-Sheet-Reference.md).
- **Modelos CPU por opcodes e equivalência de chamadas.** Cada `SL<ARCH>CPU` suporta a execução de um fluxo nativo de instruções através do ciclo fetch-decode-execute e a execução equivalente de código de outro processo sobre a infraestrutura VM partilhada. Zie [`lib/cpu/EXECUTION.md` §5](lib/cpu/EXECUTION.md).

## III. Bouwen en testen

**Vereisten:** een toolchain C/C++ (GCC ou Clang) en GNU Make; Python 3 para ferramentas de manifesto e fluxo de trabalho; em Linux, também os pacotes de desenvolvimento X11/Xrandr para produtos de visualização. Het commando `make deps` de cada módulo indica ou instala as dependências necessárias. A compilação em `impl/` falha de forma segura se o manifesto SHA-256 de confiança não for satisfeito; o Makefile da raiz inclui-o automaticamente.

De snelste start vanuit een schone clone:

```sh
make                                  # alles bouwen vanuit de repository-root
```

Of alleen de native kern bouwen en testen:

```sh
make -C impl all                      # compilar apenas o núcleo do compilador e do ambiente de execução
bash ./test-suites/run-all.sh --all   # executar o conjunto completo de testes
```

Het commando `make` in de root compila todos os produtos (núcleo, VM, codecs, churn, antivírus, etc.) e termina com a criação do **registo** e a materialização da sua tabela-base. Assim, uma árvore de trabalho compilada contém desde logo um registo verificável.

Belangrijke doelen:

```sh
make                     # compilar tudo (alias de make all; registo criado no fim)
make all                 # a mesma compilação completa, explicitamente indicada
sudo make install        # instalar comandos e ficheiros de suporte do ambiente de execução (ver § IV)
make clean               # remover resultados de compilação de todos os sistemas integrados
make help                # apresentar todos os objetivos do Makefile da raiz
```

Andere nuttige doelen:

```sh
make registry            # compilar apenas o registo e materializar a tabela
./build-registry.sh      # a mesma operação através de um script direto
make tests               # compilar e executar o conjunto de testes
make ballum              # compilar a biblioteca de visualização Ballum para este sistema operativo
bin/install-modules      # instalar/compilar módulos de forma faseada durante cerca de 10–20 minutos
```

O objetivo `registry` executa [`build-registry.sh`](build-registry.sh): compila o núcleo do registo, coloca a tabela-base no armazenamento de texto portátil `registry/build/sleela.sleelareg` e exporta o script da tabela MySQL `registry/build/registry.sql` (através de `sleela registry export`). O registo pode ser inspecionado com `sleela registry` ou `registry/build/sleela-registry review`. Zie o [guia do registo](registry/README.md) e a [arquitetura Registry 3.0](registry/REGISTRY-3.0-ARCHITECTURE.md).

Zie os guias de compilação e testes para conhecer os pré-requisitos e os passos específicos de cada plataforma. A compilação bem-sucedida de um componente não demonstra que todas as bibliotecas ou plataformas estejam prontas para lançamento.

## IV. Installatie

Installeer na het bouwen van de native producten os comandos e os ficheiros de suporte do ambiente de execução:

```sh
sudo make install
make install PREFIX="$HOME/.local"        # instalação apenas para o utilizador, sem sudo
make install DESTDIR="$PWD/package-root" # instalação num diretório de preparação de pacotes
make install-check
```

Het conventionele installatieprogramma gebruikt standaard `/usr/local/bin`, `/usr/local/lib/sleela` e `/usr/local/share/sleela`. Het behoudt een bestaande opstartconfiguratie e não substitui comandos preexistentes que não estejam sob controlo de versões. Verwijderen wist uitsluitend os ficheiros registados pelo instalador; a configuração e os dados de execução não registados são preservados.

Gebruik voor een gerichte installatie van uitsluitend het hoofdpakket — os binários `sleela`, `sleelvac` e `nordshrift`, a biblioteca padrão, os protocolos e os módulos de suporte — utilize o instalador limpo:

```sh
make clean-install       # instalação orientada apenas do pacote principal
```

Primeiro, é perguntado se o pacote principal deve ser instalado; em seguida, escolhe-se o âmbito da instalação. Se a operação for bem-sucedida, pode ser apresentado um seletor opcional de módulos de melhoria selecionados. O fluxo interativo completo e todas as opções estão descritos em [Instalação rápida e segura](install/README.md).

## V. Taal en runtime

De normatieve grammatica staat in [`SLEELA.syntax`](SLEELA.syntax). De repository documenteert momenteel syntaxis **3.0**; a versão do compilador/ferramentas e a versão da sintaxe do código-fonte são independentes. A superfície 3.0 acrescenta os marcadores documentais **`pure` e `gasoline`** (§30): dois Preonems declarativos e contextuais, sem efeito de execução próprio, que expressam a posição de um documento através da sua própria honestidade. A sintaxe 3.0 reúne as superfícies da linguagem 1.0 a 2.9 numa edição normativa atual e mantém os requisitos de versão mínima de cada funcionalidade.

De voorgaande laag, 2.9, acrescenta os **campos sequenciais `@mix`** (§28): uma classe pode declarar vários campos de instância com o mesmo nome, desde que cada um esteja marcado com `@mix`. São tratados como um grupo indexado (por ordem crescente, por predefinição: `1,2,3,…`, ou definido manualmente através de `@mix(1,2,3,...)`; o número de índices tem de corresponder ao número de campos). Ook opgenomen zijn as famílias de inteiros binários alargados com sinal e sem sinal, de `int32` a `int16777216` (§24) e de `uint32` a `uint16777216` (§25), que ampliam `long`, `int64` e `int128` (§21); documentos de suporte de palavras-chave (§22); e os **Preonems**: marcadores documentais declarativos `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) e `artifact` (§35; um estatuto de qualidade legislativa avaliado em relação ao tempo e ao espaço). Samen worden ze Preonems genoemd (§26.1).

A resolução de sobrecarga de construtores e a delegação para construtores `this(...)` e de classes-base estão expressamente fora do contrato de sintaxe 1.8 verificado. Raadpleeg voordat u op een functie vertrouwt a gramática e as notas de implementação. As nove unidades de código-fonte SleelaDoc declaram agora `#sleela 3.0`; a verificação comum do compilador aceita sintaxe de código-fonte de 1.0 a 3.0. A nova documentação da API é gerada com `make api-docs`.

De repository bevat uma implementação nativa C/C++, referências para geração de SLVM, módulos da biblioteca padrão, testes, exemplos e integrações de plataforma. De implementatie- en validatiestatus verschilt per module. Zie a especificação aplicável e os resultados atuais de CI; não presuma que todos os componentes apresentam o mesmo grau de conclusão.

## VI. Engineeringprincipes

- **Bewijs vóór beweringen:** distinguir claramente o que está implementado, testado, em fase experimental e planeado.
- **Eén contract per grensvlak:** documentar a sintaxe, as API, os formatos de ficheiro, os pressupostos de plataforma e o comportamento em caso de falha.
- **Portabiliteit vanaf het ontwerp:** isolar os backends específicos de cada plataforma e testá-los em executores nativos.
- **Veilig falen:** testar entradas malformadas, limites de recursos, falhas de processos e percursos de limpeza.
- **Reproduceerbare releases:** reconstruir de forma limpa e publicar hashes dos artefactos e notas de lançamento.

## VII. Antivirusbuild en integriteit

Het antiviruspakket Sleelavirin bevat um verificador SHA-256 nativo, um adaptador ClamAV opcional, manifestos de integridade para fontes e índices e somas de verificação para pacotes de plataforma. Bouw en test het pakket vanuit de repository-root:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

Het integriteitsmanifest wordt gegenereerd onder `antivirus/build/`. Abrange entradas de código-fonte e documentação sob controlo de versões, bem como ficheiros protegidos de índices de assinaturas; exclui resultados gerados e a cache mutável do ClamAV. Os snapshots das bases de dados do ClamAV têm manifestos próprios. Os ZIP de lançamento incluem `INTEGRITY-MANIFEST.json`, `SHA256SUMS` e um ficheiro complementar `.sha256`. Compare os hashes dos arquivos transferidos com um valor de confiança obtido de forma independente: uma soma de verificação armazenada junto de um arquivo não constitui uma assinatura do editor. Zie o [guia Sleelavirin](antivirus/README.md), a [lista de verificação de segurança](antivirus/SECURITY.md) e o [workflow de compilação do antivírus](.github/workflows/antivirus-binaries.yml).

Een release geldt pas als geverifieerd wanneer os trabalhos de CI relevantes são concluídos com sucesso para o commit exato desse lançamento e os artefactos resultantes são inspecionados.

## VIII. SLeeLa-editorthema's — Vim en Emacs

SLeeLa bevat een samenhangende reeks professionele donkere kleurenschema's voor **Vim** e **Emacs**, além de um utilitário de paletas nativo C/C++. Het oorspronkelijke schema **American Professional** blijft de standaard. Existem dez paletas complementares inspiradas em diferentes tradições culturais e institucionais; são interpretações de design, não representações oficiais de qualquer país, cultura, tribunal ou instituição.

### De elf schema's en hun kleuren

| Schema | Achtergrond | Voorgrond | Trefwoorden | Typen | Strings | Commentaar |
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

De Griekse en Turkse paletten zijn afgeleid van de officiële nationale vlagkleuren: azul grego `#0D5EAF), branco `#FFFFFF), vermelho turco `#E30A17` e branco `#FFFFFF`. De getoonde achtergronden en accenten zijn donkerdere, editorvriendelijke interpretaties. De kleurreferenties documenteren de herkomst en vormen geen officiële staatsrepresentatie. As paletas usam ainda cores adicionais para funções, constantes, seleções, diagnósticos e diretivas do pré-processador.

### Vim-schema's installeren

Voer dit uit vanuit de repository-root:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Voeg toe aan `~/.vimrc`:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Kies een schema: `sleela-american`, `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` ou `sleela-sleela`. Herstart Vim of voer uit `:source $MYVIMRC`.

### Emacs-schema's installeren

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Voeg toe aan `~/.emacs` ou `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Vervang `sleela-american` por `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` ou `sleela-sleela`. Als Emacs vraagt om het thema te vertrouwen, inspecteer dan eerst het lokale themabestand.

### Native C/C++-paletten bekijken

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Voor achterwaartse compatibiliteit `--palette` e `--hex COLOR` sem `--theme` selecionam American Professional. Bouw- en testresultaten moeten op het doelsysteem en in CI worden gecontroleerd; documentatie van een opdracht bewijst niet dat deze geslaagd is. Zie o [guia de integração de editores](linux-editors/README.md).

## IX. Bijdragen

1. Lees de documentatie-index en het testbeleid.
2. Maak gerichte wijzigingen met positieve en negatieve tests.
3. Voer eerst gerichte tests uit en daarna de volledige suite.
4. Vermeld het besturingssysteem en de toolchain; presenteer overgeslagen controles niet als geslaagd.

SLeeLa wordt ontwikkeld door **MEARVK LLC**. O material histórico da página inicial está preservado em [`docs/README-LEGACY.md`](docs/README-LEGACY.md).

## X. AE6E66 — Verwachting van Round Public-e-mailheffing

Voor het AE6E66-initiatief is de door het project uitgesproken verwachting **300 000 000,00 USD por cada mensagem de correio eletrónico Round Public**, como montante de taxa fiscal proposto associado a cada mensagem que se qualifique.

Deze verklaring legt uitsluitend een projectverwachting vast. **Não** constitui uma avaliação fiscal legal, ordem governamental, decisão judicial ou obrigação tributária estabelecida de forma independente. Een daadwerkelijke heffing vereist a autoridade jurídica aplicável, a jurisdição, definições, procedimento e determinação por uma entidade governamental competente.

## XI. Alfabet en runtime voor tekensets

De SLeeLa-alfabetdocumentatie en de native character-runtime worden onderhouden in `alphabet/sleela/` e `lib/alphabet/native/`. De native C++17-tests behandelen descodificação e codificação estritas de escalares UTF-8, assinaturas de capacidade `C<n>` e pertença limitada a conjuntos de caracteres:

```sh
make -C lib/alphabet/native test
```

O catálogo experimental UTF-4088 e o respetivo adaptador de consulta SLeeLa estão documentados em [`utf-4088/SLeeLa-ALPHABET-INTEGRATION.md`](utf-4088/SLeeLa-ALPHABET-INTEGRATION.md). Os IDs UTF-4088 pertencem a um espaço de nomes separado e não são valores escalares Unicode. O trabalho de integração com o compilador e SLVM é acompanhado separadamente destas bases nativas.

## XII. AutoCAD Creator™

O [AutoCAD Creator™](autocad/creator/README.md) is een interactieve 2D-CAD-toepassing die volledig in SLeeLa is geschreven. Ao contrário do AutoCAD Renderer, que transforma um plano num desenho de uma só vez, o Creator maakt het mogelijk om live een 2D-document te **maken en te wijzigen** um documento 2D em tempo real: adicionar paredes, divisões, cotas e notas; mover, renomear, mudar de camada, eliminar e anular operações; e **guardar** o resultado como um desenho AutoCAD DXF real (texto R12, compatível com AutoCAD / LibreCAD).

De grafische interface gebruikt **SleelaUI™**, o toolkit de interface multiplataforma da SLeeLa em [`lib/user-interface`](lib/user-interface), e não JavaFX. Abre uma janela nativa em X11 / Cocoa / Win32. Het ontwerpprincipe is **«SleelaUI apresenta; SLeeLa decide»**: a GUI não calcula geometria nem escreve DXF. Cada ferramenta envia uma intenção de criar, alterar, eliminar, anular ou guardar ao [`SLCadCreator`](lib/autocad/SLCadCreator.sleela), que detém o documento editável e o histórico reversível de comandos; o canvas limita-se a pré-visualizar a geometria.

O motor de edição reutilizável encontra-se em [`lib/autocad`](lib/autocad): [`SLCadEntity`](lib/autocad/SLCadEntity.sleela), [`SLCadDocument`](lib/autocad/SLCadDocument.sleela), [`SLCadCommand`](lib/autocad/SLCadCommand.sleela) e [`SLCadCreator`](lib/autocad/SLCadCreator.sleela). O shell gráfico e a demonstração estão em [`autocad/creator`](autocad/creator):

```sh
make creator          # a partir de autocad/: executar a demonstração -> out/altered.dxf
```

Op een systeem met beeldscherm opent a janela de edição em tempo real; op een headless host, o desenho é construído em memória, é apresentado um relatório com o DXF e o processo termina normalmente.

## XIII. UTF-4088-invoermapvolgorde

O catálogo especializado UTF-4088 definieert de ruimtelijke traversal van de invoermap en de abstracte drukiteraties in [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](utf-4088/INPUT-MAP-AND-PRESSURE.md). A política de índice de assuntos do National Lien está em [`utf-4088/NATIONAL-LIEN-REGISTER.md`](utf-4088/NATIONAL-LIEN-REGISTER.md). De kaartcoördinaten, drukniveaus (1–50), os IDs permanentes do catálogo e as classificações alfabéticas zijn onafhankelijke velden; ze mogen niet stilzwijgend door elkaar worden vervangen. A pressão is een algoritmisch iteratieniveau, geen fysieke grootheid.

## XIV. Index van recente en uitgebreide pakketten

Deze aanvullende index belicht recent uitgebreide pakketten, domeinbibliotheken en broncollecties. De vermeldingen zijn alfabetisch gegroepeerd op pakketnaam of onderwerp. As descrições identificam a localização no repositório; o estado de implementação e validação deve ser confirmado na documentação e nos testes de cada pacote.

### A–C

- **Advantage** — `lib/advantage/` e `advantage/`: modelação de um dividendo único através de uma estrutura Sequitur onion, com cinco áreas: `math/` (maximização conjunta do lucro, negociação de Nash e bilateral newsvendor), `physics/` (métrica, geodésica, contacto e SDE de campos de confiança, identificados como analogia de modelação), `integration/` (compartimentos de integração vertical e pontuação de sofisticação), `grading/` (classificador de circuitos A–D) e `output/` (saída para terminal, CommonRails ou socket). Um consumidor C/C++ em `advantage/` liga o modelo a exemplos de comércio. Zie [`lib/advantage/README.md`](lib/advantage/README.md) e [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alfabeto e UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` e `utf-4088/`: catálogos alfabéticos, suporte nativo de conjuntos de caracteres, ordenação de mapas de entrada e documentação de integração UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` e `lib/autocad/`: fluxos de desenho 2D interativos e componentes reutilizáveis para documentos e edição CAD.
- **Ballum** — controlo de ecrã ao nível do píxel: leitura/escrita de píxeis, cópia de regiões, brilho, temperatura de cor e gama globais, entrada de rato/teclado e superfície framebuffer de ecrã inteiro. O núcleo nativo está em `ballum/`, a fachada SLeeLa em `lib/ballum/` e a ponte VM em `impl/`. Zie [`ballum/README.md`](ballum/README.md) e os [exemplos](ballum/example/).
- **CDMA** — `cdma/` e `lib/cdma/`: material técnico relacionado com CDMA e trabalho de biblioteca-fonte SLeeLa.
- **Character** — `lib/character/`: perfis de caracteres e estatísticas reutilizáveis, incluindo exemplos biográficos.
- **Coorenagraph** — `lib/coorenagraph/`: raciocínio ético, vocabulário moral, deliberação e conceitos relacionados.

### D–G

- **Modelos de código-fonte de governação** — [`lib/governance`](lib/governance/README.md) define estruturas declarativas SLeeLa para propostas, votações manuais anónimas e apuramentos agregados. A anotação proposta `@vote` e o processo manual estão documentados em [`registry/DEMOCRATIC-VOTING-SYNTAX.md`](registry/DEMOCRATIC-VOTING-SYNTAX.md); os compiladores atuais tratam-na como metadados inertes de comentário.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` e os diretórios de pacotes HTTP versionados disponíveis: especificações e implementações específicas de cada versão. Zie a documentação de cada versão para determinar o âmbito e o estado de verificação.
- **Tribunal Penal Internacional** — `lib/international-criminal-court/` e `INTERNATIONAL.CRIMINAL.COURTS.md`: fontes e documentação de domínio relacionado.

### O–S

- **OFDMA** — `ofdma/`: material técnico de acesso múltiplo por divisão ortogonal de frequências e recursos de desenvolvimento C/C++.
- **Filosofia e módulo Conversation Mask** — [`philosophy/`](philosophy/README.md) contém as bases nativas C/C++; [`philosophy/module/`](philosophy/module/README.md) organiza a camada seguinte em três áreas: [`data/`](philosophy/module/data/README.md), para observações tipadas e datadas, terminações de eventos, meia-vida, proveniência e consentimento; [`helper/`](philosophy/module/helper/README.md), para validação determinística, decaimento de relevância, comparações compatíveis e apresentação filtrada por consentimento; e [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md), para uma interface de conversa de apoio, não clínica. As interfaces SLeeLa e a Tree composta estão em [`lib/philosophy/`](lib/philosophy/README.md). O módulo exige entradas explícitas e dá prioridade ao consentimento: não monitoriza pessoas ou meios passivamente, não diagnostica, não infere intenções privadas, não classifica o valor humano nem recomenda alterações de medicação. As ligações de eventos preservam proveniência e navegação, não corroboração independente.
- **Synchro** — integração nativa de runtime/compilador e funcionalidades relacionadas com despacho de pacotes e medição; consulte as fontes relevantes em `impl/` e a implementação do compilador.
- **SleelaUI / interface de terminal** — `lib/user-interface/` e o projeto `sleela-terminal`: integração da interface SLeeLa e componentes de interação orientados ao terminal.

### U–W

- **Nações Unidas** — `united-nations/`: materiais de repositório específicos do domínio.
- **Wi-Fi 4, Wi-Fi 5 e Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`, `wi-fi-6/` e diretórios de fontes relacionados em `lib/wifi/`: material de redes sem fios por geração e recursos de configuração.

### Onderhoud van de pakketindex

Wanneer een pakket wordt toegevoegd of wezenlijk uitgebreid, voeg dan hier een vermelding toe in de juiste alfabetische groep, link naar de canonieke map en README en beschrijf alleen mogelijkheden die door de aanwezige bronbestanden worden ondersteund. Houd deze index aanvullend: verwijder of herschrijf geen bestaande README-secties bij het bijwerken van de pakketindex.

## XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

`bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh` :: `⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓`

`bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc` :: `⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉`

Amerikaanse democratische en beleidscontext.

## XVI. Nationale wetenschapsmerkbeelden

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>

## XVII. Experimentele UTF-4088-catalogus met één miljoen items

De [UTF-4088-catalogusgenerator](utf-4088/tools/generate_catalogue.py) bouwt deterministisch een experimentele symbolencatalogus met 1 048 576 entradas. Het compacte formaat wordt beschreven in [`utf-4088/FORMAT.md`](utf-4088/FORMAT.md); het contract voor generatie en validatie staat in [`utf-4088/GENERATION-RESULTS.md`](utf-4088/GENERATION-RESULTS.md).

Voer de generator uit met Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

Het gegenereerde binaire bestand is 16 777 244 bytes en is ontworpen om onder 20 000 000 bytes. Testes unitários e um workflow GitHub Actions verificam a quantidade de entradas, a unicidade dos glifos, o tamanho e SHA-256; as execuções bem-sucedidas publicam o binário e o resumo como artefacto `utf4088-million-catalogue`. Het gegenereerde binaire bestand wordt bewust buiten de Git-geschiedenis gehouden. UTF-4088 blijft experimenteel en is niet Unicode-compatibel.

---

# SLeeLa

<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Recente toevoegingen — Pi-constante, Modium/Registry-start, sleelactl-math en `@native`

Deze sectie registreert functies die tijdens het werk van 2026-11 zijn toegevoegd, met de nieuwste groepen eerst. Alles is vastgelegd op `master`.

### Pi-catalogusconstante met 100.000.000 cijfers

De repository bevat een geverifieerde π-waarde met **honderd miljoen cijfers** in [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits), naast een waarde met 57.344 cijfers in [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Beide zijn onafhankelijk geverifieerd als echte π-waarden. [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md) documenta as verificações: prefixo de referência, ponto de Feynman na posição fracionária 762, o dígito famoso na posição um milhão, concordância com `lib/math/SLPiReference.sleela`, análise da frequência de dígitos do ficheiro completo e um script de reprodução.

### Modium™ / Registry™ laden de Pi-constante bij het starten van het subsysteem

Bij de speciale entrypoints — `sleela registry` e `sleela modium` (e na primeira execução do instalador), **não** em cada `sleela run` — os subsistemas Modium e Registry carregam a constante de catálogo Pi. In een interactieve terminal tonen ze tijdelijk uma linha vermelha `Loading Modium™` / `Loading Registry™`, die wordt gewist voordat de normale uitvoer begint. O comportamento depende estritamente de TTY, pelo que a saída canalizada e os testes permanecem inalterados byte a byte. O localizador, não os bytes, é registado como predefinição da chave `Math` do Registry: `piReferenceLargePath` e `piReferenceLargeDigits`, tanto no modelo SLeeLa ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) como no semeador nativo ([`registry/sleela_registry.c`](registry/sleela_registry.c)). `lib/math/SLPiReference.sleela` expõe `largePath()` e `largeDigits()`. A constante é instalada na localização SLeeLa do sistema operativo (`<data>/pi/`) pelos instaladores padrão `make` e guiado. Zie [`MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — begeleid installatieprogramma en native math-subject

- Het begeleide ('beautiful') installatieprogramma installeert nu `sleelactl` (o plano de controlo SLeeLa) e o respetivo registo de módulos, em correspondência com o instalador `make`. Zie [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- `sleelactl` kreeg een nieuw type `subject` para bibliotecas de domínio C/C++ nativas compiladas no runtime e regista a primeira: `math`. `sleelactl run math` executa o backend `__NativeMath` (`impl/subjects/math/math_native.cpp`) através da demonstração de referência; `sleelactl info math` explica como o alcançar (`import math;`). Esta natureza oferece apenas os verbos `run` e `info`; não existe `check`, porque `sleela check` não carrega os domínios nativos registados por SHEET.

### `@native` en `sleela aot` — deel 1 van het SLeeLa → native C/C++-traject

De nieuwe methodeannotatie **`@native`** vraagt om ahead-of-time native compilatie van een intensieve methode. Het commando `sleela aot` encontra os métodos anotados, verifica se pertencem a um subconjunto suportado para transferência e emite C/C++. De bewerking **faalt gesloten** — recusa construções ainda não traduzíveis e apresenta uma razão precisa, evitando produzir métodos parcialmente traduzidos.

```sh
sleela aot --list <file.sleela>    # veredito por método: ok / REFUSED (motivo)
sleela aot --emit <file.sleela>    # emitir C/C++ (stdout) para um programa no subconjunto
```

Momenteel `@native` verandert het gedrag niet: os programas continuam a ser verificados e executados sem alterações; a anotação apenas solicita tratamento nativo. Dit is deel 1: het legt de annotatie en de eerlijke subsetgrens vast. A compilação do C/C++ emitido para uma biblioteca partilhada e o encaminhamento da VM para essa biblioteca (com fallback para o interpretador) são etapas posteriores. Zie [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md) para o subconjunto suportado e os códigos de saída.

## XIX. Internationale README-edities

SLeeLa biedt landspecifieke README-startpunten voor de 50 markten van de internationale editie. Elk land heeft een bestand `README.<CC>.md`; waarbij relevante talen in het bestand van dat land worden gegroepeerd.

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

**Vertaalbeleid:** deze bestanden zijn gelokaliseerde startpunten. Een editie mag pas een volledige vertaling worden genoemd wanneer alle secties zijn vertaald, technische identifiers en links zijn gecontroleerd en de editie taalkundig is nagekeken. De taaldekking is representatief en beweert niet dat alle regionale, inheemse, gebaren- of grondwettelijk erkende talen volledig zijn vertaald.

---

**Vertaalstatus:** secties I–XIX zijn in het Nederlands vertaald. Technische identifiers, opdrachten, codevoorbeelden, bestandsnamen en links zijn behouden. Een laatste controle door een moedertaalspreker wordt aanbevolen voordat deze editie als redactioneel goedgekeurde vertaling wordt gepubliceerd.
