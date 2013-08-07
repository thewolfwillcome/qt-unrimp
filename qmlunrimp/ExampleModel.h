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


#ifndef EXAMPLEMODEL_H
#define EXAMPLEMODEL_H

#include <QAbstractListModel>

class UnrimpNode;
class ExampleBase;
typedef ExampleBase* (*FabricatorMethod)();

struct ExampleItem {
	QString name;
	QString type;
	FabricatorMethod f;

	ExampleBase* operator()() const
	{
		if (!f)
			return nullptr;
		return f();
	}
};

class ExampleModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum AnimalRoles {
		NameRole = Qt::UserRole + 1,
		TypeRole
	};

	ExampleModel(QObject *parent = 0);

	int rowCount(const QModelIndex & parent = QModelIndex()) const;

	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

	const QList<ExampleItem>& examples();

protected:
	virtual QHash<int, QByteArray> roleNames() const;

private:
	QList<ExampleItem> m_examples;
};

#endif // EXAMPLEMODEL_H
