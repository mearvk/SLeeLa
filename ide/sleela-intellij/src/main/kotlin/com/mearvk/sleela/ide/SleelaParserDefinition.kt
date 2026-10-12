package com.mearvk.sleela.ide

import com.intellij.extapi.psi.PsiFileBase
import com.intellij.lang.ParserDefinition
import com.intellij.lang.PsiParser
import com.intellij.lexer.Lexer
import com.intellij.psi.PsiElement
import com.intellij.psi.PsiFile
import com.intellij.psi.FileViewProvider
import com.intellij.psi.tree.IElementType
import com.intellij.psi.tree.IFileElementType
import com.intellij.lang.ASTNode

/** IntelliJ parser boundary. Tokens come from the authoritative SLeeLa compiler bridge. */
class SleelaParserDefinition : ParserDefinition {
    companion object {
        val FILE = IFileElementType(SleelaLanguage)
    }

    override fun createLexer(project: com.intellij.openapi.project.Project?): Lexer = SleelaLexer(project)
    override fun getWhitespaceTokens() = SleelaTokenTypes.WHITE_SPACES
    override fun getCommentTokens() = SleelaTokenTypes.COMMENTS
    override fun getStringLiteralElements() = SleelaTokenTypes.STRINGS
    override fun createParser(project: com.intellij.openapi.project.Project?): PsiParser = SleelaParser()
    override fun getFileNodeType() = FILE
    override fun createFile(viewProvider: FileViewProvider): PsiFile =
        object : PsiFileBase(viewProvider, SleelaLanguage) {
            override fun getFileType(): com.intellij.openapi.fileTypes.FileType = SleelaFileType
        }
    override fun spaceExistanceTypeBetweenTokens(left: ASTNode?, right: ASTNode?): ParserDefinition.SpaceRequirements =
        ParserDefinition.SpaceRequirements.MAY
    override fun createElement(node: ASTNode): PsiElement = com.intellij.extapi.psi.ASTWrapperPsiElement(node)
}
