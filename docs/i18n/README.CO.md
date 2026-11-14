<img align="right" src="../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — Colombia (CO)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>Un lenguaje nativo de sistemas, con compilador, entorno de ejecución y ecosistema de máquinas virtuales.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="../../docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **Edición internacional por país.** Esta edición para lectores de Colombia se ofrece en español. Los identificadores técnicos, comandos, ejemplos de código, rutas y enlaces no se modifican.

**The Now. The Ingredient.** Un lenguaje de programación es más que su sintaxis: incluye el compilador, el entorno de ejecución, las bibliotecas, las herramientas y los contratos de plataforma que hacen que el software sea utilizable.

**Ámbitos de trabajo de SLeeLa.** SLeeLa aborda **Math**, **Object**, **Science** y **Reach**, las cuatro dimensiones de su trabajo: lo calculable (Math), el objeto establecido en el catálogo de lo que existe (Object), la investigación disciplinada de lo verdadero (Science) y la ampliación de la capacidad de un sistema hacia otros sistemas y personas (Reach). **Los intereses nacionales siguen vigentes**; ninguna de estas cuatro dimensiones los sustituye. Cuando un tema afecta a los intereses de una nación, esa relación se conserva y documenta fielmente, por ejemplo, en los campos de metadatos de una clase `metaOrigin` y `metaNationalSchedule`.

## I. Primeros pasos

