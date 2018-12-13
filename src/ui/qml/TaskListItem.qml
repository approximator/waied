import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Pane {
    RowLayout {
        width: parent.width

        Label {
            Layout.preferredWidth: 100
            height: childrenRect.height
            horizontalAlignment: Text.AlignHCenter
            text: status
            font.pixelSize: 15
            font.bold: true
            color: (status == "Resolved" || status == "Closed") ? "green" : "orange"
        }

        Column {
            Layout.fillWidth: true
            height: childrenRect.height

            Label {
                text: title
                font.pixelSize: 20
                font.bold: false
            }

            Row {
                spacing: 20
                Label {
                    color: "steelblue"
                    text: "You spent on this task: " + currentUserSpentStr
                }
            }
        }
    }
}
