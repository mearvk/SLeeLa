<img align="right" src="../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# SLeeLa 3.0 — 日本 (JP)
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>ネイティブシステム言語、コンパイラ、ランタイム、仮想マシンのエコシステム。</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="../../docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **国別国際版（日本語）。** このファイルは日本語版の国別エディションで、READMEのセクションI～XIXを日本語で提供します。技術識別子、コマンド、コード、ファイル名、リンクは原文のまま保持しています。

**The Now. The Ingredient.** 言語は構文だけではありません。ソフトウェアを実用可能にするコンパイラ、ランタイム、ライブラリ、ツール、プラットフォーム契約も含みます。

**SLeeLaの対象領域。** SLeeLaの活動は **Math**、**Object**、**Science**、**Reach** の四つの側面から成ります。Mathは計算可能なもの、Objectは存在するもののカタログに位置づけられた確定した対象、Scienceは真理に対する体系的な探究、Reachはシステムの働きを他のシステムや人々へ広げることを表します。**国家的利益は維持されます。** いずれの側面も国家的利益を排除しません。国家の利益に関係する事項は、その関係を保持し、たとえば各クラスのMetadataにある `metaOrigin` / `metaNationalSchedule` に正確に記録します。

## I. はじめに

- [ネイティブ実装のビルド](../../impl/README.md)
- [仮想マシンとModium — 実践ユーザーガイド](../../virtual-machine/USER-GUIDE.md)（`sleela vm`、`sleela modium`）
- [SLeeLaソース内のCPUスタック](../../lib/cpu/CPU.md)：論理ゲートからALU、コア、OS、プログラムまでを扱う機械構成、130以上のCPUプロファイル、調整可能な[電圧・可変クロック・高度なタイミング設定](../../lib/cpu/TIMING.md)、および[仕様準拠実行とオーバークロック](../../lib/cpu/components/README.md)。
- [ドキュメント索引](../../docs/README.md)
- [CSPMLデータソースおよびSalt & Salary調査リファレンス](../../CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md)：米国の地域、社会経済、労働力、教育、公立学校に関する外部データの出典と統合指針。
- [生成済みAPIドキュメント](../../docs/api/README.md)：現行HTMLリファレンス、バージョン別API版、再生成手順。
- [APIカタログ](../../api/README.md)
- [計算比較標準](../../comparison/README.md)：SLeeLa VM、Java/JVM、ネイティブC/C++の型、メモリ、ABI、適合性に関する標準。
- [再帰スタンション — CおよびC++プロトタイプ](../../recursion/stanchion/README.md)：Boolean、Byte、Character、Integer、Long、Float、Doubleの各プリミティブ契約に特化した例と、汎用・応用・科学・教育向けの再帰パターン。
- [プラットフォーム対応および検証マトリクス](../../docs/PLATFORM-SUPPORT.md)
- [ブランドガイド](../../docs/BRAND-GUIDE.md)
- [製品コンセプトとアセット](../../merchandise/README.md)
- [NitroWebExpressロゴ](../../images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [テストポリシーとリリース判定基準](../../markdown/TESTING.md)

## II. 最近の追加機能

言語、ドライバー、標準ライブラリへの最近の追加：

- **標準ライブラリの `advantage` パッケージ — 数学、物理、統合、評価、ルーティング可能な出力を伴う配当の優位化（advantaging）。**
  [`lib/advantage/`](../../lib/advantage/README.md) は、*モデルに対する固有の配当を優位化すること*を、階層化された **Sequitur onion**（dividend → review → concerns → maths → additions → subtraction → regrouping → iteration）としてカプセル化します。内側にも外側にも移動でき、動的な更新と restitute（復元）を完全にサポートし、簡潔な**ユーザーモデル**と、完全な制御が可能な**開発者モデル**を通じて公開されます。五つの主題領域を備えています。**`math/`** は二者間貿易経済学（共同利益最大化、ナッシュ交渉、二者間ニュースベンダーの臨界比率）、**`physics/`** は構造経済学における「信頼場の物理学」（計量テンソルとしての信頼、取引測地線、アナウンスメント接触構造、CIR＋ジャンプによる信頼減衰SDE — モデル化上の類推であり、その旨を明示しています）、**`integration/`** は資本主義／小売向けの再帰的に構成可能な垂直統合モデル（コンパートメント、エンジニアリングベイ、試験コンパートメント、ニュートン中央値トレイ、システム洗練度スコア）、**`grading/`** は各モジュールと、勝ち／負け／グレーダー／絶対モジュールの任意の集合をA～Dで評価する洗練度回路グレーダー、**`output/`** はモジュールの出力を端末、**CommonRails** 形式の出力面、またはソケット／コネクター（ネイティブの `slnet_write` 向けにフレーム化）へ送るルーティング可能なemitシームです。すべてのクラスがMetadata-Classレコードを持ち、実行可能な五つのデモが各主題を検証します。補足資料の
  [`DEFINITIONS.md`](../../lib/advantage/DEFINITIONS.md)、
  [`MATH.md`](../../lib/advantage/MATH.md)、
  [`PHYSICS.md`](../../lib/advantage/PHYSICS.md) がそれらを説明しています。[`advantage/`](../../advantage/README.md) 配下のネイティブC/C++コンシューマー（Cコア `advantage_value`／`advantage_mean`、XMLナレッジストア、安全なHTTPS／SFTP／SSH取得＋キーストア＋DH／ECDHトランスポートライブラリ）は、モデルを実際の
  [取引例](../../advantage/examples/README.md)に結び付けます。内容は、想定されるインターネット上の情報源からのプラスの取引、対象者との関連性の系列（Traders、Consumers、Helpers、Healers）、およびA～Dで順位付けされた八つのモデル（Dが最もCement、Aが最もBrand）です。
- **ループ時間注釈 — `@loopDelay` / `@loopBudget`（および組み込み関数 `sleep(ms)`）。** SLeeLaで最初の**文レベルの動作**注釈です。`for` / `while` / `do` ループの前の独立した行に記述し、ループ実行に実際の待機を挿入します。`@loopDelay(ms = 250)` / `@loopDelay(seconds = 2)` は各反復で一定時間スリープします（引数のみの `@loopDelay(250)` はミリ秒とみなされます）。`@loopBudget(seconds = 10, iterations = 1000)` は、既知の反復回数を実時間の総予算にわたって均等に配分します。どちらも新しい追記型opcode **`OP_SLEEP`**（ISA 157）へ変換され、どこからでも呼び出せる汎用の組み込み関数 **`sleep(<ms>)`** もこのopcodeを使用します。待機は移植可能な形（`nanosleep` / `Sleep`）で実装されています。仕様は [`SLEELA.syntax` §32](../../SLEELA.syntax)、APIリファレンスは [`api/sleela/3.0/LOOP_TIMING_API.md`](../../api/sleela/3.0/LOOP_TIMING_API.md)、実行可能な例は
  [`examples/loop-timing-annotation-demo.sleela`](../../examples/loop-timing-annotation-demo.sleela) です。
  セッション追加分の索引（本件、ネイティブの `OP_BIGMUL` 多倍長整数乗算、および単体ツール **[local-generator](../../local-generator/README.md)** と **[pi-generator](../../pi-generator/README.md)** を含む）は
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](../../api/sleela/3.0/SESSION-ADDITIONS-API.md) にあります。
  生成された三つのAPI面
  （[`sleeladoc-api/`](../../sleeladoc-api/README.md)、
  [`docs/api/`](../../docs/api/README.md)、および不変の
  [`api/sleela/3.0/version-014/`](../../api/sleela/3.0/version-014/index.html)
  スナップショット）は `make api-docs` で再構築され、`vm` パッケージのページに新しい `SLISASleep` / `SLISABigMul` 命令の項目が追加されています。
