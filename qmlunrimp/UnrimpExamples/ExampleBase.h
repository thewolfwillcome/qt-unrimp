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


#include <Renderer/Renderer.h>
#include "UnrimpExamples/Color4.h"
#include <QString>

class ExampleBase
{
public:
	virtual ~ExampleBase();

	virtual void Init(Renderer::IRendererPtr renderer) { m_Renderer = renderer; }
	virtual void Deinit() = 0;
	virtual void Render() = 0;
	virtual QString name() = 0;

	void setSize(int width, int height);
	virtual bool wantsCyclicUpdate() {return false;}

protected:
	ExampleBase();
	virtual void onSizeChanged(){}
	Renderer::IRendererPtr getRenderer() { return m_Renderer; }

protected:
	int mWidth, mHeigth;
	
private:
	Renderer::IRendererPtr m_Renderer;
};



template <class ExampleClass>
ExampleBase* ExampleFabricator()
{
	return new ExampleClass();
}

typedef ExampleBase* (*FabricatorMethod)();

#endif // EXAMPLEBASE_H
