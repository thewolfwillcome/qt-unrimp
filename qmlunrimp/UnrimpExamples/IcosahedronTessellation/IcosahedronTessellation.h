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


#ifndef ICOSAHEDRONTESSELLATION_H
#define ICOSAHEDRONTESSELLATION_H

#include "UnrimpExamples/ExampleBase.h"

class IcosahedronTessellation : public ExampleBase
{
public:
	IcosahedronTessellation();

	virtual void Init(Renderer::IRendererPtr renderer) override;
	virtual void Deinit() override;
	virtual void Render() override;
    virtual QString name() { return "IcosahedronTessellation"; } override;

private:
	Renderer::IUniformBufferPtr	mUniformBufferDynamicTcs;	///< Dynamic tessellation control shader uniform buffer object (UBO), can be a null pointer
	Renderer::IUniformBufferPtr	mUniformBufferStaticTes;	///< Static tessellation evaluation shader uniform buffer object (UBO), can be a null pointer
	Renderer::IUniformBufferPtr	mUniformBufferStaticGs;		///< Static geometry shader uniform buffer object (UBO), can be a null pointer
	Renderer::IUniformBufferPtr	mUniformBufferStaticFs;		///< Static fragment shader uniform buffer object (UBO), can be a null pointer
	Renderer::IProgramPtr		mProgram;					///< Program, can be a null pointer
	Renderer::IVertexArrayPtr	mVertexArray;				///< Vertex array object (VAO), can be a null pointer
	float						mTessellationLevelOuter;	///< Outer tessellation level
	float						mTessellationLevelInner;	///< Inner tessellation level

};

#endif // ICOSAHEDRONTESSELLATION_H
