/*
 *
 */

#include "FirstTriangle.h"
#include <cstring>

void FirstTriangle::Init(Renderer::IRendererPtr renderer)
{
	Renderer::IShaderLanguagePtr shaderLanguage(renderer->getShaderLanguage());
	if (nullptr != shaderLanguage)
	{
		{ // Create the program
			// Get the shader source code (outsourced to keep an overview)
			const char *vertexShaderSourceCode = nullptr;
			const char *fragmentShaderSourceCode = nullptr;
			#include "FirstTriangle_GLSL_110.h"
			
			// Create the vertex shader
			Renderer::IVertexShader *vertexShader = shaderLanguage->createVertexShader(vertexShaderSourceCode);
			
			// Create the fragment shader
			Renderer::IFragmentShader *fragmentShader = shaderLanguage->createFragmentShader(fragmentShaderSourceCode);
			
			// Create the program
			m_Program = shaderLanguage->createProgram(vertexShader, fragmentShader);
		}

		// Is there a valid program?
		if (nullptr != m_Program)
		{
			// Create the vertex buffer object (VBO)
			// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
// 				static const float VERTEX_POSITION[] =
// 				{					// Vertex ID	Triangle on screen
// 					 0.0f, 1.0f,	// 0				0
// 					 1.0f, 0.0f,	// 1			   .   .
// 					 -0.5f, 0.0f	// 2			  2.......1
// 				};
			
			static const float VERTEX_POSITION[] =
			{					// Vertex ID	Triangle on screen
					0.0f, 1.0f,	// 0				0
					1.0f, 0.0f,	// 1			   .   .
					-0.5f, 0.0f	// 2			  2.......1
			};
			
			static const  float points[] = {
				0.0f,  0.5f,
				0.5f, -0.5f, 
				-0.5f, -0.5f,
			};
			
			Renderer::IVertexBufferPtr vertexBuffer(renderer->createVertexBuffer(sizeof(points), points, Renderer::BufferUsage::STATIC_DRAW));
			
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
			m_VertexArray = m_Program->createVertexArray(sizeof(vertexArray) / sizeof(Renderer::VertexArrayAttribute), vertexArray);
		}
	}
}
 

void FirstTriangle::Deinit()
{
	m_VertexArray = nullptr;
	m_Program = nullptr;
}

void FirstTriangle::Render(Renderer::IRendererPtr renderer)
{
	// Clear the color buffer of the current render target with green
	float color_green[4] = {1.0f, 1.0f, 0.0f, 1.0f};
	renderer->clear(Renderer::ClearFlag::COLOR, color_green, 1.0f, 0);
	
	if (renderer->beginScene())
	{
		// Set the used program
		renderer->setProgram(m_Program);

		{ // Setup input assembly (IA)
			// Set the used vertex array
			renderer->iaSetVertexArray(m_VertexArray);

			// Set the primitive topology used for draw calls
			renderer->iaSetPrimitiveTopology(Renderer::PrimitiveTopology::TRIANGLE_LIST);
		}

		// Render the specified geometric primitive, based on an array of vertices
		renderer->draw(0, 3);

		// End scene rendering
		// -> Required for Direct3D 9
		// -> Not required for Direct3D 10, Direct3D 11, OpenGL and OpenGL ES 2
		renderer->endScene();
	}
}