- **コマンドラインからのSleelaDoc — `sleela docs`。** SleelaDocをターミナルから直接利用できるようになりました。`sleela docs` は文書化可能なすべてのトピックを一覧表示し、`sleela docs crypto` はパッケージを表示し、`sleela docs stringbuilder` はクラスを（大文字小文字を区別せず、また標準ライブラリの `SL` 接頭辞により `SLStringBuilder` へ）解決して、充実したページを出力します。ページには**定義、設計目的、関係、構造、利用者向けclay、五段階の習熟度ラダー**が含まれ、近い綴りの場合は順位付きの「もしかして」候補が表示されます。作者は、SleelaDocの `@` タグ（`@intent`、`@thesis`、`@axiom`、`@param`、`@see`、`@mastery`）を追加するだけで、任意のページを充実させられます。`sleela docs --generate [dir]` は（クラスごとに1ページ＋ `TOPICS.index` の）プレーンテキストのコーパスを書き出し、CLIはそれをオフラインで読み取ります。**`make sleeladoc`** は、そのコーパス**と対応する `.sleela` ソース**を `sleeladoc/build/sleeladoc.jar` にまとめます。ファサードのみの主題（例：`psychiatry`、`politico`）では、`sleela docs --facade <topic>` が実行時にファイルシステムを検索し、実際の**フォルダー（続いて自然な動きとしてファイル）**を、ユーザーが上書き可能な上限（**6 MB / 6000ファイル / 6分**）の範囲内で一覧表示します。`lib/sleeladoc` の各単位は現行版の **`#sleela 3.0`** で保守されています。[`man/man1/sleela.1`](../../man/man1/sleela.1) と [`lib/sleeladoc/README.md`](../../lib/sleeladoc/README.md) を参照してください。
- **SleelaDoc — Javadoc形式のドキュメントソリューションと、生成されるHTMLサイト。** **SleelaDoc** は、SLeeLaのソース内ドキュメントコメント規約（`@doc` / `@page` を番兵とする山括弧コメント。[`manifesto/SLEELADOC.md`](../../manifesto/SLEELADOC.md) を参照）であり、[`lib/sleeladoc`](../../lib/sleeladoc/README.md) は、`javadoc` がJavaを変換するのと同じように、`.sleela` 単位をMarkdownまたはHTMLのAPIページに変換する**抽出器**で、*意図*を文書化します。**`/lib` 全体の閲覧可能な完全なHTMLドキュメントサイト**は、`tools/generate-sleeladoc-api.py` によって [`/sleeladoc-api`](../../sleeladoc-api/README.md) に生成されます（`sleeladoc-api/index.html` を開いてください。青／水色／薄灰色／白の簡潔なスタイルで、パッケージ → クラス → メンバーの階層と、*Student 2* 読者向けに書かれた用語集を備えます）。コンパイラはコンパイルの副産物として、作者がマークしたドキュメント出力（`--wiki` によるMediaWikiページ＋元のソースの双子）も出力します。これは [`manifesto/SLEELADOC.WIKI.md`](../../manifesto/SLEELADOC.WIKI.md) に記載されています。実行可能な例：
  [`lib/sleeladoc/sleeladoc-demo.sleela`](../../lib/sleeladoc/sleeladoc-demo.sleela)。
- **XML Document Object Model（`lib/xml`）。** SLeeLa側のXML DOMです。要素／テキスト／コメント／CDATA／処理命令ノードのツリーを所有し、順序付き子ノードと名前付き属性を持つドキュメント、シリアライザー（コンパクト／整形、XML 1.0 §2.4のエスケープ）、文字セルパーサー、品質レジスター `SLXmlAspect` を備えます。ノード等価性は信頼の三段階（同一性 → 同一対象 → 構造）で判定され、反射的・対称的・決定的です。[`lib/xml/README.md`](../../lib/xml/README.md) と、実行可能な
  [`lib/xml/xml-dom.sleela`](../../lib/xml/xml-dom.sleela) を参照してください。
