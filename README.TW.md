# SLeeLa 3.0 — 台灣（TW）

## 繁體中文

SLeeLa 是一套原生系統程式語言、編譯器、執行環境與虛擬機器生態系統。本版本面向台灣的繁體中文讀者。

**The Now. The Ingredient.** 程式語言不只是語法；編譯器、執行環境、函式庫、工具與平台契約，共同構成可實際使用的軟體系統。

## 一、快速開始

- [建置原生實作](impl/README.md)
- [虛擬機器與 Modium 使用指南](virtual-machine/USER-GUIDE.md)（`sleela vm`、`sleela modium`）
- [SLeeLa 原始碼中的 CPU 層次架構](lib/cpu/CPU.md)：從邏輯閘、ALU、處理器核心到作業系統與程式；包含 130 多種 CPU 設定，以及[電壓、時脈與時序參數](lib/cpu/TIMING.md)。
- [文件索引](docs/README.md)
- [產生的 API 文件](docs/api/README.md)與 [API 目錄](api/README.md)
- [計算比較標準](comparison/README.md)：SLeeLa VM、Java/JVM 與原生 C/C++ 的型別、記憶體、ABI 與相容性標準。
- [Recursion stanchion：C 與 C++ 參考原型](recursion/stanchion/README.md)
- [平台支援與驗證矩陣](docs/PLATFORM-SUPPORT.md)
- [品牌指南](docs/BRAND-GUIDE.md)
- [測試政策與發行門檻](markdown/TESTING.md)

## 二、建置與測試

**需求：** C/C++ 工具鏈（GCC 或 Clang）、GNU Make，以及用於清單與工作流程工具的 Python 3。Linux 圖形元件可能另外需要 X11/Xrandr 開發套件。

在儲存庫根目錄執行：

```sh
make
```

建置原生實作並執行完整測試套件：

```sh
make -C impl all
bash ./test-suites/run-all.sh --all
```

常用 Make 目標：

```sh
make all
sudo make install
make clean
make help
```

請參閱建置、測試及平台文件，以確認各目標的實際需求。單一元件建置成功，不代表所有函式庫或平台都已符合發行標準。

## 三、安裝

建置完成後，安裝命令與執行環境支援檔案：

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

預設系統安裝前綴通常為 `/usr/local`。使用 `PREFIX="$HOME/.local"` 可進行使用者本機安裝，通常不需要管理員權限。安裝程式會保留既有啟動設定，並避免覆寫不受其管理的既有命令。

若只需安裝主要套件（`sleela`、`sleelvac`、`nordshrift` 執行檔、標準函式庫、協定與支援模組），請執行：

```sh
make clean-install
```

其他選項請參閱[快速安全安裝指南](install/README.md)。

## 四、語言與執行環境

規範語法定義於[`SLEELA.syntax`](SLEELA.syntax)。儲存庫文件採用 **3.0** 語法版本。編譯器版本與原始碼語法版本是不同概念。3.0 彙整了 1.0 至 2.9 的語言功能，同時保留各項功能本身的最低版本需求。

技術文件涵蓋 `@mix` 連續欄位、擴充整數型別家族、Preonem 文件標記、SleelaDoc、API 文件產生及 Nordshrift 工具。技術識別名稱、命令、檔名、API 名稱與程式碼範例維持原樣，以確保可以直接使用。

## 五、工程原則與驗證

專案工程原則包括：以證據為依據、在元件邊界訂定明確契約、從設計階段考量可攜性、失敗時採取安全行為，以及產生可重現的發行版本。

驗證狀態必須依模組及平台分別評估。未執行的檢查不可標示為通過；單一元件編譯成功也不能取代完整的發行驗證。

Sleelavirin 防護工具包含本機 SHA-256 驗證器、選用的 ClamAV 介接器及完整性清單。只有在目標發行提交的 CI 檢查完成，且產生的成品經過審查後，才能將該發行視為已驗證。

## 六、編輯器與貢獻

SLeeLa 為 **Vim** 與 **Emacs** 提供佈景主題及語法整合，並提供原生 C/C++ 色盤工具。詳細資訊請參閱[編輯器整合指南](linux-editors/README.md)。

