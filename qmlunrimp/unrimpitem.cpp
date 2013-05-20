
#include "unrimpitem.h"
#include "unrimpnode.h"

#include <QtCore/QPropertyAnimation>

UnrimpItem::UnrimpItem(QQuickItem *parent)
    : QQuickItem(parent)
    , m_timerID(0)
{
    setFlag(ItemHasContents);
    setSmooth(false);

    startTimer(16);
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
        node = new UnrimpNode();
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
