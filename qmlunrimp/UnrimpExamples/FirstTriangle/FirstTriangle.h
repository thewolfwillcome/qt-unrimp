/*
 *
 */

#ifndef FIRSTTRIANGLE_H
#define FIRSTTRIANGLE_H

#include "UnrimpExamples/ExampleBase.h"

class FirstTriangle : public ExampleBase
{
public:
	virtual void Init(Renderer::IRendererPtr renderer) override;
	virtual void Deinit() override;
	virtual void Render(Renderer::IRendererPtr renderer) override;
    virtual QString name() { return "FirstTriangle"; } override;

private:
	Renderer::IProgramPtr m_Program;
	Renderer::IVertexArrayPtr m_VertexArray;

};

#endif // FIRSTTRIANGLE_H
