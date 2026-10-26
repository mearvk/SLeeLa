# SLeeLa Java Connector

The top-level `connector/` module contains Java-side connectors, the JDK bridge,
class and native-library loaders, HTTP servlet support, and deployment examples.

## Requirements

- JDK 22 or newer. The native-library connector uses the finalized Java Foreign
  Function & Memory API introduced in JDK 22.
- GNU Make and a POSIX-compatible shell (Linux/macOS, or Windows with MSYS2 or
  another environment that provides GNU Make and a POSIX shell).
- `curl` for downloading the Jakarta Servlet API dependency on first build.
- Python 3 is optional and checks that `deploy/web.xml` is well-formed.
- A Jakarta Servlet API 6.0 JAR is needed to compile the HTTP servlet source.
  The Makefile downloads it into generated `build/deps/` by default. Set
  `SERVLET_API_JAR=/path/to/jakarta.servlet-api.jar` to use a local copy.

## Build and test

From the repository root:

```sh
make -C connector help
make -C connector clean
make -C connector all
make -C connector test
make -C connector jar
```

Compiled classes are written to `connector/build/classes/`, downloaded
dependencies go under `connector/build/deps/`, and the packaged JAR is
`connector/build/dist/sleela-connector.jar`. The repository's existing
`**/build/` ignore rule excludes these generated outputs from Git.

Install the JAR into a directory you control:

```sh
make -C connector install DESTDIR=/opt/sleela/connector
```

On Windows, use a GNU Make/POSIX shell environment such as MSYS2. If the Java
tools require Windows classpath separators, pass `CLASSPATH_SEP=';'` to Make.
This is a GNU Makefile, not an MSVC `nmake` file.

## Targets

| Target | Description |
|---|---|
| `all`, `build`, `compile` | Compile all Java connector sources |
| `deps` | Download Jakarta Servlet API 6.0.0 |
| `test` | Run JDK bridge conformance and deployment-file checks |
| `conformance` | Run `SleelaJdkConformance` with assertions enabled |
| `jar`, `package` | Build the connector JAR |
| `check` | Check required deployment files and validate `web.xml` XML |
| `install` | Copy the JAR to an explicit `DESTDIR` |
| `clean` | Remove generated build output |
| `help` | List targets and overridable settings |

## HTTP deployment notes

The JAR includes compiled connector classes and the servlet implementation, but
does not bundle a servlet container or the Jakarta Servlet API. Deploy the
servlet in a compatible container such as Tomcat and follow
[the deployment guide](deploy/README.md). The Apache and Tomcat files in
`deploy/` are examples and must be reviewed for the target server, TLS
settings, backend pipeline wiring, and operational policy before production use.

## Security note

The class and native-library loaders enforce their documented allow-list and
root policies, but loading native code executes with the process's privileges.
Only allow trusted libraries and directories. This build does not sandbox native
code.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>
