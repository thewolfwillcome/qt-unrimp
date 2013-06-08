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
	
	virtual void Init(Renderer::IRendererPtr renderer) { m_Renderer = renderer; }
	virtual void Deinit() = 0;
	virtual void Render() = 0;
	virtual QString name() = 0;
	
	void setSize(int width, int height);

protected:
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
