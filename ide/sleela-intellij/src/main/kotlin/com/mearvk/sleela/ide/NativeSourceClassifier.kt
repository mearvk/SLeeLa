package com.mearvk.sleela.ide

import java.nio.file.Path

enum class NativeLanguage { C, CPP, HEADER, UNKNOWN }

object NativeSourceClassifier {
    fun classify(path: Path): NativeLanguage = when (path.fileName.toString().substringAfterLast('.', "").lowercase()) {
        "c" -> NativeLanguage.C
        "cpp", "cc", "cxx" -> NativeLanguage.CPP
        "h", "hh", "hpp", "hxx" -> NativeLanguage.HEADER
        else -> NativeLanguage.UNKNOWN
    }

    fun isNative(path: Path): Boolean = classify(path) != NativeLanguage.UNKNOWN
}