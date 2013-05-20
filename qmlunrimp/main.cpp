#include "unrimpitem.h"

#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlContext>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<UnrimpItem>("Unrimp", 1, 0, "UnrimpItem");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl::fromLocalFile("resources/example.qml"));
    view.rootContext()->setContextProperty("Window", &view);
    view.show();
    view.raise();

    return app.exec();
}
