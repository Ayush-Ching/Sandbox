An interactive sand simulation written in `C` and `Raylib`

https://github.com/user-attachments/assets/7d7e7435-aac8-44cc-9757-8e77f79399ec

The compiles binary file is available [here](https://ayush-ching.itch.io/sandbox)

## Required Dependencies

You need a `C` compiler like `gcc` or `clang`, `raylib` library installed to compile it.

Along with these, you can use `make` or `cmake` for compiling.

The included `Makefile` in the project assumes you have `gcc` and `raylib` installed.

On Windows, you can get `gcc`, `raylib` and `make` using msys2 UCRT64 terminal (found [here](https://www.msys2.org/)).<br>
After installing the msys2 ucrt64, open it and run the following command:
```
pacman -S mingw-w64-ucrt-x86_64-gcc \
          mingw-w64-ucrt-x86_64-make \
          mingw-w64-ucrt-x86_64-raylib
```

For Linux machines, install `raylib` from your package manager if it is provided or build it from source.

## Compiling

Just run `make` from the project root if you have `make` installed or else use any other way to compile and link the files yourself.
