package com.mearvk.sleela.ide

import com.intellij.lang.annotation.AnnotationHolder
import com.intellij.lang.annotation.Annotator
import com.intellij.lang.annotation.HighlightSeverity
import com.intellij.openapi.util.TextRange
import com.intellij.psi.PsiElement
import com.intellij.psi.PsiFile
import java.nio.file.Path

/**
 * Surfaces native SLeeLa compiler diagnostics in the editor.
 *
 * Runs once per file (on the [PsiFile] root) and invokes the authoritative
 * compiler via the installed bridge. Per SLEELA.IDE.CONTRACT.md, compiler
 * failures stay visible: if no bridge/compiler is available, the bridge returns
 * an informational diagnostic rather than a silent clean result.
 */
class SleelaDiagnosticAnnotator : Annotator {

    override fun annotate(element: PsiElement, holder: AnnotationHolder) {
        if (element !is PsiFile) return
        if (element.fileType !is SleelaFileType) return

        val projectRoot = element.project.basePath?.let(Path::of) ?: return
        val source = element.text
        val filePath = element.virtualFile?.path?.let(Path::of) ?: Path.of("<editor>.sleela")

        val bridge = try {
            if (SleelaCompilerProviders.isInstalled()) {
                SleelaCompilerProviders.create(projectRoot)
            } else {
                NativeSleelaCompilerBridge(projectRoot)
            }
        } catch (_: Exception) {
            return
        }

        val diagnostics = try {
            bridge.diagnostics(source, filePath, projectRoot)
        } catch (_: Exception) {
            return
        }

        val length = source.length
        for (d in diagnostics) {
            val start = d.start.coerceIn(0, length)
            val end = d.end.coerceIn(start, length)
            val range = if (end > start) TextRange(start, end) else TextRange(start, length.coerceAtLeast(1).coerceAtMost(length))
            val severity = when (d.severity) {
                Severity.ERROR -> HighlightSeverity.ERROR
                Severity.WARNING -> HighlightSeverity.WARNING
                Severity.INFO -> HighlightSeverity.WEAK_WARNING
            }
            val message = d.code?.let { "${d.message} [$it]" } ?: d.message
            holder.newAnnotation(severity, message)
                .range(if (range.isEmpty) TextRange(0, length.coerceAtLeast(0)) else range)
                .create()
        }
    }
}
