/*
 *
 */

#include "Fxaa.h"
#include <cstring>

void Fxaa::Init(Renderer::IRendererPtr renderer)
{
	ExampleBase::Init(renderer);
	
	if (nullptr != renderer)
	{
		// Begin debug event
		RENDERER_BEGIN_DEBUG_EVENT_FUNCTION(renderer)

		// Create the framebuffer object (FBO) instance by using the current window size
		recreateFramebuffer();

		{ // Create sampler state
		  // -> Our texture does not have any mipmaps, set "Renderer::SamplerState::maxLOD" to zero
		  //    in order to ensure a correct behaviour across the difference graphics APIs
		  // -> When not doing this you usually have no issues when using OpenGL, OpenGL ES 2, Direct 10,
		  //    Direct3D 11 or Direct3D 9 with the "ps_2_0"-profile, but when using Direct3D 9 with the
		  //    "ps_3_0"-profile you might get into trouble due to another internal graphics API behaviour
			Renderer::SamplerState samplerState = Renderer::ISamplerState::getDefaultSamplerState();
			samplerState.maxLOD = 0.0f;	// We don't use mipmaps
			mSamplerState = renderer->createSamplerState(samplerState);
		}

		{ // Depth stencil state
		  // -> By default depth test is enabled
		  // -> In this simple example we don't need depth test, so, disable it so we don't need to care about the depth buffer

			// Create depth stencil state
			Renderer::DepthStencilState depthStencilState = Renderer::IDepthStencilState::getDefaultDepthStencilState();
			depthStencilState.depthEnable = false;
			mDepthStencilState = renderer->createDepthStencilState(depthStencilState);

			// Set the depth stencil state directly within this initialization phase, we don't change it later on
			renderer->omSetDepthStencilState(mDepthStencilState);
		}

		// Decide which shader language should be used (for example "GLSL", "HLSL" or "Cg")
		Renderer::IShaderLanguagePtr shaderLanguage(renderer->getShaderLanguage());
		if (nullptr != shaderLanguage)
		{
			{ // Create the program for scene rendering
				// Get the shader source code (outsourced to keep an overview)
				const char *vertexShaderSourceCode = nullptr;
				const char *fragmentShaderSourceCode = nullptr;
				#include "Fxaa_SceneRendering_GLSL_120.h"

				// Create the program for scene rendering
				mProgramSceneRendering = shaderLanguage->createProgram(shaderLanguage->createVertexShader(vertexShaderSourceCode), shaderLanguage->createFragmentShader(fragmentShaderSourceCode));
			}

			// Is there a valid program for scene rendering?
			if (nullptr != mProgramSceneRendering)
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
				mVertexArraySceneRendering = mProgramSceneRendering->createVertexArray(sizeof(vertexArray) / sizeof(Renderer::VertexArrayAttribute), vertexArray);
			}

			// Create the post-processing program instance by using the current window size
			recreatePostProcessingProgram();

			// Is there a valid program for post-processing?
			if (nullptr != mProgramPostProcessing)
			{
				// Create the vertex buffer object (VBO)
				// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
				static const float VERTEX_POSITION[] =
				{					// Vertex ID	Triangle strip on screen
					-1.0f, -1.0f,	// 0			  1.......3
					-1.0f,  1.0f,	// 1			  .	  .   .
					 1.0f, -1.0f,	// 2			  0.......2
					 1.0f,  1.0f	// 3
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
				mVertexArrayPostProcessing = mProgramSceneRendering->createVertexArray(sizeof(vertexArray) / sizeof(Renderer::VertexArrayAttribute), vertexArray);
			}
		}

		// End debug event
		RENDERER_END_DEBUG_EVENT(renderer)
	}
}
 

void Fxaa::Deinit()
{
	// Release the used resources
	mVertexArrayPostProcessing = nullptr;
	mProgramPostProcessing = nullptr;
	mVertexArraySceneRendering = nullptr;
	mProgramSceneRendering = nullptr;
	mDepthStencilState = nullptr;
	mSamplerState = nullptr;
	mFramebuffer = nullptr;
	mTexture2D = nullptr;
}

void Fxaa::Render()
{
	Renderer::IRendererPtr renderer(getRenderer());

	if (nullptr != renderer)
	{
		// Begin debug event
		RENDERER_BEGIN_DEBUG_EVENT_FUNCTION(renderer)

		// Scene rendering
		sceneRendering();

		// Post-processing
		postProcessing();

		// End debug event
		RENDERER_END_DEBUG_EVENT(renderer)
	}
}

void Fxaa::onSizeChanged()
{
	// Recreate the framebuffer object (FBO) instance by using the current window size
	recreateFramebuffer();

	// Recreate the post-processing program instance by using the current window size
	recreatePostProcessingProgram();
}

