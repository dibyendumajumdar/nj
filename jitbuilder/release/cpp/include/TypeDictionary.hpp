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

#ifndef TypeDictionary_INCL
#define TypeDictionary_INCL

#include "TypeDictionaryExtrasOutsideClass.hpp"

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

class TypeDictionary {
public:
public: IlType * NoType;
public: IlType * Int8;
public: IlType * Int16;
public: IlType * Int32;
public: IlType * Int64;
public: IlType * Float;
public: IlType * Double;
public: IlType * Address;
public: IlType * VectorInt8;
public: IlType * VectorInt16;
public: IlType * VectorInt32;
public: IlType * VectorInt64;
public: IlType * VectorFloat;
public: IlType * VectorDouble;
public: IlType * Word;
public: IlType * pNoType;
public: IlType * pInt8;
public: IlType * pInt16;
public: IlType * pInt32;
public: IlType * pInt64;
public: IlType * pFloat;
public: IlType * pDouble;
public: IlType * pAddress;
public: IlType * pVectorInt8;
public: IlType * pVectorInt16;
public: IlType * pVectorInt32;
public: IlType * pVectorInt64;
public: IlType * pVectorFloat;
public: IlType * pVectorDouble;
public: IlType * pWord;
public: void* _impl;
public: TypeDictionary();
public: explicit TypeDictionary(void * impl);
protected: void initializeFromImpl(void * impl);
public: ~TypeDictionary();
public: void CloseStruct(const char * structName);
public: void CloseStruct(const char * structName, size_t size);
public: void CloseUnion(const char * unionName);
public: void DefineField(const char * structName, const char * fieldName, IlType * type);
public: void DefineField(const char * structName, const char * fieldName, IlType * type, size_t offset);
public: IlType * DefineStruct(const char * structName);
public: IlType * DefineUnion(const char * unionName);
public: IlType * GetFieldType(const char * structName, const char * fieldName);
public: IlType * LookupStruct(const char * structName);
public: IlType * LookupUnion(const char * unionName);
public: size_t OffsetOf(const char * structName, const char * fieldName);
public: IlType * PointerTo(IlType * baseType);
public: IlType * PointerTo(const char * structName);
public: void UnionField(const char * unionName, const char * fieldName, IlType * fieldType);
public: IlType * UnionFieldType(const char * unionName, const char * fieldName);
#include "TypeDictionaryExtrasInsideClass.hpp"
};

extern "C" void * allocateTypeDictionary(void * impl);

} // JitBuilder
} // OMR

#endif // TypeDictionary_INCL
