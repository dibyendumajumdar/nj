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

#ifndef OMR_TREE_EVALUATOR_INCL
#define OMR_TREE_EVALUATOR_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_TREE_EVALUATOR_CONNECTOR
#define OMR_TREE_EVALUATOR_CONNECTOR
namespace OMR { class TreeEvaluator; }
namespace OMR { typedef OMR::TreeEvaluator TreeEvaluatorConnector; }
#endif

#include <stddef.h>               // for NULL
#include <stdint.h>               // for int32_t, etc
#include "infra/Annotations.hpp"  // for OMR_EXTENSIBLE
#include "il/Node.hpp"            // for vcount_t

class TR_OpaqueClassBlock;
namespace TR { class SymbolReference; }
namespace TR { class CodeGenerator; }
namespace TR { class Register; }

typedef TR::Register *(* TR_TreeEvaluatorFunctionPointer)(TR::Node *node, TR::CodeGenerator *codeGen);

namespace OMR
{

class OMR_EXTENSIBLE TreeEvaluator
   {
   public:
   static bool instanceOfOrCheckCastNeedEqualityTest(TR::Node * castClassNode, TR::CodeGenerator *cg);
   static bool instanceOfOrCheckCastNeedSuperTest(TR::Node * castClassNode, TR::CodeGenerator *cg);

   static TR_GlobalRegisterNumber getHighGlobalRegisterNumberIfAny(TR::Node *node, TR::CodeGenerator *cg); 

   static int32_t classDepth(TR::Node * castClassNode, TR::CodeGenerator * cg);
   static int32_t checkNonNegativePowerOfTwo(int32_t value);
   static int32_t checkNonNegativePowerOfTwo(int64_t value);
   static int32_t checkPositiveOrNegativePowerOfTwo(int32_t value);
   static int32_t checkPositiveOrNegativePowerOfTwo(int64_t value);

   /**
    * @brief unImpOpEvaluator A dummy evaluator for IL opcodes that could be
    * supported by a codegen but that are not implemented
    */
   static TR::Register *unImpOpEvaluator(TR::Node *node, TR::CodeGenerator *cg);

   /**
    * @brief badILOpEvaluator A dummy evaluator for IL opcodes that are not
    * supported by a codegen (e.g. because they are specific to another codegen
    * or are not applicable to a particular project)
    */
   static TR::Register *badILOpEvaluator(TR::Node *node, TR::CodeGenerator *cg);

   static TR::Register *compressedRefsEvaluator(TR::Node *node, TR::CodeGenerator *cg);
   static TR::Register *computeCCEvaluator(TR::Node *node, TR::CodeGenerator *cg);
   struct TR_ArithmeticOverflowCheckNodes { TR::Node *operationNode, *leftChild, *rightChild; };
   static bool nodeIsIArithmeticOverflowCheck(TR::Node *node, TR_ArithmeticOverflowCheckNodes *u);
   static bool nodeIsLArithmeticOverflowCheck(TR::Node *node, TR_ArithmeticOverflowCheckNodes *u);
   static bool nodeIsIAddOverflowCheck(TR::Node *node, TR_ArithmeticOverflowCheckNodes *u);
   static bool nodeIsISubOverflowCheck(TR::Node *node, TR_ArithmeticOverflowCheckNodes *u);
   static bool nodeIsLAddOverflowCheck(TR::Node *node, TR_ArithmeticOverflowCheckNodes *u);
   static bool nodeIsLSubOverflowCheck(TR::Node *node, TR_ArithmeticOverflowCheckNodes *u);
   static void evaluateNodesWithFutureUses(TR::Node *node, TR::CodeGenerator *cg);
   static void initializeStrictlyFutureUseCounts(TR::Node *node, vcount_t visitCount, TR::CodeGenerator *cg);

   protected:
   static bool isStaticClassSymRef(TR::SymbolReference * symRef);

   };

}

#endif
