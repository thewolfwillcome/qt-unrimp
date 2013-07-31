/*
 *
 */

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
