package com.mearvk.sleela.ide

import java.nio.file.Path

/**
 * Adapts native `sleela check` output into [SleelaDiagnostic]s.
 *
 * Observed native format (Sleelvac 0.3.x):
 * ```
 * sleelvac: <file>: Syntax error (line 3, col 22): unexpected token ';' in expression
 * <file>: ok (syntax declared 1.3)
 * ```
 * Compiler positions are one-based (line, column); the IDE converts them to
 * zero-based character offsets against the editor source (SLEELA.IDE.CONTRACT.md).
 */
object SleelaDiagnosticParser {

    // sleelvac: <path>: <severity-ish> (line N, col M): <message>
    private val LOCATED = Regex(
        """sleelvac:\s*.*?:\s*(.+?)\s*\(line\s+(\d+),\s*col\s+(\d+)\)\s*:\s*(.*)""",
        RegexOption.IGNORE_CASE,
    )
    // sleelvac: <path>: <message>   (no location)
    private val UNLOCATED = Regex("""sleelvac:\s*.*?:\s*(.*)""", RegexOption.IGNORE_CASE)

    fun parse(output: String, source: String, @Suppress("UNUSED_PARAMETER") file: Path): List<SleelaDiagnostic> {
        val lineIndex = LineIndex(source)
        val diagnostics = mutableListOf<SleelaDiagnostic>()
        for (raw in output.lineSequence()) {
            val text = raw.trim()
            if (text.isEmpty()) continue
            if (!text.startsWith("sleelvac:", ignoreCase = true)) continue
            if (text.contains(": ok", ignoreCase = true)) continue

            val located = LOCATED.find(text)
            if (located != null) {
                val (kind, lineStr, colStr, message) = located.destructured
                val offset = lineIndex.offsetOf(lineStr.toInt(), colStr.toInt())
                diagnostics += SleelaDiagnostic(
                    severity = severityOf(kind, message),
                    message = message.ifBlank { kind }.trim(),
                    start = offset,
                    end = lineIndex.endOfToken(offset),
                    code = "SLEELA-CHECK",
                )
                continue
            }
            val unlocated = UNLOCATED.find(text)
            if (unlocated != null) {
                val message = unlocated.groupValues[1].trim()
                if (message.isEmpty() || message.equals("ok", ignoreCase = true)) continue
                diagnostics += SleelaDiagnostic(
                    severity = severityOf(message, message),
                    message = message,
                    start = 0,
                    end = 0,
                    code = "SLEELA-CHECK",
                )
            }
        }
        return diagnostics
    }

    private fun severityOf(kind: String, message: String): Severity {
        val haystack = (kind + " " + message).lowercase()
        return when {
            "error" in haystack || "fatal" in haystack -> Severity.ERROR
            "warn" in haystack -> Severity.WARNING
            else -> Severity.ERROR // `check` only reports when something is wrong
        }
    }

    /** Maps one-based (line, column) positions to zero-based character offsets. */
    private class LineIndex(private val source: String) {
        private val lineStartOffsets: IntArray

        init {
            val starts = mutableListOf(0)
            source.forEachIndexed { i, c -> if (c == '\n') starts.add(i + 1) }
            lineStartOffsets = starts.toIntArray()
        }

        fun offsetOf(line1: Int, col1: Int): Int {
            val line = (line1 - 1).coerceIn(0, lineStartOffsets.size - 1)
            val base = lineStartOffsets[line]
            val offset = base + (col1 - 1).coerceAtLeast(0)
            return offset.coerceIn(0, source.length)
        }

        /** Extends to the end of the identifier/token at [offset] for a usable highlight range. */
        fun endOfToken(offset: Int): Int {
            var i = offset
            while (i < source.length && (source[i].isLetterOrDigit() || source[i] == '_')) i++
            return if (i > offset) i else (offset + 1).coerceAtMost(source.length)
        }
    }
}
