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


#ifndef FIRSTINSTANCING_H
#define FIRSTINSTANCING_H

#include "UnrimpExamples/ExampleBase.h"

class FirstInstancing : public ExampleBase
{
public:
	virtual void onInit(Renderer::IRendererPtr renderer) override;
	virtual void onDeinit() override;
	virtual void Render() override;
    virtual QString name() override { return "FirstInstancing"; }


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
private:
	void fillCommandBuffer();


//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
private:
	Renderer::IBufferManagerPtr mBufferManager;					///< Buffer manager, can be a null pointer
	Renderer::IRootSignaturePtr mRootSignature;					///< Root signature, can be a null pointer
	Renderer::CommandBuffer		mCommandBuffer;					///< Command buffer
	// Left side (green): Instanced arrays (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
	Renderer::IPipelineStatePtr mPipelineStateInstancedArrays;	///< Pipeline state object (PSO), can be a null pointer
	Renderer::IVertexArrayPtr   mVertexArrayInstancedArrays;	///< Vertex array object (VAO), can be a null pointer
	// Right side (blue): Draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
	Renderer::IPipelineStatePtr mPipelineStateDrawInstanced;	///< Pipeline state object (PSO), can be a null pointer
	Renderer::IVertexArrayPtr   mVertexArrayDrawInstanced;		///< Vertex array object (VAO), can be a null pointer

};

#endif // FIRSTINSTANCING_H
