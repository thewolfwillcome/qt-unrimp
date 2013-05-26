

#include "unrimpnode.h"
#include <OpenGLRenderer/OpenGLRenderer.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include "UnrimpExamples/FirstTriangle/FirstTriangle.h"
#include "UnrimpExamples/VertexBuffer/VertexBuffer.h"
#include "UnrimpExamples/FirstTexture/FirstTexture.h"
#include "UnrimpExamples/FirstRenderToTexture/FirstRenderToTexture.h"


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
							{"FirstTriangle", ExampleFabricatorPtr(new ExampleFabricator<FirstTriangle>) },
						    {"VertexBuffer", ExampleFabricatorPtr(new ExampleFabricator<VertexBuffer>) },
							{"FirstTexture", ExampleFabricatorPtr(new ExampleFabricator<FirstTexture>) },
							{"FirstRenderToTexture", ExampleFabricatorPtr(new ExampleFabricator<FirstRenderToTexture>) }
						  })
	, m_example(new FirstTriangle)
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
	if (exampleName == example() || !m_availableExamples.contains(exampleName))
		return false;
	
	m_newExampleName = exampleName;
	m_exampleChanged = true;
	return true;
}


void UnrimpNode::saveUnrimpState()
{
    QOpenGLContext *ctx = QOpenGLContext::currentContext();

    ctx->doneCurrent();
    m_qtContext->makeCurrent(m_quickWindow);
}

void UnrimpNode::restoreUnrimpState()
{
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

	// Set the render target to render into
	m_renderer->omSetRenderTarget(m_frameBuffer);
	
	m_example->Render(m_renderer);

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
    restoreUnrimpState();

    if (!m_initialized)
        init();

    if (m_dirtyFBO) {
        updateFBO();
        m_dirtyFBO = false;
    }
    
    if (m_exampleChanged)
	{
		m_example->Deinit();
		ExampleFabricatorPtr fabricator(m_availableExamples[m_newExampleName]); 
		m_example = QSharedPointer<ExampleBase>((*fabricator)());
		
		m_example->Init(m_renderer);
		m_exampleChanged = false;
	}

    saveUnrimpState();
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

    QSGGeometry::updateTexturedRectGeometry(&m_geometry,
                                            QRectF(0, 0, m_size.width(), m_size.height()),
                                            QRectF(0, 0, 1, 1));

	delete m_texture;
    m_texture = m_quickWindow->createTextureFromId(static_cast<OpenGLRenderer::Texture2D*>(texture2D)->getOpenGLTexture(), m_size);

    m_material.setTexture(m_texture);
    m_materialO.setTexture(m_texture);
	
		
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
	
	m_example->Init(m_renderer);
	
    m_initialized = true;
}
