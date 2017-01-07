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


#ifndef EXAMPLEBASE_H
#define EXAMPLEBASE_H

#include <Framework/ExampleBase.h>

#include <Renderer/Renderer.h>
#include <QString>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cassert>
#include <memory> // For "std::unique_ptr"

class ExampleApplicationFrontend;

struct UnrimpExampleMetaData
{
	QString name;
	bool wantsCyclicUpdate;
	bool needsRendererRuntime;
};

class UnrimpExample
{


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
public:
	UnrimpExample(std::unique_ptr<ExampleBase> example, UnrimpExampleMetaData metadata);
	~UnrimpExample();

	void Init(ExampleApplicationFrontend* applicationFrontend);
	void Deinit();

	void setSize(int width, int height);

	void Render();
	QString name() {return mMetadata.name;}
	bool wantsCyclicUpdate() {return mMetadata.wantsCyclicUpdate;}

	bool needsRendererRuntime() {return mMetadata.needsRendererRuntime;}


//[-------------------------------------------------------]
//[ Protected virtual ExampleBase methods                 ]
//[-------------------------------------------------------]
protected:
	virtual void onSizeChanged(){}
	

//[-------------------------------------------------------]
//[ Protected data                                        ]
//[-------------------------------------------------------]
protected:
	int mWidth, mHeigth;
	

//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
private:
	UnrimpExampleMetaData mMetadata;
	std::unique_ptr<ExampleBase> mExample;
	ExampleApplicationFrontend* mFrontend;
};


#endif // EXAMPLEBASE_H
