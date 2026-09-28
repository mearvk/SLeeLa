package com.mearvk.sleela.ide

import java.nio.file.Path

data class SleelaModuleModel(
    val name: String,
    val sourceRoots: List<Path>,
    val testRoots: List<Path>,
    val libRoots: List<Path>,
    val nativeIncludeRoots: List<Path> = emptyList(),
    val nativeLibraries: List<Path> = emptyList(),
    val outputDirectory: Path? = null
 ) {
    fun sleelaFiles(): Sequence<Path> = sourceRoots.asSequence().flatMap { root ->
        java.nio.file.Files.walk(root).filter { it.toString().endsWith(".sleela") }.iterator().asSequence()
    }
}