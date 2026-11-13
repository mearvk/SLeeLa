<img align="right" src="images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — 香港（HK）

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>原生系統語言、編譯器、執行環境與虛擬機生態系統。</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **國際國家版本。** 本檔案提供香港版本，以香港慣用的繁體中文書面語撰寫，並在適當之處附有英文附錄。技術識別名稱、命令、程式碼、檔名、API 名稱及連結維持不變；完整的技術參考內容與英文規範版 [README.md](README.md) 一致。

**The Now. The Ingredient.** 程式語言不只是語法；編譯器、執行環境、程式庫、工具與平台契約，共同構成可實際使用的軟件。

**SLeeLa 所涉足的領域。** SLeeLa 涉足 **Math（數學）**、**Object（物件）**、
**Science（科學）**與 **Reach（觸及）**——這是其工作的四個面向：可計算者
（Math）、在存在目錄中已然確立的事物（Object）、對真理嚴謹的研究
（Science），以及將一個系統之手伸向其他系統與人（Reach）。**國家利益依然保留**——
四者皆不會將其擱置；凡主題涉及國家利益之處，
該地位均予保留（如實記錄，例如記錄於各類別 Metadata 的
`metaOrigin` / `metaNationalSchedule`），絕不消解。

## I. 開始使用
- [建置原生實作](impl/README.md)
- [虛擬機與 Modium——實作使用指南](virtual-machine/USER-GUIDE.md)（`sleela vm`、`sleela modium`）
- [SLeeLa 原始碼中的 CPU 堆疊](lib/cpu/CPU.md)——完整的邏輯閘→ALU→處理器核心→作業系統→程式機器、130 種以上的 CPU 設定檔，以及可調整的[電壓、可變時鐘與進階時序](lib/cpu/TIMING.md)，你可以[依規格執行並超頻](lib/cpu/components/README.md)
- [文件索引](docs/README.md)
- [CSPML 資料來源與 Salt & Salary 研究參考](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md)——美國外部地點、社會經濟、勞動力、教育與公立學校資料的來源出處與整合指引。
- [產生的 API 文件](docs/api/README.md)——最新的 HTML 參考、版本化 API 版次與重新產生說明
- [API 目錄](api/README.md)
- [計算比較標準](comparison/README.md)——SLeeLa VM、Java/JVM 與原生 C/C++ 的類型、記憶體、ABI 與一致性標準。
- [Recursion stanchion——C 與 C++ 原型](recursion/stanchion/README.md)——針對 Boolean、Byte、Character、Integer、Long、Float 與 Double 的基本類型特化範例，以及通用、應用、科學與教學用的遞迴模式。
- [平台支援與驗證矩陣](docs/PLATFORM-SUPPORT.md)
- [品牌指南](docs/BRAND-GUIDE.md)
- [周邊商品概念與素材](merchandise/README.md)
- [NitroWebExpress 標誌](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [測試政策與發佈門檻](markdown/TESTING.md)

## II. 最新消息

語言、驅動程式與標準程式庫的近期新增內容：

- **`advantage` 標準程式庫套件——對股利進行 advantaging，內含數學、物理、整合、分級與可路由輸出。**
  [`lib/advantage/`](lib/advantage/README.md) 將*針對某一模型 advantaging 一筆獨特股利*
  封裝為分層的 **Sequitur onion**
  （股利 → 審查 → 疑慮 → 數學 → 增補 → 減除 → 重組
  → 迭代），可向內或向外瀏覽，並具備完整的動態更新與
  復原（restitute）功能，透過簡易的**使用者模型**與完全掌控的
  **開發者模型**對外提供。它涵蓋五個主題領域：**`math/`**
  雙邊貿易經濟學（聯合利潤最大化、納許議價、雙邊
  報童臨界比率）、**`physics/`** 結構經濟學的「信任場物理學」
  （信任作為度量張量、交易測地線、公告接觸結構、
  CIR＋跳躍的信任衰減 SDE——屬建模類比，並已如此標示）、**`integration/`**
  適用於資本主義／零售業、可遞迴組合的垂直整合模型
  （隔間、工程艙、試驗隔間、牛頓中位數托盤，以及
  系統成熟度評分）、**`grading/`** 成熟度電路分級器，
  將每個模組以及任何一組勝出／落敗／分級者／絕對模組評為 A–D，以及 **`output/`**
  可路由的輸出介面，可將模組輸出送往終端機、
  **CommonRails** 格式的介面，或 socket／連接器（為原生
  `slnet_write` 加上框架）。每個類別都帶有 Metadata-Class 記錄；五個可執行的
  示範驗證這些主題，並由隨附的
  [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md)、
  [`MATH.md`](lib/advantage/MATH.md) 與
  [`PHYSICS.md`](lib/advantage/PHYSICS.md) 加以說明。位於 [`advantage/`](advantage/README.md) 之下的原生 C/C++
  使用端（C 核心
  `advantage_value`／`advantage_mean`、XML 知識庫、安全的
  HTTPS/SFTP/SSH 取得＋金鑰庫＋DH/ECDH 傳輸程式庫）將此
  模型連結至實際的
  [交易範例](advantage/examples/README.md)——來自
  假定互聯網來源的正向交易、受眾關聯性序列（交易者、
  消費者、協助者、療癒者），以及依 A–D 排名的八個模型（D＝最多 Cement，
  A＝最多 Brand）。
- **迴圈計時註解——`@loopDelay` / `@loopBudget`（另有內建函數 `sleep(ms)`）。**
  這是 SLeeLa 中首批**陳述式層級的行為性**註解：
  寫在 `for` / `while` / `do` 迴圈之前的獨立一行，會在迴圈執行中插入
  真實的暫停。`@loopDelay(ms = 250)` /
  `@loopDelay(seconds = 2)` 在每次迭代睡眠固定時間（單寫
  `@loopDelay(250)` 預設以毫秒計）；`@loopBudget(seconds = 10,
  iterations = 1000)` 將已知的迭代次數平均分配到總
  實際時間預算內。兩者都會降階為新的僅可附加（append-only）運算碼 **`OP_SLEEP`**
  （ISA 157），該運算碼也支撐可在任何地方呼叫的通用內建函數 **`sleep(<ms>)`**；
  暫停以可攜方式實作（`nanosleep` / `Sleep`）。規範見
  [`SLEELA.syntax` §32](SLEELA.syntax)；API 參考見
  [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md)；
  可執行範例見
  [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela)。
  一份工作階段新增項目索引（包含本項、原生 `OP_BIGMUL`
  大整數乘法，以及獨立的 **[local-generator](local-generator/README.md)**
  與 **[pi-generator](pi-generator/README.md)** 工具）位於
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md)。
  三個產生的 API 介面
  （[`sleeladoc-api/`](sleeladoc-api/README.md)、
  [`docs/api/`](docs/api/README.md)，以及不可變的
  [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html)
  快照）已透過 `make api-docs` 重新建置，並在其 `vm` 套件頁面中新增
  `SLISASleep` / `SLISABigMul` 指令條目。
