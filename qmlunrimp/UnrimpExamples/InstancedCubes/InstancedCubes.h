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


#ifndef INSTANCEDCUBES_H
#define INSTANCEDCUBES_H

#include "UnrimpExamples/ExampleRenderToolkit.h"
#include "UnrimpExamples/Stopwatch.h"

class ICubeRenderer;

class InstancedCubes : public ExampleRenderToolkit
{
public:
    InstancedCubes();
	virtual void Init(Renderer::IRendererPtr renderer) override;
	virtual void Deinit() override;
	virtual void Render() override;
    virtual QString name() { return "InstancedCubes"; } override;
	virtual bool wantsCyclicUpdate() {return true;} override;
	
//[-------------------------------------------------------]
//[ Private static data                                   ]
//[-------------------------------------------------------]
private:
	static const unsigned int NUMBER_OF_CHANGED_CUBES = 10000;	///< Number of changed cubes on key interaction
	static const unsigned int NUMBER_OF_TEXTURES	  = 8;		///< Number of textures
	static const unsigned int SCENE_RADIUS			  = 10;		///< Scene radius

private:
	RendererToolkit::IFontPtr  mFont;					///< Font, can be a null pointer
	ICubeRenderer			  *mCubeRenderer;			///< Cube renderer instance, can be a null pointer
	unsigned int			   mNumberOfCubeInstances;	///< Number of cube instances
	// The rest is for timing and statistics
	Stopwatch mStopwatch;			///< Stopwatch instance
	float	  mGlobalTimer;			///< Global timer
	float	  mGlobalScale;			///< Global scale
	bool	  mDisplayStatistics;	///< Display statistics?
	float	  mFPSUpdateTimer;		///< Timer for FPS update
	int		  mFramesSinceCheck;	///< Number of frames since last FPS update
	float	  mFramesPerSecond;		///< Current frames per second

};

#endif // INSTANCEDCUBES_H
