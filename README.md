# Symple Code *(Syc)* [![License](https://img.shields.io/github/license/SympleLib/SympleCode?label=License)](LICENSE) ![Activity](https://img.shields.io/github/commit-activity/y/SympleLib/SympleCode?color=4&label=Commits)

![Symple](https://symplelib.github.io/syc/icon.png "Symple")

## About

Syc is a low-level programming language based on the [C Family](https://en.wikipedia.org/wiki/List_of_C-family_programming_languages) meant to give high level programming at low level speeds. Syc uses an LLVM backend to produce code on any platform.

***NOTE**: Not yet tested on platforms other than Windows*

### Design Goals

To see full heirachy, visit the [wiki](https://youtu.be/dQw4w9WgXcQ). Here is a short summary:

   - C# style syntax for familiarity
   - No undefined behavior
     - No uninitialized data
     - Memory objects are automatically allocated either on the stack or the heap with reference counting depending on the use
   - Variables are immutable by default
   - You can use variables, functions, classes & structs from anywhere in the file
   - Modular programming with `import` statements
   - Easy interface with c libraries, including libc

## Getting the Source

- Make sure [**Git**](https://git-scm.com/download) (Source control program) is installed on your device
- Run `git clone --recursive https://github.com/SympleLib/SympleCode.git` in your terminal in a designated folder
  
## Remarks

  - [Minsk Compiler (C#)](https://github.com/terrajobst/minsk) and the [tutorial series](https://www.youtube.com/playlist?list=PLRAdsfhKI4OWNOSfS7EUu5GRAVmze1t2y) to go with it
  - [chibicc Compiler (C)](https://github.com/rui314/chibicc)
  - [HyperLnage Compiler (C++)](https://github.com/SkillerRaptor/HyperLang)
