package com.mearvk.sleela.ide

import java.nio.file.Path

/**
 * Service-provider boundary for connecting the plugin to the repository compiler.
 * The concrete provider is supplied by the SLeeLa build/runtime distribution.
 */
interface SleelaCompilerProvider {
    fun create(projectRoot: Path): SleelaCompilerBridge
}

object SleelaCompilerProviders {
    @Volatile private var provider: SleelaCompilerProvider? = null

    fun install(value: SleelaCompilerProvider) { provider = value }
    fun create(projectRoot: Path): SleelaCompilerBridge =
        requireNotNull(provider) { "No SLeeLa compiler provider has been installed for this IDE project" }
            .create(projectRoot)
}