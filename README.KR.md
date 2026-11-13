# SLeeLa 3.0 — 대한민국 (KR)
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

<p align="center"><strong>네이티브 시스템 언어, 컴파일러, 런타임 및 가상 머신 생태계.</strong></p>
<p align="center"><a href="https://github.com/mearvk/SLeeLa/actions"><img alt="GitHub Actions" src="https://img.shields.io/github/actions/workflow/status/mearvk/SLeeLa/test-suite.yml?branch=main&label=test%20suite"></a> <a href="docs/PLATFORM-SUPPORT.md"><img alt="Platform matrix" src="https://img.shields.io/badge/platforms-Linux%20%7C%20macOS%20%7C%20Windows-273238"></a></p>

> **국가별 국제판(한국어).** 이 파일은 한국어 국가별 에디션으로, README의 I~XIX 섹션을 한국어로 제공합니다. 기술 식별자, 명령, 코드, 파일명 및 링크는 원문 그대로 유지합니다.

**The Now. The Ingredient.** 프로그래밍 언어는 문법만으로 이루어지지 않습니다. 소프트웨어를 실제로 사용할 수 있게 하는 컴파일러, 런타임, 라이브러리, 도구 및 플랫폼 계약도 언어의 일부입니다.

**SLeeLa의 활동 영역.** SLeeLa는 **Math**, **Object**, **Science**, **Reach**라는 네 가지 측면을 다룹니다. Math는 계산 가능한 것, Object는 존재하는 것의 목록에 자리 잡은 확정된 대상, Science는 진리에 대한 체계적인 연구, Reach는 시스템의 기능을 다른 시스템과 사람들에게 확장하는 것을 뜻합니다. **국가적 이익은 계속 유지됩니다.** 네 영역 중 어느 것도 국가적 이익을 배제하지 않습니다. 어떤 주제가 한 국가의 이익과 관련되면 그 관계를 보존하며, 예를 들어 각 클래스의 Metadata 필드인 `metaOrigin` 및 `metaNationalSchedule`에 충실하게 기록합니다.

## I. 시작하기

- [네이티브 구현 빌드](impl/README.md)
- [가상 머신 및 Modium — 실습 사용자 안내서](virtual-machine/USER-GUIDE.md) (`sleela vm`, `sleela modium`)
- [SLeeLa 소스의 CPU 스택](lib/cpu/CPU.md): 논리 게이트에서 ALU, 코어, OS, 프로그램까지 이어지는 전체 기계 구조, 130개 이상의 CPU 프로필, 조정 가능한 [전압·가변 클록·전문가용 타이밍](lib/cpu/TIMING.md), 그리고 [사양에 따른 실행 및 오버클로킹](lib/cpu/components/README.md).
- [문서 색인](docs/README.md)
- [CSPML 데이터 소스 및 Salt & Salary 연구 참조](CSPML.DATA.SOURCE.AND.SALT-AND-SALARY.REFERENCE.md): 미국 지역, 사회경제, 노동력, 교육 및 공립학교 관련 외부 데이터의 출처와 통합 지침.
- [생성된 API 문서](docs/api/README.md): 최신 HTML 참조, 버전별 API 에디션 및 재생성 안내.
- [API 카탈로그](api/README.md)
- [계산 비교 표준](comparison/README.md): SLeeLa VM, Java/JVM 및 네이티브 C/C++의 타입, 메모리, ABI 및 적합성 표준.
- [재귀 스탠션 — C 및 C++ 프로토타입](recursion/stanchion/README.md): Boolean, Byte, Character, Integer, Long, Float, Double의 기본 타입 계약별 예제와 일반·응용·과학·교육용 재귀 패턴.
- [플랫폼 지원 및 검증 매트릭스](docs/PLATFORM-SUPPORT.md)
- [브랜드 가이드](docs/BRAND-GUIDE.md)
- [상품 콘셉트 및 에셋](merchandise/README.md)
- [NitroWebExpress 로고](images/nitro-web-express/nitrowebexpress-logo-004.jpeg)
- [테스트 정책 및 릴리스 게이트](markdown/TESTING.md)

## II. 최근 추가 사항

언어, 드라이버 및 표준 라이브러리에 최근 추가된 기능:

- **표준 라이브러리 `advantage` 패키지 — 수학, 물리, 통합, 등급 평가, 라우팅 가능한 출력을 갖춘 배당의 우위화(advantaging).**
  [`lib/advantage/`](lib/advantage/README.md)는 *모델에 대한 고유 배당을 우위화하는 것*을 계층형 **Sequitur onion**(dividend → review → concerns → maths → additions → subtraction → regrouping → iteration)으로 캡슐화합니다. 안쪽과 바깥쪽으로 이동할 수 있고 완전한 동적 갱신과 restitute(복원)를 지원하며, 간단한 **사용자 모델**과 전체 제어가 가능한 **개발자 모델**로 노출됩니다. 다섯 가지 주제 영역을 포함합니다. **`math/`**는 양자 간 무역 경제학(공동 이익 최대화, 내시 협상, 양자 뉴스벤더 임계 비율), **`physics/`**는 구조경제학의 "신뢰 필드 물리학"(계량 텐서로서의 신뢰, 거래 측지선, 발표 접촉 구조, CIR+점프 신뢰 감쇠 SDE — 모델링 유추이며 그렇게 표시되어 있음), **`integration/`**은 자본주의/소매업을 위한 재귀적으로 구성 가능한 수직 통합 모델(구획, 엔지니어링 베이, 시험 구획, 뉴턴 중앙값 트레이, 시스템 정교성 점수), **`grading/`**은 각 모듈과 승리/패배/채점기/절대 모듈의 임의 집합을 A–D로 평가하는 정교성 회로 채점기, **`output/`**은 모듈 출력을 터미널, **CommonRails** 형식 표면 또는 소켓/커넥터(네이티브 `slnet_write`용으로 프레이밍)로 보내는 라우팅 가능한 emit 접합부입니다. 모든 클래스는 Metadata-Class 레코드를 가지며, 실행 가능한 다섯 개의 데모가 각 주제를 검증하고, 보조 문서인
  [`DEFINITIONS.md`](lib/advantage/DEFINITIONS.md),
  [`MATH.md`](lib/advantage/MATH.md),
  [`PHYSICS.md`](lib/advantage/PHYSICS.md)가 이를 설명합니다. [`advantage/`](advantage/README.md) 아래의 네이티브 C/C++ 소비자(C 코어 `advantage_value`/`advantage_mean`, XML 지식 저장소, 보안 HTTPS/SFTP/SSH 수집 + 키스토어 + DH/ECDH 전송 라이브러리)는 모델을 실제
  [거래 예제](advantage/examples/README.md)에 연결합니다. 예제는 추정된 인터넷 출처에서 나온 긍정적 거래, 대상 관련성 순서(Traders, Consumers, Helpers, Healers), 그리고 A–D로 순위를 매긴 여덟 개의 모델(D = 가장 Cement, A = 가장 Brand)입니다.
