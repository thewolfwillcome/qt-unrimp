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


#ifndef UNRIMPNODE_H
#define UNRIMPNODE_H

#include "Renderer/Renderer.h"
#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGTextureMaterial>
#include <QtQuick/QSGOpaqueTextureMaterial>
#include <QtQuick/QQuickWindow>
#include "ExampleModel.h"

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
	bool setExample(ExampleItem exampleFac);
	bool exampleNeedsCyclicUpdate();

private:
	void ResetUnrimpStates();

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
	ExampleItem m_newExampleFac;

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
