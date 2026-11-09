# SLeeLa 3.0 — 中国大陆（CN）

## 简体中文版

SLeeLa 是一套原生系统编程语言，配有编译器、运行时和虚拟机生态系统。项目还包括标准库、开发工具以及平台集成契约。本版本面向中国大陆简体中文读者。技术标识符、命令、路径、文件名、代码示例和 API 名称保持原样，以便读者直接使用。

**The Now. The Ingredient.** 编程语言不只是语法；编译器、运行时、库、工具和平台契约共同构成可用的软件系统。

SLeeLa 将工作领域概括为 **Math**、**Object**、**Science** 和 **Reach**。与主题有关的国家利益仍以明确元数据记录，不会因为这四个领域而被忽略。

## 一、快速开始

- [构建原生实现](impl/README.md)。
- [虚拟机与 Modium 用户指南](virtual-machine/USER-GUIDE.md)，包括 `sleela vm` 和 `sleela modium`。
- [SLeeLa 源码中的 CPU 架构](lib/cpu/CPU.md)：从逻辑门、ALU、核心到操作系统和程序；目录包含 130 多种 CPU 配置。另见[时序、电压和频率](lib/cpu/TIMING.md)。
- [文档索引](docs/README.md)。
- [CSPML 数据源与 Salt & Salary 研究参考](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md)。
- [生成的 API 文档](docs/api/README.md)和[API 目录](api/README.md)。
- [计算比较标准](comparison/README.md)：SLeeLa VM、Java/JVM 与原生 C/C++ 的类型、内存、ABI 和一致性标准。
- [Recursion stanchion 的 C/C++ 参考原型](recursion/stanchion/README.md)。
- [平台支持与验证矩阵](docs/PLATFORM-SUPPORT.md)。
- [品牌指南](docs/BRAND-GUIDE.md)。
- [测试政策与发布门槛](markdown/TESTING.md)。

## 二、项目功能与近期进展

**Advantage 标准库包**（`lib/advantage/`）用于相对于模型分析一项 dividend，采用分层结构组织 review、concerns、maths、additions、subtraction、regrouping 和 iteration。其主题包括双边交易经济学、作为建模类比的信任场结构、垂直整合、模块评分和可路由输出。物理部分明确属于建模类比，而不是已经证实的自然定律。参见 [Advantage 文档](lib/advantage/README.md)。

其他已记录功能包括：

- **循环时序注解：** `@loopDelay` 和 `@loopBudget` 用于表达循环暂停与时间预算；实现使用 `OP_SLEEP` 和 `sleep(ms)`。参见 [语法规范 §32](SLEELA.syntax) 和 [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md)。
- **SleelaDoc：** 源码文档约定和工具，可生成 Markdown/HTML 页面、主题索引及 API 文档。命令 `sleela docs` 可列出主题、显示包和类的文档。参见 [SleelaDoc](lib/sleeladoc/README.md)。
- **XML DOM：** XML 文档树，包含元素、文本、注释、CDATA、处理指令、有序子节点和属性，以及解析和序列化功能。参见 [XML DOM](lib/xml/README.md)。
- **扩展整数类型：** 文档包括 `long`、`int64`、`int128` 以及有符号和无符号整数族。实际位宽应以规范为准，不能只根据类型名称推断。
- **`@mix` 连续字段：** 允许同名实例字段组成按索引排列的序列；索引可自动生成，也可按规范手动声明。
- **Preonem 文档标记：** 包括 `author`、`authorial`、`material`、`memory`、`determine`、`object` 和 `field`。含义取决于语法版本及标记所在位置；在标记位置之外，它们仍是普通标识符。
- **Nordshrift：** 使用与编译器相同的递归 `/lib` 索引解析包和源代码单元。
- **尖括号注释：** `< ... >` 和 `<< ... >>` 是文档记录的注释形式。
- **基数整数：** 语法支持十六进制、二进制和八进制整数，以及数字分隔符。

规范文件 [`SLEELA.syntax`](SLEELA.syntax) 是语法的权威来源。描述文档中提及某项功能，并不意味着所有编译器版本都已支持该功能。

## 三、构建与测试

