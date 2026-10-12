package com.mearvk.sleela.ide.actions

import com.mearvk.sleela.ide.SleelaIcons

/**
 * Build a persistent SLeeLa artifact from the current file:
 * `sleela compile <file> -o <file>.out.sleela`.
 */
class SleelaBuildFileAction : SleelaFileAction(
    text = "Build SLeeLa Artifact",
    description = "Compile the current SLeeLa file into a persistent artifact",
    icon = SleelaIcons.BUILD,
) {
    override fun arguments(filePath: String): List<String> {
        val output = if (filePath.endsWith(".sleela")) {
            filePath.removeSuffix(".sleela") + ".out.sleela"
        } else {
            "$filePath.out.sleela"
        }
        return listOf("compile", filePath, "-o", output)
    }

    override fun actionTitle() = "Build"
}
