# Presumed JDK source trees

`javaconv` (registered in sleelactl as `java-convert`) converts the Java source
of a **presumed JDK version** into a functionally equivalent SLeeLa version.

```
sleelactl/jdk/<N>/java/     INPUT   the JDK <N> Java source tree (you place it here; not committed)
sleelactl/jdk/<N>/sleela/   OUTPUT  the converted SLeeLa tree + CONVERSION-REPORT.md / .json
```

Use any layout under `java/` (`java.base/java/util/...` from `src.zip`, or flat
packages). Relative paths are mirrored under `sleela/`.

```sh
# put the JDK 28 sources in place (example; src.zip ships with every JDK)
unzip -q "$JAVA_HOME/lib/src.zip" -d sleelactl/jdk/28/java

bin/sleelactl build java-convert
bin/sleelactl run   java-convert --jdk-version 28
```

The JDK source is GPLv2 with the Classpath Exception and is not committed
(`.gitignore`). Output is derived from it: carry its notices if you publish it.
The report states exactly what converted and what failed closed and why.