- **루프 타이밍 주석 — `@loopDelay` / `@loopBudget`(+ 내장 함수 `sleep(ms)`).** SLeeLa 최초의 **문장 수준 동작** 주석입니다. `for` / `while` / `do` 루프 앞의 독립된 줄에 작성하며, 루프 실행에 실제 대기 시간을 삽입합니다. `@loopDelay(ms = 250)` / `@loopDelay(seconds = 2)`는 반복마다 고정된 시간만큼 대기하고(인수만 쓴 `@loopDelay(250)`은 밀리초로 간주), `@loopBudget(seconds = 10, iterations = 1000)`은 알려진 반복 횟수를 전체 실제 시간 예산에 걸쳐 고르게 분배합니다. 둘 다 새로운 추가 전용 opcode **`OP_SLEEP`**(ISA 157)로 낮춰지며, 어디서든 호출할 수 있는 범용 내장 함수 **`sleep(<ms>)`**도 이 opcode를 사용합니다. 대기는 이식 가능한 방식(`nanosleep` / `Sleep`)으로 구현됩니다. 사양은 [`SLEELA.syntax` §32](SLEELA.syntax), API 참조는 [`api/sleela/3.0/LOOP_TIMING_API.md`](api/sleela/3.0/LOOP_TIMING_API.md), 실행 가능한 예제는
  [`examples/loop-timing-annotation-demo.sleela`](examples/loop-timing-annotation-demo.sleela)입니다.
  세션 추가 사항 색인(이 항목, 네이티브 `OP_BIGMUL` 큰 정수 곱셈, 독립 실행형 도구 **[local-generator](local-generator/README.md)** 및 **[pi-generator](pi-generator/README.md)** 포함)은
  [`api/sleela/3.0/SESSION-ADDITIONS-API.md`](api/sleela/3.0/SESSION-ADDITIONS-API.md)에 있습니다.
  생성된 세 개의 API 표면
  ([`sleeladoc-api/`](sleeladoc-api/README.md),
  [`docs/api/`](docs/api/README.md), 그리고 불변의
  [`api/sleela/3.0/version-014/`](api/sleela/3.0/version-014/index.html)
  스냅샷)은 `make api-docs`로 다시 빌드되었으며, `vm` 패키지 페이지에 새로운 `SLISASleep` / `SLISABigMul` 명령 항목이 추가되었습니다.
- **명령줄의 SleelaDoc — `sleela docs`.** 이제 터미널에서 SleelaDoc을 바로 사용할 수 있습니다. `sleela docs`는 문서화 가능한 모든 주제를 나열하고, `sleela docs crypto`는 패키지를 보여 주며, `sleela docs stringbuilder`는 클래스를(대소문자를 구분하지 않고, 표준 라이브러리 접두사 `SL`을 통해 `SLStringBuilder`로) 찾아 풍부한 페이지를 출력합니다. 페이지에는 **정의, 설계 목적, 관계, 구조, 사용자 clay, 5단계 숙련도 사다리**가 포함되며, 비슷한 철자의 경우 순위가 매겨진 "혹시 이것을 찾으셨나요" 결과가 표시됩니다. 작성자는 SleelaDoc `@` 태그(`@intent`, `@thesis`, `@axiom`, `@param`, `@see`, `@mastery`)를 추가하는 것만으로 어떤 페이지든 보강할 수 있습니다. `sleela docs --generate [dir]`는 (클래스당 한 페이지 + `TOPICS.index`로 이루어진) 일반 텍스트 코퍼스를 작성하고, CLI는 이를 오프라인에서 읽습니다. **`make sleeladoc`**은 그 코퍼스**와 해당 `.sleela` 소스**를 `sleeladoc/build/sleeladoc.jar`에 묶습니다. 퍼사드 전용 주제(예: `psychiatry`, `politico`)의 경우 `sleela docs --facade <topic>`이 런타임에 파일 시스템을 검색하여 실제 **폴더(그다음 자연스러운 흐름으로 파일)**를 나열하며, 사용자가 재정의할 수 있는 한도인 **6 MB / 6000개 파일 / 6분** 안에서 동작합니다. `lib/sleeladoc` 단위는 현재 에디션인 **`#sleela 3.0`**으로 유지됩니다. [`man/man1/sleela.1`](man/man1/sleela.1)과 [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md)를 참조하세요.
- **SleelaDoc — 생성된 HTML 사이트를 갖춘 Javadoc 스타일 문서화 솔루션.** **SleelaDoc**은 SLeeLa의 소스 내 문서 주석 규약(`@doc` / `@page`를 표지로 하는 꺾쇠 괄호 주석, [`manifesto/SLEELADOC.md`](manifesto/SLEELADOC.md) 참조)이며, [`lib/sleeladoc`](lib/sleeladoc/README.md)은 `javadoc`이 Java를 렌더링하는 것과 같은 방식으로 `.sleela` 단위를 Markdown 또는 HTML API 페이지로 렌더링하는 **추출기**로서 *의도*를 문서화합니다. **전체 `/lib`에 대한 탐색 가능한 완전한 HTML 문서 사이트**는 `tools/generate-sleeladoc-api.py`에 의해 [`/sleeladoc-api`](sleeladoc-api/README.md)에 생성됩니다(`sleeladoc-api/index.html`을 여세요. 파랑 / 연한 파랑 / 연한 회색 / 흰색의 깔끔한 스타일, 패키지 → 클래스 → 멤버 계층 구조, *Student 2* 독자를 위해 작성된 용어집을 제공합니다). 컴파일러는 컴파일의 부산물로 작성자가 표시한 문서 출력(`--wiki` MediaWiki 페이지 + 원본 소스의 쌍둥이)도 내보내며, 이는 [`manifesto/SLEELADOC.WIKI.md`](manifesto/SLEELADOC.WIKI.md)에 문서화되어 있습니다. 실행 가능한 예제:
  [`lib/sleeladoc/sleeladoc-demo.sleela`](lib/sleeladoc/sleeladoc-demo.sleela).
- **XML Document Object Model(`lib/xml`).** SLeeLa 측 XML DOM으로, 요소/텍스트/주석/CDATA/처리 명령 노드의 트리를 소유하며 순서 있는 자식 노드와 이름 있는 속성을 가진 문서, 직렬화기(압축/정형, XML 1.0 §2.4 이스케이프), 문자 셀 파서, 품질 레지스터 `SLXmlAspect`를 제공합니다. 노드 동등성은 신뢰의 세 단계(동일성 → 동일 주제 → 구조)로 판단되며 반사적·대칭적·결정적입니다. [`lib/xml/README.md`](lib/xml/README.md)와 실행 가능한
  [`lib/xml/xml-dom.sleela`](lib/xml/xml-dom.sleela)를 참조하세요.
