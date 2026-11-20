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

## Last measurement

2026-11-13, JDK 25.0.2 `src.zip`, `java.base` packages `java/lang`, `java/util`, `java/math`, `java/text`
(with subpackages; 884 files):

```sh
unzip -q "$JAVA_HOME/lib/src.zip" 'java.base/java/lang/*' 'java.base/java/util/*' 'java.base/java/math/*' 'java.base/java/text/*' -d /tmp/jdk25
sleelactl/convert/build/javaconv --jdk /tmp/jdk25 --out /tmp/jdk25-sleela --jdk-version 25
```

Result (model 1.3.0): 420 converted (496 SLeeLa files), 464 failed closed; 426 with `--single-thread`. Details and the
refusal table: `sleelactl/convert/README.md`.

2026-11-20, Corretto 25.0.5.1 `src.zip` (extract with Python `zipfile`; the same four packages, 884 files): 254 converted
(262 SLeeLa files), 630 failed closed, 251 of 262 emitted files pass `tests/javaconv_check_tree.sh`. All of `java.base`
(3344 files, about 5 minutes): 841 converted (859 SLeeLa files), 2503 failed closed, 806 of 859 emitted files pass.

2026-11-20 (later), same input, after the field-resolution fixes: subset 259 converted (267 SLeeLa files), 625 failed
closed, 256 of 267 emitted files pass `tests/javaconv_check_tree.sh`. All of `java.base`: 842 converted (860 SLeeLa
files), 2502 failed closed; the whole-tree checker was not re-run on it.

2026-11-20 (final), same input, after the access and indirect-call fixes: subset 262 converted (270 SLeeLa files), 622
failed closed. All of `java.base`: 862 converted (879 SLeeLa files), 2482 failed closed. The whole-tree checker was not
re-run on this generation.
