# SLeeLa 3.0 — Chile (CL)

## Edición en español

SLeeLa es un lenguaje nativo de programación de sistemas, acompañado de compilador, entorno de ejecución y un ecosistema de máquinas virtuales. Esta edición está dirigida a lectores de Chile. Conserva los nombres técnicos, identificadores, comandos, rutas, ejemplos de código y nombres de API para que las instrucciones puedan utilizarse directamente.

**The Now. The Ingredient.** Un lenguaje es más que su sintaxis: comprende el compilador, el entorno de ejecución, las bibliotecas, las herramientas y los contratos de plataforma que hacen posible desarrollar y ejecutar software.

SLeeLa organiza su trabajo en cuatro dimensiones: **Math** (lo calculable), **Object** (las entidades descritas), **Science** (el estudio disciplinado de lo verificable) y **Reach** (la interacción con otros sistemas y personas). Los intereses nacionales que correspondan a un tema se conservan como metadatos explícitos; no se eliminan por el uso de estas cuatro dimensiones.

## I. Primeros pasos

- [Compilar la implementación nativa](impl/README.md).
- [Máquinas virtuales y Modium: guía práctica](virtual-machine/USER-GUIDE.md), incluidos los comandos `sleela vm` y `sleela modium`.
- [Arquitectura de CPU en código SLeeLa](lib/cpu/CPU.md): recorrido desde las puertas lógicas y la ALU hasta los núcleos, el sistema operativo y los programas. El catálogo incluye más de 130 perfiles de CPU; consulte también [temporización, voltajes y frecuencias](lib/cpu/TIMING.md).
- [Índice de documentación](docs/README.md).
- [Documentación API generada](docs/api/README.md) y [catálogo de API](api/README.md).
- [Estándares de comparación computacional](comparison/README.md): tipos, memoria, ABI y conformidad entre SLeeLa VM, Java/JVM y C/C++ nativo.
- [Recursion stanchion: prototipos de referencia C y C++](recursion/stanchion/README.md).
- [Matriz de plataformas y validación](docs/PLATFORM-SUPPORT.md).
- [Guía de marca](docs/BRAND-GUIDE.md).
- [Política de pruebas y criterios de publicación](markdown/TESTING.md).

## II. Compilación y pruebas

**Requisitos previos:** una cadena de herramientas C/C++ (GCC o Clang), GNU Make y Python 3 para las herramientas de manifiestos y flujos de trabajo. En Linux, los productos gráficos pueden requerir los paquetes de desarrollo de X11/Xrandr. Consulte la documentación de cada módulo para conocer sus dependencias específicas.

Desde una copia limpia del repositorio, ejecute en la raíz:

```sh
make
```

Para compilar únicamente el núcleo nativo y ejecutar la suite completa:

```sh
make -C impl all
bash ./test-suites/run-all.sh --all
```

Objetivos habituales del Makefile raíz:

```sh
make all
sudo make install
make clean
make help
make registry
make tests
make ballum
```

El objetivo raíz `make` distribuye la compilación entre los productos del repositorio. El objetivo `registry` compila el núcleo del registro y materializa su tabla de referencia; consulte [la guía del Registry](registry/README.md) y la [arquitectura Registry 3.0](registry/REGISTRY-3.0-ARCHITECTURE.md). Para instalar o compilar módulos de forma guiada, consulte `bin/` y la documentación de instalación.

Una compilación satisfactoria de un componente no demuestra que todas las bibliotecas, plataformas o configuraciones estén listas para una publicación. La validación debe evaluarse por módulo y plataforma. Las comprobaciones omitidas no deben comunicarse como aprobadas.

## III. Instalación

Después de compilar, instale los ejecutables y los archivos de soporte del entorno de ejecución:

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

La instalación convencional utiliza normalmente `/usr/local/bin`, `/usr/local/lib/sleela` y `/usr/local/share/sleela`. Para una instalación limitada al usuario, `PREFIX="$HOME/.local"` suele evitar la necesidad de `sudo`. `DESTDIR` permite preparar un árbol de empaquetado sin instalar directamente en el sistema.

El instalador conserva la configuración de inicio existente y evita sobrescribir comandos no administrados por él. La desinstalación se limita a los archivos registrados por el instalador y debe preservar la configuración y los datos ajenos a su gestión.

