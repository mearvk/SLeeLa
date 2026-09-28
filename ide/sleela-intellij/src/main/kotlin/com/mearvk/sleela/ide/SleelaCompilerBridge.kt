package com.mearvk.sleela.ide

import java.nio.file.Path

/**
 * Adapter boundary to the authoritative SLeeLa compiler.
 *
 * The IDE never owns SLeeLa semantics; a repository compiler adapter supplies
 * tokens, AST facts, diagnostics, and symbol information.
 */
interface SleelaCompilerBridge {
    fun lex(source: String, file: Path): SleelaLexResult
    fun parse(source: String, file: Path): SleelaParseResult
    fun diagnostics(source: String, file: Path, project: Path): List<SleelaDiagnostic>
}

data class SleelaLexResult(val tokens: List<SleelaToken>)
data class SleelaParseResult(val root: SleelaAstNode?)
data class SleelaToken(val kind: String, val text: String, val start: Int, val end: Int, val line: Int, val column: Int)
data class SleelaAstNode(val kind: String, val start: Int, val end: Int, val children: List<SleelaAstNode> = emptyList())
data class SleelaDiagnostic(val severity: Severity, val message: String, val start: Int, val end: Int, val code: String? = null)
enum class Severity { INFO, WARNING, ERROR }