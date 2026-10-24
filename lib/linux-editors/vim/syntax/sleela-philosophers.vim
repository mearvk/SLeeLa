" sleela-philosophers.vim -- SLeeLa Constructor Guide-chain coloring.
"
" Highlights the philosophers "Guide chain" growth syntax inside .sleela files:
"
"     Constructor() :: Patch :: Constructor :: Aim :: Guide
"
" The "::" growth separator is rendered as a conform operator; each Guide name
" after a "::" is colored by its impart -- Gold/Yellow when the Guide carries
" approving favor, Green when the Guide is still growing. This file is additive:
" it can be sourced on top of any sleela-* color scheme (see lib/linux-editors).
"
" Install: copy to ~/.vim/syntax/ (or a runtimepath syntax/ dir) and
"   :source it after your .sleela syntax, or let the bundled ftplugin do it.

if exists("b:current_syntax_philosophers")
  finish
endif

" ---- the "::" growth separator (the conform token) -----------------------
" Two colons, surrounded by optional spaces, joining one growth to the next.
syntax match slPhilGrowth "\s*::\s*" contained containedin=ALL

" ---- approving Guides -> Gold / Yellow -----------------------------------
" The IQ-201+ Guides that carry favor forward (see SLPhilosopherSeating):
" Warrant, Patch, Aim, Health, Guide, Office, Saint, Favor, Approval.
syntax keyword slPhilGuideGold contained Warrant Patch Aim Health Guide
      \ Office Saint Favor Approval

" ---- growing Guides -> Green ---------------------------------------------
" The Guides that are still maturing toward Approval.
syntax keyword slPhilGuideGreen contained Approach Notice Seed Frame Course
      \ Measure Vigor Pattern Ministry Counsel Impart Conform

" Only treat these names as Guides when they sit right after a "::" growth,
" so ordinary identifiers elsewhere are untouched. The region opens at "::".
syntax match slPhilGuideSite "::\s*\%(Warrant\|Patch\|Aim\|Health\|Guide\|Office\|Saint\|Favor\|Approval\)\>"
      \ contains=slPhilGrowth,slPhilGuideGold
syntax match slPhilGuideSite "::\s*\%(Approach\|Notice\|Seed\|Frame\|Course\|Measure\|Vigor\|Pattern\|Ministry\|Counsel\|Impart\|Conform\)\>"
      \ contains=slPhilGrowth,slPhilGuideGreen

" ---- Ordain -> the orthogonal ordinality axis ----------------------------
" Ordain is not a Guide and not a seating; it is the ordering of order that
" stands across all twenty-one. It is given its own distinct, authoritative
" color so it reads as orthogonal to the chain rather than part of it.
syntax keyword slPhilOrdain Ordain SLOrdain

" ---- colors: Gold/Yellow (approving) and Green (growing) ------------------
" Gold/Yellow impart.
highlight default slPhilGuideGold  guifg=#F2C078 gui=bold ctermfg=222 cterm=bold
" Green impart.
highlight default slPhilGuideGreen guifg=#82C91E gui=bold ctermfg=112 cterm=bold
" The conform operator "::" -- a calm, unified join.
highlight default slPhilGrowth     guifg=#73DACA ctermfg=116
" Ordain -- the orthogonal ordinality axis; a high, ordaining violet-gold.
highlight default slPhilOrdain     guifg=#C4A7E7 gui=bold,underline ctermfg=183 cterm=bold,underline

let b:current_syntax_philosophers = 1
