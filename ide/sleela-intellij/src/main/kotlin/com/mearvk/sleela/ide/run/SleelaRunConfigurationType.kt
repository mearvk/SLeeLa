package com.mearvk.sleela.ide.run

import com.intellij.execution.configurations.ConfigurationFactory
import com.intellij.execution.configurations.ConfigurationType
import com.intellij.execution.configurations.RunConfiguration
import com.intellij.openapi.project.Project
import com.mearvk.sleela.ide.SleelaIcons
import javax.swing.Icon

/** The "SLeeLa" run configuration type shown in Run/Debug Configurations. */
class SleelaRunConfigurationType : ConfigurationType {
    override fun getDisplayName() = "SLeeLa"
    override fun getConfigurationTypeDescription() = "Run a SLeeLa program with the native runtime"
    override fun getIcon(): Icon = SleelaIcons.RUN
    override fun getId() = "SLEELA_RUN_CONFIGURATION"
    override fun getConfigurationFactories() = arrayOf(SleelaConfigurationFactory(this))

    companion object {
        @JvmStatic
        val instance: SleelaRunConfigurationType
            get() = com.intellij.execution.configurations.ConfigurationTypeUtil
                .findConfigurationType(SleelaRunConfigurationType::class.java)
    }
}

class SleelaConfigurationFactory(type: ConfigurationType) : ConfigurationFactory(type) {
    override fun getId() = "SLeeLa"
    override fun createTemplateConfiguration(project: Project): RunConfiguration =
        SleelaRunConfiguration(project, this, "SLeeLa")
}
