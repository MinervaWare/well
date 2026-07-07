# Wellang

<div align="center">
  <br />
  <p>
    <a href="https://github.com/wellang/wellang.git"><img src="https://www.minervaware.net/_app/immutable/assets/wellang.Bu2wDHtA.png" width="800" alt="wellang" /></a>
  </p>
  <br />
  <p>
    <br> <a href="https://discord.gg/sBqBFKq3Ab"><img src="https://img.shields.io/discord/1377132755392925727?color=5865F2&logo=discord&logoColor=white" alt="MinervaWare discord server" /></a> </br>
  </p>
</div>


> [!WARNING]
> Wellang is in an extremely early state with pending syntax changes and the development of the compiler. MAKE SURE you keep wellang up to date!


# About

Wellang is a high-level assembly language. Our goal is to achieve one instruction set for every CPU, give a faster alternative to LLVM for language developers, and provide a good educational path to people looking to learn the basics of an assembly language.

### Supported Architectures

This is the list of actively maintained architectures for Wellang. This does not mean fully implemented or finished (reference the note above), it simply means they are being actively developed. 

* [x] ALPHA
* [x] AMD_X86_64
* [x] I386
* [x] ARM_MAC (aarch64 - ARMv8)
* [ ] ARMv7 (aarch32)
* [x] RV64 (RISC-V 64-bit generic)
* [ ] RV32 (RISC-V 32-bit generic)
* [ ] ITANIUM_64
* [x] POWERPC (ppc64, tested with the Cell Power4) 
* [ ] RS6000 (POWER)
* [ ] SPARC
* [ ] S390 (s390 Z/VM 4.4)
* [ ] S390X
* [ ] IBM_Z
* [x] MIPS32 (mips32r2el)
* [ ] MIPS64
* [ ] HPPA

### NEED HELP?

Wellang discord server: https://discord.gg/sBqBFKq3Ab

MinervaWare Contact: https://www.minervaware.net/contact

# Compile Wellang

Wellang has no dependancies other than gcc.

To compile Wellang simply run:

```
make
```

## Hello World program

```
~constants {
    ~string:text = "Hello World!"
}

~int:main {
    move~ text, r1
    call~ printf
    return~ 0
}
```

## compile program
to compile your well program you can run ``well`` in your terminal:
```
well main.well -o out
```

## Vim syntax highlighting
```
make vim
```

### Credits
![Contributors](https://contrib.rocks/image?repo=wellang/well)

## Stargazers over time

[![Stargazers over time](https://starchart.cc/wellang/well.svg)](https://starchart.cc/wellang/well)

