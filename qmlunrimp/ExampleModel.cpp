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

#include "ExampleModel.h"

#include "UnrimpExamples/FirstTriangle/FirstTriangle.h"
#include "UnrimpExamples/VertexBuffer/VertexBuffer.h"
#include "UnrimpExamples/FirstTexture/FirstTexture.h"
#include "UnrimpExamples/FirstRenderToTexture/FirstRenderToTexture.h"
#include "UnrimpExamples/FirstInstancing/FirstInstancing.h"
#include "UnrimpExamples/FirstMultipleRenderTargets/FirstMultipleRenderTargets.h"
#include "UnrimpExamples/FirstGeometryShader/FirstGeometryShader.h"
#include "UnrimpExamples/FirstTessellation/FirstTessellation.h"
#include "UnrimpExamples/Fxaa/Fxaa.h"
#include "UnrimpExamples/InstancedCubes/InstancedCubes.h"
#include "UnrimpExamples/FirstPostProcessing/FirstPostProcessing.h"
#include "UnrimpExamples/IcosahedronTessellation/IcosahedronTessellation.h"
#include "UnrimpExamples/FirstFont/FirstFont.h"

ExampleModel::ExampleModel(QObject *parent)
	: m_examples({
					// Basic
					{"FirstTriangle",				"Basic",	&ExampleFabricator<FirstTriangle> },
					{"VertexBuffer",				"Basic",	&ExampleFabricator<VertexBuffer> },
					{"FirstTexture",				"Basic",	&ExampleFabricator<FirstTexture> },
					{"FirstRenderToTexture",		"Basic",	&ExampleFabricator<FirstRenderToTexture> },
					{"FirstPostProcessing",			"Basic",	&ExampleFabricator<FirstPostProcessing> },
					{"FirstInstancing",				"Basic",	&ExampleFabricator<FirstInstancing> },
					{"FirstMultipleRenderTargets",	"Basic",	&ExampleFabricator<FirstMultipleRenderTargets> },
					{"FirstGeometryShader",			"Basic",	&ExampleFabricator<FirstGeometryShader> },
					{"FirstTessellation",			"Basic",	&ExampleFabricator<FirstTessellation> },
					// Advanced
					{"Fxaa",						"Advanced",	&ExampleFabricator<Fxaa> },
					{"InstancedCubes",				"Advanced",	&ExampleFabricator<InstancedCubes> },
					{"IcosahedronTessellation",		"Advanced",	&ExampleFabricator<IcosahedronTessellation> },
					{"FirstFont",					"Advanced",	&ExampleFabricator<FirstFont> },
					}), QAbstractListModel(parent)
{
	
}

int ExampleModel::rowCount(const QModelIndex & parent) const
{
	return m_examples.count();
}

QVariant ExampleModel::data(const QModelIndex & index, int role) const
{
	if (index.row() < 0 || index.row() >= m_examples.count())
		return QVariant();

	const ExampleItem &animal = m_examples[index.row()];

	if (role == TypeRole)
		return animal.type;
	else if (role == NameRole)
		return animal.name;
	return QVariant();
}

const QList<ExampleItem>& ExampleModel::examples() {
	return m_examples;
}

QHash<int, QByteArray> ExampleModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[TypeRole] = "type";
	roles[NameRole] = "name";
	return roles;
}