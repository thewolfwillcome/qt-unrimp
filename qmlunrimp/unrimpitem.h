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


#ifndef UNRIMPITEM_H
#define UNRIMPITEM_H

#include <QtQuick/QQuickItem>
#include <QtCore/QPropertyAnimation>
#include "ExampleModel.h"

class UnrimpNode;


class UnrimpItem : public QQuickItem
{
	Q_OBJECT

	Q_PROPERTY(QString exampleName READ example NOTIFY exampleChanged)
	Q_PROPERTY(QString openglVersionName READ openglVersionName NOTIFY openglVersionNameChanged)
	Q_PROPERTY(ExampleItem* exampleItem READ exampleItem WRITE setExampleItem NOTIFY exampleChanged)

public:
	UnrimpItem(QQuickItem *parent = 0);
    virtual ~UnrimpItem();
	QString example();
	QString openglVersionName();
	
	ExampleModel* exampleModel();
	
	void setExampleItem(ExampleItem* item);
	ExampleItem* exampleItem();

Q_SIGNALS:
	void exampleChanged();
	void openglVersionNameChanged();

protected:
	virtual QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

	virtual void timerEvent(QTimerEvent *);

private:
	void startCyclicTimer();
	void stopCyclicTimer();

private:
	int m_timerID;
	UnrimpNode *m_node;     // Don't destroy this item -> this is managed by the QScenegraph
	ExampleItem *m_currentExample;
	int m_currentExampleIndex;

	QString mOpenGLVersionName;


};

#endif // UNRIMPITEM_H
