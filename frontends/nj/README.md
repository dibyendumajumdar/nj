# NJ C Api Front-end

## Goals

* Create a minimal front-end extension mechanism
* Create a C api so that we can work with OMR without having to make the front-end part of the project itself
* A secondary goal is to try and remove as much code as possible from the library to reduce the size of the
  library

## Rationale

The OMR compiler backend is not available as a separate library. The OMR code relies upon static 
polymorphism - which means that a front-end needs to supply code that is then used by the backend and this is
done at compile time. The way this works is described [here](https://github.com/eclipse/omr/blob/master/doc/compiler/extensible_classes/Extensible_Classes.md) and [here](https://github.com/eclipse/omr/blob/master/doc/compiler/extensible_classes/Extensible_Classes_in_OMR.md).

This approach has the problem that the front-end and back-end code must be kind of compiled together as 
a unit. The OMR project has a JitBuilder project that attempts to abstract away some of this so that clients
can use the compiler as a library. However the JitBuilder api is somewhat higher level and has a lot of scaffolding that you may not want. It would be nice to be able to expose the lower level IL api as well.

### `TR::CodeGenerator`

Following is from OMR presentation:

* The `CodeGenerator` is overall orchestrator for the translation of IL trees to machine code
* `OMR::CodeGenerator` contains architecture neutral functionality
* The `CodeGenerator` drives intruction selection, register assignment and binary encoding

The backend references this via `TR::CodeGenerator`. A simple front-end can ensure that this simply extends a platform specific implementation. The hierarchy goes like this; `TR::CodeGenerator` extends `NJCompiler::CodeGenerator` which extends `OMR::X86::I386::CodeGenerator` which extends `OMR::CodeGenerator` I think! 

A simple front-end need not add any functionality to this component.


