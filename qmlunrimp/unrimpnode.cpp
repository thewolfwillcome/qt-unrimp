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
#include "UnrimpExamples/UnrimpExample.h"

#include "ExampleApplicationFrontend.h"

#include <Framework/ExampleBase.h>
#include <Framework/Color4.h>

#include <Renderer/Public/StdLog.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QtGui/QOpenGLFramebufferObject>

#include <iostream>


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{


		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		Renderer::StdLog g_RendererLog;


//[-------------------------------------------------------]
//[ Anonymous detail namespace                            ]
//[-------------------------------------------------------]
	} // detail
}

class EmptyExample : public UnrimpExample
{
public:
	EmptyExample() : UnrimpExample(nullptr, {"", false, false})
	{}
};

UnrimpNode::UnrimpNode()
	: QSGGeometryNode()
	, m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4)
	, m_texture(0)
	, m_quickWindow(0)
	, m_unrimpContext(0)
	, m_qtContext(0)
	, m_samples(0)
	, m_AAEnabled(false)
    , m_renderer(nullptr)
	, mBufferManager(nullptr)
	, mTextureManager(nullptr)
	, m_renderTexture(nullptr)
	, m_renderDepthStencilTexture(nullptr)
	, m_frameBuffer(nullptr)
	, m_example(new EmptyExample)
	, m_newExampel(nullptr)
	, m_initialized(false)
	, m_dirtyFBO(false)
	, m_dirtySize(false)
    , m_exampleChanged(false)
{
	setMaterial(&m_material);
	setOpaqueMaterial(&m_materialO);
	setGeometry(&m_geometry);
}

UnrimpNode::~UnrimpNode()
{
	if (m_example)
		m_example->Deinit();
	
	if (m_qtContext)
		saveUnrimpState();

	m_frameBuffer = nullptr;
	m_renderTexture = nullptr;
	m_renderDepthStencilTexture = nullptr;
	
	mBufferManager = nullptr;
	mTextureManager = nullptr;

	m_renderer = nullptr;
	delete m_unrimpContext;
}

bool UnrimpNode::setExample(std::unique_ptr<UnrimpExample> newExample)
{
	m_newExampel = std::move(newExample);
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
	if (!m_qtContext)
		return;

	QOpenGLContext *ctx = QOpenGLContext::currentContext();

	ctx->doneCurrent();
	m_qtContext->makeCurrent(m_quickWindow);
	m_qtContext = nullptr;

	//glPushAttrib(GL_ALL_ATTRIB_BITS);
}

void UnrimpNode::restoreUnrimpState()
{
	//glPopAttrib();
	m_qtContext = QOpenGLContext::currentContext();
	m_qtContext->functions()->glUseProgram(0);
	m_qtContext->doneCurrent();

	m_unrimpContext->makeCurrent(m_quickWindow);
}

void UnrimpNode::setQuickWindow(QQuickWindow *window)
{
	m_quickWindow = window;

	// create a new shared OpenGL context to be used exclusively by Unrimp
	m_unrimpContext = new QOpenGLContext();
	QSurfaceFormat qSurfaceFormat = m_quickWindow->requestedFormat();
	
	#ifdef USEOPENGL
		// unrimp needs at least a OpenGL 4.1 context
		qSurfaceFormat.setMajorVersion(4);
		qSurfaceFormat.setMinorVersion(1);
		//format.setProfile(QSurfaceFormat::CompatibilityProfile);
		qSurfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
		//#ifdef _DEBUG
		qSurfaceFormat.setOption(QSurfaceFormat::DebugContext, true);
		//#endif
	#endif
		
	qSurfaceFormat.setRedBufferSize(8);
	qSurfaceFormat.setGreenBufferSize(8);
	qSurfaceFormat.setBlueBufferSize(8);
	qSurfaceFormat.setAlphaBufferSize(8);
	qSurfaceFormat.setStencilBufferSize(8);
	qSurfaceFormat.setDepthBufferSize(24);

	auto r = qSurfaceFormat.redBufferSize();
	auto g = qSurfaceFormat.greenBufferSize();
	auto b = qSurfaceFormat.blueBufferSize();
	auto a = qSurfaceFormat.alphaBufferSize();
	auto s = qSurfaceFormat.stencilBufferSize();
	auto d = qSurfaceFormat.depthBufferSize();
	m_unrimpContext->setFormat(qSurfaceFormat);
	m_unrimpContext->setShareContext(QOpenGLContext::currentContext());
    m_unrimpContext->create();
}

QString UnrimpNode::getOpenglVersionName() const
{
	if (nullptr != m_unrimpContext)
	{
		QOpenGLVersionProfile profile(m_unrimpContext->format());
		auto profileValue = profile.profile();
		auto versionValue = profile.version();

		QString versionString;
		
		if (m_unrimpContext->isOpenGLES())
			versionString = "OpenGLES %1.%2 %3";
		else
			versionString = "OpenGL %1.%2 %3";
		
		return QString(versionString).arg(versionValue.first).arg(versionValue.second).arg(profileValue == QSurfaceFormat::CoreProfile ? "core" : "");
	}
	return QString();
}

void UnrimpNode::update()
{
	restoreUnrimpState();

	if (!m_initialized || m_dirtyFBO || m_exampleChanged)
	{
		if (!m_initialized) {
			init();
			if (m_example)
				m_example->Init(mApplicationFrontend.get());
		}

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

			if (m_newExampel) {
				m_example.reset(m_newExampel.release());
			}
			else {
				m_example.reset(new EmptyExample);
			}

			m_example->setSize(m_size.width(), m_size.height());
			m_example->Init(mApplicationFrontend.get());
			m_exampleChanged = false;
			markDirty(DirtyMaterial);
			
			// When the example changes we need an additional render call.
			// Otherwise some example doesn't render correctly (e.g. FirstRenderToTexture, FirstPostProcessing)
			// I don't know why this is needed..
			Render();
		}
		
	} else if (exampleNeedsCyclicUpdate()) {
		// when an example needs cyclic update (e.g. to drive an animation) we mark the material dirty.
		// Otherweise the QML engine doesn't use the new content
		markDirty(DirtyMaterial);
	}

	// Render the example
	Render();

	saveUnrimpState();
}

