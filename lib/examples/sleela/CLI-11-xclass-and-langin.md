# XClass and JVM-family language input

XClass accepts `--run` (default), `--emit`, or `--info`:

```sh
sleela xclass --info path/to/compiled.xclass
sleela xclass --emit path/to/compiled.xclass
sleela xclass --run path/to/compiled.xclass
```

Langin accepts `--run` (default), `--emit-sleela`, `--emit-xclass`, or
`--info`. `--emit` is an alias for `--emit-sleela`.

```sh
sleela langin --info path/to/Example.java
sleela langin --emit-sleela path/to/Example.java
sleela langin --emit-xclass path/to/Example.kt
sleela langin --run path/to/Example.scala
```

The documented JVM-family inputs are Java, Kotlin, Scala, Groovy, and Clojure.
Use a valid source file and inspect generated output before executing it.
