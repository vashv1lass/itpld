# ITPLD ELF Linker

## What is ITPLD

ITPLD is a pet-project by Maxim vashv1las Vashkevich. It is an ELF Linker for
x86-64 Linux systems, written without any dependencies (except the libc dependency).
It is written using C99 and can link some simple ELF `.o` files.

## Design goals

ITPLD is a simple linker, so it does not support such things as resolving big .a
static libraries, or all the types of relocations and it does not support any
architecture that is different from x86-64, but it works fine when the task
is to link a simple program into executable. It just resolves symbols (strong-weak)
and performs the relocation, nothing more.

## How to build the linker

ITPLD linker uses the LLVM stack, so make sure that clang and lld are installed
on your system.

Building (Release version, without unit tests):

```bash
cmake -B build -DCMAKE_C_COMPILER=clang -DCMAKE_LINKER_TYPE=LLD \
                                  -DITPLD_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

To build Debug version, omit the `-DCMAKE_BUILD_TYPE=Release` parameter.
To build tests, omit the `-DITPLD_BUILD_TESTS=OFF` parameter.

## Development process

### Git Hooks

After you clone the repo, set up git hooks in scripts/hooks:

```bash
git config core.hooksPath scripts/hooks
```

### Testing the ITPLD

Testing the ITPLD is performed without using any frameworks, such as GoogleTest.
Only libc `assert()`.
