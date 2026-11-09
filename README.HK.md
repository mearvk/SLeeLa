# SLeeLa 3.0 — 香港（HK）

## 繁體中文

SLeeLa 是一套原生系統程式語言、編譯器、執行環境及虛擬機器生態系統。本版本面向香港的繁體中文讀者。

## English

SLeeLa is a native systems programming language, compiler, runtime, and virtual-machine ecosystem. Technical identifiers, source code, commands, filenames, and API names remain unchanged in this edition.

**The Now. The Ingredient.** 程式語言不止是語法；編譯器、執行環境、程式庫、工具及平台契約，都是令軟件能夠實際使用的重要部分。

## 一、開始使用

- [建置原生實作](impl/README.md)
- [虛擬機器及 Modium 使用指南](virtual-machine/USER-GUIDE.md)（`sleela vm`、`sleela modium`）
- [SLeeLa 原始碼中的 CPU 架構](lib/cpu/CPU.md)：由邏輯閘、ALU、處理器核心到作業系統及程式；包括 130 多種 CPU 設定，以及[電壓、時鐘及時序參數](lib/cpu/TIMING.md)。
- [文件索引](docs/README.md)
- [產生的 API 文件](docs/api/README.md)及 [API 目錄](api/README.md)
- [計算比較標準](comparison/README.md)：SLeeLa VM、Java/JVM 及原生 C/C++ 的類型、記憶體、ABI 及相容性標準。
- [Recursion stanchion：C 及 C++ 參考原型](recursion/stanchion/README.md)
- [平台支援及驗證矩陣](docs/PLATFORM-SUPPORT.md)
- [品牌指南](docs/BRAND-GUIDE.md)
- [測試政策及發佈門檻](markdown/TESTING.md)

## 二、建置及測試

**所需工具：** C/C++ 工具鏈（GCC 或 Clang）、GNU Make，以及用於清單和工作流程工具的 Python 3。Linux 圖形元件可能亦需要 X11/Xrandr 開發套件。

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

請參閱建置、測試及平台文件，以確認每個目標的實際要求。單一元件成功建置，並不代表所有程式庫或平台均已符合發佈標準。

## 三、安裝

完成建置後，可安裝命令及執行環境支援檔案：

```sh
sudo make install
make install PREFIX="$HOME/.local"
make install DESTDIR="$PWD/package-root"
make install-check
```

預設系統安裝前綴通常為 `/usr/local`。使用 `PREFIX="$HOME/.local"` 可進行使用者本機安裝，一般毋須管理員權限。安裝程式會保留現有啟動設定，並避免覆寫不由它管理的現有命令。

如只需安裝主要套件（`sleela`、`sleelvac`、`nordshrift` 執行檔、標準程式庫、協定及支援模組），請執行：

```sh
make clean-install
```

其他選項請參閱[快速安全安裝指南](install/README.md)。

## 四、語言及執行環境

規範語法載於[`SLEELA.syntax`](SLEELA.syntax)。儲存庫文件採用 **3.0** 語法版本。編譯器版本與原始碼語法版本是兩個不同概念。3.0 整合 1.0 至 2.9 的語言功能，同時保留各項功能本身的最低版本要求。

技術文件涵蓋 `@mix` 連續欄位、擴展整數類型、Preonem 文件標記、SleelaDoc、API 文件產生及 Nordshrift 工具。技術識別名稱、命令、檔名、API 名稱及程式碼範例均維持原樣，方便直接使用。

## 五、工程原則及驗證

項目工程原則包括以證據為先、在元件邊界訂立清晰契約、從設計階段考慮可攜性、在失敗時採取安全行為，以及建立可重現的發佈版本。

驗證狀態必須按模組及平台分別評估。未有執行的檢查不可標示為通過；單一元件成功編譯亦不能代替完整的發佈驗證。

Sleelavirin 防護工具包括本機 SHA-256 驗證器、可選用的 ClamAV 介接器及完整性清單。只有當目標發佈提交的 CI 檢查完成，並且產生的成品經過審查後，才可將該發佈視為已驗證。