- **幅広い符号付き整数プリミティブ（`long`、`int64`、`int128`）。** `int`、`long`、`int64` はいずれも64ビット符号付き整数で、型検査も実行も同一に動作します。`int128` は整数型として受理されます（現時点では64ビット基盤で、真の128ビットは文書化された今後の課題です）。2進の符号付き系列 `int32`..`int16777216`（構文2.7）と、対応する符号なし系列 `uint32`..`uint16777216`（構文2.8）がこれを拡張します。[`SLEELA.syntax`](../../SLEELA.syntax) §21、§24、§25を参照してください。
- **`@mix` 連続（consequent）フィールド（構文2.9）。** 各繰り返しが `@mix` を持つ場合、クラスは**同じ名前**の複数のインスタンスフィールドを宣言できます。言語はこのグループを連続したもの（添字をキーとする配列／リスト）として扱い、既定では上から下へ `1,2,3,…` と索引付けされます。手動の場合は `@mix(1,2,3,...)` を使い、その索引数はフィールド数と一致しなければなりません。[`SLEELA.syntax`](../../SLEELA.syntax) §28を参照してください。
- **文書マーカー — Natural Process、名称はPreonems（`author`/`authorial`、`material`、`memory`、`determine`、`object`、`field`）。** これらの単独の文書マーカー語は、総称して **Preonems** と呼ばれます（個々のマーカーは*Preonem*、その集合は*Preonems*。[`SLEELA.syntax`](../../SLEELA.syntax) §26.1を参照）。文書が作られる順序の中で、文書が*何であるか*を示す、第一級で宣言的専用の文書マーカーです。`author`/`authorial`（構文2.4。専門家評議会が扱い、動作上の効果を持つ唯一のメンバー）、`material;`（確定したSource）と `memory;`（構文2.5）、`determine;`（構文2.6。医師、内科医、あるいはghostによってさらに処理され、法的な手による完成へ向かう文書）、そして最新の二つ、**`object;`**（存在するもののカタログに位置づけられた確定した対象 — **構文2.8で具体化され、具体的な位置づけを与えられました**）と **`field;`**（対象が立ち、動く関係の基盤）です。後の二つは構文2.6からゲートされています。いずれも単独で、文脈依存で、フロントエンドにより構文バージョンで制御され、構文解析、型付け、コード生成には影響しません。マーカー位置以外ではどれも通常の識別子のままです（したがって `field` がクラスメンバーや `.field` アクセスを妨げることはありません）。[`SLEELA.syntax`](../../SLEELA.syntax) §18～§20、§23、§26を参照してください。
- **構文3.0のNordshriftとライブラリ全体へのアクセス。** Nordshriftはコンパイラと同じ再帰的な `/lib` インデックスを通じて解決し、すべてのパッケージと `.sleela` 単位に到達します。`nordshrift check` はツリー全体の件数を報告します。
- **再帰スタンション。** [`recursion/stanchion/`](../../recursion/stanchion/README.md) ディレクトリに、文書化された七つのプリミティブ契約に対応する専用のCおよびC++参照プロトタイプと、汎用・応用・科学・教育向けの例が追加されました。これらは参照用ソースです。検証済みと見なす前に、コンパイルしてテストしてください。

- **山括弧コメント（`<` / `<<`）。** 新しい今後向けのコメントスタイル
  — `< line >` と `<< block >>` — をSLeeLaフロントエンドが字句解析し、
  [`SLEELA.syntax`](../../SLEELA.syntax) §1.1に記載されています。
- **基数付き整数リテラル。** `int` リテラルは、`_` 区切りを含む16進数（`0x`）、2進数（`0b`）、8進数（`0o`）形式を受け付けるようになりました。SLeeLaソース言語（[`SLEELA.syntax`](../../SLEELA.syntax) §1.4）と
  [`.xlt` CPUモデル形式](../../lib/xlt/README.md)の両方で利用できます。
- **ライブラリ全体のMetadata Class標準。** 記述的メタデータのメタパターン（出所、合法性、管轄範囲）が、`/lib` 以下のクラスを含むすべての `.sleela` ファイルに適用されるようになりました。[`SLEELA.syntax`](../../SLEELA.syntax) §15および
  [チュートリアルの課](../../tutorial/13-metadata-class.md)を参照してください。
- **より充実したテキスト／Stringファミリー。** `lib/text` は、言語内の実用的なヘルパー、本物の `charAt`／`substring`／`indexOf`／`reverse`／列揃えを提供する文字セルビュー、および本物の位置指定テンプレート機能を備えました。[テキストとStringのチュートリアル](../../tutorial/14-text-and-strings.md)と、実行可能な
  [`lib/text/text-string-demo.sleela`](../../lib/text/text-string-demo.sleela)を参照してください。
- **Nordshriftシートでの第一級の `String`。** `.sst` シートは型付き `strings:` ブロックを宣言でき、ビルドのコンポーネントマニフェストに反映されます。
  [Nordshrift Sheet Reference](../../wiki/Nordshrift-Sheet-Reference.md)を参照してください。
- **opcodeと関数呼び出しの等価性の両方で動作するCPUモデル。** すべての `SL<ARCH>CPU` は、fetch-decode-executeループを通じてネイティブ命令ストリームを実行する基盤*と*、共有VM基盤上の等価な呼び出しによって想定される別プロセスのコードを実行する基盤の両方を継承します — [`lib/cpu/EXECUTION.md`](../../lib/cpu/EXECUTION.md) §5を参照してください。

## III. ビルドとテスト

**前提条件：** GCCまたはClangなどのC/C++ツールチェーン、GNU Make、マニフェスト／フロー用ツールのPython 3が必要です。Linuxでディスプレイ関連製品をビルドする場合はX11/Xrandr開発パッケージも必要です（各モジュールの `make deps` が必要な依存関係を報告または導入します）。`impl/` のビルドは、信頼されたSHA-256マニフェストを満たさない場合に失敗する安全側の設計です。ルートの `Makefile` がこのマニフェストを自動的に参照します。

クリーンなチェックアウトからの最短手順：

```sh
make                                  # リポジトリのルートから全体をビルド
```

ネイティブコアだけをビルドしてテストする場合：

```sh
make -C impl all                      # コアのコンパイラ／ランタイムのみをビルド
bash ./test-suites/run-all.sh --all   # テストスイート全体を実行
```

ルートの `make` はコア、VM、codec、churn、antivirusなど、すべての製品をビルドし、最後に**レジストリ**をビルドしてベースライン表を生成します。主なターゲット：

```sh
make                     # 全体をビルド（make allの別名。レジストリは最後）
make all                 # 同じ全体ビルドを明示
sudo make install        # コマンドとランタイム支援ファイルをインストール（§IV参照）
make clean               # 各ビルドシステムの生成物を削除
make help                # ルートMakefileの全ターゲットを表示
```

その他のターゲット：

```sh
make registry            # レジストリのみをビルドし、表を生成
./build-registry.sh      # 同じ処理を直接実行
make tests               # テストスイートをビルドして実行
make ballum              # このOS向けのBallumディスプレイライブラリをビルド
bin/install-modules      # 各モジュールを段階的にインストール／ビルド（約10～20分）
```