Para una instalación guiada del paquete principal —los ejecutables `sleela`, `sleelvac` y `nordshrift`, la biblioteca estándar, los protocolos y los módulos de soporte— utilice:

```sh
make clean-install
```

Consulte [Quick and Safe Install](install/README.md) para conocer el flujo guiado, los ámbitos de instalación y las opciones disponibles.

## IV. Lenguaje y entorno de ejecución

La gramática normativa se encuentra en [`SLEELA.syntax`](SLEELA.syntax). La documentación actual del repositorio corresponde a la sintaxis **3.0**. La versión del compilador y la versión de sintaxis declarada por el código fuente son conceptos distintos. La versión 3.0 reúne la superficie del lenguaje de las versiones 1.0 a 2.9 y conserva los requisitos mínimos de versión de cada característica.

Entre las características documentadas se encuentran:

- **Marcadores Preonem:** marcadores documentales declarativos como `author`, `authorial`, `material`, `memory`, `determine`, `object`, `field`, `pure`, `gasoline` y `artifact`. Su interpretación depende de la posición contextual y de la versión de sintaxis; fuera de esa posición, los identificadores conservan su uso ordinario.
- **Campos consecuentes `@mix`:** una clase puede agrupar campos de instancia con el mismo nombre cuando cada declaración lleva `@mix`. El grupo se entiende como una secuencia indexada; los índices pueden ser automáticos o declarados manualmente, y deben corresponder al número de campos.
- **Familias de enteros ampliados:** las referencias incluyen `long`, `int64`, `int128`, además de las familias de enteros con signo y sin signo documentadas en la gramática. La capacidad real de cada tipo debe consultarse en la especificación y no inferirse únicamente del nombre.
- **Comentarios de ángulo:** los formatos `< ... >` y `<< ... >>` se documentan como estilos de comentario del lenguaje.
- **Literales con radix:** los enteros admiten notación hexadecimal, binaria y octal con separadores de dígitos, conforme a la gramática.
- **Anotaciones de temporización:** `@loopDelay` y `@loopBudget` permiten expresar pausas y presupuestos temporales para bucles; el opcode `OP_SLEEP` y la función `sleep(ms)` forman parte de la implementación descrita.
- **SleelaDoc:** convención de documentación en el código fuente y herramientas para generar páginas Markdown/HTML, índices de temas y documentación API. Consulte [SleelaDoc](lib/sleeladoc/README.md) y [la documentación generada](sleeladoc-api/README.md).
- **Nordshrift:** herramientas que utilizan el índice recursivo de `/lib` para resolver paquetes y unidades de código fuente.

La gramática es la autoridad para la sintaxis. No se debe asumir que una característica está disponible en todos los compiladores solo porque aparezca en una sección descriptiva.

## V. Arquitectura, bibliotecas y herramientas

SLeeLa incluye una implementación nativa, compilador, runtime, máquinas virtuales, herramientas de documentación y una biblioteca organizada por paquetes. Entre los proyectos descritos en el README principal se encuentran:

- **Advantage** (`lib/advantage/` y `advantage/`): modelo por capas para analizar un dividendo respecto de un modelo. Sus áreas incluyen matemáticas de negociación bilateral, una analogía de campos de confianza en física, integración vertical, evaluación de sofisticación y emisión de resultados. Las analogías de modelado se identifican como tales; consulte [la documentación Advantage](lib/advantage/README.md).
- **XML DOM** (`lib/xml/`): árbol de nodos XML, atributos ordenados, análisis y serialización con formatos compacto y legible. Consulte [XML](lib/xml/README.md).
- **CPU** (`lib/cpu/`): modelos, perfiles, ejecución y parámetros de temporización.
- **Ballum** (`ballum/` y `lib/ballum/`): control de pantalla a nivel de píxel, regiones, brillo, gamma y entrada de puntero/teclado. La disponibilidad depende del backend y del sistema operativo; consulte [Ballum](ballum/README.md).
- **AutoCAD Creator™** (`autocad/creator/` y `lib/autocad/`): aplicación CAD 2D interactiva escrita en SLeeLa. Permite crear y modificar entidades, capas, cotas y notas, deshacer operaciones y guardar dibujos DXF. La interfaz presenta el documento; el modelo de edición mantiene la autoridad sobre la geometría y el historial. Consulte [AutoCAD Creator](autocad/creator/README.md).
- **Alphabet y UTF-4088** (`alphabet/`, `lib/alphabet/native/`, `utf-4088/`): catálogo de caracteres y herramientas experimentales. UTF-4088 utiliza un espacio de identificadores propio y **no es compatible con Unicode**; no deben confundirse sus identificadores con valores escalares Unicode.
- **Pi Generator** (`pi-generator/`): catálogo de constantes de π y documentación de verificación y reproducción.
- **Modium y Registry**: subsistemas de registro y herramientas de inspección; consulte [Registry](registry/README.md).
- **Philosophy** (`philosophy/` y `lib/philosophy/`): fundamentos nativos y módulos de datos, ayuda y conversación. El componente de conversación se describe como no clínico, explícito en sus entradas y basado en consentimiento; no diagnostica ni supervisa personas de forma pasiva.
- **CDMA, OFDMA, HTTP y Wi-Fi**: carpetas técnicas y bibliotecas versionadas. El alcance y el estado de validación deben verificarse en la documentación de cada paquete.

El índice de paquetes es descriptivo. Para conocer la funcionalidad real, las dependencias y el estado de pruebas de un módulo, consulte sus fuentes, README y pruebas específicas.

## VI. C, C++ y compilación nativa

El repositorio mantiene componentes nativos en C y C++ además de la implementación SLeeLa. El límite entre el código nativo y el runtime debe respetar los contratos de ABI documentados. La anotación `@native` y el comando `sleela aot` representan una etapa inicial del camino de compilación anticipada: identifican métodos anotados, comprueban si pertenecen a un subconjunto admitido y pueden emitir C/C++ para ese subconjunto.

```sh
sleela aot --list <file.sleela>
sleela aot --emit <file.sleela>
```

La comprobación es deliberadamente conservadora: las construcciones que no se pueden traducir se rechazan con un motivo, en lugar de generar una traducción parcial engañosa. La anotación `@native` no cambia por sí sola el comportamiento del programa. La compilación del código emitido como biblioteca compartida y su despacho desde la VM son etapas posteriores; no deben darse por completadas sin evidencia de implementación y pruebas. Consulte [AT-NATIVE](pi-generator/AT-NATIVE.md).

## VII. Integridad, seguridad y verificación

El proyecto documenta manifiestos SHA-256, comprobaciones locales de integridad y un adaptador opcional para ClamAV mediante Sleelavirin. La presencia de una herramienta o un manifiesto no constituye por sí sola una certificación de seguridad.

Los principios de ingeniería incluyen:

1. Contratos explícitos entre componentes.
2. Portabilidad considerada desde el diseño.
3. Comportamiento seguro cuando una operación falla.
4. Evidencia reproducible y vinculada al commit evaluado.
5. Separación entre identificar una referencia y autorizar una ejecución.
6. Informes precisos: las pruebas no ejecutadas, omitidas o bloqueadas deben indicarse como tales.

Para evaluar una publicación, revise los resultados de CI del commit concreto, los artefactos generados, las pruebas pertinentes y la matriz de plataformas. No extrapole el resultado de Linux a Windows o macOS sin evidencia específica.

## VIII. Editores y experiencia de desarrollo

SLeeLa proporciona integración de sintaxis y temas para **Vim** y **Emacs**, además de una herramienta nativa de paletas C/C++. Consulte [la guía de integración de editores](linux-editors/README.md). La documentación de colores y utilidades describe perfiles y opciones de línea de comandos; los resultados de compilación y pruebas deben verificarse en el sistema de destino.

## IX. Contribución

1. Lea el [índice de documentación](docs/README.md) y la [política de pruebas](markdown/TESTING.md).
2. Realice cambios acotados y añada pruebas positivas y negativas.
3. Ejecute primero las pruebas específicas y después la suite más amplia pertinente.
4. Indique sistema operativo, compilador y cadena de herramientas utilizados.
5. No describa las comprobaciones omitidas como aprobadas.

