# SLeeLa 3.0 — España (ES)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>

<p align="center"><strong>Un lenguaje nativo de sistemas, con compilador, entorno de ejecución y ecosistema de máquinas virtuales.</strong></p>

> **Edición internacional por país.** Este archivo reúne las versiones lingüísticas previstas para España: español, catalán, gallego y euskera. La traducción técnica se realiza por etapas. Las secciones traducidas se reproducen íntegramente; las pendientes se identifican expresamente.

**The Now. The Ingredient.** Un lenguaje de programación es más que su sintaxis: incluye el compilador, el entorno de ejecución, las bibliotecas, las herramientas y los contratos de plataforma que hacen que el software sea utilizable.

**Ámbitos de trabajo de SLeeLa.** SLeeLa aborda **Math**, **Object**, **Science** y **Reach**, las cuatro dimensiones de su trabajo: lo calculable (Math), el objeto establecido en el catálogo de lo que existe (Object), la investigación disciplinada de lo verdadero (Science) y la ampliación de la capacidad de un sistema hacia otros sistemas y personas (Reach). **Los intereses nacionales siguen vigentes**; ninguna de estas cuatro dimensiones los sustituye. Cuando un tema afecta a los intereses de una nación, esa relación se conserva y documenta fielmente, por ejemplo, en los campos de metadatos de una clase `metaOrigin` y `metaNationalSchedule`.

## I. Primeros pasos