- **확장 부호 있는 정수 기본형(`long`, `int64`, `int128`).** `int`, `long`, `int64`는 모두 64비트 부호 있는 정수이며 타입 검사와 실행이 동일합니다. `int128`은 정수 타입으로 허용됩니다(현재는 64비트 기반이며 실제 128비트는 문서화된 후속 작업). 2진 부호 있는 계열 `int32`..`int16777216`(구문 2.7)과 대응하는 부호 없는 계열 `uint32`..`uint16777216`(구문 2.8)이 이를 확장합니다. [`SLEELA.syntax`](SLEELA.syntax) §21, §24, §25를 참조하세요.
- **`@mix` 연속(consequent) 필드(구문 2.9).** 반복되는 각 선언에 `@mix`가 붙어 있으면 클래스는 **같은 이름**의 인스턴스 필드를 여러 개 선언할 수 있습니다. 언어는 이 그룹을 연속된 것(인덱스를 키로 하는 배열/리스트)으로 이해하며, 기본적으로 위에서 아래로 `1,2,3,…` 순서이고 수동으로는 `@mix(1,2,3,...)`를 쓰되 인덱스 수가 필드 수와 일치해야 합니다. [`SLEELA.syntax`](SLEELA.syntax) §28을 참조하세요.
- **문서 마커 — Natural Process, 명칭은 Preonems(`author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`).**
  이 단독 문서 마커 단어들은 통틀어 **Preonems**라고 부릅니다(마커 하나는 *Preonem*, 집합은 *Preonems* — [`SLEELA.syntax`](SLEELA.syntax) §26.1 참조). 문서가 만들어지는 순서에서 문서가 *무엇인지*를 나타내는 일급의 선언 전용 문서 마커입니다. `author`/`authorial`(구문 2.4, 전문가 위원회가 처리하며 동작상 효과를 가진 유일한 구성원), `material;`(확정된 Source)과 `memory;`(구문 2.5), `determine;`(구문 2.6, 의사·의료인 또는 ghost가 추가로 처리하여 법적 손길에 의한 완성에 이르도록 열려 있는 문서), 그리고 가장 새로운 둘인 **`object;`**(존재하는 것의 목록에 자리 잡은 확정된 대상 — **구문 2.8에서 구체화되어 구체적 지위를 부여받음**)와 **`field;`**(대상들이 서고 움직이는 관계의 토대)입니다. 이 둘은 구문 2.6부터 게이트가 적용됩니다. 각각 단독으로 쓰이고 문맥에 의존하며 프런트엔드가 구문 버전으로 제어하고, 파싱·타입 검사·코드 생성에는 영향을 주지 않습니다. 마커 위치 밖에서는 모두 일반 식별자로 남습니다(따라서 `field`는 클래스 멤버나 `.field` 접근을 방해하지 않습니다). [`SLEELA.syntax`](SLEELA.syntax) §18–§20, §23, §26을 참조하세요.
- **구문 3.0의 Nordshrift와 전체 라이브러리 접근.** Nordshrift는 컴파일러와 동일한 재귀적 `/lib` 색인을 통해 해석하여 모든 패키지와 `.sleela` 단위에 도달합니다. `nordshrift check`는 전체 트리의 개수를 보고합니다.
- **재귀 스탠션.** [`recursion/stanchion/`](recursion/stanchion/README.md) 디렉터리에 문서화된 7개 기본 타입 계약을 위한 전용 C 및 C++ 참조 프로토타입과 일반·응용·과학·교육용 예제가 추가되었습니다. 이들은 참조 소스입니다. 검증된 것으로 취급하기 전에 컴파일하고 테스트하세요.

- **꺾쇠 괄호 주석(`<` / `<<`).** 새로운 앞으로의 주석 스타일인
  `< line >` 및 `<< block >>`을 SLeeLa 프런트엔드가 어휘 분석하며,
  [`SLEELA.syntax`](SLEELA.syntax) §1.1에 문서화되어 있습니다.
- **기수 정수 리터럴.** `int` 리터럴은 이제 `_` 자릿수 구분자와 함께 16진수(`0x`), 2진수(`0b`), 8진수(`0o`) 형식을 허용합니다. SLeeLa 소스 언어([`SLEELA.syntax`](SLEELA.syntax) §1.4)와
  [`.xlt` CPU 모델 형식](lib/xlt/README.md) 모두에서 사용할 수 있습니다.
- **라이브러리 전반의 Metadata Class 표준.** 설명형 메타데이터 메타패턴(출처, 적법성, 관할 범위)이 이제 `/lib` 아래 클래스가 포함된 모든 `.sleela` 파일에 적용됩니다. [`SLEELA.syntax`](SLEELA.syntax) §15 및
  [튜토리얼 과](tutorial/13-metadata-class.md)를 참조하세요.
- **더 풍부한 텍스트 / String 계열.** `lib/text`는 언어 내의 실용적인 도우미, 실제 `charAt`/`substring`/`indexOf`/`reverse`/열 정렬을 제공하는 문자 셀 뷰, 그리고 실제 위치 기반 템플릿 기능을 추가했습니다. [텍스트 및 String 튜토리얼](tutorial/14-text-and-strings.md)과 실행 가능한
  [`lib/text/text-string-demo.sleela`](lib/text/text-string-demo.sleela)를 참조하세요.
- **Nordshrift 시트의 일급 `String`.** `.sst` 시트는 타입이 지정된 `strings:` 블록을 선언할 수 있으며, 이는 빌드의 구성요소 매니페스트에 반영됩니다.
  [Nordshrift Sheet Reference](wiki/Nordshrift-Sheet-Reference.md)를 참조하세요.
- **opcode와 함수 호출 동등성으로 실행되는 CPU 모델.** 모든 `SL<ARCH>CPU`는 fetch-decode-execute 루프를 통해 네이티브 명령 스트림을 실행하는 기반 *그리고* 공유 VM 기반에서 동등한 호출로 가정된 다른 프로세스의 코드를 실행하는 기반을 모두 상속합니다 — [`lib/cpu/EXECUTION.md`](lib/cpu/EXECUTION.md) §5를 참조하세요.

## III. 빌드 및 테스트

**필수 항목:** GCC 또는 Clang과 같은 C/C++ 도구체인, GNU Make, 매니페스트 및 흐름 도구용 Python 3가 필요합니다. Linux에서 디스플레이 제품을 빌드하려면 X11/Xrandr 개발 패키지도 필요합니다(각 모듈의 `make deps`가 필요한 의존성을 보고하거나 설치합니다). `impl/` 빌드는 신뢰된 SHA-256 매니페스트를 충족하지 못하면 실패하도록 설계되어 있으며, 루트 `Makefile`이 해당 매니페스트를 자동으로 참조합니다.

깨끗한 체크아웃에서 가장 빠르게 시작하려면 다음을 실행합니다.

```sh
make                                  # 저장소 루트에서 전체 빌드
```

네이티브 핵심만 빌드하고 테스트하려면 다음을 실행합니다.

```sh
make -C impl all                      # 핵심 컴파일러/런타임만 빌드
bash ./test-suites/run-all.sh --all   # 전체 테스트 스위트 실행
```

저장소 루트의 `make`는 코어, VM, 코덱, churn, antivirus 등을 포함한 모든 제품을 빌드하고 마지막에 **레지스트리**를 빌드하여 기준 테이블을 생성합니다. 주요 타깃은 다음과 같습니다.

```sh
make                     # 전체 빌드(make all의 별칭이며 레지스트리는 마지막에 실행)
make all                 # 동일한 전체 빌드를 명시적으로 실행
sudo make install        # 명령 및 런타임 지원 파일 설치(§IV 참조)
make clean               # 각 빌드 시스템의 생성물 제거
make help                # 루트 Makefile의 모든 타깃 표시
```

그 밖의 유용한 타깃:

```sh
make registry            # 레지스트리만 빌드하고 테이블 생성
./build-registry.sh      # 동일한 작업을 스크립트로 직접 실행
make tests               # 테스트 스위트 빌드 및 실행
make ballum              # 현재 운영체제용 Ballum 디스플레이 라이브러리 빌드
bin/install-modules      # 각 모듈을 단계적으로 설치/빌드(약 10~20분)
```

