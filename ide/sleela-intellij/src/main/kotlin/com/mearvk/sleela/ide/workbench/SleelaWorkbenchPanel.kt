package com.mearvk.sleela.ide.workbench

import com.intellij.openapi.application.ApplicationManager
import com.intellij.openapi.fileEditor.FileDocumentManager
import com.intellij.openapi.fileEditor.FileEditorManager
import com.intellij.openapi.fileEditor.OpenFileDescriptor
import com.intellij.openapi.project.Project
import com.intellij.openapi.vfs.LocalFileSystem
import com.intellij.openapi.vfs.VirtualFile
import com.intellij.openapi.vfs.VirtualFileManager
import com.intellij.ui.ColoredListCellRenderer
import com.intellij.ui.ColoredTreeCellRenderer
import com.intellij.ui.OnePixelSplitter
import com.intellij.ui.SimpleTextAttributes
import com.intellij.ui.components.JBList
import com.intellij.ui.components.JBScrollPane
import com.intellij.ui.components.JBLabel
import com.intellij.ui.treeStructure.Tree
import com.intellij.util.ui.JBUI
import com.mearvk.sleela.ide.SleelaIcons
import com.mearvk.sleela.ide.Severity
import java.awt.BorderLayout
import javax.swing.DefaultListModel
import javax.swing.JButton
import javax.swing.JComponent
import javax.swing.JList
import javax.swing.JPanel
import javax.swing.JTree
import javax.swing.tree.DefaultMutableTreeNode
import javax.swing.tree.DefaultTreeModel
import javax.swing.tree.TreePath

/**
 * The interactive SLeeLa Workbench UI.
 *
 * This Swing panel is the host rendering of the lib/ide-workbench contracts:
 *   - a project tree (SLIdeProjectTree) over the real project files;
 *   - an open-tabs list (SLIdeTabGroup) with dirty markers;
 *   - a diagnostics list (SLIdeDiagnosticsPanel);
 *   - command buttons (SLIdeCommandRegistry): Open, Compile, Save, Close.
 *
 * The command buttons drive SleelaWorkbenchService, which invokes the real
 * `sleela` toolchain (Nordshrift for .sst, then `check`). SLeeLa owns meaning;
 * IntelliJ owns presentation, filesystem access, and desktop lifecycle.
 */
class SleelaWorkbenchPanel(private val project: Project) : JPanel(BorderLayout()) {

    private val tabs = WorkbenchTabGroup()
    private val diagnostics = WorkbenchDiagnostics()

    private val tabsModel = DefaultListModel<WorkbenchEditor>()
    private val tabsList = JBList(tabsModel)

    private val diagnosticsModel = DefaultListModel<WorkbenchDiagnostics.Entry>()
    private val diagnosticsList = JBList(diagnosticsModel)

    private val projectTree: Tree
    private val statusLabel = JBLabel()

    private val compileButton = JButton("Compile", SleelaIcons.CHECK)
    private val saveButton = JButton("Save")
    private val openButton = JButton("Open", SleelaIcons.FILE)
    private val closeButton = JButton("Close")

    init {
        border = JBUI.Borders.empty(6)

        // --- command bar (SLIdeCommandRegistry affordances) ------------------
        val commandBar = JPanel().apply {
            layout = java.awt.FlowLayout(java.awt.FlowLayout.LEFT, JBUI.scale(4), 0)
            add(openButton)
            add(compileButton)
            add(saveButton)
            add(closeButton)
        }
        add(commandBar, BorderLayout.NORTH)

        // --- project tree (SLIdeProjectTree) ---------------------------------
        projectTree = Tree(buildProjectModel()).apply {
            isRootVisible = true
            cellRenderer = ProjectTreeRenderer()
        }

        // --- open tabs (SLIdeTabGroup) ---------------------------------------
        tabsList.cellRenderer = TabRenderer()
        tabsList.addListSelectionListener {
            if (!it.valueIsAdjusting) {
                val i = tabsList.selectedIndex
                if (i >= 0) { tabs.focus(i); syncCommandState() }
            }
        }

        // --- diagnostics (SLIdeDiagnosticsPanel) -----------------------------
        diagnosticsList.cellRenderer = DiagnosticRenderer()
        diagnosticsList.addListSelectionListener {
            if (!it.valueIsAdjusting) jumpToSelectedDiagnostic()
        }

        // Open on double-click in the tree; keep command state in sync with selection.
        projectTree.addMouseListener(object : java.awt.event.MouseAdapter() {
            override fun mouseClicked(e: java.awt.event.MouseEvent) {
                if (e.clickCount == 2) openSelectedFromTree()
            }
        })
        projectTree.addTreeSelectionListener { syncCommandState() }

        val leftRight = OnePixelSplitter(false, 0.42f).apply {
            firstComponent = titled("Project", JBScrollPane(projectTree))
            val tabsAndDiag = OnePixelSplitter(true, 0.5f).apply {
                firstComponent = titled("Open files", JBScrollPane(tabsList))
                secondComponent = titled("Problems", JBScrollPane(diagnosticsList))
            }
            secondComponent = tabsAndDiag
        }
        add(leftRight, BorderLayout.CENTER)
        add(statusLabel, BorderLayout.SOUTH)

        wireCommands()
        syncCommandState()
        refreshStatus()
    }

