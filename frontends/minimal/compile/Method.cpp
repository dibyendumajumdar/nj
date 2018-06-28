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

// needs major overhaul
ResolvedMethod::ResolvedMethod(TR_OpaqueMethodBlock *method)
   {
	// trouble! trouble! where do we get TypeDictionary from now?
	_ilInjector = reinterpret_cast<TR::IlGenerator *>(method);

	TR::ResolvedMethod * resolvedMethod = (TR::ResolvedMethod *)_ilInjector->methodSymbol()->getResolvedMethod();
	//_fileName = resolvedMethod->classNameChars();
	_name = resolvedMethod->nameChars();
	_numParms = resolvedMethod->getNumArgs();
	_parmTypes = resolvedMethod->_parmTypes;
	//_lineNumber = resolvedMethod->getLineNumber();
	_returnType = resolvedMethod->returnType();
	//_signature = resolvedMethod->getSignature();
	//_externalName = 0;
	_entryPoint = resolvedMethod->getEntryPoint();
	strncpy(_signatureChars, resolvedMethod->signatureChars(), 62); // TODO: introduce concept of robustness

   }

const char *
ResolvedMethod::signature(TR_Memory * trMemory, TR_AllocationKind allocKind)
   {
   if( !_signature )
      {
	   char * s = ""; // FIXME
      return s;
      }
   else
      return _signature;
   }

TR::DataType
ResolvedMethod::parmType(uint32_t slot)
   {
   TR_ASSERT((slot < _numParms), "Invalid slot provided for Parameter Type");
   return TR::DataTypes::NoType;
   }


void
ResolvedMethod::makeParameterList(TR::ResolvedMethodSymbol *methodSym)
   {
	// FIXME
   }

char *
ResolvedMethod::localName(uint32_t slot,
                          uint32_t bcIndex,
                          int32_t &nameLength,
                          TR_Memory *trMemory)
   {
	return ""; // FIXME
   }

} // namespace TestCompiler
