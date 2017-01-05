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


#include "UnrimpExamples/InstancedCubes/InstancedCubes.h"
//#include "UnrimpExamples/InstancedCubes/CubeRendererDrawInstanced/CubeRendererDrawInstanced.h"
#include "UnrimpExamples/InstancedCubes/CubeRendererInstancedArrays/CubeRendererInstancedArrays.h"

#include <RendererRuntime/DebugGui/DebugGuiManager.h>

#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtc/matrix_transform.hpp>

#include <cstring>


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
	// Nothing here
}


void InstancedCubes::onInit(Renderer::IRendererPtr renderer)
{
	ExampleRenderRuntime::onInit(renderer);
	
	if (nullptr != renderer)
	{
		// Create the cube renderer instance
		// -> Evaluate the feature set of the used renderer
		/*if (renderer->getCapabilities().drawInstanced && renderer->getCapabilities().maximumNumberOf2DTextureArraySlices > 0 && renderer->getCapabilities().maximumTextureBufferSize > 0)
		{
			// Render cubes by using draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
			mCubeRenderer = new CubeRendererDrawInstanced(*renderer, NUMBER_OF_TEXTURES, SCENE_RADIUS);
		}
		else */if (renderer->getCapabilities().instancedArrays)
		{
			// Render cubes by using instanced arrays (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
			mCubeRenderer = new CubeRendererInstancedArrays(*renderer, NUMBER_OF_TEXTURES, SCENE_RADIUS);
		}

		// Tell the cube renderer about the number of cubes
		if (nullptr != mCubeRenderer)
		{
			mCubeRenderer->setNumberOfCubes(mNumberOfCubeInstances);
		}
	}
}
 

void InstancedCubes::onDeinit()
{
	// Destroy the cube renderer, in case there's one
	if (nullptr != mCubeRenderer)
	{
		delete mCubeRenderer;
		mCubeRenderer = nullptr;
	}
	
	ExampleRenderRuntime::onDeinit();
}

void InstancedCubes::Render()
{
	Renderer::IRendererPtr renderer(getRenderer());

	/// Update Timer
	{
		// Stop the stopwatch
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
		// Clear the color buffer of the current render target with gray, do also clear the depth buffer
		Renderer::Command::Clear::create(mCommandBuffer, Renderer::ClearFlag::COLOR_DEPTH, Color4::GRAY, 1.0f, 0);

		// Submit command buffer to the renderer backend
		mCommandBuffer.submitAndClear(*renderer);

		// Draw the cubes
		if (nullptr != mCubeRenderer)
		{
			mCubeRenderer->draw(mGlobalTimer, mGlobalScale, sin(mGlobalTimer * 0.001f) * SCENE_RADIUS, sin(mGlobalTimer * 0.0005f) * SCENE_RADIUS, cos(mGlobalTimer * 0.0008f) * SCENE_RADIUS);
		}

		// Display statistics
		if (mDisplayStatistics)
		{
			RendererRuntime::IRendererRuntime* rendererRuntime = getRendererRuntime();
			if (nullptr != rendererRuntime && nullptr != getMainRenderTarget())
			{
				RendererRuntime::DebugGuiManager& debugGuiManager = rendererRuntime->getDebugGuiManager();
				debugGuiManager.newFrame(*getMainRenderTarget());

				// Is there a cube renderer instance?
				if (nullptr != mCubeRenderer)
				{
					char text[128];

					// Number of cubes
					sprintf(text, "Number of cubes: %d", mNumberOfCubeInstances);
					debugGuiManager.drawText(text, 10.0f, 10.0f);

					// Frames per second
					sprintf(text, "Frames per second: %.2f", mFramesPerSecond);
					debugGuiManager.drawText(text, 10.0f, 40.0f);

					// Cubes per second
					// -> In every frame we draw n-cubes...
					// -> TODO(co) This number can get huge... had over 1 million cubes with >25 FPS... million cubes at ~2.4 FPS...
					sprintf(text, "Cubes per second: %u", static_cast<uint32_t>(mFramesPerSecond) * mNumberOfCubeInstances);
					debugGuiManager.drawText(text, 10.0f, 70.0f);
				}
				else
				{
					debugGuiManager.drawText("No cube renderer instance", 10.0f, 10.0f);
				}
				debugGuiManager.fillCommandBufferUsingFixedBuildInRendererConfiguration(mCommandBuffer);

				// Submit command buffer to the renderer backend
				mCommandBuffer.submitAndClear(*renderer);
			}
		}
	}
}
