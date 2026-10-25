" SLeeLa Greek (Hellenic): the national colours of Greece.
" Source colours (Law 851/1978 on the National Flag; Hellenic Republic MFA
" state symbols): national Blue #0D5EAF ("galazio", the sky and the seas) and
" White #FFFFFF (the purity of the independence struggle; the cross denotes
" Eastern Orthodox Christianity). Heraldic blazon: azure, four bars argent.
" Interpretive dark design palette, not an official state representation.
set background=dark
hi clear
if exists("syntax_on") | syntax reset | endif
let g:colors_name = "sleela-greek"
" Foreground is marble white; keywords and UI chrome carry the exact national blue.
hi Normal guifg=#FFFFFF guibg=#0A1A30
hi Cursor guifg=#0A1A30 guibg=#0D5EAF
hi CursorLine guibg=#102744
hi LineNr guifg=#6E86A6 guibg=#0A1A30
hi CursorLineNr guifg=#0D5EAF gui=bold
hi Visual guifg=#FFFFFF guibg=#0D5EAF
hi Search guifg=#0A1A30 guibg=#FFFFFF
hi StatusLine guifg=#FFFFFF guibg=#0D5EAF gui=bold
hi Pmenu guifg=#EAF1FB guibg=#14335C
hi PmenuSel guifg=#FFFFFF guibg=#0D5EAF
hi Error guifg=#FF6B6B gui=undercurl
hi Todo guifg=#0A1A30 guibg=#FFFFFF gui=bold
hi Keyword guifg=#2E7BD6 gui=bold
hi Statement guifg=#2E7BD6 gui=bold
hi Type guifg=#5E9FE0 gui=bold
hi String guifg=#9FC3EC
hi Number guifg=#C7DBF3
hi Constant guifg=#4C8FDE
hi Function guifg=#FFFFFF gui=bold
hi Identifier guifg=#EAF1FB
hi Operator guifg=#C7D4E4
hi Comment guifg=#6E86A6 gui=italic
hi PreProc guifg=#5E9FE0
hi Special guifg=#9FC3EC
hi ErrorMsg guifg=#FF6B6B guibg=#2A1620 gui=bold
hi WarningMsg guifg=#9FC3EC