- **從命令列使用 SleelaDoc——`sleela docs`。** 現在可直接從終端機使用 SleelaDoc。
  `sleela docs` 列出所有可記載的主題；
  `sleela docs crypto` 顯示一個套件；`sleela docs stringbuilder` 解析
  一個類別（不分大小寫，並可藉由 `SL` 標準程式庫前綴 →
  `SLStringBuilder`），並印出內容豐富的頁面——**定義、設計目的、
  關係、結構、使用者 clays，以及五階精熟度階梯**——若輸入接近但不完全相符，則附上排序過的「你是不是要找」結果。
  作者只要加入 SleelaDoc `@` 標籤（`@intent`、`@thesis`、`@axiom`、`@param`、
  `@see`、`@mastery`），即可充實任何頁面。`sleela docs --generate [dir]` 會寫出純文字語料庫
  （每個類別一頁，外加 `TOPICS.index`），CLI 之後可離線讀取；
  **`make sleeladoc`** 會將該語料庫**連同對應的 `.sleela` 原始碼**
  打包成 `sleeladoc/build/sleeladoc.jar`。對於僅有門面（facade）的主題（例如
  `psychiatry`、`politico`），`sleela docs --facade <topic>` 會在執行時搜尋
  檔案系統，並列出實際的**資料夾（然後是檔案，作為自然動作）**，
  並受使用者可覆寫的上限約束：**6 MB / 6000 個檔案 / 6
  分鐘**。`lib/sleeladoc` 各單元維持在目前版次
  **`#sleela 3.0`**。請參閱 [`man/man1/sleela.1`](man/man1/sleela.1) 與
  [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md)。
- **SleelaDoc——類似 Javadoc 的文件解決方案，並產生 HTML
  網站。** **SleelaDoc** 是 SLeeLa 的原始碼內文件註解慣例
  （以 `@doc` / `@page` 標示哨兵的角括號註解；請見
  [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md)），而
  [`lib/sleeladoc`](lib/sleeladoc/README.md) 是將 `.sleela` 單元
  轉譯為 Markdown 或 HTML API 頁面的**擷取器**——如同 `javadoc` 轉譯 Java，
  記載的是*意圖*。針對
  整個 `/lib` 的完整、可瀏覽 **HTML 文件網站**，由 `tools/generate-sleeladoc-api.py`
  產生到 [`/sleeladoc-api`](sleeladoc-api/README.md)
  （開啟 `sleeladoc-api/index.html`；採簡潔的
  藍／淺藍／淺灰／白配色，具有套件 → 類別 → 成員的
  層級，以及為 *Student 2* 讀者撰寫的詞彙表）。編譯器也會
  在編譯過程中附帶輸出作者標記的文件成品（`--wiki`
  MediaWiki 頁面＋一份由來源衍生的孿生原始碼），記載於
  [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md)。可執行範例：
  [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela)。
- **XML 文件物件模型（`lib/xml`）。** SLeeLa 端的 XML DOM——一份文件
  擁有由 element／text／comment／CDATA／PI 節點組成的樹，具備有序的子節點與
  具名屬性，另有序列化器（精簡／美化，遵循 XML 1.0 §2.4 跳脫規則）、
  字元格剖析器，以及 `SLXmlAspect` 品質登錄。節點相等性是
  三階的信任關係（同一性 → 同一主體 → 結構），具反身性、
  對稱性且具決定性。請參閱 [`lib/xml/README.md`](lib/xml/README.md) 與
  可執行的 [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela)。
- **寬幅帶號整數基本類型（`long`、`int64`、`int128`）。** `int`、`long`
  與 `int64` 都是 64 位元帶號整數，類型檢查與執行行為完全相同；
  `int128` 可作為整數類型接受（目前以 64 位元為底層，真正的
  128 位元已記載為後續工作）。以 2 為底的帶號系列 `int32`..`int16777216`
  （語法 2.7）與對應的無號系列 `uint32`..`uint16777216`
  （語法 2.8）進一步擴充此項。請參閱 [`SLEELA.syntax`](SLEELA.syntax) §21、§24、§25。
- **`@mix` 後繼欄位（語法 2.9）。** 當每個重複的欄位都帶有 `@mix` 時，類別可宣告多個
  **同名**的實例欄位；語言會將該群組理解為後繼（依索引排列的陣列／清單），
  預設為由上而下的 `1,2,3,…`，或以 `@mix(1,2,3,...)` 手動指定，其索引
  數量必須與欄位數量相符。請參閱 [`SLEELA.syntax`](SLEELA.syntax) §28。
