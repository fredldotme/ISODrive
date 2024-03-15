import QtQuick 2.0
import Sailfish.Silica 1.0
import QtDocGallery 5.0

Page {
    id: page

    DocumentGalleryModel {
        id: queryModel
        properties: ["url", "fileName"]
        sortProperties: ["+fileName"]
        rootType: DocumentGallery.File
        filter: GalleryFilterUnion {
            filters: [
                GalleryEqualsFilter { property: "fileExtension"; value: "iso" },
                GalleryEqualsFilter { property: "fileExtension"; value: "ISO" }
            ]
        }
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
                        var deleteSuccess = fileManager.removeFile(url)
                        console.log(url+"ISO file deletion success? " + deleteSuccess)
                        delayedReload.restart();
                    }, 3000)
                }

                TextSwitch {
                    id: textSwitch
                    x: Theme.paddingLarge
                    text: fileName
                    anchors.verticalCenter: parent.verticalCenter
                    checked: isoManager.isEnabledISO(url)

                    property var isoContextMenu : null

                    onClicked: {
                        if(isoManager.isEnabledISO(url) && !checked) {
                            isoManager.resetISO();
                        }

                        if(checked) {
                            isoManager.enableISO(url);
                            checked = isoManager.isEnabledISO(url)
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


