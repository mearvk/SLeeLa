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

完全性マニフェストは `antivirus/build/` に生成され、追跡対象のソースとドキュメント、および保護された署名索引ファイルを対象とします。生成物と変更可能なClamAVキャッシュは除外されます。ClamAVデータベースのスナップショットには専用マニフェストがあります。リリースZIPには `INTEGRITY-MANIFEST.json`、`SHA256SUMS`、`.sha256` サイドカーファイルが含まれます。ダウンロードしたアーカイブのハッシュは、独立して入手した信頼できる値と照合してください。同じアーカイブに添付されたチェックサムは発行者署名ではありません。詳細：[Sleelavirinガイド](antivirus/README.md)、[セキュリティチェックリスト](antivirus/SECURITY.md)、[ビルドワークフロー](.github/workflows/antivirus-binaries.yml)。該当CIが正確なリリースコミットで成功し、成果物が検査されるまで、リリースを検証済みとは見なしません。

## VIII. SLeeLaエディターテーマ — VimとEmacs

SLeeLaには、**Vim**と**Emacs**向けの統一されたプロフェッショナルなダークテーマと、ネイティブC/C++パレットユーティリティがあります。既定は **American Professional** です。文化的・制度的デザインから着想を得た補助パレットが10種類ありますが、いずれも国家、文化、裁判所、機関の公式表現ではありません。

### 11種類のスキームと色

各行の色は、背景／前景／キーワード／型／文字列／コメントの順です。

| スキーム | 背景 | 前景 | キーワード | 型 | 文字列 | コメント |
|---|---|---|---|---|---|---|
| American Professional | `#101820` | `#F4F7FA` | `#7FB3FF` | `#73DACA` | `#F2C078` | `#8A96A3` |
| Pan-Eurasian | `#111827` | `#F0F2F5` | `#7AA7E8` | `#69C9B3` | `#D7B56D` | `#8995A5` |
| Russian and Slavic — Medium | `#202733` | `#E8EAF0` | `#A9C0DD` | `#8FC4B2` | `#D6BD8B` | `#9AA3AF` |
| Central Asian / Silk Road | `#1C2529` | `#F5F0E5` | `#7FADE0` | `#45C6C2` | `#D9B56D` | `#92A3A0` |
| East Asian | `#171A1C` | `#F2EFE7` | `#D97878` | `#72C7A5` | `#D8B76A` | `#969F9B` |
| ICC — International Criminal Court | `#0E1A2B` | `#EAF0F7` | `#5B8FD6` | `#7FB7C9` | `#D9B25A` | `#7E8CA0` |
| SCOTUS — U.S. Supreme Court | `#1A1416` | `#F3EEE6` | `#9E2B35` | `#8FA9B8` | `#C9A24B` | `#8C8178` |
| Greek (Hellenic) | `#0A1A30` | `#FFFFFF` | `#2E7BD6` | `#5E9FE0` | `#9FC3EC` | `#6E86A6` |
| Turkish (Al Bayrak) | `#2A0609` | `#FFFFFF` | `#F23D48` | `#F0A9AE` | `#F0C9A9` | `#A8767A` |
| Kiro | `#0B0B12` | `#F5F5FA` | `#A98EDA` | `#6E8BE0` | `#E8C547` | `#6A6482` |
| SLeeLa | `#08080E` | `#FFFFFF` | `#B07CFF` | `#5A7CFF` | `#FFD21E` | `#6E5F9E` |

### 国旗色の参照パレット

GreekとTurkishのテーマは、各国の**公式国旗色**を基礎とします。上記のテーマ色はエディター向けに暗く調整した解釈です。

