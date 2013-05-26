/*
 *
 */

#ifndef EXAMPLEBASE_H
#define EXAMPLEBASE_H


#include <Renderer/Renderer.h>
#include "UnrimpExamples/Color4.h"
#include <QString>

class ExampleBase
{
public:
    ExampleBase();
    virtual ~ExampleBase();
	
	virtual void Init(Renderer::IRendererPtr renderer) = 0;
	virtual void Deinit() = 0;
	virtual void Render(Renderer::IRendererPtr renderer) = 0;
	virtual QString name() = 0;
};



template <class ExampleClass>
ExampleBase* ExampleFabricator()
{
	return new ExampleClass();
}

typedef ExampleBase* (*FabricatorMethod)();

#endif // EXAMPLEBASE_H
