import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {

    Column {
        anchors.centerIn: parent

        Row {
            Label {
                text: "Selected ISO:"
            }
        }

        Row {
            Label {
                id: coverLabel
                text: isoManager.selectedISO
            }
        }
    }

    CoverActionList {
        id: coverAction
        enabled: isoManager.selectedISO !== "none"

        CoverAction {
            iconSource: "image://theme/icon-cover-cancel"
            onTriggered: {
                isoManager.resetISO();
            }
        }

    }
}