| 国 | 色 | 16進値 | 根拠・意味 |
|---|---|---|---|
| Greece | Blue | `#0D5EAF` | 法律851/1978に基づく国旗の青。空と海を表し、紋章学ではazure。 |
| Greece | White | `#FFFFFF` | 国旗の白。独立闘争の純粋さを表し、十字は東方正教会を示す。 |
| Türkiye | Red (al bayrak) | `#E30A17` | トルコ国旗法に基づく赤。伝統的に殉教者の血とオスマン主権を表す。 |
| Türkiye | White | `#FFFFFF` | 国旗の白。三日月と星は平和、純粋さ、テュルク／イスラムの遺産を象徴する。 |

各テーマは前景に国旗の**白**を採用し、キーワード、カーソル、選択範囲、ステータス行には正確な国旗の**青**（`#0D5EAF`）または**赤**（`#E30A17`）を使用します。これは解釈に基づくパレットであり、いずれの国の公式表現でもありません。色の意味はギリシャ共和国外務省の国家シンボル説明とトルコ国旗法の伝統に基づきます。

### SVGスウォッチと追加色

[American Professional](lib/linux-editors/palettes/american-professional.svg)、[Pan-Eurasian](lib/linux-editors/palettes/pan-eurasian.svg)、[Russian and Slavic](lib/linux-editors/palettes/russian-slavic-medium.svg)、[Central Asian / Silk Road](lib/linux-editors/palettes/central-asian-silk-road.svg)、[East Asian](lib/linux-editors/palettes/east-asian.svg)、[ICC](lib/linux-editors/palettes/icc.svg)、[SCOTUS](lib/linux-editors/palettes/scotus.svg)、[Greek](lib/linux-editors/palettes/greek.svg)、[Turkish](lib/linux-editors/palettes/turkish.svg)、[Kiro](lib/linux-editors/palettes/kiro.svg)、[SLeeLa](lib/linux-editors/palettes/sleela.svg)の各SVGは、背景、前景、キーワード、型、文字列、コメントの6色をこの順序で表示します。関数、定数、選択範囲、診断、プリプロセッサーディレクティブには追加のアクセント色を使用します。Russian and Slavicテーマは落ち着いた専門的な外観のため、中程度のスレート、青、セージ、ルビー、アンティークゴールドを採用しています。

### Vimテーマのインストール

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

`~/.vimrc` に追加します。

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

選択できる名前は `sleela-american`、`sleela-eurasia`、`sleela-russian-slavic`、`sleela-silk-road`、`sleela-east-asian`、`sleela-greek`、`sleela-turkish`、`sleela-kiro`、`sleela-sleela` です。たとえば `colorscheme sleela-russian-slavic`、`colorscheme sleela-greek`、`colorscheme sleela-turkish` を指定できます。Vimを再起動するか `:source $MYVIMRC` を実行してください。

### Emacsテーマのインストール

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

`~/.emacs` または `~/.emacs.d/init.el` に追加します。

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

`sleela-american` は `sleela-eurasia`、`sleela-russian-slavic`、`sleela-silk-road`、`sleela-east-asian`、`sleela-greek`、`sleela-turkish`、`sleela-kiro`、`sleela-sleela` に置き換えられます。Emacsがテーマの信頼を尋ねた場合は、承認前にローカルファイルを確認してください。

### ネイティブC/C++パレットのプレビュー

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

後方互換性のため、`--theme` なしの `--palette` と `--hex COLOR` はAmerican Professionalを選びます。ビルドとテストは対象システムおよびCIで確認してください。コマンドの文書化はテスト成功の証拠ではありません。詳細は[エディター統合ガイド](linux-editors/README.md)を参照してください。

## IX. 貢献

1. ドキュメント索引とテストポリシーを読みます。
2. 成功・失敗の両方のテストを伴う、焦点を絞った変更を行います。
3. 対象テストを実行し、その後に広範なテストスイートを実行します。
4. OSとツールチェーンを報告し、スキップした検査を成功と表現しないでください。

SLeeLaは **MEARVK LLC** によって開発されています。過去のランディングページ資料は [`docs/README-LEGACY.md`](docs/README-LEGACY.md) に保存されています。

## X. AE6E66 — Round Publicメール課税額の想定

