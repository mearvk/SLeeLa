package com.mearvk.sleela.ide

import com.intellij.psi.tree.IElementType
import com.intellij.psi.tree.TokenSet

/**
 * Stable [IElementType] instances keyed by the token kind strings produced by
 * [SleelaFallbackLexer] / the compiler bridge. Types are interned so the same
 * kind always maps to the same element type (IntelliJ requires identity).
 */
object SleelaTokenTypes {
    private val cache = linkedMapOf<String, IElementType>()

    val DIRECTIVE = of("DIRECTIVE")
    val LINE_COMMENT = of("LINE_COMMENT")
    val BLOCK_COMMENT = of("BLOCK_COMMENT")
    val STRING = of("STRING")
    val NUMBER = of("NUMBER")
    val KEYWORD = of("KEYWORD")
    val TYPE = of("TYPE")
    val IDENTIFIER = of("IDENTIFIER")
    val PUNCTUATION = of("PUNCTUATION")
    val OPERATOR = of("OPERATOR")
    val WHITESPACE = of("WHITESPACE")
    val BAD = of("BAD")

    fun of(kind: String): IElementType = cache.getOrPut(kind) {
        IElementType("SLEELA_$kind", SleelaLanguage)
    }

    val WHITE_SPACES: TokenSet = TokenSet.create(WHITESPACE)
    val COMMENTS: TokenSet = TokenSet.create(LINE_COMMENT, BLOCK_COMMENT)
    val STRINGS: TokenSet = TokenSet.create(STRING)
}
