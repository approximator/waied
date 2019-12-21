import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import waied 1.0

Pane {
    signal logTime(string taskKey, string timeStr)

    RowLayout {
        width: parent.width

        Label {
            Layout.preferredWidth: 100
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

        Column {
            Row {
                spacing: 5
                height: childrenRect.height

                ComboBox {
                    id: logTimeInput
                    currentIndex: 1
                    // editable: true
                    // validator: RegExpValidator { regExp: /(\d{1,2})([.]\d{1,2})?([hd]\d{1})$/ }
                    model: ["0.5h", "1h", "2h", "4h", "6h", "1d"]
                }
                Button {
                    text: "Log"
                    onClicked: logTime(key, logTimeInput.currentText)
                }
            }
        }
    }
}
