import QtQuick 2.0

ListView {
    width: parent.width

    model: ListModel {
        ListElement {
            title: "Task 1"
            timeLogged: "1h"
            status: "in progress"
        }
        ListElement {
            title: "Task 2"
            timeLogged: "5h"
            status: "resolved"
        }
        ListElement {
            title: "Task 3"
            timeLogged: "2h"
            status: "closed"
        }
    }
    delegate: TaskListItem {}
}
