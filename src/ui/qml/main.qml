import QtQuick 2.10
import QtQuick.Window 2.0
import QtQuick.Controls 2.4

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 1024
    height: 768

    title: qsTr("W A I E D")

    StackView {
        anchors.fill: parent
        initialItem: Qt.resolvedUrl("TasksView.qml")
    }
}
