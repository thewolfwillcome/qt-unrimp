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


#ifndef ASSIMPMESH_H
#define ASSIMPMESH_H

#include "UnrimpExamples/ExampleBase.h"
#include "UnrimpExamples/Stopwatch.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Mesh;

class AssimpMesh : public ExampleBase
{
public:
	AssimpMesh();
	virtual void Init(Renderer::IRendererPtr renderer) override;
	virtual void Deinit() override;
	virtual void Render() override;
    virtual QString name() { return "AssimpMesh"; } override;
	virtual bool wantsCyclicUpdate() {return true;} override;

private:
	Renderer::IUniformBufferPtr			  mUniformBuffer;			///< Uniform buffer object (UBO), can be a null pointer
	Renderer::IProgramPtr				  mProgram;					///< Program, can be a null pointer
	Mesh								 *mMesh;					///< Mesh instance, can be a null pointer
	Renderer::ITextureCollectionPtr		  mTextureCollection;		///< Texture collection, can be a null pointer
	Renderer::ISamplerStateCollectionPtr  mSamplerStateCollection;	///< Sampler state collection, can be a null pointer
	// Optimization: Cache data to not bother the renderer API to much
	unsigned int mUniformBlockIndex;							///< Uniform block index
	uint		 mObjectSpaceToClipSpaceMatrixUniformHandle;	///< Object space to clip space matrix uniform handle, can be NULL_HANDLE
	uint		 mObjectSpaceToViewSpaceMatrixUniformHandle;	///< Object space to view space matrix uniform handle, can be NULL_HANDLE
	// For timing
	Stopwatch mStopwatch;	///< Stopwatch instance
	float	  mGlobalTimer;	///< Global timer

};

#endif // ASSIMPMESH_H
