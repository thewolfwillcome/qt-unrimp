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
// #include "UnrimpExamples/VertexBuffer/VertexBuffer.h"
// #include "UnrimpExamples/FirstTexture/FirstTexture.h"
#include "UnrimpExamples/FirstRenderToTexture/FirstRenderToTexture.h"
#include "UnrimpExamples/FirstInstancing/FirstInstancing.h"
// #include "UnrimpExamples/FirstMultipleRenderTargets/FirstMultipleRenderTargets.h"
// #include "UnrimpExamples/FirstGeometryShader/FirstGeometryShader.h"
// #include "UnrimpExamples/FirstTessellation/FirstTessellation.h"
// #include "UnrimpExamples/Fxaa/Fxaa.h"
#ifndef ANDROID // crn_decomp uses malloc_usable_size which doesn't exits on android
#include "UnrimpExamples/InstancedCubes/InstancedCubes.h"
#endif
// #include "UnrimpExamples/FirstPostProcessing/FirstPostProcessing.h"
// #include "UnrimpExamples/IcosahedronTessellation/IcosahedronTessellation.h"
// #include "UnrimpExamples/FirstFont/FirstFont.h"
// #include "UnrimpExamples/FirstAssimp/FirstAssimp.h"
// #include "UnrimpExamples/AssimpMesh/AssimpMesh.h"

ExampleModel::ExampleModel(QObject *parent)
	: QAbstractListModel(parent), m_examples({
					// Basic
 					new ExampleItem("FirstTriangle",				"Basic",	&ExampleFabricator<FirstTriangle>, this ),
// 					new ExampleItem("VertexBuffer",					"Basic",	&ExampleFabricator<VertexBuffer>, this ),
// 					new ExampleItem("FirstTexture",					"Basic",	&ExampleFabricator<FirstTexture>, this ),
					new ExampleItem("FirstRenderToTexture",			"Basic",	&ExampleFabricator<FirstRenderToTexture>, this ),
// 					new ExampleItem("FirstPostProcessing",			"Basic",	&ExampleFabricator<FirstPostProcessing>, this ),
					new ExampleItem("FirstInstancing",				"Basic",	&ExampleFabricator<FirstInstancing>, this ),
// 					new ExampleItem("FirstMultipleRenderTargets",	"Basic",	&ExampleFabricator<FirstMultipleRenderTargets>, this ),
// 					new ExampleItem("FirstGeometryShader",			"Basic",	&ExampleFabricator<FirstGeometryShader>, this ),
// 					new ExampleItem("FirstTessellation",			"Basic",	&ExampleFabricator<FirstTessellation>, this ),
// 					// Advanced
// 					new ExampleItem("Fxaa",							"Advanced",	&ExampleFabricator<Fxaa>, this ),
#ifndef ANDROID // crn_decomp uses malloc_usable_size which doesn't exits on android
					new ExampleItem("InstancedCubes",				"Advanced",	&ExampleFabricator<InstancedCubes>, this ),
#endif
// 					new ExampleItem("IcosahedronTessellation",		"Advanced",	&ExampleFabricator<IcosahedronTessellation>, this ),
// 					new ExampleItem("FirstFont",					"Advanced",	&ExampleFabricator<FirstFont>, this ),
// 					new ExampleItem("FirstAssimp",					"Advanced",	&ExampleFabricator<FirstAssimp>, this ),
// 					new ExampleItem("AssimpMesh",					"Advanced",	&ExampleFabricator<AssimpMesh>, this ),
					})
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

	const ExampleItem *animal = m_examples[index.row()];

	if (role == TypeRole)
		return animal->type();
	else if (role == NameRole)
		return animal->name();
	return QVariant();
}

QHash<int, QByteArray> ExampleModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[TypeRole] = "type";
	roles[NameRole] = "name";
	return roles;
}
