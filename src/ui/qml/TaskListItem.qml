import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Pane {
    width: 100

    Column {
        id: itemLayout
        width: parent.width
        height: childrenRect.height

        Label {
            width: parent.width
            text: title
            font.pixelSize: 20
            font.bold: false
        }

        Label {
            width: parent.width
            text: timeSpentStr
            font.pixelSize: 12
            font.bold: false
        }
    }
}
