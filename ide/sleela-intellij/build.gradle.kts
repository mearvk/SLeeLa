import org.jetbrains.intellij.platform.gradle.TestFrameworkType

plugins {
    id("java")
    id("org.jetbrains.kotlin.jvm") version "2.2.20"
    // No version here: the IntelliJ Platform plugin is already placed on the
    // build classpath by org.jetbrains.intellij.platform.settings (pinned to
    // 2.19.0 in settings.gradle.kts). Re-declaring a version causes
    // "already on the classpath with an unknown version" at resolution time.
    id("org.jetbrains.intellij.platform")
}

group = providers.gradleProperty("group").get()
version = providers.gradleProperty("version").get()

// Repositories are declared centrally in settings.gradle.kts
// (dependencyResolutionManagement with RepositoriesMode.FAIL_ON_PROJECT_REPOS),
// so a project-level repositories { } block here is both redundant and rejected
// ("repository 'MavenRepo' was added by build file"). Do not add one.

dependencies {
    intellijPlatform {
        // Two ways to supply the IntelliJ Platform:
        //  1. localIdePath — point at an already-installed IDE (no download,
        //     guaranteed to match that IDE's build). Set it in gradle.properties
        //     or on the CLI: -PlocalIdePath=/path/to/idea-IU-XXX
        //  2. otherwise download the IntelliJ IDEA Community SDK pinned by
        //     platformVersion (needs network + a published version).
        val localIdePath = providers.gradleProperty("localIdePath").orNull
        if (!localIdePath.isNullOrBlank()) {
            local(localIdePath)
        } else {
            intellijIdea(providers.gradleProperty("platformVersion").get())
        }
        testFramework(TestFrameworkType.Platform)
    }
    testImplementation("org.junit.jupiter:junit-jupiter:5.11.4")
    testRuntimeOnly("org.junit.platform:junit-platform-launcher")
}

// SLeeLa IDE targets JDK 21. The IntelliJ Platform Gradle Plugin 2.x and the
// bundled Kotlin compiler do not accept JDK 25; pin the toolchain explicitly so
// the build is reproducible regardless of the host default JDK.
java {
    toolchain { languageVersion.set(JavaLanguageVersion.of(21)) }
}

kotlin {
    jvmToolchain(21)
}

intellijPlatform {
    pluginConfiguration {
        name = "SLeeLa IDE Integration"
        version = project.version.toString()
        ideaVersion {
            sinceBuild = providers.gradleProperty("pluginSinceBuild")
            // Only set untilBuild when the property is non-blank; an empty value
            // leaves the upper bound open so patch updates of the same line keep
            // the plugin enabled.
            val until = providers.gradleProperty("pluginUntilBuild").orNull
            if (!until.isNullOrBlank()) {
                untilBuild = until
            }
        }
    }
}

tasks.test {
    useJUnitPlatform()
}
