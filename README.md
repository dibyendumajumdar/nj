# nj

This is a cut-down version of IBM's [Eclipse OMR](https://github.com/eclipse/omr) project focussing just on the compiler / JIT backend. 

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
* A [C API](https://github.com/dibyendumajumdar/nj/blob/master/jitbuilder/c-api/nj_api.h) is implemented to allow easy use of the JIT engine in projects; note this is a low level api, not the same as the JitBuilder api provided by Eclipse OMR. A user guide for this api will be available at a future date.
* An easy way to get started is to use the C front end [dmr_C](https://github.com/dibyendumajumdar/dmr_c), which enables you to generate code from C code taking away a lot of the complexities of working with the JIT api.

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
cmake -DCMAKE_INSTALL_PREFIX=/Software/omr -G "Visual Studio 15 2017 Win64" ..
cmake --build . --target INSTALL --config Debug
```


