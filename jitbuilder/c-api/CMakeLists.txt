###############################################################################
# Copyright (c) 2017, 2018 IBM Corp. and others
#
# This program and the accompanying materials are made available under
# the terms of the Eclipse Public License 2.0 which accompanies this
# distribution and is available at https://www.eclipse.org/legal/epl-2.0/
# or the Apache License, Version 2.0 which accompanies this distribution and
# is available at https://www.apache.org/licenses/LICENSE-2.0.
#
# This Source Code may also be made available under the following
# Secondary Licenses when the conditions for such availability set
# forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
# General Public License, version 2 with the GNU Classpath
# Exception [1] and GNU General Public License, version 2 with the
# OpenJDK Assembly Exception [2].
#
# [1] https://www.gnu.org/software/classpath/license.html
# [2] http://openjdk.java.net/legal/assembly-exception.html
#
# SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
###############################################################################

include(OmrCompilerSupport)

message(STATUS "omr_SOURCE_DIR is ${omr_SOURCE_DIR}")

# JitBuilder Files
set(JITBUILDER_OBJECTS
	nj_api.cpp
	${omr_SOURCE_DIR}/jitbuilder/env/FrontEnd.cpp
	${omr_SOURCE_DIR}/jitbuilder/compile/Method.cpp
	${omr_SOURCE_DIR}/jitbuilder/control/Jit.cpp
	${omr_SOURCE_DIR}/jitbuilder/ilgen/JBIlGeneratorMethodDetails.cpp
	${omr_SOURCE_DIR}/jitbuilder/optimizer/JBOptimizer.hpp
	${omr_SOURCE_DIR}/jitbuilder/optimizer/JBOptimizer.cpp
	${omr_SOURCE_DIR}/jitbuilder/optimizer/Optimizer.hpp
	${omr_SOURCE_DIR}/jitbuilder/runtime/JBCodeCacheManager.cpp
	${omr_SOURCE_DIR}/compiler/ilgen/VirtualMachineOperandArray.cpp
	${omr_SOURCE_DIR}/compiler/ilgen/VirtualMachineOperandStack.cpp
)

if(OMR_ARCH_X86)
	list(APPEND JITBUILDER_OBJECTS
		${omr_SOURCE_DIR}/jitbuilder/x/codegen/Evaluator.cpp
	)
elseif(OMR_ARCH_S390)
	list(APPEND JITBUILDER_OBJECTS
		${omr_SOURCE_DIR}/jitbuilder/z/codegen/Evaluator.cpp
		${omr_SOURCE_DIR}/jitbuilder/z/codegen/JBCodeGenerator.cpp
	)
elseif(OMR_ARCH_POWER)
	list(APPEND JITBUILDER_OBJECTS
		${omr_SOURCE_DIR}/jitbuilder/p/codegen/Evaluator.cpp
		${omr_SOURCE_DIR}/jitbuilder/p/runtime/CodeSync.cpp
		${omr_SOURCE_DIR}/jitbuilder/p/runtime/AsmUtil.spp
		${omr_SOURCE_DIR}/jitbuilder/p/runtime/CodeDispatch.spp
		${omr_SOURCE_DIR}/compiler/p/env/OMRDebugEnv.cpp
	)
endif()


# Create jitbuilder library.
create_omr_compiler_library(
	NAME    njapi
	OBJECTS ${JITBUILDER_OBJECTS}
	DEFINES PROD_WITH_ASSUMES JITTEST
)

# Add interface path so that include paths propagate.
# NOTE: `release` directory  isn't being automatically setup, so this
#       is adding the actual compiler dir, as opposed to the 'composed'
#       install directory.
target_include_directories(njapi
	INTERFACE
	${omr_SOURCE_DIR}/jitbuilder/release/include
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(njapitest1
	tests/test1.cpp
)

target_link_libraries(njapitest1
	njapi
)

install(FILES nj_api.h
        DESTINATION include/nj)
install(TARGETS njapi
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib)

