/*
 *
 */

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
