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

/*
 * This file will be included within a table of function pointers.
 * Only valid static methods within CodeGenPhase class should be included.
 */

 // The entries in this file must be kept in sync with compiler/codegen/OMRCodeGenPhaseEnum.hpp
   TR::CodeGenPhase::performReserveCodeCachePhase,                                           //ReserveCodeCachePhase
   TR::CodeGenPhase::performLowerTreesPhase,                                                 //LowerTreesPhase
   TR::CodeGenPhase::performUncommonCallConstNodesPhase,                                     //UncommonCallConstNodesPhase
   TR::CodeGenPhase::performSetupForInstructionSelectionPhase,                               //SetupForInstructionSelectionPhase
   TR::CodeGenPhase::performInstructionSelectionPhase,                                       //InstructionSelectionPhase
   TR::CodeGenPhase::performCreateStackAtlasPhase,                                           //CreateStackAtlasPhase
   TR::CodeGenPhase::performRegisterAssigningPhase,                                          //RegisterAssigningPhase
   TR::CodeGenPhase::performMapStackPhase,                                                   //MapStackPhase
   TR::CodeGenPhase::performPeepholePhase,                                                   //PeepholePhase
   TR::CodeGenPhase::performBinaryEncodingPhase,                                             //BinaryEncodingPhase
   TR::CodeGenPhase::performEmitSnippetsPhase,                                               //EmitSnippetsPhase
   TR::CodeGenPhase::performProcessRelocationsPhase,                                         //ProcessRelocationsPhase
   TR::CodeGenPhase::performFindAndFixCommonedReferencesPhase,                               //FindAndFixCommonedReferencesPhase
   TR::CodeGenPhase::performRemoveUnusedLocalsPhase,                                         //RemoveUnusedLocalsPhase
   TR::CodeGenPhase::performInliningReportPhase,                                             //InliningReportPhase
   TR::CodeGenPhase::performInsertDebugCountersPhase,
   TR::CodeGenPhase::performCleanUpFlagsPhase,
