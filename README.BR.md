# SLeeLa 3.0 — Brasil (BR)

## Edição em português brasileiro

SLeeLa é uma linguagem nativa de programação de sistemas, acompanhada de compilador, ambiente de execução e um ecossistema de máquinas virtuais. Esta edição apresenta o projeto a leitores do Brasil. Nomes técnicos, identificadores, comandos, caminhos, exemplos de código e nomes de API são mantidos para que as instruções possam ser utilizadas diretamente.

**The Now. The Ingredient.** Uma linguagem é mais do que sua sintaxe: inclui o compilador, o ambiente de execução, as bibliotecas, as ferramentas e os contratos de plataforma que tornam o desenvolvimento e a execução de software possíveis.

O trabalho de SLeeLa é organizado em quatro dimensões: **Math** (o que pode ser calculado), **Object** (as entidades descritas), **Science** (o estudo disciplinado do que pode ser verificado) e **Reach** (a interação com outros sistemas e pessoas). Interesses nacionais pertinentes permanecem registrados como metadados explícitos; não são eliminados pelo uso dessas quatro dimensões.

## I. Comece aqui

- [Compilar a implementação nativa](impl/README.md).
- [Máquinas virtuais e Modium: guia prático](virtual-machine/USER-GUIDE.md), incluindo os comandos `sleela vm` e `sleela modium`.
- [Arquitetura de CPU em código SLeeLa](lib/cpu/CPU.md): das portas lógicas e da ALU aos núcleos, ao sistema operacional e aos programas. O catálogo inclui mais de 130 perfis de CPU; consulte também [temporização, tensões e frequências](lib/cpu/TIMING.md).
- [Índice da documentação](docs/README.md).
- [Documentação de API gerada](docs/api/README.md) e [catálogo de APIs](api/README.md).
- [Padrões de comparação computacional](comparison/README.md): tipos, memória, ABI e conformidade entre SLeeLa VM, Java/JVM e C/C++ nativo.
- [Recursion stanchion: protótipos de referência em C e C++](recursion/stanchion/README.md).
- [Matriz de plataformas e validação](docs/PLATFORM-SUPPORT.md).
- [Guia de marca](docs/BRAND-GUIDE.md).
- [Política de testes e critérios de lançamento](markdown/TESTING.md).

## II. Compilação e testes

**Pré-requisitos:** toolchain C/C++ (GCC ou Clang), GNU Make e Python 3 para ferramentas de manifesto e fluxo de trabalho. No Linux, produtos gráficos podem exigir pacotes de desenvolvimento X11/Xrandr. Consulte a documentação de cada módulo para obter os requisitos específicos.

Em uma cópia limpa do repositório, execute na raiz:

```sh
make
```

Para compilar somente o núcleo nativo e executar a suíte completa:

```sh
make -C impl all
bash ./test-suites/run-all.sh --all
```

Alvos comuns do Makefile raiz:

```sh
make all
sudo make install
make clean
make help
make registry
make tests
make ballum
```

O alvo raiz `make` coordena a compilação dos produtos distribuídos pelo repositório. O alvo `registry` compila o núcleo do registro e materializa sua tabela de referência; consulte [o guia do Registry](registry/README.md) e a [arquitetura Registry 3.0](registry/REGISTRY-3.0-ARCHITECTURE.md).

Uma compilação bem-sucedida de um componente não comprova que todas as bibliotecas, plataformas ou configurações estejam prontas para lançamento. A validação deve ser avaliada por módulo e por plataforma. Verificações ignoradas não devem ser relatadas como aprovadas.

## III. Instalação

Depois da compilação, instale os executáveis e os arquivos de suporte do ambiente de execução:

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

A instalação convencional utiliza normalmente `/usr/local/bin`, `/usr/local/lib/sleela` e `/usr/local/share/sleela`. Para uma instalação restrita ao usuário, `PREFIX="$HOME/.local"` normalmente dispensa `sudo`. `DESTDIR` permite preparar uma árvore de empacotamento sem instalar diretamente no sistema.

O instalador preserva a configuração de inicialização existente e evita sobrescrever comandos que não estejam sob seu controle. A desinstalação deve se limitar aos arquivos registrados pelo instalador, preservando configurações e dados que não sejam gerenciados por ele.

