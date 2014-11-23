import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    property var isoList;
    property var selectedItem;
    id: page

    Component.onCompleted: {
        isoList = fileManager.getISOFiles();
        noFileHint.visible = (isoList.count > 0)
    }

    Connections {
        target: isoManager
        onSelectedISOChanged: {
            if(isoManager.selectedISO == "none" && selectedItem != undefined) {
                selectedItem.checked = false
            }
        }
    }

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    isoList = fileManager.getISOFiles();
                    noFileHint.visible = (isoList.count > 0)
                }
            }
        }

        SilicaListView {
            id: listView
            model: isoList
            anchors.fill: parent
            header: PageHeader {
                title: qsTr("ISOs")
            }
            delegate: BackgroundItem {
                id: delegate

                TextSwitch {
                    id: textSwitch
                    x: Theme.paddingLarge
                    text: isoList[index]
                    anchors.verticalCenter: parent.verticalCenter
                    //text.folor: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
                    checked: isoManager.selectedISO === isoList[index]

                    onCheckedChanged: {
                        if(isoManager.selectedISO === isoList[index] && !checked) {
                            isoManager.resetISO();
                        }

                        if(checked) {
                            isoManager.enableISO(isoList[index]);
                            selectedItem = textSwitch
                        }
                    }
                }
            }
            VerticalScrollDecorator {}

            Label {
                id: noFileHint
                anchors.centerIn: parent
                text: qsTr("No ISO file in the 'Downloads' folder")
                font.pixelSize: Theme.largeFontSize
                visible: true
            }
        }
    }
}


