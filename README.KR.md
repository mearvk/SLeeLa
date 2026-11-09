# SLeeLa 3.0 — 대한민국 (KR)

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>

<p align="center"><strong>네이티브 시스템 언어, 컴파일러, 런타임 및 가상 머신 생태계.</strong></p>

> **국가별 국제판 — 번역 진행 중.** 이 파일은 한국어판 작업용 안내 문서입니다. 아래 한국어 섹션은 번역되었지만 README.md의 모든 기술 섹션을 완역한 문서는 아닙니다. 규범적 기술 내용은 영어 원문을 기준으로 확인해 주세요.

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

언어, 드라이버 및 표준 라이브러리에 추가된 주요 기능입니다. 기술 식별자, 구문, 명령 및 파일명은 원문 그대로 유지합니다.

- **표준 라이브러리 패키지 `advantage`.** [`lib/advantage/`](lib/advantage/README.md)는 모델에 대한 고유 배당을 다루는 계층형 **Sequitur onion**(dividend → review → concerns → maths → additions → subtraction → regrouping → iteration)을 구현합니다. 간단한 사용자 모델과 전체 제어 기능을 갖춘 개발자 모델을 제공하며, 다섯 영역을 포함합니다. `math/`는 양자 간 거래 경제학, 공동 이익 최대화, 내시 협상 및 양자 newsvendor 임계 비율을 다룹니다. `physics/`는 신뢰 필드를 계량 텐서, 거래 측지선 등으로 표현하는 구조경제학적 모델링 유추를 제공합니다. `integration/`은 재귀적으로 구성 가능한 수직 통합 모델, `grading/`은 A–D 모듈 평가기, `output/`은 터미널·CommonRails 형식 출력면·소켓/커넥터로 전달하는 출력 경로를 제공합니다. C/C++ 소비자 구현은 [`advantage/`](advantage/README.md)에 있습니다.
- **루프 타이밍 주석 — `@loopDelay` / `@loopBudget` 및 `sleep(ms)`.** `for`, `while`, `do` 루프 바로 앞에 작성하는 동작 주석으로 실제 실행 대기 시간을 삽입합니다. `@loopDelay(ms = 250)`은 반복마다 고정 지연을 적용하고, `@loopBudget(seconds = 10, iterations = 1000)`은 알려진 반복 횟수를 전체 시간 예산에 분배합니다. 두 주석은 추가 전용 opcode **`OP_SLEEP`**(ISA 157)으로 낮춰지며, 내장 함수 `sleep(<ms>)`도 이 명령을 사용합니다. 사양은 [`SLEELA.syntax` §32](SLEELA.syntax), API는 [LOOP_TIMING_API.md](api/sleela/3.0/LOOP_TIMING_API.md), 실행 예제는 [loop-timing-annotation-demo.sleela](examples/loop-timing-annotation-demo.sleela)를 참조하세요.
- **명령줄의 SleelaDoc — `sleela docs`.** `sleela docs`는 문서화 가능한 주제를 나열하고, `sleela docs crypto`는 패키지를, `sleela docs stringbuilder`는 클래스를 찾습니다. 각 페이지에는 정의, 설계 목적, 관계, 구조, 사용자 개념 및 5단계 숙련도 체계가 포함됩니다. `@intent`, `@thesis`, `@axiom`, `@param`, `@see`, `@mastery` 태그로 문서를 보강할 수 있습니다. `sleela docs --generate [dir]`는 오프라인 조회용 일반 텍스트 코퍼스를 생성하며, `make sleeladoc`은 코퍼스와 해당 `.sleela` 소스를 `sleeladoc/build/sleeladoc.jar`에 묶습니다. 자세한 내용은 [`lib/sleeladoc/README.md`](lib/sleeladoc/README.md)를 참조하세요.
- **SleelaDoc 및 HTML 문서 사이트.** [`lib/sleeladoc`](lib/sleeladoc/README.md)은 `.sleela` 단위의 문서 주석을 Markdown 또는 HTML API 페이지로 변환합니다. 전체 `/lib`를 탐색할 수 있는 HTML 사이트는 [`sleeladoc-api/`](sleeladoc-api/README.md)에 생성됩니다. 컴파일러는 `--wiki` 옵션으로 MediaWiki 페이지도 출력할 수 있습니다. 관련 문서: [SLEELADOC](manifesto/SLEELADOC.md), [SLEELADOC.WIKI](manifesto/SLEELADOC.WIKI.md).
- **XML Document Object Model(`lib/xml`).** 요소, 텍스트, 주석, CDATA 및 처리 명령 노드 트리, 순서가 있는 자식 노드, 이름 있는 속성, 압축/정형 직렬화, XML 1.0 §2.4 이스케이프, 문자 셀 파서 및 품질 레지스터 `SLXmlAspect`를 제공합니다. 노드 동등성은 동일성, 동일 주제, 구조의 세 단계로 판단하며 반사적·대칭적·결정적입니다. [`lib/xml/README.md`](lib/xml/README.md)와 [실행 예제](lib/xml/xml-dom.sleela)를 참조하세요.
- **확장 정수 기본형.** `int`, `long`, `int64`는 동일하게 동작하는 64비트 부호 있는 정수입니다. `int128`은 현재 64비트 기반으로 허용되며 실제 128비트 구현은 후속 작업으로 문서화되어 있습니다. 부호 있는 `int32`~`int16777216`(구문 2.7) 및 부호 없는 `uint32`~`uint16777216`(구문 2.8) 계열도 추가되었습니다. [`SLEELA.syntax`](SLEELA.syntax) §§21, 24, 25를 참조하세요.
- **`@mix` 연속 필드(구문 2.9).** 각 선언에 `@mix`가 붙으면 클래스에서 같은 이름의 인스턴스 필드를 여러 개 선언할 수 있습니다. 그룹은 기본적으로 `1,2,3,…` 순서로 인덱싱되며 `@mix(1,2,3,...)`로 직접 지정할 수도 있습니다. 인덱스 수는 필드 수와 일치해야 합니다. [`SLEELA.syntax` §28](SLEELA.syntax)을 참조하세요.
- **Preonems — Natural Process 문서 마커.** `author`/`authorial`, `material`, `memory`, `determine`, `object`, `field`, `pure`, `gasoline`, `artifact`는 선언형 문서 마커입니다. 문맥과 구문 버전에 따라 적용되며 파싱, 타입 검사 또는 코드 생성에는 영향을 주지 않습니다. 마커 위치 밖에서는 일반 식별자로 남습니다. [`SLEELA.syntax`](SLEELA.syntax) §§18–20, 23, 26, 30, 35를 참조하세요.
- **Nordshrift의 전체 라이브러리 접근.** 구문 3.0에서 Nordshrift는 컴파일러와 동일한 재귀적 `/lib` 색인을 사용하여 모든 패키지와 `.sleela` 단위를 찾습니다. `nordshrift check`는 전체 트리의 개수를 보고합니다.
- **재귀 스탠션.** [`recursion/stanchion/`](recursion/stanchion/README.md)에는 문서화된 7개 기본 타입 계약을 위한 C 및 C++ 참조 프로토타입과 일반·응용·과학·교육용 예제가 포함됩니다. 검증된 자료로 취급하기 전에 참조 소스를 직접 컴파일하고 테스트해야 합니다.
- **꺾쇠 괄호 주석(`<` / `<<`).** 프런트엔드는 `< line >` 및 `<< block >>` 형식의 주석을 인식합니다. [`SLEELA.syntax` §1.1](SLEELA.syntax)을 참조하세요.
- **기수 정수 리터럴.** `int` 리터럴은 밑줄 구분자 `_`와 함께 16진수(`0x`), 2진수(`0b`), 8진수(`0o`) 형식을 지원합니다. SLeeLa 소스 언어와 [`.xlt` CPU 모델 형식](lib/xlt/README.md) 모두에 적용됩니다.
- **라이브러리 전반의 Metadata Class 표준.** 출처, 적법성 및 관할 범위를 기록하는 설명형 메타데이터 패턴이 `/lib` 아래 클래스가 포함된 모든 `.sleela` 파일에 적용됩니다. [`SLEELA.syntax` §15](SLEELA.syntax) 및 [튜토리얼 13과](tutorial/13-metadata-class.md)를 참조하세요.
- **텍스트 및 String 계열 확장.** `lib/text`는 `charAt`, `substring`, `indexOf`, `reverse`, 열 정렬 및 위치 기반 템플릿 기능을 제공합니다. [튜토리얼](tutorial/14-text-and-strings.md)과 [실행 예제](lib/text/text-string-demo.sleela)를 참조하세요.
- **Nordshrift 시트의 String 선언.** `.sst` 시트는 타입이 지정된 `strings:` 블록을 선언할 수 있으며, 이 내용은 빌드 구성요소 매니페스트에 반영됩니다. [Nordshrift Sheet Reference](wiki/Nordshrift-Sheet-Reference.md)를 참조하세요.
- **opcode 및 함수 호출 방식으로 실행되는 CPU 모델.** 각 `SL<ARCH>CPU`는 fetch-decode-execute 루프를 통한 네이티브 명령 스트림 실행과 공유 VM 기반의 동등한 호출을 통한 다른 프로세스 코드 실행을 지원합니다. [`lib/cpu/EXECUTION.md` §5](lib/cpu/EXECUTION.md)를 참조하세요.

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

