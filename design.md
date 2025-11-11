# DESIGN

## Sample

```
~include <std.well>

~constants {
	~string:text = 'Hello World!'
}

~int:main {
    ~string:someLocal = 'hellope'
	move~ 1, rax
	move~ 1, rdi
	move~ text, rsi
	move~ text.len, rdx
	syscall
	return~ 0
}
```

## Variables

Variables are held as constants in a const section

```
~constants: {
    ; variable . . .
}
```

```
~type:name = ...

~string:text = "Hello World!"

~char:h = 'h'

~int:number = 64

~float:decimal = 3.14
```

## Functions

```
~return_type:name {
    ...
}

~int:sum {
    return~ (r1+r2)
}
```

## Registers

* Wellang is using a custom set of "virtual" registers (r1 - r7)
* Why? So it can easily be mapped to arm & x86 during compilation.
