cmake_minimum_required(VERSION 2.8.8)

project(qmlunrimp)

find_package(Qt5Quick 5.1 REQUIRED)

##################################################
## Platform
##################################################
# Check which architecture has the host system
# X86_64 aka x64
if(${CMAKE_GENERATOR} MATCHES "Visual Studio 10 Win64")
	set(ARCHITECTURE_X64 1)
	set(ARCHITECTURE "x64")
elseif(${CMAKE_GENERATOR} MATCHES "Visual Studio 10")
	set(ARCHITECTURE_X86 1)
	set(ARCHITECTURE "x86")
elseif(${CMAKE_GENERATOR} MATCHES "Unix Makefiles")
	if(CMAKE_SIZEOF_VOID_P MATCHES 8)
		set(ARCHITECTURE_X64 1)
		set(ARCHITECTURE "x64")
	else()
		set(ARCHITECTURE_X86 1)
		set(ARCHITECTURE "x86")
	endif()
else()
	message("Unsupported architecture")
endif()

# Operation system + architecture
if(WIN32)
	set(OS_ARCHITECTURE "Windows_${ARCHITECTURE}")
elseif(UNIX)
	set(OS_ARCHITECTURE "Linux_${ARCHITECTURE}")
else()
	message("Unsupported operation system")
endif()
message(STATUS "Operation system + architecture: ${OS_ARCHITECTURE}")
set(UNRIMPBASE_PATH "${UNRIMPBASE_PATH}" CACHE PATH "")

set(UNRIMP_LIB_DIR "${UNRIMPBASE_PATH}/lib/${OS_ARCHITECTURE}")
set(UNRIMP_INCLUDE_DIR "${UNRIMPBASE_PATH}/include")

unset (${UNRIMP_RENDERER_FOUND} CACHE)
find_file(UNRIMP_RENDERER_FOUND "Renderer/Renderer.h" PATH ${UNRIMP_INCLUDE_DIR})
if("${UNRIMP_RENDERER_FOUND}" STREQUAL "UNRIMP_RENDERER_FOUND-NOTFOUND" )
	message(SEND_ERROR "unrimp header not found" )
endif()

if (UNIX)
	set(Unrimp_OpenGL_Renderer_lib_name "libOpenGLRendererStatic.a")
elseif(WIN32)
	set(Unrimp_OpenGL_Renderer_lib_name "OpenGLRendererStatic.lib")
endif()

if (UNIX)
	set(Unrimp_Renderer_Runtime_lib_name "libRendererRuntimeStatic.a")
elseif(WIN32)
	set(Unrimp_Renderer_Runtime_lib_name "RendererRuntimeStatic.lib")
endif()

unset (${UNRIMP_OPENGLRENDERER_FOUND} CACHE)
find_file(UNRIMP_OPENGLRENDERER_FOUND "${Unrimp_OpenGL_Renderer_lib_name}" PATH ${UNRIMP_LIB_DIR})
if("${UNRIMP_OPENGLRENDERER_FOUND}" STREQUAL "UNRIMP_OPENGLRENDERER_FOUND-NOTFOUND" )
	message(SEND_ERROR "unrimp opengl renderer static lib not found" )
endif()

unset (${UNRIMP_RENDERER_RUNTIME_FOUND} CACHE)
find_file(UNRIMP_RENDERER_RUNTIME_FOUND "${Unrimp_Renderer_Runtime_lib_name}" PATH ${UNRIMP_LIB_DIR})
if("${UNRIMP_RENDERER_RUNTIME_FOUND}" STREQUAL "UNRIMP_RENDERER_RUNTIME_FOUND-NOTFOUND" )
	message(SEND_ERROR "unrimp renderer runtime static lib not found" )
endif()


# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SRC_FILES main.cpp
	unrimpitem.cpp
	unrimpnode.cpp
	ExampleModel.cpp
	ExampleApplicationFrontend.cpp
	UnrimpExamples/UnrimpExample.cpp
)