    val component: JComponent get() = this

    // ------------------------------------------------------------------------
    // Commands (SLIdeCommandRegistry -> SleelaWorkbenchService)
    // ------------------------------------------------------------------------

    private fun wireCommands() {
        openButton.addActionListener { openSelectedFromTree() }
        compileButton.addActionListener { compileActive() }
        saveButton.addActionListener { saveActive() }
        closeButton.addActionListener { closeActive() }
    }

    /** SLIdeWorkbench.openFile: open the tree-selected file into a tab. */
    private fun openSelectedFromTree() {
        val vf = selectedTreeFile() ?: return
        if (vf.isDirectory || !WorkbenchLanguage.isSource(vf.path)) return
        openVirtualFile(vf)
    }

    private fun openVirtualFile(vf: VirtualFile) {
        val id = vf.url
        val existing = tabs.indexOf(id)
        if (existing >= 0) { tabs.focus(existing); refreshTabs(); return }

        val editor = WorkbenchEditor(id, vf.path, WorkbenchLanguage.forPath(vf.path))
        val text = String(vf.contentsToByteArray(), Charsets.UTF_8)
        editor.loadText(text, revisionOf(vf))
        tabs.open(editor)

        // Also open it in the real IntelliJ editor so the worker can type.
        OpenFileDescriptor(project, vf).navigate(true)

        refreshTabs()
        rearmWatch()
    }

    /** SLIdeWorkbench.compileActive: SST Nordshrit (.sst) or direct (.sleela). */
    private fun compileActive() {
        val editor = tabs.active() ?: return
        diagnostics.clearDocument(editor.documentId)

        // Persist IntelliJ edits so the compiler sees current content.
        ApplicationManager.getApplication().invokeAndWait {
            FileDocumentManager.getInstance().saveAllDocuments()
        }
        val source = liveSourceFor(editor)

        statusLabel.text = "Compiling ${editor.displayName}…"
        ApplicationManager.getApplication().executeOnPooledThread {
            val outcome = SleelaWorkbenchService.compile(project, editor, source)
            ApplicationManager.getApplication().invokeLater {
                outcome.diagnostics.forEach {
                    diagnostics.publish(it.documentId, it.severity, it.line, it.column, it.message)
                }
                refreshDiagnostics()
                refreshStatus()
            }
        }
    }

    /** SLIdeWorkbench.saveActive: persist and mark the buffer clean. */
    private fun saveActive() {
        val editor = tabs.active() ?: return
        ApplicationManager.getApplication().invokeAndWait {
            FileDocumentManager.getInstance().saveAllDocuments()
        }
        findVirtualFile(editor)?.let { editor.markSaved(revisionOf(it)) }
        refreshTabs()
        syncCommandState()
    }

    private fun closeActive() {
        if (tabs.activeIndex < 0) return
        tabs.close(tabs.activeIndex)
        refreshTabs()
        rearmWatch()
    }

    // ------------------------------------------------------------------------
    // View sync
    // ------------------------------------------------------------------------

    private fun syncCommandState() {
        val active = tabs.active()
        compileButton.isEnabled = active != null
        saveButton.isEnabled = active != null && active.dirty
        closeButton.isEnabled = active != null
        openButton.isEnabled = selectedTreeFile()?.let { !it.isDirectory } ?: false
    }

    private fun refreshTabs() {
        tabsModel.clear()
        tabs.all().forEach { tabsModel.addElement(it) }
        if (tabs.activeIndex in 0 until tabsModel.size()) {
            tabsList.selectedIndex = tabs.activeIndex
        }
        syncCommandState()
        refreshStatus()
    }

    private fun refreshDiagnostics() {
        diagnosticsModel.clear()
        diagnostics.all().forEach { diagnosticsModel.addElement(it) }
    }

    private fun refreshStatus() {
        val watch = "watch set ${tabs.watchSetSize()}/${WorkbenchTabGroup.SLGUI_MAX_DOCUMENTS}"
        statusLabel.text = "${diagnostics.statusLine()} · ${tabs.size} open · $watch"
    }

    /** SLIdeTabGroup -> lib/gui 1..14 OS watch registration. */
    private fun rearmWatch() {
        // The real OS WatchService is owned by the gui/ host; here we keep the
        // registration honest against the 1..14 bound and refresh the status.
        check(tabs.watchSetWithinGuiBound() || tabs.size == 0) {
            "watch set must stay within the lib/gui 1..14 bound"
        }
        refreshStatus()
    }

