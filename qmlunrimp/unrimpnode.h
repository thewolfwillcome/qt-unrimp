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

#include <memory> // for std::unique_ptr

class UnrimpExample;
class ExampleApplicationFrontend;

class UnrimpNode : public QSGGeometryNode
{
public:
	UnrimpNode();
	~UnrimpNode();

	void setSize(const QSize &size);
	QSize size() const { return m_size; }

	void setQuickWindow(QQuickWindow *window);

	QString getOpenglVersionName() const;

	void setAAEnabled(bool enable);

	void update();
	void updateFBO();

	void init();

	void saveUnrimpState();
	void restoreUnrimpState();

	bool setExample(std::unique_ptr<UnrimpExample> newExample);
	bool exampleNeedsCyclicUpdate();

private:
	void ResetUnrimpStates();
	void Render();
	void fillCommandBuffer();

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
	Renderer::IBufferManagerPtr  mBufferManager;	///< Buffer manager, can be a null pointer
    Renderer::ITextureManagerPtr mTextureManager;	///< Texture manager, can be a null pointer
	Renderer::ITexture2DPtr m_renderTexture;
	Renderer::ITexture2DPtr m_renderDepthStencilTexture;
	Renderer::IFramebufferPtr m_frameBuffer;
	Renderer::CommandBuffer		 mCommandBuffer;	///< Command buffer

	std::unique_ptr<UnrimpExample> m_example; // Holds the instance to the current active example
	std::unique_ptr<UnrimpExample> m_newExampel; // Holds the instance to an new example to switch to

	bool m_initialized;
	bool m_dirtyFBO;
	bool m_dirtySize;
	bool m_exampleChanged;
	
	std::unique_ptr<ExampleApplicationFrontend> mApplicationFrontend;
};

#endif // UNRIMPNODE_H
