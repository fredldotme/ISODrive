import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {

    Image {
        id: icon
        source: "image://theme/ISODrive"
        anchors.centerIn: parent
    }

    Label {
        id: coverLabel
        text: isoManager.selectedISO
        wrapMode: Text.WrapAnywhere
        width: parent.width
        anchors.centerIn: parent
        horizontalAlignment: Text.AlignHCenter
    }


    CoverActionList {
        id: coverAction
        enabled: isoManager.selectedISO.trim() !== ""

        CoverAction {
            iconSource: "image://theme/icon-cover-cancel"
            onTriggered: {
                isoManager.resetISO();
            }
        }

    }
}


