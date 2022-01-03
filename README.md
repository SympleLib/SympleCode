# Symple Code *(Syc)* [![License](https://img.shields.io/github/license/SympleLib/SympleCode?label=License)](LICENSE) ![Activity](https://img.shields.io/github/commit-activity/y/SympleLib/SympleCode?color=4&label=Commits)

![Symple](https://symplelib.github.io/syc/icon.png "Symple")

## About

Syc is a low-level programming language based on the [C Family](https://en.wikipedia.org/wiki/List_of_C-family_programming_languages) meant to give high level programming at low level speeds. Syc uses an LLVM backend to produce code on any platform.

***NOTE**: Not tested yet on platforms other than Windows*

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

1) Get required tools
    - [**Git**](https://git-scm.com/download) (Source control program)
    - [**Visual Studio**](https://visualstudio.microsoft.com/)
2) Clone the repository
   - `git clone --recursive https://github.com/SympleLib/SympleCode.git`
3) Open the Solution
    - In the folder **_SuperCode_**, you should see a file called **_SuperCode.sln_**, open it and you can view, edit, and compile the source code.
  
## Remarks

  - [Minsk Compiler (C#)](https://github.com/terrajobst/minsk)
  - [chibicc Compiler (C)](https://github.com/rui314/chibicc)