`registry` 타깃은 [`build-registry.sh`](build-registry.sh)를 실행합니다. 레지스트리 코어를 빌드하고 기준 테이블을 이식 가능한 텍스트 저장소 `registry/build/sleela.sleelareg`에 채운 다음, `sleela registry export`를 통해 MySQL 테이블 스크립트 `registry/build/registry.sql`을 내보냅니다. `sleela registry` 또는 `registry/build/sleela-registry review`로 검토할 수 있습니다. [레지스트리 안내서](registry/README.md)와 [Registry 3.0 아키텍처](registry/REGISTRY-3.0-ARCHITECTURE.md)를 참조하세요.

빌드 및 테스트 안내서에서 필수 조건과 플랫폼별 절차를 확인하세요. 한 구성요소의 빌드 성공이 모든 라이브러리와 플랫폼의 릴리스 준비 완료를 의미하지는 않습니다.

## IV. 설치

네이티브 제품을 빌드한 후 명령과 런타임 지원 파일을 설치합니다.

```sh
sudo make install
make install PREFIX="$HOME/.local"       # sudo 없는 사용자 로컬 설치
make install DESTDIR="$PWD/package-root" # 패키지 스테이징 설치
make install-check
```

기본 설치 경로는 `/usr/local/bin`, `/usr/local/lib/sleela`, `/usr/local/share/sleela`입니다. 기존 시작 설정을 보존하고 추적되지 않는 기존 명령을 덮어쓰지 않습니다. 제거 작업은 설치 프로그램이 기록한 파일만 삭제하며 설정과 추적되지 않는 런타임 데이터는 보존합니다.

주요 패키지(`sleela`, `sleelvac`, `nordshrift` 실행 파일, 표준 라이브러리, 프로토콜 및 지원 모듈)만 안내에 따라 설치하려면 다음을 사용합니다.

```sh
make clean-install       # 주요 패키지만 안내형 설치
```

이 절차는 먼저 주요 패키지 설치 여부를 묻고, 다음으로 설치 범위를 선택하며, 성공 후에는 엄선된 확장 모듈을 선택적으로 설치할 수 있게 합니다. 대화형 배포 절차와 모든 설치 옵션은 [Quick and Safe Install](install/README.md)을 참조하세요.

## V. 언어 및 런타임

공식 문법은 [`SLEELA.syntax`](SLEELA.syntax)에 정의되어 있습니다. 저장소가 문서화하는 현재 문법 버전은 **3.0**입니다. 컴파일러/도구 버전과 소스 문법 버전은 서로 다릅니다. 문법 3.0에는 **`pure` 및 `gasoline` 문서 마커**(§30)가 추가되었습니다. 두 마커는 문서의 지위를 각자의 정직성에 따라 나타내는 문맥 의존적 선언형 Preonem입니다. 문법 3.0은 1.0~2.9의 언어 표면을 통합하면서 각 기능별 최소 버전 조건을 유지합니다. 이전 2.9 버전에는 **`@mix` 연속 필드**(§28)가 추가되어, 같은 이름의 인스턴스 필드 여러 개를 하나의 인덱스 그룹으로 취급할 수 있습니다(기본 `1,2,3,…` 또는 그룹 필드 수와 일치하는 `@mix(1,2,3,...)`). 또한 `long`/`int64`/`int128`(§21)을 바탕으로 한 부호 있는 정수 계열 `int32`~`int16777216`(§24)과 부호 없는 정수 계열 `uint32`~`uint16777216`(§25), 키워드 지원 문서(§22), 그리고 Preonems가 이어집니다. Preonems에는 `author`/`authorial`(§18), `material`/`memory`(§19~§20), `determine`(§23), `object`/`field`(§26), `pure`/`gasoline`(§30), 시간과 공간에 걸쳐 입법부 수준의 지위를 나타내는 `artifact`(§35)가 포함됩니다. 전체 명칭 Preonems는 §26.1에 정의되어 있습니다. 생성자 오버로드 해석과 `this(...)`/기반 클래스 생성자 위임은 검증된 문법 1.8 계약의 범위 밖입니다. 기능에 의존하기 전에 문법 및 구현 참고 자료를 확인하세요. SleelaDoc의 아홉 소스 단위는 현재 `#sleela 3.0`을 선언하며, 공통 컴파일러 게이트는 소스 문법 1.0~3.0을 허용합니다. 새 API 빌드는 `make api-docs`로 생성됩니다.

저장소에는 네이티브 C/C++ 구현, SLVM 생성 참조, 표준 라이브러리 모듈, 테스트, 예제 및 플랫폼 통합이 포함되어 있습니다. 구현 및 검증 상태는 모듈마다 다릅니다. 모든 모듈이 동일하게 완전하다고 가정하지 말고 해당 사양과 현재 CI 결과를 확인하세요.

## VI. 엔지니어링 원칙

- **주장보다 증거:** 구현됨, 테스트됨, 실험적, 계획됨 상태를 구분합니다.
- **경계마다 하나의 계약:** 구문, API, 파일 형식, 플랫폼 전제 조건 및 실패 동작을 문서화합니다.
- **설계 단계부터 이식성:** 플랫폼별 백엔드를 분리하고 네이티브 실행 환경에서 테스트합니다.
- **안전한 실패:** 잘못된 입력, 리소스 한도, 프로세스 실패 및 정리 경로를 테스트합니다.
- **재현 가능한 릴리스:** 깨끗한 상태에서 다시 빌드하고 아티팩트 해시와 릴리스 노트를 게시합니다.

## VII. Antivirus 빌드 및 무결성

Sleelavirin antivirus 패키지에는 네이티브 SHA-256 스캐너, 선택적 ClamAV 어댑터, 소스/색인 무결성 매니페스트 및 플랫폼 패키지 체크섬이 포함됩니다. 저장소 루트에서 실행합니다.

```sh
make antivirus
make antivirus-test
make antivirus-integrity
make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json
```

무결성 매니페스트는 `antivirus/build/`에 생성되며 추적되는 소스와 문서, 보호된 서명 색인 파일을 포함합니다. 생성된 출력물과 변경 가능한 ClamAV 캐시는 제외합니다. ClamAV 데이터베이스 스냅샷에는 별도 매니페스트가 있습니다. 릴리스 ZIP에는 `INTEGRITY-MANIFEST.json`, `SHA256SUMS`, `.sha256` 사이드카 파일이 포함됩니다. 다운로드한 아카이브의 해시는 독립적으로 확보한 신뢰 가능한 값과 대조해야 합니다. 아카이브와 함께 저장된 체크섬은 게시자 서명이 아닙니다. 자세한 내용은 [Sleelavirin 안내서](antivirus/README.md), [보안 체크리스트](antivirus/SECURITY.md), [antivirus 빌드 워크플로](.github/workflows/antivirus-binaries.yml)를 참조하세요. 해당 CI 작업이 정확한 릴리스 커밋에서 통과하고 결과 아티팩트가 검사되기 전에는 릴리스를 검증된 것으로 간주하지 않습니다.

## VIII. SLeeLa 편집기 테마 — Vim 및 Emacs

