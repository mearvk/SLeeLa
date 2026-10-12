package com.mearvk.sleela.ide.actions

import com.intellij.ide.actions.CreateFileFromTemplateAction
import com.intellij.ide.actions.CreateFileFromTemplateDialog
import com.intellij.openapi.project.Project
import com.intellij.psi.PsiDirectory
import com.mearvk.sleela.ide.SleelaIcons

/**
 * "New → SLeeLa File" action. Offers class / main-program / empty templates so
 * novices get a correct starting point and professionals get a quick scaffold.
 */
class SleelaNewFileAction : CreateFileFromTemplateAction(
    "SLeeLa File",
    "Create a new SLeeLa source file",
    SleelaIcons.FILE,
) {
    override fun buildDialog(project: Project, directory: PsiDirectory, builder: CreateFileFromTemplateDialog.Builder) {
        builder
            .setTitle("New SLeeLa File")
            .addKind("Class", SleelaIcons.FILE, TEMPLATE_CLASS)
            .addKind("Program (with main)", SleelaIcons.RUN, TEMPLATE_PROGRAM)
            .addKind("Empty file", SleelaIcons.FILE, TEMPLATE_EMPTY)
    }

    override fun getActionName(directory: PsiDirectory?, newName: String, templateName: String?) =
        "Create SLeeLa File $newName"

    companion object {
        const val TEMPLATE_CLASS = "SLeeLa Class"
        const val TEMPLATE_PROGRAM = "SLeeLa Program"
        const val TEMPLATE_EMPTY = "SLeeLa File"
    }
}