//[-------------------------------------------------------]
//[ Private methods                                       ]
//[-------------------------------------------------------]
void Fxaa::recreateFramebuffer()
{
	// Get and check the renderer instance
	Renderer::IRendererPtr renderer(getRenderer());
	if (nullptr != renderer)
	{
		// Begin debug event
		RENDERER_BEGIN_DEBUG_EVENT_FUNCTION(renderer)

		// Get the window size
		int width  = this->mWidth;
		int height = this->mHeigth;

		// Due to the usage of smart pointers there's no need to explicitly free the previous resources, this is done automatically

		// Create the texture instance, but without providing texture data (we use the texture as render target)
		// -> Use the "Renderer::TextureFlag::RENDER_TARGET"-flag to mark this texture as a render target
		// -> Required for Direct3D 9, Direct3D 10 and Direct3D 11
		// -> Not required for OpenGL and OpenGL ES 2
		Renderer::ITexture *texture2D = mTexture2D = renderer->createTexture2D(static_cast<unsigned int>(width), static_cast<unsigned int>(height), Renderer::TextureFormat::R8G8B8A8, nullptr, Renderer::TextureFlag::RENDER_TARGET);

		// Create the framebuffer object (FBO) instance
		mFramebuffer = renderer->createFramebuffer(1, &texture2D);

		// End debug event
		RENDERER_END_DEBUG_EVENT(renderer)
	}
}

void Fxaa::recreatePostProcessingProgram()
{
	// Get and check the renderer instance
	Renderer::IRendererPtr renderer(getRenderer());
	if (nullptr != renderer)
	{
		// Begin debug event
		RENDERER_BEGIN_DEBUG_EVENT_FUNCTION(renderer)

		// Decide which shader language should be used (for example "GLSL", "HLSL" or "Cg")
		Renderer::IShaderLanguagePtr shaderLanguage(renderer->getShaderLanguage());
		if (nullptr != shaderLanguage)
		{
			// Get the shader source code (outsourced to keep an overview)
			const char *vertexShaderSourceCode = nullptr;
			const char *fragmentShaderSourceCode_Definitions = nullptr;
			const char *fragmentShaderSourceCode = nullptr;
			#include "Fxaa_PostProcessing_GLSL_120.h"
			#include "Fxaa_PostProcessing.h"

			// Get the window size
			int width  = this->mWidth;
			int height = this->mHeigth;

			// The FXAA shader comments state: "RCPFRAME SHOULD PIXEL SHADER CONSTANTS"
			char dynamicDefinition[256];
			dynamicDefinition[0] = '\0';
			if (0 == strcmp(shaderLanguage->getShaderLanguageName(), "Cg") || 0 == strcmp(renderer->getName(), "Direct3D9") || 0 == strcmp(renderer->getName(), "Direct3D10") || 0 == strcmp(renderer->getName(), "Direct3D11"))
			{
				sprintf(dynamicDefinition, "#define RCPFRAME float2(%ff, %ff)\n", 1.0f / width, 1.0f / height);
			}
			else if (0 == strcmp(renderer->getName(), "OpenGL") || 0 == strcmp(renderer->getName(), "OpenGLES2"))
			{
				sprintf(dynamicDefinition, "#define RCPFRAME vec2(%f, %f)\n", 1.0f / width, 1.0f / height);
			}

			// Compose the fragment shader source code
			const size_t definitionsLength		  = strlen(fragmentShaderSourceCode_Definitions);
			const size_t dynamicDefinitionLength  = strlen(dynamicDefinition);
			const size_t postProcessingLength	  = strlen(fragmentShaderSourceCode);
			const size_t part1Length			  = strlen(fxaa_FS_Part1);
			const size_t part2Length			  = strlen(fxaa_FS_Part2);
			const size_t length					  = definitionsLength + dynamicDefinitionLength + part1Length + part2Length + postProcessingLength + 1;	// +1 for the terminating zero
			char *sourceCode = new char[length];
			char *sourceCodeCurrent = sourceCode;
			// Definitions
			strncpy(sourceCodeCurrent, fragmentShaderSourceCode_Definitions, definitionsLength);
			sourceCodeCurrent += definitionsLength;
			// Dynamic definitions
			strncpy(sourceCodeCurrent, dynamicDefinition, dynamicDefinitionLength);
			sourceCodeCurrent += dynamicDefinitionLength;
			// FXAA fragment shader
			strncpy(sourceCodeCurrent, fxaa_FS_Part1, part1Length);
			sourceCodeCurrent += part1Length;
			strncpy(sourceCodeCurrent, fxaa_FS_Part2, part2Length);
			sourceCodeCurrent += part2Length;
			// Fragment shader shell
			strncpy(sourceCodeCurrent, fragmentShaderSourceCode, postProcessingLength);
			sourceCodeCurrent += postProcessingLength;
			*sourceCodeCurrent = '\0';

			// Due to the usage of smart pointers there's no need to explicitly free the previous resources, this is done automatically

			// Create the program for the FXAA post processing
			mProgramPostProcessing = shaderLanguage->createProgram(shaderLanguage->createVertexShader(vertexShaderSourceCode), shaderLanguage->createFragmentShader(sourceCode));

			// Free the memory
			delete [] sourceCode;
		}

		// End debug event
		RENDERER_END_DEBUG_EVENT(renderer)
	}
}

