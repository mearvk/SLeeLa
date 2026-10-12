package com.mearvk.sleela.ide

import com.intellij.lang.Commenter

/** Comment forms per SLEELA.syntax §1.1: `//` line and `/* */` block. */
class SleelaCommenter : Commenter {
    override fun getLineCommentPrefix() = "//"
    override fun getBlockCommentPrefix() = "/*"
    override fun getBlockCommentSuffix() = "*/"
    override fun getCommentedBlockCommentPrefix(): String? = null
    override fun getCommentedBlockCommentSuffix(): String? = null
}
