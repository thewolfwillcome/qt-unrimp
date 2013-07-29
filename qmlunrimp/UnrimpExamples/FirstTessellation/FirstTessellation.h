/*
 *
 */

#ifndef FIRSTTESSELLATION_H
#define FIRSTTESSELLATION_H

#include "UnrimpExamples/ExampleBase.h"

class FirstTessellation : public ExampleBase
{
public:
	virtual void Init(Renderer::IRendererPtr renderer) override;
	virtual void Deinit() override;
	virtual void Render() override;
    virtual QString name() { return "FirstTessellation"; } override;

private:
	Renderer::IProgramPtr		  mProgram;			///< Program, can be a null pointer
	Renderer::IVertexArrayPtr	  mVertexArray;		///< Vertex array object (VAO), can be a null pointer
	Renderer::IRasterizerStatePtr mRasterizerState;	///< Rasterizer state, can be a null pointer

};

#endif // FIRSTTESSELLATION_H
