package com.mearvk.sleela.ide.run

import com.intellij.openapi.fileChooser.FileChooserDescriptorFactory
import com.intellij.openapi.options.SettingsEditor
import com.intellij.openapi.ui.TextFieldWithBrowseButton
import com.intellij.ui.components.JBTextField
import com.intellij.util.ui.FormBuilder
import javax.swing.JComponent
import javax.swing.JPanel

/** Settings form for a SLeeLa run configuration. */
class SleelaRunConfigurationEditor : SettingsEditor<SleelaRunConfiguration>() {

    private val fileField = TextFieldWithBrowseButton().apply {
        addBrowseFolderListener(
            null,
            FileChooserDescriptorFactory.createSingleFileDescriptor("sleela")
                .withTitle("Select SLeeLa File")
                .withDescription("Choose the .sleela program to run"),
        )
    }
    private val argsField = JBTextField()

    private val panel: JPanel = FormBuilder.createFormBuilder()
        .addLabeledComponent("SLeeLa file:", fileField)
        .addLabeledComponent("Program arguments:", argsField)
        .panel

    override fun resetEditorFrom(config: SleelaRunConfiguration) {
        fileField.text = config.sleelaFilePath
        argsField.text = config.programArguments
    }

    override fun applyEditorTo(config: SleelaRunConfiguration) {
        config.sleelaFilePath = fileField.text.trim()
        config.programArguments = argsField.text.trim()
    }

    override fun createEditor(): JComponent = panel
}