공식 문법은 [`SLEELA.syntax`](SLEELA.syntax)에 정의되어 있습니다. 저장소가 문서화하는 현재 문법 버전은 **3.0**입니다. 컴파일러/도구 버전과 소스 문법 버전은 서로 다릅니다. 문법 3.0에는 **`pure` 및 `gasoline` 문서 마커**(§30)가 추가되었습니다. 두 마커는 문서의 지위를 각자의 정직성에 따라 나타내는 문맥 의존적 선언형 Preonem입니다. 문법 3.0은 1.0~2.9의 언어 표면을 통합하면서 각 기능별 최소 버전 조건을 유지합니다. 이전 2.9 버전에는 **`@mix` 연속 필드**(§28)가 추가되어, 같은 이름의 인스턴스 필드 여러 개를 하나의 인덱스 그룹으로 취급할 수 있습니다(기본 `1,2,3,…` 또는 그룹 필드 수와 일치하는 `@mix(1,2,3,...)`). 또한 `long`/`int64`/`int128`(§21)을 바탕으로 한 부호 있는 정수 계열 `int32`~`int16777216`(§24)과 부호 없는 정수 계열 `uint32`~`uint16777216`(§25), 키워드 지원 문서(§22), 그리고 Preonems가 이어집니다. Preonems에는 `author`/`authorial`(§18), `material`/`memory`(§19~§20), `determine`(§23), `object`/`field`(§26), `pure`/`gasoline`(§30), 시간과 공간에 걸쳐 입법부 수준의 지위를 나타내는 `artifact`(§35)가 포함됩니다. 전체 명칭 Preonems는 §26.1에 정의되어 있습니다. 생성자 오버로드 해석과 `this(...)`/기반 클래스 생성자 위임은 검증된 문법 1.8 계약의 범위 밖입니다. 기능에 의존하기 전에 문법 및 구현 참고 자료를 확인하세요. SleelaDoc의 아홉 소스 단위는 현재 `#sleela 3.0`을 선언하며, 공통 컴파일러 게이트는 소스 문법 1.0~3.0을 허용합니다.

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

