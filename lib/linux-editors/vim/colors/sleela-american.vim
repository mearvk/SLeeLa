" SLeeLa American Professional color scheme.
set background=dark
hi clear
if exists("syntax_on") | syntax reset | endif
let g:colors_name = "sleela-american"
hi Normal guifg=#F4F7FA guibg=#101820 ctermfg=255 ctermbg=234
hi Cursor guifg=#101820 guibg=#7FB3FF ctermfg=234 ctermbg=111
hi CursorLine guibg=#172632 ctermbg=235
hi LineNr guifg=#687887 guibg=#101820 ctermfg=245 ctermbg=234
hi CursorLineNr guifg=#7FB3FF gui=bold ctermfg=111 cterm=bold
hi Visual guifg=#F4F7FA guibg=#233D55 ctermfg=255 ctermbg=24
hi Search guifg=#101820 guibg=#F2C078 ctermfg=234 ctermbg=222
hi StatusLine guifg=#F4F7FA guibg=#233D55 gui=bold ctermfg=255 ctermbg=24 cterm=bold
hi Pmenu guifg=#F4F7FA guibg=#233D55 ctermfg=255 ctermbg=24
hi PmenuSel guifg=#101820 guibg=#7FB3FF ctermfg=234 ctermbg=111
hi MatchParen guifg=#F4F7FA guibg=#31536C gui=bold ctermfg=255 ctermbg=24 cterm=bold
hi Error guifg=#FF6B6B gui=undercurl ctermfg=203
hi Todo guifg=#F2C078 guibg=#233D55 gui=bold ctermfg=222 ctermbg=24 cterm=bold
hi Keyword guifg=#7FB3FF gui=bold ctermfg=111 cterm=bold
hi Statement guifg=#7FB3FF gui=bold ctermfg=111 cterm=bold
hi Type guifg=#73DACA gui=bold ctermfg=116 cterm=bold
hi String guifg=#F2C078 ctermfg=222
hi Number guifg=#C4A7E7 ctermfg=183
hi Constant guifg=#FF8F8F ctermfg=210
hi Function guifg=#82C91E gui=bold ctermfg=112 cterm=bold
hi Identifier guifg=#F4F7FA ctermfg=255
hi Operator guifg=#D8DEE9 ctermfg=253
hi Comment guifg=#8A96A3 gui=italic ctermfg=245 cterm=italic
hi PreProc guifg=#F0A6CA ctermfg=218
hi Special guifg=#73DACA ctermfg=116
hi ErrorMsg guifg=#FF6B6B guibg=#301A22 gui=bold ctermfg=203 ctermbg=235 cterm=bold
hi WarningMsg guifg=#F2C078 ctermfg=222