Para uma instalação guiada somente do pacote principal — executáveis `sleela`, `sleelvac` e `nordshrift`, biblioteca padrão, protocolos e módulos de suporte — execute:

```sh
make clean-install
```

Consulte [Quick and Safe Install](install/README.md) para conhecer o fluxo guiado, os escopos de instalação e as opções disponíveis.

## IV. Linguagem e ambiente de execução

A gramática normativa está em [`SLEELA.syntax`](SLEELA.syntax). A documentação atual do repositório corresponde à sintaxe **3.0**. A versão do compilador e a versão da sintaxe declarada pelo código-fonte são conceitos diferentes. A versão 3.0 consolida a superfície da linguagem das versões 1.0 a 2.9 e preserva os requisitos mínimos de versão de cada recurso.

Entre os recursos documentados estão:

- **Marcadores Preonem:** marcadores documentais declarativos como `author`, `authorial`, `material`, `memory`, `determine`, `object`, `field`, `pure`, `gasoline` e `artifact`. Sua interpretação depende da posição contextual e da versão da sintaxe; fora dessa posição, os identificadores mantêm seu uso comum.
- **Campos consequentes `@mix`:** uma classe pode agrupar campos de instância com o mesmo nome quando cada declaração usa `@mix`. O grupo é tratado como uma sequência indexada; os índices podem ser automáticos ou declarados manualmente e devem corresponder à quantidade de campos.
- **Famílias de inteiros estendidos:** a documentação inclui `long`, `int64`, `int128` e as famílias de inteiros com e sem sinal descritas na gramática. A capacidade efetiva de cada tipo deve ser consultada na especificação, e não inferida apenas pelo nome.
- **Comentários angulares:** os formatos `< ... >` e `<< ... >>` são documentados como estilos de comentário da linguagem.
- **Literais com radix:** inteiros aceitam notação hexadecimal, binária e octal com separadores de dígitos, conforme a gramática.
- **Anotações de temporização:** `@loopDelay` e `@loopBudget` expressam pausas e orçamentos de tempo para loops; o opcode `OP_SLEEP` e a função `sleep(ms)` fazem parte da implementação documentada.
- **SleelaDoc:** convenção de documentação no código-fonte e ferramentas para gerar páginas Markdown/HTML, índices de tópicos e documentação de API. Consulte [SleelaDoc](lib/sleeladoc/README.md) e [a documentação gerada](sleeladoc-api/README.md).
- **Nordshrift:** ferramentas que usam o índice recursivo de `/lib` para resolver pacotes e unidades de código-fonte.

A gramática é a autoridade para a sintaxe. Não se deve presumir que um recurso esteja disponível em todos os compiladores apenas porque aparece em uma seção descritiva.

## V. Arquitetura, bibliotecas e ferramentas

SLeeLa inclui implementação nativa, compilador, runtime, máquinas virtuais, ferramentas de documentação e uma biblioteca organizada por pacotes. Entre os projetos descritos no README principal estão:

