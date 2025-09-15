import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import SeregaApp

Item {
    property bool self: index % 2 === 0

    height: message.height

    RowLayout {
        anchors.fill: parent

        Item {
            Layout.preferredWidth: self ? 0 : messagesList.width - message.width;
        }

        Rectangle {
            id: message
            Layout.preferredWidth: content.implicitWidth + 20
            Layout.preferredHeight: content.implicitHeight + 20
            Layout.maximumWidth: messagesList.width * 0.8

            Label {
                id: content
                anchors.centerIn: parent

                width: parent.width - 20

                text: "Lorem ipsum dolor sit amet, occaecat nostrud nostrud aliqua dolor aliquip"
                wrapMode: Text.WordWrap

                font.pixelSize: 16
            }

            color: self ? Material.color(Material.Blue) : Material.color(Material.DeepPurple, Material.Shade600)

            radius: 20
        }
    }
}
