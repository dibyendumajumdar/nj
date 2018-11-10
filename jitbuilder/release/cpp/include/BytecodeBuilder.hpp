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

#ifndef BytecodeBuilder_INCL
#define BytecodeBuilder_INCL

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

class BytecodeBuilder: public IlBuilder {
public:
public: explicit BytecodeBuilder(void * impl);
protected: void initializeFromImpl(void * impl);
public: ~BytecodeBuilder();
public: int32_t bcIndex();
public: char * name();
public: VirtualMachineState * vmState();
public: void AddFallThroughBuilder(BytecodeBuilder * ftb);
public: void AddSuccessorBuilders(uint32_t numBuilders, BytecodeBuilder ** builders);
public: void AddSuccessorBuilders(uint32_t numBuilders, ...);
public: void AddSuccessorBuilder(BytecodeBuilder ** b);
public: void Goto(BytecodeBuilder * b);
public: void Goto(BytecodeBuilder ** b);
public: void IfCmpEqual(BytecodeBuilder * target, IlValue * left, IlValue * right);
public: void IfCmpEqual(BytecodeBuilder ** target, IlValue * left, IlValue * right);
public: void IfCmpLessOrEqual(BytecodeBuilder * target, IlValue * left, IlValue * right);
public: void IfCmpLessOrEqual(BytecodeBuilder ** target, IlValue * left, IlValue * right);
public: void IfCmpLessThan(BytecodeBuilder * target, IlValue * left, IlValue * right);
public: void IfCmpLessThan(BytecodeBuilder ** target, IlValue * left, IlValue * right);
public: void IfCmpGreaterOrEqual(BytecodeBuilder * target, IlValue * left, IlValue * right);
public: void IfCmpGreaterOrEqual(BytecodeBuilder ** target, IlValue * left, IlValue * right);
public: void IfCmpGreaterThan(BytecodeBuilder * target, IlValue * left, IlValue * right);
public: void IfCmpGreaterThan(BytecodeBuilder ** target, IlValue * left, IlValue * right);
public: void IfCmpNotEqual(BytecodeBuilder * target, IlValue * left, IlValue * right);
public: void IfCmpNotEqual(BytecodeBuilder ** target, IlValue * left, IlValue * right);
public: void IfCmpUnsignedLessOrEqual(BytecodeBuilder * target, IlValue * left, IlValue * right);
public: void IfCmpUnsignedLessOrEqual(BytecodeBuilder ** target, IlValue * left, IlValue * right);
public: void IfCmpUnsignedLessThan(BytecodeBuilder * target, IlValue * left, IlValue * right);
public: void IfCmpUnsignedLessThan(BytecodeBuilder ** target, IlValue * left, IlValue * right);
public: void IfCmpUnsignedGreaterOrEqual(BytecodeBuilder * target, IlValue * left, IlValue * right);
public: void IfCmpUnsignedGreaterOrEqual(BytecodeBuilder ** target, IlValue * left, IlValue * right);
public: void IfCmpUnsignedGreaterThan(BytecodeBuilder * target, IlValue * left, IlValue * right);
public: void IfCmpUnsignedGreaterThan(BytecodeBuilder ** target, IlValue * left, IlValue * right);
public: void IfCmpEqualZero(BytecodeBuilder * target, IlValue * condition);
public: void IfCmpEqualZero(BytecodeBuilder ** target, IlValue * condition);
public: void IfCmpNotEqualZero(BytecodeBuilder * target, IlValue * condition);
public: void IfCmpNotEqualZero(BytecodeBuilder ** target, IlValue * condition);
};

extern "C" void * allocateBytecodeBuilder(void * impl);

} // JitBuilder
} // OMR

#endif // BytecodeBuilder_INCL
