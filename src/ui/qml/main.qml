import QtQuick 2.10
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4

import waied 1.0

ApplicationWindow {
    visible: true

    Material.theme: Material.Dark

    width: 1024
    height: 768
    title: qsTr("Hello World")

    TaskManager {
        id: taskManager
    }

    Summary {
        id: summary
        loggedToday: taskManager.reportedTodayStr
        loggedYesterday: taskManager.reportedYesterdayStr
        loggedThisWeek: taskManager.reportedThisWeekStr

        anchors.top: parent.top
        width: parent.width
    }

    TaskList {
        model: taskManager.model
        width: parent.width
        anchors.top: summary.bottom
        anchors.bottom: parent.bottom
    }
}