AE6E66イニシアチブにおいて、プロジェクトが示す想定額は、条件を満たす各Round Publicメールに関連する提案上の課税額として、**1通あたり300,000,000.00米ドル**です。

この記述はプロジェクト上の想定のみを記録します。**法的な課税決定、政府命令、裁判所の判決、または独立して確立された納税義務ではありません。** 実際の課税には、適用される法的権限、管轄、定義、手続、および権限ある政府機関による決定が必要です。

## XI. アルファベットおよび文字セットランタイム

SLeeLaのアルファベット文書とネイティブ文字ランタイムは、`alphabet/sleela/` および `lib/alphabet/native/` で管理されています。ネイティブC++17テストでは、UTF-8スカラー値の厳密なデコード／エンコード、`C<n>` 容量シグネチャ、境界付き文字セット所属判定を検証します。

```sh
make -C lib/alphabet/native test
```

実験的なUTF-4088カタログとSLeeLa検索アダプターについては、[`utf-4088/SLeeLa-ALPHABET-INTEGRATION.md`](utf-4088/SLeeLa-ALPHABET-INTEGRATION.md)に記載されています。UTF-4088 IDはUnicodeスカラー値とは別の名前空間です。コンパイラーおよびSLVMへのバインディング作業は、これらのネイティブ基盤とは別に追跡されています。

## XII. AutoCAD Creator™

[AutoCAD Creator™](autocad/creator/README.md)は、SLeeLaのみで記述された対話型2D CADアプリケーションです。関連するAutoCAD Rendererが図面を一括生成するのに対し、Creatorでは2D AutoCAD文書をその場で**作成・編集**できます。壁、部屋、寸法、注記を追加し、配置した要素を移動、再ラベル付け、レイヤー変更、削除、取り消しできます。結果は実際のAutoCAD DXF図面（R12テキスト形式。AutoCAD／LibreCADで開けます）として**保存**できます。

GUIはJavaFXではなく、SLeeLa独自のクロスプラットフォームUIツールキット **SleelaUI™**（[`lib/user-interface`](lib/user-interface)）で構築され、X11／Cocoa／Win32上にネイティブウィンドウを表示します。設計原則は **「SleelaUIは提示し、SLeeLaが判断する」** です。GUIはジオメトリ計算もDXF書き込みも行いません。各ツールは、編集可能な文書と可逆的なコマンド履歴を所有する権威ある[`SLCadCreator`](lib/autocad/SLCadCreator.sleela)に作成／変更／削除／取り消し／保存の意図を送ります。キャンバスはCreatorが保持するジオメトリをプレビューするだけです。

再利用可能な編集エンジンは[`lib/autocad`](lib/autocad)にあり、[`SLCadEntity`](lib/autocad/SLCadEntity.sleela)、[`SLCadDocument`](lib/autocad/SLCadDocument.sleela)、[`SLCadCommand`](lib/autocad/SLCadCommand.sleela)、[`SLCadCreator`](lib/autocad/SLCadCreator.sleela)で構成されます。ヘッドレス環境でもエンドツーエンドで動作します。GUIシェルとデモは[`autocad/creator`](autocad/creator)にあります。

```sh
make creator          # autocad/ から実行：Creatorデモ -> out/altered.dxf
```

ディスプレイがある環境ではライブ編集ウィンドウが開きます。ヘッドレス環境では図面をメモリ内で構築し、レポートとDXFを出力して正常終了します。

## XIII. UTF-4088入力マップの順序

特殊なUTF-4088カタログでは、空間入力マップの走査順序と抽象的な圧力反復を[`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md)に定義しています。National Lienの主題索引ポリシーは[`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md)にあります。マップ座標、圧力レベル（1～50）、永続カタログID、アルファベット順位はそれぞれ独立したフィールドであり、相互に置き換えてはなりません。圧力はアルゴリズム上の反復レベルであり、物理量ではありません。

## XIV. 最近拡張されたパッケージ索引