- **Advantage** (`lib/advantage/` e `advantage/`): modelo em camadas para analisar um dividendo em relação a um modelo. Suas áreas incluem matemática de negociação bilateral, uma analogia de campos de confiança em física, integração vertical, avaliação de sofisticação e emissão de resultados. As analogias de modelagem são identificadas como tais; consulte [a documentação Advantage](lib/advantage/README.md).
- **XML DOM** (`lib/xml/`): árvore de nós XML, atributos ordenados, análise e serialização em formatos compacto e legível. Consulte [XML](lib/xml/README.md).
- **CPU** (`lib/cpu/`): modelos, perfis, execução e parâmetros de temporização.
- **Ballum** (`ballum/` e `lib/ballum/`): controle de tela em nível de pixel, regiões, brilho, gamma e entrada de ponteiro/teclado. A disponibilidade depende do backend e do sistema operacional; consulte [Ballum](ballum/README.md).
- **AutoCAD Creator™** (`autocad/creator/` e `lib/autocad/`): aplicação CAD 2D interativa escrita em SLeeLa. Permite criar e alterar entidades, camadas, cotas e notas, desfazer operações e salvar desenhos DXF. A interface apresenta o documento; o modelo de edição mantém a autoridade sobre a geometria e o histórico. Consulte [AutoCAD Creator](autocad/creator/README.md).
- **Alphabet e UTF-4088** (`alphabet/`, `lib/alphabet/native/`, `utf-4088/`): catálogo de caracteres e ferramentas experimentais. UTF-4088 usa um espaço próprio de identificadores e **não é compatível com Unicode**; seus identificadores não devem ser confundidos com valores escalares Unicode.
- **Pi Generator** (`pi-generator/`): catálogo de constantes de π e documentação de verificação e reprodução.
- **Modium e Registry:** subsistemas de registro e ferramentas de inspeção; consulte [Registry](registry/README.md).
- **Philosophy** (`philosophy/` e `lib/philosophy/`): fundamentos nativos e módulos de dados, assistência e conversação. O componente de conversação é descrito como não clínico, com entradas explícitas e baseado em consentimento; não diagnostica nem monitora pessoas passivamente.
- **CDMA, OFDMA, HTTP e Wi-Fi:** diretórios técnicos e bibliotecas versionadas. O escopo e o estado de validação devem ser conferidos na documentação de cada pacote.

O índice de pacotes é descritivo. Para conhecer a funcionalidade real, as dependências e o estado dos testes de um módulo, consulte suas fontes, README e testes específicos.

## VI. C, C++ e compilação nativa

O repositório mantém componentes nativos em C e C++, além da implementação SLeeLa. A fronteira entre o código nativo e o runtime deve respeitar os contratos de ABI documentados. A anotação `@native` e o comando `sleela aot` representam uma etapa inicial do caminho de compilação antecipada: identificam métodos anotados, verificam se pertencem a um subconjunto compatível e podem emitir C/C++ para esse subconjunto.

```sh
sleela aot --list <file.sleela>
sleela aot --emit <file.sleela>
```

A verificação é deliberadamente conservadora: construções que não podem ser traduzidas são recusadas com uma justificativa, em vez de produzir uma tradução parcial enganosa. A anotação `@native` não altera, por si só, o comportamento do programa. Compilar o código emitido como biblioteca compartilhada e despachá-lo a partir da VM são etapas posteriores; não devem ser consideradas concluídas sem evidências de implementação e testes. Consulte [AT-NATIVE](pi-generator/AT-NATIVE.md).

## VII. Integridade, segurança e verificação

O projeto documenta manifestos SHA-256, verificações locais de integridade e um adaptador opcional para ClamAV por meio do Sleelavirin. A presença de uma ferramenta ou de um manifesto não constitui, isoladamente, uma certificação de segurança.

Os princípios de engenharia incluem:

1. Contratos explícitos entre componentes.
2. Portabilidade considerada desde o projeto.
3. Comportamento seguro quando uma operação falha.
4. Evidências reproduzíveis e vinculadas ao commit avaliado.
5. Separação entre identificar uma referência e autorizar uma execução.
6. Relatórios precisos: testes não executados, ignorados ou bloqueados devem ser indicados como tais.

Para avaliar um lançamento, examine os resultados de CI do commit específico, os artefatos gerados, os testes pertinentes e a matriz de plataformas. Não extrapole um resultado de Linux para Windows ou macOS sem evidência específica.

## VIII. Editores e experiência de desenvolvimento

SLeeLa oferece integração de sintaxe e temas para **Vim** e **Emacs**, além de uma ferramenta nativa de paletas C/C++. Consulte [o guia de integração de editores](linux-editors/README.md). A documentação de cores e utilitários descreve perfis e opções de linha de comando; resultados de compilação e testes devem ser verificados no sistema de destino.

## IX. Contribuição

1. Leia o [índice da documentação](docs/README.md) e a [política de testes](markdown/TESTING.md).
2. Faça alterações de escopo definido e acrescente testes positivos e negativos.
3. Execute primeiro os testes específicos e depois a suíte mais ampla pertinente.
4. Informe o sistema operacional, o compilador e a toolchain utilizados.
5. Não descreva verificações ignoradas como aprovadas.

