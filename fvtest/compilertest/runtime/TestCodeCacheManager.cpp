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
#if defined(OMR_OS_WINDOWS)
#include <windows.h>
#else
#include <sys/mman.h>
#endif /* OMR_OS_WINDOWS */
#include "runtime/CodeCache.hpp"
#include "runtime/CodeCacheManager.hpp"
#include "runtime/CodeCacheMemorySegment.hpp"
#include "env/FrontEnd.hpp"


// Allocate and initialize a new code cache
// If reservingCompThreadID >= -1, then the new code codecache will be reserved
// A value of -1 for this parameter means that an application thread is requesting the reservation
// A positive value means that a compilation thread is requesting the reservation
// A value of -2 (or less) means that no reservation is requested


TR::CodeCacheManager *TestCompiler::CodeCacheManager::_codeCacheManager = NULL;
TestCompiler::CodeCacheManager::CodeCacheManager(TR::RawAllocator rawAllocator)
   : OMR::CodeCacheManagerConnector(rawAllocator)
   {
   _codeCacheManager = self();
   }

TR::CodeCacheManager *
TestCompiler::CodeCacheManager::self()
   {
   return static_cast<TR::CodeCacheManager *>(this);
   }

TR::CodeCacheMemorySegment *
TestCompiler::CodeCacheManager::allocateCodeCacheSegment(size_t segmentSize,
                                              size_t &codeCacheSizeToAllocate,
                                              void *preferredStartAddress)
   {
   // ignore preferredStartAddress for now, since it's NULL anyway
   //   goal would be to allocate code cache segments near the JIT library address
   codeCacheSizeToAllocate = segmentSize;
   TR::CodeCacheConfig & config = self()->codeCacheConfig();
   if (segmentSize < config.codeCachePadKB() << 10)
      codeCacheSizeToAllocate = config.codeCachePadKB() << 10;

#if defined(OMR_OS_WINDOWS)
   auto memorySlab = reinterpret_cast<uint8_t *>(
         VirtualAlloc(NULL,
            codeCacheSizeToAllocate,
            MEM_COMMIT,
            PAGE_EXECUTE_READWRITE));
#else
   auto memorySlab = reinterpret_cast<uint8_t *>(
         mmap(NULL,
              codeCacheSizeToAllocate,
              PROT_READ | PROT_WRITE | PROT_EXEC,
              MAP_ANONYMOUS | MAP_PRIVATE,
              0,
              0));
#endif /* OMR_OS_WINDOWS */
   TR::CodeCacheMemorySegment *memSegment = (TR::CodeCacheMemorySegment *) ((size_t)memorySlab + codeCacheSizeToAllocate - sizeof(TR::CodeCacheMemorySegment));
   new (memSegment) TR::CodeCacheMemorySegment(memorySlab, reinterpret_cast<uint8_t *>(memSegment));
   return memSegment;
   }
