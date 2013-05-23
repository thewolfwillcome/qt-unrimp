

#include "unrimpnode.h"
#include <OpenGLRenderer/OpenGLRenderer.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions>


UnrimpNode::UnrimpNode()
    : QSGGeometryNode()
    , m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4)
    , m_texture(0)
    , m_quickWindow(0)
    , m_unrimpContext(0)
    , m_qtContext(0)
    , m_samples(0)
    , m_AAEnabled(false)
    , m_initialized(false)
    , m_dirtyFBO(false)
{
    setMaterial(&m_material);
    setOpaqueMaterial(&m_materialO);
    setGeometry(&m_geometry);
    setFlag(UsePreprocess);
}

UnrimpNode::~UnrimpNode()
{
	m_VertexArray = nullptr;
	m_program = nullptr;
	m_frameBuffer = nullptr;
	m_renderTexture = nullptr;
	m_renderer = nullptr;
    delete m_unrimpContext;
}

void UnrimpNode::saveUnrimpState()
{
    QOpenGLContext *ctx = QOpenGLContext::currentContext();
    /*ctx->functions()->glBindBuffer(GL_ARRAY_BUFFER, 0);
    ctx->functions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    ctx->functions()->glBindRenderbuffer(GL_RENDERBUFFER, 0);
    ctx->functions()->glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);*/

    ctx->doneCurrent();
    m_qtContext->makeCurrent(m_quickWindow);

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

void UnrimpNode::preprocess()
{
    restoreUnrimpState();

	// Backup the currently used render target
	Renderer::IRenderTargetPtr renderTarget(m_renderer->omGetRenderTarget());

	// Set the render target to render into
	m_renderer->omSetRenderTarget(m_frameBuffer);
	
	// Clear the color buffer of the current render target with green
	float color_green[4] = {1.0f, 1.0f, 0.0f, 1.0f};
	m_renderer->clear(Renderer::ClearFlag::COLOR, color_green, 1.0f, 0);
	
	if (m_renderer->beginScene())
	{
		// Set the used program
		m_renderer->setProgram(m_program);

		{ // Setup input assembly (IA)
			// Set the used vertex array
			m_renderer->iaSetVertexArray(m_VertexArray);

			// Set the primitive topology used for draw calls
			m_renderer->iaSetPrimitiveTopology(Renderer::PrimitiveTopology::TRIANGLE_LIST);
		}

		// Render the specified geometric primitive, based on an array of vertices
		m_renderer->draw(0, 3);

		// End scene rendering
		// -> Required for Direct3D 9
		// -> Not required for Direct3D 10, Direct3D 11, OpenGL and OpenGL ES 2
		m_renderer->endScene();
	}

	// Restore the previously set render target
	m_renderer->omSetRenderTarget(renderTarget);
    saveUnrimpState();
}

void UnrimpNode::setQuickWindow(QQuickWindow *window)
{
    m_quickWindow = window;

    // create a new shared OpenGL context to be used exclusively by Ogre
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
	Renderer::IRendererPtr renderer(m_renderer);
	Renderer::IShaderLanguagePtr shaderLanguage(m_renderer->getShaderLanguage());
		if (nullptr != shaderLanguage)
		{
			{ // Create the program
				// Get the shader source code (outsourced to keep an overview)
				const char *vertexShaderSourceCode = nullptr;
				const char *fragmentShaderSourceCode = nullptr;
				#include "FirstTriangle_GLSL_110.h"
				
				// Create the vertex shader
				Renderer::IVertexShader *vertexShader = shaderLanguage->createVertexShader(vertexShaderSourceCode);
				
				// Create the fragment shader
				Renderer::IFragmentShader *fragmentShader = shaderLanguage->createFragmentShader(fragmentShaderSourceCode);
				
				// Create the program
				m_program = shaderLanguage->createProgram(vertexShader, fragmentShader);
			}

			// Is there a valid program?
			if (nullptr != m_program)
			{
				// Create the vertex buffer object (VBO)
				// -> Clip space vertex positions, left/bottom is (-1,-1) and right/top is (1,1)
// 				static const float VERTEX_POSITION[] =
// 				{					// Vertex ID	Triangle on screen
// 					 0.0f, 1.0f,	// 0				0
// 					 1.0f, 0.0f,	// 1			   .   .
// 					 -0.5f, 0.0f	// 2			  2.......1
// 				};
				
				static const float VERTEX_POSITION[] =
				{					// Vertex ID	Triangle on screen
					 0.0f, 1.0f,	// 0				0
					 1.0f, 0.0f,	// 1			   .   .
					 -0.5f, 0.0f	// 2			  2.......1
				};
				Renderer::IVertexBufferPtr vertexBuffer(m_renderer->createVertexBuffer(sizeof(VERTEX_POSITION), VERTEX_POSITION, Renderer::BufferUsage::STATIC_DRAW));
				
				// Create vertex array object (VAO)
				// -> The vertex array object (VAO) keeps a reference to the used vertex buffer object (VBO)
				// -> This means that there's no need to keep an own vertex buffer object (VBO) reference
				// -> When the vertex array object (VAO) is destroyed, it automatically decreases the
				//    reference of the used vertex buffer objects (VBO). If the reference counter of a
				//    vertex buffer object (VBO) reaches zero, it's automatically destroyed.
				const Renderer::VertexArrayAttribute vertexArray[] =
				{
					{ // Attribute 0
						// Data destination
						Renderer::VertexArrayFormat::FLOAT_2,	// vertexArrayFormat (Renderer::VertexArrayFormat::Enum)
						"Position",								// name[64] (char)
						"POSITION",								// semantic[64] (char)
						0,										// semanticIndex (unsigned int)
						// Data source
						vertexBuffer,							// vertexBuffer (Renderer::IVertexBuffer *)
						0,										// offset (unsigned int)
						sizeof(float) * 2,						// stride (unsigned int)
						// Data source, instancing part
						0										// instancesPerElement (unsigned int)
					}
				};
				m_VertexArray = m_program->createVertexArray(sizeof(vertexArray) / sizeof(Renderer::VertexArrayAttribute), vertexArray);
			}
		}
			
    m_initialized = true;
}
