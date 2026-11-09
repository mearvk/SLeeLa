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

**Tradução pendente.** Os anúncios completos de alterações e funcionalidades da secção II inglesa serão traduzidos numa próxima etapa. Fonte de referência: [README.md — secção II](README.md).

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

O comando `make` na raiz do repositório compila todos os produtos (núcleo, VM, codecs, churn, antivírus, etc.) e termina com a criação do **registo** e a materialização da sua tabela-base. Assim, uma árvore de trabalho compilada contém desde logo um registo verificável.

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

## Secções ainda por traduzir

As seguintes secções ainda não foram traduzidas e serão acrescentadas em etapas futuras: **II. Novidades; VIII. Temas do editor SLeeLa; IX. Contribuir; X–XVII. Outras áreas do projeto; XVIII. Adições recentes; XIX. Edições internacionais do README.** Até à conclusão da tradução, o [README.md canónico em inglês](README.md) continua a ser a referência completa.

---
**Estado da tradução:** tradução parcial — secções I e III–VII traduzidas. Os identificadores técnicos, comandos, exemplos de código, nomes de ficheiros e ligações foram mantidos sem alterações.
