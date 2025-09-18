import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import SeregaApp

Item {
    id: root

    required property int userId

    Item {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 20

        width: Math.max(newChatButton.implicitWidth, searchField.implicitWidth)
        height: newChatButton.implicitHeight

        property bool searchMode: false

        z: 1

        Button {
            id: newChatButton
            anchors.fill: parent

            visible: !parent.searchMode
            opacity: parent.searchMode ? 0.0 : 1.0

            text: "New chat"

            onClicked: searchField.forceActiveFocus()

            Behavior on opacity { NumberAnimation { duration: 200 } }
        }

        TextField {
            id: searchField
            anchors.fill: parent

            visible: parent.searchMode
            opacity: parent.searchMode ? 1.0 : 0.0
            implicitWidth: parent.searchMode ? root.width - 40 : 0

            placeholderText: "Enter username..."

            onActiveFocusChanged: parent.searchMode = focus
            Keys.onEscapePressed: parent.searchMode = false

            onAccepted: {
                parent.searchMode = false
                Navigation.push("Chat.qml", { userId: root.userId, chatId: 8, chatName: displayText })
                clear()
            }

            Behavior on opacity { NumberAnimation { duration: 200 } }
        }

        Behavior on width { NumberAnimation { duration: 250; easing.type: Easing.InOutQuad } }
    }

    ColumnLayout {
        spacing: 0

        anchors.fill: parent

        ToolBar {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            Layout.preferredHeight: 54

            Material.primary: Material.color(Material.Grey, Material.Shade900)
            Material.elevation: 6

            z: 1

            Label {
                text: "Serega"

                font.pixelSize: 18
                font.bold: true

                anchors.fill: parent

                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignHCenter
            }
        }

        ListModel {
            id: chatsModel

            Component.onCompleted: {
                chatsModel.append({ chat: { chatId: 1, name: "Hello world", lastMessage: "Hiiiiii!!!! how are you???", lastMessageDate: new Date() } })
                chatsModel.append({ chat: { chatId: 2, name: "Meow meow meow", lastMessage: "I'm a cute little kitten :3", lastMessageDate: new Date() } })
                chatsModel.append({ chat: { chatId: 3, name: "Mwah UwU", lastMessage: "1982374098657348016512893", lastMessageDate: new Date() } })
            }
        }

        ListView {
            id: chatsList
            spacing: 2

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 10

            model: chatsModel
            delegate: ChatDelegate {
                required property var chat

                userId: root.userId
                item: chat
            }
        }
    }
}
