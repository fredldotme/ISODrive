import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    property var isoList;
    property TextSwitch selectedItem;
    id: page

    Component.onCompleted: {
        isoList = fileManager.getISOFiles();
        noFileHint.visible = (isoList.length  === 0);
    }

    Connections {
        target: isoManager
        onSelectedISOChanged: {
            if(isoManager.selectedISO == "" && selectedItem != undefined) {
                selectedItem.checked = false
            }
        }
    }

    SilicaFlickable {
        anchors.fill: parent

        ViewPlaceholder {
            id: noFileHint
            anchors.centerIn: parent
            text: qsTr("No ISO file in the 'Downloads' folder")
            enabled: listView.count == 0
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Refresh")
                onClicked: {
                    isoList = fileManager.getISOFiles();
                    noFileHint.visible = (isoList.length  === 0);
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
                    checked: isoManager.selectedISO === isoList[index]

                    onClicked: {
                        if(selectedItem != undefined){
                            selectedItem.checked = false;
                        }
                        selectedItem = textSwitch;

                        if(isoManager.selectedISO === isoList[index] && !checked) {
                            isoManager.resetISO();
                        }

                        if(checked) {
                            isoManager.enableISO(isoList[index]);
                            if(isoManager.selectedISO !== isoList[index]) {
                                checked = false;
                            }
                        }
                    }
                }
            }
            VerticalScrollDecorator {}
        }
    }
}


