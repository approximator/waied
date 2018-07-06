import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

Pane {
    width: parent.width
    Material.elevation: 1

    ColumnLayout {
        anchors.fill: parent
        Label {
            text: title
            font.pixelSize: 20
            font.bold: false
        }

        Label {
            text: timeSpent
            font.pixelSize: 12
            font.bold: false
        }
    }
}
