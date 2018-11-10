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

#ifndef VirtualMachineOperandStack_INCL
#define VirtualMachineOperandStack_INCL

#include "VirtualMachineState.hpp"

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

class VirtualMachineOperandStack: public VirtualMachineState {
public:
public: VirtualMachineOperandStack(MethodBuilder * mb, int32_t numOfElements, IlType * elementType, VirtualMachineRegister * arrayBase);
public: explicit VirtualMachineOperandStack(void * impl);
protected: void initializeFromImpl(void * impl);
public: ~VirtualMachineOperandStack();
public: virtual void Commit(IlBuilder * b);
public: virtual void Reload(IlBuilder * b);
public: virtual VirtualMachineState * MakeCopy();
public: virtual void MergeInto(VirtualMachineOperandStack * vmState, IlBuilder * b);
public: void Drop(IlBuilder * b, int32_t depth);
public: void Dup(IlBuilder * b);
public: IlValue * Pick(int32_t depth);
public: IlValue * Pop(IlBuilder * b);
public: void Push(IlBuilder * b, IlValue * value);
public: IlValue * Top();
public: void UpdateStack(IlBuilder * b, IlValue * array);
};

extern "C" void * allocateVirtualMachineOperandStack(void * impl);

} // JitBuilder
} // OMR

#endif // VirtualMachineOperandStack_INCL
