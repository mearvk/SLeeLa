" SLeeLa brand color scheme -- the potent cut of the Kiro palette.
" Palette: purple, black, dark blue, yellow, white (more saturated).
set background=dark
hi clear
if exists("syntax_on") | syntax reset | endif
let g:colors_name = "sleela-sleela"
hi Normal guifg=#FFFFFF guibg=#08080E ctermfg=231 ctermbg=232
hi Cursor guifg=#08080E guibg=#B07CFF ctermfg=232 ctermbg=141
hi CursorLine guibg=#141022 ctermbg=233
hi LineNr guifg=#5B4F82 guibg=#08080E ctermfg=60 ctermbg=232
hi CursorLineNr guifg=#B07CFF gui=bold ctermfg=141 cterm=bold
hi Visual guifg=#FFFFFF guibg=#17236A ctermfg=231 ctermbg=18
hi Search guifg=#08080E guibg=#FFD21E ctermfg=232 ctermbg=220
hi StatusLine guifg=#FFFFFF guibg=#2A1358 gui=bold ctermfg=231 ctermbg=54 cterm=bold
hi Pmenu guifg=#FFFFFF guibg=#2A1358 ctermfg=231 ctermbg=54
hi PmenuSel guifg=#08080E guibg=#B07CFF ctermfg=232 ctermbg=141
hi MatchParen guifg=#FFFFFF guibg=#17236A gui=bold ctermfg=231 ctermbg=18 cterm=bold
hi Error guifg=#FF5C7A gui=undercurl ctermfg=204
hi Todo guifg=#FFD21E guibg=#2A1358 gui=bold ctermfg=220 ctermbg=54 cterm=bold
hi Keyword guifg=#B07CFF gui=bold ctermfg=141 cterm=bold
hi Statement guifg=#B07CFF gui=bold ctermfg=141 cterm=bold
hi Type guifg=#5A7CFF gui=bold ctermfg=63 cterm=bold
hi String guifg=#FFD21E ctermfg=220
hi Number guifg=#9B6CFF ctermfg=99
hi Constant guifg=#D7B3FF ctermfg=183
hi Function guifg=#FFE23D gui=bold ctermfg=220 cterm=bold
hi Identifier guifg=#FFFFFF ctermfg=231
hi Operator guifg=#E6E1FF ctermfg=189
hi Comment guifg=#6E5F9E gui=italic ctermfg=61 cterm=italic
hi PreProc guifg=#C98CFF ctermfg=141
hi Special guifg=#5A7CFF ctermfg=63
hi ErrorMsg guifg=#FF5C7A guibg=#2A0F1E gui=bold ctermfg=204 ctermbg=233 cterm=bold
hi WarningMsg guifg=#FFD21E ctermfg=220
