# SLeeLa 3.0 — Portugal (PT)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>

<p align="center"><strong>Uma linguagem nativa de sistemas, com compilador, ambiente de execução e ecossistema de máquinas virtuais.</strong></p>

> **Edição internacional por país.** Este ficheiro reúne as versões linguísticas previstas para Portugal. A tradução técnica é realizada por etapas. As secções já traduzidas são reproduzidas integralmente; as secções ainda por traduzir são identificadas de forma explícita.

**The Now. The Ingredient.** Uma linguagem de programação é mais do que a sua sintaxe: inclui o compilador, o ambiente de execução, as bibliotecas, as ferramentas e os contratos de plataforma que tornam o software utilizável.

**Áreas de trabalho da SLeeLa.** A SLeeLa aborda **Math**, **Object**, **Science** e **Reach**, as quatro dimensões do seu trabalho: aquilo que é calculável (Math), o objeto estabelecido no catálogo daquilo que existe (Object), a investigação disciplinada da verdade (Science) e a extensão da capacidade de um sistema a outros sistemas e pessoas (Reach). **Os interesses nacionais mantêm-se**; nenhuma destas quatro dimensões os substitui. Quando um tema afeta os interesses de uma nação, essa relação é preservada e documentada com rigor, por exemplo, nos campos de metadados de uma classe `metaOrigin` e `metaNationalSchedule`.

## I. Começar aqui

