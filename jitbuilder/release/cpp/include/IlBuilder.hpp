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

#ifndef IlBuilder_INCL
#define IlBuilder_INCL


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

class IlBuilder {
public:
class JBCase {
public:
public: void* _impl;
public: JBCase(int32_t caseValue, IlBuilder * caseBuilder, int32_t caseFallsThrough);
public: explicit JBCase(void * impl);
protected: void initializeFromImpl(void * impl);
public: ~JBCase();
};
class JBCondition {
public:
public: void* _impl;
public: JBCondition(IlBuilder * conditionBuilder, IlValue * conditionValue);
public: explicit JBCondition(void * impl);
protected: void initializeFromImpl(void * impl);
public: ~JBCondition();
};
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
public: void* _impl;
public: explicit IlBuilder(void * impl);
protected: void initializeFromImpl(void * impl);
public: ~IlBuilder();
public: virtual bool buildIL();
public: IlBuilder * OrphanBuilder();
public: BytecodeBuilder * OrphanBytecodeBuilder(int32_t bcIndex, char * name);
public: IlValue * Copy(IlValue * value);
public: TypeDictionary * typeDictionary();
public: IlValue * ConstInteger(IlType * type, int64_t value);
public: IlValue * ConstAddress(void * value);
public: IlValue * ConstDouble(double value);
public: IlValue * ConstFloat(float value);
public: IlValue * ConstInt8(int8_t value);
public: IlValue * ConstInt16(int16_t value);
public: IlValue * ConstInt32(int32_t value);
public: IlValue * ConstInt64(int64_t value);
public: IlValue * ConstString(char * value);
public: IlValue * Const(void * value);
public: IlValue * Const(double value);
public: IlValue * Const(float value);
public: IlValue * Const(int8_t value);
public: IlValue * Const(int16_t value);
public: IlValue * Const(int32_t value);
public: IlValue * Const(int64_t value);
public: IlValue * NullAddress();
public: IlValue * Add(IlValue * left, IlValue * right);
public: IlValue * AddWithOverflow(IlBuilder ** overflowHandler, IlValue * left, IlValue * right);
public: IlValue * AddWithUnsignedOverflow(IlBuilder ** overflowHandler, IlValue * left, IlValue * right);
public: IlValue * And(IlValue * left, IlValue * right);
public: IlValue * Div(IlValue * left, IlValue * right);
public: IlValue * IndexAt(IlType * dt, IlValue * base, IlValue * index);
public: IlValue * Mul(IlValue * left, IlValue * right);
public: IlValue * MulWithOverflow(IlBuilder ** overflowHandler, IlValue * left, IlValue * right);
public: IlValue * Negate(IlValue * v);
public: IlValue * Or(IlValue * left, IlValue * right);
public: IlValue * Rem(IlValue * left, IlValue * right);
public: IlValue * ShiftL(IlValue * left, IlValue * right);
public: IlValue * ShiftR(IlValue * left, IlValue * right);
public: IlValue * Sub(IlValue * left, IlValue * right);
public: IlValue * SubWithOverflow(IlBuilder ** overflowHandler, IlValue * left, IlValue * right);
public: IlValue * SubWithUnsignedOverflow(IlBuilder ** overflowHandler, IlValue * left, IlValue * right);
public: IlValue * UnsignedShiftR(IlValue * left, IlValue * right);
public: IlValue * Xor(IlValue * left, IlValue * right);
public: IlValue * EqualTo(IlValue * left, IlValue * right);
public: IlValue * LessOrEqualTo(IlValue * left, IlValue * right);
public: IlValue * LessThan(IlValue * left, IlValue * right);
public: IlValue * GreaterOrEqualTo(IlValue * left, IlValue * right);
public: IlValue * GreaterThan(IlValue * left, IlValue * right);
public: IlValue * NotEqualTo(IlValue * left, IlValue * right);
public: IlValue * UnsignedLessOrEqualTo(IlValue * left, IlValue * right);
public: IlValue * UnsignedLessThan(IlValue * left, IlValue * right);
public: IlValue * UnsignedGreaterOrEqualTo(IlValue * left, IlValue * right);
public: IlValue * UnsignedGreaterThan(IlValue * left, IlValue * right);
public: IlValue * ConvertBitsTo(IlType * type, IlValue * value);
public: IlValue * ConvertTo(IlType * type, IlValue * value);
public: IlValue * UnsignedConvertTo(IlType * type, IlValue * value);
public: IlValue * AtomicAdd(IlValue * baseAddress, IlValue * value);
public: IlValue * CreateLocalArray(int32_t numElements, IlType * elementType);
public: IlValue * CreateLocalStruct(IlType * structType);
public: IlValue * Load(const char * name);
public: IlValue * LoadAt(IlType * type, IlValue * address);
public: IlValue * LoadIndirect(const char * type, const char * field, IlValue * object);
public: void Store(const char * name, IlValue * value);
public: void StoreAt(IlValue * address, IlValue * value);
public: void StoreIndirect(const char * type, const char * field, IlValue * object, IlValue * value);
public: void StoreOver(IlValue * dest, IlValue * value);
public: void Transaction(IlBuilder ** persistentFailureBuilder, IlBuilder ** transientFailureBuilder, IlBuilder ** transactionBuilder);
public: void TransactionAbort();
public: IlValue * StructFieldInstanceAddress(const char * structName, const char * fieldName, IlValue * obj);
public: IlValue * UnionFieldInstanceAddress(const char * unionName, const char * fieldName, IlValue * obj);
public: IlValue * VectorLoad(char * name);
public: IlValue * VectorLoadAt(IlType * type, IlValue * address);
public: void VectorStore(char * name, IlValue * value);
public: void VectorStoreAt(IlValue * address, IlValue * value);
public: void AppendBuilder(IlBuilder * b);
public: IlValue * Call(const char * name, int32_t numArgs, IlValue ** arguments);
public: IlValue * Call(const char * name, int32_t numArgs, ...);
public: IlValue * Call(MethodBuilder * name, int32_t numArgs, IlValue ** arguments);
public: IlValue * Call(MethodBuilder * name, int32_t numArgs, ...);
public: IlValue * ComputedCall(char * name, int32_t numArgs, IlValue ** arguments);
public: IlValue * ComputedCall(char * name, int32_t numArgs, ...);
public: void DoWhileLoop(char * exitCondition, IlBuilder ** body);
public: void DoWhileLoop(char * exitCondition, IlBuilder ** body, IlBuilder ** breakBuilder, IlBuilder ** continueBuilder);
public: void DoWhileLoopWithBreak(char * exitCondition, IlBuilder ** body, IlBuilder ** breakBuilder);
public: void DoWhileLoopWithContinue(char * exitCondition, IlBuilder ** body, IlBuilder ** continueBuilder);
public: void Goto(IlBuilder * b);
public: void Goto(IlBuilder ** b);
public: IlBuilder::JBCondition * MakeCondition(IlBuilder * conditionBuilder, IlValue * conditionValue);
public: void IfAnd(IlBuilder ** allTrueBuilder, IlBuilder ** anyFalseBuilder, int32_t numTerms, IlBuilder::JBCondition ** terms);
public: void IfAnd(IlBuilder ** allTrueBuilder, IlBuilder ** anyFalseBuilder, int32_t numTerms, ...);
public: void IfCmpEqual(IlBuilder * target, IlValue * left, IlValue * right);
public: void IfCmpEqual(IlBuilder ** target, IlValue * left, IlValue * right);
public: void IfCmpLessOrEqual(IlBuilder * target, IlValue * left, IlValue * right);
public: void IfCmpLessOrEqual(IlBuilder ** target, IlValue * left, IlValue * right);
public: void IfCmpLessThan(IlBuilder * target, IlValue * left, IlValue * right);
public: void IfCmpLessThan(IlBuilder ** target, IlValue * left, IlValue * right);
public: void IfCmpGreaterOrEqual(IlBuilder * target, IlValue * left, IlValue * right);
public: void IfCmpGreaterOrEqual(IlBuilder ** target, IlValue * left, IlValue * right);
public: void IfCmpGreaterThan(IlBuilder * target, IlValue * left, IlValue * right);
public: void IfCmpGreaterThan(IlBuilder ** target, IlValue * left, IlValue * right);
public: void IfCmpNotEqual(IlBuilder * target, IlValue * left, IlValue * right);
public: void IfCmpNotEqual(IlBuilder ** target, IlValue * left, IlValue * right);
public: void IfCmpUnsignedLessOrEqual(IlBuilder * target, IlValue * left, IlValue * right);
public: void IfCmpUnsignedLessOrEqual(IlBuilder ** target, IlValue * left, IlValue * right);
public: void IfCmpUnsignedLessThan(IlBuilder * target, IlValue * left, IlValue * right);
public: void IfCmpUnsignedLessThan(IlBuilder ** target, IlValue * left, IlValue * right);
public: void IfCmpUnsignedGreaterOrEqual(IlBuilder * target, IlValue * left, IlValue * right);
public: void IfCmpUnsignedGreaterOrEqual(IlBuilder ** target, IlValue * left, IlValue * right);
public: void IfCmpUnsignedGreaterThan(IlBuilder * target, IlValue * left, IlValue * right);
public: void IfCmpUnsignedGreaterThan(IlBuilder ** target, IlValue * left, IlValue * right);
public: void IfCmpEqualZero(IlBuilder * target, IlValue * condition);
public: void IfCmpEqualZero(IlBuilder ** target, IlValue * condition);
public: void IfCmpNotEqualZero(IlBuilder * target, IlValue * condition);
public: void IfCmpNotEqualZero(IlBuilder ** target, IlValue * condition);
public: void IfOr(IlBuilder ** anyTrueBuilder, IlBuilder ** allFalseBuilder, int32_t numTerms, IlBuilder::JBCondition ** terms);
public: void IfOr(IlBuilder ** anyTrueBuilder, IlBuilder ** allFalseBuilder, int32_t numTerms, ...);
public: void IfThen(IlBuilder ** thenPath, IlValue * condition);
public: void IfThenElse(IlBuilder ** thenPath, IlBuilder ** elsePath, IlValue * condition);
public: void ForLoop(bool countsUp, char * indVar, IlBuilder ** body, IlBuilder ** breakBuilder, IlBuilder ** continueBuilder, IlValue * initial, IlValue * iterateWhile, IlValue * increment);
public: void ForLoopDown(char * indVar, IlBuilder ** body, IlValue * initial, IlValue * iterateWhile, IlValue * increment);
public: void ForLoopUp(char * indVar, IlBuilder ** body, IlValue * initial, IlValue * iterateWhile, IlValue * increment);
public: void ForLoopWithBreak(bool countsUp, char * indVar, IlBuilder ** body, IlBuilder ** breakBuilder, IlValue * initial, IlValue * iterateWhile, IlValue * increment);
public: void ForLoopWithContinue(bool countsUp, char * indVar, IlBuilder ** body, IlBuilder ** continueBuilder, IlValue * initial, IlValue * iterateWhile, IlValue * increment);
public: void Return();
public: void Return(IlValue * value);
public: void Switch(const char * selectionVar, IlBuilder ** defaultBuilder, int32_t numCases, IlBuilder::JBCase ** cases);
public: void Switch(const char * selectionVar, IlBuilder ** defaultBuilder, int32_t numCases, ...);
public: IlBuilder::JBCase * MakeCase(int32_t value, IlBuilder ** builder, int32_t fallsThrough);
public: void WhileDoLoop(char * exitCondition, IlBuilder ** body);
public: void WhileDoLoopWithBreak(char * exitCondition, IlBuilder ** body, IlBuilder ** breakBuilder);
public: void WhileDoLoopWithContinue(char * exitCondition, IlBuilder ** body, IlBuilder ** continueBuilder);
};

extern "C" void * allocateIlBuilderJBCase(void * impl);
extern "C" void * allocateIlBuilderJBCondition(void * impl);
extern "C" void * allocateIlBuilder(void * impl);

} // JitBuilder
} // OMR

#endif // IlBuilder_INCL