SLeeLa es desarrollado por **MEARVK LLC**. El material histórico de la página de inicio se conserva en [README-LEGACY](docs/README-LEGACY.md).

## X. Iniciativas y declaraciones del proyecto

El README principal registra iniciativas y expectativas del proyecto que deben leerse en su contexto. Por ejemplo, la sección **AE6E66 — Round Public Email Levy Expectation** expresa una expectativa propuesta de importe por correo electrónico. El propio documento aclara que no es una liquidación tributaria legal, una orden gubernamental, una sentencia ni una obligación fiscal establecida. Cualquier gravamen real requeriría autoridad legal aplicable, jurisdicción, definiciones, procedimiento y determinación de la autoridad competente.

Las referencias a iniciativas, marcas o propuestas dentro del repositorio no deben interpretarse como respaldo oficial de gobiernos, organismos internacionales o terceros.

## XI. Alfabeto y runtime de caracteres

La documentación del alfabeto y el runtime nativo de caracteres se mantiene en `alphabet/sleela/` y `lib/alphabet/native/`. Las pruebas nativas C++17 cubren la decodificación y codificación estricta de escalares UTF-8, firmas de capacidad `C<n>` y pertenencia acotada a conjuntos de caracteres:

```sh
make -C lib/alphabet/native test
```

La integración experimental UTF-4088 se documenta por separado. Sus identificadores no son escalares Unicode, y el trabajo de enlace con el compilador y SLVM se sigue de manera independiente de los fundamentos nativos.

## XII. AutoCAD Creator™

AutoCAD Creator™ es una aplicación interactiva de dibujo CAD 2D escrita en SLeeLa. Permite crear y alterar documentos en vivo, añadir paredes, habitaciones, cotas y notas, mover o cambiar etiquetas y capas, eliminar elementos, deshacer operaciones y guardar dibujos DXF de texto R12. La interfaz utiliza SleelaUI™ y puede abrir ventanas nativas mediante X11, Cocoa o Win32, según el sistema.

La regla arquitectónica documentada es: **«SleelaUI presenta; SLeeLa decide»**. La interfaz no es la autoridad geométrica ni la encargada de escribir el DXF; las intenciones de edición se envían al modelo `SLCadCreator`, que mantiene el documento y el historial reversible.

```sh
make creator
```

Ejecute el comando desde `autocad/`. En un entorno gráfico se abre la ventana de edición; en un entorno sin pantalla, el ejemplo puede construir el dibujo en memoria, emitir un informe y finalizar.

## XIII. Ordenación de mapas de entrada UTF-4088

El catálogo especializado documenta el recorrido espacial de mapas de entrada y las iteraciones abstractas de presión. Las coordenadas, los niveles de presión (1–50), los identificadores permanentes del catálogo y los rangos alfabéticos son campos independientes. La presión es un nivel de iteración algorítmica, no una magnitud física. Consulte [Input Map and Pressure](utf-4088/INPUT-MAP-AND-PRESSURE.md) y [National Lien Register](utf-4088/NATIONAL-LIEN-REGISTER.md).

## XIV. Índice ampliado de paquetes

El README principal mantiene un índice alfabético de paquetes y colecciones de código. Entre los temas enumerados figuran Advantage, Alphabet/UTF-4088, AutoCAD Creator, Ballum, CDMA, Character, Coorenagraph, Governance, HTTP, International Criminal Court, OFDMA, Philosophy, Synchro, SleelaUI, United Nations y Wi-Fi 4/5/6.

Las descripciones del índice señalan ubicaciones y propósitos generales; no sustituyen la documentación del paquete. Cuando un paquete se amplía, su entrada debe enlazar al directorio y README canónicos y describir únicamente capacidades respaldadas por el código disponible.

## XV. Bitcoin Conjegeum y material gráfico

El README incluye una sección titulada **Bitcoin Conjegeum**, con direcciones y material gráfico. La presencia de direcciones o imágenes en el repositorio no constituye una recomendación financiera, una verificación de titularidad ni una instrucción para transferir fondos. Verifique cualquier dato de forma independiente antes de utilizarlo.

## XVI. Imágenes de identidad científica

El repositorio contiene imágenes de marca y material visual relacionado con SLeeLa Science. Estos recursos son elementos gráficos del proyecto; no deben interpretarse como prueba de afiliación, certificación o aprobación institucional.

