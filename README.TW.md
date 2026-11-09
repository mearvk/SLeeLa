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
