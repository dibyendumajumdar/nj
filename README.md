# nj

This is an experimental project that aims to create a cut-down version of IBM's [Eclipse OMR](https://github.com/eclipse/omr) project focussing just on the compiler / JIT backend. 

Here is what is included:

* **`compiler`**:       Components for building compiler technology, such as JIT
                        compilers.
* **`jitbuilder`**:     An easy to use high level abstraction on top of the
                        compiler technology.
* **`tool`**:           Code generation tools for the build system
* **`fvtest`**:         Tests for the compiler

Following OMR components have been removed:

* ~~`gc`:             Garbage collection framework for managed heaps~~
* ~~`port`:           Platform porting library~~
* ~~`thread`:         A cross platform pthread-like threading library~~
* ~~`util`:           general utilities useful for building cross platform
                        runtimes~~
* ~~`omrsigcompat`:   Signal handling compatibility library~~
* ~~`omrtrace`:       Tracing library for communication with IBM Health Center
                        monitoring tools~~
* ~~`vm`:             APIs to manage per-interpreter and per-thread contexts~~
* ~~`example`:        Demonstration code to show how a language runtime might
                        consume some Eclipse OMR components~~

## Status

* Builds successfully on Linux, Mac OSX and Windows 10 
* Not all tests pass on Windows 10

## Roadmap

* Immediate focus is on trying to use the JIT engine behind a C front end [dmr_C](https://github.com/dibyendumajumdar/dmr_c)
* I would like to further trim the library to make it smaller and more focussed

## Build Instructions

### Linux or Mac OSX

* You will need CMake, Perl, bison and flex installed
* Follow steps below from the location of source directory.

```
mkdir build
cd build
cmake ..
make
```

### Windows 10

* You will need Visual Studio 2017, Perl, [Win flex-bison](https://sourceforge.net/projects/winflexbison/), and CMake installed
* Follow these steps from the source directory
```
mkdir build
cd build
cmake -G "Visual Studio 15 2017 Win64" ..
cmake --build . --config Debug
```


