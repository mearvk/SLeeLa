" sleela-philosophers.vim (ftplugin) -- load the Guide-chain coloring.
"
" Sourced for .sleela buffers; it layers the philosophers Constructor
" Guide-chain syntax (the "::" growth separator and the Gold/Green Guide
" imparts) on top of whatever base .sleela syntax is active. It is harmless if
" sourced more than once.
"
" Install: place under an ftplugin/ directory on your runtimepath, e.g.
"   ~/.vim/ftplugin/sleela-philosophers.vim, together with the matching
"   syntax/sleela-philosophers.vim.

if exists("b:did_ftplugin_philosophers")
  finish
endif
let b:did_ftplugin_philosophers = 1

" Pull in the additive Guide-chain syntax if it is on the runtimepath.
runtime! syntax/sleela-philosophers.vim
