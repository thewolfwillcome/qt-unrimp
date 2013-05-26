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

class ExampleBaseFabricator {
public:
    virtual ~ExampleBaseFabricator(){}
    virtual ExampleBase* operator()()  = 0;
};

template<typename T>
class ExampleFabricator : public ExampleBaseFabricator {
    virtual T* operator()() override {return new T;}
};

typedef QSharedPointer<ExampleBaseFabricator> ExampleFabricatorPtr;

#endif // EXAMPLEBASE_H
