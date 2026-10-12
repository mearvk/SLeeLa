// The `intellijPlatform { }` repositories DSL used in dependencyResolutionManagement
// is an extension function contributed by the IntelliJ Platform settings plugin.
// In a settings.gradle.kts it must be imported explicitly, otherwise it resolves
// to "Unresolved reference 'intellijPlatform'". See the plugin's repositories
// extension docs and the JetBrains reference settings files.
import org.jetbrains.intellij.platform.gradle.extensions.intellijPlatform

pluginManagement {
    repositories { gradlePluginPortal(); mavenCentral() }
}

// Apply the settings plugin here (version in sync with the
// `org.jetbrains.intellij.platform` plugin in build.gradle.kts). It requires
// Gradle 9.0.0+, which the wrapper is pinned to.
plugins {
    id("org.jetbrains.intellij.platform.settings") version "2.19.0"
}

dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    @Suppress("UnstableApiUsage")
    repositories {
        mavenCentral()
        intellijPlatform { defaultRepositories() }
    }
}

rootProject.name = "sleela-intellij"
