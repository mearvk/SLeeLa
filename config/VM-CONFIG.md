# SLeeLa VM Selection Configuration

config/vm.conf is the source template for persistent VM selection. A launcher loads it from the resolved configuration root or uses the repository template during development.

## Supported versions

vm.version accepts integer generations 1 through 11; default is 11. This selects a requested generation, not a guarantee that its runtime artifacts are installed. /impl remains the operational Core substrate while VM generations provide their defined execution and hosting layers.

## Selection precedence

1. Explicit launcher/CLI VM selection.
2. SLEELA_VM_VERSION environment override, when applied by the launcher.
3. vm.version in the selected vm.conf.
4. Compiled default (11).

With vm.strict = true, invalid versions must fail validation rather than be silently changed. A launcher must separately verify that the requested VM is available; the current C parser validates the version range, not installed-artifact presence.

vm.profile is descriptive ownership metadata: user, administrator, or developer. It does not grant capabilities. The key/value format is intended to be portable across Linux, Windows, and macOS. Validate with make config from the repository root.

## Java host and interoperability settings

Java is a separate, optional execution surface. Selecting VM generation 10 or 11 does **not** mean that the SLeeLa VM embeds a JVM or implements Java bytecode.

- `java.enabled`: permits or disables the external Java interoperability path.
- `java.mode = external`: launches a configured Java executable as a child process. This is the dependency-light default.
- `java.mode = disabled`: refuses Java delegation and reports a clear diagnostic.
- `java.mode = embedded`: reserved for a future opt-in JNI Invocation API build. It must not be treated as implemented merely because it appears in configuration.
- `java.executable = auto`: discover Java using SLEELA_JAVA, JAVA_HOME, then PATH.
- `java.classpath`: default classpath for compiled Java entry points.
- `javafx.module_path` and `javafx.modules`: optional JavaFX module configuration; JavaFX classes and native platform libraries must actually be installed.
- `java.headless = auto`: detect the environment where practical and report that graphical UI may be unavailable.

For direct command-line launches, the initial supported forms are `sleela run app.jar [args...]`, `sleela run --class-path <path> <MainClass> [args...]`, and `sleela run --class <MainClass> [args...]`. The JAR must declare `Main-Class`; class files must be under the supplied class-path root with package directories intact. These forms currently consume the external-Java environment discovery and enable/disable controls; the key/value file settings are not yet parsed by the launcher.

Capability reporting must distinguish detection from verified execution. A Java executable does not prove that Swing can open a display or that JavaFX modules are installed. Runtime configuration keys are descriptive until the launcher consumes them; unsupported settings must not be reported as active behavior.

`config/slvm-runtime.conf` is a separate SLVM/6 runtime profile (`vm.instance = 6`), not a competing global default. It applies only when a deployment explicitly selects that SLVM/6 profile; `config/vm.conf` remains the general default selection (generation 11).