SLeeLa에는 **Vim**과 **Emacs**용으로 조정된 전문 다크 테마와 네이티브 C/C++ 팔레트 유틸리티가 포함됩니다. 기본 테마는 **American Professional**입니다. 문화적·제도적 디자인에서 영감을 얻은 보조 팔레트 10종도 제공되지만, 어떤 국가·문화·법원·기관의 공식 표현도 아닙니다.

### 11개 스킴과 색상

| 스킴 | 배경 | 전경 | 키워드 | 타입 | 문자열 | 주석 |
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

#### 국가 국기 색상(기준 팔레트)

Greek 및 Turkish 스킴은 두 나라의 **공식 국기 색상**에서 유래했으며, 공표된 정확한 16진수 값과 함께 표시합니다. 위 스킴의 배경/강조색은 이 원래 색상을 어둡게 하여 편집기에 맞게 해석한 것이며, 아래 표는 "국가적으로 공표된 색상" 참조입니다.

| 국가 | 색상 | 16진수 | 근거 및 의미 |
|---|---|---|---|
| **그리스** | Blue | <span style="display:inline-block;width:1em;height:1em;background:#0D5EAF;border:1px solid #888"></span> `#0D5EAF` | 국기의 파랑(법률 851/1978). 하늘과 바다를 뜻하며 문장학상 *azure*로 기술됩니다. |
| **그리스** | White | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | 국기의 흰색. 독립 투쟁의 순수함을 뜻하고 십자가는 동방 정교회를 나타냅니다. |
| **Türkiye** | Red(*al bayrak*) | <span style="display:inline-block;width:1em;height:1em;background:#E30A17;border:1px solid #888"></span> `#E30A17` | 국기의 빨강(터키 국기법). 바탕색이며 전통적으로 순교자의 피와 오스만 주권을 뜻합니다. |
| **Türkiye** | White | <span style="display:inline-block;width:1em;height:1em;background:#FFFFFF;border:1px solid #888"></span> `#FFFFFF` | 국기의 흰색. 초승달과 별은 평화, 순수함, 튀르크/이슬람 유산의 상징입니다. |

각 스킴은 편집기 전경색을 국가의 **흰색**에, 키워드·커서·선택 영역·상태 표시줄을 정확한 국가 **파랑**(`#0D5EAF`) 또는 **빨강**(`#E30A17`)에 고정하므로, 중요한 색상은 말 그대로 국가의 색입니다. 이는 해석에 따른 디자인 팔레트이며, 국기 색상 참조는 출처를 밝히기 위해 제공되는 것으로 어느 국가의 공식 표현도 아닙니다. 색상의 의미는 그리스 공화국 외교부의 국가 상징 설명과 터키 국기법의 전통을 따릅니다.


### 색상 스와치 표

아래 팔레트 셀은 **SVG 스와치**를 사용하므로 GitHub에서 빈 HTML 색상 셀이 아니라 실제 색상이 렌더링됩니다. 각 띠는 배경, 전경, 키워드, 타입, 문자열, 주석의 여섯 색상을 이 순서로 보여 줍니다. 정확한 16진수 코드는 위 참조 표와 같은 스킴 순서로 각 스와치 옆에 나열되어 있습니다.

| 스킴 | 렌더링된 팔레트 색상 | 16진수 코드(배경 · 전경 · 키워드 · 타입 · 문자열 · 주석) |
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

함수, 상수, 선택 영역, 진단 및 전처리기 지시문에는 추가 강조색이 사용됩니다. Russian and Slavic 팔레트는 더 차분한 전문적 외관을 위해 중간 강도의 슬레이트, 파랑, 세이지, 루비, 앤티크 골드를 의도적으로 사용합니다.

### Vim 스킴 설치

저장소 루트에서 실행합니다.

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

SLeeLa 구문 강조를 활성화하고 스킴을 선택하려면 `~/.vimrc`에 다음을 추가합니다.

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

다음 colorscheme 이름 중 정확히 하나를 선택하세요.

- `sleela-american`
- `sleela-eurasia`
- `sleela-russian-slavic`
- `sleela-silk-road`
- `sleela-east-asian`
- `sleela-greek`
- `sleela-turkish`
- `sleela-kiro`
- `sleela-sleela`

예를 들어 중간 강도의 Russian and Slavic 팔레트에는 `colorscheme sleela-russian-slavic`을, Hellenic 및 Al Bayrak 팔레트에는 `colorscheme sleela-greek` / `colorscheme sleela-turkish`를 사용합니다. Vim을 다시 시작하거나 `:source $MYVIMRC`로 설정을 다시 불러오세요.

### Emacs 스킴 설치

로컬 테마 디렉터리를 만들고 저장소 루트에서 모드 및 테마 파일을 복사합니다.

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

`~/.emacs` 또는 `~/.emacs.d/init.el`에 다음을 추가합니다.

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

다른 스킴을 선택하려면 `sleela-american`을 `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro`, `sleela-sleela` 중 하나로 바꿉니다. Emacs가 테마를 신뢰할지 묻는다면 수락하기 전에 로컬 테마 파일을 확인하세요.

### 네이티브 C/C++ 팔레트 미리 보기

작은 네이티브 팔레트 유틸리티는 스킴을 나열하고, 터미널 색상을 출력하며, 개별 색상을 조회할 수 있습니다.

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

하위 호환성을 위해 `--theme` 없이 `--palette` 또는 `--hex COLOR`를 실행하면 American Professional이 선택됩니다. 빌드와 테스트 결과는 대상 시스템과 CI에서 확인해야 합니다. 명령이 문서화되어 있다는 사실 자체가 그 명령이 통과했다는 증거는 아닙니다.

전체 팔레트 설명과 유틸리티 세부 사항은 [편집기 통합 안내서](linux-editors/README.md)를 참조하세요.

## IX. 기여하기

1. 문서 색인과 테스트 정책을 읽습니다.
2. 성공 및 실패 테스트를 포함하여 범위를 좁힌 변경을 수행합니다.
3. 대상 테스트를 실행한 뒤 전체 테스트 스위트를 실행합니다.
4. OS 및 도구체인 정보를 보고하고, 건너뛴 검사를 통과한 것으로 설명하지 않습니다.

SLeeLa는 **MEARVK LLC**가 개발합니다. 과거 랜딩 페이지 자료는 [`docs/README-LEGACY.md`](docs/README-LEGACY.md)에 보존되어 있습니다.

## X. AE6E66 — Round Public 이메일 부과금 예상

AE6E66 이니셔티브에서 프로젝트가 제시하는 예상액은 자격을 충족하는 각 Round Public 이메일에 연계된 제안 세금 부과액으로 **이메일 한 건당 미화 300,000,000.00달러**입니다.

이 문구는 프로젝트의 예상만을 기록합니다. **법적 세금 부과 결정, 정부 명령, 법원 판결 또는 독립적으로 확정된 납세 의무가 아닙니다.** 실제 부과에는 적용 가능한 법적 권한, 관할권, 정의, 절차 및 권한 있는 정부 기관의 결정이 필요합니다.

## XI. 알파벳 및 문자 집합 런타임

