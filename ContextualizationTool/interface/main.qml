import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

ApplicationWindow {    
    id : mainWindow
    visible: true
    minimumWidth: Screen.width * 0.4
    minimumHeight: Screen.height * 0.4
    //visibility: "Maximized"
    width: 800
    height: 480
    title: qsTr("Contextualization tool")

    //        style: ApplicationWindowStyle {
    //                background: BorderImage {
    //                    source: "images/wallpaper.jpg"
    //                      width: 180; height: 180
    //                    border { left: 20; top: 20; right: 20; bottom: 20 }
    //                    horizontalTileMode: BorderImage.Repeat
    //                    verticalTileMode: BorderImage.Repeat
    //                }
    //            }


    menuBar:  MenuBar {
        property int height: 21

        id: menuBar

        Menu {
            title: "File"
            MenuItem {
                text : "Exit"
                shortcut : "Ctrl+Q"
                iconSource: "qrc:/images/exit.png"
                onTriggered: Qt.quit()
            }
        }
        Menu {
            title: "Edit"
            MenuItem {
                text: "Copy"
                shortcut: "Ctrl+C"
                iconSource: "qrc:/images/copy.png"
                onTriggered: activeFocusItem.copy()
            }
            MenuItem {
                text: "Paste"
                shortcut: "Ctrl+V"
                iconSource: "qrc:/images/paste.png"
                onTriggered: activeFocusItem.paste()
            }
        }
    }

    //    ContextualizationController {
    //        id: controller
    //        objectName: "controller"
    //        tableModel: customModel
    //    }

    ScrollView {

        property int barWidth: 17

        id: mainScroll
        anchors.fill: parent
        verticalScrollBarPolicy: Qt.ScrollBarAsNeeded
        horizontalScrollBarPolicy: Qt.ScrollBarAsNeeded

        RowLayout {
            property int minWidth: Screen.width * 0.5
            property int minHeight: Screen.height * 0.5

            id: container
            width: calculateWidth()
            height: calculateHeight()

            function calculateWidth() {
                if (mainWindow.width < minWidth)
                    return minWidth;
                else
                    return mainWindow.width - mainScroll.barWidth;
            }

            function calculateHeight() {
                if (mainWindow.height < minHeight)
                    return minHeight;
                else
                    return mainWindow.height - mainScroll.barWidth - menuBar.height;
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.topMargin: 18
                Layout.bottomMargin: 10
                Layout.rightMargin: 10
                Layout.leftMargin: 18

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    border.width : 1
                    border.color: "black"
                    clip: true
                    Image {
                        objectName: "containerImage"
                        cache: false
                        source: "https://i.ytimg.com/vi/Qv1ffQL1-i8/maxresdefault.jpg"
                        fillMode: Image.PreserveAspectFit
                        asynchronous: true
                        anchors.fill: parent
                        anchors.margins: 1
                    }
                }

                CheckBox {
                    id : autoDetectCheckBox
                    text: "Auto detect new strings when load image"
                    Layout.alignment: Qt.AlignHCenter
                }
                RowLayout {
                    Layout.fillWidth: true
                    Layout.topMargin: 20

                    Item {
                        Layout.fillWidth: true
                    }
                    RowLayout {
                        Layout.preferredWidth: autoDetectCheckBox.width
                        Layout.maximumWidth: autoDetectCheckBox.width
                        Layout.minimumWidth: autoDetectCheckBox.width

                        Button {
                            id: captureAreaButon
                            objectName: "captureAreaButon"
                            text: "CAPTURE AREA"
                            Layout.preferredHeight: 45
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                        Button {
                            id: loadImageButton
                            objectName: "loadImageButton"
                            text: "LOAD IMAGE"
                            Layout.preferredWidth: captureAreaButon.width
                            Layout.preferredHeight: 45
                        }
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                }
            }
            ColumnLayout {
                id: bb
                Layout.preferredWidth: parent.width * 0.45
                Layout.maximumWidth: parent.width * 0.45
                Layout.alignment: Qt.AlignRight
                Layout.topMargin: 18
                Layout.bottomMargin: 10

                RowLayout {
                    Layout.fillWidth: true

                    TextField {
                        id: newStringInput
                        placeholderText: qsTr("New string to add...")
                        Layout.fillWidth: true
                    }
                    Button {

                        signal customClicked(string newString)

                        id: addStringButton
                        objectName: "addStringButton"
                        text : "ADD"

                        onClicked: customClicked(newStringInput.text)
                    }
                }
                TableView {
                    id: stringsTable
                    objectName: "stringsTable"
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    TableViewColumn {
                        id: checkboxsColumn
                        role: "checkboxsColumn"
                        width: 20
                        resizable: false
                        horizontalAlignment: Text.AlignHCenter

                        delegate: Item {
                            anchors.fill: parent
                            CheckBox {
                                enabled: true
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter

                                onClicked: {
                                    //var srcIndex = prjCont.model.mapToSource(prjCont.model.index(styleData.row,0));
                                    //prjCont.model.getEntry(srcIndex.row).checked = !prjCont.model.getEntry(styleData.row).checked;

                                    console.debug("row in Table " + styleData.row);
                                    //console.debug("-got: " +  prjCont.model.getEntry(srcIndex.row).checked);
                                }
                            }
                        }
                    }

                    TableViewColumn {
                        role: "stringkey"
                        title: "String Key"
                        horizontalAlignment: Text.AlignHCenter
                        width: (stringsTable.width - checkboxsColumn.width - buttonsColumn.width) * 0.35
                    }

                    TableViewColumn {
                        role: "string"
                        title: "String"
                        horizontalAlignment: Text.AlignHCenter
                        width: (stringsTable.width - checkboxsColumn.width - buttonsColumn.width) * 0.65
                    }

                    TableViewColumn {
                        signal buttonClicked (int row)

                        id: buttonsColumn
                        objectName: "buttonsColumn"
                        role: "buttonsColumn"
                        width: 25
                        resizable: false

                        delegate: Button {
                            signal buttonClicked (int row)

                            objectName: "deleteStringButton"
                            enabled: true

                            Image {
                                objectName: "captureImage"
                                anchors.fill: parent
                                source: "qrc:/images/trash.png"
                                fillMode: Image.PreserveAspectFit
                            }

                            onClicked: buttonsColumn.buttonClicked(styleData.row)
                        }
                    }
                    //model: customModel
                }

                RowLayout {
                    id: buttonContainer
                    Layout.fillWidth: true

                    Button {
                        id: detectStringsButton
                        text: "DETECT STRINGS ON IMAGE"
                        Layout.alignment: Qt.AlignRight
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    Button {
                        id: clearButton
                        objectName: "clearButton"
                        text: "CLEAR"
                        Layout.alignment: Qt.AlignLeft
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignBottom
                    Layout.topMargin: 20
                    spacing: 20

                    Item {
                        Layout.fillWidth: true
                    }

                    Button {
                        id: cancelButton
                        objectName: "cancelButton"
                        Layout.preferredHeight: 45
                        text: "CANCEL"

                    }

                    Button {
                        id: sendButton
                        objectName: "sendButton"
                        Layout.preferredHeight: 45
                        text: "SEND"
                    }
                }
            }
        }
    }
}
