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


#ifndef FXAA_H
#define FXAA_H

#include "UnrimpExamples/ExampleBase.h"

class Fxaa : public ExampleBase
{
public:
	virtual void Init(Renderer::IRendererPtr renderer) override;
	virtual void Deinit() override;
	virtual void Render() override;
    virtual QString name() { return "Fxaa"; } override;
	
protected:
	virtual void onSizeChanged() override;

//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
private:
	/**
	*  @brief
	*    Create/recreate the framebuffer object (FBO) instance by using the current window size
	*/
	void recreateFramebuffer();

	/**
	*  @brief
	*    Create/recreate the post-processing program instance by using the current window size
	*/
	void recreatePostProcessingProgram();

	/**
	*  @brief
	*    Scene rendering
	*/
	void sceneRendering();

	/**
	*  @brief
	*    Post-processing
	*/
	void postProcessing();

private:
	Renderer::ITexture2DPtr			mTexture2D;			///< 2D texture, can be a null pointer
	Renderer::IFramebufferPtr		mFramebuffer;		///< Framebuffer object (FBO), can be a null pointer
	Renderer::ISamplerStatePtr		mSamplerState;		///< Sampler state, can be a null pointer
	Renderer::IDepthStencilStatePtr	mDepthStencilState;	///< Depth stencil state, can be a null pointer
	// Scene rendering
	Renderer::IProgramPtr	   mProgramSceneRendering;		///< Program for scene rendering, can be a null pointer
	Renderer::IVertexArrayPtr  mVertexArraySceneRendering;	///< Vertex array object (VAO) for scene rendering, can be a null pointer
	// Post-processing
	Renderer::IProgramPtr	   mProgramPostProcessing;		///< Program for post-processing, can be a null pointer
	Renderer::IVertexArrayPtr  mVertexArrayPostProcessing;	///< Vertex array object (VAO) for post-processing, can be a null pointer

};

#endif // FXAA_H
