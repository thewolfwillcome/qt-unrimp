QT += qml quick

CONFIG += c++11

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    ExampleModel.cpp \
    unrimpitem.cpp \
    unrimpnode.cpp \
    ExampleApplicationFrontend.cpp \
    QtFileManager.cpp \
    UnrimpExamples/UnrimpExample.cpp

# Set base path to unrimp, please don't commit this path
UNRIMPBASE_PATH = "/home/stephan/data/projects/pixellight-src/unrimp_new.git"
UNRIMP_EXAMPLES_PATH = "$$UNRIMPBASE_PATH/Example/Examples/src"

SOURCES += $$UNRIMP_EXAMPLES_PATH/Framework/Color4.cpp
SOURCES += $$UNRIMP_EXAMPLES_PATH/Framework/EulerAngles.cpp
SOURCES += $$UNRIMP_EXAMPLES_PATH/Framework/ExampleBase.cpp
    # Basic
SOURCES += $$UNRIMP_EXAMPLES_PATH/Basics/FirstTriangle/FirstTriangle.cpp
SOURCES += $$UNRIMP_EXAMPLES_PATH/Basics/FirstIndirectBuffer/FirstIndirectBuffer.cpp
SOURCES += $$UNRIMP_EXAMPLES_PATH/Basics/FirstTexture/FirstTexture.cpp
SOURCES += $$UNRIMP_EXAMPLES_PATH/Basics/VertexBuffer/VertexBuffer.cpp
SOURCES += $$UNRIMP_EXAMPLES_PATH/Basics/FirstRenderToTexture/FirstRenderToTexture.cpp
SOURCES += $$UNRIMP_EXAMPLES_PATH/Basics/FirstMultipleRenderTargets/FirstMultipleRenderTargets.cpp
SOURCES += $$UNRIMP_EXAMPLES_PATH/Basics/FirstInstancing/FirstInstancing.cpp
SOURCES += $$UNRIMP_EXAMPLES_PATH/Basics/FirstTessellation/FirstTessellation.cpp
SOURCES += $$UNRIMP_EXAMPLES_PATH/Basics/FirstGeometryShader/FirstGeometryShader.cpp
    # RendererRuntime
SOURCES += $$UNRIMP_EXAMPLES_PATH/Advanced/InstancedCubes/CubeRendererDrawInstanced/BatchDrawInstanced.cpp
SOURCES += $$UNRIMP_EXAMPLES_PATH/Advanced/InstancedCubes/CubeRendererDrawInstanced/CubeRendererDrawInstanced.cpp
SOURCES += $$UNRIMP_EXAMPLES_PATH/Advanced/InstancedCubes/CubeRendererInstancedArrays/BatchInstancedArrays.cpp
SOURCES += $$UNRIMP_EXAMPLES_PATH/Advanced/InstancedCubes/CubeRendererInstancedArrays/CubeRendererInstancedArrays.cpp
SOURCES += $$UNRIMP_EXAMPLES_PATH/Advanced/InstancedCubes/ICubeRenderer.cpp
SOURCES += $$UNRIMP_EXAMPLES_PATH/Advanced/InstancedCubes/InstancedCubes.cpp
SOURCES += $$UNRIMP_EXAMPLES_PATH/Runtime/FirstMesh/FirstMesh.cpp


INCLUDEPATH += $$quote($$UNRIMPBASE_PATH/include)
INCLUDEPATH += $$quote($$UNRIMPBASE_PATH/External/glm/include/)
INCLUDEPATH += $$quote($$UNRIMP_EXAMPLES_PATH/)

# TODO(sw) Wy do we have to add an include path to the build directory of renderer runtime and renderer? For what are then the include files in UNRIMPBASE_PATH/include good for?
INCLUDEPATH += $$quote($$UNRIMPBASE_PATH/Renderer/RendererRuntime/include)
INCLUDEPATH += $$quote($$UNRIMPBASE_PATH/Renderer/Renderer/include)

android-g++ {
    # TODO(sw) Android ndk is switching to clang + llvm libc++ thus don't update gcc/gnustl. But Qt isn't yet released with an clang build so we need to use gcc 4.9 with gnustl which has some problems
    # e.g. std::to_string are not part of the gnustl c++ runtime library -> so Renderer debug disabled
    LIBS += -L$$quote($$UNRIMPBASE_PATH/lib/Android_armeabi-v7a/) -lOpenGLES3RendererStatic -lRendererRuntimeStatic -ldl -landroid
    DEFINES += LINUX ANDROID USEGLES RENDERER_NO_DEBUG
}

linux-g++ {
    DEFINES += LINUX USEOPENGL RENDERER_NO_DEBUG
    LIBS += -L$$quote($$UNRIMPBASE_PATH/lib/Linux_x64/) -lOpenGLRendererStatic -ldl -lX11 -lRendererRuntimeStatic  -lpthread
}

linux-g++:QMAKE_TARGET.arch = $$QMAKE_HOST.arch
linux-g++-32:QMAKE_TARGET.arch = x86
linux-g++-64:QMAKE_TARGET.arch = x86_64

contains(QMAKE_TARGET.arch, x86_64):{
    message( "Building for 64 bit")
    DEFINES += X64_ARCHITECTURE
}

QMAKE_CXXFLAGS += -std=c++11
# -Wcast-align

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += android/AndroidManifest.xml

HEADERS += \
    ExampleModel.h \
    unrimpitem.h \
    unrimpnode.h \
    ExampleApplicationFrontend.h \
    QtFileManager.h \
    UnrimpExamples/UnrimpExample.h

DISTFILES += \
    android/assets/qml/ExampleList2.qml \
    android/assets/qml/main.qml \
    android/assets/qml/SlideOut.qml