レジストリターゲットは[`build-registry.sh`](../../build-registry.sh)を実行します。レジストリコアをビルドし、ベースライン表を移植可能なテキストストア `registry/build/sleela.sleelareg` に格納し、`sleela registry export` を通じてMySQLテーブル用スクリプト `registry/build/registry.sql` を出力します。`sleela registry` または `registry/build/sleela-registry review` で確認できます。[レジストリガイド](../../registry/README.md)と[Registry 3.0アーキテクチャ](../../registry/REGISTRY-3.0-ARCHITECTURE.md)を参照してください。

ビルドおよびテストのガイドで、前提条件とプラットフォーム固有の手順を確認してください。あるコンポーネントのビルド成功は、すべてのライブラリやプラットフォームがリリース可能であることを意味しません。

## IV. インストール

ネイティブ製品のビルド後、コマンドとランタイム支援ファイルをインストールします。

```sh
sudo make install
make install PREFIX="$HOME/.local"       # sudoを使わないユーザー単位のインストール
make install DESTDIR="$PWD/package-root" # パッケージ用のステージングインストール
make install-check
```

標準インストーラーの既定の配置先は `/usr/local/bin`、`/usr/local/lib/sleela`、`/usr/local/share/sleela` です。既存の起動設定を保持し、追跡されていない既存コマンドの上書きを拒否します。アンインストール時はインストーラーが記録したファイルのみを削除し、設定と未追跡のランタイムデータを保持します。

メインパッケージ（`sleela`、`sleelvac`、`nordshrift` バイナリ、標準ライブラリ、プロトコル、支援モジュール）のみを対話形式で導入する場合：

```sh
make clean-install       # メインパッケージのみをガイド付きでインストール
```

この手順では、まずメインパッケージをインストールするかを選択し、次に対象範囲を選び、成功後に厳選された拡張モジュールを任意で選択します。対話型の導入フローと全オプションについては[Quick and Safe Install](../../install/README.md)を参照してください。

## V. 言語とランタイム

正式な文法は[`SLEELA.syntax`](../../SLEELA.syntax)です。リポジトリが文書化している現行構文は **3.0** です。コンパイラ／ツールのバージョンとソース構文のバージョンは別のものです。構文3.0では **`pure` と `gasoline` の文書マーカー**（§30）が追加されています。これらは文書の位置づけをそれぞれの誠実さによって示す、文脈依存の宣言的Preonemです。構文3.0は1.0～2.9の言語機能を統合し、各機能に定められた最低バージョン要件を維持します。以前の構文2.9では **`@mix` 連続フィールド**（§28）が追加され、同名のインスタンスフィールドを一つの索引付きグループとして扱えるようになりました（既定の `1,2,3,…`、またはグループ内フィールド数と一致する `@mix(1,2,3,...)`）。また、`long`／`int64`／`int128`（§21）を基礎とする2進の符号付き／符号なし整数ファミリー `int32`～`int16777216`（§24）および `uint32`～`uint16777216`（§25）、キーワードサポーター文書（§22）、そしてPreonemsが引き継がれています。Preonemsには `author`／`authorial`（§18）、`material`／`memory`（§19～§20）、`determine`（§23）、`object`／`field`（§26）、`pure`／`gasoline`（§30）、および立法府級の位置づけを時間と空間にわたって示す `artifact`（§35）が含まれます。Preonems全体の名称は§26.1で定義されています。コンストラクターのオーバーロード解決と `this(...)`／基底クラスコンストラクターへの委譲は、検証済みの構文1.8契約の対象外です。機能を利用する前に文法と実装ノートを確認してください。SleelaDocの九つのソース単位は現在 `#sleela 3.0` を宣言し、共通コンパイラゲートはソース構文1.0～3.0を受け入れます。新しいAPIビルドは `make api-docs` で生成されます。

リポジトリには、ネイティブC/C++実装、SLVM生成リファレンス、標準ライブラリモジュール、テスト、例、プラットフォーム統合が含まれます。実装と検証の状況はモジュールごとに異なります。一様に完全であると仮定せず、各仕様と現在のCI結果を確認してください。

## VI. エンジニアリング原則

- **主張より証拠：** 実装済み、テスト済み、実験段階、計画中の動作を区別します。
- **境界ごとに一つの契約：** 構文、API、ファイル形式、プラットフォーム前提条件、失敗時の動作を文書化します。
- **移植性を設計段階から確保：** プラットフォーム固有のバックエンドを分離し、ネイティブ環境でテストします。
- **安全側に失敗：** 不正入力、リソース上限、プロセス障害、クリーンアップ経路をテストします。
- **再現可能なリリース：** クリーンな状態から再ビルドし、成果物ハッシュとリリースノートを公開します。

## VII. Antivirusのビルドと完全性

Sleelavirin antivirusパッケージには、ネイティブSHA-256スキャナー、任意のClamAVアダプター、ソース／索引の完全性マニフェスト、プラットフォームパッケージのチェックサムが含まれます。リポジトリルートから実行します。

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

完全性マニフェストは `antivirus/build/` に生成され、追跡対象のソースとドキュメント、および保護された署名索引ファイルを対象とします。生成物と変更可能なClamAVキャッシュは除外されます。ClamAVデータベースのスナップショットには専用マニフェストがあります。リリースZIPには `INTEGRITY-MANIFEST.json`、`SHA256SUMS`、`.sha256` サイドカーファイルが含まれます。ダウンロードしたアーカイブのハッシュは、独立して入手した信頼できる値と照合してください。同じアーカイブに添付されたチェックサムは発行者署名ではありません。詳細：[Sleelavirinガイド](../../antivirus/README.md)、[セキュリティチェックリスト](../../antivirus/SECURITY.md)、[ビルドワークフロー](../../.github/workflows/antivirus-binaries.yml)。該当CIが正確なリリースコミットで成功し、成果物が検査されるまで、リリースを検証済みとは見なしません。

## VIII. SLeeLaエディターテーマ — VimとEmacs

SLeeLaには、**Vim**と**Emacs**向けの統一されたプロフェッショナルなダークテーマと、ネイティブC/C++パレットユーティリティがあります。既定は **American Professional** です。文化的・制度的デザインから着想を得た補助パレットが10種類ありますが、いずれも国家、文化、裁判所、機関の公式表現ではありません。

