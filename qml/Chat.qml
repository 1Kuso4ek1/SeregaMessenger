import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import SeregaApp

Item {
    required property int userId
    required property int chatId
    required property string chatName

    Connections {
        target: InputMethod

        function onKeyboardRectangleChanged() {
            if(Qt.platform.os !== "android") // For android we use android:windowSoftInputMode="adjustResize"
                root.anchors.topMargin = InputMethod.keyboardRectangle.height
            messagesList.positionViewAtEnd()
        }
    }

    Flickable {
        anchors.fill: parent

        contentWidth: parent.width + 100
        contentX: 100
        width: contentWidth * 1.5
        boundsBehavior: Flickable.StopAtBounds

        Behavior on contentX { NumberAnimation { duration: 200 } }

        onMovementEnded: {
            if(atXBeginning)
                Navigation.pop()
            else
                contentX = 100
        }

        ColumnLayout {
            id: root
            spacing: 2

            anchors.fill: parent
            anchors.leftMargin: 100

            ToolBar {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop
                Layout.preferredHeight: 54 + parent.SafeArea.margins.top

                Material.primary: Material.color(Material.Grey, Material.Shade900)
                Material.elevation: 6

                z: 1

                RoundButton {
                    id: backButton
                    anchors.top: parent.top
                    anchors.topMargin: parent.SafeArea.margins.top

                    text: "←"

                    font.pixelSize: 20
                    font.bold: true

                    onClicked: Navigation.pop()
                }

                RowLayout {
                    anchors.centerIn: parent
                    anchors.verticalCenterOffset: parent.SafeArea.margins.top

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

                Component.onCompleted: {
                    messagesModel.append({
                        message: {
                            id: 1,
                            created_at: new Date(),
                            content: "sup, how ya doin?",
                            sender_id: 2,
                            seen: true
                        }
                    })
                    messagesModel.append({
                        message: {
                            id: 2,
                            created_at: new Date(),
                            content: "im good cuz, wbu?",
                            sender_id: 1,
                            seen: true
                        }
                    })
                    messagesModel.append({
                        message: {
                            id: 3,
                            created_at: new Date(),
                            content: "same my man",
                            sender_id: 2,
                            seen: true
                        }
                    })
                }
            }

            ListView {
                id: messagesList
                spacing: 6
                cacheBuffer: 1000

                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 10
                Layout.bottomMargin: 0

                Component.onCompleted: positionViewAtEnd()

                model: messagesModel
                delegate: Message {
                    required property var message

                    item: message
                    self: userId === message.sender_id
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 70 + parent.SafeArea.margins.bottom
                Layout.alignment: Qt.AlignBottom

                color: Material.background

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 12 + parent.SafeArea.margins.left
                    anchors.bottomMargin: 10 + parent.SafeArea.margins.bottom
                    spacing: 5

                    ScrollView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        ScrollBar.vertical.policy: ScrollBar.AlwaysOff
                        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

                        TextArea {
                            id: messageField

                            placeholderText: "Message..."
                            focusPolicy: Qt.StrongFocus
                            verticalAlignment: Qt.AlignVCenter
                            topPadding: 14
                            bottomPadding: 10.0

                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                            function sendMessage() {
                                InputMethod.commit()
                                const res = messageField.text.trim()
                                if(res.length === 0)
                                    return
                                messageField.clear()
                                messagesList.model.append({
                                    message: {
                                        messageId: messagesList.count,
                                        created_at: new Date(),
                                        content: res,
                                        sender_id: userId,
                                        seen: true
                                    }
                                })
                                messagesList.positionViewAtEnd()
                            }

                            // For PCs
                            Keys.onPressed: (event) => {
                                if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                                    if (event.modifiers & Qt.ShiftModifier)
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
        }
    }
}
