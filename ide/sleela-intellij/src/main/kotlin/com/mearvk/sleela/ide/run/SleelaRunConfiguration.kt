package com.mearvk.sleela.ide.run

import com.intellij.execution.ExecutionException
import com.intellij.execution.Executor
import com.intellij.execution.configurations.ConfigurationFactory
import com.intellij.execution.configurations.GeneralCommandLine
import com.intellij.execution.configurations.RunConfigurationBase
import com.intellij.execution.configurations.RuntimeConfigurationError
import com.intellij.execution.filters.TextConsoleBuilderFactory
import com.intellij.execution.process.ProcessHandler
import com.intellij.execution.runners.ExecutionEnvironment
import com.intellij.execution.ui.ConsoleView
import com.intellij.execution.configurations.CommandLineState
import com.intellij.openapi.options.SettingsEditor
import com.intellij.openapi.project.Project
import com.intellij.openapi.util.JDOMExternalizerUtil
import com.intellij.util.execution.ParametersListUtil
import com.mearvk.sleela.ide.SleelaCommandRunner
import com.mearvk.sleela.ide.SleelaCompilerMissingException
import org.jdom.Element

/** A runnable SLeeLa program: `sleela run <file> [args]`. */
class SleelaRunConfiguration(
    project: Project,
    factory: ConfigurationFactory,
    name: String,
) : RunConfigurationBase<Any>(project, factory, name) {

    var sleelaFilePath: String = ""
    var programArguments: String = ""

    override fun getConfigurationEditor(): SettingsEditor<out RunConfigurationBase<Any>> =
        SleelaRunConfigurationEditor()

    override fun checkConfiguration() {
        if (sleelaFilePath.isBlank()) {
            throw RuntimeConfigurationError("A SLeeLa source file must be selected.")
        }
        if (!sleelaFilePath.endsWith(".sleela")) {
            throw RuntimeConfigurationError("The selected file is not a .sleela source file.")
        }
    }

    override fun getState(executor: Executor, environment: ExecutionEnvironment): CommandLineState =
        object : CommandLineState(environment) {
            override fun startProcess(): ProcessHandler {
                val args = buildList {
                    add("run")
                    add(sleelaFilePath)
                    addAll(ParametersListUtil.parse(programArguments))
                }
                val commandLine: GeneralCommandLine = try {
                    SleelaCommandRunner.commandLine(project, *args.toTypedArray())
                } catch (_: SleelaCompilerMissingException) {
                    throw ExecutionException(
                        "The native SLeeLa compiler could not be located. Build it with `make -C impl`.",
                    )
                }
                return SleelaCommandRunner.processHandler(commandLine)
            }

            override fun createConsole(executor: Executor): ConsoleView =
                TextConsoleBuilderFactory.getInstance().createBuilder(project).console
        }

    override fun writeExternal(element: Element) {
        super.writeExternal(element)
        JDOMExternalizerUtil.writeField(element, "sleelaFilePath", sleelaFilePath)
        JDOMExternalizerUtil.writeField(element, "programArguments", programArguments)
    }

    override fun readExternal(element: Element) {
        super.readExternal(element)
        sleelaFilePath = JDOMExternalizerUtil.readField(element, "sleelaFilePath") ?: ""
        programArguments = JDOMExternalizerUtil.readField(element, "programArguments") ?: ""
    }
}
