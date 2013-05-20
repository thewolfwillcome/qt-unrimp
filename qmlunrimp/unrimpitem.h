
#ifndef UNRIMPITEM_H
#define UNRIMPITEM_H

#include <QtQuick/QQuickItem>
#include <QtCore/QPropertyAnimation>

class UnrimpItem : public QQuickItem
{
    Q_OBJECT

public:
    UnrimpItem(QQuickItem *parent = 0);

protected:
    virtual QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    void timerEvent(QTimerEvent *);

private:
    int m_timerID;
};

#endif // UNRIMPITEM_H
