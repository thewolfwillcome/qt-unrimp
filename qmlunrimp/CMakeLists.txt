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

unset (${UNRIMP_OPENGLRENDERER_FOUND} CACHE)
find_file(UNRIMP_OPENGLRENDERER_FOUND "${Unrimp_OpenGL_Renderer_lib_name}" PATH ${UNRIMP_LIB_DIR})
if("${UNRIMP_OPENGLRENDERER_FOUND}" STREQUAL "UNRIMP_OPENGLRENDERER_FOUND-NOTFOUND" )
	message(SEND_ERROR "unrimp opengl renderer static lib not found" )
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
	UnrimpExamples/Color4.cpp
# 	UnrimpExamples/TextureFromFileLoader.cpp
	UnrimpExamples/Stopwatch.cpp
	UnrimpExamples/EulerAngles.cpp
	UnrimpExamples/ExampleBase.cpp
	UnrimpExamples/FirstTriangle/FirstTriangle.cpp

# 	UnrimpExamples/VertexBuffer/VertexBuffer.cpp
# 	UnrimpExamples/FirstTexture/FirstTexture.cpp
	UnrimpExamples/FirstRenderToTexture/FirstRenderToTexture.cpp
# 	UnrimpExamples/FirstPostProcessing/FirstPostProcessing.cpp
# 	UnrimpExamples/FirstMultipleRenderTargets/FirstMultipleRenderTargets.cpp
# 	UnrimpExamples/FirstGeometryShader/FirstGeometryShader.cpp
# 	UnrimpExamples/FirstTessellation/FirstTessellation.cpp
# 	UnrimpExamples/FirstInstancing/FirstInstancing.cpp
# 	UnrimpExamples/Fxaa/Fxaa.cpp
# 	UnrimpExamples/InstancedCubes/InstancedCubes.cpp
# 	UnrimpExamples/InstancedCubes/ICubeRenderer.cpp
# 	UnrimpExamples/InstancedCubes/CubeRendererDrawInstanced/BatchDrawInstanced.cpp
# 	UnrimpExamples/InstancedCubes/CubeRendererDrawInstanced/CubeRendererDrawInstanced.cpp
# 	UnrimpExamples/InstancedCubes/CubeRendererInstancedArrays/BatchInstancedArrays.cpp
# 	UnrimpExamples/InstancedCubes/CubeRendererInstancedArrays/CubeRendererInstancedArrays.cpp
# 	UnrimpExamples/IcosahedronTessellation/IcosahedronTessellation.cpp
# 	UnrimpExamples/FirstFont/FirstFont.cpp
# 	UnrimpExamples/FirstAssimp/FirstAssimp.cpp
# 	UnrimpExamples/FirstAssimp/PointsMesh.cpp
# 	UnrimpExamples/AssimpMesh/AssimpMesh.cpp
# 	UnrimpExamples/AssimpMesh/Mesh.cpp
# 	UnrimpExamples/ExampleRenderToolkit.cpp
)
  
include_directories("${UNRIMPBASE_PATH}/include")

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
target_link_libraries(qmlunrimp ${UNRIMP_OPENGLRENDERER_FOUND})

if(UNIX)
	# needed from the static opengl renderer
	target_link_libraries(qmlunrimp X11 dl)
endif()

qt5_use_modules(qmlunrimp Quick Qml Widgets)