void Fxaa::sceneRendering()
{
	// Get and check the renderer instance
	Renderer::IRendererPtr renderer(getRenderer());
	if (nullptr != renderer && nullptr != mProgramSceneRendering && nullptr != mProgramPostProcessing)
	{
		// Begin debug event
		RENDERER_BEGIN_DEBUG_EVENT_FUNCTION(renderer)

		// This in here is of course just an example. In a real application
		// there would be no point in constantly updating texture content
		// without having any real change.

		// Unbind our texture from the texture unit before rendering into it
		// -> Direct3D 9, OpenGL and OpenGL ES 2 don't mind as long as the texture is not used inside the shader while rendering into it
		// -> Direct3D 10 & 11 go crazy if you're going to render into a texture which is still bound at a texture unit:
		//    "D3D11: WARNING: ID3D11DeviceContext::OMSetRenderTargets: Resource being set to OM RenderTarget slot 0 is still bound on input! [ STATE_SETTING WARNING #9: DEVICE_OMSETRENDERTARGETS_HAZARD ]"
		//    "D3D11: WARNING: ID3D11DeviceContext::OMSetRenderTargets[AndUnorderedAccessViews]: Forcing PS shader resource slot 0 to NULL. [ STATE_SETTING WARNING #7: DEVICE_PSSETSHADERRESOURCES_HAZARD ]"
		renderer->fsSetTexture(mProgramPostProcessing->setTextureUnit(mProgramPostProcessing->getUniformHandle("DiffuseMap"), 0), nullptr);

		// Backup the currently used render target
		Renderer::IRenderTargetPtr renderTarget(renderer->omGetRenderTarget());

		// Set the render target to render into
		renderer->omSetRenderTarget(mFramebuffer);

		// Clear the color buffer of the current render target with black
		renderer->clear(Renderer::ClearFlag::COLOR, Color4::BLACK, 1.0f, 0);

		// Begin scene rendering
		// -> Required for Direct3D 9
		// -> Not required for Direct3D 10, Direct3D 11, OpenGL and OpenGL ES 2
		if (renderer->beginScene())
		{
			// Set the used program
			renderer->setProgram(mProgramSceneRendering);

			{ // Setup input assembly (IA)
				// Set the used vertex array
				renderer->iaSetVertexArray(mVertexArraySceneRendering);

				// Set the primitive topology used for draw calls
				renderer->iaSetPrimitiveTopology(Renderer::PrimitiveTopology::TRIANGLE_LIST);
			}

			// Render the specified geometric primitive, based on indexing into an array of vertices
			renderer->draw(0, 3);

			// End scene rendering
			// -> Required for Direct3D 9
			// -> Not required for Direct3D 10, Direct3D 11, OpenGL and OpenGL ES 2
			renderer->endScene();
		}

		// Restore the previously set render target
		renderer->omSetRenderTarget(renderTarget);

		// End debug event
		RENDERER_END_DEBUG_EVENT(renderer)
	}
}

void Fxaa::postProcessing()
{
	// Get and check the renderer instance
	Renderer::IRendererPtr renderer(getRenderer());
	if (nullptr != renderer && mProgramPostProcessing)
	{
		// Begin debug event
		RENDERER_BEGIN_DEBUG_EVENT_FUNCTION(renderer)

		// We don't need to clear the current render target because our fullscreen quad covers the full screen

		// Begin scene rendering
		// -> Required for Direct3D 9
		// -> Not required for Direct3D 10, Direct3D 11, OpenGL and OpenGL ES 2
		if (renderer->beginScene())
		{
			// Set the used program
			renderer->setProgram(mProgramPostProcessing);

			{ // Setup input assembly (IA)
				// Set the used vertex array
				renderer->iaSetVertexArray(mVertexArrayPostProcessing);

				// Set the primitive topology used for draw calls
				renderer->iaSetPrimitiveTopology(Renderer::PrimitiveTopology::TRIANGLE_STRIP);
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
				const unsigned int unit = mProgramPostProcessing->setTextureUnit(mProgramPostProcessing->getUniformHandle("DiffuseMap"), 0);

				// Set the used texture at the texture unit
				renderer->fsSetTexture(unit, mTexture2D);

				// Set the used sampler state at the texture unit
				renderer->fsSetSamplerState(unit, mSamplerState);
			}

			// Render the specified geometric primitive, based on indexing into an array of vertices
			renderer->draw(0, 4);

			// End scene rendering
			// -> Required for Direct3D 9
			// -> Not required for Direct3D 10, Direct3D 11, OpenGL and OpenGL ES 2
			renderer->endScene();
		}

		// End debug event
		RENDERER_END_DEBUG_EVENT(renderer)
	}
}
