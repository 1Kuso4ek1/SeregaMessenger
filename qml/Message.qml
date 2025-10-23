import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import SeregaApp

Item {
    id: root

    required property var item

    required property bool self
    required property int index
    property bool last: index === messagesList.count - 1

    height: message.height

    RowLayout {
        anchors.fill: parent

        Item {
            Layout.preferredWidth: {
                const dif = messagesList.width - message.width
                return self ? dif < 600 ? dif : 0 : 0
            }
        }

        Rectangle {
            id: message

            Layout.preferredWidth: Math.max(content.implicitWidth, messageInfo.implicitWidth) + 20
            Layout.preferredHeight: layout.implicitHeight
            Layout.maximumWidth: messagesList.width * 0.8

            Behavior on opacity { NumberAnimation { duration: 300; easing.type: Easing.OutCubic } }

            opacity: 0.0
            Component.onCompleted: opacity = 1.0

            ColumnLayout {
                id: layout

                // Make this a TextEdit to make text selection available
                Label {
                    id: content

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredWidth: message.width - 20
                    Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                    Layout.margins: 10
                    Layout.bottomMargin: 0

                    // Cause the default color for 'TextEdit' is black
                    // color: "white"

                    // readOnly: true
                    // selectByMouse: true

                    text: item.content
                    textFormat: Text.MarkdownText
                    wrapMode: implicitWidth > message.Layout.maximumWidth ? Text.WrapAtWordBoundaryOrAnywhere : Text.NoWrap

                    font.pixelSize: 16

                    onLinkActivated: (link) => Qt.openUrlExternally(link)
                }

                RowLayout {
                    id: messageInfo
                    spacing: 10

                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignRight
                    Layout.margins: 10
                    Layout.topMargin: 0
                    Layout.leftMargin: 0

                    Label {
                        id: messageStatus

                        Layout.alignment: Qt.AlignRight

                        text: item.seen ? "seen" : "sent"
                        visible: last && self

                        font.pixelSize: 12
                        color: "#d0d0d0"
                    }

                    Label {
                        id: time

                        Layout.alignment: Qt.AlignBottom | Qt.AlignRight

                        text: item.created_at.toLocaleString(Qt.locale(), "hh:mm");

                        font.pixelSize: 12
                        color: "#d0d0d0"
                    }
                }
            }

            color: self ? Material.color(Material.Blue) : Material.color(Material.DeepPurple, Material.Shade600)

            radius: 20
        }
    }
}
