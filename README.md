# ITPLD ELF Linker

## What is ITPLD

ITPLD is a pet-project by Maxim vashv1las Vashkevich. It is an ELF Linker for
x86-64 Linux systems, written without any dependencies (except the libc)
using standard C99.

## Design goals

ITPLD is a simple linker, so it does not support such things as resolving big .a
static libraries, or all the types of relocations and it does not support any
architecture that is different from x86-64, but it works fine when the task
is to link a simple program into executable. It just resolves symbols (strong-weak)
and performs the relocation, nothing more.

## Dependencies

### Runtime dependencies

There is only one runtime dependency - the ISO C99. No system-specific
dependencies, no bloated frameworks. Not even matter you if you run glibc or musl.

### Build dependencies

Clang, LLD, clang-tidy (`-DITPLD_ENABLE_CLANG_TIDY=ON`), CMake.

## How to build the linker

ITPLD linker uses the LLVM stack, so it is highly recommended for you to use it
either. Make sure that clang and lld are installed on your system.

Building (Release version, without unit tests):

```bash
cmake -B build -DCMAKE_C_COMPILER=clang -DCMAKE_LINKER_TYPE=LLD
cmake --build build
```

To build Debug version, add the `-DCMAKE_BUILD_TYPE=Debug` parameter.
To build tests, add the `-DITPLD_BUILD_TESTS=ON` parameter.
To enable linter (clang-tidy), add the `-DITPLD_ENABLE_CLANG_TIDY=ON` parameter.

## How to use the linker

ITPLD options:

```text
-h - help message
-v - version
-e - entry
-o - output
[no key] input
```

ITPLD supports only these keys, and only short (such as `-h` or `-v` options)
due to complexity of CLI parser (no system-specific dependencies like
`getopt()`, remember?)

## Development process

### Git Hooks

After you clone the repo, set up git hooks in scripts/hooks:

```bash
git config core.hooksPath scripts/hooks
```

### Testing the ITPLD

Testing the ITPLD is performed without using any frameworks, such as GoogleTest.
Only libc `assert()`.

### Current status

Foundation infrastructure is implemented;
CLI logic is implemented.
ELF parsing and linking are not implemented yet.
