import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import waied 1.0

Page {
    property TaskManager taskManager: taskManager

    TaskManager {
        id: taskManager
    }

    header: ToolBar {
        RowLayout {
            anchors.fill: parent

            ToolButton {
                id: btnUpdate
                text: "\uE800" // icon-update
                font.family: "fontello"
                onClicked: taskManager.updateTasks()
            }
            ToolButton {
                id: btnSettings
                text: "\uE801" // icon-settings
                font.family: "fontello"
                onClicked: showSettings()
            }
            ToolSeparator { }
            ComboBox {
                id: sortBy
                Layout.preferredWidth: 400
                displayText: "Sort by: " + currentText
                model: ["updated", "lastCurrentUserLog", "status"]

                onCurrentTextChanged: taskManager.model.sortBy = sortBy.currentText
            }

            Item {
                Layout.fillWidth: true
            }
            ToolSeparator { }
            ToolButton {
                Layout.alignment: Qt.AlignLeft
                id: btnExit
                text: "\uE802" // icon-exit
                font.family: "fontello"
                onClicked: Qt.exit(0)
            }
        }
    }

    Summary {
        id: summary
        loggedToday: taskManager.reportedTodayStr
        loggedYesterday: taskManager.reportedYesterdayStr
        loggedThisWeek: taskManager.reportedThisWeekStr

        anchors.top: parent.top
        width: parent.width
    }

    ListView {
        id: taskList
        clip: true
        model: taskManager.model
        width: parent.width
        anchors.top: summary.bottom
        anchors.topMargin: 5
        anchors.bottom: parent.bottom

        delegate: TaskListItem {
            width: taskList.width
        }
    }
}
