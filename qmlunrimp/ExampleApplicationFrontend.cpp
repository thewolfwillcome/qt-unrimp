/*********************************************************\
 * Copyright (c) 2013-2013 Stephan Wezel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


#include "ExampleApplicationFrontend.h"
#include "QtFileManager.h"

#include <RendererRuntime/Asset/AssetManager.h>


RendererRuntime::IRendererRuntime* createRendererRuntime(Renderer::IRenderer& renderer, RendererRuntime::IFileManager& fileManager)
{
	extern RendererRuntime::IRendererRuntime *createRendererRuntimeInstance(Renderer::IRenderer &renderer, RendererRuntime::IFileManager& fileManager);

	// Create the renderer runtime instance
	return createRendererRuntimeInstance(renderer, fileManager);
}

ExampleApplicationFrontend::ExampleApplicationFrontend(Renderer::IRendererPtr renderer) :
	mRenderer(renderer),
	mMainRenderTarget(nullptr),
	mRendererRuntime(nullptr),
	mFileManager(nullptr)
{
}

ExampleApplicationFrontend::~ExampleApplicationFrontend()
{
	delete mFileManager;
}

void ExampleApplicationFrontend::setMainRenderTarget(Renderer::IRenderTarget* mainRenderTarget)
{
	mMainRenderTarget = mainRenderTarget;
}

void ExampleApplicationFrontend::setupRendererRuntime()
{
	if (mRenderer && nullptr == mRendererRuntime)
	{
		if (nullptr == mFileManager)
		{
			mFileManager = new QtFileManager();
		}
		mRendererRuntime = createRendererRuntime(*mRenderer, *mFileManager);

		if (nullptr != mRendererRuntime)
		{
			// TODO(co) Under construction: Will probably become "mount asset package"
			// Add used asset package
			const bool rendererIsOpenGLES = 0 == strcmp(mRenderer->getName(), "OpenGLES2");
			if (rendererIsOpenGLES)
			{
#ifdef ANDROID
                mRendererRuntime->getAssetManager().addAssetPackageByFilename("assets:/DataMobile/Content/AssetPackage.assets");
#else
                mRendererRuntime->getAssetManager().addAssetPackageByFilename("android/assets/DataMobile/Content/AssetPackage.assets");
#endif
			}
			else
			{
				mRendererRuntime->getAssetManager().addAssetPackageByFilename("android/assets/DataPc/Content/AssetPackage.assets");
			}
		}
	}
}

void ExampleApplicationFrontend::teardownRendererRuntime()
{
	mRendererRuntime = nullptr;
}
