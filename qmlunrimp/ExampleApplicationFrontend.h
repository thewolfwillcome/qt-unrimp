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


#pragma once

#include <Framework/IApplicationFrontend.h>

#include <Renderer/Renderer.h>
#include <RendererRuntime/IRendererRuntime.h>

namespace RendererRuntime
{
	class Context;
}

class QtFileManager;


class ExampleApplicationFrontend : public IApplicationFrontend
{


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	ExampleApplicationFrontend(Renderer::IRendererPtr renderer);
	~ExampleApplicationFrontend();
	void setMainRenderTarget(Renderer::IRenderTarget* mainRenderTarget);

	void setupRendererRuntime();
	void teardownRendererRuntime();


//[-------------------------------------------------------]
//[ Public virtual IApplicationFrontend methods           ]
//[-------------------------------------------------------]
public:
	inline virtual Renderer::IRenderer *getRenderer() const override { return mRenderer;}
	inline virtual Renderer::IRenderTarget *getMainRenderTarget() const override { return mMainRenderTarget;}
	inline virtual RendererRuntime::IRendererRuntime *getRendererRuntime() const override { return mRendererRuntime;}
	inline virtual RendererToolkit::IRendererToolkit *getRendererToolkit() override { return nullptr;}

private:
	Renderer::IRendererPtr mRenderer;
	Renderer::IRenderTarget* mMainRenderTarget;
	RendererRuntime::IRendererRuntimePtr mRendererRuntime;

	QtFileManager* mFileManager;
	RendererRuntime::Context* mRendererRuntimeContext;


};