**依赖项：** GCC 或 Clang 等 C/C++ 工具链、GNU Make，以及用于清单和工作流工具的 Python 3。Linux 图形组件可能需要 X11/Xrandr 开发包。具体模块的依赖要求以该模块文档为准。

在仓库根目录执行：

```sh
make
```

只构建原生核心并运行完整测试套件：

```sh
make -C impl all
bash ./test-suites/run-all.sh --all
```

常用 Make 目标：

```sh
make all
make registry
make tests
make ballum
make help
make clean
```

根目录的构建目标负责调度仓库中的产品；`registry` 目标构建注册表核心并生成基线表。参见 [Registry 指南](registry/README.md) 和 [Registry 3.0 架构](registry/REGISTRY-3.0-ARCHITECTURE.md)。

单个组件构建成功并不能证明所有库、平台或配置均已达到发布标准。验证状态必须按模块和平台分别评估。跳过或未执行的检查不得标记为通过。

## 四、安装

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

常规系统安装通常使用 `/usr/local/bin`、`/usr/local/lib/sleela` 和 `/usr/local/share/sleela`。设置 `PREFIX="$HOME/.local"` 通常可以在用户目录安装而无需 `sudo`。`DESTDIR` 可用于准备软件包目录，而不直接写入系统路径。

如需仅安装主软件包，包括 `sleela`、`sleelvac`、`nordshrift`、标准库、协议和支持模块，可运行：

```sh
make clean-install
```

更多信息见[快速安全安装指南](install/README.md)。安装器会保留已有启动配置，并避免覆盖未由其管理的命令；卸载范围应限于安装器登记的文件。

## 五、语言与运行时

仓库文档采用 **SLeeLa 3.0** 语法。编译器版本和源代码声明的语法版本是不同概念。3.0 汇集了 1.0 至 2.9 的语言表面，同时保留每项功能所要求的最低语法版本。

SLeeLa 包含原生实现、编译器、运行时、虚拟机、文档工具和按包组织的库。仓库同时维护 C/C++ 原生组件和 SLeeLa 源码。原生代码与运行时之间的边界必须遵守已记录的 ABI 契约。

## 六、C、C++ 与原生编译

`@native` 注解和 `sleela aot` 命令代表预先编译路径的早期、保守阶段。它们可以列出带注解的方法，判断方法是否属于受支持子集，并为受支持的部分生成 C/C++：

```sh
sleela aot --list <file.sleela>
sleela aot --emit <file.sleela>
```

不支持的结构会被明确拒绝并说明原因，而不是生成容易造成误解的部分翻译。`@native` 本身不会改变程序行为。若要声称共享库构建或 VM 原生分派已完成，必须有实现和测试证据。参见 [AT-NATIVE](pi-generator/AT-NATIVE.md)。

## 七、库与子系统

- **CPU**（`lib/cpu/`）：处理器模型、配置、执行和时序参数。
- **Ballum**（`ballum/`、`lib/ballum/`）：像素级显示控制、区域、亮度、gamma，以及键盘和指针输入；可用性取决于操作系统和后端。参见 [Ballum](ballum/README.md)。
- **AutoCAD Creator™**（`autocad/creator/`）：SLeeLa 编写的交互式二维 CAD 应用，可创建实体、图层、尺寸和注释，撤销操作并保存 DXF。编辑模型负责几何和历史记录，界面负责呈现。参见 [AutoCAD Creator](autocad/creator/README.md)。
- **Alphabet 与 UTF-4088：** 字符目录和实验性工具。UTF-4088 使用自己的标识符空间，**不兼容 Unicode**；不得将其标识符视为 Unicode 标量值。
- **Pi Generator：** π 常数目录、验证和复现工具。
- **Modium 与 Registry：** 注册表子系统和检查工具。
- **Philosophy：** 原生基础、数据、辅助和对话模块。对话模块被描述为非临床、基于明确输入和同意；它不提供诊断。
- **CDMA、OFDMA、HTTP 与 Wi-Fi：** 技术目录和有版本的库。各模块范围与验证状态应以其文档为准。

包目录用于概览，不能替代模块源码、README 和专门测试。

## 八、完整性、安全与验证

项目记录 SHA-256 清单、本地完整性检查，以及通过 Sleelavirin 使用的可选 ClamAV 适配器。存在某项工具或清单，并不等于获得安全认证。

