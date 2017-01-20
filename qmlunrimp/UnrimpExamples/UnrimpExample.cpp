/*********************************************************\
 * Copyright (c) 2013-2013 Stephan Wezel
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


#include "UnrimpExample.h"
#include "ExampleApplicationFrontend.h"


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
UnrimpExample::UnrimpExample(std::unique_ptr<ExampleBase> example, UnrimpExampleMetaData metadata) :
	mMetadata(metadata),
	mExample(std::move(example)),
	mFrontend(nullptr)
{
	// Nothing here
}

UnrimpExample::~UnrimpExample()
{
	// Nothing here
}

void UnrimpExample::Init(ExampleApplicationFrontend* applicationFrontend)
{
	mFrontend = applicationFrontend;
	if (mExample)
	{
		// TODO(sw) Right location to do this? Might be better done outside if this class (e.g. urimpnode) where it knows if the next example needs the runtime too
		if (nullptr != mFrontend && needsRendererRuntime())
		{
			mFrontend->setupRendererRuntime();
		}

		mExample->setApplicationFrontend(applicationFrontend);
		
		mExample->initialize();
	}
}

void UnrimpExample::Deinit()
{
	if (mExample)
	{
		mExample->deinitialize();
	}

	if (nullptr != mFrontend && needsRendererRuntime())
	{
		mFrontend->teardownRendererRuntime();
	}
}

void UnrimpExample::setSize(int width, int height)
{
	if (mWidth != width || mHeigth != height) {
		mWidth = width;
		mHeigth = height;
	}
}

void UnrimpExample::Render()
{
	if (mExample)
	{
		if (wantsCyclicUpdate())
		{
			if (nullptr != mFrontend)
			{
				RendererRuntime::IRendererRuntime* rendererRuntime = mFrontend->getRendererRuntime();
				if (nullptr != rendererRuntime)
				{
					CLocaleChanger clocale;
					rendererRuntime->update();
				}
			}
			
			mExample->onUpdate();
		}

		mExample->onDraw();
	}
}
