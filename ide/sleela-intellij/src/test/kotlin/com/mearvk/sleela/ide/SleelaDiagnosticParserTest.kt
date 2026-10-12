package com.mearvk.sleela.ide

import org.junit.jupiter.api.Assertions.assertEquals
import org.junit.jupiter.api.Assertions.assertTrue
import org.junit.jupiter.api.Test
import java.nio.file.Path

class SleelaDiagnosticParserTest {

    private val badSource = "#sleela 1.3\nclass Bad {\n  void m() { int x = ; }\n}\n"

    @Test
    fun parsesLocatedSyntaxError() {
        val output = "sleelvac: /tmp/Bad.sleela: Syntax error (line 3, col 22): unexpected token ';' in expression"
        val diags = SleelaDiagnosticParser.parse(output, badSource, Path.of("/tmp/Bad.sleela"))
        assertEquals(1, diags.size)
        val d = diags.first()
        assertEquals(Severity.ERROR, d.severity)
        assertTrue(d.message.contains("unexpected token"))
        // line 3, col 22 -> offset within source; must be a valid, non-negative offset.
        assertTrue(d.start in 0..badSource.length)
        assertTrue(d.end >= d.start)
    }

    @Test
    fun ignoresOkLines() {
        val output = "/tmp/Hello.sleela: ok (syntax declared 1.3)"
        val diags = SleelaDiagnosticParser.parse(output, "class Hello {}", Path.of("/tmp/Hello.sleela"))
        assertTrue(diags.isEmpty())
    }

    @Test
    fun lineColumnMapsToExpectedOffset() {
        // "#sleela 1.3\n" is 12 chars (incl. newline). Line 3 col 22:
        // line1 = "#sleela 1.3", line2 = "class Bad {", line3 = "  void m() { int x = ; }"
        val output = "sleelvac: x: Syntax error (line 3, col 1): test"
        val diags = SleelaDiagnosticParser.parse(output, badSource, Path.of("x"))
        // line 3 starts after lines 1 and 2 (12 + 12 = 24).
        assertEquals(24, diags.first().start)
    }
}
