package com.mearvk.sleela.ide.actions

import com.mearvk.sleela.ide.SleelaIcons

/**
 * Transpile the current file through Nordshrift:
 * `sleela nordshrift --emit --target=java <file>`.
 */
class SleelaTranspileFileAction : SleelaFileAction(
    text = "Transpile with Nordshrift",
    description = "Emit the current SLeeLa file through the Nordshrift transpiler (Java target)",
    icon = SleelaIcons.TRANSPILE,
) {
    override fun arguments(filePath: String) =
        listOf("nordshrift", "--emit", "--target=java", filePath)

    override fun actionTitle() = "Nordshrift"
}
