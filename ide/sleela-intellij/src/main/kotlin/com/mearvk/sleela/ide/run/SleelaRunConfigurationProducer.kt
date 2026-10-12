package com.mearvk.sleela.ide.run

import com.intellij.execution.actions.ConfigurationContext
import com.intellij.execution.actions.LazyRunConfigurationProducer
import com.intellij.execution.configurations.ConfigurationFactory
import com.intellij.openapi.util.Ref
import com.intellij.psi.PsiElement
import com.intellij.psi.PsiFile
import com.mearvk.sleela.ide.SleelaFileType

/**
 * Produces a SLeeLa run configuration from a `.sleela` file in context, so the
 * gutter "Run" icon and right-click "Run" work with zero setup — the smooth
 * path for novices, and a one-click scaffold for professionals.
 */
class SleelaRunConfigurationProducer : LazyRunConfigurationProducer<SleelaRunConfiguration>() {

    override fun getConfigurationFactory(): ConfigurationFactory =
        SleelaRunConfigurationType.instance.configurationFactories.first()

    override fun setupConfigurationFromContext(
        configuration: SleelaRunConfiguration,
        context: ConfigurationContext,
        sourceElement: Ref<PsiElement>,
    ): Boolean {
        val file = context.psiLocation?.containingFile ?: return false
        if (!isSleela(file)) return false
        val virtualFile = file.virtualFile ?: return false
        configuration.sleelaFilePath = virtualFile.path
        configuration.name = virtualFile.nameWithoutExtension
        return true
    }

    override fun isConfigurationFromContext(
        configuration: SleelaRunConfiguration,
        context: ConfigurationContext,
    ): Boolean {
        val file = context.psiLocation?.containingFile ?: return false
        if (!isSleela(file)) return false
        return configuration.sleelaFilePath == file.virtualFile?.path
    }

    private fun isSleela(file: PsiFile): Boolean =
        file.fileType is SleelaFileType || file.virtualFile?.extension == "sleela"
}
