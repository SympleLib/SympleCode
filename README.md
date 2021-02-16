# Symple C *(Symple Code)* [![License](https://img.shields.io/github/license/TeddyTelanoff/SympleCode.svg)](LICENSE)

![Symple](/res/Symple.png?raw=true "Symple")

## About SympleCode

Symple C is a programming language that is similar to the [C Programming Lanaguage](https://en.wikipedia.org/wiki/C_(programming_language)), but with some additional features.

## Getting the source

```
Disclamer: Symple Code only supports windows at the moment!
```

1) Get required tools
    - **Git** (Source control program) you can download from https://git-scm.com/download
    - **Visual Studio 2017 or later**
    - **Clang** (Compiler)
2) Clone the repository
   - `git clone --recursive https://github.com/TeddyTelanoff/SympleCode.git`
3) Run premake
    - In your folder, you should see a file called **GenerateProjects.bat**
    - Run **GenerateProjects.bat**, it will prompt you what version of visual studio you are using
      - for **Visual Studio 2017**, type in `vs2017`
      - for **Visual Studio 2019**, type in `vs2019`
    - Type enter.
4) Open the Solution
    - You should see a file called **SympleCode.sln**, open it and you can view, edit, and compile the source code.
  
## Remarks
  
Obviously, making a compiler is no symple task, here are some resources I used:
  - [Minsk Compiler (C#)](https://github.com/terrajobst/minsk)
  - [Simple C++ Lexer](https://gist.github.com/arrieta/1a309138689e09375b90b3b1aa768e20)
  - [chibicc Compiler (C)](https://github.com/rui314/chibicc)
