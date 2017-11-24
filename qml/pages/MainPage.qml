import QtQuick 2.0
import Sailfish.Silica 1.0
import QtSparql 1.0

Page {
    property TextSwitch selectedItem;
    id: page

    SparqlListModel {
        id: queryModel
        objectName: "queryModel"

        connection: SparqlConnection {
            id:sparqlConnection;
            objectName:"sparqlConnection";
            driver:"QTRACKER_DIRECT"
        }

        query: "SELECT ?filePath ?fileName WHERE { ?fileUri a nfo:FileDataObject ; "+
               "nie:url $filePath ; nfo:fileName ?fileName. " +
               "FILTER (fn:ends-with(fn:lower-case(nfo:fileName(?fileUri)), '.iso')) }"
    }

    Component.onCompleted: {
        noFileHint.visible = (queryModel.length  === 0);
    }

    Connections {
        target: isoManager
        onSelectedISOChanged: {
            if(isoManager.selectedISO.trim() == "" && selectedItem != undefined) {
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
                    queryModel.reload()
                    noFileHint.visible = (queryModel.length  === 0);
                }
            }
        }

        SilicaListView {
            id: listView
            model: queryModel
            anchors.fill: parent
            header: PageHeader {
                title: qsTr("ISOs")
            }
            delegate: BackgroundItem {
                id: delegate

                TextSwitch {
                    id: textSwitch
                    x: Theme.paddingLarge
                    text: fileName
                    anchors.verticalCenter: parent.verticalCenter
                    checked: isoManager.isEnabledISO(filePath)

                    onClicked: {
                        if(selectedItem != undefined){
                            selectedItem.checked = false;
                        }
                        selectedItem = textSwitch;

                        if(isoManager.isEnabledISO(filePath) && !checked) {
                            isoManager.resetISO();
                        }

                        if(checked) {
                            isoManager.enableISO(filePath);
                            checked = isoManager.isEnabledISO(filePath)
                        }
                    }
                }
            }
            VerticalScrollDecorator {}
        }
    }
}


