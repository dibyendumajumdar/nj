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

#ifndef OMR_Z_CODEGEN_PHASE
#define OMR_Z_CODEGEN_PHASE

/*
 * The following #define and typedef must appear before any #includes in this file
 */

#ifndef OMR_CODEGEN_PHASE_CONNECTOR
#define OMR_CODEGEN_PHASE_CONNECTOR
namespace OMR { namespace Z { class CodeGenPhase; } }
namespace OMR { typedef OMR::Z::CodeGenPhase CodeGenPhaseConnector; }
#else
#error OMR::Z::CodeGenPhase expected to be a primary connector, but a OMR connector is already defined
#endif

#include "compiler/codegen/OMRCodeGenPhase.hpp"

namespace OMR
{

namespace Z
{

class OMR_EXTENSIBLE CodeGenPhase : public OMR::CodeGenPhase
   {
   protected:

   CodeGenPhase(TR::CodeGenerator *cg): OMR::CodeGenPhase(cg) {}

   public:
   static void performMarkLoadAsZeroOrSignExtensionPhase(TR::CodeGenerator * cg, TR::CodeGenPhase *);
   static void performSetBranchOnCountFlagPhase(TR::CodeGenerator * cg, TR::CodeGenPhase *);
   static void performPreRAPeepholePhase(TR::CodeGenerator * cg, TR::CodeGenPhase *);
   // override base class implementation so that tracelog includes Post RA in phase title
   static void performPeepholePhase(TR::CodeGenerator * cg, TR::CodeGenPhase *);

   // override base class implementation because new phases are being added
   static int getNumPhases();
   const char * getName();
   static const char* getName(PhaseValue phase);
   };
}

}

#endif