### 11種類のスキームと色

| スキーム | 背景 | 前景 | キーワード | 型 | 文字列 | コメント |
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

#### 国旗色（基準パレット）

ギリシャとトルコのスキームは、両国の**公式国旗色**に由来し、公表されている正確な16進値とともに示します。上のスキームの背景／アクセントは、これらの元の色を暗くしてエディター向けに解釈したもので、下の表は「国として公表されている色」の参照です。

| 国 | 色 | 16進値 | 根拠・意味 |
|---|---|---|---|
| **ギリシャ** | Blue | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | 国旗の青（法律851/1978）。空と海を表し、紋章学では*azure*と記述される。 |
| **ギリシャ** | White | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | 国旗の白。独立闘争の純粋さを表し、十字は東方正教会を示す。 |
| **Türkiye** | Red（*al bayrak*） | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | 国旗の赤（トルコ国旗法）。地の色であり、伝統的に殉教者の血とオスマン主権を表す。 |
| **Türkiye** | White | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | 国旗の白。三日月と星は、平和、純粋さ、テュルク／イスラムの遺産を象徴する。 |

各スキームは、エディターの前景を国旗の**白**に、キーワード、カーソル、選択範囲、ステータス行を国旗そのものの**青**（`#0D5EAF`）または**赤**（`#E30A17`）に定めているため、重要な色は文字どおり国の色です。これらは解釈に基づくデザインパレットであり、国旗色の参照は出所を示すために提供されるもので、いずれの国の公式表現でもありません。色の意味は、ギリシャ共和国外務省の国家シンボルの説明とトルコ国旗法の伝統に基づきます。


### カラースウォッチ表

以下のパレットのセルには**SVGスウォッチ**を使用しているため、GitHubでは空のHTMLカラーセルではなく実際の色が表示されます。各ストリップは、背景、前景、キーワード、型、文字列、コメントの6色をこの順序で示します。正確な16進コードは各スウォッチの横に、上の参照表と同じスキーム順で記載しています。

| スキーム | 描画されたパレット色 | 16進コード（背景 · 前景 · キーワード · 型 · 文字列 · コメント） |
|---|---|---|
| **American Professional** | ![American Professional palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/american-professional.svg) | `#101820` · `#F4F7FA` · `#7FB3FF` · `#73DACA` · `#F2C078` · `#8A96A3` |
| **Pan-Eurasian** | ![Pan-Eurasian palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/pan-eurasian.svg) | `#111827` · `#F0F2F5` · `#7AA7E8` · `#69C9B3` · `#D7B56D` · `#8995A5` |
| **Russian and Slavic — Medium** | ![Russian and Slavic Medium palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/russian-slavic-medium.svg) | `#202733` · `#E8EAF0` · `#A9C0DD` · `#8FC4B2` · `#D6BD8B` · `#9AA3AF` |
| **Central Asian / Silk Road** | ![Central Asian Silk Road palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/central-asian-silk-road.svg) | `#1C2529` · `#F5F0E5` · `#7FADE0` · `#45C6C2` · `#D9B56D` · `#92A3A0` |
| **East Asian** | ![East Asian palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/east-asian.svg) | `#171A1C` · `#F2EFE7` · `#D97878` · `#72C7A5` · `#D8B76A` · `#969F9B` |
| **ICC — International Criminal Court** | ![ICC palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/icc.svg) | `#0E1A2B` · `#EAF0F7` · `#5B8FD6` · `#7FB7C9` · `#D9B25A` · `#7E8CA0` |
| **SCOTUS — U.S. Supreme Court** | ![SCOTUS palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/scotus.svg) | `#1A1416` · `#F3EEE6` · `#9E2B35` · `#8FA9B8` · `#C9A24B` · `#8C8178` |
| **Greek (Hellenic)** | ![Greek Hellenic palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/greek.svg) | `#0A1A30` · `#FFFFFF` · `#2E7BD6` · `#5E9FE0` · `#9FC3EC` · `#6E86A6` |
| **Turkish (Al Bayrak)** | ![Turkish Al Bayrak palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/turkish.svg) | `#2A0609` · `#FFFFFF` · `#F23D48` · `#F0A9AE` · `#F0C9A9` · `#A8767A` |
| **Kiro** | ![Kiro palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/kiro.svg) | `#0B0B12` · `#F5F5FA` · `#A98EDA` · `#6E8BE0` · `#E8C547` · `#6A6482` |
| **SLeeLa** | ![SLeeLa palette: background, foreground, keywords, types, strings, comments](../../lib/linux-editors/palettes/sleela.svg) | `#08080E` · `#FFFFFF` · `#B07CFF` · `#5A7CFF` · `#FFD21E` · `#6E5F9E` |

関数、定数、選択範囲、診断、プリプロセッサーディレクティブには追加のアクセント色を使用します。Russian and Slavicパレットは、より落ち着いた専門的な外観のため、中程度の強さのスレート、青、セージ、ルビー、アンティークゴールドを意図的に採用しています。

### Vimスキームのインストール

リポジトリのルートから実行します。

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

SLeeLaの構文強調を有効にしてスキームを選ぶには、`~/.vimrc` に次を追加します。

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

次のcolorscheme名から、ちょうど一つを選んでください。

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

たとえば、中程度のRussian and Slavicパレットには `colorscheme sleela-russian-slavic` を、HellenicおよびAl Bayrakパレットには `colorscheme sleela-greek` / `colorscheme sleela-turkish` を使用します。Vimを再起動するか、`:source $MYVIMRC` で設定を読み込み直してください。

### Emacsスキームのインストール

ローカルのテーマディレクトリを作成し、リポジトリのルートからモードとテーマのファイルをコピーします。

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

`~/.emacs` または `~/.emacs.d/init.el` に次を追加します。

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

別のスキームを選ぶには、`sleela-american` を `sleela-eurasia`、`sleela-russian-slavic`、`sleela-silk-road`、`sleela-east-asian`、`sleela-greek`、`sleela-turkish`、`sleela-kiro`、`sleela-sleela` のいずれかに置き換えます。Emacsがテーマを信頼するかどうか尋ねた場合は、承認する前にローカルのテーマファイルを確認してください。

### ネイティブC/C++パレットのプレビュー

小さなネイティブパレットユーティリティで、スキームの一覧表示、端末色の出力、個別の色の検索ができます。

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

