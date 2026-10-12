package com.mearvk.sleela.ide.actions

import com.intellij.notification.NotificationType
import com.intellij.openapi.actionSystem.ActionUpdateThread
import com.intellij.openapi.actionSystem.AnAction
import com.intellij.openapi.actionSystem.AnActionEvent
import com.intellij.notification.NotificationGroupManager
import com.mearvk.sleela.ide.SleelaCommandRunner
import com.mearvk.sleela.ide.SleelaCompilerMissingException

/** Show the native SLeeLa compiler version: `sleela version`. */
class SleelaVersionAction : AnAction("SLeeLa Compiler Version", "Show the native SLeeLa compiler version", null) {

    override fun getActionUpdateThread() = ActionUpdateThread.BGT

    override fun actionPerformed(e: AnActionEvent) {
        val project = e.project ?: return
        val commandLine = try {
            SleelaCommandRunner.commandLine(project, "version")
        } catch (_: SleelaCompilerMissingException) {
            SleelaCommandRunner.notifyCompilerMissing(project)
            return
        }
        val output = try {
            val process = commandLine.createProcess()
            val text = process.inputStream.bufferedReader().readText().trim()
            process.waitFor()
            text.lineSequence().firstOrNull { it.contains("Sleelvac", ignoreCase = true) } ?: text
        } catch (ex: Exception) {
            "Failed to query compiler: ${ex.message}"
        }
        NotificationGroupManager.getInstance()
            .getNotificationGroup(SleelaCommandRunner.NOTIFICATION_GROUP)
            .createNotification("SLeeLa compiler", output, NotificationType.INFORMATION)
            .notify(project)
    }
}
