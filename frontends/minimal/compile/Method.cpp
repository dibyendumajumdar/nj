/*******************************************************************************
 * Copyright (c) 2000, 2018 IBM Corp. and others
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

#include "compile/Method.hpp"
#include "compile/Compilation.hpp"
#include "compile/SymbolReferenceTable.hpp"
#include "il/SymbolReference.hpp"
#include "il/symbol/ParameterSymbol.hpp"
#include "ilgen/NJIlGenerator.hpp"
#include "ilgen/IlGeneratorMethodDetails_inlines.hpp"

namespace NJCompiler
{

	const char *signatureNameForType[TR::NumOMRTypes] =
	{
		"V",  // NoType
		"B",  // Int8
		"C",  // Int16
		"I",  // Int32
		"J",  // Int64
		"F",  // Float
		"D",  // Double
		"L",  // Address
		"V1", // VectorInt8
		"V2", // VectorInt16
		"V4", // VectorInt32
		"V8", // VectorInt64
		"VF", // VectorFloat
		"VD", // VectorDouble
		"AG", // Aggregate??? TODO this was not present in JitBuilder
	};

// needs major overhaul
ResolvedMethod::ResolvedMethod(TR_OpaqueMethodBlock *method)
   {
	_ilInjector = reinterpret_cast<TR::IlGenerator *>(method);

	TR::ResolvedMethod * resolvedMethod = (TR::ResolvedMethod *)_ilInjector->methodSymbol()->getResolvedMethod();
	//_fileName = resolvedMethod->classNameChars();
	snprintf(_name, sizeof _name, "%s", resolvedMethod->nameChars());
	_numParms = resolvedMethod->getNumArgs();
	_parmTypes = resolvedMethod->_parmTypes;
	//_lineNumber = resolvedMethod->getLineNumber();
	_returnType = resolvedMethod->returnType();
	snprintf(_signature, sizeof _signature, "%s", resolvedMethod->getSignature());
	_externalName[0] = 0;
	_entryPoint = resolvedMethod->getEntryPoint();
	strncpy(_signatureChars, resolvedMethod->signatureChars(), 62); // TODO: introduce concept of robustness
   }

ResolvedMethod::ResolvedMethod(const char *fileName, const char *lineNumber, const char *name,
	int32_t numParms, TR::DataType *parmTypes,
	TR::DataType returnType, void *entryPoint,
	TR::IlGenerator *ilInjector) {
	//_fileName = fileName;
	snprintf(_name, sizeof _name, "%s", name);
	_numParms = numParms;
	for (int i = 0; i < numParms; i++)
		_parmTypes.push_back(parmTypes[i]);
	//_lineNumber = lineNumber;
	_returnType = returnType;
	_signature[0] = 0;
	_externalName[0] = 0;
	_entryPoint = entryPoint;
	_ilInjector = ilInjector;
	computeSignatureChars(); // TODO: introduce concept of robustness
}


const char *
ResolvedMethod::signature(TR_Memory * trMemory, TR_AllocationKind allocKind)
   {
	if (!_signature[0])
	{
		snprintf(_signature, sizeof _signature, "%s:%s:%s", ""/*_fileName*/, ""/*_lineNumber*/, _name);
	}
	return _signature;
}

TR::DataType
ResolvedMethod::parmType(uint32_t slot)
   {
   TR_ASSERT((slot < _numParms), "Invalid slot provided for Parameter Type");
   return TR::DataTypes::NoType;
   }

void
ResolvedMethod::computeSignatureChars()
{
	const char *name = NULL;
	uint32_t len = 3;
	for (int32_t p = 0; p < _numParms; p++)
	{
		TR::DataType type = _parmTypes[p];
		len += strlen(signatureNameForType[type.getDataType()]);
	}
	len += strlen(signatureNameForType[_returnType.getDataType()]);
	TR_ASSERT(len < 64, "signature array may not be large enough"); // TODO: robustness

	int32_t s = 0;
	_signatureChars[s++] = '(';
	for (int32_t p = 0; p < _numParms; p++)
	{
		name = signatureNameForType[_parmTypes[p].getDataType()];
		len = strlen(name);
		strncpy(_signatureChars + s, name, len);
		s += len;
	}
	_signatureChars[s++] = ')';
	name = signatureNameForType[_returnType.getDataType()];
	len = strlen(name);
	strncpy(_signatureChars + s, name, len);
	s += len;
	_signatureChars[s++] = 0;
}

void
ResolvedMethod::makeParameterList(TR::ResolvedMethodSymbol *methodSym)
   {
	ListAppender<TR::ParameterSymbol> la(&methodSym->getParameterList());
	TR::ParameterSymbol *parmSymbol;
	int32_t slot = 0;
	int32_t ordinal = 0;

	uint32_t parmSlots = numberOfParameterSlots();
	for (int32_t parmIndex = 0; parmIndex < parmSlots; ++parmIndex)
	{
		TR::DataType dt = _parmTypes[parmIndex];
		int32_t size = methodSym->convertTypeToSize(dt);

		parmSymbol = methodSym->comp()->getSymRefTab()->createParameterSymbol(methodSym, slot, dt);
		parmSymbol->setOrdinal(ordinal++);

		const char *s = signatureNameForType[dt.getDataType()];
		uint32_t len = strlen(s);
		parmSymbol->setTypeSignature(s, len);

		la.add(parmSymbol);

		++slot;
	}

	int32_t lastInterpreterSlot = slot + numberOfTemps();
	methodSym->setTempIndex(lastInterpreterSlot, methodSym->comp()->fe());
	methodSym->setFirstJitTempIndex(methodSym->getTempIndex());
}

char *
ResolvedMethod::localName(uint32_t slot,
                          uint32_t bcIndex,
                          int32_t &nameLength,
                          TR_Memory *trMemory)
   {
	char *name = (char *)trMemory->allocateHeapMemory(10 * sizeof(char));
	sprintf(name, "Parm %2d", slot);

	nameLength = strlen(name);
	return name;
}

TR::IlGenerator *ResolvedMethod::getInjector(
	TR::IlGeneratorMethodDetails * details,
	TR::ResolvedMethodSymbol *methodSymbol,
	TR::FrontEnd *fe,
	TR::SymbolReferenceTable *symRefTab) {
	_ilInjector->initialize(details, methodSymbol, fe, symRefTab);
	return _ilInjector;
}

const char *
ResolvedMethod::externalName(TR_Memory *trMemory, TR_AllocationKind allocKind)
{
	return _name;
}

} // namespace TestCompiler
