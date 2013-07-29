/*
 *
 */

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