## XVII. Catálogo experimental UTF-4088 de un millón de entradas

El generador de [UTF-4088](utf-4088/tools/generate_catalogue.py) produce de forma determinista un catálogo experimental de **1.048.576 entradas**. El formato compacto se describe en [FORMAT](utf-4088/FORMAT.md), y el contrato de generación y validación en [GENERATION-RESULTS](utf-4088/GENERATION-RESULTS.md).

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

El binario generado ocupa 16.777.244 bytes y está diseñado para mantenerse por debajo de 20.000.000 bytes. Las pruebas y un flujo de GitHub Actions verifican el número de entradas, la unicidad de glifos, el tamaño y SHA-256. El binario se publica como artefacto del flujo de trabajo y se mantiene fuera del historial Git. UTF-4088 sigue siendo experimental y no es compatible con Unicode.

## XVIII. Incorporaciones recientes: Pi, Modium, Registry, sleelactl y `@native`

El README registra incorporaciones recientes al repositorio:

### Catálogo de π de cien millones de dígitos

El repositorio incluye un valor decimal de π de **100.000.000 de dígitos** y otro de 57.344 dígitos. [VERIFIED](pi-generator/VERIFIED.md) describe las comprobaciones y el procedimiento de reproducción. La verificación debe atribuirse a los controles documentados, no a una mera afirmación de tamaño.

### Inicio de Modium y Registry

En los puntos de entrada específicos `sleela registry` y `sleela modium`, y durante el primer inicio del instalador, los subsistemas cargan el catálogo grande de π. El mensaje transitorio de carga solo se muestra en terminales interactivos; la salida redirigida y la de pruebas deben permanecer sin alteraciones. El registro conserva localizadores y metadatos de referencia, no una copia implícita de todos los datos.

### `sleelactl` y el sujeto nativo `math`

El instalador guiado instala `sleelactl` y su registro de módulos. El plano de control incorpora la naturaleza `subject` para bibliotecas nativas C/C++ integradas al runtime, con `math` como primer sujeto descrito. `sleelactl run math` ejecuta la demostración de referencia del backend nativo; `sleelactl info math` explica cómo se accede a él desde SLeeLa.

### `@native` y `sleela aot`

La anotación `@native` solicita compilación anticipada de métodos que pertenezcan a un subconjunto compatible. `sleela aot --list` informa el veredicto por método y `sleela aot --emit` emite C/C++ para los casos admitidos. La función es conservadora y rechaza construcciones no compatibles; no se debe confundir esta etapa inicial con un despacho nativo completo desde la VM.

## XIX. Ediciones internacionales del README

El repositorio mantiene ediciones por país con el patrón `README.<CC>.md`. Cuando un país utiliza más de un idioma, pueden agruparse varias secciones lingüísticas en el mismo archivo. La cobertura lingüística indicada por una edición es representativa y no implica que todos los idiomas regionales, indígenas, oficiales o de señas estén traducidos.

**Política de traducción:** los nombres técnicos, identificadores, comandos, rutas, enlaces y ejemplos de código se conservan para que las instrucciones sigan siendo utilizables. Una edición solo debe describirse como traducción integral cuando todas las secciones hayan sido traducidas y revisadas lingüística y técnicamente. Esta edición en español ofrece una referencia extensa y localizada; el README canónico en inglés continúa siendo la referencia normativa cuando una formulación técnica no esté traducida o revisada.

## Referencias principales

- [README canónico](README.md)
- [Especificación de sintaxis SLeeLa](SLEELA.syntax)
- [Índice de documentación](docs/README.md)
- [Documentación API](docs/api/README.md)
- [Soporte de plataformas](docs/PLATFORM-SUPPORT.md)
- [Política de pruebas](markdown/TESTING.md)
- [Repositorio SLeeLa](https://github.com/mearvk/SLeeLa)

**Estado de esta edición:** se han desarrollado en español las secciones principales de introducción, compilación, instalación, lenguaje, arquitectura, seguridad, contribución y los temas técnicos destacados del README. La revisión por especialistas y la comparación final con cada cambio del README canónico siguen siendo necesarias.
