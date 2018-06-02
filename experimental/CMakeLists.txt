include(OmrCompilerSupport)

set(API_FILES
	compile/Method.cpp
	control/NJJit.cpp
	env/FrontEnd.cpp
	ilgen/NJIlGeneratorMethodDetails.cpp
	runtime/NJCodeCacheManager.cpp
	runtime/NJJitConfig.cpp
)

# Create njapi library
# The create_omr_compiler_library() function
# will add all the backend Compiler code to the library
create_omr_compiler_library(
	NAME    njapi
	DEFINES NONJAVA
	OBJECTS ${API_FILES}
)

target_link_libraries(njapi
)
