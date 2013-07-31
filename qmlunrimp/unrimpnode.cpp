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


#include "unrimpnode.h"
#include <OpenGLRenderer/OpenGLRenderer.h>
#include <OpenGLRenderer/Framebuffer.h>
#include "UnrimpExamples/Color4.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include "UnrimpExamples/FirstTriangle/FirstTriangle.h"
#include "UnrimpExamples/VertexBuffer/VertexBuffer.h"
#include "UnrimpExamples/FirstTexture/FirstTexture.h"
#include "UnrimpExamples/FirstRenderToTexture/FirstRenderToTexture.h"
#include "UnrimpExamples/FirstInstancing/FirstInstancing.h"
#include "UnrimpExamples/FirstMultipleRenderTargets/FirstMultipleRenderTargets.h"
#include "UnrimpExamples/FirstGeometryShader/FirstGeometryShader.h"
#include "UnrimpExamples/FirstTessellation/FirstTessellation.h"
#include "UnrimpExamples/Fxaa/Fxaa.h"
#include "UnrimpExamples/InstancedCubes/InstancedCubes.h"
#include "UnrimpExamples/FirstPostProcessing/FirstPostProcessing.h"
#include "UnrimpExamples/IcosahedronTessellation/IcosahedronTessellation.h"
#include "UnrimpExamples/FirstFont/FirstFont.h"


UnrimpNode::UnrimpNode()
	: QSGGeometryNode()
	, m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4)
	, m_texture(0)
	, m_quickWindow(0)
	, m_unrimpContext(0)
	, m_qtContext(0)
	, m_samples(0)
	, m_AAEnabled(false)
	, m_newExampleName("FirstTriangle")
	, m_availableExamples({
							// Basic
							{"FirstTriangle", 				&ExampleFabricator<FirstTriangle> },
						    {"VertexBuffer", 				&ExampleFabricator<VertexBuffer> },
							{"FirstTexture", 				&ExampleFabricator<FirstTexture> },
							{"FirstRenderToTexture", 		&ExampleFabricator<FirstRenderToTexture> },
							{"FirstPostProcessing", 		&ExampleFabricator<FirstPostProcessing> },
							{"FirstInstancing", 			&ExampleFabricator<FirstInstancing> },
							{"FirstMultipleRenderTargets", 	&ExampleFabricator<FirstMultipleRenderTargets> },
							{"FirstGeometryShader", 		&ExampleFabricator<FirstGeometryShader> },
							{"FirstTessellation", 			&ExampleFabricator<FirstTessellation> },
							// Advanced
							{"Fxaa", 						&ExampleFabricator<Fxaa> },
							{"InstancedCubes", 				&ExampleFabricator<InstancedCubes> },
							{"IcosahedronTessellation", 	&ExampleFabricator<IcosahedronTessellation> },
							{"FirstFont", 					&ExampleFabricator<FirstFont> },
						  })
	, m_example(nullptr)
	, m_initialized(false)
	, m_dirtyFBO(false)
	, m_exampleChanged(false)
{
	setMaterial(&m_material);
	setOpaqueMaterial(&m_materialO);
	setGeometry(&m_geometry);
	setFlag(UsePreprocess);
}

UnrimpNode::~UnrimpNode()
{
	if (m_example)
		m_example->Deinit();

	m_frameBuffer = nullptr;
	m_renderTexture = nullptr;
	m_renderer = nullptr;
	delete m_unrimpContext;
}

QString UnrimpNode::example()
{
	return m_newExampleName;
}

bool UnrimpNode::setExample(QString exampleName)
{
	// check if given example name is known
	if (exampleName == example() || !m_availableExamples.contains(exampleName))
		return false;
	
	// save example name and mark the material (texture) dirty
	// this causes an update, in which the new example is created.
	// This makes sure that the current active example is changed on the right time
	// (e.g. the unrimp opengl context is active and we are in the correct thread context )
	m_newExampleName = exampleName;
	m_exampleChanged = true;
	return true;
}

bool UnrimpNode::exampleNeedsCyclicUpdate()
{
	if (!m_example)
		return false;
	return m_example->wantsCyclicUpdate();
}

void UnrimpNode::saveUnrimpState()
{
	QOpenGLContext *ctx = QOpenGLContext::currentContext();

	ctx->doneCurrent();
	m_qtContext->makeCurrent(m_quickWindow);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
}

void UnrimpNode::restoreUnrimpState()
{
	glPopAttrib();
	m_qtContext = QOpenGLContext::currentContext();
	m_qtContext->functions()->glUseProgram(0);
	m_qtContext->doneCurrent();

	m_unrimpContext->makeCurrent(m_quickWindow);
}

void UnrimpNode::preprocess()
{
	restoreUnrimpState();

	// Backup the currently used render target
	Renderer::IRenderTargetPtr renderTarget(m_renderer->omGetRenderTarget());

	m_renderer->omSetRenderTarget(m_frameBuffer);

	// Do the drawing :)
	m_example->Render();

	// Restore the previously set render target
	m_renderer->omSetRenderTarget(renderTarget);

	saveUnrimpState();
}