工程原则包括：

1. 组件之间使用明确契约。
2. 从设计阶段考虑可移植性。
3. 操作失败时采取安全行为。
4. 验证证据可复现，并与受测 Git commit 绑定。
5. 区分“识别一个引用”和“授权执行”。
6. 如实报告未执行、跳过或受阻的测试。

评估发布版本时，应检查对应 commit 的 CI 结果、生成产物、相关测试和平台矩阵。没有独立证据时，不应将 Linux 结果推及 Windows 或 macOS。

## 九、编辑器与贡献

SLeeLa 提供 **Vim** 和 **Emacs** 的语法集成与主题，并提供原生 C/C++ 调色板工具。参见[编辑器集成指南](linux-editors/README.md)。

贡献建议：

1. 阅读[文档索引](docs/README.md)和[测试政策](markdown/TESTING.md)。
2. 提交范围清晰的更改，并添加正向和负向测试。
3. 先运行相关测试，再运行适当的更大测试集。
4. 记录操作系统、编译器和工具链。
5. 不得将跳过的检查描述为通过。

SLeeLa 由 **MEARVK LLC** 开发。

## 十、其他项目内容

主 README 还记录 CSPML 数据来源及集成指南、研究参考、商品概念和视觉素材，并列出 Character、Governance、Synchro、SleelaUI、United Nations 等主题。目录描述的是仓库内容，不代表政府或机构的认可。

仓库中另有名为 **Bitcoin Conjegeum** 的章节，包含地址和图像素材。地址或图像的存在不构成投资建议、所有权验证或转账指令。使用前应独立核实相关信息。

## 十一、文档与 API 工具

`sleela docs` 可列出可文档化主题、显示包和类的页面，并可生成离线使用的纯文本文档集合。SleelaDoc 使用 `@intent`、`@thesis`、`@axiom`、`@param`、`@see` 和 `@mastery` 等标记扩展文档。参见 [SleelaDoc](lib/sleeladoc/README.md) 和[生成的 API 文档](sleeladoc-api/README.md)。

## 十二、AutoCAD Creator™

AutoCAD Creator™ 是 SLeeLa 编写的交互式二维 CAD 绘图应用。它支持创建和修改文档、添加墙体、房间、尺寸和注释、移动标签、管理图层、删除元素、撤销操作，以及保存 R12 文本 DXF 文件。界面使用 SleelaUI™，并可按平台通过 X11、Cocoa 或 Win32 打开原生窗口。

其架构原则是：**SleelaUI 负责呈现；SLeeLa 负责决策。** `SLCadCreator` 模型维护文档几何和可撤销历史，界面不是几何数据的权威来源。

```sh
make creator
```

请从 `autocad/` 目录执行。在无显示设备的环境中，示例也可以在内存中创建图形、输出报告并退出。

## 十三、UTF-4088 实验目录

`utf-4088/tools/generate_catalogue.py` 可确定性地生成包含 **1,048,576** 个条目的实验目录。文档描述的二进制文件大小为 16,777,244 字节。测试和 GitHub Actions 工作流检查条目数量、字形唯一性、文件大小和 SHA-256。二进制文件作为工作流产物发布，而不是存入 Git 历史。UTF-4088 仍属实验项目，且不兼容 Unicode。

## 十四、π、Modium、Registry 与 sleelactl

仓库记录了 π 的十进制数字目录，其中包括 **100,000,000** 位版本和 57,344 位版本。[VERIFIED](pi-generator/VERIFIED.md) 说明相关检查和复现方法。

`sleela registry` 和 `sleela modium` 等入口会加载大型 π 目录。引导安装器会安装 `sleelactl` 及其模块注册信息。项目将原生 C/C++ 库的 `math` 描述为首个集成 subject；具体行为应以模块文档和测试为准。

## 十五、国际化 README

国家版本采用 `README.<CC>.md` 命名方式。一个国家文件可根据实际语言情况包含多个语言章节。列出某种语言不代表该国所有区域语言、原住民语言、官方语言或手语均已翻译。