貢獻流程建議：

1. 閱讀文件索引與測試政策。
2. 提交範圍明確的變更，並提供正向及負向測試。
3. 執行相關測試，再執行較完整的測試套件。
4. 說明使用的作業系統與工具鏈；不得將略過的檢查描述為通過。

SLeeLa 由 **MEARVK LLC** 開發。

## 七、其他專案內容

主要 README 也介紹 Advantage、SleelaDoc 與 XML DOM 函式庫；整數基本型別；字元執行環境；AutoCAD Creator™；實驗性 UTF-4088 目錄；Pi、Modium、Registry 與 `sleelactl` 工具；以及由 `@native` 標記啟動的原生 C/C++ 編譯路徑第一階段。

## 完整技術參考

[英文規範 README.md](README.md)包含完整功能目錄、語言與執行環境參考、建置及安裝說明、API 連結、範例與版本記錄。本繁體中文版已在地化主要使用說明與工程指南，但尚未逐行翻譯規範 README 的所有技術章節。在完整翻譯並完成技術審閱之前，英文 README 仍是規範性技術來源。

- [規範 README](README.md)
- [SLeeLa 語法規範](SLEELA.syntax)
- [文件索引](docs/README.md)
- [平台支援](docs/PLATFORM-SUPPORT.md)
- [GitHub 儲存庫](https://github.com/mearvk/SLeeLa)

**翻譯狀態：** 已翻譯專案介紹、快速開始、建置、安裝、語言與執行環境概述、工程驗證及貢獻指南。完整技術章節仍需持續翻譯，並由熟悉技術領域的繁體中文審校者複核。


## 八、編譯器架構與程式流程

SLeeLa 將來源語言、編譯器前端、中介表示、程式碼產生及 runtime 分成不同層次。Compiler release、syntax edition 與 VM/ISA version 是不同的版本軸。某項功能出現在規格中，不代表它已在所有目標平台完整實作並通過驗證。

一般流程如下：

1. **來源檔案：** 使用 UTF-8 編碼的 `.sleela` 檔案，包含版本宣告與程式宣告。
2. **詞法與語法分析：** 將文字轉換成 token 與語法結構。
3. **語意分析：** 檢查名稱、符號、型別與語言規則。
4. **中介表示：** 產生供後續 lowering 使用的結構化程式表示。
5. **程式碼產生：** 輸出至受支援的 VM 或中介目標。
6. **Runtime：** 使用現有的記憶體管理與控制流程機制執行編譯結果。

規範依據為 [SLEELA.syntax](SLEELA.syntax)。來源格式與生命週期請參閱 [source documentation](markdown/SOURCE.md) 及 [compiler documentation](markdown/COMPILER.md)。

## 九、型別系統與物件模型

SLeeLa 的核心型別詞彙包括 `int`、`double`、`boolean`、`String`、`void`、自行宣告的 `struct`，以及受支援的 reference-type signatures。依目前文件，`int`、`long` 與 `int64` 都是 signed 64-bit integer。`int128` 雖可作為整數型別接受，但目前以 64-bit 為基礎，不應視為完整的 128-bit arithmetic。

`struct` 是由具名欄位組成的 aggregate。VM 管理的 struct handle 並非直接暴露的 raw pointer。指派與參數傳遞會共用同一個 instance；相等性依 identity 判斷。跨程序或跨 VM 傳輸時，應使用文件所述的序列化方式，例如 `structPack(instance)` 與 `structUnpack(TypeName, json)`，不可將 VM-local handle 當作可攜式網路資料。

Class 包含 fields 與 methods。現行執行模型中的 fields 是共享的 class state，而 method-local variables 屬於各 thread。Inheritance、nested classes 與 generics 受 syntax-version gates 限制。完整說明見 [International Language Reference](LANGUAGE.INTERNATIONAL.3.0.md)。

## 十、控制流程與迴圈計時

語言支援條件判斷、迴圈、方法呼叫及遞迴。Statement-level annotations `@loopDelay` 與 `@loopBudget` 用於指定迴圈暫停時間或總時間預算。標註寫在迴圈之前，並會 lowering 成 VM instruction `OP_SLEEP`（ISA 157）；一般內建函式 `sleep(ms)` 也使用此指令。

```sleela
@loopDelay(ms = 250)
while (running) {
    processNext();
}
```

此程式碼僅展示語法形式；實際程式必須自行定義所需的 identifiers 與 enclosing class。請參閱 [SLEELA.syntax §32](SLEELA.syntax) 及 [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md)。

## 十一、標準函式庫與主要模組

Repository 包含成熟度不同的多個模組：

- `lib/advantage` — 以模型為基礎的分層結果評估，涵蓋數學、雙邊貿易經濟、整合、分級及可路由輸出。文件中的 trust-field physics 應視為建模類比，而非物理定律主張。
- `lib/xml` — XML document tree，包含 element、text、comment、CDATA、processing instruction、attributes、parser 與 serializer。
- `lib/text` — 以 character cell 為基礎的文字與字串操作，例如 `charAt`、`substring`、`indexOf`、`reverse` 及位置格式化。
- `lib/sleeladoc` — 原始碼文件擷取與 CLI reference pages。`sleela docs` 可查詢 topics、packages 與 classes；`sleela docs --generate [dir]` 可建立離線文字語料。
- `lib/cpu` 與 `lib/xlt` — CPU profiles、架構元件、模型格式及 timing references。
- `recursion/stanchion` — C/C++ reference prototypes 與 recursion examples。

模組出現在 catalog 中，不代表它已可用於 production。使用前應檢查模組 README、測試涵蓋率、平台限制及 validation records。

## 十二、C/C++ 整合與 native 邊界

SLeeLa repository 包含 C 與 C++ components，以及受限制的 native code-generation path。`@native` 與 `sleela aot` 等項目屬於 native development 範圍，不是執行任意 native code 的通用授權。

C/C++ 整合必須明確定義 ABI、calling convention、memory ownership、data layout、error handling 與 platform dependencies。應將 native code 視為獨立的 trust boundary。Reference 或 declaration 只能識別資源，不能授予執行權限。請參閱 [native implementation](impl/README.md)、[platform matrix](docs/PLATFORM-SUPPORT.md) 與 [comparison standards](comparison/README.md)。

## 十三、建置、安裝與平台支援

Native build 通常需要 C/C++ toolchain（例如 GCC 或 Clang）及 GNU Make。部分 tooling 使用 Python 3；Linux GUI components 可能需要 X11 與 XRandR development packages。

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
```

系統層級安裝通常使用：

```sh
sudo make install
```

僅供目前使用者安裝：

```sh
make install PREFIX="$HOME/.local"
```

請依照 [installation guide](install/README.md) 與 [platform support matrix](docs/PLATFORM-SUPPORT.md) 檢查平台需求。Linux、macOS 與 Windows 的相依套件及安裝方式不應被假設為完全相同。

## 十四、完整性、安全與測試

Hash manifests 與 verification tools 可檢查 repository files 是否符合預期內容。PASS 只適用於明確檢查的範圍與時間。測試報告必須分開列出 pass、fail 與 skip；被略過或未執行的測試不得標示為通過。

重要原則：

- Reference 用來識別資源，不代表授權執行。
- Native interfaces 必須明確且受限制。
- Platform-support claims 必須有實際 build 與 tests 支持。
- Generated documentation 必須能追溯至 source 與 generator。
- 缺少必要 integrity data 時，security checks 應採 fail-closed 行為。

請參閱 [testing policy](markdown/TESTING.md) 與 [platform validation](docs/PLATFORM-SUPPORT.md)。

## 十五、文件、編輯器與貢獻流程

建議從 [documentation index](docs/README.md)、[generated API reference](docs/api/README.md)、[API catalogue](api/README.md) 及 [SleelaDoc](lib/sleeladoc/README.md) 開始。Linux editor resources 請見 [linux-editors](linux-editors/README.md)。

提交變更前，請閱讀相關 module README、界定修改範圍、加入 positive 與 negative tests、執行相關測試、記錄 OS/toolchain/commands/actual results，並在 public behavior 改變時同步更新 README 與 API documentation。

## 十六、其他專案領域

CPU 與 hardware models、GUI、networking、text processing、UTF-4088、π tools、Modium、Registry、`sleelactl` 及 AutoCAD Creator™ 都是不同的 project areas，其目的、相依性與成熟度可能不同。沒有文件化驗證時，不應將 experimental directory 或 example 描述為穩定的 production feature。UTF-4088 的資料量或 entry count 本身不能證明完整 Unicode compatibility；AutoCAD compatibility 則需要明確的版本矩陣與測試結果。

## 十七、台灣繁體中文在地化

本版本以台灣常用的繁體中文術語為主，例如「建置」、「執行環境」、「函式庫」、「程式碼」及「驗證」。技術 identifiers、commands、filenames、API names 與 source code 必須維持原樣，避免翻譯後無法直接複製執行。後續術語調整應維持全文件一致，並建議由熟悉系統程式設計的繁體中文母語者審閱。

**翻譯狀態：** 本繁體中文版已擴充編譯流程、型別與物件模型、迴圈計時、函式庫、native integration、建置與安裝、安全、測試及貢獻流程。它仍不是 canonical English README 與所有連結規格的逐行完整翻譯。


## 十八、目標平台、設定檔與執行授權

SLeeLa 將 source code 與 build/execution targets 分開。文件列出的 target family 包括 `SLVM`、`SLJVM` 及 `SLIR`；`NATIVE` 是受限制且有條件的 target，不應假設所有平台均可使用。`BASIC_COMPLETE` 與 `ADVANCED_TOTAL` 代表不同的 profile scope。每次建置都應確認實際 target、profile 及平台支援情況。

Reference 用來識別檔案、module 或 resource，本身不會授予 execution authorization。程式使用檔案、外部 libraries、native bindings 或 system resources 時，必須明確界定執行權限及安全邊界。文件中的功能描述不能單獨作為 runtime security guarantee 的證據；應同時查閱 implementation 和相應測試。

## 十九、資源生命週期與除錯

對跨 function 或 thread 共用的資源，應記錄 ownership、lifetime 和 synchronization responsibilities。區分 local variables、shared object state、VM handles 及 serializable data。VM-local handle 不應當作穩定的 file format 或 network message。使用 module 定義的 serialization interface，並測試 invalid、missing 及 boundary values。

Debug 時記錄 commit、OS、architecture 和 toolchain。先 build 最小相關 target，再執行對應測試；保留完整 error output，分辨 compiler errors 與 runtime errors，並建立可重現的最小案例。Build 成功不等於 runtime behavior 已正確。

驗證報告應列出 exact commit、OS/architecture、compiler versions、完整 build/test commands、pass/fail/skip 數量及已知限制。Skipped 或未執行的 tests 不可列為 passed。

**擴充翻譯狀態：** 台灣繁體中文版現亦涵蓋 target/profile、execution authorization、resource lifecycle、debugging 及 verification reporting。


## 二十、模組 API、相容性與測試策略

新增 API 時，應記錄輸入、輸出、錯誤條件、資料 ownership，以及操作是否產生 side effects。文件範例應同時涵蓋正常流程，以及 invalid 或 missing input 的處理。未經 implementation 與 tests 證實的行為，不應描述為保證。

測試應依 compiler pipeline 分層：lexing/parsing、semantic analysis、IR lowering、code generation、VM execution 與 native integration。每個 regression case 都應保留最小可重現輸入、預期結果與實際結果。API 變更必須說明 backward compatibility、migration steps，以及對既有 examples 的影響。

## 二十一、語言在地化、編碼與可讀性

技術 identifiers、commands、paths、syntax 和 code blocks 應在不同語言版本保持一致。翻譯說明文字，不翻譯 source code 使用的 token 或名稱。應明確標示語言與地區語境；台灣繁體中文與其他繁體中文地區的術語可能不同，應避免未經確認便混用。

每次更新後檢查 Markdown links、標題層級、code fences 和範例。英文 canonical documents 更新時，應比對相關章節並同步翻譯，不應只更新簡介。尚未完成語言或技術審校的內容須清楚標示。
