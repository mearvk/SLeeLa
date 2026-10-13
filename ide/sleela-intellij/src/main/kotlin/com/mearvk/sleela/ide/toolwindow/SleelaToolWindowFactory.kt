package com.mearvk.sleela.ide.toolwindow

import com.intellij.openapi.actionSystem.ActionManager
import com.intellij.openapi.actionSystem.ActionToolbar
import com.intellij.openapi.actionSystem.DefaultActionGroup
import com.intellij.openapi.project.Project
import com.intellij.openapi.wm.ToolWindow
import com.intellij.openapi.wm.ToolWindowFactory
import com.intellij.ui.components.JBLabel
import com.intellij.ui.components.panels.VerticalLayout
import com.intellij.ui.content.ContentFactory
import com.intellij.util.ui.JBUI
import com.mearvk.sleela.ide.SleelaCommandRunner
import com.mearvk.sleela.ide.workbench.SleelaWorkbenchPanel
import javax.swing.JPanel

/**
 * The "SLeeLa" tool window. It exposes two tabs:
 *
 *  - **Workbench** — the interactive lib/ide-workbench UI: a project tree, the
 *    open-file (tab) list, a problems list, and Open / Compile / Save / Close
 *    commands wired to the real `sleela` toolchain (Nordshrift for .sst).
 *  - **Quick actions** — the original branded quick-access panel (Run / Check /
 *    Build / Transpile) for discoverability alongside the menus.
 */
class SleelaToolWindowFactory : ToolWindowFactory {

    override fun createToolWindowContent(project: Project, toolWindow: ToolWindow) {
        val factory = ContentFactory.getInstance()

        // Tab 1: the interactive workbench.
        val workbench = SleelaWorkbenchPanel(project)
        toolWindow.contentManager.addContent(
            factory.createContent(workbench.component, "Workbench", false),
        )

        // Tab 2: the original quick-access buttons + compiler status.
        toolWindow.contentManager.addContent(
            factory.createContent(quickActionsPanel(project), "Quick actions", false),
        )
    }

    private fun quickActionsPanel(project: Project): JPanel {
        val panel = JPanel(VerticalLayout(JBUI.scale(8))).apply {
            border = JBUI.Borders.empty(10)
        }

        panel.add(JBLabel("<html><b>SLeeLa</b> — The Now. The Ingredient.</html>"))

        val status = if (SleelaCommandRunner.isCompilerAvailable(project)) {
            "Compiler: ready"
        } else {
            "Compiler: not found (run `make -C impl`)"
        }
        panel.add(JBLabel(status))

        // Reuse the registered action group so the buttons stay in sync with menus.
        val group = ActionManager.getInstance().getAction("Sleela.ToolWindow.Actions") as? DefaultActionGroup
        if (group != null) {
            val toolbar: ActionToolbar =
                ActionManager.getInstance().createActionToolbar("SleelaToolWindow", group, false)
            toolbar.targetComponent = panel
            panel.add(toolbar.component)
        }
        return panel
    }
}