後方互換性のため、`--theme` なしの `--palette` と `--hex COLOR` はAmerican Professionalを選びます。ビルドとテストの結果は対象システムとCIで確認してください。コマンドが文書化されていること自体は、それが成功した証拠ではありません。

パレットの詳細な説明とユーティリティの詳細は、[エディター統合ガイド](../../linux-editors/README.md)を参照してください。

## IX. 貢献

1. ドキュメント索引とテストポリシーを読みます。
2. 成功・失敗の両方のテストを伴う、焦点を絞った変更を行います。
3. 対象テストを実行し、その後に広範なテストスイートを実行します。
4. OSとツールチェーンを報告し、スキップした検査を成功と表現しないでください。

SLeeLaは **MEARVK LLC** によって開発されています。過去のランディングページ資料は [`docs/README-LEGACY.md`](../../docs/README-LEGACY.md) に保存されています。

## X. AE6E66 — Round Publicメール課税額の想定

AE6E66イニシアチブにおいて、プロジェクトが示す想定額は、条件を満たす各Round Publicメールに関連する提案上の課税額として、**1通あたり300,000,000.00米ドル**です。

この記述はプロジェクト上の想定のみを記録します。**法的な課税決定、政府命令、裁判所の判決、または独立して確立された納税義務ではありません。** 実際の課税には、適用される法的権限、管轄、定義、手続、および権限ある政府機関による決定が必要です。

## XI. アルファベットおよび文字セットランタイム

SLeeLaのアルファベット文書とネイティブ文字ランタイムは、`alphabet/sleela/` および `lib/alphabet/native/` で管理されています。ネイティブC++17テストでは、UTF-8スカラー値の厳密なデコード／エンコード、`C<n>` 容量シグネチャ、境界付き文字セット所属判定を検証します。

```sh
make -C lib/alphabet/native test
```

実験的なUTF-4088カタログとSLeeLa検索アダプターについては、[`utf-4088/SLeeLa-ALPHABET-INTEGRATION.md`](../../utf-4088/SLeeLa-ALPHABET-INTEGRATION.md)に記載されています。UTF-4088 IDはUnicodeスカラー値とは別の名前空間です。コンパイラーおよびSLVMへのバインディング作業は、これらのネイティブ基盤とは別に追跡されています。

## XII. AutoCAD Creator™

[AutoCAD Creator™](../../autocad/creator/README.md)は、SLeeLaのみで記述された対話型2D CADアプリケーションです。関連するAutoCAD Rendererが図面を一括生成するのに対し、Creatorでは2D AutoCAD文書をその場で**作成・編集**できます。壁、部屋、寸法、注記を追加し、配置した要素を移動、再ラベル付け、レイヤー変更、削除、取り消しできます。結果は実際のAutoCAD DXF図面（R12テキスト形式。AutoCAD／LibreCADで開けます）として**保存**できます。

