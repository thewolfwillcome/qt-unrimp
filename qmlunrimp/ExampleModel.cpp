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
#include <Basics/FirstTriangle/FirstTriangle.h>
#include <Basics/FirstIndirectBuffer/FirstIndirectBuffer.h>
#include <Basics/VertexBuffer/VertexBuffer.h>
#include <Basics/FirstTexture/FirstTexture.h>
#include <Basics/FirstRenderToTexture/FirstRenderToTexture.h>
#include <Basics/FirstInstancing/FirstInstancing.h>
#include <Basics/FirstMultipleRenderTargets/FirstMultipleRenderTargets.h>
#include <Basics/FirstGeometryShader/FirstGeometryShader.h>
#include <Basics/FirstTessellation/FirstTessellation.h>
#ifndef ANDROID // Generates runtime errors such as "W/Adreno-GSL( 1320): <gsl_ldd_control:405>: ioctl fd 30 code 0x400c0907 (IOCTL_KGSL_DEVICE_WAITTIMESTAMP_CTXTID) failed: errno 35 Resource deadlock would occur" an my GLES 3 device with Adreno 330 GPU
#include <Advanced/InstancedCubes/InstancedCubes.h>
#endif
#include <Runtime/FirstMesh/FirstMesh.h>
#include <Runtime/FirstScene/FirstScene.h>


template <class ExampleClass>
ExampleFabricatorMethod createFabricator(const char* name, bool cyclicUpdate = false, bool needsRendererRuntime = false)
{
	auto f =[name, cyclicUpdate, needsRendererRuntime]() {
		return new UnrimpExample(std::unique_ptr<ExampleBase>(new ExampleClass()), {name, cyclicUpdate, needsRendererRuntime});
	};
	return f;
}

ExampleModel::ExampleModel(QObject *parent)
	: QAbstractListModel(parent), m_examples({
					// Basic
   					new ExampleItem("FirstTriangle",				"Basic",	createFabricator<FirstTriangle>("FirstTriangle", false), this ),
// 					new ExampleItem("FirstIndirectBuffer",			"Basic",	createFabricator<FirstIndirectBuffer>("FirstIndirectBuffer", false), this ),
// 					new ExampleItem("VertexBuffer",					"Basic",	createFabricator<VertexBuffer>("VertexBuffer", false), this ),
// 					new ExampleItem("FirstTexture",					"Basic",	createFabricator<FirstTexture>("FirstTexture", false), this ),
// 					new ExampleItem("FirstRenderToTexture",			"Basic",	createFabricator<FirstRenderToTexture>("FirstRenderToTexture", false), this ),
// 					new ExampleItem("FirstInstancing",				"Basic",	createFabricator<FirstInstancing>("FirstInstancing", false), this ),
// 					new ExampleItem("FirstMultipleRenderTargets",	"Basic",	createFabricator<FirstMultipleRenderTargets>("FirstMultipleRenderTargets", false), this ),
// 					new ExampleItem("FirstGeometryShader",			"Basic",	createFabricator<FirstGeometryShader>("FirstGeometryShader", false), this ),
// 					new ExampleItem("FirstTessellation",			"Basic",	createFabricator<FirstTessellation>("FirstTessellation", false), this ),
					// Advanced
// #ifndef ANDROID // Generates runtime errors such as "W/Adreno-GSL( 1320): <gsl_ldd_control:405>: ioctl fd 30 code 0x400c0907 (IOCTL_KGSL_DEVICE_WAITTIMESTAMP_CTXTID) failed: errno 35 Resource deadlock would occur" an my GLES 3 device with Adreno 330 GPU
// 					new ExampleItem("InstancedCubes",				"Advanced",	createFabricator<InstancedCubes>("InstancedCubes", true, true), this ),
// #endif
					new ExampleItem("FirstMesh",				"Advanced",	createFabricator<FirstMesh>("FirstMesh", true, true), this ),
					new ExampleItem("FirstScene",				"Advanced",	createFabricator<FirstScene>("FirstScene", true, true), this )
					})
{
}

int ExampleModel::rowCount(const QModelIndex& ) const
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
