if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
inoremap <S-Tab> =InsertTabWrapper("backward")
map! <S-Insert> <MiddleMouse>
vmap ,gq :s/\s\+/ /ggvgq
nmap ,gq :%s/\s\+/ /ggq1G
vmap [% [%m'gv``
map \c a/* */hi
vmap ]% ]%m'gv``
vmap _j :call Justify('tw',4)
nmap _j :%call Justify('tw',4)
vmap a% [%v]%
nmap gx <Plug>NetrwBrowseX
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cfile>"),0)
map <S-Insert> <MiddleMouse>
inoremap 	 =InsertTabWrapper("forward")
imap \s BRstruct ${}F$
let &cpo=s:cpo_save
unlet s:cpo_save
set background=dark
set backspace=indent,eol,start
set cmdheight=2
set completeopt=longest,menuone
set fileencodings=ucs-bom,utf-8,default,latin1
set helplang=ru
set hlsearch
set langmap=ёйцукенгшщзхъфывап��олдэячсмить;`qwertyuiop[]asdfghjkl'zxcvbnm
set nomodeline
set printoptions=paper:a4
set ruler
set runtimepath=~/.vim,/var/lib/vim/addons,/usr/share/vim/vimfiles,/usr/share/vim/vim74,/usr/share/vim/vimfiles/after,/var/lib/vim/addons/after,~/.vim/after
set showcmd
set smartindent
set suffixes=.bak,~,.swp,.o,.info,.aux,.log,.dvi,.bbl,.blg,.brf,.cb,.ind,.idx,.ilg,.inx,.out,.toc
set wildmenu
" vim: set ft=vim :
