# External Library Manifest

This directory holds **download scripts** for the external (third-party)
libraries the SLeeLa build links against, plus this manifest. The scripts fetch
each library into a relative subdirectory of `libs/`. Nothing large is vendored
into git — the scripts are the source of truth and are meant to be run in an
environment that has network access.

> **Size policy.** Per request, only libraries whose download is **50 MB or
> less** are fetched. Each script enforces this: it checks the on-disk size of
> every downloaded artifact and **skips/aborts anything larger than 50 MB**
> (`MAX_BYTES=52428800`). The JavaFX full SDK (~49–59 MB depending on the point
> release) sits on the boundary, so the Java script fetches the **individual
> Maven JARs** (each only a few MB) instead of the monolithic SDK.

> **Network.** This sandbox runs with no external network
> (`INTEGRATIONS_ONLY`), so the downloads cannot run here. Run the scripts where
> package registries / release CDNs are reachable.

## Scripts

| Script | Fetches into | Covers |
|---|---|---|
| `libs/download-all.sh` | — | Dispatcher: runs the Java and native scripts |
| `libs/download-java-deps.sh` | `libs/java/` | Maven artifacts (JavaFX + build plugins) |
| `libs/download-native-deps.sh` | `libs/native/` | Native C/C++ libraries the build links |

Run all: `bash libs/download-all.sh`
Run one: `bash libs/download-java-deps.sh` / `bash libs/download-native-deps.sh`

## Java / Maven dependencies

Declared in `gui/pom.xml`, `audio/gui/pom.xml`, and
`telephony-skya/javafx/pom.xml`. All are from Maven Central
(`https://repo1.maven.org/maven2`). The `-linux` classifier JAR carries the
native code and is required at runtime on Linux.

| Artifact | Version(s) | Approx size | ≤50 MB |
|---|---|---|---|
| `org.openjfx:javafx-base` (jar + `:linux`) | 21.0.6, 21.0.8 | ~0.8 MB each | yes |
| `org.openjfx:javafx-graphics` (jar + `:linux`) | 21.0.6, 21.0.8 | ~5 MB each | yes |
| `org.openjfx:javafx-controls` (jar + `:linux`) | 21.0.6, 21.0.8 | ~3 MB each | yes |
| `org.openjfx:javafx-fxml` (jar + `:linux`) | 21.0.8 | ~0.2 MB each | yes |
| `org.apache.maven.plugins:maven-compiler-plugin` | 3.13.0 | ~0.1 MB | yes |
| `org.apache.maven.plugins:maven-surefire-plugin` | 3.5.2 | ~0.05 MB | yes |
| `org.openjfx:javafx-maven-plugin` | 0.0.8 | ~0.05 MB | yes |

`javafx-controls` depends on `javafx-graphics`, which depends on `javafx-base`;
the script fetches the whole chain. The full JavaFX **SDK** bundle (~49–59 MB)
is intentionally **not** used because it straddles the 50 MB limit — the
per-module JARs above are each well under it.

Sources: Oracle JavaFX archive downloads
(`https://www.oracle.com/java/technologies/javase/javafx21-archive-downloads.html`)
and `https://jdk.java.net/javafx21/`. Content was rephrased for compliance with
licensing restrictions.

## Native (C/C++) dependencies

Linked by the C/C++ Makefiles (`-l<name>`), confirmed by grepping the build:

| Library | Link flag | Used by | Typical dev-package size | ≤50 MB |
|---|---|---|---|---|
| nghttp2 | `-lnghttp2` | `impl` HTTP/2 server | < 2 MB | yes |
| OpenSSL (libssl/libcrypto) | `-lssl -lcrypto` | crypto / TLS, HTTP 3.0+ | ~10–15 MB (source) | yes |
| readline | `-lreadline` | sleela terminal | < 3 MB | yes |
| termcap/ncurses | `-ltermcap` | sleela terminal | ~3–4 MB | yes |
| ws2_32 | `-lws2_32` | Windows build only | n/a — Windows system lib | n/a |

`ws2_32` is a Windows system import library (ships with the Windows SDK/MinGW),
not a downloadable third-party library, so it is excluded.

The native script's default method is the system package manager's "download
only" mode (no install, no privilege escalation). A source-tarball fallback is
provided and commented.
