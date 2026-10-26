" SLeeLa Turkish (Al Bayrak): the national colours of Turkiye.
" Source colours (Turkish Flag Law; the flag is called al bayrak, the red
" flag): national Red #E30A17 (the field; by tradition the blood of martyrs and
" Ottoman sovereignty) and White #FFFFFF (the crescent and star -- peace,
" purity, and the Turkic/Islamic heritage symbol).
" Interpretive dark design palette, not an official state representation.
set background=dark
hi clear
if exists("syntax_on") | syntax reset | endif
let g:colors_name = "sleela-turkish"
" Foreground is crescent white; keywords and UI chrome carry the exact national red.
hi Normal guifg=#FFFFFF guibg=#2A0609
hi Cursor guifg=#2A0609 guibg=#E30A17
hi CursorLine guibg=#3E0F13
hi LineNr guifg=#A8767A guibg=#2A0609
hi CursorLineNr guifg=#E30A17 gui=bold
hi Visual guifg=#FFFFFF guibg=#E30A17
hi Search guifg=#2A0609 guibg=#FFFFFF
hi StatusLine guifg=#FFFFFF guibg=#E30A17 gui=bold
hi Pmenu guifg=#FBEAEB guibg=#4A1519
hi PmenuSel guifg=#FFFFFF guibg=#E30A17
hi Error guifg=#FF8A3D gui=undercurl
hi Todo guifg=#2A0609 guibg=#FFFFFF gui=bold
hi Keyword guifg=#F23D48 gui=bold
hi Statement guifg=#F23D48 gui=bold
hi Type guifg=#F0A9AE gui=bold
hi String guifg=#F0C9A9
hi Number guifg=#F3D5C7
hi Constant guifg=#E8505B
hi Function guifg=#FFFFFF gui=bold
hi Identifier guifg=#FBEAEB
hi Operator guifg=#E4CCCE
hi Comment guifg=#A8767A gui=italic
hi PreProc guifg=#F0A9AE
hi Special guifg=#F0C9A9
hi ErrorMsg guifg=#FF8A3D guibg=#3E0F13 gui=bold
hi WarningMsg guifg=#F0C9A9