GUIはJavaFXではなく、SLeeLa独自のクロスプラットフォームUIツールキット **SleelaUI™**（[`lib/user-interface`](../../lib/user-interface)）で構築され、X11／Cocoa／Win32上にネイティブウィンドウを表示します。設計原則は **「SleelaUIは提示し、SLeeLaが判断する」** です。GUIはジオメトリ計算もDXF書き込みも行いません。各ツールは、編集可能な文書と可逆的なコマンド履歴を所有する権威ある[`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela)に作成／変更／削除／取り消し／保存の意図を送ります。キャンバスはCreatorが保持するジオメトリをプレビューするだけです。

再利用可能な編集エンジンは[`lib/autocad`](../../lib/autocad)にあり、[`SLCadEntity`](../../lib/autocad/SLCadEntity.sleela)、[`SLCadDocument`](../../lib/autocad/SLCadDocument.sleela)、[`SLCadCommand`](../../lib/autocad/SLCadCommand.sleela)、[`SLCadCreator`](../../lib/autocad/SLCadCreator.sleela)で構成されます。ヘッドレス環境でもエンドツーエンドで動作します。GUIシェルとデモは[`autocad/creator`](../../autocad/creator)にあります。

```sh
make creator          # autocad/ から実行：Creatorデモ -> out/altered.dxf
```

ディスプレイがある環境ではライブ編集ウィンドウが開きます。ヘッドレス環境では図面をメモリ内で構築し、レポートとDXFを出力して正常終了します。

## XIII. UTF-4088入力マップの順序

特殊なUTF-4088カタログでは、空間入力マップの走査順序と抽象的な圧力反復を[`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md)に定義しています。National Lienの主題索引ポリシーは[`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md)にあります。マップ座標、圧力レベル（1～50）、永続カタログID、アルファベット順位はそれぞれ独立したフィールドであり、相互に置き換えてはなりません。圧力はアルゴリズム上の反復レベルであり、物理量ではありません。

## XIV. 最近拡張されたパッケージ索引

この追加索引は、最近拡張されたパッケージ、ドメインライブラリ、ソースコレクションを示します。項目はパッケージ名またはトピックのアルファベット順に分類されています。説明はリポジトリ内の場所を示します。実装状況と検証状況は、各パッケージ固有の文書およびテストで確認してください。

### A–C

- **Advantage** — `lib/advantage/` および `advantage/`：モデルに対する独自の配当最適化を、階層化されたSequitur onionとして扱います。5つの主題サブフォルダーがあります。`math/`（共同利益最大化、Nash交渉、二者間ニュースベンダー）、`physics/`（trust-fieldの計量／測地線／接触／SDE。モデル化上の類推であることを明示）、`integration/`（垂直統合コンパートメントと複雑度スコア）、`grading/`（モジュールおよびセット向けA～D複雑度回路グレーダー）、`output/`（端末／CommonRails／ソケット出力）。ネイティブC/C++コンシューマー（`advantage/`）はモデルを実例の取引に組み込みます。[README](../../lib/advantage/README.md)と[DEFINITIONS.md](../../lib/advantage/DEFINITIONS.md)を参照してください。
- **Alphabet and UTF-4088** — `alphabet/sleela/`、`lib/alphabet/native/`、`utf-4088/`：アルファベットカタログ、ネイティブ文字セット対応、入力マップ順序、UTF-4088統合文書。
- **AutoCAD Creator** — `autocad/creator/` および `lib/autocad/`：対話型2D作図ワークフローと再利用可能なCAD文書／編集コンポーネント。
- **Ballum** — ピクセル単位のディスプレイ制御：ピクセルの設定／取得、領域blit、画面全体の輝度／暖色度／ガンマ、ポインター／キーボード入力、および全面的に所有する高速ピクセル制御用の**全画面フレームバッファー面**（Linux/X11ではoverride-redirectウィンドウと単一の`XPutImage`／MIT-SHM present）。ネイティブコアは`ballum/`、SLeeLaファサードは`lib/ballum/`、VMブリッジは`impl/`にあります。[ガイド](../../ballum/README.md)と[実行可能な例](../../ballum/example/)を参照してください。
- **CDMA** — `cdma/` および `lib/cdma/`：CDMA関連の技術資料とSLeeLaソースライブラリ。
- **Character** — `lib/character/`：人物プロファイル、再利用可能な人物統計、経歴プロファイルの例。
- **Coorenagraph** — `lib/coorenagraph/`：倫理的推論、道徳語彙、熟議、および関連概念。パッケージREADMEとソースを参照してください。

### D–G

- **Governance source models** — [`lib/governance`](../../lib/governance/README.md)は、提案、匿名の手動投票、集計結果のための宣言的SLeeLaソース形式を定義します。提案中の`@vote`注釈と手動プロセスは[`registry/DEMOCRATIC-VOTING-SYNTAX.md`](../../registry/DEMOCRATIC-VOTING-SYNTAX.md)に記載されています。現行コンパイラーでは、これは不活性なコメントメタデータとして扱われます。

### H–I

- **HTTP 1.0–8.0** — 利用可能なバージョン別HTTPパッケージディレクトリ（`lib/http-1.0/`以降）：バージョン固有のプロトコル仕様と実装。正確な範囲と検証状況は各バージョンの文書を確認してください。
- **International Criminal Court** — `lib/international-criminal-court/` および `INTERNATIONAL.CRIMINAL.COURTS.md`：関連する専門分野のソースと文書。

### O–S

- **OFDMA** — `ofdma/`：直交周波数分割多元接続に関する技術資料とC/C++開発リソース。
- **Philosophy and the Conversation Mask module** — [`philosophy/`](../../philosophy/README.md)にはネイティブC/C++基盤があり、[`philosophy/module/`](../../philosophy/module/README.md)は次のモジュール層を3領域に整理します。[data/](../../philosophy/module/data/README.md)は型付き・日付付き観測、イベント終了、半減期設定、来歴、同意を扱います。[helper/](../../philosophy/module/helper/README.md)は決定論的検証、関連性の減衰、互換性のある比較、同意に基づく提示を担います。[psychiatry-friend/](../../philosophy/module/psychiatry-friend/README.md)は支援的で非臨床的な会話インターフェースです。SLeeLa側のハンドルと統合Treeは、より広いphilosophyパッケージとともに[`lib/philosophy/`](../../lib/philosophy/README.md)にあります。このモジュールは明示的入力と同意を優先し、人やメディアを受動的に監視せず、診断、私的意図の推測、人間の価値の採点、服薬変更の推奨を行いません。イベントリンクは来歴とナビゲーションを保持しますが、独立した裏付けを意味しません。
- **Synchro** — ネイティブランタイム／コンパイラー統合、パケットディスパッチ、測定関連機能。関連する`impl/`ソースとコンパイラー実装を参照してください。
- **SleelaUI / terminal UI** — `lib/user-interface/` および`sleela-terminal`プロジェクト：SLeeLa UI統合と端末向け対話コンポーネント。

### U–W

- **United Nations** — `united-nations/`：国連に関するドメイン固有のリポジトリ資料。
- **Wi-Fi 4、Wi-Fi 5、Wi-Fi 6** — `wi-fi-4/`、`wi-fi-5/`、`wi-fi-6/`、関連する`lib/wifi/`ソースディレクトリ：世代別の無線ネットワーク資料と構成リソース。

### パッケージ索引の保守

パッケージを追加または大幅に拡張した場合は、適切なアルファベット区分に項目を追加し、正式なディレクトリとREADMEにリンクし、チェックイン済みソースで裏付けられる機能だけを説明してください。この索引は追加方式で保守し、パッケージ索引の更新を理由に既存のREADMEセクションを削除または書き換えないでください。

---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

`bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh` :: `⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓`

`bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc` :: `⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉`

US Democratic and US Policy.

---

## XVI. National Scienceブランド画像

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>

## XVII. UTF-4088 100万件実験カタログ

[UTF-4088カタログ生成ツール](../../utf-4088/tools/generate_catalogue.py)は、1,048,576件の実験的シンボルカタログを決定論的に生成します。コンパクト形式は[`utf-4088/FORMAT.md`](../../utf-4088/FORMAT.md)、生成および検証契約は[`utf-4088/GENERATION-RESULTS.md`](../../utf-4088/GENERATION-RESULTS.md)に記載されています。

Python 3で次のように生成します。

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

生成バイナリは16,777,244バイトで、20,000,000バイト未満に収まる設計です。ユニットテストとGitHub Actionsワークフローが、エントリー数、グリフの一意性、サイズ、SHA-256を検証します。成功した実行では、バイナリとサマリーが`utf4088-million-catalogue`ワークフロー成果物として公開されます。生成バイナリは意図的にGit履歴へ含めません。UTF-4088は実験段階であり、Unicode互換ではありません。

---

# SLeeLa

<p align="center"><img src="../../images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. 最近の追加機能 — Pi定数、Modium／Registryの起動、sleelactl Math、`@native`

このセクションは、2026-11の作業で追加された機能を、新しいグループから順に記録します。リンク先のソースファイルとその検証状況が正式な根拠です。レビュー中の変更は、`master` に取り込まれるまでマージ済みとは記述しません。

### 1億桁のPiカタログ定数

リポジトリには、検証済みの**1億桁**のπ値[`pi-generator/pi.100000000.digits`](../../pi-generator/pi.100000000.digits)（正しい小数形式、`3.14159…`）と、57,344桁の[`pi-generator/pi.57344.digits`](../../pi-generator/pi.57344.digits)が含まれます。両方が真のπであることは独立に検証されています。検証内容（教科書的な先頭部分、小数第762位のFeynman point、著名な100万桁目、[`lib/math/SLPiReference.sleela`](../../lib/math/SLPiReference.sleela)との一致、ファイル全体の数字頻度分析）と一括再現スクリプトは[`pi-generator/VERIFIED.md`](../../pi-generator/VERIFIED.md)を参照してください。

### Modium™／Registry™はサブシステム起動時にPi定数を読み込む

専用サブシステムのエントリーポイント、すなわち`sleela registry`、`sleela modium`（およびインストーラーの初回実行）で、ModiumとRegistryは大きなπカタログ定数を読み込みます。`sleela run`の実行ごとには読み込みません。対話型端末では、一時的な赤色の`Loading Modium™`／`Loading Registry™`行を表示し、通常出力の前にその場で消去します。この表示はTTYの場合に限定されるため、パイプやテスト出力はバイト単位で変化しません。データ本体ではなくロケーターが、SLeeLaモデル（[`lib/registry/SLRegistry.sleela`](../../lib/registry/SLRegistry.sleela)）とネイティブシーダー（[`registry/sleela_registry.c`](../../registry/sleela_registry.c)）の両方で、Registryの`Math`キー既定値`piReferenceLargePath`および`piReferenceLargeDigits`として登録されます。`lib/math/SLPiReference.sleela`は`largePath()`／`largeDigits()`を公開します。π定数は標準の`make`インストーラーとガイド付き「beautiful」インストーラーの両方によって、OS上のSLeeLa配置先（`<data>/pi/`）にインストールされます。詳細は[`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](../../pi-generator/MODIUM-REGISTRY-PI-STARTUP.md)を参照してください。

