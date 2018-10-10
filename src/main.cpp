#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFontDatabase>
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

    QFontDatabase fontDatabase;
    if (fontDatabase.addApplicationFont(":/fonts/fontello.ttf") == -1) {
        qWarning() << "Failed to load fontello.ttf";
    }

    qmlRegisterType<TaskManager>("waied", 1, 0, "TaskManager");
    qRegisterMetaType<TTasksModel *>("TTasksModel*");
    qRegisterMetaType<std::chrono::seconds>("std::chrono::seconds");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
