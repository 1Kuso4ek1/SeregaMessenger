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
            Layout.preferredHeight: 80
            Layout.alignment: Qt.AlignBottom

            color: Material.background

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 5
                anchors.bottomMargin: 20
                spacing: 5

                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    ScrollBar.vertical.policy: ScrollBar.AlwaysOff

                    TextArea {
                        id: messageField

                        placeholderText: "Message..."
                        focusPolicy: Qt.StrongFocus
                        verticalAlignment: Qt.AlignVCenter
                        topPadding: 13.5

                        function sendMessage() {
                            InputMethod.commit()
                            const res = messageField.text.trim()
                            if(res.length === 0)
                                return
                            messageField.clear()
                            messagesList.model.append({ a: 42 })
                            messagesList.appended = true
                        }

                        // For PCs
                        Keys.onPressed: (event) => {
                            if(event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                                if(event.modifiers & Qt.ShiftModifier)
                                    event.accepted = false
                                else {
                                    sendMessage()
                                    event.accepted = true
                                }
                            }
                        }
                    }
                }

                RoundButton {
                    id: sendButton

                    Layout.preferredWidth: 60
                    Layout.preferredHeight: 60
                    Layout.topMargin: 3
                    Layout.alignment: Qt.AlignVCenter

                    text: ">"
                    focusPolicy: Qt.NoFocus

                    font.pixelSize: 20
                    font.bold: true

                    onClicked: messageField.sendMessage()
                }
            }
        }

        Connections {
            target: InputMethod

            function onVisibleChanged() {
                messagesList.contentHeightChanged()
            }
        }
    }
}
