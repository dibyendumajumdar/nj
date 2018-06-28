include(OmrCompilerSupport)

set(API_FILES
	compile/Method.cpp
	control/NJJit.cpp
	env/FrontEnd.cpp
	ilgen/NJIlGeneratorMethodDetails.cpp
	ilgen/NJIlGenerator.cpp
	ilgen/JIT_API.cpp
	runtime/NJCodeCacheManager.cpp
	runtime/NJJitConfig.cpp
)

# Create njmin library
# The create_omr_compiler_library() function
# will add all the backend Compiler code to the library
create_omr_compiler_library(
	NAME    njmin
	DEFINES NONJAVA
	OBJECTS ${API_FILES}
)

target_link_libraries(njmin
)
