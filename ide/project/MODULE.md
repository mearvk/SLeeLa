# SLeeLa IDE Module Model

Version: 0.2.0-dev

A module is a buildable unit containing one or more language domains.

## Module manifest

`name`
`languages`
`sourceRoots`
`testRoots`
`generatedRoots`
`sleelaRoot`
`libRoots`
`includeRoots`
`nativeLibraries`
`jvmDependencies`
`compiler`
`cCompiler`
`cppCompiler`
`javaToolchain`
`buildCommand`
`runCommand`
`testCommand`
`debugCommand`
`outputDirectory`

## Language domains
A module may contain SLeeLa, C, C++, and Java. The language domains share project identity but retain their own compilers and semantic authorities.

## /lib
All configured SLeeLa `/lib` roots are indexed. The index records library identity, exported symbols, source/object provenance, and compiler/toolchain compatibility.