## 六、編輯器及貢獻

SLeeLa 為 **Vim** 及 **Emacs** 提供主題和語法整合，並提供原生 C/C++ 色彩工具。詳情請參閱[編輯器整合指南](linux-editors/README.md)。

貢獻時建議：

1. 閱讀文件索引及測試政策。
2. 提交範圍清晰的修改，並附上正向及負向測試。
3. 執行相關測試，再執行較完整的測試套件。
4. 說明所用作業系統及工具鏈；不可把略過的檢查說成已通過。

SLeeLa 由 **MEARVK LLC** 開發。

## 七、其他項目內容

主要 README 亦介紹 Advantage、SleelaDoc 及 XML DOM 程式庫；整數基本類型；字元執行環境；AutoCAD Creator™；實驗性 UTF-4088 目錄；Pi、Modium、Registry 及 `sleelactl` 工具；以及由 `@native` 標記啟動的原生 C/C++ 編譯路徑第一階段。

## 完整技術參考

[英文規範 README.md](README.md)載有完整功能目錄、語言及執行環境參考、建置和安裝說明、API 連結、範例及版本記錄。本香港版本已翻譯主要使用說明和工程指南，但尚未逐行翻譯規範 README 的全部技術章節。在完整翻譯並完成技術審閱之前，英文 README 仍是規範性技術來源。

