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

#ifndef OMR_ARM64_DEBUG_ENV_INCL
#define OMR_ARM64_DEBUG_ENV_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_DEBUG_ENV_CONNECTOR
#define OMR_DEBUG_ENV_CONNECTOR
namespace OMR { namespace ARM64 { class DebugEnv; } }
namespace OMR { typedef OMR::ARM64::DebugEnv DebugEnvConnector; }
#else
#error OMR::ARM64::DebugEnv expected to be a primary connector, but an OMR connector is already defined
#endif

#include "compiler/env/OMRDebugEnv.hpp"
#include "infra/Annotations.hpp"  // for OMR_EXTENSIBLE

namespace OMR
{

namespace ARM64
{

class OMR_EXTENSIBLE DebugEnv : public OMR::DebugEnv
   {
public:

   DebugEnv();

   };

}

}

#endif //OMR_ARM64_DEBUG_ENV_INCL
