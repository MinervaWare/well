if exists("b:current_syntax")
	finish
endif

syntax keyword welldatatypes string long int mut resb resw resq resy resz 
syntax keyword wellins push move lea call lib asm jump push pop add sub aaa aas aad aam adc and movef nor xor return extern syscall 
syntax keyword wellfunc func
syntax keyword wellvar var

syn match welldirective "\.[A-Za-z][0-9A-Za-z-_]*"

syntax match wellnum "\-\?\<\d\+\>" display
syntax region wellstring start=/\v'/ skip=/\v\\./ end=/\v'/

highlight link welldatatypes Type
highlight link wellins keyword

highlight def link welldirective Statement

highlight link wellnum Number
highlight link wellstring String

highlight link wellfunc Function
highlight link wellvar Function

let b:current_syntax = "well"
