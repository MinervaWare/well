# TODO

## Type Bitfields

In C you can define struct data with specific bit sizes like this:

```c
struct foo {
   int bar :4; // an int that is 4 bits in size. 
}
```

I want to handle type sizes in wellang in a similar manner. Currently you can do this:

```
~constants {
    ~int:foo = 10
    ~float:bar = 10.1
}
```

Which is fine if you don't need extreme optimizations. 

The idea is to allow users to specify specific bit sizes (bitfield) for the types. This would be the standard to make things 32-bit, 16-bit, 64-bit, etc.

It would look like this:

```
~constants {
    ~int:32:foo = 10
    ; or if you need specific optimization
    ; 10 is only 4 bits - 1010
    ~int:4:bar = 10 
}
```

## Instruction operators

This is just being able to evaluate something as an instruction argument like this:

```
move~ (i<<1), r1
```

## If statements & loops

Self-explanatory, this follows the operators and just adds a new scope.

These would look something like this:
```
move~ foo, r1
move~ bar, r2
~if(r1<r2) {
    call~ baz
}

~while(r1<r2) {
    call~ baz
}
```

# Finished TODOs

This is the past TODOs from this list that have been implemented.

Finished implementatons should be appended with the date they were finished in their name like this:

```
pre-finished: ## Something
finished: ## Something (mm/dd/yy)
```
