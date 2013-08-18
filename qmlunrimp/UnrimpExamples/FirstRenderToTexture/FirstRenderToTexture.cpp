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


#include "FirstRenderToTexture.h"
#include <cstring>

void FirstRenderToTexture::Init(Renderer::IRendererPtr renderer)
{
	ExampleBase::Init(renderer);

	if (nullptr != renderer)
	{
		// Begin debug event
		RENDERER_BEGIN_DEBUG_EVENT_FUNCTION(renderer)

		// Create the texture instance, but without providing texture data (we use the texture as render target)
		// -> Use the "Renderer::TextureFlag::RENDER_TARGET"-flag to mark this texture as a render target
		// -> Required for Direct3D 9, Direct3D 10 and Direct3D 11
		// -> Not required for OpenGL and OpenGL ES 2
		Renderer::ITexture *texture2D = mTexture2D = renderer->createTexture2D(16, 16, Renderer::TextureFormat::R8G8B8A8, nullptr, Renderer::TextureFlag::RENDER_TARGET);

		// Create the framebuffer object (FBO) instance
		mFramebuffer = renderer->createFramebuffer(1, &texture2D);

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

		// Decide which shader language should be used (for example "GLSL", "HLSL" or "Cg")
		Renderer::IShaderLanguagePtr shaderLanguage(renderer->getShaderLanguage());
		if (nullptr != shaderLanguage)
		{
			{ // Create the program
				// Get the shader source code (outsourced to keep an overview)
				const char *vertexShaderSourceCode = nullptr;
				const char *fragmentShaderSourceCode = nullptr;
				#include "FirstRenderToTexture_GLSL_110.h"

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
 

void FirstRenderToTexture::Deinit()
{
	mVertexArray = nullptr;
	mProgram = nullptr;
	mSamplerState = nullptr;
	mFramebuffer = nullptr;
	mTexture2D = nullptr;
}

void FirstRenderToTexture::Render()
{
	Renderer::IRendererPtr renderer(getRenderer());

	if (nullptr != renderer && nullptr != mProgram)
	{
		// Begin debug event
		RENDERER_BEGIN_DEBUG_EVENT_FUNCTION(renderer)

		{ // Render to texture
			// Begin debug event
			RENDERER_BEGIN_DEBUG_EVENT(renderer, L"Render to texture")

			// This in here is of course just an example. In a real application
			// there would be no point in constantly updating texture content
			// without having any real change.

			// Unbind our texture from the texture unit before rendering into it
			// -> Direct3D 9, OpenGL and OpenGL ES 2 don't mind as long as the texture is not used inside the shader while rendering into it
			// -> Direct3D 10 & 11 go crazy if you're going to render into a texture which is still bound at a texture unit:
			//    "D3D11: WARNING: ID3D11DeviceContext::OMSetRenderTargets: Resource being set to OM RenderTarget slot 0 is still bound on input! [ STATE_SETTING WARNING #9: DEVICE_OMSETRENDERTARGETS_HAZARD ]"
			//    "D3D11: WARNING: ID3D11DeviceContext::OMSetRenderTargets[AndUnorderedAccessViews]: Forcing PS shader resource slot 0 to NULL. [ STATE_SETTING WARNING #7: DEVICE_PSSETSHADERRESOURCES_HAZARD ]"
			renderer->fsSetTexture(mProgram->setTextureUnit(mProgram->getUniformHandle("DiffuseMap"), 0), nullptr);

			// Backup the currently used render target
			Renderer::IRenderTargetPtr renderTarget(renderer->omGetRenderTarget());

			// Set the render target to render into
			renderer->omSetRenderTarget(mFramebuffer);

			// Clear the color buffer of the current render target with green
			renderer->clear(Renderer::ClearFlag::COLOR, Color4::GREEN, 1.0f, 0);

			// Restore the previously set render target
			renderer->omSetRenderTarget(renderTarget);

			// End debug event
			RENDERER_END_DEBUG_EVENT(renderer)
		}

		{ // Use the render to texture result
			// Begin debug event
			RENDERER_BEGIN_DEBUG_EVENT(renderer, L"Use the render to texture result")

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

		// End debug event
		RENDERER_END_DEBUG_EVENT(renderer)
	}
}
