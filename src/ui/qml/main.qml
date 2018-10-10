import QtQuick 2.10
import QtQuick.Window 2.0
import QtQuick.Controls 2.4
import Qt.labs.settings 1.0

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 1024
    height: 768

    title: qsTr("W A I E D")

    StackView {
        id: stackView

        anchors.fill: parent
        initialItem: TasksView {
            id: taskView
        }
    }

    Component.onCompleted: {
        if (jiraSettings.apiUrl === "" || jiraSettings.username === "" || jiraSettings.pass === "") {
            showSettings()
        }
    }

    Settings {
        id: jiraSettings
        category: "jira"
        property string apiUrl
        property string username
        property string pass
    }

    Settings {
        category: "Window"
        property alias x: mainWindow.x
        property alias y: mainWindow.y
        property alias width: mainWindow.width
        property alias height: mainWindow.height
    }

    function onSettingsSaved(apiUrl, username, pass) {
        stackView.pop()
        taskView.taskManager.updateSettings(apiUrl, username, pass)
        taskView.taskManager.updateTasks()
    }

    function showSettings() {
        if (stackView.currentItem.objectName === "SettingsPage") {
            return
        }

        var infoView = stackView.push(Qt.resolvedUrl("UserInfoView.qml"), {"objectName": "SettingsPage"})
        infoView.onSaved.connect(onSettingsSaved)
        infoView.onCancelled.connect(stackView.pop)
    }
}
