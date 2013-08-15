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
	, m_currentExampleIndex(0)
{
    setFlag(ItemHasContents);
    setSmooth(false);
}

QString UnrimpItem::example()
{
	if (!m_node)
		return "FirstTriangle";
	return m_node->example();
}

ExampleModel* UnrimpItem::exampleModel() {
	return &m_exampleModel;
}

void UnrimpItem::setExampleIndex(int index) {
	const QList<ExampleItem>& examples = m_exampleModel.examples();
	if (index < 0 || examples.count() <= index || m_currentExampleIndex == index)
		return;
	m_currentExampleIndex = index;
	ExampleItem item  = examples[index];
	
	if (m_node) {
 		if (m_node->setExample(item)) {
 			emit exampleChanged();
			update();
 		}
	}
}

int UnrimpItem::exampleIndex() {
	return m_currentExampleIndex;
}

QSGNode *UnrimpItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
	if (width() <= 0 || height() <= 0) {
		delete oldNode;
		return 0;
	}

	UnrimpNode *node = static_cast<UnrimpNode *>(oldNode);
	if (!node)
	{
		m_node = node = new UnrimpNode();
		node->setQuickWindow(window());
		node->setExample(m_exampleModel.examples()[0]);
	}

	node->setSize(QSize(width(), height()));
	node->setAAEnabled(smooth());
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

