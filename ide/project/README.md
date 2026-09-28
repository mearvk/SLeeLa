[![SLeeLa](https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png)](https://github.com/mearvk/SLeeLa)
# IDE Project Model

A SLeeLa project can contain SLeeLa source, C source/headers, C++ source/headers, Java source, generated source, SLeeLa standard-library objects, native libraries, JVM dependencies and tests.

Module metadata should record:
```
language
sourceRoots
testRoots
generatedRoots
dependencies
nativeToolchain
jvmToolchain
sleelaToolchain
outputDirectory
runTargets
testTargets
debugTargets
```

The model is independent from a single build system.