SLeeLa의 알파벳 문서와 네이티브 문자 런타임은 `alphabet/sleela/` 및 `lib/alphabet/native/`에서 관리됩니다. 네이티브 C++17 테스트는 엄격한 UTF-8 스칼라 디코딩/인코딩, `C<n>` 용량 시그니처, 경계가 제한된 문자 집합 포함 여부를 검사합니다.

```sh
make -C lib/alphabet/native test
```

실험적인 UTF-4088 카탈로그와 SLeeLa 조회 어댑터는 [`utf-4088/SLeeLa-ALPHABET-INTEGRATION.md`](utf-4088/SLeeLa-ALPHABET-INTEGRATION.md)에 설명되어 있습니다. UTF-4088 ID는 Unicode 스칼라 값과 별개의 네임스페이스입니다. 컴파일러 및 SLVM 바인딩 작업은 이러한 네이티브 기반 작업과 별도로 추적됩니다.

## XII. AutoCAD Creator™

[AutoCAD Creator™](autocad/creator/README.md)는 SLeeLa만으로 작성된 대화형 2D CAD 애플리케이션입니다. 함께 제공되는 AutoCAD Renderer가 도면을 한 번에 생성하는 반면, Creator에서는 2D AutoCAD 문서를 실시간으로 **만들고 수정**할 수 있습니다. 벽, 방, 치수 및 메모를 추가한 다음 배치한 요소를 이동, 재라벨링, 레이어 변경, 삭제하고 실행 취소할 수 있으며, 결과를 실제 AutoCAD DXF 도면(R12 텍스트 형식, AutoCAD/LibreCAD에서 열림)으로 **저장**할 수 있습니다.

그래픽 인터페이스는 JavaFX가 아니라 SLeeLa 자체의 크로스 플랫폼 UI 툴킷인 **SleelaUI™**([`lib/user-interface`](lib/user-interface))으로 구축되며 X11/Cocoa/Win32에서 네이티브 창을 표시합니다. 설계 원칙은 **“SleelaUI는 보여 주고, SLeeLa가 결정한다”**입니다. GUI는 기하 계산이나 DXF 쓰기를 수행하지 않습니다. 각 도구는 편집 가능한 문서와 되돌릴 수 있는 명령 기록을 소유한 권위 있는 [`SLCadCreator`](lib/autocad/SLCadCreator.sleela)에 생성/수정/삭제/실행 취소/저장 의도를 전달합니다. 캔버스는 Creator가 보유한 기하 정보를 미리 보기만 합니다.

재사용 가능한 편집 엔진은 [`lib/autocad`](lib/autocad)에 있으며 [`SLCadEntity`](lib/autocad/SLCadEntity.sleela), [`SLCadDocument`](lib/autocad/SLCadDocument.sleela), [`SLCadCommand`](lib/autocad/SLCadCommand.sleela), [`SLCadCreator`](lib/autocad/SLCadCreator.sleela)로 구성됩니다. 헤드리스 환경에서도 처음부터 끝까지 실행됩니다. GUI 셸과 데모는 [`autocad/creator`](autocad/creator)에 있습니다.

```sh
make creator          # autocad/ 디렉터리에서 실행: Creator 데모 -> out/altered.dxf
```

디스플레이가 있는 환경에서는 실시간 편집 창이 열립니다. 헤드리스 호스트에서는 메모리에서 도면을 만들고 보고서와 DXF를 출력한 뒤 정상 종료합니다.

## XIII. UTF-4088 입력 맵 순서

특수 UTF-4088 카탈로그는 공간 입력 맵 순회와 추상 압력 반복을 [`utf-4088/INPUT-MAP-AND-PRESSURE.md`](../utf-4088/INPUT-MAP-AND-PRESSURE.md)에 정의합니다. National Lien 주제 색인 정책은 [`utf-4088/NATIONAL-LIEN-REGISTER.md`](../utf-4088/NATIONAL-LIEN-REGISTER.md)에 있습니다. 맵 좌표, 압력 수준(1–50), 영구 카탈로그 ID 및 알파벳 순위는 서로 독립된 필드이며 어느 하나를 다른 필드 대신 사용해서는 안 됩니다. 압력은 알고리즘의 반복 수준이지 물리량이 아닙니다.

## XIV. 최근 확장된 패키지 색인

이 추가 색인은 최근 확장된 패키지, 도메인 라이브러리 및 소스 컬렉션을 소개합니다. 항목은 패키지 이름 또는 주제의 알파벳 순으로 묶었습니다. 설명은 저장소 내 위치를 안내합니다. 구현 및 검증 상태는 각 패키지의 문서와 테스트에서 확인해야 합니다.

### A–C

- **Advantage** — `lib/advantage/` 및 `advantage/`: 모델에 대한 고유 배당 최적화를 계층형 Sequitur onion으로 다룹니다. 다섯 주제 하위 폴더는 `math/`(공동 이익 최대화, Nash 협상, 양자 뉴스벤더), `physics/`(trust-field 계량/측지선/접촉/SDE — 모델링 유추임을 표시), `integration/`(수직 통합 구획과 정교성 점수), `grading/`(모듈과 세트를 위한 A–D 정교성 회로 채점기), `output/`(터미널/CommonRails/소켓 출력)입니다. 네이티브 C/C++ 소비자(`advantage/`)는 모델을 실제 거래 예제에 연결합니다. [README](lib/advantage/README.md) 및 [DEFINITIONS.md](lib/advantage/DEFINITIONS.md)를 참조하세요.
- **Alphabet and UTF-4088** — `alphabet/sleela/`, `lib/alphabet/native/`, `utf-4088/`: 알파벳 카탈로그, 네이티브 문자 집합 지원, 입력 맵 순서 및 UTF-4088 통합 문서.
- **AutoCAD Creator** — `autocad/creator/` 및 `lib/autocad/`: 대화형 2D 도면 작업 흐름과 재사용 가능한 CAD 문서/편집 구성 요소.
- **Ballum** — 픽셀 수준 디스플레이 제어: 픽셀 설정/가져오기, 영역 blit, 전체 디스플레이 밝기/따뜻함/감마, 포인터/키보드 입력, 전면적이고 빠른 픽셀 제어를 위한 **전체 화면 프레임버퍼 표면**(Linux/X11에서는 override-redirect 창과 단일 `XPutImage`/MIT-SHM present). 네이티브 코어는 `ballum/`, SLeeLa 파사드는 `lib/ballum/`, VM 브리지는 `impl/`에 있습니다. [안내서](ballum/README.md)와 [실행 가능한 예제](ballum/example/)를 참조하세요.
- **CDMA** — `cdma/` 및 `lib/cdma/`: CDMA 관련 기술 자료와 SLeeLa 소스 라이브러리 작업.
- **Character** — `lib/character/`: 인물 프로필과 재사용 가능한 인물 통계, 전기 프로필 예제.
- **Coorenagraph** — `lib/coorenagraph/`: 윤리적 추론, 도덕 어휘, 숙의 및 관련 개념. 패키지 README와 소스 파일을 참조하세요.

### D–G

- **Governance source models** — [`lib/governance`](lib/governance/README.md)는 제안, 익명 수동 투표 및 집계 결과를 위한 선언형 SLeeLa 소스 형태를 정의합니다. 제안된 `@vote` 주석과 수동 절차는 [`registry/DEMOCRATIC-VOTING-SYNTAX.md`](registry/DEMOCRATIC-VOTING-SYNTAX.md)에 설명되어 있습니다. 현재 컴파일러에서는 실행되지 않는 주석 메타데이터로 취급됩니다.

