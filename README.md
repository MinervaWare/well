# Wellang

<div align="center">
  <br />
  <p>
    <a href="https://github.com/wellang/wellang.git"><img src="https://github.com/wellang/well/blob/main/wellang.png" width="800" alt="wellang" /></a>
  </p>
  <br />
  <p>
    <br> <a href="https://discord.gg/sBqBFKq3Ab"><img src="https://img.shields.io/discord/1377132755392925727?color=5865F2&logo=discord&logoColor=white" alt="MinervaWare discord server" /></a> </br>
  </p>
</div>

# NOTE

* Wellang is in an extremely early state with pending syntax changes and the development of the compiler. MAKE SURE you keep wellang up to date in these early stages.

# About

Wellang is a high-level assembly language targeted as an educational resource for people looking to learn assembly language and CPU logic. Wellang comes with a single instruction set that will compile to any system architecture making it very portable and easy to use on any device.

### NEED HELP?

Wellang discord server: https://discord.gg/sBqBFKq3Ab

MinervaWare Contact: https://www.minervaware.net/contact

# Compile Wellang

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