### `sleelactl` — beautifulインストーラーとネイティブバックエンドの`math` subject

- ガイド付き（「beautiful」）インストーラーは、`make`インストーラーと同様に、SLeeLaコントロールプレーンの`sleelactl`とそのモジュールレジストリをインストールするようになりました。[ドキュメント](../../bin/SLEELACTL.md)を参照してください。
- `sleelactl`には、ランタイムにコンパイルされるネイティブC/C++ subjectライブラリ向けの新しい`subject`種別が追加され、最初のsubjectとして`math`が登録されました。`sleelactl run math`は、subjectの参照デモを通じてネイティブ`__NativeMath`バックエンド（`impl/subjects/math/math_native.cpp`）をエンドツーエンドで実行します。`sleelactl info math`は到達方法（`import math;`）を説明します。subject種別が提供する動詞は`run`と`info`のみです。`sleela check`はSHEET登録のネイティブsubjectをロードしないため、意図的に`check`動詞はありません。

### `@native`、Native Drive、および多言語実行

新しい**`@native`**メソッド注釈は、ホットメソッドのAOT（Ahead-of-Time）ネイティブコンパイルを要求します。新しい `sleela aot` コマンドは `@native` メソッドを検出し、それぞれが対応する*オフロード可能なサブセット*内にあることを検証してC/C++を出力します。まだ翻訳できない構文があれば、正確な理由を示して**fail closed**（拒否）するため、翻訳が中途半端なメソッドが生成されることはありません。

```sh
sleela aot --list <file.sleela>    # メソッドごとの判定：ok / REFUSED（理由）
sleela aot --emit <file.sleela>    # サブセット内のプログラムについてC/C++を標準出力へ出力
```

`@native` は引き続きAOT処理の要求であり、対応していない構文は出力前に拒否されます。[Native Drive](../../impl/native-drive/README.md) は、ホストコンパイラー（MSVCを含む）による複数単位のC/C++実行ファイルおよび共有ライブラリのビルド、対応するSLeeLa AOT出力、ネイティブローダーABIに対するシンボル検査、シェルを介さないプロセス実行、stdin/stdout/stderrの処理、多段パイプ、XClassから本番ローダーへの委譲をサポートします。`sleelactl` はそのビルド／実行コマンドを公開し、`inspect native-drive` を通じてXClassの検査と、明示的なsystemdユニットの生成／インストールを中継します。VMは、必須のホスト認可と世代チェック付きハンドルを備えた、上限のあるネイティブモジュールハンドルテーブルを所有します。構文3.0では新たに `nativeOpen`、`nativeCall`、`nativeClose` が追加されました。追記型バイトコードのディスパッチは、別途バージョン管理されるnull/bool/int64/double/UTF-8のABIを越えて行われ、VMが所有する配列、構造体、その他の複合値はインタープリター内に留まり、同名バイトコードへのフォールバックを使用できます。プラグインは任意で、バージョン付きのホストコールバックテーブルを受け取り、入れ子のVM実行を通じて正確なSLeeLa関数を呼び出せます。その際、中断されたスタックのGCルート指定、ホストコールバック名の明示的な許可リスト、再帰的なネイティブAPI呼び出しの拒否が適用されます。ネイティブコードは依然として信頼されたプロセス内コードであり、サンドボックスではありません。ソースのプロセスポリシー注釈にはバージョン付きスキーマがありますが、まだ構文解析も強制もされていません。詳細、セキュリティ境界、七つのワークストリーム台帳は、[Native Drive](../../impl/native-drive/README.md) と [`pi-generator/AT-NATIVE.md`](../../pi-generator/AT-NATIVE.md) を参照してください。

## XIX. 国際版README

SLeeLaは、国際版に含まれる50市場向けに国別READMEのエントリーポイントを提供します。各国に`README.<CC>.md`というファイルが1つあり、複数言語が関係する場合は同じ国のファイル内にまとめられます。

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

**翻訳ポリシー：** これらのファイルは、最初の段階では各国向けにローカライズされたエントリーポイントです。導入部と見出しは各言語にローカライズされていますが、技術リファレンス全体は英語の正式版[README.md](../../README.md)にあります。すべてのセクションを翻訳し、技術識別子とリンクを確認し、言語レビューを受けるまでは、国別ファイルを完全翻訳版と表記してはなりません。この初期段階の言語範囲は代表的なものであり、憲法上認められるすべての地域語、先住民語、手話が完全に翻訳されたことを意味しません。

## 翻訳状況

**翻訳済み — 言語レビュー推奨。** 日本語版ではREADMEのセクションI～XIXを翻訳しました。技術識別子、コマンド、コード例、ファイル名、URL、アドレス文字列は原文どおり保持しています。公開前にネイティブスピーカーによる言語レビューと技術リンク確認を推奨します。

