# SLeeLa 3.0 — Brasil (BR)

## Português brasileiro

SLeeLa é uma linguagem nativa de programação de sistemas, acompanhada de compilador, ambiente de execução e ecossistema de máquinas virtuais. Esta edição apresenta o projeto ao público brasileiro.

**The Now. The Ingredient.** Uma linguagem é mais do que sua sintaxe: ela também inclui o compilador, o ambiente de execução, as bibliotecas, as ferramentas e os contratos de plataforma que tornam o software utilizável.

## I. Comece aqui

- [Compilar a implementação nativa](impl/README.md)
- [Máquinas virtuais e Modium — guia prático](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [Pilha de CPU em código SLeeLa](lib/cpu/CPU.md) — uma cadeia completa de portas lógicas, ALU, núcleos, sistema operacional e programas; inclui mais de 130 perfis de CPU e ajustes de tensão, frequência e temporização em [TIMING.md](lib/cpu/TIMING.md).
- [Índice da documentação](docs/README.md)
- [Documentação de API gerada](docs/api/README.md) e [catálogo de APIs](api/README.md)
- [Padrões de comparação computacional](comparison/README.md) — tipos, memória, ABI e conformidade para SLeeLa VM, Java/JVM e C/C++ nativo.
- [Recursion stanchion — protótipos de referência em C e C++](recursion/stanchion/README.md)
- [Matriz de suporte e validação de plataformas](docs/PLATFORM-SUPPORT.md)
- [Guia da marca](docs/BRAND-GUIDE.md)
- [Política de testes e critérios de lançamento](markdown/TESTING.md)

## II. Compilação e testes

**Pré-requisitos:** uma cadeia de ferramentas C/C++ (GCC ou Clang), GNU Make e Python 3 para as ferramentas de manifesto e fluxo de trabalho. No Linux, os produtos gráficos também podem exigir os pacotes de desenvolvimento X11/Xrandr.

Em uma cópia limpa do repositório, o comando inicial é:

```sh
make
```

Para compilar somente o núcleo nativo e executar a suíte completa de testes:

```sh
make -C impl all
bash ./test-suites/run-all.sh --all
```

O `make` executado na raiz constrói os produtos distribuídos pelo repositório. Alguns alvos essenciais são:

```sh
make all
sudo make install
make clean
make help
```

Consulte os guias de compilação e testes para conhecer todos os alvos, os requisitos específicos de cada plataforma e os limites da validação. A compilação bem-sucedida de um componente não comprova que todas as bibliotecas ou plataformas estejam prontas para lançamento.

## III. Instalação

Depois da compilação, instale os comandos e os arquivos de suporte do ambiente de execução:

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

Por padrão, o instalador convencional utiliza `/usr/local/bin`, `/usr/local/lib/sleela` e `/usr/local/share/sleela`. Para uma instalação somente do usuário, `PREFIX="$HOME/.local"` normalmente dispensa `sudo`. O instalador preserva uma configuração de inicialização existente e se recusa a sobrescrever comandos existentes que não estejam sob seu controle.

Para instalar somente o pacote principal — os executáveis `sleela`, `sleelvac` e `nordshrift`, a biblioteca padrão, os protocolos e os módulos de suporte — execute:

```sh
make clean-install
```

Consulte o [guia de instalação rápida e segura](install/README.md) para conhecer o fluxo guiado e as demais opções.

## IV. Linguagem e ambiente de execução

A gramática normativa está em [`SLEELA.syntax`](SLEELA.syntax). O repositório documenta a sintaxe **3.0**. A versão do compilador e a versão da sintaxe do código-fonte são distintas. A sintaxe 3.0 consolida a superfície da linguagem das versões 1.0 a 2.9 e preserva os requisitos mínimos de versão de cada recurso.

A documentação técnica descreve, entre outros assuntos, os campos consequentes `@mix`, as famílias de inteiros estendidos, os marcadores documentais Preonem, o SleelaDoc, a geração de documentação de API e as ferramentas Nordshrift. Identificadores técnicos, nomes de arquivos, comandos e exemplos de código permanecem inalterados para que possam ser usados diretamente.

## V. Princípios de engenharia e validação

Os princípios do projeto incluem priorizar evidências, definir contratos explícitos nas fronteiras dos componentes, projetar com portabilidade desde o início, adotar comportamento seguro em caso de falha e produzir lançamentos reproduzíveis.

A validação deve ser avaliada por módulo e por plataforma. Verificações ignoradas não devem ser apresentadas como aprovadas; a compilação isolada de um componente não equivale à validação completa de um lançamento.

## VI. Editores e contribuições

SLeeLa oferece temas e integração de sintaxe para **Vim** e **Emacs**, além de um utilitário nativo de paletas C/C++. As instruções estão no [guia de integração com editores](linux-editors/README.md).

Para contribuir:

1. Leia o índice da documentação e a política de testes.
2. Faça alterações focadas, acompanhadas de testes positivos e negativos.
3. Execute os testes direcionados e, em seguida, a suíte mais ampla.
4. Informe o sistema operacional e a cadeia de ferramentas utilizados; não apresente verificações ignoradas como aprovadas.

SLeeLa é desenvolvido pela **MEARVK LLC**.

## VII. Outros tópicos documentados

O README principal também apresenta as bibliotecas Advantage, SleelaDoc e XML DOM; as primitivas de inteiros; o runtime de caracteres; o AutoCAD Creator™; o catálogo experimental UTF-4088; as ferramentas Pi, Modium, Registry e `sleelactl`; e a etapa inicial do caminho de compilação nativa solicitado por `@native`.

## Referência técnica completa

O [README.md canônico](README.md) contém o catálogo completo de recursos, a referência da linguagem e do ambiente de execução, as instruções de compilação e instalação, os links de API, os exemplos e as notas de versão. Esta edição brasileira amplia a apresentação e as instruções essenciais em português, mas ainda não é uma tradução integral, linha por linha, das seções técnicas I–XIX. O README em inglês permanece como referência normativa até que cada tradução completa seja revisada.

- [README canônico](README.md)
- [Especificação da sintaxe SLeeLa](SLEELA.syntax)
- [Índice da documentação](docs/README.md)
- [Suporte a plataformas](docs/PLATFORM-SUPPORT.md)
- [Repositório no GitHub](https://github.com/mearvk/SLeeLa)

**Status da tradução:** apresentação, início rápido, compilação, instalação, princípios de engenharia e contribuições localizados em português brasileiro. A tradução integral do README canônico ainda precisa ser concluída e revisada por especialistas técnicos fluentes em português.