- [Compilar la implementación nativa](../../impl/README.md)
- [Máquinas virtuales y Modium: guía práctica del usuario](../../virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [Pila de CPU en el código fuente de SLeeLa](../../lib/cpu/CPU.md): una cadena de máquina completa, desde las puertas lógicas hasta la ALU, los núcleos, el sistema operativo y los programas; incluye más de 130 perfiles de CPU y [tensiones, frecuencias variables y parámetros de temporización avanzados](../../lib/cpu/TIMING.md), con ejecución [según especificación o mediante overclocking](../../lib/cpu/components/README.md).
- [Índice de documentación](../../docs/README.md)
- [Fuente de datos CSPML y referencia de investigación Salt & Salary](../../CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md): procedencia y orientación de integración para datos externos de EE. UU. sobre lugares, condiciones socioeconómicas, fuerza laboral, educación y escuelas públicas.
- [Documentación de API generada](../../docs/api/README.md): referencia HTML actual, versión de API y orientación para regenerarla.
- [Catálogo de API](../../api/README.md)
- [Estándares de comparación computacional](../../comparison/README.md): estándares de tipos, memoria, ABI y conformidad para SLeeLa VM, Java/JVM y C/C++ nativo.
- [Stanchion de recursión: prototipos C y C++](../../recursion/stanchion/README.md): ejemplos especializados en los tipos primitivos Boolean, Byte, Character, Integer, Long, Float y Double, además de patrones de recursión genéricos, aplicados, científicos y educativos.
- [Matriz de compatibilidad y validación de plataformas](../../docs/PLATFORM-SUPPORT.md)
- [Guía de marca](../../docs/BRAND-GUIDE.md)
- [Conceptos y recursos gráficos de merchandising](../../merchandise/README.md)
- [Logotipo de NitroWebExpress](../../images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Política de pruebas y criterios de publicación](../../markdown/TESTING.md)

## II. Novedades

Incorporaciones recientes al lenguaje, los controladores y la biblioteca estándar:

- **Paquete estándar `advantage`: distribución de dividendos con matemáticas, física, integración, evaluación y salida enrutable.** [`lib/advantage/`](../../lib/advantage/README.md) modela la obtención de un dividendo único con respecto a un modelo mediante una estructura por capas denominada **Sequitur onion** (dividendo → revisión → cuestiones → matemáticas → adiciones → sustracción → reagrupación → iteración). Se puede recorrer hacia dentro y hacia fuera, con actualización dinámica y restitución completas, mediante un **modelo de usuario** sencillo y un **modelo de desarrollador** con control integral. Comprende cinco áreas: **`math/`**, economía del comercio bilateral (maximización conjunta de beneficios, negociación de Nash y razón crítica bilateral del modelo newsvendor); **`physics/`**, una «física de los campos de confianza» aplicada a la economía estructural (confianza como tensor métrico, geodésica transaccional, estructura de contacto de anuncios y una SDE de decaimiento de confianza CIR con saltos, presentada expresamente como analogía de modelización); **`integration/`**, modelo de integración vertical componible recursivamente para capitalismo y comercio minorista (compartimentos, bahías de ingeniería, compartimentos de prueba, bandejas de mediana newtoniana y puntuación de sofisticación del sistema); **`grading/`**, evaluador de circuitos de sofisticación que califica cada módulo y conjuntos de módulos ganadores, perdedores, evaluadores o absolutos A–D; y **`output/`**, una interfaz de emisión enrutable que dirige la salida al terminal, a una superficie con formato **CommonRails** o a un socket/conector (previsto para un `slnet_write` nativo). Cada clase contiene el registro Metadata-Class; cinco demostraciones ejecutables validan las áreas. La documentación complementaria está en [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md), [`MATH.md`](../../lib/advantage/MATH.md) y [`PHYSICS.md`](../../lib/advantage/PHYSICS.md). Un consumidor nativo C/C++ en [`advantage/`](../../advantage/README.md) —con el núcleo C `advantage_value`/`advantage_mean`, un almacén de conocimiento XML y una biblioteca segura de adquisición HTTPS/SFTP/SSH, almacén de claves y transporte DH/ECDH— conecta el modelo con [ejemplos de comercio](../../advantage/examples/README.md): operaciones positivas procedentes de fuentes de internet supuestas, una secuencia de relevancia para Traders, Consumers, Helpers y Healers, y ocho modelos clasificados de A a D (D = máximo Cement; A = máximo Brand).
- **Anotaciones de temporización de bucles: `@loopDelay` / `@loopBudget` y función integrada `sleep(ms)`.** Son las primeras anotaciones **de comportamiento a nivel de instrucción** de SLeeLa: se escriben en una línea propia antes de un bucle `for`, `while` o `do` e insertan una pausa real en su ejecución. `@loopDelay(ms = 250)` y `@loopDelay(seconds = 2)` esperan una cantidad fija por iteración (la forma abreviada `@loopDelay(250)` usa milisegundos); `@loopBudget(seconds = 10, iterations = 1000)` distribuye un número conocido de iteraciones a lo largo de un presupuesto total de tiempo de pared. Ambas se reducen al nuevo opcode append-only **`OP_SLEEP`** (ISA 157), que también respalda la función general integrada **`sleep(<ms>)`**, invocable desde cualquier lugar. La pausa se implementa de forma portable mediante `nanosleep` / `Sleep`. Está especificado en [`SLEELA.syntax` §32](../../SLEELA.syntax), documentado en la [referencia API de temporización](../../api/sleela/3.0/LOOP_TIMING_API.md) y demostrado en [`examples/loop-timing-annotation-demo.sleela`](../../examples/loop-timing-annotation-demo.sleela). El índice de incorporaciones de la sesión —que también incluye la multiplicación nativa de enteros grandes `OP_BIGMUL` y las herramientas independientes [local-generator](../../local-generator/README.md) y [pi-generator](../../pi-generator/README.md)— está en [`SESSION-ADDITIONS-API.md`](../../api/sleela/3.0/SESSION-ADDITIONS-API.md). Las tres superficies de API generadas ([`sleeladoc-api/`](../../sleeladoc-api/README.md), [`docs/api/`](../../docs/api/README.md) y la instantánea inmutable [`api/sleela/3.0/version-014/`](../../api/sleela/3.0/version-014/index.html)) se reconstruyeron con `make api-docs` e incluyen las nuevas instrucciones `SLISASleep` y `SLISABigMul` en la página del paquete `vm`.
- **SleelaDoc desde la línea de comandos: `sleela docs`.** SleelaDoc ya está disponible directamente en el terminal. `sleela docs` enumera todos los temas documentables; `sleela docs crypto` muestra un paquete; y `sleela docs stringbuilder` resuelve una clase sin distinguir mayúsculas y minúsculas, además de reconocer el prefijo estándar `SL` y resolverlo como `SLStringBuilder`. Muestra una página completa con **definición, finalidad de diseño, relaciones, estructura, user clays y una escala de dominio de cinco niveles**, además de sugerencias clasificadas de «¿quería decir?» ante nombres aproximados. Los autores amplían cualquier página añadiendo etiquetas SleelaDoc `@` como `@intent`, `@thesis`, `@axiom`, `@param`, `@see` y `@mastery`. `sleela docs --generate [dir]` genera un corpus de texto plano (una página por clase y un índice `TOPICS.index`) que la CLI puede leer después sin conexión; **`make sleeladoc`** empaqueta ese corpus y sus fuentes `.sleela` correspondientes en `sleeladoc/build/sleeladoc.jar`. Para temas que solo disponen de una fachada, como `psychiatry` y `politico`, `sleela docs --facade <topic>` busca en el sistema de archivos durante la ejecución y enumera las carpetas reales y después los archivos, con límites configurables por el usuario de **6 MB, 6000 archivos y 6 minutos**. Las unidades de `lib/sleeladoc` utilizan la edición actual **`#sleela 3.0`**. Véanse [`man/man1/sleela.1`](../../man/man1/sleela.1) y [`lib/sleeladoc/README.md`](../../lib/sleeladoc/README.md).
- **SleelaDoc: documentación de estilo Javadoc con sitio HTML generado.** **SleelaDoc** es la convención de comentarios documentales en el código fuente de SLeeLa (comentarios de ángulo delimitados por `@doc` / `@page`; véase [`manifesto/SLEELADOC.md`](../../manifesto/SLEELADOC.md)). [`lib/sleeladoc`](../../lib/sleeladoc/README.md) es el **extractor** que convierte una unidad `.sleela` en una página API Markdown o HTML, de forma comparable a cómo `javadoc` genera documentación Java, y documenta la *intención*. El sitio HTML completo y navegable de todo `/lib` se genera en [`/sleeladoc-api`](../../sleeladoc-api/README.md) mediante `tools/generate-sleeladoc-api.py` (abra `sleeladoc-api/index.html`): utiliza una presentación limpia en azul, azul claro, gris claro y blanco, una jerarquía paquete → clase → miembro y un glosario dirigido al lector *Student 2*. El compilador también produce como resultado de la compilación documentos marcados por el autor (`--wiki`: una página MediaWiki y una copia de la fuente de origen), descritos en [`manifesto/SLEELADOC.WIKI.md`](../../manifesto/SLEELADOC.WIKI.md). Ejemplo ejecutable: [`lib/sleeladoc/sleeladoc-demo.sleela`](../../lib/sleeladoc/sleeladoc-demo.sleela).
- **Modelo de objetos de documento XML (`lib/xml`).** DOM XML del lado de SLeeLa: un documento posee un árbol de nodos de elemento, texto, comentario, CDATA e instrucción de procesamiento, con hijos ordenados y atributos con nombre; incluye un serializador compacto o legible (escape XML 1.0 §2.4), un analizador por celdas de caracteres y el registro de calidad `SLXmlAspect`. La igualdad entre nodos sigue tres niveles de confianza (identidad → mismo sujeto → estructura), y es reflexiva, simétrica y determinista. Véanse [`lib/xml/README.md`](../../lib/xml/README.md) y el ejemplo ejecutable [`lib/xml/xml-dom.sleela`](../../lib/xml/xml-dom.sleela).
- **Primitivas de enteros con signo de gran anchura (`long`, `int64`, `int128`).** `int`, `long` e `int64` son enteros con signo de 64 bits y se verifican y ejecutan de forma idéntica. `int128` se acepta como tipo entero, aunque por ahora utiliza una representación de 64 bits; el soporte real de 128 bits está documentado como trabajo futuro. La serie de enteros con signo de base dos, `int32`…`int16777216` (sintaxis 2.7), y la serie sin signo correspondiente, `uint32`…`uint16777216` (sintaxis 2.8), amplían estas capacidades. Véase [`SLEELA.syntax`](../../SLEELA.syntax) §21, §24 y §25.
- **Campos consecuentes `@mix` (sintaxis 2.9).** Una clase puede declarar varios campos de instancia con el **mismo nombre** cuando cada repetición lleva `@mix`. El lenguaje interpreta el grupo como una secuencia (un array o lista indexada): por defecto, en orden ascendente `1,2,3,…`, o de forma manual mediante `@mix(1,2,3,...)`; la cantidad de índices debe coincidir con la cantidad de campos. Véase [`SLEELA.syntax`](../../SLEELA.syntax) §28.
- **Marcadores documentales: el Proceso Natural, denominado Preonems (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).** Estas palabras desnudas que marcan documentos reciben colectivamente el nombre de **Preonems** (un marcador es un *Preonem*; el conjunto, los *Preonems*; véase [`SLEELA.syntax` §26.1](../../SLEELA.syntax)). Son marcadores documentales declarativos de primera clase que expresan qué *es* un documento en el orden de su creación: `author`/`authorial` (sintaxis 2.4; gestionado por el consejo profesional, el único miembro con efecto de comportamiento), `material;` (Source establecido) y `memory;` (sintaxis 2.5), `determine;` (sintaxis 2.6; documento abierto a procesamiento posterior por un doctor, médico o ghost hasta su conclusión por vía legal), y los dos más recientes: **`object;`** (una cosa establecida en el catálogo de lo que existe, reificada y dotada de posición concreta en sintaxis 2.8) y **`field;`** (el ámbito relacional en el que los objetos existen y se mueven), ambos habilitados desde la sintaxis 2.6. Cada marcador es contextual, independiente y condicionado por versión en el frontend; no altera el análisis sintáctico, el tipado ni la generación de código. Fuera de la posición de marcador, cada palabra sigue siendo un identificador normal; por ejemplo, `field` no interfiere con un miembro de clase ni con un acceso `.field`. Véase [`SLEELA.syntax`](../../SLEELA.syntax) §18–§20, §23 y §26.
- **Nordshrift en sintaxis 3.0 con acceso completo a la biblioteca.** Nordshrift resuelve los nombres mediante el mismo índice recursivo de `/lib` que utiliza el compilador, por lo que alcanza todos los paquetes y unidades `.sleela`; `nordshrift check` informa de los recuentos de todo el árbol.
- **Stanchion de recursión.** [`recursion/stanchion/`](../../recursion/stanchion/README.md) incluye ahora prototipos de referencia C y C++ específicos para los siete contratos primitivos documentados, además de ejemplos genéricos, aplicados, científicos y didácticos. Son fuentes de referencia: deben compilarse y probarse antes de considerarlas verificadas.
- **Comentarios con corchetes angulares (`<` / `<<`).** Nuevo estilo de comentarios para desarrollos futuros: `< line >` y `<< block >>`, reconocido por el frontend de SLeeLa y documentado en [`SLEELA.syntax` §1.1](../../SLEELA.syntax).
- **Literales enteros con radix.** Los literales `int` aceptan ahora formas hexadecimal (`0x`), binaria (`0b`) y octal (`0o`), con separadores de dígitos `_`, tanto en el lenguaje fuente SLeeLa ([`SLEELA.syntax` §1.4](../../SLEELA.syntax)) como en el formato de modelos de CPU [`.xlt`](../../lib/xlt/README.md).
- **Estándar Metadata Class en toda la biblioteca.** El metapatrón de metadatos descriptivos (procedencia, legalidad y ámbito jurisdiccional) se aplica ahora a todos los archivos `.sleela` que contienen clases bajo `/lib`. Véanse [`SLEELA.syntax` §15](../../SLEELA.syntax) y la [lección del tutorial](../../tutorial/13-metadata-class.md).
- **Familia de texto y String ampliada.** `lib/text` incorpora asistentes reales en el lenguaje, una vista por celdas de caracteres con operaciones auténticas `charAt`, `substring`, `indexOf`, `reverse` y alineación de columnas, además de un motor de plantillas posicional. Véanse el [tutorial de texto y String](../../tutorial/14-text-and-strings.md) y el ejemplo ejecutable [`lib/text/text-string-demo.sleela`](../../lib/text/text-string-demo.sleela).
- **`String` de primera clase en las hojas Nordshrift.** Una hoja `.sst` puede declarar un bloque tipado `strings:` que contribuye al manifiesto de componentes de la compilación. Véase la [referencia de Nordshrift Sheet](../../wiki/Nordshrift-Sheet-Reference.md).
- **Modelos de CPU ejecutados mediante opcodes y equivalencia de llamadas a funciones.** Cada `SL<ARCH>CPU` hereda tanto la capacidad de ejecutar un flujo de instrucciones nativas mediante su ciclo de búsqueda, decodificación y ejecución como la de ejecutar el código de otro proceso supuesto mediante llamadas equivalentes sobre la infraestructura compartida de la VM. Véase [`lib/cpu/EXECUTION.md` §5](../../lib/cpu/EXECUTION.md).

## III. Compilación y pruebas

**Requisitos:** una cadena de herramientas C/C++ (GCC o Clang) y GNU Make; Python 3 para las herramientas de manifiestos y flujos de trabajo; en Linux, además, los paquetes de desarrollo X11/Xrandr para los productos de visualización. El comando `make deps` de cada módulo informa o instala las dependencias necesarias. La compilación de `impl/` falla de forma segura si no se satisface el manifiesto SHA-256 de confianza; el Makefile raíz lo incorpora automáticamente.

La forma más rápida de empezar desde un clon limpio:

```sh
make                                  # compilar todo desde la raíz del repositorio
```

O compilar y probar únicamente el núcleo nativo:

```sh
make -C impl all                      # compilar solo el núcleo del compilador y del entorno de ejecución
bash ./test-suites/run-all.sh --all   # ejecutar la suite completa de pruebas
```

El comando `make` de la raíz compila todos los productos (núcleo, VM, códecs, churn, antivirus, etc.) y termina creando el **registro** y materializando su tabla base. Por tanto, un árbol de trabajo compilado incluye desde el principio un registro verificable.

Objetivos principales:

```sh
make                     # compilar todo (alias de make all; el registro se crea al final)
make all                 # la misma compilación completa, indicada explícitamente
sudo make install        # instalar comandos y archivos de soporte del entorno de ejecución (véase § IV)
make clean               # eliminar salidas de compilación de todos los sistemas integrados
make help                # mostrar todos los objetivos del Makefile raíz
```

Otros objetivos útiles:

```sh
make registry            # compilar solo el registro y materializar la tabla
./build-registry.sh      # la misma operación mediante un script directo
make tests               # compilar y ejecutar la suite de pruebas
make ballum              # compilar la biblioteca de visualización Ballum para este sistema operativo
bin/install-modules      # instalar/compilar módulos de forma escalonada durante unos 10–20 minutos
```

El objetivo `registry` ejecuta [`build-registry.sh`](../../build-registry.sh): compila el núcleo del registro, incorpora la tabla base al almacenamiento de texto portable `registry/build/sleela.sleelareg` y exporta el script de tabla MySQL `registry/build/registry.sql` (mediante `sleela registry export`). El registro puede revisarse con `sleela registry` o `registry/build/sleela-registry review`. Consulte la [guía del registro](../../registry/README.md) y la [arquitectura Registry 3.0](../../registry/REGISTRY-3.0-ARCHITECTURE.md).

Consulte las guías de compilación y pruebas para conocer los requisitos y los pasos específicos de cada plataforma. Que un componente compile correctamente no demuestra que todas las bibliotecas o plataformas estén listas para publicarse.

## IV. Instalación

Después de compilar los productos nativos, instale los comandos y los archivos de soporte del entorno de ejecución:

```sh
sudo make install
make install PREFIX="$HOME/.local"        # instalación para el usuario, sin sudo
make install DESTDIR="$PWD/package-root" # instalación en un directorio de preparación de paquetes
make install-check
```

El instalador convencional utiliza de forma predeterminada `/usr/local/bin`, `/usr/local/lib/sleela` y `/usr/local/share/sleela`. Conserva la configuración de inicio existente y no sobrescribe comandos preexistentes que no estén bajo seguimiento. La desinstalación elimina únicamente los archivos registrados por el instalador; la configuración y los datos de ejecución no registrados permanecen intactos.

Para un despliegue guiado y específico del paquete principal —los binarios `sleela`, `sleelvac` y `nordshrift`, la biblioteca estándar, los protocolos y los módulos de soporte— utilice el instalador limpio:

```sh
make clean-install       # instalación guiada únicamente del paquete principal
```

Primero se pregunta si se desea instalar el paquete principal y después se elige el alcance de la instalación. Si el proceso termina correctamente, puede aparecer un selector opcional de módulos de mejora seleccionados. El flujo interactivo completo y todas las opciones se describen en [Instalación rápida y segura](../../install/README.md).

## V. Lenguaje y entorno de ejecución

La gramática normativa se encuentra en [`SLEELA.syntax`](../../SLEELA.syntax). El repositorio documenta actualmente la sintaxis **3.0**; la versión del compilador/herramientas y la versión de sintaxis del código fuente son independientes. La superficie 3.0 añade los marcadores documentales **`pure` y `gasoline`** (§30): dos Preonems declarativos y contextuales, sin efecto de ejecución propio, que expresan la posición de un documento mediante su propia honestidad. La sintaxis 3.0 reúne las superficies del lenguaje 1.0 a 2.9 en una edición normativa actual y conserva los requisitos de versión mínima de cada función.

La superficie anterior, 2.9, añade los **campos secuenciales `@mix`** (§28): una clase puede declarar varios campos de instancia con el mismo nombre si cada uno lleva la marca `@mix`. Se interpretan como un grupo indexado (ascendente por defecto: `1,2,3,…`, o definido manualmente mediante `@mix(1,2,3,...)`; el número de índices debe coincidir con el número de campos). También incluye las familias de enteros binarios amplios con signo y sin signo, desde `int32` hasta `int16777216` (§24) y desde `uint32` hasta `uint16777216` (§25), que amplían `long`, `int64` y `int128` (§21); documentos de soporte de palabras clave (§22); y los **Preonems**: marcadores documentales declarativos `author`/`authorial` (§18), `material`/`memory` (§19–§20), `determine` (§23), `object`/`field` (§26), `pure`/`gasoline` (§30) y `artifact` (§35; un estado de calidad legislativa evaluado en relación con el tiempo y el espacio). En conjunto reciben el nombre de Preonems (§26.1).

La resolución de sobrecarga de constructores y la delegación a constructores `this(...)` y de clases base quedan expresamente fuera del contrato de sintaxis 1.8 verificado. Antes de depender de una función, consulte la gramática y las notas de implementación. Las nueve unidades fuente de SleelaDoc declaran ahora `#sleela 3.0`; la comprobación común del compilador acepta sintaxis fuente de 1.0 a 3.0. La nueva documentación de API se genera con `make api-docs`.

El repositorio contiene una implementación nativa C/C++, referencias para la generación de SLVM, módulos de biblioteca estándar, pruebas, ejemplos e integraciones de plataforma. El estado de implementación y validación varía según el módulo. Consulte la especificación correspondiente y los resultados actuales de CI; no presuponga que todos los componentes tienen el mismo grado de finalización.

## VI. Principios de ingeniería

- **Evidencia antes que afirmaciones:** distinguir claramente lo implementado, probado, experimental y planificado.
- **Un contrato por frontera:** documentar la sintaxis, las API, los formatos de archivo, las suposiciones de plataforma y el comportamiento ante errores.
- **Portabilidad desde el diseño:** aislar los backends específicos de cada plataforma y probarlos en ejecutores nativos.
- **Fallos seguros:** probar entradas malformadas, límites de recursos, fallos de procesos y rutas de limpieza.
- **Versiones reproducibles:** reconstruir desde cero y publicar hashes de artefactos y notas de versión.

## VII. Compilación del antivirus e integridad

El paquete antivirus Sleelavirin incluye un escáner SHA-256 nativo, un adaptador opcional de ClamAV, manifiestos de integridad para fuentes e índices y sumas de comprobación para paquetes de plataforma. Compile y pruebe el paquete desde la raíz del repositorio:

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

El manifiesto de integridad se genera en `antivirus/build/`. Incluye entradas de código fuente y documentación bajo seguimiento, así como archivos protegidos de índices de firmas; excluye los resultados generados y la caché mutable de ClamAV. Las instantáneas de bases de datos de ClamAV tienen sus propios manifiestos. Los ZIP de publicación incluyen `INTEGRITY-MANIFEST.json`, `SHA256SUMS` y un archivo complementario `.sha256`. Compare los hashes de los archivos descargados con un valor de confianza obtenido de forma independiente: una suma de comprobación almacenada junto al archivo no constituye una firma del editor. Consulte la [guía de Sleelavirin](../../antivirus/README.md), la [lista de comprobación de seguridad](../../antivirus/SECURITY.md) y el [flujo de compilación del antivirus](../../.github/workflows/antivirus-binaries.yml).

Una versión solo se considera verificada cuando los trabajos de CI pertinentes han terminado correctamente para el commit exacto de esa versión y se han comprobado los artefactos resultantes.

## VIII. Temas del editor SLeeLa: Vim y Emacs

SLeeLa incluye un conjunto coordinado de esquemas de color oscuros y profesionales para **Vim** y **Emacs**, además de una utilidad nativa de paletas para C/C++. El esquema original **American Professional** sigue siendo el predeterminado. Hay diez paletas complementarias inspiradas en distintos diseños culturales e institucionales; son interpretaciones de diseño, no representaciones oficiales de ninguna nación, cultura, corte o institución.

### Los once esquemas y sus colores

| Esquema | Fondo | Primer plano | Palabras clave | Tipos | Cadenas | Comentarios |
|---|---|---|---|---|---|---|
| **American Professional** | <span style="display:inline-block;width:1em;height:1em;background:#101820;border:1px solid #888"></span> `#101820` | <span style="display:inline-block;width:1em;height:1em;background:#F4F7FA;border:1px solid #888"></span> `#F4F7FA` | <span style="display:inline-block;width:1em;height:1em;background:#7FB3FF;border:1px solid #888"></span> `#7FB3FF` | <span style="display:inline-block;width:1em;height:1em;background:#73DACA;border:1px solid #888"></span> `#73DACA` | <span style="display:inline-block;width:1em;height:1em;background:#F2C078;border:1px solid #888"></span> `#F2C078` | <span style="display:inline-block;width:1em;height:1em;background:#8A96A3;border:1px solid #888"></span> `#8A96A3` |
| **Pan-Eurasian** | <span style="display:inline-block;width:1em;height:1em;background:#111827;border:1px solid #888"></span> `#111827` | <span style="display:inline-block;width:1em;height:1em;background:#F0F2F5;border:1px solid #888"></span> `#F0F2F5` | <span style="display:inline-block;width:1em;height:1em;background:#7AA7E8;border:1px solid #888"></span> `#7AA7E8` | <span style="display:inline-block;width:1em;height:1em;background:#69C9B3;border:1px solid #888"></span> `#69C9B3` | <span style="display:inline-block;width:1em;height:1em;background:#D7B56D;border:1px solid #888"></span> `#D7B56D` | <span style="display:inline-block;width:1em;height:1em;background:#8995A5;border:1px solid #888"></span> `#8995A5` |
| **Russian and Slavic — Medium** | <span style="display:inline-block;width:1em;height:1em;background:#202733;border:1px solid #888"></span> `#202733` | <span style="display:inline-block;width:1em;height:1em;background:#E8EAF0;border:1px solid #888"></span> `#E8EAF0` | <span style="display:inline-block;width:1em;height:1em;background:#A9C0DD;border:1px solid #888"></span> `#A9C0DD` | <span style="display:inline-block;width:1em;height:1em;background:#8FC4B2;border:1px solid #888"></span> `#8FC4B2` | <span style="display:inline-block;width:1em;height:1em;background:#D6BD8B;border:1px solid #888"></span> `#D6BD8B` | <span style="display:inline-block;width:1em;height:1em;background:#9AA3AF;border:1px solid #888"></span> `#9AA3AF` |
| **Central Asian / Silk Road** | <span style="display:inline-block;width:1em;height:1em;background:#1C2529;border:1px solid #888"></span> `#1C2529` | <span style="display:inline-block;width:1em;height:1em;background:#F5F0E5;border:1px solid #888"></span> `#F5F0E5` | <span style="display:inline-block;width:1em;height:1em;background:#7FADE0;border:1px solid #888"></span> `#7FADE0` | <span style="display:inline-block;width:1em;height:1em;background:#45C6C2;border:1px solid #888"></span> `#45C6C2` | <span style="display:inline-block;width:1em;height:1em;background:#D9B56D;border:1px solid #888"></span> `#D9B56D` | <span style="display:inline-block;width:1em;height:1em;background:#92A3A0;border:1px solid #888"></span> `#92A3A0` |
| **East Asian** | <span style="display:inline-block;width:1em;height:1em;background:#171A1C;border:1px solid #888"></span> `#171A1C` | <span style="display:inline-block;width:1em;height:1em;background:#F2EFE7;border:1px solid #888"></span> `#F2EFE7` | <span style="display:inline-block;width:1em;height:1em;background:#D97878;border:1px solid #888"></span> `#D97878` | <span style="display:inline-block;width:1em;height:1em;background:#72C7A5;border:1px solid #888"></span> `#72C7A5` | <span style="display:inline-block;width:1em;height:1em;background:#D8B76A;border:1px solid #888"></span> `#D8B76A` | <span style="display:inline-block;width:1em;height:1em;background:#969F9B;border:1px solid #888"></span> `#969F9B` |
| **ICC — Corte Penal Internacional** | <span style="display:inline-block;width:1em;height:1em;background:#0E1A2B;border:1px solid #888"></span> `#0E1A2B` | <span style="display:inline-block;width:1em;height:1em;background:#EAF0F7;border:1px solid #888"></span> `#EAF0F7` | <span style="display:inline-block;width:1em;height:1em;background:#5B8FD6;border:1px solid #888"></span> `#5B8FD6` | <span style="display:inline-block;width:1em;height:1em;background:#7FB7C9;border:1px solid #888"></span> `#7FB7C9` | <span style="display:inline-block;width:1em;height:1em;background:#D9B25A;border:1px solid #888"></span> `#D9B25A` | <span style="display:inline-block;width:1em;height:1em;background:#7E8CA0;border:1px solid #888"></span> `#7E8CA0` |
| **SCOTUS — Tribunal Supremo de EE. UU.** | <span style="display:inline-block;width:1em;height:1em;background:#1A1416;border:1px solid #888"></span> `#1A1416` | <span style="display:inline-block;width:1em;height:1em;background:#F3EEE6;border:1px solid #888"></span> `#F3EEE6` | <span style="display:inline-block;width:1em;height:1em;background:#9E2B35;border:1px solid #888"></span> `#9E2B35` | <span style="display:inline-block;width:1em;height:1em;background:#8FA9B8;border:1px solid #888"></span> `#8FA9B8` | <span style="display:inline-block;width:1em;height:1em;background:#C9A24B;border:1px solid #888"></span> `#C9A24B` | <span style="display:inline-block;width:1em;height:1em;background:#8C8178;border:1px solid #888"></span> `#8C8178` |
| **Greek (Hellenic)** | <span style="display:inline-block;width:1em;height:1em;background:#0A1A30;border:1px solid #888"></span> `#0A1A30` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#2E7BD6;border:1px solid #888"></span> `#2E7BD6` | <span style="display:inline-block;width:1em;height:1em;background:#5E9FE0;border:1px solid #888"></span> `#5E9FE0` | <span style="display:inline-block;width:1em;height:1em;background:#9FC3EC;border:1px solid #888"></span> `#9FC3EC` | <span style="display:inline-block;width:1em;height:1em;background:#6E86A6;border:1px solid #888"></span> `#6E86A6` |
| **Turkish (Al Bayrak)** | <span style="display:inline-block;width:1em;height:1em;background:#2A0609;border:1px solid #888"></span> `#2A0609` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#F23D48;border:1px solid #888"></span> `#F23D48` | <span style="display:inline-block;width:1em;height:1em;background:#F0A9AE;border:1px solid #888"></span> `#F0A9AE` | <span style="display:inline-block;width:1em;height:1em;background:#F0C9A9;border:1px solid #888"></span> `#F0C9A9` | <span style="display:inline-block;width:1em;height:1em;background:#A8767A;border:1px solid #888"></span> `#A8767A` |
| **Kiro** | <span style="display:inline-block;width:1em;height:1em;background:#0B0B12;border:1px solid #888"></span> `#0B0B12` | <span style="display:inline-block;width:1em;height:1em;background:#F5F5FA;border:1px solid #888"></span> `#F5F5FA` | <span style="display:inline-block;width:1em;height:1em;background:#A98EDA;border:1px solid #888"></span> `#A98EDA` | <span style="display:inline-block;width:1em;height:1em;background:#6E8BE0;border:1px solid #888"></span> `#6E8BE0` | <span style="display:inline-block;width:1em;height:1em;background:#E8C547;border:1px solid #888"></span> `#E8C547` | <span style="display:inline-block;width:1em;height:1em;background:#6A6482;border:1px solid #888"></span> `#6A6482` |
| **SLeeLa** | <span style="display:inline-block;width:1em;height:1em;background:#08080E;border:1px solid #888"></span> `#08080E` | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | <span style="display:inline-block;width:1em;height:1em;background:#B07CFF;border:1px solid #888"></span> `#B07CFF` | <span style="display:inline-block;width:1em;height:1em;background:#5A7CFF;border:1px solid #888"></span> `#5A7CFF` | <span style="display:inline-block;width:1em;height:1em;background:#FFD21E;border:1px solid #888"></span> `#FFD21E` | <span style="display:inline-block;width:1em;height:1em;background:#6E5F9E;border:1px solid #888"></span> `#6E5F9E` |

### Colores de las banderas nacionales (paleta de origen)

Los esquemas griego y turco derivan de los **colores oficiales de las banderas nacionales** de ambos países, indicados aquí con sus valores hexadecimales publicados. Los fondos y acentos de los esquemas anteriores son interpretaciones oscurecidas y adaptadas a editores; la tabla siguiente conserva los colores de referencia tal como se presentan a nivel nacional.

| País | Color | Hexadecimal | Fundamento y significado |
|---|---|---|---|
| **Grecia** | Azul | `#0D5EAF` | Azul de la bandera nacional (Ley 851/1978). El cielo y los mares; en heráldica, *azur*. |
| **Grecia** | Blanco | `#FFFFFF` | Blanco de la bandera nacional. Pureza de la lucha por la independencia; la cruz representa el cristianismo ortodoxo oriental. |
| **Türkiye** | Rojo (*al bayrak*) | `#E30A17` | Rojo de la bandera nacional (Ley de la Bandera turca). El campo; por tradición, la sangre de los mártires y la soberanía otomana. |
| **Türkiye** | Blanco | `#FFFFFF` | Blanco de la bandera nacional. La media luna y la estrella: paz, pureza y el símbolo del patrimonio túrquico/islámico. |

Los esquemas utilizan el **blanco** nacional como base para el primer plano de cada editor y el **azul** nacional exacto (`#0D5EAF`) o el **rojo** (`#E30A17`) para palabras clave, cursor, selección y barra de estado. Así, los colores principales son literalmente los colores nacionales. Son paletas interpretativas; la referencia de colores de las banderas documenta su procedencia y no constituye una representación oficial de ninguno de los dos Estados. Los significados cromáticos siguen la descripción de símbolos estatales del Ministerio de Asuntos Exteriores de la República Helénica y la tradición de la Ley de la Bandera turca.

### Tabla de muestras de color

Las celdas de paleta siguientes utilizan **muestras SVG**, para que GitHub muestre los colores reales en lugar de celdas HTML vacías. Cada franja presenta seis colores en este orden: fondo, primer plano, palabras clave, tipos, cadenas y comentarios. Los códigos hexadecimales exactos aparecen junto a cada muestra, en el mismo orden de esquemas que la tabla de referencia.

| Esquema | Paleta representada | Códigos hexadecimales (fondo · primer plano · palabras clave · tipos · cadenas · comentarios) |
|---|---|---|
| **American Professional** | ![Paleta American Professional](../../lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Paleta Pan-Eurasian](../../lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russian and Slavic — Medium** | ![Paleta Russian and Slavic Medium](../../lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Central Asian / Silk Road** | ![Paleta Central Asian Silk Road](../../lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **East Asian** | ![Paleta East Asian](../../lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC — Corte Penal Internacional** | ![Paleta ICC](../../lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS — Tribunal Supremo de EE. UU.** | ![Paleta SCOTUS](../../lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Greek (Hellenic)** | ![Paleta griega helénica](../../lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turkish (Al Bayrak)** | ![Paleta turca Al Bayrak](../../lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Paleta Kiro](../../lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![Paleta SLeeLa](../../lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

Se utilizan colores de acento adicionales para funciones, constantes, selecciones, diagnósticos y directivas del preprocesador. La paleta Russian and Slavic emplea deliberadamente tonos medios de pizarra, azul, salvia, rubí y oro antiguo para ofrecer una apariencia profesional más serena.

### Instalar los esquemas de Vim

Desde la raíz del repositorio:

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

Añada lo siguiente a `~/.vimrc` para activar el resaltado de sintaxis de SLeeLa y elegir un esquema:

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

Elija exactamente uno de estos nombres de esquema:

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

Por ejemplo, utilice `colorscheme sleela-russian-slavic` para la paleta rusa y eslava de intensidad media, o `colorscheme sleela-greek` / `colorscheme sleela-turkish` para las paletas helénica y Al Bayrak. Reinicie Vim o vuelva a cargar la configuración con `:source $MYVIMRC`.

### Instalar los esquemas de Emacs

Cree un directorio local para los temas y copie desde la raíz del repositorio los archivos del modo y de los temas:

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

Añada lo siguiente a `~/.emacs` o `~/.emacs.d/init.el`:

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

Sustituya `sleela-american` por `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro` o `sleela-sleela` para seleccionar otro esquema. Si Emacs pregunta si debe confiar en el tema, inspeccione el archivo local del tema antes de aceptarlo.

### Previsualizar las paletas nativas C/C++

La pequeña utilidad nativa de paletas puede enumerar esquemas, mostrar sus colores de terminal y consultar un color individual:

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

Por compatibilidad con versiones anteriores, `--palette` y `--hex COLOR` sin `--theme` seleccionan American Professional. Los resultados de compilación y prueba deben comprobarse en el sistema de destino y en CI; documentar un comando no demuestra por sí solo que se haya ejecutado correctamente.

Consulte la [guía de integración de editores](../../linux-editors/README.md) para obtener las descripciones completas de las paletas y los detalles de la utilidad.

## IX. Contribuciones

1. Lea el índice de documentación y la política de pruebas.
2. Realice cambios acotados, acompañados de pruebas positivas y negativas.
3. Ejecute primero las pruebas específicas y después la suite amplia.
4. Indique los detalles del sistema operativo y de la cadena de herramientas; no describa comprobaciones omitidas como si hubieran pasado.

SLeeLa es desarrollado por **MEARVK LLC**. El material histórico de la página de inicio se conserva en [`docs/README-LEGACY.md`](../../docs/README-LEGACY.md).

## X. AE6E66 — Expectativa de gravamen por correo electrónico público Round

Para la iniciativa AE6E66, la expectativa declarada del proyecto es de **300.000.000,00 USD por cada correo electrónico público Round**, como importe propuesto de gravamen fiscal asociado a cada correo electrónico Round Public que reúna los requisitos.

Esta declaración registra únicamente una expectativa del proyecto. **No** constituye una liquidación tributaria legal, una orden gubernamental, una sentencia judicial ni una obligación fiscal establecida de forma independiente. Cualquier gravamen efectivo requeriría la autoridad legal aplicable, la jurisdicción, las definiciones, el procedimiento y una determinación de la autoridad gubernamental competente.

## XI. Entorno de ejecución de alfabetos y conjuntos de caracteres

La documentación del alfabeto de SLeeLa y el entorno nativo de caracteres se mantienen en `alphabet/sleela/` y `lib/alphabet/native/`. Las pruebas nativas C++17 cubren la decodificación y codificación estrictas de escalares UTF-8, las firmas de capacidad `C<n>` y la pertenencia acotada a conjuntos de caracteres:

```sh
make -C lib/alphabet/native test
```

El catálogo experimental UTF-4088 y su adaptador de consulta desde SLeeLa están documentados en [`utf-4088/SLeeLa-ALPHABET-INTEGRATION.md`](../../utf-4088/SLeeLa-ALPHABET-INTEGRATION.md). Los identificadores UTF-4088 pertenecen a un espacio de nombres independiente; no son valores escalares Unicode. El trabajo de vinculación con el compilador y SLVM se sigue expresamente por separado de estos fundamentos nativos.

## XII. AutoCAD Creator™

[**AutoCAD Creator™**](../../autocad/creator/README.md) es una aplicación CAD 2D interactiva escrita íntegramente en SLeeLa. Mientras que el AutoCAD Renderer hermano convierte un plano en un dibujo de una sola vez, Creator permite **crear y modificar** en directo un documento AutoCAD 2D: añadir paredes, habitaciones, cotas y notas; después mover, cambiar etiquetas, reorganizar capas, eliminar y deshacer elementos; y **guardar** el resultado como un dibujo DXF real de AutoCAD (texto R12, compatible con AutoCAD / LibreCAD).

Su interfaz gráfica está construida sobre **SleelaUI™**, el propio conjunto de herramientas de interfaz multiplataforma de SLeeLa (en [`lib/user-interface`](../../lib/user-interface)), no sobre JavaFX, y abre una ventana nativa real en X11 / Cocoa / Win32. El diseño sigue una regla: **«SleelaUI presenta; SLeeLa decide»**. La GUI no calcula geometría ni escribe DXF. Cada herramienta emite una intención de crear, modificar, eliminar, deshacer o guardar al componente autorizado [`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela), que es propietario del documento editable y de un historial reversible de comandos; el lienzo únicamente previsualiza la geometría que Creator mantiene.

El motor de edición reutilizable reside en [`lib/autocad`](../../lib/autocad): [`SLCadEntity`](../../lib/autocad/SLCadEntity.sleela), [`SLCadDocument`](../../lib/autocad/SLCadDocument.sleela), [`SLCadCommand`](../../lib/autocad/SLCadCommand.sleela) y [`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela). Funciona de extremo a extremo sin interfaz gráfica. El armazón de la GUI y la demostración se encuentran en [`autocad/creator`](../../autocad/creator):

```sh
make creator          # desde autocad/: ejecuta la demostración de Creator -> out/altered.dxf
```

En una sesión con pantalla, se abre la ventana de edición en directo; en un sistema sin interfaz gráfica, se construye el dibujo en memoria, se imprime un informe junto con el DXF y el proceso termina limpiamente.

## XIII. Ordenación del mapa de entrada UTF-4088

El catálogo especializado UTF-4088 define su recorrido espacial del mapa de entrada y sus iteraciones abstractas de presión en [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md). La política del índice de materias del Registro Nacional de Gravámenes se encuentra en [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md). Las coordenadas del mapa, los niveles de presión (1–50), los identificadores permanentes del catálogo y los rangos alfabéticos son campos independientes; ninguno debe sustituirse silenciosamente por otro. La presión es un nivel de iteración algorítmica, no una magnitud física.

## XIV. Índice de paquetes recientes y ampliados

Este índice añadido destaca paquetes ampliados recientemente, bibliotecas de dominio y colecciones de código fuente. Las entradas se agrupan alfabéticamente por nombre de paquete o tema. Las descripciones identifican la ubicación en el repositorio; la implementación y el estado de validación deben comprobarse en la documentación y las pruebas de cada paquete.

### A–C

- **Advantage** — `lib/advantage/` y `advantage/`: optimización de un dividendo único con respecto a un modelo, mediante una estructura por capas denominada Sequitur onion, con cinco subcarpetas temáticas: `math/` (maximización conjunta de beneficios, negociación de Nash y modelo bilateral newsvendor), `physics/` (métrica, geodésica, contacto y SDE del campo de confianza; analogía de modelización expresamente señalada), `integration/` (compartimentos de integración vertical y puntuación de sofisticación), `grading/` (evaluador de circuitos de sofisticación A–D para módulos y conjuntos) y `output/` (emisión a terminal, CommonRails o socket). Un consumidor nativo C/C++ (`advantage/`) integra el modelo en ejemplos prácticos de comercio. Véanse [`lib/advantage/README.md`](../../lib/advantage/README.md) y [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md).
- **Alphabet y UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/` y `utf-4088/`: catálogos de alfabetos, soporte nativo de conjuntos de caracteres, ordenación de mapas de entrada y documentación de integración UTF-4088.
- **AutoCAD Creator** — `autocad/creator/` y `lib/autocad/`: flujos de trabajo interactivos de dibujo 2D y componentes reutilizables para documentos y edición CAD.
- **Ballum** — control de pantalla a nivel de píxel: establecer y consultar píxeles, copiar bloques de regiones, ajustar brillo/temperatura/gamma de toda la pantalla, entrada de puntero y teclado, y una **superficie framebuffer a pantalla completa** para un control total, propio y rápido de cada píxel (ventana override-redirect y presentación mediante una sola llamada `XPutImage`/MIT-SHM en Linux/X11). El núcleo nativo está en `ballum/`, la fachada SLeeLa en `lib/ballum/` y el puente de VM en `impl/`; véanse [`ballum/README.md`](../../ballum/README.md) y el [ejemplo ejecutable](../../ballum/example/).
- **CDMA** — `cdma/` y `lib/cdma/`: material técnico relacionado con CDMA y trabajo de biblioteca de código fuente SLeeLa.
- **Character** — `lib/character/`: perfiles de personajes y estadísticas de personajes reutilizables, incluidos ejemplos de perfiles biográficos.
- **Coorenagraph** — `lib/coorenagraph/`: razonamiento ético, vocabulario moral, deliberación y conceptos relacionados; véanse el README del paquete y sus archivos fuente.

### D–G

- **Modelos de fuentes de gobernanza** — [`lib/governance`](../../lib/governance/README.md) define estructuras declarativas de código fuente SLeeLa para propuestas, votaciones manuales anónimas y recuentos agregados. La anotación propuesta `@vote` y el proceso manual se documentan en [`registry/DEMOCRATIC-VOTING-SYNTAX.md`](../../registry/DEMOCRATIC-VOTING-SYNTAX.md); los compiladores actuales la tratan como metadatos de comentario sin efecto.

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` hasta los directorios disponibles de paquetes HTTP versionados: especificaciones de protocolo e implementaciones específicas de cada versión. Consulte la documentación de cada versión para conocer su alcance preciso y estado de verificación.
- **Corte Penal Internacional** — `lib/international-criminal-court/` e `INTERNATIONAL.CRIMINAL.COURTS.md`: código fuente y documentación relacionados con este dominio.

### O–S

- **OFDMA** — `ofdma/`: material técnico de acceso múltiple por división de frecuencias ortogonales y recursos de desarrollo C/C++.
- **Filosofía y módulo Conversation Mask** — [`philosophy/`](../../philosophy/README.md) contiene los fundamentos nativos C/C++, y [`philosophy/module/`](../../philosophy/module/README.md) organiza la siguiente capa del módulo en tres áreas explícitas: [`data/`](../../philosophy/module/data/README.md), para observaciones tipadas y fechadas, finales de eventos, ajustes de semivida, procedencia y consentimiento; [`helper/`](../../philosophy/module/helper/README.md), para validación determinista, decaimiento de relevancia, comparaciones compatibles y presentación filtrada por consentimiento; y [`psychiatry-friend/`](../../philosophy/module/psychiatry-friend/README.md), para una interfaz conversacional de apoyo y no clínica. Los puntos de acceso orientados a SLeeLa y el Tree compuesto se encuentran en [`lib/philosophy/`](../../lib/philosophy/README.md), junto al paquete de filosofía más amplio. El módulo utiliza entradas explícitas y prioriza el consentimiento: no supervisa pasivamente a personas ni medios, no diagnostica, no infiere intenciones privadas, no asigna puntuaciones al valor humano ni recomienda cambios de medicación. Los vínculos entre eventos conservan la procedencia y la navegación, pero no constituyen corroboración independiente.
- **Synchro** — integración nativa del compilador y del entorno de ejecución, además de funciones relacionadas con el despacho de paquetes y la medición; véanse el código fuente correspondiente en `impl/` y la implementación del compilador.
- **SleelaUI / interfaz de terminal** — `lib/user-interface/` y el proyecto `sleela-terminal`: integración de la interfaz de usuario SLeeLa y componentes de interacción orientados al terminal.

### U–W

- **Naciones Unidas** — `united-nations/`: materiales del repositorio específicos de este ámbito.
- **Wi-Fi 4, Wi-Fi 5 y Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`, `wi-fi-6/` y los directorios de código relacionados `lib/wifi/`: material de redes inalámbricas y recursos de configuración específicos de cada generación.

### Mantenimiento del índice de paquetes

Cuando se añada un paquete o se amplíe sustancialmente, incorpore aquí su entrada en el grupo alfabético adecuado, enlace su directorio y README canónicos, y describa únicamente capacidades respaldadas por el código fuente incluido en el repositorio. Mantenga este índice como una adición: no elimine ni reescriba las secciones existentes del README al actualizar el índice de paquetes.

---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

`bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh` :: `⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓`

`bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc` :: `⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉`

Democracia de EE. UU. y política estadounidense.

---

## XVI. Imágenes de marca de ciencia nacional

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="Ciencia Nacional SLeeLa" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="NSF Nacional SLeeLa" width="560"></p>

## XVII. Catálogo experimental UTF-4088 de un millón de entradas

El [generador del catálogo UTF-4088](../../utf-4088/tools/generate_catalogue.py) construye de forma determinista un catálogo experimental de símbolos con 1.048.576 entradas. El formato compacto documentado se describe en [`utf-4088/FORMAT.md`](../../utf-4088/FORMAT.md); el contrato de generación y validación se encuentra en [`utf-4088/GENERATION-RESULTS.md`](../../utf-4088/GENERATION-RESULTS.md).

Ejecute el generador con Python 3:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

El archivo binario generado tiene 16.777.244 bytes y está diseñado para permanecer por debajo de 20.000.000 bytes. Las pruebas unitarias y un flujo de trabajo de GitHub Actions verifican el número de entradas, la unicidad de los glifos, el tamaño y SHA-256; las ejecuciones satisfactorias publican el binario y el resumen como artefacto del flujo de trabajo `utf4088-million-catalogue`. El binario generado se mantiene deliberadamente fuera del historial de Git. UTF-4088 sigue siendo experimental y no es compatible con Unicode.

---

# SLeeLa
<p align="center"><img src="../../images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. Incorporaciones recientes: Pi, Modium/Registry, sleelactl y `@native`

Esta sección recoge las funciones añadidas durante el trabajo de 2026-11, con los grupos más recientes primero. Los archivos fuente enlazados y su estado de validación son la referencia autorizada; los cambios en revisión no se describen como integrados hasta que llegan a `master`.

### Constante de catálogo Pi de 100.000.000 de dígitos

El repositorio incluye un valor verificado de π de **cien millones de dígitos** en [`pi-generator/pi.100000000.digits`](../../pi-generator/pi.100000000.digits), además de un valor de 57.344 dígitos en [`pi-generator/pi.57344.digits`](../../pi-generator/pi.57344.digits). Ambos son valores auténticos de π verificados independientemente. [`pi-generator/VERIFIED.md`](../../pi-generator/VERIFIED.md) documenta las comprobaciones: prefijo de referencia, punto de Feynman en la posición fraccionaria 762, dígito de la posición un millón, concordancia con `lib/math/SLPiReference.sleela`, análisis de frecuencias de todo el archivo y script de reproducción.

### Modium™ / Registry™ cargan la constante Pi al iniciar el subsistema

En los puntos de entrada dedicados —`sleela registry` y `sleela modium`, así como en la primera ejecución del instalador—, y **no** en cada `sleela run`, los subsistemas cargan la constante grande del catálogo π. En terminales interactivos aparece una línea roja transitoria `Loading Modium™` / `Loading Registry™`, que se borra antes de la salida normal. El efecto solo se activa en TTY; la salida canalizada y la de pruebas permanecen idénticas byte por byte. El localizador, no los bytes, se registra como valores predeterminados de las claves `Math` `piReferenceLargePath` y `piReferenceLargeDigits`, tanto en el modelo [`lib/registry/SLRegistry.sleela`](../../lib/registry/SLRegistry.sleela) como en el inicializador nativo [`registry/sleela_registry.c`](../../registry/sleela_registry.c). `lib/math/SLPiReference.sleela` expone `largePath()` / `largeDigits()`. La constante se instala en la ubicación SLeeLa del sistema operativo (`<data>/pi/`) mediante los instaladores estándar `make` y guiado «beautiful». Detalles: [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](../../pi-generator/MODIUM-REGISTRY-PI-STARTUP.md).

### `sleelactl`: instalador guiado y tema `math` nativo

- El instalador guiado («beautiful») instala ahora `sleelactl` (el plano de control de SLeeLa) y su registro de módulos, al igual que el instalador `make`. Véase [`bin/SLEELACTL.md`](../../bin/SLEELACTL.md).
- `sleelactl` incorpora la naturaleza **`subject`** para bibliotecas de temas nativos C/C++ compiladas en el runtime, y registra la primera: **`math`**. `sleelactl run math` ejecuta de extremo a extremo el backend `__NativeMath` (`impl/subjects/math/math_native.cpp`) mediante su demostración de referencia; `sleelactl info math` explica el acceso mediante `import math;`. Esta naturaleza solo ofrece `run` e `info`; no incluye `check`, porque `sleela check` no carga los temas nativos registrados mediante SHEET.

### `@native`, Native Drive y ejecución multilingüe

La nueva anotación de método **`@native`** solicita compilación nativa anticipada (*ahead-of-time*) para un método intensivo. El comando `sleela aot` localiza estos métodos, verifica que estén dentro del *subconjunto descargable* admitido y genera C/C++. **Falla de forma cerrada**: rechaza con un motivo preciso toda construcción que aún no pueda traducir, evitando generar métodos parcialmente traducidos.

```sh
sleela aot --list <file.sleela>    # veredicto por método: ok / REFUSED (motivo)
sleela aot --emit <file.sleela>    # genera C/C++ (stdout) para un programa admitido
```

`@native` sigue siendo una solicitud de tratamiento AOT, y las construcciones no admitidas se rechazan antes de la emisión. El [Native Drive](../../impl/native-drive/README.md) admite compilaciones de ejecutables y bibliotecas compartidas C/C++ de varias unidades mediante compiladores del host (incluido MSVC), la salida AOT admitida de SLeeLa, la inspección de símbolos frente a la ABI del cargador nativo, la ejecución de procesos sin shell, la gestión de stdin/stdout/stderr, el encadenamiento de varias etapas mediante tuberías y la delegación de XClass al cargador de producción. `sleelactl` expone sus comandos de compilación y ejecución, y encamina la inspección de XClass y la generación e instalación explícitas de unidades systemd mediante `inspect native-drive`. La VM posee una tabla acotada de manejadores de módulos nativos, con autorización obligatoria del host y manejadores verificados por generación. La sintaxis 3.0 añade ahora `nativeOpen`, `nativeCall` y `nativeClose`: el despacho de bytecode de solo anexión atraviesa una ABI con versión independiente (null/bool/int64/double/UTF-8), mientras que los arrays, structs y demás valores compuestos propiedad de la VM permanecen dentro del intérprete y pueden recurrir al bytecode del mismo nombre como alternativa. Los plugins pueden recibir opcionalmente una tabla de callbacks del host con versión para invocar funciones SLeeLa exactas mediante ejecución anidada de la VM, con raíces de GC para pilas suspendidas, una lista de permitidos explícita de nombres de callback del host y rechazo de las llamadas recursivas a la API nativa. El código nativo sigue siendo código de confianza dentro del proceso, no un entorno aislado. Las anotaciones de política de procesos en el código fuente tienen un esquema con versión, pero todavía no se analizan ni se aplican. Detalles, límites de seguridad y registro de los siete flujos de trabajo: [Native Drive](../../impl/native-drive/README.md) y [`pi-generator/AT-NATIVE.md`](../../pi-generator/AT-NATIVE.md).


## XIX. Ediciones internacionales del README

SLeeLa ofrece puntos de entrada README específicos por país para los 50 mercados de su edición internacional. Cada país dispone de un archivo `README.<CC>.md`; cuando corresponden varios idiomas, se agrupan dentro del archivo de ese país.

- [README.AE](README.AE.md) · [README.AT](README.AT.md) · [README.AU](README.AU.md) · [README.BE](README.BE.md) · [README.BR](README.BR.md)
- [README.CA](README.CA.md) · [README.CH](README.CH.md) · [README.CL](README.CL.md) · [README.CN](README.CN.md) · [README.CO](README.CO.md)
- [README.CZ](README.CZ.md) · [README.DE](README.DE.md) · [README.DK](README.DK.md) · [README.EE](README.EE.md) · [README.ES](README.ES.md)
- [README.FI](README.FI.md) · [README.FR](README.FR.md) · [README.GB](README.GB.md) · [README.GR](README.GR.md) · [README.HK](README.HK.md)
- [README.HU](README.HU.md) · [README.ID](README.ID.md) · [README.IE](README.IE.md) · [README.IL](README.IL.md) · [README.IN](README.IN.md)
- [README.IS](README.IS.md) · [README.IT](README.IT.md) · [README.JP](README.JP.md) · [README.KR](README.KR.md) · [README.LU](README.LU.md)
- [README.MX](README.MX.md) · [README.MY](README.MY.md) · [README.NL](README.NL.md) · [README.NO](README.NO.md) · [README.NZ](README.NZ.md)
- [README.PH](README.PH.md) · [README.PL](README.PL.md) · [README.PT](README.PT.md) · [README.QA](README.QA.md) · [README.RU](README.RU.md)
- [README.SA](README.SA.md) · [README.SE](README.SE.md) · [README.SG](README.SG.md) · [README.SI](README.SI.md) · [README.SK](README.SK.md)
- [README.TH](README.TH.md) · [README.TR](README.TR.md) · [README.TW](README.TW.md) · [README.US](README.US.md) · [README.ZA](README.ZA.md)

**Política de traducción:** Estos archivos son puntos de entrada localizados iniciales. Sus introducciones y encabezados de idioma están localizados, mientras que la referencia técnica completa permanece en el [README.md canónico en inglés](../../README.md). Ningún archivo de país debe describirse como traducción completa hasta traducir todas sus secciones, comprobar los identificadores técnicos y enlaces, y someter la edición a revisión lingüística. La cobertura de esta primera fase es representativa; no afirma que todos los idiomas reconocidos constitucionalmente, regionales, indígenas o de signos estén traducidos íntegramente.

## Apéndice — Orientación regional conservada

### Referencias principales

- [README canónico](../../README.md)
- [Especificación de sintaxis SLeeLa](../../SLEELA.syntax)
- [Índice de documentación](../../docs/README.md)
- [Documentación API](../../docs/api/README.md)
- [Soporte de plataformas](../../docs/PLATFORM-SUPPORT.md)
- [Política de pruebas](../../markdown/TESTING.md)
- [Repositorio SLeeLa](https://github.com/mearvk/SLeeLa)

**Estado de traducción:** las secciones I–XIX están traducidas al español. Se han conservado los identificadores técnicos, comandos, ejemplos de código, nombres de archivo, enlaces y direcciones. Se recomienda una revisión lingüística final por hablantes nativos.
