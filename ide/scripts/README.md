# SLeeLa IDE build scripts

Bash scripts that reproduce the "downloads"/build steps for the SLeeLa IDE. Run
them from anywhere; each resolves the repo layout relative to its own location.

| Script | What it produces | Network? |
|---|---|---|
| `gen-gradle-wrapper.sh` | `sleela-intellij/gradle/wrapper/gradle-wrapper.jar` (pinned to Gradle 8.14.3) | No |
| `build-native-compiler.sh` | `impl/build/sleela` — the native compiler the IDE drives | No |
| `build-plugin.sh` | `sleela-intellij/build/distributions/sleela-intellij-<ver>.zip` | **Yes** (IntelliJ SDK) |
| `build-all.sh` | All of the above in order | Yes (step 3 only) |
| `common.sh` | Shared helpers (JDK 21 discovery, gradle launcher) — sourced, not run | — |

## Requirements

- **JDK 21** — auto-detected (mise, `/usr/lib/jvm`, macOS); or set `JAVA_HOME`.
  The IntelliJ Platform Gradle Plugin 2.x rejects JDK 25+, so 21 is required.
- **gcc / g++ / make / python3** — for the native compiler.
- A **system `gradle`** — only needed by `gen-gradle-wrapper.sh` to mint the jar.
- **Network** — only `build-plugin.sh` needs it (downloads the IC 2026.1 SDK,
  Kotlin, and the IntelliJ Platform Gradle Plugin from JetBrains + Maven Central).

## Quick start

```sh
cd ide/scripts

# Offline-buildable pieces (wrapper jar + native compiler):
SKIP_PLUGIN=1 ./build-all.sh

# Everything, including the installable plugin ZIP (needs network):
./build-all.sh
```

## Notes

- `gen-gradle-wrapper.sh` is idempotent; set `FORCE=1` to regenerate the jar.
- `build-native-compiler.sh` defaults the mandatory SHA-256 verify gate to
  `security/sha256-manifest.json`; override with `SLEELA_SHA256_MANIFEST`.
  Pass `all` as an argument to also build Nordshrift and the staged package.
- `build-plugin.sh` accepts a Gradle task: `build`, `runIde`, `verifyPlugin`,
  or `buildPlugin` (default).
- The IDE auto-discovers the compiler at `impl/build/sleela`; override its
  location with the `SLEELA_COMPILER` environment variable.
