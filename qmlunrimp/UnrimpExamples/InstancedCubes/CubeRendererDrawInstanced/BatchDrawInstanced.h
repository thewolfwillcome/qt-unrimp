/*********************************************************\
 * Copyright (c) 2012-2013 Christian Ofenberg
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


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once
#ifndef __INSTANCEDCUBES_BATCH_DRAWINSTANCED_H__
#define __INSTANCEDCUBES_BATCH_DRAWINSTANCED_H__


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <Renderer/Renderer.h>


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Batch of instanced cubes using draw instanced
*/
class BatchDrawInstanced
{


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Default constructor
	*/
	BatchDrawInstanced();

	/**
	*  @brief
	*    Destructor
	*/
	~BatchDrawInstanced();

	/**
	*  @brief
	*    Initialize the batch
	*
	*  @param[in] renderer
	*    Owner renderer instance
	*  @param[in] numberOfCubeInstances
	*    Number of cube instances
	*  @param[in] alphaBlending
	*    Does this batch use alpha blending?
	*  @param[in] numberOfTextures
	*    Number of textures
	*  @param[in] sceneRadius
	*    Scene radius
	*/
	void initialize(Renderer::IRenderer &renderer, unsigned int numberOfCubeInstances, bool alphaBlending, unsigned int numberOfTextures, unsigned int sceneRadius);

	/**
	*  @brief
	*    Draw the batch
	*/
	void draw();


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
private:
	/**
	*  @brief
	*    Copy constructor
	*
	*  @param[in] source
	*    Source to copy from
	*/
	explicit BatchDrawInstanced(const BatchDrawInstanced &source);

	/**
	*  @brief
	*    Copy operator
	*
	*  @param[in] source
	*    Source to copy from
	*
	*  @return
	*    Reference to this instance
	*/
	BatchDrawInstanced &operator =(const BatchDrawInstanced &source);


//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
private:
	Renderer::IRendererPtr		mRenderer;						///< Owner renderer instance, can be a null pointer
	unsigned int				mNumberOfCubeInstances;			///< Number of cube instances
	Renderer::ITextureBufferPtr	mTextureBufferPerInstanceData;	///< Texture buffer with per instance data (used via vertex texture fetch), each entry can contain a null pointer
	Renderer::IBlendStatePtr	mBlendState;					///< Blend state, can be a null pointer


};


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#endif // __INSTANCEDCUBES_BATCH_DRAWINSTANCED_H__
