" SLeeLa Turkish (Al Bayrak): crimson red and crescent white.
" Derived from the national colours of Türkiye (Turkish Flag Law): Red #E30A17
" and White #FFFFFF. Interpretive dark design palette, not an official
" representation of the Republic of Türkiye.
set background=dark
hi clear
if exists("syntax_on") | syntax reset | endif
let g:colors_name = "sleela-turkish"
hi Normal guifg=#F6EEEF guibg=#2A0A0D
hi Cursor guifg=#2A0A0D guibg=#E8505B
hi CursorLine guibg=#3C1115
hi LineNr guifg=#A8767A guibg=#2A0A0D
hi CursorLineNr guifg=#E8505B gui=bold
hi Visual guifg=#FFFFFF guibg=#8A1A22
hi Search guifg=#2A0A0D guibg=#F0A9AE
hi StatusLine guifg=#FFFFFF guibg=#E30A17 gui=bold
hi Pmenu guifg=#F6EEEF guibg=#4A1519
hi PmenuSel guifg=#2A0A0D guibg=#E8505B
hi Error guifg=#FF8A3D gui=undercurl
hi Todo guifg=#2A0A0D guibg=#F0A9AE gui=bold
hi Keyword guifg=#E8505B gui=bold
hi Statement guifg=#E8505B gui=bold
hi Type guifg=#F0A9AE gui=bold
hi String guifg=#F0C9A9
hi Number guifg=#F3D5C7
hi Constant guifg=#E87A82
hi Function guifg=#F6E3E4 gui=bold
hi Identifier guifg=#F6EEEF
hi Operator guifg=#E4CCCE
hi Comment guifg=#A8767A gui=italic
hi PreProc guifg=#F0A9AE
hi Special guifg=#F0C9A9
hi ErrorMsg guifg=#FF8A3D guibg=#3C1115 gui=bold
hi WarningMsg guifg=#F0C9A9