    private fun jumpToSelectedDiagnostic() {
        val entry = diagnosticsList.selectedValue ?: return
        val editor = tabs.all().firstOrNull { it.documentId == entry.documentId } ?: return
        val vf = findVirtualFile(editor) ?: return
        val descriptor = OpenFileDescriptor(
            project, vf,
            (entry.line - 1).coerceAtLeast(0),
            (entry.column - 1).coerceAtLeast(0),
        )
        FileEditorManager.getInstance(project).openTextEditor(descriptor, true)
    }

    // ------------------------------------------------------------------------
    // Project tree model over the real project files
    // ------------------------------------------------------------------------

    private fun buildProjectModel(): DefaultTreeModel {
        val base = project.basePath
        val rootVf = base?.let { LocalFileSystem.getInstance().findFileByPath(it) }
        val rootNode = DefaultMutableTreeNode(rootVf ?: base ?: "SLeeLa project")
        if (rootVf != null) addChildren(rootNode, rootVf, depth = 0)
        return DefaultTreeModel(rootNode)
    }

    /** Lazily include a shallow slice of source files (deep trees stay cheap). */
    private fun addChildren(node: DefaultMutableTreeNode, dir: VirtualFile, depth: Int) {
        if (depth > 6) return
        val children = dir.children ?: return
        children.sortedWith(compareBy({ !it.isDirectory }, { it.name })).forEach { child ->
            if (child.name.startsWith(".")) return@forEach
            if (child.isDirectory) {
                val dirNode = DefaultMutableTreeNode(child)
                addChildren(dirNode, child, depth + 1)
                if (dirNode.childCount > 0) node.add(dirNode)
            } else if (WorkbenchLanguage.isSource(child.path)) {
                node.add(DefaultMutableTreeNode(child))
            }
        }
    }

    private fun selectedTreeFile(): VirtualFile? {
        val path: TreePath = projectTree.selectionPath ?: return null
        val node = path.lastPathComponent as? DefaultMutableTreeNode ?: return null
        return node.userObject as? VirtualFile
    }

    // ------------------------------------------------------------------------
    // Helpers
    // ------------------------------------------------------------------------

    private fun findVirtualFile(editor: WorkbenchEditor): VirtualFile? =
        VirtualFileManager.getInstance().findFileByUrl(editor.documentId)

    private fun liveSourceFor(editor: WorkbenchEditor): String {
        val vf = findVirtualFile(editor) ?: return editor.text
        val doc = FileDocumentManager.getInstance().getDocument(vf)
        return doc?.text ?: String(vf.contentsToByteArray(), Charsets.UTF_8)
    }

    private fun revisionOf(vf: VirtualFile): String = "${vf.timeStamp}:${vf.length}"

    private fun titled(title: String, inner: JComponent): JComponent =
        JPanel(BorderLayout()).apply {
            add(JBLabel(" $title").apply { border = JBUI.Borders.empty(2) }, BorderLayout.NORTH)
            add(inner, BorderLayout.CENTER)
        }

    // ------------------------------------------------------------------------
    // Renderers
    // ------------------------------------------------------------------------

    private class TabRenderer : ColoredListCellRenderer<WorkbenchEditor>() {
        override fun customizeCellRenderer(
            list: JList<out WorkbenchEditor>, value: WorkbenchEditor?,
            index: Int, selected: Boolean, hasFocus: Boolean,
        ) {
            value ?: return
            icon = SleelaIcons.FILE
            val marker = if (value.dirty) " *" else ""
            append(value.displayName + marker,
                if (value.dirty) SimpleTextAttributes.REGULAR_BOLD_ATTRIBUTES
                else SimpleTextAttributes.REGULAR_ATTRIBUTES)
            append("  [${value.language}]", SimpleTextAttributes.GRAYED_ATTRIBUTES)
        }
    }

    private class DiagnosticRenderer : ColoredListCellRenderer<WorkbenchDiagnostics.Entry>() {
        override fun customizeCellRenderer(
            list: JList<out WorkbenchDiagnostics.Entry>, value: WorkbenchDiagnostics.Entry?,
            index: Int, selected: Boolean, hasFocus: Boolean,
        ) {
            value ?: return
            val sev = when (value.severity) {
                Severity.ERROR -> "error"
                Severity.WARNING -> "warning"
                Severity.INFO -> "info"
            }
            append("$sev ", SimpleTextAttributes.GRAYED_ATTRIBUTES)
            append("(${value.line}:${value.column}) ", SimpleTextAttributes.GRAYED_ATTRIBUTES)
            append(value.message, SimpleTextAttributes.REGULAR_ATTRIBUTES)
        }
    }

    private class ProjectTreeRenderer : ColoredTreeCellRenderer() {
        override fun customizeCellRenderer(
            tree: JTree, value: Any?, selected: Boolean, expanded: Boolean,
            leaf: Boolean, row: Int, hasFocus: Boolean,
        ) {
            val node = value as? DefaultMutableTreeNode ?: return
            when (val obj = node.userObject) {
                is VirtualFile -> {
                    if (!obj.isDirectory) icon = SleelaIcons.FILE
                    append(obj.name)
                }
                else -> append(obj?.toString() ?: "")
            }
        }
    }
}
