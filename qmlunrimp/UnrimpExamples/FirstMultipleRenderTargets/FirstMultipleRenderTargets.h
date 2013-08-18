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


#ifndef FIRSTMULTIPLERENDERTARGETS_H
#define FIRSTTRIANGLE_H

#include "UnrimpExamples/ExampleBase.h"

class FirstMultipleRenderTargets : public ExampleBase
{
public:
	virtual void Init(Renderer::IRendererPtr renderer) override;
	virtual void Deinit() override;
	virtual void Render() override;
    virtual QString name() { return "FirstMultipleRenderTargets"; } override;

private:
	static const unsigned int TEXTURE_SIZE		 = 16;	///< Texture size
	static const unsigned int NUMBER_OF_TEXTURES = 2;	///< Number of textures

private:
	Renderer::ITexture2DPtr	   mTexture2D[NUMBER_OF_TEXTURES];	///< 2D textures, can be a null pointer
	Renderer::IFramebufferPtr  mFramebuffer;					///< Framebuffer object (FBO), can be a null pointer
	Renderer::ISamplerStatePtr mSamplerState;					///< Sampler state, can be a null pointer
	Renderer::IProgramPtr	   mProgramMultipleRenderTargets;	///< Program multiple render targets, can be a null pointer
	Renderer::IProgramPtr	   mProgram;						///< Program, can be a null pointer
	Renderer::IVertexArrayPtr  mVertexArray;					///< Vertex array object (VAO), can be a null pointer

};

#endif // FIRSTTRIANGLE_H
