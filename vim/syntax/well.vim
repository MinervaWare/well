"" https://vimhelp.org/syntax.txt.html

if exists("b:current_syntax")
	finish
endif

syntax keyword welldatatypes int float char string ptr Any 
syntax keyword wellins call move inline return not and or nor nand xor add sub mul div mod je jne jle jge
syntax keyword wellCond if while for
syntax keyword wellInc #include 

syntax match wellImp "@[A-Za-z][0-9A-Za-z-_]*"
syntax match wellCom "; [A-Za-z][0-9A-Za-z-_]*"

syntax match welldirective "#[A-Za-z][0-9A-Za-z-_]*"

syntax match wellnum "\-\?\<\d\+\>" display
syntax region wellstring start=/\v'/ skip=/\v\\./ end=/\v'/

highlight link welldatatypes Type
highlight link wellins keyword

highlight def link welldirective Statement

highlight link wellnum Number
highlight link wellstring String

highlight link wellCond Conditional
highlight link wellInc Include
highlight link wellImp Todo	
highlight link wellCom Comment

let b:current_syntax = "well"
