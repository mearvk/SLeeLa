package com.mearvk.sleela.ide

import com.intellij.openapi.fileTypes.LanguageFileType

object SleelaFileType : LanguageFileType(SleelaLanguage) {
    override fun getName() = "SLeeLa"
    override fun getDescription() = "SLeeLa source file"
    override fun getDefaultExtension() = "sleela"
    override fun getIcon() = null
}
