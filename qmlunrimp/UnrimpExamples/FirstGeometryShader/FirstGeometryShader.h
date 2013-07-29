/*
 *
 */

#ifndef FIRSTGEOMETRYSHADER_H
#define FIRSTGEOMETRYSHADER_H

#include "UnrimpExamples/ExampleBase.h"

class FirstGeometryShader : public ExampleBase
{
public:
	virtual void Init(Renderer::IRendererPtr renderer) override;
	virtual void Deinit() override;
	virtual void Render() override;
    virtual QString name() { return "FirstGeometryShader"; } override;

private:
	Renderer::IProgramPtr	  mProgram;		///< Program, can be a null pointer
	Renderer::IVertexArrayPtr mVertexArray;	///< Vertex array object (VAO), can be a null pointer

};

#endif // FIRSTGEOMETRYSHADER_H
