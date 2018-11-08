import QtQuick 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2
import Qt.labs.settings 1.0

Page {
    id: settingsPage

    signal saved(string apiUrl, string username, string pass)
    signal cancelled

    Pane {
        padding: 50
        anchors.centerIn: parent
        Material.elevation: 7

        ColumnLayout {
            spacing: 10
            anchors.centerIn: parent

            Text {
                text: qsTr("Enter Your Jira Info")
                font.capitalization: Font.SmallCaps
                font.pointSize: 20
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

            TextField {
                id: apiUrl
                Layout.fillWidth: true
                text: jiraSettings.apiUrl
                placeholderText: qsTr("Jira URL → https://jira.example.com/jira/rest/api/2")
                selectByMouse: true
            }
            TextField {
                id: username
                Layout.fillWidth: true
                text: jiraSettings.username
                placeholderText: qsTr("User name")
                selectByMouse: true
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                TextField {
                    id: pass
                    Layout.fillWidth: true
                    text: jiraSettings.pass
                    placeholderText: qsTr("P@ssw0rd")
                    echoMode: TextInput.Password
                    selectByMouse: true
                }
                Text {
                    // "\uE803"  icon-eye
                    // "\uE804"  icon-eye-off
                    text: (pass.echoMode == TextInput.Normal ? "\uE804" : "\uE803")
                    font.family: "fontello"
                    font.pointSize: 20
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            pass.echoMode = (pass.echoMode == TextInput.Normal ? TextInput.Password : TextInput.Normal)
                            pass.forceActiveFocus()
                        }
                    }
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                Button {
                    text: qsTr("Save")
                    Layout.fillWidth: true
                    onClicked: {
                        jiraSettings.apiUrl = apiUrl.text
                        jiraSettings.username = username.text
                        jiraSettings.pass = pass.text

                        settingsPage.saved(apiUrl.text, username.text, pass.text)
                    }
                }

                Button {
                    text: qsTr("Cancel")
                    Layout.fillWidth: true
                    onClicked: settingsPage.cancelled()
                }
            }
        }
    }

    Settings {
        id: jiraSettings
        category: "jira"
        property string apiUrl
        property string username
        property string pass
    }
}
