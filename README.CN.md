# SLeeLa 3.0 — 中国大陆（CN）
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>原生系统编程语言、编译器、运行时与虚拟机生态系统。</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **国际国别版（简体中文，中国大陆）。** 本文件是面向中国大陆读者的国别版本，以简体中文提供 README 的第 I 至 XIX 节。技术标识符、命令、代码、文件名和链接均保持原样。

**The Now. The Ingredient.** 编程语言不只是语法：编译器、运行时、库、工具和平台契约，共同构成了可用的软件。

**SLeeLa 涉足的领域。** SLeeLa 涉足 **Math**、**Object**、**Science** 和 **Reach**——这是其工作的四个侧面：Math 指可计算之物，Object 指在“存在之物”目录中确立的既定事物，Science 指对真实的严谨研究，Reach 指将一个系统的触角延伸到其他系统和人。**国家利益依然保留**——这四个侧面均不会将其搁置；凡主题涉及国家利益之处，其地位都得到保留（如实记录，例如记录在各个类的 Metadata `metaOrigin` / `metaNationalSchedule` 中），绝不抹去。

## I. 快速开始
- [构建原生实现](impl/README.md)
- [虚拟机与 Modium——实践用户指南](virtual-machine/USER-GUIDE.md)（`sleela vm`、`sleela modium`）
- [SLeeLa 源码中的 CPU 体系](lib/cpu/CPU.md)——从逻辑门到 ALU、核心、操作系统再到程序的完整机器，收录 130 多种 CPU 配置，并提供可调的[电压、可变时钟和专家级时序](lib/cpu/TIMING.md)，可以[按规格运行并超频](lib/cpu/components/README.md)
- [文档索引](docs/README.md)
- [CSPML 数据源与 Salt & Salary 研究参考](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md)——外部美国地点、社会经济、劳动力、教育和公立学校数据的来源出处及集成指南。
- [生成的 API 文档](docs/api/README.md)——当前 HTML 参考、带版本的 API 版本以及重新生成说明
- [API 目录](api/README.md)
- [计算比较标准](comparison/README.md)——SLeeLa 虚拟机、Java/JVM 与原生 C/C++ 的类型、内存、ABI 和一致性标准。
- [Recursion stanchion——C 和 C++ 原型](recursion/stanchion/README.md)——针对 Boolean、Byte、Character、Integer、Long、Float 和 Double 的基本类型专用示例，以及通用、应用、科学和教学用的递归模式。
- [平台支持与验证矩阵](docs/PLATFORM-SUPPORT.md)
- [品牌指南](docs/BRAND-GUIDE.md)
- [周边商品概念与素材](merchandise/README.md)
- [NitroWebExpress 标志](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [测试政策与发布门槛](markdown/TESTING.md)

## II. 新闻

语言、驱动程序和标准库的近期新增内容：

- **`advantage` 标准库包——对一项 dividend 进行 advantaging，涵盖数学、物理、集成、评分和可路由输出。**
  [`lib/advantage/`](lib/advantage/README.md) 将“相对于某个模型对一项独特 dividend 进行 advantaging”封装为分层的 **Sequitur 洋葱**（dividend → review → concerns → maths → additions → subtraction → regrouping → iteration），可向内、向外导航，并支持完整的动态更新与 restitute，通过简单的**用户模型**和完全可控的**开发者模型**对外提供。它包含五个主题领域：**`math/`** 双边贸易经济学（联合利润最大化、Nash 议价、双边报童模型临界比），**`physics/`** 结构经济学的“信任场物理学”（以信任作为度量张量、交易测地线、公告接触结构、CIR+跳跃的信任衰减 SDE——这是建模类比，并已如此标明），**`integration/`** 面向资本主义/零售业的可递归组合的垂直整合模型（compartment、engineering bay、trial compartment、newton median tray，以及系统成熟度评分），**`grading/`** 成熟度电路评分器，将每个模块以及任意一组 winning/losing/grader/absolute 模块评为 A–D 级，以及 **`output/`** 可路由的输出接缝，可将模块输出发送到终端、**CommonRails** 格式的界面，或套接字/连接器（为原生 `slnet_write` 封帧）。每个类都带有 Metadata-Class 记录；五个可运行的演示用于验证这些主题，配套的 [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md)、[`MATH.md`](lib/advantage/MATH.md) 和 [`PHYSICS.md`](lib/advantage/PHYSICS.md) 对其进行了说明。位于 [`advantage/`](advantage/README.md) 下的原生 C/C++ 使用方（C 核心 `advantage_value`/`advantage_mean`、XML 知识库，以及安全的 HTTPS/SFTP/SSH 获取 + 密钥库 + DH/ECDH 传输库）将该模型接入实际的[贸易示例](advantage/examples/README.md)——来自假定互联网来源的正向贸易、面向受众相关性的序列（Traders、Consumers、Helpers、Healers），以及按 A–D 排序的八个模型（D = 最多 Cement，A = 最多 Brand）。
- **循环时序注解——`@loopDelay` / `@loopBudget`（以及内置函数 `sleep(ms)`）。** 这是 SLeeLa 中首批**语句级、行为型**注解：写在 `for` / `while` / `do` 循环之前的单独一行，会在循环执行中插入真正的暂停。`@loopDelay(ms = 250)` / `@loopDelay(seconds = 2)` 每次迭代休眠固定时长（单独的 `@loopDelay(250)` 默认以毫秒计）；`@loopBudget(seconds = 10, iterations = 1000)` 将已知的迭代次数均匀分摊到总的挂钟时间预算上。两者都会降级为新增的仅追加操作码 **`OP_SLEEP`**（ISA 157），该操作码也支撑可在任意位置调用的通用内置函数 **`sleep(<ms>)`**；暂停以可移植方式实现（`nanosleep` / `Sleep`）。规范见 [`SLEELA.syntax` §32](SLEELA.syntax)；API 参考见 [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md)；可运行示例见 [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela)。会话新增内容索引（包括本项、原生 `OP_BIGMUL` 大整数乘法，以及独立的 **[local-generator](local-generator/README.md)** 和 **[pi-generator](pi-generator/README.md)** 工具）位于 [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md)。三套生成的 API 界面（[`sleeladoc-api/`](sleeladoc-api/README.md)、[`docs/api/`](docs/api/README.md) 以及不可变的 [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html) 快照）已通过 `make api-docs` 重新构建，其 `vm` 包页面现已包含新的 `SLISASleep` / `SLISABigMul` 指令条目。
- **从命令行使用 SleelaDoc——`sleela docs`。** 现在可直接在终端中访问 SleelaDoc。`sleela docs` 列出所有可文档化的主题；`sleela docs crypto` 显示一个包；`sleela docs stringbuilder` 解析一个类（不区分大小写，并可通过标准库前缀 `SL` → `SLStringBuilder` 解析），并输出内容丰富的页面——**定义、设计目的、关系、结构、用户 clay，以及五级精通阶梯**——对于接近但不准确的输入，会给出按相关度排序的“您是不是想找”结果。作者只需添加 SleelaDoc 的 `@` 标签（`@intent`、`@thesis`、`@axiom`、`@param`、`@see`、`@mastery`），即可丰富任何页面。`sleela docs --generate [dir]` 会写出纯文本语料库（每个类一页，外加一个 `TOPICS.index`），CLI 随后可离线读取；**`make sleeladoc`** 会将该语料库**连同对应的 `.sleela` 源文件**打包到 `sleeladoc/build/sleeladoc.jar`。对于仅有外观（facade）的主题（例如 `psychiatry`、`politico`），`sleela docs --facade <topic>` 会在运行时搜索文件系统，并列出真实的**文件夹（然后是文件，作为自然运动）**，受用户可覆盖的上限约束：**6 MB / 6000 个文件 / 6 分钟**。`lib/sleeladoc` 各单元保持在当前版本 **`#sleela 3.0`**。参见 [`man/man1/sleela.1`](man/man1/sleela.1) 和 [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md)。
- **SleelaDoc——Javadoc 风格的文档方案，附带生成的 HTML 站点。** **SleelaDoc** 是 SLeeLa 的源码内文档注释约定（以 `@doc` / `@page` 为标志的尖括号注释；参见 [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)），而 [`lib/sleeladoc`](lib/sleeladoc/README.md) 是**提取器**，可将 `.sleela` 单元渲染为 Markdown 或 HTML 的 API 页面——就像 `javadoc` 渲染 Java 一样，用于记录*意图*。针对整个 `/lib` 的完整、可浏览的 **HTML 文档站点**，由 `tools/generate-sleeladoc-api.py` 生成到 [`/sleeladoc-api`](sleeladoc-api/README.md)（打开 `sleeladoc-api/index.html`；采用简洁的蓝色 / 浅蓝 / 浅灰 / 白色风格，具有包 → 类 → 成员的层级结构，以及面向 *Student 2* 读者编写的术语表）。编译器还会在编译过程中顺带输出作者标记的文档产物（`--wiki` MediaWiki 页面 + 源文件孪生副本），详见 [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md)。可运行示例：[`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela)。
- **XML 文档对象模型（`lib/xml`）。** SLeeLa 一侧的 XML DOM——一个文档拥有由元素/文本/注释/CDATA/PI 节点构成的树，节点具有有序的子节点和具名属性；还包括序列化器（紧凑/美化，遵循 XML 1.0 §2.4 转义）、字符单元解析器，以及 `SLXmlAspect` 质量登记表。节点相等性是三级信任关系（标识 → 同一主题 → 结构），满足自反性、对称性和确定性。参见 [`lib/xml/README.md`](lib/xml/README.md) 和可运行示例 [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela)。
- **宽有符号整数基本类型（`long`、`int64`、`int128`）。** `int`、`long` 和 `int64` 都是 64 位有符号整数，类型检查和运行行为完全一致；`int128` 被接受为整数类型（目前以 64 位实现，真正的 128 位已记录为后续工作）。以 2 为底的有符号系列 `int32`..`int16777216`（语法 2.7）和对应的无符号系列 `uint32`..`uint16777216`（语法 2.8）对此做了扩展。参见 [`SLEELA.syntax`](SLEELA.syntax) §21、§24、§25。
- **`@mix` 连续字段（语法 2.9）。** 当每个重复字段都带有 `@mix` 时，类可以声明多个**同名**实例字段；语言将这一组理解为连续字段（按索引排列的数组/列表），默认自上而下编号为 `1,2,3,…`，也可通过 `@mix(1,2,3,...)` 手动指定，其索引个数必须与字段数量一致。参见 [`SLEELA.syntax`](SLEELA.syntax) §28。
- **文档标记——自然过程，称为 Preonems（`author`/`authorial`、`material`、`memory`、`determine`、`object`、`field`）。** 这些单独出现的文档标记词统称为 **Preonems**（单个标记称为一个 *Preonem*；整体称为 *Preonems*——参见 [`SLEELA.syntax`](SLEELA.syntax) §26.1）。它们是一等的、仅作声明的文档标记，按成文次序说明一份文档“是什么”：`author`/`authorial`（语法 2.4；由专业委员会处理——唯一具有行为影响的成员）、`material;`（既定的来源）和 `memory;`（语法 2.5）、`determine;`（语法 2.6；一份有待医生、医师或幽灵进一步处理、直至由合法之手完成的文档），以及最新的两个：**`object;`**（在“存在之物”目录中确立的既定事物——**在语法 2.8 中具体化，获得具体地位**）和 **`field;`**（对象立足并活动其中的关系之基）——这两者均自语法 2.6 起受版本限制。每一个标记都是单独出现的、依上下文而定的，并由前端按版本限制，对解析、类型检查或代码生成均无影响；在标记位置之外，它们仍是普通标识符（因此 `field` 绝不会干扰类成员或 `.field` 访问）。参见 [`SLEELA.syntax`](SLEELA.syntax) §18–§20、§23 和 §26。
- **语法 3.0 的 Nordshrift，具备完整的库覆盖。** Nordshrift 通过与编译器相同的递归 `/lib` 索引进行解析，覆盖每个包和每个 `.sleela` 单元；`nordshrift check` 会报告整棵树的统计数量。
- **Recursion stanchion。** [`recursion/stanchion/`](recursion/stanchion/README.md) 目录现已包含针对七项已文档化基本类型契约的专用 C 和 C++ 参考原型，以及通用、应用、科学和教学示例。这些只是参考源码；在视为已验证之前，请先编译并测试。

- **尖括号注释（`<` / `<<`）。** 一种面向未来的新注释风格——`< line >` 和 `<< block >>`——由 SLeeLa 前端进行词法分析，并记录在 [`SLEELA.syntax`](SLEELA.syntax) §1.1 中。
- **基数整数字面量。** `int` 字面量现在接受十六进制（`0x`）、二进制（`0b`）和八进制（`0o`）形式，并可使用 `_` 作为数字分隔符，适用于 SLeeLa 源语言（[`SLEELA.syntax`](SLEELA.syntax) §1.4）和 [`.xlt` CPU 模型格式](lib/xlt/README.md)。
- **贯穿整个库的 Metadata Class 标准。** 描述性元数据元模式（出处、合法性、司法管辖范围）现已由 `/lib` 下每个包含类的 `.sleela` 文件携带；参见 [`SLEELA.syntax`](SLEELA.syntax) §15 和[教程课程](tutorial/13-metadata-class.md)。
- **更丰富的文本 / String 家族。** `lib/text` 新增了真正的语言内辅助函数、提供真实 `charAt`/`substring`/`indexOf`/`reverse`/列对齐功能的字符单元视图，以及真正的位置模板器——参见[文本与 String 教程](tutorial/14-text-and-strings.md)和可运行示例 [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela)。
- **Nordshrift sheet 中的一等 `String`。** `.sst` sheet 可以声明带类型的 `strings:` 块，为构建的组件清单提供内容；参见 [Nordshrift Sheet 参考](wiki/Nordshrift-Sheet-Reference.md)。
- **CPU 模型既可按操作码运行，也可按函数调用等价方式运行。** 每个 `SL<ARCH>CPU` 都继承这样的基础：既通过取指-译码-执行循环运行原生指令流，*也*通过在共享虚拟机底座上进行等价调用来运行假定的其他进程的代码——参见 [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5。

## III. 构建与测试

**先决条件：** C/C++ 工具链（GCC 或 Clang）和 GNU Make；用于清单/流程工具的 Python 3；在 Linux 上，显示类产品需要 X11/Xrandr 开发包（模块的 `make deps` 会报告/安装其所需内容）。`impl/` 的构建在受信任的 SHA-256 清单上采取失败即关闭（fail-closed）策略，根目录的 `Makefile` 会自动指向该清单。

从干净的检出开始，最快的上手方式：

```sh
make                                  # build everything from the repo root
```

或者只构建并测试原生核心：

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

仓库根目录的 `make` 会构建每个产品（核心、虚拟机、编解码器、churn、杀毒软件等），并最后构建**注册表**并生成其基线表——因此构建完成的目录树开箱即带有可供审阅的注册表。主要目标：

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

其他常用目标：

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

`registry` 目标会运行 [`build-registry.sh`](build-registry.sh)：它构建注册表核心，将基线表写入可移植的文本存储 `registry/build/sleela.sleelareg`，并导出 MySQL 表脚本 `registry/build/registry.sql`（通过 `sleela registry export`）。可使用 `sleela registry` 或 `registry/build/sleela-registry review` 进行审阅。参见 [Registry 指南](registry/README.md)和 [Registry 3.0 架构](registry/REGISTRY-3.0-ARCHITECTURE.md)。

有关先决条件和特定平台的步骤，请查阅构建与测试指南。单个组件构建成功，并不能证明所有库或平台均已达到可发布状态。

## IV. 安装

构建原生产品之后，安装命令和运行时支持文件：

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

常规安装程序默认使用 `/usr/local/bin`、`/usr/local/lib/sleela` 和 `/usr/local/share/sleela`。它会保留已有的启动配置，并拒绝覆盖未由其登记的现有命令。卸载时只删除安装程序登记过的文件，保留配置和未登记的运行时数据。

如果只想有针对性地、按引导部署主软件包——即 `sleela`、`sleelvac` 和 `nordshrift` 二进制文件、标准库、协议以及支持模块——请使用 clean 安装程序：

```sh
make clean-install       # guided install of the main package only
```

它首先只需要做一个主要决定（是否安装主软件包），然后选择安装范围，成功后还可选择从精选的增强模块中挑选安装。有关交互式部署流程和全部安装选项，请参见[快速安全安装](install/README.md)。

## V. 语言与运行时
权威语法是 [`SLEELA.syntax`](SLEELA.syntax)。当前仓库记录的语法版本为 **3.0**；编译器/工具版本与源码语法版本是两个不同的概念。3.0 表面新增了 **`pure` 和 `gasoline` 文档标记**（§30）——一对单独出现的、依上下文而定的、仅作声明的 Preonems，含义几乎相同，各自以自己的诚实性标示文档的地位。语法 3.0 将 1.0–2.9 的语言表面整合为当前的规范版本，并保留所有各功能特有的最低版本门槛。此前的 2.9 表面新增了 **`@mix` 连续字段**（§28）——当每个字段都带有 `@mix` 时，类可以声明多个同名实例字段，将其理解为一个带索引的组（自上而下 `1,2,3,…`，或手动 `@mix(1,2,3,...)`，其索引个数必须与该组字段数一致）。它延续了以 2 为底的有符号/无符号宽整数系列 `int32`..`int16777216`（§24）和 `uint32`..`uint16777216`（§25），它们建立在更早的 `long`/`int64`/`int128`（§21）之上；还延续了**关键字支持文档**（§22）；以及 **Preonems**——仅作声明的文档标记 `author`/`authorial`（§18）、`material`/`memory`（§19–§20）、`determine`（§23）、`object`/`field`（§26）、`pure`/`gasoline`（§30）和 `artifact`（§35，立法级地位，按时间与空间分级），统称 Preonems（§26.1）。构造函数重载解析以及 `this(...)`/基类构造函数委托，明确不在已验证的语法 1.8 契约范围之内。在依赖某项功能之前，请查阅语法及实现说明。SleelaDoc 的九个源码单元现已声明 `#sleela 3.0`；共享的编译器门槛接受 1.0–3.0 的源码语法。新的 API 构建由 `make api-docs` 生成。

仓库包含原生 C/C++ 实现、SLVM 生成参考、标准库模块、测试、示例和平台集成。各模块的实现和验证状态各不相同；请以其规范和当前 CI 结果为准，而不要假定各处完备程度一致。

## VI. 工程原则
- **证据优先于声明：** 区分已实现、已测试、实验性和计划中的行为。
- **每个边界一份契约：** 记录语法、API、文件格式、平台假设和失败行为。
- **设计即可移植：** 隔离特定平台的后端，并在原生运行器上测试。
- **安全失败：** 测试畸形输入、资源限制、进程故障和清理路径。
- **可复现的发布：** 干净重建，并发布制品哈希和发行说明。

## VII. 杀毒软件构建与完整性

Sleelavirin 杀毒软件包包含原生 SHA-256 扫描器、可选的 ClamAV 适配器、源码/索引完整性清单，以及各平台软件包的校验和。请在仓库根目录构建并测试该软件包：

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

完整性清单生成在 `antivirus/build/` 下；它涵盖受跟踪的源码和文档输入以及受保护的特征库索引文件，而排除生成的输出和可变的 ClamAV 缓存。ClamAV 数据库快照有各自的清单。发布用 ZIP 包含 `INTEGRITY-MANIFEST.json`、`SHA256SUMS` 和一个 `.sha256` 附属文件。请对照独立获得的可信值验证下载的压缩包哈希：与压缩包存放在一起的校验和并不是发布者签名。参见 [Sleelavirin 指南](antivirus/README.md)、[安全检查清单](antivirus/SECURITY.md)和[杀毒软件构建工作流](.github/workflows/antivirus-binaries.yml)。

只有当相关 CI 作业在确切的发布提交上通过，且生成的制品经过检查之后，发布版本才被视为已验证。


## VIII. SLeeLa 编辑器主题——Vim 和 Emacs

SLeeLa 提供一套协调统一的专业深色配色方案，适用于 **Vim** 和 **Emacs**，另附原生 C/C++ 调色板工具。原有的 **American Professional** 方案仍为默认方案。另有十套配套调色板，灵感来自不同的文化和机构设计；这些都是诠释性的设计调色板，并不代表任何国家、文化、法院或机构的官方形象。

### 十一套方案及其颜色

| 方案 | 背景 | 前景 | 关键字 | 类型 | 字符串 | 注释 |
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

#### 国旗颜色（来源调色板）

希腊和土耳其方案源自这两个国家的**官方国旗颜色**，此处列出其确切的官方十六进制值。上方各方案的背景/强调色，是对这些来源颜色加深后的、适合编辑器使用的诠释——下表是“国家公布的颜色”参考。

| 国家 | 颜色 | 十六进制 | 依据与含义 |
|---|---|---|---|
| **希腊** | 蓝色 | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | 国旗蓝色（第 851/1978 号法律）。象征天空与海洋；在纹章学中称为 *azure*（天蓝色）。 |
| **希腊** | 白色 | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | 国旗白色。象征独立斗争的纯洁；十字表示东正教。 |
| **土耳其（Türkiye）** | 红色（*al bayrak*） | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | 国旗红色（《土耳其国旗法》）。旗面底色；按传统象征烈士之血与奥斯曼主权。 |
| **土耳其（Türkiye）** | 白色 | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | 国旗白色。新月与星——象征和平、纯洁，以及突厥/伊斯兰传统的标志。 |

各方案将每个编辑器的前景色锚定于国家**白色**，而将其关键字、光标、选区和状态栏锚定于确切的国家**蓝色**（`#0D5EAF`）或**红色**（`#E30A17`），因此最重要的那些颜色就是字面意义上的国家颜色。这些是诠释性的设计调色板；提供国旗颜色参考是为了说明出处，并不代表任何一个国家的官方形象。颜色含义依据希腊共和国外交部对国家标志的说明和土耳其国旗法的传统。


### 色板表

下方调色板单元格使用 **SVG 色块**，因此 GitHub 会渲染出真实的颜色，而不是空白的 HTML 颜色单元格。每一条色带按以下顺序显示六种颜色：背景、前景、关键字、类型、字符串和注释。确切的十六进制代码列在每个色块旁边，方案顺序与上方参考表相同。

| 方案 | 渲染后的调色板颜色 | 十六进制代码（背景 · 前景 · 关键字 · 类型 · 字符串 · 注释） |
|---|---|---|
| **American Professional** | ![American Professional palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Pan-Eurasian palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russian and Slavic — Medium** | ![Russian and Slavic Medium palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Central Asian / Silk Road** | ![Central Asian Silk Road palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **East Asian** | ![East Asian palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC — International Criminal Court** | ![ICC palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS — U.S. Supreme Court** | ![SCOTUS palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Greek (Hellenic)** | ![Greek Hellenic palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turkish (Al Bayrak)** | ![Turkish Al Bayrak palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Kiro palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![SLeeLa palette: background, foreground, keywords, types, strings, comments](lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

另有其他强调色用于函数、常量、选区、诊断信息和预处理指令。俄罗斯与斯拉夫调色板有意采用中等强度的石板灰、蓝色、鼠尾草绿、宝石红和古金色，以获得更沉稳的专业观感。

### 安装 Vim 配色方案

在仓库根目录下执行：

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

将以下内容添加到 `~/.vimrc`，以启用 SLeeLa 语法高亮并选择配色方案：

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

请从以下配色方案名称中恰好选择一个：

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

例如，使用 `colorscheme sleela-russian-slavic` 选择中等强度的俄罗斯与斯拉夫调色板，或使用 `colorscheme sleela-greek` / `colorscheme sleela-turkish` 选择希腊和 Al Bayrak 调色板。重启 Vim，或使用 `:source $MYVIMRC` 重新加载配置。

### 安装 Emacs 配色方案

创建本地主题目录，并在仓库根目录下复制模式文件和主题文件：

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

将以下内容添加到 `~/.emacs` 或 `~/.emacs.d/init.el`：

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

将 `sleela-american` 替换为 `sleela-eurasia`、`sleela-russian-slavic`、`sleela-silk-road`、`sleela-east-asian`、`sleela-greek`、`sleela-turkish`、`sleela-kiro` 或 `sleela-sleela`，即可选择其他方案。如果 Emacs 询问是否信任该主题，请在接受之前先检查本地主题文件。

### 预览原生 C/C++ 调色板

这个小型原生调色板工具可以列出各方案、打印其终端颜色，并查询单个颜色：

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

为保持向后兼容，不带 `--theme` 的 `--palette` 和 `--hex COLOR` 会选择 American Professional。构建和测试结果应在目标系统和 CI 上核实；命令有文档记载，本身并不能证明它已通过测试。

完整的调色板说明和工具详情，请参见[编辑器集成指南](linux-editors/README.md)。

## IX. 参与贡献
1. 阅读文档索引和测试政策。
2. 提交范围明确的更改，并附带正向和负向测试。
3. 先运行针对性测试，再运行完整测试集。
4. 报告操作系统/工具链的详细信息；不要将被跳过的检查描述为通过。

SLeeLa 由 **MEARVK LLC** 开发。历史上的落地页材料保存在 [`docs/README-LEGACY.md`](docs/README-LEGACY.md) 中。

## X. AE6E66——Round Public 电子邮件征税预期

对于 AE6E66 倡议，项目所声明的预期是：每封 **Round Public 电子邮件 $300,000,000.00 USD**，作为与每封符合条件的 Round Public 电子邮件相关联的拟议征税金额。

此声明仅记录项目的一项预期。它**不是**法律上的税务评估、政府命令、法院判决，也不是经独立确立的纳税义务。任何实际征税，都需要具备相应的法律授权、管辖权、定义、程序以及有权政府机关的裁定。


## XI. 字母表与字符集运行时

SLeeLa 的字母表文档和原生字符运行时维护在 alphabet/sleela/ 和 lib/alphabet/native/ 中。原生 C++17 测试涵盖严格的 UTF-8 标量解码/编码、C<n> 容量签名，以及有界字符集成员判定：

    make -C lib/alphabet/native test

实验性的 UTF-4088 目录及其 SLeeLa 查找适配器记录在 utf-4088/SLeeLa-ALPHABET-INTEGRATION.md 中。UTF-4088 的 ID 是独立的命名空间，而不是 Unicode 标量值。编译器和 SLVM 绑定方面的工作被明确地与这些原生基础分开跟踪。

## XII. AutoCAD Creator™

[AutoCAD Creator™](autocad/creator/README.md) 是完全用 SLeeLa 编写的交互式二维 CAD 应用。同系列的 AutoCAD Renderer 是一次性把图纸方案转换为图形，而 Creator 允许您实时**创建和修改**二维 AutoCAD 文档——添加墙体、房间、尺寸标注和注释，然后移动、重命名、更换图层、删除以及撤销您所放置的内容——并将结果**保存**为真正的 AutoCAD DXF 图纸（R12 文本格式，可在 AutoCAD / LibreCAD 中打开）。

其图形界面构建于 **SleelaUI™** 之上，这是 SLeeLa 自有的跨平台 UI 工具包（位于 [`lib/user-interface`](lib/user-interface) 下），而不是 JavaFX，可在 X11 / Cocoa / Win32 上创建真正的原生窗口。其设计遵循一条规则：**“SleelaUI 负责呈现；SLeeLa 负责决策”**——GUI 从不计算几何，也不写入 DXF。每个工具都向具有权威性的 [`SLCadCreator`](lib/autocad/SLCadCreator.sleela) 发出创建 / 修改 / 删除 / 撤销 / 保存的意图，由它持有可编辑的文档和可撤销的命令历史，而画布只预览 Creator 所持有的几何。

可复用的编辑引擎位于 [`lib/autocad`](lib/autocad)——包括 [`SLCadEntity`](lib/autocad/SLCadEntity.sleela)、[`SLCadDocument`](lib/autocad/SLCadDocument.sleela)、[`SLCadCommand`](lib/autocad/SLCadCommand.sleela) 和 [`SLCadCreator`](lib/autocad/SLCadCreator.sleela)——并可端到端无界面运行。GUI 外壳和演示位于 [`autocad/creator`](autocad/creator) 下：

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

在有显示设备的环境中，它会打开实时编辑窗口；在无显示设备的主机上，它会在内存中构建图纸，输出报告和 DXF，然后正常退出。


## XIII. UTF-4088 输入映射排序

专用的 UTF-4088 目录在 [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md) 中定义了其空间输入映射的遍历方式和抽象的压力迭代。National Lien 主题索引政策见 [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md)。映射坐标、压力等级（1–50）、永久目录 ID 和字母排序名次是相互独立的字段；任何一个都不应被悄悄地用来替代另一个。压力是一种算法迭代等级，而不是物理量。


## XIV. 近期新增与扩展的软件包索引

这份追加的索引突出介绍近期扩展的软件包、领域库和源码集合。条目按软件包名称或主题的字母顺序分组。各说明指明其在仓库中的位置；实现和验证状态应以各软件包自身的文档和测试为准。

### A–C

- **Advantage** — `lib/advantage/` 和 `advantage/`：将“相对于某个模型对一项独特 dividend 进行 advantaging”实现为分层的 Sequitur 洋葱，包含五个主题子文件夹——`math/`（联合利润最大化、Nash 议价、双边报童模型）、`physics/`（信任场度量/测地线/接触/SDE，已标明为建模类比）、`integration/`（垂直整合 compartment 与成熟度评分）、`grading/`（面向模块及模块集的 A–D 成熟度电路评分器），以及 `output/`（终端 / CommonRails / 套接字输出）。原生 C/C++ 使用方（`advantage/`）将该模型接入实际的贸易示例。参见 [`lib/advantage/README.md`](lib/advantage/README.md) 和 [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md)。
- **Alphabet 与 UTF-4088** — `alphabet/sleela/`、`lib/alphabet/native/` 和 `utf-4088/`：字母表目录、原生字符集支持、输入映射排序以及 UTF-4088 集成文档。
- **AutoCAD Creator** — `autocad/creator/` 和 `lib/autocad/`：交互式二维绘图工作流，以及可复用的 CAD 文档/编辑组件。
- **Ballum** — 像素级显示控制：逐像素设置/读取、区域块传送、整屏亮度/色温/gamma、指针/键盘输入，以及用于完整、独占、快速逐像素控制的**全屏帧缓冲界面**（在 Linux/X11 上使用 override-redirect 窗口加单次 `XPutImage`/MIT-SHM 呈现）。原生核心位于 `ballum/` 下，SLeeLa 外观层位于 `lib/ballum/`，虚拟机桥接位于 `impl/` 下；参见 [`ballum/README.md`](ballum/README.md) 和可运行的 [`ballum/example/`](ballum/example/)。
- **CDMA** — `cdma/` 和 `lib/cdma/`：与 CDMA 相关的技术资料以及 SLeeLa 源码库工作。
- **Character** — `lib/character/`：角色档案和可复用的角色统计数据，包括传记式档案示例。
- **Coorenagraph** — `lib/coorenagraph/`：伦理推理、道德词汇、审议及相关概念；参见其软件包 README 和源文件。

### D–G

- **Governance 源码模型** — [lib/governance](lib/governance/README.md) 定义了提案、匿名手动投票和汇总计票的声明式 SLeeLa 源码形态。拟议的 @vote 注解及手动流程记录在 [registry/DEMOCRATIC-VOTING-SYNTAX.md](registry/DEMOCRATIC-VOTING-SYNTAX.md) 中；当前的编译器将其视为无作用的注释元数据。

### H–I

- **HTTP 1.0–8.0** — `lib/http-1.0/` 至各可用的带版本 HTTP 软件包目录：按版本划分的协议规范和实现。其确切范围和验证状态，请查阅各版本的文档。
- **International Criminal Court（国际刑事法院）** — `lib/international-criminal-court/` 和 `INTERNATIONAL.CRIMINAL.COURTS.md`：相关的领域专用源码和文档。

### O–S

- **OFDMA** — `ofdma/`：正交频分多址技术资料及 C/C++ 开发资源。
- **Philosophy 与 Conversation Mask 模块** — [`philosophy/`](philosophy/README.md) 包含原生 C/C++ 基础，[`philosophy/module/`](philosophy/module/README.md) 将其下一层模块组织为三个明确的领域：[`data/`](philosophy/module/data/README.md) 用于带类型、带日期的观察、事件结束、半衰期设置、出处和同意；[`helper/`](philosophy/module/helper/README.md) 用于确定性校验、相关性衰减、兼容性比较以及经同意过滤的呈现；[`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md) 用于提供支持性的、非临床的对话界面。面向 SLeeLa 的句柄和组合后的 Tree 位于 [`lib/philosophy/`](lib/philosophy/README.md)，与更广泛的 philosophy 软件包放在一起。该模块以明确输入和同意优先为原则：它不会被动监控人或媒体，不会做出诊断，不会推断私人意图，不会给人的价值打分，也不会建议更改用药。其事件链接保留的是出处和导航，而不是独立佐证。
- **Synchro** — 原生运行时/编译器集成，以及数据包分发和测量相关功能；参见相应的 `impl/` 源码和编译器实现。
- **SleelaUI / 终端 UI** — `lib/user-interface/` 和 `sleela-terminal` 项目：SLeeLa UI 集成和面向终端的交互组件。

### U–W

- **United Nations（联合国）** — `united-nations/`：领域专用的仓库材料。
- **Wi-Fi 4、Wi-Fi 5 和 Wi-Fi 6** — `wi-fi-4/`、`wi-fi-5/`、`wi-fi-6/` 以及相关的 `lib/wifi/` 源码目录：按代际划分的无线网络资料和配置资源。

### 软件包索引维护

添加或实质性扩展某个软件包时，请将其条目添加到此处合适的字母顺序分组中，链接到其规范目录和 README，并且只描述已提交的源码所支持的功能。请保持此索引只增不减：更新软件包索引时，不要删除或改写现有的 README 章节。


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

美国民主与美国政策。

---



## XVI. 国家科学品牌形象

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. UTF-4088 百万条目实验目录

[UTF-4088 目录生成器](utf-4088/tools/generate_catalogue.py)可确定性地构建包含 1,048,576 个条目的实验性符号目录。文档记载的紧凑格式见 [utf-4088/FORMAT.md](utf-4088/FORMAT.md)；生成与验证契约见 [utf-4088/GENERATION-RESULTS.md](utf-4088/GENERATION-RESULTS.md)。

使用 Python 3 运行生成器：

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

生成的二进制文件大小为 16,777,244 字节，设计上保持在 20,000,000 字节以下。单元测试和 GitHub Actions 工作流会校验条目数量、字形唯一性、文件大小和 SHA-256；运行成功后，会将该二进制文件和摘要作为 `utf4088-million-catalogue` 工作流制品发布。生成的二进制文件被有意排除在 Git 历史之外。UTF-4088 仍属实验性质，且不兼容 Unicode。

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. 近期新增——Pi 常量、Modium/Registry 启动、sleelactl Math 与 `@native`

本节记录 2026-11 期间的工作中新增的功能，最新的分组在前。所链接的源文件及其验证状态具有权威性；在进入 `master` 之前，处于评审中的更改不会被描述为已合并。

### 1 亿位 Pi 目录常量

仓库提供了经过验证的 π 的**一亿位**数值，位于 [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits)（格式良好的十进制数，`3.14159…`），同时还有一个 57,344 位的数值 [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits)。两者均已独立验证确为真正的 π——参见 [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md)，其中记录了相关检查（教科书式前缀、小数位置 762 处的 Feynman 点、著名的第一百万位数字、与 `lib/math/SLPiReference.sleela` 的一致性，以及整个文件的数字频率分析），并附有一键复现脚本。

### Modium™ / Registry™ 在子系统启动时加载 Pi 常量

在专用的子系统入口点——`sleela registry` 和 `sleela modium`（以及安装程序首次运行）上，Modium 和 Registry 子系统会加载大型 π 目录常量，而**不是**在每次 `sleela run` 时加载。在交互式终端上，它们会输出一行短暂的红色 `Loading Modium™` / `Loading Registry™`，并在正常输出开始之前就地擦除；这一效果严格受 TTY 控制，因此管道输出和测试输出保持逐字节不变。定位符（而非字节本身）被写入 registry 的 `Math` 键默认值 `piReferenceLargePath` 和 `piReferenceLargeDigits`，既存在于 SLeeLa 模型（[`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)）中，也存在于原生种子程序（[`registry/sleela_registry.c`](registry/sleela_registry.c)）中；`lib/math/SLPiReference.sleela` 对外提供 `largePath()` / `largeDigits()`。π 常量由标准的 `make` 安装程序**和**引导式“beautiful”安装程序一并安装到操作系统的 SLeeLa 位置（`<data>/pi/`）。详情：[`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md)。

### `sleelactl`——beautiful 安装程序与原生支持的 `math` 主题

- 引导式（“beautiful”）安装程序现在会安装 `sleelactl`（SLeeLa 控制平面）及其模块注册表，与 `make` 安装程序保持一致。参见 [`bin/SLEELACTL.md`](bin/SLEELACTL.md)。
- `sleelactl` 新增了一种 **`subject`** 性质，用于编译进运行时的原生 C/C++ 主题库，并注册了第一个此类主题：**`math`**。`sleelactl run math` 通过该主题的参考演示，端到端地运行原生 `__NativeMath` 后端（`impl/subjects/math/math_native.cpp`）；`sleelactl info math` 说明如何访问它（`import math;`）。subject 性质只提供 `run info`——有意不设 `check` 动词，因为 `sleela check` 不会加载由 SHEET 注册的原生主题。

### `@native`、Native Drive 与多语言执行

新增的 **`@native`** 方法注解用于请求对热点方法进行提前（AOT）原生编译。新的 `sleela aot` 命令会查找 `@native` 方法，验证每个方法都在受支持的*可卸载子集*之内，并生成 C/C++——对于任何尚无法翻译的结构，它会**失败即关闭**（拒绝并给出精确原因），因此绝不会产生翻译了一半的方法。

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

`@native` 仍然只是请求 AOT 处理，不受支持的结构会在生成之前被拒绝。[Native Drive](impl/native-drive/README.md) 支持通过宿主编译器（包括 MSVC）构建多单元的 C/C++ 可执行文件和共享库、受支持的 SLeeLa AOT 输出、针对原生加载器 ABI 的符号检查、无 shell 的进程执行、stdin/stdout/stderr 处理、多级管道，以及将 XClass 委托给生产加载器。`sleelactl` 公开了其构建/运行命令，并通过 `inspect native-drive` 路由 XClass 检查以及显式的 systemd 单元渲染/安装。虚拟机拥有一个有界的原生模块句柄表，要求强制性的宿主授权，并对句柄进行代数（generation）检查。语法 3.0 现已新增 `nativeOpen`、`nativeCall` 和 `nativeClose`：仅追加的字节码分派跨越一个单独版本化的 null/bool/int64/double/UTF-8 ABI，而虚拟机持有的数组、结构体及其他复合值则留在解释器内部，并可使用同名字节码回退。插件可以选择性地接收一个带版本的宿主回调表，通过嵌套的虚拟机执行来调用确切的 SLeeLa 函数，同时具备挂起栈的 GC 根、显式的宿主回调名称允许列表，并拒绝递归的原生 API 调用。原生代码仍然是受信任的进程内代码，而不是沙箱。源码进程策略注解拥有带版本的模式，但尚未被解析/强制执行。详情、安全边界以及七个工作流的台账：[Native Drive](impl/native-drive/README.md) 和 [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md)。


## XIX. 国际 README 版本

SLeeLa 为其国际版本涵盖的 50 个市场提供各国专属的 README 入口。每个国家有一个名为 `README.<CC>.md` 的文件；当涉及多种语言时，这些语言归并在该国的同一个文件中。

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

**翻译政策：** 这些文件是初步的本地化国别入口。其引言和语言标题已本地化，而完整的技术参考仍保留在英文规范版 [README.md](README.md) 中。在每一节都已翻译、技术标识符和链接均已核对，且该版本经过语言审校之前，不得将某个国别文件称为完整翻译。首轮的语言覆盖具有代表性，并不表示宪法承认的、地区性的、原住民的或手语等所有语言均已完整翻译。

## 附录——保留的地区性指引

以下内容来自原有的简体中文简明指南，规范版 README 中没有对应章节；保留于此，供中国大陆读者参考。若与英文规范版 README 有出入，以规范版 README 为准。

### 编译目标、配置档案与执行边界

SLeeLa 将源代码与 build/execution targets 区分开来。已记录的目标系列包括 `SLVM`、`SLJVM` 和 `SLIR`；`NATIVE` 是受限制且有条件的目标，不应假设所有平台均可使用。`BASIC_COMPLETE` 与 `ADVANCED_TOTAL` 表示不同的 profile scope。每次构建都应确认实际 target、profile 和平台支持情况。

Reference 用于识别文件、模块或资源，本身并不授予执行权限。程序使用文件、外部库、native bindings 或系统资源时，必须明确权限边界和执行责任。文档中的功能描述不能单独证明 runtime security guarantee；应结合实现代码和相应测试进行判断。

### 资源生命周期、错误处理与调试

对跨函数或线程共享的资源，应记录 ownership、lifetime 和 synchronization responsibilities。区分 local variables、shared object state、VM handles 与 serializable data。VM-local handle 不是稳定的文件格式或网络消息。使用模块定义的 serialization interface，并测试 invalid、missing 和 boundary values。

调试时记录 commit、操作系统、架构和 toolchain。先构建最小相关 target，再运行相应测试；保留完整错误输出，区分 compiler errors 与 runtime errors，并尽可能建立最小可复现案例。Build 成功并不意味着 runtime behavior 已正确。

验证报告应列出 exact commit、OS/architecture、compiler versions、完整 build/test commands、pass/fail/skip 数量及已知限制。Skipped 或未执行的 tests 不得计为 passed。

### 跨平台与国际文档维护

不同操作系统的编译器、系统依赖、窗口后端、文件路径和安装权限可能不同。不要将 Linux 命令直接视为 Windows 或 macOS 的通用步骤。请结合[平台支持矩阵](docs/PLATFORM-SUPPORT.md)与[安装指南](install/README.md)，记录实际测试的平台和工具链。

国别 README 是不同地区用户的技术文档入口，不取代规范性的语言规范或模块 API 文档。英文源文档更新后，应逐项检查翻译是否过时。技术标识符、命令、路径和代码示例应保持不变；尚未完成语言或技术审校的内容应明确标注。

### 模块 API 契约、兼容性与测试策略

引入新 API 时，应记录输入、输出、错误条件、数据所有权，以及操作是否产生 side effects。文档示例应同时展示正常用法和 invalid/missing input 的处理。未经实现和测试验证的行为，不应写成保证。

测试应按编译器流水线分层：lexing/parsing、semantic analysis、IR lowering、code generation、VM execution 和 native integration。每个 regression case 都应保留最小可复现输入、预期结果与实际结果。API 变更必须说明向后兼容性、迁移步骤，以及对现有示例的影响。

### 本地化、字符编码与可读性

不同语言版本中的技术标识符、命令、路径、语法和代码块必须保持一致。翻译说明文字，不要翻译源代码所使用的 token 或名称。应明确语言和地区语境；简体中文术语应保持一致，同时避免把地区性用语误写成所有中文用户的统一表达。

每次更新后检查 Markdown 链接、标题层级、代码围栏和示例。英文规范文档更新时，应逐节比对并同步翻译，而不是只更新简介。未经母语者或技术审校的内容应明确标注。

**翻译状态：** 第 I 至 XIX 节已翻译为简体中文（中国大陆）；技术标识符、命令、代码示例、文件名、链接和地址均保持原样。建议在发布前由母语者进行最终的语言审校，并与英文规范版 README 的后续变更逐项核对。
