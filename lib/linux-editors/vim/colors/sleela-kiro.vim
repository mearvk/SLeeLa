" Kiro color scheme. Palette: purple, black, dark blue, yellow, white.
set background=dark
hi clear
if exists("syntax_on") | syntax reset | endif
let g:colors_name = "sleela-kiro"
hi Normal guifg=#F5F5FA guibg=#0B0B12 ctermfg=255 ctermbg=233
hi Cursor guifg=#0B0B12 guibg=#A98EDA ctermfg=233 ctermbg=140
hi CursorLine guibg=#15131F ctermbg=234
hi LineNr guifg=#5A5470 guibg=#0B0B12 ctermfg=60 ctermbg=233
hi CursorLineNr guifg=#A98EDA gui=bold ctermfg=140 cterm=bold
hi Visual guifg=#F5F5FA guibg=#1E2A55 ctermfg=255 ctermbg=17
hi Search guifg=#0B0B12 guibg=#E8C547 ctermfg=233 ctermbg=221
hi StatusLine guifg=#F5F5FA guibg=#241B3A gui=bold ctermfg=255 ctermbg=236 cterm=bold
hi Pmenu guifg=#F5F5FA guibg=#241B3A ctermfg=255 ctermbg=236
hi PmenuSel guifg=#0B0B12 guibg=#A98EDA ctermfg=233 ctermbg=140
hi MatchParen guifg=#F5F5FA guibg=#1E2A55 gui=bold ctermfg=255 ctermbg=17 cterm=bold
hi Error guifg=#FF6B6B gui=undercurl ctermfg=203
hi Todo guifg=#E8C547 guibg=#241B3A gui=bold ctermfg=221 ctermbg=236 cterm=bold
hi Keyword guifg=#A98EDA gui=bold ctermfg=140 cterm=bold
hi Statement guifg=#A98EDA gui=bold ctermfg=140 cterm=bold
hi Type guifg=#6E8BE0 gui=bold ctermfg=68 cterm=bold
hi String guifg=#E8C547 ctermfg=221
hi Number guifg=#8A7BD8 ctermfg=104
hi Constant guifg=#C0A9F0 ctermfg=183
hi Function guifg=#F2D45C gui=bold ctermfg=221 cterm=bold
hi Identifier guifg=#F5F5FA ctermfg=255
hi Operator guifg=#CFCFE6 ctermfg=252
hi Comment guifg=#6A6482 gui=italic ctermfg=60 cterm=italic
hi PreProc guifg=#B79CE6 ctermfg=146
hi Special guifg=#6E8BE0 ctermfg=68
hi ErrorMsg guifg=#FF6B6B guibg=#2A1222 gui=bold ctermfg=203 ctermbg=234 cterm=bold
hi WarningMsg guifg=#E8C547 ctermfg=221
