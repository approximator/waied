import QtQuick 2.0
import QtQuick.Controls 2.2
import waied 1.0

Page {
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
