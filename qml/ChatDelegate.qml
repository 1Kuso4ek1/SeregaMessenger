import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import SeregaApp

Pane {
    id: root

    required property int userId
    required property var item

    width: chatsList.width
    height: 100

    Material.background: Material.color(Material.Grey, Material.Shade900)

    RowLayout {
        anchors.fill: parent

        spacing: 10

        Avatar {
            size: 50
        }

        ColumnLayout {
            spacing: 20

            RowLayout {
                Label {
                    Layout.fillWidth: true

                    text: item.name
                    horizontalAlignment: Qt.AlignLeft

                    elide: Text.ElideRight

                    font.bold: true
                }

                Label {
                    Layout.fillWidth: true

                    text: item.lastMessageDate.toLocaleString(Qt.locale(), "dd.MM.yyyy hh:mm");
                    horizontalAlignment: Qt.AlignRight

                    font.pixelSize: 12
                    color: "#606060"
                }
            }

            Label {
                Layout.fillWidth: true

                text: item.lastMessage

                elide: Text.ElideRight
            }
        }
    }

    MouseArea {
        anchors.fill: parent

        onClicked: { Navigation.push("Chat.qml", { userId: root.userId, chatId: item.chatId, chatName: item.name }) }
        propagateComposedEvents: true
    }
}
