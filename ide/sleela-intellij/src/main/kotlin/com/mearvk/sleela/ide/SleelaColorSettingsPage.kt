package com.mearvk.sleela.ide

import com.intellij.openapi.editor.colors.TextAttributesKey
import com.intellij.openapi.fileTypes.SyntaxHighlighter
import com.intellij.openapi.options.colors.AttributesDescriptor
import com.intellij.openapi.options.colors.ColorDescriptor
import com.intellij.openapi.options.colors.ColorSettingsPage
import javax.swing.Icon

/** "SLeeLa" entry under Settings | Editor | Color Scheme. */
class SleelaColorSettingsPage : ColorSettingsPage {

    override fun getIcon(): Icon = SleelaIcons.FILE
    override fun getHighlighter(): SyntaxHighlighter = SleelaSyntaxHighlighter()
    override fun getDisplayName(): String = "SLeeLa"

    override fun getAttributeDescriptors(): Array<AttributesDescriptor> = DESCRIPTORS
    override fun getColorDescriptors(): Array<ColorDescriptor> = ColorDescriptor.EMPTY_ARRAY
    override fun getAdditionalHighlightingTagToDescriptorMap(): MutableMap<String, TextAttributesKey>? = null

    override fun getDemoText(): String = """
        #sleela 1.11

        // SLeeLa — The Now. The Ingredient.
        /* A Java-like class on a native C/C++ core. */
        class Greeter {
            static void main() {
                let name = "SLeeLa";
                int count = 42;
                print("Hello from " + name);
                x = gc 50;
            }
        }
    """.trimIndent()

    companion object {
        private val DESCRIPTORS = arrayOf(
            AttributesDescriptor("Keyword", SleelaSyntaxHighlighter.KEYWORD),
            AttributesDescriptor("Type", SleelaSyntaxHighlighter.TYPE),
            AttributesDescriptor("String", SleelaSyntaxHighlighter.STRING),
            AttributesDescriptor("Number", SleelaSyntaxHighlighter.NUMBER),
            AttributesDescriptor("Comment", SleelaSyntaxHighlighter.COMMENT),
            AttributesDescriptor("Version directive", SleelaSyntaxHighlighter.DIRECTIVE),
            AttributesDescriptor("Operator", SleelaSyntaxHighlighter.OPERATOR),
            AttributesDescriptor("Punctuation", SleelaSyntaxHighlighter.PUNCTUATION),
            AttributesDescriptor("Identifier", SleelaSyntaxHighlighter.IDENTIFIER),
            AttributesDescriptor("Bad character", SleelaSyntaxHighlighter.BAD),
        )
    }
}
