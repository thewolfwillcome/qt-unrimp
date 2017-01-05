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


#ifndef EXAMPLEBASE_H
#define EXAMPLEBASE_H

#include "UnrimpExamples/PlatformTypes.h"
#include "UnrimpExamples/Color4.h"

#include <Renderer/Renderer.h>
#include <QString>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cassert>

class ExampleBase
{


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	virtual ~ExampleBase();

	void Init(Renderer::IRendererPtr renderer, Renderer::IRenderTarget* mainRenderTarget);
	void Deinit();

	void setSize(int width, int height);


//[-------------------------------------------------------]
//[ Public virtual ExampleBase methods                    ]
//[-------------------------------------------------------]
public:
	virtual void onInit(Renderer::IRendererPtr renderer) = 0;
	virtual void onDeinit() = 0;

	virtual void Render() = 0;
	virtual QString name() = 0;
	virtual bool wantsCyclicUpdate() {return false;}


//[-------------------------------------------------------]
//[ Protected ExampleBase methods                         ]
//[-------------------------------------------------------]
protected:
	ExampleBase();
	inline Renderer::IRendererPtr getRenderer() { return m_Renderer; }
	inline Renderer::IRenderTarget* getMainRenderTarget() { return mMainRenderTarget; }
	

//[-------------------------------------------------------]
//[ Protected virtual ExampleBase methods                 ]
//[-------------------------------------------------------]
protected:
	virtual void onSizeChanged(){}
	

//[-------------------------------------------------------]
//[ Protected data                                        ]
//[-------------------------------------------------------]
protected:
	int mWidth, mHeigth;
	

//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
private:
	Renderer::IRendererPtr m_Renderer;
	Renderer::IRenderTarget* mMainRenderTarget;
};



template <class ExampleClass>
ExampleBase* ExampleFabricator()
{
	return new ExampleClass();
}

typedef ExampleBase* (*FabricatorMethod)();

#endif // EXAMPLEBASE_H
