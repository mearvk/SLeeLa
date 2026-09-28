package com.mearvk.sleela.ide

import java.nio.file.Files
import java.nio.file.Path

data class SleelaLibraryEntry(val root: Path, val files: List<Path>)

object SleelaLibraryIndex {
    fun scan(roots: List<Path>): List<SleelaLibraryEntry> = roots.filter(Files::isDirectory).map { root ->
        SleelaLibraryEntry(root, Files.walk(root).filter { Files.isRegularFile(it) && it.toString().endsWith(".sleela") }.toList())
    }
}