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


#include "unrimpitem.h"
#include "unrimpnode.h"

#include <QtCore/QPropertyAnimation>

UnrimpItem::UnrimpItem(QQuickItem *parent)
	: QQuickItem(parent)
	, m_timerID(0)
	, m_node(nullptr)
	, m_currentExample(nullptr)
{
    setFlag(ItemHasContents);
    setSmooth(false);
}

UnrimpItem::~UnrimpItem()
{
	// TODO(sw) Needed? In android version it was removed this line
	//delete m_node;
}


QString UnrimpItem::example()
{
	if (!m_currentExample)
		return QString();
	return m_currentExample->name();
}

QString UnrimpItem::openglVersionName()
{
	return mOpenGLVersionName;
}

void UnrimpItem::setExampleItem(ExampleItem* item){
	if (item == nullptr || m_currentExample == item)
		return;

	m_currentExample = item;
	if (m_node) {
		if (m_node->setExample(item->fabricator())) {
			emit exampleChanged();
			update();
		}
	}
}

ExampleItem* UnrimpItem::exampleItem()
{
	return m_currentExample;
}

QSGNode *UnrimpItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
	// When this method returns the ownership of the node item goes to the caller
	// We can manage the livetime of the node item ourself, when setting the flag QSGNode::OwnedByParent on the created node
	// Only the given old node item can be destroyed inside this method
	if (width() <= 0 || height() <= 0) {
		delete oldNode;
		return 0;
	}

	UnrimpNode *node = static_cast<UnrimpNode *>(oldNode);
	if (!node)
	{
		m_node = node = new UnrimpNode();
		node->setQuickWindow(window());
		if (m_currentExample)
			node->setExample(m_currentExample->fabricator());
		
		mOpenGLVersionName = node->getOpenglVersionName();
		emit openglVersionNameChanged();
	}
	node->setSize(QSize(width(), height()));
	node->update();
	
	if (node->exampleNeedsCyclicUpdate())
		startCyclicTimer();
	else
		stopCyclicTimer();

	return node;
}

void UnrimpItem::timerEvent(QTimerEvent *)
{
	update();
}

void UnrimpItem::startCyclicTimer()
{
	if(m_timerID)
		return;
	m_timerID = startTimer(16);
}

void UnrimpItem::stopCyclicTimer()
{
	if(!m_timerID)
		return;

	killTimer(m_timerID);
	m_timerID = 0;
}

