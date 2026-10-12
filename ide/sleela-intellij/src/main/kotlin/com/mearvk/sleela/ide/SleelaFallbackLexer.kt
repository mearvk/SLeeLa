package com.mearvk.sleela.ide

/**
 * A structural tokenizer for SLeeLa, derived from the lexical grammar in
 * `SLEELA.syntax` (§1). It is NOT a second definition of the language: the
 * native compiler remains authoritative for parsing and semantics. This lexer
 * exists so the editor can colour tokens and build a flat PSI even before the
 * native front end exposes a token stream over the CLI.
 *
 * Keyword inventory tracks SLEELA.syntax §1.2 (reserved words) and the
 * contextual built-ins noted there.
 */
object SleelaFallbackLexer {

    /** Reserved keywords (SLEELA.syntax §1.2). */
    val KEYWORDS = setOf(
        "class", "static", "void", "struct", "new",
        "int", "double", "boolean", "String",
        "if", "else", "while", "for", "return",
        "true", "false", "null", "print",
        "let", "extends", "to", "linear", "group", "services",
        "gc", "mem",
    )

    /** Primitive/type names highlighted distinctly. */
    val TYPES = setOf("int", "double", "boolean", "String", "void", "struct")

    fun tokenize(source: String): List<SleelaToken> {
        val tokens = mutableListOf<SleelaToken>()
        var i = 0
        var line = 1
        var col = 1
        val n = source.length

        fun advanceOver(start: Int, end: Int) {
            var k = start
            while (k < end) {
                if (source[k] == '\n') { line++; col = 1 } else { col++ }
                k++
            }
        }

        while (i < n) {
            val c = source[i]
            val startLine = line
            val startCol = col

            when {
                c == '#' && (i == 0 || source[i - 1] == '\n') -> {
                    // #sleela version directive (SLEELA.syntax §1.5) — treat whole line
                    val end = source.indexOf('\n', i).let { if (it < 0) n else it }
                    tokens += SleelaToken("DIRECTIVE", source.substring(i, end), i, end, startLine, startCol)
                    advanceOver(i, end); i = end
                }
                c == '/' && i + 1 < n && source[i + 1] == '/' -> {
                    val end = source.indexOf('\n', i).let { if (it < 0) n else it }
                    tokens += SleelaToken("LINE_COMMENT", source.substring(i, end), i, end, startLine, startCol)
                    advanceOver(i, end); i = end
                }
                c == '/' && i + 1 < n && source[i + 1] == '*' -> {
                    val close = source.indexOf("*/", i + 2)
                    val end = if (close < 0) n else close + 2
                    tokens += SleelaToken("BLOCK_COMMENT", source.substring(i, end), i, end, startLine, startCol)
                    advanceOver(i, end); i = end
                }
                c == '"' -> {
                    var k = i + 1
                    while (k < n && source[k] != '"') { if (source[k] == '\\' && k + 1 < n) k++; k++ }
                    val end = (k + 1).coerceAtMost(n)
                    tokens += SleelaToken("STRING", source.substring(i, end), i, end, startLine, startCol)
                    advanceOver(i, end); i = end
                }
                c.isWhitespace() -> {
                    var k = i
                    while (k < n && source[k].isWhitespace()) k++
                    tokens += SleelaToken("WHITESPACE", source.substring(i, k), i, k, startLine, startCol)
                    advanceOver(i, k); i = k
                }
                c.isDigit() -> {
                    var k = i
                    while (k < n && (source[k].isDigit() || source[k] == '.' || source[k] == '_')) k++
                    tokens += SleelaToken("NUMBER", source.substring(i, k), i, k, startLine, startCol)
                    advanceOver(i, k); i = k
                }
                c.isLetter() || c == '_' -> {
                    var k = i
                    while (k < n && (source[k].isLetterOrDigit() || source[k] == '_')) k++
                    val word = source.substring(i, k)
                    val kind = when {
                        word in TYPES -> "TYPE"
                        word in KEYWORDS -> "KEYWORD"
                        else -> "IDENTIFIER"
                    }
                    tokens += SleelaToken(kind, word, i, k, startLine, startCol)
                    advanceOver(i, k); i = k
                }
                c in "{}()[];,." -> {
                    tokens += SleelaToken("PUNCTUATION", c.toString(), i, i + 1, startLine, startCol)
                    col++; i++
                }
                c in "+-*/%=<>!&|^~?:" -> {
                    var k = i
                    while (k < n && source[k] in "+-*/%=<>!&|^~?:") k++
                    tokens += SleelaToken("OPERATOR", source.substring(i, k), i, k, startLine, startCol)
                    advanceOver(i, k); i = k
                }
                else -> {
                    tokens += SleelaToken("BAD", c.toString(), i, i + 1, startLine, startCol)
                    col++; i++
                }
            }
        }
        return tokens
    }
}