この追加索引は、最近拡張されたパッケージ、ドメインライブラリ、ソースコレクションを示します。項目はパッケージ名またはトピックのアルファベット順に分類されています。説明はリポジトリ内の場所を示します。実装状況と検証状況は、各パッケージ固有の文書およびテストで確認してください。

### A–C

- **Advantage** — `lib/advantage/` および `advantage/`：モデルに対する独自の配当最適化を、階層化されたSequitur onionとして扱います。5つの主題サブフォルダーがあります。`math/`（共同利益最大化、Nash交渉、二者間ニュースベンダー）、`physics/`（trust-fieldの計量／測地線／接触／SDE。モデル化上の類推であることを明示）、`integration/`（垂直統合コンパートメントと複雑度スコア）、`grading/`（モジュールおよびセット向けA～D複雑度回路グレーダー）、`output/`（端末／CommonRails／ソケット出力）。ネイティブC/C++コンシューマー（`advantage/`）はモデルを実例の取引に組み込みます。[README](lib/advantage/README.md)と[DEFINITIONS.md](lib/advantage/DEFINITIONS.md)を参照してください。
- **Alphabet and UTF-4088** — `alphabet/sleela/`、`lib/alphabet/native/`、`utf-4088/`：アルファベットカタログ、ネイティブ文字セット対応、入力マップ順序、UTF-4088統合文書。
- **AutoCAD Creator** — `autocad/creator/` および `lib/autocad/`：対話型2D作図ワークフローと再利用可能なCAD文書／編集コンポーネント。
- **Ballum** — ピクセル単位のディスプレイ制御：ピクセルの設定／取得、領域blit、画面全体の輝度／暖色度／ガンマ、ポインター／キーボード入力、および全面的に所有する高速ピクセル制御用の**全画面フレームバッファー面**（Linux/X11ではoverride-redirectウィンドウと単一の`XPutImage`／MIT-SHM present）。ネイティブコアは`ballum/`、SLeeLaファサードは`lib/ballum/`、VMブリッジは`impl/`にあります。[ガイド](ballum/README.md)と[実行可能な例](ballum/example/)を参照してください。
- **CDMA** — `cdma/` および `lib/cdma/`：CDMA関連の技術資料とSLeeLaソースライブラリ。
- **Character** — `lib/character/`：人物プロファイル、再利用可能な人物統計、経歴プロファイルの例。
- **Coorenagraph** — `lib/coorenagraph/`：倫理的推論、道徳語彙、熟議、および関連概念。パッケージREADMEとソースを参照してください。

### D–G

- **Governance source models** — [`lib/governance`](lib/governance/README.md)は、提案、匿名の手動投票、集計結果のための宣言的SLeeLaソース形式を定義します。提案中の`@vote`注釈と手動プロセスは[`registry/DEMOCRATIC-VOTING-SYNTAX.md`](registry/DEMOCRATIC-VOTING-SYNTAX.md)に記載されています。現行コンパイラーでは、これは不活性なコメントメタデータとして扱われます。

### H–I

- **HTTP 1.0–8.0** — 利用可能なバージョン別HTTPパッケージディレクトリ（`lib/http-1.0/`以降）：バージョン固有のプロトコル仕様と実装。正確な範囲と検証状況は各バージョンの文書を確認してください。
- **International Criminal Court** — `lib/international-criminal-court/` および `INTERNATIONAL.CRIMINAL.COURTS.md`：関連する専門分野のソースと文書。

### O–S

