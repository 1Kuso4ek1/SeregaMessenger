import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import SeregaApp

Item {
    id: root

    required property int userId

    ColumnLayout {
        spacing: 0

        anchors.fill: parent

        ToolBar {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            Layout.preferredHeight: 54 + parent.SafeArea.margins.top // SafeArea is empty at this point. Do smth with it

            Material.primary: Material.color(Material.Grey, Material.Shade900)
            Material.elevation: 6

            z: 1

            Label {
                id: header

                text: "Serega"

                font.pixelSize: 18
                font.bold: true

                anchors.fill: parent
                anchors.topMargin: parent.SafeArea.margins.top

                opacity: searchContainer.searchMode ? 0.0 : 1.0

                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignHCenter

                Behavior on opacity { NumberAnimation { duration: 300; easing.type: Easing.InOutQuad } }
            }

            Item {
                id: searchContainer

                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.topMargin: parent.SafeArea.margins.top
                anchors.margins: 20

                width: Math.max(newChatButton.implicitWidth, searchField.implicitWidth)
                height: parent.height - (searchMode ? 15 : 10)

                property bool searchMode: false

                Button {
                    id: newChatButton
                    anchors.fill: parent

                    visible: !parent.searchMode
                    opacity: parent.searchMode ? 0.0 : 1.0

                    text: "Search"

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

                    onActiveFocusChanged: parent.searchMode = activeFocus
                    Keys.onEscapePressed: parent.searchMode = false

                    onAccepted: {
                        parent.searchMode = false
                        chatsModel.append({ chat: { chatId: chatsModel.count, name: displayText, lastMessage: "...", lastMessageDate: new Date() } })
                        Navigation.push("Chat.qml", { userId: root.userId, chatId: 8, chatName: displayText })
                        clear()
                    }

                    Behavior on opacity { NumberAnimation { duration: 200 } }
                }

                Behavior on width { NumberAnimation { duration: 250; easing.type: Easing.InOutQuad } }
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

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            currentIndex: chatsModel.count !== 0 ? 0 : 1

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

            Label {
                Layout.fillWidth: true
                Layout.fillHeight: true

                text: "ain't no heat on these lines cuz, keep it low."
                wrapMode: Text.WordWrap

                color: Material.color(Material.Grey, Material.Shade700)

                font.pixelSize: 30

                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
            }
        }
    }

    Connections {
        target: InputMethod

        function onVisibleChanged() {
            if(searchContainer.searchMode) // So the searchMode won't become true when you open the keyboard in the chat
                searchContainer.searchMode = InputMethod.visible
        }
    }
}
