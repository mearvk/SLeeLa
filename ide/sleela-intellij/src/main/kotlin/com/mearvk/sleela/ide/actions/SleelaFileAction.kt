package com.mearvk.sleela.ide.actions

import com.intellij.execution.filters.TextConsoleBuilderFactory
import com.intellij.execution.ui.ConsoleViewContentType
import com.intellij.openapi.actionSystem.ActionUpdateThread
import com.intellij.openapi.actionSystem.AnAction
import com.intellij.openapi.actionSystem.AnActionEvent
import com.intellij.openapi.actionSystem.CommonDataKeys
import com.intellij.openapi.application.ApplicationManager
import com.intellij.openapi.project.Project
import com.intellij.openapi.vfs.VirtualFile
import com.intellij.openapi.wm.ToolWindowId
import com.intellij.openapi.wm.ToolWindowManager
import com.intellij.ui.content.ContentFactory
import com.mearvk.sleela.ide.SleelaCommandRunner
import com.mearvk.sleela.ide.SleelaCompilerMissingException
import com.mearvk.sleela.ide.SleelaFileType
import javax.swing.Icon

/**
 * Base class for actions that operate on the current `.sleela` file by invoking
 * a native `sleela` subcommand and streaming output to a console.
 */
abstract class SleelaFileAction(
    text: String,
    description: String,
    icon: Icon,
) : AnAction(text, description, icon) {

    /** The `sleela` subcommand arguments, given the target file path. */
    protected abstract fun arguments(filePath: String): List<String>

    /** Short label shown in the console tab / run header. */
    protected abstract fun actionTitle(): String

    override fun getActionUpdateThread() = ActionUpdateThread.BGT

    override fun update(e: AnActionEvent) {
        val file = targetFile(e)
        e.presentation.isEnabledAndVisible = file != null && isSleela(file)
    }

    override fun actionPerformed(e: AnActionEvent) {
        val project = e.project ?: return
        val file = targetFile(e) ?: return
        if (!isSleela(file)) return

        // Persist edits so the compiler sees current content.
        com.intellij.openapi.fileEditor.FileDocumentManager.getInstance().saveAllDocuments()

        val commandLine = try {
            SleelaCommandRunner.commandLine(project, *arguments(file.path).toTypedArray())
        } catch (_: SleelaCompilerMissingException) {
            SleelaCommandRunner.notifyCompilerMissing(project)
            return
        }

        runInConsole(project, "${actionTitle()}: ${file.name}", commandLine)
    }

    private fun runInConsole(project: Project, title: String, commandLine: com.intellij.execution.configurations.GeneralCommandLine) {
        val console = TextConsoleBuilderFactory.getInstance().createBuilder(project).console
        console.print("$ ${commandLine.commandLineString}\n", ConsoleViewContentType.SYSTEM_OUTPUT)

        val handler = SleelaCommandRunner.processHandler(commandLine)
        console.attachToProcess(handler)
        handler.startNotify()

        val toolWindow = ToolWindowManager.getInstance(project).getToolWindow(ToolWindowId.RUN)
        if (toolWindow != null) {
            ApplicationManager.getApplication().invokeLater {
                val content = ContentFactory.getInstance().createContent(console.component, title, false)
                content.setDisposer(console)
                toolWindow.contentManager.addContent(content)
                toolWindow.contentManager.setSelectedContent(content)
                toolWindow.activate(null)
            }
        }
    }

    private fun targetFile(e: AnActionEvent): VirtualFile? =
        e.getData(CommonDataKeys.VIRTUAL_FILE)

    private fun isSleela(file: VirtualFile): Boolean =
        !file.isDirectory && (file.fileType is SleelaFileType || file.extension == "sleela")
}
