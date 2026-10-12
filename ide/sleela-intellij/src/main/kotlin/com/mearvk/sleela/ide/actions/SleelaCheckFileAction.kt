package com.mearvk.sleela.ide.actions

import com.mearvk.sleela.ide.SleelaIcons

/** Validate the current `.sleela` file: `sleela check <file>`. */
class SleelaCheckFileAction : SleelaFileAction(
    text = "Check SLeeLa File",
    description = "Validate the current SLeeLa file with the compiler",
    icon = SleelaIcons.CHECK,
) {
    override fun arguments(filePath: String) = listOf("check", filePath)
    override fun actionTitle() = "Check"
}
