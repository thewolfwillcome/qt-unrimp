
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

    startTimer(16);
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

    return node;
}

void UnrimpItem::timerEvent(QTimerEvent *)
{
	update();
}
