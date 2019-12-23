#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFontDatabase>
#include <QSettings>
#include <QtQml>

#include "taskmanager.h"

Q_IMPORT_PLUGIN(QtQuick2Plugin)
Q_IMPORT_PLUGIN(QtQuick2WindowPlugin)
Q_IMPORT_PLUGIN(QtQuickControls2Plugin)
Q_IMPORT_PLUGIN(QtQuickTemplates2Plugin)
Q_IMPORT_PLUGIN(QtQuickLayoutsPlugin)
Q_IMPORT_PLUGIN(QtQuickControls2FusionStylePlugin)
// Q_IMPORT_PLUGIN(QtQuickControls2MaterialStylePlugin)
//Q_IMPORT_PLUGIN(QtQuickControls2UniversalStylePlugin)
Q_IMPORT_PLUGIN(QmlSettingsPlugin)

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{time process}  [%{type}] %{if-debug}%{file}:%{line}%{endif}    %{message}");
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
    if (fontDatabase.addApplicationFont(":/fonts/dejavu-fonts-ttf-2.37/ttf/DejaVuSans.ttf") == -1) {
        qWarning() << "Failed to load fontello.ttf";
    }

    qWarning() << fontDatabase.families();

    qmlRegisterType<TaskManager>("waied", 1, 0, "TaskManager");
    qRegisterMetaType<TTasksModel *>("TTasksModel*");
    qRegisterMetaType<std::chrono::seconds>("std::chrono::seconds");
    qRegisterMetaType<std::chrono::system_clock::time_point>("std::chrono::system_clock::time_point");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
