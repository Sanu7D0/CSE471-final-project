cmake_minimum_required (VERSION 3.8)
project ("cg-finalproject")

### Project configure

# Add .lib files
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Add source files
file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)

# Add header files
file(GLOB_RECURSE HEADER_FILES
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)

# Configure assets header file
configure_file(src/helper/RootDir.h.in src/helper/RootDir.h)
include_directories(${CMAKE_BINARY_DIR}/src)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

# Set cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


### Find packages

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

set(LIBS ${GLFW3_LIBRARY} ${OPENGL_LIBRARY} ${CMAKE_DL_LIBS} ${ASSIMP_LIBRARY})

set(LIBS ${LIBS} freetype OpenAL32)
add_library(STB_IMAGE "src/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)
add_library(GLAD "src/glad.c")
set(LIBS ${LIBS} GLAD)

# Define the include DIRs
include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/include/glad"
	"${CMAKE_SOURCE_DIR}/include/glm"
)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})


# Copy shader files to build directory
#file(GLOB SHADERS
#	"resouce/shader/*.fs"
#	"resouce/shader/*.vs"
#)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/resource"
	$<TARGET_FILE_DIR:${PROJECT_NAME}>/resource)

# Symbolic link -> WARNING: not actually copied to build
#add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
#	COMMAND ${CMAKE_COMMAND} -E create_symlink
#	${CMAKE_SOURCE_DIR}/resource $<TARGET_FILE_DIR:${PROJECT_NAME}>/resource)
#)


# Copy dlls to build directory
#file(GLOB DLLS "dll/*.dll")
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/dll"
	$<TARGET_FILE_DIR:${PROJECT_NAME}>)
