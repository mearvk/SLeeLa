pluginManagement {
    repositories { gradlePluginPortal(); mavenCentral() }
}

// The IntelliJ Platform settings plugin contributes the `intellijPlatform { }`
// repositories DSL used below. It must be applied here, in settings.gradle.kts,
// not only in build.gradle.kts — otherwise `intellijPlatform`/`defaultRepositories`
// are unresolved references at settings-evaluation time. Keep the version in sync
// with the `org.jetbrains.intellij.platform` plugin in build.gradle.kts.
plugins {
    id("org.jetbrains.intellij.platform.settings") version "2.19.0"
}

dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories { mavenCentral(); intellijPlatform { defaultRepositories() } }
}

rootProject.name = "sleela-intellij"