# unrimp example sources
set(UNRIMP_EXAMPLES_PATH "${UNRIMPBASE_PATH}/Example/Examples/src")
set(SRC_FILES ${SRC_FILES}
	${UNRIMP_EXAMPLES_PATH}/Framework/Color4.cpp
	${UNRIMP_EXAMPLES_PATH}/Framework/EulerAngles.cpp
	${UNRIMP_EXAMPLES_PATH}/Framework/Stopwatch.cpp
	${UNRIMP_EXAMPLES_PATH}/Framework/ExampleBase.cpp
	# Basic
	${UNRIMP_EXAMPLES_PATH}/Basics/FirstTriangle/FirstTriangle.cpp
	${UNRIMP_EXAMPLES_PATH}/Basics/FirstIndirectBuffer/FirstIndirectBuffer.cpp
	${UNRIMP_EXAMPLES_PATH}/Basics/FirstTexture/FirstTexture.cpp
	${UNRIMP_EXAMPLES_PATH}/Basics/VertexBuffer/VertexBuffer.cpp
	${UNRIMP_EXAMPLES_PATH}/Basics/FirstRenderToTexture/FirstRenderToTexture.cpp
	${UNRIMP_EXAMPLES_PATH}/Basics/FirstMultipleRenderTargets/FirstMultipleRenderTargets.cpp
	${UNRIMP_EXAMPLES_PATH}/Basics/FirstInstancing/FirstInstancing.cpp
	${UNRIMP_EXAMPLES_PATH}/Basics/FirstTessellation/FirstTessellation.cpp
	${UNRIMP_EXAMPLES_PATH}/Basics/FirstGeometryShader/FirstGeometryShader.cpp
	# RendererRuntime
	${UNRIMP_EXAMPLES_PATH}/Advanced/InstancedCubes/CubeRendererDrawInstanced/BatchDrawInstanced.cpp
	${UNRIMP_EXAMPLES_PATH}/Advanced/InstancedCubes/CubeRendererDrawInstanced/CubeRendererDrawInstanced.cpp
	${UNRIMP_EXAMPLES_PATH}/Advanced/InstancedCubes/CubeRendererInstancedArrays/BatchInstancedArrays.cpp
	${UNRIMP_EXAMPLES_PATH}/Advanced/InstancedCubes/CubeRendererInstancedArrays/CubeRendererInstancedArrays.cpp
	${UNRIMP_EXAMPLES_PATH}/Advanced/InstancedCubes/ICubeRenderer.cpp
	${UNRIMP_EXAMPLES_PATH}/Advanced/InstancedCubes/InstancedCubes.cpp
)
  
include_directories("${UNRIMPBASE_PATH}/include")
include_directories("${UNRIMPBASE_PATH}/Example/Examples/src")

# TODO(sw) Wy do we have to add an include path to the build directory of renderer runtime and renderer? For what are then the include files in UNRIMPBASE_PATH/include good for?
include_directories("${UNRIMPBASE_PATH}/Renderer/RendererRuntime/include")
include_directories("${UNRIMPBASE_PATH}/Renderer/Renderer/include")

# We need c++0x/c++11 support:
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	message(STATUS "enable c++11 support")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()
# At first we treat UNIX = Linux
if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLINUX -DUSEOPENGL -DRENDERER_NO_DEBUG")
endif()

link_directories(${UNRIMP_LIB_DIR})

if (${ARCHITECTURE_X64})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DX64_ARCHITECTURE")
endif()

if (${ARCHITECTURE_X68})
	message(STATUS "32Bit")
endif()

add_executable(qmlunrimp ${SRC_FILES})
target_link_libraries(qmlunrimp ${UNRIMP_OPENGLRENDERER_FOUND} ${UNRIMP_RENDERER_RUNTIME_FOUND})

if(UNIX)
	# needed from the static opengl renderer and renderer runtime
	target_link_libraries(qmlunrimp X11 dl pthread)
endif()

qt5_use_modules(qmlunrimp Quick Qml Widgets)
