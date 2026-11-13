<img align="right" src="images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — Portugal (PT)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Uma linguagem nativa de sistemas, com compilador, ambiente de execução e ecossistema de máquinas virtuais.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Edição internacional por país.** Este ficheiro disponibiliza a edição em português europeu (pt-PT) destinada a Portugal. Os identificadores técnicos, comandos, exemplos de código, nomes de ficheiros e ligações permanecem inalterados.

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

- **O pacote de biblioteca padrão `advantage` — tirar partido de um dividendo, com matemática, física, integração, classificação e saída encaminhável.** [`lib/advantage/`](lib/advantage/README.md) encapsula *a obtenção de vantagem sobre um dividendo único relativamente a um modelo* sob a forma de uma **Sequitur onion** em camadas (dividendo → revisão → preocupações → matemática → adições → subtração → reagrupamento → iteração), navegável para dentro e para fora, com atualização dinâmica completa e restituição, exposta através de um **modelo do utilizador** simples e de um **modelo do programador** com controlo total. Contém cinco áreas temáticas: **`math/`**, economia de comércio bilateral (maximização conjunta do lucro, negociação de Nash, rácio crítico newsvendor bilateral); **`physics/`**, uma «física dos campos de confiança» de economia estrutural (a confiança como tensor métrico, a geodésica transacional, uma estrutura de contacto de anúncio, uma SDE de decaimento de confiança CIR+salto — uma analogia de modelação, assinalada como tal); **`integration/`**, um modelo de integração vertical recursivamente componível para o capitalismo/comércio a retalho (compartimentos, baias de engenharia, compartimentos de ensaio, tabuleiros de mediana newton e uma pontuação de sofisticação do sistema); **`grading/`**, um classificador de circuito de sofisticação que avalia cada módulo e qualquer conjunto de módulos vencedores/perdedores/classificadores/absolutos de A a D; e **`output/`**, um ponto de emissão encaminhável que envia a saída dos módulos para o terminal, para uma superfície formatada por **CommonRails** ou para um socket/conector (enquadrada para um `slnet_write` nativo). Cada classe transporta o registo Metadata-Class; cinco demonstrações executáveis validam os temas, e os documentos complementares [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md), [`MATH.md`](lib/advantage/MATH.md) e [`PHYSICS.md`](lib/advantage/PHYSICS.md) descrevem-nos. Um consumidor nativo C/C++ em [`advantage/`](advantage/README.md) (o núcleo C `advantage_value`/`advantage_mean`, um repositório de conhecimento XML, uma biblioteca segura de aquisição HTTPS/SFTP/SSH + armazém de chaves + transporte DH/ECDH) liga o modelo a [exemplos de comércio](advantage/examples/README.md) elaborados — comércios positivos a partir de fontes presumidas da internet, uma sequência de relevância para o público (Comerciantes, Consumidores, Auxiliares, Curadores) e oito modelos classificados de A a D (D = mais Cement, A = mais Brand).
- **Anotações de temporização de ciclos — `@loopDelay` / `@loopBudget` (e a função integrada `sleep(ms)`).** As primeiras anotações **ao nível da instrução e comportamentais** da SLeeLa: escritas na sua própria linha antes de um ciclo `for` / `while` / `do`, introduzem uma pausa real na execução do ciclo. `@loopDelay(ms = 250)` / `@loopDelay(seconds = 2)` aguarda um período fixo por iteração (um simples `@loopDelay(250)` assume milissegundos); `@loopBudget(seconds = 10, iterations = 1000)` distribui uniformemente um número conhecido de iterações por um orçamento total de tempo real. Ambas são reduzidas ao novo opcode aditivo **`OP_SLEEP`** (ISA 157), que suporta também uma função integrada geral **`sleep(<ms>)`**, invocável em qualquer lugar; a pausa é implementada de forma portátil (`nanosleep` / `Sleep`). Especificado em [`SLEELA.syntax` §32](SLEELA.syntax); referência da API em [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md); [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela) é executável. Um índice de adições da sessão (incluindo esta, a multiplicação nativa de inteiros grandes `OP_BIGMUL` e as ferramentas autónomas **[local-generator](local-generator/README.md)** e **[pi-generator](pi-generator/README.md)**) encontra-se em [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md). As três superfícies de API geradas ([`sleeladoc-api/`](sleeladoc-api/README.md), [`docs/api/`](docs/api/README.md) e o instantâneo imutável [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html)) foram reconstruídas com `make api-docs` e passam a incluir as novas entradas de instruções `SLISASleep` / `SLISABigMul` na página do pacote `vm`.
- **SleelaDoc a partir da linha de comandos — `sleela docs`.** O SleelaDoc é agora acessível diretamente a partir do terminal. `sleela docs` lista todos os tópicos documentáveis; `sleela docs crypto` mostra um pacote; `sleela docs stringbuilder` resolve uma classe (sem distinção entre maiúsculas e minúsculas e pelo prefixo `SL` da biblioteca padrão → `SLStringBuilder`) e apresenta uma página completa — **definição, objetivo de conceção, relações, estrutura, conceitos de utilização e uma escada de domínio de cinco degraus** — com resultados ordenados do tipo «quis dizer» em caso de aproximação. Os autores enriquecem qualquer página simplesmente acrescentando etiquetas `@` do SleelaDoc (`@intent`, `@thesis`, `@axiom`, `@param`, `@see`, `@mastery`). `sleela docs --generate [dir]` escreve um corpus de texto simples (uma página por classe + um `TOPICS.index`) que a CLI lê depois offline; **`make sleeladoc`** reúne esse corpus **e as fontes `.sleela` correspondentes** em `sleeladoc/build/sleeladoc.jar`. Para temas apenas de fachada (por exemplo, `psychiatry`, `politico`), `sleela docs --facade <topic>` pesquisa o sistema de ficheiros em tempo de execução e lista as **pastas (e depois os ficheiros, como movimento natural)** reais, limitado por tetos substituíveis pelo utilizador de **6 MB / 6000 ficheiros / 6 minutos**. As unidades `lib/sleeladoc` são mantidas na edição atual, **`#sleela 3.0`**. Consulte [`man/man1/sleela.1`](man/man1/sleela.1) e [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md).
- **SleelaDoc — uma solução de documentação ao estilo Javadoc, com um sítio HTML gerado.** O **SleelaDoc** é a convenção de comentários de documentação no código-fonte da SLeeLa (comentários entre ângulos, sinalizados com `@doc` / `@page`; ver [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)), e [`lib/sleeladoc`](lib/sleeladoc/README.md) é o **extrator** que converte uma unidade `.sleela` numa página de API em Markdown ou HTML — tal como o `javadoc` o faz para Java, documentando a *intenção*. Um sítio de documentação HTML completo e navegável para **todo o `/lib`** é gerado em [`/sleeladoc-api`](sleeladoc-api/README.md) por `tools/generate-sleeladoc-api.py` (abra `sleeladoc-api/index.html`; um estilo limpo em azul / azul-claro / cinzento-claro / branco, com uma hierarquia pacote → classe → membro e um glossário escrito para o leitor *Student 2*). O compilador emite também, como subproduto da compilação, saídas de documentação marcadas pelo autor (uma página MediaWiki com `--wiki` + um gémeo de código-fonte originado), documentadas em [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md). Executável: [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela).
- **Modelo de Objetos de Documento XML (`lib/xml`).** Um DOM XML do lado da SLeeLa — um documento que detém uma árvore de nós de elemento/texto/comentário/CDATA/PI com filhos ordenados e atributos nomeados, um serializador (compacto/formatado, escape XML 1.0 §2.4), um analisador por células de caracteres e o registo de qualidade `SLXmlAspect`. A igualdade de nós é um empate de confiança em três degraus (identidade → mesmo assunto → estrutura), reflexivo, simétrico e determinístico. Consulte [`lib/xml/README.md`](lib/xml/README.md) e o exemplo executável [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela).
- **Primitivos inteiros com sinal alargados (`long`, `int64`, `int128`).** `int`, `long` e `int64` são todos inteiros com sinal de 64 bits, com verificação de tipos e execução idênticas; `int128` é aceite como tipo inteiro (suportado por 64 bits por agora, sendo o verdadeiro 128 bits um seguimento documentado). A série binária com sinal `int32`..`int16777216` (sintaxe 2.7) e a série sem sinal correspondente `uint32`..`uint16777216` (sintaxe 2.8) ampliam-na. Consulte [`SLEELA.syntax`](SLEELA.syntax) §21, §24, §25.
- **Campos consequentes `@mix` (sintaxe 2.9).** Uma classe pode declarar vários campos de instância com o **mesmo nome** quando cada repetição transporta `@mix`; a linguagem entende o grupo como consequente (um array / lista indexado), de cima para baixo `1,2,3,…` por predefinição ou manualmente através de `@mix(1,2,3,...)`, cuja contagem de índices tem de corresponder ao número de campos. Consulte [`SLEELA.syntax`](SLEELA.syntax) §28.
- **Marcadores documentais — o Processo Natural, intitulado os Preonems (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).** Estas palavras-marcador documentais simples são coletivamente intituladas os **Preonems** (um marcador é um *Preonem*; o conjunto, os *Preonems* — ver [`SLEELA.syntax`](SLEELA.syntax) §26.1). Marcadores documentais de primeira classe, exclusivamente declarativos, que nomeiam o que um documento *é* na ordem da sua feitura: `author`/`authorial` (sintaxe 2.4; tratado por conselho profissional — o único membro com efeito comportamental), `material;` (Fonte assente) e `memory;` (sintaxe 2.5), `determine;` (sintaxe 2.6; um documento aberto a ser processado por um médico, um físico ou um fantasma com vista à conclusão por mão legal) e os dois mais recentes, **`object;`** (uma coisa assente que figura no catálogo do que é — **reificada, com existência concreta, na sintaxe 2.8**) e **`field;`** (o terreno da relação em que os objetos se encontram e se movem) — ambos condicionados a partir da sintaxe 2.6. Cada um é simples, contextual e condicionado por versão pelo frontend, sem efeito na análise sintática, na tipagem nem na geração de código; fora da posição de marcador, todos continuam a ser identificadores normais (pelo que `field` nunca perturba um membro de classe nem um acesso `.field`). Consulte [`SLEELA.syntax`](SLEELA.syntax) §18–§20, §23 e §26.
- **Nordshrift na sintaxe 3.0 com alcance integral da biblioteca.** O Nordshrift resolve através do mesmo índice recursivo de `/lib` que o compilador, alcançando todos os pacotes e unidades `.sleela`; `nordshrift check` apresenta as contagens da árvore completa.
- **Stanchion de recursão.** O diretório [`recursion/stanchion/`](recursion/stanchion/README.md) inclui agora protótipos de referência dedicados em C e C++ para os sete contratos primitivos documentados, além de exemplos genéricos, aplicados, científicos e pedagógicos. São fontes de referência; compile-as e teste-as antes de as considerar verificadas.

