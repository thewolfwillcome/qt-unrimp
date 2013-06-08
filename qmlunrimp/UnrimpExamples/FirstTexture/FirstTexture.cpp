/*
 *
 */

#include "FirstTexture.h"
#include <cstring>

void FirstTexture::Init(Renderer::IRendererPtr renderer)
{
	ExampleBase::Init(renderer);

	Renderer::IShaderLanguagePtr shaderLanguage(renderer->getShaderLanguage());
	if (nullptr != renderer)
	{
		// Begin debug event
		RENDERER_BEGIN_DEBUG_EVENT_FUNCTION(renderer)

		{ // Create the texture
			static const unsigned int TEXTURE_WIDTH   = 128;
			static const unsigned int TEXTURE_HEIGHT  = 128;
			static const unsigned int NUMBER_OF_BYTES = TEXTURE_WIDTH * TEXTURE_HEIGHT * 4;

			// Allocate memory for the texture
			unsigned char *data = new unsigned char[NUMBER_OF_BYTES];

			// TODO(co) Be a little bit more creative while filling the texture data
			// Random content
			unsigned char *dataCurrent = data;
			for (unsigned int i = 0; i < TEXTURE_WIDTH * TEXTURE_HEIGHT; ++i)
			{
				*dataCurrent = static_cast<unsigned char>(rand() % 255);
				++dataCurrent;
				*dataCurrent = static_cast<unsigned char>(rand() % 255);
				++dataCurrent;
				*dataCurrent = static_cast<unsigned char>(rand() % 255);
				++dataCurrent;
				*dataCurrent = static_cast<unsigned char>(rand() % 255);
				++dataCurrent;
			}

			// Create the texture instance
			mTexture2D = renderer->createTexture2D(128, 128, Renderer::TextureFormat::R8G8B8A8, data, Renderer::TextureFlag::MIPMAPS);

			// Free texture memory
			delete [] data;
		}

		{ // Create sampler state
			Renderer::SamplerState samplerState = Renderer::ISamplerState::getDefaultSamplerState();
			samplerState.addressU = Renderer::TextureAddressMode::WRAP;
			samplerState.addressV = Renderer::TextureAddressMode::WRAP;
			mSamplerState = renderer->createSamplerState(samplerState);
		}

		// Decide which shader language should be used (for example "GLSL", "HLSL" or "Cg")
		Renderer::IShaderLanguagePtr shaderLanguage(renderer->getShaderLanguage());
		if (nullptr != shaderLanguage)
		{
			{ // Create the program
				// Get the shader source code (outsourced to keep an overview)
				const char *vertexShaderSourceCode = nullptr;
				const char *fragmentShaderSourceCode = nullptr;
				#include "FirstTexture_GLSL_110.h"

				// Create the program
				mProgram = shaderLanguage->createProgram(
					shaderLanguage->createVertexShader(vertexShaderSourceCode),
					shaderLanguage->createFragmentShader(fragmentShaderSourceCode));
			}

			// Is there a valid program?
			if (nullptr != mProgram)
			{
				// Create the vertex buffer object (VBO)
				// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
				static const float VERTEX_POSITION[] =
				{					// Vertex ID	Triangle on screen
					 0.0f, 1.0f,	// 0				0
					 1.0f, 0.0f,	// 1			   .   .
					-0.5f, 0.0f		// 2			  2.......1
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
				mVertexArray = mProgram->createVertexArray(sizeof(vertexArray) / sizeof(Renderer::VertexArrayAttribute), vertexArray);
			}
		}

		// End debug event
		RENDERER_END_DEBUG_EVENT(renderer)
	}
}
 

void FirstTexture::Deinit()
{
	mVertexArray = nullptr;
	mProgram = nullptr;
	mSamplerState = nullptr;
	mTexture2D = nullptr;
}

void FirstTexture::Render()
{
	Renderer::IRendererPtr renderer(getRenderer());

	if (nullptr != renderer && nullptr != mProgram)
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
			// Set the used program
			renderer->setProgram(mProgram);

			{ // Setup input assembly (IA)
				// Set the used vertex array
				renderer->iaSetVertexArray(mVertexArray);

				// Set the primitive topology used for draw calls
				renderer->iaSetPrimitiveTopology(Renderer::PrimitiveTopology::TRIANGLE_LIST);
			}

			{ // Set diffuse map (texture unit 0 by default)
				// Tell the renderer API which texture should be bound to which texture unit
				// -> When using OpenGL or OpenGL ES 2 this is required
				// -> OpenGL 4.2 supports explicit binding points ("layout(binding=0)" in GLSL shader),
				//    for backward compatibility we don't use it in here
				// -> When using Direct3D 9, Direct3D 10, Direct3D 11 or the Cg shader language, the texture unit
				//    to use is usually defined directly within the shader by using the "register"-keyword
				// -> Usually, this should only be done once during initialization, this example does this
				//    every frame to keep it local for better overview
				const unsigned int unit = mProgram->setTextureUnit(mProgram->getUniformHandle("DiffuseMap"), 0);

				// Set the used texture at the texture unit
				renderer->fsSetTexture(unit, mTexture2D);

				// Set the used sampler state at the texture unit
				renderer->fsSetSamplerState(unit, mSamplerState);
			}

			// Render the specified geometric primitive, based on an array of vertices
			renderer->draw(0, 3);

			// End scene rendering
			// -> Required for Direct3D 9
			// -> Not required for Direct3D 10, Direct3D 11, OpenGL and OpenGL ES 2
			renderer->endScene();
		}

		// End debug event
		RENDERER_END_DEBUG_EVENT(renderer)
	}
}
