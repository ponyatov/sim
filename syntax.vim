" Language: sim
" Maintainer: (c) Dmitry Ponyatov <dponyatov@gmail.com>, all rights reserved
" GitHub: https://github.com/ponyatov/sim

syntax match Comment	"\v#.*"
syntax match Operator	"\v[=@+~]"
syntax match Type		"\v[\[\]()]"
"syntax match Constant	"\v[a-zA-Z0-9.]+"
syntax match Constant	"\vdir|fn|op"
syntax region String	start="'" end="'"
