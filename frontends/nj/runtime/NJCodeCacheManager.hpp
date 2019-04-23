/*******************************************************************************
 * Copyright (c) 2000, 2016 IBM Corp. and others
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

#ifndef NJ_CODECACHEMANAGER_INCL
#define NJ_CODECACHEMANAGER_INCL

#ifndef NJ_CODECACHEMANAGER_COMPOSED
#define NJ_CODECACHEMANAGER_COMPOSED
namespace NJCompiler { class CodeCacheManager; }
namespace NJCompiler { typedef CodeCacheManager CodeCacheManagerConnector; }
#endif

#include <stddef.h>
#include <stdint.h>
#include "runtime/OMRCodeCacheManager.hpp"

namespace TR { class CodeCacheMemorySegment; }
namespace TR { class CodeCache; }
namespace TR { class CodeCacheManager; }

namespace NJCompiler
{

class JitConfig;
class FrontEnd;

class OMR_EXTENSIBLE CodeCacheManager : public OMR::CodeCacheManagerConnector
   {
   TR::CodeCacheManager *self();

public:
   CodeCacheManager(TR::RawAllocator rawAllocator);

   void *operator new(size_t s, TR::CodeCacheManager *m) { return m; }

   static TR::CodeCacheManager *instance()
      {
      TR_ASSERT_FATAL(_codeCacheManager, "CodeCacheManager not yet instantiated");
      return _codeCacheManager;
      }

   TR::CodeCacheMemorySegment *allocateCodeCacheSegment(size_t segmentSize,
                                                        size_t &codeCacheSizeToAllocate,
                                                        void *preferredStartAddress);

   /**
    * @brief Override of OMR::freeCodeCacheSegment that actually frees memory.
    */
   void freeCodeCacheSegment(TR::CodeCacheMemorySegment * memSegment);

private :
   static TR::CodeCacheManager *_codeCacheManager;
   };

} // namespace NJCompiler

#endif // NJ_CODECACHEMANAGER_INCL
