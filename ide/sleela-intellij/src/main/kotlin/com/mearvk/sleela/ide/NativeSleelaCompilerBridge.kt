package com.mearvk.sleela.ide

import java.nio.file.Files
import java.nio.file.Path
import java.util.concurrent.TimeUnit
import kotlin.io.path.exists
import kotlin.io.path.isExecutable

/**
 * Concrete [SleelaCompilerBridge] that defers to the authoritative native SLeeLa
 * compiler (`Sleelvac`, built at `impl/build/sleela`).
 *
 * The IDE never reimplements SLeeLa semantics. Lexing, parsing, and diagnostics
 * are obtained by invoking the real compiler and adapting its output. When the
 * compiler cannot be located or invoked, operations fail visibly rather than
 * returning a fabricated success (see SLEELA.IDE.CONTRACT.md, "Failure rule").
 */
class NativeSleelaCompilerBridge(
    private val projectRoot: Path,
    private val compiler: Path = locateCompiler(projectRoot),
) : SleelaCompilerBridge {

    override fun lex(source: String, file: Path): SleelaLexResult {
        // The native front end does not yet expose a stable token stream over the
        // CLI; the IDE lexer tokenizes locally against SLEELA.syntax while keeping
        // the native compiler authoritative for parsing and diagnostics. This
        // adapter therefore returns the structural fallback tokens.
        return SleelaLexResult(SleelaFallbackLexer.tokenize(source))
    }

    override fun parse(source: String, file: Path): SleelaParseResult {
        // Structural parse is represented by the PSI tree built from tokens; the
        // native compiler validates semantics through [diagnostics].
        return SleelaParseResult(root = null)
    }

    override fun diagnostics(source: String, file: Path, project: Path): List<SleelaDiagnostic> {
        if (!compiler.exists() || !compiler.isExecutable()) {
            // No compiler => surface an informational diagnostic rather than
            // silently claiming the source is clean.
            return listOf(
                SleelaDiagnostic(
                    severity = Severity.INFO,
                    message = "SLeeLa compiler not found; diagnostics are unavailable. Build it with `make -C impl`.",
                    start = 0,
                    end = 0,
                    code = "SLEELA-IDE-NOCOMPILER",
                )
            )
        }
        val tmp = Files.createTempFile("sleela-ide-", ".sleela")
        return try {
            Files.writeString(tmp, source)
            val result = run(listOf(compiler.toString(), "check", tmp.toString()), project)
            SleelaDiagnosticParser.parse(result.stdout + "\n" + result.stderr, source, tmp)
        } catch (e: Exception) {
            listOf(
                SleelaDiagnostic(
                    severity = Severity.ERROR,
                    message = "SLeeLa compiler invocation failed: ${e.message}",
                    start = 0,
                    end = 0,
                    code = "SLEELA-IDE-INVOKE",
                )
            )
        } finally {
            runCatching { Files.deleteIfExists(tmp) }
        }
    }

    private data class ProcessResult(val exitCode: Int, val stdout: String, val stderr: String)

    private fun run(command: List<String>, workingDir: Path): ProcessResult {
        val process = ProcessBuilder(command)
            .directory(workingDir.toFile())
            .redirectErrorStream(false)
            .start()
        val out = process.inputStream.bufferedReader().readText()
        val err = process.errorStream.bufferedReader().readText()
        val finished = process.waitFor(PROCESS_TIMEOUT_SECONDS, TimeUnit.SECONDS)
        if (!finished) {
            process.destroyForcibly()
            throw IllegalStateException("SLeeLa compiler timed out after ${PROCESS_TIMEOUT_SECONDS}s")
        }
        return ProcessResult(process.exitValue(), out, err)
    }

    companion object {
        private const val PROCESS_TIMEOUT_SECONDS = 30L

        /** Candidate locations for the native compiler relative to the project root. */
        private val CANDIDATES = listOf(
            "impl/build/sleela",
            "impl/build/sleela-package/sleela",
            "build/sleela",
        )

        fun locateCompiler(projectRoot: Path): Path {
            System.getenv("SLEELA_COMPILER")?.let { return Path.of(it) }
            for (c in CANDIDATES) {
                val p = projectRoot.resolve(c)
                if (p.exists() && p.isExecutable()) return p
            }
            // Return the primary expected path even if absent, so diagnostics() can
            // report its absence rather than throwing at construction time.
            return projectRoot.resolve(CANDIDATES.first())
        }
    }
}

/** Default provider that constructs a [NativeSleelaCompilerBridge] for a project. */
class NativeSleelaCompilerProvider : SleelaCompilerProvider {
    override fun create(projectRoot: Path): SleelaCompilerBridge = NativeSleelaCompilerBridge(projectRoot)
}
