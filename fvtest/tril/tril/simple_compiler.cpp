/*******************************************************************************
 * Copyright (c) 2017, 2017 IBM Corp. and others
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

#include "simple_compiler.hpp"

#include "env/jittypes.h"
#include "il/DataTypes.hpp"
#include "il/ILOpCodes.hpp"
#include "compile/Compilation.hpp"
#include "compile/CompilationTypes.hpp"
#include "compile/Method.hpp"
#include "control/CompileMethod.hpp"
#include "env/jittypes.h"
#include "il/DataTypes.hpp"
#include "ilgen.hpp"
#include "ilgen/IlGeneratorMethodDetails_inlines.hpp"

#include <algorithm>

int32_t Tril::SimpleCompiler::compile() {
   return compileWithVerifier(NULL);
}

int32_t Tril::SimpleCompiler::compileWithVerifier(TR::IlVerifier* verifier) {
    // construct an IL generator for the method
    auto methodInfo = getMethodInfo();
    TR::TypeDictionary types;
    Tril::TRLangBuilder ilgenerator{methodInfo.getBodyAST(), &types};

    // get a list of the method's argument types and transform it
    // into a list of `TR::IlType`
    auto argTypes = methodInfo.getArgTypes();
    auto argIlTypes = std::vector<TR::IlType*>{argTypes.size()};
    auto it_argIlTypes = argIlTypes.begin(); 
    for (auto it = argTypes.cbegin(); it != argTypes.cend(); it++) {
          *it_argIlTypes++ = types.PrimitiveType(*it);
    }
    // construct a `TR::ResolvedMethod` instance from the IL generator and use
    // to compile the method
    TR::ResolvedMethod resolvedMethod("file", "line", "name",
                                      argIlTypes.size(),
                                      argIlTypes.data(),
                                      types.PrimitiveType(methodInfo.getReturnType()),
                                      0,
                                      &ilgenerator);
    TR::IlGeneratorMethodDetails methodDetails(&resolvedMethod);

    // If a verifier is provided, set one up. 
    if (NULL != verifier) 
       {
       methodDetails.setIlVerifier(verifier);
       }

    int32_t rc = 0;
    auto entry_point = compileMethodFromDetails(NULL, methodDetails, warm, rc);

    // if compilation was successful, set the entry point for the compiled body
    if (rc == 0) setEntryPoint(entry_point);

    // return the return code for the compilation
    return rc;
}