- [規範 README](README.md)
- [SLeeLa 語法規範](SLEELA.syntax)
- [文件索引](docs/README.md)
- [平台支援](docs/PLATFORM-SUPPORT.md)
- [GitHub 儲存庫](https://github.com/mearvk/SLeeLa)

**翻譯狀態：** 已翻譯項目介紹、開始使用、建置、安裝、語言及執行環境概述、工程驗證及貢獻指南。完整技術章節仍需繼續翻譯，並由熟悉技術領域的繁體中文審校者複核。


## 八、軟件編輯器及工程工作流程

SLeeLa 為 Vim 及 Emacs 提供主題和語法整合，亦提供原生 C/C++ 色彩工具。請參閱[編輯器整合指南](linux-editors/README.md)，並按實際使用的編輯器版本及作業系統選擇安裝方法。

建議貢獻流程：

1. 閱讀文件索引、相關模組文件及測試政策。
2. 將修改限制在清晰界定的範圍，並說明設計理由。
3. 加入正向測試及負向測試，涵蓋預期行為和錯誤情況。
4. 執行相關測試，再按需要執行完整測試套件。
5. 記錄實際使用的作業系統、工具鏈和測試結果；不可將略過的檢查標示為通過。

## 九、程式庫及項目範圍

主要 README 涵蓋 Advantage、SleelaDoc 及 XML DOM 程式庫，亦列出整數基本類型、字元執行環境、AutoCAD Creator™、UTF-4088 實驗性目錄、Pi 常數、Modium™、Registry™ 及 `sleelactl` 工具。

UTF-4088 的百萬項目實驗目錄及其他實驗資料，必須按照各自文件理解。目錄中存在某項目，並不代表該項目已投入正式生產使用。維護套件索引時，應保留路徑、負責範圍、相依性及測試狀態的可追溯性。

## 十、`@native` 與原生 C/C++ 路徑

文件將 `@native` 標記及 `sleela aot` 命令描述為 SLeeLa 原始碼通往原生 C/C++ 路徑的第一階段。這不代表可以不受限制地執行原生程式碼。介面邊界、ABI、明確授權及安全檢查仍然重要。原生目標的實際支援狀態，必須以目前實作及測試結果為準。

## 十一、發佈及國際化文件

項目新聞、發佈記錄和各國 README 文件清單均由主要 README 維護。國家或語言版本是本地化入口，不應自動視為所有技術章節的完整翻譯。技術識別名稱、命令、檔名及程式碼範例維持原樣，以便讀者直接對照原始碼和規範。

## 十二、參考連結

- [規範 README（英文）](README.md)
- [SLeeLa 語法規範](SLEELA.syntax)
- [文件索引](docs/README.md)
- [平台支援](docs/PLATFORM-SUPPORT.md)
- [原生實作建置文件](impl/README.md)
- [安裝指南](install/README.md)
- [測試政策](markdown/TESTING.md)
- [SLeeLa GitHub 儲存庫](https://github.com/mearvk/SLeeLa)

## 翻譯狀態補充

本香港版本現已涵蓋項目介紹、開始使用、建置、安裝、語言及執行環境、工程驗證、編輯器整合、程式庫範圍和原生 C/C++ 路徑。它仍未逐行翻譯規範 README 的全部章節；在完整翻譯並完成技術審閱之前，英文 [README.md](README.md) 仍是規範性技術來源。

SLeeLa 由 MEARVK LLC 開發。


## 十三、編譯器架構與執行流程

SLeeLa 將來源語言、compiler frontend、中介表示、程式碼產生及 runtime execution 分成不同階段。Compiler release、syntax edition 與 VM/ISA version 是不同的版本軸。規格中列出某項功能，並不代表它已在所有 target 完整實作及驗證。

一般流程由 UTF-8 `.sleela` source files 開始，進行 lexical/syntax analysis、symbol/type checks、intermediate representation lowering，為受支援的 VM 或 intermediate targets 產生程式碼，最後由 runtime 執行。規範語法見 [SLEELA.syntax](SLEELA.syntax)；source lifecycle 及 compiler pipeline 見 [source documentation](markdown/SOURCE.md) 和 [compiler documentation](markdown/COMPILER.md)。

## 十四、型別系統與物件模型

已記錄的 source types 包括 `int`、`double`、`boolean`、`String`、`void`、自行宣告的 `struct` types，以及受支援的 reference-type signatures。`int`、`long`、`int64` 為 signed 64-bit integers。`int128` 雖可作為 integer type 接受，但目前文件所述 implementation 仍以 64-bit 為基礎，不應視為完整 128-bit arithmetic。

`struct` 是具名 aggregate。VM 管理的 struct handles 並非 raw pointers；assignment 與 parameter passing 共用同一 instance，equality 依 identity 判斷。跨 VM 傳輸時，應使用 `structPack(instance)` 和 `structUnpack(TypeName, json)` 等文件化 serialization helpers，不可直接傳送 VM-local handle。Class 包含 fields 與 methods；現行模型中的 fields 是共享狀態，method locals 則屬於各 thread。Inheritance、nested classes 及 generics 受 syntax-version gates 限制。

## 十五、迴圈計時、函式庫與 native 整合

Statement-level annotations `@loopDelay` 和 `@loopBudget` 用來設定迴圈暫停或總時間預算，並會 lowering 成 VM instruction `OP_SLEEP`（ISA 157）；built-in `sleep(ms)` 亦使用相同指令。詳情見 [SLEELA.syntax §32](SLEELA.syntax) 及 [Loop Timing API](api/sleela/3.0/LOOP_TIMING_API.md)。

主要文件化模組包括 `lib/advantage`（model-based evaluation、數學、整合、grading 及 routed output）、`lib/xml`（XML DOM）、`lib/text`（character-cell text/string operations）、`lib/sleeladoc`（source documentation 及 CLI reference）、`lib/cpu`、`lib/xlt`（CPU profiles 與 model/timing references），以及 `recursion/stanchion`（C/C++ recursion prototypes）。模組出現在 catalog 不代表已達 production readiness；使用前應檢查測試與平台限制。

C/C++ boundary 必須明確定義 ABI、calling convention、memory ownership、data layout、error handling 及 platform dependencies。`@native` 和 `sleela aot` 並非執行任意 native code 的一般授權。Reference 只識別資源，不授予 execution authorization。

## 十六、建置、安裝與驗證

Native build 通常需要 GCC 或 Clang 及 GNU Make，部分工具使用 Python 3。常用命令：

```sh
make
make -C impl all
bash ./test-suites/run-all.sh --all
sudo make install
make install PREFIX="$HOME/.local"
```

請查閱 [installation guide](install/README.md)、[platform matrix](docs/PLATFORM-SUPPORT.md) 及 [testing policy](markdown/TESTING.md)。Hash manifests 與 verification tools 的 PASS 只適用於明確列出的檢查範圍與時間。測試報告必須分開列出 pass、fail、skip；略過或未執行的測試不可標示為成功。

**更新後的翻譯狀態：** 香港版本已增加 compiler pipeline、types/object model、loop timing、libraries、native integration、build/install 與 verification 等繁體中文技術內容。本文仍不是 canonical English README 及所有相關規格的逐行完整翻譯。


## 十七、編譯目標、設定檔與安全邊界

SLeeLa 將 source code 與 execution/build targets 分開。文件列出的 target family 包括 `SLVM`、`SLJVM` 及 `SLIR`；`NATIVE` 是受限制且有條件的 target，不應假設所有平台均可使用。`BASIC_COMPLETE` 與 `ADVANCED_TOTAL` 代表不同的 profile scope。每次 build 都應核對實際 target、profile 及平台支援情況。

Reference 用來識別檔案、module 或 resource，本身不會授予執行權限。程式使用檔案、外部 libraries、native bindings 或 system resources 時，必須明確界定執行權限及安全邊界。文件中的功能描述不能單獨作為 runtime security guarantee 的證據；應同時查閱 implementation 和相應測試。

## 十八、資源生命週期與錯誤處理

對跨 function 或 thread 共用的資源，應記錄 ownership、lifetime 和同步責任。區分 local variables、shared object state、VM handles 及 serializable data。VM-local handle 不應當作穩定的 file format 或 network message。使用 module 提供的 serialization interface，並測試 invalid、missing 及 boundary values。

Debug 時記錄 commit、OS、architecture 和 toolchain。先 build 最小相關 target，再執行對應測試；保留完整 error output，分辨 compiler errors 與 runtime errors，並盡量建立可重現的最小案例。Build 成功不等於 runtime behavior 已正確。

驗證報告應列出 exact commit、OS/architecture、compiler versions、完整 build/test commands、pass/fail/skip 數量及已知限制。Skipped 或未執行的 tests 不可列為 passed。

## 十九、國際文件維護與版本同步

Country-specific README 是各地區使用者的文件入口，不取代 normative language specification 或 module API documentation。英文來源更新後，應逐項檢查翻譯是否過時。Technical identifiers、commands、paths 和 code examples 應保持原樣；未經語言或技術審校的內容須明確標示。

**擴充翻譯狀態：** 香港繁體中文技術指南現亦涵蓋 target/profile、native execution boundary、resource ownership、debugging、test reporting 及國際文件維護。


## 二十、模組 API、相容性與測試策略

新增 API 時，應清楚記錄輸入、輸出、錯誤條件、資料 ownership，以及操作是否產生 side effects。文件範例應同時展示正常流程與 invalid/missing input 的處理方式。未經 implementation 和 tests 證實的行為，不應寫成保證。

測試應按 compiler pipeline 分層：lexing/parsing、semantic analysis、IR lowering、code generation、VM execution 及 native integration。每個 regression case 都應保留最小可重現輸入、預期結果及實際結果。API 變更須說明 backward compatibility、migration steps，以及對既有 examples 的影響。

## 二十一、在地化、編碼與可讀性

技術 identifiers、commands、paths、syntax 和 code blocks 必須跨語言保持一致。翻譯說明文字，不要翻譯 source code 中使用的 token 或名稱。語言和地區用語應清楚標示，不應假設香港、台灣及其他繁體中文社群的用語完全相同。

每次更新後檢查 Markdown links、標題層級、code fences 和範例。英文 canonical documents 更新時，應比對相關章節並同步翻譯，而非只修改簡介。未經語言或技術審校的內容應明確標示。
