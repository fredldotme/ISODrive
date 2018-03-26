import QtQuick 2.0
import Sailfish.Silica 1.0
import QtSparql 1.0

Page {
    id: page

    SparqlListModel {
        id: queryModel
        objectName: "queryModel"

        connection: SparqlConnection {
            id:sparqlConnection;
            objectName: "sparqlConnection";
            driver: "QTRACKER_DIRECT"
        }

        query: "SELECT ?filePath ?fileName WHERE { ?fileUri a nfo:FileDataObject ; "+
               "nie:url $filePath ; nfo:fileName ?fileName. " +
               "FILTER (fn:ends-with(fn:lower-case(nfo:fileName(?fileUri)), '.iso')) }"
    }

    Timer {
        id: delayedReload
        interval: 100
        repeat: false
        onTriggered: queryModel.reload()
    }

    Connections {
        target: isoManager
        onSelectedISOChanged: queryModel.reload()
    }
    SilicaListView {
        id: listView
        anchors.fill: parent
        model: queryModel
        header: PageHeader {
            title: qsTr("ISOs")
        }
        delegate: Component {
            ListItem {
                id: delegate
                contentHeight: Theme.itemSizeSmall

                menu: Component {
                    ContextMenu {
                        id: isoCtxMenu
                        MenuItem {
                            text: qsTr("Delete")
                            onClicked: {
                                showRemorseItem();
                            }
                        }
                    }
                }

                function showRemorseItem() {
                    var idx = index
                    remorse.execute(delegate, qsTr("Deleting"), function() {
                        var deleteSuccess = fileManager.removeFile(filePath)
                        console.log("ISO file deletion success? " + deleteSuccess)
                        delayedReload.restart();
                    }, 3000)
                }

                TextSwitch {
                    id: textSwitch
                    x: Theme.paddingLarge
                    text: fileName
                    anchors.verticalCenter: parent.verticalCenter
                    checked: isoManager.isEnabledISO(filePath)

                    property var isoContextMenu : null

                    onClicked: {
                        if(isoManager.isEnabledISO(filePath) && !checked) {
                            isoManager.resetISO();
                        }

                        if(checked) {
                            isoManager.enableISO(filePath);
                            checked = isoManager.isEnabledISO(filePath)
                        }
                    }

                    onPressAndHold: {
                        delegate.showMenu()
                    }
                }

                RemorseItem { id: remorse }
            }
        }

        ViewPlaceholder {
            text: qsTr("No ISO file found on the device or external storage")
            enabled: queryModel.count < 1
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Eject")
                enabled: (isoManager.selectedISO.trim() !== "")
                onClicked: isoManager.resetISO()

            }
            MenuItem {
                text: qsTr("Refresh")
                onClicked: queryModel.reload()
            }
        }

        VerticalScrollDecorator {}
    }
}