### H–I

- **HTTP 1.0–8.0** — 사용 가능한 버전별 HTTP 패키지 디렉터리(`lib/http-1.0/`부터): 버전별 프로토콜 사양과 구현입니다. 정확한 범위와 검증 상태는 각 버전의 문서를 확인하세요.
- **International Criminal Court** — `lib/international-criminal-court/` 및 `INTERNATIONAL.CRIMINAL.COURTS.md`: 관련 전문 분야 소스와 문서입니다.

### O–S

- **OFDMA** — `ofdma/`: 직교 주파수 분할 다중 접속 기술 자료와 C/C++ 개발 리소스.
- **Philosophy and the Conversation Mask module** — [`philosophy/`](philosophy/README.md)에는 네이티브 C/C++ 기반이 있으며, [`philosophy/module/`](philosophy/module/README.md)은 다음 모듈 계층을 세 영역으로 구성합니다. [data/](philosophy/module/data/README.md)는 타입 지정 및 날짜가 있는 관찰, 이벤트 종료, 반감기 설정, 출처 정보 및 동의를 다룹니다. [helper/](philosophy/module/helper/README.md)는 결정론적 검증, 관련성 감소, 호환 가능한 비교 및 동의 기반 표시를 담당합니다. [psychiatry-friend/](philosophy/module/psychiatry-friend/README.md)는 지원적이고 비임상적인 대화 인터페이스입니다. SLeeLa 측 핸들과 통합 Tree는 더 넓은 philosophy 패키지와 함께 [`lib/philosophy/`](lib/philosophy/README.md)에 있습니다. 이 모듈은 명시적 입력과 동의를 우선합니다. 사람이나 미디어를 수동으로 감시하거나, 진단하거나, 사적인 의도를 추론하거나, 인간의 가치를 점수화하거나, 약물 변경을 권고하지 않습니다. 이벤트 링크는 출처 정보와 탐색 경로를 보존할 뿐 독립적인 사실 확인을 의미하지 않습니다.
- **Synchro** — 네이티브 런타임/컴파일러 통합, 패킷 디스패치 및 측정 관련 기능. 관련 `impl/` 소스와 컴파일러 구현을 참조하세요.
- **SleelaUI / terminal UI** — `lib/user-interface/` 및 `sleela-terminal` 프로젝트: SLeeLa UI 통합과 터미널 중심 상호작용 구성 요소.

### U–W

- **United Nations** — `united-nations/`: UN 관련 도메인별 저장소 자료.
- **Wi-Fi 4, Wi-Fi 5, Wi-Fi 6** — `wi-fi-4/`, `wi-fi-5/`, `wi-fi-6/` 및 관련 `lib/wifi/` 소스 디렉터리: 세대별 무선 네트워킹 자료와 구성 리소스.

### 패키지 색인 유지 관리

패키지를 추가하거나 크게 확장하면 적절한 알파벳 그룹에 항목을 추가하고, 정식 디렉터리와 README에 연결하며, 저장소에 포함된 소스로 뒷받침되는 기능만 설명합니다. 이 색인은 추가 방식으로 유지합니다. 패키지 색인을 갱신한다는 이유로 기존 README 섹션을 삭제하거나 다시 작성하지 마세요.

---

## <img src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="25" height="25" alt="Debian"> XV. Bitcoin Conjegeum