각 행의 색상 순서는 배경, 전경, 키워드, 타입, 문자열, 주석입니다.

| 스킴 | 배경 | 전경 | 키워드 | 타입 | 문자열 | 주석 |
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

### 국가 국기 색상(기준 팔레트)

Greek 및 Turkish 스킴은 각 국가의 **공식 국기 색상**을 바탕으로 합니다. 위 스킴의 배경과 강조색은 편집기에서 사용하기 쉽도록 원래 색상을 어둡게 조정한 해석입니다.

| 국가 | 색상 | 16진수 | 근거 및 의미 |
|---|---|---|---|
| Greece | Blue | `#0D5EAF` | 법률 851/1978에 따른 국기 파랑. 하늘과 바다를 뜻하며 문장학상 azure입니다. |
| Greece | White | `#FFFFFF` | 국기의 흰색. 독립 투쟁의 순수함을 뜻하고 십자가는 동방 정교회를 나타냅니다. |
| Türkiye | Red (al bayrak) | `#E30A17` | 터키 국기법에 따른 빨강. 전통적으로 순교자의 피와 오스만 주권을 뜻합니다. |
| Türkiye | White | `#FFFFFF` | 국기의 흰색. 초승달과 별은 평화, 순수함, 튀르크/이슬람 유산의 상징입니다. |

