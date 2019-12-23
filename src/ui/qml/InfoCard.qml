import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Pane {
    property alias text: infoText.text
    property alias subText: subText.text

    Layout.fillWidth: true

    ColumnLayout {
        anchors.fill: parent
        Label {
            id: infoText
            font.pixelSize: 50
            font.bold: true
        }

        Label {
            id: subText
            font.pixelSize: 18
            font.bold: false
        }
    }
}