- **OFDMA** — `ofdma/`：直交周波数分割多元接続に関する技術資料とC/C++開発リソース。
- **Philosophy and the Conversation Mask module** — [`philosophy/`](philosophy/README.md)にはネイティブC/C++基盤があり、[`philosophy/module/`](philosophy/module/README.md)は次のモジュール層を3領域に整理します。[data/](philosophy/module/data/README.md)は型付き・日付付き観測、イベント終了、半減期設定、来歴、同意を扱います。[helper/](philosophy/module/helper/README.md)は決定論的検証、関連性の減衰、互換性のある比較、同意に基づく提示を担います。[psychiatry-friend/](philosophy/module/psychiatry-friend/README.md)は支援的で非臨床的な会話インターフェースです。SLeeLa側のハンドルと統合Treeは、より広いphilosophyパッケージとともに[`lib/philosophy/`](lib/philosophy/README.md)にあります。このモジュールは明示的入力と同意を優先し、人やメディアを受動的に監視せず、診断、私的意図の推測、人間の価値の採点、服薬変更の推奨を行いません。イベントリンクは来歴とナビゲーションを保持しますが、独立した裏付けを意味しません。
- **Synchro** — ネイティブランタイム／コンパイラー統合、パケットディスパッチ、測定関連機能。関連する`impl/`ソースとコンパイラー実装を参照してください。
- **SleelaUI / terminal UI** — `lib/user-interface/` および`sleela-terminal`プロジェクト：SLeeLa UI統合と端末向け対話コンポーネント。

### U–W

- **United Nations** — `united-nations/`：国連に関するドメイン固有のリポジトリ資料。
- **Wi-Fi 4、Wi-Fi 5、Wi-Fi 6** — `wi-fi-4/`、`wi-fi-5/`、`wi-fi-6/`、関連する`lib/wifi/`ソースディレクトリ：世代別の無線ネットワーク資料と構成リソース。

### パッケージ索引の保守

パッケージを追加または大幅に拡張した場合は、適切なアルファベット区分に項目を追加し、正式なディレクトリとREADMEにリンクし、チェックイン済みソースで裏付けられる機能だけを説明してください。この索引は追加方式で保守し、パッケージ索引の更新を理由に既存のREADMEセクションを削除または書き換えないでください。

---

## XV. Bitcoin Conjegeum

<escape><img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"></escape>

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

`bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh` :: `⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓`

`bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc` :: `⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉`

US Democratic and US Policy.

---

## XVI. National Scienceブランド画像

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>

## XVII. UTF-4088 100万件実験カタログ

[UTF-4088カタログ生成ツール](utf-4088/tools/generate_catalogue.py)は、1,048,576件の実験的シンボルカタログを決定論的に生成します。コンパクト形式は[`utf-4088/FORMAT.md`](utf-4088/FORMAT.md)、生成および検証契約は[`utf-4088/GENERATION-RESULTS.md`](utf-4088/GENERATION-RESULTS.md)に記載されています。

Python 3で次のように生成します。

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

生成バイナリは16,777,244バイトで、20,000,000バイト未満に収まる設計です。ユニットテストとGitHub Actionsワークフローが、エントリー数、グリフの一意性、サイズ、SHA-256を検証します。成功した実行では、バイナリとサマリーが`utf4088-million-catalogue`ワークフロー成果物として公開されます。生成バイナリは意図的にGit履歴へ含めません。UTF-4088は実験段階であり、Unicode互換ではありません。

---

# SLeeLa

<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. 最近の追加機能 — Pi定数、Modium／Registryの起動、sleelactl Math、`@native`

このセクションは2026-11の作業で追加された機能を、新しいグループから順に記録します。すべて`master`にコミットされています。

### 1億桁のPiカタログ定数