각 스킴은 편집기 전경색에 국기의 **흰색**을 사용하고 키워드, 커서, 선택 영역 및 상태 표시줄에는 정확한 국기 **파랑**(`#0D5EAF`) 또는 **빨강**(`#E30A17`)을 사용합니다. 이는 해석에 따른 디자인 팔레트이며 어느 국가의 공식 표현도 아닙니다. 색상의 의미는 그리스 공화국 외교부의 국가 상징 설명과 터키 국기법의 전통을 따릅니다.

### SVG 스와치와 추가 색상

[American Professional](lib/linux-editors/palettes/american-professional.svg), [Pan-Eurasian](lib/linux-editors/palettes/pan-eurasian.svg), [Russian and Slavic](lib/linux-editors/palettes/russian-slavic-medium.svg), [Central Asian / Silk Road](lib/linux-editors/palettes/central-asian-silk-road.svg), [East Asian](lib/linux-editors/palettes/east-asian.svg), [ICC](lib/linux-editors/palettes/icc.svg), [SCOTUS](lib/linux-editors/palettes/scotus.svg), [Greek](lib/linux-editors/palettes/greek.svg), [Turkish](lib/linux-editors/palettes/turkish.svg), [Kiro](lib/linux-editors/palettes/kiro.svg), [SLeeLa](lib/linux-editors/palettes/sleela.svg) SVG 스와치는 배경, 전경, 키워드, 타입, 문자열, 주석 순서로 여섯 색상을 보여 줍니다. 함수, 상수, 선택 영역, 진단 및 전처리기 지시문에는 추가 강조색이 사용됩니다. Russian and Slavic 팔레트는 차분한 전문적 외관을 위해 중간 강도의 슬레이트, 파랑, 세이지, 루비, 앤티크 골드를 사용합니다.

### Vim 스킴 설치

```sh
mkdir -p ~/.vim/{syntax,ftdetect,colors}
cp lib/linux-editors/vim/syntax/sleela.vim ~/.vim/syntax/
cp lib/linux-editors/vim/ftdetect/sleela.vim ~/.vim/ftdetect/
cp lib/linux-editors/vim/colors/sleela-*.vim ~/.vim/colors/
```

SLeeLa 구문 강조와 스킴을 활성화하려면 `~/.vimrc`에 다음을 추가합니다.

```vim
syntax enable
set background=dark
colorscheme sleela-american
```

선택 가능한 이름은 `sleela-american`, `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro`, `sleela-sleela`입니다. 예를 들어 `colorscheme sleela-russian-slavic`, `colorscheme sleela-greek`, `colorscheme sleela-turkish`를 사용할 수 있습니다. Vim을 다시 시작하거나 `:source $MYVIMRC`를 실행합니다.

