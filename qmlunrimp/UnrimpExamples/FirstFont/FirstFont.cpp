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


#include "FirstFont.h"
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <cstring>

void FirstFont::Init(Renderer::IRendererPtr renderer)
{
	ExampleRenderToolkit::Init(renderer);
	
	// Get and check the renderer toolkit instance
	RendererToolkit::IRendererToolkitPtr rendererToolkit(getRendererToolkit());
	if (nullptr != rendererToolkit)
	{
		// Begin debug event
		RENDERER_BEGIN_DEBUG_EVENT_FUNCTION(getRenderer())

		// Create the font instance
		// -> In order to keep it simple, we use simple ASCII strings as filenames which are relative to the executable
		mFont = rendererToolkit->createFontTexture("../Data/Font/LinBiolinum_R.otf");

		// End debug event
		RENDERER_END_DEBUG_EVENT(getRenderer())
	}
}
 

void FirstFont::Deinit()
{
	// Begin debug event
	RENDERER_BEGIN_DEBUG_EVENT_FUNCTION(getRenderer())

	// Release the used resources
	mFont = nullptr;

	// End debug event
	RENDERER_END_DEBUG_EVENT(getRenderer())

	ExampleRenderToolkit::Deinit();
}

void FirstFont::Render()
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
			// Draw text
			mFont->drawText("42", Color4::GREEN, glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.3f, 0.0f))), 0.005f, 0.005f);

			// End scene rendering
			// -> Required for Direct3D 9
			// -> Not required for Direct3D 10, Direct3D 11, OpenGL and OpenGL ES 2
			renderer->endScene();
		}

		// End debug event
		RENDERER_END_DEBUG_EVENT(renderer)
	}
}
