import QtQuick 2.10
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4

ApplicationWindow {
    visible: true

    Material.theme: Material.Dark

    width: 1024
    height: 768
    title: qsTr("Hello World")

    Summary {
        id: summary
        anchors.top: parent.top
        width: parent.width
    }

    TaskList {
        width: parent.width
        anchors.top: summary.bottom
        anchors.bottom: parent.bottom
    }
}
