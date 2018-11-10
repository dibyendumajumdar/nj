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

#include <stddef.h>                   // for NULL
#include <stdint.h>                   // for int32_t, int64_t, uint32_t
#include <string.h>
#include "env/CompilerEnv.hpp"
#include "env/jittypes.h"             // for uintptrj_t, intptrj_t
#include "infra/Assert.hpp"           // for TR_ASSERT
#include "compile/Compilation.hpp"    // for Compilation

#define notImplemented(A) TR_ASSERT(0, "OMR::ClassEnv::%s is undefined", (A) )

char *
OMR::ClassEnv::classNameChars(TR::Compilation *comp, TR::SymbolReference *symRef, int32_t & len)
   {
   char *name = "<no class name>";
   len = strlen(name);
   return name;
   }

uintptrj_t
OMR::ClassEnv::getArrayElementWidthInBytes(TR::Compilation *comp, TR_OpaqueClassBlock* arrayClass)
   {
   notImplemented("getArrayElementWidthInBytes");
   return 0;
   }

intptrj_t
OMR::ClassEnv::getVFTEntry(TR::Compilation *comp, TR_OpaqueClassBlock* clazz, int32_t offset)
   {
   return *(intptrj_t*) (((uint8_t *)clazz) + offset);
   }

bool
OMR::ClassEnv::classUnloadAssumptionNeedsRelocation(TR::Compilation *comp)
   {
   return comp->compileRelocatableCode();
   }
