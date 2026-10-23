" SLeeLa syntax highlighting.
if exists("b:current_syntax") | finish | endif
syn case match
syn keyword sleelaKeyword let var const if else while for in return break continue class interface extends implements new this super import package public private protected static final true false null nil and or not
syn keyword sleelaType String SLString Integer Int Long Float Double Boolean Character Byte Short Void Object List Map Set Array
syn match sleelaNumber "\<\d\+\>"
syn region sleelaString start=+"+ skip=+\\.+ end=+"+
syn region sleelaString start=+'+ skip=+\\.+ end=+'
syn match sleelaLineComment "//.*$"
syn region sleelaBlockComment start="/\*" end="\*/"
syn match sleelaTodo "\<\(TODO\|FIXME\|NOTE\|XXX\)\>" contained
syn match sleelaAnnotation "@[A-Za-z_][A-Za-z0-9_]*"
syn match sleelaFunction "\<[A-Za-z_][A-Za-z0-9_]*\ze\s*("
hi def link sleelaKeyword Keyword
hi def link sleelaType Type
hi def link sleelaNumber Number
hi def link sleelaString String
hi def link sleelaLineComment Comment
hi def link sleelaBlockComment Comment
hi def link sleelaTodo Todo
hi def link sleelaAnnotation PreProc
hi def link sleelaFunction Function
let b:current_syntax = "sleela"
