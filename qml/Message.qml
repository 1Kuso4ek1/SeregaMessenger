import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import SeregaApp

Item {
    property bool self: index % 3 === 0

    height: message.height

    RowLayout {
        anchors.fill: parent

        Item {
            Layout.preferredWidth: {
                const dif = messagesList.width - message.width
                return self ? 0 : dif < 600 ? dif : 0
            }
        }

        Rectangle {
            id: message

            Layout.preferredWidth: content.implicitWidth + 20
            Layout.preferredHeight: layout.implicitHeight
            Layout.maximumWidth: messagesList.width * 0.8

            ColumnLayout {
                id: layout

                Label {
                    id: content

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredWidth: message.width - 20
                    Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                    Layout.margins: 10
                    Layout.bottomMargin: 0

                    text: "Lorem ipsum dolor sit amet, occaecat nostrud nostrud aliqua dolor aliquip"
                    wrapMode: Text.WordWrap

                    font.pixelSize: 16
                }

                Label {
                    id: time

                    Layout.alignment: Qt.AlignBottom | Qt.AlignRight
                    Layout.margins: 10
                    Layout.topMargin: 0
                    Layout.leftMargin: 0

                    text: (new Date()).toLocaleString(Qt.locale(), "hh:mm");

                    font.pixelSize: 12
                    color: "#d0d0d0"
                }
            }

            color: self ? Material.color(Material.Blue) : Material.color(Material.DeepPurple, Material.Shade600)

            radius: 20
        }
    }
}
