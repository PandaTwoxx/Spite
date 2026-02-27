SpiteLang v1.2

The "C# Killer" Turing Tarpit.

SpiteLang is a low-level, stack-and-void based language designed to make simple tasks difficult and complex tasks legendary. It compiles directly to native C++ machine code.

Installation

Install SpiteLang with a single command:

curl -sSL [https://raw.githubusercontent.com/PandaTwoxx/Spite/main/install.sh](https://raw.githubusercontent.com/PandaTwoxx/Spite/main/install.sh) | bash


Language Specification

The Architecture

Alpha Stack: Primary data stack.

Beta Stack: Secondary transit stack.

The Void: A configurable block of integer memory (Default: 64KB).

The Echo: Hidden call stack for ^ (Leap) and ↩ (Return).

Instruction Set

Symbol

Name

Description

∆

Rise

Pop Alpha, increment, push to Beta.

∇

Fall

Pop Beta, decrement, push to Alpha.

ø

Clone

Duplicate top of Alpha.

§

Speak

Print Alpha as ASCII.

#

Echo

Print Alpha as Integer.

⊞

Bury

Void[P] = Alpha.pop()

⊟

Exhume

Alpha.push(Void[P])

» / «

Shift

Increment/Decrement Pointer P.

¿ / ?

If/End

Skip block if Alpha top is 0.

@ / ^

Mark/Leap

Define/Call a checkpoint.

↩

Return

Return to last Leap point.

Compiling to Native Executable

Transpile SpiteLang into optimized C++:

spite -c your_code.st out_binary
./out_binary


Standard Library

Import using: #import "syslib.st"
The compiler automatically checks ~/.spite/lib/ for global imports.