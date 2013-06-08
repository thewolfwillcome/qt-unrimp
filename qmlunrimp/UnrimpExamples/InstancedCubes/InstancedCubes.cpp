/*
 *
 */

#include "InstancedCubes.h"
#include "CubeRendererDrawInstanced/CubeRendererDrawInstanced.h"
#include "CubeRendererInstancedArrays/CubeRendererInstancedArrays.h"

#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtc/matrix_transform.hpp>

#include <cstring>

RendererToolkit::IRendererToolkit* createRendererToolkit(Renderer::IRenderer& renderer)
{
	extern RendererToolkit::IRendererToolkit *createRendererToolkitInstance(Renderer::IRenderer &renderer);

	// Create the renderer toolkit instance
	return createRendererToolkitInstance(renderer);
}



InstancedCubes::InstancedCubes()
	:
	mCubeRenderer(nullptr),
	mNumberOfCubeInstances(1000),
	mGlobalTimer(0.0f),
	mGlobalScale(1.0f),
	mDisplayStatistics(true),
	mFPSUpdateTimer(0.0f),
	mFramesSinceCheck(0),
	mFramesPerSecond(0.0f)
{

}


void InstancedCubes::Init(Renderer::IRendererPtr renderer)
{
	ExampleBase::Init(renderer);

	rendererToolkit = createRendererToolkit(*renderer);
	
	if (nullptr != renderer)
	{
		// Begin debug event
		RENDERER_BEGIN_DEBUG_EVENT_FUNCTION(renderer)

		{ // Create the font instance
			// Get and check the renderer toolkit instance
			if (nullptr != rendererToolkit)
			{
				// Create the font instance
				// -> In order to keep it simple, we use simple ASCII strings as filenames which are relative to the executable
				mFont = rendererToolkit->createFontTexture("../Data/Font/LinBiolinum_R.otf");
			}
		}

		// Create the cube renderer instance
		// -> Evaluate the feature set of the used renderer
		if (renderer->getCapabilities().drawInstanced && renderer->getCapabilities().maximumNumberOf2DTextureArraySlices > 0 && renderer->getCapabilities().maximumTextureBufferSize > 0)
		{
			// Render cubes by using draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
			mCubeRenderer = new CubeRendererDrawInstanced(*renderer, NUMBER_OF_TEXTURES, SCENE_RADIUS);
		}
		else if (renderer->getCapabilities().instancedArrays)
		{
			// Render cubes by using instanced arrays (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
			mCubeRenderer = new CubeRendererInstancedArrays(*renderer, NUMBER_OF_TEXTURES, SCENE_RADIUS);
		}

		// Tell the cube renderer about the number of cubes
		if (nullptr != mCubeRenderer)
		{
			mCubeRenderer->setNumberOfCubes(mNumberOfCubeInstances);
		}

		// End debug event
		RENDERER_END_DEBUG_EVENT(renderer)
	}
}
 

void InstancedCubes::Deinit()
{
	// Begin debug event
	RENDERER_BEGIN_DEBUG_EVENT_FUNCTION(getRenderer())

	// Destroy the cube renderer, in case there's one
	if (nullptr != mCubeRenderer)
	{
		delete mCubeRenderer;
		mCubeRenderer = nullptr;
	}

	// Release the used resources
	mFont = nullptr;

	// End debug event
	RENDERER_END_DEBUG_EVENT(getRenderer())
}

void InstancedCubes::Render()
{
	Renderer::IRendererPtr renderer(getRenderer());

	/// Update Timer
	{
		mStopwatch.stop();

		// Update the global timer (FPS independent movement)
		const float timeDifference = mStopwatch.getMilliseconds();
		mGlobalTimer += timeDifference;

		// Calculate the current FPS
		++mFramesSinceCheck;
		mFPSUpdateTimer += timeDifference;
		if (mFPSUpdateTimer > 1000.0f)
		{
			mFramesPerSecond   = static_cast<float>(mFramesSinceCheck / (mFPSUpdateTimer / 1000.0f));
			mFPSUpdateTimer   -= 1000.0f;
			mFramesSinceCheck  = 0;
		}

		// Start the stopwatch
		mStopwatch.start();
	}
	/// Update Timer End
	
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
			// Draw the cubes
			if (nullptr != mCubeRenderer)
			{
				mCubeRenderer->draw(mGlobalTimer, mGlobalScale, sin(mGlobalTimer * 0.001f) * SCENE_RADIUS, sin(mGlobalTimer * 0.0005f) * SCENE_RADIUS, cos(mGlobalTimer * 0.0008f) * SCENE_RADIUS);
			}

			// End scene rendering
			// -> Required for Direct3D 9
			// -> Not required for Direct3D 10, Direct3D 11, OpenGL and OpenGL ES 2
			renderer->endScene();
		}

		// Display statistics and is there a font instance?
		if (mDisplayStatistics && nullptr != mFont)
		{
			// Is there a cube renderer instance?
			if (nullptr != mCubeRenderer)
			{
				char text[128];

				// Number of cubes
				sprintf(text, "Number of cubes: %d", mNumberOfCubeInstances);
				mFont->drawText(text, Color4::WHITE, glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(-0.95f, 0.9f, 0.0f))), 0.0025f, 0.0025f);

				// Frames per second
				sprintf(text, "Frames per second: %.2f", mFramesPerSecond);
				mFont->drawText(text, Color4::WHITE, glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(-0.95f, 0.85f, 0.0f))), 0.0025f, 0.0025f);

				// Cubes per second
				// -> In every frame we draw n-cubes...
				// -> TODO(co) This number can get huge... had over 1 million cubes with >25 FPS... million cubes at ~2.4 FPS...
				sprintf(text, "Cubes per second: %u", static_cast<unsigned int>(mFramesPerSecond) * mNumberOfCubeInstances);
				mFont->drawText(text, Color4::WHITE, glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(-0.95f, 0.8f, 0.0f))), 0.0025f, 0.0025f);
			}
			else
			{
				mFont->drawText("No cube renderer instance", Color4::WHITE, glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(-0.95f, 0.9f, 0.0f))), 0.0025f, 0.0025f);
			}
		}

		// End debug event
		RENDERER_END_DEBUG_EVENT(renderer)
	}
}
