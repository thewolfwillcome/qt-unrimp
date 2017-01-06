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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "UnrimpExamples/InstancedCubes/CubeRendererInstancedArrays/CubeRendererInstancedArrays.h"
#include "UnrimpExamples/InstancedCubes/CubeRendererInstancedArrays/BatchInstancedArrays.h"
#include "UnrimpExamples/PlatformTypes.h"	// For "OUTPUT_DEBUG_STRING()"
#include "UnrimpExamples/ExampleBase.h"

#include <math.h>
#include <string.h>
#include <float.h> // For FLT_MAX
#include <stdlib.h> // For rand()


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		// Vertex input layout
		const Renderer::VertexAttribute VertexAttributesLayout[] =
		{
			// Mesh data
			{ // Attribute 0
				// Data destination
				Renderer::VertexAttributeFormat::FLOAT_3,	// vertexAttributeFormat (Renderer::VertexAttributeFormat)
				"Position",									// name[32] (char)
				"POSITION",									// semanticName[32] (char)
				0,											// semanticIndex (uint32_t)
				// Data source
				0,											// inputSlot (uint32_t)
				0,											// alignedByteOffset (uint32_t)
				// Data source, instancing part
				0											// instancesPerElement (uint32_t)
			},
			{ // Attribute 1
				// Data destination
				Renderer::VertexAttributeFormat::FLOAT_2,	// vertexAttributeFormat (Renderer::VertexAttributeFormat)
				"TexCoord",									// name[32] (char)
				"TEXCOORD",									// semanticName[32] (char)
				0,											// semanticIndex (uint32_t)
				// Data source
				0,											// inputSlot (uint32_t)
				sizeof(float) * 3,							// alignedByteOffset (uint32_t)
				// Data source, instancing part
				0											// instancesPerElement (uint32_t)
			},
			{ // Attribute 2
				// Data destination
				Renderer::VertexAttributeFormat::FLOAT_3,	// vertexAttributeFormat (Renderer::VertexAttributeFormat)
				"Normal",									// name[32] (char)
				"NORMAL",									// semanticName[32] (char)
				0,											// semanticIndex (uint32_t)
				// Data source
				0,											// inputSlot (uint32_t)
				sizeof(float) * (3 + 2),					// alignedByteOffset (uint32_t)
				// Data source, instancing part
				0											// instancesPerElement (uint32_t)
			},

			// Per-instance data
			{ // Attribute 3
				// Data destination
				Renderer::VertexAttributeFormat::FLOAT_4,	// vertexAttributeFormat (Renderer::VertexAttributeFormat)
				"PerInstancePositionTexture",				// name[32] (char)
				"TEXCOORD",									// semanticName[32] (char)
				1,											// semanticIndex (uint32_t)
				// Data source
				1,											// inputSlot (uint32_t)
				0,											// alignedByteOffset (uint32_t)
				// Data source, instancing part
				1											// instancesPerElement (uint32_t)
			},
			{ // Attribute 4
				// Data destination
				Renderer::VertexAttributeFormat::FLOAT_4,	// vertexAttributeFormat (Renderer::VertexAttributeFormat)
				"PerInstanceRotationScale",					// name[32] (char)
				"TEXCOORD",									// semanticName[32] (char)
				2,											// semanticIndex (uint32_t)
				// Data source
				1,											// inputSlot (uint32_t)
				sizeof(float) * 4,							// alignedByteOffset (uint32_t)
				// Data source, instancing part
				1											// instancesPerElement (uint32_t)
			}
		};
		const Renderer::VertexAttributes VertexAttributes(glm::countof(VertexAttributesLayout), VertexAttributesLayout);


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
CubeRendererInstancedArrays::CubeRendererInstancedArrays(Renderer::IRenderer &renderer, uint32_t numberOfTextures, uint32_t sceneRadius) :
	mRenderer(&renderer),
	mNumberOfTextures(numberOfTextures),
	mSceneRadius(sceneRadius),
	mMaximumNumberOfInstancesPerBatch(0),
	mNumberOfBatches(0),
	mBatches(nullptr)
{
	// Create the buffer and texture manager
	mBufferManager = mRenderer->createBufferManager();
	mTextureManager = mRenderer->createTextureManager();

	// Check number of textures (limit of this implementation and renderer limit)
	if (mNumberOfTextures > MAXIMUM_NUMBER_OF_TEXTURES)
	{
		mNumberOfTextures = MAXIMUM_NUMBER_OF_TEXTURES;
	}

	// Get the maximum number of instances per batch
	// -> When using instanced arrays, the limit is the available memory for a vertex buffer
	// -> To be on the safe side and not bumping into a limitation of less capable cards we set a decent maximum number of instances per batch
	mMaximumNumberOfInstancesPerBatch = 65536;

	{ // Create the textures
		static const uint32_t TEXTURE_WIDTH   = 128;
		static const uint32_t TEXTURE_HEIGHT  = 128;
		static const uint32_t NUMBER_OF_BYTES = TEXTURE_WIDTH * TEXTURE_HEIGHT * 4;

		// Allocate memory for the 2D texture
		uint8_t *data = new uint8_t[NUMBER_OF_BYTES * mNumberOfTextures];

		{ // Fill the texture content
			// TODO(co) Be a little bit more creative while filling the texture data
			uint8_t *dataCurrent = data;
			const float colors[][MAXIMUM_NUMBER_OF_TEXTURES] =
			{
				{ 1.0f, 0.0f, 0.0f},
				{ 0.0f, 0.1f, 0.0f},
				{ 0.0f, 0.0f, 0.1f},
				{ 0.5f, 0.5f, 0.5f},
				{ 1.0f, 1.0f, 1.0f},
				{ 0.1f, 0.2f, 0.2f},
				{ 0.2f, 0.5f, 0.5f},
				{ 0.1f, 0.8f, 0.2f}
			};
			for (uint32_t j = 0; j < mNumberOfTextures; ++j)
			{
				// Random content
				for (uint32_t i = 0; i < TEXTURE_WIDTH * TEXTURE_HEIGHT; ++i)
				{
					*dataCurrent = static_cast<uint8_t>((rand() % 255) * colors[j][0]);
					++dataCurrent;
					*dataCurrent = static_cast<uint8_t>((rand() % 255) * colors[j][1]);
					++dataCurrent;
					*dataCurrent = static_cast<uint8_t>((rand() % 255) * colors[j][2]);
					++dataCurrent;
					*dataCurrent = 255;
					++dataCurrent;
				}
			}
		}

		// Create the texture instance
		// -> This implementation has to support Direct3D 9 which has no 2D array textures
		// -> We simply create a single simple 2D texture atlas with the textures aligned along the vertical axis
		mTexture2D = mTextureManager->createTexture2D(TEXTURE_WIDTH, TEXTURE_HEIGHT * mNumberOfTextures, Renderer::TextureFormat::R8G8B8A8, data, Renderer::TextureFlag::GENERATE_MIPMAPS);

		// Free texture memory
		delete [] data;
	}

	// Create sampler state
	mSamplerState = mRenderer->createSamplerState(Renderer::ISamplerState::getDefaultSamplerState());

	{ // Create the root signature
		Renderer::DescriptorRangeBuilder ranges[5];
		ranges[0].initialize(Renderer::DescriptorRangeType::UBV, 1, 0, "UniformBlockStaticVs", 0);
		ranges[1].initialize(Renderer::DescriptorRangeType::UBV, 1, 1, "UniformBlockDynamicVs", 0);
		ranges[2].initializeSampler(1, 0);
		ranges[3].initialize(Renderer::DescriptorRangeType::SRV, 1, 0, "DiffuseMap", 2);
		ranges[4].initialize(Renderer::DescriptorRangeType::UBV, 1, 0, "UniformBlockDynamicFs", 0);

		Renderer::RootParameterBuilder rootParameters[5];
		rootParameters[0].initializeAsDescriptorTable(1, &ranges[0], Renderer::ShaderVisibility::VERTEX);
		rootParameters[1].initializeAsDescriptorTable(1, &ranges[1], Renderer::ShaderVisibility::VERTEX);
		rootParameters[2].initializeAsDescriptorTable(1, &ranges[2], Renderer::ShaderVisibility::FRAGMENT);
		rootParameters[3].initializeAsDescriptorTable(1, &ranges[3], Renderer::ShaderVisibility::FRAGMENT);
		rootParameters[4].initializeAsDescriptorTable(1, &ranges[4], Renderer::ShaderVisibility::FRAGMENT);

		// Setup
		Renderer::RootSignatureBuilder rootSignature;
		rootSignature.initialize(glm::countof(rootParameters), rootParameters, 0, nullptr, Renderer::RootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		// Create the instance
		mRootSignature = mRenderer->createRootSignature(rootSignature);
	}

	// Uniform buffer object (UBO, "constant buffer" in Direct3D terminology) supported?
	// -> If they are there, we really want to use them (performance and ease of use)
	if (mRenderer->getCapabilities().maximumUniformBufferSize > 0)
	{
		{ // Create and set constant program uniform buffer at once
			// TODO(co) Ugly fixed hacked in model-view-projection matrix
			// TODO(co) OpenGL matrix, Direct3D has minor differences within the projection matrix we have to compensate
			static const float MVP[] =
			{
				 1.2803299f,	-0.97915620f,	-0.58038759f,	-0.57922798f,
				 0.0f,			 1.9776078f,	-0.57472473f,	-0.573576453f,
				-1.2803299f,	-0.97915620f,	-0.58038759f,	-0.57922798f,
				 0.0f,			 0.0f,			 9.8198195f,	 10.0f
			};
			mUniformBufferStaticVs = mBufferManager->createUniformBuffer(sizeof(MVP), MVP, Renderer::BufferUsage::STATIC_DRAW);
		}

		// Create dynamic uniform buffers
		mUniformBufferDynamicVs = mBufferManager->createUniformBuffer(sizeof(float) * 2, nullptr, Renderer::BufferUsage::DYNAMIC_DRAW);
		mUniformBufferDynamicFs = mBufferManager->createUniformBuffer(sizeof(float) * 3, nullptr, Renderer::BufferUsage::DYNAMIC_DRAW);
	}

	// Create the program: Decide which shader language should be used (for example "GLSL" or "HLSL")
	Renderer::IShaderLanguagePtr shaderLanguage(mRenderer->getShaderLanguage());
	if (nullptr != shaderLanguage)
	{
		// Get the shader source code (outsourced to keep an overview)
		const char *vertexShaderSourceCode = nullptr;
		const char *fragmentShaderSourceCode = nullptr;
		#include "CubeRendererInstancedArrays_GLSL_130.h"
		#include "CubeRendererInstancedArrays_GLSL_140.h"
		#include "CubeRendererInstancedArrays_GLSL_ES3.h"
		#include "CubeRendererInstancedArrays_HLSL_D3D10_D3D11_D3D12.h"
		#include "CubeRendererInstancedArrays_HLSL_D3D9.h"
		#include "CubeRendererInstancedArrays_Null.h"

		// Create the program
		mProgram = shaderLanguage->createProgram(
			*mRootSignature,
			detail::VertexAttributes,
			shaderLanguage->createVertexShaderFromSourceCode(detail::VertexAttributes, vertexShaderSourceCode),
			shaderLanguage->createFragmentShaderFromSourceCode(fragmentShaderSourceCode));
	}

	{ // Create the vertex buffer object (VBO)
		static const float VERTEX_POSITION[] =
		{
			// Front face
			// Position					TexCoord		 Normal				// Vertex ID
			-0.5f, -0.5f,  0.5f,		0.0f, 0.0f,		 0.0f, 0.0f, 1.0f,	// 0
			 0.5f, -0.5f,  0.5f,		1.0f, 0.0f,		 0.0f, 0.0f, 1.0f,	// 1
			 0.5f,  0.5f,  0.5f,		1.0f, 1.0f,		 0.0f, 0.0f, 1.0f,	// 2
			-0.5f,  0.5f,  0.5f,		0.0f, 1.0f,		 0.0f, 0.0f, 1.0f,	// 3
			// Back face
			-0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		 0.0f, 0.0f,-1.0f,	// 4
			-0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		 0.0f, 0.0f,-1.0f,	// 5
			 0.5f,  0.5f, -0.5f,		0.0f, 1.0f,		 0.0f, 0.0f,-1.0f, 	// 6
			 0.5f, -0.5f, -0.5f,		0.0f, 0.0f,		 0.0f, 0.0f,-1.0f,	// 7
			// Top face
			-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,		 0.0f, 1.0f, 0.0f,	// 8
			-0.5f,  0.5f,  0.5f,		0.0f, 0.0f,		 0.0f, 1.0f, 0.0f,	// 9
			 0.5f,  0.5f,  0.5f,		1.0f, 0.0f,		 0.0f, 1.0f, 0.0f,	// 10
			 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		 0.0f, 1.0f, 0.0f,	// 11
			// Bottom face
			-0.5f, -0.5f, -0.5f,		1.0f, 1.0f,		 0.0f,-1.0f, 0.0f,	// 12
			 0.5f, -0.5f, -0.5f,		0.0f, 1.0f,		 0.0f,-1.0f, 0.0f,	// 13
			 0.5f, -0.5f,  0.5f,		0.0f, 0.0f,		 0.0f,-1.0f, 0.0f,	// 14
			-0.5f, -0.5f,  0.5f,		1.0f, 0.0f,		 0.0f,-1.0f, 0.0f,	// 15
			// Right face
			 0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		 1.0f, 0.0f, 0.0f,	// 16
			 0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		 1.0f, 0.0f, 0.0f,	// 17
			 0.5f,  0.5f,  0.5f,		0.0f, 1.0f,		 1.0f, 0.0f, 0.0f,	// 18
			 0.5f, -0.5f,  0.5f,		0.0f, 0.0f,		 1.0f, 0.0f, 0.0f,	// 19
			// Left face
			-0.5f, -0.5f, -0.5f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	// 20
			-0.5f, -0.5f,  0.5f,		1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,	// 21
			-0.5f,  0.5f,  0.5f,		1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,	// 22
			-0.5f,  0.5f, -0.5f,		0.0f, 1.0f,		-1.0f, 0.0f, 0.0f	// 23
		};
		mVertexBuffer = mBufferManager->createVertexBuffer(sizeof(VERTEX_POSITION), VERTEX_POSITION, Renderer::BufferUsage::STATIC_DRAW);
	}

	{ // Create the index buffer object (IBO)
		static const uint16_t INDICES[] =
		{
			// Front face	Triangle ID
			 1,  0,  2,		// 0
			 3,  2,  0,		// 1
			// Back face
			 6,  5,  4,		// 2
			 4,  7,  6,		// 3
			// Top face
			 9,  8, 10,		// 4
			11, 10,  8,		// 5
			// Bottom face
			13, 12, 14,		// 6
			15, 14, 12,		// 7
			// Right face
			17, 16, 18,		// 8
			19, 18, 16,		// 9
			// Left face
			21, 20, 22,		// 10
			23, 22, 20		// 11
		};
		mIndexBuffer = mBufferManager->createIndexBuffer(sizeof(INDICES), Renderer::IndexBufferFormat::UNSIGNED_SHORT, INDICES, Renderer::BufferUsage::STATIC_DRAW);
	}
}

CubeRendererInstancedArrays::~CubeRendererInstancedArrays()
{
	// The renderer resource pointers are released automatically

	// Destroy the batches
	if (nullptr != mBatches)
	{
		delete [] mBatches;
	}
}


//[-------------------------------------------------------]
//[ Public virtual ICubeRenderer methods                  ]
//[-------------------------------------------------------]
void CubeRendererInstancedArrays::setNumberOfCubes(uint32_t numberOfCubes)
{
	// Destroy previous batches, in case there are any
	if (nullptr != mBatches)
	{
		delete [] mBatches;
		mNumberOfBatches = 0;
		mBatches = nullptr;
	}

	// A third of the cubes should be rendered using alpha blending
	const uint32_t numberOfTransparentCubes = numberOfCubes / 3;
	const uint32_t numberOfSolidCubes       = numberOfCubes - numberOfTransparentCubes;

	// There's a limitation how many instances can be created per draw call
	// -> If required, create multiple batches
	const uint32_t numberOfSolidBatches       = static_cast<uint32_t>(ceil(static_cast<float>(numberOfSolidCubes)       / mMaximumNumberOfInstancesPerBatch));
	const uint32_t numberOfTransparentBatches = static_cast<uint32_t>(ceil(static_cast<float>(numberOfTransparentCubes) / mMaximumNumberOfInstancesPerBatch));

	// Create a batch instances
	mNumberOfBatches = numberOfSolidBatches + numberOfTransparentBatches;
	mBatches = new BatchInstancedArrays[mNumberOfBatches];

	// Initialize the solid batches
	BatchInstancedArrays *batch     = mBatches;
	BatchInstancedArrays *lastBatch = mBatches + numberOfSolidBatches;
	for (int remaningNumberOfCubes = static_cast<int>(numberOfSolidCubes); batch < lastBatch; ++batch, remaningNumberOfCubes -= mMaximumNumberOfInstancesPerBatch)
	{
		const uint32_t currentNumberOfCubes = (remaningNumberOfCubes > static_cast<int>(mMaximumNumberOfInstancesPerBatch)) ? mMaximumNumberOfInstancesPerBatch : remaningNumberOfCubes;
		batch->initialize(*mBufferManager, *mRootSignature, detail::VertexAttributes, *mVertexBuffer, *mIndexBuffer, *mProgram, currentNumberOfCubes, false, mNumberOfTextures, mSceneRadius);
	}

	// Initialize the transparent batches
	// -> TODO(co) For correct alpha blending, the transparent instances should be sorted from back to front.
	lastBatch = batch + numberOfTransparentBatches;
	for (int remaningNumberOfCubes = static_cast<int>(numberOfTransparentCubes); batch < lastBatch; ++batch, remaningNumberOfCubes -= mMaximumNumberOfInstancesPerBatch)
	{
		const uint32_t currentNumberOfCubes = (remaningNumberOfCubes > static_cast<int>(mMaximumNumberOfInstancesPerBatch)) ? mMaximumNumberOfInstancesPerBatch : remaningNumberOfCubes;
		batch->initialize(*mBufferManager, *mRootSignature, detail::VertexAttributes, *mVertexBuffer, *mIndexBuffer, *mProgram, currentNumberOfCubes, true, mNumberOfTextures, mSceneRadius);
	}

	// Since we're always submitting the same commands to the renderer, we can fill the command buffer once during initialization and then reuse it multiple times during runtime
	mCommandBuffer.clear();
	fillCommandBuffer();
}

void CubeRendererInstancedArrays::draw(float globalTimer, float globalScale, float lightPositionX, float lightPositionY, float lightPositionZ)
{
	// Sanity checks
	assert(nullptr != mProgram);

	{ // Update program uniform data
		// Some counting timer, we don't want to touch the buffers on the GPU
		const float timerAndGlobalScale[] = { globalTimer, globalScale };

		// Animate the point light world space position
		const float lightPosition[] = { lightPositionX, lightPositionY, lightPositionZ };

		// Use uniform buffer?
		if (nullptr != mUniformBufferDynamicVs)
		{
			// Copy data into the uniform buffer
			mUniformBufferDynamicVs->copyDataFrom(sizeof(timerAndGlobalScale), timerAndGlobalScale);
			if (nullptr != mUniformBufferDynamicFs)
			{
				// Copy data into the uniform buffer
				mUniformBufferDynamicFs->copyDataFrom(sizeof(lightPosition), lightPosition);
			}
		}
		else
		{
			// Set individual program uniforms
			// -> Using uniform buffers (aka constant buffers in Direct3D) would be more efficient, but Direct3D 9 doesn't support it (neither does e.g. OpenGL ES 2.0)
			// -> To keep it simple in here, I just use a less performant string to identity the uniform (does not really hurt in here)
			// TODO(co) Update
			// mProgram->setUniform2fv(mProgram->getUniformHandle("TimerAndGlobalScale"), timerAndGlobalScale);
			// mProgram->setUniform3fv(mProgram->getUniformHandle("LightPosition"), lightPosition);
		}
	}

	// Set constant program uniform
	if (nullptr == mUniformBufferStaticVs)
	{
		// TODO(co) Ugly fixed hacked in model-view-projection matrix
		// TODO(co) OpenGL matrix, Direct3D has minor differences within the projection matrix we have to compensate
		static const float MVP[] =
		{
				1.2803299f,	-0.97915620f,	-0.58038759f,	-0.57922798f,
				0.0f,			 1.9776078f,	-0.57472473f,	-0.573576453f,
			-1.2803299f,	-0.97915620f,	-0.58038759f,	-0.57922798f,
				0.0f,			 0.0f,			 9.8198195f,	 10.0f
		};

		// There's no uniform buffer: We have to set individual uniforms
		// TODO(co) Update
		// mProgram->setUniformMatrix4fv(mProgram->getUniformHandle("MVP"), MVP);
	}

	// Submit command buffer to the renderer backend
	mCommandBuffer.submit(*mRenderer);
}


//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void CubeRendererInstancedArrays::fillCommandBuffer()
{
	// Sanity checks
	assert(nullptr != mRootSignature);
	assert(nullptr != mUniformBufferStaticVs);
	assert(nullptr != mUniformBufferDynamicVs);
	assert(nullptr != mSamplerState);
	assert(nullptr != mTexture2D);
	assert(nullptr != mUniformBufferDynamicFs);
	assert(mCommandBuffer.isEmpty());

	// Begin debug event
	COMMAND_BEGIN_DEBUG_EVENT_FUNCTION(mCommandBuffer)

	// Set the used graphics root signature
	Renderer::Command::SetGraphicsRootSignature::create(mCommandBuffer, mRootSignature);

	// Set diffuse map
	Renderer::Command::SetGraphicsRootDescriptorTable::create(mCommandBuffer, 0, mUniformBufferStaticVs);
	Renderer::Command::SetGraphicsRootDescriptorTable::create(mCommandBuffer, 1, mUniformBufferDynamicVs);
	Renderer::Command::SetGraphicsRootDescriptorTable::create(mCommandBuffer, 2, mSamplerState);
	Renderer::Command::SetGraphicsRootDescriptorTable::create(mCommandBuffer, 3, mTexture2D);
	Renderer::Command::SetGraphicsRootDescriptorTable::create(mCommandBuffer, 4, mUniformBufferDynamicFs);

	{ // Setup input assembly (IA)
		// Set the primitive topology used for draw calls
		Renderer::Command::SetPrimitiveTopology::create(mCommandBuffer, Renderer::PrimitiveTopology::TRIANGLE_LIST);
	}

	// Draw the batches
	if (nullptr != mBatches)
	{
		// Loop though all batches
		BatchInstancedArrays *batch     = mBatches;
		BatchInstancedArrays *lastBatch = mBatches + mNumberOfBatches;
		for (; batch < lastBatch; ++batch)
		{
			// Draw this batch
			batch->fillCommandBuffer(mCommandBuffer);
		}
	}

	// End debug event
	COMMAND_END_DEBUG_EVENT(mCommandBuffer)
}