- [Compilar a implementação nativa](impl/README.md)
- [Máquinas virtuais e Modium — guia prático do utilizador](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [Pilha de CPU no código-fonte da SLeeLa](lib/cpu/CPU.md): uma cadeia de máquina completa, desde as portas lógicas até à ALU, aos núcleos, ao sistema operativo e aos programas; inclui mais de 130 perfis de CPU e [tensões, frequências variáveis e parâmetros avançados de temporização](lib/cpu/TIMING.md), com execução [dentro das especificações ou em overclock](lib/cpu/components/README.md).
- [Índice da documentação](docs/README.md)
- [Fonte de dados CSPML e referência de investigação Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md): proveniência e orientação de integração para dados externos dos EUA sobre locais, condições socioeconómicas, força de trabalho, educação e escolas públicas.
- [Documentação de API gerada](docs/api/README.md): referência HTML atual, edição versionada da API e instruções para a regenerar.
- [Catálogo de API](api/README.md)
- [Normas de comparação computacional](comparison/README.md): normas de tipos, memória, ABI e conformidade para a SLeeLa VM, Java/JVM e C/C++ nativo.
- [Stanchion de recursão — protótipos C e C++](recursion/stanchion/README.md): exemplos especializados nos tipos primitivos Boolean, Byte, Character, Integer, Long, Float e Double, bem como padrões de recursão genéricos, aplicados, científicos e educativos.
- [Matriz de suporte e validação de plataformas](docs/PLATFORM-SUPPORT.md)
- [Guia de marca](docs/BRAND-GUIDE.md)
- [Conceitos e recursos gráficos de merchandising](merchandise/README.md)
- [Logótipo NitroWebExpress](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Política de testes e critérios de lançamento](markdown/TESTING.md)

## II. Novidades

Adições recentes à linguagem, aos controladores e à biblioteca padrão:

- **Pacote de biblioteca padrão `advantage`.** [`lib/advantage/`](lib/advantage/README.md) modela a obtenção de um dividendo único relativamente a um modelo, através de uma estrutura em camadas denominada **Sequitur onion** (dividendo → revisão → preocupações → matemática → adições → subtração → reagrupamento → iteração). Inclui os domínios `math/` (economia de comércio bilateral, maximização conjunta do lucro, negociação de Nash e rácio crítico newsvendor), `physics/` (analogia de modelação estrutural-económica dos campos de confiança), `integration/` (integração vertical recursiva), `grading/` (classificação de sofisticação A–D) e `output/` (saída encaminhável para terminal, CommonRails ou socket/conector). Inclui exemplos executáveis e documentação de definições, matemática e física. O consumidor C/C++ em [`advantage/`](advantage/README.md) integra o modelo em exemplos de comércio.
- **Anotações de temporização de ciclos — `@loopDelay` / `@loopBudget`.** Estas anotações comportamentais, colocadas antes de ciclos `for`, `while` ou `do`, introduzem pausas reais na execução. `@loopDelay(ms = 250)` define uma pausa por iteração; `@loopBudget(seconds = 10, iterations = 1000)` distribui um número conhecido de iterações por um orçamento de tempo total. Ambas utilizam o opcode aditivo `OP_SLEEP` (ISA 157), que também suporta a função integrada `sleep(<ms>)`. A implementação usa `nanosleep` / `Sleep`. Consulte [`SLEELA.syntax` §32](SLEELA.syntax), a [referência da API](api/sleela/3.0/LOOP_TIMING_API.md) e o [exemplo executável](examples/loop-timing-annotation-demo.sleela).
- **SleelaDoc na linha de comandos — `sleela docs`.** O comando lista tópicos documentáveis, apresenta pacotes e resolve classes, incluindo nomes sem distinção entre maiúsculas e minúsculas e o prefixo padrão `SL`. As páginas incluem definição, objetivo de conceção, relações, estrutura, conceitos de utilização e uma escala de domínio de cinco níveis. As etiquetas `@intent`, `@thesis`, `@axiom`, `@param`, `@see` e `@mastery` enriquecem a documentação. `sleela docs --generate [dir]` cria um corpus de texto para consulta offline; `make sleeladoc` reúne o corpus e as fontes correspondentes em `sleeladoc/build/sleeladoc.jar`. Consulte [`lib/sleeladoc/`](lib/sleeladoc/README.md) e [`sleela(1)`](man/man1/sleela.1).
- **SleelaDoc e documentação HTML.** O extrator [`lib/sleeladoc`](lib/sleeladoc/README.md) transforma comentários documentais de unidades `.sleela` em páginas Markdown ou HTML. O sítio navegável de toda a biblioteca `/lib` é gerado em [`sleeladoc-api/`](sleeladoc-api/README.md). A compilação pode também emitir páginas MediaWiki através de `--wiki`. Consulte [SLEELADOC](manifesto/SLEELADOC.md), [SLEELADOC.WIKI](manifesto/SLEELADOC.WIKI.md) e o [exemplo](lib/sleeladoc/sleeladoc-demo.sleela).
- **Modelo de objetos de documento XML (`lib/xml`).** A biblioteca inclui uma árvore DOM com nós de elemento, texto, comentário, CDATA e instrução de processamento, atributos nomeados, serialização compacta/formatada, escape XML 1.0 §2.4, análise por células de caracteres e registo de qualidade `SLXmlAspect`. A igualdade entre nós é reflexiva, simétrica e determinística, com níveis de identidade, assunto e estrutura. Consulte [`lib/xml/README.md`](lib/xml/README.md) e o [exemplo executável](lib/xml/xml-dom.sleela).
- **Primitivos inteiros alargados.** `int`, `long` e `int64` são inteiros assinados de 64 bits; `int128` é atualmente suportado por uma representação de 64 bits, estando o suporte real de 128 bits documentado como trabalho futuro. As famílias binárias assinadas `int32`–`int16777216` (sintaxe 2.7) e não assinadas `uint32`–`uint16777216` (sintaxe 2.8) ampliam a família. Consulte [`SLEELA.syntax`](SLEELA.syntax) §§21, 24 e 25.
- **Campos consequentes `@mix` (sintaxe 2.9).** Uma classe pode declarar campos de instância repetidos com o mesmo nome se cada declaração tiver `@mix`. O grupo é indexado por ordem crescente (`1,2,3,…`) ou por índices explícitos em `@mix(1,2,3,...)`; a quantidade de índices tem de corresponder à quantidade de campos. Consulte [§28](SLEELA.syntax).
- **Preonems — marcadores documentais do Processo Natural.** Os marcadores declarativos incluem `author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`, `pure`, `gasoline` e `artifact`. São contextuais, dependentes da versão e não alteram a análise sintática, a tipagem ou a geração de código; fora da posição de marcador continuam a ser identificadores normais. Consulte [`SLEELA.syntax`](SLEELA.syntax) §§18–20, 23, 26, 30 e 35.
- **Nordshrift com acesso integral à biblioteca.** Na sintaxe 3.0, Nordshrift utiliza o mesmo índice recursivo de `/lib` que o compilador, abrangendo pacotes e unidades `.sleela`; `nordshrift check` apresenta as contagens da árvore completa.
- **Stanchion de recursão.** [`recursion/stanchion/`](recursion/stanchion/README.md) contém protótipos de referência C e C++ para sete contratos primitivos documentados, além de exemplos genéricos, aplicados, científicos e pedagógicos. As fontes de referência devem ser compiladas e testadas antes de serem consideradas verificadas.
- **Comentários angulares (`<` / `<<`).** O frontend reconhece comentários de linha `< line >` e de bloco `<< block >>`; consulte [§1.1](SLEELA.syntax).
- **Literais inteiros por radix.** Os literais `int` aceitam hexadecimal (`0x`), binário (`0b`) e octal (`0o`), com separadores `_`, tanto na linguagem como no formato de modelos CPU [`.xlt`](lib/xlt/README.md). Consulte [§1.4](SLEELA.syntax).
- **Norma Metadata Class.** O padrão de metadados descritivos — proveniência, legalidade e âmbito jurisdicional — é aplicado aos ficheiros `.sleela` que contêm classes em `/lib`. Consulte [§15](SLEELA.syntax) e a [lição 13](tutorial/13-metadata-class.md).
- **Família de texto e String.** `lib/text` fornece operações de caracteres e cadeias, incluindo `charAt`, `substring`, `indexOf`, `reverse`, alinhamento de colunas e modelos posicionais. Consulte o [tutorial](tutorial/14-text-and-strings.md) e o [exemplo](lib/text/text-string-demo.sleela).
- **String em folhas Nordshrift.** Uma folha `.sst` pode declarar um bloco tipado `strings:` que contribui para o manifesto de componentes da compilação. Consulte a [referência Nordshrift](wiki/Nordshrift-Sheet-Reference.md).
- **Modelos CPU por opcodes e equivalência de chamadas.** Cada `SL<ARCH>CPU` suporta a execução de um fluxo nativo de instruções através do ciclo fetch-decode-execute e a execução equivalente de código de outro processo sobre a infraestrutura VM partilhada. Consulte [`lib/cpu/EXECUTION.md` §5](lib/cpu/EXECUTION.md).

## III. Compilação e testes

**Pré-requisitos:** uma cadeia de ferramentas C/C++ (GCC ou Clang) e GNU Make; Python 3 para ferramentas de manifesto e fluxo de trabalho; em Linux, também os pacotes de desenvolvimento X11/Xrandr para produtos de visualização. O comando `make deps` de cada módulo indica ou instala as dependências necessárias. A compilação em `impl/` falha de forma segura se o manifesto SHA-256 de confiança não for satisfeito; o Makefile da raiz inclui-o automaticamente.

A forma mais rápida de começar a partir de um clone limpo:

```sh
make                                  # compilar tudo a partir da raiz do repositório
```

Ou compilar e testar apenas o núcleo nativo:

```sh
make -C impl all                      # compilar apenas o núcleo do compilador e do ambiente de execução
bash ./test-suites/run-all.sh --all   # executar o conjunto completo de testes
```

O comando `make` na raiz compila todos os produtos (núcleo, VM, codecs, churn, antivírus, etc.) e termina com a criação do **registo** e a materialização da sua tabela-base. Assim, uma árvore de trabalho compilada contém desde logo um registo verificável.

Objetivos principais:

```sh
make                     # compilar tudo (alias de make all; registo criado no fim)
make all                 # a mesma compilação completa, explicitamente indicada
sudo make install        # instalar comandos e ficheiros de suporte do ambiente de execução (ver § IV)
make clean               # remover resultados de compilação de todos os sistemas integrados
make help                # apresentar todos os objetivos do Makefile da raiz
```

Outros objetivos úteis:

```sh
make registry            # compilar apenas o registo e materializar a tabela
./build-registry.sh      # a mesma operação através de um script direto
make tests               # compilar e executar o conjunto de testes
make ballum              # compilar a biblioteca de visualização Ballum para este sistema operativo
bin/install-modules      # instalar/compilar módulos de forma faseada durante cerca de 10–20 minutos
```

O objetivo `registry` executa [`build-registry.sh`](build-registry.sh): compila o núcleo do registo, coloca a tabela-base no armazenamento de texto portátil `registry/build/sleela.sleelareg` e exporta o script da tabela MySQL `registry/build/registry.sql` (através de `sleela registry export`). O registo pode ser inspecionado com `sleela registry` ou `registry/build/sleela-registry review`. Consulte o [guia do registo](registry/README.md) e a [arquitetura Registry 3.0](registry/REGISTRY-3.0-ARCHITECTURE.md).

Consulte os guias de compilação e testes para conhecer os pré-requisitos e os passos específicos de cada plataforma. A compilação bem-sucedida de um componente não demonstra que todas as bibliotecas ou plataformas estejam prontas para lançamento.

## IV. Instalação

Depois de compilar os produtos nativos, instale os comandos e os ficheiros de suporte do ambiente de execução:

```sh
sudo make install
make install PREFIX="$HOME/.local"        # instalação apenas para o utilizador, sem sudo
make install DESTDIR="$PWD/package-root" # instalação num diretório de preparação de pacotes
make install-check
```

O instalador convencional utiliza, por predefinição, `/usr/local/bin`, `/usr/local/lib/sleela` e `/usr/local/share/sleela`. Preserva uma configuração de arranque existente e não substitui comandos preexistentes que não estejam sob controlo de versões. A desinstalação remove apenas os ficheiros registados pelo instalador; a configuração e os dados de execução não registados são preservados.

Para uma implementação orientada e específica apenas do pacote principal — os binários `sleela`, `sleelvac` e `nordshrift`, a biblioteca padrão, os protocolos e os módulos de suporte — utilize o instalador limpo:

```sh
make clean-install       # instalação orientada apenas do pacote principal
```

Primeiro, é perguntado se o pacote principal deve ser instalado; em seguida, escolhe-se o âmbito da instalação. Se a operação for bem-sucedida, pode ser apresentado um seletor opcional de módulos de melhoria selecionados. O fluxo interativo completo e todas as opções estão descritos em [Instalação rápida e segura](install/README.md).

## V. Linguagem e ambiente de execução

A gramática normativa encontra-se em [`SLEELA.syntax`](SLEELA.syntax). O repositório documenta atualmente a sintaxe **3.0**; a versão do compilador/ferramentas e a versão da sintaxe do código-fonte são independentes. A superfície 3.0 acrescenta os marcadores documentais **`pure` e `gasoline`** (§30): dois Preonems declarativos e contextuais, sem efeito de execução próprio, que expressam a posição de um documento através da sua própria honestidade. A sintaxe 3.0 reúne as superfícies da linguagem 1.0 a 2.9 numa edição normativa atual e mantém os requisitos de versão mínima de cada funcionalidade.

A superfície anterior, 2.9, acrescenta os **campos sequenciais `@mix`** (§28): uma classe pode declarar vários campos de instância com o mesmo nome, desde que cada um esteja marcado com `@mix`. São tratados como um grupo indexado (por ordem crescente, por predefinição: `1,2,3,…`, ou definido manualmente através de `@mix(1,2,3,...)`; o número de índices tem de corresponder ao número de campos). Inclui também as famílias de inteiros binários alargados com sinal e sem sinal, de `int32` a `int16777216` (§24) e de `uint32` a `uint16777216` (§25), que ampliam `long`, `int64` e `int128` (§21); documentos de suporte de palavras-chave (§22); e os **Preonems**: marcadores documentais declarativos `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) e `artifact` (§35; um estatuto de qualidade legislativa avaliado em relação ao tempo e ao espaço). Em conjunto, são designados Preonems (§26.1).

A resolução de sobrecarga de construtores e a delegação para construtores `this(...)` e de classes-base estão expressamente fora do contrato de sintaxe 1.8 verificado. Antes de depender de uma funcionalidade, consulte a gramática e as notas de implementação. As nove unidades de código-fonte SleelaDoc declaram agora `#sleela 3.0`; a verificação comum do compilador aceita sintaxe de código-fonte de 1.0 a 3.0. A nova documentação da API é gerada com `make api-docs`.

O repositório contém uma implementação nativa C/C++, referências para geração de SLVM, módulos da biblioteca padrão, testes, exemplos e integrações de plataforma. O estado de implementação e validação varia consoante o módulo. Consulte a especificação aplicável e os resultados atuais de CI; não presuma que todos os componentes apresentam o mesmo grau de conclusão.

## VI. Princípios de engenharia

- **Evidência antes de afirmações:** distinguir claramente o que está implementado, testado, em fase experimental e planeado.
- **Um contrato por fronteira:** documentar a sintaxe, as API, os formatos de ficheiro, os pressupostos de plataforma e o comportamento em caso de falha.
- **Portabilidade desde a conceção:** isolar os backends específicos de cada plataforma e testá-los em executores nativos.
- **Falha segura:** testar entradas malformadas, limites de recursos, falhas de processos e percursos de limpeza.
- **Lançamentos reproduzíveis:** reconstruir de forma limpa e publicar hashes dos artefactos e notas de lançamento.

## VII. Compilação do antivírus e integridade

O pacote antivírus Sleelavirin inclui um verificador SHA-256 nativo, um adaptador ClamAV opcional, manifestos de integridade para fontes e índices e somas de verificação para pacotes de plataforma. Compile e teste o pacote a partir da raiz do repositório:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

O manifesto de integridade é gerado em `antivirus/build/`. Abrange entradas de código-fonte e documentação sob controlo de versões, bem como ficheiros protegidos de índices de assinaturas; exclui resultados gerados e a cache mutável do ClamAV. Os snapshots das bases de dados do ClamAV têm manifestos próprios. Os ZIP de lançamento incluem `INTEGRITY-MANIFEST.json`, `SHA256SUMS` e um ficheiro complementar `.sha256`. Compare os hashes dos arquivos transferidos com um valor de confiança obtido de forma independente: uma soma de verificação armazenada junto de um arquivo não constitui uma assinatura do editor. Consulte o [guia Sleelavirin](antivirus/README.md), a [lista de verificação de segurança](antivirus/SECURITY.md) e o [workflow de compilação do antivírus](.github/workflows/antivirus-binaries.yml).

Um lançamento só é considerado verificado quando os trabalhos de CI relevantes são concluídos com sucesso para o commit exato desse lançamento e os artefactos resultantes são inspecionados.

## VIII. Temas do editor SLeeLa — Vim e Emacs

A SLeeLa inclui um conjunto coordenado de esquemas de cores escuros e profissionais para **Vim** e **Emacs**, além de um utilitário de paletas nativo C/C++. O esquema original **American Professional** continua a ser o predefinido. Existem dez paletas complementares inspiradas em diferentes tradições culturais e institucionais; são interpretações de design, não representações oficiais de qualquer país, cultura, tribunal ou instituição.

### Os onze esquemas e as respetivas cores

| Esquema | Fundo | Primeiro plano | Palavras-chave | Tipos | Cadeias | Comentários |
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

As paletas grega e turca derivam das cores oficiais das bandeiras nacionais: azul grego `#0D5EAF), branco `#FFFFFF), vermelho turco `#E30A17` e branco `#FFFFFF`. Os fundos e acentos apresentados são interpretações escurecidas e adequadas a editores. As referências de cor documentam a proveniência e não constituem representação oficial dos Estados. As paletas usam ainda cores adicionais para funções, constantes, seleções, diagnósticos e diretivas do pré-processador.

### Instalar os esquemas Vim

A partir da raiz do repositório:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Adicione a `~/.vimrc`:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Escolha um esquema: `sleela-american`, `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` ou `sleela-sleela`. Reinicie o Vim ou execute `:source $MYVIMRC`.

### Instalar os esquemas Emacs

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Adicione a `~/.emacs` ou `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Substitua `sleela-american` por `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` ou `sleela-sleela`. Se o Emacs pedir autorização para confiar no tema, inspecione primeiro o ficheiro local.

### Pré-visualizar as paletas nativas C/C++

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Por compatibilidade, `--palette` e `--hex COLOR` sem `--theme` selecionam American Professional. Os resultados de compilação e testes devem ser verificados no sistema de destino e em CI; a documentação de um comando não prova, por si só, que tenha passado. Consulte o [guia de integração de editores](linux-editors/README.md).

## IX. Contribuir

1. Leia o índice da documentação e a política de testes.
2. Faça alterações focadas, com testes positivos e negativos.
3. Execute primeiro os testes específicos e depois o conjunto abrangente.
4. Indique o sistema operativo e a cadeia de ferramentas; não descreva verificações ignoradas como aprovadas.

A SLeeLa é desenvolvida pela **MEARVK LLC**. O material histórico da página inicial está preservado em [`docs/README-LEGACY.md`](docs/README-LEGACY.md).

## X. AE6E66 — Expectativa de taxa sobre correio eletrónico público Round

Para a iniciativa AE6E66, a expectativa declarada pelo projeto é de **300 000 000,00 USD por cada mensagem de correio eletrónico Round Public**, como montante de taxa fiscal proposto associado a cada mensagem que se qualifique.

Esta declaração regista apenas uma expectativa do projeto. **Não** constitui uma avaliação fiscal legal, ordem governamental, decisão judicial ou obrigação tributária estabelecida de forma independente. Qualquer taxa efetiva exigiria a autoridade jurídica aplicável, a jurisdição, definições, procedimento e determinação por uma entidade governamental competente.

## XI. Alfabeto e ambiente de execução de conjuntos de caracteres

A documentação do alfabeto SLeeLa e o ambiente de execução nativo de caracteres são mantidos em `alphabet/sleela/` e `lib/alphabet/native/`. Os testes nativos C++17 abrangem descodificação e codificação estritas de escalares UTF-8, assinaturas de capacidade `C<n>` e pertença limitada a conjuntos de caracteres:

```sh
make -C lib/alphabet/native test
```

O catálogo experimental UTF-4088 e o respetivo adaptador de consulta SLeeLa estão documentados em [`utf-4088/SLeeLa-ALPHABET-INTEGRATION.md`](utf-4088/SLeeLa-ALPHABET-INTEGRATION.md). Os IDs UTF-4088 pertencem a um espaço de nomes separado e não são valores escalares Unicode. O trabalho de integração com o compilador e SLVM é acompanhado separadamente destas bases nativas.

## XII. AutoCAD Creator™

O [AutoCAD Creator™](autocad/creator/README.md) é uma aplicação CAD 2D interativa, escrita integralmente em SLeeLa. Ao contrário do AutoCAD Renderer, que transforma um plano num desenho de uma só vez, o Creator permite **criar e alterar** um documento 2D em tempo real: adicionar paredes, divisões, cotas e notas; mover, renomear, mudar de camada, eliminar e anular operações; e **guardar** o resultado como um desenho AutoCAD DXF real (texto R12, compatível com AutoCAD / LibreCAD).

A interface gráfica utiliza **SleelaUI™**, o toolkit de interface multiplataforma da SLeeLa em [`lib/user-interface`](lib/user-interface), e não JavaFX. Abre uma janela nativa em X11 / Cocoa / Win32. O princípio de conceção é **«SleelaUI apresenta; SLeeLa decide»**: a GUI não calcula geometria nem escreve DXF. Cada ferramenta envia uma intenção de criar, alterar, eliminar, anular ou guardar ao [`SLCadCreator`](lib/autocad/SLCadCreator.sleela), que detém o documento editável e o histórico reversível de comandos; o canvas limita-se a pré-visualizar a geometria.

O motor de edição reutilizável encontra-se em [`lib/autocad`](lib/autocad): [`SLCadEntity`](lib/autocad/SLCadEntity.sleela), [`SLCadDocument`](lib/autocad/SLCadDocument.sleela), [`SLCadCommand`](lib/autocad/SLCadCommand.sleela) e [`SLCadCreator`](lib/autocad/SLCadCreator.sleela). O shell gráfico e a demonstração estão em [`autocad/creator`](autocad/creator):

```sh
make creator          # a partir de autocad/: executar a demonstração -> out/altered.dxf
```

Num sistema com ecrã, abre-se a janela de edição em tempo real; num anfitrião sem interface gráfica, o desenho é construído em memória, é apresentado um relatório com o DXF e o processo termina normalmente.

## XIII. Ordenação do mapa de entrada UTF-4088

O catálogo especializado UTF-4088 define o percurso espacial do mapa de entrada e as iterações abstratas de pressão em [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](utf-4088/INPUT-MAP-AND-PRESSURE.md). A política de índice de assuntos do National Lien está em [`utf-4088/NATIONAL-LIEN-REGISTER.md`](utf-4088/NATIONAL-LIEN-REGISTER.md). As coordenadas do mapa, os níveis de pressão (1–50), os IDs permanentes do catálogo e as classificações alfabéticas são campos independentes; nenhum deve ser substituído silenciosamente por outro. A pressão é um nível de iteração algorítmica, não uma grandeza física.

## XIV. Índice de pacotes recentes e ampliados

Este índice adicional destaca pacotes, bibliotecas de domínio e coleções de fontes recentemente ampliados. As entradas estão agrupadas alfabeticamente por nome de pacote ou tema. As descrições identificam a localização no repositório; o estado de implementação e validação deve ser confirmado na documentação e nos testes de cada pacote.

### A–C

- **Advantage** — `lib/advantage/` e `advantage/`: modelação de um dividendo único através de uma estrutura Sequitur onion, com cinco áreas: `math/` (maximização conjunta do lucro, negociação de Nash e bilateral newsvendor), `physics/` (métrica, geodésica, contacto e SDE de campos de confiança, identificados como analogia de modelação), `integration/` (compartimentos de integração vertical e pontuação de sofisticação), `grading/` (classificador de circuitos A–D) e `output/` (saída para terminal, CommonRails ou socket). Um consumidor C/C++ em `advantage/` liga o modelo a exemplos de comércio. Consulte [`lib/advantage/README.md`](lib/advantage/README.md) e [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alfabeto e UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` e `utf-4088/`: catálogos alfabéticos, suporte nativo de conjuntos de caracteres, ordenação de mapas de entrada e documentação de integração UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` e `lib/autocad/`: fluxos de desenho 2D interativos e componentes reutilizáveis para documentos e edição CAD.
- **Ballum** — controlo de ecrã ao nível do píxel: leitura/escrita de píxeis, cópia de regiões, brilho, temperatura de cor e gama globais, entrada de rato/teclado e superfície framebuffer de ecrã inteiro. O núcleo nativo está em `ballum/`, a fachada SLeeLa em `lib/ballum/` e a ponte VM em `impl/`. Consulte [`ballum/README.md`](ballum/README.md) e os [exemplos](ballum/example/).
- **CDMA** — `cdma/` e `lib/cdma/`: material técnico relacionado com CDMA e trabalho de biblioteca-fonte SLeeLa.
- **Character** — `lib/character/`: perfis de caracteres e estatísticas reutilizáveis, incluindo exemplos biográficos.
- **Coorenagraph** — `lib/coorenagraph/`: raciocínio ético, vocabulário moral, deliberação e conceitos relacionados.

### D–G

- **Modelos de código-fonte de governação** — [`lib/governance`](lib/governance/README.md) define estruturas declarativas SLeeLa para propostas, votações manuais anónimas e apuramentos agregados. A anotação proposta `@vote` e o processo manual estão documentados em [`registry/DEMOCRATIC-VOTING-SYNTAX.md`](registry/DEMOCRATIC-VOTING-SYNTAX.md); os compiladores atuais tratam-na como metadados inertes de comentário.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` e os diretórios de pacotes HTTP versionados disponíveis: especificações e implementações específicas de cada versão. Consulte a documentação de cada versão para determinar o âmbito e o estado de verificação.
- **Tribunal Penal Internacional** — `lib/international-criminal-court/` e `INTERNATIONAL.CRIMINAL.COURTS.md`: fontes e documentação de domínio relacionado.

### O–S

- **OFDMA** — `ofdma/`: material técnico de acesso múltiplo por divisão ortogonal de frequências e recursos de desenvolvimento C/C++.
- **Filosofia e módulo Conversation Mask** — [`philosophy/`](philosophy/README.md) contém as bases nativas C/C++; [`philosophy/module/`](philosophy/module/README.md) organiza a camada seguinte em três áreas: [`data/`](philosophy/module/data/README.md), para observações tipadas e datadas, terminações de eventos, meia-vida, proveniência e consentimento; [`helper/`](philosophy/module/helper/README.md), para validação determinística, decaimento de relevância, comparações compatíveis e apresentação filtrada por consentimento; e [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md), para uma interface de conversa de apoio, não clínica. As interfaces SLeeLa e a Tree composta estão em [`lib/philosophy/`](lib/philosophy/README.md). O módulo exige entradas explícitas e dá prioridade ao consentimento: não monitoriza pessoas ou meios passivamente, não diagnostica, não infere intenções privadas, não classifica o valor humano nem recomenda alterações de medicação. As ligações de eventos preservam proveniência e navegação, não corroboração independente.
- **Synchro** — integração nativa de runtime/compilador e funcionalidades relacionadas com despacho de pacotes e medição; consulte as fontes relevantes em `impl/` e a implementação do compilador.
- **SleelaUI / interface de terminal** — `lib/user-interface/` e o projeto `sleela-terminal`: integração da interface SLeeLa e componentes de interação orientados ao terminal.

### U–W

- **Nações Unidas** — `united-nations/`: materiais de repositório específicos do domínio.
- **Wi-Fi 4, Wi-Fi 5 e Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`, `wi-fi-6/` e diretórios de fontes relacionados em `lib/wifi/`: material de redes sem fios por geração e recursos de configuração.

### Manutenção do índice de pacotes

Quando um pacote é adicionado ou substancialmente ampliado, acrescente aqui a respetiva entrada no grupo alfabético adequado, ligue ao diretório canónico e ao README e descreva apenas capacidades sustentadas pelas fontes presentes no repositório. Mantenha este índice aditivo: não remova nem reescreva as secções existentes do README ao atualizar o índice de pacotes.

## XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

`bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh` :: `⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓`

`bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc` :: `⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉`

Política democrática e política dos EUA.

## XVI. Imagens de marca da ciência nacional

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>

## XVII. Catálogo experimental UTF-4088 com um milhão de entradas

O [gerador do catálogo UTF-4088](utf-4088/tools/generate_catalogue.py) constrói deterministicamente um catálogo experimental de símbolos com 1 048 576 entradas. O formato compacto está descrito em [`utf-4088/FORMAT.md`](utf-4088/FORMAT.md); o contrato de geração e validação encontra-se em [`utf-4088/GENERATION-RESULTS.md`](utf-4088/GENERATION-RESULTS.md).

Execute o gerador com Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

O ficheiro binário gerado tem 16 777 244 bytes e foi concebido para permanecer abaixo de 20 000 000 bytes. Testes unitários e um workflow GitHub Actions verificam a quantidade de entradas, a unicidade dos glifos, o tamanho e SHA-256; as execuções bem-sucedidas publicam o binário e o resumo como artefacto `utf4088-million-catalogue`. O binário gerado é deliberadamente excluído do histórico Git. UTF-4088 continua experimental e não é compatível com Unicode.

---

# SLeeLa

<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Adições recentes — constante Pi, arranque Modium/Registry, matemática sleelactl e `@native`

Esta secção regista funcionalidades adicionadas no trabalho de 2026-11, começando pelos grupos mais recentes. Todas estão comprometidas em `master`.

### Constante de catálogo Pi com 100 000 000 dígitos

O repositório inclui um valor verificado de π com **cem milhões de dígitos** em [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits), além de um valor com 57 344 dígitos em [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Ambos foram verificados independentemente como valores genuínos de π. [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md) documenta as verificações: prefixo de referência, ponto de Feynman na posição fracionária 762, o dígito famoso na posição um milhão, concordância com `lib/math/SLPiReference.sleela`, análise da frequência de dígitos do ficheiro completo e um script de reprodução.

### Modium™ / Registry™ carregam a constante Pi no arranque do subsistema

Nos pontos de entrada dedicados — `sleela registry` e `sleela modium` (e na primeira execução do instalador), **não** em cada `sleela run` — os subsistemas Modium e Registry carregam a constante de catálogo Pi. Num terminal interativo, apresentam temporariamente uma linha vermelha `Loading Modium™` / `Loading Registry™`, que é apagada antes da saída normal. O comportamento depende estritamente de TTY, pelo que a saída canalizada e os testes permanecem inalterados byte a byte. O localizador, não os bytes, é registado como predefinição da chave `Math` do Registry: `piReferenceLargePath` e `piReferenceLargeDigits`, tanto no modelo SLeeLa ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) como no semeador nativo ([`registry/sleela_registry.c`](registry/sleela_registry.c)). `lib/math/SLPiReference.sleela` expõe `largePath()` e `largeDigits()`. A constante é instalada na localização SLeeLa do sistema operativo (`<data>/pi/`) pelos instaladores padrão `make` e guiado. Consulte [`MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — instalador guiado e domínio matemático com backend nativo

- O instalador guiado («beautiful») instala agora `sleelactl` (o plano de controlo SLeeLa) e o respetivo registo de módulos, em correspondência com o instalador `make`. Consulte [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- `sleelactl` recebeu uma nova natureza `subject` para bibliotecas de domínio C/C++ nativas compiladas no runtime e regista a primeira: `math`. `sleelactl run math` executa o backend `__NativeMath` (`impl/subjects/math/math_native.cpp`) através da demonstração de referência; `sleelactl info math` explica como o alcançar (`import math;`). Esta natureza oferece apenas os verbos `run` e `info`; não existe `check`, porque `sleela check` não carrega os domínios nativos registados por SHEET.

### `@native` e `sleela aot` — Parte 1 do percurso SLeeLa → C/C++ nativo

A nova anotação de método **`@native`** solicita compilação nativa antecipada de um método intensivo. O comando `sleela aot` encontra os métodos anotados, verifica se pertencem a um subconjunto suportado para transferência e emite C/C++. A operação **falha de forma fechada** — recusa construções ainda não traduzíveis e apresenta uma razão precisa, evitando produzir métodos parcialmente traduzidos.

```sh
sleela aot --list <file.sleela>    # veredito por método: ok / REFUSED (motivo)
sleela aot --emit <file.sleela>    # emitir C/C++ (stdout) para um programa no subconjunto
```

Atualmente, `@native` não altera o comportamento: os programas continuam a ser verificados e executados sem alterações; a anotação apenas solicita tratamento nativo. Esta é a Parte 1: estabelece a anotação e a fronteira honesta do subconjunto. A compilação do C/C++ emitido para uma biblioteca partilhada e o encaminhamento da VM para essa biblioteca (com fallback para o interpretador) são etapas posteriores. Consulte [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md) para o subconjunto suportado e os códigos de saída.

## XIX. Edições internacionais do README

A SLeeLa disponibiliza pontos de entrada README específicos por país para os 50 mercados da edição internacional. Cada país tem um ficheiro `README.<CC>.md`; quando são relevantes várias línguas, estas são agrupadas no ficheiro desse país.

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

**Política de tradução:** estes ficheiros são pontos de entrada localizados. Uma edição só deve ser descrita como tradução completa quando todas as secções estiverem traduzidas, os identificadores técnicos e as ligações tiverem sido verificados e a edição tiver recebido revisão linguística. A cobertura linguística é representativa e não afirma que todas as línguas regionais, indígenas, gestuais ou constitucionalmente reconhecidas estejam integralmente traduzidas.

---

**Estado da tradução:** secções I–XIX traduzidas para português. Identificadores técnicos, comandos, exemplos de código, nomes de ficheiros e ligações foram preservados. Recomenda-se revisão final por um falante nativo antes de publicação como tradução editorial aprovada.
