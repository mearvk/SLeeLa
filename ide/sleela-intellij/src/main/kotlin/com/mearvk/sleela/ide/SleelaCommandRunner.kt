package com.mearvk.sleela.ide

import com.intellij.execution.configurations.GeneralCommandLine
import com.intellij.execution.process.OSProcessHandler
import com.intellij.execution.process.ProcessTerminatedListener
import com.intellij.notification.NotificationGroupManager
import com.intellij.notification.NotificationType
import com.intellij.openapi.project.Project
import java.nio.file.Files
import java.nio.file.Path
import kotlin.io.path.exists
import kotlin.io.path.isExecutable

/**
 * Locates the native SLeeLa compiler and builds [GeneralCommandLine]s for its
 * subcommands. The IDE always invokes the real toolchain (never a reimplemented
 * one), honouring SLEELA.IDE.CONTRACT.md.
 */
object SleelaCommandRunner {

    const val NOTIFICATION_GROUP = "SLeeLa"

    private val CANDIDATES = listOf(
        "impl/build/sleela",
        "impl/build/sleela-package/sleela",
        "build/sleela",
    )

    /** Resolve the compiler for [project], or null if it cannot be found. */
    fun locateCompiler(project: Project): Path? {
        System.getenv("SLEELA_COMPILER")?.let {
            val p = Path.of(it)
            if (p.exists() && p.isExecutable()) return p
        }
        val root = project.basePath?.let(Path::of) ?: return null
        for (c in CANDIDATES) {
            val p = root.resolve(c)
            if (p.exists() && p.isExecutable()) return p
        }
        return null
    }

    /** True when the compiler is available for [project]. */
    fun isCompilerAvailable(project: Project): Boolean = locateCompiler(project) != null

    /**
     * Build a command line that runs `sleela <args...>` from the project root.
     * @throws SleelaCompilerMissingException when the compiler cannot be located.
     */
    fun commandLine(project: Project, vararg args: String): GeneralCommandLine {
        val compiler = locateCompiler(project)
            ?: throw SleelaCompilerMissingException()
        val root = project.basePath ?: compiler.parent.toString()
        return GeneralCommandLine(compiler.toString(), *args)
            .withWorkDirectory(root)
            .withCharset(Charsets.UTF_8)
    }

    /** Create a process handler with console-friendly termination messages. */
    fun processHandler(commandLine: GeneralCommandLine): OSProcessHandler {
        val handler = OSProcessHandler(commandLine)
        ProcessTerminatedListener.attach(handler)
        return handler
    }

    /** Tell the user the compiler is missing and how to build it. */
    fun notifyCompilerMissing(project: Project) {
        NotificationGroupManager.getInstance()
            .getNotificationGroup(NOTIFICATION_GROUP)
            .createNotification(
                "SLeeLa compiler not found",
                "Build the native compiler with <code>make -C impl</code>, or set the " +
                    "<code>SLEELA_COMPILER</code> environment variable to its path.",
                NotificationType.WARNING,
            )
            .notify(project)
    }

    fun notifyInfo(project: Project, title: String, content: String) {
        NotificationGroupManager.getInstance()
            .getNotificationGroup(NOTIFICATION_GROUP)
            .createNotification(title, content, NotificationType.INFORMATION)
            .notify(project)
    }

    /** Writes [source] to a temp `.sleela` file (used by annotator/quick checks). */
    fun writeTempSource(source: String): Path {
        val tmp = Files.createTempFile("sleela-ide-", ".sleela")
        Files.writeString(tmp, source)
        return tmp
    }
}

/** Thrown when an operation needs the native compiler but it is not installed. */
class SleelaCompilerMissingException :
    RuntimeException("The native SLeeLa compiler could not be located.")
