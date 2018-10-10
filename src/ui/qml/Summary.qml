import QtQuick 2.0
import QtQuick.Layouts 1.3

RowLayout {
    property alias loggedToday: loggedToday.text
    property alias loggedYesterday: loggedYesterday.text
    property alias loggedThisWeek: loggedThisWeek.text

    id: layout
    spacing: 5

    InfoCard {
        id: loggedYesterday
        subText: "Logged yesterday"
    }

    InfoCard {
        id: loggedToday
        subText: "Logged today"
    }

    InfoCard {
        id: loggedThisWeek
        subText: "Logged this week"
    }

}