SLeeLa é desenvolvido pela **MEARVK LLC**. O material histórico da página inicial está preservado em [README-LEGACY](docs/README-LEGACY.md).

## X. Iniciativas e declarações do projeto

O README principal registra iniciativas e expectativas do projeto que devem ser lidas em seu contexto. Por exemplo, a seção **AE6E66 — Round Public Email Levy Expectation** declara uma expectativa proposta de valor por e-mail Round Public. O próprio documento esclarece que isso não é lançamento tributário legal, ordem governamental, decisão judicial nem obrigação fiscal estabelecida. Qualquer cobrança real exigiria autoridade legal aplicável, jurisdição, definições, procedimento e determinação da autoridade governamental competente.

Referências a iniciativas, marcas ou propostas dentro do repositório não devem ser interpretadas como endosso oficial de governos, organismos internacionais ou terceiros.

## XI. Alfabeto e runtime de caracteres

A documentação do alfabeto e o runtime nativo de caracteres são mantidos em `alphabet/sleela/` e `lib/alphabet/native/`. Os testes nativos C++17 cobrem decodificação e codificação estritas de escalares UTF-8, assinaturas de capacidade `C<n>` e associação limitada a conjuntos de caracteres:

```sh
make -C lib/alphabet/native test
```

A integração experimental UTF-4088 é documentada separadamente. Seus identificadores não são escalares Unicode, e o trabalho de ligação com o compilador e o SLVM é acompanhado independentemente dos fundamentos nativos.

## XII. AutoCAD Creator™

AutoCAD Creator™ é uma aplicação interativa de desenho CAD 2D escrita em SLeeLa. Permite criar e alterar documentos em tempo real, adicionar paredes, cômodos, cotas e notas, mover ou renomear itens e alterar camadas, excluir elementos, desfazer operações e salvar desenhos DXF de texto R12. A interface usa SleelaUI™ e pode abrir janelas nativas por X11, Cocoa ou Win32, conforme o sistema.

A regra arquitetural documentada é: **“SleelaUI apresenta; SLeeLa decide.”** A interface não é a autoridade geométrica nem a responsável por gravar o DXF; as intenções de edição são enviadas ao modelo `SLCadCreator`, que mantém o documento e o histórico reversível.

```sh
make creator
```

Execute o comando a partir de `autocad/`. Em um ambiente gráfico, a janela de edição é aberta; em um ambiente sem display, o exemplo pode construir o desenho em memória, emitir um relatório e encerrar.

## XIII. Ordenação de mapas de entrada UTF-4088

O catálogo especializado documenta o percurso espacial de mapas de entrada e iterações abstratas de pressão. Coordenadas, níveis de pressão (1–50), identificadores permanentes do catálogo e posições alfabéticas são campos independentes. Pressão é um nível de iteração algorítmica, não uma grandeza física. Consulte [Input Map and Pressure](utf-4088/INPUT-MAP-AND-PRESSURE.md) e [National Lien Register](utf-4088/NATIONAL-LIEN-REGISTER.md).

## XIV. Índice ampliado de pacotes

O README principal mantém um índice alfabético de pacotes e coleções de código. Entre os temas listados estão Advantage, Alphabet/UTF-4088, AutoCAD Creator, Ballum, CDMA, Character, Coorenagraph, Governance, HTTP, International Criminal Court, OFDMA, Philosophy, Synchro, SleelaUI, United Nations e Wi-Fi 4/5/6.

As descrições do índice indicam localizações e finalidades gerais; não substituem a documentação do pacote. Quando um pacote é ampliado, sua entrada deve apontar para o diretório e README canônicos e descrever apenas capacidades sustentadas pelo código disponível.

## XV. Bitcoin Conjegeum e material gráfico

O README inclui uma seção intitulada **Bitcoin Conjegeum**, com endereços e material gráfico. A presença de endereços ou imagens no repositório não constitui recomendação financeira, verificação de titularidade nem instrução para transferir fundos. Verifique qualquer informação de forma independente antes de utilizá-la.

## XVI. Imagens de identidade científica