リポジトリには、検証済みの**1億桁**のπ値[`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits)（正しい小数形式、`3.14159…`）と、57,344桁の[`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits)が含まれます。両方が真のπであることは独立に検証されています。検証内容（教科書的な先頭部分、小数第762位のFeynman point、著名な100万桁目、[`lib/math/SLPiReference.sleela`](lib/math/SLPiReference.sleela)との一致、ファイル全体の数字頻度分析）と一括再現スクリプトは[`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md)を参照してください。

### Modium™／Registry™はサブシステム起動時にPi定数を読み込む

専用サブシステムのエントリーポイント、すなわち`sleela registry`、`sleela modium`（およびインストーラーの初回実行）で、ModiumとRegistryは大きなπカタログ定数を読み込みます。`sleela run`の実行ごとには読み込みません。対話型端末では、一時的な赤色の`Loading Modium™`／`Loading Registry™`行を表示し、通常出力の前にその場で消去します。この表示はTTYの場合に限定されるため、パイプやテスト出力はバイト単位で変化しません。データ本体ではなくロケーターが、SLeeLaモデル（[`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela)）とネイティブシーダー（[`registry/sleela_registry.c`](registry/sleela_registry.c)）の両方で、Registryの`Math`キー既定値`piReferenceLargePath`および`piReferenceLargeDigits`として登録されます。`lib/math/SLPiReference.sleela`は`largePath()`／`largeDigits()`を公開します。π定数は標準の`make`インストーラーとガイド付き「beautiful」インストーラーの両方によって、OS上のSLeeLa配置先（`<data>/pi/`）にインストールされます。詳細は[`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md)を参照してください。

### `sleelactl` — beautifulインストーラーとネイティブバックエンドの`math` subject

- ガイド付き（「beautiful」）インストーラーは、`make`インストーラーと同様に、SLeeLaコントロールプレーンの`sleelactl`とそのモジュールレジストリをインストールするようになりました。[ドキュメント](bin/SLEELACTL.md)を参照してください。
- `sleelactl`には、ランタイムにコンパイルされるネイティブC/C++ subjectライブラリ向けの新しい`subject`種別が追加され、最初のsubjectとして`math`が登録されました。`sleelactl run math`は、subjectの参照デモを通じてネイティブ`__NativeMath`バックエンド（`impl/subjects/math/math_native.cpp`）をエンドツーエンドで実行します。`sleelactl info math`は到達方法（`import math;`）を説明します。subject種別が提供する動詞は`run`と`info`のみです。`sleela check`はSHEET登録のネイティブsubjectをロードしないため、意図的に`check`動詞はありません。

### `@native`と`sleela aot` — SLeeLaからネイティブC/C++への経路、第1段階

新しい**`@native`**メソッド注釈は、ホットメソッドのAhead-of-Time（AOT）ネイティブコンパイルを要求します。新しい`sleela aot`コマンドは`@native`メソッドを検出し、各メソッドが対応する*オフロード可能なサブセット*内にあるかを検証して、C/C++を出力します。翻訳できない構文があれば、正確な理由を示して**fail closed**（拒否）します。不完全なメソッドを生成することはありません。

```sh
sleela aot --list <file.sleela>    # メソッドごとの判定：ok / REFUSED（理由）
sleela aot --emit <file.sleela>    # サブセット内のプログラムについてC/C++を標準出力へ出力
```

現在、`@native`は動作を変えません。プログラムは従来どおりチェック・実行され、注釈はネイティブ処理を**要求するだけ**です。これは第1段階であり、注釈の表面と、対応範囲を正直に制限するゲートを確立します。出力されたC/C++を共有オブジェクトへコンパイルし、インタープリターへのフォールバックを備えてVMから呼び出す機能は後続段階です。詳細、対応サブセット、終了コードは[`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md)を参照してください。

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

**翻訳ポリシー：** これらのファイルは、最初の段階では各国向けにローカライズされたエントリーポイントです。導入部と見出しは各言語にローカライズされていますが、技術リファレンス全体は英語の正式版[README.md](README.md)にあります。すべてのセクションを翻訳し、技術識別子とリンクを確認し、言語レビューを受けるまでは、国別ファイルを完全翻訳版と表記してはなりません。この初期段階の言語範囲は代表的なものであり、憲法上認められるすべての地域語、先住民語、手話が完全に翻訳されたことを意味しません。

## 翻訳状況

**作業中 — 部分翻訳。** 日本語版ではREADMEのセクションI～XIXを翻訳しました。技術識別子、コマンド、コード例、ファイル名、URL、アドレス文字列は原文どおり保持しています。公開前にネイティブスピーカーによる言語レビューと技術リンク確認を推奨します。

