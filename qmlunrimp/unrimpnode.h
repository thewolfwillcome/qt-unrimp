

#ifndef UNRIMPNODE_H
#define UNRIMPNODE_H

#include "Renderer/Renderer.h"
#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGTextureMaterial>
#include <QtQuick/QSGOpaqueTextureMaterial>
#include <QtQuick/QQuickWindow>

class UnrimpNode : public QSGGeometryNode
{
public:
    UnrimpNode();
    ~UnrimpNode();

    void setSize(const QSize &size);
    QSize size() const { return m_size; }

    void setQuickWindow(QQuickWindow *window);

    void setAAEnabled(bool enable);

    void update();
    void updateFBO();

    void init();

    void saveUnrimpState();
    void restoreUnrimpState();

    void preprocess();

private:
    QSGTextureMaterial m_material;
    QSGOpaqueTextureMaterial m_materialO;
    QSGGeometry m_geometry;
    QSGTexture *m_texture;
    QQuickWindow *m_quickWindow;
    /** Pointer to QOpenGLContext to be used by Unrimp. */
    QOpenGLContext* m_unrimpContext;
    /** Pointer to QOpenGLContext to be restored after Unrimp context. */
    QOpenGLContext* m_qtContext;

    int m_samples;
    bool m_AAEnabled;
    QSize m_size;

    Renderer::IRendererPtr m_renderer;
	Renderer::ITexture2DPtr m_renderTexture;
	Renderer::IFramebufferPtr m_frameBuffer;
	Renderer::IProgramPtr m_program;
	Renderer::IVertexArrayPtr m_VertexArray;

    bool m_initialized;
    bool m_dirtyFBO;
	bool m_dirtySize;
};

#endif // UNRIMPNODE_H
