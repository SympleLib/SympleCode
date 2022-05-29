# Symple Code *(Syc)* [![License](https://img.shields.io/github/license/SympleLib/SympleCode?label=License)](LICENSE) ![Activity](https://img.shields.io/github/commit-activity/y/SympleLib/SympleCode?color=4&label=Commits)

This repository contains the source code for SympleCode. <br />
Anyone is welcome to contribute or use the source from SympleCode.

## Contents

- [About](#about)
- [Getting Started](#getting-started)
- [Contributing](#contributing)
- [License](#license)

## About

![syc](https://symplelib.github.io/syc/icon.png "syc")

Syc is a low-level programming language based on the [C Family](https://en.wikipedia.org/wiki/List_of_C-family_programming_languages) meant to give high level programming at low level speeds. Syc uses an LLVM backend to produce code on any platform.

***NOTE**: Not yet tested on platforms other than Windows*

### Design Goals

To see full heirachy, visit the [wiki](https://youtu.be/dQw4w9WgXcQ). Here is a short summary:

   - C# style syntax for familiarity
   - No undefined behavior
     - Variables can either be copied, passed, borrowed, or referenced to function calls (referenced by default). Syc chooses the optimal memory management depending on variable use.
   - Variables are immutable by default
   - You can use symbols from anywhere in the file
   - Modular programming with `import` statements

Rust is a well-designed language. So we (I) copied them.

#### System similar to borrow checker
- All variables must be initialized before use
- You cannot pass the same value more than once
- You cannot pass a value whilst borrowing/referenced it
- You may not read/write to a value while it is being borrowed (by another owner)
- 

## Getting Started

### Building

1. Download the source code by using Git by cloning the repository, or
   downloading it as an archive file.

```shell
git clone https://github.com/SympleLib/SympleCode/HyperLang
cd SympleCode
```

2. Create a folder for CMake to generate the project files.

```shell
mkdir build
cd build
```

3. Configure the project by using CMake.

```shell
cmake .. -D CMAKE_BUILD_TYPE=Release
```

4. Build the project with CMake.

```shell
cmake --build .
```

5. Run the compiler to check if everything was built successfully.

## Contributing

Pull requests are welcome. For major changes, please open an issue first to
discuss what you would like to change.

## License

SympleCode is distributed under
the [MIT license](https://github.com/SympleLib/SympleCode/blob/master/LICENSE).
  
## Remarks

  - [Minsk Compiler (C#)](https://github.com/terrajobst/minsk) and the [tutorial series](https://www.youtube.com/playlist?list=PLRAdsfhKI4OWNOSfS7EUu5GRAVmze1t2y) to go with it
  - [chibicc Compiler (C)](https://github.com/rui314/chibicc)
  - [HyperLang Compiler (C++)](https://github.com/SkillerRaptor/HyperLang)
  - [LLVM Kaleidoscope Tutorial (C++)](https://llvm.org/docs/tutorial/)