void UnrimpNode::setQuickWindow(QQuickWindow *window)
{
	m_quickWindow = window;

	// create a new shared OpenGL context to be used exclusively by Unrimp
	m_unrimpContext = new QOpenGLContext();
	m_unrimpContext->setFormat(m_quickWindow->requestedFormat());
	m_unrimpContext->setShareContext(QOpenGLContext::currentContext());
	m_unrimpContext->create();
}

void UnrimpNode::update()
{
	if (!m_initialized || m_dirtyFBO || m_exampleChanged)
	{
		restoreUnrimpState();

		if (!m_initialized)
			init();

		if (m_dirtyFBO) {
			updateFBO();
			m_dirtyFBO = false;
		}
		
		if (m_exampleChanged || !m_example)
		{
			if(m_example) {
				m_example->Deinit();
			
				ResetUnrimpStates();
			}
			
			FabricatorMethod fabricator(m_availableExamples[m_newExampleName]); 
			m_example = QSharedPointer<ExampleBase>(fabricator());
			
			m_example->setSize(m_size.width(), m_size.height());
			m_example->Init(m_renderer);
			m_exampleChanged = false;
			markDirty(DirtyMaterial);
		}

		saveUnrimpState();
	}
	// for simplicity always mark the material as dirty to force a redraw of the node
	markDirty(DirtyMaterial);
}

void UnrimpNode::updateFBO()
{
	
	Renderer::ITexture *texture2D = m_renderTexture = m_renderer->createTexture2D(m_size.width(), m_size.height(), Renderer::TextureFormat::R8G8B8A8, nullptr, Renderer::TextureFlag::RENDER_TARGET);
	m_frameBuffer = m_renderer->createFramebuffer(1, &texture2D);
	
	// Backup the currently used render target
	Renderer::IRenderTargetPtr renderTarget(m_renderer->omGetRenderTarget());

	// Set the render target to render into
	m_renderer->omSetRenderTarget(m_frameBuffer);

	const Renderer::Viewport viewport =
	{
		0.0f,						// topLeftX (float)
		0.0f,						// topLeftY (float)
		static_cast<float>(m_size.width()),	// width (float)
		static_cast<float>(m_size.height()),	// height (float)
		0.0f,						// minDepth (float)
		1.0f						// maxDepth (float)
	};
	m_renderer->rsSetViewports(1, &viewport);

	// specify texture coordinates which flips the y-axis of the texture
	// a 1-to-1 map would be (0, 0, 1, 1) but the result of the rendered example isn't bottom up in the texture (as OpenGL expect it)
	QRectF textureCoordinatesYAxisFlipped(0,1,1,-1);
	QSGGeometry::updateTexturedRectGeometry(&m_geometry,
											QRectF(0, 0, m_size.width(), m_size.height()),
											textureCoordinatesYAxisFlipped);

	delete m_texture;
	m_texture = m_quickWindow->createTextureFromId(static_cast<OpenGLRenderer::Texture2D*>(texture2D)->getOpenGLTexture(), m_size);


	m_material.setTexture(m_texture);
	m_materialO.setTexture(m_texture);
	
	if (m_example)
		m_example->setSize(m_size.width(), m_size.height());

	m_renderer->omSetRenderTarget(renderTarget);
}

void UnrimpNode::setSize(const QSize &size)
{
	if (size == m_size)
		return;

	m_size = size;
	m_dirtyFBO = true;
	markDirty(DirtyGeometry);
}

void UnrimpNode::setAAEnabled(bool enable)
{
	if (m_AAEnabled == enable)
		return;

	m_AAEnabled = enable;
	m_dirtyFBO = true;
	markDirty(DirtyMaterial);
}

void UnrimpNode::init()
{
	const QOpenGLContext *ctx = QOpenGLContext::currentContext();
	QSurfaceFormat format = ctx->format();
	m_samples = format.samples();
	
	extern Renderer::IRenderer *createOpenGLRendererInstance2(Renderer::handle, bool);

	// Create the renderer instance
	m_renderer = createOpenGLRendererInstance2(0, true);

	m_initialized = true;
}

void UnrimpNode::ResetUnrimpStates()
{
	Renderer::IRenderTargetPtr renderTarget(m_renderer->omGetRenderTarget());
	m_renderer->rsSetState(nullptr);
	m_renderer->vsSetTexture(0, nullptr);

	// Set the used blend state
	m_renderer->omSetBlendState(nullptr);

	m_renderer->omSetDepthStencilState(nullptr);

	m_renderer->vsSetSamplerState(0, nullptr);

	m_renderer->fsSetTexture(0, nullptr);
	m_renderer->setProgram(nullptr);

	m_renderer->omSetRenderTarget(m_frameBuffer);

	m_renderer->clear(Renderer::ClearFlag::COLOR_DEPTH, Color4::GRAY, 1.0f, 0);
	
	m_renderer->omSetRenderTarget(renderTarget);
}

