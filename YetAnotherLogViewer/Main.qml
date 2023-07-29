import QtQml
import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls


Window {
    visible: true
    width: 720
    height: 480
    title: qsTr("Hello World")

    Rectangle {
        id: topPanel
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 40

        Row {
            spacing: 5
            TextField {
                id: regex
                height: regBtn.height
                font.pointSize: 17
            }

            Button {
                id: regBtn
                text: "Change regex"
                onClicked: {
                    logViewer.model.setRegex(regex.text)
                }
            }

            Button {
                id: connPopupBtn
                text: "connect"
                onClicked: connConfPopup.open()
            }
            
            Button {
                id: fetchBtn
                text: "fetch"
                onClicked: logViewer.model.fetchLogs();
            }
            
            Button {
                id: saveBtn
                text: "save"
                onClicked: logViewer.model.saveLogsToFile();
            }
            
            Button {
                id: cleanBtn
                text: "clean"
                onClicked: logViewer.model.cleanLogs();
            }
        }
    }
    Popup {
        id: connConfPopup
        x: 100
        y: 100
        width: 200
        height: 160
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
        Column {
            anchors.fill: parent
            spacing: 5
            TextField {
                    id: ipField
                    placeholderText: qsTr("IP")
                    height: (connConfPopup.height - 80)/2
                    width: (connConfPopup.width - 20) 
                    y: 10
                    x: 5

                    /*
                    anchors.top: parent.top
                    anchors.bottom: portField.top
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.topMargin: 10
                    anchors.bottomMargin: 10
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    */
                    font.pointSize: 15
            }
        
            TextField {
                    id: portField
                    placeholderText: qsTr("PORT")
                    height: (connConfPopup.height - 80)/2
                    width: (connConfPopup.width - 20)
                    y: 50
                    x: 5
                    /*
                    anchors.top: ipField.bottom
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.topMargin: 10
                    anchors.bottomMargin: 10
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    */
                    font.pointSize: 15
            }

            Row {
                spacing: 20
                RadioButton {
                    id: udpBtn
                    text: "UDP"
                    checked: true
                }

                RadioButton {
                    id: tcpBtn
                    text: "TCP"
                }

                RadioButton {
                    id: sslBtn
                    text: "SSL"
                }
            }

            Button {
                id: saveConfBtn
                text: "Save"
                x: 5
                height: 30

                onClicked: {
                    logViewer.model.setEndpoint(ipField.text, portField.text)
                    console.log("setting end point: ", ipField.text, ":", portField.text)
                    connConfPopup.close()
                    if (udpBtn.checked)
                        logViewer.model.setProto("2")
                    else if (tcpBtn.checked) 
                        logViewer.model.setProto("1")
                    else if (sslBtn.checked)
                        logViewer.model.setProto("3")
                }
            }
        }
    }


    Item {
        id: downloadFileDropArea
        anchors.fill: parent

        Rectangle {
            id: dropAreaBg
            anchors.fill: parent
            color: "grey"
            opacity: 0.5
            visible: false
        }

        Item {
            anchors.fill: parent
            DropArea {
                anchors.fill: parent

                onEntered: {
                    console.log("Entered");
                    dropAreaBg.visible = true;
                }

                onExited: {
                    console.log("Exited")
                    dropAreaBg.visible = false;
                }

                onDropped: function(drop) {
                    console.log("Dropped ", drop.text)
                    logViewer.model.openFile(drop.text)
                    dropAreaBg.visible = false;
                }
            }
        }
    }

    Row {
        id: header
        width: logViewer.contentWidth
        height: 40
        anchors.top: topPanel.bottom
        anchors.topMargin: 4
        x: -logViewer.contentX
        z: 1
        spacing: 4
        Repeater {
            id: rep
            model: logViewer.model.columnCount()
            Rectangle {
                id: rect
                width: Math.min(600, logViewer.model.columnWidth(column))
                height: parent.height
                color: "orange"

                TextField {
                    //anchors.fill: parent
                    anchors.verticalCenter: parent.verticalCenter
                    x: 4
                    width: parent.width - 4
                    text: logViewer.model.headerData(index, Qt.Horizontal)
                    horizontalAlignment: TextInput.AlignHCenter
                    verticalAlignment: TextInput.AlignVCenter
                    onEditingFinished: logViewer.model.setHeaderData(index, Qt.Horizontal, text)
                }

            }

        }

        Connections {
            target: logViewer.model
            function onHeaderDataChanged() {
                updateColumns()
            }

            function onModelReloaded() {
                updateColumns()
            }

        }
    }


    function updateColumns() {
        //console.log("qml: columnWidth " + logViewer.model.columnWidth(0))
        //console.log("qml: columnCount " + logViewer.model.columnCount())
        rep.model = logViewer.model.columnCount()
        for (var i = 0; i < rep.count; i++) {
            rep.itemAt(i).width = logViewer.model.columnWidth(i)
            logViewer.forceLayout()
        }

    }


    TableView {
        id: logViewer
        columnSpacing: 4
        rowSpacing: 4
        anchors.fill: parent
        //anchors.top: header.bottom
        anchors.topMargin: header.height + header.y
        model: loggerModel //logViewer.model {}
        columnWidthProvider: function(column) { return Math.min(logViewer.model.columnWidth(column)) }

        delegate: Rectangle {
            implicitHeight: text.implicitHeight
            color: column === logViewer.activeColumn
                   ? "brown"
                   : "grey"
            //color: "#EEE"
            Text {
                id: text
                text: display
                color: "#CFF"
                width: parent.width
                //height: parent.height
                //font.pointSize: 10
                //minimumPointSize: 3
                //fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: column == 49 ? Text.ElideLeft : Text.ElideRight
                font.preferShaping: false
            }
        }

        ScrollBar.horizontal: ScrollBar { }
        ScrollBar.vertical: ScrollBar { }
    }
}
