# SLeeLa 3.0 — 中国（CN）

## 简体中文

SLeeLa 是一套原生系统编程语言、编译器、运行时和虚拟机生态系统。本版本面向中国大陆的简体中文读者。

**The Now. The Ingredient.** 编程语言不只是语法。编译器、运行时、库、工具以及平台契约共同构成可实际使用的软件系统。

## 一、快速开始

- [构建原生实现](impl/README.md)
- [虚拟机与 Modium 用户指南](virtual-machine/USER-GUIDE.md)（`sleela vm`、`sleela modium`）
- [SLeeLa 源码中的 CPU 层次结构](lib/cpu/CPU.md)：从逻辑门、ALU、处理器核心到操作系统和程序；包含 130 多种 CPU 配置，以及[电压、时钟和时序参数](lib/cpu/TIMING.md)。
- [文档索引](docs/README.md)
- [生成的 API 文档](docs/api/README.md)和[API 目录](api/README.md)
- [计算比较规范](comparison/README.md)：SLeeLa VM、Java/JVM 与原生 C/C++ 的类型、内存、ABI 和一致性规范。
- [Recursion stanchion：C 与 C++ 参考原型](recursion/stanchion/README.md)
- [平台支持与验证矩阵](docs/PLATFORM-SUPPORT.md)
- [品牌指南](docs/BRAND-GUIDE.md)
- [测试政策与发布门槛](markdown/TESTING.md)

## 二、构建与测试

**依赖项：** C/C++ 工具链（GCC 或 Clang）、GNU Make，以及用于清单和工作流工具的 Python 3。Linux 图形组件可能还需要 X11/Xrandr 开发包。

在仓库根目录运行：

```sh
make
```

构建原生实现并运行完整测试套件：

```sh
make -C impl all
bash ./test-suites/run-all.sh --all
```

常用 Make 目标：

```sh
make all
sudo make install
make clean
make help
```

请查阅构建、测试和平台文档，确认每个目标的具体要求。单个组件构建成功，并不代表所有库或平台均已达到发布标准。

## 三、安装

构建完成后，可安装命令和运行时支持文件：

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

默认系统安装前缀通常为 `/usr/local`。使用 `PREFIX="$HOME/.local"` 可进行用户级安装，通常无需管理员权限。安装程序会保留现有启动配置，并避免覆盖不受其管理的现有命令。

仅安装主要软件包（`sleela`、`sleelvac`、`nordshrift` 可执行文件、标准库、协议和支持模块）：

```sh
make clean-install
```

更多选项请参阅[快速安全安装指南](install/README.md)。

## 四、语言与运行时

规范语法定义于[`SLEELA.syntax`](SLEELA.syntax)。仓库文档采用 **3.0** 语法版本。编译器版本与源代码语法版本是两个不同概念。3.0 汇总了 1.0 至 2.9 的语言功能，同时保留各项功能自身的最低版本要求。

技术文档涵盖 `@mix` 连续字段、扩展整数类型族、Preonem 文档标记、SleelaDoc、API 文档生成和 Nordshrift 工具。技术标识符、命令、文件名、API 名称及代码示例保持原样，以确保可直接使用。

## 五、工程原则与验证

项目工程原则包括：以证据为依据、在组件边界定义明确契约、从设计阶段考虑可移植性、在失败时采取安全行为，以及生成可重复的发布版本。

验证状态必须按模块和平台分别评估。未执行的检查不能标记为通过；单个组件的编译成功也不能代替完整的发布验证。

Sleelavirin 防护工具包含本地 SHA-256 校验器、可选的 ClamAV 适配器和完整性清单。只有在目标发布提交对应的 CI 检查完成并且生成产物经过审查后，才能将相应发布视为已验证。

## 六、编辑器与贡献

SLeeLa 为 **Vim** 和 **Emacs** 提供主题及语法集成，并提供原生 C/C++ 调色板工具。详情见[编辑器集成指南](linux-editors/README.md)。

贡献流程建议：

1. 阅读文档索引和测试政策。
2. 提交范围明确的修改，并配套正向与负向测试。
3. 运行相关测试，再运行更完整的测试套件。
4. 说明使用的操作系统和工具链；不得把跳过的检查描述为通过。

SLeeLa 由 **MEARVK LLC** 开发。

## 七、其他项目内容

主 README 还介绍 Advantage、SleelaDoc 和 XML DOM 库；整数基础类型；字符运行时；AutoCAD Creator™；实验性 UTF-4088 目录；Pi、Modium、Registry 和 `sleelactl` 工具；以及由 `@native` 标记启用的原生 C/C++ 编译路径初始阶段。

## 完整技术参考

[英文规范 README.md](README.md)包含完整功能目录、语言与运行时参考、构建和安装说明、API 链接、示例及版本记录。本中文版本已本地化主要使用说明和工程指南，但尚未逐行翻译规范 README 的全部技术章节。完整翻译经技术审校之前，英文 README 仍是规范性技术来源。

- [规范 README](README.md)
- [SLeeLa 语法规范](SLEELA.syntax)
- [文档索引](docs/README.md)
- [平台支持](docs/PLATFORM-SUPPORT.md)
- [GitHub 仓库](https://github.com/mearvk/SLeeLa)

**翻译状态：** 已翻译项目介绍、快速开始、构建、安装、语言与运行时概述、工程验证及贡献指南。完整技术章节仍需继续翻译并由熟悉技术领域的简体中文审校者复核。
