include(OmrCompilerSupport)

set(API_FILES
	compile/Method.cpp
	control/NJJit.cpp
	env/FrontEnd.cpp
	ilgen/NJIlGeneratorMethodDetails.cpp
	ilgen/NJIlGenerator.cpp
	ilgen/nj_api.cpp
	runtime/NJCodeCacheManager.cpp
	runtime/NJJitConfig.cpp
)

# To reduce size we try to exclude code that we
# don't need - for now this consists of some of the 
# JitBuilder code 
set(FILTER_FILES
	${omr_SOURCE_DIR}/compiler/ilgen/IlBuilder.cpp
	${omr_SOURCE_DIR}/compiler/ilgen/IlValue.cpp
	${omr_SOURCE_DIR}/compiler/ilgen/IlInjector.cpp
	${omr_SOURCE_DIR}/compiler/ilgen/MethodBuilder.cpp
	${omr_SOURCE_DIR}/compiler/ilgen/BytecodeBuilder.cpp
	${omr_SOURCE_DIR}/compiler/ilgen/TypeDictionary.cpp
	${omr_SOURCE_DIR}/compiler/ilgen/ThunkBuilder.cpp		
)

# Create njmin library
# The create_omr_compiler_library() function
# will add all the backend Compiler code to the library
create_omr_compiler_library(
	NAME    nj
	DEFINES NONJAVA
	OBJECTS ${API_FILES}
	FILTER ${FILTER_FILES}
)

target_link_libraries(nj
)

install(FILES ilgen/nj_api.h
        DESTINATION include/nj)
install(TARGETS nj
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)