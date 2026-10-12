package com.mearvk.sleela.ide.actions

import com.mearvk.sleela.ide.SleelaIcons

/** Run the current `.sleela` file: `sleela run <file>`. */
class SleelaRunFileAction : SleelaFileAction(
    text = "Run SLeeLa File",
    description = "Run the current SLeeLa program with the native runtime",
    icon = SleelaIcons.RUN,
) {
    override fun arguments(filePath: String) = listOf("run", filePath)
    override fun actionTitle() = "Run"
}