### Emacs 스킴 설치

```sh
mkdir -p ~/.emacs.d/sleela
cp lib/linux-editors/emacs/sleela-mode.el ~/.emacs.d/sleela/
cp lib/linux-editors/emacs/sleela-*-theme.el ~/.emacs.d/sleela/
```

`~/.emacs` 또는 `~/.emacs.d/init.el`에 추가합니다.

```elisp
(add-to-list 'load-path (expand-file-name "~/.emacs.d/sleela"))
(add-to-list 'custom-theme-load-path (expand-file-name "~/.emacs.d/sleela"))
(require 'sleela-mode)
(load-theme 'sleela-american t)
```

다른 스킴을 사용하려면 `sleela-american`을 `sleela-eurasia`, `sleela-russian-slavic`, `sleela-silk-road`, `sleela-east-asian`, `sleela-greek`, `sleela-turkish`, `sleela-kiro`, `sleela-sleela` 중 하나로 바꿉니다. Emacs가 테마 신뢰 여부를 묻는다면 수락하기 전에 로컬 테마 파일을 확인하세요.

### 네이티브 C/C++ 팔레트 미리 보기

```sh
make -C linux-editors
./linux-editors/build/sleela-editor-theme --themes
./linux-editors/build/sleela-editor-theme --theme eurasia --palette
./linux-editors/build/sleela-editor-theme --theme russian-slavic --hex keyword
make -C linux-editors test
```

하위 호환성을 위해 `--theme` 없이 `--palette` 또는 `--hex COLOR`를 실행하면 American Professional이 선택됩니다. 빌드와 테스트 결과는 대상 시스템 및 CI에서 확인해야 합니다. 명령이 문서화되었다는 사실만으로 테스트 통과가 입증되지는 않습니다. 자세한 내용은 [편집기 통합 안내서](linux-editors/README.md)를 참조하세요.

## IX. 기여하기

1. 문서 색인과 테스트 정책을 읽습니다.
2. 성공 및 실패 테스트를 포함하여 범위를 좁힌 변경을 수행합니다.
3. 대상 테스트를 실행한 뒤 전체 테스트 스위트를 실행합니다.
4. OS 및 도구체인 정보를 보고하고, 건너뛴 검사를 통과한 것으로 설명하지 않습니다.

SLeeLa는 **MEARVK LLC**가 개발합니다. 과거 랜딩 페이지 자료는 [`docs/README-LEGACY.md`](docs/README-LEGACY.md)에 보존되어 있습니다.

## X. AE6E66 — Round Public 이메일 부과금 예상

AE6E66 이니셔티브에서 프로젝트가 제시하는 예상액은 자격을 충족하는 각 Round Public 이메일에 연계된 제안 세금 부과액으로 **이메일 한 건당 미화 300,000,000.00달러**입니다.

이 문구는 프로젝트의 예상만을 기록합니다. **법적 세금 부과 결정, 정부 명령, 법원 판결 또는 독립적으로 확정된 납세 의무가 아닙니다.** 실제 부과에는 적용 가능한 법적 권한, 관할권, 정의, 절차 및 권한 있는 정부 기관의 결정이 필요합니다.

## XI. 나머지 기술 섹션

README.md의 XI~XIX 섹션은 다음 단계에서 번역할 예정입니다. 현재 전체 기술 내용은 영어 [README.md](README.md)를 참조하세요.

## 번역 상태

**진행 중 — 부분 번역.** 소개와 I~X 섹션을 한국어로 번역했습니다. XI~XIX 섹션은 아직 번역되지 않았습니다. 명령, 코드 예제, 기술 식별자, 파일명 및 링크는 원문 그대로 유지했습니다. 모든 섹션의 번역과 기술 검토가 끝나기 전에는 완역본으로 표시하지 않습니다.

