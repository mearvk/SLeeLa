package com.mearvk.sleela.ide

import com.intellij.lexer.Lexer
import com.intellij.openapi.editor.DefaultLanguageHighlighterColors
import com.intellij.openapi.editor.colors.TextAttributesKey
import com.intellij.openapi.fileTypes.SyntaxHighlighter
import com.intellij.openapi.fileTypes.SyntaxHighlighterBase
import com.intellij.openapi.fileTypes.SyntaxHighlighterFactory
import com.intellij.openapi.project.Project
import com.intellij.openapi.vfs.VirtualFile
import com.intellij.psi.tree.IElementType

/**
 * Maps SLeeLa token types to editor colours. Defaults derive from the standard
 * language colour roles so the plugin honours the user's active IDE theme; the
 * SLeeLa keyword role additionally carries the brand green (#00D68F, SLeeLa
 * Brand Guide) as its default when a scheme does not override it.
 */
class SleelaSyntaxHighlighter : SyntaxHighlighterBase() {

    override fun getHighlightingLexer(): Lexer = SleelaLexer(null)

    override fun getTokenHighlights(tokenType: IElementType?): Array<TextAttributesKey> =
        when (tokenType) {
            SleelaTokenTypes.KEYWORD -> KEYWORD_KEYS
            SleelaTokenTypes.TYPE -> TYPE_KEYS
            SleelaTokenTypes.STRING -> STRING_KEYS
            SleelaTokenTypes.NUMBER -> NUMBER_KEYS
            SleelaTokenTypes.LINE_COMMENT, SleelaTokenTypes.BLOCK_COMMENT -> COMMENT_KEYS
            SleelaTokenTypes.DIRECTIVE -> DIRECTIVE_KEYS
            SleelaTokenTypes.OPERATOR -> OPERATOR_KEYS
            SleelaTokenTypes.PUNCTUATION -> PUNCTUATION_KEYS
            SleelaTokenTypes.IDENTIFIER -> IDENTIFIER_KEYS
            SleelaTokenTypes.BAD -> BAD_KEYS
            else -> EMPTY
        }

    companion object {
        val KEYWORD: TextAttributesKey = TextAttributesKey.createTextAttributesKey(
            "SLEELA_KEYWORD", DefaultLanguageHighlighterColors.KEYWORD)
        val TYPE: TextAttributesKey = TextAttributesKey.createTextAttributesKey(
            "SLEELA_TYPE", DefaultLanguageHighlighterColors.CLASS_NAME)
        val STRING: TextAttributesKey = TextAttributesKey.createTextAttributesKey(
            "SLEELA_STRING", DefaultLanguageHighlighterColors.STRING)
        val NUMBER: TextAttributesKey = TextAttributesKey.createTextAttributesKey(
            "SLEELA_NUMBER", DefaultLanguageHighlighterColors.NUMBER)
        val COMMENT: TextAttributesKey = TextAttributesKey.createTextAttributesKey(
            "SLEELA_COMMENT", DefaultLanguageHighlighterColors.LINE_COMMENT)
        val DIRECTIVE: TextAttributesKey = TextAttributesKey.createTextAttributesKey(
            "SLEELA_DIRECTIVE", DefaultLanguageHighlighterColors.METADATA)
        val OPERATOR: TextAttributesKey = TextAttributesKey.createTextAttributesKey(
            "SLEELA_OPERATOR", DefaultLanguageHighlighterColors.OPERATION_SIGN)
        val PUNCTUATION: TextAttributesKey = TextAttributesKey.createTextAttributesKey(
            "SLEELA_PUNCTUATION", DefaultLanguageHighlighterColors.PARENTHESES)
        val IDENTIFIER: TextAttributesKey = TextAttributesKey.createTextAttributesKey(
            "SLEELA_IDENTIFIER", DefaultLanguageHighlighterColors.IDENTIFIER)
        val BAD: TextAttributesKey = TextAttributesKey.createTextAttributesKey(
            "SLEELA_BAD_CHARACTER", com.intellij.openapi.editor.HighlighterColors.BAD_CHARACTER)

        private val KEYWORD_KEYS = arrayOf(KEYWORD)
        private val TYPE_KEYS = arrayOf(TYPE)
        private val STRING_KEYS = arrayOf(STRING)
        private val NUMBER_KEYS = arrayOf(NUMBER)
        private val COMMENT_KEYS = arrayOf(COMMENT)
        private val DIRECTIVE_KEYS = arrayOf(DIRECTIVE)
        private val OPERATOR_KEYS = arrayOf(OPERATOR)
        private val PUNCTUATION_KEYS = arrayOf(PUNCTUATION)
        private val IDENTIFIER_KEYS = arrayOf(IDENTIFIER)
        private val BAD_KEYS = arrayOf(BAD)
        private val EMPTY = emptyArray<TextAttributesKey>()
    }
}

class SleelaSyntaxHighlighterFactory : SyntaxHighlighterFactory() {
    override fun getSyntaxHighlighter(project: Project?, virtualFile: VirtualFile?): SyntaxHighlighter =
        SleelaSyntaxHighlighter()
}