- **Comentários entre ângulos (`<` / `<<`).** Um novo estilo de comentário para o futuro — `< linha >` e `<< bloco >>` — analisado lexicalmente pelo frontend da SLeeLa e documentado em [`SLEELA.syntax`](SLEELA.syntax) §1.1.
- **Literais inteiros por base.** Os literais `int` aceitam agora as formas hexadecimal (`0x`), binária (`0b`) e octal (`0o`) com separadores de dígitos `_`, tanto na linguagem de código-fonte SLeeLa ([`SLEELA.syntax`](SLEELA.syntax) §1.4) como no [formato de modelos de CPU `.xlt`](lib/xlt/README.md).
- **A norma Metadata Class em toda a biblioteca.** A metapadronização de metadados descritivos (proveniência, legalidade, âmbito jurisdicional) é agora transportada por todos os ficheiros `.sleela` com classes em `/lib`; consulte [`SLEELA.syntax`](SLEELA.syntax) §15 e [a lição do tutorial](tutorial/13-metadata-class.md).
- **Uma família de texto / String mais rica.** `lib/text` ganha auxiliares reais na própria linguagem, uma vista por células de caracteres com `charAt`/`substring`/`indexOf`/`reverse`/alinhamento de colunas genuínos e um modelador posicional real — consulte o [tutorial de texto e String](tutorial/14-text-and-strings.md) e o exemplo executável [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela).
- **`String` de primeira classe nas folhas Nordshrift.** Uma folha `.sst` pode declarar um bloco `strings:` tipado que contribui para o manifesto de componentes da compilação; consulte a [Referência de Folhas Nordshrift](wiki/Nordshrift-Sheet-Reference.md).
- **Modelos de CPU executados por opcodes e por equivalência de chamadas de função.** Cada `SL<ARCH>CPU` herda a base de executar um fluxo de instruções nativo através do seu ciclo de obtenção-descodificação-execução *e* de executar o código de um outro processo presumido por chamadas equivalentes sobre o substrato VM partilhado — consulte [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5.

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

A gramática normativa encontra-se em [`SLEELA.syntax`](SLEELA.syntax). O repositório documenta atualmente a sintaxe **3.0**; a versão do compilador/ferramentas e a versão da sintaxe do código-fonte são independentes. A superfície 3.0 acrescenta os marcadores documentais **`pure` e `gasoline`** (§30): um par de Preonems simples, contextuais e exclusivamente declarativos, que significam quase o mesmo, marcando cada um a posição do documento através da sua própria honestidade. A sintaxe 3.0 reúne as superfícies da linguagem 1.0 a 2.9 numa edição normativa atual e mantém os requisitos de versão mínima de cada funcionalidade.

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

A SLeeLa inclui um conjunto coordenado de esquemas de cores escuros e profissionais para **Vim** e **Emacs**, além de um utilitário de paletas nativo C/C++. O esquema original **American Professional** continua a ser o predefinido. Existem dez paletas complementares inspiradas em diferentes tradições culturais e institucionais; são paletas de design interpretativas, não representações oficiais de qualquer país, cultura, tribunal ou instituição.

### Os onze esquemas e as respetivas cores

| Esquema | Fundo | Primeiro plano | Palavras-chave | Tipos | Cadeias | Comentários |
|---|---|---|---|---|---|---|
| **American Professional** | <span style="display:inline-block;width:1em;height:1em;background:#101820;border:1px solid #888"></span> `#101820` | <span style="display:inline-block;width:1em;height:1em;background:#F4F7FA;border:1px solid #888"></span> `#F4F7FA` | <span style="display:inline-block;width:1em;height:1em;background:#7FB3FF;border:1px solid #888"></span> `#7FB3FF` | <span style="display:inline-block;width:1em;height:1em;background:#73DACA;border:1px solid #888"></span> `#73DACA` | <span style="display:inline-block;width:1em;height:1em;background:#F2C078;border:1px solid #888"></span> `#F2C078` | <span style="display:inline-block;width:1em;height:1em;background:#8A96A3;border:1px solid #888"></span> `#8A96A3` |
| **Pan-Eurasian** | <span style="display:inline-block;width:1em;height:1em;background:#111827;border:1px solid #888"></span> `#111827` | <span style="display:inline-block;width:1em;height:1em;background:#F0F2F5;border:1px solid #888"></span> `#F0F2F5` | <span style="display:inline-block;width:1em;height:1em;background:#7AA7E8;border:1px solid #888"></span> `#7AA7E8` | <span style="display:inline-block;width:1em;height:1em;background:#69C9B3;border:1px solid #888"></span> `#69C9B3` | <span style="display:inline-block;width:1em;height:1em;background:#D7B56D;border:1px solid #888"></span> `#D7B56D` | <span style="display:inline-block;width:1em;height:1em;background:#8995A5;border:1px solid #888"></span> `#8995A5` |
| **Russian and Slavic — Medium** | <span style="display:inline-block;width:1em;height:1em;background:#202733;border:1px solid #888"></span> `#202733` | <span style="display:inline-block;width:1em;height:1em;background:#E8EAF0;border:1px solid #888"></span> `#E8EAF0` | <span style="display:inline-block;width:1em;height:1em;background:#A9C0DD;border:1px solid #888"></span> `#A9C0DD` | <span style="display:inline-block;width:1em;height:1em;background:#8FC4B2;border:1px solid #888"></span> `#8FC4B2` | <span style="display:inline-block;width:1em;height:1em;background:#D6BD8B;border:1px solid #888"></span> `#D6BD8B` | <span style="display:inline-block;width:1em;height:1em;background:#9AA3AF;border:1px solid #888"></span> `#9AA3AF` |
| **Central Asian / Silk Road** | <span style="display:inline-block;width:1em;height:1em;background:#1C2529;border:1px solid #888"></span> `#1C2529` | <span style="display:inline-block;width:1em;height:1em;background:#F5F0E5;border:1px solid #888"></span> `#F5F0E5` | <span style="display:inline-block;width:1em;height:1em;background:#7FADE0;border:1px solid #888"></span> `#7FADE0` | <span style="display:inline-block;width:1em;height:1em;background:#45C6C2;border:1px solid #888"></span> `#45C6C2` | <span style="display:inline-block;width:1em;height:1em;background:#D9B56D;border:1px solid #888"></span> `#D9B56D` | <span style="display:inline-block;width:1em;height:1em;background:#92A3A0;border:1px solid #888"></span> `#92A3A0` |
| **East Asian** | <span style="display:inline-block;width:1em;height:1em;background:#171A1C;border:1px solid #888"></span> `#171A1C` | <span style="display:inline-block;width:1em;height:1em;background:#F2EFE7;border:1px solid #888"></span> `#F2EFE7` | <span style="display:inline-block;width:1em;height:1em;background:#D97878;border:1px solid #888"></span> `#D97878` | <span style="display:inline-block;width:1em;height:1em;background:#72C7A5;border:1px solid #888"></span> `#72C7A5` | <span style="display:inline-block;width:1em;height:1em;background:#D8B76A;border:1px solid #888"></span> `#D8B76A` | <span style="display:inline-block;width:1em;height:1em;background:#969F9B;border:1px solid #888"></span> `#969F9B` |
| **ICC — International Criminal Court** | <span style="display:inline-block;width:1em;height:1em;background:#0E1A2B;border:1px solid #888"></span> `#0E1A2B` | <span style="display:inline-block;width:1em;height:1em;background:#EAF0F7;border:1px solid #888"></span> `#EAF0F7` | <span style="display:inline-block;width:1em;height:1em;background:#5B8FD6;border:1px solid #888"></span> `#5B8FD6` | <span style="display:inline-block;width:1em;height:1em;background:#7FB7C9;border:1px solid #888"></span> `#7FB7C9` | <span style="display:inline-block;width:1em;height:1em;background:#D9B25A;border:1px solid #888"></span> `#D9B25A` | <span style="display:inline-block;width:1em;height:1em;background:#7E8CA0;border:1px solid #888"></span> `#7E8CA0` |
| **SCOTUS — U.S. Supreme Court** | <span style="display:inline-block;width:1em;height:1em;background:#1A1416;border:1px solid #888"></span> `#1A1416` | <span style="display:inline-block;width:1em;height:1em;background:#F3EEE6;border:1px solid #888"></span> `#F3EEE6` | <span style="display:inline-block;width:1em;height:1em;background:#9E2B35;border:1px solid #888"></span> `#9E2B35` | <span style="display:inline-block;width:1em;height:1em;background:#8FA9B8;border:1px solid #888"></span> `#8FA9B8` | <span style="display:inline-block;width:1em;height:1em;background:#C9A24B;border:1px solid #888"></span> `#C9A24B` | <span style="display:inline-block;width:1em;height:1em;background:#8C8178;border:1px solid #888"></span> `#8C8178` |
| **Greek (Hellenic)** | <span style="display:inline-block;width:1em;height:1em;background:#0A1A30;border:1px solid #888"></span> `#0A1A30` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#2E7BD6;border:1px solid #888"></span> `#2E7BD6` | <span style="display:inline-block;width:1em;height:1em;background:#5E9FE0;border:1px solid #888"></span> `#5E9FE0` | <span style="display:inline-block;width:1em;height:1em;background:#9FC3EC;border:1px solid #888"></span> `#9FC3EC` | <span style="display:inline-block;width:1em;height:1em;background:#6E86A6;border:1px solid #888"></span> `#6E86A6` |
| **Turkish (Al Bayrak)** | <span style="display:inline-block;width:1em;height:1em;background:#2A0609;border:1px solid #888"></span> `#2A0609` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#F23D48;border:1px solid #888"></span> `#F23D48` | <span style="display:inline-block;width:1em;height:1em;background:#F0A9AE;border:1px solid #888"></span> `#F0A9AE` | <span style="display:inline-block;width:1em;height:1em;background:#F0C9A9;border:1px solid #888"></span> `#F0C9A9` | <span style="display:inline-block;width:1em;height:1em;background:#A8767A;border:1px solid #888"></span> `#A8767A` |
| **Kiro** | <span style="display:inline-block;width:1em;height:1em;background:#0B0B12;border:1px solid #888"></span> `#0B0B12` | <span style="display:inline-block;width:1em;height:1em;background:#F5F5FA;border:1px solid #888"></span> `#F5F5FA` | <span style="display:inline-block;width:1em;height:1em;background:#A98EDA;border:1px solid #888"></span> `#A98EDA` | <span style="display:inline-block;width:1em;height:1em;background:#6E8BE0;border:1px solid #888"></span> `#6E8BE0` | <span style="display:inline-block;width:1em;height:1em;background:#E8C547;border:1px solid #888"></span> `#E8C547` | <span style="display:inline-block;width:1em;height:1em;background:#6A6482;border:1px solid #888"></span> `#6A6482` |
| **SLeeLa** | <span style="display:inline-block;width:1em;height:1em;background:#08080E;border:1px solid #888"></span> `#08080E` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#B07CFF;border:1px solid #888"></span> `#B07CFF` | <span style="display:inline-block;width:1em;height:1em;background:#5A7CFF;border:1px solid #888"></span> `#5A7CFF` | <span style="display:inline-block;width:1em;height:1em;background:#FFD21E;border:1px solid #888"></span> `#FFD21E` | <span style="display:inline-block;width:1em;height:1em;background:#6E5F9E;border:1px solid #888"></span> `#6E5F9E` |

#### Cores das bandeiras nacionais (paleta de origem)

Os esquemas grego e turco derivam das **cores oficiais das bandeiras nacionais** dos dois países, aqui apresentadas com os respetivos valores hexadecimais exatos publicados. Os fundos e os acentos dos esquemas acima são interpretações escurecidas e adequadas a editores destas cores de origem — a tabela seguinte é a referência das «cores tal como expostas nacionalmente».

| País | Cor | Hex | Base e significado |
|---|---|---|---|
| **Grécia** | Azul | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | Azul da bandeira nacional (Lei 851/1978). O céu e os mares; descrito heraldicamente como *azure*. |
| **Grécia** | Branco | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Branco da bandeira nacional. A pureza da luta pela independência; a cruz denota o cristianismo ortodoxo oriental. |
| **Türkiye** | Vermelho (*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | Vermelho da bandeira nacional (Lei da Bandeira Turca). O campo; por tradição, o sangue dos mártires e a soberania otomana. |
| **Türkiye** | Branco | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | Branco da bandeira nacional. O crescente e a estrela — paz, pureza e o símbolo da herança turcomana/islâmica. |

Os esquemas ancoram o primeiro plano de cada editor no **branco** nacional e as palavras-chave, o cursor, a seleção e a linha de estado no **azul** nacional exato (`#0D5EAF`) ou no **vermelho** nacional exato (`#E30A17`), de modo que as cores que importam são literalmente as cores nacionais. Trata-se de paletas de design interpretativas; a referência das cores das bandeiras é fornecida a título de proveniência e não constitui uma representação oficial de nenhum dos Estados. Os significados das cores seguem a descrição dos símbolos de Estado do Ministério dos Negócios Estrangeiros da República Helénica e a tradição da Lei da Bandeira Turca.

### Tabela de amostras de cores

As células de paleta abaixo usam **amostras SVG**, pelo que o GitHub apresenta as cores reais em vez de células de cor HTML vazias. Cada faixa mostra as seis cores por esta ordem: fundo, primeiro plano, palavras-chave, tipos, cadeias e comentários. Os códigos hexadecimais exatos estão listados ao lado de cada amostra, pela mesma ordem de esquemas da tabela de referência acima.

| Esquema | Cores da paleta apresentadas | Códigos hex (fundo · primeiro plano · palavras-chave · tipos · cadeias · comentários) |
|---|---|---|
| **American Professional** | ![Paleta American Professional: fundo, primeiro plano, palavras-chave, tipos, cadeias, comentários](lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Paleta Pan-Eurasian: fundo, primeiro plano, palavras-chave, tipos, cadeias, comentários](lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russian and Slavic — Medium** | ![Paleta Russian and Slavic Medium: fundo, primeiro plano, palavras-chave, tipos, cadeias, comentários](lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Central Asian / Silk Road** | ![Paleta Central Asian Silk Road: fundo, primeiro plano, palavras-chave, tipos, cadeias, comentários](lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **East Asian** | ![Paleta East Asian: fundo, primeiro plano, palavras-chave, tipos, cadeias, comentários](lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC — International Criminal Court** | ![Paleta ICC: fundo, primeiro plano, palavras-chave, tipos, cadeias, comentários](lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS — U.S. Supreme Court** | ![Paleta SCOTUS: fundo, primeiro plano, palavras-chave, tipos, cadeias, comentários](lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Greek (Hellenic)** | ![Paleta Greek Hellenic: fundo, primeiro plano, palavras-chave, tipos, cadeias, comentários](lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turkish (Al Bayrak)** | ![Paleta Turkish Al Bayrak: fundo, primeiro plano, palavras-chave, tipos, cadeias, comentários](lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Paleta Kiro: fundo, primeiro plano, palavras-chave, tipos, cadeias, comentários](lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![Paleta SLeeLa: fundo, primeiro plano, palavras-chave, tipos, cadeias, comentários](lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

Utilizam-se cores de acento adicionais para funções, constantes, seleções, diagnósticos e diretivas do pré-processador. A paleta Russian and Slavic usa intencionalmente ardósia, azul, verde-sálvia, rubi e ouro antigo de intensidade média, para uma aparência profissional mais calma.

### Instalar os esquemas Vim

A partir da raiz do repositório:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Adicione o seguinte a `~/.vimrc` para ativar o realce de sintaxe SLeeLa e escolher um esquema:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Escolha exatamente um destes nomes de esquema de cores:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Por exemplo, utilize `colorscheme sleela-russian-slavic` para a paleta Russian and Slavic de intensidade média, ou `colorscheme sleela-greek` / `colorscheme sleela-turkish` para as paletas helénica e Al Bayrak. Reinicie o Vim ou carregue a sua configuração com `:source $MYVIMRC`.

### Instalar os esquemas Emacs

Crie um diretório local de temas e copie os ficheiros de modo e de tema a partir da raiz do repositório:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Adicione o seguinte a `~/.emacs` ou `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Substitua `sleela-american` por `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` ou `sleela-sleela` para selecionar outro esquema. Se o Emacs perguntar se deve confiar no tema, inspecione primeiro o ficheiro de tema local antes de aceitar.

### Pré-visualizar as paletas nativas C/C++

O pequeno utilitário de paletas nativo pode listar esquemas, imprimir as respetivas cores de terminal e consultar uma cor individual:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Por compatibilidade retroativa, `--palette` e `--hex COLOR` sem `--theme` selecionam American Professional. Os resultados de compilação e de testes devem ser verificados no sistema de destino e em CI; a documentação de um comando não é, por si só, prova de que passou.

Consulte [o guia de integração de editores](linux-editors/README.md) para as descrições completas das paletas e os pormenores do utilitário.

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

O catálogo especializado UTF-4088 define o percurso espacial do mapa de entrada e as iterações abstratas de pressão em [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). A política de índice de assuntos do National Lien está em [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). As coordenadas do mapa, os níveis de pressão (1–50), os IDs permanentes do catálogo e as classificações alfabéticas são campos independentes; nenhum deve ser substituído silenciosamente por outro. A pressão é um nível de iteração algorítmica, não uma grandeza física.

## XIV. Índice de pacotes recentes e ampliados

Este índice adicional destaca pacotes, bibliotecas de domínio e coleções de fontes recentemente ampliados. As entradas estão agrupadas alfabeticamente por nome de pacote ou tema. As descrições identificam a localização no repositório; o estado de implementação e validação deve ser confirmado na documentação e nos testes de cada pacote.

### A–C

- **Advantage** — `lib/advantage/` e `advantage/`: obtenção de vantagem sobre um dividendo único relativamente a um modelo, como uma Sequitur onion em camadas, com cinco subpastas temáticas — `math/` (maximização conjunta do lucro, negociação de Nash, newsvendor bilateral), `physics/` (métrica/geodésica/contacto/SDE de campos de confiança, uma analogia de modelação assinalada), `integration/` (compartimentos de integração vertical e uma pontuação de sofisticação), `grading/` (um classificador de circuito de sofisticação A–D para módulos e conjuntos) e `output/` (emissão para terminal / CommonRails / socket). Um consumidor nativo C/C++ (`advantage/`) liga o modelo a exemplos de comércio elaborados. Consulte [`lib/advantage/README.md`](lib/advantage/README.md) e [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md).
- **Alfabeto e UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` e `utf-4088/`: catálogos alfabéticos, suporte nativo de conjuntos de caracteres, ordenação de mapas de entrada e documentação de integração UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` e `lib/autocad/`: fluxos de desenho 2D interativos e componentes reutilizáveis para documentos e edição CAD.
- **Ballum** — controlo de ecrã ao nível do píxel: definição/leitura por píxel, cópia de regiões (blit), brilho/calor/gama de todo o ecrã, entrada de rato/teclado e uma **superfície framebuffer de ecrã inteiro** para um controlo por píxel total, próprio e rápido (janela override-redirect + uma única apresentação `XPutImage`/MIT-SHM em Linux/X11). Núcleo nativo em `ballum/`, com a fachada SLeeLa em `lib/ballum/` e uma ponte VM em `impl/`; consulte [`ballum/README.md`](ballum/README.md) e os [exemplos executáveis](ballum/example/) em `ballum/example/`.
- **CDMA** — `cdma/` e `lib/cdma/`: material técnico relacionado com CDMA e trabalho de biblioteca-fonte SLeeLa.
- **Character** — `lib/character/`: perfis de caracteres e estatísticas reutilizáveis, incluindo exemplos biográficos.
- **Coorenagraph** — `lib/coorenagraph/`: raciocínio ético, vocabulário moral, deliberação e conceitos relacionados; consulte o README do pacote e os ficheiros de código-fonte.

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

---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

Política democrática e política dos EUA.

---

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

Esta secção regista funcionalidades adicionadas durante o trabalho de 2026-11, começando pelos grupos mais recentes. Os ficheiros de origem ligados e o respetivo estado de validação são a referência autorizada; as alterações em revisão não são descritas como integradas até chegarem a `master`.

### Constante de catálogo Pi com 100 000 000 dígitos

O repositório inclui um valor verificado de π com **cem milhões de dígitos** em [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) (um decimal bem formado, `3.14159…`), além de um valor com 57 344 dígitos em [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits). Ambos foram verificados independentemente como valores genuínos de π — consulte [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md), que documenta as verificações (prefixo de manual, o ponto de Feynman na posição fracionária 762, o célebre milionésimo dígito, concordância com `lib/math/SLPiReference.sleela` e uma análise da frequência de dígitos do ficheiro completo) e um script de reprodução num só passo.

### Modium™ / Registry™ carregam a constante Pi no arranque do subsistema

Nos pontos de entrada dedicados — `sleela registry` e `sleela modium` (e na primeira execução do instalador), **não** em cada `sleela run` — os subsistemas Modium e Registry carregam a constante de catálogo π de grande dimensão. Num terminal interativo, apresentam uma linha vermelha transitória `Loading Modium™` / `Loading Registry™`, que é depois apagada no próprio local antes de começar a saída normal; o efeito está estritamente condicionado a TTY, pelo que a saída canalizada e a dos testes permanecem inalteradas byte a byte. O localizador (não os bytes) é registado como predefinições da chave `Math` do Registry, `piReferenceLargePath` e `piReferenceLargeDigits`, tanto no modelo SLeeLa ([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)) como no semeador nativo ([`registry/sleela_registry.c`](registry/sleela_registry.c)); `lib/math/SLPiReference.sleela` expõe `largePath()` / `largeDigits()`. A constante π é instalada na localização SLeeLa do sistema operativo (`<data>/pi/`) **tanto** pelo instalador `make` padrão **como** pelo instalador guiado «beautiful». Pormenores: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl` — o instalador guiado e um domínio `math` com backend nativo

- O instalador guiado («beautiful») instala agora `sleelactl` (o plano de controlo da SLeeLa) e o respetivo registo de módulos, em correspondência com o instalador `make`. Consulte [`bin/SLEELACTL.md`](bin/SLEELACTL.md).
- `sleelactl` ganhou uma nova natureza **`subject`** para as bibliotecas de domínio nativas C/C++ compiladas no runtime, e regista a primeira: **`math`**. `sleelactl run math` exercita de ponta a ponta o backend nativo `__NativeMath` (`impl/subjects/math/math_native.cpp`) através da demonstração de referência do domínio; `sleelactl info math` explica como é alcançado (`import math;`). A natureza de domínio oferece apenas `run info` — não existe deliberadamente um verbo `check`, porque `sleela check` não carrega os domínios nativos registados por SHEET.

### `@native`, Native Drive e execução multilinguagem

Uma nova anotação de método **`@native`** solicita a compilação nativa antecipada de um método intensivo. O novo comando `sleela aot` encontra os métodos `@native`, verifica se cada um está dentro de um *subconjunto transferível* suportado e emite C/C++ — **falhando de forma fechada** (recusando, com uma razão precisa) perante qualquer construção que ainda não consiga traduzir, de modo que nunca é produzido um método traduzido pela metade.

```sh
sleela aot --list <file.sleela>    # veredito por método: ok / REFUSED (motivo)
sleela aot --emit <file.sleela>    # emitir C/C++ (stdout) para um programa no subconjunto
```

`@native` continua a ser um pedido de tratamento AOT, com as construções não suportadas rejeitadas antes da emissão. O [Native Drive](impl/native-drive/README.md) suporta compilações multiunidade de executáveis e bibliotecas partilhadas C/C++ através de compiladores do anfitrião (incluindo MSVC), saída AOT SLeeLa suportada, inspeção de símbolos face à ABI do carregador nativo, execução de processos sem shell, tratamento de stdin/stdout/stderr, encadeamento em várias fases e delegação XClass ao carregador de produção. O `sleelactl` expõe os respetivos comandos de compilação/execução e encaminha a inspeção XClass e a renderização/instalação explícita de unidades systemd através de `inspect native-drive`. A VM possui uma tabela limitada de identificadores de módulos nativos, com autorização obrigatória do anfitrião e identificadores verificados por geração. A sintaxe 3.0 acrescenta agora `nativeOpen`, `nativeCall` e `nativeClose`: o despacho de bytecode aditivo atravessa uma ABI com versão própria para null/bool/int64/double/UTF-8, enquanto arrays, structs e outros valores compostos pertencentes à VM permanecem dentro do interpretador e podem usar o recurso de bytecode com o mesmo nome. Os plugins podem opcionalmente receber uma tabela de callbacks do anfitrião com versão, para invocar funções SLeeLa exatas através de execução aninhada da VM, com raízes de GC para pilhas suspensas, uma lista de permissão explícita de nomes de callbacks do anfitrião e chamadas recursivas à API nativa rejeitadas. O código nativo continua a ser código confiável em processo, não uma sandbox. As anotações de política de processos no código-fonte têm um esquema com versão, mas ainda não são analisadas nem aplicadas. Pormenores, fronteiras de segurança e o registo das sete frentes de trabalho: [Native Drive](impl/native-drive/README.md) e [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md).


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

**Política de tradução:** estes ficheiros são pontos de entrada localizados e iniciais. As respetivas introduções e títulos de língua estão localizados, enquanto a referência técnica completa permanece no [README.md](README.md) canónico em inglês. Um ficheiro de país não deve ser descrito como tradução completa até que todas as secções tenham sido traduzidas, os identificadores técnicos e as ligações tenham sido verificados e a edição tenha recebido revisão linguística. A cobertura linguística desta primeira fase é representativa e não constitui uma afirmação de que todas as línguas constitucionalmente reconhecidas, regionais, indígenas ou gestuais estejam integralmente traduzidas.

---

**Estado da tradução:** secções I–XIX traduzidas para português europeu. Identificadores técnicos, comandos, exemplos de código, nomes de ficheiros, ligações e endereços foram preservados. Recomenda-se revisão final por falantes nativos antes de a edição ser publicada como tradução editorial aprovada.
