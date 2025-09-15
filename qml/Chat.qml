import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import SeregaApp

Item {
    required property string chatName

    ColumnLayout {
        id: root
        spacing: 2

        anchors.fill: parent

        ToolBar {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            Layout.preferredHeight: 54

            Material.primary: Material.color(Material.Grey, Material.Shade900)
            Material.elevation: 6

            z: 1

            RoundButton {
                id: backButton

                text: "←"

                font.pixelSize: 20
                font.bold: true

                onClicked: Navigation.pop()
            }

            RowLayout {
                anchors.centerIn: parent

                Avatar {}

                Label {
                    text: chatName

                    font.pixelSize: 18
                    font.bold: false

                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                }
            }
        }

        ListModel {
            id: messagesModel

            ListElement { a: 1 }
            ListElement { a: 2 }
            ListElement { a: 3 }
        }

        ListView {
            id: messagesList
            spacing: 6

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 10

            property var prev: 0
            property var appended: false

            Component.onCompleted: positionViewAtEnd()
            // A very hacky way to scroll down the list independently of the delegates' height
            onContentHeightChanged: {
                if(!appended)
                    return
                if(contentHeight - prev > 0)
                    positionViewAtEnd()
                else
                    appended = false
                prev = contentHeight
            }

            model: messagesModel
            delegate: Message{}
            cacheBuffer: 40
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 65
            Layout.alignment: Qt.AlignBottom

            color: Material.background

            RowLayout {
                anchors.fill: parent
                anchors.margins: 5
                spacing: 5

                TextField {
                    id: messageField

                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    placeholderText: "Message..."

                    onAccepted: {
                        const res = text.trim()
                        if(res.length === 0)
                            return;
                        clear()
                        messagesList.model.append({ a: 42 })
                        messagesList.appended = true
                    }
                }

                RoundButton {
                    id: sendButton

                    text: "➤"

                    font.pixelSize: 20
                    font.bold: true

                    onClicked: messageField.accepted()
                }
            }
        }
    }
}
