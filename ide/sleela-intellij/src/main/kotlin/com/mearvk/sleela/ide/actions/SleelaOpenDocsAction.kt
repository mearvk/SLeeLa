package com.mearvk.sleela.ide.actions

import com.intellij.ide.BrowserUtil
import com.intellij.openapi.actionSystem.AnAction
import com.intellij.openapi.actionSystem.AnActionEvent

/** Open the SLeeLa language syntax reference on GitHub. */
class SleelaOpenSyntaxAction : AnAction("Language Syntax Reference", "Open the SLeeLa syntax specification", null) {
    override fun actionPerformed(e: AnActionEvent) {
        BrowserUtil.browse("https://github.com/mearvk/SLeeLa/blob/master/SLEELA.syntax")
    }
}

/** Open the SLeeLa project homepage/README on GitHub. */
class SleelaOpenHomepageAction : AnAction("SLeeLa on GitHub", "Open the SLeeLa project on GitHub", null) {
    override fun actionPerformed(e: AnActionEvent) {
        BrowserUtil.browse("https://github.com/mearvk/SLeeLa")
    }
}
