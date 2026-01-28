# Alpha v0.0.1 - 2024

## 11/23/24 - Total rewrite

* This "change" is deleting all of what was and adding this initial commit.
* functional parser.

## 11/25/24 - Design, parser, asmOut

* Started the asm output file.
* finished instruction parsing, this pretty much completes the parser.
* some design additions, added to the design file documentation.
* asm "example" test coorilating to the helloworld.well program.

## 11/26/24 - parser, asmOut

* Fixed some asmOut initialization issues.
* Did variable parsing.
* added some Werrors, fixed tab eater.

## 11/28/24 - variables to asm

* string and char conversions to asm

## 11/30/24 - hex conversions

* hex converter for integers.
* asm conversions for integers.

## 12/01/24 - Working Hello World 

* CPU architecture detection and "todo" setup for function conversions.
* Free parser data
* Simple arm macos instruction conversion (this allows a simple hello world to work)
* run gnu assembler on asm output

## 12/02/24 (and like 12-2am on 03) - Optimization and buf fixing

* Fixing and cleaning up all the memory issues and whatnot.
* trying to figure out a weird Instruction arg issue I am having.

## 12/03/24 - return types

* support for function types added.
* asm output for returning values and void functions made

## 12/05/24 - directives, compiler options, externs

* Added compiler directives for .well files, ``#NOMAIN`` for example tells the compiler to ignore main verification.
* Added compiler CLI options for output names and object-only output.
* Added extern support: ``~extern foo``.

## 12/06/24 - Local file inclusion, C flags and object compilation, register conversion

* Added support for including local ``.well`` files. ``~include "foo.well"``
* Changed the way files are compiled and linked.
* Added Arm Mac register conversions.
* added sstruct & lstruct instructions to save and load returned C structures.

## 12/08/24 - Get well compiling on windoez

* Changed some memory stuff so it will run fine on windows with Cygwin.
* Started adding stuff for AMD_X86_64 compilation.

## 12/09/24 - X86_64 instruction converstions

* "Added" support for X86_64 systems. (Working hello world).
