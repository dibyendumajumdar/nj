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

#ifndef TR_MEMORYREFERENCE_INCL
#define TR_MEMORYREFERENCE_INCL

#include "codegen/OMRMemoryReference.hpp"

#include <stdint.h>

namespace TR { class SymbolReference; }
namespace TR { class CodeGenerator; }
namespace TR { class Node; }
namespace TR { class Register; }

namespace TR
{

class OMR_EXTENSIBLE MemoryReference : public OMR::MemoryReferenceConnector
   {
   public:

   /**
    * @brief Constructor
    * @param[in] cg : CodeGenerator object
    */
   MemoryReference(TR::CodeGenerator *cg) :
      OMR::MemoryReferenceConnector(cg) {}

   /**
    * @brief Constructor
    * @param[in] br : base register
    * @param[in] ir : index register
    * @param[in] cg : CodeGenerator object
    */
   MemoryReference(TR::Register *br,
      TR::Register *ir,
      TR::CodeGenerator *cg) :
         OMR::MemoryReferenceConnector(br, ir, cg) {}

   /**
    * @brief Constructor
    * @param[in] br : base register
    * @param[in] ir : index register
    * @param[in] scale : scale of index
    * @param[in] cg : CodeGenerator object
    */
   MemoryReference(
      TR::Register *br,
      TR::Register *ir,
      uint8_t scale,
      TR::CodeGenerator *cg) :
         OMR::MemoryReferenceConnector(br, ir, scale, cg) {}

   /**
    * @brief Constructor
    * @param[in] br : base register
    * @param[in] disp : displacement
    * @param[in] cg : CodeGenerator object
    */
   MemoryReference(TR::Register *br,
      int32_t disp,
      TR::CodeGenerator *cg) :
         OMR::MemoryReferenceConnector(br, disp, cg) {}

   /**
    * @brief Constructor
    * @param[in] node : load or store node
    * @param[in] len : length
    * @param[in] cg : CodeGenerator object
    */
   MemoryReference(TR::Node *node,
      uint32_t len,
      TR::CodeGenerator *cg) :
         OMR::MemoryReferenceConnector(node, len, cg) {}

   /**
    * @brief Constructor
    * @param[in] node : node
    * @param[in] symRef : symbol reference
    * @param[in] len : length
    * @param[in] cg : CodeGenerator object
    */
   MemoryReference(TR::Node *node,
      TR::SymbolReference *symRef,
      uint32_t len,
      TR::CodeGenerator *cg) :
         OMR::MemoryReferenceConnector(node, symRef, len, cg) {}
   };
} // TR

#endif
