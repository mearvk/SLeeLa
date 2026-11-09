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

**Traducción pendiente.** Los anuncios completos de cambios y funciones de la sección II inglesa se incorporarán en una próxima tanda. Fuente normativa: [README.md — sección II](README.md).

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

## Secciones pendientes de traducción

Todavía deben traducirse las siguientes secciones, que se incorporarán en próximas tandas: **II. Novedades; VIII. Temas del editor SLeeLa; IX. Contribuir; X–XVII. Otras áreas del proyecto; XVIII. Incorporaciones recientes; XIX. Ediciones internacionales del README.** Hasta completar la traducción, el [README.md canónico en inglés](README.md) sigue siendo la referencia completa.

---
**Estado de traducción:** traducción parcial: se han traducido las secciones I y III–VII. Los identificadores técnicos, comandos, ejemplos de código, nombres de archivo y enlaces se han mantenido sin cambios.
