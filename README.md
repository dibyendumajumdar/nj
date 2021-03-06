# nj

This is a leaner version of the [Eclipse OMR](https://github.com/eclipse/omr) project focussing just on the compiler / JIT backend. 

Here is what is included:

* **`compiler`**:       Components for building compiler technology, such as JIT
                        compilers.
* **`jitbuilder`**:     An easy to use high level abstraction on top of the
                        compiler technology.
* **`tool`**:           Code generation tools for the build system (only required parts)
* **`fvtest`**:         Tests for the compiler/JitBuilder (includes tril)

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
* Some tests fail on Windows 10
* A [C API](https://github.com/dibyendumajumdar/nj/blob/master/frontends/nj/ilgen/nj_api.h) is implemented to allow easy use of the JIT engine in projects; note this is a low level api, not the same as the JITBuilder api provided by Eclipse OMR. A user guide for this api will be available at a future date.
* An easy way to get started is to use the C front end [dmr_C](https://github.com/dibyendumajumdar/dmr_c), which enables you to generate code from C code taking away a lot of the complexities of working with the JIT api.
* Or you can use the JITBuilder api from OMR which is included and installed by default.

## Roadmap

* Immediate focus is on trying to use the JIT engine behind a C front end [dmr_C](https://github.com/dibyendumajumdar/dmr_c) and with the help of that, as a backend JIT engine for [Ravi, a Lua 5.3 derived language](https://github.com/dibyendumajumdar/ravi).
* I would like to further trim the library to make it even leaner and focussed - especially enable a build option that is minimal in size

## Known Issues
* The OMR technology originated for Java. In Java, stack values cannot be referenced through pointers. Hence stack values cannot be modified indirectly via pointers, nor can they be aliased by function calls. OMR's optimizer (mainly the alias analysis framework in the optimizer) does not handle these scenarios, hence resulting in incorrect optimization. Therefore, OMR at present cannot be successfully used as the JIT for a language such as C. The approach I am taking in dmr_C for now is to not allow constructs that are not supported.

## Merge Strategy

I have [submitted a pull request](https://github.com/eclipse/omr/pull/3606) to Eclipse OMR for the C api as it stands. However that  does not do away with the need to maintain this project as I disagree with one aspect of the Eclipse OMR strategy which is to make the compiler depend on the port and thread libraries. 

Initially I wanted to merge the commits from Eclipse OMR using git's `am` facility. But I got hopelessly lost and some of the
merges did not succeed. So sadly I am taking the easy way and am doing periodic manual merges from the OMR tree using BeyondCompare tool. The downside is that the upstream commit history is not maintained.

## Build Instructions

You can build the libraries and install the header files plus library. On Windows a static library is built. On Mac OSX and Linux
a shared library will be built (JITBuilder library will be static however).

Note - the only builds I have done so far as X86-64 versions. I have no means of testing other architectures.

**Note that due to the way the OMR code is structured the code is compiled several times, once for each target. You can reduce the time taken to build by commenting out some of the targets you do not require in the top-level CMakeLists.txt file.**

### Linux or Mac OSX

* You will need CMake, Perl, Python, bison and flex installed
* I follow steps below from the location of source directory.

```
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=$HOME/Software/nj -DCMAKE_BUILD_TYPE=MinSizeRel ..
make install
```

### Windows 10

* You will need Visual Studio 2017, Python, Perl, [Win flex-bison](https://sourceforge.net/projects/winflexbison/), and CMake installed
* Follow these steps from the source directory
```
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/Software/nj -G "Visual Studio 15 2017 Win64" ..
cmake --build . --target INSTALL --config MinSizeRel
```



