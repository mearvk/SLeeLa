package com.mearvk.sleela.ide.workbench

import com.intellij.execution.configurations.GeneralCommandLine
import com.intellij.execution.process.CapturingProcessHandler
import com.intellij.openapi.diagnostic.logger
import com.intellij.openapi.project.Project
import com.mearvk.sleela.ide.SleelaCommandRunner
import com.mearvk.sleela.ide.SleelaCompilerMissingException
import com.mearvk.sleela.ide.SleelaDiagnosticParser
import com.mearvk.sleela.ide.Severity
import java.nio.file.Files
import java.nio.file.Path

/**
 * The host-side service that performs the real work behind the workbench's
 * commands, implementing the SLIdeWorkbench.compileActive / saveActive loop
 * against the authoritative toolchain.
 *
 * Compile routing follows lib/nordshrift and SLEELA.IDE.CONTRACT.md:
 *   .sst    -> `sleela nordshrift --emit` (SST Nordshrit) then `sleela check`
 *   .sleela -> `sleela check` directly
 * The SLeeLa compiler stays authoritative; this service never reimplements it.
 */
object SleelaWorkbenchService {

    private val LOG = logger<SleelaWorkbenchService>()

    /** Upper bound for a single compile/transpile invocation. */
    private const val COMPILE_TIMEOUT_MS = 60_000

    data class CompileOutcome(
        val ok: Boolean,
        val diagnostics: List<WorkbenchDiagnostics.Entry>,
        val rawOutput: String,
    )

    /**
     * Compile [editor]'s current buffer. [source] is the live editor text (so
     * unsaved edits are honored). Returns parsed diagnostics; a missing compiler
     * is reported as a single ERROR, preserving the IDE failure rule.
     */
    fun compile(project: Project, editor: WorkbenchEditor, source: String): CompileOutcome {
        val sb = StringBuilder()

        // .sst goes through Nordshrift first (SST Nordshrit path), producing the
        // .sleela source the compiler then checks.
        if (editor.isSst) {
            val transpile = runSubcommand(
                project, sb,
                "nordshrift", "--emit", "--target=sleela", pathArg(editor, source, ".sst"),
            ) ?: return missingCompiler(editor, sb)
            if (transpile != 0) {
                return CompileOutcome(
                    ok = false,
                    diagnostics = listOf(
                        WorkbenchDiagnostics.Entry(
                            editor.documentId, Severity.ERROR, 1, 1,
                            "Nordshrift could not transpile the .sst sheet (exit $transpile)",
                        ),
                    ),
                    rawOutput = sb.toString(),
                )
            }
        }

        // The authoritative check step over the SLeeLa source.
        val checkPath = pathArg(editor, source, if (editor.isSst) ".sleela" else ".sleela")
        val exit = runSubcommand(project, sb, "check", checkPath)
            ?: return missingCompiler(editor, sb)

        val parsed = SleelaDiagnosticParser
            .parse(sb.toString(), source, Path.of(checkPath))
            .map { d ->
                val (line, col) = lineColOf(source, d.start)
                WorkbenchDiagnostics.Entry(editor.documentId, d.severity, line, col, d.message)
            }

        val ok = parsed.none { it.severity == Severity.ERROR } && exit == 0
        return CompileOutcome(ok, parsed, sb.toString())
    }

    /** Run `sleela <args>` capturing combined output into [sink]; null when the compiler is missing. */
    private fun runSubcommand(
        project: Project,
        sink: StringBuilder,
        vararg args: String,
    ): Int? {
        val commandLine: GeneralCommandLine = try {
            SleelaCommandRunner.commandLine(project, *args)
        } catch (_: SleelaCompilerMissingException) {
            return null
        }
        return try {
            // CapturingProcessHandler merges stdout+stderr and applies the
            // command line's charset; a bounded timeout keeps the UI responsive.
            val handler = CapturingProcessHandler(commandLine)
            val output = handler.runProcess(COMPILE_TIMEOUT_MS)
            sink.append(output.stdout)
            if (output.stderr.isNotEmpty()) sink.append(output.stderr)
            if (output.isTimeout) {
                sink.appendLine("sleelvac: timed out after ${COMPILE_TIMEOUT_MS}ms")
                -1
            } else {
                output.exitCode
            }
        } catch (t: Throwable) {
            LOG.warn("sleela ${args.joinToString(" ")} failed", t)
            sink.appendLine("sleelvac: internal error: ${t.message}")
            -1
        }
    }

    private fun missingCompiler(editor: WorkbenchEditor, sb: StringBuilder): CompileOutcome {
        sb.appendLine("sleelvac: compiler not found")
        return CompileOutcome(
            ok = false,
            diagnostics = listOf(
                WorkbenchDiagnostics.Entry(
                    editor.documentId, Severity.ERROR, 1, 1,
                    "SLeeLa compiler not found (run `make -C impl` or set SLEELA_COMPILER)",
                ),
            ),
            rawOutput = sb.toString(),
        )
    }

    /**
     * The compiler reads from disk. When the editor has an on-disk path we pass
     * it directly; otherwise (untitled / unsaved) we spill the buffer to a temp
     * file with the right extension so the toolchain can read it.
     */
    private fun pathArg(editor: WorkbenchEditor, source: String, ext: String): String {
        if (editor.path.isNotEmpty() && editor.path.endsWith(ext)) return editor.path
        val tmp = Files.createTempFile("sleela-workbench-", ext)
        Files.writeString(tmp, source)
        return tmp.toString()
    }

    private fun lineColOf(source: String, offset: Int): Pair<Int, Int> {
        var line = 1; var col = 1
        val end = offset.coerceIn(0, source.length)
        for (i in 0 until end) {
            if (source[i] == '\n') { line++; col = 1 } else col++
        }
        return line to col
    }
}
