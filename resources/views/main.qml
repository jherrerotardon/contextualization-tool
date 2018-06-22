import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import io.controllers.guicontroller 1.0

ApplicationWindow {
    signal newProjectRequested();
    signal clearRequested()
    signal addRequested(string newString, int findType)
    signal stringRemoved(string idString)
    signal cancelRequested()
    signal sendRequested()
    signal captureRequested(bool detectStringsOnLoad)
    signal loadImageRequested(bool detectStringsOnLoad)
    signal detectStringsRequested()
    signal openRequested()
    signal saveAsRequested()
    signal fpFileConfigRequested()
    signal remoteHostConfigRequested()
    signal validStatesConfigRequested()
    signal refreshRequested()

    id : mainWindow
    visible: true
    minimumWidth: Screen.width * 0.4
    minimumHeight: Screen.height * 0.4
    width: 800
    height: 480
    title: qsTr("Contextualization tool")

    onClosing: {
        close.accepted = false
        cancelRequested()
    }

    style: ApplicationWindowStyle {
        background: BorderImage {
            source: "qrc:/images/background.png"
            horizontalTileMode: BorderImage.Repeat
            verticalTileMode: BorderImage.Stretch
        }
    }


    menuBar:  MenuBar {
        property int height: 21

        id: menuBar

        Menu {
            title: "File"

            MenuItem {
                text : "New Project..."
                shortcut : "Ctrl+N"
                iconSource: "qrc:/images/newProject.png"

                onTriggered: newProjectRequested()
            }

            MenuItem {
                text : "Open Project..."
                shortcut : "Ctrl+O"
                iconSource: "qrc:/images/import.png"

                onTriggered: openRequested()
            }

            MenuItem {
                text : "Save As..."
                shortcut : "Ctrl+S"
                iconSource: "qrc:/images/export.png"

                onTriggered: saveAsRequested()
            }

            MenuItem {
                text : "Refresh"
                shortcut : "Ctrl+R"
                iconSource: "qrc:/images/refresh.png"
                onTriggered: refreshRequested()
            }


            MenuItem {
                text : "Exit"
                shortcut : "Ctrl+Q"
                iconSource: "qrc:/images/exit.png"
                onTriggered: close()
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

        Menu {
            title: "Configuration"

            MenuItem {
                text: "Fp File"
                onTriggered: fpFileConfigRequested()
            }

            MenuItem {
                text: "Remote Host"
                onTriggered: remoteHostConfigRequested()
            }

            MenuItem {
                text: "Valid States"
                onTriggered: validStatesConfigRequested()
            }
        }
    }

    Controller {
        id: controller
        view: mainWindow
    }

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
                    radius: 6
                    color: "#FAFAFA"
                    clip: true

                    Image {
                        objectName: "containerImage"
                        cache: false
                        source: "file:" + controller.image
                        fillMode: Image.PreserveAspectFit
                        asynchronous: true
                        anchors.fill: parent
                        anchors.margins: 1
                    }
                }

                CheckBox {
                    id : autoDetectCheckBox
                    text: "Detect strings when image is loaded"
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
                            style: generalButtonStyle

                            onClicked: captureRequested(autoDetectCheckBox.checked)
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
                            style: generalButtonStyle

                            onClicked: loadImageRequested(autoDetectCheckBox.checked)
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                    }
                }
            }

            ColumnLayout {
                Layout.preferredWidth: parent.width * 0.5
                Layout.maximumWidth: parent.width * 0.5
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

                    ComboBox {
                        id: findTypeComboBox
                        Layout.preferredWidth: 93
                        currentIndex: 0
                        model: ListModel {
                            id: findTypeItems
                            ListElement { text: "by ID" }
                            ListElement { text: "by Value" }
                        }
                    }

                    Button {
                        id: addStringButton
                        objectName: "addStringButton"
                        text : "ADD"
                        style: generalButtonStyle

                        onClicked: addRequested(newStringInput.text, findTypeComboBox.currentIndex)
                    }
                }

                Rectangle {
                    implicitHeight: 22
                    radius: 5
                    Layout.fillWidth: true
                    color: "white"

                    CheckBox {
                        text: qsTr("Only DONE strings")
                        checked: controller.onlyDoneStrings

                        onClicked: controller.onlyDoneStrings = checked
                    }
                }

                TableView {
                    id: stringsTable
                    objectName: "stringsTable"
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    model: controller.tableModel

                    TableViewColumn {
                        id: checkboxsColumn
                        role: "selected"
                        horizontalAlignment: Text.AlignHCenter
                        width: 20
                        resizable: false

                        delegate: Item {
                            anchors.fill: parent
                            CheckBox {
                                enabled: true
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                checked: styleData.value

                                onClicked: modelData.selected = checked
                            }
                        }

                    }

                    TableViewColumn {
                        id: idColumn
                        title: "String Key"
                        role: "id"
                        horizontalAlignment: Text.AlignHCenter
                        width: (stringsTable.width - checkboxsColumn.width - buttonsColumn.width) * 0.45

                        delegate: TextField {
                            anchors.fill: parent
                            height: 25
                            verticalAlignment: Text.AlignVCenter
                            text: styleData.value

                            style: TextFieldStyle
                            {
                                padding.bottom: -1
                                background: Rectangle {
                                    opacity: 0
                                }


                            }
                        }
                    }

                    TableViewColumn {
                        id: valueColumn
                        title: "String"
                        role: "value"
                        horizontalAlignment: Text.AlignHCenter
                        width: (stringsTable.width - checkboxsColumn.width - buttonsColumn.width) * 0.55

                        delegate: Item {
                            anchors.fill: parent

                            TextField {
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                text: "\"" + styleData.value + "\""

                                style: TextFieldStyle
                                {
                                    padding.bottom: -1
                                    background: Rectangle {
                                        opacity: 0
                                    }


                                }
                            }
                        }
                    }

                    TableViewColumn {
                        id: buttonsColumn
                        role: "id"
                        objectName: "buttonsColumn"
                        width: 25
                        resizable: false

                        delegate: Button {
                            enabled: true

                            Image {
                                objectName: "captureImage"
                                anchors.fill: parent
                                source: "qrc:/images/trash.png"
                                fillMode: Image.PreserveAspectFit
                            }

                            onClicked: stringRemoved(styleData.value)
                        }
                    }
                }

                RowLayout {
                    id: buttonContainer
                    Layout.fillWidth: true

                    Button {
                        id: detectStringsButton
                        objectName: "detectStringsButton"
                        text: "DETECT STRINGS ON IMAGE"
                        Layout.alignment: Qt.AlignRight
                        style: generalButtonStyle

                        onClicked: detectStringsRequested()
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    Button {
                        id: clearButton
                        objectName: "clearButton"
                        text: "CLEAR"
                        Layout.preferredWidth: 85
                        Layout.alignment: Qt.AlignLeft
                        style: generalButtonStyle

                        onClicked: clearRequested()
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
                        Layout.preferredWidth: 85
                        Layout.preferredHeight: 45
                        text: "CANCEL"
                        style: redButtonStyle

                        onClicked: cancelRequested()
                    }

                    Button {
                        id: sendButton
                        objectName: "sendButton"
                        Layout.preferredWidth: 85
                        Layout.preferredHeight: 45
                        text: "SEND"
                        style: greenButtonStyle

                        onClicked: sendRequested()
                    }
                }
            }
        }
    }

    Component {
        id: generalButtonStyle

        ButtonStyle {
            background: Rectangle {
                //implicitWidth: 100
                border.width: control.activeFocus ? 2 : 1
                border.color: "#888"
                radius: 4
                gradient: Gradient {
                    GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#fff" }
                    GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ddd" }
                }
            }
        }
    }

    Component {
        id: redButtonStyle

        ButtonStyle {
            background: Rectangle {
                border.width: control.activeFocus ? 2 : 1
                border.color: "#888"
                radius: 4
                gradient: Gradient {
                    GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#A31C1C" }
                    GradientStop { position: 2 ; color: control.pressed ? "#aaa" : "#eee" }
                }
            }
        }
    }

    Component {
        id: greenButtonStyle

        ButtonStyle {
            background: Rectangle {
                border.width: control.activeFocus ? 2 : 1
                border.color: "#888"
                radius: 4
                gradient: Gradient {
                    GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#287100" }
                    GradientStop { position: 2 ; color: control.pressed ? "#aaa" : "#eee" }
                }
            }
        }
    }
}
