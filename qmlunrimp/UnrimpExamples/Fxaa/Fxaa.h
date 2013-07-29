/*
 *
 */

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
