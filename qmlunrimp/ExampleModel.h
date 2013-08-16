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
typedef ExampleBase* (*ExampleFabricatorMethod)();

class ExampleItem : public QObject {
	Q_OBJECT
	
	Q_PROPERTY(QString name READ name CONSTANT)
	Q_PROPERTY(QString type READ type CONSTANT)

public:
    explicit ExampleItem(const QString name, const QString type, ExampleFabricatorMethod fab, QObject* parent = 0)
		: m_name(name), m_type(type), f(fab), QObject(parent)
	{
		
	}
	
	QString name() const
	{
		return m_name;
	}
	
	QString type () const
	{
		return m_type;
	}
	
	ExampleFabricatorMethod fabricator() const
	{
		return f;
	}
	
private:
	QString m_name;
	QString m_type;
	ExampleFabricatorMethod f;

};

class ExampleModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum ExampleRoles {
		NameRole = Qt::UserRole + 1,
		TypeRole
	};

	ExampleModel(QObject *parent = 0);

	int rowCount(const QModelIndex & parent = QModelIndex()) const;

	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	
	Q_INVOKABLE ExampleItem* get(int index)
	{
		return m_examples.at(index);
	}

protected:
	virtual QHash<int, QByteArray> roleNames() const;

private:
	QVector<ExampleItem*> m_examples;
};

#endif // EXAMPLEMODEL_H
