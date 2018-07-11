#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QSettings>
#include <QtQml>

#include "taskmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    // Set up settings
    app.setOrganizationName("waied");
    app.setApplicationName("waied");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    qmlRegisterType<TaskManager>("waied", 1, 0, "TaskManager");
    qRegisterMetaType<TTasksModel *>("TTasksModel*");
    qRegisterMetaType<std::chrono::seconds>("std::chrono::seconds");

    QQmlApplicationEngine engine;
    QQuickStyle::setStyle("Material");
    engine.load(QUrl(QStringLiteral("qrc:/ui/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
