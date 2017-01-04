QT += qml quick

CONFIG += c++11

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    ExampleModel.cpp \
    unrimpitem.cpp \
    unrimpnode.cpp \
    UnrimpExamples/ExampleBase.cpp \
    UnrimpExamples/Color4.cpp \
    UnrimpExamples/Color4.inl \
    UnrimpExamples/Stopwatch.cpp \
    UnrimpExamples/Stopwatch.inl \
    UnrimpExamples/FirstTriangle/FirstTriangle.cpp \
    UnrimpExamples/FirstRenderToTexture/FirstRenderToTexture.cpp

# UnrimpExamples/VertexBuffer/VertexBuffer.cpp
#UnrimpExamples/FirstTexture/FirstTexture.cpp \
#UnrimpExamples/FirstPostProcessing/FirstPostProcessing.cpp
#UnrimpExamples/FirstAssimp/FirstAssimp.cpp
#UnrimpExamples/FirstAssimp/PointsMesh.cpp
#UnrimpExamples/TextureFromFileLoader.cpp
#UnrimpExamples/AssimpMesh/AssimpMesh.cpp
#UnrimpExamples/AssimpMesh/Mesh.cpp

# Set base path to unrimp, please don't commit this path
UNRIMPBASE_PATH ="<PathToUnrimp>"
INCLUDEPATH += $$quote($$UNRIMPBASE_PATH/include)
INCLUDEPATH += $$quote($$UNRIMPBASE_PATH/External/glm/include/)

android-g++ {
    # TODO(sw) Android ndk is switching to clang + llvm libc++ thus don't update gcc/gnustl. But Qt isn't yet released with an clang build so we need to use gcc 4.9 with gnustl which has some problems
    # e.g. std::log2 and std::to_string are not part of the gnustl c++ runtime library and also L"" in macros don't work -> so Renderer debug disabled
    LIBS += $$quote($$UNRIMPBASE_PATH/lib/Android_armeabi-v7a/libOpenGLES2RendererStatic.a) -ldl -landroid
    DEFINES += LINUX USEGLES RENDERER_NO_DEBUG
}

linux-g++ {
    DEFINES += LINUX USEOPENGL RENDERER_NO_DEBUG
    LIBS += -L$$quote($$UNRIMPBASE_PATH/lib/Linux_x64/) -lOpenGLRendererStatic -ldl -lX11
}

linux-g++:QMAKE_TARGET.arch = $$QMAKE_HOST.arch
linux-g++-32:QMAKE_TARGET.arch = x86
linux-g++-64:QMAKE_TARGET.arch = x86_64

contains(QMAKE_TARGET.arch, x86_64):{
    message( "Building for 64 bit")
    DEFINES += X64_ARCHITECTURE
}

QMAKE_CXXFLAGS += -std=c++11

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += android/AndroidManifest.xml \
    android/assets/Imrod/ImrodLowPoly.obj

HEADERS += \
    ExampleModel.h \
    unrimpitem.h \
    unrimpnode.h \
    UnrimpExamples/ExampleBase.h \
    UnrimpExamples/Color4.h \
    UnrimpExamples/PlatformTypes.h \
    UnrimpExamples/FirstTriangle/FirstTriangle_GLSL_ES2.h \
    UnrimpExamples/FirstTriangle/FirstTriangle.h \
    UnrimpExamples/FirstTriangle/FirstTriangle_GLSL_410.h \
    UnrimpExamples/FirstRenderToTexture/FirstRenderToTexture.h \
    UnrimpExamples/FirstRenderToTexture/FirstRenderToTexture_GLSL_410.h \
    UnrimpExamples/FirstRenderToTexture/FirstRenderToTexture_GLSL_ES2.h \
    CLocaleChanger.h

#    UnrimpExamples/VertexBuffer/VertexBuffer_GLSL_410.h \
#    UnrimpExamples/VertexBuffer/VertexBuffer_GLSL_ES2.h \
#    UnrimpExamples/VertexBuffer/VertexBuffer.h \
#    UnrimpExamples/FirstTexture/FirstTexture_GLSL_410.h \
#    UnrimpExamples/FirstTexture/FirstTexture_GLSL_ES2.h \
#    UnrimpExamples/FirstTexture/FirstTexture.h \
#    UnrimpExamples/FirstPostProcessing/FirstPostProcessing.h \
#    UnrimpExamples/FirstPostProcessing/FirstPostProcessing_GLSL_410.h \
#    UnrimpExamples/FirstPostProcessing/FirstPostProcessing_GLSL_ES2.h \
#    UnrimpExamples/FirstAssimp/FirstAssimp_GLSL_410.h \
#    UnrimpExamples/FirstAssimp/FirstAssimp.h \
#    UnrimpExamples/FirstAssimp/PointsMesh.h \
#    UnrimpExamples/Stopwatch.h \
#    UnrimpExamples/TextureFromFileLoader.h \
#    UnrimpExamples/FirstAssimp/FirstAssimp_GLSL_ES2.h \
#    UnrimpExamples/AssimpMesh/AssimpMesh_GLSL_410.h \
#    UnrimpExamples/AssimpMesh/AssimpMesh_GLSL_ES2.h \
#    UnrimpExamples/AssimpMesh/AssimpMesh.h \
#    UnrimpExamples/AssimpMesh/Mesh.h

DISTFILES += \
    android/assets/qml/ExampleList2.qml \
    android/assets/qml/main.qml \
    android/assets/qml/SlideOut.qml


