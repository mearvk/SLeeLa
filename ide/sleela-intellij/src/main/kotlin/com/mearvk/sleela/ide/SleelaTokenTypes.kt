package com.mearvk.sleela.ide

import com.intellij.psi.tree.IElementType

object SleelaTokenTypes {
    private val cache = linkedMapOf<String, IElementType>()

    fun of(kind: String): IElementType = cache.getOrPut(kind) {
        IElementType("SLEEELA_$kind", SleelaLanguage)
    }
}