O repositório contém imagens de marca e material visual relacionado à SLeeLa Science. Esses recursos são elementos gráficos do projeto; não devem ser interpretados como prova de afiliação, certificação ou aprovação institucional.

## XVII. Catálogo experimental UTF-4088 com um milhão de entradas

O gerador [UTF-4088](utf-4088/tools/generate_catalogue.py) produz deterministicamente um catálogo experimental de **1.048.576 entradas**. O formato compacto está descrito em [FORMAT](utf-4088/FORMAT.md), e o contrato de geração e validação em [GENERATION-RESULTS](utf-4088/GENERATION-RESULTS.md).

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

O binário gerado ocupa 16.777.244 bytes e foi projetado para permanecer abaixo de 20.000.000 bytes. Testes unitários e um workflow do GitHub Actions verificam a quantidade de entradas, a unicidade dos glifos, o tamanho e o SHA-256. O binário é publicado como artefato do workflow e mantido fora do histórico Git. UTF-4088 continua experimental e não é compatível com Unicode.

## XVIII. Adições recentes: Pi, Modium, Registry, sleelactl e `@native`

O README registra adições recentes ao repositório:

### Catálogo de π com cem milhões de dígitos

O repositório inclui um valor decimal de π com **100.000.000 de dígitos** e outro com 57.344 dígitos. [VERIFIED](pi-generator/VERIFIED.md) descreve as verificações e o procedimento de reprodução. A verificação deve ser atribuída aos controles documentados, não apenas à afirmação de tamanho.

### Inicialização do Modium e do Registry

Nos pontos de entrada específicos `sleela registry` e `sleela modium`, e na primeira execução do instalador, os subsistemas carregam o catálogo grande de π. A mensagem temporária de carregamento aparece apenas em terminais interativos; saídas redirecionadas e de testes devem permanecer inalteradas. O registro conserva localizadores e metadados de referência, não uma cópia implícita de todos os dados.

### `sleelactl` e o subject nativo `math`

O instalador guiado instala `sleelactl` e seu registro de módulos. O plano de controle inclui a natureza `subject` para bibliotecas nativas C/C++ integradas ao runtime, com `math` como primeiro subject descrito. `sleelactl run math` executa a demonstração de referência do backend nativo; `sleelactl info math` explica como ele é acessado a partir de SLeeLa.

### `@native` e `sleela aot`

A anotação `@native` solicita compilação antecipada de métodos que pertençam a um subconjunto compatível. `sleela aot --list` informa o resultado por método e `sleela aot --emit` emite C/C++ para os casos aceitos. A função é conservadora e recusa construções incompatíveis; esta etapa inicial não deve ser confundida com despacho nativo completo pela VM.

## XIX. Edições internacionais do README

O repositório mantém edições por país no padrão `README.<CC>.md`. Quando um país utiliza mais de um idioma, várias seções linguísticas podem ser agrupadas no mesmo arquivo. A cobertura indicada é representativa e não significa que todos os idiomas regionais, indígenas, oficiais ou de sinais estejam traduzidos.

**Política de tradução:** nomes técnicos, identificadores, comandos, caminhos, links e exemplos de código são preservados para manter as instruções utilizáveis. Uma edição só deve ser descrita como tradução integral quando todas as seções tiverem sido traduzidas e revisadas linguisticamente e tecnicamente. Esta edição em português brasileiro oferece uma referência extensa e localizada; o README canônico em inglês continua sendo a referência normativa quando uma formulação técnica ainda não tiver sido traduzida ou revisada.

## Referências principais

- [README canônico](README.md)
- [Especificação da sintaxe SLeeLa](SLEELA.syntax)
- [Índice da documentação](docs/README.md)
- [Documentação de API](docs/api/README.md)
- [Suporte a plataformas](docs/PLATFORM-SUPPORT.md)
- [Política de testes](markdown/TESTING.md)
- [Repositório SLeeLa](https://github.com/mearvk/SLeeLa)

**Status desta edição:** as seções principais de apresentação, compilação, instalação, linguagem, arquitetura, segurança, contribuição e os destaques técnicos do README foram desenvolvidas em português brasileiro. A revisão por especialistas e a comparação final com cada alteração do README canônico ainda são necessárias.
