/*
 *
 */

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "UnrimpExamples/ExampleBase.h"
class VertexBuffer : public ExampleBase
{
public:
	virtual void Init(Renderer::IRendererPtr renderer) override;
	virtual void Deinit() override;
	virtual void Render(Renderer::IRendererPtr renderer) override;
    virtual QString name() { return "VertexBuffer"; } override;

private:
	Renderer::IProgramPtr m_Program;
	Renderer::IVertexArrayPtr mVertexArrayVBO;	///< Vertex array object (VAO) using one vertex buffer object (VBO), can be a null pointer
	Renderer::IVertexArrayPtr mVertexArrayVBOs;	///< Vertex array object (VAO) using multiple vertex buffer objects (VBO), can be a null pointer

};

#endif // VERTEXBUFFER_H
