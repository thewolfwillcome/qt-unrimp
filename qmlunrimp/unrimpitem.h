
#ifndef UNRIMPITEM_H
#define UNRIMPITEM_H

#include <QtQuick/QQuickItem>
#include <QtCore/QPropertyAnimation>

class UnrimpNode;

class UnrimpItem : public QQuickItem
{
    Q_OBJECT
    
    Q_PROPERTY(QString exampleName READ example WRITE setExample NOTIFY exampleChanged)

public:
    UnrimpItem(QQuickItem *parent = 0);
	QString example();
	void setExample(QString exampleName);
	
Q_SIGNALS:	
    //void exampleChanged(QString);
	void exampleChanged();

protected:
    virtual QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    void timerEvent(QTimerEvent *);

private:
    int m_timerID;
	UnrimpNode *m_node;
};

#endif // UNRIMPITEM_H
