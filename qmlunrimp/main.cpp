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
#include "unrimpitem.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QStandardPaths>

struct bla
{
	int a;
	float b;
};

int main(int argc, char **argv)
{
	//QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

	qmlRegisterType<UnrimpItem>("Unrimp", 1, 0, "UnrimpItem");
	qmlRegisterType<ExampleModel>("Unrimp", 1, 0, "ExampleModel");
	qmlRegisterUncreatableType<ExampleItem>("Unrimp", 1, 0, "ExampleItem", "not createable");

    QString dataPath = QStandardPaths::displayName(QStandardPaths::AppDataLocation);

    QString fdataPath = QStandardPaths::locate(QStandardPaths::AppDataLocation, "test.txt");

    QUrl url("assets:/qml/main.qml");
    QString tt = url.url();
    QString tt1 = url.path();


    QQmlApplicationEngine engine;
    #ifdef Q_OS_ANDROID
    engine.load(QUrl("assets:/qml/main.qml"));
    #else
    engine.load(QUrl(QLatin1String("android/assets/qml/main.qml")));
    #endif

	return app.exec();
}
