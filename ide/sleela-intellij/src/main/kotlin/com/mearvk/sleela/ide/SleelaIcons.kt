package com.mearvk.sleela.ide

import com.intellij.openapi.util.IconLoader

/** Brand icons for the SLeeLa plugin (Slick-Black ground, brand-green mark). */
object SleelaIcons {
    /** SLeeLa source file icon. */
    @JvmField val FILE = load("/icons/sleela-file.svg")

    /** Run the current SLeeLa program. */
    @JvmField val RUN = load("/icons/sleela-run.svg")

    /** Check / validate SLeeLa source. */
    @JvmField val CHECK = load("/icons/sleela-check.svg")

    /** Build a persistent SLeeLa artifact. */
    @JvmField val BUILD = load("/icons/sleela-build.svg")

    /** Nordshrift transpile. */
    @JvmField val TRANSPILE = load("/icons/sleela-transpile.svg")

    /** SLeeLa tool window. */
    @JvmField val TOOL_WINDOW = load("/icons/sleela-toolwindow.svg")

    private fun load(path: String) = IconLoader.getIcon(path, SleelaIcons::class.java)
}
