include(OmrCompilerSupport)

set(API_FILES
	env/FrontEnd.cpp
	compile/Method.cpp
	control/NJJit.cpp
	ilgen/NJIlGeneratorMethodDetails.cpp
	ilgen/NJIlGenerator.cpp
	ilgen/nj_api.cpp
	optimizer/NJOptimizer.cpp
	runtime/NJCodeCacheManager.cpp
	runtime/NJJitConfig.cpp
)

# To reduce size we try to exclude code that we
# don't need - for now this consists of some of the 
# JitBuilder code 
set(FILTER_FILES
	${omr_SOURCE_DIR}/compiler/ilgen/OMRIlBuilder.cpp
	${omr_SOURCE_DIR}/compiler/ilgen/OMRIlType.cpp
	${omr_SOURCE_DIR}/compiler/ilgen/OMRIlValue.cpp
	${omr_SOURCE_DIR}/compiler/ilgen/IlInjector.cpp
	${omr_SOURCE_DIR}/compiler/ilgen/OMRMethodBuilder.cpp
	${omr_SOURCE_DIR}/compiler/ilgen/OMRBytecodeBuilder.cpp
	${omr_SOURCE_DIR}/compiler/ilgen/OMRTypeDictionary.cpp
	${omr_SOURCE_DIR}/compiler/ilgen/OMRThunkBuilder.cpp		
	${omr_SOURCE_DIR}/compiler/ilgen/OMRVirtualMachineOperandArray.cpp		
	${omr_SOURCE_DIR}/compiler/ilgen/OMRVirtualMachineOperandStack.cpp		
	${omr_SOURCE_DIR}/compiler/ilgen/OMRVirtualMachineRegister.cpp
	${omr_SOURCE_DIR}/compiler/ilgen/OMRVirtualMachineRegisterInStruct.cpp
	${omr_SOURCE_DIR}/compiler/ilgen/OMRVirtualMachineState.cpp	
)

if (NOT OMR_TOOLCONFIG MATCHES "msvc")
        # On Linux at least creating static library
        # makes it hard to link with C code
        set(NJ_LIB_TYPE SHARED)
endif()

# Create nj library
# The create_omr_compiler_library() function
# will add all the backend Compiler code to the library
create_omr_compiler_library(
	NAME    nj
	${NJ_LIB_TYPE}
	DEFINES NONJAVA
	OBJECTS ${API_FILES}
	FILTER ${FILTER_FILES}
)

include_directories(
	ilgen
)

install(FILES ilgen/nj_api.h
        DESTINATION include/nj)
install(TARGETS nj
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)

add_subdirectory(c-api-tests)