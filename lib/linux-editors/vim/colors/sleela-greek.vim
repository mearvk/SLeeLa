" SLeeLa Greek (Hellenic): sea blue and marble white.
" Derived from the national colours of Greece (Law 851/1978 on the National
" Flag): Blue #0D5EAF and White #FFFFFF. Interpretive dark design palette, not
" an official representation of the Hellenic Republic.
set background=dark
hi clear
if exists("syntax_on") | syntax reset | endif
let g:colors_name = "sleela-greek"
hi Normal guifg=#EEF3FA guibg=#0B1B33
hi Cursor guifg=#0B1B33 guibg=#4C8FDE
hi CursorLine guibg=#122744
hi LineNr guifg=#6E86A6 guibg=#0B1B33
hi CursorLineNr guifg=#4C8FDE gui=bold
hi Visual guifg=#FFFFFF guibg=#1C4C86
hi Search guifg=#0B1B33 guibg=#9FC3EC
hi StatusLine guifg=#FFFFFF guibg=#0D5EAF gui=bold
hi Pmenu guifg=#EEF3FA guibg=#15335C
hi PmenuSel guifg=#0B1B33 guibg=#4C8FDE
hi Error guifg=#FF6B6B gui=undercurl
hi Todo guifg=#0B1B33 guibg=#9FC3EC gui=bold
hi Keyword guifg=#4C8FDE gui=bold
hi Statement guifg=#4C8FDE gui=bold
hi Type guifg=#7FB2E8 gui=bold
hi String guifg=#9FC3EC
hi Number guifg=#C7DBF3
hi Constant guifg=#6FA8E0
hi Function guifg=#E8EEF6 gui=bold
hi Identifier guifg=#EEF3FA
hi Operator guifg=#C7D4E4
hi Comment guifg=#6E86A6 gui=italic
hi PreProc guifg=#7FB2E8
hi Special guifg=#9FC3EC
hi ErrorMsg guifg=#FF6B6B guibg=#2A1620 gui=bold
hi WarningMsg guifg=#9FC3EC
