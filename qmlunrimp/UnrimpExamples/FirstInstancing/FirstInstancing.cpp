/*********************************************************\
 * Copyright (c) 2013-2013 Christian Ofenberg, Stephan Wezel
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


#include "FirstInstancing.h"
#include <cstring>

void FirstInstancing::Init(Renderer::IRendererPtr renderer)
{
	ExampleBase::Init(renderer);
	
	Renderer::IShaderLanguagePtr shaderLanguage(renderer->getShaderLanguage());
	if (nullptr != renderer)
	{
		// Begin debug event
		RENDERER_BEGIN_DEBUG_EVENT_FUNCTION(renderer)

		// Decide which shader language should be used (for example "GLSL", "HLSL" or "Cg")
		Renderer::IShaderLanguagePtr shaderLanguage(renderer->getShaderLanguage());
		if (nullptr != shaderLanguage)
		{
			// There are two instancing aproaches available
			// - Instanced arrays (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
			// - Draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
			//
			// In general, instanced arrays will probably run on the most systems:
			// -> Direct3D 10 and Direct3D 11 support both aproaches
			// -> Direct3D 9 has support for instanced arrays, but does not support draw instanced
			// -> OpenGL 3.1 introduced draw instance ("GL_ARB_draw_instanced"-extension)
			// -> OpenGL 3.3 introduced instance array ("GL_ARB_instanced_arrays"-extension)
			// -> OpenGL ES 2 has no instancing support at all

			// Left side (green): Instanced arrays (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
			if (renderer->getCapabilities().instancedArrays)
			{
				{ // Create the program
					// Get the shader source code (outsourced to keep an overview)
					const char *vertexShaderSourceCode = nullptr;
					const char *fragmentShaderSourceCode = nullptr;
					#include "FirstInstancing_InstancedArrays_GLSL_110.h"

					// Create the program
					mProgramInstancedArrays = shaderLanguage->createProgram(
						shaderLanguage->createVertexShader(vertexShaderSourceCode),
						shaderLanguage->createFragmentShader(fragmentShaderSourceCode));
				}

				// Is there a valid program?
				if (nullptr != mProgramInstancedArrays)
				{
					// Create the vertex buffer object (VBO)
					// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
					static const float VERTEX_POSITION[] =
					{					// Vertex ID	Triangle on screen
						 0.0f, 1.0f,	// 0					 .0
						 0.0f, 0.0f,	// 1				 .    .
						-1.0f, 0.0f		// 2			  2.......1
					};
					Renderer::IVertexBufferPtr vertexBufferPosition(renderer->createVertexBuffer(sizeof(VERTEX_POSITION), VERTEX_POSITION, Renderer::BufferUsage::STATIC_DRAW));

					// Create the per-instance-data vertex buffer object (VBO)
					// -> Simple instance ID in order to keep it similiar to the "draw instanced" version on the right side (blue)
					static const float INSTANCE_ID[] =
					{
						 0.0f, 1.0f
					};
					Renderer::IVertexBufferPtr vertexBufferInstanceId(renderer->createVertexBuffer(sizeof(INSTANCE_ID), INSTANCE_ID, Renderer::BufferUsage::STATIC_DRAW));

					// Create the index buffer object (IBO)
					// -> In this example, we only draw a simple triangle and therefore usually do not need an index buffer
					// -> In Direct3D 9, instanced arrays with hardware support is only possible when drawing indexed primitives, see
					//    "Efficiently Drawing Multiple Instances of Geometry (Direct3D 9)"-article at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb173349%28v=vs.85%29.aspx#Drawing_Non_Indexed_Geometry
					static const unsigned short INDICES[] =
					{
						0, 1, 2
					};
					Renderer::IIndexBuffer *indexBufferInstancedArrays = renderer->createIndexBuffer(sizeof(INDICES), Renderer::IndexBufferFormat::UNSIGNED_SHORT, INDICES, Renderer::BufferUsage::STATIC_DRAW);

					// Create vertex array object (VAO)
					// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
					// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
					// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
					//    reference of the used vertex buffer objects (VBO). If the reference counter of a
					//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.
					const Renderer::VertexArrayAttribute vertexArray[] =
					{
						{ // Attribute 0
							// Data destination
							Renderer::VertexArrayFormat::FLOAT_2,	// vertexArrayFormat (Renderer::VertexArrayFormat::Enum)
							"Position",								// name[64] (char)
							"POSITION",								// semantic[64] (char)
							0,										// semanticIndex (unsigned int)
							// Data source
							vertexBufferPosition,					// vertexBuffer (Renderer::IVertexBuffer *)
							0,										// offset (unsigned int)
							sizeof(float) * 2,						// stride (unsigned int)
							// Data source, instancing part
							0										// instancesPerElement (unsigned int)
						},
						{ // Attribute 1
							// Data destination
							Renderer::VertexArrayFormat::FLOAT_1,	// vertexArrayFormat (Renderer::VertexArrayFormat::Enum)
							"InstanceID",							// name[64] (char)
							"TEXCOORD",								// semantic[64] (char)
							0,										// semanticIndex (unsigned int)
							// Data source
							vertexBufferInstanceId,					// vertexBuffer (Renderer::IVertexBuffer *)
							0,										// offset (unsigned int)
							sizeof(float),							// stride (unsigned int)
							// Data source, instancing part
							1										// instancesPerElement (unsigned int)
						}
					};
					mVertexArrayInstancedArrays = mProgramInstancedArrays->createVertexArray(sizeof(vertexArray) / sizeof(Renderer::VertexArrayAttribute), vertexArray, indexBufferInstancedArrays);
				}
			}

			// Right side (blue): Draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
			if (renderer->getCapabilities().drawInstanced)
			{
				{ // Create the program
					// Get the shader source code (outsourced to keep an overview)
					const char *vertexShaderSourceCode = nullptr;
					const char *fragmentShaderSourceCode = nullptr;
					#include "FirstInstancing_DrawInstanced_GLSL_140.h"

					// Create the program
					mProgramDrawInstanced = shaderLanguage->createProgram(
						shaderLanguage->createVertexShader(vertexShaderSourceCode),
						shaderLanguage->createFragmentShader(fragmentShaderSourceCode));
				}

				// Is there a valid program?
				if (nullptr != mProgramDrawInstanced)
				{
					// Create the vertex buffer object (VBO)
					// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
					static const float VERTEX_POSITION[] =
					{					// Vertex ID	Triangle on screen
						 0.0f, 1.0f,	// 0			  0.	
						 1.0f, 0.0f,	// 1			  .    .
						 0.0f, 0.0f		// 2			  2.......1
					};
					Renderer::IVertexBufferPtr vertexBuffer(renderer->createVertexBuffer(sizeof(VERTEX_POSITION), VERTEX_POSITION, Renderer::BufferUsage::STATIC_DRAW));

					// Create vertex array object (VAO)
					// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
					// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
					// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
					//    reference of the used vertex buffer objects (VBO). If the reference counter of a
					//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.
					const Renderer::VertexArrayAttribute vertexArray[] =
					{
						{ // Attribute 0
							// Data destination
							Renderer::VertexArrayFormat::FLOAT_2,	// vertexArrayFormat (Renderer::VertexArrayFormat::Enum)
							"Position",								// name[64] (char)
							"POSITION",								// semantic[64] (char)
							0,										// semanticIndex (unsigned int)
							// Data source
							vertexBuffer,							// vertexBuffer (Renderer::IVertexBuffer *)
							0,										// offset (unsigned int)
							sizeof(float) * 2,						// stride (unsigned int)
							// Data source, instancing part
							0										// instancesPerElement (unsigned int)
						}
					};
					mVertexArrayDrawInstanced = mProgramDrawInstanced->createVertexArray(sizeof(vertexArray) / sizeof(Renderer::VertexArrayAttribute), vertexArray);
				}
			}
		}

		// End debug event
		RENDERER_END_DEBUG_EVENT(renderer)
	}
}
 

void FirstInstancing::Deinit()
{
	mVertexArrayDrawInstanced = nullptr;
	mProgramDrawInstanced = nullptr;
	mVertexArrayInstancedArrays = nullptr;
	mProgramInstancedArrays = nullptr;
}

void FirstInstancing::Render()
{
	Renderer::IRendererPtr renderer(getRenderer());

	if (nullptr != renderer)
	{
		// Begin debug event
		RENDERER_BEGIN_DEBUG_EVENT_FUNCTION(renderer)

		// Clear the color buffer of the current render target with gray, do also clear the depth buffer
		renderer->clear(Renderer::ClearFlag::COLOR_DEPTH, Color4::GRAY, 1.0f, 0);

		// Begin scene rendering
		// -> Required for Direct3D 9
		// -> Not required for Direct3D 10, Direct3D 11, OpenGL and OpenGL ES 2
		if (renderer->beginScene())
		{
			// Left side (green): Instanced arrays (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
			if (renderer->getCapabilities().instancedArrays)
			{
				// Begin debug event
				RENDERER_BEGIN_DEBUG_EVENT(renderer, L"Draw using instanced arrays")

				// Set the used program
				renderer->setProgram(mProgramInstancedArrays);

				{ // Setup input assembly (IA)
					// Set the used vertex array
					renderer->iaSetVertexArray(mVertexArrayInstancedArrays);

					// Set the primitive topology used for draw calls
					renderer->iaSetPrimitiveTopology(Renderer::PrimitiveTopology::TRIANGLE_LIST);
				}

				// Render the specified geometric primitive, based on an array of vertices
				// -> In this example, we only draw a simple triangle and therefore usually do not need an index buffer
				// -> In Direct3D 9, instanced arrays with hardware support is only possible when drawing indexed primitives, see
				//    "Efficiently Drawing Multiple Instances of Geometry (Direct3D 9)"-article at MSDN: http://msdn.microsoft.com/en-us/library/windows/desktop/bb173349%28v=vs.85%29.aspx#Drawing_Non_Indexed_Geometry
				renderer->drawIndexedInstanced(0, 3, 0, 0, 3, 2);

				// End debug event
				RENDERER_END_DEBUG_EVENT(renderer)
			}

			// Right side (blue): Draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
			if (renderer->getCapabilities().drawInstanced)
			{
				// Begin debug event
				RENDERER_BEGIN_DEBUG_EVENT(renderer, L"Draw instanced")

				// Set the used program
				renderer->setProgram(mProgramDrawInstanced);

				{ // Setup input assembly (IA)
					// Set the used vertex array
					renderer->iaSetVertexArray(mVertexArrayDrawInstanced);

					// Set the primitive topology used for draw calls
					renderer->iaSetPrimitiveTopology(Renderer::PrimitiveTopology::TRIANGLE_LIST);
				}

				// Render the specified geometric primitive, based on an array of vertices
				renderer->drawInstanced(0, 3, 2);
			}

			// End scene rendering
			// -> Required for Direct3D 9
			// -> Not required for Direct3D 10, Direct3D 11, OpenGL and OpenGL ES 2
			renderer->endScene();

			// End debug event
			RENDERER_END_DEBUG_EVENT(renderer)
		}

		// End debug event
		RENDERER_END_DEBUG_EVENT(renderer)
	}
}