![CremeOak](https://github.com/mearvk/Java.Web.Server.Telnet.Front.Java.21/raw/main/images/national-bitcoin-icc-logo-005.jpg)

`bc1qs6v4q9zsw70t0umk3m0quhvf9dr6cdeskl28dh` :: `⠝⠇⠼⠁⠟⠎⠼⠋⠧⠼⠙⠟⠼⠊⠵⠎⠺⠼⠛⠼⠚⠞⠼⠚⠥⠍⠦⠼⠉⠍⠼⠚⠟⠥⠓⠧⠦⠼⠊⠙⠫⠼⠋⠉⠙⠑⠎⠅⠇⠼⠼⠃⠼⠗⠙⠓`

`bc1qx33vma553gddsjkq372l3xlm8e8huv2vzad3cc` :: `⠃⠉⠼⠁⠟⠭⠼⠉⠼⠉⠪⠍⠁⠼⠑⠑⠛⠙⠙⠎⠚⠅⠟⠼⠉⠼⠛⠼⠝⠇⠼⠉⠼⠭⠇⠍⠼⠨e⠼⠨a⠓⠥⠪⠼⠪⠼⠪⠵⠁⠙⠼⠉⠉`

US Democratic and US Policy.

---

## XVI. National Science 브랜드 이미지

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-science-004.jpg" alt="National SLeeLa Science" width="560"></p>

<p align="center"><img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/brand/national-sleela-nsf-001.jpg" alt="National SLeeLa NSF" width="560"></p>

## XVII. UTF-4088 백만 항목 실험 카탈로그

[UTF-4088 카탈로그 생성기](utf-4088/tools/generate_catalogue.py)는 1,048,576개 항목으로 구성된 실험용 기호 카탈로그를 결정론적으로 생성합니다. 압축 형식은 [`utf-4088/FORMAT.md`](utf-4088/FORMAT.md)에, 생성 및 검증 계약은 [`utf-4088/GENERATION-RESULTS.md`](utf-4088/GENERATION-RESULTS.md)에 설명되어 있습니다.

Python 3으로 다음 명령을 실행합니다.

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

생성된 바이너리는 16,777,244바이트이며 20,000,000바이트 미만을 유지하도록 설계되었습니다. 단위 테스트와 GitHub Actions 워크플로가 항목 수, 글리프 고유성, 크기 및 SHA-256을 검증합니다. 실행이 성공하면 바이너리와 요약 파일이 `utf4088-million-catalogue` 워크플로 아티팩트로 게시됩니다. 생성 바이너리는 의도적으로 Git 기록에 포함하지 않습니다. UTF-4088은 실험 단계이며 Unicode와 호환되지 않습니다.

---

# SLeeLa

<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>

---

## XVIII. 최근 추가 사항 — Pi 상수, Modium/Registry 시작, sleelactl Math 및 `@native`

이 섹션은 2026-11 작업에서 추가된 기능을 최신 그룹부터 기록합니다. 링크된 소스 파일과 그 검증 상태가 기준이며, 검토 중인 변경 사항은 `master`에 반영되기 전까지 병합된 것으로 설명하지 않습니다.

### 100,000,000자리 Pi 카탈로그 상수

저장소에는 검증된 **1억 자리** π 값인 [`pi-generator/pi.100000000.digits`](pi-generator/pi.100000000.digits)(올바른 소수 형식, `3.14159…`)과 57,344자리 값인 [`pi-generator/pi.57344.digits`](pi-generator/pi.57344.digits)이 포함되어 있습니다. 두 값 모두 실제 π임을 독립적으로 검증했습니다. 검증 항목(교과서적 접두부, 소수부 762번째 위치의 Feynman point, 유명한 백만 번째 자리, [`lib/math/SLPiReference.sleela`](lib/math/SLPiReference.sleela)와의 일치, 전체 파일 숫자 빈도 분석)과 한 번에 재현하는 스크립트는 [`pi-generator/VERIFIED.md`](pi-generator/VERIFIED.md)를 참조하세요.

### Modium™ / Registry™는 하위 시스템 시작 시 Pi 상수를 로드합니다

전용 하위 시스템 진입점인 `sleela registry`, `sleela modium`(및 설치 프로그램 최초 실행)에서 Modium과 Registry는 대형 π 카탈로그 상수를 로드합니다. 모든 `sleela run` 실행 때마다 로드하는 것은 아닙니다. 대화형 터미널에서는 일시적인 빨간색 `Loading Modium™` / `Loading Registry™` 줄을 표시한 뒤 정상 출력이 시작되기 전에 같은 위치에서 지웁니다. 이 효과는 TTY에서만 작동하므로 파이프 및 테스트 출력은 바이트 단위로도 바뀌지 않습니다. 데이터 자체가 아니라 위치 정보가 SLeeLa 모델([`lib/registry/SLRegistry.sleela`](lib/registry/SLRegistry.sleela))과 네이티브 시더([`registry/sleela_registry.c`](registry/sleela_registry.c)) 모두에서 Registry의 `Math` 키 기본값 `piReferenceLargePath` 및 `piReferenceLargeDigits`로 설정됩니다. `lib/math/SLPiReference.sleela`는 `largePath()` / `largeDigits()`를 제공합니다. π 상수는 표준 `make` 설치 프로그램과 안내형 “beautiful” 설치 프로그램 모두를 통해 OS의 SLeeLa 위치(`<data>/pi/`)에 설치됩니다. 자세한 내용은 [`pi-generator/MODIUM-REGISTRY-PI-STARTUP.md`](pi-generator/MODIUM-REGISTRY-PI-STARTUP.md)를 참조하세요.

### `sleelactl` — beautiful 설치 프로그램과 네이티브 백엔드 `math` subject

- 안내형(“beautiful”) 설치 프로그램은 이제 `make` 설치 프로그램과 동일하게 SLeeLa 제어 플레인인 `sleelactl`과 모듈 레지스트리를 설치합니다. [문서](bin/SLEELACTL.md)를 참조하세요.
- `sleelactl`에는 런타임에 컴파일된 네이티브 C/C++ subject 라이브러리를 위한 새로운 `subject` 종류가 추가되었고, 첫 subject로 `math`가 등록되었습니다. `sleelactl run math`는 subject 참조 데모를 통해 네이티브 `__NativeMath` 백엔드(`impl/subjects/math/math_native.cpp`)를 처음부터 끝까지 실행합니다. `sleelactl info math`는 접근 방법(`import math;`)을 설명합니다. subject 종류는 `run`과 `info`만 제공합니다. `sleela check`는 SHEET에 등록된 네이티브 subject를 로드하지 않으므로 의도적으로 `check` 동사는 제공하지 않습니다.

### `@native`, Native Drive 및 다중 언어 실행

새로운 **`@native`** 메서드 주석은 핫 메서드의 AOT(Ahead-of-Time) 네이티브 컴파일을 요청합니다. 새 `sleela aot` 명령은 `@native` 메서드를 찾아 각 메서드가 지원되는 *오프로딩 가능 부분집합*에 속하는지 확인한 뒤 C/C++를 출력합니다. 아직 번역할 수 없는 구문이 있으면 정확한 이유를 제시하며 **fail closed** 방식으로 거부하므로, 반쯤 번역된 메서드는 생성되지 않습니다.

```sh
sleela aot --list <file.sleela>    # 메서드별 판정: ok / REFUSED (사유)
sleela aot --emit <file.sleela>    # 부분집합에 속하는 프로그램의 C/C++ 출력(stdout)
```

`@native`는 여전히 AOT 처리를 요청하는 것이며, 지원되지 않는 구문은 출력 전에 거부됩니다. [Native Drive](impl/native-drive/README.md)는 호스트 컴파일러(MSVC 포함)를 통한 다중 단위 C/C++ 실행 파일 및 공유 라이브러리 빌드, 지원되는 SLeeLa AOT 출력, 네이티브 로더 ABI에 대한 심볼 검사, 셸을 거치지 않는 프로세스 실행, stdin/stdout/stderr 처리, 다단계 파이프 연결, XClass에서 프로덕션 로더로의 위임을 지원합니다. `sleelactl`은 해당 빌드/실행 명령을 노출하며 `inspect native-drive`를 통해 XClass 검사와 명시적 systemd 유닛 렌더링/설치를 중계합니다. VM은 필수 호스트 인가와 세대 검사가 적용된 핸들을 갖춘, 크기가 제한된 네이티브 모듈 핸들 테이블을 소유합니다. 이제 구문 3.0에는 `nativeOpen`, `nativeCall`, `nativeClose`가 추가되었습니다. 추가 전용 바이트코드 디스패치는 별도로 버전 관리되는 null/bool/int64/double/UTF-8 ABI를 가로지르며, VM이 소유한 배열, 구조체 및 기타 복합 값은 인터프리터 내부에 머물고 동일한 이름의 바이트코드 대체 경로를 사용할 수 있습니다. 플러그인은 선택적으로 버전이 지정된 호스트 콜백 테이블을 받아 중첩된 VM 실행을 통해 정확한 SLeeLa 함수를 호출할 수 있으며, 이때 일시 중단된 스택의 GC 루팅, 호스트 콜백 이름의 명시적 허용 목록이 적용되고 재귀적인 네이티브 API 호출은 거부됩니다. 네이티브 코드는 여전히 신뢰되는 프로세스 내 코드이며 샌드박스가 아닙니다. 소스 프로세스 정책 주석에는 버전이 지정된 스키마가 있지만 아직 구문 분석되거나 강제되지는 않습니다. 세부 사항, 보안 경계 및 일곱 개 워크스트림 대장은 [Native Drive](impl/native-drive/README.md)와 [`pi-generator/AT-NATIVE.md`](pi-generator/AT-NATIVE.md)를 참조하세요.

## XIX. 국제 README 에디션

SLeeLa는 국제 에디션에 포함된 50개 시장을 위해 국가별 README 진입점을 제공합니다. 각 국가에는 `README.<CC>.md` 형식의 파일이 하나 있으며, 여러 언어가 관련된 경우 해당 국가 파일 안에 함께 구성됩니다.

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

**번역 정책:** 이 파일들은 국가별 현지화 진입점의 초기 버전입니다. 소개와 언어 제목은 현지화되어 있지만 전체 기술 참조는 표준 영어 [README.md](README.md)에 있습니다. 모든 섹션을 번역하고 기술 식별자와 링크를 확인하며 언어 검토를 마치기 전에는 국가별 파일을 완역본으로 표시해서는 안 됩니다. 이 첫 단계의 언어 범위는 대표적인 예시이며 헌법상 인정되는 모든 지역어, 토착어 또는 수어가 완전히 번역되었다는 뜻은 아닙니다.

## 번역 상태

**완역 초안 — 언어 검토 필요.** 한국어판은 README의 I~XIX 섹션을 번역했습니다. 기술 식별자, 명령, 코드 예제, 파일명, URL 및 주소 문자열은 원문 그대로 유지했습니다. 공개 전에 한국어 원어민의 언어 검토와 기술 링크 확인을 권장합니다.

