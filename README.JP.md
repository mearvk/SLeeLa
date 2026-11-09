# SLeeLa 3.0 — 日本 (JP)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>

<p align="center"><strong>ネイティブシステム言語、コンパイラ、ランタイム、仮想マシンのエコシステム。</strong></p>

> **国別国際版 — 翻訳作業中。** このファイルは日本語版の作業用エントリーポイントです。以下の日本語セクションは翻訳済みですが、README.md の全技術セクションを逐語的に翻訳した完全版ではありません。仕様の正確な内容については英語の原文を参照してください。

**The Now. The Ingredient.** 言語は構文だけではありません。ソフトウェアを実用可能にするコンパイラ、ランタイム、ライブラリ、ツール、プラットフォーム契約も含みます。

**SLeeLaの対象領域。** SLeeLaの活動は **Math**、**Object**、**Science**、**Reach** の四つの側面から成ります。Mathは計算可能なもの、Objectは存在するもののカタログに位置づけられた確定した対象、Scienceは真理に対する体系的な探究、Reachはシステムの働きを他のシステムや人々へ広げることを表します。**国家的利益は維持されます。** いずれの側面も国家的利益を排除しません。国家の利益に関係する事項は、その関係を保持し、たとえば各クラスのMetadataにある `metaOrigin` / `metaNationalSchedule` に正確に記録します。

## I. はじめに

