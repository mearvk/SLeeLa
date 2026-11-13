<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# connector: Explanation

| | |
|---|---|
| Nature | protocol (`bin/sleelactl help connector`) |
| Registry summary | Native data-source / transport connector |
| Lives in | `connector/` (Java under `connector/java/`, deployment samples under `connector/deploy/`) |
| Writeup type | Explanation, because `make -C connector test` and `make -C connector all` both fail to compile in this checkout, so no hands-on path works end to end. |
| Verified | 2026-11-13 against `master` e048707fd: `bin/sleelactl validate connector` exit 1 (servlet jar download returns 403); with a local servlet jar `javac` reports 14 errors. |

## What it is

`connector/` is a Java 22+ package, `com.mearvk.sleela.connector`, that lets Java code and the SLeeLa runtime talk to each other and to native code. Its parts ([`README.md`](../../../connector/README.md), [`CLASS_LOADER.md`](../../../connector/CLASS_LOADER.md), [`DYNAMITE_CONNECTOR.md`](../../../connector/DYNAMITE_CONNECTOR.md)):

- **`SleelaClassLoader`**: loads raw `.class` files or jars, but only from an allow-list, without running static initializers by default, and fails closed.
- **`DynamiteConnector`**: finds and loads native libraries (`.so`, `.dll`, `.dylib`) from explicit roots, by JNI `System.load` or by an FFM `SymbolLookup`. It also fails closed unless a name is allow-listed (or `allowAny()` is called).
- **`SleelaJavaConnector`** (interface) with `SleelaInvocation` and `SleelaResult`, implemented over HTTP (`http/SleelaHttpConnector`), a child process (`process/SleelaProcessConnector`) and RMI (`rmi/SleelaRmiConnector`).
- **`SleelaJdkBridge`** with its test `SleelaJdkConformance`: reflective construct/invoke of JDK classes.
- **HTTP/3-over-HTTP/2 deployment**: `SleelaH3Servlet`, `SleeuumLedger`, and `deploy/web.xml`, `tomcat-server.xml.sample`, `apache-h3.conf` ([`deploy/README.md`](../../../connector/deploy/README.md)).

The README's security note applies: loading native code runs with the process's privileges and is not sandboxed.

## How it is meant to be built

[`connector/Makefile`](../../../connector/Makefile) targets: `all/build/compile`, `deps`, `test` (= `conformance` + `check`), `conformance`, `jar/package`, `install DESTDIR=...`, `check`, `clean`, `help`. Output goes to `connector/build/` (git-ignored). `deps` downloads `jakarta.servlet-api-6.0.0.jar` from Maven Central with `curl`. `bin/sleelactl validate connector` runs `make -C connector test`.

## What I ran, and what failed

1. `bin/sleelactl validate connector` (exit 1):

```
  make -C connector test
Java compiler requirement: satisfied
Downloading Jakarta Servlet API 6.0.0...
curl: (22) The requested URL returned error: 403
make: *** [Makefile:44: build/deps/jakarta.servlet-api-6.0.0.jar] Error 22
  ✗ connector validation reported issues
```

   The sandbox has no route to Maven Central. This is the environment, not the module.

2. The README documents an override, `SERVLET_API_JAR=/path/to/jakarta.servlet-api.jar`. I used a jar that happened to be on the machine (`/inner_container/opt/jdtls/plugins/jakarta.servlet-api_6.1.0.jar`, Jakarta Servlet 6.1, not the 6.0 the README asks for): `make -C connector SERVLET_API_JAR=<that jar> test`. The Java compile step failed (exit 2) with 14 errors. The distinct ones (from a second run of `make ... compile` with the errors filtered):

| File:line | Error |
|---|---|
| `http/SleelaHttpConnector.java:23` | `variable maxResponseBytes is already defined in class SleelaHttpConnector` (the declaration at line 22 is repeated at line 23) |
| `http/SleelaHttpConnector.java:48` | `variable requestId is already defined in method invoke(SleelaInvocation)` (lines 47 and 48 are identical) |
| `SleelaJdkConformance.java:23` | `cannot find symbol: method add(String)`, because `directList` is declared as `Object` (line 18) but used as a list |
| `http/SleelaHttpGateway.java:3` | `package com.mearvk.sleela.gui does not exist`; `SleelaRuntime` is then unresolved at lines 30, 33, 37 |
| `process/SleelaProcessConnector.java:6, 14, 21` | `package com.mearvk.sleela.gui does not exist`; `ProcessSleelaRuntime` unresolved |
| `rmi/SleelaRmiConnector.java:6, 12, 15, 18` | `package com.mearvk.sleela.rmi does not exist`; `SleelaRmiClient` unresolved |

   The missing classes exist elsewhere in the repository: `ProcessSleelaRuntime` at `gui/java/com/mearvk/sleela/gui/ProcessSleelaRuntime.java:18` and `SleelaRmiClient` at `rmi/java/com/mearvk/sleela/rmi/SleelaRmiClient.java:8`. The Makefile compiles only `connector/java` (`JAVA_SOURCES := $(shell find java ...)`), so those packages are never on the source path. The duplicate declarations and the `Object` variable are plain source errors that would fail even with the right source path.

