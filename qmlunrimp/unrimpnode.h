

#ifndef UNRIMPNODE_H
#define UNRIMPNODE_H

#include "Renderer/Renderer.h"
#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGTextureMaterial>
#include <QtQuick/QSGOpaqueTextureMaterial>
#include <QtQuick/QQuickWindow>

class ExampleBase;

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
	
	
	QString example();
	bool setExample(QString exampleName);

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
	QString m_newExampleName;

    Renderer::IRendererPtr m_renderer;
	Renderer::ITexture2DPtr m_renderTexture;
	Renderer::IFramebufferPtr m_frameBuffer;
	QSharedPointer<ExampleBase> m_example;

    bool m_initialized;
    bool m_dirtyFBO;
	bool m_dirtySize;
	bool m_exampleChanged;
};

#endif // UNRIMPNODE_H
