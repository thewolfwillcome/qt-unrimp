/*
 *
 */

#ifndef FIRSTRENDERTOTEXTURE_H
#define FIRSTRENDERTOTEXTURE_H

#include "UnrimpExamples/ExampleBase.h"
class FirstRenderToTexture : public ExampleBase
{
public:
	virtual void Init(Renderer::IRendererPtr renderer) override;
	virtual void Deinit() override;
	virtual void Render() override;
    virtual QString name() { return "FirstRenderToTexture"; } override;

private:
	Renderer::ITexture2DPtr	   mTexture2D;		///< 2D texture, can be a null pointer
	Renderer::IFramebufferPtr  mFramebuffer;	///< Framebuffer object (FBO), can be a null pointer
	Renderer::ISamplerStatePtr mSamplerState;	///< Sampler state, can be a null pointer
	Renderer::IProgramPtr	   mProgram;		///< Program, can be a null pointer
	Renderer::IVertexArrayPtr  mVertexArray;	///< Vertex array object (VAO), can be a null pointer

};

#endif // FIRSTRENDERTOTEXTURE_H