3. The two loaders do compile on their own. In a scratch directory (removed afterwards) I ran `javac --release 22` on `SleelaClassLoader.java`, `DynamiteConnector.java`, `SleelaJavaConnector.java`, `SleelaInvocation.java`, `SleelaResult.java` and `SleelaJdkBridge.java`: exit 0, 10 class files. I did not run them. So the loader code can be built, but not through the module's Makefile.

I ran `make -C connector clean` afterwards. `connector/build/` did not exist before my runs and does not exist now.

## What the other sleelactl verbs do for this module

| Command | Result |
|---|---|
| `bin/sleelactl info connector` | exit 0; `see: connector/README.md`; docs `CLASS_LOADER.md`, `README.md`, `DYNAMITE_CONNECTOR.md` |
| `bin/sleelactl --dry-run validate connector` | `[dry-run] would run: make -C connector test` |
| `bin/sleelactl negotiate connector` | "protocol-metadata operation defined by the generated spec", points to "the negotiate section in connector/README.md", then `no standalone negotiate CLI is built` (exit 3). The README has no negotiate section. |
| `bin/sleelactl grade connector` | same, for `grade` (exit 3) |
| `bin/sleelactl simulate connector` | `no simulator registered (sim= unset)` (exit 3) |

## Where it lives

| File | Role |
|---|---|
| `connector/Makefile` | GNU Make build (`javac --release 22`, `-Xlint:all`) |
| `connector/README.md`, `CLASS_LOADER.md`, `DYNAMITE_CONNECTOR.md` | documentation |
| `connector/java/.../SleelaClassLoader.java` (202 lines), `DynamiteConnector.java` (262) | hardened class and native-library loaders |
| `connector/java/.../SleelaJavaConnector.java`, `SleelaInvocation.java`, `SleelaResult.java` | connector interface and value types |
| `connector/java/.../SleelaJdkBridge.java`, `SleelaJdkConformance.java` | JDK reflection bridge and its test |
| `connector/java/.../http/`, `process/`, `rmi/` | HTTP, child-process and RMI connectors; H3 servlet and ledger |
| `connector/deploy/` | `web.xml`, Tomcat and Apache samples, deployment README |

## Status and limits

- **Broken here.** The module's own `test`, `all` and `jar` targets cannot succeed: three source errors plus two missing package paths (above). I did not run `conformance` or `jar`, because they depend on `compile`.
- **Documentation says otherwise.** `README.md` presents `make -C connector all`, `test` and `jar` as the build path, and says `test` runs JDK bridge conformance and deployment checks. I could not run either part through `make`. `make check` (deployment-file checks and `web.xml` XML parse) does not depend on compilation, but I did not run it separately.
- **The download needs network access.** Without the default URL you must supply `SERVLET_API_JAR`.
- The H3 deployment README refers to `scripts/install-linux-h3.sh` and `http-3.0/http3_transport.py`; both files exist. I did not run them.

## Related

- [route](route.md), [resolver](../tutorials/resolver.md), [preferred-routers](../tutorials/preferred-routers.md), [telephony-skya](../tutorials/telephony-skya.md)

## Sources

- Files read: `connector/Makefile`, `README.md`, `CLASS_LOADER.md`, `DYNAMITE_CONNECTOR.md`, `deploy/README.md`, and parts of `SleelaHttpConnector.java`, `SleelaJdkConformance.java`; `bin/sleelactl.d/registry.conf` (line 65).
- Commands run: `bin/sleelactl [--dry-run] info|validate|negotiate|grade|simulate connector`; `make -C connector SERVLET_API_JAR=... test` and `... compile`; scratch `javac` of the six loader files; `make -C connector clean`; `java -version` (OpenJDK 25.0.2).
