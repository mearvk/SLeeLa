package com.mearvk.sleela.ide

import com.intellij.lexer.LexerBase
import com.intellij.openapi.project.Project
import com.intellij.psi.tree.IElementType
import java.nio.file.Path

/**
 * IntelliJ lexer adapter.
 *
 * Tokens come from the authoritative SLeeLa compiler bridge when a provider has
 * been installed; otherwise the plugin uses [SleelaFallbackLexer], a structural
 * tokenizer derived from SLEELA.syntax, so syntax highlighting works without a
 * running native compiler. Either way the IDE never invents SLeeLa *semantics* —
 * parsing and diagnostics remain the compiler's responsibility.
 */
class SleelaLexer(private val project: Project?) : LexerBase() {
    private var bufferText = ""
    private var tokens = emptyList<SleelaToken>()
    private var index = 0
    private var tokenStart = 0
    private var tokenEnd = 0
    private var current: SleelaToken? = null

    override fun start(buffer: CharSequence, startOffset: Int, endOffset: Int, initialState: Int) {
        bufferText = buffer.subSequence(startOffset, endOffset).toString()
        index = 0
        current = null
        tokenStart = startOffset
        tokenEnd = startOffset
        tokens = lexTokens(bufferText)
        advance()
    }

    private fun lexTokens(text: String): List<SleelaToken> {
        val root = project?.basePath?.let(Path::of)
        if (root != null && SleelaCompilerProviders.isInstalled()) {
            try {
                return SleelaCompilerProviders.create(root).lex(text, Path.of("<editor>.sleela")).tokens
            } catch (_: Exception) {
                // Fall through to the local structural lexer.
            }
        }
        return SleelaFallbackLexer.tokenize(text)
    }

    override fun getState() = 0
    override fun getTokenType(): IElementType? = current?.let { SleelaTokenTypes.of(it.kind) }
    override fun getTokenStart() = tokenStart
    override fun getTokenEnd() = tokenEnd
    override fun getBufferSequence(): CharSequence = bufferText
    override fun getBufferEnd() = bufferText.length

    override fun advance() {
        if (index >= tokens.size) {
            current = null
            tokenStart = bufferText.length
            tokenEnd = bufferText.length
            return
        }
        current = tokens[index++]
        tokenStart = current!!.start
        tokenEnd = current!!.end
    }
}
