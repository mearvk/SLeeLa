" Detect SLeeLa source files.
augroup sleela_filetype
  autocmd!
  autocmd BufRead,BufNewFile *.sleela setfiletype sleela
augroup END
