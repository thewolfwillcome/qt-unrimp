
#include "unrimpitem.h"
#include "unrimpnode.h"

#include <QtCore/QPropertyAnimation>

UnrimpItem::UnrimpItem(QQuickItem *parent)
	: QQuickItem(parent)
	, m_timerID(0)
	, m_node(nullptr)
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

void UnrimpItem::setExample(QString exampleName)
{
	if (m_node) {
 		if (m_node->setExample(exampleName)) {
 			emit exampleChanged();
			update();
 		}
	}
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