- **文件標記——自然程序（Natural Process），統稱 Preonems
  （`author`/`authorial`、`material`、`memory`、`determine`、`object`、`field`）。**
  這些單獨使用的文件標記詞彙統稱為 **Preonems**
  （單一標記為一個 *Preonem*；整組則為 *Preonems*——請見
  [`SLEELA.syntax`](SLEELA.syntax) §26.1）。它們是一等公民、僅限宣告的
  文件標記，依製作順序指明文件*是什麼*：
  `author`/`authorial`（語法 2.4；由專業委員會處理——唯一具有行為效果的成員）、`material;`（已確立的來源）與 `memory;`
  （語法 2.5）、`determine;`（語法 2.6；開放讓醫師、
  醫生或幽靈進一步處理、最終由合法之手完成的文件），
  以及最新的兩個：**`object;`**（在存在目錄中已確立的事物——**於語法 2.8 具象化、賦予具體地位**）與
  **`field;`**（物件於其中立足並移動的關係基礎）——
  兩者皆自語法 2.6 起受版本管制。每個標記都是單獨使用、依上下文判定，並由
  前端依版本控管，不影響剖析、類型檢查或程式碼產生；在標記位置之外，每個詞彙
  仍是普通識別名稱（因此 `field` 絕不會干擾
  類別成員或 `.field` 存取）。請參閱 [`SLEELA.syntax`](SLEELA.syntax)
  §18–§20、§23 與 §26。
- **Nordshrift 於語法 3.0 具備完整程式庫涵蓋範圍。** Nordshrift 透過與編譯器相同的遞迴 `/lib` 索引進行解析，
  涵蓋每個套件與 `.sleela` 單元；`nordshrift check` 會回報整個樹狀結構的統計數量。
- **Recursion stanchion。** [`recursion/stanchion/`](recursion/stanchion/README.md) 目錄現在包含針對七項已記載基本類型契約的專用 C 與 C++ 參考原型，以及通用、應用、科學與教學範例。這些是參考原始碼；在視為已驗證之前，請先編譯並測試。

- **角括號註解（`<` / `<<`）。** 一種新的、面向未來的註解風格——
  `< 行 >` 與 `<< 區塊 >>`——由 SLeeLa 前端進行詞法分析，並記載於
  [`SLEELA.syntax`](SLEELA.syntax) §1.1。
- **進位制整數常值。** `int` 常值現在接受十六進位（`0x`）、
  二進位（`0b`）與八進位（`0o`）形式，並可使用 `_` 數字分隔符號，
  適用於 SLeeLa 原始碼語言（[`SLEELA.syntax`](SLEELA.syntax) §1.4）與
  [`.xlt` CPU 模型格式](lib/xlt/README.md)。
- **整個程式庫採用 Metadata Class 標準。** 描述性中繼資料
  元模式（來源、合法性、管轄範圍）現在由
  `/lib` 之下每個含有類別的 `.sleela` 檔案承載；請見
  [`SLEELA.syntax`](SLEELA.syntax) §15 與
  [教學課程](tutorial/13-metadata-class.md)。
- **更豐富的 text / String 家族。** `lib/text` 新增真正的語言內建輔助函數、
  提供真正 `charAt`／`substring`／`indexOf`／`reverse`／
  欄位對齊的字元格檢視，以及真正的位置式範本引擎——請見
  [text 與 String 教學](tutorial/14-text-and-strings.md)與可執行的
  [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela)。
- **Nordshrift 工作表中的一等公民 `String`。** `.sst` 工作表可宣告具類型的
  `strings:` 區塊，該區塊會納入建置的元件清單；
  請見 [Nordshrift Sheet Reference](wiki/Nordshrift-Sheet-Reference.md)。