void UnrimpNode::updateFBO()
{
    if (nullptr == mTextureManager)
        return;

	Renderer::ITexture *texture2D = m_renderTexture = mTextureManager->createTexture2D(m_size.width(), m_size.height(), Renderer::TextureFormat::R8G8B8A8, nullptr, Renderer::TextureFlag::RENDER_TARGET);
	Renderer::ITexture* depthStencilTexture2D = m_renderDepthStencilTexture = mTextureManager->createTexture2D(m_size.width(), m_size.height(), Renderer::TextureFormat::D32_FLOAT, nullptr, Renderer::TextureFlag::RENDER_TARGET);
	{
		Renderer::FramebufferAttachment colorFramebufferAttachment(texture2D);
		Renderer::FramebufferAttachment depthStencilFramebufferAttachment(depthStencilTexture2D);
		m_frameBuffer = m_renderer->createFramebuffer(1, &colorFramebufferAttachment, &depthStencilFramebufferAttachment);
	}

	mApplicationFrontend->setMainRenderTarget(m_frameBuffer);

	fillCommandBuffer();

	// specify texture coordinates which flips the y-axis of the texture
	// a 1-to-1 map would be (0, 0, 1, 1) but the result of the rendered example isn't bottom up in the texture (as OpenGL expect it)
	QRectF textureCoordinatesYAxisFlipped(0,1,1,-1);
	QSGGeometry::updateTexturedRectGeometry(&m_geometry,
											QRectF(0, 0, m_size.width(), m_size.height()),
											textureCoordinatesYAxisFlipped);

	delete m_texture;
    
	m_texture = m_quickWindow->createTextureFromId(static_cast<uint32_t>(reinterpret_cast<uintptr_t>(texture2D->getInternalResourceHandle())), m_size);

    m_material.setTexture(m_texture);
    m_materialO.setTexture(m_texture);
	
	if (m_example)
		m_example->setSize(m_size.width(), m_size.height());
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

	QSurfaceFormat qSurfaceFormat = m_quickWindow->requestedFormat();
	// TODO(sw) We misuse the windows context because no window handle is given
	mRendererContext = std::unique_ptr<Renderer::Context>(new Renderer::Context(Renderer::Context::ContextType::WINDOWS, ::detail::g_RendererLog, 0, true));

#if USEOPENGL
	extern Renderer::IRenderer *createOpenGLRendererInstance(const Renderer::Context&);

	// Create the renderer instance
	m_renderer = createOpenGLRendererInstance(*mRendererContext);
#elif USEGLES
	extern Renderer::IRenderer *createOpenGLES3RendererInstance(const Renderer::Context&);

    m_renderer = createOpenGLES3RendererInstance(*mRendererContext);
#endif

    if (nullptr != m_renderer)
    {
        mBufferManager = m_renderer->createBufferManager();
        mTextureManager = m_renderer->createTextureManager();
    }
    
    mApplicationFrontend = std::unique_ptr<ExampleApplicationFrontend>(new ExampleApplicationFrontend(m_renderer));

	m_initialized = true;
}

void UnrimpNode::ResetUnrimpStates()
{
// 	Renderer::IRenderTargetPtr renderTarget(m_renderer->omGetRenderTarget());
// 	m_renderer->rsSetState(nullptr);
// 	m_renderer->vsSetTexture(0, nullptr);
// 
// 	// Set the used blend state
// 	m_renderer->omSetBlendState(nullptr);
// 
// 	m_renderer->omSetDepthStencilState(nullptr);
// 
// 	m_renderer->vsSetSamplerState(0, nullptr);
// 
// 	m_renderer->fsSetTexture(0, nullptr);
// 	m_renderer->setProgram(nullptr);
// 
// 	m_renderer->omSetRenderTarget(m_frameBuffer);
// 
// 	m_renderer->clear(Renderer::ClearFlag::COLOR_DEPTH, Color4::GRAY, 1.0f, 0);
// 	
// 	m_renderer->omSetRenderTarget(renderTarget);
}

void UnrimpNode::Render()
{
    if (nullptr == m_renderer)
        return;

	// Submit command buffer to the renderer backend
	mCommandBuffer.submit(*m_renderer);

	// Do the drawing :)
	m_example->Render();
}

void UnrimpNode::fillCommandBuffer()
{
    if (nullptr == m_renderer)
        return;

    mCommandBuffer.clear();

    // Set the render target to render into
    Renderer::Command::SetRenderTarget::create(mCommandBuffer, m_frameBuffer);

    { // Since Direct3D 12 is command list based, the viewport and scissor rectangle
      // must be set in every draw call to work with all supported renderer APIs
        // Get the window size
        uint32_t width  = static_cast<uint32_t>(m_size.width());
        uint32_t height = static_cast<uint32_t>(m_size.height());

        // Set the viewport and scissor rectangle
        Renderer::Command::SetViewportAndScissorRectangle::create(mCommandBuffer, 0, 0, width, height);
    }

    // Clear the color buffer of the current render target with blue
    Renderer::Command::Clear::create(mCommandBuffer, Renderer::ClearFlag::COLOR, Color4::BLUE, 1.0f, 0);
}
