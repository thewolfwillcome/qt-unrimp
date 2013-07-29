/*
 *
 */

#ifndef FIRSTINSTANCING_H
#define FIRSTINSTANCING_H

#include "UnrimpExamples/ExampleBase.h"

class FirstInstancing : public ExampleBase
{
public:
	virtual void Init(Renderer::IRendererPtr renderer) override;
	virtual void Deinit() override;
	virtual void Render() override;
    virtual QString name() { return "FirstInstancing"; } override;

private:
	// Left side (green): Instanced arrays (shader model 3 feature, vertex array element advancing per-instance instead of per-vertex)
	Renderer::IProgramPtr	  mProgramInstancedArrays;		///< Program, can be a null pointer
	Renderer::IVertexArrayPtr mVertexArrayInstancedArrays;	///< Vertex array object (VAO), can be a null pointer
	// Right side (blue): Draw instanced (shader model 4 feature, build in shader variable holding the current instance ID)
	Renderer::IProgramPtr	  mProgramDrawInstanced;		///< Program, can be a null pointer
	Renderer::IVertexArrayPtr mVertexArrayDrawInstanced;	///< Vertex array object (VAO), can be a null pointer

};

#endif // FIRSTINSTANCING_H
