pluginManagement {
    repositories { gradlePluginPortal(); mavenCentral() }
}

dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories { mavenCentral(); intellijPlatform { defaultRepositories() } }
}

rootProject.name = "sleela-intellij"
