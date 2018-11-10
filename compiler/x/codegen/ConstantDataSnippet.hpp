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

#ifndef X86CONSTANTDATASNIPPET_INCL
#define X86CONSTANTDATASNIPPET_INCL

#include "x/codegen/DataSnippet.hpp"

#include <stdint.h>                   // for uint8_t
#include "codegen/Snippet.hpp"        // for TR::X86Snippet::Kind, etc

namespace TR { class CodeGenerator; }
namespace TR { class Node; }

namespace TR {

class X86ConstantDataSnippet : public TR::X86DataSnippet
   {
   public:

   inline X86ConstantDataSnippet(TR::CodeGenerator *cg, TR::Node *n, void *c, size_t size) : TR::X86DataSnippet(cg, n, c, size) { }
   virtual Kind getKind() { return IsConstantData; }
   };

}

#endif