**翻译政策：** 技术名称、标识符、命令、路径、链接和代码示例保持不变。只有全部章节完成翻译并经过语言和技术审校后，才能称为完整翻译。尚未翻译或审校的技术内容仍以英文规范 README 为准。

## 主要参考

- [规范 README](README.md)
- [SLeeLa 语法规范](SLEELA.syntax)
- [文档索引](docs/README.md)
- [API 文档](docs/api/README.md)
- [平台支持](docs/PLATFORM-SUPPORT.md)
- [测试政策](markdown/TESTING.md)
- [GitHub 仓库](https://github.com/mearvk/SLeeLa)

**翻译状态：** 本版本已扩展项目介绍、入门、构建、安装、语言、功能、架构、库、安全、贡献和主要技术主题。仍需专业语言审校，并与英文 README 的后续变更逐项核对。


## 十六、编译目标、配置档案与执行边界

SLeeLa 将源代码与 build/execution targets 区分开来。已记录的目标系列包括 `SLVM`、`SLJVM` 和 `SLIR`；`NATIVE` 是受限制且有条件的目标，不应假设所有平台均可使用。`BASIC_COMPLETE` 与 `ADVANCED_TOTAL` 表示不同的 profile scope。每次构建都应确认实际 target、profile 和平台支持情况。

Reference 用于识别文件、模块或资源，本身并不授予执行权限。程序使用文件、外部库、native bindings 或系统资源时，必须明确权限边界和执行责任。文档中的功能描述不能单独证明 runtime security guarantee；应结合实现代码和相应测试进行判断。

## 十七、资源生命周期、错误处理与调试

对跨函数或线程共享的资源，应记录 ownership、lifetime 和 synchronization responsibilities。区分 local variables、shared object state、VM handles 与 serializable data。VM-local handle 不是稳定的文件格式或网络消息。使用模块定义的 serialization interface，并测试 invalid、missing 和 boundary values。

调试时记录 commit、操作系统、架构和 toolchain。先构建最小相关 target，再运行相应测试；保留完整错误输出，区分 compiler errors 与 runtime errors，并尽可能建立最小可复现案例。Build 成功并不意味着 runtime behavior 已正确。

验证报告应列出 exact commit、OS/architecture、compiler versions、完整 build/test commands、pass/fail/skip 数量及已知限制。Skipped 或未执行的 tests 不得计为 passed。

## 十八、跨平台与国际文档维护

不同操作系统的编译器、系统依赖、窗口后端、文件路径和安装权限可能不同。不要将 Linux 命令直接视为 Windows 或 macOS 的通用步骤。请结合 [platform support matrix](docs/PLATFORM-SUPPORT.md) 与 [installation guide](install/README.md)，记录实际测试的平台和工具链。

Country-specific README 是不同地区用户的技术文档入口，不取代 normative language specification 或 module API documentation。英文源文档更新后，应逐项检查翻译是否过时。Technical identifiers、commands、paths 和 code examples 应保持不变；尚未完成语言或技术审校的内容应明确标注。

**扩展翻译状态：** 简体中文指南现已增加 target/profile、执行权限边界、资源生命周期、调试与测试报告、跨平台安装注意事项及国际文档维护说明。


## 十九、模块 API 契约、兼容性与测试策略

引入新 API 时，应记录输入、输出、错误条件、数据所有权，以及操作是否产生 side effects。文档示例应同时展示正常用法和 invalid/missing input 的处理。未经 implementation 和 tests 验证的行为，不应写成保证。

测试应按 compiler pipeline 分层：lexing/parsing、semantic analysis、IR lowering、code generation、VM execution 和 native integration。每个 regression case 都应保留最小可复现输入、预期结果与实际结果。API 变更必须说明 backward compatibility、migration steps，以及对现有 examples 的影响。

## 二十、本地化、字符编码与可读性

不同语言版本中的 technical identifiers、commands、paths、syntax 和 code blocks 必须保持一致。翻译说明文字，不要翻译 source code 使用的 token 或名称。应明确语言和地区语境；简体中文术语应保持一致，同时避免把地区性用语误写成所有中文用户的统一表达。

每次更新后检查 Markdown links、标题层级、code fences 和示例。英文 canonical documents 更新时，应逐节比对并同步翻译，而不是只更新简介。未经母语者或技术审校的内容应明确标注。
