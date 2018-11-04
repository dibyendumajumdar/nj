/*******************************************************************************
 * Copyright (c) 2018, 2018 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#ifndef MethodBuilder_INCL
#define MethodBuilder_INCL

#include "IlBuilder.hpp"

namespace OMR {
namespace JitBuilder {

// forward declarations for all API classes
class BytecodeBuilder;
class IlBuilder;
class MethodBuilder;
class IlType;
class IlValue;
class ThunkBuilder;
class TypeDictionary;
class VirtualMachineOperandArray;
class VirtualMachineOperandStack;
class VirtualMachineRegister;
class VirtualMachineRegisterInStruct;
class VirtualMachineState;

class MethodBuilder: public IlBuilder {
public:
public: MethodBuilder(TypeDictionary * dict);
public: MethodBuilder(TypeDictionary * dict, VirtualMachineState * vmState);
public: MethodBuilder(MethodBuilder * callerMB);
public: MethodBuilder(MethodBuilder * callerMB, VirtualMachineState * vmState);
public: explicit MethodBuilder(void * impl);
protected: void initializeFromImpl(void * impl);
public: ~MethodBuilder();
public: virtual bool RequestFunction(const char * name);
public: void AllLocalsHaveBeenDefined();
public: void AppendBuilder(IlBuilder * b);
public: void AppendBuilder(BytecodeBuilder * b);
public: void AppendBytecodeBuilder(BytecodeBuilder * b);
public: void DefineFile(const char * fileName);
public: void DefineLine(const char * line);
public: void DefineLine(int32_t line);
public: void DefineName(const char * name);
public: void DefineParameter(const char * name, IlType * type);
public: void DefineArrayParameter(const char * name, IlType * type);
public: void DefineReturnType(IlType * type);
public: void DefineLocal(const char * name, IlType * type);
public: void DefineMemory(const char * name, IlType * type, void * location);
public: void DefineFunction(const char * name, const char * fileName, const char * lineNumber, void * entryPoint, IlType * returnType, int32_t numParms, IlType ** parmTypes);
public: void DefineFunction(const char * name, const char * fileName, const char * lineNumber, void * entryPoint, IlType * returnType, int32_t numParms, ...);
public: const char * GetMethodName();
public: int32_t GetNextBytecodeFromWorklist();
public: void setVMState(VirtualMachineState * vmState);
};

extern "C" void * allocateMethodBuilder(void * impl);

} // JitBuilder
} // OMR

#endif // MethodBuilder_INCL
