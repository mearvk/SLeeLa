package com.mearvk.sleela.ide

import java.nio.file.Path

data class NativeToolchain(
    val cCompiler: Path? = null,
    val cppCompiler: Path? = null,
    val compileCommands: Path? = null,
    val target: String? = null,
    val sysroot: Path? = null
)

object NativeToolchainDiscovery {
    fun fromEnvironment(): NativeToolchain = NativeToolchain(
        cCompiler = System.getenv("CC")?.let(Path::of),
        cppCompiler = System.getenv("CXX")?.let(Path::of),
        compileCommands = System.getenv("COMPILE_COMMANDS")?.let(Path::of),
        target = System.getenv("TARGET"),
        sysroot = System.getenv("SYSROOT")?.let(Path::of)
    )
}