- **CPU 模型同時以運算碼與函數呼叫等價方式執行。** 每個
  `SL<ARCH>CPU` 都繼承這項基礎：既透過其擷取—解碼—執行迴圈執行原生指令串流，
  *也*在共用的 VM 基底上，以等價呼叫執行假定的
  其他程序之程式碼——請見
  [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5。

## III. 建置與測試

**先決條件：** C/C++ 工具鏈（GCC 或 Clang）與 GNU Make；用於
清單／流程工具的 Python 3；在 Linux 上，顯示產品所需的 X11/Xrandr 開發套件
（模組的 `make deps` 會回報／安裝其所需項目）。
`impl/` 建置採用以受信任 SHA-256 清單為依據的失敗即封閉（fail-closed）機制，
根目錄的 `Makefile` 會自動指向該清單。

從乾淨的檢出（checkout）開始，最快的入門方式：

```sh
make                                  # build everything from the repo root
```

或者只建置並測試原生核心：

```sh
make -C impl all                      # build the core compiler/runtime only
bash ./test-suites/run-all.sh --all   # run the full test suite
```

儲存庫根目錄的 `make` 會建置所有產品（核心、VM、編解碼器、churn、
防毒軟件……），並在最後建置**登錄庫（registry）**並具體化其
基準資料表——因此建置完成的目錄樹開箱即附有可供審閱的登錄庫。
主要目標如下：

```sh
make                     # build everything (alias of `make all`; registry runs last)
make all                 # the same full build, named explicitly
sudo make install        # install commands + runtime support (see §IV)
make clean               # remove build outputs from every dispatched build system
make help                # list every target the root Makefile provides
```

其他實用目標：

```sh
make registry            # just the registry: build + materialize the table
./build-registry.sh      # same thing, invoked directly
make tests               # build and run the test suite
make ballum              # build the Ballum display library for this OS (see ballum/)
bin/install-modules      # paced install/build of each module over ~10-20 min (see bin/)
```

`registry` 目標會執行 [`build-registry.sh`](build-registry.sh)：它建置
登錄庫核心，將基準資料表植入可攜的文字儲存庫
`registry/build/sleela.sleelareg`，並匯出 MySQL 資料表腳本
`registry/build/registry.sql`（透過 `sleela registry export`）。可使用
`sleela registry` 或 `registry/build/sleela-registry review` 檢閱。請參閱
[登錄庫指南](registry/README.md)與 [Registry 3.0 架構](registry/REGISTRY-3.0-ARCHITECTURE.md)。

請參閱建置與測試指南，了解先決條件及各平台的特定步驟。單一元件建置成功，並不代表所有程式庫或平台皆已達到可發佈的程度。

## IV. 安裝

建置原生產品後，安裝命令與執行環境支援檔案：

```sh
sudo make install
make install PREFIX="$HOME/.local"       # user-local install, no sudo
make install DESTDIR="$PWD/package-root" # staged package install
make install-check
```

傳統安裝程式預設使用 `/usr/local/bin`、`/usr/local/lib/sleela`
與 `/usr/local/share/sleela`。它會保留既有的啟動設定，並拒絕覆寫未受追蹤的既有命令。解除安裝
只會移除安裝程式所記錄的檔案，並保留設定與
未受追蹤的執行環境資料。

若要針對主套件進行專注、引導式的部署——包含 `sleela`、
`sleelvac` 與 `nordshrift` 執行檔、標準程式庫、協定與
支援模組——請使用 clean 安裝程式：

```sh
make clean-install       # guided install of the main package only
```

它只做一個主要決定（安裝主套件），接著選擇範圍，
成功後可選擇性地挑選精選的強化模組。關於
互動式部署流程與所有安裝選項，請參閱
[快速安全安裝](install/README.md)。

## V. 語言與執行環境
權威語法為 [`SLEELA.syntax`](SLEELA.syntax)。目前儲存庫記載的語法為 **3.0**；編譯器／工具版本與原始碼語法版本是不同的概念。3.0 版面新增 **`pure` 與 `gasoline` 文件標記**（§30）——這是一對單獨使用、依上下文判定、僅限宣告的 Preonems，意義幾近相同，各自以其誠實性標示文件的地位。語法 3.0 將 1.0–2.9 的語言面貌整併為目前的規範版次，並保留所有各功能專屬的最低版本管制。先前的 2.9 版面新增 **`@mix` 後繼欄位**（§28）——當每個欄位都帶有 `@mix` 時，類別可宣告多個同名的實例欄位，並視為一個有索引的群組（預設由上而下 `1,2,3,…`，或手動指定 `@mix(1,2,3,...)`，其索引數量必須與群組相符）。它延續以 2 為底的帶號／無號寬幅整數系列 `int32`..`int16777216`（§24）與 `uint32`..`uint16777216`（§25），建立在先前的 `long`/`int64`/`int128`（§21）之上；**關鍵字支援文件**（§22）；以及 **Preonems**——僅限宣告的文件標記 `author`/`authorial`（§18）、`material`/`memory`（§19–§20）、`determine`（§23）、`object`/`field`（§26）、`pure`/`gasoline`（§30），以及 `artifact`（§35，依時間與空間分級的立法級地位），統稱為 Preonems（§26.1）。建構子多載解析，以及 `this(...)`／基底建構子委派，明確不在已驗證的語法 1.8 契約範圍內。在依賴某項功能之前，請先查閱語法與實作說明。SleelaDoc 的九個原始碼單元現在宣告 `#sleela 3.0`；共用的編譯器閘門接受 1.0–3.0 的原始碼語法。新的 API 建置由 `make api-docs` 產生。

本儲存庫包含原生 C/C++ 實作、SLVM 產生參考、標準程式庫模組、測試、範例與平台整合。各模組的實作與驗證狀態不盡相同；請查閱其規格與目前的 CI 結果，而不要假設一律完整。

## VI. 工程原則
- **以證據取代主張：** 區分已實作、已測試、實驗性與規劃中的行為。
- **每個邊界一份契約：** 記載語法、API、檔案格式、平台假設與失敗行為。
- **設計即可攜：** 隔離平台專屬後端，並在原生執行器上測試。
- **安全失敗：** 測試格式錯誤的輸入、資源限制、程序失敗與清理路徑。
- **可重現的發佈：** 乾淨地重新建置，並發布成品雜湊值與發佈說明。

## VII. 防毒軟件建置與完整性

Sleelavirin 防毒套件包含原生 SHA-256 掃描器、選用的
ClamAV 介接器、原始碼／索引完整性清單，以及平台套件校驗和。
請從儲存庫根目錄建置並測試此套件：

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

完整性清單產生於 `antivirus/build/` 之下；它涵蓋受追蹤的
原始碼與文件輸入，以及受保護的特徵碼索引檔案，
但排除產生的輸出與可變動的 ClamAV 快取。ClamAV 資料庫
快照各有自己的清單。發佈 ZIP 包含 `INTEGRITY-MANIFEST.json`、
`SHA256SUMS` 與 `.sha256` 附屬檔案。請將下載封存檔的雜湊值，與獨立取得的
可信數值核對：與封存檔放在一起的校驗和
並不是發布者簽章。請參閱 [Sleelavirin 指南](antivirus/README.md)、
[安全檢查清單](antivirus/SECURITY.md)與
[防毒二進位檔建置工作流程](.github/workflows/antivirus-binaries.yml)。

在相關 CI 工作於確切的發佈提交上通過，且產生的成品
經過檢查之前，該發佈不會被視為已驗證。

## VIII. SLeeLa 編輯器佈景主題——Vim 與 Emacs

SLeeLa 內含一組協調一致、適用於 **Vim** 與 **Emacs** 的專業深色配色方案，另附原生 C/C++ 色盤工具。原有的 **American Professional** 方案仍為預設。另有十種搭配色盤，靈感分別來自不同的文化與機構設計風格；這些是詮釋性的設計色盤，並不代表任何國家、文化、法院或機構的官方形象。

### 十一種配色方案及其色彩

| 方案 | 背景 | 前景 | 關鍵字 | 類型 | 字串 | 註解 |
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

#### 各國國旗色彩（來源色盤）

希臘與土耳其的配色方案源自這兩個國家**官方國旗色彩**，此處列出其確切的公布十六進位色碼。上方各方案的背景／強調色，是這些來源色彩經調暗、適合編輯器使用的詮釋版本——下表即為「國家公開色彩」的參考。

| 國家 | 色彩 | 色碼 | 依據與意涵 |
|---|---|---|---|
| **希臘** | 藍 | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | 國旗藍（第 851/1978 號法律）。象徵天空與海洋；在紋章學上稱為 *azure*。 |
| **希臘** | 白 | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | 國旗白。象徵獨立鬥爭的純潔；十字代表東正教信仰。 |
| **土耳其（Türkiye）** | 紅（*al bayrak*） | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | 國旗紅（土耳其國旗法）。旗面底色；依傳統代表烈士的鮮血與鄂圖曼主權。 |
| **土耳其（Türkiye）** | 白 | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | 國旗白。新月與星辰——象徵和平、純潔，以及突厥／伊斯蘭傳統的標誌。 |

這些方案將各編輯器的前景色定為國家的**白色**，並將關鍵字、游標、選取範圍與狀態列定為確切的國家**藍色**（`#0D5EAF`）或**紅色**（`#E30A17`），因此最重要的色彩就是國家色彩本身。這些是詮釋性的設計色盤；國旗色彩參考僅為說明來源出處，並非任何一國的官方形象。色彩意涵依循希臘共和國外交部對國家象徵的說明，以及土耳其國旗法的傳統。


### 色票表

下方色盤儲存格使用 **SVG 色票**，因此 GitHub 會顯示真實色彩，而非空白的 HTML 色彩儲存格。每條色帶依序顯示六種顏色：背景、前景、關鍵字、類型、字串與註解。確切的十六進位色碼列於各色票旁，方案順序與上方參考表相同。

| 方案 | 呈現的色盤顏色 | 色碼（背景 · 前景 · 關鍵字 · 類型 · 字串 · 註解） |
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


另有額外的強調色用於函數、常數、選取範圍、診斷訊息與前置處理器指令。俄羅斯與斯拉夫色盤刻意採用中等明度的石板灰、藍、鼠尾草綠、寶石紅與古董金，呈現較為沉穩的專業外觀。

### 安裝 Vim 配色方案

在儲存庫根目錄執行：

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

將下列內容加入 `~/.vimrc`，即可啟用 SLeeLa 語法醒目提示並選擇配色方案：

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

請從下列配色方案名稱中恰好選擇一個：

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

例如，使用 `colorscheme sleela-russian-slavic` 可套用中等明度的俄羅斯與斯拉夫色盤，使用 `colorscheme sleela-greek` / `colorscheme sleela-turkish` 則可套用希臘與 Al Bayrak 色盤。請重新啟動 Vim，或以 `:source $MYVIMRC` 重新載入設定。

### 安裝 Emacs 配色方案

請建立本機佈景主題目錄，並從儲存庫根目錄複製模式與佈景主題檔案：

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

將下列內容加入 `~/.emacs` 或 `~/.emacs.d/init.el`：

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

將 `sleela-american` 替換為 `sleela-eurasia`、`sleela-russian-slavic`、`sleela-silk-road`、`sleela-east-asian`、`sleela-greek`、`sleela-turkish`、`sleela-kiro` 或 `sleela-sleela`，即可選擇其他方案。如果 Emacs 詢問是否信任該佈景主題，請在接受之前先檢查本機的佈景主題檔案。

### 預覽原生 C/C++ 色盤

這個小型原生色盤工具可以列出各方案、印出其終端機色彩，並查詢單一顏色：

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

為保持向下相容，未指定 `--theme` 時，`--palette` 與 `--hex COLOR` 會選用 American Professional。建置與測試結果應在目標系統及 CI 上確認；命令有文件記載，本身並不是它已通過的證據。

完整的色盤說明與工具細節，請參閱[編輯器整合指南](linux-editors/README.md)。

## IX. 參與貢獻
1. 閱讀文件索引與測試政策。
2. 提交範圍明確的變更，並附上正向與負向測試。
3. 先執行針對性測試，再執行完整測試套件。
4. 回報作業系統／工具鏈細節；請勿將略過的檢查描述為通過。

SLeeLa 由 **MEARVK LLC** 開發。歷史版本的登陸頁面資料保存在 [`docs/README-LEGACY.md`](docs/README-LEGACY.md)。

## X. AE6E66——Round Public 電子郵件稅賦期望

對於 AE6E66 倡議，本項目所陳述的期望為：**每封 Round Public 電子郵件 $300,000,000.00 USD**，作為與每封符合條件的 Round Public 電子郵件相關的擬議稅賦金額。

本聲明僅記錄項目的期望。它**不是**法定的稅務核定、政府命令、法院判決，或獨立成立的納稅義務。任何實際的徵收，均須有適用的法律權限、管轄權、定義、程序，以及有權限的政府機關所作的認定。


## XI. 字母表與字元集執行環境

SLeeLa 的字母表文件與原生字元執行環境維護於 alphabet/sleela/ 與 lib/alphabet/native/。原生 C++17 測試涵蓋嚴格的 UTF-8 純量解碼／編碼、C<n> 容量簽章，以及有界的字元集成員判定：

    make -C lib/alphabet/native test

實驗性的 UTF-4088 目錄及其 SLeeLa 查詢轉接器，記載於 utf-4088/SLeeLa-ALPHABET-INTEGRATION.md。UTF-4088 的 ID 是獨立的命名空間，並非 Unicode 純量值。編譯器與 SLVM 繫結工作明確地與這些原生基礎分開追蹤。

## XII. AutoCAD Creator™

[AutoCAD Creator™](autocad/creator/README.md) 是完全以 SLeeLa 撰寫的互動式 2D CAD
應用程式。姊妹產品 AutoCAD Renderer 一次就將平面圖轉成圖面，而 Creator 讓你即時**建立與修改** 2D
AutoCAD 文件——新增牆壁、房間、尺寸標註與註記，再移動、
重新命名標籤、重新指定圖層、刪除，並復原你所放置的內容——並將結果**儲存**為
真正的 AutoCAD DXF 圖面（R12 文字格式，可在 AutoCAD / LibreCAD 中開啟）。

其圖形介面建構於 **SleelaUI™** 之上，這是 SLeeLa 自有的跨平台 UI
工具組（位於 [`lib/user-interface`](lib/user-interface) 之下）——並非 JavaFX——可在 X11 / Cocoa / Win32 上
開啟真正的原生視窗。其設計遵循一項規則：
**「SleelaUI 負責呈現；SLeeLa 負責決定」**：GUI 從不計算幾何圖形，也不寫入
DXF。每項工具都會向具權威性的
[`SLCadCreator`](lib/autocad/SLCadCreator.sleela) 發出建立／修改／刪除／復原／儲存的意圖，由它掌管
可編輯的文件與可逆的指令歷程，而畫布只負責預覽
Creator 所持有的幾何圖形。

可重複使用的編輯引擎位於 [`lib/autocad`](lib/autocad)——
[`SLCadEntity`](lib/autocad/SLCadEntity.sleela)、
[`SLCadDocument`](lib/autocad/SLCadDocument.sleela)、
[`SLCadCommand`](lib/autocad/SLCadCommand.sleela) 與
[`SLCadCreator`](lib/autocad/SLCadCreator.sleela)——並可在無顯示環境下完整端到端執行。
GUI 外殼與示範位於 [`autocad/creator`](autocad/creator)：

```sh
make creator          # from autocad/: run the Creator demo -> out/altered.dxf
```

在有顯示器的環境中，這會開啟即時編輯視窗；在無顯示器的主機上，它會在記憶體中建立
圖面，印出報告與 DXF，然後正常結束。


## XIII. UTF-4088 輸入對映排序

專用的 UTF-4088 目錄在 [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md) 中定義其空間輸入對映走訪與抽象壓力迭代。National Lien 主題索引政策位於 [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md)。對映座標、壓力層級（1–50）、永久目錄 ID 與字母排名是彼此獨立的欄位；不應默默以其中之一取代另一個。壓力是演算法的迭代層級，而非物理量。


## XIV. 近期與擴充的套件索引

這份附加索引重點列出近期擴充的套件、領域程式庫與
原始碼集合。各條目依套件名稱或主題按字母順序分組。
說明僅指出儲存庫中的位置；實作與驗證
狀態應在各套件自己的文件與測試中確認。

### A–C

- **Advantage**——`lib/advantage/` 與 `advantage/`：針對某一模型對一筆獨特
  股利進行 advantaging，採分層的 Sequitur onion，並有五個
  主題子資料夾——`math/`（聯合利潤最大化、納許議價、
  雙邊報童模型）、`physics/`（信任場度量／測地線／接觸／SDE，
  並已標示為建模類比）、`integration/`（垂直整合
  隔間與成熟度評分）、`grading/`（針對模組與集合的 A–D 成熟度
  電路分級器），以及 `output/`（終端機／CommonRails／
  socket 輸出）。原生 C/C++ 使用端（`advantage/`）將該模型連結至
  實際的交易範例。請參閱 [`lib/advantage/README.md`](lib/advantage/README.md)
  與 [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md)。
- **字母表與 UTF-4088**——`alphabet/sleela/`、`lib/alphabet/native/` 與
  `utf-4088/`：字母表目錄、原生字元集支援、輸入對映
  排序，以及 UTF-4088 整合文件。
- **AutoCAD Creator**——`autocad/creator/` 與 `lib/autocad/`：互動式
  2D 繪圖工作流程，以及可重複使用的 CAD 文件／編輯元件。
- **Ballum**——像素層級的顯示控制：逐像素設定／讀取、區域位塊傳送、
  整個顯示器的亮度／色溫／伽瑪、指標／鍵盤輸入，以及
  **全螢幕畫面緩衝區介面**，提供完整、獨佔、快速的逐像素控制
  （在 Linux/X11 上使用 override-redirect 視窗＋單次 `XPutImage`/MIT-SHM 呈現）。
  原生核心位於 `ballum/`，SLeeLa 門面位於 `lib/ballum/`，VM
  橋接位於 `impl/` 之下；請參閱 [`ballum/README.md`](ballum/README.md) 與可執行的
  [`ballum/example/`](ballum/example/)。
- **CDMA**——`cdma/` 與 `lib/cdma/`：CDMA 相關技術資料與
  SLeeLa 原始碼程式庫工作。
- **Character**——`lib/character/`：角色設定檔與可重複使用的角色
  統計資料，包括傳記式設定檔範例。
- **Coorenagraph**——`lib/coorenagraph/`：倫理推理、道德詞彙、
  審議及相關概念；請參閱其套件 README 與原始碼檔案。

### D–G

- **治理原始碼模型**——[lib/governance](lib/governance/README.md) 為提案、匿名人工投票與彙總計票定義宣告式的 SLeeLa 原始碼形式。擬議的 @vote 註解與人工流程記載於 [registry/DEMOCRATIC-VOTING-SYNTAX.md](registry/DEMOCRATIC-VOTING-SYNTAX.md)；目前的編譯器將其視為無作用的註解中繼資料。

### H–I

- **HTTP 1.0–8.0**——`lib/http-1.0/` 至所有可用的
  版本化 HTTP 套件目錄：各版本專屬的協定規格與
  實作。有關其確切範圍與驗證狀態，請查閱各版本的文件。
- **International Criminal Court（國際刑事法院）**——`lib/international-criminal-court/` 與
  `INTERNATIONAL.CRIMINAL.COURTS.md`：相關的領域專屬原始碼與
  文件。

### O–S

- **OFDMA**——`ofdma/`：正交分頻多重存取
  技術資料與 C/C++ 開發資源。
- **Philosophy 與 Conversation Mask 模組**——[`philosophy/`](philosophy/README.md)
  包含原生 C/C++ 基礎，而 [`philosophy/module/`](philosophy/module/README.md)
  將其下一個模組層組織為三個明確的領域：
  [`data/`](philosophy/module/data/README.md)，用於具類型、註明日期的觀察、
  事件結束、半衰期設定、來源出處與同意；[`helper/`](philosophy/module/helper/README.md)，
  用於確定性驗證、關聯性衰減、相容的比較，以及
  經同意過濾的呈現；以及 [`psychiatry-friend/`](philosophy/module/psychiatry-friend/README.md)，
  提供支持性、非臨床的對話介面。面向 SLeeLa 的
  控制代碼與組合而成的 Tree 位於 [`lib/philosophy/`](lib/philosophy/README.md)，
  與較廣的 philosophy 套件並列。此模組採明確輸入與
  同意優先：它不會被動監控人或媒體、不會診斷、不會推斷
  私人意圖、不會評量人的價值，也不會建議變更藥物。其
  事件連結保留的是來源出處與導覽，而非獨立佐證。
- **Synchro**——原生執行環境／編譯器整合，以及封包分派與
  量測相關功能；請參閱相關的 `impl/` 原始碼與
  編譯器實作。
- **SleelaUI / 終端機 UI**——`lib/user-interface/` 與
  `sleela-terminal` 項目：SLeeLa UI 整合與以終端機為導向的
  互動元件。

### U–W

- **聯合國（United Nations）**——`united-nations/`：領域專屬的儲存庫資料。
- **Wi-Fi 4、Wi-Fi 5 與 Wi-Fi 6**——`wi-fi-4/`、`wi-fi-5/`、
  `wi-fi-6/` 及相關的 `lib/wifi/` 原始碼目錄：各世代專屬的
  無線網絡資料與設定資源。

### 套件索引維護

新增套件或實質擴充套件時，請將其條目依字母順序加入此處的
適當群組，連結至其標準目錄與 README，
並僅描述已簽入原始碼所支援的功能。請讓此
索引維持累加式：套件索引更新時，請勿移除或改寫現有的 README 各節。


---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh :: ⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓


bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc :: ⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉

美國民主與美國政策。

---

## XVI. 國家科學品牌形象

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>


## XVII. UTF-4088 百萬條目實驗性目錄

[UTF-4088 目錄產生器](utf-4088/tools/generate_catalogue.py)以確定性方式建置包含 1,048,576 個條目的實驗性符號目錄。文件記載的精簡格式說明於 [utf-4088/FORMAT.md](utf-4088/FORMAT.md)；產生與驗證契約位於 [utf-4088/GENERATION-RESULTS.md](utf-4088/GENERATION-RESULTS.md)。

請以 Python 3 執行產生器：

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

產生的二進位檔為 16,777,244 位元組，設計上維持在 20,000,000 位元組以下。單元測試與 GitHub Actions 工作流程會驗證條目數量、字形唯一性、大小與 SHA-256；成功執行後，會將二進位檔與摘要以 `utf4088-million-catalogue` 工作流程成品發布。產生的二進位檔刻意不納入 Git 歷史。UTF-4088 仍屬實驗性質，與 Unicode 並不相容。

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. 近期新增——圓周率常數、Modium/Registry 啟動、sleelactl 數學，以及 `@native`

本節記錄 2026-11 期間新增的功能，最新的群組在前。所連結的原始碼檔案及其驗證狀態具有權威性；審查中的變更在進入 `master` 之前，不會被描述為已合併。

### 一億位數的圓周率目錄常數

儲存庫在 [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits) 提供一個經過驗證的**一億位數** π 值（格式良好的十進位小數，`3.14159…`），另有一個位於 [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits) 的 57,344 位數值。兩者均經獨立驗證為真正的 π——請見 [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md)，其中記載了各項檢查（教科書前綴、小數位置 762 處的費曼點、著名的第一百萬位數字、與 `lib/math/SLPiReference.sleela` 一致，以及整個檔案的數字頻率分析），並附有一鍵重現的腳本。

### Modium™ / Registry™ 於子系統啟動時載入圓周率常數

在專用的子系統進入點——`sleela registry` 與 `sleela modium`（以及安裝程式首次執行）上，**而非**每次 `sleela run`——Modium 與 Registry 子系統會載入大型 π 目錄常數。在互動式終端機上，它們會印出短暫的紅色 `Loading Modium™` / `Loading Registry™` 行，並在正常輸出開始前就地擦除；此效果嚴格限於 TTY，因此經管線傳遞的輸出與測試輸出維持逐位元組不變。定位器（而非位元組內容）會以 registry `Math` 鍵的預設值 `piReferenceLargePath` 與 `piReferenceLargeDigits` 植入，同時見於 SLeeLa 模型（[`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)）與原生植入器（[`registry/sleela_registry.c`](registry/sleela_registry.c)）；`lib/math/SLPiReference.sleela` 對外提供 `largePath()` / `largeDigits()`。π 常數會由標準 `make` 安裝程式**以及**引導式「beautiful」安裝程式，安裝到作業系統的 SLeeLa 位置（`<data>/pi/`）。詳情：[`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md)。

### `sleelactl`——beautiful 安裝程式與以原生為後盾的 `math` 主題

- 引導式（「beautiful」）安裝程式現在會安裝 `sleelactl`（SLeeLa 控制平面）及其模組登錄庫，與 `make` 安裝程式一致。請參閱 [`bin/SLEELACTL.md`](bin/SLEELACTL.md)。
- `sleelactl` 新增了一種 **`subject`** 性質，用於編譯進執行環境的原生 C/C++ 主題程式庫，並註冊了第一個：**`math`**。`sleelactl run math` 會透過該主題的參考示範，端到端演練原生 `__NativeMath` 後端（`impl/subjects/math/math_native.cpp`）；`sleelactl info math` 則說明如何存取它（`import math;`）。subject 性質僅提供 `run info`——刻意不設 `check` 動詞，因為 `sleela check` 不會載入以 SHEET 註冊的原生主題。

### `@native`、Native Drive 與多語言執行

新的 **`@native`** 方法註解會要求對熱點方法進行預先（ahead-of-time）原生編譯。新的 `sleela aot` 命令會找出 `@native` 方法，驗證每個方法都在受支援的*可卸載子集*之內，並產生 C/C++——對於任何尚無法轉譯的構造，**以失敗即封閉的方式處理**（拒絕並附上明確原因），因此絕不會產生轉譯到一半的方法。

```sh
sleela aot --list <file.sleela>    # per-method verdict: ok / REFUSED (reason)
sleela aot --emit <file.sleela>    # emit C/C++ (stdout) for an in-subset program
```

`@native` 仍然只是對 AOT 處理的請求，不受支援的構造會在輸出之前就被拒絕。[Native Drive](impl/native-drive/README.md) 支援透過主機編譯器（包括 MSVC）建置多單元 C/C++ 可執行檔與共用程式庫、處理受支援的 SLeeLa AOT 輸出、依原生載入器 ABI 檢視符號、不經 shell 的程序執行、stdin/stdout/stderr 處理、多階段管線，以及將 XClass 委派給正式環境載入器。`sleelactl` 對外提供其建置／執行命令，並透過 `inspect native-drive` 轉送 XClass 檢視，以及明確的 systemd 單元轉譯／安裝。VM 擁有一個有界的原生模組控制代碼表，須有強制性的主機授權，且控制代碼會做世代檢查。語法 3.0 現在新增 `nativeOpen`、`nativeCall` 與 `nativeClose`：僅可附加的位元組碼分派會跨越一個獨立版本化的 null/bool/int64/double/UTF-8 ABI，而由 VM 擁有的陣列、結構與其他複合值則留在直譯器內，並可使用同名位元組碼後援。外掛程式可選擇性地接收一份版本化的主機回呼表，透過巢狀 VM 執行來呼叫確切的 SLeeLa 函數，並具備暫停堆疊的 GC 根集、明確的主機回呼名稱允許清單，且遞迴的原生 API 呼叫會被拒絕。原生程式碼仍是受信任的行程內程式碼，而不是沙箱。原始碼的程序政策註解有版本化的結構描述，但尚未被剖析／強制執行。詳情、安全邊界與七條工作流的帳冊：[Native Drive](impl/native-drive/README.md) 與 [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md)。


## XIX. 國際 README 版本

SLeeLa 為其國際版本涵蓋的 50 個市場，各提供一個國家專屬的 README 入口。每個國家有一個名為 `README.<CC>.md` 的檔案；若涉及多種語言，則集中於該國家的檔案之中。

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

**翻譯政策：** 這些檔案最初是在地化的國家入口。其簡介與語言標題已在地化，而完整的技術參考仍保留於英文規範版 [README.md](README.md)。除非每一節都已翻譯、技術識別名稱與連結都已檢查，且該版本已通過語言審閱，否則不得將國家檔案描述為完整翻譯。第一階段的語言涵蓋範圍僅具代表性，並不聲稱每一種憲法承認的、區域性的、原住民的或手語語言都已完整翻譯。

## 附錄——保留的香港地區指引

以下內容保留自本檔案先前較簡短的香港版指南，且與英文規範版 README.md 並無牴觸。

### 香港繁體中文用語

本版本以香港慣用的繁體中文書面語為主，例如「軟件」、「編譯器」、「執行環境」、「虛擬機」、「檔案」、「預設」、「伺服器」、「網絡」、「支援」、「程式庫」、「程式碼」及「驗證」。技術識別名稱、命令、檔名、API 名稱及原始碼必須維持原樣，以便讀者直接對照原始碼和規範。語言和地區用語應清楚標示，不應假設香港、台灣及其他繁體中文社群的用語完全相同。

### 國際文件維護與在地化

- Country-specific README 是各地區用戶的文件入口，不取代語言規範或模組 API 文件。英文來源更新後，應逐項檢查翻譯是否過時。
- 每次更新後，請檢查 Markdown 連結、標題層級、程式碼圍欄及範例。英文規範版文件更新時，應比對相關章節並同步翻譯，而非只修改簡介。
- 未經語言或技術審校的內容應明確標示。

### English

SLeeLa is a native systems programming language, compiler, runtime, and virtual-machine ecosystem. Technical identifiers, source code, commands, filenames, and API names remain unchanged in this edition.

**翻譯狀態：** 第 I–XIX 節已翻譯為香港繁體中文。技術識別名稱、命令、程式碼範例、檔名、連結及位址均予保留。建議由母語人士進行最終語言審閱。
