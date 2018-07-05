import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

RowLayout {
    id: layout
    spacing: 5

    InfoCard {
        text: "0"
        subText: "Logged yesterday"
    }

    InfoCard {
        text: "0"
        subText: "Logged today"
    }

    InfoCard {
        text: "0"
        subText: "Logged this week"
    }

}