- [ネイティブ実装のビルド](impl/README.md)
- [仮想マシンとModium — 実践ユーザーガイド](virtual-machine/USER-GUIDE.md)（`sleela vm`、`sleela modium`）
- [SLeeLaソース内のCPUスタック](lib/cpu/CPU.md)：論理ゲートからALU、コア、OS、プログラムまでを扱う機械構成、130以上のCPUプロファイル、調整可能な[電圧・可変クロック・高度なタイミング設定](lib/cpu/TIMING.md)、および[仕様準拠実行とオーバークロック](lib/cpu/components/README.md)。
- [ドキュメント索引](docs/README.md)
- [CSPMLデータソースおよびSalt & Salary調査リファレンス](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md)：米国の地域、社会経済、労働力、教育、公立学校に関する外部データの出典と統合指針。
- [生成済みAPIドキュメント](docs/api/README.md)：現行HTMLリファレンス、バージョン別API版、再生成手順。
- [APIカタログ](api/README.md)
- [計算比較標準](comparison/README.md)：SLeeLa VM、Java/JVM、ネイティブC/C++の型、メモリ、ABI、適合性に関する標準。
- [再帰スタンション — CおよびC++プロトタイプ](recursion/stanchion/README.md)：Boolean、Byte、Character、Integer、Long、Float、Doubleの各プリミティブ契約に特化した例と、汎用・応用・科学・教育向けの再帰パターン。
- [プラットフォーム対応および検証マトリクス](docs/PLATFORM-SUPPORT.md)
- [ブランドガイド](docs/BRAND-GUIDE.md)
- [製品コンセプトとアセット](merchandise/README.md)
- [NitroWebExpressロゴ](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [テストポリシーとリリース判定基準](markdown/TESTING.md)

## II. 最近の追加機能

言語、ドライバー、標準ライブラリに加えられた主な機能を示します。識別子、構文、コマンド、ファイル名は原文どおり保持しています。

- **標準ライブラリの `advantage` パッケージ。** [`lib/advantage/`](lib/advantage/README.md) は、モデルに対する固有の配当を扱う階層型 **Sequitur onion**（dividend → review → concerns → maths → additions → subtraction → regrouping → iteration）を実装します。ユーザーモデルと開発者向けフルコントロールモデルを備え、`math/`（二者間取引経済、共同利益最大化、ナッシュ交渉、bilateral newsvendorの臨界比率）、`physics/`（信頼場を計量テンソルや取引測地線などで表す構造経済学上のモデル化類推）、`integration/`（再帰的に構成可能な垂直統合モデル）、`grading/`（A–Dのモジュール評価）、`output/`（端末、CommonRails形式の出力面、ソケット／コネクターへの出力）の五領域を含みます。C/C++コンシューマーは[`advantage/`](advantage/README.md)にあります。
- **ループ時間注釈 — `@loopDelay` / `@loopBudget` と `sleep(ms)`。** `for`、`while`、`do`ループの直前に記述する動作注釈で、ループ実行に実際の待機を挿入します。`@loopDelay(ms = 250)` は各反復に固定の待機を設け、`@loopBudget(seconds = 10, iterations = 1000)` は指定反復数を合計時間の予算内に配分します。両者は追記型opcode **`OP_SLEEP`**（ISA 157）へ変換され、組み込み関数 `sleep(<ms>)` も同じ命令を使用します。仕様は[`SLEELA.syntax` §32](SLEELA.syntax)、APIは[LOOP_TIMING_API.md](api/sleela/3.0/LOOP_TIMING_API.md)、実行例は[loop-timing-annotation-demo.sleela](examples/loop-timing-annotation-demo.sleela)を参照してください。
- **コマンドラインからのSleelaDoc — `sleela docs`。** `sleela docs` は文書化可能なトピックを一覧表示し、`sleela docs crypto` はパッケージを、`sleela docs stringbuilder` はクラスを検索します。ページには定義、設計目的、関係、構造、利用概念、五段階の習熟度が含まれます。`@intent`、`@thesis`、`@axiom`、`@param`、`@see`、`@mastery` タグで内容を拡張できます。`sleela docs --generate [dir]` はオフライン参照用テキストコーパスを生成し、`make sleeladoc` はコーパスと対応する `.sleela` ソースをJARにまとめます。詳細は[`lib/sleeladoc/README.md`](lib/sleeladoc/README.md)を参照してください。
- **SleelaDocとHTMLドキュメントサイト。** [`lib/sleeladoc`](lib/sleeladoc/README.md)は、`.sleela` ソースのドキュメントコメントをMarkdownまたはHTMLのAPIページに変換します。`/lib`全体を対象とする閲覧可能なHTMLサイトは[`sleeladoc-api/`](sleeladoc-api/README.md)に生成されます。コンパイラは `--wiki` によりMediaWikiページも出力できます。関連資料：[SLEELADOC](manifesto/SLEELADOC.md)、[SLEELADOC.WIKI](manifesto/SLEELADOC.WIKI.md)。
- **XML Document Object Model（`lib/xml`）。** 要素、テキスト、コメント、CDATA、処理命令のノードツリー、順序付き子ノード、名前付き属性、コンパクト／整形シリアライザー、XML 1.0 §2.4のエスケープ、文字セルパーサー、品質レジスター `SLXmlAspect` を備えます。ノード等価性は同一性、同一対象、構造という三段階で判定され、反射的・対称的・決定的です。[`lib/xml/README.md`](lib/xml/README.md)と[実行例](lib/xml/xml-dom.sleela)を参照してください。
- **幅広い符号付き整数プリミティブ。** `int`、`long`、`int64` は同じ64ビット符号付き整数として動作します。`int128` は現時点では64ビット基盤で受理され、真の128ビット実装は今後の課題として記録されています。符号付き `int32`〜`int16777216`（構文2.7）と符号なし `uint32`〜`uint16777216`（構文2.8）も追加されています。[`SLEELA.syntax`](SLEELA.syntax) §§21、24、25を参照してください。
- **`@mix` 連続フィールド（構文2.9）。** 各宣言に `@mix` が付いていれば、クラス内で同名のインスタンスフィールドを複数宣言できます。グループは既定で `1,2,3,…` と索引付けされ、`@mix(1,2,3,...)` による手動指定も可能です。指定する索引数はフィールド数と一致する必要があります。詳細は[`SLEELA.syntax` §28](SLEELA.syntax)を参照してください。
- **Preonems — Natural Processの文書マーカー。** `author`/`authorial`、`material`、`memory`、`determine`、`object`、`field`、`pure`、`gasoline`、`artifact` は宣言的な文書マーカーです。文脈と構文バージョンにより制御され、構文解析、型付け、コード生成には影響しません。マーカー位置以外では通常の識別子です。詳細は[`SLEELA.syntax`](SLEELA.syntax) §§18–20、23、26、30、35を参照してください。
- **Nordshriftのライブラリ全体へのアクセス。** 構文3.0では、Nordshriftはコンパイラと同じ再帰的な `/lib` インデックスを使用し、すべてのパッケージと `.sleela` 単位を解決します。`nordshrift check` はツリー全体の件数を報告します。
- **再帰スタンション。** [`recursion/stanchion/`](recursion/stanchion/README.md)には、文書化された七つのプリミティブ契約に対応するC/C++参照プロトタイプと、汎用・応用・科学・教育向けの例があります。検証済みと見なす前に、参照ソースをコンパイルしてテストしてください。
- **山括弧コメント（`<` / `<<`）。** `< line >` と `<< block >>` 形式のコメントをフロントエンドが認識します。[`SLEELA.syntax` §1.1](SLEELA.syntax)を参照してください。
- **基数付き整数リテラル。** `int` リテラルは、`_` 区切りを含む16進数（`0x`）、2進数（`0b`）、8進数（`0o`）に対応します。SLeeLaソース言語と[`.xlt` CPUモデル形式](lib/xlt/README.md)の両方で利用できます。
- **ライブラリ全体のMetadata Class標準。** 出所、合法性、管轄範囲を記録するメタデータパターンが、`/lib` 以下のクラスを含む `.sleela` ファイルに適用されています。[`SLEELA.syntax` §15](SLEELA.syntax)および[チュートリアル第13課](tutorial/13-metadata-class.md)を参照してください。
- **テキスト／Stringファミリーの拡張。** `lib/text` は `charAt`、`substring`、`indexOf`、`reverse`、列揃え、位置指定テンプレートなどを提供します。[チュートリアル](tutorial/14-text-and-strings.md)と[実行例](lib/text/text-string-demo.sleela)を参照してください。
- **NordshriftシートでのString宣言。** `.sst` シートは型付き `strings:` ブロックを宣言でき、ビルドのコンポーネントマニフェストに反映されます。[Nordshrift Sheet Reference](wiki/Nordshrift-Sheet-Reference.md)を参照してください。
- **opcodeと関数呼び出しの両方で動作するCPUモデル。** 各 `SL<ARCH>CPU` は、fetch-decode-executeループによるネイティブ命令ストリームの実行と、共有VM基盤上の同等呼び出しによる別プロセスのコード実行を扱います。[`lib/cpu/EXECUTION.md` §5](lib/cpu/EXECUTION.md)を参照してください。

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

レジストリターゲットは[`build-registry.sh`](build-registry.sh)を実行します。レジストリコアをビルドし、ベースライン表を移植可能なテキストストア `registry/build/sleela.sleelareg` に格納し、`sleela registry export` を通じてMySQLテーブル用スクリプト `registry/build/registry.sql` を出力します。`sleela registry` または `registry/build/sleela-registry review` で確認できます。[レジストリガイド](registry/README.md)と[Registry 3.0アーキテクチャ](registry/REGISTRY-3.0-ARCHITECTURE.md)を参照してください。

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

この手順では、まずメインパッケージをインストールするかを選択し、次に対象範囲を選び、成功後に厳選された拡張モジュールを任意で選択します。対話型の導入フローと全オプションについては[Quick and Safe Install](install/README.md)を参照してください。

## V. 言語とランタイム

正式な文法は[`SLEELA.syntax`](SLEELA.syntax)です。リポジトリが文書化している現行構文は **3.0** です。コンパイラ／ツールのバージョンとソース構文のバージョンは別のものです。構文3.0では **`pure` と `gasoline` の文書マーカー**（§30）が追加されています。これらは文書の位置づけをそれぞれの誠実さによって示す、文脈依存の宣言的Preonemです。構文3.0は1.0～2.9の言語機能を統合し、各機能に定められた最低バージョン要件を維持します。以前の構文2.9では **`@mix` 連続フィールド**（§28）が追加され、同名のインスタンスフィールドを一つの索引付きグループとして扱えるようになりました（既定の `1,2,3,…`、またはグループ内フィールド数と一致する `@mix(1,2,3,...)`）。また、`long`／`int64`／`int128`（§21）を基礎とする2進の符号付き／符号なし整数ファミリー `int32`～`int16777216`（§24）および `uint32`～`uint16777216`（§25）、キーワードサポーター文書（§22）、そしてPreonemsが引き継がれています。Preonemsには `author`／`authorial`（§18）、`material`／`memory`（§19～§20）、`determine`（§23）、`object`／`field`（§26）、`pure`／`gasoline`（§30）、および立法府級の位置づけを時間と空間にわたって示す `artifact`（§35）が含まれます。Preonems全体の名称は§26.1で定義されています。コンストラクターのオーバーロード解決と `this(...)`／基底クラスコンストラクターへの委譲は、検証済みの構文1.8契約の対象外です。機能を利用する前に文法と実装ノートを確認してください。SleelaDocの九つのソース単位は現在 `#sleela 3.0` を宣言し、共通コンパイラゲートはソース構文1.0～3.0を受け入れます。

## VI. 以降の技術セクション

この初期日本語版では、README.mdの残りの技術セクションをまだ翻訳していません。以下の英語原文を参照してください。

- [README.md — 完全な技術ドキュメント](README.md)
- [APIカタログ](api/README.md)
- [プラットフォーム対応](docs/PLATFORM-SUPPORT.md)
- [テストポリシー](markdown/TESTING.md)

## 翻訳状況

**作業中 — 部分翻訳。** 冒頭およびセクションI～Vを日本語化しました。README.mdの残りのセクションは未翻訳です。コマンド、コード例、技術識別子、ファイル名、リンクは原文どおり保持しています。全セクションの翻訳と技術レビューが完了するまでは、完全翻訳版として扱わないでください。
