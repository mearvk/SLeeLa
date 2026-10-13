package com.mearvk.sleela.ide.workbench

import com.mearvk.sleela.ide.Severity

/**
 * Host-side implementation of the `lib/ide-workbench` contracts.
 *
 * These data types mirror, one-for-one, the SLeeLa library classes in
 * `lib/ide-workbench` (SLIdeEditor, SLIdeTabGroup, SLIdeProjectTree,
 * SLIdeCommandRegistry, SLIdeDiagnosticsPanel). The SLeeLa classes own the
 * *contract and meaning*; this Kotlin layer is the IntelliJ-Platform *host* that
 * renders them and performs the real OS/compiler work — exactly the division of
 * authority described in lib/gui/ARCHITECTURE.md (SLeeLa owns meaning, the host
 * owns presentation and lifecycle).
 */

/** Mirrors SLIdeEditor: one open document buffer. */
class WorkbenchEditor(
    val documentId: String,
    val path: String,
    val language: String,
) {
    var text: String = ""
        private set
    private var savedRevision: String = "absent"
    var dirty: Boolean = false
        private set

    /** A load/reset: not itself a user edit. */
    fun loadText(value: String, revision: String) {
        text = value; savedRevision = revision; dirty = false
    }

    /** A user edit. */
    fun setText(value: String) {
        if (value != text) { text = value; dirty = true }
    }

    fun markSaved(revision: String) { savedRevision = revision; dirty = false }

    fun staleAgainst(diskRevision: String?): Boolean =
        diskRevision != null && diskRevision != savedRevision

    val isSst: Boolean get() = language == "sst"
    val isSleela: Boolean get() = language == "sleela"
    val displayName: String get() = path.substringAfterLast('/').ifEmpty { documentId }
}

/**
 * Mirrors SLIdeTabGroup: an unbounded set of open editors, exposing a bounded
 * (<=14) watch set so the lib/gui SLGuiDocumentListener 1..14 contract holds.
 */
class WorkbenchTabGroup {
    companion object { const val SLGUI_MAX_DOCUMENTS = 14 }

    private val editors = mutableListOf<WorkbenchEditor>()
    var activeIndex: Int = -1
        private set

    val size: Int get() = editors.size
    fun editorAt(index: Int): WorkbenchEditor? = editors.getOrNull(index)
    fun all(): List<WorkbenchEditor> = editors.toList()

    fun indexOf(documentId: String): Int = editors.indexOfFirst { it.documentId == documentId }

    /** Open (or focus if already open); returns the active index. */
    fun open(editor: WorkbenchEditor): Int {
        val existing = indexOf(editor.documentId)
        if (existing >= 0) { activeIndex = existing; return existing }
        editors.add(editor)
        activeIndex = editors.lastIndex
        return activeIndex
    }

    fun focus(index: Int): Boolean {
        if (index !in editors.indices) return false
        activeIndex = index
        return true
    }

    fun close(index: Int): Boolean {
        if (index !in editors.indices) return false
        editors.removeAt(index)
        activeIndex = when {
            editors.isEmpty() -> -1
            activeIndex >= editors.size -> editors.lastIndex
            else -> activeIndex.coerceAtMost(editors.lastIndex)
        }
        return true
    }

    fun active(): WorkbenchEditor? = editors.getOrNull(activeIndex)

    fun dirtyCount(): Int = editors.count { it.dirty }
    fun anyDirty(): Boolean = dirtyCount() > 0

    /** The OS-watch registration size, honoring the hard 1..14 lib/gui bound. */
    fun watchSetSize(): Int = when {
        editors.isEmpty() -> 0
        editors.size > SLGUI_MAX_DOCUMENTS -> SLGUI_MAX_DOCUMENTS
        else -> editors.size
    }

    fun watchSetWithinGuiBound(): Boolean =
        watchSetSize() in 1..SLGUI_MAX_DOCUMENTS
}

/** Mirrors SLIdeDiagnosticsPanel: compiler problems with the IDE failure rule. */
class WorkbenchDiagnostics {
    data class Entry(
        val documentId: String,
        val severity: Severity,
        val line: Int,
        val column: Int,
        val message: String,
    )

    private val entries = mutableListOf<Entry>()

    fun clear() = entries.clear()
    fun clearDocument(documentId: String) { entries.removeAll { it.documentId == documentId } }

    fun publish(documentId: String, severity: Severity, line: Int, column: Int, message: String) {
        entries.add(Entry(documentId, severity, line, column, message))
    }

    fun all(): List<Entry> = entries.toList()
    val size: Int get() = entries.size
    fun errors(): Int = entries.count { it.severity == Severity.ERROR }
    fun warnings(): Int = entries.count { it.severity == Severity.WARNING }

    /** The IDE contract: any error means the compile run failed. */
    fun compileSucceeded(): Boolean = errors() == 0

    fun statusLine(): String =
        if (entries.isEmpty()) "No problems"
        else "${errors()} error(s), ${warnings()} warning(s)"
}

/** Mirrors SLIdeProjectTree language routing (.sst -> Nordshrift, .sleela -> direct). */
object WorkbenchLanguage {
    fun forPath(path: String): String = when {
        path.endsWith(".sst") -> "sst"
        path.endsWith(".sleela") -> "sleela"
        path.endsWith(".sldocument") -> "sldocument"
        path.endsWith(".cpp") || path.endsWith(".cc") || path.endsWith(".hpp") -> "cpp"
        path.endsWith(".c") || path.endsWith(".h") -> "c"
        path.endsWith(".java") -> "java"
        else -> "text"
    }

    /** Files the workbench can open in an editor / compile. */
    fun isSource(path: String): Boolean =
        forPath(path) in setOf("sst", "sleela", "sldocument", "c", "cpp", "java")
}
