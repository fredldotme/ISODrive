import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    property var isoList: fileManager.getISOFiles();
    id: page

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("Refresh file list")
                onClicked: {
                    isoList = fileManager.getISOFiles();
                    console.log(isoManager.enabled());
                }
            }
            MenuItem {
                text: qsTr("Unmount ISO")
                onClicked: {
                    isoManager.resetISO();
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

                Label {
                    x: Theme.paddingLarge
                    text: isoList[index]
                    anchors.verticalCenter: parent.verticalCenter
                    color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
                }
                onClicked: {
                    console.log(isoList[index]);
                    isoManager.enableISO(isoList[index]);
                }
            }
            VerticalScrollDecorator {}
        }
    }
}


