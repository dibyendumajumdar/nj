include(OmrCompilerSupport)

add_library(njapi STATIC
	nj_api.cpp
)

# This is mildly peculiar. Because tril relies
# on jitbuilder as a 'backend' of sorts, we use
# its includes. Tril exports this as interface
make_compiler_target(njapi PUBLIC COMPILER jitbuilder) 

target_include_directories(njapi PUBLIC
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(njapi PUBLIC 
	jitbuilder
	${CMAKE_DL_LIBS}
)