- [Compilar la implementación nativa](impl/README.md)
- [Máquinas virtuales y Modium: guía práctica del usuario](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [Pila de CPU en el código fuente de SLeeLa](lib/cpu/CPU.md): una cadena de máquina completa, desde las puertas lógicas hasta la ALU, los núcleos, el sistema operativo y los programas; incluye más de 130 perfiles de CPU y [tensiones, frecuencias variables y parámetros de temporización avanzados](lib/cpu/TIMING.md), con ejecución [según especificación o mediante overclocking](lib/cpu/components/README.md).
- [Índice de documentación](docs/README.md)
- [Fuente de datos CSPML y referencia de investigación Salt & Salary](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md): procedencia y orientación de integración para datos externos de EE. UU. sobre lugares, condiciones socioeconómicas, fuerza laboral, educación y escuelas públicas.
- [Documentación de API generada](docs/api/README.md): referencia HTML actual, versión de API y orientación para regenerarla.
- [Catálogo de API](api/README.md)
- [Estándares de comparación computacional](comparison/README.md): estándares de tipos, memoria, ABI y conformidad para SLeeLa VM, Java/JVM y C/C++ nativo.
- [Stanchion de recursión: prototipos C y C++](recursion/stanchion/README.md): ejemplos especializados en los tipos primitivos Boolean, Byte, Character, Integer, Long, Float y Double, además de patrones de recursión genéricos, aplicados, científicos y educativos.
- [Matriz de compatibilidad y validación de plataformas](docs/PLATFORM-SUPPORT.md)
- [Guía de marca](docs/BRAND-GUIDE.md)
- [Conceptos y recursos gráficos de merchandising](merchandise/README.md)
- [Logotipo de NitroWebExpress](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [Política de pruebas y criterios de publicación](markdown/TESTING.md)

## II. Novedades

Incorporaciones recientes al lenguaje, los controladores y la biblioteca estándar:

- **Paquete estándar `advantage`: distribución de dividendos con matemáticas, física, integración, evaluación y salida enrutable.** [`lib/advantage/`](lib/advantage/README.md) modela la obtención de un dividendo único con respecto a un modelo mediante una estructura por capas denominada **Sequitur onion** (dividendo → revisión → cuestiones → matemáticas → adiciones → sustracción → reagrupación → iteración). Se puede recorrer hacia dentro y hacia fuera, con actualización dinámica y restitución completas, mediante un **modelo de usuario** sencillo y un **modelo de desarrollador** con control integral. Comprende cinco áreas: **`math/`**, economía del comercio bilateral (maximización conjunta de beneficios, negociación de Nash y razón crítica bilateral del modelo newsvendor); **`physics/`**, una «física de los campos de confianza» aplicada a la economía estructural (confianza como tensor métrico, geodésica transaccional, estructura de contacto de anuncios y una SDE de decaimiento de confianza CIR con saltos, presentada expresamente como analogía de modelización); **`integration/`**, modelo de integración vertical componible recursivamente para capitalismo y comercio minorista (compartimentos, bahías de ingeniería, compartimentos de prueba, bandejas de mediana newtoniana y puntuación de sofisticación del sistema); **`grading/`**, evaluador de circuitos de sofisticación que califica cada módulo y conjuntos de módulos ganadores, perdedores, evaluadores o absolutos A–D; y **`output/`**, una interfaz de emisión enrutable que dirige la salida al terminal, a una superficie con formato **CommonRails** o a un socket/conector (previsto para un `slnet_write` nativo). Cada clase contiene el registro Metadata-Class; cinco demostraciones ejecutables validan las áreas. La documentación complementaria está en [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md), [`MATH.md`](lib/advantage/MATH.md) y [`PHYSICS.md`](lib/advantage/PHYSICS.md). Un consumidor nativo C/C++ en [`advantage/`](advantage/README.md) —con el núcleo C `advantage_value`/`advantage_mean`, un almacén de conocimiento XML y una biblioteca segura de adquisición HTTPS/SFTP/SSH, almacén de claves y transporte DH/ECDH— conecta el modelo con [ejemplos de comercio](advantage/examples/README.md): operaciones positivas procedentes de fuentes de internet supuestas, una secuencia de relevancia para Traders, Consumers, Helpers y Healers, y ocho modelos clasificados de A a D (D = máximo Cement; A = máximo Brand).
- **Anotaciones de temporización de bucles: `@loopDelay` / `@loopBudget` y función integrada `sleep(ms)`.** Son las primeras anotaciones **de comportamiento a nivel de instrucción** de SLeeLa: se escriben en una línea propia antes de un bucle `for`, `while` o `do` e insertan una pausa real en su ejecución. `@loopDelay(ms = 250)` y `@loopDelay(seconds = 2)` esperan una cantidad fija por iteración (la forma abreviada `@loopDelay(250)` usa milisegundos); `@loopBudget(seconds = 10, iterations = 1000)` distribuye un número conocido de iteraciones a lo largo de un presupuesto total de tiempo de pared. Ambas se reducen al nuevo opcode append-only **`OP_SLEEP`** (ISA 157), que también respalda la función general integrada **`sleep(<ms>)`**, invocable desde cualquier lugar. La pausa se implementa de forma portable mediante `nanosleep` / `Sleep`. Está especificado en [`SLEELA.syntax` §32](SLEELA.syntax), documentado en la [referencia API de temporización](api/sleela/3.0/LOOP_TIMING_API.md) y demostrado en [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela). El índice de incorporaciones de la sesión —que también incluye la multiplicación nativa de enteros grandes `OP_BIGMUL` y las herramientas independientes [local-generator](local-generator/README.md) y [pi-generator](pi-generator/README.md)— está en [`SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md). Las tres superficies de API generadas ([`sleeladoc-api/`](sleeladoc-api/README.md), [`docs/api/`](docs/api/README.md) y la instantánea inmutable [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html)) se reconstruyeron con `make api-docs` e incluyen las nuevas instrucciones `SLISASleep` y `SLISABigMul` en la página del paquete `vm`.
- **SleelaDoc desde la línea de comandos: `sleela docs`.** SleelaDoc ya está disponible directamente en el terminal. `sleela docs` enumera todos los temas documentables; `sleela docs crypto` muestra un paquete; y `sleela docs stringbuilder` resuelve una clase sin distinguir mayúsculas y minúsculas, además de reconocer el prefijo estándar `SL` y resolverlo como `SLStringBuilder`. Muestra una página completa con **definición, finalidad de diseño, relaciones, estructura, user clays y una escala de dominio de cinco niveles**, además de sugerencias clasificadas de «¿quería decir?» ante nombres aproximados. Los autores amplían cualquier página añadiendo etiquetas SleelaDoc `@` como `@intent`, `@thesis`, `@axiom`, `@param`, `@see` y `@mastery`. `sleela docs --generate [dir]` genera un corpus de texto plano (una página por clase y un índice `TOPICS.index`) que la CLI puede leer después sin conexión; **`make sleeladoc`** empaqueta ese corpus y sus fuentes `.sleela` correspondientes en `sleeladoc/build/sleeladoc.jar`. Para temas que solo disponen de una fachada, como `psychiatry` y `politico`, `sleela docs --facade <topic>` busca en el sistema de archivos durante la ejecución y enumera las carpetas reales y después los archivos, con límites configurables por el usuario de **6 MB, 6000 archivos y 6 minutos**. Las unidades de `lib/sleeladoc` utilizan la edición actual **`#sleela 3.0`**. Véanse [`man/man1/sleela.1`](man/man1/sleela.1) y [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md).
- **SleelaDoc: documentación de estilo Javadoc con sitio HTML generado.** **SleelaDoc** es la convención de comentarios documentales en el código fuente de SLeeLa (comentarios de ángulo delimitados por `@doc` / `@page`; véase [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)). [`lib/sleeladoc`](lib/sleeladoc/README.md) es el **extractor** que convierte una unidad `.sleela` en una página API Markdown o HTML, de forma comparable a cómo `javadoc` genera documentación Java, y documenta la *intención*. El sitio HTML completo y navegable de todo `/lib` se genera en [`/sleeladoc-api`](sleeladoc-api/README.md) mediante `tools/generate-sleeladoc-api.py` (abra `sleeladoc-api/index.html`): utiliza una presentación limpia en azul, azul claro, gris claro y blanco, una jerarquía paquete → clase → miembro y un glosario dirigido al lector *Student 2*. El compilador también produce como resultado de la compilación documentos marcados por el autor (`--wiki`: una página MediaWiki y una copia de la fuente de origen), descritos en [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md). Ejemplo ejecutable: [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela).
- **Modelo de objetos de documento XML (`lib/xml`).** DOM XML del lado de SLeeLa: un documento posee un árbol de nodos de elemento, texto, comentario, CDATA e instrucción de procesamiento, con hijos ordenados y atributos con nombre; incluye un serializador compacto o legible (escape XML 1.0 §2.4), un analizador por celdas de caracteres y el registro de calidad `SLXmlAspect`. La igualdad entre nodos sigue tres niveles de confianza (identidad → mismo sujeto → estructura), y es reflexiva, simétrica y determinista. Véanse [`lib/xml/README.md`](lib/xml/README.md) y el ejemplo ejecutable [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela).
- **Primitivas de enteros con signo de gran anchura (`long`, `int64`, `int128`).** `int`, `long` e `int64` son enteros con signo de 64 bits y se verifican y ejecutan de forma idéntica. `int128` se acepta como tipo entero, aunque por ahora utiliza una representación de 64 bits; el soporte real de 128 bits está documentado como trabajo futuro. La serie de enteros con signo de base dos, `int32`…`int16777216` (sintaxis 2.7), y la serie sin signo correspondiente, `uint32`…`uint16777216` (sintaxis 2.8), amplían estas capacidades. Véase [`SLEELA.syntax`](SLEELA.syntax) §21, §24 y §25.
- **Campos consecuentes `@mix` (sintaxis 2.9).** Una clase puede declarar varios campos de instancia con el **mismo nombre** cuando cada repetición lleva `@mix`. El lenguaje interpreta el grupo como una secuencia (un array o lista indexada): por defecto, en orden ascendente `1,2,3,…`, o de forma manual mediante `@mix(1,2,3,...)`; la cantidad de índices debe coincidir con la cantidad de campos. Véase [`SLEELA.syntax`](SLEELA.syntax) §28.
- **Marcadores documentales: el Proceso Natural, denominado Preonems (`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).** Estas palabras desnudas que marcan documentos reciben colectivamente el nombre de **Preonems** (un marcador es un *Preonem*; el conjunto, los *Preonems*; véase [`SLEELA.syntax` §26.1](SLEELA.syntax)). Son marcadores documentales declarativos de primera clase que expresan qué *es* un documento en el orden de su creación: `author`/`authorial` (sintaxis 2.4; gestionado por el consejo profesional, el único miembro con efecto de comportamiento), `material;` (Source establecido) y `memory;` (sintaxis 2.5), `determine;` (sintaxis 2.6; documento abierto a procesamiento posterior por un doctor, médico o ghost hasta su conclusión por vía legal), y los dos más recientes: **`object;`** (una cosa establecida en el catálogo de lo que existe, reificada y dotada de posición concreta en sintaxis 2.8) y **`field;`** (el ámbito relacional en el que los objetos existen y se mueven), ambos habilitados desde la sintaxis 2.6. Cada marcador es contextual, independiente y condicionado por versión en el frontend; no altera el análisis sintáctico, el tipado ni la generación de código. Fuera de la posición de marcador, cada palabra sigue siendo un identificador normal; por ejemplo, `field` no interfiere con un miembro de clase ni con un acceso `.field`. Véase [`SLEELA.syntax`](SLEELA.syntax) §18–§20, §23 y §26.
- **Nordshrift en sintaxis 3.0 con acceso completo a la biblioteca.** Nordshrift resuelve los nombres mediante el mismo índice recursivo de `/lib` que utiliza el compilador, por lo que alcanza todos los paquetes y unidades `.sleela`; `nordshrift check` informa de los recuentos de todo el árbol.
- **Stanchion de recursión.** [`recursion/stanchion/`](recursion/stanchion/README.md) incluye ahora prototipos de referencia C y C++ específicos para los siete contratos primitivos documentados, además de ejemplos genéricos, aplicados, científicos y didácticos. Son fuentes de referencia: deben compilarse y probarse antes de considerarlas verificadas.
- **Comentarios con corchetes angulares (`<` / `<<`).** Nuevo estilo de comentarios para desarrollos futuros: `< line >` y `<< block >>`, reconocido por el frontend de SLeeLa y documentado en [`SLEELA.syntax` §1.1](SLEELA.syntax).
- **Literales enteros con radix.** Los literales `int` aceptan ahora formas hexadecimal (`0x`), binaria (`0b`) y octal (`0o`), con separadores de dígitos `_`, tanto en el lenguaje fuente SLeeLa ([`SLEELA.syntax` §1.4](SLEELA.syntax)) como en el formato de modelos de CPU [`.xlt`](lib/xlt/README.md).
- **Estándar Metadata Class en toda la biblioteca.** El metapatrón de metadatos descriptivos (procedencia, legalidad y ámbito jurisdiccional) se aplica ahora a todos los archivos `.sleela` que contienen clases bajo `/lib`. Véanse [`SLEELA.syntax` §15](SLEELA.syntax) y la [lección del tutorial](tutorial/13-metadata-class.md).
- **Familia de texto y String ampliada.** `lib/text` incorpora asistentes reales en el lenguaje, una vista por celdas de caracteres con operaciones auténticas `charAt`, `substring`, `indexOf`, `reverse` y alineación de columnas, además de un motor de plantillas posicional. Véanse el [tutorial de texto y String](tutorial/14-text-and-strings.md) y el ejemplo ejecutable [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela).
- **`String` de primera clase en las hojas Nordshrift.** Una hoja `.sst` puede declarar un bloque tipado `strings:` que contribuye al manifiesto de componentes de la compilación. Véase la [referencia de Nordshrift Sheet](wiki/Nordshrift-Sheet-Reference.md).
- **Modelos de CPU ejecutados mediante opcodes y equivalencia de llamadas a funciones.** Cada `SL<ARCH>CPU` hereda tanto la capacidad de ejecutar un flujo de instrucciones nativas mediante su ciclo de búsqueda, decodificación y ejecución como la de ejecutar el código de otro proceso supuesto mediante llamadas equivalentes sobre la infraestructura compartida de la VM. Véase [`lib/cpu/EXECUTION.md` §5](lib/cpu/EXECUTION.md).

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

El objetivo `registry` ejecuta [`build-registry.sh`](build-registry.sh): compila el núcleo del registro, incorpora la tabla base al almacenamiento de texto portable `registry/build/sleela.sleelareg` y exporta el script de tabla MySQL `registry/build/registry.sql` (mediante `sleela registry export`). El registro puede revisarse con `sleela registry` o `registry/build/sleela-registry review`. Consulte la [guía del registro](registry/README.md) y la [arquitectura Registry 3.0](registry/REGISTRY-3.0-ARCHITECTURE.md).

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

Primero se pregunta si se desea instalar el paquete principal y después se elige el alcance de la instalación. Si el proceso termina correctamente, puede aparecer un selector opcional de módulos de mejora seleccionados. El flujo interactivo completo y todas las opciones se describen en [Instalación rápida y segura](install/README.md).

## V. Lenguaje y entorno de ejecución

La gramática normativa se encuentra en [`SLEELA.syntax`](SLEELA.syntax). El repositorio documenta actualmente la sintaxis **3.0**; la versión del compilador/herramientas y la versión de sintaxis del código fuente son independientes. La superficie 3.0 añade los marcadores documentales **`pure` y `gasoline`** (§30): dos Preonems declarativos y contextuales, sin efecto de ejecución propio, que expresan la posición de un documento mediante su propia honestidad. La sintaxis 3.0 reúne las superficies del lenguaje 1.0 a 2.9 en una edición normativa actual y conserva los requisitos de versión mínima de cada función.

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

El manifiesto de integridad se genera en `antivirus/build/`. Incluye entradas de código fuente y documentación bajo seguimiento, así como archivos protegidos de índices de firmas; excluye los resultados generados y la caché mutable de ClamAV. Las instantáneas de bases de datos de ClamAV tienen sus propios manifiestos. Los ZIP de publicación incluyen `INTEGRITY-MANIFEST.json`, `SHA256SUMS` y un archivo complementario `.sha256`. Compare los hashes de los archivos descargados con un valor de confianza obtenido de forma independiente: una suma de comprobación almacenada junto al archivo no constituye una firma del editor. Consulte la [guía de Sleelavirin](antivirus/README.md), la [lista de comprobación de seguridad](antivirus/SECURITY.md) y el [flujo de compilación del antivirus](.github/workflows/antivirus-binaries.yml).

Una versión solo se considera verificada cuando los trabajos de CI pertinentes han terminado correctamente para el commit exacto de esa versión y se han comprobado los artefactos resultantes.

## VIII. Temas del editor SLeeLa: Vim y Emacs

SLeeLa incluye un conjunto coordinado de esquemas de color oscuros y profesionales para **Vim** y **Emacs**, además de una utilidad nativa de paletas para C/C++. El esquema original **American Professional** sigue siendo el predeterminado. Hay diez paletas complementarias inspiradas en distintos diseños culturales e institucionales; son interpretaciones de diseño, no representaciones oficiales de ninguna nación, cultura, corte o institución.

### Los once esquemas y sus colores

| Esquema | Fondo | Primer plano | Palabras clave | Tipos | Cadenas | Comentarios |
|---|---|---|---|---|---|---|
| **American Professional** | `#101820` | `#F4F7FA` | `#7FB3FF` | `#73DACA` | `#F2C078` | `#8A96A3` |
| **Pan-Eurasian** | `#111827` | `#F0F2F5` | `#7AA7E8` | `#69C9B3` | `#D7B56D` | `#8995A5` |
| **Russian and Slavic — Medium** | `#202733` | `#E8EAF0` | `#A9C0DD` | `#8FC4B2` | `#D6BD8B` | `#9AA3AF` |
| **Central Asian / Silk Road** | `#1C2529` | `#F5F0E5` | `#7FADE0` | `#45C6C2` | `#D9B56D` | `#92A3A0` |
| **East Asian** | `#171A1C` | `#F2EFE7` | `#D97878` | `#72C7A5` | `#D8B76A` | `#969F9B` |
| **ICC — Corte Penal Internacional** | `#0E1A2B` | `#EAF0F7` | `#5B8FD6` | `#7FB7C9` | `#D9B25A` | `#7E8CA0` |
| **SCOTUS — Tribunal Supremo de EE. UU.** | `#1A1416` | `#F3EEE6` | `#9E2B35` | `#8FA9B8` | `#C9A24B` | `#8C8178` |
| **Greek (Hellenic)** | `#0A1A30` | `#FFFFFF` | `#2E7BD6` | `#5E9FE0` | `#9FC3EC` | `#6E86A6` |
| **Turkish (Al Bayrak)** | `#2A0609` | `#FFFFFF` | `#F23D48` | `#F0A9AE` | `#F0C9A9` | `#A8767A` |
| **Kiro** | `#0B0B12` | `#F5F5FA` | `#A98EDA` | `#6E8BE0` | `#E8C547` | `#6A6482` |
| **SLeeLa** | `#08080E` | `#FFFFFF` | `#B07CFF` | `#5A7CFF` | `#FFD21E` | `#6E5F9E` |

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
| **American Professional** | ![Paleta American Professional](lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Paleta Pan-Eurasian](lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russian and Slavic — Medium** | ![Paleta Russian and Slavic Medium](lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Central Asian / Silk Road** | ![Paleta Central Asian Silk Road](lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **East Asian** | ![Paleta East Asian](lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC — Corte Penal Internacional** | ![Paleta ICC](lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS — Tribunal Supremo de EE. UU.** | ![Paleta SCOTUS](lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Greek (Hellenic)** | ![Paleta griega helénica](lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turkish (Al Bayrak)** | ![Paleta turca Al Bayrak](lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Paleta Kiro](lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![Paleta SLeeLa](lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

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

Consulte la [guía de integración de editores](linux-editors/README.md) para obtener las descripciones completas de las paletas y los detalles de la utilidad.

## Secciones pendientes de traducción

Todavía deben traducirse las siguientes secciones, que se incorporarán en próximas tandas: **II. Novedades; VIII. Temas del editor SLeeLa; IX. Contribuir; X–XVII. Otras áreas del proyecto; XVIII. Incorporaciones recientes; XIX. Ediciones internacionales del README.** Hasta completar la traducción, el [README.md canónico en inglés](README.md) sigue siendo la referencia completa.

---
**Estado de traducción:** traducción parcial: se han traducido las secciones I–VII. Siguen pendientes las secciones VIII–XIX. Los identificadores técnicos, comandos, ejemplos de código, nombres de archivo y enlaces se han mantenido sin cambios.
