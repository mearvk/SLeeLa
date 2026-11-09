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
