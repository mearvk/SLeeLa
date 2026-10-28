" SLeeLa syntax highlighting.
if exists("b:current_syntax") | finish | endif
syn case match
syn keyword sleelaKeyword let var const if else while for in return break continue class interface extends implements new this super import package public private protected static final true false null nil and or not
syn keyword sleelaType String SLString Integer Int Long Float Double Boolean Character Byte Short Void Object List Map Set Array
" Document markers (contextual words recognised only at document scope, not
" reserved keywords): author/authorial (syntax 2.4), material and memory (both
" syntax 2.5). Matched only when they begin a line (optionally indented) and are
" followed by an optional ';' and end of line, so an ordinary identifier named
" e.g. 'material' or 'memory' elsewhere is NOT highlighted -- mirroring how the
" compiler treats them (SLEELA.syntax sections 18, 19 and 20).
syn match sleelaMarker "^\s*\%(authorial\|author\|material\|memory\)\s*;\=\s*$"
" The #sleela / #schema document pragmas.
syn match sleelaPragma "^\s*#\%(sleela\|schema\)\>.*$"
syn match sleelaNumber "\<\d\+\>"
syn region sleelaString start=+"+ skip=+\\.+ end=+"+
syn region sleelaString start=+'+ skip=+\\.+ end=+'
" Comment forms the SLeeLa lexer accepts: // line, /* */ block, and the
" go-forward house style << >> block and < > single-line angle comments. An
" angle comment must begin a line (only whitespace before the opener).
syn match sleelaLineComment "//.*$"
syn region sleelaBlockComment start="/\*" end="\*/"
syn region sleelaAngleBlock start="^\s*<<" end=">>"
syn match sleelaAngleLine "^\s*<[^<].*>\=$"
syn match sleelaTodo "\<\(TODO\|FIXME\|NOTE\|XXX\)\>" contained
syn match sleelaAnnotation "@[A-Za-z_][A-Za-z0-9_]*"
" The  (U+2234) originated-source brander (syntax 2.4), the house analogue of @.
syn match sleelaBrander "\%u2234"
syn match sleelaFunction "\<[A-Za-z_][A-Za-z0-9_]*\ze\s*("
hi def link sleelaKeyword Keyword
hi def link sleelaType Type
hi def link sleelaMarker PreProc
hi def link sleelaPragma PreProc
hi def link sleelaBrander Special
hi def link sleelaNumber Number
hi def link sleelaString String
hi def link sleelaLineComment Comment
hi def link sleelaBlockComment Comment
hi def link sleelaAngleBlock Comment
hi def link sleelaAngleLine Comment
hi def link sleelaTodo Todo
hi def link sleelaAnnotation PreProc
hi def link sleelaFunction Function
let b:current_syntax = "sleela"
