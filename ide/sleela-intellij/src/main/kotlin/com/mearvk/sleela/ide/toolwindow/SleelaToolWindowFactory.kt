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
import javax.swing.JPanel

/**
 * The "SLeeLa" tool window: a branded quick-access panel with Run / Check /
 * Build / Transpile buttons and the current compiler status. Gives novices a
 * discoverable button surface alongside the menus.
 */
class SleelaToolWindowFactory : ToolWindowFactory {

    override fun createToolWindowContent(project: Project, toolWindow: ToolWindow) {
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

        val content = ContentFactory.getInstance().createContent(panel, "", false)
        toolWindow.contentManager.addContent(content)
    }
}
